"""Universal key extractor — pull authentication keys from any connected Xbox 360.

Scans live console memory for:
- Pi marker (0x31415926...) to locate the crypto workspace
- Derived session key (HMAC-SHA1 output, 20 bytes before pi marker)
- Encrypted keyvault blocks
- XAM security context (authentication state, nonces, function pointers)
- ETX telemetry (machine token, environment, LIVE session state)
- Kernel version and hardware info
- Console certificate (if accessible)

Outputs a JSON file compatible with the LIVE server's database.py add format.
"""

import struct
import json
import time
from typing import Optional
from pathlib import Path


# The pi marker that anchors the crypto workspace
PI_MARKER = bytes.fromhex("31415926271828180123456789ABCDEF")

# Known XAM allocation base
XAM_ALLOC_BASE = 0x8C000000

# ETX base
ETX_BASE = 0x3A000000

# Kernel version location
KERNEL_VERSION_ADDR = 0x80170000

# Kernel hardware info
HARDWARE_INFO_ADDR = 0x80040454

# Security context search ranges
SECURITY_LIST_ADDR = 0x801B0F00


def scan_for_pi_marker(probe, start: int = 0x8C000000, size: int = 0x20000) -> Optional[int]:
    """Scan memory for the pi marker to locate the crypto workspace.
    
    The pi marker (3.14159265... + 2.71828182... + 0x0123456789ABCDEF) 
    is a 16-byte constant that anchors the XAM crypto workspace.
    It appears at a fixed offset within the XAM heap allocation.
    
    Returns the address of the pi marker, or None.
    """
    chunk_size = 0x1000
    for offset in range(0, size, chunk_size):
        data = probe.read_memory(start + offset, chunk_size + 16)  # overlap for boundary
        if not data:
            continue
        idx = data.find(PI_MARKER)
        if idx >= 0:
            return start + offset + idx
    return None


def extract_derived_key(probe, pi_addr: int) -> Optional[bytes]:
    """Extract the 20-byte HMAC-SHA1 derived session key.
    
    The key can be either BEFORE or AFTER the pi marker depending on
    the XAM heap layout for this boot cycle. We check both sides and
    return whichever has the highest-entropy 20-byte block.
    """
    # Read 48 bytes before and 48 bytes after the marker
    data = probe.read_memory(pi_addr - 0x30, 0x70)
    if not data:
        return None
    
    marker_offset = 0x30  # Pi marker starts at this offset in our read
    
    candidates = []
    
    # Check AFTER the marker (marker is 16 bytes)
    after = data[marker_offset + 16:marker_offset + 16 + 24]
    if len(after) >= 20:
        # Find the high-entropy block
        for start in range(min(5, len(after) - 20 + 1)):
            key = after[start:start + 20]
            entropy = len(set(key))
            zeros = key.count(0)
            if entropy >= 12 and zeros <= 2:
                candidates.append((entropy, "after", start, key))
    
    # Check BEFORE the marker
    before = data[marker_offset - 24:marker_offset]
    if len(before) >= 20:
        # Scan for the high-entropy block (skip leading zeros)
        for start in range(min(5, len(before) - 20 + 1)):
            key = before[start:start + 20]
            entropy = len(set(key))
            zeros = key.count(0)
            if entropy >= 12 and zeros <= 2:
                candidates.append((entropy, "before", start, key))
    
    if not candidates:
        return None
    
    # Return the highest-entropy candidate
    candidates.sort(key=lambda x: x[0], reverse=True)
    return candidates[0][3]


def extract_encrypted_kv_blocks(probe, pi_addr: int) -> dict:
    """Extract the encrypted keyvault data blocks near the pi marker.
    
    The derived key (20 bytes) sits adjacent to the pi marker.
    The real encrypted KV blocks are larger (hundreds to thousands of bytes)
    and start further out. We skip small blocks (<64 bytes) to avoid
    picking up the derived key or padding as a "block".
    """
    # Read a large region around the pi marker
    read_start = pi_addr - 0x100
    data = probe.read_memory(read_start, 0x1200)
    if not data:
        return {}
    
    blocks = {}
    MIN_BLOCK = 64  # Minimum size to qualify as a KV block
    
    # Scan for all contiguous non-zero regions
    all_blocks = []
    in_data = False
    block_start = 0
    zero_run = 0
    
    for i in range(len(data)):
        if data[i] != 0:
            if not in_data:
                block_start = i
                in_data = True
            zero_run = 0
        else:
            zero_run += 1
            if in_data and zero_run >= 16:
                block_end = i - zero_run
                size = block_end - block_start
                if size >= MIN_BLOCK:
                    all_blocks.append((read_start + block_start, size, 
                                      data[block_start:block_end]))
                in_data = False
    
    if in_data:
        block_end = len(data)
        size = block_end - block_start
        if size >= MIN_BLOCK:
            all_blocks.append((read_start + block_start, size,
                              data[block_start:block_end]))
    
    # Assign blocks by size (largest first)
    all_blocks.sort(key=lambda x: x[1], reverse=True)
    
    for idx, (addr, size, bdata) in enumerate(all_blocks[:3]):
        name = f"block{idx + 1}"
        blocks[name] = {
            "offset": addr,
            "size": size,
            "data": bdata,
        }
    
    return blocks


