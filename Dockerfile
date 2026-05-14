FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y git build-essential scons python3 python3-dev python3-tk libgoogle-perftools-dev m4 zlib1g zlib1g-dev libprotobuf-dev protobuf-compiler libprotoc-dev libboost-all-dev libhdf5-dev pkg-config python3-venv && apt-get clean
WORKDIR /work
