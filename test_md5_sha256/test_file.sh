#!/usr/bin/env bash

count=0

./ft_ssl md5 *
echo "------"
md5 *

while [[ count -lt 30 ]]
do
	echo $count
	str=$(echo $count | python3  generate_string.py)
	echo $str > temp
	one=$(./ft_ssl md5 temp)
	two=$(md5 temp)
	# echo "-----"
	echo $one
	echo $two
	let count=$count+1
done
rm temp
