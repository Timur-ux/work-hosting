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

USER 1000


