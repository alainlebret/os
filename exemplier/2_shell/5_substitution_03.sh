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

# Another simple script to show how to use substitution in a more complex command

echo "Let BACKUP=./backup-\$(date +%d-%m-%y).tar.gz"
BACKUP=./backup-$(date +%d-%m-%y).tar.gz
echo "Then we can create automatically some tarball with a new date:"
tar -czf ${BACKUP} $HOME/Desktop/os/courses/examples/shell
