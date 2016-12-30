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
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
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
