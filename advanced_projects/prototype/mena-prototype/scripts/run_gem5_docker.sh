#!/bin/bash
# Wrapper script to run scons/gem5 commands inside the isolated gem5-dev Docker Compose container
# Usage: ./scripts/run_gem5_docker.sh [arguments]
# Example: ./scripts/run_gem5_docker.sh scons build/ARM/gem5.opt -j$(nproc)

docker compose run --rm gem5-dev "$@"
