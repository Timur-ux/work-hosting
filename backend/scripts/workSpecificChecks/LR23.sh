#!/usr/bin/env bash
file="main.out"
if [[ ! -f $file ]]; then
	echo "No $file file found!"
	exit 1
fi

