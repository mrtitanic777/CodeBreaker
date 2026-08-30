#!/usr/bin/env bash
# clean_sweep_mt.sh -- BATCHED parallel corpus scale-up. Launch a batch of JOBS modules, WAIT for
# the whole batch to finish, then the next batch, etc. (bounded contention, clean boundaries). Each
# module: memory oracle -> its own manifest, then unified clean_promote (whole-graph + memory +
# Tier-1 sigs) -> build/clean/<mod>_clean.c; per-module logs in <OUT>/logs. Naming runs at the end.
#   JOBS=6 clean_sweep_mt.sh OUTDIR MODULE [MODULE ...]        (run from the win95 data root)
set -uo pipefail
T="C:/Users/mrtit/Documents/GitHub/CodeBreaker/engine/recompile95/tools"
OUT="$1"; shift
JOBS="${JOBS:-6}"
mkdir -p "$OUT" "$OUT/logs" "$OUT/manifests"
export T OUT

do_one() {
  m="${1%$'\r'}"; [ -z "$m" ] && return 0
  # RESUMABLE: skip modules a prior run already COMPLETED (clean_promote's final line is in the log).
  # Stop the sweep any time; re-launch the SAME command and it picks up where it left off.
  # Force a rebuild of everything with FORCE=1.
  if [ "${FORCE:-0}" != 1 ] && grep -q 'GENUINE clean C' "$OUT/logs/${m}.log" 2>/dev/null; then
    g=$(grep -oE 'oracle-verified\): [0-9]+' "$OUT/logs/${m}.log" | grep -oE '[0-9]+' | head -1)
    printf "  %-18s SKIP (already done, genuine=%s)\n" "$m" "${g:-?}"
    return 0
  fi
  MM="$OUT/manifests/${m}.mem.txt"; : > "$MM"    # keep manifests out of the source tree
  timeout 600 python3 "$T/memdifftest.py"  "$m" --clean --graph --manifest "$MM" >"$OUT/logs/${m}.mem.log" 2>&1
  timeout 900 python3 "$T/clean_promote.py" "$m" --out "$OUT" --mem-manifest "$MM" >"$OUT/logs/${m}.log" 2>&1
  g=$(grep -oE 'oracle-verified\): [0-9]+' "$OUT/logs/${m}.log" | grep -oE '[0-9]+' | head -1)
  printf "  %-18s genuine=%s\n" "$m" "${g:-ERR}"
}
export -f do_one

mods=("$@"); total=${#mods[@]}; idx=0; b=0
echo "== BATCHED sweep: $total modules, batch size JOBS=$JOBS =="
while [ $idx -lt $total ]; do
  b=$((b+1))
  batch=("${mods[@]:idx:JOBS}")
  echo "-- batch $b: ${#batch[@]} modules ($((idx+1))-$((idx+${#batch[@]})) / $total) --"
  for m in "${batch[@]}"; do do_one "$m" & done
  wait                                    # wait for the WHOLE batch before the next
  idx=$((idx+JOBS))
done
echo "== naming layer =="
python3 "$T/clean_name.py" "$OUT" 2>&1 | tail -1
tot=$(for f in "$OUT"/*_clean.c; do grep -c '^/\* promoted' "$f" 2>/dev/null; done | paste -sd+ | bc 2>/dev/null)
echo "== SWEEP DONE: ~$tot genuine oracle-verified functions across $(ls "$OUT"/*_clean.c 2>/dev/null | wc -l) modules -> $OUT =="
