import os
import time
import torch
from transformers import AutoModelForCausalLM, AutoConfig

def main():
    model_id = "microsoft/Phi-tiny-MoE-instruct"
    print("Loading model config and class...")
    config = AutoConfig.from_pretrained(model_id, trust_remote_code=True)
    
    # Instantiate the model structure without loading weights to be instant and safe
    with torch.device("meta"):
        model = AutoModelForCausalLM.from_config(config, trust_remote_code=True)
        
    print("Model Architecture Modules:")
    for name, module in model.named_modules():
        if "gate" in name.lower() or "mlp" in name.lower() or "router" in name.lower():
            print(f" - {name}: {type(module)}")

if __name__ == "__main__":
    main()
