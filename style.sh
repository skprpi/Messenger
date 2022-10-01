#!/bin/bash

script_path="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
style_dir="${script_path}/microservices"
cd ${script_path}

# TODO style test and here script add to env regex and base_dir
files_path="$(find ${style_dir} -regex '.*\.\(cpp\|h\)' | grep -v _build/ | grep -v build/)" # -v exclude result
clang-format -i -style=file:.clang-format ${files_path}
