# se_moe_bench.py
# gem5 wrapper configuration for MoE scheduling microarchitecture simulation.
# Forwards parameters to gem5's deprecated se.py config to run SE mode with custom caches.

import sys
import os
import argparse

# Parse wrapper arguments
parser = argparse.ArgumentParser(description="gem5 Wrapper for MoE Benchmarks")
parser.add_argument('--cmd', type=str, required=True, help="Path to executable binary")
parser.add_argument('--options', type=str, default="", help="Command line options for the binary")
parser.add_argument('--l1d_size', type=str, default='64kB', help="L1 D-Cache size")
parser.add_argument('--l2_size', type=str, default='512kB', help="L2 Cache size")

args, unknown = parser.parse_known_args()

# Construct sys.argv for the target se.py script
# We hardcode O3CPU and cache flags as standard microarchitecture simulation parameters
sys.argv = [
    '/gem5/configs/deprecated/example/se.py',
    '--cpu-type=O3CPU',
    '--caches',
    '--l2cache',
    '--l1d_size=' + args.l1d_size,
    '--l1i_size=64kB',
    '--l2_size=' + args.l2_size,
    '--cmd=' + args.cmd,
    '--options=' + args.options
] + unknown

se_py_path = '/gem5/configs/deprecated/example/se.py'
if not os.path.exists(se_py_path):
    raise FileNotFoundError(f"Underlying gem5 se.py config not found at {se_py_path}")

# Execute se.py in the current namespace
with open(se_py_path, 'r') as f:
    code = compile(f.read(), se_py_path, 'exec')
    exec(code, globals())
