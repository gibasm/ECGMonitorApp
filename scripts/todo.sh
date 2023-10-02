#!/bin/bash

if [[ $# -ge 1 ]]; then

	echo "Usage ./todo.sh <project root directory>"
	echo "Desc: Lists every 'TODO' comment in the project, with a corresponding file name and line number"
	exit 1
fi

SRC_DIR=./

if [[ $# -eq 1 ]]; then
	SRC_DIR=$1

	if [[ ! -d $1 ]]; then
		echo "The first argument must be project's root directory"
		exit 2
	fi
fi 

grep -r -T --color=always --exclude-dir="$SRC_DIR"build --exclude-dir="$SRC_DIR"scripts -n -H -s TODO $SRC_DIR**
