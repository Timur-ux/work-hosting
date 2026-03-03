#!/usr/bin/env bash

for file in reader.out searcher.out generator.out; do
	if [[ ! -f $file ]]; then
		echo "No $file file found!"
		exit 1
	fi
done