def extract_xam_security_context(probe) -> Optional[dict]:
    """Find and decode the XAM security context from the kernel security list.
    
    The security list at 0x801B0F38 contains a pointer to the XAM context.
    The context structure is ~128 bytes.
    """
    # Read the security list entry that points to XAM
    list_data = probe.read_memory(0x801B0F38, 8)
    if not list_data:
        return None
    
    xam_ctx_addr = struct.unpack('>I', list_data[0:4])[0]
    
    # Validate pointer
    if not (0x81000000 <= xam_ctx_addr <= 0x82000000):
        return None
    
    # Read the full context
    ctx_data = probe.read_memory(xam_ctx_addr, 0x80)
    if not ctx_data:
        return None
    
    ctx = {
        "address": xam_ctx_addr,
        "kernel_backptr": struct.unpack('>I', ctx_data[0:4])[0],
        "xam_module_base": struct.unpack('>I', ctx_data[4:8])[0],
        "security_token": struct.unpack('>I', ctx_data[0x10:0x14])[0],
        "crypto_state_ptr": struct.unpack('>I', ctx_data[0x18:0x1C])[0],
        "key_material_ptr": struct.unpack('>I', ctx_data[0x1C:0x20])[0],
        "timeout_ms": struct.unpack('>I', ctx_data[0x24:0x28])[0],
        "tick_counter": struct.unpack('>I', ctx_data[0x28:0x2C])[0],
        "max_entries": struct.unpack('>I', ctx_data[0x2C:0x30])[0],
        "auth_dispatch": struct.unpack('>I', ctx_data[0x40:0x44])[0],
        "kernel_callback": struct.unpack('>I', ctx_data[0x54:0x58])[0],
        "active": struct.unpack('>I', ctx_data[0x68:0x6C])[0],
        "crypto_key_ptr": struct.unpack('>I', ctx_data[0x78:0x7C])[0],
        "key_buffer_size": struct.unpack('>I', ctx_data[0x7C:0x80])[0],
    }
    
    return ctx


def extract_etx_tokens(probe) -> dict:
    """Extract machine token and session info from the ETX telemetry buffer."""
    data = probe.read_memory(ETX_BASE, 0x200)
    if not data or data[0:3] != b"ETX":
        return {}
    
    result = {
        "magic": "ETX",
        "flags": struct.unpack('>I', data[4:8])[0],
    }
    
    # Find machine token (alphanumeric string, typically 14 chars)
    # Follows "LogonFail_Other" or similar status string
    for i in range(len(data) - 14):
        candidate = data[i:i+14]
        if all((0x30 <= b <= 0x39) or (0x41 <= b <= 0x5A) for b in candidate):
            # Check it's not inside a known string like "LogonFail"
            before = data[max(0, i-20):i]
            if b"Logon" not in before and b"event" not in before and b"PROD" not in before:
                result["machine_token"] = candidate.decode('ascii')
                break
    
    # Find environment
    if b"PROD" in data:
        result["environment"] = "PROD"
    elif b"CERT" in data:
        result["environment"] = "CERT"
    elif b"TEST" in data:
        result["environment"] = "TEST"
    
    # Find logon status
    if b"LogonFail_Other" in data:
        result["logon_status"] = "LogonFail_Other"
    elif b"LogonSuccess" in data:
        result["logon_status"] = "LogonSuccess"
    elif b"LogonFail" in data:
        result["logon_status"] = "LogonFail"
    
    return result


def extract_kernel_info(probe) -> dict:
    """Extract kernel version and hardware info."""
    info = {}
    
    # Kernel version at 0x80170000
    ver_data = probe.read_memory(KERNEL_VERSION_ADDR, 16)
    if ver_data:
        w0 = struct.unpack('>H', ver_data[0:2])[0]
        w1 = struct.unpack('>H', ver_data[2:4])[0]
        w2 = struct.unpack('>H', ver_data[4:6])[0]
        info["kernel_version"] = f"{w0:04X}.{w1:04X}.{w2:04X}"
        info["kernel_build"] = w1
        info["kernel_qfe"] = w2
    
    # Hardware info at 0x80040454
    hw_data = probe.read_memory(HARDWARE_INFO_ADDR, 16)
    if hw_data:
        info["hardware_flags"] = struct.unpack('>I', hw_data[0:4])[0]
        info["hardware_version"] = struct.unpack('>I', hw_data[4:8])[0]
    
    # Kernel build string from export table
    build_str_data = probe.read_memory(0x80056CB8, 64)
    if build_str_data:
        end = build_str_data.find(0)
        if end > 0:
            info["build_string"] = build_str_data[:end].decode('ascii', errors='replace')
    
    return info


