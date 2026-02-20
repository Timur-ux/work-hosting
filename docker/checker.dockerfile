FROM ubuntu:24.04 AS builder

RUN apt update 

RUN apt install -y build-essential cmake git libzstd-dev

WORKDIR /app

COPY ./backend .

RUN make release-target-checker

# store shared libs
RUN ldd /app/build-release/services/checker/checker | grep "=>" | awk '{print $3}' | xargs tar -uvf service-deps.tar

FROM ubuntu:24.04 AS checker

RUN apt update && apt install -y build-essential sudo cmake netcat-traditional git && apt clean all
RUN if [ ! -z $(id 1000 | grep "no such" ) ]; then \
		useradd -m -u 1000 checker;\
	else \
		usermod --login checker --move-home --home /home/checker $(getent passwd 1000 | cut -d ':' -f1);\
	fi

COPY --from=builder --chown=checker:checker /app/build-release/services/checker/checker /app/checker

# copy shared libs
COPY --from=builder /app/service-deps.tar /tmp/
RUN tar --overwrite -xf /tmp/service-deps.tar -C / && rm /tmp/service-deps.tar

# copy scripts
COPY ./backend/scripts /home/checker/scripts

USER checker

VOLUME [ "/checker", "/scripts", "/logs", "/home/checker/.ssh"]

ENTRYPOINT [ "/init.sh", "/app/checker" ]
