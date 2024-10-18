#!/bin/bash

# Enable recursive globbing
shopt -s globstar

path="./pearson_opt"
output_file="pearson_time_opt.txt"

printf "Execution time:\n" > "${output_file}"

for i in 128 256 512 1024; do
    input="${i}.data"
    output="pearson_${i}.data"
    
    printf "%s:\n" "${input}" >> "${output_file}"
    
    sum=0

    for j in {1..5}; do
        TIMEFORMAT='%3R'

        time_output=$( { time ${path}/pearson "${path}/data/${input}" "${path}/data/${output}" ; } 2>&1 )
        echo "Time output: ${time_output}"

        printf "%ss\n" "${time_output}" >> "${output_file}"

        sum=$(echo "$sum + $time_output" | bc) #  bc (basic calculator)
    done

    average=$(echo "scale=3; $sum / 5" | bc)
    printf "Average: %ss\n\n" "$average" >> "${output_file}"
done