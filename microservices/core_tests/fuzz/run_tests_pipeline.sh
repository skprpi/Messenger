#!/bin/bash

script_path="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd ${script_path}


# $1 - file path
check_file_exist () {
    if [ ! -f ${1} ]; then
        echo "Can't find $1" 
        exit 77
    fi
}

core_fuzz_tests="${script_path}/_build/core_fuzz_test"
check_file_exist "${core_unit_tests}"
${core_fuzz_tests} -runs=1000 -print_final_stats=1 -max_len=50
