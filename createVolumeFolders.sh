#!/usr/bin/env bash

# 
#	Creates folder for volume points
#


if [[ -z "$STORAGE_PATH" ]]; then
	echo "STORAGE_PATH env not specified!"
	exit 0
fi

if [[ -z "$LOG_PATH" ]]; then
	echo "LOG_PATH env not specified!"
	exit 0
fi

echo "Following mount points will be created:"

echo "${STORAGE_PATH}/postgres"
echo "${STORAGE_PATH}/checker"
echo "${LOG_PATH}/api"
echo "${LOG_PATH}/checker"
echo "${LOG_PATH}/nginx"

read -p "Continue? (Ctrl-C to cancel) ..."

mkdir -p "${STORAGE_PATH}/postgres"
mkdir -p "${STORAGE_PATH}/checker"
mkdir -p "${LOG_PATH}/api"
mkdir -p "${LOG_PATH}/checker"
mkdir -p "${LOG_PATH}/nginx"
