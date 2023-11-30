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

# A simple script to demonstrate the use of arrays in Bash

# Define an array of programming languages
languages=("C" "C++" "Java" "Scala" "Python")

# Iterate over the array and print each language
echo "List of programming languages:"
for language in "${languages[@]}"; do
    echo "Language: $language"
done

# Add a new language to the array
echo "Adding 'Rust' to the list of programming languages..."
languages+=("Rust")

# Print the updated array
echo "Updated list of programming languages:"
for language in "${languages[@]}"; do
    echo "Language: $language"
done

# Demonstrating other array operations
echo "The first language in the list is: ${language[0]}" # Accessing the first element
echo "The list of languages contains ${#languages[@]} elements." # Number of elements in the array
