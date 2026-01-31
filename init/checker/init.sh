#!/bin/sh

if [ -z "$LOG_PATH" ]; then
	echo "LOG_PATH env not specified!"
	exit 1
fi

exec "$@" | tee -a $LOG_PATH
