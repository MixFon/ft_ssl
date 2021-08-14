#!/usr/bin/env bash

count=0

while [[ count -lt 30 ]]
do
	echo $count
	str=$(cat /dev/random | base64 | head -c $count )
	echo $str > temp
	one=$(./ft_ssl sha256 -r temp)
	two=$(shasum -a 256 temp)
	# echo "-----"
	echo $one
	echo $two
	let count=$count+1
done
rm temp
