#!/usr/bin/env bash

count=0

while [[ count -lt 30 ]]
do
	echo $count
	str=$(echo $count | python3  generate_string.py)
	one=$(echo $str | ./ft_ssl md5 -p)
	two=$(echo $str | md5 -p)
	# echo "-----"
	echo $one
	echo $two
	let count=$count+1
done
