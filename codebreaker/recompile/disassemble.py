"""disassemble.py -- Xbox 360 PowerPC as (mnemonic, operands) text, for the recompiler.

    from codebreaker.recompile import disassemble
    disassemble.listing(code, base_va)              (va, raw word, mnemonic, operands), every word
    disassemble.classify(code, base_va, entries)    offsets flow reaches from `entries`: code
    disassemble.code_section(path)                  (bytes, address, entry) of an image's code

    python -m codebreaker.recompile.disassemble <image or .xex> [--limit N]

A word is read as VMX128 first (codebreaker/decompile/vmx128.py), the Xenon's vector set: Capstone
reads most VMX128 words as other, later instructions (ISA 2.07/3.0, which the Xenon does not
have). Everything else is Capstone's (one decoder per thread: a handle is not safe to share, and
the whole-image recompile runs on a thread pool). The vector builders read VMX128 operands in
that table's order. Every instruction is one word, four-byte aligned, so there is no
boundary to recover: code is what flow reaches, the rest is data.
"""
import os
import sys
import threading

try:
    import capstone
    from capstone import ppc
except ImportError:
    capstone = ppc = None

from codebreaker.decompile import vmx128

WORD = 4
_tls = threading.local()
# flow does not go on past these; calls (bl bla bcl bctrl bclrl) come back
_ENDERS = {"b", "ba", "blr", "bctr", "bctrl", "rfi", "rfid"}
_CALLS = {"bl", "bla", "bcl", "bctrl", "bclrl"}


def _md():
    """This thread's decoder, or None when Capstone is not installed."""
    if capstone is None:
        return None
    md = getattr(_tls, "md", None)
    if md is None:
        md = capstone.Cs(capstone.CS_ARCH_PPC, capstone.CS_MODE_32 | capstone.CS_MODE_BIG_ENDIAN)
        md.detail = True
        _tls.md = md
    return md


def is_vmx128(raw4):
    """A word in VMX128's primary opcodes (4, 5, 6) -- a vector instruction, not data."""
    return (raw4[0] >> 2) in (4, 5, 6)


def _decode_one(code, off, vaddr):
    md = _md()
    if md is None or off + WORD > len(code):
        return None
    return next(iter(md.disasm(bytes(code[off:off + WORD]), vaddr + off, count=1)), None)


def classify(code, base_vaddr, entries):
    """The byte offsets of `code` that flow reaches from `entries` (addresses)."""
    covered = set()
    if _md() is None:
        return covered
    n, seen, work = len(code), set(), [e - base_vaddr for e in entries]
    while work:
        off = work.pop()
        if off in seen or not 0 <= off < n or off % WORD:
            continue
        seen.add(off)
        ins = _decode_one(code, off, base_vaddr)
        if ins is None:
            if is_vmx128(bytes(code[off:off + WORD])):     # vector code: falls through, never branches
                covered.update(range(off, min(off + WORD, n)))
                work.append(off + WORD)
            continue
        covered.update(range(off, min(off + WORD, n)))
        if ins.group(capstone.CS_GRP_JUMP) or ins.group(capstone.CS_GRP_CALL):
            for op in ins.operands:
                if op.type == ppc.PPC_OP_IMM and 0 <= op.imm - base_vaddr < n:
                    work.append(op.imm - base_vaddr)
        if ins.mnemonic not in _ENDERS:
            work.append(off + WORD)
    return covered


def data_spans(code, base_vaddr, entries, minrun=WORD):
    """[(start, end)] -- the runs of offsets flow never reaches."""
    covered = classify(code, base_vaddr, entries)
    out, i, n = [], 0, len(code)
    while i < n:
        if i in covered:
            i += 1
            continue
        j = i
        while j < n and j not in covered:
            j += 1
        if j - i >= minrun:
            out.append((i, j))
        i = j
    return out


def listing(code, base_vaddr, start=0, count=None):
    """(address, raw word, mnemonic, operands) for every word from `start`, `count` at most. A word
    neither decoder reads is `.long 0x...`."""
    off, emitted = start - start % WORD, 0
    while off + WORD <= len(code):
        raw = bytes(code[off:off + WORD])
        vec = vmx128.decode(int.from_bytes(raw, "big"))    # first: Capstone reads these as other
        ins = None if vec else _decode_one(code, off, base_vaddr)   # instructions (ISA 2.07/3.0)
        if vec:
            yield (base_vaddr + off, raw) + vec
        elif ins is not None:
            yield base_vaddr + off, raw, ins.mnemonic, ins.op_str
        else:
            yield base_vaddr + off, raw, ".long", "0x%08X" % int.from_bytes(raw, "big")
        off += WORD
        emitted += 1
        if count is not None and emitted >= count:
            return


def image(path):
    """(image bytes, base address, entry, sections [(name, address, size, executable)]) for a
    PowerPC image or a .xex (its basefile)."""
    from codebreaker.formats import powerpc_image, xex
    img = powerpc_image.read(xex.basefile(path) if xex.is_xex(path) else path)
    flat = bytearray(max(va + n for va, _o, n in img.spans) - img.base)
    for va, off, n in img.spans:
        flat[va - img.base:va - img.base + n] = img.data[off:off + n]
    sections = [(name, va, size, bool(chars & 0x20000000 or name == ".text"))
                for name, va, size, chars in img.sections]
    return bytes(flat), img.base, img.entry or img.code[0], sections


def code_section(path):
    """(bytes, address, entry) of the image's code section."""
    from codebreaker.formats import powerpc_image, xex
    img = powerpc_image.read(xex.basefile(path) if xex.is_xex(path) else path)
    lo, hi = img.code
    return img.bytes_at(lo, hi - lo), lo, img.entry or lo


def main(argv):
    if not argv:
        print(__doc__.split("\n\n")[0])
        return 2
    if _md() is None:
        print("capstone is not installed (pip install capstone)")
        return 2
    limit = int(argv[argv.index("--limit") + 1]) if "--limit" in argv else 60
    body, base, entry = code_section(argv[0])
    print("%s  code at 0x%08X, entry 0x%08X, %d bytes" % (os.path.basename(argv[0]), base, entry, len(body)))
    covered = classify(body, base, [entry, base])
    for va, raw, mn, ops in listing(body, base, count=limit):
        print("  %s %08X  %s  %-10s %s" % (" " if va - base in covered else "d", va, raw.hex(), mn, ops))
    data = sum(e - s for s, e in data_spans(body, base, [entry, base]))
    print("  --- %d bytes, %d read as data (%.1f%%)" % (len(body), data, 100.0 * data / max(1, len(body))))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
