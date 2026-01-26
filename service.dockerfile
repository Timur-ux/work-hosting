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

FROM ubuntu:24.04 AS release

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
COPY --from=builder --chown=service:service /app/build-release/service/service /app/service

# copy shared libs
COPY --from=deps /usr/lib/ /usr/lib/

USER service

VOLUME [ "/logs" ]

ENTRYPOINT [ "/app/service" ]

FROM ubuntu:24.04 AS develop

# copy shared libs
COPY --from=deps /usr/lib/ /usr/lib/

RUN apt update &&\
    apt install -y wget git python3-neovim zip python3-pip python3-virtualenv npm &&\
		apt clean all

RUN useradd develop -m -s /bin/bash
USER develop

ENV HOME=/home/develop
ENV NVIM_BIN=/home/develop/nvim-linux-x86_64/bin

# nvim setup
RUN cd /home/develop/ && \
    wget https://github.com/neovim/neovim/releases/download/v0.11.5/nvim-linux-x86_64.tar.gz &&\
	  tar xzvf nvim-linux-x86_64.tar.gz

# setup my config
RUN mkdir -p /home/develop/.config && \
		git clone https://github.com/Timur-ux/nvim-setup.git /home/develop/.config/nvim

# install lsps,formatters,debuggers ...
# RUN ${NVIM_BIN}/nvim --headless -c "lua MasonInstallDefaults()" -c qall

VOLUME [ "/app" ]

ENTRYPOINT [ "/home/develop/nvim-linux-x86_64/bin/nvim", "/app" ]
