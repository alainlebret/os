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
# This work is licensed under a Creative Commons Attribution-ShareAlike 4.0
# International License. You should have received a copy of the License.
# If not, see https://creativecommons.org/licenses/by-sa/4.0/.
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
