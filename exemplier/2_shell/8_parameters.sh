#!/bin/bash

# A simple script to show how to manipulate parameters

if [ $# -eq 1 ]
then
  if [ ! -d .mytrash ]
  then
    mkdir .mytrash
  fi
  mv -t "$1" .mytrash
else
  echo "Usage: $0 filename"
  exit 1
fi

