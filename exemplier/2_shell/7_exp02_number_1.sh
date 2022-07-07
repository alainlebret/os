#!/bin/bash

# A simple script to show how to test an expression

echo -n "Enter an integer value x such as: 1 < x < 10 : "
read -r x
if [ "$x" -lt 10 ]
then
  if [ "$x" -gt 1 ]
  then 
    echo "$x*$x=$((x*x))"
  else 
    echo "Bad choice!"
  fi
else
  echo "Bad choice!"
fi
