#!/usr/bin/env bash

shopt -s nullglob
set -o pipefail

user="checker"
if [ "$(whoami)" == "root" ]; then
	echo "No use under root allowed"
	return 1
fi

# constants
COPY_DOCS=true

# init variables
organizationName="mai_labs_2025_2026"
baseDir="/checker/data"
if [ ! -d "$baseDir" ]; then
	mkdir -p "$baseDir"
fi

backupDir="$baseDir/backups"
worksDir="$baseDir/works"
docsDir="$baseDir/docs"

read workType
read workNumber
read student
read first_name
read last_name
read father_name
read group_number
read in_group_order

workName="${workType}${workNumber}-${student}"
logFile="${docsDir}/logs/$workName.log"

# Status functions
stepStart() {
	if (( "$#" < 1 )); then echo "Usage: $0 <Step name>"; exit 1; fi
	echo "[LOG] Step {$1} started..."
}

stepPassed() {
	if (( "$#" < 1 )); then echo "Usage: $0 <Step name>"; exit 1; fi
	echo "[OK] {$1}"
}

stepFailed() {
	if (( "$#" < 2 )); then echo "Usage: $0 <Step name> <Error output>"; exit 1; fi
	echo "[ERROR] {$1} failed. See error output below:"
	echo "$2"
	exit 1
}

# create dir for backups(if not exist)
if [ ! -d "$backupDir" ]; then
	mkdir -p "$backupDir"
fi

# create dir for works (should be mounted for usage from container)
if [ ! -d "$worksDir" ]; then
	mkdir -p "$worksDir"
fi

# create dir for documents (should be mounted for usage from container)
if [ ! -d "$docsDir" ]; then
	mkdir -p "$docsDir"
	if [ ! -d "$docsDir/logs" ]; then
		mkdir -p "${docsDir}/logs"
	fi
fi

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
	message="$($2 2>/dev/stdout)"
	if [ "$?" != "0" ]; then
		stepFailed "$1" "$message" >> "$logFile"
	else
		stepPassed "$1" >> "$logFile"
	fi
}

# step 0: fetching repo
fetchRepository() {
	local gvSshLink="ssh://git@gitverse.ru:2222/${organizationName}/${workName}.git"
	if [[ -d "${worksDir}/${workName}" ]]; then
		cd "${worksDir}/${workName}" && \
			git clean -fd && git fetch origin && git reset --hard
		return $?
	fi
	message=$(git clone "$gvSshLink" "${worksDir}/${workName}" 2>/dev/stdout)
	if [ "$?" != "0" ]; then
		echo "$message"
		return 1
	fi
	return 0
}
doStep "Скачивание репозитория" fetchRepository

cd "${worksDir}/${workName}"
# step 1: pull solution branch
pullSolutionBranch() {
	message=$(git pull origin solution)
	if [ "$?" != "0" ]; then
		echo "$message"
		return 1
	fi
	return 0
}
doStep "Скачивание ветки solution" pullSolutionBranch

# step 2: switch to solution branch
switchToSolutionBranch() {
	message=$(git checkout solution --)
	if [ "$?" != "0" ]; then
		echo "$message"
		return 1
	fi
	return 0
}
doStep "Переход на ветку solution" switchToSolutionBranch

# step 3: check solution/ and solution/report/ folders
checkFolders() {
	if [ ! -d "$(pwd)/solution" ]; then
		echo "Folder solution/ not found"
		return 1
	elif [ ! -d "$(pwd)/solution/report" ]; then
		echo "Folder solution/report/ not found"
		return 1
	fi
	return 0
}
doStep "Проверка наличия solution/ и solution/report/ папок" checkFolders

cd ./solution
# step 3: check for report and applications
checkReportAndApplications() {
	local filesCount="$(ls ./report | wc -l)"
	local rusLRName="ЛР"
	if [[ "$workType" == "KP" ]]; then
		rusLRName="КП"
	fi
	if [[ "$filesCount" != "2" ]]; then
		echo "Expected 2 files in solution/report/ folder, but given $filesCount"
		return 1
	fi
	local spacedFiles=$(ls ./report | grep -E "\s")
	if [[ ! -z "$spacedFiles" ]]; then
		echo "Spaces in file names not allowed: $spacedFiles"
		return 1
	fi

	reportName="${rusLRName}${workNumber}_${last_name}${first_name:0:2}${father_name:0:2}_${group_number}_${in_group_order}.pdf"
	if [[ ! -f "./report/$reportName" ]]; then
		echo "Expected report with name ./solution/report/$reportName but it is not found!"
		echo "Founded files $(ls ./report)"
		return 1
	fi
	applicationName="Приложение${reportName}"
	if [[ ! -f "./report/$applicationName" ]]; then
		echo "Expected application with name ./solution/report/$reportName but it is not found!"
		return 1
	fi
}
doStep "Проверка формата отчетов" checkReportAndApplications

# step 3.1: copy report and application if COPY_DOCS = true
copyDocs() {
	message=$(cp ./report/* $docsDir 2>/dev/stdout)
	if [[ "$?" != "0" ]]; then
		echo "Unexpected error while copying report files, tell developer to fix it: $message"
		return 1
	fi
}
if [[ $COPY_DOCS ]]; then
	doStep "Копирование отчетов" copyDocs
fi

# step 4: check for files outside solution folder

printFilesOutsideSolutionDir() {
	local badFiles=0
	git diff --name-only origin/solution origin/master -- | while read line; do
		echo "$line" >> /tmp/log.log
		if [[ "$line" != solution* && "$line" != \"solution* ]]; then
			echo "$(pwd)/$line not in solution/ directory!"
			badFiles=$(( $badFiles + 1 ))
		fi
	done
	return $badFiles
}

checkFilesOutsideSolutionDir() {
	message=$(printFilesOutsideSolutionDir)
	if [[ "$?" != "0" ]]; then
		echo "Error while checking files outside solution directory:"
		echo "$message"
		echo "-------------hint--------------"
		echo "if you need add files to .gitignore,"
		echo "place .gitignore file into solution/ directory"
		return 1
	fi
	return 0
}

doStep "Проверка на отсутствие файлов вне solution/ папки" checkFilesOutsideSolutionDir

# step 5: check Make build
checkMakeBuild() {
	if [ ! -f ./Makefile ]; then
		echo "File solution/Makefile not found!"
		return 1
	fi

	message=$(sed 's/gcc/gcc $(CFLAGS)/' -i ./Makefile 2>/dev/stdout)
	if [ "$?" != "0" ]; then
		echo "$message"
		return 1
	fi

	message=$(CFLAGS="-Wextra -Wall -Wfloat-equal -pedantic" make 2>/dev/stdout)
	if [ "$?" != "0" ]; then
		echo "$message"
		return 1
	fi

	if [ ! -f ./main.out ]; then
		echo "Make build success but main.out file not found in solution/ directory!"
		return 1
	fi

	return 0
}
doStep "Сборка программы" checkMakeBuild


# send work to checking queue if all steps passed done
zmq_push() {
	m=$(</dev/stdin)
 	echo -e $(printf '\\x01\\x00\\x%02xx%00s' $((1 + ${#m})) "$m") > /dev/tcp/$1/$2
}

sendWorkToQueue() {
	local host="$CHECKING_QUEUE_UPDATER_HOST"
	local port="$CHECKING_QUEUE_UPDATER_PORT"
	echo "${workName}" | zmq_push "$host" "$port"
}
doStep "Отправка работы в очередь на финальную проверку" sendWorkToQueue
