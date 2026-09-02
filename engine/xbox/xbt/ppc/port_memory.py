# -*- coding: utf-8 -*-
# ============================================================================
# Faithful port of RexGlue codegen builder: rexcodegen/builders/memory.cpp
#   (source staged at /tmp/cg/builders/memory.cpp)
# Emission helpers referenced: /tmp/cg/builders/context.cpp,
#   /tmp/cg/builders/helpers.h, /tmp/cg/builders/builder_context.h
# Opcode->builder map: /tmp/cg/instruction_dispatch.cpp
#
# Category: MEMORY (loads/stores incl. indexed/update/algebraic/byte-reversed,
#   atomics lwarx/ldarx/stwcx./stdcx., FP loads/stores, vector loads/stores).
#
# Register access follows the brief: ALWAYS ctx.rN / ctx.fN / ctx.vN / ctx.crN
# (the C++ local-variable optimization in BuilderContext::r/f/v/cr is ignored).
# ctx.ea() -> "ea", ctx.temp() -> "temp", ctx.reserved() -> "ctx.reserved",
# ctx.xer() -> "ctx.xer". emit_set_flush_mode(false) -> "ctx.fpscr.disableFlushMode();".
#
# MMIO: the C++ special-cases MMIO (see UNPORTED + the DETAIL note at bottom of
# this file). The emit(mn, ops, ...) signature exposes neither the next-word
# `eieio` lookahead nor the per-function lis-based mmio-base register tracking,
# so every builder here emits the NON-MMIO path (PPC_LOAD_*/PPC_STORE_*), which
# is what mmio_check_{d,x}_form() returns when neither trigger is present.
# ============================================================================


