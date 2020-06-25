#!/bin/bash
# a)
if [[ 'whoami' != 'root' ]]; then
echo "You must be root to run this script!"
exit 1
fi

users_list=$(mktemp)

ps -e -o user,rss,pid | sort | awk '{print $1}' | uniq > $users_list

while read line; do
user_rss=$(ps -e -o user,rss | sort | grep "$line" | awk '{sum += $2} END {print sum}')
#echo $line : $(ps -e -o user,rss | sort | egrep -c "$line") : $user_rss
done < $users_list

# b)

while read line; do
process_count=$(ps -e -o user,rss | sort | grep "$line" | wc -l)
avg=$((user_rss / process_count))
largest_process=$(ps -e -o user,rss | grep "$line" | awk '{print $2}' | sort -r -n | head -n 1)
largest_process_PID=$(ps -e -o user,rss,pid | grep "$line" | awk '{print $2,$3}' | sort -r -k1n | head -n 1 | awk '{print $2}')

if [[ $largest_process -gt $avg ]]; then
kill -s SIGTERM ${largest_process_PID}
sleep 2
kill -s SIGKILL ${largest_process_PID}
fi

done < $users_list


rm -- "${users_list}"
