#!/bin/bash
# Author: David Holmqvist <daae19@student.bth.se>

status=0
temp_dir=$(mktemp -d 2> /dev/null || mktemp -d -t 'mytmpdir')
red=$(tput setaf 1)
reset=$(tput sgr0)

for size in 128 256 512 1024
    do
    ./pearson/pearson "pearson/data/$size.data" "$temp_dir/${size}_seq.data"
    ./pearson_opt/pearson "pearson_opt/data/$size.data" "$temp_dir/${size}_par.data"

    if ! cmp -s "$temp_dir/${size}_seq.data" "$temp_dir/${size}_par.data"
    then
        echo "${red}Incongruent output data detected with size $size"
        status=1
    fi

    rm "$temp_dir/${size}_seq.data" "$temp_dir/${size}_par.data"
done

exit $status
