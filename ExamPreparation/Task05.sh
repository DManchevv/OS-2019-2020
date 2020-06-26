#!/bin/bash
  
find / -user $(whoami) | wc -l
