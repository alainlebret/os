#!/bin/bash

# A simple script to show how to use substitution in a more complex command

echo "Remove backup files on the desktop"
rm "$( find ~/Desktop/ -name "*.*~" )"
