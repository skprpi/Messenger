#!/bin/bash

script_path="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd ${script_path}

rm -rf ./_build
mkdir _build

cd ./_build
cmake .. && make
