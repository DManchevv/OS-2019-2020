#!/bin/bash
if [[ $# -ne 1 ]]; then
echo "No valid arguments!"
exit 1
fi

file=$1

if [[ ! -f $file ]]; then
echo "Not a valid file!"
exit 2
fi

outputfile=$(mktemp)
declare -i numRow=1

while read line; do
   echo $numRow. $line | cut -d '.' -f1,3,4,5,6,7 | sed 's/ - / /' >> $outputfile
   numRow=$((numRow+1))
done < "${file}"
cat $outputfile | sort -k2.3 | head -n 4

rm -- "${outputfile}"