def emit(mn, ops, _reg, _vreg, _freg, _imm, _cr, _mem, _rv, _base_expr, _addr, va, WORD):

    def i32(x):
        x &= 0xFFFFFFFF
        return x - 0x100000000 if x & 0x80000000 else x

    def dform_addr(a, disp):
        # C++: if (operands[base] != 0) print("{}.u32 + "); print(disp)
        return (f"ctx.r{a}.u32 + " if a != 0 else "") + f"{disp}"

    def xform_addr(a, b):
        # C++: if (operands[rA] != 0) print("{}.u32 + "); print("{}.u32", rB)
        return (f"ctx.r{a}.u32 + " if a != 0 else "") + f"ctx.r{b}.u32"

    # -----------------------------------------------------------------------
    # Load Immediate family (L* category; not real memory ops)
    # -----------------------------------------------------------------------
    if mn == "li":
        d = _reg(ops[0])
        return [f"ctx.r{d}.s64 = {i32(_imm(ops[1]))};"]

    if mn == "lis":
        # C++: rD.s64 = static_cast<int32_t>(imm << 16); (+ mmio-base tracking, dropped)
        d = _reg(ops[0])
        imm = _imm(ops[1]) & 0xFFFFFFFF
        return [f"ctx.r{d}.s64 = {i32(imm << 16)};"]

    # -----------------------------------------------------------------------
    # D-form loads: rD.u64 = MACRO(rA + D)          (emit_load_d_form)
    # -----------------------------------------------------------------------
    _LD_D = {"lbz": "PPC_LOAD_U8", "lhz": "PPC_LOAD_U16",
             "lwz": "PPC_LOAD_U32", "ld": "PPC_LOAD_U64"}
    if mn in _LD_D:
        d = _reg(ops[0]); disp, a = _mem(ops[1])
        return [f"ctx.r{d}.u64 = {_LD_D[mn]}({dform_addr(a, disp)});"]

    # -----------------------------------------------------------------------
    # X-form loads: rD.u64 = MACRO(rA + rB)         (emit_load_x_form)
    # -----------------------------------------------------------------------
    _LD_X = {"lbzx": "PPC_LOAD_U8", "lhzx": "PPC_LOAD_U16",
             "lwzx": "PPC_LOAD_U32", "ldx": "PPC_LOAD_U64"}
    if mn in _LD_X:
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.u64 = {_LD_X[mn]}({xform_addr(a, b)});"]

    # -----------------------------------------------------------------------
    # D-form load w/ update: ea=D+rA; rD.u64=MACRO(ea); rA.u32=ea  (emitLoadWithUpdate)
    # -----------------------------------------------------------------------
    _LDU_D = {"lbzu": "PPC_LOAD_U8", "lhzu": "PPC_LOAD_U16",
              "lwzu": "PPC_LOAD_U32", "ldu": "PPC_LOAD_U64"}
    if mn in _LDU_D:
        d = _reg(ops[0]); disp, a = _mem(ops[1])
        return [f"ea = {disp} + ctx.r{a}.u32;",
                f"ctx.r{d}.u64 = {_LDU_D[mn]}(ea);",
                f"ctx.r{a}.u32 = ea;"]

    # -----------------------------------------------------------------------
    # X-form load w/ update: ea=rA+rB; rD.u64=MACRO(ea); rA.u32=ea (emitLoadXFormWithUpdate)
    # -----------------------------------------------------------------------
    _LDU_X = {"lbzux": "PPC_LOAD_U8", "lhzux": "PPC_LOAD_U16",
              "lwzux": "PPC_LOAD_U32", "ldux": "PPC_LOAD_U64"}
    if mn in _LDU_X:
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ea = ctx.r{a}.u32 + ctx.r{b}.u32;",
                f"ctx.r{d}.u64 = {_LDU_X[mn]}(ea);",
                f"ctx.r{a}.u32 = ea;"]

    # -----------------------------------------------------------------------
    # Sign-extending D-form loads: rD.s64 = CAST(MACRO(rA + D)) (emitSignExtendLoadDForm)
    # -----------------------------------------------------------------------
    _LDA_D = {"lha": ("int16_t", "PPC_LOAD_U16"), "lwa": ("int32_t", "PPC_LOAD_U32")}
    if mn in _LDA_D:
        cast, macro = _LDA_D[mn]
        d = _reg(ops[0]); disp, a = _mem(ops[1])
        return [f"ctx.r{d}.s64 = {cast}({macro}({dform_addr(a, disp)}));"]

    # -----------------------------------------------------------------------
    # Sign-extending X-form loads: rD.s64 = CAST(MACRO(rA + rB)) (emitSignExtendLoadXForm)
    # -----------------------------------------------------------------------
    _LDA_X = {"lhax": ("int16_t", "PPC_LOAD_U16"), "lwax": ("int32_t", "PPC_LOAD_U32")}
    if mn in _LDA_X:
        cast, macro = _LDA_X[mn]
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.s64 = {cast}({macro}({xform_addr(a, b)}));"]

    # -----------------------------------------------------------------------
    # Algebraic loads with update (explicit builders)
    # -----------------------------------------------------------------------
    if mn == "lhau":  # D-form: ea=D+rA; rD.s64=int16_t(LOAD16(ea)); rA.u32=ea
        d = _reg(ops[0]); disp, a = _mem(ops[1])
        return [f"ea = {disp} + ctx.r{a}.u32;",
                f"ctx.r{d}.s64 = int16_t(PPC_LOAD_U16(ea));",
                f"ctx.r{a}.u32 = ea;"]

    if mn == "lhaux":  # X-form: ea=rA+rB; rD.s64=int16_t(LOAD16(ea)); rA.u32=ea
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ea = ctx.r{a}.u32 + ctx.r{b}.u32;",
                f"ctx.r{d}.s64 = int16_t(PPC_LOAD_U16(ea));",
                f"ctx.r{a}.u32 = ea;"]

    if mn == "lwaux":  # X-form: ea=rA+rB; rD.s64=int32_t(LOAD32(ea)); rA.u32=ea
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ea = ctx.r{a}.u32 + ctx.r{b}.u32;",
                f"ctx.r{d}.s64 = int32_t(PPC_LOAD_U32(ea));",
                f"ctx.r{a}.u32 = ea;"]

    # -----------------------------------------------------------------------
    # Byte-reverse indexed loads (X-form). Non-MMIO -> PPC_LOAD_*.
    # -----------------------------------------------------------------------
    _LBR = {"lhbrx": ("__builtin_bswap16", "PPC_LOAD_U16"),
            "lwbrx": ("__builtin_bswap32", "PPC_LOAD_U32")}
    if mn in _LBR:
        bswap, macro = _LBR[mn]
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.u64 = {bswap}({macro}({xform_addr(a, b)}));"]

    # -----------------------------------------------------------------------
    # Atomic load-and-reserve (X-form). (emitAtomicLoadReserve)
    #   ea = [rA +] rB; reserved.field = *(T*)PPC_RAW_ADDR(ea); rD.u64 = bswap(reserved.field)
    # -----------------------------------------------------------------------
    _LARX = {"lwarx": ("uint32_t", "__builtin_bswap32", "u32"),
             "ldarx": ("uint64_t", "__builtin_bswap64", "u64")}
    if mn in _LARX:
        ptr, bswap, field = _LARX[mn]
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ea = {xform_addr(a, b)};",
                f"ctx.reserved.{field} = *({ptr}*)PPC_RAW_ADDR(ea);",
                f"ctx.r{d}.u64 = {bswap}(ctx.reserved.{field});"]

    # -----------------------------------------------------------------------
    # Floating-point loads (all emit disableFlushMode first)
    # -----------------------------------------------------------------------
    if mn == "lfd":  # D-form: fD.u64 = PPC_LOAD_U64(rA + D)
        d = _freg(ops[0]); disp, a = _mem(ops[1])
        return ["ctx.fpscr.disableFlushMode();",
                f"ctx.f{d}.u64 = PPC_LOAD_U64({dform_addr(a, disp)});"]

    if mn == "lfdx":  # X-form: fD.u64 = PPC_LOAD_U64(rA + rB)
        d, a, b = _freg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return ["ctx.fpscr.disableFlushMode();",
                f"ctx.f{d}.u64 = PPC_LOAD_U64({xform_addr(a, b)});"]

    if mn == "lfs":  # D-form: temp.u32 = LOAD32(rA + D); fD.f64 = double(temp.f32)
        d = _freg(ops[0]); disp, a = _mem(ops[1])
        return ["ctx.fpscr.disableFlushMode();",
                f"temp.u32 = PPC_LOAD_U32({dform_addr(a, disp)});",
                f"ctx.f{d}.f64 = double(temp.f32);"]

    if mn == "lfsx":  # X-form: temp.u32 = LOAD32(rA + rB); fD.f64 = double(temp.f32)
        d, a, b = _freg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return ["ctx.fpscr.disableFlushMode();",
                f"temp.u32 = PPC_LOAD_U32({xform_addr(a, b)});",
                f"ctx.f{d}.f64 = double(temp.f32);"]

    if mn == "lfdu":  # D-form update
        d = _freg(ops[0]); disp, a = _mem(ops[1])
        return ["ctx.fpscr.disableFlushMode();",
                f"ea = {disp} + ctx.r{a}.u32;",
                f"ctx.f{d}.u64 = PPC_LOAD_U64(ea);",
                f"ctx.r{a}.u32 = ea;"]

    if mn == "lfdux":  # X-form update
        d, a, b = _freg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return ["ctx.fpscr.disableFlushMode();",
                f"ea = ctx.r{a}.u32 + ctx.r{b}.u32;",
                f"ctx.f{d}.u64 = PPC_LOAD_U64(ea);",
                f"ctx.r{a}.u32 = ea;"]

    if mn == "lfsu":  # D-form update (single -> double)
        d = _freg(ops[0]); disp, a = _mem(ops[1])
        return ["ctx.fpscr.disableFlushMode();",
                f"ea = {disp} + ctx.r{a}.u32;",
                "temp.u32 = PPC_LOAD_U32(ea);",
                f"ctx.f{d}.f64 = double(temp.f32);",
                f"ctx.r{a}.u32 = ea;"]

    if mn == "lfsux":  # X-form update (single -> double)
        d, a, b = _freg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return ["ctx.fpscr.disableFlushMode();",
                f"ea = ctx.r{a}.u32 + ctx.r{b}.u32;",
                "temp.u32 = PPC_LOAD_U32(ea);",
                f"ctx.f{d}.f64 = double(temp.f32);",
                f"ctx.r{a}.u32 = ea;"]

    # -----------------------------------------------------------------------
    # D-form stores: MACRO(rA + D, rS.field)        (emit_store_d_form)
    # -----------------------------------------------------------------------
    _ST_D = {"stb": ("PPC_STORE_U8", "u8"), "sth": ("PPC_STORE_U16", "u16"),
             "stw": ("PPC_STORE_U32", "u32"), "std": ("PPC_STORE_U64", "u64")}
    if mn in _ST_D:
        macro, field = _ST_D[mn]
        s = _reg(ops[0]); disp, a = _mem(ops[1])
        return [f"{macro}({dform_addr(a, disp)}, ctx.r{s}.{field});"]

    # -----------------------------------------------------------------------
    # X-form stores: MACRO(rA + rB, rS.field)       (emit_store_x_form)
    # -----------------------------------------------------------------------
    _ST_X = {"stbx": ("PPC_STORE_U8", "u8"), "sthx": ("PPC_STORE_U16", "u16"),
             "stwx": ("PPC_STORE_U32", "u32"), "stdx": ("PPC_STORE_U64", "u64")}
    if mn in _ST_X:
        macro, field = _ST_X[mn]
        s, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{macro}({xform_addr(a, b)}, ctx.r{s}.{field});"]

    # -----------------------------------------------------------------------
    # D-form store w/ update: ea=D+rA; MACRO(ea, rS.field); rA.u32=ea (emitStoreWithUpdate)
    # -----------------------------------------------------------------------
    _STU_D = {"stbu": ("PPC_STORE_U8", "u8"), "sthu": ("PPC_STORE_U16", "u16"),
              "stwu": ("PPC_STORE_U32", "u32"), "stdu": ("PPC_STORE_U64", "u64")}
    if mn in _STU_D:
        macro, field = _STU_D[mn]
        s = _reg(ops[0]); disp, a = _mem(ops[1])
        return [f"ea = {disp} + ctx.r{a}.u32;",
                f"{macro}(ea, ctx.r{s}.{field});",
                f"ctx.r{a}.u32 = ea;"]

    # -----------------------------------------------------------------------
    # X-form store w/ update: ea=rA+rB; MACRO(ea, rS.field); rA.u32=ea (emitStoreXFormWithUpdate)
    #   (C++ selects mmio macro here; non-MMIO -> normal macro)
    # -----------------------------------------------------------------------
    _STU_X = {"stbux": ("PPC_STORE_U8", "u8"), "sthux": ("PPC_STORE_U16", "u16"),
              "stwux": ("PPC_STORE_U32", "u32"), "stdux": ("PPC_STORE_U64", "u64")}
    if mn in _STU_X:
        macro, field = _STU_X[mn]
        s, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ea = ctx.r{a}.u32 + ctx.r{b}.u32;",
                f"{macro}(ea, ctx.r{s}.{field});",
                f"ctx.r{a}.u32 = ea;"]

    # -----------------------------------------------------------------------
    # Byte-reverse indexed stores (X-form). Non-MMIO -> PPC_STORE_*.
    # -----------------------------------------------------------------------
    _SBR = {"sthbrx": ("PPC_STORE_U16", "__builtin_bswap16", "u16"),
            "stwbrx": ("PPC_STORE_U32", "__builtin_bswap32", "u32")}
    if mn in _SBR:
        macro, bswap, field = _SBR[mn]
        s, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{macro}({xform_addr(a, b)}, {bswap}(ctx.r{s}.{field}));"]

    # -----------------------------------------------------------------------
    # Atomic store-conditional (X-form). (emitAtomicStoreConditional)
    #   ea=[rA +]rB; cr0.lt=0; cr0.gt=0;
    #   cr0.eq = __sync_bool_compare_and_swap((T*)PPC_RAW_ADDR(ea), reserved.field, bswap(rS.field));
    #   cr0.so = xer.so;
    # Mnemonics carry the mandatory '.' record suffix; accept dotted and bare.
    # -----------------------------------------------------------------------
    _STCX = {"stwcx.": ("uint32_t", "__builtin_bswap32", "s32"),
             "stwcx":  ("uint32_t", "__builtin_bswap32", "s32"),
             "stdcx.": ("uint64_t", "__builtin_bswap64", "s64"),
             "stdcx":  ("uint64_t", "__builtin_bswap64", "s64")}
    if mn in _STCX:
        ptr, bswap, field = _STCX[mn]
        s, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ea = {xform_addr(a, b)};",
                "ctx.cr0.lt = 0;",
                "ctx.cr0.gt = 0;",
                f"ctx.cr0.eq = __sync_bool_compare_and_swap(reinterpret_cast<{ptr}*>(PPC_RAW_ADDR(ea)), ctx.reserved.{field}, {bswap}(ctx.r{s}.{field}));",
                "ctx.cr0.so = ctx.xer.so;"]

    # -----------------------------------------------------------------------
    # Floating-point stores (all emit disableFlushMode first)
    # -----------------------------------------------------------------------
    if mn == "stfd":  # D-form: STORE64(rA + D, fS.u64)
        s = _freg(ops[0]); disp, a = _mem(ops[1])
        return ["ctx.fpscr.disableFlushMode();",
                f"PPC_STORE_U64({dform_addr(a, disp)}, ctx.f{s}.u64);"]

    if mn == "stfdx":  # X-form: STORE64(rA + rB, fS.u64)
        s, a, b = _freg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return ["ctx.fpscr.disableFlushMode();",
                f"PPC_STORE_U64({xform_addr(a, b)}, ctx.f{s}.u64);"]

    if mn == "stfdux":  # X-form update (plain PPC_STORE_U64, no mmio in C++)
        s, a, b = _freg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return ["ctx.fpscr.disableFlushMode();",
                f"ea = ctx.r{a}.u32 + ctx.r{b}.u32;",
                f"PPC_STORE_U64(ea, ctx.f{s}.u64);",
                f"ctx.r{a}.u32 = ea;"]

    if mn == "stfiwx":  # X-form: STORE32(rA + rB, fS.u32)
        s, a, b = _freg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return ["ctx.fpscr.disableFlushMode();",
                f"PPC_STORE_U32({xform_addr(a, b)}, ctx.f{s}.u32);"]

    if mn == "stfs":  # D-form: temp.f32 = float(fS.f64); STORE32(rA + D, temp.u32)
        s = _freg(ops[0]); disp, a = _mem(ops[1])
        return ["ctx.fpscr.disableFlushMode();",
                f"temp.f32 = float(ctx.f{s}.f64);",
                f"PPC_STORE_U32({dform_addr(a, disp)}, temp.u32);"]

    if mn == "stfsx":  # X-form: temp.f32 = float(fS.f64); STORE32(rA + rB, temp.u32)
        s, a, b = _freg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return ["ctx.fpscr.disableFlushMode();",
                f"temp.f32 = float(ctx.f{s}.f64);",
                f"PPC_STORE_U32({xform_addr(a, b)}, temp.u32);"]

    if mn == "stfdu":  # D-form update
        s = _freg(ops[0]); disp, a = _mem(ops[1])
        return ["ctx.fpscr.disableFlushMode();",
                f"ea = {disp} + ctx.r{a}.u32;",
                f"PPC_STORE_U64(ea, ctx.f{s}.u64);",
                f"ctx.r{a}.u32 = ea;"]

    if mn == "stfsu":  # D-form update (double -> float)
        s = _freg(ops[0]); disp, a = _mem(ops[1])
        return ["ctx.fpscr.disableFlushMode();",
                f"ea = {disp} + ctx.r{a}.u32;",
                f"temp.f32 = float(ctx.f{s}.f64);",
                "PPC_STORE_U32(ea, temp.u32);",
                f"ctx.r{a}.u32 = ea;"]

    if mn == "stfsux":  # X-form update (double -> float); non-MMIO -> PPC_STORE_U32
        s, a, b = _freg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return ["ctx.fpscr.disableFlushMode();",
                f"temp.f32 = float(ctx.f{s}.f64);",
                f"ea = ctx.r{a}.u32 + ctx.r{b}.u32;",
                "PPC_STORE_U32(ea, temp.u32);",
                f"ctx.r{a}.u32 = ea;"]

    # -----------------------------------------------------------------------
    # Vector loads. NOTE(tomc): endian handling reverses the WHOLE vector
    # (VectorMaskL shuffle), not individual elements. Ported verbatim.
    # dispatch: lvx/lvx128/lvxl128/lvebx/lvehx/lvewx/lvewx128 -> build_lvx
    # -----------------------------------------------------------------------
    if mn in ("lvx", "lvx128", "lvxl128", "lvebx", "lvehx", "lvewx", "lvewx128"):
        d, a, b = _vreg(ops[0]), _reg(ops[1]), _reg(ops[2])
        inner = (f"ctx.r{a}.u32 + " if a != 0 else "") + f"ctx.r{b}.u32"
        return [f"ea = ({inner}) & ~0xF;",
                (f"simde_mm_store_si128((simde__m128i*)ctx.v{d}.u8, "
                 f"simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), "
                 f"simde_mm_load_si128((simde__m128i*)VectorMaskL)));")]

    if mn in ("lvlx", "lvlx128"):
        d, a, b = _vreg(ops[0]), _reg(ops[1]), _reg(ops[2])
        inner = (f"ctx.r{a}.u32 + " if a != 0 else "") + f"ctx.r{b}.u32"
        return [f"temp.u32 = {inner};",
                (f"simde_mm_store_si128((simde__m128i*)ctx.v{d}.u8, "
                 f"simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(temp.u32 & ~0xF)), "
                 f"simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));")]

    if mn in ("lvrx", "lvrx128"):
        d, a, b = _vreg(ops[0]), _reg(ops[1]), _reg(ops[2])
        inner = (f"ctx.r{a}.u32 + " if a != 0 else "") + f"ctx.r{b}.u32"
        return [f"temp.u32 = {inner};",
                (f"simde_mm_store_si128((simde__m128i*)ctx.v{d}.u8, temp.u32 & 0xF ? "
                 f"simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(temp.u32 & ~0xF)), "
                 f"simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : "
                 f"simde_mm_setzero_si128());")]

    if mn == "lvsl":
        d, a, b = _vreg(ops[0]), _reg(ops[1]), _reg(ops[2])
        inner = (f"ctx.r{a}.u32 + " if a != 0 else "") + f"ctx.r{b}.u32"
        return [f"temp.u32 = {inner};",
                (f"simde_mm_store_si128((simde__m128i*)ctx.v{d}.u8, "
                 f"simde_mm_load_si128((simde__m128i*)&VectorShiftTableL[(temp.u32 & 0xF) * 16]));")]

    if mn == "lvsr":
        d, a, b = _vreg(ops[0]), _reg(ops[1]), _reg(ops[2])
        inner = (f"ctx.r{a}.u32 + " if a != 0 else "") + f"ctx.r{b}.u32"
        return [f"temp.u32 = {inner};",
                (f"simde_mm_store_si128((simde__m128i*)ctx.v{d}.u8, "
                 f"simde_mm_load_si128((simde__m128i*)&VectorShiftTableR[(temp.u32 & 0xF) * 16]));")]

    # -----------------------------------------------------------------------
    # Vector stores. NOTE: full-vector-reversal is accounted for in the
    # element index math (e.g. u16[7 - ...], u32[3 - ...], u8[15 - i]).
    # -----------------------------------------------------------------------
    if mn in ("stvehx",):
        s, a, b = _vreg(ops[0]), _reg(ops[1]), _reg(ops[2])
        inner = (f"ctx.r{a}.u32 + " if a != 0 else "") + f"ctx.r{b}.u32"
        return [f"ea = ({inner}) & ~0x1;",
                f"PPC_STORE_U16(ea, ctx.v{s}.u16[7 - ((ea & 0xF) >> 1)]);"]

    if mn in ("stvewx", "stvewx128"):
        s, a, b = _vreg(ops[0]), _reg(ops[1]), _reg(ops[2])
        inner = (f"ctx.r{a}.u32 + " if a != 0 else "") + f"ctx.r{b}.u32"
        return [f"ea = ({inner}) & ~0x3;",
                f"PPC_STORE_U32(ea, ctx.v{s}.u32[3 - ((ea & 0xF) >> 2)]);"]

    if mn in ("stvlx", "stvlx128", "stvlxl128"):
        s, a, b = _vreg(ops[0]), _reg(ops[1]), _reg(ops[2])
        inner = (f"ctx.r{a}.u32 + " if a != 0 else "") + f"ctx.r{b}.u32"
        return [f"ea = {inner};",
                "for (size_t i = 0; i < (16 - (ea & 0xF)); i++)",
                f"\tPPC_STORE_U8(ea + i, ctx.v{s}.u8[15 - i]);"]

    if mn in ("stvrx", "stvrx128"):
        s, a, b = _vreg(ops[0]), _reg(ops[1]), _reg(ops[2])
        inner = (f"ctx.r{a}.u32 + " if a != 0 else "") + f"ctx.r{b}.u32"
        return [f"ea = {inner};",
                "for (size_t i = 0; i < (ea & 0xF); i++)",
                f"\tPPC_STORE_U8(ea - i - 1, ctx.v{s}.u8[i]);"]

    if mn in ("stvx", "stvx128"):
        s, a, b = _vreg(ops[0]), _reg(ops[1]), _reg(ops[2])
        inner = (f"ctx.r{a}.u32 + " if a != 0 else "") + f"ctx.r{b}.u32"
        return [f"ea = ({inner}) & ~0xF;",
                (f"simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), "
                 f"simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v{s}.u8), "
                 f"simde_mm_load_si128((simde__m128i*)VectorMaskL)));")]

    return None


