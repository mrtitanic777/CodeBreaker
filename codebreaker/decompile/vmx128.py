"""VMX128 -- the Xbox 360 (Xenon) 128-register extension of AltiVec.

    python -m codebreaker.decompile.vmx128 <hex-word> ...      decode instruction words
    python -m codebreaker.decompile.vmx128 --coverage <file>   vector decode coverage over .text
    python -m codebreaker.decompile.vmx128 --sleigh <out>      write the SLEIGH (vmx128.sinc)

Stock PowerPC decoders get VMX128 silently wrong: its extra register bits sit where a base
decoder reads opcode-extension bits, so a VMX128 word is usually a legal but different stock
instruction (`lvx128 v1,r3,r4` = 0x102320C3 reads as `vucmprlb 1,3,4`). This table is the one
place VMX128 is described: the native decompiler's SLEIGH (languages/src/ppc/vmx128.sinc, the
PowerPC:BE:64:Xenon language) is generated from it.

Fields (IBM numbering, bit 0 = most significant):
  VD128 = bits 28..29 ++ 6..10   (destination; also a source in the multiply-add forms)
  VA128 = bits 21, 26 ++ 11..15  (its high bits sit inside the extended opcode)
  VB128 = bits 30..31 ++ 16..20
  VC128 = bits 23..25            (3 bits: vperm128's control is v0..v7)

Clean-room from the decode specification (the field layout and the 77-row opcode table),
checked against the Xbox 360 compiler's own /FAcs listings: every row decodes to its mnemonic."""
import re
import sys


def vd128(w):
    return (((w >> 2) & 3) << 5) | ((w >> 21) & 31)


def va128(w):
    return (((w >> 10) & 1) << 6) | (((w >> 5) & 1) << 5) | ((w >> 16) & 31)


def vb128(w):
    return ((w & 3) << 5) | ((w >> 11) & 31)


def vc128(w):
    return (w >> 6) & 7


