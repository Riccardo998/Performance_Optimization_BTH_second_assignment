#!/bin/bash

# Enable recursive globbing
shopt -s globstar

# Initialize the output file
printf "Execution time:\n" > "pearson_time.txt"

# Loop through the different files
for i in 128 256 512 1024; do
    timetaken=$( { /usr/bin/time -f %e ./pearson/pearson ./pearson/data/${i}.data ./pearson/data/${i}_pearson.data; } 2>&1 )
    printf "%s: %6s s\n" "${i}.data" "$timetaken" >> "pearson_time.txt"
done