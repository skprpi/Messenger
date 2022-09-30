#!/bin/bash

SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

cd ${SCRIPTPATH}

file_list="$(find ${SCRIPTPATH} -type f -name *.cpp)"

clang-format --dry-run -i -style=file:.clang-format ${file_list}
