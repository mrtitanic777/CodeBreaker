"""Console scanner — memory search, ordinal brute force, pattern matching."""

import struct
import time
from typing import Optional


def search_memory(probe, start_addr: int, size: int, pattern: bytes,
                  callback=None) -> list:
    """Search console memory for a byte pattern.
    
    Reads in 1024-byte chunks and scans for the pattern.
    Returns list of addresses where pattern was found.
    """
    results = []
    chunk_size = 1024
    overlap = len(pattern) - 1  # Overlap to catch patterns spanning chunks

    offset = 0
    while offset < size:
        read_size = min(chunk_size, size - offset)
        # Read a bit extra for overlap (except at start)
        actual_read = min(read_size + overlap, size - offset)
        data = probe.read_memory(start_addr + offset, actual_read)

        if data:
            # Search within this chunk
            search_start = 0
            while True:
                idx = data.find(pattern, search_start)
                if idx == -1:
                    break
                found_addr = start_addr + offset + idx
                results.append(found_addr)
                search_start = idx + 1

        if callback:
            callback(offset, size, len(results))

        offset += read_size

    return results


def search_dword(probe, start_addr: int, size: int, value: int,
                 callback=None) -> list:
    """Search console memory for a specific DWORD value (big-endian)."""
    pattern = struct.pack(">I", value)
    return search_memory(probe, start_addr, size, pattern, callback)


def search_string(probe, start_addr: int, size: int, text: str,
                  callback=None) -> list:
    """Search console memory for an ASCII string."""
    return search_memory(probe, start_addr, size, text.encode("ascii"), callback)


def scan_all_ordinals(probe, module: int = 0, start: int = 1, end: int = 900,
                      callback=None) -> dict:
    """Brute-force resolve all ordinals in a module on the live console.
    
    Args:
        probe: ConsoleLink instance
        module: 0 = xboxkrnl.exe, 1 = xam.xex
        start: First ordinal to scan
        end: Last ordinal to scan
        callback: Called with (ordinal, result_or_none, progress)
    
    Returns: Dict of ordinal -> {"address": int, "first_instruction": int, "type": str}
    """
    results = {}
    total = end - start + 1

    for ordinal in range(start, end + 1):
        result = probe.resolve_ordinal(module, ordinal)
        
        if result:
            addr = result["address"]
            first = result["first_instruction"]
            
            # Classify the function
            if first == 0x7D8802A6:
                ftype = "REAL"       # Standard prologue
            elif first == 0x4E800020:
                ftype = "STUB_NOP"   # Bare blr (no-op return)
            elif (first & 0xFFFF0000) == 0x38600000:
                ret_val = first & 0xFFFF
                if ret_val == 0:
                    ftype = "STUB_OK"     # li r3, 0; (return S_OK)
                else:
                    ftype = f"STUB_RET_{ret_val}"
            elif (first & 0xFC000000) == 0x3C000000:
                ftype = "REAL_LIS"   # Starts with lis (loads constant)
            else:
                ftype = "REAL_OTHER"
            
            results[ordinal] = {
                "address": addr,
                "first_instruction": first,
                "type": ftype,
            }

        if callback:
            progress = (ordinal - start + 1) * 100 // total
            callback(ordinal, result, progress)

        # Small delay to not flood the console
        time.sleep(0.005)

    return results


def classify_scan_results(results: dict) -> dict:
    """Classify scan results into categories."""
    stats = {
        "total": len(results),
        "real": 0,
        "stub": 0,
        "by_type": {},
        "address_range": (0xFFFFFFFF, 0),
    }

    for ordinal, info in results.items():
        ftype = info["type"]
        
        if ftype.startswith("REAL"):
            stats["real"] += 1
        else:
            stats["stub"] += 1

        if ftype not in stats["by_type"]:
            stats["by_type"][ftype] = 0
        stats["by_type"][ftype] += 1

        addr = info["address"]
        if addr < stats["address_range"][0]:
            stats["address_range"] = (addr, stats["address_range"][1])
        if addr > stats["address_range"][1]:
            stats["address_range"] = (stats["address_range"][0], addr)

    return stats
