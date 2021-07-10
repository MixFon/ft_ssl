#!/usr/bin/env bash

count=0

while [[ count -lt 10 ]]
do
	echo $count
	str=$(echo $count | python3 test_one.py)
	one=$(./ft_ssl md5 -s $str)
	two=$(md5 -q -s $str)
	# echo "-----"
	# echo $two
	# echo $one
	if [ "$one" = "$two" ]
	then
		echo "Yes"
	else
		echo "No"
		echo $str
	fi
	let count=$count+1
done
