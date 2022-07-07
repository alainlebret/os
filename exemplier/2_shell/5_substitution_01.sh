#!/bin/bash

# A simple script to show the command substitution

COMMAND1=`ls`
echo "$COMMAND1"

echo "It can also be done using:"

COMMAND2=$(ls)
echo "$COMMAND2"

