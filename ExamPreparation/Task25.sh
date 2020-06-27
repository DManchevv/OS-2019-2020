#!/bin/bash
  
if [[ $# -ne 1 ]]; then
     echo "No valid arguments!"
     exit 1
fi

LOGDIR=$1

if [[ ! -d $LOGDIR ]]; then
     echo "${LOGDIR} is not a directory!"
     exit 2
fi

if [[ ! -r $LOGDIR ]]; then
     echo "${LOGDIR} is not readable!"
     exit 3
fi

FriendsLIST=$(mktemp)
NumberROWS=$(mktemp)


find ${LOGDIR} -type f | rev | cut -d '/' -f2 | rev | sort | uniq > ${FriendsLIST}

while read line; do

    printf "%s\n" $line:$(find ${LOGDIR} -type f | grep -w "$line" | xargs cat | wc -l) >> ${NumberROWS}

done < "${FriendsLIST}"

cat ${NumberROWS} | sort -r -k2n | cut -d ':' -f1 | head -n 10

rm -- "${NumberROWS}"
rm -- "${FriendsLIST}"

