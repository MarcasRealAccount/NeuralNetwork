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

function deleteDir {
	if [ "$CLEAN_QUIET" == "false" ]; then
		echo Deleting \"$1\"
	fi
	rm -r -f "$1" >/dev/null 2>&1
}

function deleteFile {
	if [ "$CLEAN_QUIET" == "false" ]; then
		echo Deleting \"$1\"
	fi
	rm -f "$1" >/dev/null 2>&1
}

CLEAN_PATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
deleteDir $CLEAN_PATH/.vs/
deleteDir $CLEAN_PATH/bin/
deleteDir $CLEAN_PATH/bin-int/
deleteFile $CLEAN_PATH/NeuralNetwork.sln
deleteFile $CLEAN_PATH/NeuralNetwork.vcxproj
deleteFile $CLEAN_PATH/NeuralNetwork.vcxproj.filter
deleteFile $CLEAN_PATH/NeuralNetwork.vcxproj.user
deleteFile $CLEAN_PATH/Makefile
deleteFile $CLEAN_PATH/NeuralNetwork.make
if [ "$CLEAN_QUIET" == "false" ]; then
	echo Finished cleanup
fi
