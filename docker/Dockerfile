FROM alpine
RUN \
    apk update && apk upgrade && \
    apk add --no-cache curl

# [SR-4632] Swift on Alpine Linux (support musl libc)
# https://github.com/swiftlang/swift/issues/47209
# Swift on Alpine not yet supported
#
# RUN curl -s https://archive.swiftlang.xyz/install.sh | bash && \

RUN \
    apk add --no-cache \
        bash \
        ca-certificates \
        clang \
        cmake \
        curl-dev \
        freeglut-dev \
        git \
        libpng-dev \
        lsb-release \
        meson \
        pango-dev \
        pkgconf \
        shadow # For useradd \
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
