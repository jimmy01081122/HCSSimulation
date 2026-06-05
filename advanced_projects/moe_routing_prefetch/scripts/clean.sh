#!/bin/bash
# clean.sh
# Cleans generated trace files, simulation results, and testbench outputs.

# Get the directory of the script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_DIR="$DIR/.."

echo "Cleaning up generated files..."

# Clean traces directory except .gitkeep or README.md
find "$PROJECT_DIR/traces" -type f ! -name "*.md" ! -name ".gitkeep" -delete

# Clean results directory except .gitkeep or README.md
find "$PROJECT_DIR/results" -type f ! -name "*.md" ! -name ".gitkeep" -delete

# Clean tb directory compiled and hex files except .gitkeep or README.md
find "$PROJECT_DIR/tb" -type f \( -name "*.vvp" -o -name "*.vcd" -o -name "stimulus_*.hex" \) -delete

echo "Cleanup complete."
