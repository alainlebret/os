#!/bin/bash

# A simple script that edits files $2,... which contain the word $1

if [ $# -le 1 ]
then
  echo "Usage: $0 word file ..." ; exit 1
else
  word=$1 # Store the word (first parameter $1)
  shift	# Shift parameters by one
  while [ $# -gt 0 ] # The new parameter ($1) is the first file
  do
    grep "${word}" "$1" > /dev/null
    if [ $? -eq 0 ] ; then # If grep has found the word
      vim -nw "$1" & # we edit the file with vim
    fi
    shift # Shift parameters again
  done
fi   
