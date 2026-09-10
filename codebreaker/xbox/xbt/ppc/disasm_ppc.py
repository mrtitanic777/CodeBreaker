#!/usr/bin/env python3
"""
disasm_ppc.py — capstone recursive-descent PowerPC disassembler for Xbox 360.

The PC side of this toolchain disassembles x86: disasm16.py (16-bit NE) and the
PE path (32-bit). The Xbox 360 runs a big-endian **PowerPC** (the "Xenon" core),
so its code needs a different decoder entirely. This is that decoder — the PPC
analogue of disasm16.py, and it inherits the same design:

  * **Recursive descent, not a linear sweep.** From the entry points we decode
    one instruction, mark its bytes as code, and queue its branch target and
    fall-through — so data (jump tables, strings, blobs) is identified by never
    being reached, exactly as disasm16 does for x86.

  * PowerPC makes this cleaner than x86 in one crucial way: **every instruction
    is exactly 4 bytes, 4-byte aligned.** There is no instruction-boundary
    ambiguity to recover from — the reason the byte-exact PPC reconstructor
    (asmrecon_ppc.py) can be more confident than its 16-bit x86 cousin.

Big-endian throughout (CS_MODE_BIG_ENDIAN); Xenon game code is the 32-bit PPC
ABI with image bases up around 0x82000000.

    python disasm_ppc.py default.xex                 # disassemble .text
    python disasm_ppc.py default.xex --section .text --limit 40
    python disasm_ppc.py basefile.exe --raw          # a pre-extracted PE
"""
import argparse
import os
import sys
import threading

try:
    import capstone
    from capstone import ppc
    _HAVE_CS = True
except Exception:
    capstone = None
    ppc = None
    _HAVE_CS = False

# A capstone Cs handle is NOT thread-safe to share, and the whole-image
# recompile drives disassembly across a thread pool (parallel.tmap). So each
# thread gets its own Cs — safe under fan-out, and the availability check
# (_md() is None) still tells callers whether capstone is present at all.
_tls = threading.local()


def _md():
    if not _HAVE_CS:
        return None
    md = getattr(_tls, "md", None)
    if md is None:
        md = capstone.Cs(capstone.CS_ARCH_PPC,
                         capstone.CS_MODE_32 | capstone.CS_MODE_BIG_ENDIAN)
        md.detail = True
        _tls.md = md
    return md

WORD = 4  # PowerPC fixed instruction width

# VMX128 — the Xbox-360 Xenon vector extension. Stock Capstone's PPC mode does
# not decode it (it needs binutils "cell 64"), so those words come back
# undecodable. They live in primary opcodes 4 (VMX/VMX128), 5 and 6 (VMX128-only:
# vperm128, vpkd3d128, the 128-register load/store forms). We do NOT fabricate
# operands — we CLASSIFY a failed decode as a VMX128 vector op when its primary
# opcode is in that space, so vector code is not mistaken for data. A full
# operand renderer would port the VMX128 opcode tables (see ROADMAP).
_VMX128_PRIMARY = frozenset((4, 5, 6))


def is_vmx128(raw4):
    """True if this word is (very likely) a VMX128 vector instruction — primary
    opcode 4/5/6. Used only when Capstone has already failed to decode it."""
    return (raw4[0] >> 2) in _VMX128_PRIMARY


def _vmx_decode(word):
    """Native VMX128 decode -> (mnemonic, operands) or None. Imported lazily so
    the disassembler still loads if the vmx128 table module is unavailable."""
    try:
        import vmx128
        return vmx128.decode(word)
    except Exception:
        return None

# Flow-ending mnemonics. `b`/`ba` are unconditional branches; the `blr`/`bctr`
# family are the returns/computed jumps that terminate a straight-line run.
_ENDERS = {"b", "ba", "blr", "bctr", "bctrl", "rfi", "rfid"}
# `bl`/`bla`/`bctrl` LINK (they are calls) — they fall through, so not enders
# except bctrl which both calls and needs the return handled as fall-through.
_CALLS = {"bl", "bla", "bcl", "bctrl", "bclrl"}


def _decode_one(code, off, vaddr):
    """Decode a single 4-byte PPC instruction at byte offset `off`."""
    md = _md()
    if md is None or off + WORD > len(code):
        return None
    return next(iter(md.disasm(bytes(code[off:off + WORD]), vaddr + off, count=1)),
                None)


def classify(code, base_vaddr, entries):
    """Set of byte offsets that are reachable CODE by recursive descent from
    `entries` (which are given as VIRTUAL addresses). Everything else is data."""
    covered = set()
    if _md() is None:
        return covered
    n = len(code)
    seen = set()
    wl = [e - base_vaddr for e in entries]
    while wl:
        off = wl.pop()
        if off in seen or not (0 <= off < n) or off % WORD != 0:
            continue
        seen.add(off)
        ins = _decode_one(code, off, base_vaddr)
        if ins is None:
            # a VMX128 vector op is still real, fixed-width code — count it and
            # fall through (VMX128 ops never branch), so its function keeps flowing
            if is_vmx128(bytes(code[off:off + WORD])):
                for k in range(off, min(off + WORD, n)):
                    covered.add(k)
                wl.append(off + WORD)
            continue
        for k in range(off, min(off + WORD, n)):
            covered.add(k)
        mn = ins.mnemonic
        # follow a direct branch target (capstone resolves it to an absolute VA)
        is_branch = ins.group(capstone.CS_GRP_JUMP) or ins.group(capstone.CS_GRP_CALL)
        if is_branch:
            for op in ins.operands:
                if op.type == ppc.PPC_OP_IMM:
                    t = op.imm - base_vaddr
                    if 0 <= t < n:
                        wl.append(t)
        # fall through unless this instruction ends the run
        if mn not in _ENDERS:
            wl.append(off + WORD)
    return covered


