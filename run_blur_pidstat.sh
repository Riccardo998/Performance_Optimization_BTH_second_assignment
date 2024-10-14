#!/bin/bash

# Enable recursive globbing
shopt -s globstar

# Loop through images 1-4
for ((i = 1; i <= 4; i++)); do
    
	echo "Starting blurring im${i}.ppm"
	pidstat -u -d -r -h -I 1 -e ./blur/blur 15 blur/data/im${i}.ppm blur/data/blurred_im${i}.ppm > pidstat_output/pidstat_blur_im${i}_15.txt
	echo "Done blurring im${i}.ppm"

done

