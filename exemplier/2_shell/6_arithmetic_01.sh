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

#!/bin/bash

# A simple script to demonstrate arithmetic operations in Bash

# Using "let" for arithmetic operations
echo "Using let for arithmetic:"
let x=8*2+3
echo "Let x = 8 * 2 + 3, x = $x"

let y=5+x
echo "Let y = 5 + x, y = $y"

# Using "$((...))" for arithmetic expansion (recommended method)
echo "Using \$(()) for arithmetic expansion:"
echo "\$((8 * 2 + 3)) = $((8 * 2 + 3))"

# Demonstrating more complex operations
echo "More complex operations:"
echo "\$((x * y)) = $((x * y))"
echo "\$((x % y)) (modulo) = $((x % y))"

# Deprecated "$[]" syntax for arithmetic (should be avoided)
echo "Deprecated \$[] syntax for arithmetic (not recommended):"
VALUE=$[12+20]
echo "\$[12 + 20] = $VALUE"
echo "\$[2 * \$VALUE] = $[2 * $VALUE]"
echo "\$[\$VALUE / 2] = $[$VALUE / 2]"

# Note: The use of $[] for arithmetic is deprecated and should be avoided in favor of $((...))
