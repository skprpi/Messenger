#!/bin/bash

script_path="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd ${script_path}

source ./venv/bin/activate

out=$(which tox)
if [ -z "$out" ]; then
    echo "Installing tox..."
    pip install tox
fi

microservices_directory="${script_path}/../microservices"
export docker_compose_setup_script="${microservices_directory}/m2m_chat_service/run_docker.sh"

if [ ! -f ${docker_compose_setup_script} ]; then
    echo "Can't find docker_compose_setup_script" 
    exit 77
fi

tox -r