# ---------------------------------------------------------------------------
# Runtime helpers required by the emitted code that are NOT standard simde.
#
# memory.cpp itself defines no helper functions/tables -- it references symbols
# that the RexGlue PPC runtime provides. They are declared (not defined) here so
# the integrator knows the closure set; the actual table CONTENTS are runtime
# data (256-byte shuffle/mask tables) and are intentionally NOT fabricated here
# (guessing them is exactly the class of bug the brief warns against). They must
# be pulled verbatim from the RexGlue runtime (ppc_context / ppc_recomp_shared).
#
#   Macros:  PPC_LOAD_U8/16/32/64, PPC_STORE_U8/16/32/64, PPC_RAW_ADDR,
#            (and the MMIO variants PPC_MM_LOAD_*/PPC_MM_STORE_* -- see UNPORTED)
#   Builtins: __builtin_bswap16/32/64, __sync_bool_compare_and_swap
#   Tables (extern, 16-byte-aligned, indexed as [(off)*16] byte layouts):
#            VectorMaskL[], VectorMaskR[], VectorShiftTableL[], VectorShiftTableR[]
# ---------------------------------------------------------------------------
RUNTIME_HELPERS = r"""
// Provided by the RexGlue PPC runtime (do NOT redefine if already included):
//   #include <simde/x86/sse4.1.h>   // simde_mm_shuffle_epi8, etc.
// extern const uint8_t VectorMaskL[];        // whole-vector byte-reverse mask
// extern const uint8_t VectorMaskR[];
// extern const uint8_t VectorShiftTableL[];  // lvsl table
// extern const uint8_t VectorShiftTableR[];  // lvsr table
// PPC_LOAD_U*/PPC_STORE_U*/PPC_RAW_ADDR come from the recompiler runtime header.
"""


