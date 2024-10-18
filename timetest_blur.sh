#!/bin/bash

# Enable recursive globbing
shopt -s globstar

path="./blur_opt"
output_file="blur_time_opt1.txt"

printf "Execution time:\n" > "${output_file}"

# Loop through images 1-4
for i in {1..4}; do
    image="im${i}.ppm"
    blurred_image="blurred_im${i}.ppm"
    
    printf "%s:\n" "${image}" >> "${output_file}"
    
    sum=0

    for j in {1..5}; do
        TIMEFORMAT='%3R'

        time_output=$( { time ${path}/blur 15 "${path}/data/${image}" "${path}/data/${blurred_image}" ; } 2>&1 )
        echo "Time output: ${time_output}"

        printf "%ss\n" "${time_output}" >> "${output_file}"

        sum=$(echo "$sum + $time_output" | bc) #  bc (basic calculator)
    done

    average=$(echo "scale=3; $sum / 5" | bc)
    printf "Average: %ss\n\n" "$average" >> "${output_file}"
done