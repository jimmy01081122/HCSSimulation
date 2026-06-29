# ============================================================
# MoE-HostAssist Research Project Makefile
# Discrete Event Simulation & Experiment Automation
# ============================================================

DOCKER_IMG := moe-hostassist-env
WORK_DIR := /home/a/HCSSimulation

# Docker run command
DOCKER_RUN := docker run --rm \
	-v $(WORK_DIR):/work \
	-w /work/advanced_projects/moe_hostassist \
	$(DOCKER_IMG)

.PHONY: all image test rtl validation experiments figures clean

all: image test rtl validation experiments figures
	@echo "===== All phases complete ====="

image:
	@echo ">>> Building Docker image moe-hostassist-env..."
	docker build -t $(DOCKER_IMG) -f docker/Dockerfile .

test:
	@echo ">>> Running simulator unit tests..."
	$(DOCKER_RUN) python3 -m pytest tests/ -v

rtl:
	@echo ">>> Compiling and simulating M2 (Hardware Queue Manager)..."
	iverilog -o hqm_sim hardware_rtl/hardware_queue_manager.v hardware_rtl/tb_hardware_queue_manager.v && ./hqm_sim
	@echo ">>> Compiling and simulating M3 (Expert Cache Admission Accelerator)..."
	iverilog -o cea_sim hardware_rtl/cache_eviction_accelerator.v hardware_rtl/tb_cache_eviction_accelerator.v && ./cea_sim

validation:
	@echo ">>> Running E3 validation sweep (Emulation vs Simulation)..."
	$(DOCKER_RUN) python3 validation/emulate_e3.py

experiments:
	@echo ">>> Running MoE-HostAssist core simulation sweeps (E1-E5)..."
	$(DOCKER_RUN) python3 experiments/run_experiments.py

figures:
	@echo ">>> Generating publication figures..."
	$(DOCKER_RUN) python3 experiments/plot_results.py

clean:
	@echo ">>> Cleaning generated files..."
	$(DOCKER_RUN) rm -rf .pytest_cache simulator/__pycache__ tests/__pycache__ experiments/__pycache__ validation/__pycache__
	rm -rf results/*.json results/*.csv
	rm -rf figures/*.png figures/*.pdf
	rm -f hqm_sim cea_sim