def data_spans(code, base_vaddr, entries, minrun=WORD):
    """[(start_off, end_off)] maximal runs never reached by control flow."""
    covered = classify(code, base_vaddr, entries)
    out = []
    i, n = 0, len(code)
    while i < n:
        if i not in covered:
            j = i
            while j < n and j not in covered:
                j += 1
            if j - i >= minrun:
                out.append((i, j))
            i = j
        else:
            i += 1
    return out


def listing(code, base_vaddr, start=0, count=None):
    """Yield (vaddr, raw4, mnemonic, op_str) linearly. `count` caps rows.

    Linear because the emitter (asmrecon_ppc) wants every 4-byte slot accounted
    for; the code/data split above is what tells it which rows are real code."""
    n = len(code)
    off = start - (start % WORD)
    emitted = 0
    while off + WORD <= n:
        raw = bytes(code[off:off + WORD])
        ins = _decode_one(code, off, base_vaddr)
        if ins is None:
            dec = _vmx_decode(int.from_bytes(raw, "big"))
            if dec is not None:
                # native VMX128 decode (mnemonic + operands), byte-exact word
                yield (base_vaddr + off, raw, dec[0], dec[1])
            elif is_vmx128(raw):
                yield (base_vaddr + off, raw, "vmx128",
                       ".word 0x%08X  # VMX128 (op%d)" % (int.from_bytes(raw, "big"),
                                                          raw[0] >> 2))
            else:
                yield (base_vaddr + off, raw, ".long",
                       "0x%08X" % int.from_bytes(raw, "big"))
        else:
            yield (base_vaddr + off, raw, ins.mnemonic, ins.op_str)
        off += WORD
        emitted += 1
        if count is not None and emitted >= count:
            return


# --------------------------------------------------------------------------
# Getting to the code: accept a raw .xex, or an already-extracted PE basefile.
# --------------------------------------------------------------------------
def _code_section(path, section, raw):
    """Return (code_bytes, base_vaddr, entry_vaddr) for the chosen code section."""
    if raw:
        with open(path, "rb") as f:
            img = f.read()
        entry = None
        base = 0
    else:
        import xex
        x = xex.open_xex(path)
        img = x.basefile()
        entry = x.entry_point
        base = x.image_base or 0
    from xex import _pe_sections
    secs = _pe_sections(img)
    chosen = None
    for s in secs:
        if section and s["name"] == section:
            chosen = s
            break
        if not section and s["code"]:
            chosen = s
            break
    if chosen is None:
        raise SystemExit(f"no code section {section or '(first CODE)'} in {path}")
    # The XEX basefile is the LOADED image (flat by virtual address), so a
    # section's content sits at its VirtualAddress, NOT its PE PointerToRawData
    # (which is the stale on-disk offset — for .text they differ by the pre-.text
    # virtual padding). Map by vaddr; confirmed against the reference (guest
    # 0x82080000 -> mflr r12, matching sub_82080000).
    body = img[chosen["vaddr"]: chosen["vaddr"] + chosen["raw_size"]]
    sec_va = (base + chosen["vaddr"]) if base else chosen["vaddr"]
    return body, sec_va, (entry or sec_va)


def main():
    ap = argparse.ArgumentParser(description="PowerPC disassembler (Xbox 360)")
    ap.add_argument("path")
    ap.add_argument("--section", default=None, help="PE section (default: first CODE)")
    ap.add_argument("--raw", action="store_true",
                    help="path is an already-extracted PE basefile, not a .xex")
    ap.add_argument("--limit", type=int, default=60, help="rows to print")
    args = ap.parse_args()
    if _md() is None:
        print("capstone (PPC) not available: pip install capstone")
        sys.exit(2)

    body, base_va, entry_va = _code_section(args.path, args.section, args.raw)
    print(f"{os.path.basename(args.path)}  base=0x{base_va:08X}  "
          f"entry=0x{entry_va:08X}  {len(body):,} code bytes")
    covered = classify(body, base_va, [entry_va, base_va])
    for va, raw, mn, ops in listing(body, base_va, count=args.limit):
        off = va - base_va
        tag = " " if off in covered else "d"   # 'd' = classified data
        print("  %c %08X  %02X%02X%02X%02X  %-8s %s"
              % (tag, va, raw[0], raw[1], raw[2], raw[3], mn, ops))
    db = sum(e - s for s, e in data_spans(body, base_va, [entry_va, base_va]))
    print("  --- %d code bytes, %d classified data (%.1f%%)"
          % (len(body), db, 100.0 * db / max(1, len(body))))


if __name__ == "__main__":
    main()
