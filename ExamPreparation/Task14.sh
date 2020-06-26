#!/bin/bash

if [[ $# -ne 1 ]]; then
echo "No valid arguments!"
exit 1
fi

number=$1

if [[ ! $number =~ [0-9]+ ]]; then
echo "Please enter a number!"
exit 2
fi

#if [[ 'whoami' != 'root' ]]; then
#echo "You must be root to run this script!"
#exit 3
#fi

for _user in $(ps haux | awk '{print $1}' | sort -u)
do
    userTotalRSS=$(ps haux | awk -v user=${_user} '$1 ~ user {sum += $6} END {print sum}')
    if [[ $userTotalRSS -gt $number ]]; then
         #kill process
    fi
done

