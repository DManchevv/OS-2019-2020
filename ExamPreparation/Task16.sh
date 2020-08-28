#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "Please enter 3 arguments!"
    exit 1
fi

file=$1
str1=$2
str2=$3
isFound=0

if [[ ! -f ${file} ]]; then
    echo "Please enter valid file!"
    exit 2
fi

if [[ $(cat ${file} | egrep -c "${str1}=") -eq 0 ]]; then
    echo "The first string is not in the file!"
    exit 3
fi

if [[ $(cat ${file} | egrep -c "${str2}=") -eq 0 ]]; then
    echo "The second string is not in the file!"
    exit 4
fi

newFile=$(mktemp)
cat ${file} | grep -v "${str2}=" > ${newFile}

if [[ $(cat ${file} | egrep -c "${str2}") -eq 0 ]]; then
    echo "${str2}=" >> ${newFile}
fi

if [[ $(cat ${file} | egrep -c "${str2}") -ne 0 ]]; then
    echo -n "${str2}=" >> ${newFile}
    for i in $(cat ${file} | grep "${str2}=" | cut -d '=' -f2); do
        isFound=0
        for j in $(cat ${file} | grep "${str1}=" | cut -d '=' -f2); do
 	    if [[ $i == $j ]]; then
                isFound=1
            fi
        done
        if [[ ${isFound} -eq 0 ]]; then
            echo -n "${i} " >> ${newFile}
        fi
    done
    echo " " >> ${newFile}
    cat ${newFile} > ${file}
fi


rm -- ${newFile}
