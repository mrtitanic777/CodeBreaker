#!/usr/bin/env bash
# MULTITHREADED corpus FF build via MSVC 2.0. Fans build_msvc_one over ff_targets.txt.
# Resumable (skips modules already present in the MSVC output dir). Run under Git Bash/MSYS2.
set -uo pipefail
REPO="${REPO:-$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")/../../.." && pwd)}"
SP="${SP:-$REPO/binaries/work/build}"
export DLLDIR="${DLLDIR:-$SP/ffcorpus_msvc/dll}"          # separate dir so we don't clobber mingw corpus
export ONE="${ONE:-$REPO/binaries/win95/pipeline/build_msvc_one.sh}"
TARGETS="${TARGETS:-$REPO/binaries/win95/pipeline/ff_targets.txt}"
JOBS="${JOBS:-6}"
LOG="$SP/build_ff_msvc.log"
mkdir -p "$DLLDIR"

build_it() {
  m="${1%$'\r'}"; m="$(echo "$m" | tr -d '[:space:]')"; [ -z "$m" ] && return 0
  up=$(echo "$m" | tr '[:lower:]' '[:upper:]')
  [ -f "$DLLDIR/$up" ] && { echo "SKIP $m"; return 0; }
  bash "$ONE" "$m" "$DLLDIR" 2>/dev/null || echo "FAIL $m"
}
export -f build_it

total=$(grep -cve '^[[:space:]]*$' "$TARGETS")
echo "=== MSVC corpus build: $total modules, $JOBS jobs -> $DLLDIR ===" | tee "$LOG"
grep -ve '^[[:space:]]*$' "$TARGETS" | tr -d '\r' \
  | xargs -P "$JOBS" -I{} bash -c 'build_it "$@"' _ {} | tee -a "$LOG"
echo "=== MSVC build DONE: OK=$(grep -c '^OK' "$LOG") FAIL=$(grep -c '^FAIL' "$LOG") SKIP=$(grep -c '^SKIP' "$LOG") / $total ===" | tee -a "$LOG"
grep '^FAIL' "$LOG" | head -20 || echo "(no failures)"
