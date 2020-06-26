// a)
find . -maxdepth 1 -type f -size 0 |xargs -I{} rm {}

// b)
find . -maxdepth 2 -type f | du | sort -r -n | head -n 6 | grep "./." | awk '{print $2}' |xargs -I{} rm -r {}
