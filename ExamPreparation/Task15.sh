// Find which users do not have homedir

#!/bin/bash
  
if [[ 'whoami' != 'root' ]]; then
echo "You must be root to run this script"
exit 1
fi

cat /etc/passwd | cut -d ':' -f1,7 | awk -F '/' '{print $1,$NF}' | grep "nologin" | awk -F ':' '{print $1}'
