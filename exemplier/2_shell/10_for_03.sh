#!/bin/bash

#
# ENSICAEN
# 6 Boulevard Maréchal Juin
# F-14050 Caen Cedex
#
# Unix System Programming Examples / Exemplier de programmation système Unix
# "Shell bash" / "Interpréteur de commandes bash"
#
# Copyright (C) 1995-2026 Alain Lebret (alain.lebret@ensicaen.fr)
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

# A script to display file extensions, list files, and count the number of files in the current directory
# It displays extensions, files and the number of files in the current directory.
# This also can be done using:
#   ls | wc -l
# or in a more robust way:
#   find ./ -type f | wc -l
#
count=0
LIST=""

for file in *; do
    if [ -f "$file" ]; then  # Check if it's a regular file
        name=${file%%.*}       # before the first dot
        extension=${file##*.}  # after the last dot
        echo "$name -> $extension"
        LIST="$LIST $file"
        count=$((count + 1))   # counted here: a name may contain spaces
    fi
done

echo "List of files: $LIST"
echo
echo "Number of files: $count"
