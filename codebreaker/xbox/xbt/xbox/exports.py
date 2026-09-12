"""Kernel export table parser — read the PE export directory from live memory."""

import struct
from typing import Optional
from pathlib import Path


def parse_export_directory(data: bytes, edata_rva: int = 0x15E00) -> dict:
    """Parse a PE IMAGE_EXPORT_DIRECTORY from raw section data.
    
    Args:
        data: Raw .edata section bytes
        edata_rva: RVA of the .edata section (default for kernel)
    
    Returns dict with export directory fields and ordinal->RVA mapping.
    """
    if len(data) < 40:
        return {"error": "Data too small for export directory"}
    
    ed = {
        "flags": struct.unpack('<I', data[0:4])[0],
        "timestamp": struct.unpack('<I', data[4:8])[0],
        "major_version": struct.unpack('<H', data[8:10])[0],
        "minor_version": struct.unpack('<H', data[10:12])[0],
        "name_rva": struct.unpack('<I', data[12:16])[0],
        "ordinal_base": struct.unpack('<I', data[16:20])[0],
        "num_functions": struct.unpack('<I', data[20:24])[0],
        "num_names": struct.unpack('<I', data[24:28])[0],
        "addr_table_rva": struct.unpack('<I', data[28:32])[0],
        "name_ptrs_rva": struct.unpack('<I', data[32:36])[0],
        "ordinal_table_rva": struct.unpack('<I', data[36:40])[0],
    }
    
    # Parse module name
    name_offset = ed["name_rva"] - edata_rva
    if 0 <= name_offset < len(data):
        end = data.index(0, name_offset) if 0 in data[name_offset:name_offset+128] else name_offset + 128
        ed["module_name"] = data[name_offset:end].decode('ascii', errors='replace')
    
    # Parse address table
    addr_offset = ed["addr_table_rva"] - edata_rva
    ed["exports"] = {}
    
    if 0 <= addr_offset < len(data):
        for i in range(ed["num_functions"]):
            off = addr_offset + i * 4
            if off + 4 > len(data):
                break
            rva = struct.unpack('<I', data[off:off+4])[0]
            ordinal = ed["ordinal_base"] + i
            ed["exports"][ordinal] = {
                "rva": rva,
                "address": 0x80040000 + rva if rva != 0 else 0,
            }
    
    return ed


def parse_export_from_live(probe, edata_addr: int = 0x80055E00,
                           edata_size: int = 0x1000) -> Optional[dict]:
    """Read and parse the kernel export table from live console memory."""
    data = probe.read_memory(edata_addr, edata_size)
    if not data:
        return None
    return parse_export_directory(data)


def export_to_ordinal_file(exports: dict, output_path: str,
                           live_scan: dict = None) -> int:
    """Write export table to ordinal file format.
    
    Returns number of entries written.
    """
    count = 0
    with open(output_path, 'w') as f:
        f.write(f"# {exports.get('module_name', 'unknown')} export table\n")
        f.write(f"# {exports['num_functions']} exports, base {exports['ordinal_base']}\n")
        f.write(f"# Timestamp: 0x{exports['timestamp']:08X}\n")
        f.write(f"#\n")
        f.write(f"# Ordinal  Address     RVA         LiveMatch\n")
        
        for ordinal in sorted(exports["exports"].keys()):
            exp = exports["exports"][ordinal]
            addr = exp["address"]
            rva = exp["rva"]
            
            # Check against live scan
            live_match = ""
            if live_scan and ordinal in live_scan:
                if live_scan[ordinal] == addr:
                    live_match = "MATCH"
                else:
                    live_match = f"DIFFER(0x{live_scan[ordinal]:08X})"
            
            f.write(f"{ordinal:4d}  0x{addr:08X}  0x{rva:08X}  {live_match}\n")
            count += 1
    
    return count


def find_hidden_exports(exports: dict, live_scan: dict) -> list:
    """Find ordinals that exist in the export table but weren't resolved live.
    
    These are 'hidden' exports — the kernel has them but didn't expose them
    through the normal resolution mechanism.
    """
    hidden = []
    export_ords = set(exports["exports"].keys())
    live_ords = set(live_scan.keys())
    
    for ordinal in sorted(export_ords - live_ords):
        exp = exports["exports"][ordinal]
        if exp["address"] != 0x80040000 and exp["address"] != 0:
            hidden.append({
                "ordinal": ordinal,
                "address": exp["address"],
                "rva": exp["rva"],
            })
    
    return hidden


DESCRIPTION = "parse a PE export directory (.edata) into an ordinal -> RVA map"


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: exports360 <edata.bin> [edata_rva_hex]"); return
    data = open(a[0], "rb").read()
    rva = int(a[1], 16) if len(a) > 1 else 0x15E00
    d = parse_export_directory(data, rva)
    for k, v in d.items():
        if k in ("ordinals", "ordinal_map", "exports"):
            print("  %-18s %d entries" % (k, len(v)))
        else:
            print("  %-18s %s" % (k, v))


if __name__ == "__main__":
    main()
