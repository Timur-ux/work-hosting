#!/usr/bin/env bash

shopt -s nullglob
set -o pipefail

stepStart() {
	if (( "$#" < 1 )); then echo "Usage: $0 <Step name>"; exit 1; fi
	echo "[LOG] Step $1 started..."
}

stepPassed() {
	if (( "$#" < 1 )); then echo "Usage: $0 <Step name>"; exit 1; fi
	echo "[OK] $1"
}

stepFailed() {
	if (( "$#" < 2 )); then echo "Usage: $0 <Step name> <Error output>"; exit 1; fi
	echo "[ERROR] $1 failed. See error output below:"
	echo "$2"
	exit 1
}

if (( "$#" < "2" )); then
	echo "Usage: $0 <Lab work name> <student GV name>"
	exit 0
fi

organizationName="mai_labs_2025_2026"

LR="$1"
student="$2"

workName="$LR-$student"
logFile="/tmp/$workName.log"

# create dir for backups
backupDir="/tmp/backups"
if [ ! -d "$backupDir" ]; then
	mkdir -p "$backupDir"
fi

# create dir for works (should be mounted for later usage)
worksDir="/mnt/storage/raison/works"

# if file exist -- do backup
if [ -f "$logFile" ]; then
	# find quantity of backups
	backupsQuantity=$(ls "$backupDir" | grep "$workName" | wc -l)
	mv "$logFile" "${backupDir}/${workName}_${backupsQuantity}.log.bak"
fi

# clear logfile
> "$logFile"

# start checking

doStep() {
	if (( "$#" < 2 )); then
		echo "Usage: $0 <step name> <step func>"
		exit 1
	fi

	stepStart "$1" >> "$logFile"
	message="$($2)"
	local retCode="$?"
	if [ "$retCode" != "0" ]; then 
		stepFailed "$1" "$message" >> "$logFile"
	else
		stepPassed "$1" >> "$logFile"
	fi
}

# step 0: fetching repo
fetchRepository() {
	local gvSshLink="ssh://git@gitverse.ru:2222/${organizationName}/${workName}.git"
	message=$(git clone "$gvSshLink" "${worksDir}/${workName}" 2>/dev/stdout)
	if [ "$?" != "0" ]; then
		echo "$message"
		return 1
	fi
	return 0
}
doStep "Fetching repository" fetchRepository

