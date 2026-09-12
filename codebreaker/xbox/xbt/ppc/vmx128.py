"""VMX128 decoder — the Xbox 360 (Xenon) 128-vector-register AltiVec extension.

Stock PowerPC disassemblers get VMX128 silently WRONG: the two extra register
bits were scavenged from what a base decoder reads as opcode-extension space, so
a VMX128 word is usually a *legal but different* stock instruction (e.g.
`lvx128 vr1,r3,r4` == `0x102320c3` decodes as `vucmprlb 1,3,4` under llvm-mc with
no error). This module decodes them correctly.

Reimplemented clean-room from the VMX128 decode specification (field layout +
the 77-row opcode table), which is public-table knowledge verified against the
compiler's own `/FAcs` listing — black-box, not disassembly of any binary.

Field layout (IBM bit numbering, bit 0 = MSB):
  VDS128 = bits 28..29 ++ 6..10   (dest, also a source in 3-op forms)
  VA128  = bits 21, 26, 11..15    (high bits straddle the extended opcode)
  VB128  = bits 30..31 ++ 16..20
  VC128  = bits 23..25            (3-bit: vperm128's vC is only vr0..vr7)
"""


def vds128(w):
    return (((w >> 2) & 3) << 5) | ((w >> 21) & 31)


def va128(w):
    return (((w >> 10) & 1) << 6) | (((w >> 5) & 1) << 5) | ((w >> 16) & 31)


def vb128(w):
    return ((w & 3) << 5) | ((w >> 11) & 31)


def vc128(w):
    return (w >> 6) & 7


def _rA(w):
    return (w >> 16) & 31


def _rB(w):
    return (w >> 11) & 31


# (mnemonic, pattern, mask) — the 77 VMX128 rows, verified 6701/6701 against the
# MSVC back end's own /FAcs listing. Microsoft's spellings for the convert ops.
_TABLE = [
    ("lvewx128", 0x10000083, 0xFC0007F3), ("lvlx128", 0x10000403, 0xFC0007F3),
    ("lvlxl128", 0x10000603, 0xFC0007F3), ("lvrx128", 0x10000443, 0xFC0007F3),
    ("lvrxl128", 0x10000643, 0xFC0007F3), ("lvsl128", 0x10000003, 0xFC0007F3),
    ("lvsr128", 0x10000043, 0xFC0007F3), ("lvx128", 0x100000C3, 0xFC0007F3),
    ("lvxl128", 0x100002C3, 0xFC0007F3), ("stvewx128", 0x10000183, 0xFC0007F3),
    ("stvlx128", 0x10000503, 0xFC0007F3), ("stvlxl128", 0x10000703, 0xFC0007F3),
    ("stvrx128", 0x10000543, 0xFC0007F3), ("stvrxl128", 0x10000743, 0xFC0007F3),
    ("stvx128", 0x100001C3, 0xFC0007F3), ("stvxl128", 0x100003C3, 0xFC0007F3),
    ("vsldoi128", 0x10000010, 0xFC000010),
    ("vaddfp128", 0x14000010, 0xFC0003D0), ("vand128", 0x14000210, 0xFC0003D0),
    ("vandc128", 0x14000250, 0xFC0003D0), ("vmaddcfp128", 0x14000110, 0xFC0003D0),
    ("vmaddfp128", 0x140000D0, 0xFC0003D0), ("vmsum3fp128", 0x14000190, 0xFC0003D0),
    ("vmsum4fp128", 0x140001D0, 0xFC0003D0), ("vmulfp128", 0x14000090, 0xFC0003D0),
    ("vnmsubfp128", 0x14000150, 0xFC0003D0), ("vnor128", 0x14000290, 0xFC0003D0),
    ("vor128", 0x140002D0, 0xFC0003D0), ("vperm128", 0x14000000, 0xFC000210),
    ("vpkshss128", 0x14000200, 0xFC0003D0), ("vpkshus128", 0x14000240, 0xFC0003D0),
    ("vpkswss128", 0x14000280, 0xFC0003D0), ("vpkswus128", 0x140002C0, 0xFC0003D0),
    ("vpkuhum128", 0x14000300, 0xFC0003D0), ("vpkuhus128", 0x14000340, 0xFC0003D0),
    ("vpkuwum128", 0x14000380, 0xFC0003D0), ("vpkuwus128", 0x140003C0, 0xFC0003D0),
    ("vsel128", 0x14000350, 0xFC0003D0), ("vslo128", 0x14000390, 0xFC0003D0),
    ("vsro128", 0x140003D0, 0xFC0003D0), ("vsubfp128", 0x14000050, 0xFC0003D0),
    ("vxor128", 0x14000310, 0xFC0003D0),
    ("vcfpsxws128", 0x18000230, 0xFC0007F0), ("vcfpuxws128", 0x18000270, 0xFC0007F0),
    ("vcmpbfp128", 0x18000180, 0xFC000390), ("vcmpeqfp128", 0x18000000, 0xFC000390),
    ("vcmpequw128", 0x18000200, 0xFC000390), ("vcmpgefp128", 0x18000080, 0xFC000390),
    ("vcmpgtfp128", 0x18000100, 0xFC000390), ("vcsxwfp128", 0x180002B0, 0xFC0007F0),
    ("vcuxwfp128", 0x180002F0, 0xFC0007F0), ("vexptefp128", 0x180006B0, 0xFC1F07F0),
    ("vlogefp128", 0x180006F0, 0xFC1F07F0), ("vmaxfp128", 0x18000280, 0xFC0003D0),
    ("vminfp128", 0x180002C0, 0xFC0003D0), ("vmrghw128", 0x18000300, 0xFC0003D0),
    ("vmrglw128", 0x18000340, 0xFC0003D0), ("vpermwi128", 0x18000210, 0xFC000630),
    ("vpkd3d128", 0x18000610, 0xFC000730), ("vrefp128", 0x18000630, 0xFC1F07F0),
    ("vrfim128", 0x18000330, 0xFC1F07F0), ("vrfin128", 0x18000370, 0xFC1F07F0),
    ("vrfip128", 0x180003B0, 0xFC1F07F0), ("vrfiz128", 0x180003F0, 0xFC1F07F0),
    ("vrlimi128", 0x18000710, 0xFC000730), ("vrlw128", 0x18000050, 0xFC0003D0),
    ("vrsqrtefp128", 0x18000670, 0xFC1F07F0), ("vslw128", 0x180000D0, 0xFC0003D0),
    ("vspltisw128", 0x18000770, 0xFC0007F0), ("vspltw128", 0x18000730, 0xFC0007F0),
    ("vsraw128", 0x18000150, 0xFC0003D0), ("vsrw128", 0x180001D0, 0xFC0003D0),
    ("vupkd3d128", 0x180007F0, 0xFC0007F0), ("vupkhsb128", 0x18000380, 0xFC1F07F0),
    ("vupkhsh128", 0x180007A0, 0xFC1F07F0), ("vupklsb128", 0x180003C0, 0xFC1F07F0),
    ("vupklsh128", 0x180007E0, 0xFC1F07F0),
]
# Match the most specific (widest mask) row first so a broad row never shadows.
_TABLE.sort(key=lambda r: bin(r[2]).count("1"), reverse=True)

