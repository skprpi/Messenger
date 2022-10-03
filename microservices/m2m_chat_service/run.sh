#!/bin/bash

script_path="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd ${script_path}


# setup

# timeout limiter
export SERVER__TIMEOUT_LIMITER__EXPIRATION_MILLISECONDS=1000000000 # 10 seconds

# network
export SERVER__PORT=2021 # service port > 1024 TODO: add handling bad port
export SERVER__IPv4="0.0.0.0"


./_build/m2m_chat_service
