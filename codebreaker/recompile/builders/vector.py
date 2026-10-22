"""vector.py -- AltiVec and VMX128 vector instructions, as C++ for the recompiler (recompile/powerpc.py).

Each `emit(mnemonic, operands, ...)` returns the C++ statements for one instruction, or None
when the mnemonic is not this file's. Registers are always written as the context's fields
(ctx.rN, ctx.fN, ctx.vN, ctx.crN, ctx.xer, ctx.ctr, ctx.lr); `ea` and `temp` are locals every
recompiled function declares.
Vector registers are held element-reversed (for little-endian SIMD), and the index arithmetic
that compensates is the code generator's own: vspltb perm = 15 - UIMM; vsplth perm = 7 - UIMM,
then (perm*2) | ((perm*2+1) << 8); vspltw perm = 3 - UIMM, repeated in each 2-bit lane;
vpermwi128 x=3-(imm&3), y=3-((imm>>2)&3), z=3-((imm>>4)&3), w=3-((imm>>6)&3); vsldoi's alignr
count is 16 - SH; the pack and unpack forms use reversed element indices. Operations that run
in VMX mode first emit `ctx.fpscr.enableFlushMode();`. The vector loads and stores, and
mtvscr/mfvscr, are memory.py's. The SIMD helpers the statements call (simde_mm_* ...) are in the
runtime header (powerpc.runtime_header).

Derived from the rexglue SDK's code generator (rexcodegen/builders/vector.cpp; BSD-3-Clause,
Copyright (c) 2026 Tom Clay, with parts from Xenia -- see NOTICE).
"""
import struct

# Runtime helpers referenced by the emitted code (rex::simde_mm_*) are NOT defined
# in vector.cpp; they are provided by the recompiler runtime (ppc.h / simde ext).
# We reproduce the emitted *calls* exactly and forward-declare the helpers here so
# the generated translation unit resolves them.  See notes at bottom.
RUNTIME_HELPERS = r'''
// ---------------------------------------------------------------------------
// VMX runtime helpers the vector statements call. They are the runtime's (the rex namespace
// of native/rexglue/kernel: ppc.h and its SIMD extension header), declared here, not defined.
//
//   namespace rex {
//     simde__m128i simde_mm_adds_epu32(simde__m128i a, simde__m128i b);   // saturating u32 add
//     simde__m128i simde_mm_avg_epi8 (simde__m128i a, simde__m128i b);    // signed byte average
//     simde__m128i simde_mm_avg_epi16(simde__m128i a, simde__m128i b);    // signed hword average
//     simde__m128i simde_mm_avg_epi32(simde__m128i a, simde__m128i b);    // signed word average
//     simde__m128i simde_mm_cmpgt_epu8 (simde__m128i a, simde__m128i b);  // unsigned byte  >
//     simde__m128i simde_mm_cmpgt_epu16(simde__m128i a, simde__m128i b);  // unsigned hword >
//     simde__m128i simde_mm_vctsxs(simde__m128 v);      // float->signed word saturate
//     simde__m128i simde_mm_vctuxs(simde__m128 v);      // float->unsigned word saturate
//     simde__m128  simde_mm_cvtepu32_ps_(simde__m128i v); // unsigned word -> float
//     simde__m128i simde_mm_perm_epi8_(simde__m128i a, simde__m128i b, simde__m128i c); // vperm
//     simde__m128i simde_mm_vsl (simde__m128i a, simde__m128i b);  // 128-bit shift-left  (bits)
//     simde__m128i simde_mm_vsr (simde__m128i a, simde__m128i b);  // 128-bit shift-right (bits)
//     simde__m128i simde_mm_vslo(simde__m128i a, simde__m128i b);  // 128-bit shift-left  (octets)
//     simde__m128i simde_mm_vsro(simde__m128i a, simde__m128i b);  // 128-bit shift-right (octets)
//   }
//
// Constants referenced by vpkd3d128 (NORMPACKED32) come from ppc/context.h:
//   kPack2101010_Min10 / kPack2101010_Max10 / kPack2101010_Min2 / kPack2101010_Max2
// ---------------------------------------------------------------------------
'''

# ---------------------------------------------------------------------------
# Mnemonics handled (base forms + Xbox360 VMX128 aliases). Record-form ('.')
# variants of the vcmp* instructions are handled dynamically (trailing '.').
# ---------------------------------------------------------------------------
MNEMONICS = [
    # FP arithmetic
    "vaddfp", "vaddfp128", "vsubfp", "vsubfp128", "vmulfp128",
    "vmaddfp", "vmaddfp128", "vmaddcfp128", "vnmsubfp", "vnmsubfp128",
    "vmaxfp", "vmaxfp128", "vminfp", "vminfp128",
    "vrefp", "vrefp128", "vrsqrtefp", "vrsqrtefp128",
    "vexptefp", "vexptefp128", "vlogefp", "vlogefp128",
    # dot products
    "vmsum3fp128", "vmsum4fp128",
    # rounding
    "vrfim", "vrfim128", "vrfin", "vrfin128",
    "vrfip", "vrfip128", "vrfiz", "vrfiz128",
    # integer arithmetic / saturating
    "vaddsbs", "vaddshs", "vaddsws", "vaddubm", "vaddubs", "vadduhm",
    "vadduwm", "vadduws", "vadduhs",
    "vsubsbs", "vsubsws", "vsububm", "vsububs", "vsubuhs", "vsubuws",
    "vsubuhm", "vsubuwm", "vsubshs",
    "vmaxsw", "vmaxsh", "vmaxsb", "vminsh", "vminsb", "vminsw",
    "vmaxuh", "vminuh", "vmaxub", "vminub",
    # average
    "vavgsb", "vavgsh", "vavgsw", "vavgub", "vavguh",
    # logical
    "vand", "vand128", "vandc", "vandc128", "vor", "vor128",
    "vxor", "vxor128", "vnor", "vnor128", "vsel", "vsel128",
    # compare
    "vcmpbfp", "vcmpbfp128", "vcmpeqfp", "vcmpeqfp128",
    "vcmpequb", "vcmpequh", "vcmpequw", "vcmpequw128",
    "vcmpgefp", "vcmpgefp128", "vcmpgtfp", "vcmpgtfp128",
    "vcmpgtub", "vcmpgtuh", "vcmpgtuw",
    "vcmpgtsb", "vcmpgtsh", "vcmpgtsw",
    # conversion
    "vctsxs", "vcfpsxws128", "vctuxs", "vcfpuxws128",
    "vcfsx", "vcsxwfp128", "vcfux", "vcuxwfp128",
    # merge
    "vmrghb", "vmrghh", "vmrghw", "vmrghw128",
    "vmrglb", "vmrglh", "vmrglw", "vmrglw128",
    # permute
    "vperm", "vperm128", "vpermwi128", "vrlimi128",
    # shift / rotate
    "vsl", "vslb", "vslh", "vsldoi", "vsldoi128", "vslw", "vslw128",
    "vslo", "vslo128", "vsr", "vsrh", "vsrb", "vsrab", "vsrah",
    "vsraw", "vsraw128", "vsrw", "vsrw128", "vsro", "vsro128",
    "vrlh", "vrlw", "vrlw128",
    # splat
    "vspltb", "vsplth", "vspltisb", "vspltish", "vspltisw", "vspltisw128",
    "vspltw", "vspltw128",
    # pack
    "vpkshus", "vpkshus128", "vpkuhum", "vpkuhum128", "vpkuhus", "vpkuhus128",
    "vpkuwum", "vpkuwum128", "vpkuwus", "vpkuwus128",
    "vpkshss", "vpkshss128", "vpkswss", "vpkswss128", "vpkswus", "vpkswus128",
    "vpkd3d128",
    # unpack
    "vupkd3d128",
    "vupkhsb", "vupkhsb128", "vupkhsh", "vupkhsh128",
    "vupklsb", "vupklsb128", "vupklsh", "vupklsh128",
]

