#!/usr/bin/env python3
"""detect.py -- the ONE place that says what kind of executable a file is.

Every MZ-family executable starts with a DOS header whose field at 0x3C (e_lfanew, 4 bytes)
points at the "new" header; its two-byte signature decides the format. This check was written
out separately in each era's target module, the corpus sweeps, the native-decompiler router and
others, with small disagreements (some read e_lfanew as 2 bytes, which mis-locates the header
of any file whose DOS stub is 64 KB or more). Dependency-free, like the rest of formats/.

    signature(path)   -> b"PE" | b"NE" | b"LE" | b"LX" | b"" (MZ, no known new header) | None (not MZ)
    format_of(path)   -> "PE" | "NE" | "LE" | "LX" | "MZ" | None
    lane(path)        -> "pe" | "ne" | "le" | "dos" | None      (the decompiler lane)
"""
import struct

_KNOWN = (b"PE", b"NE", b"LE", b"LX")


def signature_of_bytes(head, read_at=None):
    """Signature from the first 0x40 bytes. `read_at(offset, n)` fetches the new-header bytes
    when they lie beyond `head`; without it only `head` is consulted."""
    if len(head) < 0x40 or head[:2] != b"MZ":
        return None
    off = struct.unpack_from("<I", head, 0x3C)[0]
    if off + 2 <= len(head):
        sig = head[off:off + 2]
    elif read_at is not None and 0 < off < 0x10000000:
        sig = read_at(off, 2)
    else:
        sig = b""
    return sig if sig in _KNOWN else b""


def signature(path):
    """New-header signature of the file at `path` (see module docstring), or None when the file
    is unreadable or not an MZ executable."""
    try:
        with open(path, "rb") as f:
            head = f.read(0x40)

            def read_at(off, n):
                f.seek(off)
                return f.read(n)
            return signature_of_bytes(head, read_at)
    except OSError:
        return None


def format_of(path):
    """'PE' | 'NE' | 'LE' | 'LX' | 'MZ' (plain DOS executable) | None (not an MZ executable)."""
    sig = signature(path)
    if sig is None:
        return None
    return sig.decode("ascii") if sig else "MZ"


_LANE = {b"PE": "pe", b"NE": "ne", b"LE": "le", b"LX": "le", b"": "dos"}


def lane(path):
    """The decompiler lane for a file: 'pe', 'ne', 'le' (VxD; LE and LX), 'dos', or None."""
    sig = signature(path)
    return None if sig is None else _LANE[sig]
