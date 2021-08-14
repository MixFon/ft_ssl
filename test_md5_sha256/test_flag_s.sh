#!/usr/bin/env bash

count=0

while [[ count -lt 30 ]]
do
	echo $count
	str=$(cat /dev/random | base64 | head -c $count )
	one=$(./ft_ssl md5 -s $str)
	two=$(md5 -s $str)
	echo $one
	echo $two
	let count=$count+1
done
