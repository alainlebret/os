#!/bin/bash
#
# A script that runs a command every 1 minute

while true; do
    echo "Sending SIGUSR1 to " $1
    kill -s SIGUSR1 $1
    echo "Sending SIGUSR2 to " $2
    kill -s SIGUSR2 $2
    sleep 1m
done

