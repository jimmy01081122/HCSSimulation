
#!/usr/bin/env bash

set -euo pipefail

GEM5_ROOT="/work"
GEM5_DIR="$GEM5_ROOT/gem5"
GEM5_TAG="v25.1.0.1"
JOBS="${JOBS:-$(nproc)}"

echo "============================================================"
echo "gem5 v25.1.0.1 ARM/RISCV install script for Ubuntu 22.04"
echo "Install path: $GEM5_DIR"
echo "Build jobs: $JOBS"
echo "============================================================"

echo
echo "[1/8] Checking Ubuntu version"
lsb_release -a || true

echo
echo "[2/8] Installing dependencies"
#sudo 
apt update

#sudo 
  apt install -y \
  build-essential git m4 scons zlib1g zlib1g-dev \
  libprotobuf-dev protobuf-compiler libprotoc-dev libgoogle-perftools-dev \
  python3-dev libboost-all-dev pkg-config python3-tk clang-format-15 \
  libcapstone-dev libpng-dev libelf-dev wget cmake

echo
echo "[3/8] Setting clang-format-15 as default if available"
if command -v clang-format-15 >/dev/null 2>&1; then
  sudo update-alternatives --install \
    /usr/bin/clang-format clang-format /usr/bin/clang-format-15 150 || true
fi

echo
echo "[4/8] Creating root directory"
mkdir -p "$GEM5_ROOT"
cd "$GEM5_ROOT"

echo
echo "[5/8] Cloning or updating gem5"
if [ -d "$GEM5_DIR/.git" ]; then
  echo "Existing gem5 repository found: $GEM5_DIR"
  cd "$GEM5_DIR"
  git fetch --all --tags
else
  git clone https://github.com/gem5/gem5.git "$GEM5_DIR"
  cd "$GEM5_DIR"
  git fetch --all --tags
fi

echo
echo "[6/8] Checking out $GEM5_TAG"
git checkout "$GEM5_TAG"

echo
echo "[7/8] Exporting UTF-8 environment for this shell"
export LANG=C.UTF-8
export LC_ALL=C.UTF-8
export PYTHONUTF8=1

echo "LANG=$LANG"
echo "LC_ALL=$LC_ALL"
python3 - <<'PYEOF'
import locale
print("Python preferred encoding:", locale.getpreferredencoding(False))
PYEOF

echo
echo "[8/8] Building RISCV and ARM gem5.opt"
echo "Building RISCV..."
scons build/RISCV/gem5.opt -j$(nproc)

echo
echo "Building ARM..."
scons build/ARM/gem5.opt -j$(nproc)

echo
echo "============================================================"
echo "Build completed"
echo "============================================================"

ls -lh build/RISCV/gem5.opt
ls -lh build/ARM/gem5.opt

echo
echo "Creating test output directory"
mkdir -p "$GEM5_ROOT/experiments/gem5-v25-test"

echo
echo "Testing RISCV hello with built-in test binary"
./build/RISCV/gem5.opt \
  --outdir="$GEM5_ROOT/experiments/gem5/se_hello.py" \
  configs/deprecated/example/se.py \
  -c tests/test-progs/hello/bin/riscv/linux/hello

echo
echo "RISCV test stats:"
grep -E "simInsts|simTicks|hostSeconds|hostInstRate" \
  "$GEM5_ROOT/experiments/gem5/se_hello.py/stats.txt" || true

echo
echo "============================================================"
echo "Done"
echo "gem5 path: $GEM5_DIR"
echo "RISCV binary: $GEM5_DIR/build/RISCV/gem5.opt"
echo "ARM binary:   $GEM5_DIR/build/ARM/gem5.opt"
echo "============================================================"
