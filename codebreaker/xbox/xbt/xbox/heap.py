"""Heap analyzer — parse XAM/kernel heap structures from live memory."""

import struct
from typing import Optional


# XAM heap signatures
XAM_HEAP_MAGIC = 0xEEFFEEFF
XAM_ALLOC_BASE = 0x8C000000

# Kernel heap pool tags
POOL_TAGS = {
    0x41: "General",
    0x42: "IO",
    0x43: "Security",
    0x44: "Network",
    0x45: "Graphics",
}


def parse_xam_heap_header(data: bytes, base: int = XAM_ALLOC_BASE) -> dict:
    """Parse the XAM heap header structure at 0x8C000000."""
    if len(data) < 0x70:
        return {"error": "Data too small"}
    
    heap = {
        "base": base,
        "pool_size": struct.unpack('>I', data[0:4])[0],
        "flags": struct.unpack('>I', data[4:8])[0],
    }
    
    # Check for magic
    magic = struct.unpack('>I', data[0x10:0x14])[0]
    heap["magic"] = magic
    heap["valid"] = (magic == XAM_HEAP_MAGIC)
    
    # Page/block sizes
    heap["page_size"] = struct.unpack('>I', data[0x14:0x18])[0]
    heap["total_pages"] = struct.unpack('>I', data[0x1C:0x20])[0]
    heap["used_pages"] = struct.unpack('>I', data[0x20:0x24])[0]
    heap["free_pages"] = struct.unpack('>I', data[0x24:0x28])[0]
    heap["block_size"] = struct.unpack('>I', data[0x28:0x2C])[0]
    heap["block_count"] = struct.unpack('>I', data[0x2C:0x30])[0]
    
    # Pool bitmap info
    heap["bitmap_bits"] = struct.unpack('>I', data[0x30:0x34])[0]
    heap["bitmap_mask"] = struct.unpack('>I', data[0x34:0x38])[0]
    heap["free_hint"] = struct.unpack('>I', data[0x38:0x3C])[0]
    
    # Allocation list pointers
    heap["alloc_list_ptr"] = struct.unpack('>I', data[0x4C:0x50])[0]
    heap["free_list_flink"] = struct.unpack('>I', data[0x58:0x5C])[0]
    heap["free_list_blink"] = struct.unpack('>I', data[0x5C:0x60])[0]
    heap["pool_table_ptr"] = struct.unpack('>I', data[0x60:0x64])[0]
    
    return heap


def parse_linked_lists(data: bytes, base: int, start_offset: int = 0x178,
                       max_count: int = 64) -> list:
    """Parse an array of LIST_ENTRY structures (Flink/Blink pairs)."""
    lists = []
    offset = start_offset
    
    while offset + 8 <= len(data) and len(lists) < max_count:
        flink = struct.unpack('>I', data[offset:offset+4])[0]
        blink = struct.unpack('>I', data[offset+4:offset+8])[0]
        
        self_addr = base + offset
        
        # Check if this looks like a valid list entry
        if (flink & 0xFF000000) == (base & 0xFF000000) or flink == 0:
            is_empty = (flink == self_addr and blink == self_addr)
            is_null = (flink == 0 and blink == 0)
            
            if not is_null:
                lists.append({
                    "address": self_addr,
                    "flink": flink,
                    "blink": blink,
                    "empty": is_empty,
                    "entries": 0 if is_empty else 1,  # Approximate
                })
        
        offset += 8
    
    return lists


def parse_pool_entry(data: bytes, base: int) -> dict:
    """Parse a pool allocation entry (allocation bitmap node)."""
    if len(data) < 0x40:
        return {"error": "Data too small"}
    
    entry = {
        "address": base,
        "flags": struct.unpack('>I', data[0:4])[0],
        "block_size": struct.unpack('>I', data[4:8])[0],
        "flink": struct.unpack('>I', data[8:12])[0],
        "blink": struct.unpack('>I', data[12:16])[0],
    }
    
    # Pool type from flags
    pool_type = (entry["flags"] >> 8) & 0xFF
    entry["pool_type"] = pool_type
    entry["pool_name"] = POOL_TAGS.get(pool_type, f"Type_{pool_type:02X}")
    
    # Bitmap at offset 0x30 (128 bits = 16 bytes)
    if len(data) >= 0x40:
        bitmap = data[0x30:0x40]
        entry["bitmap"] = bitmap
        # Count free bits (1 = free, 0 = allocated)
        total_bits = 128
        free_bits = sum(bin(b).count('1') for b in bitmap)
        entry["total_blocks"] = total_bits
        entry["free_blocks"] = free_bits
        entry["used_blocks"] = total_bits - free_bits
        entry["utilization"] = f"{100 * (total_bits - free_bits) / total_bits:.0f}%"
    
    return entry


def analyze_heap_fragmentation(lists: list) -> dict:
    """Analyze heap fragmentation from list data."""
    stats = {
        "total_lists": len(lists),
        "empty_lists": sum(1 for l in lists if l["empty"]),
        "active_lists": sum(1 for l in lists if not l["empty"]),
    }
    return stats


DESCRIPTION = "analyze an Xbox 360 XAM heap dump (header, pools, fragmentation)"


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: heap <xam-heap-dump> [base_hex]"); return
    data = open(a[0], "rb").read()
    base = int(a[1], 16) if len(a) > 1 else XAM_ALLOC_BASE
    for k, v in parse_xam_heap_header(data, base).items():
        print("  %-22s %s" % (k, v))
    try:
        lists = parse_linked_lists(data, base)
        for k, v in analyze_heap_fragmentation(lists).items():
            print("  frag.%-16s %s" % (k, v))
    except Exception as e:
        print("  (list analysis: %s)" % e)


if __name__ == "__main__":
    main()
