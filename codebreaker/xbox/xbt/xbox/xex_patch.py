"""XEX2 header patching utilities.

Includes:
  - PAL-50 incompatibility flag clearer (from xex2-pal50-clear)
  - Generic privilege/flag editor
  - Region flag manipulation

These operate only on the unencrypted XEX2 header directory entries.
They do NOT modify code segments or require decryption.
"""

import struct
from pathlib import Path
from typing import Optional


# ── XEX2 Constants ──────────────────────────────────────

XEX2_MAGIC = b"XEX2"
XEX2_HEADER_SIZE = 0x18

# Directory entry keys (big-endian in file)
KEY_PRIVILEGES          = 0x00030000
KEY_EXECUTION_ID        = 0x00040006
KEY_GAME_RATINGS        = 0x00040010
KEY_TITLE_WORKSPACE     = 0x00040201
KEY_SYSTEM_FLAGS        = 0x00030100
KEY_ORIGINAL_BASE_ADDR  = 0x00010100
KEY_ENTRY_POINT         = 0x00010100

# Privilege bit masks
PRIV_PAL50_INCOMPATIBLE     = 0x00000400
PRIV_INSECURE_SOCKETS       = 0x00000200
PRIV_ALLOW_PROFILE_SHARING  = 0x00008000


def _be32(data: bytes, offset: int = 0) -> int:
    return struct.unpack_from(">I", data, offset)[0]


def _put_be32(data: bytearray, offset: int, value: int):
    struct.pack_into(">I", data, offset, value & 0xFFFFFFFF)


# ── XEX2 Header Parsing ────────────────────────────────

def parse_xex2_header(data: bytes) -> dict:
    """Parse the XEX2 file header. Returns header info dict."""
    if len(data) < XEX2_HEADER_SIZE:
        raise ValueError("File too short for XEX2 header")
    if data[:4] != XEX2_MAGIC:
        raise ValueError(f"Not a XEX2 file (magic: {data[:4]!r})")

    module_flags = _be32(data, 4)
    size_of_headers = _be32(data, 8)
    size_discard = _be32(data, 12)
    security_offset = _be32(data, 16)
    dir_count = _be32(data, 20)

    return {
        "module_flags": module_flags,
        "size_of_headers": size_of_headers,
        "security_offset": security_offset,
        "dir_count": dir_count,
    }


def parse_directory_entries(data: bytes, dir_count: int) -> list:
    """Parse directory entries from XEX2 header.

    Returns list of (file_offset, key, value).
    """
    entries = []
    offset = XEX2_HEADER_SIZE
    for _ in range(dir_count):
        if offset + 8 > len(data):
            break
        key = _be32(data, offset)
        val = _be32(data, offset + 4)
        entries.append((offset, key, val))
        offset += 8
    return entries


def find_entry(entries: list, key: int) -> Optional[tuple]:
    """Find a directory entry by key. Returns (offset, key, value) or None."""
    for entry in entries:
        if entry[1] == key:
            return entry
    return None


# ── Patching Functions ──────────────────────────────────

def clear_pal50(data: bytes, dry_run: bool = False) -> tuple:
    """Clear the PAL-50 incompatibility flag from a XEX2 file.

    Returns (patched_data, was_modified, message).
    """
    hdr = parse_xex2_header(data)
    entries = parse_directory_entries(data, hdr["dir_count"])
    priv = find_entry(entries, KEY_PRIVILEGES)

    if priv is None:
        return (data, False, "Privileges entry not found in XEX header")

    offset, key, val = priv
    if not (val & PRIV_PAL50_INCOMPATIBLE):
        return (data, False, "PAL-50 bit already clear — no patch needed")

    new_val = val & ~PRIV_PAL50_INCOMPATIBLE
    msg = f"Privileges 0x{val:08X} → 0x{new_val:08X} (cleared PAL-50)"

    if dry_run:
        return (data, False, f"[DRY RUN] {msg}")

    patched = bytearray(data)
    _put_be32(patched, offset + 4, new_val)
    return (bytes(patched), True, msg)


def set_privilege_flag(data: bytes, flag: int, enable: bool = True) -> tuple:
    """Set or clear a privilege flag in the XEX2 header.

    Returns (patched_data, was_modified, message).
    """
    hdr = parse_xex2_header(data)
    entries = parse_directory_entries(data, hdr["dir_count"])
    priv = find_entry(entries, KEY_PRIVILEGES)

    if priv is None:
        return (data, False, "Privileges entry not found")

    offset, key, val = priv
    if enable:
        new_val = val | flag
    else:
        new_val = val & ~flag

    if new_val == val:
        return (data, False, f"Flag 0x{flag:08X} already {'set' if enable else 'clear'}")

    patched = bytearray(data)
    _put_be32(patched, offset + 4, new_val)
    action = "set" if enable else "cleared"
    return (bytes(patched), True, f"Privileges 0x{val:08X} → 0x{new_val:08X} ({action} 0x{flag:08X})")


def get_xex2_info(data: bytes) -> dict:
    """Extract basic info from a XEX2 header."""
    hdr = parse_xex2_header(data)
    entries = parse_directory_entries(data, hdr["dir_count"])

    info = {
        "module_flags": hdr["module_flags"],
        "header_size": hdr["size_of_headers"],
        "security_offset": hdr["security_offset"],
        "dir_entries": len(entries),
        "privileges": None,
        "pal50_incompatible": False,
    }

    priv = find_entry(entries, KEY_PRIVILEGES)
    if priv:
        info["privileges"] = priv[2]
        info["pal50_incompatible"] = bool(priv[2] & PRIV_PAL50_INCOMPATIBLE)

    return info


def patch_file(input_path: str, output_path: str = None,
               clear_pal50_flag: bool = True, dry_run: bool = False) -> str:
    """Patch a XEX2 file on disk.

    Returns a status message.
    """
    inp = Path(input_path)
    if not inp.exists():
        return f"File not found: {inp}"

    data = inp.read_bytes()

    if clear_pal50_flag:
        patched, modified, msg = clear_pal50(data, dry_run=dry_run)
    else:
        return "No patch operation specified"

    if not modified:
        return msg

    if output_path is None:
        output_path = str(inp.with_suffix(".patched.xex"))

    Path(output_path).write_bytes(patched)
    return f"{msg}\nSaved: {output_path}"


DESCRIPTION = "patch an Xbox 360 XEX2: clear the PAL-50 region flag (info/dry-run)"


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: xexpatch <in.xex> [out.xex] [--dry]  |  xexpatch info <in.xex>"); return
    if a[0] == "info" and len(a) > 1:
        for k, v in get_xex2_info(open(a[1], "rb").read()).items():
            print("  %-18s %s" % (k, v))
        return
    dry = "--dry" in a
    args = [x for x in a if x != "--dry"]
    out = args[1] if len(args) > 1 else None
    print(patch_file(args[0], out, clear_pal50_flag=True, dry_run=dry))


if __name__ == "__main__":
    main()
