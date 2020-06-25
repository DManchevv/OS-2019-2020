#!/bin/bash
#First Solution
if [[ $# -eq 0 ]]; then
echo "No valid arguments!"
exit 1
fi

dir=$1

if [[ $# -eq 2 ]]; then
numb=$2
fi

if [[ ! -d $dir ]]; then
echo "$dir is not a directory!"
exit 2
fi

hardlinksCount=$(mktemp)
targetinodes=$(mktemp)
ls -li "${dir}" | awk '{print $1}' | sort -k1n | uniq -c -w 6 | sort -r -n > "${hardlinksCount}"


while read line; do
linenumb=$(echo "$line" | awk '{print $1}')
lineinode=$(echo "$line" | awk '{print $2}')
if [[ $linenumb -ge $numb ]]; then
echo $lineinode > "${targetinodes}"

fi
done < "${hardlinksCount}"

#cat "${targetinodes}"

while read line; do
printf '%s\n' $(ls -li "${dir}" |  grep "$line" | awk '{print $NF}')
done < "${targetinodes}"
