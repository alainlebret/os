#!/bin/bash

# This scripts removes temporary files using a temporary trash directory

mkdir .trash
mv *~ .trash
rm -rf .trash
echo "Temporary files are removed!"