# (mnemonic, pattern, mask, form, semantics): the 77 rows. `form` is the operand shape (FORMS);
# `semantics` is what the SLEIGH does: an expression over vD vA vB vC (and EA, IMM) for the
# operations C can say plainly, else the name of the user operation the decompiler shows.
ROWS = [
    ("lvewx128", 0x10000083, 0xFC0007F3, "load", "loadVectorElementWordIndexed"),
    ("lvlx128", 0x10000403, 0xFC0007F3, "load", "loadVectorLeftIndexed"),
    ("lvlxl128", 0x10000603, 0xFC0007F3, "load", "loadVectorLeftIndexedLRU"),
    ("lvrx128", 0x10000443, 0xFC0007F3, "load", "loadVectorRightIndexed"),
    ("lvrxl128", 0x10000643, 0xFC0007F3, "load", "loadVectorRightIndexedLRU"),
    ("lvsl128", 0x10000003, 0xFC0007F3, "load", "loadVectorForShiftLeft"),
    ("lvsr128", 0x10000043, 0xFC0007F3, "load", "loadVectorForShiftRight"),
    ("lvx128", 0x100000C3, 0xFC0007F3, "load", "=load"),
    ("lvxl128", 0x100002C3, 0xFC0007F3, "load", "=load"),
    ("stvewx128", 0x10000183, 0xFC0007F3, "store", "storeVectorElementWordIndexed"),
    ("stvlx128", 0x10000503, 0xFC0007F3, "store", "storeVectorLeftIndexed"),
    ("stvlxl128", 0x10000703, 0xFC0007F3, "store", "storeVectorLeftIndexedLRU"),
    ("stvrx128", 0x10000543, 0xFC0007F3, "store", "storeVectorRightIndexed"),
    ("stvrxl128", 0x10000743, 0xFC0007F3, "store", "storeVectorRightIndexedLRU"),
    ("stvx128", 0x100001C3, 0xFC0007F3, "store", "=store"),
    ("stvxl128", 0x100003C3, 0xFC0007F3, "store", "=store"),
    ("vsldoi128", 0x10000010, 0xFC000010, "sldoi", "=sldoi"),
    ("vaddfp128", 0x14000010, 0xFC0003D0, "abc", "vectorAddFloatingPoint"),
    ("vand128", 0x14000210, 0xFC0003D0, "abc", "=vA & vB"),
    ("vandc128", 0x14000250, 0xFC0003D0, "abc", "=vA & ~vB"),
    ("vmaddcfp128", 0x14000110, 0xFC0003D0, "madd", "vectorMultiplyAddFloatingPointToD"),
    ("vmaddfp128", 0x140000D0, 0xFC0003D0, "madd", "vectorMultiplyAddFloatingPoint"),
    ("vmsum3fp128", 0x14000190, 0xFC0003D0, "abc", "vectorMultiplySum3FloatingPoint"),
    ("vmsum4fp128", 0x140001D0, 0xFC0003D0, "abc", "vectorMultiplySum4FloatingPoint"),
    ("vmulfp128", 0x14000090, 0xFC0003D0, "abc", "vectorMultiplyFloatingPoint"),
    ("vnmsubfp128", 0x14000150, 0xFC0003D0, "madd", "vectorNegativeMultiplySubtractFloatingPoint"),
    ("vnor128", 0x14000290, 0xFC0003D0, "abc", "=~(vA | vB)"),
    ("vor128", 0x140002D0, 0xFC0003D0, "abc", "=vA | vB"),
    ("vperm128", 0x14000000, 0xFC000210, "perm", "vectorPermute"),
    ("vpkshss128", 0x14000200, 0xFC0003D0, "abc", "vectorPackSignedHalfWordSignedSaturate"),
    ("vpkshus128", 0x14000240, 0xFC0003D0, "abc", "vectorPackSignedHalfWordUnsignedSaturate"),
    ("vpkswss128", 0x14000280, 0xFC0003D0, "abc", "vectorPackSignedWordSignedSaturate"),
    ("vpkswus128", 0x140002C0, 0xFC0003D0, "abc", "vectorPackSignedWordUnsignedSaturate"),
    ("vpkuhum128", 0x14000300, 0xFC0003D0, "abc", "vectorPackUnsignedHalfWordUnsignedModulo"),
    ("vpkuhus128", 0x14000340, 0xFC0003D0, "abc", "vectorPackUnsignedHalfWordUnsignedSaturate"),
    ("vpkuwum128", 0x14000380, 0xFC0003D0, "abc", "vectorPackUnsignedWordUnsignedModulo"),
    ("vpkuwus128", 0x140003C0, 0xFC0003D0, "abc", "vectorPackUnsignedWordUnsignedSaturate"),
    ("vsel128", 0x14000350, 0xFC0003D0, "abc", "=(vA & ~vD) | (vB & vD)"),
    ("vslo128", 0x14000390, 0xFC0003D0, "abc", "vectorShiftLeftByOctet"),
    ("vsro128", 0x140003D0, 0xFC0003D0, "abc", "vectorShiftRightByOctet"),
    ("vsubfp128", 0x14000050, 0xFC0003D0, "abc", "vectorSubtractFloatingPoint"),
    ("vxor128", 0x14000310, 0xFC0003D0, "abc", "=vA ^ vB"),
    ("vcfpsxws128", 0x18000230, 0xFC0007F0, "bimm", "vectorConvertToSignedFixedPointWordSaturate"),
    ("vcfpuxws128", 0x18000270, 0xFC0007F0, "bimm", "vectorConvertToUnsignedFixedPointWordSaturate"),
    ("vcmpbfp128", 0x18000180, 0xFC000390, "cmp", "vectorCompareBoundsFloatingPoint"),
    ("vcmpeqfp128", 0x18000000, 0xFC000390, "cmp", "vectorCompareEqualToFloatingPoint"),
    ("vcmpequw128", 0x18000200, 0xFC000390, "cmp", "vectorCompareEqualToUnsignedWord"),
    ("vcmpgefp128", 0x18000080, 0xFC000390, "cmp", "vectorCompareGreaterThanOrEqualToFloatingPoint"),
    ("vcmpgtfp128", 0x18000100, 0xFC000390, "cmp", "vectorCompareGreaterThanFloatingPoint"),
    ("vcsxwfp128", 0x180002B0, 0xFC0007F0, "bimm", "vectorConvertFromSignedFixedPointWord"),
    ("vcuxwfp128", 0x180002F0, 0xFC0007F0, "bimm", "vectorConvertFromUnsignedFixedPointWord"),
    ("vexptefp128", 0x180006B0, 0xFC1F07F0, "b", "vector2RaisedToTheExponentEstimateFloatingPoint"),
    ("vlogefp128", 0x180006F0, 0xFC1F07F0, "b", "vectorLog2EstimateFloatingPoint"),
    ("vmaxfp128", 0x18000280, 0xFC0003D0, "abc", "vectorMaximumFloatingPoint"),
    ("vminfp128", 0x180002C0, 0xFC0003D0, "abc", "vectorMinimumFloatingPoint"),
    ("vmrghw128", 0x18000300, 0xFC0003D0, "abc", "vectorMergeHighWord"),
    ("vmrglw128", 0x18000340, 0xFC0003D0, "abc", "vectorMergeLowWord"),
    ("vpermwi128", 0x18000210, 0xFC000630, "permwi", "vectorPermuteWordImmediate"),
    ("vpkd3d128", 0x18000610, 0xFC000730, "d3d", "vectorPackD3D"),
    ("vrefp128", 0x18000630, 0xFC1F07F0, "b", "vectorReciprocalEstimateFloatingPoint"),
    ("vrfim128", 0x18000330, 0xFC1F07F0, "b", "vectorRoundToFloatingPointIntegerTowardMinusInfinity"),
    ("vrfin128", 0x18000370, 0xFC1F07F0, "b", "vectorRoundToFloatingPointIntegerNearest"),
    ("vrfip128", 0x180003B0, 0xFC1F07F0, "b", "vectorRoundToFloatingPointIntegerTowardPluInfinity"),
    ("vrfiz128", 0x180003F0, 0xFC1F07F0, "b", "vectorRoundToFloatingPointIntegerTowardZero"),
    ("vrlimi128", 0x18000710, 0xFC000730, "rlimi", "vectorRotateLeftImmediateAndMaskInsert"),
    ("vrlw128", 0x18000050, 0xFC0003D0, "abc", "vectorRotateLeftIntegerWord"),
    ("vrsqrtefp128", 0x18000670, 0xFC1F07F0, "b", "vectorReciprocalSquareRootEstimateFloatingPoint"),
    ("vslw128", 0x180000D0, 0xFC0003D0, "abc", "vectorShiftLeftIntegerWord"),
    ("vspltisw128", 0x18000770, 0xFC0007F0, "simm", "=splat"),
    ("vspltw128", 0x18000730, 0xFC0007F0, "bimm", "vectorSplatWord"),
    ("vsraw128", 0x18000150, 0xFC0003D0, "abc", "vectorShiftRightAlgebraicWord"),
    ("vsrw128", 0x180001D0, 0xFC0003D0, "abc", "vectorShiftRightWord"),
    ("vupkd3d128", 0x180007F0, 0xFC0007F0, "bimm", "vectorUnpackD3D"),
    ("vupkhsb128", 0x18000380, 0xFC1F07F0, "b", "vectorUnpackHighSignedByte"),
    ("vupkhsh128", 0x180007A0, 0xFC1F07F0, "b", "vectorUnpackHighSignedHalfWord"),
    ("vupklsb128", 0x180003C0, 0xFC1F07F0, "b", "vectorUnpackLowSignedByte"),
    ("vupklsh128", 0x180007E0, 0xFC1F07F0, "b", "vectorUnpackLowSignedHalfWord"),
]
# The Xenon's (and Cell's) unaligned vector loads and stores on the 32 AltiVec registers, in
# primary opcode 31 -- not VMX128, but missing from stock decoders alike: (mnemonic, extended
# opcode, form, the user operation the SLEIGH calls)
X31 = [("lvlx", 519, "load", "loadVectorLeftIndexed"), ("lvlxl", 775, "load", "loadVectorLeftIndexedLRU"),
       ("lvrx", 551, "load", "loadVectorRightIndexed"), ("lvrxl", 807, "load", "loadVectorRightIndexedLRU"),
       ("stvlx", 647, "store", "storeVectorLeftIndexed"), ("stvlxl", 903, "store", "storeVectorLeftIndexedLRU"),
       ("stvrx", 679, "store", "storeVectorRightIndexed"), ("stvrxl", 935, "store", "storeVectorRightIndexedLRU")]
