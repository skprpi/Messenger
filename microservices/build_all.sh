#!/bin/bash

script_path="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

cd ${script_path}
chmod +x ./m2m_chat_service/build.sh && ./m2m_chat_service/build.sh
# I will add other services later
