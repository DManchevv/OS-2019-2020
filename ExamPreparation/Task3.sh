#!/bin/bash
  
first=$1
second=$2
if [[ ! $first =~ ^[0-9]+$ || ! $second =~ ^[0-9]+$ ]]; then
exit 1
fi
files=$(find . -maxdepth 1 -type f)
for i in $files; do
if [[ $(cat $i | wc -l) < $first ]]; then
mv $i /a
elif [[ $(cat $i | wc -l) > $first && $(cat $i | wc -l) < $second ]]; then
mv $i /b
else mv $i /c