_X31 = {xo: (m, form) for m, xo, form, _s in X31}
# most specific (widest mask) first, so a broad row never shadows a narrow one
_BY_SPECIFICITY = sorted(ROWS, key=lambda r: bin(r[2]).count("1"), reverse=True)


def decode(word):
    """(mnemonic, operand text) for a VMX128 instruction word (or one of X31's), else None."""
    w = word & 0xFFFFFFFF
    if w >> 26 == 31 and not w & 1 and (w >> 1) & 0x3FF in _X31:
        ra, rb = (w >> 16) & 31, (w >> 11) & 31
        return _X31[(w >> 1) & 0x3FF][0], "v%d,%s,r%d" % ((w >> 21) & 31, "r%d" % ra if ra else "0", rb)
    if (w >> 26) not in (4, 5, 6):
        return None
    for mnem, pat, mask, form, _sem in _BY_SPECIFICITY:
        if w & mask == pat:
            if form == "cmp" and w & 0x40:
                mnem += "."
            return mnem, _operands(form, w)
    return None


def _operands(form, w):
    d, a, b, c = vd128(w), va128(w), vb128(w), vc128(w)
    ra, rb = (w >> 16) & 31, (w >> 11) & 31
    if form in ("load", "store"):                    # (rA 0 is the literal zero, written 0)
        return "v%d,%s,r%d" % (d, "r%d" % ra if ra else "0", rb)
    if form == "sldoi":
        return "v%d,v%d,v%d,%d" % (d, a, b, (w >> 6) & 0xF)
    if form == "perm":
        return "v%d,v%d,v%d,v%d" % (d, a, b, c)
    if form == "madd":
        return "v%d,v%d,v%d,v%d" % (d, a, d, b)
    if form == "simm":
        s = (w >> 16) & 31
        return "v%d,%d" % (d, s - 32 if s >= 16 else s)
    if form == "bimm":
        return "v%d,v%d,%d" % (d, b, (w >> 16) & 31)
    if form == "permwi":
        return "v%d,v%d,%d" % (d, b, (((w >> 6) & 7) << 5) | ((w >> 16) & 31))
    if form == "d3d":
        return "v%d,v%d,%d,%d,%d" % (d, b, (w >> 18) & 7, (w >> 16) & 3, (w >> 6) & 3)
    if form == "rlimi":
        return "v%d,v%d,%d,%d" % (d, b, (w >> 16) & 31, (w >> 6) & 3)
    if form == "b":
        return "v%d,v%d" % (d, b)
    return "v%d,v%d,v%d" % (d, a, b)


