#!/bin/bash

# Another simple script to show how to use loops
# It displays extensions, files and the number of files in the current directory.
# This also can be done using:
#   ls | wc -l
# or in a more robust way:
#   find ./ -type f | wc -l
#

LIST=""
echo "${LIST}"

for file in *
	do
		nom=${file%%.*}
		extension=${file##*.}
		echo "$extension"
		LIST="$LIST $file"
	done

echo "$LIST"
echo " "
nb="$( echo "$LIST" | wc -w ) files"
echo "${nb}"
