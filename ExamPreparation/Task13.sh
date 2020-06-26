#!/bin/bash
args_path=$1
if [[ ! -d $args_path ]]; then
exit 1
fi
find "${args_path}" -type l -printf "%Y %p\n" 2>/dev/null | grep -e "^[^NL]" | cut -c3-