# Elsewhere: the vector loads and stores (lvx lvxl lvlx lvrx lvebx lvehx lvewx lvsl lvsr, stvx
# stvxl stvlx stvrx stvebx stvehx stvewx) and mtvscr/mfvscr (memory.py).


def emit(mn, ops, _reg, _vreg, _freg, _imm, _cr, _mem, _rv, _base_expr, _addr, va, WORD):
    # a record form is a mnemonic ending in '.'
    rec = mn.endswith(".")
    core = mn[:-1] if rec else mn

    # ---- inlined emission templates (from context.cpp) --------------------
    def vfp2(op, d, a, b):
        # emit_vec_fp_binary
        return ("simde_mm_store_ps(ctx.v%d.f32, simde_mm_%s_ps("
                "simde_mm_load_ps(ctx.v%d.f32), simde_mm_load_ps(ctx.v%d.f32)));"
                % (d, op, a, b))

    def vint2(op, ty, d, a, b):
        # emit_vec_int_binary (a,b already in emitted order; swapped callers pass b,a)
        return ("simde_mm_store_si128((simde__m128i*)ctx.v%d.%s, simde_mm_%s("
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.%s), "
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.%s)));"
                % (d, ty, op, a, ty, b, ty))

    FLUSH = "ctx.fpscr.enableFlushMode();"

    def _r(k):
        return _vreg(ops[k])

    # =======================================================================
    # Vector Floating Point Arithmetic
    # =======================================================================
    if core in ("vaddfp", "vaddfp128"):
        return [FLUSH, vfp2("add", _r(0), _r(1), _r(2))]
    if core in ("vsubfp", "vsubfp128"):
        return [FLUSH, vfp2("sub", _r(0), _r(1), _r(2))]
    if core == "vmulfp128":
        return [FLUSH, vfp2("mul", _r(0), _r(1), _r(2))]
    if core in ("vmaddfp", "vmaddfp128", "vmaddcfp128"):
        d, a, c, b = _r(0), _r(1), _r(2), _r(3)
        return [FLUSH,
                ("simde_mm_store_ps(ctx.v%d.f32, simde_mm_add_ps(simde_mm_mul_ps("
                 "simde_mm_load_ps(ctx.v%d.f32), simde_mm_load_ps(ctx.v%d.f32)), "
                 "simde_mm_load_ps(ctx.v%d.f32)));" % (d, a, c, b))]
    if core in ("vnmsubfp", "vnmsubfp128"):
        d, a, c, b = _r(0), _r(1), _r(2), _r(3)
        return [FLUSH,
                ("simde_mm_store_ps(ctx.v%d.f32, simde_mm_xor_ps(simde_mm_sub_ps("
                 "simde_mm_mul_ps(simde_mm_load_ps(ctx.v%d.f32), simde_mm_load_ps(ctx.v%d.f32)), "
                 "simde_mm_load_ps(ctx.v%d.f32)), "
                 "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));"
                 % (d, a, c, b))]
    if core in ("vmaxfp", "vmaxfp128"):
        return [FLUSH, vfp2("max", _r(0), _r(1), _r(2))]
    if core in ("vminfp", "vminfp128"):
        return [FLUSH, vfp2("min", _r(0), _r(1), _r(2))]
    if core in ("vrefp", "vrefp128"):
        d, a = _r(0), _r(1)
        return [FLUSH,
                ("simde_mm_store_ps(ctx.v%d.f32, simde_mm_div_ps(simde_mm_set1_ps(1), "
                 "simde_mm_load_ps(ctx.v%d.f32)));" % (d, a))]
    if core in ("vrsqrtefp", "vrsqrtefp128"):
        d, a = _r(0), _r(1)
        return [FLUSH,
                ("simde_mm_store_ps(ctx.v%d.f32, simde_mm_div_ps(simde_mm_set1_ps(1), "
                 "simde_mm_sqrt_ps(simde_mm_load_ps(ctx.v%d.f32))));" % (d, a))]
    if core in ("vexptefp", "vexptefp128"):
        d, a = _r(0), _r(1)
        return [FLUSH] + ["ctx.v%d.f32[%d] = exp2f(ctx.v%d.f32[%d]);" % (d, i, a, i)
                          for i in range(4)]
    if core in ("vlogefp", "vlogefp128"):
        d, a = _r(0), _r(1)
        return [FLUSH] + ["ctx.v%d.f32[%d] = log2f(ctx.v%d.f32[%d]);" % (d, i, a, i)
                          for i in range(4)]

    # =======================================================================
    # Vector Dot Products (VMX128)
    # =======================================================================
    if core == "vmsum3fp128":
        d, a, b = _r(0), _r(1), _r(2)
        return [FLUSH,
                ("simde_mm_store_ps(ctx.v%d.f32, simde_mm_dp_ps("
                 "simde_mm_load_ps(ctx.v%d.f32), simde_mm_load_ps(ctx.v%d.f32), 0xEF));"
                 % (d, a, b))]
    if core == "vmsum4fp128":
        d, a, b = _r(0), _r(1), _r(2)
        return [FLUSH,
                ("simde_mm_store_ps(ctx.v%d.f32, simde_mm_dp_ps("
                 "simde_mm_load_ps(ctx.v%d.f32), simde_mm_load_ps(ctx.v%d.f32), 0xFF));"
                 % (d, a, b))]

    # =======================================================================
    # Vector Rounding
    # =======================================================================
    _round = {
        "vrfim": "SIMDE_MM_FROUND_TO_NEG_INF | SIMDE_MM_FROUND_NO_EXC",
        "vrfin": "SIMDE_MM_FROUND_TO_NEAREST_INT | SIMDE_MM_FROUND_NO_EXC",
        "vrfip": "SIMDE_MM_FROUND_TO_POS_INF | SIMDE_MM_FROUND_NO_EXC",
        "vrfiz": "SIMDE_MM_FROUND_TO_ZERO | SIMDE_MM_FROUND_NO_EXC",
    }
    _round_core = core[:-3] if core.endswith("128") and core[:-3] in _round else core
    if _round_core in _round:
        d, a = _r(0), _r(1)
        return [FLUSH,
                ("simde_mm_store_ps(ctx.v%d.f32, simde_mm_round_ps("
                 "simde_mm_load_ps(ctx.v%d.f32), %s));" % (d, a, _round[_round_core]))]

    # =======================================================================
    # Vector Integer Arithmetic
    # =======================================================================
    _intbin = {
        "vaddsbs": ("adds_epi8", "s8"),  "vaddshs": ("adds_epi16", "s16"),
        "vaddubm": ("add_epi8", "u8"),   "vaddubs": ("adds_epu8", "u8"),
        "vadduhm": ("add_epi16", "u16"), "vadduwm": ("add_epi32", "u32"),
        "vadduhs": ("adds_epu16", "u16"),
        "vsubsbs": ("subs_epi8", "s8"),  "vsububm": ("sub_epi8", "u8"),
        "vsububs": ("subs_epu8", "u8"),  "vsubuhs": ("subs_epu16", "u16"),
        "vsubuhm": ("sub_epi16", "u16"), "vsubuwm": ("sub_epi32", "u32"),
        "vsubshs": ("subs_epi16", "s16"),
        "vmaxsw": ("max_epi32", "s32"),  "vmaxsh": ("max_epi16", "s16"),
        "vmaxsb": ("max_epi8", "s8"),    "vminsh": ("min_epi16", "s16"),
        "vminsb": ("min_epi8", "s8"),    "vminsw": ("min_epi32", "s32"),
        "vmaxuh": ("max_epu16", "u16"),  "vminuh": ("min_epu16", "u16"),
        "vmaxub": ("max_epu8", "u8"),    "vminub": ("min_epu8", "u8"),
        "vavgub": ("avg_epu8", "u8"),    "vavguh": ("avg_epu16", "u16"),
    }
    if core in _intbin:
        op, ty = _intbin[core]
        return [vint2(op, ty, _r(0), _r(1), _r(2))]

    if core == "vaddsws":
        d, a, b = _r(0), _r(1), _r(2)
        return [
            "{",
            "simde__m128i a = simde_mm_load_si128((simde__m128i*)ctx.v%d.u8);" % a,
            "simde__m128i b = simde_mm_load_si128((simde__m128i*)ctx.v%d.u8);" % b,
            "simde__m128i sum = simde_mm_add_epi32(a, b);",
            ("simde__m128i overflow = simde_mm_and_si128(simde_mm_xor_si128(a, sum), "
             "simde_mm_xor_si128(b, sum));"),
            ("simde__m128i sat_val = simde_mm_xor_si128(simde_mm_srai_epi32(a, 31), "
             "simde_mm_set1_epi32(0x7FFFFFFF));"),
            ("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_blendv_epi8("
             "sum, sat_val, overflow));" % d),
            "}",
        ]

    if core == "vadduws":
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u32, rex::simde_mm_adds_epu32("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)));" % (d, a, b))]

    if core == "vsubsws":
        d, a, b = _r(0), _r(1), _r(2)
        out = []
        for i in range(4):
            out.append("temp.s64 = int64_t(ctx.v%d.s32[%d]) - int64_t(ctx.v%d.s32[%d]);"
                       % (a, i, b, i))
            out.append("ctx.v%d.s32[%d] = temp.s64 > INT_MAX ? INT_MAX : "
                       "temp.s64 < INT_MIN ? INT_MIN : temp.s64;" % (d, i))
        return out

    if core == "vsubuws":
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u32, simde_mm_sub_epi32("
                 "simde_mm_load_si128((simde__m128i*) ctx.v%d.u32), "
                 "simde_mm_min_epu32(simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32))));" % (d, a, a, b))]

    # =======================================================================
    # Vector Average (rex helpers for the signed forms)
    # =======================================================================
    if core == "vavgsb":
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, rex::simde_mm_avg_epi8("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)));" % (d, a, b))]
    if core == "vavgsh":
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u16, rex::simde_mm_avg_epi16("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16)));" % (d, a, b))]
    if core == "vavgsw":
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.s32, rex::simde_mm_avg_epi32("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.s32), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.s32)));" % (d, a, b))]

    # =======================================================================
    # Vector Logical
    # =======================================================================
    if core in ("vand", "vand128"):
        return [vint2("and_si128", "u8", _r(0), _r(1), _r(2))]
    if core in ("vandc", "vandc128"):
        # vD = vA & ~vB : emit_vec_int_binary_swapped(andnot_si128, u8) -> op(vB, vA)
        return [vint2("andnot_si128", "u8", _r(0), _r(2), _r(1))]
    if core in ("vor", "vor128"):
        d, a, b = _r(0), _r(1), _r(2)
        if a != b:
            body = ("simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                    "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)));" % (a, b))
        else:
            body = "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8));" % a
        return ["simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, %s" % (d, body)]
    if core in ("vxor", "vxor128"):
        d, a, b = _r(0), _r(1), _r(2)
        if a != b:
            body = ("simde_mm_xor_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                    "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)));" % (a, b))
        else:
            body = "simde_mm_setzero_si128());"
        return ["simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, %s" % (d, body)]
    if core in ("vnor", "vnor128"):
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_xor_si128("
                 "simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)), "
                 "simde_mm_set1_epi32(-1)));" % (d, a, b))]
    if core in ("vsel", "vsel128"):
        d = _r(0)
        a, b, c = _r(1), _r(2), _r(3)  # operands: vD, vA, vB, vC(mask)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_or_si128("
                 "simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)), "
                 "simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8))));"
                 % (d, c, a, c, b))]

    # =======================================================================
    # Vector Compare (record forms update CR6 via setFromMask)
    # =======================================================================
    if core in ("vcmpbfp", "vcmpbfp128"):
        d, a, b = _r(0), _r(1), _r(2)
        out = [FLUSH]
        out.append(
            "simde_mm_store_ps(vTemp.f32, simde_mm_and_ps(simde_mm_cmpgt_ps("
            "simde_mm_load_ps(ctx.v%d.f32), simde_mm_load_ps(ctx.v%d.f32)), "
            "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));" % (a, b))
        out.append(
            "simde_mm_store_ps(ctx.v%d.f32, simde_mm_and_ps(simde_mm_cmplt_ps("
            "simde_mm_load_ps(ctx.v%d.f32), simde_mm_xor_ps(simde_mm_load_ps(ctx.v%d.f32), "
            "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000))))), "
            "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x40000000)))));" % (d, a, b))
        out.append(
            "simde_mm_store_ps(ctx.v%d.f32, simde_mm_or_ps(simde_mm_load_ps(vTemp.f32), "
            "simde_mm_load_ps(ctx.v%d.f32)));" % (d, d))
        if rec:
            out.append(
                "ctx.cr6.setFromMask(simde_mm_castsi128_ps(simde_mm_or_si128("
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.f32), "
                "simde_mm_slli_epi32(simde_mm_load_si128((simde__m128i*)ctx.v%d.f32), 1))), 0xF);"
                % (d, d))
        return out

    if core in ("vcmpeqfp", "vcmpeqfp128"):
        d = _r(0)
        out = [FLUSH, vfp2("cmpeq", d, _r(1), _r(2))]
        if rec:
            out.append("ctx.cr6.setFromMask(simde_mm_load_ps(ctx.v%d.f32), 0xF);" % d)
        return out
    if core in ("vcmpgefp", "vcmpgefp128"):
        d = _r(0)
        out = [FLUSH, vfp2("cmpge", d, _r(1), _r(2))]
        if rec:
            out.append("ctx.cr6.setFromMask(simde_mm_load_ps(ctx.v%d.f32), 0xF);" % d)
        return out
    if core in ("vcmpgtfp", "vcmpgtfp128"):
        d = _r(0)
        out = [FLUSH, vfp2("cmpgt", d, _r(1), _r(2))]
        if rec:
            out.append("ctx.cr6.setFromMask(simde_mm_load_ps(ctx.v%d.f32), 0xF);" % d)
        return out

    if core == "vcmpequb":
        d = _r(0)
        out = [vint2("cmpeq_epi8", "u8", d, _r(1), _r(2))]
        if rec:
            out.append("ctx.cr6.setFromMask(simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), 0xFFFF);" % d)
        return out
    if core == "vcmpequh":
        d, a, b = _r(0), _r(1), _r(2)
        out = [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_cmpeq_epi16("
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), "
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16)));" % (d, a, b))]
        if rec:
            out.append("ctx.cr6.setFromMask(simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), 0xFFFF);" % d)
        return out
    if core in ("vcmpequw", "vcmpequw128"):
        d, a, b = _r(0), _r(1), _r(2)
        out = [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_cmpeq_epi32("
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), "
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)));" % (d, a, b))]
        if rec:
            out.append("ctx.cr6.setFromMask(simde_mm_load_ps(ctx.v%d.f32), 0xF);" % d)
        return out
    if core == "vcmpgtub":
        d, a, b = _r(0), _r(1), _r(2)
        out = [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, rex::simde_mm_cmpgt_epu8("
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)));" % (d, a, b))]
        if rec:
            out.append("ctx.cr6.setFromMask(simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), 0xFFFF);" % d)
        return out
    if core == "vcmpgtuh":
        d, a, b = _r(0), _r(1), _r(2)
        out = [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, rex::simde_mm_cmpgt_epu16("
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), "
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16)));" % (d, a, b))]
        if rec:
            out.append("ctx.cr6.setFromMask(simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), 0xFFFF);" % d)
        return out
    if core == "vcmpgtuw":
        d, a, b = _r(0), _r(1), _r(2)
        out = [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u32, simde_mm_cmpgt_epi32("
                "simde_mm_xor_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), "
                "simde_mm_set1_epi32((int32_t)0x80000000)), "
                "simde_mm_xor_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), "
                "simde_mm_set1_epi32((int32_t)0x80000000))));" % (d, a, b))]
        if rec:
            out.append("ctx.cr6.setFromMask(simde_mm_castsi128_ps("
                       "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)), 0xF);" % d)
        return out
    if core == "vcmpgtsb":
        d = _r(0)
        out = [vint2("cmpgt_epi8", "u8", d, _r(1), _r(2))]
        if rec:
            out.append("ctx.cr6.setFromMask(simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), 0xFFFF);" % d)
        return out
    if core == "vcmpgtsh":
        d, a, b = _r(0), _r(1), _r(2)
        out = [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_cmpgt_epi16("
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), "
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16)));" % (d, a, b))]
        if rec:
            out.append("ctx.cr6.setFromMask(simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), 0xFFFF);" % d)
        return out
    if core == "vcmpgtsw":
        d = _r(0)
        out = [vint2("cmpgt_epi32", "u32", d, _r(1), _r(2))]
        if rec:
            out.append("ctx.cr6.setFromMask(simde_mm_castsi128_ps("
                       "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)), 0xF);" % d)
        return out

    # =======================================================================
    # Vector Conversion
    # =======================================================================
    if core in ("vctsxs", "vcfpsxws128"):
        d, a = _r(0), _r(1)
        u = _imm(ops[2])
        if u != 0:
            inner = ("simde_mm_mul_ps(simde_mm_load_ps(ctx.v%d.f32), simde_mm_set1_ps(%d))"
                     % (a, (1 << u)))
        else:
            inner = "simde_mm_load_ps(ctx.v%d.f32)" % a
        return [FLUSH,
                "simde_mm_store_si128((simde__m128i*)ctx.v%d.s32, rex::simde_mm_vctsxs(%s));"
                % (d, inner)]
    if core in ("vctuxs", "vcfpuxws128"):
        d, a = _r(0), _r(1)
        u = _imm(ops[2])
        if u != 0:
            inner = ("simde_mm_mul_ps(simde_mm_load_ps(ctx.v%d.f32), simde_mm_set1_ps(%d))"
                     % (a, (1 << u)))
        else:
            inner = "simde_mm_load_ps(ctx.v%d.f32)" % a
        return [FLUSH,
                "simde_mm_store_si128((simde__m128i*)ctx.v%d.u32, rex::simde_mm_vctuxs(%s));"
                % (d, inner)]
    if core in ("vcfsx", "vcsxwfp128"):
        d, a = _r(0), _r(1)
        u = _imm(ops[2])
        out = [FLUSH]
        if u != 0:
            bits = struct.unpack("<I", struct.pack("<f", 2.0 ** (-u)))[0]
            out.append(
                "simde_mm_store_ps(ctx.v%d.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps("
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)), "
                "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x%X)))));" % (d, a, bits))
        else:
            out.append(
                "simde_mm_store_ps(ctx.v%d.f32, simde_mm_cvtepi32_ps("
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)));" % (d, a))
        return out
    if core in ("vcfux", "vcuxwfp128"):
        d, a = _r(0), _r(1)
        u = _imm(ops[2])
        out = [FLUSH]
        if u != 0:
            bits = struct.unpack("<I", struct.pack("<f", 2.0 ** (-u)))[0]
            out.append(
                "simde_mm_store_ps(ctx.v%d.f32, simde_mm_mul_ps(rex::simde_mm_cvtepu32_ps_("
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)), "
                "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x%X)))));" % (d, a, bits))
        else:
            out.append(
                "simde_mm_store_ps(ctx.v%d.f32, rex::simde_mm_cvtepu32_ps_("
                "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)));" % (d, a))
        return out

    # =======================================================================
    # Vector Merge (emit_vec_int_binary_swapped)
    # =======================================================================
    _merge = {
        "vmrghb": ("unpackhi_epi8", "u8"),  "vmrghh": ("unpackhi_epi16", "u16"),
        "vmrghw": ("unpackhi_epi32", "u32"),
        "vmrglb": ("unpacklo_epi8", "u8"),  "vmrglh": ("unpacklo_epi16", "u16"),
        "vmrglw": ("unpacklo_epi32", "u32"),
    }
    _merge_core = core[:-3] if core.endswith("128") and core[:-3] in _merge else core
    if _merge_core in _merge:
        op, ty = _merge[_merge_core]
        return [vint2(op, ty, _r(0), _r(2), _r(1))]  # swapped

    # =======================================================================
    # Vector Permute
    # =======================================================================
    if core in ("vperm", "vperm128"):
        d, a, b, c = _r(0), _r(1), _r(2), _r(3)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, rex::simde_mm_perm_epi8_("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)));" % (d, a, b, c))]
    if core == "vpermwi128":
        d, a = _r(0), _r(1)
        imm = _imm(ops[2])
        x = 3 - (imm & 0x3)
        y = 3 - ((imm >> 2) & 0x3)
        z = 3 - ((imm >> 4) & 0x3)
        w = 3 - ((imm >> 6) & 0x3)
        perm = x | (y << 2) | (z << 4) | (w << 6)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u32, simde_mm_shuffle_epi32("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), 0x%X));" % (d, a, perm))]
    if core == "vrlimi128":
        d, a = _r(0), _r(1)
        blend = _imm(ops[2])
        idx = _imm(ops[3])
        shuffles = [(3 << 6) | (2 << 4) | (1 << 2) | 0,
                    (2 << 6) | (1 << 4) | (0 << 2) | 3,
                    (1 << 6) | (0 << 4) | (3 << 2) | 2,
                    (0 << 6) | (3 << 4) | (2 << 2) | 1]
        return [("simde_mm_store_ps(ctx.v%d.f32, simde_mm_blend_ps("
                 "simde_mm_load_ps(ctx.v%d.f32), simde_mm_permute_ps("
                 "simde_mm_load_ps(ctx.v%d.f32), %d), %d));"
                 % (d, d, a, shuffles[idx], blend))]

    # =======================================================================
    # Vector Shift / Rotate
    # =======================================================================
    if core == "vsl":
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, rex::simde_mm_vsl("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)));" % (d, a, b))]
    if core in ("vslo", "vslo128"):
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, rex::simde_mm_vslo("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)));" % (d, a, b))]
    if core in ("vsro", "vsro128"):
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, rex::simde_mm_vsro("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)));" % (d, a, b))]
    if core == "vsr":
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, rex::simde_mm_vsr("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)));" % (d, a, b))]
    if core in ("vsldoi", "vsldoi128"):
        d, a, b = _r(0), _r(1), _r(2)
        sh = _imm(ops[3])
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_alignr_epi8("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), %d));"
                 % (d, a, b, 16 - sh))]
    if core == "vslb":
        d, a, b = _r(0), _r(1), _r(2)
        return ["ctx.v%d.u8[%d] = ctx.v%d.u8[%d] << (ctx.v%d.u8[%d] & 0x7);"
                % (d, i, a, i, b, i) for i in range(16)]
    if core == "vslh":
        d, a, b = _r(0), _r(1), _r(2)
        return ["ctx.v%d.u16[%d] = ctx.v%d.u16[%d] << (ctx.v%d.u16[%d] & 0xF);"
                % (d, i, a, i, b, i) for i in range(8)]
    if core == "vsrh":
        d, a, b = _r(0), _r(1), _r(2)
        return ["ctx.v%d.u16[%d] = ctx.v%d.u16[%d] >> (ctx.v%d.u16[%d] & 0xF);"
                % (d, i, a, i, b, i) for i in range(8)]
    if core == "vsrb":
        d, a, b = _r(0), _r(1), _r(2)
        return ["ctx.v%d.u8[%d] = ctx.v%d.u8[%d] >> (ctx.v%d.u8[%d] & 0x7);"
                % (d, i, a, i, b, i) for i in range(16)]
    if core == "vsrab":
        d, a, b = _r(0), _r(1), _r(2)
        return ["ctx.v%d.s8[%d] = ctx.v%d.s8[%d] >> (ctx.v%d.u8[%d] & 0x7);"
                % (d, i, a, i, b, i) for i in range(16)]
    if core == "vsrah":
        d, a, b = _r(0), _r(1), _r(2)
        return ["ctx.v%d.s16[%d] = ctx.v%d.s16[%d] >> (ctx.v%d.u16[%d] & 0xF);"
                % (d, i, a, i, b, i) for i in range(8)]
    if core in ("vslw", "vslw128"):
        d, a, b = _r(0), _r(1), _r(2)
        return ["ctx.v%d.u32[%d] = ctx.v%d.u32[%d] << (ctx.v%d.u8[%d] & 0x1F);"
                % (d, i, a, i, b, i * 4) for i in range(4)]
    if core in ("vsraw", "vsraw128"):
        d, a, b = _r(0), _r(1), _r(2)
        return ["ctx.v%d.s32[%d] = ctx.v%d.s32[%d] >> (ctx.v%d.u8[%d] & 0x1F);"
                % (d, i, a, i, b, i * 4) for i in range(4)]
    if core in ("vsrw", "vsrw128"):
        d, a, b = _r(0), _r(1), _r(2)
        return ["ctx.v%d.u32[%d] = ctx.v%d.u32[%d] >> (ctx.v%d.u8[%d] & 0x1F);"
                % (d, i, a, i, b, i * 4) for i in range(4)]
    if core == "vrlh":
        d, a, b = _r(0), _r(1), _r(2)
        out = []
        for i in range(8):
            out.append("{ uint16_t sh = ctx.v%d.u16[%d] & 0xF;" % (b, i))
            out.append("ctx.v%d.u16[%d] = (ctx.v%d.u16[%d] << sh) | "
                       "(sh ? (ctx.v%d.u16[%d] >> (16 - sh)) : 0); }" % (d, i, a, i, a, i))
        return out
    if core in ("vrlw", "vrlw128"):
        d, a, b = _r(0), _r(1), _r(2)
        out = []
        for i in range(4):
            out.append("{ uint32_t sh = ctx.v%d.u32[%d] & 0x1F;" % (b, i))
            out.append("ctx.v%d.u32[%d] = (ctx.v%d.u32[%d] << sh) | "
                       "(sh ? (ctx.v%d.u32[%d] >> (32 - sh)) : 0); }" % (d, i, a, i, a, i))
        return out

    # =======================================================================
    # Vector Splat  (element-reversal math reproduced verbatim)
    # =======================================================================
    if core == "vspltb":
        d, a = _r(0), _r(1)
        perm = 15 - _imm(ops[2])
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_shuffle_epi8("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                 "simde_mm_set1_epi8(char(0x%X))));" % (d, a, perm))]
    if core == "vsplth":
        d, a = _r(0), _r(1)
        perm = 7 - _imm(ops[2])
        perm = (perm * 2) | ((perm * 2 + 1) << 8)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u16, simde_mm_shuffle_epi8("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), "
                 "simde_mm_set1_epi16(short(0x%X))));" % (d, a, perm))]
    if core in ("vspltw", "vspltw128"):
        d, a = _r(0), _r(1)
        perm = 3 - _imm(ops[2])
        perm |= (perm << 2) | (perm << 4) | (perm << 6)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u32, simde_mm_shuffle_epi32("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), 0x%X));" % (d, a, perm))]
    if core == "vspltisb":
        d = _r(0)
        s = _imm(ops[1]) & 0x1F
        if s & 0x10:
            s -= 0x20
        return ["simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, "
                "simde_mm_set1_epi8(char(0x%X)));" % (d, s & 0xFF)]
    if core == "vspltish":
        d = _r(0)
        s = _imm(ops[1]) & 0x1F
        if s & 0x10:
            s -= 0x20
        return ["simde_mm_store_si128((simde__m128i*)ctx.v%d.s16, "
                "simde_mm_set1_epi16(short(0x%X)));" % (d, s & 0xFFFF)]
    if core in ("vspltisw", "vspltisw128"):
        d = _r(0)
        s = _imm(ops[1]) & 0x1F
        if s & 0x10:
            s -= 0x20
        return ["simde_mm_store_si128((simde__m128i*)ctx.v%d.u32, "
                "simde_mm_set1_epi32(int(0x%X)));" % (d, s & 0xFFFFFFFF)]

    # =======================================================================
    # Vector Pack
    # =======================================================================
    if core in ("vpkshus", "vpkshus128"):
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_packus_epi16("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.s16), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.s16)));" % (d, b, a))]
    if core in ("vpkuhum", "vpkuhum128"):
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_packus_epi16("
                 "simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), "
                 "simde_mm_set1_epi16(0xFF)), "
                 "simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), "
                 "simde_mm_set1_epi16(0xFF))));" % (d, b, a))]
    if core in ("vpkuwum", "vpkuwum128"):
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u16, simde_mm_packus_epi32("
                 "simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), "
                 "simde_mm_set1_epi32(0xFFFF)), "
                 "simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), "
                 "simde_mm_set1_epi32(0xFFFF))));" % (d, b, a))]
    if core in ("vpkshss", "vpkshss128"):
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.s8, simde_mm_packs_epi16("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.s16), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.s16)));" % (d, b, a))]
    if core in ("vpkswss", "vpkswss128"):
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.s16, simde_mm_packs_epi32("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.s32), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.s32)));" % (d, b, a))]
    if core in ("vpkswus", "vpkswus128"):
        d, a, b = _r(0), _r(1), _r(2)
        return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.u16, simde_mm_packus_epi32("
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.s32), "
                 "simde_mm_load_si128((simde__m128i*)ctx.v%d.s32)));" % (d, b, a))]
    if core in ("vpkuhus", "vpkuhus128"):
        d, a, b = _r(0), _r(1), _r(2)
        out = []
        for i in range(8):
            out.append("ctx.v%d.u8[%d] = ctx.v%d.u16[%d] > 0xFF ? 0xFF : (uint8_t)ctx.v%d.u16[%d];"
                       % (d, 15 - i, a, 7 - i, a, 7 - i))
            out.append("ctx.v%d.u8[%d] = ctx.v%d.u16[%d] > 0xFF ? 0xFF : (uint8_t)ctx.v%d.u16[%d];"
                       % (d, 7 - i, b, 7 - i, b, 7 - i))
        return out
    if core in ("vpkuwus", "vpkuwus128"):
        d, a, b = _r(0), _r(1), _r(2)
        out = []
        for i in range(4):
            out.append("ctx.v%d.u16[%d] = ctx.v%d.u32[%d] > 0xFFFF ? 0xFFFF : (uint16_t)ctx.v%d.u32[%d];"
                       % (d, 7 - i, a, 3 - i, a, 3 - i))
            out.append("ctx.v%d.u16[%d] = ctx.v%d.u32[%d] > 0xFFFF ? 0xFFFF : (uint16_t)ctx.v%d.u32[%d];"
                       % (d, 3 - i, b, 3 - i, b, 3 - i))
        return out

    if core == "vpkd3d128":
        return _emit_vpkd3d128(ops, _vreg, _imm)

    # =======================================================================
    # Vector Unpack
    # =======================================================================
    if core == "vupkd3d128":
        return _emit_vupkd3d128(ops, _vreg, _imm)

    if core in ("vupkhsb", "vupkhsb128"):
        # VMX128 misID fixup: VUPKHSB128 with operands[2]==0x60 -> VUPKHSH128
        if core == "vupkhsb128" and len(ops) > 2 and _imm(ops[2]) == 0x60:
            return _emit_vupkhsh(_vreg(ops[0]), _vreg(ops[1]))
        return _emit_vupkhsb(_vreg(ops[0]), _vreg(ops[1]))
    if core in ("vupkhsh", "vupkhsh128"):
        return _emit_vupkhsh(_vreg(ops[0]), _vreg(ops[1]))
    if core in ("vupklsb", "vupklsb128"):
        if core == "vupklsb128" and len(ops) > 2 and _imm(ops[2]) == 0x60:
            return _emit_vupklsh(_vreg(ops[0]), _vreg(ops[1]))
        return _emit_vupklsb(_vreg(ops[0]), _vreg(ops[1]))
    if core in ("vupklsh", "vupklsh128"):
        return _emit_vupklsh(_vreg(ops[0]), _vreg(ops[1]))

    return None