def text(word):
    r = decode(word)
    return None if r is None else "%s %s" % r


def coverage(img):
    """{'vector words', 'vmx128', 'other'} over a PowerPC image's code section (an Image from
    formats.powerpc_image): how many primary-opcode 4/5/6 words it has and how many of them are
    VMX128 (the rest are AltiVec, or data)."""
    total = vmx = 0
    for va in range(img.code[0], img.code[1], 4):
        w = img.word(va)
        if w is not None and (w >> 26) in (4, 5, 6):
            total += 1
            vmx += decode(w) is not None
    return {"vector words": total, "vmx128": vmx, "other": total - vmx}


# ---- the SLEIGH -----------------------------------------------------------------------------------
# Registers: v0..v31 are AltiVec's (the top 32 VSX registers, vs32..vs63); v32..v127 are new, at
# offset 0x5000 (free in the PowerPC register space: VSX ends at 0x4400, the context at 0x6000).
# A register field's high bits pick one of four banks of 32; each bank is a subtable constructor.
_FIELDS = {"vDlo": (21, 25), "vDhi": (2, 3), "vAlo": (16, 20), "vAb5": (5, 5), "vAb6": (10, 10),
           "vBlo": (11, 15), "vBhi": (0, 1), "vC3": (6, 8), "x128OP": (26, 31), "x128SHB": (6, 9),
           "x128IMM": (16, 20), "x128SIMM": (16, 20), "x128PERMHI": (6, 8), "x128D3D": (18, 20),
           "x128PACK": (16, 17), "x128Z": (6, 7), "x128RC": (6, 6), "x128RA": (16, 20), "x128RB": (11, 15)}


