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

OR

#!/bin/bash
  
if [[ $# -ne 1 ]]; then
    echo "Please enter 1 argument!"
    exit 1
fi

num=$1

if [[ ! $num =~ [0-9]+ ]]; then
    echo "Please enter a number!"
    exit 2
fi

#if [[ 'whoami' != 'root' ]]; then
#   echo "Permission denied! You must be root to run this script!"
#   exit 3
#fi

userList=$(mktemp)

ps -eo user | sort | uniq > ${userList}

while read line; do

userTotalRSS=$(ps -eo user,rss | grep "${line}" | tr -s [:blank:] | sort -t ' ' -k 2 -n | awk '{sum += $2} END {print sum}')
maxRSSPID=$(ps -eo user,pid,rss | grep "${line}" | tr -s [:blank:] | sort -t ' ' -k3 -n | tail -n 1 | cut -d ' ' -f2)

echo "${line} : ${userTotalRSS}"

if [[ ${userTotalRSS} -gt ${num} ]]; then
    #kill -15 ${maxRSSPID}
    sleep 0
    #kill -9 ${maxRSSPID}
fi

done < "${userList}"

rm -- "${userList}"
