#!/bin/bash

SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

cd ${SCRIPTPATH}
chmod +x ./m2m_chat_service/build.sh && ./m2m_chat_service/build.sh
# I will add other services later
