#!/bin/bash

for FILE in medium_load_2kr-*#0.vec
do
	name=$(echo $FILE | sed 's/-#.*.vec//')
	scavetool export --type v -o $name.csv -F CSV-S -v -x precision=14 -x separator=semicolon -f "low_jobs_num_stat:vector" $name-*.vec
done


