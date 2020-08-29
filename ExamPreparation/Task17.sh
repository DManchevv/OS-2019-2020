// Doing basic checks

#!/bin/bash
if [[ $# -eq 0 ]]; then
echo "No valid arguments"
exit 1
fi

#if [[ 'whoami' != 'root' ]]; then
#echo "You must be root to execute the script!"
#exit 2
#fi

target=$1
user_exist=$(ps -e -o user | egrep -c "^$target$")

if [[ $user_exist -eq 0 ]]; then
echo "The user does not exist"
exit 3
fi

// Starting with a)

processes=$(ps -e -o user,pid,%cpu,%mem,vsz,rss,tty,stat,time | awk '{print $1}' | grep -v "^$target$" | sort | uniq -c | sort -n -r)
user_count=$(ps -e -o user,pid,%cpu,%mem,vsz,rss,tty,stat,time | awk '{print $1}' | grep "^$target$" | uniq -c | awk '{print $1}')
while read -r line; do
count=$(echo $line|awk '{print $1}')
name=$(echo $line|awk '{print $2}')
if [[ $count -gt $user_count ]]; then
echo $name
fi
done <<< "$processes"

process_count=$(ps -e | wc -l)
echo $process_count
seconds_sum=$(ps -e -o time | awk -F ':' '{sum+= $3} END {print sum}')
minutes_sum=$(ps -e -o time | awk -F ':' '{sum += $2} END {print sum}')
minutes_real_sum=$((minutes_sum+seconds_sum/60))
hours_sum=$(ps -e -o time | awk -F ':' '{sum += $1} END {print sum}')
hours_real_sum=$((hours_sum+minutes_real_sum/60))
seconds_sum=$((seconds_sum+minutes_sum*60+hours_sum*3600))
seconds_sum=$((seconds_sum/process_count))
minutes_real_sum=$((minutes_sum+seconds_sum/60))
hours_real_sum=$((hours_sum+minutes_real_sum/60))
minutes_real_sum=$((minutes_real_sum%60))
seconds_sum=$((seconds_sum%60))
echo $hours_real_sum:$minutes_real_sum:$seconds_sum
