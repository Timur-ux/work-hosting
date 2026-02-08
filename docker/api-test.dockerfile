FROM ghcr.io/userver-framework/ubuntu-24.04-userver:v2.14 AS generator

# install zeromq and cppzmq wrapper
RUN apt install -y libzmq5 cppzmq-dev

WORKDIR /app

# copy only trdparty and cmake
COPY ./backend/CMakeLists.txt ./backend/Makefile ./
COPY ./backend/third_party ./third_party

# build trdparty libs and remove cache
RUN make trdparty-only
RUN rm ./build-release/CMakeCache.txt

FROM generator AS builder

WORKDIR /app

# copy the rest of files
COPY ./backend/ .

RUN make release

ENTRYPOINT bash