# Operand shape per mnemonic (only the exceptions; the default is by opcode).
_LOADSTORE = {r[0] for r in _TABLE if r[0][0] in "ls" and r[0].startswith(("lv", "stv"))}
_UNARY = {"vcfpsxws128", "vcfpuxws128", "vcsxwfp128", "vcuxwfp128", "vexptefp128",
          "vlogefp128", "vrefp128", "vrfim128", "vrfin128", "vrfip128", "vrfiz128",
          "vrsqrtefp128", "vupkhsb128", "vupkhsh128", "vupklsb128", "vupklsh128"}
_MADD = {"vmaddfp128", "vmaddcfp128", "vnmsubfp128"}


def _fmt_reg(prefix, n):
    return "%s%d" % (prefix, n)


def decode(word):
    """Decode a 32-bit big-endian instruction word. Returns (mnemonic, operands)
    if it is a VMX128 instruction (primary opcode 4/5/6), else None."""
    w = word & 0xFFFFFFFF
    if (w >> 26) not in (4, 5, 6):
        return None
    for mnem, pat, mask in _TABLE:
        if (w & mask) == pat:
            return mnem, _operands(mnem, w)
    return None


def _operands(mnem, w):
    vD, vA, vB, vC = vds128(w), va128(w), vb128(w), vc128(w)
    if mnem in _LOADSTORE:
        return "vr%d,r%d,r%d" % (vD, _rA(w), _rB(w))
    if mnem == "vsldoi128":
        shb = (w >> 6) & 0xF                       # SHB bits 22..25
        return "vr%d,vr%d,vr%d,%d" % (vD, vA, vB, shb)
    if mnem == "vperm128":
        return "vr%d,vr%d,vr%d,vr%d" % (vD, vA, vB, vC)
    if mnem in _MADD:
        return "vr%d,vr%d,vr%d,vr%d" % (vD, vA, vD, vB)   # dest is also the 3rd src
    if mnem == "vspltisw128":
        simm = (w >> 16) & 31
        return "vr%d,%d" % (vD, simm - 32 if simm >= 16 else simm)
    if mnem == "vspltw128":
        return "vr%d,vr%d,%d" % (vD, vB, (w >> 16) & 7)
    if mnem == "vpermwi128":
        perm = (((w >> 6) & 7) << 5) | ((w >> 16) & 31)   # PERM 23..25 ++ 11..15
        return "vr%d,vr%d,%d" % (vD, vB, perm)
    if mnem in ("vpkd3d128", "vupkd3d128"):
        d3d = (w >> 18) & 7                          # D3DType 11..13
        return "vr%d,vr%d,%d" % (vD, vB, d3d)
    if mnem == "vrlimi128":
        d3d = (w >> 18) & 7
        z = (w >> 6) & 3                             # Zimm 24..25
        return "vr%d,vr%d,%d,%d" % (vD, vB, d3d, z)
    if mnem in _UNARY:
        return "vr%d,vr%d" % (vD, vB)
    return "vr%d,vr%d,vr%d" % (vD, vA, vB)          # default: 3-register op


