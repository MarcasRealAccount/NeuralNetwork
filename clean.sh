#!/bin/bash

#
# Created by MarcasRealAccount on 21. May. 2021
#

set +v

CLEAN_QUIET="false"
for var in "$@"
do
	if [ "$var" == "/q" ]; then
		CLEAN_QUIET="true"
	fi
done
CLEAN_PATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd $CLEAN_PATH

function deleteDir {
	if [ -d "$1" ]; then
		for d in $(find . -type d -name $1)
		do
			if [ "$CLEAN_QUIET" == "false" ]; then
				echo Deleting \"$d\"
			fi
			rm -r -f "$d" >/dev/null 2>&1
		done
	fi
}

function deleteFiles {
	for d in $(find . -name $1)
	do
		if [[ $d != *"armadillo/examples"* ]]; then
			if [ "$CLEAN_QUIET" == "false" ]; then
				echo Deleting \"$d\"
			fi
			rm -f "$d" >/dev/null 2>&1
		fi
	done
}

deleteDir .vs
deleteDir bin
deleteDir bin-int
deleteFiles *.sln
deleteFiles *.vcxproj
deleteFiles *.vcxproj.*
deleteFiles Makefile
deleteFiles *.make
if [ "$CLEAN_QUIET" == "false" ]; then
	echo Finished cleanup
fi
