#!/usr/bin/env bash

count=1

YELLOW="\033[1;33m"
GREEN="\033[1;32m"
RED="\033[1;31m"
WHITE="\033[1;37m"
RESET="\033[0;0m"
CLEAR_SCREEN="\033[2J"
CLEAR_LINE="\033[2K\c"

while [[ count -lt 100 ]]
do
	echo $count
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(cat /dev/random | base64 | head -c $count )
	one=$(./ft_ssl des $key $str)
	two=$(echo -n "$str" | openssl dgst -md5 -hmac "$key")
	if [ "$one" = "$two" ]
	then
		echo -e "\r" $CLEAR_LINE
		echo -e $GREEN "Yes" $RESET
	else
		echo -e "   " $CLEAR_LINE
		echo -e $RED "No" $RESET
		echo "$key"
		echo "$str"
		echo $one
		echo $two
	fi
	# echo "-----"
	let count=$count+1
done