# ---------------------------------------------------------------------------
# Unpack (simple, si128) helpers
# ---------------------------------------------------------------------------
def _emit_vupkhsb(d, a):
    return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.s16, simde_mm_cvtepi8_epi16("
             "simde_mm_unpackhi_epi64(simde_mm_load_si128((simde__m128i*)ctx.v%d.s8), "
             "simde_mm_load_si128((simde__m128i*)ctx.v%d.s8))));" % (d, a, a))]


def _emit_vupkhsh(d, a):
    return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.s32, simde_mm_cvtepi16_epi32("
             "simde_mm_unpackhi_epi64(simde_mm_load_si128((simde__m128i*)ctx.v%d.s16), "
             "simde_mm_load_si128((simde__m128i*)ctx.v%d.s16))));" % (d, a, a))]


def _emit_vupklsb(d, a):
    return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.s32, simde_mm_cvtepi8_epi16("
             "simde_mm_load_si128((simde__m128i*)ctx.v%d.s16)));" % (d, a))]


def _emit_vupklsh(d, a):
    return [("simde_mm_store_si128((simde__m128i*)ctx.v%d.s32, simde_mm_cvtepi16_epi32("
             "simde_mm_load_si128((simde__m128i*)ctx.v%d.s16)));" % (d, a))]


