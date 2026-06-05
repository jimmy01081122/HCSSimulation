#!/bin/bash
# install_dependencies.sh
# Automated dependency installer for the MoE routing-aware prefetch project.

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_DIR="$DIR/.."
cd "$PROJECT_DIR"

echo "=== System Dependency Check and Installation ==="

# Check if apt-get is available (Debian/Ubuntu systems)
if [ -x "$(command -v apt-get)" ]; then
    echo "Debian-based system detected. Installing iverilog and python3-pip..."
    # Use sudo only if not running as root and sudo is available
    if [ "$EUID" -ne 0 ] && [ -x "$(command -v sudo)" ]; then
        sudo apt-get update
        sudo apt-get install -y iverilog python3-pip python3-setuptools
    else
        apt-get update
        apt-get install -y iverilog python3-pip python3-setuptools
    fi
else
    echo "Warning: apt-get not found. Please manually install 'iverilog' (Icarus Verilog) and 'pip3' for your platform."
fi

echo "=== Python Packages Installation ==="

# Try upgrading pip (optional, non-critical)
python3 -m pip install --upgrade pip --user 2>/dev/null || true

# Install python dependencies
if [ -f requirements.txt ]; then
    echo "Installing requirements from requirements.txt..."
    # Install with user mode to avoid permissions issues
    python3 -m pip install -r requirements.txt --user
else
    echo "Error: requirements.txt not found!"
    exit 1
fi

echo "=== Dependency Installation Complete ==="
