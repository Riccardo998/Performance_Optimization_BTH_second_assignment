#!/bin/bash

shopt -s globstar
# change "../testfiles/*"" to the relative path to this script of the directory
# where your files to compress are
for file in ../testfiles/*; do
	if [[ $file != *.gz ]]; then
		filename=`basename $file`
		printf "$filename I/O:\n\n" > ${filename}_strace.txt
		for ((i = 1; i <= 16; i *= 2)) do
			plural=$([ $i == 1 ] && echo "d" || echo "ds")
			printf "\t%-2d threa%s:\n" $i $plural >> ${filename}_strace.txt

			# change "../pigz/pigz" to the relative path to this executable of pigz
			strace -ff -e read,write ../pigz/pigz -k -p$i -cf $file > ../output/${filename}_${i}.gz 2> ${filename}_${i}

			reads=`pcre2grep -o1 '.*read.*\s=\s([1-9][0-9]*)$' ${filename}_${i}`
			nreads=`echo $reads | wc -w`
			reads=($reads)
			reads=$((${reads[@]/%/+}0 | bc))
			printf "\t\t%-10d bytes read in %d calls to read()\n" $reads $nreads >> ${filename}_strace.txt
			
			filebytes=`wc -c < $file`
			printf "\t\t%-10d actual bytes in file\n" $filebytes >> ${filename}_strace.txt

			writes=`pcre2grep -o1 '.*write.*\s=\s([1-9][0-9]*)$' ${filename}_${i}`
			nwrites=`echo $writes | wc -w`
			writes=($writes)
			writes=$((${writes[@]/%/+}0 | bc))
			printf "\t\t%-10d bytes written in %d calls to write()\n" $writes $nwrites >> ${filename}_strace.txt
			
			compressedbytes=`wc -c < ../output/${filename}_${i}.gz`
			printf "\t\t%-10d actual bytes in compressed file\n" $compressedbytes >> ${filename}_strace.txt

			printf "\n" >> ${filename}_strace.txt
			#rm ${filename}_${i}
		done
	fi
done

