FROM nvim-setup:latest AS develop

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

RUN \
  # Set UTC timezone \
  TZ=Etc/UTC; \
  ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone \
   \
  # Generate locales \
  && locale-gen en_US.UTF-8 \
  && update-locale LC_ALL="en_US.UTF-8" LANG="en_US.UTF-8" LANGUAGE="en_US.UTF-8"

USER 1000


