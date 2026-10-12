#!/usr/bin/env bash
# ============================================================================
# CodeBreaker environment -- the ONE place for every path.
# Source it before any tool or pipeline script:   source env.sh
# CodeBreaker is the unified RE + Win95-reconstruction tooling repo; the Windows
# 95 SOURCE TREE lives in a separate, clean repo (WT_WIN95_ROOT).
# ============================================================================

# repo root (this file's directory), resolved absolutely
export PROJECT="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")" && pwd)"

# --- the RECON (tooling + data + toolchain + vm + build) ROOT ----------------
# The git-ignored data home (corpus + toolchain + vm + build) lives at binaries/; the RECON
# var name is kept for back-compat with pipeline scripts. Fall back to the repo root.
if [ -d "$PROJECT/binaries" ]; then
  export RECON="$PROJECT/binaries"
else
  export RECON="$PROJECT"
fi

# --- the DATA ROOT (WT_DATA_ROOT) -------------------------------------------
if   [ -d "$RECON/data" ];   then export WT_DATA_ROOT="$RECON"
elif [ -d "$PROJECT/data" ]; then export WT_DATA_ROOT="$RECON"
else                              export WT_DATA_ROOT="$RECON"
fi

# --- the CodeBreaker engine (this repo) -------------------------------------
export CODEBREAKER="$PROJECT"
export PYTHONPATH="$CODEBREAKER${PYTHONPATH:+:$PYTHONPATH}"
export TOOLS="$CODEBREAKER/archive/engines/recompile/tools"     # archived: package_ff, memdifftest, clean_*, triage, il
export INC="$CODEBREAKER/archive/engines/recompile/include"     # archived: win95c.h, win95rt.c

# --- the reconstructed WIN95 SOURCE TREE (separate, clean repo) --------------
# The published Win95 source tree; win95tree.py `place` files reconstructed
# source into it. Override WT_WIN95_ROOT if your checkout lives elsewhere.
# Default: a Windows95 checkout sitting beside this repo (../Windows95).
if [ -n "${WT_WIN95_ROOT:-}" ] && [ -d "$WT_WIN95_ROOT" ]; then
  : # honor an explicit override
elif [ -d "$(dirname "$PROJECT")/Windows95/base" ]; then
  export WT_WIN95_ROOT="$(dirname "$PROJECT")/Windows95"
fi

# --- toolchains -------------------------------------------------------------
export MINGW32="C:/msys64/mingw32/bin"   # i686 gcc (32-bit FF DLLs)
export MINGW64="C:/msys64/mingw64/bin"   # x86_64 gcc (native oracle harness)
export MSVC="$RECON/toolchains/period/compilers/msvc-2.0"    # MSVC 2.0 (Win95's own compiler, ABI-exact)

# --- pipeline + build tree --------------------------------------------------
export PIPELINE="$WT_DATA_ROOT/pipeline"
export BUILD="$RECON/work/build"

# --- VM ---------------------------------------------------------------------
export VM="$RECON/vms"                      # the VMware VM dir -- boot vm/Windows95_FF.vmx in VMware
#   vm/Windows95_FF.vmx  = FF test VM (boots win95_ff_final-flat.vmdk that build_safe writes)
#   vm/Windows95.vmx     = pristine Track-A VM (boots win95full.vmdk -- keep pristine)

# echo "env: WT_DATA_ROOT=$WT_DATA_ROOT  WT_WIN95_ROOT=$WT_WIN95_ROOT  RECON=$RECON"

# --- per-era data roots (win1.0/win3.1/win98 extracted to their OWN repos) ----
# wt --target <era> resolves these via WT_<ERA>_DATA; win95 stays under binaries/.
# Default: era checkouts sitting beside this repo (../win1.0, ../win3.1, ../win98).
for _e in win1.0:WIN1_0 win3.1:WIN3_1 win98:WIN98; do
  _dir="$(dirname "$PROJECT")/${_e%%:*}"
  [ -d "$_dir" ] && export "WT_${_e##*:}_DATA=$_dir"
done; unset _e _dir
