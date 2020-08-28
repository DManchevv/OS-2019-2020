#!/bin/bash
 
ls -l | tr -s [:blank:] | awk '{print $2, $NF}' | sort -n -r | head -n 6 | tail -n 5

