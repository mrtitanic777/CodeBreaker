"""Memory diff — snapshot regions and find what changed."""

import time
from typing import Optional


def snapshot(probe, address: int, size: int) -> Optional[bytes]:
    """Take a memory snapshot."""
    return probe.read_memory(address, size)


def diff_snapshots(snap1: bytes, snap2: bytes, base_addr: int) -> list:
    """Compare two snapshots and return list of changes.
    
    Returns list of (address, old_byte, new_byte) tuples.
    """
    changes = []
    size = min(len(snap1), len(snap2))
    for i in range(size):
        if snap1[i] != snap2[i]:
            changes.append((base_addr + i, snap1[i], snap2[i]))
    return changes


def diff_dwords(snap1: bytes, snap2: bytes, base_addr: int) -> list:
    """Compare two snapshots at DWORD granularity (big-endian).
    
    Returns list of (address, old_dword, new_dword) tuples.
    """
    import struct
    changes = []
    size = min(len(snap1), len(snap2)) & ~3  # Align to 4
    for i in range(0, size, 4):
        w1 = struct.unpack(">I", snap1[i:i+4])[0]
        w2 = struct.unpack(">I", snap2[i:i+4])[0]
        if w1 != w2:
            changes.append((base_addr + i, w1, w2))
    return changes


def find_value_locations(snap: bytes, base_addr: int, value: int,
                         size: int = 4) -> list:
    """Find all locations in a snapshot that contain a specific value."""
    import struct
    results = []
    if size == 4:
        pattern = struct.pack(">I", value)
    elif size == 2:
        pattern = struct.pack(">H", value)
    elif size == 1:
        pattern = bytes([value & 0xFF])
    else:
        return results
    
    idx = 0
    while True:
        idx = snap.find(pattern, idx)
        if idx == -1:
            break
        results.append(base_addr + idx)
        idx += 1
    return results
