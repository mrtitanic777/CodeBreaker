"""PE section dumper — parse PE headers and extract sections from live console memory."""

import struct
from typing import Optional
from pathlib import Path


def parse_pe_header(probe, base_addr: int) -> Optional[dict]:
    """Parse a PE header from live console memory.
    
    Reads the MZ header, finds PE signature, parses COFF + optional + section headers.
    Xbox 360 PE files use big-endian fields but standard PE structure.
    """
    # Read MZ header (first 64 bytes)
    mz = probe.read_memory(base_addr, 0x40)
    if not mz or mz[0:2] != b"MZ":
        return None
    
    pe = {"base": base_addr}
    
    # e_lfanew at offset 0x3C — stored little-endian on Xbox 360
    e_lfanew_raw = struct.unpack("<I", mz[0x3C:0x40])[0]
    pe["e_lfanew"] = e_lfanew_raw
    
    pe_addr = base_addr + e_lfanew_raw
    
    # Read PE signature + COFF header + start of optional header (256 bytes)
    pe_data = probe.read_memory(pe_addr, 256)
    if not pe_data or pe_data[0:4] != b"PE\x00\x00":
        return None
    
    pe["pe_addr"] = pe_addr
    
    # COFF header at PE+4. Xbox 360 PE/COFF *headers* are little-endian (only the
    # PPC code is big-endian) — read them LE. (Reading BE byte-swaps every field:
    # machine 0x01F2 shows as 0xF201, TimeDateStamp reads as a year-2076 value,
    # and section offsets come out nonsensical.)
    pe["machine"] = struct.unpack("<H", pe_data[4:6])[0]
    pe["num_sections"] = struct.unpack("<H", pe_data[6:8])[0]
    pe["timestamp"] = struct.unpack("<I", pe_data[8:12])[0]
    pe["symbol_table"] = struct.unpack("<I", pe_data[12:16])[0]
    pe["num_symbols"] = struct.unpack("<I", pe_data[16:20])[0]
    pe["opt_header_size"] = struct.unpack("<H", pe_data[20:22])[0]
    pe["characteristics"] = struct.unpack("<H", pe_data[22:24])[0]

    pe["machine_name"] = {
        0x01F2: "PowerPC (Xbox 360)",
        0xF201: "PowerPC (byte-swapped read)",
        0x0200: "IA64",
        0x014C: "i386",
    }.get(pe["machine"], f"Unknown (0x{pe['machine']:04X})")
    
    # Section headers start after optional header
    section_offset = pe_addr + 24 + pe["opt_header_size"]
    
    # Read all section headers (40 bytes each)
    section_data = probe.read_memory(section_offset, pe["num_sections"] * 40)
    if not section_data:
        pe["sections"] = []
        return pe
    
    pe["sections"] = []
    for i in range(pe["num_sections"]):
        off = i * 40
        if off + 40 > len(section_data):
            break
        
        name_raw = section_data[off:off + 8]
        name = name_raw.split(b"\x00")[0].decode("ascii", errors="replace")
        
        section = {
            "name": name,
            "virtual_size": struct.unpack("<I", section_data[off + 8:off + 12])[0],
            "virtual_addr": struct.unpack("<I", section_data[off + 12:off + 16])[0],
            "raw_size": struct.unpack("<I", section_data[off + 16:off + 20])[0],
            "raw_offset": struct.unpack("<I", section_data[off + 20:off + 24])[0],
            "characteristics": struct.unpack("<I", section_data[off + 36:off + 40])[0],
        }
        
        # Decode characteristics
        chars = []
        c = section["characteristics"]
        if c & 0x00000020: chars.append("CODE")
        if c & 0x00000040: chars.append("IDATA")
        if c & 0x00000080: chars.append("UDATA")
        if c & 0x20000000: chars.append("EXEC")
        if c & 0x40000000: chars.append("READ")
        if c & 0x80000000: chars.append("WRITE")
        section["flags"] = "|".join(chars)
        
        pe["sections"].append(section)
    
    return pe


def dump_section(probe, base_addr: int, section: dict, output_dir: str,
                 callback=None) -> Optional[str]:
    """Dump a PE section from live console memory to a file.
    
    Returns the output file path or None.
    """
    # Section is loaded at base_addr + virtual_addr
    load_addr = base_addr + section["virtual_addr"]
    size = section["virtual_size"]
    name = section["name"].replace(".", "_").strip()
    
    if size == 0:
        return None
    
    data = probe.dump_range(load_addr, size, callback=callback)
    
    out_path = Path(output_dir) / f"section_{name}_{load_addr:08X}.bin"
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_bytes(data)
    
    return str(out_path)


def dump_all_sections(probe, base_addr: int, output_dir: str,
                      callback=None) -> list:
    """Parse PE and dump all sections. Returns list of (section_name, file_path)."""
    pe = parse_pe_header(probe, base_addr)
    if not pe:
        return []
    
    results = []
    for section in pe["sections"]:
        path = dump_section(probe, base_addr, section, output_dir, callback)
        if path:
            results.append((section["name"], path))
    
    return results
