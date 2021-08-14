#!/usr/bin/env bash

count_ok=0
count_error=0

end_range=1000
start_range=990

count=$start_range

YELLOW="\033[1;33m"
GREEN="\033[1;32m"
RED="\033[1;31m"
WHITE="\033[1;37m"
RESET="\033[0;0m"
BLUE="\x1b[34m"
MAGENTA="\x1b[35m"
CLEAR_SCREEN="\033[2J"
CLEAR_LINE="\033[2K\c"

base64_set=0
des_set=0
des3_set=0
des_ecb_set=0
des_cbc_set=0
des_pcbc_set=0
des_ofb_set=0
des_cfb_set=0
des_ctr_set=0
simple_set=0

function print_usage {
	echo "usage: $0 <test>..."
	echo ""
	echo "available tests: base64"
	echo "                 des"
	echo "                 des3"
	echo "                 des_ecb"
	echo "                 des_cbc"
	echo "                 des_pcbc"
	echo "                 des_ofb"
	echo "                 des_cfb"
	echo "                 des_ctr"
	echo "                 simple"
	echo "                 all"
}

if [ $# -eq 0 ] ; then
	print_usage
	exit -1
fi

for arg in "$@" ; do
	case $arg in
		base64)
			base64_set=1
			;;
		des)
			des_set=1
			;;
		des_ecb)
			des_ecb_set=1
			;;
		des_cbc)
			des_cbc_set=1
			;;
		des3)
			des3_set=1
			;;
		des_pcbc)
			des_pcbc_set=1
			;;
		des_ofb)
			des_ofb_set=1
			;;
		des_cfb)
			des_cfb_set=1
			;;
		des_ctr)
			des_ctr_set=1
			;;
		simple)
			simple_set=1
			;;
		all)
			base64_set=1
			des_set=1
			des_ecb_set=1
			des_cbc_set=1
			des3_set=1
			des_pcbc_set=1
			des_ofb_set=1
			des_cfb_set=1
			des_ctr_set=1
			simple_set=1
			;;
		*)
			print_usage
			exit 1
	esac
done

#---------------------------- BASE64 ----------------------------
#----------------------------------------------------------------

if [[ $base64_set -eq 1 ]] ; then

echo "$MAGENTA Test BASE64 $WHITE"
echo "$YELLOW Test encoding stdin $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	one=$(echo $str | ./ft_ssl base64 -e)
	two=$(echo $str | openssl base64 -A)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------------
count=$start_range
count_ok=0
count_error=0
echo "$YELLOW Test encoding file $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	echo $str > temp
	one=$(./ft_ssl base64 -e -i temp)
	two=$(openssl base64 -A -in temp)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------------
echo "$YELLOW Test write ot file $WHITE"
count=$start_range
count_ok=0
count_error=0
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	echo $str | ./ft_ssl base64 -e -o file_1
	echo $str | openssl base64 -A -out file_2
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------------
echo "$YELLOW Test decoding $WHITE"
count=$start_range
count_ok=0
count_error=0
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	chipher=$(echo $str | base64)
	one=$(echo $chipher | ./ft_ssl base64 -d)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------------
echo "$YELLOW Test decoding and write to file $WHITE"
count=$start_range
count_ok=0
count_error=0
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	chipher=$(echo $str | base64)
	echo $chipher | ./ft_ssl base64 -d -o file_1
	one=$(cat file_1)
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
rm -rf file_1
echo $CLEAR_LINE
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

fi

#---------------------------- DES-ECB ---------------------------
#----------------------------------------------------------------

if [[ $des_ecb_set -eq 1 ]] ; then

