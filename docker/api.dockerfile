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
COPY ./backend .

RUN make release-target-api

# store shared libs
RUN ldd /app/build-release/services/api/api | grep "=>" | awk '{print $3}' | xargs tar -uhf service-deps.tar

FROM ubuntu:24.04 AS service

RUN if [ ! -z $(id 1000 | grep "no such" ) ]; then \
		useradd -m -u 1000 api;\
	else \
		usermod --login api --move-home --home /home/api $(getent passwd 1000 | cut -d ':' -f1);\
	fi

# copy shared libs
COPY --from=builder /app/service-deps.tar /tmp/
RUN tar --overwrite -xf /tmp/service-deps.tar -C / && rm /tmp/service-deps.tar

# copy main service
COPY --from=builder --chown=api:api /app/build-release/services/api/api /app/api

USER api

VOLUME [ "/home/api/logs" ]

ENTRYPOINT [ "/app/api" ]
