"""Page table analyzer — decode Xbox 360 hypervisor page table entries."""

import struct
from typing import Optional


# MMU Pathways (from HV deep dive)
PATHWAYS = {
    0: "UNPROTECTED",   # No encryption, direct physical access
    1: "PROTECTED",     # AES-128 ECB + CRC16, per-boot random key
    2: "CPU_SOC",       # CPU System-on-Chip internal (not accessible)
    3: "ENCRYPTED",     # Encrypted-only (no integrity check)
}

# Page sizes on Xbox 360
LARGE_PAGE_SIZE = 0x10000   # 64KB large pages
SMALL_PAGE_SIZE = 0x1000    # 4KB small pages


def parse_pte(raw: int) -> dict:
    """Parse a single Xbox 360 Page Table Entry.
    
    Bit layout (from HV research):
    Note: Exact bit layout is architecture-specific and partially undocumented.
    This is our best interpretation from live data analysis.
    """
    pte = {
        "raw": raw,
        "hex": f"0x{raw:08X}",
    }
    
    # Attempt multiple interpretations
    # Interpretation 1: Standard Xbox 360 PTE
    pte["bit31"] = (raw >> 31) & 1  # Valid / ReadOnly
    pte["bit30"] = (raw >> 30) & 1  # Data / Write
    pte["bit29"] = (raw >> 29) & 1  # NoExecute
    pte["bit28"] = (raw >> 28) & 1  # Valid/Present
    pte["pathway"] = raw & 3
    pte["pathway_name"] = PATHWAYS.get(pte["pathway"], f"Unknown({pte['pathway']})")
    
    # Physical address extraction (multiple possible layouts)
    # Layout A: bits 12-25 = page number (14 bits, 64KB pages)
    pte["phys_page_a"] = (raw >> 12) & 0x3FFF
    pte["phys_addr_a"] = pte["phys_page_a"] * LARGE_PAGE_SIZE
    
    # Layout B: bits 2-15 = page number
    pte["phys_page_b"] = (raw >> 2) & 0x3FFF
    pte["phys_addr_b"] = pte["phys_page_b"] * LARGE_PAGE_SIZE
    
    return pte


def parse_page_table(data: bytes, base: int = 0xCC000000) -> list:
    """Parse an array of page table entries."""
    entries = []
    for i in range(0, len(data) - 3, 4):
        raw = struct.unpack('>I', data[i:i+4])[0]
        pte = parse_pte(raw)
        pte["index"] = i // 4
        pte["address"] = base + i
        entries.append(pte)
    return entries


def analyze_page_table(entries: list) -> dict:
    """Analyze page table for patterns and statistics."""
    stats = {
        "total_entries": len(entries),
        "non_zero": sum(1 for e in entries if e["raw"] != 0),
        "zero": sum(1 for e in entries if e["raw"] == 0),
        "by_pathway": {},
        "valid_count": 0,
        "protected_pages": 0,
    }
    
    for e in entries:
        if e["raw"] == 0:
            continue
        
        pathway = e["pathway"]
        if pathway not in stats["by_pathway"]:
            stats["by_pathway"][pathway] = 0
        stats["by_pathway"][pathway] += 1
        
        if e["bit28"]:
            stats["valid_count"] += 1
        if pathway == 1:
            stats["protected_pages"] += 1
    
    return stats


def find_protected_regions(entries: list) -> list:
    """Find contiguous regions of protected (encrypted) pages."""
    regions = []
    current_start = None
    current_pathway = None
    
    for e in entries:
        if e["raw"] == 0:
            if current_start is not None:
                regions.append({
                    "start_index": current_start,
                    "end_index": e["index"] - 1,
                    "pathway": current_pathway,
                    "pathway_name": PATHWAYS.get(current_pathway, "?"),
                    "count": e["index"] - current_start,
                })
                current_start = None
            continue
        
        pathway = e["pathway"]
        if current_start is None or pathway != current_pathway:
            if current_start is not None:
                regions.append({
                    "start_index": current_start,
                    "end_index": e["index"] - 1,
                    "pathway": current_pathway,
                    "pathway_name": PATHWAYS.get(current_pathway, "?"),
                    "count": e["index"] - current_start,
                })
            current_start = e["index"]
            current_pathway = pathway
    
    if current_start is not None:
        regions.append({
            "start_index": current_start,
            "end_index": entries[-1]["index"],
            "pathway": current_pathway,
            "pathway_name": PATHWAYS.get(current_pathway, "?"),
            "count": entries[-1]["index"] - current_start + 1,
        })
    
    return regions


DESCRIPTION = "parse an Xbox 360 page table dump (PTEs, protected regions)"


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: pagetable <dump> [base_hex]"); return
    data = open(a[0], "rb").read()
    kw = {"base": int(a[1], 16)} if len(a) > 1 else {}
    entries = parse_page_table(data, **kw)
    for k, v in analyze_page_table(entries).items():
        print("  %-22s %s" % (k, v))
    prot = find_protected_regions(entries)
    print("  protected regions: %d" % len(prot))


if __name__ == "__main__":
    main()