echo "$MAGENTA Test DES-ECB $WHITE"
echo "$YELLOW Test encoding-decoding -k (key) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	one=$(echo $str | ./ft_ssl des-ecb -e -k $key -o /tmp/file)
	two=$(			  ./ft_ssl des-ecb -d -k $key -i /tmp/file)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -a (base64) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	one=$(echo $str | ./ft_ssl des-ecb -e -k $key -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-ecb -d -k $key -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -p (password) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	password=$(cat /dev/random | base64 | head -c 64 )
	one=$(echo $str | ./ft_ssl des-ecb -e -p $password -o /tmp/file -a) 
	two=$(./ft_ssl des-ecb -d -p $password -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -s (salt) -p (password) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	salt=$(echo $str | md5)
	password=$(cat /dev/random | base64 | head -c 64 )
	one=$(echo $str | ./ft_ssl des-ecb -e -s $salt -p $password -o /tmp/file -a) 
	two=$(./ft_ssl des-ecb -d -s $salt -p $password -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"
fi

#------------------------- DES-CBC ------------------------------
#----------------------------------------------------------------

if [[ $des_cbc_set -eq 1 ]] || [[ $des_set -eq 1 ]] ; then
count_ok=0
count_error=0

count=$start_range

echo "$MAGENTA Test DES-CBC $WHITE"
echo "$YELLOW Test encoding-decoding -k (key) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl des-cbc -e -k $key -v $iv -o /tmp/file)
	two=$(			  ./ft_ssl des-cbc -d -k $key -v $iv -i /tmp/file)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -a (base64) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl des-cbc -e -k $key -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-cbc -d -k $key -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -a (base64) -p (password) -v (vec) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	pass=$(cat /dev/random | base64 | head -c 64 )
	iv=$(echo $salt | md5)
	one=$(echo $str | ./ft_ssl des-cbc -e -p $pass -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-cbc -d -p $pass -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -s (salt) -p (password) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	salt=$(echo $str | md5)
	pass=$(cat /dev/random | base64 | head -c 64 )
	iv=$(echo $salt | md5)
	one=$(echo $str | ./ft_ssl des-cbc -e -s $salt -p $pass -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-cbc -d -s $salt -p $pass -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"
fi

#---------------------------- DES3 ------------------------------
#----------------------------------------------------------------

if [[ $des3_set -eq 1 ]] ; then
count_ok=0
count_error=0

count=$start_range

echo "$MAGENTA Test DES3 $WHITE"
echo "$YELLOW Test encoding-decoding -k (key) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl des3 -e -k $key -v $iv -o /tmp/file)
	two=$(			  ./ft_ssl des3 -d -k $key -v $iv -i /tmp/file)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -a (base64) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl des3 -e -k $key -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des3 -d -k $key -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -a (base64) -p (password) -v (vec) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	pass=$(cat /dev/random | base64 | head -c 64 )
	iv=$(echo $salt | md5)
	one=$(echo $str | ./ft_ssl des3 -e -p $pass -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des3 -d -p $pass -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -s (salt) -p (password) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	salt=$(echo $str | md5)
	pass=$(cat /dev/random | base64 | head -c 64 )
	iv=$(echo $salt | md5)
	one=$(echo $str | ./ft_ssl des3 -e -s $salt -p $pass -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des3 -d -s $salt -p $pass -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

fi

#---------------------------- DES-PCBC --------------------------
#----------------------------------------------------------------

if [[ $des_pcbc_set -eq 1 ]] ; then
count_ok=0
count_error=0

count=$start_range

echo "$MAGENTA Test DES-PCBC $WHITE"
echo "$YELLOW Test encoding-decoding -k (key) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl des-pcbc -e -k $key -v $iv -o /tmp/file)
	two=$(			  ./ft_ssl des-pcbc -d -k $key -v $iv -i /tmp/file)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -a (base64) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl des-pcbc -e -k $key -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-pcbc -d -k $key -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -a (base64) -p (password) -v (vec) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	pass=$(cat /dev/random | base64 | head -c 64 )
	iv=$(echo $salt | md5)
	one=$(echo $str | ./ft_ssl des-pcbc -e -p $pass -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-pcbc -d -p $pass -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -s (salt) -p (password) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	salt=$(echo $str | md5)
	pas=$(cat /dev/random | base64 | head -c 64 )
	iv=$(echo $salt | md5)
	one=$(echo $str | ./ft_ssl des-pcbc -e -s $salt -p $pas -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-pcbc -d -s $salt -p $pas -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"
fi

#---------------------------- DES-OFB --------------------------
#----------------------------------------------------------------

if [[ $des_ofb_set -eq 1 ]] ; then
count_ok=0
count_error=0

count=$start_range

echo "$MAGENTA Test DES-OFB $WHITE"
echo "$YELLOW Test encoding-decoding -k (key) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl des-ofb -e -k $key -v $iv -o /tmp/file)
	two=$(			  ./ft_ssl des-ofb -d -k $key -v $iv -i /tmp/file)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -a (base64) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl des-ofb -e -k $key -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-ofb -d -k $key -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -a (base64) -p (password) -v (vec) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	pass=$(cat /dev/random | base64 | head -c 64 )
	iv=$(echo $salt | md5)
	one=$(echo $str | ./ft_ssl des-ofb -e -p $pass -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-ofb -d -p $pass -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -s (salt) -p (password) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	salt=$(echo $str | md5)
	pas=$(cat /dev/random | base64 | head -c 64 )
	iv=$(echo $salt | md5)
	one=$(echo $str | ./ft_ssl des-ofb -e -s $salt -p $pas -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-ofb -d -s $salt -p $pas -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"
fi

#---------------------------- DES-CFB --------------------------
#----------------------------------------------------------------

if [[ $des_cfb_set -eq 1 ]] ; then
count_ok=0
count_error=0

count=$start_range

echo "$MAGENTA Test DES-CFB $WHITE"
echo "$YELLOW Test encoding-decoding -k (key) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl des-cfb -e -a -k $key -v $iv -o /tmp/file)
	two=$(			  ./ft_ssl des-cfb -d -a -k $key -v $iv -i /tmp/file)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -a (base64) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl des-cfb -e -k $key -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-cfb -d -k $key -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -a (base64) -p (password) -v (vec) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	pass=$(cat /dev/random | base64 | head -c 64 )
	iv=$(echo $salt | md5)
	one=$(echo $str | ./ft_ssl des-cfb -e -p $pass -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-cfb -d -p $pass -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -s (salt) -p (password) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	salt=$(echo $str | md5)
	pas=$(cat /dev/random | base64 | head -c 64 )
	iv=$(echo $salt | md5)
	one=$(echo $str | ./ft_ssl des-cfb -e -s $salt -p $pas -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-cfb -d -s $salt -p $pas -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"
fi

#---------------------------- DES-CRT ---------------------------
#----------------------------------------------------------------

if [[ $des_ctr_set -eq 1 ]] ; then
count_ok=0
count_error=0

count=$start_range

echo "$MAGENTA Test DES-CTR mod $WHITE"
echo "$YELLOW Test encoding-decoding -k (key) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl des-ctr -e -k $key -v $iv -o /tmp/file)
	two=$(			  ./ft_ssl des-ctr -d -k $key -v $iv -i /tmp/file)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -a (base64) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl des-ctr -e -k $key -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-ctr -d -k $key -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -a (base64) -p (password) -v (vec) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	pass=$(cat /dev/random | base64 | head -c 64 )
	iv=$(echo $salt | md5)
	one=$(echo $str | ./ft_ssl des-ctr -e -p $pass -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-ctr -d -p $pass -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -s (salt) -p (password) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	salt=$(echo $str | md5)
	pas=$(cat /dev/random | base64 | head -c 64 )
	iv=$(echo $salt | md5)
	one=$(echo $str | ./ft_ssl des-ctr -e -s $salt -p $pas -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl des-ctr -d -s $salt -p $pas -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"
fi

#---------------------------- Simple ----------------------------
#----------------------------------------------------------------

if [[ $simple_set -eq 1 ]] ; then
count_ok=0
count_error=0

count=$start_range

echo "$MAGENTA Test Simple mod $WHITE"
echo "$YELLOW Test encoding-decoding -k (key) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl simple -e -k $key -v $iv -o /tmp/file)
	two=$(			  ./ft_ssl simple -d -k $key -v $iv -i /tmp/file)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -a (base64) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	key=$(echo $str | md5)
	iv=$(echo $key | md5)
	one=$(echo $str | ./ft_ssl simple -e -k $key -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl simple -d -k $key -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -a (base64) -p (password) -v (vec) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	pass=$(cat /dev/random | base64 | head -c 64 )
	iv=$(echo $salt | md5)
	one=$(echo $str | ./ft_ssl simple -e -p $pass -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl simple -d -p $pass -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"

#----------------------------------------------------------------
count_ok=0
count_error=0

count=$start_range

echo "$YELLOW Test encoding-decoding -s (salt) -p (password) -v (vect) $WHITE"
while [[ count -lt end_range ]]
do
	str=$(cat /dev/random | base64 | head -c $count )
	salt=$(echo $str | md5)
	pas=$(cat /dev/random | base64 | head -c 64 )
	iv=$(echo $salt | md5)
	one=$(echo $str | ./ft_ssl simple -e -s $salt -p $pas -v $iv -o /tmp/file -a) 
	two=$(			  ./ft_ssl simple -d -s $salt -p $pas -v $iv -i /tmp/file -a)
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
echo "$GREEN \t$count_ok OK $WHITE / $RED $count_error KO $WHITE"
fi
