"""Deep system extraction — find and dump hidden system data from live console memory."""

import struct
from typing import Optional

# Known memory locations from Emma's research + our probes
SYSTEM_LOCATIONS = {
    "hv_encrypted":     (0x80000000, 0x40000,  "Encrypted hypervisor (Pathway 1 ciphertext)"),
    "kernel_pe":        (0x80040000, 0x1A0000, "Kernel PE image (xboxkrnl.exe)"),
    "xam_system_alloc": (0x8C000000, 0x20000,  "XAM encrypted allocations"),
    "title_enc_alloc":  (0x8D000000, 0x10000,  "Title encrypted allocations"),
    "crl":              (0x8E000000, 0x20000,  "Certificate Revocation List"),
    "hv_data_mirror":   (0x8E030000, 0x10000,  "Hypervisor data mirror (flags, etc)"),
    "xex2_headers":     (0x8E050000, 0x10000,  "XEX2 headers for loaded modules"),
    "hvx_expansions":   (0x90001000, 0x1000,   "Installed HV expansions"),
    "xam_raw_image":    (0x3A000000, 0x10000,  "XAM raw image (ETX header)"),
    "phys_mirror":      (0xC0000000, 0x100,    "Physical RAM mirror (first 256 bytes)"),
    "page_tables":      (0xCC000000, 0x1000,   "Page table / hypervisor region"),
    "gpu_edram":        (0xD0000000, 0x1000,   "GPU EDRAM / framebuffer base"),
    "high_memory":      (0xFFD00000, 0x100,    "High memory region"),
}

# Keyvault search signatures
KV_SIGNATURES = [
    b"\x07\x12",  # KV version marker
]

def scan_system_regions(probe) -> dict:
    """Quick scan of all known system memory regions — check what's readable."""
    results = {}
    for name, (addr, size, desc) in SYSTEM_LOCATIONS.items():
        # Just try reading first 16 bytes
        data = probe.read_memory(addr, 16)
        readable = data is not None and len(data) == 16
        results[name] = {
            "address": addr,
            "size": size,
            "description": desc,
            "readable": readable,
            "first_bytes": data[:16].hex().upper() if readable else None,
        }
    return results


def find_keyvault(probe, search_start: int = 0x80040000, search_size: int = 0x1E0000) -> Optional[dict]:
    """Search kernel memory for the keyvault structure.
    
    The keyvault is ~0x4000 bytes. We look for known patterns:
    - The KV version bytes (0x0712)
    - Console serial number (12 ASCII bytes)
    - RSA key structures (cqw field = 0x10 for 1024-bit keys)
    """
    # Read kernel data section in chunks and search
    chunk_size = 0x1000
    for offset in range(0, search_size, chunk_size):
        data = probe.read_memory(search_start + offset, chunk_size)
        if not data:
            continue
        
        # Look for KV version marker followed by reasonable data
        for i in range(len(data) - 0x100):
            # Check for the console serial pattern — 12 ASCII printable bytes
            # The serial is at offset 0x9C in the decrypted KV, but in memory
            # it might be at a different offset depending on how it's loaded
            
            # Look for RSA key header: cqw=0x00000010 (16 qwords = 1024 bits)
            if i + 4 <= len(data):
                val = struct.unpack(">I", data[i:i+4])[0]
                if val == 0x00000010:  # cqw for 1024-bit key
                    next_val = struct.unpack(">I", data[i+4:i+8])[0] if i + 8 <= len(data) else 0
                    if next_val == 0x00010001:  # e = 65537
                        return {
                            "possible_rsa_key_at": search_start + offset + i,
                            "cqw": val,
                            "exponent": next_val,
                            "offset_in_search": offset + i,
                        }
    return None


def dump_xex_headers(probe) -> Optional[dict]:
    """Read the XEX2 headers region at 0x8E050000."""
    data = probe.read_memory(0x8E050000, 0x200)
    if not data:
        return None
    
    result = {"raw": data}
    
    # Check for XEX2 magic
    if data[:4] == b"XEX2":
        result["magic"] = "XEX2"
        result["module_flags"] = struct.unpack(">I", data[4:8])[0]
        result["pe_data_offset"] = struct.unpack(">I", data[8:12])[0]
        result["security_info_offset"] = struct.unpack(">I", data[16:20])[0]
        result["optional_header_count"] = struct.unpack(">I", data[20:24])[0]
    elif data[:4] == b"XEX1":
        result["magic"] = "XEX1"
    else:
        result["magic"] = data[:4].hex().upper()
    
    return result


def dump_hv_mirror(probe) -> Optional[dict]:
    """Read the hypervisor data mirror at 0x8E030000."""
    data = probe.read_memory(0x8E030000, 0x100)
    if not data:
        return None
    
    result = {
        "raw": data,
        "first_dword": struct.unpack(">I", data[0:4])[0],
    }
    
    # Look for interesting patterns
    # The HV mirror might contain boot flags, security state, etc.
    for i in range(0, min(64, len(data)), 4):
        val = struct.unpack(">I", data[i:i+4])[0]
        if val != 0 and val != 0xFFFFFFFF:
            result[f"offset_{i:02X}"] = f"0x{val:08X}"
    
    return result


