"""Xbox 360 eFuse and CPU key tools."""

from struct import pack, unpack_from

def set_fuseline(fuse_data: bytes, line: int, value) -> bytearray:
    """Set a fuse line (8 bytes) in fuse data."""
    data = bytearray(fuse_data)

    if isinstance(value, int):
        value = pack(">Q", value)
    elif isinstance(value, str):
        assert len(value) == 16, "Invalid fuse line (need 16 hex chars)"
        value = bytes.fromhex(value)

    assert len(value) == 8, "Invalid fuse line (need 8 bytes)"
    data[(line * 8):(line * 8) + 8] = value
    return data

def get_fuseline(fuse_data: bytes, line: int) -> bytes:
    """Get a fuse line (8 bytes) from fuse data."""
    return fuse_data[(line * 8):(line * 8) + 8]

def get_cpu_key_from_fuses(fuse_data: bytes) -> bytes:
    """Extract CPU key from fuse data (lines 3-4)."""
    return fuse_data[0x18:0x28]

def parse_fuses(fuse_data: bytes) -> dict:
    """Parse fuse data into structured info."""
    info = {}
    if len(fuse_data) < 0x60:  # 12 fuse lines × 8 bytes
        return {"error": "Fuse data too small"}

    info["fuseline_00"] = fuse_data[0x00:0x08].hex().upper()
    info["fuseline_01"] = fuse_data[0x08:0x10].hex().upper()
    info["fuseline_02"] = fuse_data[0x10:0x18].hex().upper()
    info["cpu_key"] = fuse_data[0x18:0x28].hex().upper()  # lines 3-4
    info["fuseline_05"] = fuse_data[0x28:0x30].hex().upper()
    info["fuseline_06"] = fuse_data[0x30:0x38].hex().upper()
    info["fuseline_07"] = fuse_data[0x38:0x40].hex().upper()
    info["fuseline_08"] = fuse_data[0x40:0x48].hex().upper()
    info["fuseline_09"] = fuse_data[0x48:0x50].hex().upper()
    info["fuseline_10"] = fuse_data[0x50:0x58].hex().upper()
    info["fuseline_11"] = fuse_data[0x58:0x60].hex().upper()

    # Lock Down Value (LDV) — indicates how many updates have been applied
    ldv = 0
    for byte in fuse_data[0x30:0x40]:  # fuselines 6-7
        for bit in range(8):
            if byte & (1 << bit):
                ldv += 1
    info["ldv"] = ldv

    return info


DESCRIPTION = "parse an Xbox 360 eFuse dump (fuselines, CPU key, retail/devkit)"


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: fuses <fuse-dump>"); return
    data = open(a[0], "rb").read()
    info = parse_fuses(data)
    for k, v in info.items():
        print("  %-20s %s" % (k, v))
    try:
        ck = get_cpu_key_from_fuses(data)
        if ck:
            print("  %-20s %s" % ("cpu_key", ck.hex().upper() if isinstance(ck, (bytes, bytearray)) else ck))
    except Exception:
        pass


if __name__ == "__main__":
    main()
