"""BrutalBranch analyzer — offline kernel patch candidate finder.

Based on the BrutalBranch technique by @InvoxiPlayGames.

The technique uses HvxKeysExSetKey/GetKey to corrupt encrypted kernel
memory. By precalculating unconditional branches to every BLR instruction
in the kernel, you can check each corruption attempt against the full
set of valid branch targets instead of waiting for a single BLR.

This tool:
  1. Scans a kernel binary for all BLR (0x4E800020) instructions
  2. Given a patch address, computes the PPC unconditional branch
     instruction (opcode 18) to each BLR
  3. Reports all valid candidates, reducing the bruteforce keyspace
     from 1-in-4B to 1-in-~1.8M per iteration
"""

import struct
from typing import Optional


# PPC instruction constants
BLR_OPCODE = 0x4E800020
BRANCH_OPCODE_MASK = 0xFC000003  # bits 0-5 and 30-31
BRANCH_OPCODE = 0x48000000       # opcode 18, AA=0, LK=0


def find_blr_offsets(data: bytes, base_address: int = 0x80000000) -> list:
    """Find all BLR instructions in a kernel binary.

    Returns list of (offset_in_file, virtual_address).
    """
    blrs = []
    for i in range(0, len(data) - 3, 4):
        instr = struct.unpack_from(">I", data, i)[0]
        if instr == BLR_OPCODE:
            blrs.append((i, base_address + i))
    return blrs


def compute_branch(from_addr: int, to_addr: int) -> Optional[int]:
    """Compute a PPC unconditional branch instruction from from_addr to to_addr.

    The branch displacement is 26 bits (signed), giving ±32MB range.
    Returns the 32-bit instruction word, or None if out of range.
    """
    offset = to_addr - from_addr
    # PPC branch: 26-bit signed displacement (must be aligned to 4)
    if offset < -0x2000000 or offset > 0x1FFFFFF:
        return None
    if offset & 3:
        return None
    # Encode: opcode 18 (bits 0-5), LI field (bits 6-29), AA=0, LK=0
    li = offset & 0x03FFFFFC
    return BRANCH_OPCODE | li


def analyze_kernel(data: bytes, patch_address: int,
                   base_address: int = 0x80000000,
                   target_func: str = "XeKeysVerifyRSASignature") -> dict:
    """Analyze a kernel binary for BrutalBranch patch candidates.

    Args:
        data: Raw kernel binary
        patch_address: Virtual address of the instruction to corrupt
        base_address: Kernel load address (default 0x80000000)
        target_func: Name of the function being patched (for display)

    Returns dict with:
        blr_count: Total BLR instructions found
        candidates: List of (branch_instruction, target_blr_address)
        patch_address: The address being patched
        keyspace: Estimated bruteforce keyspace (4294967296 / candidate_count)
    """
    blrs = find_blr_offsets(data, base_address)
    candidates = []

    for _, blr_addr in blrs:
        branch = compute_branch(patch_address, blr_addr)
        if branch is not None:
            candidates.append((branch, blr_addr))

    keyspace = 0xFFFFFFFF // max(len(candidates), 1)

    return {
        "target_func": target_func,
        "patch_address": patch_address,
        "base_address": base_address,
        "kernel_size": len(data),
        "blr_count": len(blrs),
        "candidates": candidates,
        "keyspace": keyspace,
    }


def format_report(result: dict) -> str:
    """Format an analysis result as a human-readable report."""
    lines = []
    lines.append(f"BrutalBranch Analysis: {result['target_func']}")
    lines.append(f"  Patch address:  0x{result['patch_address']:08X}")
    lines.append(f"  Kernel base:    0x{result['base_address']:08X}")
    lines.append(f"  Kernel size:    0x{result['kernel_size']:X} ({result['kernel_size']:,} bytes)")
    lines.append(f"  BLR count:      {result['blr_count']:,}")
    lines.append(f"  Valid branches: {len(result['candidates']):,}")
    lines.append(f"  Keyspace:       ~1 in {result['keyspace']:,} per iteration")
    lines.append("")

    if result['candidates']:
        lines.append("  Candidate branches (instruction → BLR target):")
        for i, (instr, target) in enumerate(result['candidates'][:50]):
            lines.append(f"    0x{instr:08X} → 0x{target:08X}")
        if len(result['candidates']) > 50:
            lines.append(f"    ... and {len(result['candidates']) - 50} more")

    return "\n".join(lines)


# Known patch targets for common kernel versions
KNOWN_TARGETS = {
    17559: {
        "XeKeysVerifyRSASignature": 0x80109C90,
    },
}


DESCRIPTION = "BrutalBranch: find encrypted-memory branch-patch candidates in a kernel"


def main():
    import sys
    a = sys.argv[1:]
    if len(a) < 2 or a[0] in ("-h", "--help"):
        print("usage: brutalbranch <kernel.bin> <patch_addr_hex> [base_hex]"); return
    data = open(a[0], "rb").read()
    patch = int(a[1], 16)
    base = int(a[2], 16) if len(a) > 2 else 0x80000000
    print(format_report(analyze_kernel(data, patch, base)))


if __name__ == "__main__":
    main()
