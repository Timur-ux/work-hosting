FROM ubuntu:24.04 AS deps

COPY ./third_party/userver/scripts/docs/en/deps/ubuntu-24.04.md /userver_tmp/
COPY ./third_party/userver/scripts/postgres/ubuntu-install-postgresql-includes.sh /userver_tmp/

RUN apt update \
  && apt install -y $(cat /userver_tmp/ubuntu-24.04.md) \
  && apt install -y clang-format python3-pip \
  && apt install -y \
    postgresql-16 \
    redis-server \
  && apt install -y locales \
  && apt clean all \
  && /userver_tmp/ubuntu-install-postgresql-includes.sh

FROM ubuntu:24.04 AS builder

# copy shared libs
COPY --from=deps /usr/lib/ /usr/lib/

WORKDIR /app
COPY . .

RUN make release

FROM ubuntu:24.04 AS service

RUN \
  # Set UTC timezone \
  TZ=Etc/UTC; \
  ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone \
   \
  # Generate locales \
  && locale-gen en_US.UTF-8 \
  && update-locale LC_ALL="en_US.UTF-8" LANG="en_US.UTF-8" LANGUAGE="en_US.UTF-8"

RUN useradd -u 1000 service

# copy main service
COPY --from=builder --chown=service:service /app/build-release/services/service/service /app/service

# copy shared libs
COPY --from=deps /usr/lib/ /usr/lib/

USER service

VOLUME [ "/logs" ]

ENTRYPOINT [ "/app/service" ]

FROM ubuntu:24.04 AS checker

RUN \
  # Set UTC timezone \
  TZ=Etc/UTC; \
  ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone \
   \
  # Generate locales \
  && locale-gen en_US.UTF-8 \
  && update-locale LC_ALL="en_US.UTF-8" LANG="en_US.UTF-8" LANGUAGE="en_US.UTF-8"

RUN useradd -u 1000 checker

# copy main service
COPY --from=builder --chown=checker:checker /app/build-release/services/checker/checker /app/checker

# copy shared libs
COPY --from=deps /usr/lib/ /usr/lib/

USER service

VOLUME [ "/home/checker", "/logs" ]

ENTRYPOINT [ "/app/checker" ]
