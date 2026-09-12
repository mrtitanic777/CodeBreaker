#!/usr/bin/env bash
# Boot a fully reconstructed Windows 95 32-bit kernel + driver stack.
#
# Clones a Win95 disk image and replaces its vmm32.vxd with the reconstructed VMM-only W3 container,
# then drops every OTHER reconstructed VxD into \WINDOWS\SYSTEM\VMM32 as a standalone file. Win95's
# loader accepts an uncompressed W3 vmm32.vxd, and for any registry-listed static VxD that is not in
# the bundle it falls back to a standalone VMM32\<name>.vxd -- so the machine-specific (compressed W4)
# bundle is fully replaceable by the reconstructions. The guest's OWN newer standalone drivers (e.g.
# an updated IOS/NTKERN/VFBACKUP that post-date the reconstruction oracle) are KEPT, not overwritten
# -- overwriting a newer IOS with an older one throws a Windows protection error during IOS init.
#
#   inject_and_boot.sh  SRC.vmdk  STAGE_DIR  OUT.vmdk
#
#     SRC.vmdk    a Win95 VMware disk (READ ONLY -- never modified)
#     STAGE_DIR   the reconstructed VxDs from build_vmm32.py (must contain VMM32.VXD)
#     OUT.vmdk    the clone to write and boot
#
# Requires Linux/WSL with qemu-img + mtools (mcopy/mdir/sfdisk). Boot OUT.vmdk in your VM.
set -euo pipefail
[ $# -eq 3 ] || { sed -n '2,20p' "$0"; exit 2; }
SRC="$1"; STAGE="$2"; OUT="$3"
[ -f "$STAGE/VMM32.VXD" ] || { echo "error: $STAGE has no VMM32.VXD (run build_vmm32.py first)"; exit 1; }

RAW="$(mktemp --suffix=.raw)"
trap 'rm -f "$RAW"' EXIT
echo "[1/4] clone $SRC -> raw"
qemu-img convert -f vmdk -O raw "$SRC" "$RAW"
SEC="$(sfdisk -d "$RAW" 2>/dev/null | sed -n 's/.*start=[[:space:]]*\([0-9][0-9]*\).*/\1/p' | awk '$1>0{print;exit}')"
[ -n "$SEC" ] || { echo "error: no FAT partition found"; exit 1; }
IMG="$RAW@@$(( SEC * 512 ))"

echo "[2/4] add reconstructed VxDs into VMM32\\ (keeping the guest's own newer standalone drivers)"
add=0; kept=0
for f in "$STAGE"/*.VXD; do
  fn="$(basename "$f")"
  [ "$fn" = "VMM32.VXD" ] && continue
  if mdir -i "$IMG" "::/WINDOWS/SYSTEM/VMM32/$fn" >/dev/null 2>&1; then
    kept=$((kept + 1))
  else
    mcopy -i "$IMG" "$f" "::/WINDOWS/SYSTEM/VMM32/$fn"; add=$((add + 1))
  fi
done
echo "    added $add reconstructed VxDs, kept $kept of the guest's own"

echo "[3/4] replace the vmm32.vxd bundle with the reconstructed VMM-only W3"
mcopy -o -i "$IMG" "$STAGE/VMM32.VXD" "::/WINDOWS/SYSTEM/VMM32.VXD"

echo "[4/4] raw -> $OUT"
rm -rf "$OUT.lck"
qemu-img convert -f raw -O vmdk "$RAW" "$OUT"
echo "done -- boot $OUT to run the reconstructed kernel + driver stack"
