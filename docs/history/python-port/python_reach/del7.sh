# del7.sh <files...>: count lines and git rm
n=0; for f in "$@"; do n=$((n + $(wc -l < "$f"))); done; echo "$# files $n lines"; git rm -q "$@"
