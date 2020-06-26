#!/bin/bash
# Example for a file:
# 2005г. Bonnie - "God Was in the Water" (Randall Bramblett, Davis Causey) - 5:17
# 2005г. Bonnie - "Love on One Condition" (Jon Cleary) - 3:43
# 2005г. Bonnie - "So Close" (Tony Arata, George Marinelli, Pete Wasner) - 3:22
# 2005г. Bonnie - "Trinkets" (Emory Joseph) - 5:02
# 2005г. Bonnie - "Crooked Crown" (David Batteau, Maia Sharp)  3:49
# 2005г. Bonnie - "Unnecessarily Mercenary" (Jon Cleary)  3:51
# 2005г. Bonnie - "I Will Not Be Broken" - "Deep Water" (John Capek, Marc Jordan)  3:58



if [[ $# -ne 2 ]]; then
echo "Please enter 2 arguments!"
exit 1
fi

fileOne=$1
fileTwo=$2

if [[ ! -f $fileOne || ! -f $fileTwo ]]; then
echo "Please enter valid files!"
exit 2
fi

fileOneCount=$(cat ${fileOne} | egrep -c "$fileOne")
fileTwoCount=$(cat ${fileTwo} | egrep -c "$fileTwo")

if [[ $fileOneCount -gt $fileTwoCount ]]; then
     cat ${fileOne} | cut -d '-' -f2- | cut -c3- | sort | sed -e 's/^/"/'
     elif [[ $fileOneCount -lt $fileTwoCount ]]; then
             cat ${fileTwo} | cut -d '-' -f2- | cut -c3- | sort | sed -e 's/^/"/'
     else echo "Both files have equal count!"
fi
