#!/bin/sh

#
# ENSICAEN
# 6 Boulevard Maréchal Juin
# F-14050 Caen Cedex
#
# Unix System Programming Examples / Exemplier de programmation système Unix
# "Shell bash" / "Interpréteur de commandes bash"
#
# Copyright (C) 1995-2016 Alain Lebret (alain.lebret@ensicaen.fr)
#
# This work is licensed under a Creative Commons Attribution-ShareAlike 4.0
# International License. You should have received a copy of the License.
# If not, see https://creativecommons.org/licenses/by-sa/4.0/.
#

# A simple script that edits files $2,... which contain the word $1

if [ $# -le 1 ]
then
  echo "Usage: $0 word file ..." ; exit 1
else
  word=$1 # Store the word (first parameter $1)
  shift	# Shift parameters by one
  while [ $# -gt 0 ] # The new parameter ($1) is the first file
  do
    grep ${word} $1 > /dev/null
    if [ $? -eq 0 ] ; then # If grep has found the word
      emacs -nw $1 & # we edit the file with emacs
    fi
    shift # Shift parameters again
  done
fi   
