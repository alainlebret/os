#!/bin/bash

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
