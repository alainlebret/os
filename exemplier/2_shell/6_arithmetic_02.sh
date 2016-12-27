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

# Another simple script to show some arithmetic operations

echo -n "Enter the first number: "; read x
echo -n "Enter the second number: "; read y

sum=$(($x + $y))
difference=$(($x - $y))
product=$(($x * $y))
ratio=$(($x / $y))
rest=$(($x % $y))

# Displaying results
echo "Sum: $sum"
echo "Difference: $difference"
echo "Product: $product"
echo "Ratio: $ratio"
echo "Rest: $rest"

