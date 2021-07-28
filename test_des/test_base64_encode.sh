#!/usr/bin/env bash

count=0
count_ok=0
count_error=0

len=100

YELLOW="\033[1;33m"
GREEN="\033[1;32m"
RED="\033[1;31m"
WHITE="\033[1;37m"
RESET="\033[0;0m"
CLEAR_SCREEN="\033[2J"
CLEAR_LINE="\033[2K\c"

echo "$YELLOW Test encoding stdin $WHITE"
while [[ count -lt len ]]
do
	str=$(echo $count | python3  generate_string.py)
	one=$(echo $str | ./ft_ssl base64 -e)
	two=$(echo $str | openssl base64 -A)
	# echo "-----"
	if [ "$one" = "$two" ]
	then
		echo "   " $CLEAR_LINE
		echo "$GREEN $count OK $RESET\r\c"
		let count_ok=$count_ok+1
	else
		echo $CLEAR_LINE
		echo "$RED $count KO $RESET" ; echo "## string: " ; echo $str
		echo $one
		echo $two
		let count_error=$count_error+1
	fi
	let count=$count+1
done
echo $CLEAR_LINE
echo "$GREEN \t$count_ok OK $WHITE"
echo "$RED \t$count_error KO $WHITE"

#----------------------------------------------------------------------
count=0
count_ok=0
count_error=0
echo "$YELLOW Test encoding file $WHITE"
while [[ count -lt len ]]
do
	str=$(echo $count | python3  generate_string.py)
	echo $str > temp
	one=$(./ft_ssl base64 -e -i temp)
	two=$(openssl base64 -A -in temp)
	# echo "-----"
	if [ "$one" = "$two" ]
	then
		echo "   " $CLEAR_LINE
		echo "$GREEN $count OK $RESET\r\c"
		let count_ok=$count_ok+1
	else
		echo $CLEAR_LINE
		echo "$RED $count KO $RESET" ; echo "## string: " ; echo $str
		let count_error=$count_error+1
	fi
	let count=$count+1
done
rm -rf temp
echo $CLEAR_LINE
echo "$GREEN \t$count_ok OK $WHITE"
echo "$RED \t$count_error KO $WHITE"

#----------------------------------------------------------------------
echo "$YELLOW Test write ot file $WHITE"
count=0
count_ok=0
count_error=0
while [[ count -lt len ]]
do
	str=$(echo $count | python3  generate_string.py)
	echo $str | ./ft_ssl base64 -e -o file_1
	echo $str | openssl base64 -A -out file_2
	# echo "-----"
	result=$(diff file_1 file_2)
	if [ "$result" = "" ]
	then
		echo "   " $CLEAR_LINE
		echo "$GREEN $count OK $RESET\r\c"
		let count_ok=$count_ok+1
	else
		echo $CLEAR_LINE
		echo $result
		echo "$RED $count KO $RESET" 
		echo "## string: " ; echo $str
		let count_error=$count_error+1
	fi

	let count=$count+1
done
rm -rf file_1 file_2
echo $CLEAR_LINE
echo "$GREEN \t$count_ok OK $WHITE"
echo "$RED \t$count_error KO $WHITE"

#----------------------------------------------------------------------
echo "$YELLOW Test decoding $WHITE"
count=0
count_ok=0
count_error=0
while [[ count -lt len ]]
do
	str=$(echo $count | python3  generate_string.py)
	chipher=$(echo $str | base64)
	one=$(echo $chipher | ./ft_ssl base64 -d)
	# echo "-----"
	if [ "$one" = "$str" ]
	then
		echo "   " $CLEAR_LINE
		echo "$GREEN $count OK $RESET\r\c"
		let count_ok=$count_ok+1
	else
		echo $CLEAR_LINE
		echo "$RED $count KO $RESET" 
		echo "## string: " ; echo $str
		echo $one
		let count_error=$count_error+1
	fi
	let count=$count+1
done
echo $CLEAR_LINE
echo "$GREEN \t$count_ok OK $WHITE"
echo "$RED \t$count_error KO $WHITE"

#----------------------------------------------------------------------
echo "$YELLOW Test decoding and write to file $WHITE"
count=0
count_ok=0
count_error=0
while [[ count -lt len ]]
do
	str=$(echo $count | python3  generate_string.py)
	chipher=$(echo $str | base64)
	echo $chipher | ./ft_ssl base64 -d -o file_1
	one=$(cat file_1)
	# echo "-----"
	if [ "$one" = "$str" ]
	then
		echo "   " $CLEAR_LINE
		echo "$GREEN $count OK $RESET\r\c"
		let count_ok=$count_ok+1
	else
		echo $CLEAR_LINE
		echo "$RED $count KO $RESET" 
		echo "## string: " ; echo $str
		echo $one
		let count_error=$count_error+1
	fi
	let count=$count+1
done
echo $CLEAR_LINE
echo "$GREEN \t$count_ok OK $WHITE"
echo "$RED \t$count_error KO $WHITE"
