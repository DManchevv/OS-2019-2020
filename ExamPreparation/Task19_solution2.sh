#!/bin/bash
#Pravilnoto i lesno reshenie za umni hora koito znaqt komandite :)

if [[ $# -eq 0 ]]; then
echo "No valid arguments!"
exit 1
fi

dir=$1

if [[ ! -d $dir ]]; then
echo "$dir is not a directory!"
exit 2
fi

if [[ $# -eq 2 ]]; then
numb=$2
find $dir -maxdepth 1 -type f -links +$numb |xargs -I{} ls {}
fi

if [[ $# -eq 1 ]]; then
find $dir -xtype l |xargs -I{} ls {}
fi
