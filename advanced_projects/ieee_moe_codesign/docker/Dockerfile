FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8

# Install base packages
RUN apt-get update && apt-get install -y --no-install-recommends \
    git wget curl gnupg \
    build-essential \
    python3 python3-dev python3-pip python3-venv \
    gcc-aarch64-linux-gnu g++-aarch64-linux-gnu \
    gcc-riscv64-linux-gnu g++-riscv64-linux-gnu \
    texlive-latex-base texlive-latex-recommended texlive-latex-extra \
    texlive-fonts-recommended texlive-science \
    latexmk \
    libgoogle-perftools-dev libtcmalloc-minimal4 \
    libprotobuf-dev \
    libhdf5-103-1t64 libhdf5-cpp-103-1t64 libhdf5-dev \
    && rm -rf /var/lib/apt/lists/*

# Symlink HDF5 libraries dynamically to standard serial names if needed
RUN for f in libhdf5 libhdf5_cpp; do \
      if [ -f /usr/lib/x86_64-linux-gnu/${f}_serial.so.103 ]; then \
        echo "Found ${f}_serial.so.103"; \
      elif [ -f /usr/lib/x86_64-linux-gnu/${f}.so.103 ]; then \
        ln -sf /usr/lib/x86_64-linux-gnu/${f}.so.103 /usr/lib/x86_64-linux-gnu/${f}_serial.so.103; \
      fi; \
    done

# Python scientific stack (using --break-system-packages for system-wide pip install in Ubuntu 24.04 container)
RUN python3 -m pip install --break-system-packages --no-cache-dir \
    numpy>=1.24 \
    pandas>=2.0 \
    matplotlib>=3.7 \
    scipy>=1.11 \
    pyyaml>=6.0 \
    pytest>=7.4 \
    seaborn>=0.12

WORKDIR /work

# Default: run pytest
CMD ["python3", "-m", "pytest", "simulator/tests/", "-v"]