# Every mnemonic handled above (including VMX128 / element / record-form variants).
MNEMONICS = [
    # load immediate
    "li", "lis",
    # d-form loads
    "lbz", "lhz", "lwz", "ld",
    # x-form loads
    "lbzx", "lhzx", "lwzx", "ldx",
    # d-form load w/ update
    "lbzu", "lhzu", "lwzu", "ldu",
    # x-form load w/ update
    "lbzux", "lhzux", "lwzux", "ldux",
    # algebraic loads
    "lha", "lwa", "lhax", "lwax", "lhau", "lhaux", "lwaux",
    # byte-reverse loads
    "lhbrx", "lwbrx",
    # atomic load-reserve
    "lwarx", "ldarx",
    # fp loads
    "lfd", "lfdx", "lfs", "lfsx", "lfdu", "lfdux", "lfsu", "lfsux",
    # d-form stores
    "stb", "sth", "stw", "std",
    # x-form stores
    "stbx", "sthx", "stwx", "stdx",
    # d-form store w/ update
    "stbu", "sthu", "stwu", "stdu",
    # x-form store w/ update
    "stbux", "sthux", "stwux", "stdux",
    # byte-reverse stores
    "sthbrx", "stwbrx",
    # atomic store-conditional (dotted + bare)
    "stwcx.", "stwcx", "stdcx.", "stdcx",
    # fp stores
    "stfd", "stfdx", "stfdux", "stfiwx", "stfs", "stfsx", "stfdu", "stfsu", "stfsux",
    # vector loads (+ VMX128 / element variants)
    "lvx", "lvx128", "lvxl128", "lvebx", "lvehx", "lvewx", "lvewx128",
    "lvlx", "lvlx128", "lvrx", "lvrx128", "lvsl", "lvsr",
    # vector stores (+ VMX128 variants)
    "stvehx", "stvewx", "stvewx128",
    "stvlx", "stvlx128", "stvlxl128", "stvrx", "stvrx128", "stvx", "stvx128",
]


