#!/bin/bash

sudo docker build --tag messenger:latest .
sudo docker image tag messenger mfatin/messenger
sudo docker image push mfatin/messenger
