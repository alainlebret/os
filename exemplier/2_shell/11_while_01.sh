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

# A simple script to show how to use "while" loops.

echo -n "Enter a number: "; read number

sum=0
i=1

while [ $i -le $number ]
  do
    let "sum=$sum+$i"
    i=$i+1
  done
echo "Sum from 1 to $number is equals to: $sum"

