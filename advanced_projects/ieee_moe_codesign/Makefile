# ============================================================
# HEXA-MoE Research Project Makefile
# IEEE-level MoE Co-Design Research Automation
# ============================================================

PROJ_DIR := $(shell pwd)
DOCKER_IMG := ieee-moe-env
GEM5_DIR := /home/a/gem5
WORK_DIR := /home/a/HCSSimulation

# Docker run command with all mounts
DOCKER_RUN := docker run --rm \
	-v $(WORK_DIR):/work \
	-v $(GEM5_DIR):/gem5 \
	-w /work/advanced_projects/ieee_moe_codesign \
	$(DOCKER_IMG)

.PHONY: all image test compile gem5 gem5_riscv exp1 exp2 exp3 exp4 exp5 experiments figures paper clean

# ========== Full pipeline ==========
all: image test compile experiments gem5 gem5_riscv figures
	@echo "===== All phases complete ====="

# ========== Phase 0: Docker image ==========
image:
	@echo ">>> Building Docker image..."
	docker build -t $(DOCKER_IMG) -f docker/Dockerfile .

# ========== Phase 1: Unit tests ==========
test:
	@echo ">>> Running simulator unit tests..."
	$(DOCKER_RUN) python3 -m pytest simulator/tests/ -v

# ========== Phase 2: Compile gem5 workloads ==========
compile:
	@echo ">>> Compiling gem5 C++ workloads..."
	$(DOCKER_RUN) bash -c " \
		cd gem5_workloads/src && \
		aarch64-linux-gnu-g++ -static -O3 -o centralized_arm centralized_queue.cpp && \
		aarch64-linux-gnu-g++ -static -O3 -o distributed_arm distributed_queue.cpp && \
		aarch64-linux-gnu-g++ -static -O3 -o lock_free_arm lock_free_queue.cpp && \
		riscv64-linux-gnu-g++ -static -O3 -o centralized_riscv centralized_queue.cpp && \
		riscv64-linux-gnu-g++ -static -O3 -o distributed_riscv distributed_queue.cpp && \
		riscv64-linux-gnu-g++ -static -O3 -o lock_free_riscv lock_free_queue.cpp && \
		echo 'Compilation OK' \
	"

# ========== Phase 3: DSE Experiments ==========
exp1:
	@echo ">>> Experiment 1: Cache capacity sweep..."
	$(DOCKER_RUN) python3 experiments/scripts/run_single_exp.py \
		--config experiments/configs/exp1_cache_sweep.yaml

exp2:
	@echo ">>> Experiment 2: PCIe bandwidth sweep..."
	$(DOCKER_RUN) python3 experiments/scripts/run_single_exp.py \
		--config experiments/configs/exp2_pcie_sweep.yaml

exp3:
	@echo ">>> Experiment 3: Concurrency scaling..."
	$(DOCKER_RUN) python3 experiments/scripts/run_single_exp.py \
		--config experiments/configs/exp3_concurrency.yaml

exp4:
	@echo ">>> Experiment 4: Zipf alpha sensitivity..."
	$(DOCKER_RUN) python3 experiments/scripts/run_single_exp.py \
		--config experiments/configs/exp4_zipf_alpha.yaml

exp5:
	@echo ">>> Experiment 5: CPU core scaling..."
	$(DOCKER_RUN) python3 experiments/scripts/run_single_exp.py \
		--config experiments/configs/exp5_cpu_cores.yaml

experiments: exp1 exp2 exp3 exp4 exp5
	@echo ">>> All DSE experiments complete."

# ========== Phase 4: gem5 microarch validation ==========
gem5:
	@echo ">>> Running gem5 ARM microarchitecture validation..."
	$(DOCKER_RUN) bash gem5_workloads/scripts/run_gem5_all.sh

gem5_riscv:
	@echo ">>> Running gem5 RISC-V microarchitecture validation..."
	$(DOCKER_RUN) bash gem5_workloads/scripts/run_gem5_riscv_all.sh

# ========== Phase 5: Generate figures ==========
figures:
	@echo ">>> Generating publication figures..."
	$(DOCKER_RUN) python3 experiments/figures/generate_all_figures.py

# ========== Phase 6: Compile LaTeX paper ==========
paper:
	@echo ">>> Compiling IEEE paper..."
	$(DOCKER_RUN) bash -c " \
		cd paper/ieee_en && \
		latexmk -pdf -interaction=nonstopmode main.tex \
	"

# ========== Cleanup ==========
clean:
	@echo ">>> Cleaning generated files..."
	rm -rf experiments/results/*.csv
	rm -rf experiments/figures/*.png experiments/figures/*.pdf
	rm -f gem5_workloads/src/*_arm gem5_workloads/src/*_riscv
	rm -rf paper/ieee_en/*.aux paper/ieee_en/*.log paper/ieee_en/*.pdf
