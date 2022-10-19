FROM ubuntu:22.04 

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Berlin
RUN apt-get update -y && \
    apt-get install -y tzdata

RUN apt-get install -y --no-install-recommends \
        git \
        curl \
        gcc-9 \
        g++ \
        build-essential \
        cmake \
        unzip \
        tar \
        ca-certificates \
        libboost-all-dev \
        libgtest-dev

COPY ./_build /usr/src/messenger/microservices/m2m_chat_service/_build
