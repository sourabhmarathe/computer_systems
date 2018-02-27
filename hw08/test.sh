#!/bin/bash
# Initial Author: Jason Booth
# Modified by: Sourabh Marathe
t1_start=1
t1_count=1600000
t2_start=1000000000000000
t2_count=150

for ii in 1 4 16
do
    printf "test 1.$(($ii+1)), threads: $(($ii)), start: $t1_start, count: $t1_count"
    time (./main $((2**$ii)) $t1_start $t1_count 1>> results.txt 2>> errors.txt)
	sleep 1
done


for jj in 1 4 16
do
    printf "\n"
    printf "test 2.$(($jj)), threads: $(($jj)), start: $t2_start, count: $t2_count"
    time (./main $(($jj)) $t2_start $t2_count 1>> results.txt 2>> errors.txt)
	sleep 1
done

if [ -s ./errors.txt ]; then
    echo "ERROR: error in computation"
fi
