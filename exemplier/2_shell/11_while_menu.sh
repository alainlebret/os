#!/bin/bash

#
# ENSICAEN
# 6 Boulevard Maréchal Juin
# F-14050 Caen Cedex
#
# Unix System Programming Examples / Exemplier de programmation système Unix
# "Shell bash" / "Interpréteur de commandes bash"
#
# Copyright (C) 1995-2023 Alain Lebret (alain.lebret@ensicaen.fr)
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

# A script to demonstrate a simple interactive menu using a while loop

clear
continue_loop="y"
while [ "$continue_loop" = "y" ]; do
    echo "Menu"
    echo "===="
    echo "D: Display the date"
    echo "U: Display connected users"
    echo "W: Display the path of the working directory"
    echo "Q: Quit"
    echo

    # Read user choice in silent mode
    read -s -r choix
    echo

    # Process user choice
    case "$choix" in
        [Dd]) date ;;
        [Uu]) who ;;
        [Ww]) pwd ;;
        [Qq]) continue_loop="n" ;;
        *) echo "Not a valid choice! Try again." ;;
    esac

    echo
    read -p "Press any key to continue..." -n 1 -s
    clear
done 

echo "Exiting the menu. Goodbye!"
