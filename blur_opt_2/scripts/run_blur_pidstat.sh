#!/bin/bash

# Enable recursive globbing
shopt -s globstar

# Loop through images 1-4
for ((i = 1; i <= 4; i++)); do
    
	echo "Starting blurring im${i}.ppm"
	pidstat -u -d -r -h -I 1 -e ../blur 15 ../data/im${i}.ppm ../data/blurred_im${i}.ppm > ../performance_metrics/pidstat_blur_im${i}_opt2.txt
	echo "Done blurring im${i}.ppm"

done

