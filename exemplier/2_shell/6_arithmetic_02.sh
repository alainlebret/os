#!/bin/bash

# Another simple script to show some arithmetic operations

echo -n "Enter the first number: "; read -r x
echo -n "Enter the second number: "; read -r y

sum=$((x + y))
difference=$((x - y))
product=$((x * y))
ratio=$((x / y))
rest=$((x % y))

# Displaying results
echo "Sum: $sum"
echo "Difference: $difference"
echo "Product: $product"
echo "Ratio: $ratio"
echo "Rest: $rest"

