#!/bin/bash

# Enable recursive globbing
shopt -s globstar

for i in 128 256 512 1024;do
    awk '
        NR==1 {
            # Print the header for the CSV file
            print "%CPU;%MEM;MB_rd/s;MB_wr/s"
        }

        /^[0-9]/ {
            # Extract fields using regular expression
            match($0, /([0-9]{2}:[0-9]{2}:[0-9]{2} [APM]{2})\s+([0-9]+)\s+([0-9]+)\s+([0-9]+\.[0-9]{2})\s+([0-9]+\.[0-9]{2})\s+([0-9]+\.[0-9]{2})\s+([0-9]+\.[0-9]{2})\s+([0-9]+\.[0-9]{2})\s+([0-9]+)\s+([0-9]+\.[0-9]{2})\s+([0-9]+\.[0-9]{2})\s+([0-9]+)\s+([0-9]+)\s+([0-9]+\.[0-9]{2})\s+([0-9]+\.[0-9]{2})\s+([0-9]+\.[0-9]{2})\s+([0-9]+\.[0-9]{2})\s+([0-9]+)\s+([A-Za-z0-9_\/]+)/, arr)

            percent_CPU = arr[8]
            percent_MEM = arr[14]
            kB_rd_s_value = arr[15]
            kB_wr_s_value = arr[16]

            # Convert RSS , kB_rd_s_value and kB_wr_s_value to MB/s 
            kB_rd_s_value = kB_rd_s_value / 1024
            kB_wr_s_value = kB_wr_s_value / 1024
            
            # Print the extracted %CPU, %MEM, kB_rd/s, and kB_wr/s fields to a CSV file
            printf "%s;%s;%s;%s\n", percent_CPU, percent_MEM, kB_rd_s_value, kB_wr_s_value

        }' ../performance_metrics/pidstat_pearson_${i}_opt_1.txt > ../performance_metrics/pidstat_pearson_${i}_opt_1.csv
done
