#!/bin/bash
#
# This Bash script sends SIGUSR1 to the process with ID $1 and SIGUSR2
# to the process with ID $2, repeating this action every minute 
# indefinitely.

while true; do
    echo "Sending SIGUSR1 to " $1
    kill -s SIGUSR1 $1
    echo "Sending SIGUSR2 to " $2
    kill -s SIGUSR2 $2
    sleep 1m
done