# ---------------------------------------------------------------------------
# vpkd3d128 : D3D pack (scalar). operands[0]=vD, [1]=vB(src), [2]=type,
#             [3]=mask, [4]=shift.  Emits "ctx.fpscr.enableFlushMode();" first.
# ---------------------------------------------------------------------------
def _emit_vpkd3d128(ops, _vreg, _imm):
    d = _vreg(ops[0])
    s = _vreg(ops[1])
    typ = _imm(ops[2])
    out = ["ctx.fpscr.enableFlushMode();"]

    if typ == 0:  # D3D color
        mask = _imm(ops[3])
        shift = _imm(ops[4])
        indices = [3, 0, 1, 2]
        for i in range(4):
            out.append("vTemp.u32[%d] = 0x404000FF;" % i)
            out.append(
                "vTemp.f32[%d] = !(ctx.v%d.f32[%d] >= 3.0f) ? 3.0f : (ctx.v%d.f32[%d] > "
                "vTemp.f32[%d] ? vTemp.f32[%d] : ctx.v%d.f32[%d]);"
                % (i, s, i, s, i, i, i, s, i))
            out.append("temp.u32 %s= uint32_t(vTemp.u8[%d]) << %d;"
                       % ("" if i == 0 else "|", i * 4, indices[i] * 8))
        if mask == 3 and shift == 3:
            out.append("ctx.v%d.u32[0] = 0;" % d)
        else:
            out.append("ctx.v%d.u32[%d] = temp.u32;" % (d, shift))
            if mask >= 2 and shift < 3:
                out.append("ctx.v%d.u32[%d] = 0;" % (d, shift + 1))
        return out

    if typ == 1:  # NORMSHORT2
        sh = _imm(ops[4])
        out.append("temp.s32 = ctx.v%d.s32[3] - 0x40400000;" % s)
        out.append("temp.s32 = temp.s32 > 32767 ? 32767 : (temp.s32 < -32767 ? -32767 : temp.s32);")
        out.append("vTemp.u32[0] = uint32_t(uint16_t(temp.s32)) << 16;")
        out.append("temp.s32 = ctx.v%d.s32[2] - 0x40400000;" % s)
        out.append("temp.s32 = temp.s32 > 32767 ? 32767 : (temp.s32 < -32767 ? -32767 : temp.s32);")
        out.append("vTemp.u32[0] |= uint16_t(temp.s32);")
        out.append("ctx.v%d.u32[%d] = vTemp.u32[0];" % (d, sh))
        return out

    if typ == 2:  # NORMPACKED32 (2:10:10:10)
        sh = _imm(ops[4])
        out.append("vTemp.u32[0] = 0;")
        out.append("temp.f32 = ctx.v%d.f32[3] < kPack2101010_Min10 ? kPack2101010_Min10 : "
                   "(ctx.v%d.f32[3] > kPack2101010_Max10 ? kPack2101010_Max10 : ctx.v%d.f32[3]);"
                   % (s, s, s))
        out.append("vTemp.u32[0] = temp.u32 & 0x3FF;")
        out.append("temp.f32 = ctx.v%d.f32[2] < kPack2101010_Min10 ? kPack2101010_Min10 : "
                   "(ctx.v%d.f32[2] > kPack2101010_Max10 ? kPack2101010_Max10 : ctx.v%d.f32[2]);"
                   % (s, s, s))
        out.append("vTemp.u32[0] |= (temp.u32 & 0x3FF) << 10;")
        out.append("temp.f32 = ctx.v%d.f32[1] < kPack2101010_Min10 ? kPack2101010_Min10 : "
                   "(ctx.v%d.f32[1] > kPack2101010_Max10 ? kPack2101010_Max10 : ctx.v%d.f32[1]);"
                   % (s, s, s))
        out.append("vTemp.u32[0] |= (temp.u32 & 0x3FF) << 20;")
        out.append("temp.f32 = ctx.v%d.f32[0] < kPack2101010_Min2 ? kPack2101010_Min2 : "
                   "(ctx.v%d.f32[0] > kPack2101010_Max2 ? kPack2101010_Max2 : ctx.v%d.f32[0]);"
                   % (s, s, s))
        out.append("vTemp.u32[0] |= (temp.u32 & 0x3) << 30;")
        out.append("ctx.v%d.u32[%d] = vTemp.u32[0];" % (d, sh))
        return out

    if typ in (3, 5):  # FLOAT16_2 (case 3) / FLOAT16_4 (case 5)
        n = 2 if typ == 3 else 4
        sh = _imm(ops[4])
        for i in range(n):
            src = 3 - i
            if typ == 3:
                dst = (1 - i) + (2 * sh)
            else:
                dst = (3 - i) + (2 * sh)
            out.append("temp.u32 = (ctx.v%d.u32[%d]&0x7FFFFFFF);" % (s, src))
            out.append("vTemp.u8[0] = (temp.f32 != temp.f32) || (temp.f32 > 65504.0f) ? 0xFF : "
                       "((ctx.v%d.u32[%d]&0x7f800000)>>23);" % (s, src))
            out.append("temp.u16 = vTemp.u8[0] != 0xFF ? ((ctx.v%d.u32[%d]&0x7FE000)>>13) : 0x0;"
                       % (s, src))
            out.append("ctx.v%d.u16[%d] = vTemp.u8[0] != 0xFF ? (vTemp.u8[0] > 0x70 ? "
                       "(((vTemp.u8[0]-0x70)<<10)+temp.u16) : (0x71-vTemp.u8[0] > 31 ? 0x0 : "
                       "((0x400+temp.u16)>>(0x71-vTemp.u8[0])))) : 0x7FFF;" % (d, dst))
            out.append("ctx.v%d.u16[%d] |= ((ctx.v%d.u32[%d]&0x80000000)>>16);" % (d, dst, s, src))
        return out

    if typ == 4:  # NORMSHORT4
        sh = _imm(ops[4])
        for i in range(4):
            src = 3 - i
            dst = (3 - i) + (2 * sh)
            out.append("temp.s32 = ctx.v%d.s32[%d] - 0x40400000;" % (s, src))
            out.append("temp.s32 = temp.s32 > 32767 ? 32767 : (temp.s32 < -32767 ? -32767 : temp.s32);")
            out.append("ctx.v%d.u16[%d] = uint16_t(temp.s32);" % (d, dst))
        return out

    if typ == 6:  # NORMPACKED64 (4:20:20:20)
        sh = _imm(ops[4])
        out.append("vTemp.u64[0] = 0;")
        out.append("temp.s32 = int32_t(ctx.v%d.f32[0]);" % s)
        out.append("vTemp.u64[0] = uint64_t(temp.s32 & 0xFFFFF);")
        out.append("temp.s32 = int32_t(ctx.v%d.f32[1]);" % s)
        out.append("vTemp.u64[0] |= uint64_t(temp.s32 & 0xFFFFF) << 20;")
        out.append("temp.s32 = int32_t(ctx.v%d.f32[2]);" % s)
        out.append("vTemp.u64[0] |= uint64_t(temp.s32 & 0xFFFFF) << 40;")
        out.append("temp.s32 = int32_t(ctx.v%d.f32[3]);" % s)
        out.append("vTemp.u64[0] |= uint64_t(temp.s32 & 0xF) << 60;")
        out.append("ctx.v%d.u64[%d] = vTemp.u64[0];" % (d, sh >> 1))
        return out

    out.append("__builtin_debugtrap();")
    return out


