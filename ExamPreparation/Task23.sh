#!/bin/bash
  
if [[ $# -lt 2 ]]; then
echo "Not enough arguments!"
exit 1
fi

dir=$1
str=$2

if [[ ! -d $dir ]]; then
echo "${dir} is not a valid directory!"
exit 2
elif [[ ! -r $dir ]]; then
echo "${dir} is not readable!"
exit 3
fi

arch=$(mktemp)
find $dir -maxdepth 1 -type f | grep -E  "vmlinuz-[0-9]+.[0-9]+.[0-9]+-$str" > $arch

latest_version=$(cat $arch | awk -F '-' '{print $2}' | sort -r -n | head -n 1)

echo $(cat $arch | grep "$latest_version" | rev | awk -F '/' '{print $1}' | rev)

rm -- "${arch}"
