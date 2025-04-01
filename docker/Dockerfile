FROM debian:trixie-slim

ARG DEBIAN_FRONTEND=noninteractive
ENV DEBIAN_FRONTEND=$DEBIAN_FRONTEND

RUN \
    apt update && apt upgrade -y && \
    apt install -y curl && \
    apt install -y wget gpg gnupg lsb-release &&\
    wget https://apt.llvm.org/llvm.sh && chmod +x llvm.sh && ./llvm.sh 20 &&\
    apt install -y \
        ca-certificates \
        cmake \
        freeglut3-dev \
        git \
        libcurl4-openssl-dev \
        libpango1.0-dev \
        libpng-dev \
        lsb-release \
        meson \
        pkg-config \
        sudo \
        unzip \
        wget

RUN useradd -m builder -s /bin/bash && passwd -d builder
RUN echo "builder ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
WORKDIR /home/builder
USER builder

RUN curl -fsSL https://bun.sh/install | bash
RUN \
    echo "export BUN_INSTALL=\"\$HOME/.bun\"" >> $HOME/.profile && \
    echo "export PATH=\"\$BUN_INSTALL/bin:\$PATH\"" >> $HOME/.profile

USER root
WORKDIR /
CMD ["bash", "-l"]
