#!/bin/bash

# A simple script to show how to use "while" loops.

echo -n "Enter a number: "; read -r number

sum=0
i=1

while [ "$i" -le "$number" ]
  do
    let "sum=$sum+$i"
    i=$i+1
  done
echo "Sum from 1 to $number is equals to: $sum"

