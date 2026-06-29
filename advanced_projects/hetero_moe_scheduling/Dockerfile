FROM gem5-env:latest
ENV DEBIAN_FRONTEND=noninteractive

# 安裝 ARM C++ 交叉編譯器與實驗繪圖所需的 Python 套件
RUN apt-get update && apt-get install -y \
    g++-aarch64-linux-gnu \
    python3-pip \
    python3-pandas \
    python3-matplotlib \
    python3-yaml \
    && apt-get clean

WORKDIR /work
