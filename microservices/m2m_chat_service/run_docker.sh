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

export docker_compose_m2m_chat_service="${script_path}/docker-compose-m2m-chat-service.yml"
export docker_compose_m2m_chat_service_env="${script_path}/server.env"


check_file_exist "$docker_compose_m2m_chat_service"
check_file_exist "$docker_compose_m2m_chat_service_env"

# env-file to use it in docker-compose file 
docker-compose --env-file ${docker_compose_m2m_chat_service_env} -f ${docker_compose_m2m_chat_service} up # --force-recreate
