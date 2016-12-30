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
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
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
