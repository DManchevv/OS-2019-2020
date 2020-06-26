#!/bin/bash
  

homeDirList=$(mktemp)
processList=$(mktemp)
usersList=$(mktemp)

cat /etc/passwd | cut -d ':' -f1,6 | grep -v "root" | sort  > ${homeDirList}


while read line; do
      users=$(echo $line | awk -F ':' '{print $1}')
      echo $line | awk -F ':' '{print $1}' >> ${usersList}
      homeDir=$(echo $line | awk -F ':' '{print $2}')

      if [[ ! -d $homeDir || "$(stat -c "%U" "$homeDir")" != "$users" || $(ls -ld $homeDir | cut -c 3) != "w" ]]; then
          ps -e -o user,rss,cmd | grep -v "root" | grep "$users" | sort >> $processList
      fi

done < ${homeDirList}

root_total_memory=$(ps haux | awk -v user="root" '$1 ~ user { sum += $6} END { print sum; }')

for _user in $(cat ${usersList})
do
   user_total_memory=$( ps haux | awk -v user=${_user} '$1 ~ user { sum += $6} END { print sum; }')
   if [[ $user_total_memory -gt $root_total_memory ]]; then
      echo "${_user}" has more memory than root!
      # Kill process here 
   fi
done

rm -- "${homeDirList}"
rm -- "${processList}"
rm -- "${usersList}"
