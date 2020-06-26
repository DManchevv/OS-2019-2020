#!/bin/bash
  
if [[ $# -eq 0 ]]; then
echo "No valid arguments!"
exit 1
fi

dir=$1
fileName=0

if [[ ! -d $dir ]]; then
echo "Not a valid directory!"
exit 2
fi

if [[ ! -r $dir ]]; then
echo "Directory is not readable!"
exit 3
fi

if [[ $# -eq 2 ]]; then
fileName=$2
fi

while read symlink; do
if  fgrep -q "broken sym" <(echo "${symlink}") ; then
  count=$((count+1))
  else
         if [[ -n "${fileName}" ]]; then
                  stat -c "%N" "$(echo "${symlink}" | cut -d ':' -f1)" | tr -d "\'" >> "${fileName}"
             else stat -c "%N" "$(echo "${sumlink}" | cut -d ':' -f1)" | tr -d "\'"
         fi
fi
done < <(find "${dir}" -type l 2>/dev/null -exec file {} \;)

if [[ -n "${fileName}" ]]; then
cat "${fileName}"
fi

echo "Broken symlinks: ${count}"
