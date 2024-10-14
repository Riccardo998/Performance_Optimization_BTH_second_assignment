#!/bin/bash

# Enable recursive globbing
shopt -s globstar

# Loop through the different files
for i in 128 256 512 1024; do
    
	echo "Starting computation for  ${i}.data"
	pidstat -u -d -r -h -I 1 -e ./pearson/pearson pearson/data/${i}.data pearson/data/${i}_pearson.data > pidstat_output/pidstat_pearson_${i}.txt
	echo "Done with computation for  ${i}.data"

done

