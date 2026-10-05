#!/bin/bash
#
# This Bash script sends SIGUSR1 to the process with ID $1 and SIGUSR2
# to the process with ID $2, repeating this action every minute 
# indefinitely.

if [ $# -ne 2 ]; then
    echo "Usage: $0 PID1 PID2   (PIDs shown by the two moving_window windows)" >&2
    exit 1
fi

# Stops as soon as one of the windows is closed (kill fails)
while true; do
    echo "Sending SIGUSR1 to $1"
    kill -s USR1 "$1" || exit 1
    echo "Sending SIGUSR2 to $2"
    kill -s USR2 "$2" || exit 1
    sleep 60
done

