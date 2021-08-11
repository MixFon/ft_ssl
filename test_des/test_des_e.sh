#!/usr/bin/env bash

count_ok=0
count_error=0

end_range=101
start_range=1

count=$start_range

YELLOW="\033[1;33m"
GREEN="\033[1;32m"
RED="\033[1;31m"
WHITE="\033[1;37m"
RESET="\033[0;0m"
CLEAR_SCREEN="\033[2J"
CLEAR_LINE="\033[2K\c"

echo "$YELLOW Test DES-ECB $WHITE"
echo "$YELLOW Test encoding-decoding $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	one=$(echo $str | ./ft_ssl des-ecb -e -k $key -o /tmp/file)
	two=$(			  ./ft_ssl des-ecb -d -k $key -i /tmp/file)
	# echo "-----"
	if [ "$str" = "$two" ]
	then
		echo "   " $CLEAR_LINE
		echo "$GREEN $count OK $RESET\r\c"
		let count_ok=$count_ok+1
	else
		echo $CLEAR_LINE
		echo "$RED $count KO $RESET" ; echo "## string: " 
		echo "($one)"
		echo "[$str]"
		echo "{$two}"
		let count_error=$count_error+1
	fi
	let count=$count+1
done
echo $CLEAR_LINE
echo "$GREEN \t$count_ok OK $WHITE"
echo "$RED \t$count_error KO $WHITE"
#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding with -a (base64) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	one=$(echo $str | ./ft_ssl des-ecb -e -k $key -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-ecb -d -k $key -i /tmp/file -a)
	# echo "-----"
	if [ "$str" = "$two" ]
	then
		echo "   " $CLEAR_LINE
		echo "$GREEN $count OK $RESET\r\c"
		let count_ok=$count_ok+1
	else
		echo $CLEAR_LINE
		echo "$RED $count KO $RESET" ; echo "## string: " 
		echo "($one)"
		echo "[$str]"
		echo "{$two}"
		let count_error=$count_error+1
	fi
	let count=$count+1
done
echo $CLEAR_LINE
echo "$GREEN \t$count_ok OK $WHITE"
echo "$RED \t$count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding with -s (salt) -p (password) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	salt=$(echo $str | md5)
	password=$(cat /dev/random | base64 | head -c 64 )
	one=$(echo $str | ./ft_ssl des-ecb -e -s $salt -p $password -o /tmp/file -a) 
	two=$(./ft_ssl des-ecb -d -s $salt -p $password -i /tmp/file -a)
	# echo "-----"
	if [ "$str" = "$two" ]
	then
		echo "   " $CLEAR_LINE
		echo "$GREEN $count OK $RESET\r\c"
		let count_ok=$count_ok+1
	else
		echo $CLEAR_LINE
		echo "$RED $count KO $RESET" ; echo "## string: " 
		echo "[$str]"
		echo "{$two}"
		let count_error=$count_error+1
	fi
	let count=$count+1
done
echo $CLEAR_LINE
echo "$GREEN \t$count_ok OK $WHITE"
echo "$RED \t$count_error KO $WHITE"

#------------------------- DES-CBC ------------------------------
#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test DES-CBC $WHITE"
echo "$YELLOW Test encoding-decoding -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl des-cbc -e -k $key -v $iv -o /tmp/file)
	two=$(			  ./ft_ssl des-cbc -d -k $key -v $iv -i /tmp/file)
	# echo "-----"
	if [ "$str" = "$two" ]
	then
		echo "   " $CLEAR_LINE
		echo "$GREEN $count OK $RESET\r\c"
		let count_ok=$count_ok+1
	else
		echo $CLEAR_LINE
		echo "$RED $count KO $RESET" ; echo "## string: " 
		echo "($one)"
		echo "[$str]"
		echo "{$two}"
		let count_error=$count_error+1
	fi
	let count=$count+1
done
echo $CLEAR_LINE
echo "$GREEN \t$count_ok OK $WHITE"
echo "$RED \t$count_error KO $WHITE"
#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding with -a (base64) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl des-cbc -e -k $key -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-cbc -d -k $key -v $iv -i /tmp/file -a)
	# echo "-----"
	if [ "$str" = "$two" ]
	then
		echo "   " $CLEAR_LINE
		echo "$GREEN $count OK $RESET\r\c"
		let count_ok=$count_ok+1
	else
		echo $CLEAR_LINE
		echo "$RED $count KO $RESET" ; echo "## string: " 
		echo "($one)"
		echo "[$str]"
		echo "{$two}"
		let count_error=$count_error+1
	fi
	let count=$count+1
done
echo $CLEAR_LINE
echo "$GREEN \t$count_ok OK $WHITE"
echo "$RED \t$count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding with -s (salt) -p (password) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	salt=$(echo $str | md5)
	pass=$(cat /dev/random | base64 | head -c 64 )
	iv=$(echo $salt | md5)
	one=$(echo $str | ./ft_ssl des-cbc -e -s $salt -p $pass -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-cbc -d -s $salt -p $pass -v $iv -i /tmp/file -a)
	# echo "-----"
	if [ "$str" = "$two" ]
	then
		echo "   " $CLEAR_LINE
		echo "$GREEN $count OK $RESET\r\c"
		let count_ok=$count_ok+1
	else
		echo $CLEAR_LINE
		echo "$RED $count KO $RESET" ; echo "## string: " 
		echo "[$str]"
		echo "{$two}"
		let count_error=$count_error+1
	fi
	let count=$count+1
done
echo $CLEAR_LINE
echo "$GREEN \t$count_ok OK $WHITE"
echo "$RED \t$count_error KO $WHITE"

