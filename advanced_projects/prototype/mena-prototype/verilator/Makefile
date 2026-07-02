# Makefile for Verilator simulations

VERILATOR = verilator
VERILATOR_FLAGS = -Wall --cc --trace

# Targets
all: topk_tb dispatch_tb scheduler_tb wbuf_ctrl_tb cache_ctrl_tb mena_top_tb

topk_tb: obj_dir/Vtopk_selector.mk
	$(MAKE) -C obj_dir -f Vtopk_selector.mk Vtopk_selector
	cp obj_dir/Vtopk_selector topk_sim

dispatch_tb: obj_dir/Vtoken_dispatcher.mk
	$(MAKE) -C obj_dir -f Vtoken_dispatcher.mk Vtoken_dispatcher
	cp obj_dir/Vtoken_dispatcher dispatch_sim

scheduler_tb: obj_dir/Vexpert_queue_scheduler.mk
	$(MAKE) -C obj_dir -f Vexpert_queue_scheduler.mk Vexpert_queue_scheduler
	cp obj_dir/Vexpert_queue_scheduler scheduler_sim

wbuf_ctrl_tb: obj_dir/Vweight_buffer_ctrl.mk
	$(MAKE) -C obj_dir -f Vweight_buffer_ctrl.mk Vweight_buffer_ctrl
	cp obj_dir/Vweight_buffer_ctrl wbuf_ctrl_sim

cache_ctrl_tb: obj_dir/Vexpert_cache_ctrl.mk
	$(MAKE) -C obj_dir -f Vexpert_cache_ctrl.mk Vexpert_cache_ctrl
	cp obj_dir/Vexpert_cache_ctrl cache_sim

mena_top_tb: obj_dir/Vmena_top.mk
	$(MAKE) -C obj_dir -f Vmena_top.mk Vmena_top
	cp obj_dir/Vmena_top mena_top_sim

obj_dir/Vtopk_selector.mk: ../rtl/topk_selector.sv topk_tb.cpp topk_vectors.h
	$(VERILATOR) $(VERILATOR_FLAGS) --exe topk_tb.cpp ../rtl/topk_selector.sv

obj_dir/Vtoken_dispatcher.mk: ../rtl/token_dispatcher.sv dispatch_tb.cpp dispatch_vectors.h
	$(VERILATOR) $(VERILATOR_FLAGS) --exe dispatch_tb.cpp ../rtl/token_dispatcher.sv

obj_dir/Vexpert_queue_scheduler.mk: ../rtl/expert_queue_scheduler.sv scheduler_tb.cpp
	$(VERILATOR) $(VERILATOR_FLAGS) --exe scheduler_tb.cpp ../rtl/expert_queue_scheduler.sv

obj_dir/Vweight_buffer_ctrl.mk: ../rtl/weight_buffer_ctrl.sv wbuf_ctrl_tb.cpp
	$(VERILATOR) $(VERILATOR_FLAGS) --exe wbuf_ctrl_tb.cpp ../rtl/weight_buffer_ctrl.sv

obj_dir/Vexpert_cache_ctrl.mk: ../rtl/expert_cache_ctrl.sv cache_ctrl_tb.cpp
	$(VERILATOR) $(VERILATOR_FLAGS) --exe cache_ctrl_tb.cpp ../rtl/expert_cache_ctrl.sv

obj_dir/Vmena_top.mk: ../rtl/mena_top.sv ../rtl/token_dispatcher.sv ../rtl/expert_queue_scheduler.sv ../rtl/expert_cache_ctrl.sv ../rtl/weight_buffer_ctrl.sv mena_top_tb.cpp
	$(VERILATOR) $(VERILATOR_FLAGS) --exe mena_top_tb.cpp ../rtl/mena_top.sv ../rtl/token_dispatcher.sv ../rtl/expert_queue_scheduler.sv ../rtl/expert_cache_ctrl.sv ../rtl/weight_buffer_ctrl.sv

clean:
	rm -rf obj_dir topk_sim dispatch_sim scheduler_sim wbuf_ctrl_sim cache_sim mena_top_sim

.PHONY: all clean topk_tb dispatch_tb scheduler_tb wbuf_ctrl_tb cache_ctrl_tb mena_top_tb
