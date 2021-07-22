#!/usr/bin/env bash

count=0

while [[ count -lt 30 ]]
do
	echo $count
	str=$(echo $count | python3  generate_string.py)
	one=$(echo $str | ./ft_ssl sha256)
	two=$(echo $str | shasum -a 256)
	# echo "-----"
	echo $one
	echo $two
	let count=$count+1
done
