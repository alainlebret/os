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

# A script to demonstrate parameter manipulation by moving a file to a trash directory

if [ $# -eq 1 ]; then
  file_to_move="$1"

  # Check if the file exists
  if [ ! -f "$file_to_move" ]; then
    echo "Error: File '$file_to_move' not found."
    exit 1
  fi

  # Create the trash directory if it doesn't exist
  if [ ! -d trash ]; then
    mkdir trash || { echo "Failed to create trash directory"; exit 1; }
  fi

  # Move the file to the trash directory with confirmation
  mv -i -v "$file_to_move" trash
else
  echo "Usage: $0 <filename>"
  echo "Example: $0 document.txt"
  exit 1
fi
