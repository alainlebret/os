#!/bin/bash

# A simple script to show how to use "case"

echo -n "Enter an integer value x such as: 1 < x < 10 : "
read -r x

case $x in
  2|3|4|5|6|7|8|9) echo "x = $x";;
  *) echo "Bad choice!";;
esac

