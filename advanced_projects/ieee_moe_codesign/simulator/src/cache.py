# cache.py
# GPU Expert Cache modeling with LRU, LFU, and ARC eviction policies.
# Tracks cache hits, misses, evictions, and insertion events.

class GPUExpertCache:
    def __init__(self, config):
        self.config = config
        self.capacity = config.gpu_cache_capacity
        self.cache = set()           # Set of expert_ids currently in actual cache
        self.in_flight = set()       # Set of expert_ids currently in PCIe transfer
        
        # Policy-specific metadata
        # LRU
        self.lru_list = []           # MRU is at the end, LRU is at the beginning
        
        # LFU
        self.frequency = {}          # expert_id -> frequency count
        
        # ARC (Adaptive Replacement Cache)
        self.arc_p = 0.0             # Dynamic tuning parameter
        self.T1 = []                 # Recency cache (actual cache)
        self.B1 = []                 # Recency ghost cache (metadata only)
        self.T2 = []                 # Frequency cache (actual cache)
        self.B2 = []                 # Frequency ghost cache (metadata only)

        # Statistics
        self.stats_evictions = 0
        self.stats_insertions = 0
        self.event_log = []          # List of tuples (time, event_type, expert_id, evicted_id)

    def is_hit(self, expert_id):
        """Returns True if expert_id is currently in the GPU cache."""
        return expert_id in self.cache

    def is_in_flight(self, expert_id):
        """Returns True if expert_id is currently being transferred."""
        return expert_id in self.in_flight

    def touch(self, expert_id, current_time=0.0):
        """Updates cache metadata upon a hit."""
        if expert_id not in self.cache:
            return

        # 1. Update LRU
        if expert_id in self.lru_list:
            self.lru_list.remove(expert_id)
        self.lru_list.append(expert_id)

        # 2. Update LFU
        self.frequency[expert_id] = self.frequency.get(expert_id, 0) + 1

        # 3. Update ARC
        if expert_id in self.T1:
            self.T1.remove(expert_id)
            self.T2.append(expert_id)
        elif expert_id in self.T2:
            self.T2.remove(expert_id)
            self.T2.append(expert_id)

        self.event_log.append((current_time, "hit", expert_id, None))

    def start_transfer(self, expert_id):
        """Marks an expert_id as in-flight."""
        self.in_flight.add(expert_id)

    def insert(self, expert_id, policy="lru", current_time=0.0):
        """Inserts an expert into the cache upon transfer completion. Evicts if full. Returns evicted_id or None."""
        if expert_id in self.in_flight:
            self.in_flight.remove(expert_id)

        if expert_id in self.cache:
            self.touch(expert_id, current_time)
            return None

        evicted_id = None

        if policy == "arc":
            evicted_id = self._insert_arc(expert_id, current_time)
        else:
            # For LRU/LFU, check capacity and evict
            if len(self.cache) >= self.capacity:
                evicted_id = self._evict_lru_lfu(policy)
                if evicted_id is not None:
                    self.cache.remove(evicted_id)
                    if evicted_id in self.lru_list:
                        self.lru_list.remove(evicted_id)
                    if evicted_id in self.frequency:
                        del self.frequency[evicted_id]
                    self.stats_evictions += 1
                    self.event_log.append((current_time, "evict", evicted_id, None))

            # Insert new
            self.cache.add(expert_id)
            self.lru_list.append(expert_id)
            self.frequency[expert_id] = 1
            self.stats_insertions += 1
            self.event_log.append((current_time, "insert", expert_id, evicted_id))

        return evicted_id

    def _evict_lru_lfu(self, policy):
        """Finds the eviction candidate for LRU or LFU."""
        if not self.cache:
            return None

        if policy == "lru":
            # Evict first element in lru_list that is in cache
            for exp in self.lru_list:
                if exp in self.cache:
                    return exp
            return list(self.cache)[0]

        elif policy == "lfu":
            # Evict element in cache with minimum frequency
            min_freq = float('inf')
            evict_candidate = None
            for exp in self.cache:
                freq = self.frequency.get(exp, 0)
                if freq < min_freq:
                    min_freq = freq
                    evict_candidate = exp
                elif freq == min_freq:
                    # Tie-breaker: LRU order (index in lru_list)
                    if evict_candidate in self.lru_list and exp in self.lru_list:
                        if self.lru_list.index(exp) < self.lru_list.index(evict_candidate):
                            evict_candidate = exp
            return evict_candidate

        return list(self.cache)[0]

    def _replace_arc(self, expert_id, evicted_list):
        """Helper for ARC replace step."""
        if len(self.T1) > 0 and ((expert_id in self.B2 and len(self.T1) == int(self.arc_p)) or (len(self.T1) > int(self.arc_p))):
            y = self.T1.pop(0)  # LRU of T1
            self.B1.append(y)
            evicted_list.append(y)
        else:
            y = self.T2.pop(0)  # LRU of T2
            self.B2.append(y)
            evicted_list.append(y)

    def _insert_arc(self, expert_id, current_time):
        """ARC specific insertion logic."""
        evicted_from_cache = None
        evicted_list = []

        # Case 2: Hit in B1 (Ghost Recency)
        if expert_id in self.B1:
            sz_b1 = len(self.B1)
            sz_b2 = len(self.B2)
            delta = 1 if sz_b1 == 0 else max(1, sz_b2 / sz_b1)
            self.arc_p = min(self.arc_p + delta, self.capacity)
            self._replace_arc(expert_id, evicted_list)
            self.B1.remove(expert_id)
            self.T2.append(expert_id)

        # Case 3: Hit in B2 (Ghost Frequency)
        elif expert_id in self.B2:
            sz_b1 = len(self.B1)
            sz_b2 = len(self.B2)
            delta = 1 if sz_b2 == 0 else max(1, sz_b1 / sz_b2)
            self.arc_p = max(self.arc_p - delta, 0)
            self._replace_arc(expert_id, evicted_list)
            self.B2.remove(expert_id)
            self.T2.append(expert_id)

        # Case 4: Complete Miss
        else:
            # Case 4a: L1 size is capacity
            if len(self.T1) + len(self.B1) == self.capacity:
                if len(self.T1) < self.capacity:
                    if self.B1:
                        self.B1.pop(0)
                    self._replace_arc(expert_id, evicted_list)
                else:
                    y = self.T1.pop(0)
                    evicted_list.append(y)
            # Case 4b: L1 size < capacity and total size >= capacity
            elif len(self.T1) + len(self.B1) + len(self.T2) + len(self.B2) >= self.capacity:
                if len(self.T1) + len(self.B1) + len(self.T2) + len(self.B2) == 2 * self.capacity:
                    if self.B2:
                        self.B2.pop(0)
                self._replace_arc(expert_id, evicted_list)

            self.T1.append(expert_id)

        # Apply eviction list to actual cache set
        for y in evicted_list:
            if y in self.cache:
                self.cache.remove(y)
                evicted_from_cache = y
                self.stats_evictions += 1
                self.event_log.append((current_time, "evict", y, None))

        self.cache.add(expert_id)
        self.stats_insertions += 1
        self.event_log.append((current_time, "insert", expert_id, evicted_from_cache))

        return evicted_from_cache
