"""Xbox 360 bootchain stage parser.

Ported from xenon-bltool.
Parses and identifies bootloader stages from NAND dumps:
  CB (2BL), CB_B, CD (4BL), CE (5BL), CF (6BL), CG (7BL)

Each stage has a common 16-byte header:
  uint16 magic, uint16 version, uint16 pairing, uint16 flags,
  uint32 entrypoint, uint32 size
"""

import struct
import hashlib
from typing import Optional
from dataclasses import dataclass


# ── Bootloader types ────────────────────────────────────

BL_1BL = 0
BL_CB  = 1
BL_SC  = 2
BL_CD  = 3
BL_CE  = 4
BL_CF  = 5
BL_CG  = 6
BL_HV  = 0x10

BL_NAMES = {
    BL_1BL: "1BL", BL_CB: "CB/2BL", BL_SC: "SC/3BL",
    BL_CD: "CD/4BL", BL_CE: "CE/5BL", BL_CF: "CF/6BL",
    BL_CG: "CG/7BL", BL_HV: "HV",
}

# Magic values for bootloader identification
MAGIC_CB = 0x4342  # "CB"
MAGIC_CD = 0x4344  # "CD"
MAGIC_CE = 0x4345  # "CE"
MAGIC_CF = 0x4346  # "CF"
MAGIC_CG = 0x4347  # "CG"
MAGIC_HV = 0x4856  # "HV"

MAGIC_MAP = {
    MAGIC_CB: BL_CB, MAGIC_CD: BL_CD, MAGIC_CE: BL_CE,
    MAGIC_CF: BL_CF, MAGIC_CG: BL_CG, MAGIC_HV: BL_HV,
}


@dataclass
class BootloaderHeader:
    magic: int = 0
    version: int = 0
    pairing: int = 0
    flags: int = 0
    entrypoint: int = 0
    size: int = 0
    bl_type: int = -1
    bl_name: str = "Unknown"
    is_devkit: bool = False
    is_manufacturing: bool = False
    is_cba: bool = False
    key: bytes = b""
    offset_in_nand: int = 0


@dataclass
class BootchainInfo:
    """Full bootchain analysis result."""
    stages: list = None
    nand_size: int = 0
    header_magic: int = 0
    cb_version: int = 0
    cd_version: int = 0
    ce_version: int = 0

    def __post_init__(self):
        if self.stages is None:
            self.stages = []


# ── Parsing ─────────────────────────────────────────────

def parse_header(data: bytes, offset: int = 0) -> Optional[BootloaderHeader]:
    """Parse a 16-byte bootloader header."""
    if offset + 16 > len(data):
        return None

    h = BootloaderHeader()
    h.magic = struct.unpack_from(">H", data, offset)[0]
    h.version = struct.unpack_from(">H", data, offset + 2)[0]
    h.pairing = struct.unpack_from(">H", data, offset + 4)[0]
    h.flags = struct.unpack_from(">H", data, offset + 6)[0]
    h.entrypoint = struct.unpack_from(">I", data, offset + 8)[0]
    h.size = struct.unpack_from(">I", data, offset + 12)[0]
    h.offset_in_nand = offset

    h.bl_type = MAGIC_MAP.get(h.magic, -1)
    h.bl_name = BL_NAMES.get(h.bl_type, f"Unknown(0x{h.magic:04X})")
    h.is_devkit = bool(h.pairing & 0x8000)
    h.is_manufacturing = bool(h.flags & 0x0001)
    h.is_cba = bool(h.flags & 0x0800)

    # Key is at offset +16 for CB, CD, CE, CF, CG (16 bytes)
    if h.bl_type in (BL_CB, BL_CD, BL_CE, BL_CF, BL_CG) and offset + 32 <= len(data):
        h.key = bytes(data[offset + 16:offset + 32])

    return h


def _is_valid_header(h: BootloaderHeader) -> bool:
    """Sanity check a parsed header."""
    if h.bl_type == -1:
        return False
    if h.size == 0 or h.size > 0x1000000:  # Max 16MB
        return False
    if h.version == 0 or h.version > 0xFFFF:
        return False
    return True


# ── NAND scanning ───────────────────────────────────────

