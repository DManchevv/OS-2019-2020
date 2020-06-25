#!/bin/bash
# You must create the directory from which you move files and you must create the empty directory to which you move the files.

if [[ $# -ne 3 ]]; then
echo "No valid arguments!"
exit 1
fi

if [[ ! -d $1 || ! -d $2 ]]; then
echo "You haven't entered valid directories!"
exit 2
fi

if [[ 'whoami' != 'root' ]]; then
echo "You have to be root to execute the script!"
exit 3
fi

src=$1
dst=$2
str=$3

list_dst_paths=$(mktemp)
list_src_paths=$(mktemp)
list_union_paths=$(mktemp)

move_path=$(find $src -type f | grep "$str" | sed "s,${src},${dst},")
printf "%s\n" $move_path | rev | cut -d '/' -f2- | rev > $list_dst_paths
find $src -type f | grep "$str" > $list_src_paths
paste $list_src_paths $list_dst_paths > $list_union_paths

while read line; do
mkdir -p $line
done < "${list_dst_paths}"

while read line; do
from=$(echo $line | awk '{print $1}')
to=$(echo $line | awk '{print $2}')
mv $from $to
done < "${list_union_paths}"

