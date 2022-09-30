FROM ubuntu:focal 

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Berlin
RUN apt-get update -y && \
    apt-get install -y tzdata

RUN apt-get install -y --no-install-recommends\
        git \
        curl \
        gcc-9 \
        g++ \
        clang-10 \
        build-essential \
        cmake \
        unzip \
        tar \
        ca-certificates \
        cppcheck \
        libboost-all-dev \
        libgtest-dev
