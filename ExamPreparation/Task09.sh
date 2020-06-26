#!/bin/bash
  
find ~ -type f -print0|xargs -0 stat --format '%Y :%y %n' | sort -n -r | cut -d ':' -f1 | head -n 1
