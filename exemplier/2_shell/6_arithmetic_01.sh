#!/bin/bash

# A simple script to show arithmetic operations using "let", "$(())" and "$[]"

echo "Let x = 8 * 2 + 3"
let x=8*2+3
echo "x = $x"

echo "Let y = 5 + x"
let y=5+x
echo "y = $y"

echo "Another way to calculate something can be done using: \$(())"
echo "\$((8*2+3)) = $((8*2+3))"

echo "Arithmetic operation can also be done using: \$[] (well, it is deprecated now!)"
echo "Let VALUE = \$[12+20]"
VALUE=$[12+20]
echo "\$[12+20] = $VALUE"
echo "\$[2*\$VALUE] = $[2*$VALUE]"
echo "\$[\$VALUE/2] = $[$VALUE/2]"
