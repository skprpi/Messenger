#!/bin/bash

script_path="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd ${script_path}

m2m_chat_service_docker_compose="${script_path}/microservices/m2m_chat_service/docker-compose-m2m-chat-service.yml"
docker_compose_m2m_chat_service_env="${script_path}/microservices/m2m_chat_service/server.env"
base_docker_compose="${script_path}/docker-compose.yml"

if [ ! -f ${docker_compose_m2m_chat_service_env} ]; then
    echo "Can't find docker_compose_m2m_chat_service_env" 
    exit 77
fi
if [ ! -f ${m2m_chat_service_docker_compose} ]; then
    echo "Can't find m2m_chat_service_docker_compose" 
    exit 77
fi
if [ ! -f ${base_docker_compose} ]; then
    echo "Can't find base_docker_compose" 
    exit 77
fi

docker-compose --env-file ${docker_compose_m2m_chat_service_env} -f ${m2m_chat_service_docker_compose} -f ${base_docker_compose} up --force-recreate
