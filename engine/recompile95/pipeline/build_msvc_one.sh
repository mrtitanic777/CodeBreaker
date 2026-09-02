#!/usr/bin/env bash
# Build ONE Win95 FF DLL via MSVC 2.0 (cl/link). Usage: build_msvc_one.sh MODULE.DLL [outdir]
# Places the finished DLL into the FF corpus dll dir (default) so build_safe swaps it.
set -uo pipefail
m="$1"
REPO="${REPO:-C:/Users/mrtit/Documents/GitHub/CodeBreaker}"
export WT_DATA_ROOT="${WT_DATA_ROOT:-$REPO/binaries/win95}"
T="C:/Users/mrtit/Documents/GitHub/CodeBreaker/engine/recompile95/tools"
INCU="C:/Users/mrtit/Documents/GitHub/CodeBreaker/engine/recompile95/include"
INC="C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker\\engine\\recompile95\\include"
MSVC="${MSVC:-$REPO/binaries/toolchains/period/compilers/msvc-2.0}"
MSVCW="C:\\Users\\mrtit\\Documents\\GitHub\\Windows95\\toolchain\\msvc20"
DLLDIR="${2:-$REPO/binaries/work/build/ffcorpus/dll}"
up=$(echo "$m" | tr '[:lower:]' '[:upper:]'); s=$(echo "$m" | sed 's/\./_/')
W="$REPO/binaries/work/build/msvc_build/$s"; rm -rf "$W"; mkdir -p "$W" "$DLLDIR"

# 1) generate MSVC sources  (CLEAN=1 -> boot-ceiling deregisterized + thiscall-recovered emission;
#    VERIFIED=<manifest> -> oracle-gate: only proven-bit-identical reg fns go clean, rest byte-exact)
# MUST run from the win95 DATA ROOT: package_ff's CLEAN fresh-lifts read asm via layout.asm_module_dir
# which is CWD-relative -- from the wrong cwd the asm isn't found and EVERY fn wrongly falls back to
# byte-exact. (--out/--verified are absolute, so cd is safe.)
CLEAN_FLAG=""; [ "${CLEAN:-0}" = 1 ] && CLEAN_FLAG="--clean"
VER_FLAG=""; [ -n "${VERIFIED:-}" ] && VER_FLAG="--verified ${VERIFIED}"
( cd "$WT_DATA_ROOT" && PATH="/c/msys64/mingw32/bin:$PATH" python3 "$T/package_ff.py" "$m" --out "$W" --msvc $CLEAN_FLAG $VER_FLAG ) >"$W/pf.log" 2>&1 || { echo "FAIL $m package_ff"; exit 1; }
cp "$INCU/win95rt.c" "$W/win95rt.c"

export PATH="$MSVC/BIN:$PATH"; export INCLUDE="$INC;$MSVCW\\INCLUDE"; export LIB="$MSVCW\\LIB"
export MSYS2_ARG_CONV_EXCL='*'
cd "$W"
# 2) compile the three TUs
for f in ${s}_reloc win95rt ${s}_glue; do
  "$MSVC/BIN/CL.EXE" /c /Od $f.c >$f.cl.log 2>&1
  [ -f $f.obj ] || { echo "FAIL $m compile $f"; grep -E 'error|fatal' $f.cl.log|head -3; exit 1; }
done
# 3) link, gather unresolved sub_, stub, relink
libs="kernel32.lib user32.lib advapi32.lib gdi32.lib mpr.lib shell32.lib ole32.lib comctl32.lib version.lib"
"$MSVC/BIN/LINK.EXE" /DLL /NOLOGO /BASE:0x60000000 /SUBSYSTEM:WINDOWS,4.00 /DEF:${s}.def \
  /OUT:$up ${s}_reloc.obj ${s}_glue.obj win95rt.obj $libs >link1.log 2>&1 || true
grep -oE '_sub_[0-9a-f]+' link1.log 2>/dev/null | sed 's/^_//' | sort -u > undef.txt
if [ -s undef.txt ]; then
  { echo "typedef unsigned u32;"; awk '{print "u32 "$1"(){return 0;}"}' undef.txt; } > stub.c
  "$MSVC/BIN/CL.EXE" /c /Od stub.c >stub.cl.log 2>&1
  "$MSVC/BIN/LINK.EXE" /DLL /NOLOGO /BASE:0x60000000 /SUBSYSTEM:WINDOWS,4.00 /DEF:${s}.def \
    /OUT:$up ${s}_reloc.obj ${s}_glue.obj win95rt.obj stub.obj $libs >link2.log 2>&1 || true
fi
[ -f "$W/$up" ] || { echo "FAIL $m link"; grep -iE 'error|unresolved|LNK' link2.log link1.log|head -6; exit 1; }
# 4) graft resources + patch OS version to 4.0
PATH="/c/msys64/mingw32/bin:$PATH" python3 "$T/embed_rsrc.py" "$W/$up" "$WT_DATA_ROOT/disc/staged/bin/$m" >/dev/null 2>&1 || true
PATH="/c/msys64/mingw32/bin:$PATH" python3 -c "
import struct
p=r'$W/$up'; b=bytearray(open(p,'rb').read()); e=struct.unpack_from('<I',b,0x3c)[0]
struct.pack_into('<H',b,e+24+64,4); struct.pack_into('<H',b,e+24+72,4)
open(p,'wb').write(b)
"
cp "$W/$up" "$DLLDIR/$up"
echo "OK   $m -> MSVC $up $(stat -c%s "$W/$up") b"