def _bank(base):
    return " ".join("vs%d" % (32 + i) for i in range(32)) if base == 0 else \
        " ".join("v%d" % (base + i) for i in range(32))


def _runs(bits):
    """Contiguous runs (lo, hi) of the set bits."""
    out, lo = [], None
    for i in range(33):
        on = i < 32 and bits >> i & 1
        if on and lo is None:
            lo = i
        elif not on and lo is not None:
            out.append((lo, i - 1))
            lo = None
    return out


def sleigh(declared=()):
    """The text of languages/src/ppc/vmx128.sinc; `declared`: the user operations the other
    PowerPC .sinc files already define (a second definition is an error)."""
    fixed = set()
    for _m, _p, mask, _f, _s in ROWS:
        fixed.update(_runs(mask & 0x03FFFFFF))
    fields = dict(_FIELDS)
    for lo, hi in sorted(fixed):
        fields["x128_%d_%d" % (lo, hi)] = (lo, hi)
    pieces = [(reg, k) for reg in ("vD128", "vA128", "vB128") for k in range(4)]
    for reg, k in pieces:                       # each bank: a register field and a name field
        fields["%sr%d" % (reg, k)] = fields["%slo" % reg[:2]]
        fields["%sn%d" % (reg, k)] = fields["%slo" % reg[:2]]
    fields["vC128r"] = fields["vC128n"] = fields["vC3"]
    L = ["# VMX128: the Xbox 360 (Xenon) 128-register AltiVec extension. GENERATED by",
         "# `python -m codebreaker.decompile.vmx128 --sleigh` from its table -- edit that, not this.", "",
         "define register offset=0x5000 size=16 [ %s ];" % " ".join("v%d" % i for i in range(32, 128)), "",
         "define token vmx128(32)"]
    for name, (lo, hi) in sorted(fields.items(), key=lambda kv: (kv[1], kv[0])):
        signed = " signed" if name == "x128SIMM" else ""
        L.append("\t%s=(%d,%d)%s" % (name, lo, hi, signed))
    L.append(";")
    L.append("")
    for reg, k in pieces:
        L.append("attach variables [ %sr%d ] [ %s ];" % (reg, k, _bank(32 * k)))
        L.append("attach names [ %sn%d ] [ %s ];" % (reg, k, " ".join("v%d" % (32 * k + i) for i in range(32))))
    L.append("attach variables [ x128RA x128RB ] [ %s ];" % " ".join("r%d" % i for i in range(32)))
    L.append("attach variables [ vC128r ] [ %s ];" % " ".join("vs%d" % (32 + i) for i in range(8)))
    L.append("attach names [ vC128n ] [ %s ];" % " ".join("v%d" % i for i in range(8)))
    L.append("")
    sel = {"vD128": lambda k: "vDhi=%d" % k,
           "vA128": lambda k: "vAb6=%d & vAb5=%d" % (k >> 1, k & 1),
           "vB128": lambda k: "vBhi=%d" % k}
    for reg, k in pieces:
        L.append("%s: %sn%d is %s & %sn%d & %sr%d { export %sr%d; }" % (
            reg, reg, k, sel[reg](k), reg, k, reg, k, reg, k))
        if reg == "vD128":                      # (the multiply-adds show vD again, as a source)
            L.append("vDs128: %sn%d is %s & %sn%d & %sr%d { export %sr%d; }" % (
                reg, k, sel[reg](k), reg, k, reg, k, reg, k))
    L.append("vC128: vC128n is vC128n & vC128r { export vC128r; }")
    L.append("x128PERM: val is x128PERMHI & x128IMM [ val = (x128PERMHI << 5) | x128IMM; ] "
             "{ export *[const]:1 val; }")              # (vpermwi128's 8-bit control, in two pieces)
    L.append("x128RA0: x128RA is x128RA { export x128RA; }")      # (rA, or 0 when it names r0)
    L.append("x128RA0: 0 is x128RA=0 { export 0:$(REGISTER_SIZE); }")
    L.append("")
    ops = sorted({s for *_r, s in ROWS if not s.startswith("=")} | {s for *_r, s in X31} | {"vectorCompareSetCR6"})
    L += ["define pcodeop %s;" % op for op in ops if op not in declared]
    L.append("")
    for mnem, xo, form, sem in X31:                  # (AltiVec's own fields and register names)
        reg = "vrD" if form == "load" else "vrS"
        L.append(":%s %s,RA_OR_ZERO,B is OP=31 & %s & RA_OR_ZERO & B & XOP_1_10=%d & Rc=0" % (mnem, reg, reg, xo))
        L.append("{")
        L.append("\tbuild RA_OR_ZERO;")
        L.append("\tea:$(REGISTER_SIZE) = RA_OR_ZERO + B;")
        L.append("\t%s;" % ("%s = %s(ea)" % (reg, sem) if form == "load" else "%s(ea, %s)" % (sem, reg)))
        L.append("}")
        L.append("")
    for mnem, pat, mask, form, sem in ROWS:
        cons = ["x128OP=%d" % (pat >> 26)]
        for lo, hi in _runs(mask & 0x03FFFFFF):
            cons.append("x128_%d_%d=%d" % (lo, hi, (pat >> lo) & ((1 << (hi - lo + 1)) - 1)))
        for rc in ((0, 1) if form == "cmp" else (None,)):
            c = list(cons) + (["x128RC=%d" % rc] if rc is not None else [])
            name = mnem + ("." if rc else "")
            ops_text, operands, body = _constructor(form, sem)
            L.append(":%s %s is %s & %s" % (name, ops_text, " & ".join(c), " & ".join(operands)))
            if rc:
                body = body + ["\tcr6 = vectorCompareSetCR6(vD128);"]
            L.append("{")
            L += body
            L.append("}")
            L.append("")
    return "\n".join(L)


