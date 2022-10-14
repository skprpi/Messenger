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

docker_compose_m2m_chat_service="${script_path}/docker-compose-m2m-chat-service.yml"
check_file_exist "${docker_compose_m2m_chat_service}"

docker-compose -f ${docker_compose_m2m_chat_service} down
