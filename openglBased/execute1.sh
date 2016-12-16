#!/bin/bash
for  i in `seq 1 2`
do
	echo "writing output to output_$i"
	./oorl_taxi 1 50 > output_$i
done