def dump_crl(probe) -> Optional[dict]:
    """Read the Certificate Revocation List at 0x8E000000."""
    # Read first 256 bytes to check structure
    data = probe.read_memory(0x8E000000, 0x100)
    if not data:
        return None
    
    result = {
        "raw_header": data,
    }
    
    # CRL might have a header with version, size, entry count
    # Try parsing as big-endian structure
    if len(data) >= 16:
        result["first_4_dwords"] = [
            struct.unpack(">I", data[i:i+4])[0] for i in range(0, 16, 4)
        ]
    
    # Check if it looks like real data or zeros
    non_zero = sum(1 for b in data if b != 0)
    result["non_zero_bytes"] = non_zero
    result["appears_populated"] = non_zero > 16
    
    return result


def find_thread_list(probe) -> list:
    """Try to find kernel thread structures in memory.
    
    Xbox 360 kernel threads have known structure offsets.
    Thread list is usually reachable from a kernel global variable.
    """
    # KeGetCurrentThread is ordinal 112/related
    # Thread structures contain recognizable fields:
    # - Priority (small integer)
    # - Processor affinity (bitmask, 0x3F for all 6 threads)
    # - Thread ID (incrementing integer)
    # - Stack base/limit (addresses in 0x70000000+ range)
    
    results = []
    
    # Try reading known thread-related areas
    # Thread stacks are at 0x70040000 area (from our probe v4)
    data = probe.read_memory(0x70040000, 0x100)
    if data:
        results.append({
            "region": "thread_stacks",
            "address": 0x70040000,
            "readable": True,
            "first_bytes": data[:32].hex().upper(),
        })
    
    return results


def dump_import_table(probe, module_base: int) -> Optional[list]:
    """Read the import address table from a loaded PE module.
    
    The IAT contains resolved function pointers for all imports.
    """
    # Read the PE header to find the IAT
    mz = probe.read_memory(module_base, 0x40)
    if not mz or mz[:2] != b"MZ":
        return None
    
    e_lfanew = struct.unpack("<I", mz[0x3C:0x40])[0]
    pe_addr = module_base + e_lfanew
    
    # Read PE optional header to find data directories
    pe_data = probe.read_memory(pe_addr, 0x100)
    if not pe_data or pe_data[:4] != b"PE\x00\x00":
        return None
    
    # The import directory is data directory entry 1
    # For Xbox 360 PE, optional header starts at PE+24
    # Data directories are at a fixed offset in the optional header
    # This is complex — return the raw optional header for now
    
    return [{
        "pe_addr": pe_addr,
        "optional_header": pe_data[24:88].hex().upper(),
    }]


def extract_kernel_globals(probe) -> dict:
    """Read known kernel global variables and data exports."""
    globals_found = {}
    
    import importlib
    try:
        from ordinals import KERNEL_ORDINALS
    except:
        KERNEL_ORDINALS = {}
    
    # Data exports (non-function ordinals)
    data_ordinals = {
        342: "XboxKrnlVersion",
        343: "XboxHardwareInfo", 
        344: "XboxKrnlBaseVersion",
        403: "XeKeysExecute",
    }
    
    for ordinal, name in data_ordinals.items():
        result = probe.resolve_ordinal(0, ordinal)
        if result:
            addr = result["address"]
            # Read 64 bytes at this address
            data = probe.read_memory(addr, 64)
            if data:
                globals_found[name] = {
                    "ordinal": ordinal,
                    "address": addr,
                    "data": data,
                    "hex": data[:16].hex().upper(),
                }
    
    return globals_found


def find_console_certificate(probe) -> Optional[dict]:
    """Search for the console certificate in kernel memory.
    
    The console certificate is 0x1A8 bytes and contains:
    - Certificate size (WORD) at offset 0
    - Console ID (5 bytes) at offset 2
    - Console part number (11 bytes)
    - Public key exponent (DWORD)
    - Public key modulus (0x80 bytes)
    """
    # The certificate is usually in the keyvault region or loaded into
    # a known kernel address. Try reading from known XAM data area.
    
    # XAM loads cert data around 0x8C000000
    data = probe.read_memory(0x8C000000, 0x2000)
    if not data:
        return None
    
    # Search for certificate-sized structure with valid size field
    for i in range(len(data) - 0x1A8):
        size = struct.unpack(">H", data[i:i+2])[0]
        if size == 0x01A8:  # Standard console certificate size
            cert_data = data[i:i+0x1A8]
            console_id = cert_data[2:7]
            part_number = cert_data[7:18]
            
            # Validate — console ID should be non-zero
            if any(b != 0 for b in console_id):
                return {
                    "address": 0x8C000000 + i,
                    "size": size,
                    "console_id": console_id.hex().upper(),
                    "part_number": part_number.decode("ascii", errors="replace"),
                    "raw": cert_data,
                }
    
    return None
