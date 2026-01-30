FROM ubuntu:24.04 AS builder

COPY ./backend/third_party/userver/scripts/docs/en/deps/ubuntu-24.04.md /userver_tmp/
COPY ./backend/third_party/userver/scripts/postgres/ubuntu-install-postgresql-includes.sh /userver_tmp/

RUN apt update \
  && apt install -y $(cat /userver_tmp/ubuntu-24.04.md) \
  && apt install -y clang-format python3-pip \
  && apt install -y \
    postgresql-16 \
    redis-server \
  && apt install -y locales \
  && apt clean all \
  && /userver_tmp/ubuntu-install-postgresql-includes.sh

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