# ---------------------------------------------------------------------------
# Instructions NOT ported (with reason).
# ---------------------------------------------------------------------------
UNPORTED = [
    # (a) MMIO emission path is intentionally NOT reproduced. memory.cpp routes
    #     many builders through mmio_check_d_form()/mmio_check_x_form() (see
    #     context.cpp) which selects PPC_MM_LOAD_*/PPC_MM_STORE_* instead of the
    #     normal macro. That predicate needs (1) a raw look-ahead at the NEXT
    #     instruction word for the eieio marker 0xAC06007C, and (2) per-function
    #     stateful "this GPR holds an MMIO base" tracking seeded by build_lis via
    #     locals.set_mmio_base / isMMIOUpperBits (imm in 0x7FC8..0x7FCF or 0x7FEA).
    #     Neither input is available through emit(mn, ops, ...), so every builder
    #     here emits the non-MMIO branch. See DETAIL below. Affected builders:
    #     lbz/lhz/lwz/ld, lbzx/lhzx/lwzx/ldx (emit_load_{d,x}_form check_mmio=true),
    #     stb/sth/stw/std, stbx/sthx/stwx/stdx (emit_store_{d,x}_form check_mmio=true),
    #     stbux/sthux/stwux/stdux (emitStoreXFormWithUpdate), lhbrx/lwbrx,
    #     sthbrx/stwbrx, stfd/stfdx/stfiwx/stfs/stfsx/stfsux.
    # (b) build_lis' MMIO-base side effect (locals.set_mmio_base/clear_mmio_base)
    #     is dropped: it produces NO emitted C++, only feeds the (a) predicate.
    #     The value-producing part of lis (rD.s64 = int32(imm<<16)) IS ported.
    # (c) The whole-vector-reversal shuffle tables (VectorMaskL/R,
    #     VectorShiftTableL/R) are referenced but their DATA is not defined in
    #     memory.cpp; see RUNTIME_HELPERS. Emission of the ops referencing them
    #     is fully ported; only the table contents are runtime-provided.
    #
    # NOTE: lmw/stmw (load/store multiple), lswi/stswi (string), and the cache
    # ops dcbt/dcbtst/dcbf/dcbst/dcbz/icbi are listed in the MEMORY category
    # brief but have NO build_* in memory.cpp (not present in this source file;
    # not in instruction_dispatch.cpp's memory section either). Nothing to port
    # from this file for them.
]