def is_vmx128(word):
    return decode(word) is not None


def format_insn(word):
    r = decode(word)
    return None if r is None else "%s %s" % r


DESCRIPTION = "decode Xbox 360 VMX128 vector instructions (the 128-register AltiVec extension)"


def _coverage(path):
    """Scan a XEX's (or basefile's) .text and report vector-decode coverage:
    how many primary-op 4/5/6 words the VMX128 table renders, how many are
    plain AltiVec/VMX that Capstone renders, and how many neither can decode."""
    import os
    try:
        import xex as _xex
        if path.lower().endswith(".xex"):
            x = _xex.open_xex(path)
            img = x.basefile()
            secs = x.sections()
        else:
            img = open(path, "rb").read()
            from xex import _pe_sections
            secs = _pe_sections(img)
    except Exception as e:
        print("vmx128 --coverage: cannot load %s: %s" % (path, e))
        return 2
    text = next((s for s in secs if s["name"] == ".text"), None)
    if not text:
        print("vmx128 --coverage: no .text section")
        return 2
    o = text["raw_off"]
    n = min(text["vsize"], text["raw_size"])
    try:
        import capstone
        cs = capstone.Cs(capstone.CS_ARCH_PPC,
                         capstone.CS_MODE_BIG_ENDIAN | capstone.CS_MODE_64)
    except Exception:
        cs = None
    total = vmx = altivec = neither = 0
    for off in range(0, n - 3, 4):
        w = int.from_bytes(img[o + off:o + off + 4], "big")
        if (w >> 26) not in (4, 5, 6):
            continue
        total += 1
        if decode(w) is not None:
            vmx += 1
        elif cs and list(cs.disasm(w.to_bytes(4, "big"), 0)):
            altivec += 1
        else:
            neither += 1
    rendered = vmx + altivec
    print("VMX128 coverage on %s .text:" % os.path.basename(path))
    print("  primary-op 4/5/6 words : %d" % total)
    print("  VMX128 (this table)    : %d" % vmx)
    print("  AltiVec/VMX (capstone) : %d%s" % (altivec, "" if cs else "  (capstone unavailable)"))
    print("  undecoded              : %d" % neither)
    if total:
        print("  operand coverage       : %d/%d (%.1f%%)" % (rendered, total, 100.0 * rendered / total))
    return 0


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: vmx128 <hex-word> [hex-word ...]   decode VMX128 instruction words\n"
              "       vmx128 --selftest                  verify all 77 opcode rows\n"
              "       vmx128 --coverage <file.xex>       vector-decode coverage over .text")
        return
    if a[0] == "--selftest":
        bad = [m for m, p, _ in _TABLE if (decode(p) or (None,))[0] != m]
        print("VMX128 self-test: %d/%d rows decode to their mnemonic%s" % (
            len(_TABLE) - len(bad), len(_TABLE), "" if not bad else "  BAD: " + ",".join(bad)))
        sys.exit(1 if bad else 0)
    if a[0] == "--coverage":
        if len(a) < 2:
            print("usage: vmx128 --coverage <file.xex|basefile>")
            sys.exit(2)
        sys.exit(_coverage(a[1]))
    for tok in a:
        w = int(tok, 16)
        r = format_insn(w)
        print("0x%08X  %s" % (w, r if r else "(not VMX128)"))


if __name__ == "__main__":
    main()
