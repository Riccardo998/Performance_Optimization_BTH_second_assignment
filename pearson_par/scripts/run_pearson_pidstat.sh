#!/bin/bash

# Enable recursive globbing
shopt -s globstar

# Loop through the different files
for i in 128 256 512 1024; do
    for t in 2 4 8 16 32; do
		echo "Starting computation for ${i}.data"
		pidstat -u -d -r -h -I 1 -e ../pearson ../data/${i}.data ../data/${i}_pearson.data ${t} > ../performance_metrics/pidstat_pearson_${i}_${t}.txt
		echo "Done with computation for ${i}.data"
	done
done

