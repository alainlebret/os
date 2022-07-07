#!/bin/bash

# A simple script to show how to test an expression using if

echo -n "Enter your username: "
read -r username
if [ "$username" = "$USER" ]
then 
  echo "Hello $username. How do you feel today?"
else 
  echo "You are not $USER! Who are you?"
fi

