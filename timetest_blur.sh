#!/bin/bash

# Enable recursive globbing
shopt -s globstar

# Initialize the output file
printf "Execution time:\n" > "blur_time.txt"

# Loop through images 1-4
for ((i = 1; i <= 4; i++)); do
    timetaken=$( { /usr/bin/time -f %e ./blur/blur 15 ./blur/data/im${i}.ppm ./blur/data/blurred_im${i}.ppm; } 2>&1 ) # radius = 15
    printf "%s: %6s s\n" "im${i}.ppm" "$timetaken" >> "blur_time.txt"
done