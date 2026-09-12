#!/usr/bin/env bash
# clean_sweep.sh -- corpus scale-up: for each module, run the memory oracle to a manifest, then the
# UNIFIED clean_promote (whole-call-graph + memory fold-in + Tier-1 SDK signatures) -> genuine,
# oracle-verified, deregisterized Windows 95 source per module. Emits build/clean/<MOD>_clean.c.
#   clean_sweep.sh OUTDIR MODULE.DLL [MODULE.DLL ...]     (run from the win95 data root)
set -uo pipefail
T="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")" && pwd)"
OUT="$1"; shift
MM="$OUT/sweep.mem_manifest.txt"
mkdir -p "$OUT"; : > "$MM"
tot=0
printf "%-16s %8s %8s %8s %8s\n" MODULE genuine graph mem tier1
for m in "$@"; do
  # 1) memory oracle -> append this module's memory-verified offsets to the shared manifest
  timeout 400 python3 "$T/memdifftest.py" "$m" --clean --manifest "$MM" >/dev/null 2>&1
  # 2) unified promotion (folds the memory set in)
  line=$(timeout 900 python3 "$T/clean_promote.py" "$m" --out "$OUT" --mem-manifest "$MM" 2>&1 | grep -oE 'oracle-verified\): [0-9]+.*fallback: [0-9]+')
  g=$(echo "$line" | grep -oE 'oracle-verified\): [0-9]+' | grep -oE '[0-9]+')
  gr=$(echo "$line" | grep -oE 'closures: [0-9]+' | grep -oE '[0-9]+')
  me=$(echo "$line" | grep -oE 'memory-oracle: [0-9]+' | grep -oE '[0-9]+')
  t1=$(echo "$line" | grep -oE 'exports: [0-9]+' | grep -oE '[0-9]+')
  printf "%-16s %8s %8s %8s %8s\n" "$m" "${g:-ERR}" "${gr:-0}" "${me:-0}" "${t1:-0}"
  tot=$((tot + ${g:-0}))
done
echo "== SWEEP TOTAL genuine oracle-verified functions: $tot across $# modules -> $OUT =="
