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

# A simple script to show how to use "while" loops to make a menu.

clear ; boucle=y
while [ ${boucle} = y ]
  do
    echo "Menu";  echo "===="
    echo "D: Display the date"
    echo "U: Display connected users"
    echo "W: Display the path of the working directory"
    echo "Q: Quit"
    echo
    read -s choix # silent mode with no echo
    case $choix in
      D|d) date ;;
      U|u) who ;;
      W|w) pwd ;;
      Q|q) boucle=n ;;
      *) echo "Not a valid choice!" ;;
    esac
    echo
  done 
