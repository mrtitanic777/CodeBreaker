#!/usr/bin/env bash
# Robust, qemu-free safe build: swap only DLLs NOT in keep_list_nl.txt; keep the core byte-exact.
# Reads keep-list from a FILE (no shell-arg quoting bug). Verifies KERNEL32 stays original.
set -uo pipefail
export MTOOLS_SKIP_CHECK=1
REPO="${REPO:-$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")/../../.." && pwd)}"
VM="${VM:-$REPO/binaries/vms}"
SP="${SP:-$REPO/binaries/work/build}"
DLL="${DLL:-$SP/ffcorpus/dll}"
KEEP="${KEEP:-$REPO/binaries/win95/pipeline/keep_list_GOOD_213swap.txt}"   # one DLL name per line = kept byte-exact
tr -d '\r' < "$KEEP" > "$KEEP.lf" 2>/dev/null && KEEP="$KEEP.lf"   # CRLF-proof: strip \r so WSL grep -xF matches
PRIST="$HOME/pristine.raw"            # pristine ext4 raw
WORK="$HOME/work_safe.raw"
FLAT="$VM/win95_ff_final-flat.vmdk"   # VMware flat extent (descriptor win95_ff_final.vmdk already exists)
VDM="/mnt/c/Program Files (x86)/VMware/VMware Workstation/vmware-vdiskmanager.exe"

# 0) pristine ext4 raw (make once, qemu-free, via vmware-vdiskmanager flat -> cp)
if [ ! -f "$PRIST" ]; then
  echo "== making pristine ext4 raw (one-time) =="
  "$VDM" -r "$VM/win95full.vmdk" -t 2 "$VM/pristmp.vmdk" >/dev/null 2>&1
  cp "$VM/pristmp-flat.vmdk" "$PRIST"
  rm -f "$VM/pristmp.vmdk" "$VM/pristmp-flat.vmdk"
fi
echo "== work = pristine =="
cp "$PRIST" "$WORK"

# 1) stage the SWAP set = FF DLLs whose name is NOT in keep list
rm -rf "$HOME/swapdll"; mkdir -p "$HOME/swapdll"
kept=0; swap=0
for f in "$DLL"/*.DLL; do
  b=$(basename "$f")
  if grep -qxF "$b" "$KEEP"; then kept=$((kept+1)); continue; fi
  cp "$f" "$HOME/swapdll/"; swap=$((swap+1))
done
echo "== keeping $kept byte-exact; swapping $swap FF DLLs =="

# 2) clear read-only + batched mcopy (ext4 = fast)
cd "$HOME/swapdll"
timeout 120 mattrib -r -i "$WORK@@32256" ::/WINDOWS/SYSTEM/*.DLL 2>/dev/null
timeout 600 mcopy -o -i "$WORK@@32256" *.DLL ::/WINDOWS/SYSTEM/ < /dev/null 2>&1 | tail -1

# 3) AutoScan=0 via known-good MSDOS.SYS
mattrib -h -s -r -i "$WORK@@32256" ::/MSDOS.SYS 2>/dev/null
mcopy -o -i "$WORK@@32256" "$HOME/msdos.new" ::/MSDOS.SYS 2>/dev/null
mattrib +h +s +r -i "$WORK@@32256" ::/MSDOS.SYS 2>/dev/null

# 4) VERIFY the core stayed byte-exact (KERNEL32 must be the ORIGINAL 414208, not FF 1197230)
K=$(mdir -i "$WORK@@32256" ::/WINDOWS/SYSTEM/KERNEL32.DLL 2>&1 | grep -i '1 file' | grep -oE '[0-9 ]+bytes' | tr -d ' bytes')
G=$(mdir -i "$WORK@@32256" ::/WINDOWS/SYSTEM/GDI32.DLL 2>&1 | grep -i '1 file' | grep -oE '[0-9 ]+bytes' | tr -d ' bytes')
S=$(mdir -i "$WORK@@32256" ::/WINDOWS/SYSTEM/SHELL32.DLL 2>&1 | grep -i '1 file' | grep -oE '[0-9 ]+bytes' | tr -d ' bytes')
A=$(mdir -i "$WORK@@32256" ::/WINDOWS/SYSTEM/ADVPACK.DLL 2>&1 | grep -i '1 file' | grep -oE '[0-9 ]+bytes' | tr -d ' bytes')
echo "VERIFY KERNEL32=$K GDI32=$G SHELL32=$S (expect ORIGINAL) ; ADVPACK=$A (expect FF 165217)"
if [ "$K" = "414208" ] && [ "$A" != "88336" ] && [ -n "$A" ]; then
  echo "== CORE BYTE-EXACT OK -> writing VMware flat =="
  cp "$WORK" "$FLAT"
  echo "SAFE_BUILD_OK"
else
  echo "SAFE_BUILD_FAIL (core not preserved) -- NOT writing flat"
fi
