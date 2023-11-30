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

# A script that says hello using the echo command and different string delimiters

# Double quotes allow variable expansion and command substitution
echo "Bonjour à vous !"

# Separate strings are concatenated with a space in between
echo "Bonjour" "à" "vous" "!"

# Single quotes prevent variable expansion and command substitution
echo 'Bonjour à vous !'

# Without quotes, the string is subject to word splitting and globbing
echo Bonjour à vous !

# Escaped double quotes are treated as part of the string
echo \"Bonjour à vous !\"

# Escaped single quotes are treated as part of the string
echo \'Bonjour à vous !\'
