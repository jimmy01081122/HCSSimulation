# Makefile - 異質 MoE 推理協同設計模擬與實驗管理

DOCKER_IMAGE = hetero-moe-env
WORK_DIR = /work/advanced_projects/hetero_moe_scheduling

.PHONY: all image compile gem5 hmcs plot clean help

help:
	@echo "可用指令："
	@echo "  make image       - 建立專案專屬的 Docker 映像檔"
	@echo "  make compile     - 編譯 C++ scheduling 基準測試 (ARM64)"
	@echo "  make gem5        - 執行 gem5 微架構佇列與快取模擬"
	@echo "  make hmcs        - 執行系統級 MoE 異質調度與 DCMD 模擬實驗"
	@echo "  make plot        - 將實驗結果繪製成圖表"
	@echo "  make all         - 一鍵執行：編譯、gem5模擬、系統級DSE實驗、繪圖"
	@echo "  make clean       - 清理產生的執行檔與實驗報告"

image:
	docker build -t $(DOCKER_IMAGE) .

compile:
	docker run --rm -v /home/a/HCSSimulation:/work -v /home/a/gem5:/home/a/gem5 $(DOCKER_IMAGE) aarch64-linux-gnu-g++ -static -O3 \
		-o $(WORK_DIR)/gem5_sim/cpu_sched_bench_arm \
		$(WORK_DIR)/gem5_sim/cpu_sched_bench.cpp

gem5: compile
	@echo "執行 gem5 模擬中，這可能需要幾分鐘..."
	docker run --rm -v /home/a/HCSSimulation:/work -v /home/a/gem5:/home/a/gem5 $(DOCKER_IMAGE) bash $(WORK_DIR)/gem5_sim/run_gem5_se.sh

hmcs:
	docker run --rm -v /home/a/HCSSimulation:/work -v /home/a/gem5:/home/a/gem5 $(DOCKER_IMAGE) python3 $(WORK_DIR)/experiments/run_sweeps.py

plot:
	docker run --rm -v /home/a/HCSSimulation:/work -v /home/a/gem5:/home/a/gem5 $(DOCKER_IMAGE) python3 $(WORK_DIR)/experiments/plot_results.py

all: compile gem5 hmcs plot
	@echo "一鍵實驗流程順利完成！請查閱 reports/ 目錄下的圖表與報告。"

clean:
	rm -f gem5_sim/cpu_sched_bench_arm
	rm -rf reports/gem5_centralized reports/gem5_distributed
	rm -f reports/gem5_metrics_summary.json reports/sweep_results.csv reports/*.png
