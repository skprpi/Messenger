#!/bin/bash

check_stderr() {
    if [[ -n $1 ]]; then
        echo "Please, fix bugs first"
        exit 71
    fi
}

SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

app_stderr=$( cppcheck -q --enable=all --suppress=missingIncludeSystem --template='{id} {file}:{line} {message}' ${SCRIPTPATH}/app 2>&1 ) # capture stderr
test_stderr=$( cppcheck -q --enable=style,portability --suppress=missingIncludeSystem --template='{id} {file}:{line} {message}' ${SCRIPTPATH}/tests 2>&1 )

echo "$app_stderr"
echo "$test_stderr"

check_stderr ${app_stderr}
check_stderr $test_stderr

echo "OK, no stderror"
