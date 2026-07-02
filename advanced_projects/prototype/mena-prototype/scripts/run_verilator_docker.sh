#!/bin/bash
# Wrapper script to run Verilator inside the pre-built Docker container
# Usage: ./scripts/run_verilator_docker.sh [verilator_arguments]
# Example: ./scripts/run_verilator_docker.sh make all

docker run --rm -v /home/a/prototype/mena-prototype:/work -w /work/verilator mccpu-dev:latest "$@"
