#!/bin/bash

# Enable recursive globbing
shopt -s globstar

# printf "Execution time:\n" > "${output_file}"

for i in 128 256 512 1024; do
    for t in 2 4 8 16 32; do
        output_file="../performance_metrics/pearson_time_par_${i}_${t}.txt"
        input="${i}.data"
        output="pearson_${i}.data"
        
        echo "${i}.data - ${t} thread(s)"
        
        printf "%s:\n" "${input}" >> "${output_file}"
        
        sum=0

        for j in {1..5}; do
            TIMEFORMAT='%3R'

            time_output=$( { time ../pearson "../data/${input}" "../data/${output}" ${t} ; } 2>&1 )
            echo "Time output: ${time_output}"

            printf "%ss\n" "${time_output}" >> "${output_file}"

            sum=$(echo "$sum + $time_output" | bc) #  bc (basic calculator)
        done

        average=$(echo "scale=3; $sum / 5" | bc)
        printf "Average: %ss\n\n" "$average" >> "${output_file}"
    done
done