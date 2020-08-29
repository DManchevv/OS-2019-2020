#!/bin/bash
  
userHome=$(mktemp)
latestForAllUsers=$(mktemp)

cat /etc/passwd | cut -d ':' -f 1,6 | tr ':' ' ' > "${userHome}"


while read user home; do

    if [[ ! -d "${home}" ]]; then
        echo "${home} is not valid directory!"
        continue
    fi

    if [[ ! -r "${home}" ]]; then
        echo "${home} is not readable!"
        continue
    fi

    latestMonth=$(ls -l ~ | tr -s [:blank:] | cut -d ' ' -f6- | sort -r | cut -d ' ' -f1 | tail -n 2)

    latestDate=$(ls -l ~ | grep "${latestMonth}" | tr -s [:blank:] | cut -d ' ' -f7- | sort -n | cut -d ' ' -f1 | tail -n 1)

    latestFile=$(ls -l ~ | grep "${latestMonth}" | grep "${latestDate}" | tr -s [:blank:] | cut -d ' ' -f8- | sort | tail -n 1 | cut -d ' ' -f2)

    completeFile=$(ls -l ${home} | grep "${latestMonth}" | grep "${latestDate}" | grep "${latestFile}" | tr -s [:blank:] | cut -d ' ' -f6-)

    echo "${user} ${completeFile}" >> "${latestForAllUsers}"

done < "${userHome}"

latestMonth=$(cat ${latestForAllUsers} | cut -d ' ' -f2 | sort -r | tail -n 2)

latestDate=$(cat ${latestForAllUsers} | grep "${latestMonth}" | cut -d ' ' -f3 | sort -n | tail -n 1)

latestFile=$(cat ${latestForAllUsers} | grep "${latestMonth}" | grep "${latestDate}" | cut -d ' ' -f4- | sort | tail -n 1 | cut -d ' ' -f2)

taskAnswer=$(cat ${latestForAllUsers} | grep "${latestMonth}" | grep "${latestDate}" | grep "${latestFile}" | cut -d ' ' -f1)

echo "${taskAnswer}"

rm -- ${userHome}
rm -- ${latestForAllUsers}
