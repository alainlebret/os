#!/bin/bash

# A simple script to show variables and their contents

MESSAGE="Bonjour"
message="bonjour"

echo "MESSAGE: " + MESSAGE
echo "message: " + message
echo "$MESSAGE: " + ${MESSAGE}
echo "$message: " + ${message}
echo "\$MESSAGE: " + ${MESSAGE}
echo "\$message: " + ${message}
