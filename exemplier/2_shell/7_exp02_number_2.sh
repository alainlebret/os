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

# A simple script to show how to test an expression

echo -n "Enter an integer value x such as: 1 < x < 10 : "
read x
if [ $x -gt 1 ] && [ $x -lt 10 ]
then 
  echo "$x*$x=$(($x*$x))"
else
  echo "Bad choice!"
fi

