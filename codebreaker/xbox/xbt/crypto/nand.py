"""NAND dump analysis and extraction tools."""

from pathlib import Path
from struct import unpack_from

# NAND page sizes
SMALL_PAGE_SIZE = 0x200
SMALL_SPARE_SIZE = 0x10
BIG_PAGE_SIZE = 0x800  # 2KB for big block
BIG_SPARE_SIZE = 0x10

# Magic values
MAGIC_NAND_HEADER = b"\xFF\x4F"

# HV + kernel layout
HV_SIZE = 0x40000  # 256 KB
KERNEL_OFFSET_IN_UPDATE = 0x40000

def detect_nand_type(data: bytes) -> str:
    """Detect NAND type from dump size."""
    size = len(data)
    if size == 16 * 1024 * 1024:
        return "16MB (small block)"
    elif size == 64 * 1024 * 1024:
        return "64MB (big block / jasper)"
    elif size == 256 * 1024 * 1024:
        return "256MB (corona MMC)"
    elif size == 512 * 1024 * 1024:
        return "512MB (corona 4GB MMC)"
    else:
        return f"Unknown ({size} bytes)"

def extract_header_info(data: bytes) -> dict:
    """Extract basic NAND header information."""
    info = {}
    if len(data) < 0x80:
        return info

    # Magic
    info["magic"] = data[0:2].hex().upper()

    # Build/version info
    if len(data) >= 0x10:
        info["build"] = unpack_from(">H", data, 0x02)[0]
        info["qfe"] = unpack_from(">H", data, 0x04)[0]
        info["flags"] = unpack_from(">H", data, 0x06)[0]
        info["entry_point"] = unpack_from(">I", data, 0x08)[0]
        info["size"] = unpack_from(">I", data, 0x0C)[0]

    return info

def extract_hv_from_nand(data: bytes) -> bytes:
    """Extract the hypervisor (first 256KB) from a NAND dump."""
    if len(data) < HV_SIZE:
        raise ValueError(f"NAND dump too small ({len(data)} bytes, need {HV_SIZE})")
    return data[:HV_SIZE]

def extract_hv_from_update(update_data: bytes) -> tuple:
    """Extract HV and kernel from a system update file (xboxupd.bin applied kernel)."""
    if len(update_data) < HV_SIZE + 0x1000:
        raise ValueError("Update file too small")
    hv = update_data[:HV_SIZE]
    kernel = update_data[HV_SIZE:]
    return (hv, kernel)

def get_cb_offset(data: bytes) -> int:
    """Find the CB (2BL) bootloader offset in NAND."""
    if len(data) < 0x40:
        return -1
    # CB offset is typically at 0x08 in the NAND header
    offset = unpack_from(">I", data, 0x08)[0]
    if offset > 0 and offset < len(data):
        return offset
    return -1

def nand_info(path: str) -> dict:
    """Get comprehensive NAND dump info."""
    data = Path(path).read_bytes()
    info = {
        "path": path,
        "size": len(data),
        "type": detect_nand_type(data),
        "header": extract_header_info(data),
    }
    return info


DESCRIPTION = "inspect an Xbox 360 NAND dump (type, header, CB offset, HV extract)"


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: nand info <dump>  |  nand extract-hv <dump> <out.bin>"); return
    op = a[0]
    if op == "info" and len(a) > 1:
        for k, v in nand_info(a[1]).items():
            print("  %-20s %s" % (k, v))
    elif op in ("extract-hv", "hv") and len(a) > 2:
        data = open(a[1], "rb").read()
        hv = extract_hv_from_nand(data)
        open(a[2], "wb").write(hv)
        print("extracted HV (%d bytes) -> %s" % (len(hv), a[2]))
    else:
        print("usage: nand info <dump>  |  nand extract-hv <dump> <out.bin>"); sys.exit(2)


if __name__ == "__main__":
    main()
