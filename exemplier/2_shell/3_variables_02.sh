#!/bin/bash

# A simple script to show the difference between simple and double quotes

BAD="excellent"
UNIX="Unix is a $BAD system!"
echo $UNIX

echo ""

BAD='excellent'
WINDOWS='Windows is a $BAD system!'
echo $WINDOWS
