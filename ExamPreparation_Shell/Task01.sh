# Content of philip-j-fry.txt:
# 123abv123
# 123zz123
# MMU_2.4
# 135AZ135
# 246zASWZ80

#!/bin/bash
#philip-j-fry.txt is located in my home directory.
echo The number of rows is: $(cat /home/grade/philip-j-fry.txt | grep -v "[a-w]" | grep "[0,2,4,6,8]" | wc -l)!
