FROM ghcr.io/userver-framework/ubuntu-24.04-userver:v2.14 AS generator

# install zeromq and cppzmq wrapper
RUN apt install -y libzmq5 cppzmq-dev

RUN if $(id 1000 | grep "no such"); then useradd -u 1000 api-test; fi
USER 1000

WORKDIR /app

# # copy only trdparty and cmake
# COPY --chown=1000:1000 ./backend/CMakeLists.txt ./backend/Makefile ./
# COPY --chown=1000:1000 ./backend/third_party ./third_party
#
# # build trdparty libs and remove cache
# RUN make trdparty-only
# RUN rm ./build-release/CMakeCache.txt

# FROM generator AS builder
#
# WORKDIR /app

# copy the rest of files
COPY --chown=1000:1000 ./backend/ .
COPY --chown=1000:1000 ./init ./init

RUN make release

ENTRYPOINT bash

