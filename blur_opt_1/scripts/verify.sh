#!/bin/bash
# Author: David Holmqvist <daae19@student.bth.se>
 
# 1. The script is designed to test and compare the output of two different image processing programs: a sequential version (`blur`) and a parallel version (`blur_par`) that performs image blurring.
# 
# 2. Here's the breakdown of its functionality:
# 
# - Creates a temporary directory to store intermediate results
# - Sets up color coding for error messages (red text)
# - Runs two nested loops:
#   - Outer loop iterates through different thread counts (1, 2, 4, 8, 16, 32)
#   - Inner loop iterates through different input images (im1, im2, im3, im4)
# 
# 3. For each combination of thread count and image, it:
# - Runs the sequential blur program (`blur`) with blur radius 15
# - Runs the parallel blur program (`blur_par`) with the same parameters and specified thread count
# - Compares the output files using `cmp` to ensure they're identical
# - If the outputs differ, prints an error message in red
# - Cleans up temporary files after comparison
# 
# 4. The script acts as a verification tool to ensure that the parallel implementation produces exactly the same results as the sequential version across different thread counts.
# 
# The key purpose appears to be quality assurance - making sure the parallel implementation doesn't introduce any differences in the output compared to the sequential version, regardless of how many threads are used.
# 
# If any comparison fails, the script will exit with status code 1 (indicating error); otherwise, it exits with 0 (success).
# 
# Note that this script assumes:
# - The existence of programs named `blur` and `blur_par` in the current directory
# - Input images exist in a `data/` directory with `.ppm` format
# - The user has permission to create temporary directories and files

echo "NOTE: this script relies on the binaries blur, blur_par, threshold and threshold_par to exist"

status=0
temp_dir=$(mktemp -d 2> /dev/null || mktemp -d -t 'mytmpdir')
red=$(tput setaf 1)
reset=$(tput sgr0)

for thread in 1 2 4 8 16 32
do
    for image in im1 im2 im3 im4
    do
        ./blur 15 "data/$image.ppm" "$temp_dir/blur_${image}_seq.ppm"
        ./blur_par 15 "data/$image.ppm" "$temp_dir/blur_${image}_par.ppm" $thread

        if ! cmp -s "$temp_dir/blur_${image}_seq.ppm" "$temp_dir/blur_${image}_par.ppm"
        then
            echo "${red}Incongruent output data detected when blurring image $image.ppm with $thread thread(s)${reset}"
            status=1
        fi

        rm "$temp_dir/blur_${image}_seq.ppm" "$temp_dir/blur_${image}_par.ppm"
    done
done

exit $status