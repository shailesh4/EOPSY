#!/bin/bash

# EOPSY Lab 1

self=$(basename $0)

yellow()	{ ansi 33 "$@"; }
red()		{ ansi 31 "$@"; }
green()		{ ansi 32 "$@"; }
purple()	{ ansi 35 "$@"; }
ansi()		{ echo -e "\033[0;${1}m${2}\033[0m"; }

# RED='\033[0;31m'
# NC='\033[0m' # No Color
# printf "I \033[0;31m love \033[0m Stack Overflow\n"
# 		  \033[0;33mmodify.sh\033[0m

# echo ""
# echo "Running $(yellow $self) - file/dir name changing tool"
# echo ""
# echo ""

Exit(){
	echo "The following error was encountered"
	echo "$(red "$@")"
	echo "Type $(yellow $self -h) to show help"
	
	exit 1
}

# flags for arguments
flagLower=0
flagUpper=0
flagHelp=0
flagRecursion=0

# regex for lower and upper
regexUpper="s/\(.*\)/\U\1/"
regexLower="s/\(.*\)/\L\1/"
currentRegex=""

for var in "$@"
do

	case ${var} in 
		-h)
			flagHelp=1
		;;
		-l)
			flagLower=1
		;;
		-u)
			flagUpper=1
		;;
		-r)
			flagRecursion=1
		;;
		*)
			echo ${var}
		;;
	esac
done

if [ ${flagHelp} -eq "1" ]; then
	if [[ ${flagUpper} -eq "1" || ${flagLower} -eq "1" ]]   || [ ${flagRecursion} -eq "1" ]; then
		Exit "-h is an alone argument. You cannot use multiple arguments with -h"
	fi
fi

if [ ${flagHelp} -eq "1" ]; then
	echo "How to use: "
	echo "$self [-r] [-l|-u] <dir/file names...>"
	echo "$self [-r] <sed pattern> <dir/file names...>"
	echo "$self [-h]"
	echo ""
	echo "-l: lowerizes file names"
	echo "-u: uppercasing the file names"
	echo "-r: using recursion to traverse the directories"
	echo "-h: show this help message"
	exit 1

fi

if [ ${flagUpper} -eq "1" ] && [ ${flagLower} -eq "1" ]; then
	Exit "-l and -u cannot be set at the same time."
fi

if [ $# -lt 1 ]; then
	Exit "No arguments presented. run with -h to see the help message."
fi

#### If the arguments presented are valid.

if [ ${flagLower} -eq "1" ]; then
	currentRegex=${regexLower}
elif [ ${flagUpper} -eq "1" ]; then
	currentRegex=${regexUpper}
elif [ ${flagRecursion} -eq "1" ]; then
	currentRegex=$2 # if with -r flag, sed pattern is $2
else
	currentRegex=$1 # without -r flag, sed pattern is $1
fi

## getting the files/directories from arguments

array="${!#}" # last argument for file names

for dir in "${array[@]}"; do
	
	if [ ${flagRecursion} -eq "1" ]; then
		findCall="find . | grep \"${dir}\" | tail -r"
	else
		findCall="find ${dir} -maxdepth 1"
	fi

	eval $findCall | while read -r file; do

		currentDirectoryname=$(dirname "$file")
		currentFileName=$(basename "$file")

		# if [ -d "./$currentDirectoryname/$currentFileName" ]; then
		# 	continue # it is a directory
		# fi

		
		if [ ${currentFileName} = ${self} ]; then
			continue
		fi

		newFileName=$(echo ${currentFileName} | "gsed" "${currentRegex}")
		echo "$currentDirectoryname/$newFileName"
		echo ""
		echo ""

		if [ $? -ne "0" ]; then
            continue
        fi

        if [ "$currentFilename" = "$newFileName" ]; then
            continue
        fi

        echo ""
		echo ""
		# generating new file name using sed currentRegex
		echo "Old file name $currentDirectoryname/$currentFileName"
		echo "New file Name $currentDirectoryname/$newFileName"
		echo ""
		echo ""
        
        mv -- "$file" "$currentDirectoryname/$newFileName"
	done
done