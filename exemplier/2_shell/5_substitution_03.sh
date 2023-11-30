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

# This script demonstrates using command substitution to create a dynamically named tarball backup

echo "Define BACKUP as ./backup-\$(date +%Y-%m-%d).tar.gz"
BACKUP="./backup-$(date +%Y-%m-%d).tar.gz"
echo "BACKUP filename is set to $BACKUP"

SOURCE_DIR="$HOME/Desktop/os/courses/examples/shell"
if [ -d "$SOURCE_DIR" ]; then
    echo "Creating a tarball backup of $SOURCE_DIR"
    if tar -czf "${BACKUP}" "$SOURCE_DIR"; then
        echo "Backup successfully created at ${BACKUP}"
    else
        echo "Failed to create backup."
    fi
else
    echo "Source directory $SOURCE_DIR does not exist. Backup not created."
fi
