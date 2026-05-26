#!/bin/bash
# ARM gem5 Learning Framework - Test Script
# Purpose: Verify ARM gem5 configuration and run basic experiments
# Usage: ./test_arm.sh [experiment_name]
# Available experiments: hello, cache, dse

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Configuration
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/../.." && pwd )"
GEM5_DIR="$PROJECT_ROOT/gem5"
EXPERIMENT_DIR="$SCRIPT_DIR"

# Function to print colored output
log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to check prerequisites
check_prerequisites() {
    log_info "Checking prerequisites for ARM experiments..."
    
    if [ ! -f "$GEM5_DIR/build/ARM/gem5.opt" ]; then
        log_error "gem5 ARM binary not found at $GEM5_DIR/build/ARM/gem5.opt"
        log_warn "Please compile gem5 first:"
        log_warn "  cd $GEM5_DIR"
        log_warn "  scons build/ARM/gem5.opt -j\$(nproc)"
        return 1
    fi
    
    if [ ! -f "$EXPERIMENT_DIR/configs/se_hello_arm.py" ]; then
        log_error "ARM hello configuration not found"
        return 1
    fi
    
    log_info "All prerequisites met!"
    return 0
}

# Function to run hello world experiment
run_hello_experiment() {
    local output_dir="$EXPERIMENT_DIR/results/se_hello"
    
    log_info "Running ARM hello world experiment..."
    log_info "Output directory: $output_dir"
    
    mkdir -p "$output_dir"
    
    cd "$GEM5_DIR"
    
    # Run gem5
    ./build/ARM/gem5.opt \
        --outdir="$output_dir" \
        "$EXPERIMENT_DIR/configs/se_hello_arm.py"
    
    if [ -f "$output_dir/stats.txt" ]; then
        log_info "Experiment completed successfully!"
        log_info "Statistics saved to: $output_dir/stats.txt"
        return 0
    else
        log_error "Experiment failed - no stats.txt generated"
        return 1
    fi
}

# Function to parse statistics
parse_statistics() {
    local results_dir="${1:-$EXPERIMENT_DIR/results}"
    
    log_info "Parsing statistics from $results_dir..."
    
    if [ -d "$results_dir" ]; then
        python3 "$EXPERIMENT_DIR/scripts/parse_stats_arm.py" \
            "$results_dir" \
            --out "$results_dir/summary.csv"
        
        log_info "Statistics parsed and saved to: $results_dir/summary.csv"
        return 0
    else
        log_error "Results directory not found: $results_dir"
        return 1
    fi
}

# Function to display results
display_results() {
    local results_file="${1:-$EXPERIMENT_DIR/results/se_hello/stats.txt}"
    
    if [ -f "$results_file" ]; then
        log_info "=== ARM gem5 Simulation Results ==="
        echo ""
        head -50 "$results_file"
        echo ""
        log_info "Full results available at: $results_file"
    fi
}

# Function to show usage
show_usage() {
    echo "ARM gem5 Learning Framework - Test Script"
    echo ""
    echo "Usage: $0 [command]"
    echo ""
    echo "Commands:"
    echo "  check      - Check prerequisites only"
    echo "  hello      - Run hello world experiment (default)"
    echo "  parse      - Parse statistics from results"
    echo "  view       - Display results from hello experiment"
    echo "  all        - Run hello + parse + view"
    echo "  help       - Show this help message"
    echo ""
}

# Main execution
main() {
    local command="${1:-hello}"
    
    case "$command" in
        check)
            check_prerequisites
            ;;
        hello)
            if check_prerequisites; then
                run_hello_experiment
            else
                exit 1
            fi
            ;;
        parse)
            parse_statistics
            ;;
        view)
            display_results
            ;;
        all)
            if check_prerequisites; then
                run_hello_experiment && \
                parse_statistics && \
                display_results
            else
                exit 1
            fi
            ;;
        help)
            show_usage
            ;;
        *)
            log_error "Unknown command: $command"
            show_usage
            exit 1
            ;;
    esac
}

# Run main function
main "$@"