def _constructor(form, sem):
    """(display operands, pattern operands, p-code lines) for one row."""
    ea = ["\tea:$(REGISTER_SIZE) = x128RA0 + x128RB;"]
    if form == "load":
        if sem == "=load":
            return "vD128,x128RA0,x128RB", ["vD128", "x128RA0", "x128RB"], \
                ea + ["\tvD128 = *[ram]:16 (ea & 0xfffffffffffffff0);"]
        return "vD128,x128RA0,x128RB", ["vD128", "x128RA0", "x128RB"], ea + ["\tvD128 = %s(ea);" % sem]
    if form == "store":
        if sem == "=store":
            return "vD128,x128RA0,x128RB", ["vD128", "x128RA0", "x128RB"], \
                ea + ["\t*[ram]:16 (ea & 0xfffffffffffffff0) = vD128;"]
        return "vD128,x128RA0,x128RB", ["vD128", "x128RA0", "x128RB"], ea + ["\t%s(ea, vD128);" % sem]
    if form == "sldoi":
        return "vD128,vA128,vB128,x128SHB", ["vD128", "vA128", "vB128", "x128SHB"], [
            "\ttmp:32 = (zext(vA128) << 128) | zext(vB128);",
            "\ttmp = tmp << (x128SHB * 8);",
            "\tvD128 = tmp[128,128];"]
    if form == "simm":
        return "vD128,x128SIMM", ["vD128", "x128SIMM"], [
            "\tw4:4 = sext(x128SIMM:1);",
            "\tw:16 = zext(w4);",
            "\tvD128 = w | (w << 32) | (w << 64) | (w << 96);"]
    if form == "perm":
        return "vD128,vA128,vB128,vC128", ["vD128", "vA128", "vB128", "vC128"], \
            ["\tvD128 = %s(vA128, vB128, vC128);" % sem]
    if form == "madd":
        return "vD128,vA128,vDs128,vB128", ["vD128", "vA128", "vDs128", "vB128"], \
            ["\tvD128 = %s(vA128, vDs128, vB128);" % sem]
    if form in ("bimm", "permwi", "rlimi", "d3d"):
        imm = {"bimm": "x128IMM", "rlimi": "x128IMM", "permwi": "x128PERM", "d3d": "x128D3D"}[form]
        extra = {"rlimi": ",x128Z", "d3d": ",x128PACK,x128Z"}.get(form, "")
        operands = ["vD128", "vB128", imm] + [e for e in extra.split(",") if e]
        args = ", ".join(["vB128"] + ["%s:1" % o for o in operands[2:]])
        src = "vD128, " if form == "rlimi" else ""
        return "vD128,vB128,%s%s" % (imm, extra), operands, ["\tvD128 = %s(%s%s);" % (sem, src, args)]
    if form == "b":
        return "vD128,vB128", ["vD128", "vB128"], ["\tvD128 = %s(vB128);" % sem]
    # three registers ("abc", "cmp")
    if sem.startswith("="):
        return "vD128,vA128,vB128", ["vD128", "vA128", "vB128"], ["\tvD128 = %s;" % re.sub(r"\bv([DAB])\b", r"v\g<1>128", sem[1:])]
    return "vD128,vA128,vB128", ["vD128", "vA128", "vB128"], ["\tvD128 = %s(vA128, vB128);" % sem]


def main(argv):
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__.split("\n\n")[0])
        return 0
    if argv[0] == "--sleigh":
        if len(argv) < 2:
            print("usage: --sleigh <out.sinc>")
            return 2
        import glob
        import os
        declared = set()
        for sinc in glob.glob(os.path.join(os.path.dirname(os.path.abspath(argv[1])), "*.sinc")):
            if os.path.basename(sinc) != "vmx128.sinc":
                declared.update(re.findall(r"^define pcodeop (\w+);", open(sinc).read(), re.M))
        with open(argv[1], "w", newline="\n") as f:
            f.write(sleigh(declared) + "\n")
        print("wrote %s (%d rows)" % (argv[1], len(ROWS)))
        return 0
    if argv[0] == "--coverage":
        from codebreaker.formats import powerpc_image, xex
        path = xex.basefile(argv[1]) if argv[1].lower().endswith(".xex") else argv[1]
        for k, v in coverage(powerpc_image.read(path)).items():
            print("  %-13s %d" % (k, v))
        return 0
    for tok in argv:
        w = int(tok, 16)
        print("0x%08X  %s" % (w, text(w) or "(not VMX128)"))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
