#!/bin/bash

# Another simple script to show how to use loops

let sum=0

for sum in 1 2 3 4 5 6
  do
    let "sum = $sum + $sum"
    echo "$sum"
  done	
