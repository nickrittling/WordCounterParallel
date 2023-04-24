#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 inputfile"
  exit 1
fi

inputfile="$1"
output1="output1.txt"
output2="output2.txt"
output3="output3.txt"
output4="output4.txt"

# Remove all special characters from input file
tr -cd '[:alnum:][:space:]\n' < "$inputfile" > "$output1"

# Convert all text to uppercase
tr '[:lower:]' '[:upper:]' < "$output1" > "$output2"

# Replace spaces with newlines
tr ' ' '\n' < "$output2" > "$output3"

# Remove extra newlines
tr -s '\n' < "$output3" > "$output4"

