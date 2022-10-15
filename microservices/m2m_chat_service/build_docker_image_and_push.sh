#!/bin/bash

# Run this script to build image for docker compose

script_path="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd ${script_path}

# $1 - command
# $2 - command name
process_step () {
    echo -n "$2 : " # print without switching line
    stderr="$( ${1} 2>&1 > /dev/null )"
    if [ ! -z "$stderr" ]; then
        echo "Failed"
        echo "Error: $stderr"
        exit 77
    fi
    echo "Done"
}


process_step "./build.sh" "Build project"
process_step "docker build --tag messenger:latest ." "Build docker image"
process_step "docker image tag messenger mfatin/messenger" "Make tag for docker image"
process_step "sudo docker image push mfatin/messenger" "Push docker image"
