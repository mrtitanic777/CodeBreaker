#!/usr/bin/env bash
# Build ONE FF DLL via MinGW (gcc -m32), single module -> build/ffcorpus/dll (what build_safe swaps).
# Mirrors build_ff_corpus_mt.sh's build_one. cwd-independent (sources env.sh; layout defaults to
# WT_DATA_ROOT). MinGW's gcc -w tolerates the lift's arity-flexible calls (no MSVC C2198).
#   CLEAN=1 data/pipeline/build_mingw_one.sh MODULE.DLL        # deregisterized
#   CLEAN=1 VERIFIED=<manifest> data/pipeline/build_mingw_one.sh MODULE.DLL   # oracle-gated
set -uo pipefail
m="$1"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")/../.." && pwd)"     # repo root
source "$HERE/env.sh"
# git-bash PATH needs Unix-style (/c/..); a Windows-style C:/.. entry is ignored -> gcc would fall
# through to the 64-bit mingw64 and the -m32 link fails ("cannot find -lgcc").
export PATH="$(cygpath -u "$MINGW32" 2>/dev/null || echo /c/msys64/mingw32/bin):$PATH"
DLL="$BUILD/ffcorpus/dll"; WORK="$BUILD/ffcorpus/work"; RT="$BUILD/ffcorpus/rt.o"
mkdir -p "$DLL" "$WORK"
[ -f "$RT" ] || gcc -std=gnu89 -m32 -O0 -w -c "$INC/win95rt.c" -o "$RT"
CLEAN_FLAG=""; [ "${CLEAN:-0}" = 1 ] && CLEAN_FLAG="--clean"
VER_FLAG="";   [ -n "${VERIFIED:-}" ] && VER_FLAG="--verified ${VERIFIED}"
REDIR_FLAG=""; [ "${REDIRECT:-0}" = 1 ] && REDIR_FLAG="--redirect"
up=$(echo "$m" | tr '[:lower:]' '[:upper:]'); s=$(echo "$m" | sed 's/\./_/')
final="$DLL/$up"; W="$WORK/$s"; rm -rf "$W"; mkdir -p "$W"

python3 "$TOOLS/package_ff.py" "$m" --out "$W" $CLEAN_FLAG $VER_FLAG $REDIR_FLAG >"$W/pf.log" 2>&1 \
  || { echo "FAIL $m package_ff"; tail -3 "$W/pf.log"; exit 1; }
grep -h 'fall back' "$W/pf.log"
gcc -std=gnu89 -m32 -O0 -w -I "$INC" -I "$W" -c "$W/${s}_reloc.c" -o "$W/mod.o" 2>"$W/cc1.log" \
  || { echo "FAIL $m compile-reloc"; grep -iE 'error' "$W/cc1.log" | head -3; exit 1; }
gcc -std=gnu89 -m32 -O0 -w -I "$INC" -I "$W" -c "$W/${s}_glue.c" -o "$W/glue.o" 2>"$W/cc2.log" \
  || { echo "FAIL $m compile-glue"; exit 1; }
def=$( nm "$W/mod.o" "$W/glue.o" "$RT" 2>/dev/null | awk '/ [TDBRG] /{print $3}' | sed 's/^_//' | sort -u )
undef=$( nm "$W/mod.o" "$W/glue.o" 2>/dev/null | awk '/ U /{print $2}' | sed 's/^_//' | grep -E '^sub_' | sort -u )
{ echo "typedef unsigned u32;"; for u in $(comm -23 <(echo "$undef") <(echo "$def")); do echo "u32 $u(){return 0;}"; done; } > "$W/stub.c"
gcc -std=gnu89 -m32 -O0 -w -shared -static-libgcc \
  -Wl,--image-base=0x60000000 -Wl,--major-os-version=4 -Wl,--major-subsystem-version=4 -Wl,--minor-subsystem-version=0 \
  -o "$final" "$W/glue.o" "$W/mod.o" "$W/stub.c" "$RT" "$W/${s}.def" -lgcc 2>"$W/link.log" \
  || { echo "FAIL $m link"; grep -iE 'error|undefined' "$W/link.log" | head; exit 1; }
python3 "$TOOLS/embed_rsrc.py" "$final" "$WT_DATA_ROOT/disc/staged/bin/$m" >/dev/null 2>&1 || true
echo "OK   $m -> $up $(stat -c%s "$final") b (mingw${CLEAN_FLAG:+ clean})"
rm -rf "$W"
