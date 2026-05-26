#!/bin/bash
# gem5 Learning Project - Main Test Script
# Purpose: Verify all components of the gem5 learning framework
# Usage: ./test_project.sh [command]

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
PROJECT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
GEM5_DIR="$PROJECT_ROOT/gem5"

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

log_section() {
    echo -e ""
    echo -e "${BLUE}========== $1 ==========${NC}"
}

# Function to verify project structure
verify_structure() {
    log_section "Verifying Project Structure"
    
    local required_dirs=(
        "experiments/gem5-learning-arm"
        "experiments/gem5-learning-riscv"
        "dir/TRY/dir1_memory_dma"
        "dir/TRY/dir2_moe_routing"
        "dir/TRY/dir3_kv_cache"
        "docs"
        "scripts"
        "gem5"
    )
    
    local all_present=true
    for dir in "${required_dirs[@]}"; do
        if [ -d "$PROJECT_ROOT/$dir" ]; then
            log_info "Found: $dir"
        else
            log_error "Missing: $dir"
            all_present=false
        fi
    done
    
    return $([ "$all_present" = true ] && echo 0 || echo 1)
}

# Function to verify documentation
verify_documentation() {
    log_section "Verifying Documentation"
    
    local required_docs=(
        "INDEX.md"
        "README.md"
        "TUTORIAL_START_HERE.md"
        "ISA_SELECTION_GUIDE.md"
        "prompt_generated_gem5_tutorial_ARM.md"
        "prompt_generated_gem5_tutorial_RISCV.md"
        "docs/direction1_dma_tutorial.md"
        "docs/direction2_moe_routing_tutorial.md"
        "docs/direction3_kv_cache_tutorial.md"
    )
    
    local all_present=true
    for doc in "${required_docs[@]}"; do
        if [ -f "$PROJECT_ROOT/$doc" ]; then
            log_info "Found: $doc"
        else
            log_error "Missing: $doc"
            all_present=false
        fi
    done
    
    return $([ "$all_present" = true ] && echo 0 || echo 1)
}

# Function to verify executable scripts
verify_executables() {
    log_section "Verifying Executable Scripts"
    
    local required_scripts=(
        "experiments/gem5-learning-arm/test_arm.sh"
        "experiments/gem5-learning-riscv/test_riscv.sh"
        "dir/TRY/dir1_memory_dma/run.sh"
        "dir/TRY/dir2_moe_routing/run.sh"
        "dir/TRY/dir3_kv_cache/run.sh"
    )
    
    local all_executable=true
    for script in "${required_scripts[@]}"; do
        if [ -f "$PROJECT_ROOT/$script" ]; then
            if [ -x "$PROJECT_ROOT/$script" ]; then
                log_info "Executable: $script"
            else
                log_warn "Not executable: $script"
                chmod +x "$PROJECT_ROOT/$script" 2>/dev/null || true
                if [ -x "$PROJECT_ROOT/$script" ]; then
                    log_info "Made executable: $script"
                else
                    log_error "Cannot make executable: $script"
                    all_executable=false
                fi
            fi
        else
            log_warn "Script not found (optional): $script"
        fi
    done
    
    return $([ "$all_executable" = true ] && echo 0 || echo 1)
}

# Function to verify gem5 prerequisites
verify_gem5_prerequisites() {
    log_section "Verifying gem5 Prerequisites"
    
    if [ ! -d "$GEM5_DIR" ]; then
        log_warn "gem5 source directory not found: $GEM5_DIR"
        log_warn "You can clone it with:"
        log_warn "  cd $PROJECT_ROOT"
        log_warn "  git clone https://gem5.googlesource.com/public/gem5"
        return 1
    fi
    
    if [ ! -f "$GEM5_DIR/build/ARM/gem5.opt" ]; then
        log_warn "gem5 ARM binary not compiled"
        log_info "To compile, run:"
        log_info "  cd $GEM5_DIR"
        log_info "  scons build/ARM/gem5.opt -j\$(nproc)"
    else
        log_info "gem5 ARM binary found"
    fi
    
    if [ ! -f "$GEM5_DIR/build/RISCV/gem5.opt" ]; then
        log_warn "gem5 RISC-V binary not compiled"
        log_info "To compile, run:"
        log_info "  cd $GEM5_DIR"
        log_info "  scons build/RISCV/gem5.opt -j\$(nproc)"
    else
        log_info "gem5 RISC-V binary found"
    fi
    
    return 0
}

# Function to show project status
show_status() {
    log_section "Project Status Summary"
    
    echo "Project Root: $PROJECT_ROOT"
    echo ""
    echo "Documentation:"
    find "$PROJECT_ROOT" -maxdepth 1 -name "*.md" -type f | wc -l | xargs echo "  Markdown files:"
    echo "  Experiment workspaces: 2 (ARM, RISC-V)"
    echo "  Research directions: 3 (DMA, MoE routing, KV cache)"
    echo ""
    echo "Script Status:"
    [ -x "$PROJECT_ROOT/experiments/gem5-learning-arm/test_arm.sh" ] && \
        echo "  ARM test script: Executable" || \
        echo "  ARM test script: Not executable"
    [ -x "$PROJECT_ROOT/experiments/gem5-learning-riscv/test_riscv.sh" ] && \
        echo "  RISC-V test script: Executable" || \
        echo "  RISC-V test script: Not executable"
}

# Function to show usage
show_usage() {
    cat << EOF
gem5 Learning Project - Main Test Script

Usage: $0 [command]

Commands:
  verify-structure    - Verify project directory structure
  verify-docs        - Verify all documentation files
  verify-scripts     - Verify executable scripts
  verify-gem5        - Verify gem5 prerequisites
  status             - Show project status
  full               - Run all verification checks
  help               - Show this help message

Examples:
  $0 verify-structure   # Check if all directories exist
  $0 full              # Run all checks
  $0 status            # Show current project status

Quick Start After Verification:
  1. Review: $PROJECT_ROOT/INDEX.md
  2. Choose ISA: $PROJECT_ROOT/ISA_SELECTION_GUIDE.md
  3. Run tests:
     - $PROJECT_ROOT/experiments/gem5-learning-arm/test_arm.sh check
     - $PROJECT_ROOT/experiments/gem5-learning-riscv/test_riscv.sh check

EOF
}

# Main execution
main() {
    local command="${1:-help}"
    local all_passed=true
    
    case "$command" in
        verify-structure)
            verify_structure || all_passed=false
            ;;
        verify-docs)
            verify_documentation || all_passed=false
            ;;
        verify-scripts)
            verify_executables || all_passed=false
            ;;
        verify-gem5)
            verify_gem5_prerequisites || all_passed=false
            ;;
        status)
            show_status
            ;;
        full)
            verify_structure || all_passed=false
            verify_documentation || all_passed=false
            verify_executables || all_passed=false
            verify_gem5_prerequisites || all_passed=false
            show_status
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
    
    # Print final status
    echo ""
    if [ "$all_passed" = true ]; then
        log_info "All checks passed!"
        exit 0
    else
        log_warn "Some checks failed. See above for details."
        exit 1
    fi
}

# Run main function
main "$@"