# ---------------------------------------------------------------------------
# vupkd3d128 : D3D unpack (scalar). operands[0]=vD, [1]=vB(src), [2]=type.
#              switch is on (operands[2] >> 2).
# ---------------------------------------------------------------------------
def _emit_vupkd3d128(ops, _vreg, _imm):
    d = _vreg(ops[0])
    s = _vreg(ops[1])
    sel = _imm(ops[2]) >> 2
    out = []

    if sel == 0:  # D3D color
        indices = [3, 0, 1, 2]
        for i in range(4):
            out.append("vTemp.u32[%d] = ctx.v%d.u8[%d] | 0x3F800000;" % (i, s, indices[i]))
        out.append("ctx.v%d = vTemp;" % d)
        return out

    if sel == 1:  # NORMSHORT2
        for i in range(2):
            out.append("temp.f32 = 3.0f;")
            out.append("temp.s32 += ctx.v%d.s16[%d];" % (s, 1 - i))
            out.append("vTemp.f32[%d] = temp.f32;" % (3 - i))
        out.append("vTemp.f32[1] = 0.0f;")
        out.append("vTemp.f32[0] = 1.0f;")
        out.append("ctx.v%d = vTemp;" % d)
        return out

    if sel == 2:  # NORMPACKED32
        out.append("temp.s32 = (ctx.v%d.s32[0] << 22) >> 22;" % s)
        out.append("vTemp.s32[0] = temp.s32 == -512 ? 0x7FC00000 : (temp.s32 + 0x40400000);")
        out.append("ctx.v%d.u32[3] = vTemp.u32[0];" % d)
        out.append("temp.s32 = (ctx.v%d.s32[0] << 12) >> 22;" % s)
        out.append("vTemp.s32[0] = temp.s32 + 0x40400000;")
        out.append("ctx.v%d.u32[2] = vTemp.u32[0];" % d)
        out.append("temp.s32 = (ctx.v%d.s32[0] << 2) >> 22;" % s)
        out.append("vTemp.s32[0] = temp.s32 + 0x40400000;")
        out.append("ctx.v%d.u32[1] = vTemp.u32[0];" % d)
        out.append("vTemp.u32[0] = (ctx.v%d.u32[0] >> 30) | 0x3F800000;" % s)
        out.append("ctx.v%d.u32[0] = vTemp.u32[0];" % d)
        return out

    if sel == 3:  # FLOAT16_2
        for i in range(2):
            src = 1 - i
            dst = 3 - i
            out.append("temp.u32 = ctx.v%d.u16[%d];" % (s, src))
            out.append("vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | "
                       "(((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);")
            out.append("if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;")
            out.append("ctx.v%d.u32[%d] = vTemp.u32[0];" % (d, dst))
        out.append("ctx.v%d.f32[1] = 0.0f;" % d)
        out.append("ctx.v%d.f32[0] = 1.0f;" % d)
        return out

    if sel == 4:  # NORMSHORT4
        for i in range(4):
            src = 3 - i
            dst = 3 - i
            out.append("temp.f32 = 3.0f;")
            out.append("temp.s32 += ctx.v%d.s16[%d];" % (s, src))
            out.append("ctx.v%d.f32[%d] = temp.f32;" % (d, dst))
        return out

    if sel == 5:  # FLOAT16_4
        for i in range(4):
            src = 3 - i
            dst = 3 - i
            out.append("temp.u32 = ctx.v%d.u16[%d];" % (s, src))
            out.append("vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | "
                       "(((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);")
            out.append("if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;")
            out.append("ctx.v%d.u32[%d] = vTemp.u32[0];" % (d, dst))
        return out

    if sel == 6:  # NORMPACKED64
        out.append("vTemp.u64[0] = ctx.v%d.u64[1];" % s)
        out.append("temp.s32 = (int32_t(vTemp.u64[0] << 44) >> 44);")
        out.append("ctx.v%d.f32[0] = float(temp.s32);" % d)
        out.append("temp.s32 = (int32_t(vTemp.u64[0] << 24) >> 44);")
        out.append("ctx.v%d.f32[1] = float(temp.s32);" % d)
        out.append("temp.s32 = (int32_t(vTemp.u64[0] << 4) >> 44);")
        out.append("ctx.v%d.f32[2] = float(temp.s32);" % d)
        out.append("ctx.v%d.f32[3] = float(vTemp.u64[0] >> 60);" % d)
        return out

    out.append("__builtin_debugtrap();")
    return out
