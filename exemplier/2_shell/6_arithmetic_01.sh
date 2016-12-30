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

# A simple script to show arithmetic operations using "let", "$(())" and "$[]"

echo "Let x = 8 * 2 + 3"
let x=8*2+3
echo "x = $x"

echo "Let y = 5 + x"
let y=5+x
echo "y = $y"

echo "Another way to calculate something can be done using: \$(())"
echo "\$((8*2+3)) = $((8*2+3))"

echo "Arithmetic operation can also be done using: \$[]"
echo "Let VALUE = \$[12+20]"
VALUE=$[12+20]
echo "\$[12+20] = $VALUE"
echo "\$[2*\$VALUE] = $[2*$VALUE]"
echo "\$[\$VALUE/2] = $[$VALUE/2]"
