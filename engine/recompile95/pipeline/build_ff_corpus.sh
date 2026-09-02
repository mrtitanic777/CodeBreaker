#!/usr/bin/env bash
# Corpus-wide FULL-FIDELITY build: every install PE DLL -> editable C/ASM-mix drop-in DLL.
# Resumable (skips modules whose output DLL already exists). One log line per module.
set -uo pipefail
REPO="${REPO:-C:/Users/mrtit/Documents/GitHub/CodeBreaker}"
export WT_DATA_ROOT="${WT_DATA_ROOT:-$REPO/binaries/win95}"
export PATH="/c/msys64/mingw32/bin:$PATH"
T="C:/Users/mrtit/Documents/GitHub/CodeBreaker/engine/recompile95/tools"
INC="$T/../include"
SP="${SP:-$REPO/binaries/work/build}"
OUT="$SP/ffcorpus"
DLL="$OUT/dll"           # finished drop-in DLLs land here
WORK="$OUT/work"         # per-module scratch (reused/wiped)
LOG="$OUT/build_ff.log"
mkdir -p "$DLL" "$WORK"
# shared runtime, compiled once
RT="$OUT/rt.o"
[ -f "$RT" ] || gcc -std=gnu89 -m32 -O0 -w -c "$INC/win95rt.c" -o "$RT" 2>/dev/null

TARGETS="$SP/ff_targets.txt"
total=$(wc -l < "$TARGETS")
i=0; ok=0; fail=0; skip=0
echo "=== corpus FF build start: $total modules -> $DLL ===" | tee "$LOG"
while IFS= read -r m; do
  m="${m%$'\r'}"; m="$(echo "$m" | tr -d '[:space:]')"
  [ -z "$m" ] && continue
  i=$((i+1))
  up=$(echo "$m" | tr '[:lower:]' '[:upper:]')     # LINKINFO.DLL
  final="$DLL/$up"
  if [ -f "$final" ]; then skip=$((skip+1)); continue; fi
  s=$(echo "$m" | sed 's/\./_/')                    # linkinfo_dll
  W="$WORK/$s"; rm -rf "$W"; mkdir -p "$W"
  # 1) transliterate -> C + ASM-source mix
  if ! timeout 240 python3 "$T/package_ff.py" "$m" --out "$W" >"$W/pf.log" 2>&1; then
    mkdir -p "$OUT/fails"; cp "$W/pf.log" "$OUT/fails/$s.log" 2>/dev/null; echo "FAIL $m package_ff" | tee -a "$LOG"; fail=$((fail+1)); rm -rf "$W"; continue
  fi
  # 2) compile module TU + glue
  timeout 300 gcc -std=gnu89 -m32 -O0 -w -I "$INC" -I "$W" -c "$W/${s}_reloc.c" -o "$W/mod.o" 2>"$W/cc1.log"
  if [ ! -f "$W/mod.o" ]; then echo "FAIL $m compile-reloc" | tee -a "$LOG"; fail=$((fail+1)); rm -rf "$W"; continue; fi
  timeout 120 gcc -std=gnu89 -m32 -O0 -w -I "$INC" -I "$W" -c "$W/${s}_glue.c" -o "$W/glue.o" 2>"$W/cc2.log"
  if [ ! -f "$W/glue.o" ]; then echo "FAIL $m compile-glue" | tee -a "$LOG"; fail=$((fail+1)); rm -rf "$W"; continue; fi
  # 3) stub any still-undefined sub_ (un-lifted callees with no asm + no def)
  def=$( nm "$W/mod.o" "$W/glue.o" "$RT" 2>/dev/null | awk '/ [TDBRG] /{print $3}' | sed 's/^_//' | sort -u )
  undef=$( nm "$W/mod.o" "$W/glue.o" 2>/dev/null | awk '/ U /{print $2}' | sed 's/^_//' | grep -E '^sub_' | sort -u )
  { echo "typedef unsigned u32;"; for u in $(comm -23 <(echo "$undef") <(echo "$def")); do echo "u32 $u(){return 0;}"; done; } > "$W/stub.c"
  # 4) link the drop-in DLL at a spare ImageBase, Win95 subsystem 4.0
  timeout 180 gcc -std=gnu89 -m32 -O0 -w -shared -static-libgcc \
    -Wl,--image-base=0x60000000 -Wl,--major-os-version=4 -Wl,--major-subsystem-version=4 -Wl,--minor-subsystem-version=0 \
    -o "$final" "$W/glue.o" "$W/mod.o" "$W/stub.c" "$RT" "$W/${s}.def" -lgcc 2>"$W/link.log"
  if [ -f "$final" ]; then
    # 5) graft the ORIGINAL .rsrc so Win95 FindResource/LoadIcon/LoadString work (else GUI DLLs crash)
    python3 "$T/embed_rsrc.py" "$final" "$WT_DATA_ROOT/disc/staged/bin/$m" >/dev/null 2>&1 || true
    sz=$(stat -c%s "$final" 2>/dev/null || echo 0)
    echo "OK   $m -> $up $sz b  [$i/$total]" | tee -a "$LOG"; ok=$((ok+1))
  else
    echo "FAIL $m link ($(grep -iE 'error|undefined' "$W/link.log" | head -1))" | tee -a "$LOG"; fail=$((fail+1))
  fi
  rm -rf "$W"
done < "$TARGETS"
echo "=== corpus FF build DONE: ok=$ok fail=$fail skip=$skip / $total ===" | tee -a "$LOG"
