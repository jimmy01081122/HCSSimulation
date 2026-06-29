# scheduler.py
# MoE inference request schedulers (FCFS, ELAS, F-ELAS).
# Implements locality-aware ordering and aging-based starvation prevention.

class MoEScheduler:
    def __init__(self, config, starvation_threshold=5):
        self.config = config
        self.starvation_threshold = starvation_threshold

    def schedule(self, active_streams, cache, policy="elas"):
        """
        Schedules active streams in MOE_PENDING state.
        Returns the ordered list of Request objects to execute.
        """
        if not active_streams:
            return []

        if policy == "fcfs":
            # First-Come-First-Served based on arrival time and request ID
            return sorted(active_streams, key=lambda s: (s.arrival_time, s.request_id))

        elif policy in ("elas", "f-elas"):
            # 1. Starvation prevention checking (only for f-elas)
            starving_streams = []
            normal_streams = []
            
            for stream in active_streams:
                if not hasattr(stream, "waiting_ticks"):
                    stream.waiting_ticks = 0
                
                if policy == "f-elas" and stream.waiting_ticks >= self.starvation_threshold:
                    starving_streams.append(stream)
                else:
                    normal_streams.append(stream)

            # 2. Group normal streams by cache locality
            hit_streams = []
            miss_streams = {}  # expert_id -> list of streams
            
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

            # Sort miss streams:
            # - In-flight transfers first (overlap transfer and execution)
            # - Larger batch size next (maximize resource utilization)
            # - Tie-breaker: expert_id
            sorted_miss_experts = sorted(
                miss_streams.keys(),
                key=lambda exp: (
                    not cache.is_in_flight(exp),
                    -len(miss_streams[exp]),
                    exp
                )
            )

            # 3. Assemble scheduled order:
            # Starving streams (highest priority) -> Cache hits -> Cache misses
            scheduled_order = []
            if starving_streams:
                # Longest waiting first
                starving_streams = sorted(starving_streams, key=lambda s: -s.waiting_ticks)
                scheduled_order.extend(starving_streams)

            scheduled_order.extend(hit_streams)
            for exp in sorted_miss_experts:
                scheduled_order.extend(miss_streams[exp])

            return scheduled_order

        return active_streams
