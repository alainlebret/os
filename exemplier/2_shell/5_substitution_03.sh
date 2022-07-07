#!/bin/bash

# Another simple script to show how to use substitution in a more complex command

echo "Let BACKUP=./backup-\$(date +%d-%m-%y).tar.gz"
BACKUP=./backup-$(date +%d-%m-%y).tar.gz
echo "Then we can create automatically some tarball with a new date:"
tar -czf "${BACKUP}" "${HOME}/Desktop/os/courses/examples/shell"
