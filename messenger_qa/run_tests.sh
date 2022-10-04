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
export m2m_chat_service_directory="${microservices_directory}/m2m_chat_service"
export docker_compose_m2m_chat_service="${microservices_directory}/m2m_chat_service/docker-compose-m2m-chat-service.yml"
export m2m_chat_service_docker_compose_env="${microservices_directory}/m2m_chat_service/server.env"

if [ ! -f ${docker_compose_m2m_chat_service} ]; then
    echo "Can't find docker_compose_m2m_chat_service" 
    exit 77
fi

tox -r
