#!/bin/bash
  
ls -l | awk '{print $2,$NF}' | sort -r | grep "[a-zA-Z]" | head -n 5
