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

# A simple script to show the command substitution

# Using backticks for command substitution
COMMAND1=`ls`
echo "Listing files using backticks:"
echo "$COMMAND1"

echo ""

# It is recommended to use $(...) for command substitution due to better readability and nesting ability
echo "It can also be done using $(...):"
COMMAND2=$(ls)
echo "$COMMAND2"

# Example of nested command substitution
echo ""
echo "Nested command substitution example:"
COMMAND3=$(echo "Today is $(date +%A)")
echo "$COMMAND3"

# Checking if the command was successful
if ! COMMAND4=$(ls non_existent_directory); then
    echo "Failed to execute command"
fi
