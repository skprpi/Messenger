#!/bin/bash

script_path="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd ${script_path}

# setup environment
server_env="${script_path}/server.env"
if [ ! -f ${server_env} ]; then
    echo "Can't find server_env" 
    exit 77
fi
export $(grep -v '^#' ${server_env} | xargs) # export all variables and ignore comments

# run service
./_build/m2m_chat_service
