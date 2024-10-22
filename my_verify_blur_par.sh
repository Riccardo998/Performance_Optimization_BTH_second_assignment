#!/bin/bash
# Author: David Holmqvist <daae19@student.bth.se>

status=0
temp_dir=$(mktemp -d 2> /dev/null || mktemp -d -t 'mytmpdir')
red=$(tput setaf 1)
yellow=$(tput setaf 3)
reset=$(tput sgr0)

counter=0

for num_threads in 2 4 8 16 32
do
    for i in 1 2 3 4
        do
        ./blur_par/blur 15 "blur_par/data/im${i}.ppm" "$temp_dir/blur_im${i}_par.ppm" "${num_threads}"

        if ! cmp -s "blur_im${i}_seq.ppm" "$temp_dir/blur_im${i}_par.ppm"
        then
            echo "${red}Incongruent output data detected with image ${i} with ${num_threads} thread(s)"
            status=1
        fi

        rm "$temp_dir/blur_im${i}_par.ppm"
    done
done



exit $status
