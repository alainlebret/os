#!/bin/bash

#
# ENSICAEN
# 6 Boulevard Maréchal Juin
# F-14050 Caen Cedex
#
# Unix System Programming Examples / Exemplier de programmation système Unix
# "Shell bash" / "Interpréteur de commandes bash"
#
# Copyright (C) 1995-2023 Alain Lebret (alain.lebret@ensicaen.fr)
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

# Script to open files containing a specific word in Vim

if [ $# -le 1 ]; then
  echo "Usage: $0 <word> <file1> [<file2> ...]"
  exit 1
fi

word=$1
shift

echo "Searching for files containing the word '$word'..."
while [ $# -gt 0 ]; do
  if [ -f "$1" ] && [ -r "$1" ] && grep -q "${word}" "$1"; then
    vim "$1"
  else
    echo "Word '$word' not found or file '$1' cannot be read."
  fi
  shift
done
