FROM debian:trixie-slim

ARG DEBIAN_FRONTEND=noninteractive
ENV DEBIAN_FRONTEND=$DEBIAN_FRONTEND

RUN \
    apt update && apt upgrade -y && \
    apt install -y --no-install-recommends curl && \
    curl -s https://archive.swiftlang.xyz/install.sh | bash && \
    apt install --no-install-recommends -y \
        ca-certificates \
        clang-19 \
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
        swiftlang \
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
