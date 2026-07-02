class ExpertCache:
    def __init__(self, cache_size, policy='lru', num_experts=8):
        self.cache_size = cache_size
        self.policy = policy.lower()
        self.num_experts = num_experts
        
        # Cache state: list of resident expert IDs
        self.cache = []
        
        # Access trackers
        self.lru_time = {}       # expert_id -> logical timestamp of last access
        self.lfu_freq = {}       # expert_id -> frequency of access
        self.time_counter = 0
        
        # History predictor state
        self.last_requested = None
        # transitions[prev_expert][next_expert] = count
        self.transitions = {i: {j: 0 for j in range(num_experts)} for i in range(num_experts)}
        
    def request(self, expert_id):
        self.time_counter += 1
        
        # Update frequency
        self.lfu_freq[expert_id] = self.lfu_freq.get(expert_id, 0) + 1
        
        # Record transition for history predictor
        if self.last_requested is not None:
            self.transitions[self.last_requested][expert_id] += 1
        self.last_requested = expert_id
        
        if self.cache_size == 0:
            return False, None, None, list(self.cache)
            
        hit = expert_id in self.cache
        evicted_id = None
        prefetched_id = None
        
        if hit:
            # Update LRU time
            self.lru_time[expert_id] = self.time_counter
        else:
            # Miss: need to bring it in
            if len(self.cache) >= self.cache_size:
                # Cache is full: evict
                evicted_id = self._select_eviction_target(exclude=[])
                if evicted_id is not None:
                    self.cache.remove(evicted_id)
            self.cache.append(expert_id)
            self.lru_time[expert_id] = self.time_counter
            
        # Prefetch logic (only for history predictor or if requested)
        if self.policy == 'history' and self.cache_size > 1:
            # Predict next expert based on current expert_id transition history
            next_expert_probs = self.transitions[expert_id]
            best_next = None
            max_count = 0
            for ext, count in next_expert_probs.items():
                if count > max_count:
                    max_count = count
                    best_next = ext
            
            # If we have a historical prediction and it's not in cache
            if best_next is not None and best_next not in self.cache:
                # We can prefetch it
                prefetched_id = best_next
                # Trigger eviction for prefetch if cache is full
                if len(self.cache) >= self.cache_size:
                    # Exclude the current expert we just loaded/accessed to avoid evicting it immediately
                    pevict_id = self._select_eviction_target(exclude=[expert_id])
                    if pevict_id is not None:
                        self.cache.remove(pevict_id)
                        # If we evicted something else, we might log it, but the primary eviction for the request is evicted_id.
                        # We can track prefetch evictions, but we will keep state updated.
                self.cache.append(prefetched_id)
                self.lru_time[prefetched_id] = self.time_counter
                
        return hit, evicted_id, prefetched_id, list(self.cache)

    def _select_eviction_target(self, exclude=None):
        if exclude is None:
            exclude = []
            
        candidates = [x for x in self.cache if x not in exclude]
        if not candidates:
            # If all are excluded, fall back to anything in cache
            candidates = list(self.cache)
            if not candidates:
                return None
                
        if self.policy == 'lru' or self.policy == 'history':
            # Evict least recently used among candidates
            return min(candidates, key=lambda x: self.lru_time.get(x, 0))
            
        elif self.policy == 'lfu':
            # Evict least frequently used among candidates
            # Tie break with LRU
            return min(candidates, key=lambda x: (self.lfu_freq.get(x, 0), self.lru_time.get(x, 0)))
            
        elif self.policy == 'none':
            # Dummy policy or simple FIFO
            return candidates[0]
            
        else:
            # Default to LRU
            return min(candidates, key=lambda x: self.lru_time.get(x, 0))
