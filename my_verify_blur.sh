#!/bin/bash
# Author: David Holmqvist <daae19@student.bth.se>

status=0
temp_dir=$(mktemp -d 2> /dev/null || mktemp -d -t 'mytmpdir')
red=$(tput setaf 1)
reset=$(tput sgr0)

for image in im1 im2 im3 im4
do
    ./blur_opt/blur 15 "blur_opt/data/$image.ppm" "$temp_dir/blur_${image}_par.ppm"

    if ! cmp -s "blur_${image}_seq.ppm" "$temp_dir/blur_${image}_par.ppm"
    then
        echo "${red}Incongruent output data detected when blurring image $image.ppm ${reset}"
        status=1
    fi
    rm "$temp_dir/blur_${image}_par.ppm"
done

exit $status
