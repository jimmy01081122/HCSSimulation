class MoEScheduler:
    def __init__(self, config):
        self.config = config

    def schedule(self, active_streams, cache, policy="elas"):
        """
        active_streams: List of Request 物件，這些 Request 當前正處於 "READY_FOR_MOE" 狀態。
        回傳值: 調度執行的順序 (List of Request) 以及為它們分配的執行模式與 expert。
        """
        if not active_streams:
            return []

        if policy == "fcfs":
            # FCFS: 簡單地按照 stream_id 或抵達時間排序，不做任何 locality 優化
            return sorted(active_streams, key=lambda s: (s.arrival_time, s.request_id))

        elif policy in ("elas", "f-elas"):
            # 1. 處理 aging 機制 (僅適用於 f-elas)
            starving_streams = []
            normal_streams = []
            
            for stream in active_streams:
                if not hasattr(stream, "waiting_ticks"):
                    stream.waiting_ticks = 0
                
                if policy == "f-elas":
                    stream.waiting_ticks += 1
                    if stream.waiting_ticks >= 5: # 飢餓門檻 (ms 或週期計數)
                        starving_streams.append(stream)
                        continue
                normal_streams.append(stream)

            # 2. 對於 normal streams 進行常規的 Locality-aware 分組
            hit_streams = []
            miss_streams = {} # expert_id -> list of streams
            
            for stream in normal_streams:
                expert_id = stream.get_current_expert_demand()
                if expert_id is None:
                    hit_streams.append(stream)
                elif cache.is_hit(expert_id):
                    hit_streams.append(stream)
                else:
                    if expert_id not in miss_streams:
                        miss_streams[expert_id] = []
                    miss_streams[expert_id].append(stream)
            
            # 排序 miss 佇列
            sorted_miss_experts = sorted(
                miss_streams.keys(),
                key=lambda exp: (
                    not cache.is_in_flight(exp), # 已經在 in-flight 的優先
                    -len(miss_streams[exp])       # 等待 token 數多的優先 (Batch 效應大)
                )
            )
            
            # 3. 組合調度順序：飢餓 streams -> 快取命中 normal -> 快取未命中 normal
            scheduled_order = []
            # 飢餓優先級最高，避免 starvation
            if starving_streams:
                # 飢餓時間越長的越優先
                starving_streams = sorted(starving_streams, key=lambda s: -s.waiting_ticks)
                scheduled_order.extend(starving_streams)
                
            scheduled_order.extend(hit_streams)
            for exp in sorted_miss_experts:
                scheduled_order.extend(miss_streams[exp])
                
            return scheduled_order

        else:
            return active_streams
