#!/bin/bash

# A simple script to read some inputs.

echo n "Enter the name(s) of the file(s) to remove: "
read -r filename
rm -i "$filename"
echo "File(s) have been removed."