def extract_boot_hash(probe) -> Optional[bytes]:
    """Extract the boot chain integrity hash from the security region."""
    data = probe.read_memory(0x801B0EE4, 12)
    if data:
        return data
    return None


def extract_title_path(probe) -> Optional[str]:
    """Find the running title's device path in kernel memory."""
    # Scan the region where title paths are stored
    data = probe.read_memory(0x801A5B00, 0x200)
    if not data:
        return None
    
    # Find \Device\ pattern
    idx = data.find(b"\\Device\\")
    if idx >= 0:
        end = idx
        while end < len(data) and data[end] != 0:
            end += 1
        return data[idx:end].decode('ascii', errors='replace')
    return None


def full_extraction(probe, output_path: str = None) -> dict:
    """Run complete key extraction on a connected console.
    
    Returns a dict with all extracted data, optionally saves to JSON.
    """
    result = {
        "extraction_time": time.strftime("%Y-%m-%d %H:%M:%S UTC", time.gmtime()),
        "tool": "NexGen v0.1.0 kv-extract",
    }
    
    # 1. Kernel info
    kernel = extract_kernel_info(probe)
    result["kernel"] = kernel
    
    # 2. Find pi marker
    pi_addr = scan_for_pi_marker(probe)
    result["pi_marker"] = {
        "address": f"0x{pi_addr:08X}" if pi_addr else None,
        "found": pi_addr is not None,
    }
    
    if pi_addr:
        # 3. Extract derived key
        derived = extract_derived_key(probe, pi_addr)
        if derived:
            result["derived_key"] = {
                "hex": derived.hex().upper(),
                "size": len(derived),
                "entropy": len(set(derived)),
                "address": f"0x{pi_addr - 0x18:08X}",
            }
        
        # 4. Extract encrypted KV blocks
        blocks = extract_encrypted_kv_blocks(probe, pi_addr)
        result["kv_blocks"] = {}
        for name, block in blocks.items():
            result["kv_blocks"][name] = {
                "address": f"0x{block['offset']:08X}",
                "size": block["size"],
                "first_16": block["data"][:16].hex().upper(),
                "last_16": block["data"][-16:].hex().upper(),
            }
    
    # 5. XAM security context
    ctx = extract_xam_security_context(probe)
    if ctx:
        result["security_context"] = {
            "address": f"0x{ctx['address']:08X}",
            "security_token": f"0x{ctx['security_token']:08X}",
            "crypto_state": f"0x{ctx['crypto_state_ptr']:08X}",
            "key_material": f"0x{ctx['key_material_ptr']:08X}",
            "key_buffer_size": ctx["key_buffer_size"],
            "timeout_ms": ctx["timeout_ms"],
            "active": ctx["active"],
            "auth_dispatch": f"0x{ctx['auth_dispatch']:08X}",
        }
        
        # Try to read the actual key buffer
        if ctx["crypto_key_ptr"] and ctx["key_buffer_size"] > 0:
            key_buf = probe.read_memory(ctx["crypto_key_ptr"], min(ctx["key_buffer_size"], 256))
            if key_buf:
                # Look for 16-byte keys (AES key size)
                keys_found = []
                for i in range(0, len(key_buf) - 16, 16):
                    block = key_buf[i:i+16]
                    if len(set(block)) >= 10 and block.count(0) <= 2:
                        keys_found.append({
                            "offset": i,
                            "hex": block.hex().upper(),
                            "entropy": len(set(block)),
                        })
                result["crypto_keys"] = keys_found
    
    # 6. ETX telemetry
    etx = extract_etx_tokens(probe)
    if etx:
        result["etx"] = etx
    
    # 7. Boot chain hash
    boot_hash = extract_boot_hash(probe)
    if boot_hash:
        result["boot_hash"] = boot_hash.hex().upper()
    
    # 8. Running title
    title = extract_title_path(probe)
    if title:
        result["running_title"] = title
    
    # Generate LIVE server compatible output
    result["live_server_format"] = {
        "console_id": etx.get("machine_token", "UNKNOWN"),
        "serial": "UNKNOWN",
        "derived_key": result.get("derived_key", {}).get("hex", ""),
        "hmac_sha_key": "",  # Need KV decryption for this
        "console_priv_key": "",  # Need KV decryption for this
        "online_key": "",  # Derivable from the extracted keys
        "kernel_build": kernel.get("kernel_build", 0),
        "notes": f"Extracted {result['extraction_time']} via NexGen kv-extract",
    }
    
    # Save to file if requested
    if output_path:
        # Convert any bytes to hex strings for JSON serialization
        def make_serializable(obj):
            if isinstance(obj, bytes):
                return obj.hex().upper()
            elif isinstance(obj, dict):
                return {k: make_serializable(v) for k, v in obj.items()}
            elif isinstance(obj, list):
                return [make_serializable(v) for v in obj]
            return obj
        
        serializable = make_serializable(result)
        Path(output_path).write_text(json.dumps(serializable, indent=2))
    
    return result
