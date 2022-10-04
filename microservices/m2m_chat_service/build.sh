#!/bin/bash

script_path="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd ${script_path}

rm -rf ./_build # TODO: fix (use sudo for local and don't use for docker compose)
mkdir _build

cd ./_build
cmake .. && make