# Standard NAND offsets for bootloader stages
NAND_OFFSETS = {
    "header": 0x0,
    "smc": 0x1000,
    "kv": 0x4000,
    "cb": 0x8000,
    # CB_B follows CB (variable size)
    # CD follows CB_B (variable size)
    # CE/kernel at 0x70000 or 0x73000 depending on NAND type
    # CF+CG = xboxupd.bin sections
}


def scan_nand(data: bytes) -> BootchainInfo:
    """Scan a NAND dump for all bootloader stages.

    Looks for bootloader headers at known offsets and by
    scanning for magic values.
    """
    info = BootchainInfo()
    info.nand_size = len(data)
    found_offsets = set()

    # Check standard CB offset
    cb_offset = 0x8000
    h = parse_header(data, cb_offset)
    if h and _is_valid_header(h):
        info.stages.append(h)
        found_offsets.add(cb_offset)
        info.cb_version = h.version

        # CB_B follows CB
        cb_b_offset = cb_offset + h.size
        h2 = parse_header(data, cb_b_offset)
        if h2 and _is_valid_header(h2) and h2.bl_type == BL_CB:
            h2.bl_name = "CB_B/2BL_B"
            info.stages.append(h2)
            found_offsets.add(cb_b_offset)

            # CD follows CB_B
            cd_offset = cb_b_offset + h2.size
            h3 = parse_header(data, cd_offset)
            if h3 and _is_valid_header(h3) and h3.bl_type == BL_CD:
                info.stages.append(h3)
                found_offsets.add(cd_offset)
                info.cd_version = h3.version

    # Scan for CE (kernel) at common offsets
    for ce_off in [0x70000, 0x73000, 0x95000]:
        if ce_off in found_offsets or ce_off >= len(data):
            continue
        h = parse_header(data, ce_off)
        if h and _is_valid_header(h) and h.bl_type == BL_CE:
            info.stages.append(h)
            found_offsets.add(ce_off)
            info.ce_version = h.version
            break

    # Brute-force scan for any remaining stages
    for offset in range(0, min(len(data), 0x200000), 0x10):
        if offset in found_offsets:
            continue
        if offset + 16 > len(data):
            break
        magic = struct.unpack_from(">H", data, offset)[0]
        if magic in MAGIC_MAP:
            h = parse_header(data, offset)
            if h and _is_valid_header(h) and offset not in found_offsets:
                # Avoid duplicates (same version at different offset)
                is_dup = any(s.version == h.version and s.bl_type == h.bl_type
                            for s in info.stages)
                if not is_dup:
                    info.stages.append(h)
                    found_offsets.add(offset)

    # Sort by offset
    info.stages.sort(key=lambda s: s.offset_in_nand)
    return info


def extract_stage(data: bytes, stage: BootloaderHeader) -> bytes:
    """Extract a raw bootloader stage from a NAND dump."""
    start = stage.offset_in_nand
    end = start + stage.size
    if end > len(data):
        end = len(data)
    return bytes(data[start:end])


def format_bootchain(info: BootchainInfo) -> str:
    """Format bootchain info as a readable report."""
    lines = []
    lines.append(f"NAND Bootchain Analysis ({info.nand_size:,} bytes)")
    lines.append("")

    if not info.stages:
        lines.append("  No bootloader stages found.")
        return "\n".join(lines)

    for s in info.stages:
        flags = []
        if s.is_devkit:
            flags.append("DEVKIT")
        if s.is_manufacturing:
            flags.append("MFG")
        if s.is_cba:
            flags.append("CB_A")
        flag_str = f"  [{', '.join(flags)}]" if flags else ""

        lines.append(f"  {s.bl_name:<12} v{s.version:<6} @ 0x{s.offset_in_nand:06X}"
                     f"  size=0x{s.size:06X}  entry=0x{s.entrypoint:08X}{flag_str}")

        if s.key and s.key != b"\x00" * 16:
            lines.append(f"    {'Key:':<12} {s.key.hex().upper()}")

    return "\n".join(lines)


DESCRIPTION = "parse the Xbox 360 boot chain (CB/CD/CE/CF/CG stages) from a NAND dump"


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: bootchain <nand-dump> [--extract STAGE out.bin]"); return
    data = open(a[0], "rb").read()
    info = scan_nand(data)
    print(format_bootchain(info))


if __name__ == "__main__":
    main()
