#!/usr/bin/env bash

count=0

while [[ count -lt 30 ]]
do
	echo $count
	str=$(echo $count | python3 generate_string.py)
	one=$(./ft_ssl md5 -r -s $str)
	two=$(md5 -r -s $str)
	# echo "-----"
	echo $one
	echo $two
	let count=$count+1
done