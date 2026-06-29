# workload_gen.py
# Workload generator for MoE inference simulation.
# Generates Poisson request arrivals with bounded Zipf-distributed expert demands or loads from CSV traces.

import random
import os
import csv

class Request:
    def __init__(self, request_id, arrival_time, total_tokens, total_layers, num_experts, zipf_alpha=1.2):
        self.request_id = request_id
        self.arrival_time = arrival_time
        self.total_tokens = total_tokens
        self.total_layers = total_layers
        
        self.current_token_idx = 0
        self.current_layer = 0
        
        # State: "QUEUED", "ORCHESTRATING", "GPU_DENSE", "MOE_PENDING", 
        # "MOE_TRANSFERRING", "CPU_MOE_EXECUTING", "GPU_MOE_EXECUTING", "FINISHED"
        self.status = "QUEUED"
        
        self.remaining_step_time = 0.0
        self.current_expert_needed = None
        self.waiting_ticks = 0  # Starvation count

        # Generate Zipf-distributed expert demands for all tokens and layers using bounded Zipf
        self.expert_demands = []
        for _ in range(total_tokens):
            token_demands = []
            for _ in range(total_layers):
                expert_id = self._sample_bounded_zipf(zipf_alpha, num_experts)
                token_demands.append(int(expert_id))
            self.expert_demands.append(token_demands)

        # Performance statistics
        self.start_processing_time = None
        self.finish_time = None
        self.total_gpu_time = 0.0
        self.total_cpu_time = 0.0
        self.total_pcie_wait_time = 0.0
        
        self.cpu_offload_count = 0
        self.gpu_hit_count = 0
        self.gpu_miss_count = 0

    def _sample_bounded_zipf(self, alpha, num_elements):
        probs = [1.0 / (i ** alpha) for i in range(1, num_elements + 1)]
        sum_probs = sum(probs)
        probs = [p / sum_probs for p in probs]
        
        cdf = []
        current_sum = 0.0
        for p in probs:
            current_sum += p
            cdf.append(current_sum)
            
        u = random.random()
        for idx, val in enumerate(cdf):
            if u < val:
                return idx
        return num_elements - 1

    def get_current_expert_demand(self):
        if self.current_token_idx < self.total_tokens and self.current_layer < self.total_layers:
            return self.expert_demands[self.current_token_idx][self.current_layer]
        return None

    def advance_pipeline(self):
        """Advances the token/layer pipeline. Returns True if advanced, False if request is finished."""
        if self.current_layer < self.total_layers - 1:
            self.current_layer += 1
            self.status = "ORCHESTRATING"
            self.remaining_step_time = 0.0
            return True
        else:
            if self.current_token_idx < self.total_tokens - 1:
                self.current_token_idx += 1
                self.current_layer = 0
                self.status = "ORCHESTRATING"
                self.remaining_step_time = 0.0
                return True
            else:
                self.status = "FINISHED"
                return False


class WorkloadGenerator:
    def __init__(self, num_requests=15, num_experts=16, tokens_range=(16, 48), layers=6, zipf_alpha=1.2, arrival_rate=0.15, random_seed=42):
        self.num_requests = num_requests
        self.num_experts = num_experts
        self.tokens_range = tokens_range
        self.layers = layers
        self.zipf_alpha = zipf_alpha
        self.arrival_rate = arrival_rate
        self.random_seed = random_seed

    def generate(self):
        """Generates a list of requests based on config."""
        random.seed(self.random_seed)

        requests = []
        current_arrival_time = 0.0
        
        for i in range(self.num_requests):
            interval = random.expovariate(self.arrival_rate)
            current_arrival_time += interval
            
            total_tokens = random.randint(self.tokens_range[0], self.tokens_range[1])
            req = Request(
                request_id=i,
                arrival_time=current_arrival_time,
                total_tokens=total_tokens,
                total_layers=self.layers,
                num_experts=self.num_experts,
                zipf_alpha=self.zipf_alpha
            )
            requests.append(req)
            
        return requests

    @staticmethod
    def load_trace_file(file_path):
        """Loads workload requests from a pre-generated CSV trace file."""
        if not os.path.exists(file_path):
            raise FileNotFoundError(f"Trace file {file_path} not found.")
            
        requests_dict = {}
        with open(file_path, "r") as f:
            reader = csv.DictReader(f)
            for row in reader:
                req_id = int(row["request_id"])
                token_id = int(row["token_id"])
                layer = int(row["layer"])
                expert_id = int(row["expert_id"])
                arrival_time = float(row["arrival_time_ms"])
                
                if req_id not in requests_dict:
                    requests_dict[req_id] = {
                        "arrival_time": arrival_time,
                        "tokens_data": {}
                    }
                
                tokens_data = requests_dict[req_id]["tokens_data"]
                if token_id not in tokens_data:
                    tokens_data[token_id] = {}
                tokens_data[token_id][layer] = expert_id
                
        requests = []
        for req_id in sorted(requests_dict.keys()):
            req_info = requests_dict[req_id]
            arrival_time = req_info["arrival_time"]
            tokens_data = req_info["tokens_data"]
            
            total_tokens = len(tokens_data)
            total_layers = len(tokens_data[0]) if total_tokens > 0 else 0
            
            # Create Request object
            req = Request(
                request_id=req_id,
                arrival_time=arrival_time,
                total_tokens=total_tokens,
                total_layers=total_layers,
                num_experts=1,
                zipf_alpha=1.0
            )
            
            # Reconstruct expert_demands array
            expert_demands = []
            for t_idx in sorted(tokens_data.keys()):
                layer_demands = []
                for l_idx in sorted(tokens_data[t_idx].keys()):
                    layer_demands.append(tokens_data[t_idx][l_idx])
                expert_demands.append(layer_demands)
                
            req.expert_demands = expert_demands
            requests.append(req)
            
        return requests
