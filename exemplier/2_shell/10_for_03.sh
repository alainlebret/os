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

# Another simple script to show how to use loops
# It displays extensions, files and the number of files in the current directory.
# This also can be done using:
#   ls | wc -l
# or in a more robust way:
#   find ./ -type f | wc -l
#
LIST=""
echo ${LIST}

for file in *
	do
		nom=${file%%.*}
		extension=${file##*.}
		echo "$extension"
		LIST="$LIST $file"
	done

echo "$LIST"
echo " "
nb="$( echo $LIST | wc -w ) files"
echo ${nb}
