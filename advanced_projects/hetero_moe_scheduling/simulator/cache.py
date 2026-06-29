class GPUExpertCache:
    def __init__(self, config):
        self.config = config
        self.capacity = config.gpu_cache_capacity
        self.cache = set()           # 當前在快取中的 expert_id
        self.in_flight = set()       # 正在 PCIe 傳輸中的 expert_id
        
        # 替換演算法相關資料結構
        self.lru_list = []           # 越後面越新 (LRU)
        self.frequency = {}          # LFU 的頻次計數器

    def is_hit(self, expert_id):
        return expert_id in self.cache

    def is_in_flight(self, expert_id):
        return expert_id in self.in_flight

    def touch(self, expert_id):
        """當命中或訪問時，更新替換策略狀態"""
        if expert_id in self.cache:
            # 更新 LRU
            if expert_id in self.lru_list:
                self.lru_list.remove(expert_id)
            self.lru_list.append(expert_id)
            
            # 更新 LFU
            self.frequency[expert_id] = self.frequency.get(expert_id, 0) + 1

    def start_transfer(self, expert_id):
        self.in_flight.add(expert_id)

    def insert(self, expert_id, policy="lru"):
        """傳輸完成後，將 expert 插入 cache，必要時進行淘汰"""
        if expert_id in self.in_flight:
            self.in_flight.remove(expert_id)

        if expert_id in self.cache:
            self.touch(expert_id)
            return None # 不需要淘汰

        evicted = None
        if len(self.cache) >= self.capacity:
            evicted = self._evict(policy)
            if evicted is not None:
                self.cache.remove(evicted)
                if evicted in self.lru_list:
                    self.lru_list.remove(evicted)
                if evicted in self.frequency:
                    del self.frequency[evicted]

        self.cache.add(expert_id)
        self.lru_list.append(expert_id)
        self.frequency[expert_id] = 1
        return evicted

    def _evict(self, policy):
        """根據策略淘汰一個 expert"""
        if not self.cache:
            return None

        if policy == "lru":
            # 淘汰 lru_list 中最前面且在 cache 中的 expert
            for exp in self.lru_list:
                if exp in self.cache:
                    return exp
            return list(self.cache)[0]

        elif policy == "lfu":
            # 淘汰使用頻率最低的 expert
            min_freq = float('inf')
            evict_candidate = None
            for exp in self.cache:
                freq = self.frequency.get(exp, 0)
                if freq < min_freq:
                    min_freq = freq
                    evict_candidate = exp
                elif freq == min_freq:
                    # 若頻次相同，則以 LRU 順序做 tie-breaker
                    if evict_candidate in self.lru_list and exp in self.lru_list:
                        if self.lru_list.index(exp) < self.lru_list.index(evict_candidate):
                            evict_candidate = exp
            return evict_candidate

        else:
            # 預設隨機或 FIFO 淘汰第一個
            return list(self.cache)[0]
