#!/bin/bash
echo "=== OS Version ==="
if [ -f /etc/os-release ]; then
    cat /etc/os-release | grep -E "PRETTY_NAME|VERSION="
else
    uname -a
fi

echo "=== Python & pip ==="
python3 --version 2>&1 || echo "Python3 is MISSING"
pip3 --version 2>&1 || echo "pip3 is MISSING"

echo "=== Compiler Versions ==="
gcc --version | head -n 1 || echo "gcc is MISSING"
g++ --version | head -n 1 || echo "g++ is MISSING"

echo "=== Build Tools ==="
make --version | head -n 1 || echo "make is MISSING"
cmake --version | head -n 1 || echo "cmake is MISSING"

echo "=== Verilator ==="
if command -v verilator &> /dev/null; then
    verilator --version
else
    echo "MISSING: verilator"
fi

echo "=== Icarus Verilog ==="
if command -v iverilog &> /dev/null; then
    iverilog -V | head -n 1
else
    echo "MISSING: iverilog"
fi

echo "=== Python Packages ==="
python3 -c "
packages = ['numpy', 'torch', 'pandas', 'matplotlib']
for pkg in packages:
    try:
        m = __import__(pkg)
        print(f'{pkg}: {m.__version__ if hasattr(m, \"__version__\") else \"installed\"}')
    except ImportError:
        print(f'{pkg}: MISSING')
" 2>/dev/null || echo "Failed to run Python check"

echo "=== GPU status ==="
if command -v nvidia-smi &> /dev/null; then
    nvidia-smi --query-gpu=name,memory.total --format=csv,noheader
else
    echo "nvidia-smi is MISSING or not functional"
fi
