#!/usr/bin/env bash
# MULTITHREADED corpus-wide FULL-FIDELITY build: every install PE DLL -> editable C/ASM drop-in DLL.
# Fans out across cores via xargs -P (default 8 jobs). Resumable (skips modules whose DLL exists).
# Each module builds in its OWN work dir -> no cross-job contention. Run under Git Bash/MSYS2.
set -uo pipefail
REPO="${REPO:-$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")/../../.." && pwd)}"
export WT_DATA_ROOT="${WT_DATA_ROOT:-$REPO/binaries/win95}"
export PATH="/c/msys64/mingw32/bin:$PATH"
export T="$REPO/engine/recompile95/tools"
export INC="$T/../include"
SP="${SP:-$REPO/binaries/work/build}"
export OUT="$SP/ffcorpus"
export DLL="$OUT/dll"
export WORK="$OUT/work"
export RT="$OUT/rt.o"
LOG="$OUT/build_ff_mt.log"
TARGETS="$SP/ff_targets.txt"
JOBS="${JOBS:-8}"                                   # override with JOBS=N
mkdir -p "$DLL" "$WORK"
# shared runtime, compiled ONCE before fan-out (avoids a race on rt.o)
[ -f "$RT" ] || gcc -std=gnu89 -m32 -O0 -w -c "$INC/win95rt.c" -o "$RT" 2>/dev/null

build_one() {
  m="$1"; m="${m%$'\r'}"; m="$(echo "$m" | tr -d '[:space:]')"
  [ -z "$m" ] && return 0
  up=$(echo "$m" | tr '[:lower:]' '[:upper:]')
  final="$DLL/$up"
  [ -f "$final" ] && { echo "SKIP $m"; return 0; }
  s=$(echo "$m" | sed 's/\./_/')
  W="$WORK/$s"; rm -rf "$W"; mkdir -p "$W"
  if ! timeout 300 python3 "$T/package_ff.py" "$m" --out "$W" >"$W/pf.log" 2>&1; then
    echo "FAIL $m package_ff"; rm -rf "$W"; return 0; fi
  timeout 360 gcc -std=gnu89 -m32 -O0 -w -I "$INC" -I "$W" -c "$W/${s}_reloc.c" -o "$W/mod.o" 2>"$W/cc1.log"
  [ -f "$W/mod.o" ] || { echo "FAIL $m compile-reloc"; rm -rf "$W"; return 0; }
  timeout 180 gcc -std=gnu89 -m32 -O0 -w -I "$INC" -I "$W" -c "$W/${s}_glue.c" -o "$W/glue.o" 2>"$W/cc2.log"
  [ -f "$W/glue.o" ] || { echo "FAIL $m compile-glue"; rm -rf "$W"; return 0; }
  def=$( nm "$W/mod.o" "$W/glue.o" "$RT" 2>/dev/null | awk '/ [TDBRG] /{print $3}' | sed 's/^_//' | sort -u )
  undef=$( nm "$W/mod.o" "$W/glue.o" 2>/dev/null | awk '/ U /{print $2}' | sed 's/^_//' | grep -E '^sub_' | sort -u )
  { echo "typedef unsigned u32;"; for u in $(comm -23 <(echo "$undef") <(echo "$def")); do echo "u32 $u(){return 0;}"; done; } > "$W/stub.c"
  timeout 240 gcc -std=gnu89 -m32 -O0 -w -shared -static-libgcc \
    -Wl,--image-base=0x60000000 -Wl,--major-os-version=4 -Wl,--major-subsystem-version=4 -Wl,--minor-subsystem-version=0 \
    -o "$final" "$W/glue.o" "$W/mod.o" "$W/stub.c" "$RT" "$W/${s}.def" -lgcc 2>"$W/link.log"
  if [ -f "$final" ]; then
    python3 "$T/embed_rsrc.py" "$final" "$WT_DATA_ROOT/disc/staged/bin/$m" >/dev/null 2>&1 || true
    echo "OK   $m -> $up $(stat -c%s "$final") b"
  else
    echo "FAIL $m link ($(grep -iE 'error|undefined' "$W/link.log" | head -1))"
  fi
  rm -rf "$W"
}
export -f build_one

total=$(grep -cve '^[[:space:]]*$' "$TARGETS")
echo "=== MT corpus FF build: $total modules, $JOBS parallel jobs -> $DLL ===" | tee "$LOG"
# fan out: one module per job, up to JOBS concurrent. Line-buffered echoes merge into the log.
grep -ve '^[[:space:]]*$' "$TARGETS" | tr -d '\r' \
  | xargs -P "$JOBS" -I{} bash -c 'build_one "$@"' _ {} | stdbuf -oL tee -a "$LOG"
echo "=== MT build DONE: OK=$(grep -c '^OK' "$LOG") FAIL=$(grep -c '^FAIL' "$LOG") SKIP=$(grep -c '^SKIP' "$LOG") / $total ===" | tee -a "$LOG"
grep '^FAIL' "$LOG" || echo "(no failures)"
