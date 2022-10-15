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

# docker compose file
docker_compose="${script_path}/docker-compose.yml"

# env files - used in docker compose file, not in container
docker_compose_env="${script_path}/server.env"

# check files exist
check_file_exist "${docker_compose}"
check_file_exist "${docker_compose_env}"

# run docker compose
docker-compose --env-file ${docker_compose_env} -f ${docker_compose} up --force-recreate 
