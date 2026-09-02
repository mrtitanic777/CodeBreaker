#!/usr/bin/env python3
"""
ppcrecomp.py — static recompiler: Xbox 360 PowerPC -> runnable C++ (Pipeline B).

This brings the RexGlue / XenonRecomp *method* into our own codebase. It is a
proof-of-concept AOT transpiler, not a full port: it disassembles a guest
function (via disasm_ppc) and emits **one C++ statement per instruction**
operating on a `PPCContext` register machine + a flat `uint8_t* base` guest
memory — exactly the shape the reference generates. See docs/RECOMP_DESIGN.md.

The translation model (grounded in the rexglue-sdk source and its generated
output):

    guest function        ->  void sub_XXXX(PPCContext& ctx, uint8_t* base)
    add rD,rA,rB          ->  ctx.rD.u64 = ctx.rA.u64 + ctx.rB.u64;
    lwz rD,d(rA)          ->  ctx.rD.u64 = PPC_LOAD_U32(ctx.rA.u32 + d);
    stwu rS,d(rA)         ->  ea = ctx.rA.u32 + d; PPC_STORE_U32(ea, ctx.rS.u32); ctx.rA.u32 = ea;
    cmpwi cr0,rA,imm      ->  ctx.cr0.compare<int32_t>(ctx.rA.s32, imm, ctx.xer);
    beq cr0,loc           ->  if (ctx.cr0.eq) goto loc_XXXX;
    bl  target            ->  ctx.lr = ret; sub_TARGET(ctx, base);
    bctrl                 ->  ctx.lr = ret; PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
    blr                   ->  return;

Anything not in the instruction table becomes `PPC_UNIMPLEMENTED("...")` — the
honest trap the reference also emits, never a silent wrong translation. VMX128
vector ops (disasm_ppc classifies them) currently trap too; a full port wires the
VMX128 builders (see ROADMAP).

    python ppcrecomp.py --emit-runtime rex_ppc_min.h        # write the runtime header
    python ppcrecomp.py default.xex --vaddr 0x820FBC58 --size 0x140 -o func.cpp
    python ppcrecomp.py default.xex --vaddr 0x820FBC58 --size 0x140 --check   # compile it
"""
import argparse
import os
import re
import subprocess
import sys
import tempfile

import disasm_ppc

# Ensure sibling modules (the ported RexGlue builders + jump-table recovery)
# import cleanly whether ppcrecomp is run as a script or imported by ppcproject.
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import port_float          # scalar FP builders (floating_point.cpp)
import port_arith          # integer arithmetic + comparison (arithmetic.cpp, comparison.cpp)
import port_logical        # boolean / rotate / shift / extend / immediate (logical.cpp)
import port_system         # SPR / trap / barrier / CR-logical (system.cpp)
import port_memory         # loads / stores / atomics / FP+vector mem (memory.cpp)
import port_vector         # VMX / AltiVec / VMX128 (vector.cpp)
import jumptable           # bctr switch-table recovery (function_scanner.cpp)

# The complete faithful RexGlue codegen, tried before the generic record-form
# wrapper. Order = category owner precedence on the rare cross-category mnemonic.
_PORT_MODULES = (port_float, port_arith, port_logical,
                 port_system, port_memory, port_vector)

WORD = 4

# AltiVec (128-bit) op helpers emitted into the runtime header. Kept as a plain
# string (normal C++ braces) so the f-string runtime_header() need not escape
# them. Lane order follows the register's native layout; full big-endian
# vector-load correctness is validated on real runs (see RECOMP_DESIGN.md).
_VEC_HELPERS = r"""
static inline int16_t _rex_sath(int32_t v){ return v>32767?32767:(v<-32768?-32768:(int16_t)v); }
#define REX_VBIN8(NAME,OP)  static inline void NAME(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<16;i++)d.u8[i]=(uint8_t)(OP);}
#define REX_VBIN16(NAME,OP) static inline void NAME(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<8;i++)d.u16[i]=(uint16_t)(OP);}
#define REX_VBIN32(NAME,OP) static inline void NAME(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<4;i++)d.u32[i]=(uint32_t)(OP);}
REX_VBIN8(ppc_vaddubm, a.u8[i]+b.u8[i])
REX_VBIN16(ppc_vadduhm, a.u16[i]+b.u16[i])
REX_VBIN32(ppc_vadduwm, a.u32[i]+b.u32[i])
REX_VBIN8(ppc_vsububm, a.u8[i]-b.u8[i])
REX_VBIN16(ppc_vsubuhm, a.u16[i]-b.u16[i])
REX_VBIN32(ppc_vsubuwm, a.u32[i]-b.u32[i])
REX_VBIN16(ppc_vslh, a.u16[i]<<(b.u16[i]&15))
REX_VBIN16(ppc_vsrh, a.u16[i]>>(b.u16[i]&15))
REX_VBIN32(ppc_vslw, a.u32[i]<<(b.u32[i]&31))
REX_VBIN32(ppc_vsrw, a.u32[i]>>(b.u32[i]&31))
static inline void ppc_vsrah(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<8;i++)d.s16[i]=(int16_t)(a.s16[i]>>(b.u16[i]&15));}
static inline void ppc_vsraw(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<4;i++)d.s32[i]=(int32_t)(a.s32[i]>>(b.u32[i]&31));}
static inline void ppc_vaddshs(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<8;i++)d.s16[i]=_rex_sath((int32_t)a.s16[i]+b.s16[i]);}
static inline void ppc_vsubshs(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<8;i++)d.s16[i]=_rex_sath((int32_t)a.s16[i]-b.s16[i]);}
static inline void ppc_vand(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){d.u64[0]=a.u64[0]&b.u64[0];d.u64[1]=a.u64[1]&b.u64[1];}
static inline void ppc_vor (PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){d.u64[0]=a.u64[0]|b.u64[0];d.u64[1]=a.u64[1]|b.u64[1];}
static inline void ppc_vxor(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){d.u64[0]=a.u64[0]^b.u64[0];d.u64[1]=a.u64[1]^b.u64[1];}
static inline void ppc_vandc(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){d.u64[0]=a.u64[0]&~b.u64[0];d.u64[1]=a.u64[1]&~b.u64[1];}
static inline void ppc_vnor(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){d.u64[0]=~(a.u64[0]|b.u64[0]);d.u64[1]=~(a.u64[1]|b.u64[1]);}
static inline void ppc_vmrghb(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<8;i++){t.u8[2*i]=a.u8[i];t.u8[2*i+1]=b.u8[i];}d=t;}
static inline void ppc_vmrglb(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<8;i++){t.u8[2*i]=a.u8[8+i];t.u8[2*i+1]=b.u8[8+i];}d=t;}
static inline void ppc_vmrghh(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<4;i++){t.u16[2*i]=a.u16[i];t.u16[2*i+1]=b.u16[i];}d=t;}
static inline void ppc_vmrglh(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<4;i++){t.u16[2*i]=a.u16[4+i];t.u16[2*i+1]=b.u16[4+i];}d=t;}
static inline void ppc_vmrghw(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<2;i++){t.u32[2*i]=a.u32[i];t.u32[2*i+1]=b.u32[i];}d=t;}
static inline void ppc_vmrglw(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<2;i++){t.u32[2*i]=a.u32[2+i];t.u32[2*i+1]=b.u32[2+i];}d=t;}
static inline void ppc_vperm(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b,const PPCVRegister&c){PPCVRegister t;for(int i=0;i<16;i++){int s=c.u8[i]&0x1F;t.u8[i]=s<16?a.u8[s]:b.u8[s-16];}d=t;}
static inline void ppc_vsel(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b,const PPCVRegister&c){d.u64[0]=(a.u64[0]&~c.u64[0])|(b.u64[0]&c.u64[0]);d.u64[1]=(a.u64[1]&~c.u64[1])|(b.u64[1]&c.u64[1]);}
static inline void ppc_vspltw(PPCVRegister&d,const PPCVRegister&a,int e){uint32_t v=a.u32[e&3];for(int i=0;i<4;i++)d.u32[i]=v;}
static inline void ppc_vsplth(PPCVRegister&d,const PPCVRegister&a,int e){uint16_t v=a.u16[e&7];for(int i=0;i<8;i++)d.u16[i]=v;}
static inline void ppc_vspltb(PPCVRegister&d,const PPCVRegister&a,int e){uint8_t v=a.u8[e&15];for(int i=0;i<16;i++)d.u8[i]=v;}
"""

# condition-branch mnemonic -> (C++ predicate on a cr field, is_conditional)
_COND = {
    "beq": "{cr}.eq", "bne": "!{cr}.eq",
    "blt": "{cr}.lt", "bge": "!{cr}.lt",
    "bgt": "{cr}.gt", "ble": "!{cr}.gt",
    "bso": "{cr}.so", "bns": "!{cr}.so",
    "bun": "{cr}.so", "bnu": "!{cr}.so",
}


def _reg(tok):
    tok = tok.strip()
    m = re.fullmatch(r"r(\d+)", tok)
    return int(m.group(1)) if m else None


def _imm(tok):
    try:
        return int(tok.strip(), 0)
    except ValueError:
        return None


def _mem(tok):
    """'-0x70(r1)' -> (disp, basereg) ; 'r0'-base handled by caller."""
    m = re.fullmatch(r"(-?(?:0x)?[0-9A-Fa-f]+)\((r\d+|0)\)", tok.strip())
    if not m:
        return None
    disp = int(m.group(1), 0)
    br = m.group(2)
    base = 0 if br == "0" else int(br[1:])
    return disp, base


# capstone's symbolic CR-bit suffix -> bit-within-field (un = unordered = SO).
_CRFIELD_BIT = {"lt": 0, "gt": 1, "eq": 2, "un": 3, "so": 3}


def _cr(tok):
    """Bare 'crN' -> CR *field* index 0-7 (compares/mcrf/mtcrf/branches).
    Symbolic 'crNlt|gt|eq|un|so' (capstone CR-bit operand) -> CR *bit* index
    0-31 (the crand/cror/... family, which divides by 4 to recover the field).
    The two consumers use disjoint token shapes, so one helper serves both."""
    tok = tok.strip()
    m = re.fullmatch(r"cr(\d)", tok)
    if m:
        return int(m.group(1))
    m = re.fullmatch(r"cr(\d)(lt|gt|eq|un|so)", tok)
    if m:
        return int(m.group(1)) * 4 + _CRFIELD_BIT[m.group(2)]
    return None


def _split(op_str):
    return [t.strip() for t in op_str.split(",")] if op_str.strip() else []


def _rv(n, field="u64"):
    return f"ctx.r{n}.{field}"


def _base_expr(base):
    """address base register expression; r0 in an address means literal 0."""
    return "0" if base == 0 else f"ctx.r{base}.u32"


def _addr(disp, base):
    b = _base_expr(base)
    if b == "0":
        return f"{disp}"
    return f"{b} + {disp}" if disp >= 0 else f"{b} + {disp}"


# ---------------------------------------------------------------------------
# the per-instruction emitter
# ---------------------------------------------------------------------------
def _freg(tok):
    m = re.fullmatch(r"f(\d+)", tok.strip())
    return int(m.group(1)) if m else None


def _vreg(tok):
    m = re.fullmatch(r"v(\d+)", tok.strip())
    return int(m.group(1)) if m else None


# AltiVec ops with a 3-vector-operand (vD,vA,vB) form -> a runtime helper call.
_VEC3 = frozenset((
    "vaddubm", "vadduhm", "vadduwm", "vsububm", "vsubuhm", "vsubuwm",
    "vslh", "vsrh", "vslw", "vsrw", "vsrah", "vsraw",
    "vaddshs", "vsubshs", "vand", "vor", "vxor", "vandc", "vnor",
    "vmrghb", "vmrglb", "vmrghh", "vmrglh", "vmrghw", "vmrglw"))


def emit_insn(va, mn, op_str, labels, func_lo, func_hi, raw=0, jump_tables=None):
    """Return a list of C++ statement strings for one instruction.

    `raw` is the 32-bit big-endian instruction word (some ported builders decode
    simplified forms straight out of it). `jump_tables` maps a bctr's address to
    (index_reg, [targets]) so it emits a real switch instead of an indirect call.
    """
    ops = _split(op_str)

    # Normalize the implicit-cr0 the capstone disassembler drops on compares
    # ("cmplwi r10,2" -> "cmplwi cr0,r10,2") so the ported builders — which
    # expect RexGlue's explicit [crD, rA, rB|imm] field order — index correctly.
    if mn.startswith("cmp") and ops and _cr(ops[0]) is None:
        ops = ["cr0"] + ops

    # ---- atomics: load-reserve / store-conditional --------------------
    # MUST come before the record-form ('.') handling: stwcx./stdcx. set CR0 as
    # part of their own semantics, and if these aren't implemented every guest
    # spinlock / refcount / atomic-increment loop spins forever. Matches the
    # rex/Xenia pattern: reservation value kept in ctx.reserved, store is a CAS.
    _amn = mn.rstrip(".")
    if _amn in ("lwarx", "ldarx", "stwcx", "stdcx"):
        a, b, d = _reg(ops[1]), _reg(ops[2]), _reg(ops[0])
        ea = _rv(b, "u32") if not a else f"ctx.r{a}.u32 + {_rv(b, 'u32')}"
        if _amn == "lwarx":
            return [f"{{ uint32_t _ea = {ea}; ctx.reserved.u32 = *(volatile uint32_t*)(base + _ea); "
                    f"ctx.r{d}.u64 = __builtin_bswap32(ctx.reserved.u32); }}"]
        if _amn == "ldarx":
            return [f"{{ uint32_t _ea = {ea}; ctx.reserved.u64 = *(volatile uint64_t*)(base + _ea); "
                    f"ctx.r{d}.u64 = __builtin_bswap64(ctx.reserved.u64); }}"]
        if _amn == "stwcx":
            return [f"{{ uint32_t _ea = {ea}; ctx.cr0.lt = ctx.cr0.gt = 0; ctx.cr0.so = ctx.xer.so; "
                    f"ctx.cr0.eq = __sync_bool_compare_and_swap((uint32_t*)(base + _ea), "
                    f"ctx.reserved.u32, __builtin_bswap32(ctx.r{d}.u32)); }}"]
        # stdcx.
        return [f"{{ uint32_t _ea = {ea}; ctx.cr0.lt = ctx.cr0.gt = 0; ctx.cr0.so = ctx.xer.so; "
                f"ctx.cr0.eq = __sync_bool_compare_and_swap((uint64_t*)(base + _ea), "
                f"ctx.reserved.u64, __builtin_bswap64(ctx.r{d}.u64)); }}"]

    # ---- generic integer record form (Rc bit / trailing '.') ----------------
    # Handled BEFORE the builder chain: strip the '.', emit the base op through
    # the normal path, then append the CR0 compare exactly once. Kept ahead of
    # the ports because the arithmetic port defensively strips '.' itself (and
    # would otherwise return a base op with no CR0 update, silently dropping the
    # record semantics). FP (CR1) and vector (CR6) manage their own condition
    # field inside their ports, so their dotted forms are NOT intercepted here.
    if (mn.endswith(".") and len(mn) > 1
            and not mn.startswith("f") and not mn.startswith("v")):
        base = emit_insn(va, mn[:-1], op_str, labels, func_lo, func_hi, raw, jump_tables)
        d = _reg(ops[0]) if ops else None
        # Don't double-set CR0: some ported builders (andi./andis.) already emit
        # their own cr0 update. Only append when the base didn't touch cr0.
        already_cr0 = any("cr0" in s for s in base)
        if (d is not None and not already_cr0
                and not any("UNIMPLEMENTED" in s for s in base)):
            base.append(f"ctx.cr0.compare<int32_t>(ctx.r{d}.s32, 0, ctx.xer);")
        return base

    # ---- FULL RexGlue builder chain (faithful ports of the C++ emitters) -----
    # The complete instruction set: scalar FP, integer arith+compare, logical/
    # rotate/shift, SPR/trap/CR-logical, loads/stores/atomics, and VMX/VMX128.
    # Each module's emit() returns a C++ stmt list or None. Builders that emit
    # their own condition update (vcmp*. via CR6, the FP compares) are reached
    # here directly; base integer ops arrive dotless (the record wrapper above
    # already stripped and will append CR0).
    for _pm in _PORT_MODULES:
        try:
            _r = _pm.emit(mn, ops, _reg, _vreg, _freg, _imm, _cr,
                          _mem, _rv, _base_expr, _addr, va, raw)
        except Exception:
            # Our capstone disassembler emits some operands in a different shape
            # than RexGlue's field decoder (implicit cr0 on compares, symbolic
            # CR bits like "cr1eq"). If a builder mis-indexes on that, don't
            # crash the whole recompile — fall through to the next module and
            # ultimately to the format-tuned hand-written handlers below.
            _r = None
        if _r is not None:
            return _r

    # ---- floating point ----------------------------------------------
    if mn == "fmr":
        return [f"ctx.f{_freg(ops[0])}.f64 = ctx.f{_freg(ops[1])}.f64;"]
    _FADD = {"fadds": ("float", "+"), "fsubs": ("float", "-"),
             "fmuls": ("float", "*"), "fdivs": ("float", "/"),
             "fadd": ("double", "+"), "fsub": ("double", "-"),
             "fmul": ("double", "*"), "fdiv": ("double", "/")}
    if mn in _FADD:
        ty, opc = _FADD[mn]
        a, b = _freg(ops[1]), _freg(ops[2])
        return [f"ctx.f{_freg(ops[0])}.f64 = ({ty})(ctx.f{a}.f64 {opc} ctx.f{b}.f64);"]
    if mn in ("fcmpu", "fcmpo"):
        cr = _cr(ops[0]) if _cr(ops[0]) is not None else 0
        return [f"ctx.cr{cr}.compare<double>(ctx.f{_freg(ops[1])}.f64, ctx.f{_freg(ops[2])}.f64, ctx.xer);"]
    if mn in ("lfs", "lfsu", "lfd", "lfdu"):
        mref = _mem(ops[1])
        if mref:
            disp, base = mref
            fd = _freg(ops[0])
            if mn.startswith("lfs"):
                return [f"{{ uint32_t _t = PPC_LOAD_U32({_addr(disp, base)}); float _f; "
                        f"__builtin_memcpy(&_f, &_t, 4); ctx.f{fd}.f64 = _f; }}"]
            return [f"{{ uint64_t _t = PPC_LOAD_U64({_addr(disp, base)}); double _d; "
                    f"__builtin_memcpy(&_d, &_t, 8); ctx.f{fd}.f64 = _d; }}"]
    if mn in ("stfs", "stfsu", "stfd", "stfdu"):
        mref = _mem(ops[1])
        if mref:
            disp, base = mref
            fs = _freg(ops[0])
            if mn.startswith("stfs"):
                return [f"{{ float _f = (float)ctx.f{fs}.f64; uint32_t _t; "
                        f"__builtin_memcpy(&_t, &_f, 4); PPC_STORE_U32({_addr(disp, base)}, _t); }}"]
            return [f"{{ double _d = ctx.f{fs}.f64; uint64_t _t; "
                    f"__builtin_memcpy(&_t, &_d, 8); PPC_STORE_U64({_addr(disp, base)}, _t); }}"]

    # ---- a few more integer ops --------------------------------------
    if mn == "srawi":
        return [f"ctx.r{_reg(ops[0])}.s64 = (int32_t)({_rv(_reg(ops[1]), 's32')} >> {_imm(ops[2])});"]
    if mn == "divw":
        return [f"ctx.r{_reg(ops[0])}.s64 = (int32_t)({_rv(_reg(ops[1]), 's32')} / {_rv(_reg(ops[2]), 's32')});"]
    if mn == "divwu":
        return [f"ctx.r{_reg(ops[0])}.u64 = (uint32_t)({_rv(_reg(ops[1]), 'u32')} / {_rv(_reg(ops[2]), 'u32')});"]
    if mn == "andi":
        return [f"{_rv(_reg(ops[0]))} = {_rv(_reg(ops[1]))} & {_imm(ops[2]) & 0xFFFF}u;"]
    if mn in ("tw", "twi", "twui", "trap", "twlgei", "twllei", "tweqi"):
        return ["/* tw (conditional trap) - elided in PoC */"]

    # ---- sign extension -----------------------------------------------
    if mn == "extsb":
        return [f"ctx.r{_reg(ops[0])}.s64 = (int8_t){_rv(_reg(ops[1]), 'u8')};"]
    if mn == "extsh":
        return [f"ctx.r{_reg(ops[0])}.s64 = (int16_t){_rv(_reg(ops[1]), 'u16')};"]
    if mn == "extsw":
        return [f"ctx.r{_reg(ops[0])}.s64 = (int32_t){_rv(_reg(ops[1]), 'u32')};"]
    if mn == "cntlzw":
        s = _rv(_reg(ops[1]), 'u32')
        return [f"ctx.r{_reg(ops[0])}.u64 = {s} ? (uint32_t)__builtin_clz({s}) : 32;"]
    if mn == "cntlzd":
        s = _rv(_reg(ops[1]), 'u64')
        return [f"ctx.r{_reg(ops[0])}.u64 = {s} ? (uint32_t)__builtin_clzll({s}) : 64;"]

    # ---- rotate-and-mask (general forms) ------------------------------
    if mn == "rlwinm":
        d, s, sh, mb, me = _reg(ops[0]), _reg(ops[1]), _imm(ops[2]), _imm(ops[3]), _imm(ops[4])
        return [f"ctx.r{d}.u64 = ppc_rlwinm({_rv(s, 'u32')}, {sh}, {mb}, {me});"]
    if mn == "rlwimi":
        d, s, sh, mb, me = _reg(ops[0]), _reg(ops[1]), _imm(ops[2]), _imm(ops[3]), _imm(ops[4])
        return [f"ctx.r{d}.u64 = ppc_rlwimi({_rv(d, 'u32')}, {_rv(s, 'u32')}, {sh}, {mb}, {me});"]

    # ---- 64-bit shifts / clears ---------------------------------------
    if mn == "clrldi":
        return [f"ctx.r{_reg(ops[0])}.u64 = {_rv(_reg(ops[1]), 'u64')} & (~0ull >> {_imm(ops[2])});"]
    if mn in ("sldi",):
        return [f"ctx.r{_reg(ops[0])}.u64 = {_rv(_reg(ops[1]), 'u64')} << {_imm(ops[2])};"]
    if mn in ("srdi",):
        return [f"ctx.r{_reg(ops[0])}.u64 = {_rv(_reg(ops[1]), 'u64')} >> {_imm(ops[2])};"]
    if mn == "sradi":
        return [f"ctx.r{_reg(ops[0])}.s64 = {_rv(_reg(ops[1]), 's64')} >> {_imm(ops[2])};"]
    if mn == "sld":
        d, s, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.u64 = ({_rv(b, 'u32')} & 0x40) ? 0 : ({_rv(s, 'u64')} << ({_rv(b, 'u32')} & 0x3F));"]
    if mn == "srd":
        d, s, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.u64 = ({_rv(b, 'u32')} & 0x40) ? 0 : ({_rv(s, 'u64')} >> ({_rv(b, 'u32')} & 0x3F));"]
    if mn == "slw":
        d, s, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.u64 = ({_rv(b, 'u32')} & 0x20) ? 0 : (uint32_t)({_rv(s, 'u32')} << ({_rv(b, 'u32')} & 0x1F));"]
    if mn == "srw":
        d, s, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.u64 = ({_rv(b, 'u32')} & 0x20) ? 0 : ({_rv(s, 'u32')} >> ({_rv(b, 'u32')} & 0x1F));"]
    if mn == "sraw":
        d, s, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.s64 = (int32_t)({_rv(s, 's32')} >> ({_rv(b, 'u32')} & 0x1F));"]

    # ---- more integer arithmetic --------------------------------------
    if mn == "subfic":
        return [f"ctx.r{_reg(ops[0])}.s64 = (int64_t){_imm(ops[2])} - {_rv(_reg(ops[1]), 's64')};"]
    if mn in ("addic", "subfic_"):
        return [f"ctx.r{_reg(ops[0])}.s64 = {_rv(_reg(ops[1]), 's64')} + {_imm(ops[2])};"]
    if mn == "addc":                              # add carrying (sets XER[CA])
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{{ uint64_t _s = (uint64_t){_rv(a, 'u32')} + {_rv(b, 'u32')}; "
                f"ctx.r{d}.u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }}"]
    if mn == "adde":                              # add extended (uses XER[CA])
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{{ uint64_t _s = (uint64_t){_rv(a, 'u32')} + {_rv(b, 'u32')} + ctx.xer.ca; "
                f"ctx.r{d}.u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }}"]
    if mn == "subfc":
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{{ uint64_t _s = (uint64_t){_rv(b, 'u32')} + (uint32_t)(~{_rv(a, 'u32')}) + 1; "
                f"ctx.r{d}.u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }}"]
    if mn == "subfe":
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{{ uint64_t _s = (uint64_t){_rv(b, 'u32')} + (uint32_t)(~{_rv(a, 'u32')}) + ctx.xer.ca; "
                f"ctx.r{d}.u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }}"]
    if mn == "nand":
        return [f"{_rv(_reg(ops[0]))} = ~({_rv(_reg(ops[1]))} & {_rv(_reg(ops[2]))});"]
    if mn == "andc":
        return [f"{_rv(_reg(ops[0]))} = {_rv(_reg(ops[1]))} & ~{_rv(_reg(ops[2]))};"]
    if mn == "orc":
        return [f"{_rv(_reg(ops[0]))} = {_rv(_reg(ops[1]))} | ~{_rv(_reg(ops[2]))};"]
    if mn == "nor":
        return [f"{_rv(_reg(ops[0]))} = ~({_rv(_reg(ops[1]))} | {_rv(_reg(ops[2]))});"]
    if mn == "eqv":
        return [f"{_rv(_reg(ops[0]))} = ~({_rv(_reg(ops[1]))} ^ {_rv(_reg(ops[2]))});"]
    if mn == "mulld":
        return [f"ctx.r{_reg(ops[0])}.s64 = {_rv(_reg(ops[1]), 's64')} * {_rv(_reg(ops[2]), 's64')};"]
    if mn == "divd":
        return [f"ctx.r{_reg(ops[0])}.s64 = {_rv(_reg(ops[1]), 's64')} / {_rv(_reg(ops[2]), 's64')};"]
    if mn == "divdu":
        return [f"ctx.r{_reg(ops[0])}.u64 = {_rv(_reg(ops[1]), 'u64')} / {_rv(_reg(ops[2]), 'u64')};"]

    # ---- more floating point ------------------------------------------
    if mn == "fneg":
        return [f"ctx.f{_freg(ops[0])}.f64 = -ctx.f{_freg(ops[1])}.f64;"]
    if mn == "fabs":
        return [f"ctx.f{_freg(ops[0])}.f64 = __builtin_fabs(ctx.f{_freg(ops[1])}.f64);"]
    if mn == "fnabs":
        return [f"ctx.f{_freg(ops[0])}.f64 = -__builtin_fabs(ctx.f{_freg(ops[1])}.f64);"]
    if mn == "frsp":
        return [f"ctx.f{_freg(ops[0])}.f64 = (float)ctx.f{_freg(ops[1])}.f64;"]
    if mn == "fcfid":
        return [f"ctx.f{_freg(ops[0])}.f64 = (double)(int64_t)ctx.f{_freg(ops[1])}.u64;"]
    if mn in ("fctiwz", "fctiw"):
        return [f"ctx.f{_freg(ops[0])}.u64 = (uint32_t)(int32_t)ctx.f{_freg(ops[1])}.f64;"]
    if mn in ("fctidz", "fctid"):
        return [f"ctx.f{_freg(ops[0])}.u64 = (uint64_t)(int64_t)ctx.f{_freg(ops[1])}.f64;"]
    if mn == "fsqrts":
        return [f"ctx.f{_freg(ops[0])}.f64 = (float)__builtin_sqrt(ctx.f{_freg(ops[1])}.f64);"]
    if mn == "fsqrt":
        return [f"ctx.f{_freg(ops[0])}.f64 = __builtin_sqrt(ctx.f{_freg(ops[1])}.f64);"]
    if mn == "fres":
        return [f"ctx.f{_freg(ops[0])}.f64 = (float)(1.0 / ctx.f{_freg(ops[1])}.f64);"]
    _FMA = {"fmadds": ("(float)", "+"), "fmsubs": ("(float)", "-"),
            "fnmadds": ("-(float)", "+"), "fnmsubs": ("-(float)", "-"),
            "fmadd": ("", "+"), "fmsub": ("", "-"),
            "fnmadd": ("-", "+"), "fnmsub": ("-", "-")}
    if mn in _FMA:
        pre, opc = _FMA[mn]
        a, c, b = _freg(ops[1]), _freg(ops[2]), _freg(ops[3])
        return [f"ctx.f{_freg(ops[0])}.f64 = {pre}(ctx.f{a}.f64 * ctx.f{c}.f64 {opc} ctx.f{b}.f64);"]
    if mn == "stfiwx":
        return [f"PPC_STORE_U32({_rv(_reg(ops[1]), 'u32')} + {_rv(_reg(ops[2]), 'u32')}, (uint32_t)ctx.f{_freg(ops[0])}.u64);"]

    # ---- cache / sync hints -> no-ops (correct: they are hints/barriers) --
    if mn in ("dcbt", "dcbtst", "dcbst", "dcbf", "dcbi", "icbi", "sync", "isync",
              "eieio", "lwsync", "ptesync", "tlbsync", "dcbz", "dcbz128", "dcbzl"):
        return [f"/* {mn} (cache/sync hint) */"]
    # supervisor MSR ops: we don't model MSR — no-op the writes, read as 0.
    if mn in ("mtmsr", "mtmsrd", "mtmsree"):
        return [f"/* {mn} (MSR write, not modeled) */"]
    if mn == "mfmsr":
        return [f"{_rv(_reg(ops[0]))} = 0;"]

    # ---- AltiVec vectors (element-wise; helpers in the runtime header) -----
    if mn in _VEC3:
        return [f"ppc_{mn}(ctx.v{_vreg(ops[0])}, ctx.v{_vreg(ops[1])}, ctx.v{_vreg(ops[2])});"]
    if mn == "vmr":
        return [f"ppc_vmove(ctx.v{_vreg(ops[0])}, ctx.v{_vreg(ops[1])});"]
    if mn in ("vperm", "vsel"):
        return [f"ppc_{mn}(ctx.v{_vreg(ops[0])}, ctx.v{_vreg(ops[1])}, "
                f"ctx.v{_vreg(ops[2])}, ctx.v{_vreg(ops[3])});"]
    if mn in ("vspltw", "vsplth", "vspltb"):
        return [f"ppc_{mn}(ctx.v{_vreg(ops[0])}, ctx.v{_vreg(ops[1])}, {_imm(ops[2])});"]
    if mn in ("vspltisw", "vspltish", "vspltisb"):
        d, imm = _vreg(ops[0]), _imm(ops[1])
        n, fld = {"vspltisw": (4, "s32"), "vspltish": (8, "s16"),
                  "vspltisb": (16, "s8")}[mn]
        return [f"for (int _i=0;_i<{n};_i++) ctx.v{d}.{fld}[_i] = {imm};"]
    if mn in ("lvx", "lvxl", "lvx128"):
        a, b = _reg(ops[1]), _reg(ops[2])
        return [f"{{ uint32_t _ea = ({_base_expr(a)} + {_rv(b, 'u32')}) & ~0xFu; "
                f"for (int _i=0;_i<16;_i++) ctx.v{_vreg(ops[0])}.u8[_i] = PPC_LOAD_U8(_ea+_i); }}"]
    if mn in ("stvx", "stvxl", "stvx128"):
        a, b = _reg(ops[1]), _reg(ops[2])
        return [f"{{ uint32_t _ea = ({_base_expr(a)} + {_rv(b, 'u32')}) & ~0xFu; "
                f"for (int _i=0;_i<16;_i++) PPC_STORE_U8(_ea+_i, ctx.v{_vreg(ops[0])}.u8[_i]); }}"]

    def target():
        toks = re.findall(r"0x[0-9A-Fa-f]+", op_str)
        return int(toks[-1], 16) if toks else None

    def call_or_goto(tgt, conditional_pred=None):
        # in-function -> goto label; out-of-function -> tail-call another function
        if func_lo <= tgt < func_hi:
            stmt = f"goto loc_{tgt:08X};"
        else:
            stmt = f"sub_{tgt:08X}(ctx, base); return;"
        if conditional_pred:
            return [f"if ({conditional_pred}) {{ {stmt} }}"]
        return [stmt]

    # ---- moves / SPRs -------------------------------------------------
    if mn == "mflr":
        return [f"{_rv(_reg(ops[0]))} = ctx.lr;"]
    if mn == "mtlr":
        return [f"ctx.lr = {_rv(_reg(ops[0]))};"]
    if mn == "mfctr":
        return [f"{_rv(_reg(ops[0]))} = ctx.ctr.u64;"]
    if mn == "mtctr":
        return [f"ctx.ctr.u64 = {_rv(_reg(ops[0]))};"]
    if mn == "mr":
        return [f"{_rv(_reg(ops[0]))} = {_rv(_reg(ops[1]))};"]
    if mn == "nop":
        return ["/* nop */"]

    # ---- immediates / address building --------------------------------
    if mn == "li":
        return [f"ctx.r{_reg(ops[0])}.s64 = {_imm(ops[1])};"]
    if mn == "lis":
        return [f"ctx.r{_reg(ops[0])}.s64 = (int32_t)0x{(_imm(ops[1]) << 16) & 0xFFFFFFFF:08X};"]

    # ---- ALU ----------------------------------------------------------
    if mn in ("add", "addc"):
        return [f"{_rv(_reg(ops[0]))} = {_rv(_reg(ops[1]))} + {_rv(_reg(ops[2]))};"]
    if mn in ("addi",):
        return [f"ctx.r{_reg(ops[0])}.s64 = {_rv(_reg(ops[1]), 's64')} + {_imm(ops[2])};"]
    if mn in ("addis",):
        return [f"ctx.r{_reg(ops[0])}.s64 = {_rv(_reg(ops[1]), 's64')} + (int32_t)0x{(_imm(ops[2]) << 16) & 0xFFFFFFFF:08X};"]
    if mn in ("subf",):    # subf rD,rA,rB = rB - rA
        return [f"{_rv(_reg(ops[0]))} = {_rv(_reg(ops[2]))} - {_rv(_reg(ops[1]))};"]
    if mn in ("sub",):     # extended: sub rD,rB,rA
        return [f"{_rv(_reg(ops[0]))} = {_rv(_reg(ops[1]))} - {_rv(_reg(ops[2]))};"]
    if mn == "neg":
        return [f"ctx.r{_reg(ops[0])}.s64 = -{_rv(_reg(ops[1]), 's64')};"]
    if mn == "mullw":
        return [f"ctx.r{_reg(ops[0])}.s64 = (int32_t)({_rv(_reg(ops[1]), 's32')} * {_rv(_reg(ops[2]), 's32')});"]
    if mn == "mulli":
        return [f"ctx.r{_reg(ops[0])}.s64 = (int32_t)({_rv(_reg(ops[1]), 's32')} * {_imm(ops[2])});"]
    if mn == "mulhw":
        return [f"ctx.r{_reg(ops[0])}.s64 = (int32_t)(((int64_t){_rv(_reg(ops[1]), 's32')} * (int64_t){_rv(_reg(ops[2]), 's32')}) >> 32);"]
    if mn == "mulhwu":
        return [f"ctx.r{_reg(ops[0])}.u64 = (uint32_t)(((uint64_t){_rv(_reg(ops[1]), 'u32')} * (uint64_t){_rv(_reg(ops[2]), 'u32')}) >> 32);"]

    # ---- logical ------------------------------------------------------
    if mn == "or":
        return [f"{_rv(_reg(ops[0]))} = {_rv(_reg(ops[1]))} | {_rv(_reg(ops[2]))};"]
    if mn == "and":
        return [f"{_rv(_reg(ops[0]))} = {_rv(_reg(ops[1]))} & {_rv(_reg(ops[2]))};"]
    if mn == "xor":
        return [f"{_rv(_reg(ops[0]))} = {_rv(_reg(ops[1]))} ^ {_rv(_reg(ops[2]))};"]
    if mn == "ori":
        return [f"{_rv(_reg(ops[0]))} = {_rv(_reg(ops[1]))} | {_imm(ops[2]) & 0xFFFF}u;"]
    if mn == "oris":
        return [f"{_rv(_reg(ops[0]))} = {_rv(_reg(ops[1]))} | 0x{(_imm(ops[2]) << 16) & 0xFFFFFFFF:08X}u;"]
    if mn in ("slwi",):
        return [f"ctx.r{_reg(ops[0])}.u64 = (uint32_t)({_rv(_reg(ops[1]), 'u32')} << {_imm(ops[2])});"]
    if mn in ("srwi",):
        return [f"ctx.r{_reg(ops[0])}.u64 = {_rv(_reg(ops[1]), 'u32')} >> {_imm(ops[2])};"]
    if mn in ("rotlwi",):
        return [f"ctx.r{_reg(ops[0])}.u64 = ppc_rotl32({_rv(_reg(ops[1]), 'u32')}, {_imm(ops[2])});"]
    if mn in ("clrlwi",):
        return [f"ctx.r{_reg(ops[0])}.u64 = {_rv(_reg(ops[1]), 'u32')} & (0xFFFFFFFFu >> {_imm(ops[2])});"]

    # ---- loads --------------------------------------------------------
    _LD = {"lbz": "U8", "lhz": "U16", "lha": "U16", "lwz": "U32", "ld": "U64",
           "lwzu": "U32", "lbzu": "U8", "lhzu": "U16"}
    if mn in _LD:
        mref = _mem(ops[1])
        if mref is None:
            return [f'PPC_UNIMPLEMENTED("{mn} {op_str}");']
        disp, base = mref
        w = _LD[mn]
        out = [f"ctx.r{_reg(ops[0])}.u64 = PPC_LOAD_{w}({_addr(disp, base)});"]
        if mn.endswith("u") and base:                 # update form
            out.append(f"ctx.r{base}.u32 = {_addr(disp, base)};")
        return out
    if mn in ("lwzx", "lbzx", "lhzx", "ldx"):
        w = {"lwzx": "U32", "lbzx": "U8", "lhzx": "U16", "ldx": "U64"}[mn]
        return [f"ctx.r{_reg(ops[0])}.u64 = PPC_LOAD_{w}({_rv(_reg(ops[1]), 'u32')} + {_rv(_reg(ops[2]), 'u32')});"]

    # ---- stores -------------------------------------------------------
    _ST = {"stb": "U8", "sth": "U16", "stw": "U32", "std": "U64",
           "stwu": "U32", "stbu": "U8", "sthu": "U16"}
    if mn in _ST:
        mref = _mem(ops[1])
        if mref is None:
            return [f'PPC_UNIMPLEMENTED("{mn} {op_str}");']
        disp, base = mref
        w = _ST[mn]
        rs = _reg(ops[0])
        if mn.endswith("u") and base:                 # store-with-update
            return [f"ea = {_addr(disp, base)};",
                    f"PPC_STORE_{w}(ea, ctx.r{rs}.u32);",
                    f"ctx.r{base}.u32 = ea;"]
        return [f"PPC_STORE_{w}({_addr(disp, base)}, ctx.r{rs}.u32);"]
    if mn in ("stwx", "stbx", "sthx", "stdx"):
        w = {"stwx": "U32", "stbx": "U8", "sthx": "U16", "stdx": "U64"}[mn]
        return [f"PPC_STORE_{w}({_rv(_reg(ops[1]), 'u32')} + {_rv(_reg(ops[2]), 'u32')}, ctx.r{_reg(ops[0])}.u32);"]

    # ---- compares -----------------------------------------------------
    if mn in ("cmpw", "cmpwi", "cmplw", "cmplwi", "cmpd", "cmpdi", "cmpld", "cmpldi"):
        signed = not mn.startswith("cmpl")
        wide = mn.startswith("cmpd") or mn.startswith("cmpld")
        ty = ("int64_t" if wide else "int32_t") if signed else ("uint64_t" if wide else "uint32_t")
        fld = ("s64" if wide else "s32") if signed else ("u64" if wide else "u32")
        # operands may lead with crN or default to cr0
        idx = 0
        cr = 0
        if ops and _cr(ops[0]) is not None:
            cr = _cr(ops[0]); idx = 1
        a = _rv(_reg(ops[idx]), fld)
        b_tok = ops[idx + 1]
        b = _rv(_reg(b_tok), fld) if _reg(b_tok) is not None else str(_imm(b_tok))
        return [f"ctx.cr{cr}.compare<{ty}>({a}, {b}, ctx.xer);"]

    # ---- branches -----------------------------------------------------
    if mn == "blr":
        return ["return;"]
    if mn in ("bctr",):
        jt = (jump_tables or {}).get(va)
        if jt:
            # Recovered switch table (RexGlue function_scanner): emit a real
            # switch on the index register instead of shattering control flow
            # through an indirect call. Local cases goto the block label;
            # out-of-function cases tail-call. Unknown index -> the honest
            # indirect call (the table's default/out-of-range path).
            idx_reg, targets = jt
            out = [f"switch (ctx.r{idx_reg}.u32) {{"]
            for i, t in enumerate(targets):
                if func_lo <= t < func_hi:
                    out.append(f"case {i}u: goto loc_{t:08X};")
                else:
                    out.append(f"case {i}u: sub_{t:08X}(ctx, base); return;")
            out.append("default: PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32); return;")
            out.append("}")
            return out
        return ["PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32); return;"]
    if mn in ("bctrl",):
        return [f"ctx.lr = 0x{va + WORD:08X};",
                "PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);"]
    if mn in ("b", "ba"):
        return call_or_goto(target())
    if mn in ("bl", "bla"):
        tgt = target()
        return [f"ctx.lr = 0x{va + WORD:08X};", f"sub_{tgt:08X}(ctx, base);"]
    base_mn = mn.rstrip("+-")            # strip branch-hint suffix
    if base_mn in _COND:
        cr = 0
        if ops and _cr(ops[0]) is not None:
            cr = _cr(ops[0])
        pred = _COND[base_mn].format(cr=f"ctx.cr{cr}")
        return call_or_goto(target(), conditional_pred=pred)
    if base_mn in ("beqlr", "bnelr", "bltlr", "bgelr", "bgtlr", "blelr"):
        m = {"beqlr": "eq", "bnelr": "!eq", "bltlr": "lt", "bgelr": "!lt",
             "bgtlr": "gt", "blelr": "!gt"}[base_mn]
        cr = _cr(ops[0]) if ops and _cr(ops[0]) is not None else 0
        pred = (f"!ctx.cr{cr}.{m[1:]}" if m.startswith("!") else f"ctx.cr{cr}.{m}")
        return [f"if ({pred}) return;"]

    # ---- CTR-decrement branches (bdnz/bdz) ----------------------------
    if base_mn in ("bdnz", "bdz"):
        pred = "ctx.ctr.u64 != 0" if base_mn == "bdnz" else "ctx.ctr.u64 == 0"
        return ["ctx.ctr.u64 -= 1;"] + call_or_goto(target(), conditional_pred=pred)

    # ---- integer + carry (extend/negate carrying) ---------------------
    if mn == "addze":                             # rD = rA + XER[CA]
        d, a = _reg(ops[0]), _reg(ops[1])
        return [f"{{ uint64_t _s = (uint64_t){_rv(a, 'u32')} + ctx.xer.ca; "
                f"ctx.r{d}.u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }}"]
    if mn == "addme":                             # rD = rA + XER[CA] - 1
        d, a = _reg(ops[0]), _reg(ops[1])
        return [f"{{ uint64_t _s = (uint64_t){_rv(a, 'u32')} + ctx.xer.ca + 0xFFFFFFFFull; "
                f"ctx.r{d}.u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }}"]
    if mn == "subfze":                            # rD = ~rA + XER[CA]
        d, a = _reg(ops[0]), _reg(ops[1])
        return [f"{{ uint64_t _s = (uint64_t)(uint32_t)(~{_rv(a, 'u32')}) + ctx.xer.ca; "
                f"ctx.r{d}.u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }}"]
    if mn == "subfme":                            # rD = ~rA + XER[CA] - 1
        d, a = _reg(ops[0]), _reg(ops[1])
        return [f"{{ uint64_t _s = (uint64_t)(uint32_t)(~{_rv(a, 'u32')}) + ctx.xer.ca + 0xFFFFFFFFull; "
                f"ctx.r{d}.u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }}"]

    # ---- logical immediates -------------------------------------------
    if mn == "xori":
        return [f"{_rv(_reg(ops[0]))} = {_rv(_reg(ops[1]))} ^ {_imm(ops[2]) & 0xFFFF}u;"]
    if mn == "xoris":
        return [f"{_rv(_reg(ops[0]))} = {_rv(_reg(ops[1]))} ^ 0x{(_imm(ops[2]) << 16) & 0xFFFFFFFF:08X}u;"]
    if mn == "andis":
        return [f"ctx.r{_reg(ops[0])}.u64 = {_rv(_reg(ops[1]), 'u64')} & 0x{(_imm(ops[2]) << 16) & 0xFFFFFFFF:08X}ull;"]

    # ---- 64-bit rotate + mask (rldic family) --------------------------
    if mn in ("rldicl", "rldicr", "rldic", "rldimi"):
        d, s, sh = _reg(ops[0]), _reg(ops[1]), _imm(ops[2])
        n = _imm(ops[3])
        rot = f"ppc_rotl64({_rv(s, 'u64')}, {sh})"
        if mn == "rldicl":                        # clear left: keep bits [n..63]
            return [f"ctx.r{d}.u64 = {rot} & (~0ull >> {n});"]
        if mn == "rldicr":                        # clear right: keep bits [0..n]
            return [f"ctx.r{d}.u64 = {rot} & (~0ull << (63 - {n}));"]
        if mn == "rldic":                         # mask [n .. 63-sh]
            return [f"ctx.r{d}.u64 = {rot} & ((~0ull >> {n}) & (~0ull << {sh}));"]
        # rldimi: insert rot into rD under mask [n .. 63-sh]
        return [f"{{ uint64_t _m = (~0ull >> {n}) & (~0ull << {sh}); "
                f"ctx.r{d}.u64 = ({rot} & _m) | (ctx.r{d}.u64 & ~_m); }}"]

    # ---- load word algebraic (sign-extend 32->64) ---------------------
    if mn == "lwa":
        mref = _mem(ops[1])
        if mref:
            disp, base = mref
            return [f"ctx.r{_reg(ops[0])}.s64 = (int32_t)PPC_LOAD_U32({_addr(disp, base)});"]
    if mn in ("lwax", "lwaux"):
        a, b = _reg(ops[1]), _reg(ops[2])
        out = [f"ctx.r{_reg(ops[0])}.s64 = (int32_t)PPC_LOAD_U32({_base_expr(a)} + {_rv(b, 'u32')});"]
        if mn == "lwaux" and a:
            out.append(f"ctx.r{a}.u32 = {_base_expr(a)} + {_rv(b, 'u32')};")
        return out

    # ---- float indexed / update loads & stores ------------------------
    if mn in ("lfsx", "lfsux", "lfdx", "lfdux"):
        a, b = _reg(ops[1]), _reg(ops[2])
        ea = f"{_base_expr(a)} + {_rv(b, 'u32')}"
        fd = _freg(ops[0])
        if mn.startswith("lfs"):
            out = [f"{{ uint32_t _t = PPC_LOAD_U32({ea}); float _f; __builtin_memcpy(&_f,&_t,4); ctx.f{fd}.f64 = _f; }}"]
        else:
            out = [f"{{ uint64_t _t = PPC_LOAD_U64({ea}); double _d; __builtin_memcpy(&_d,&_t,8); ctx.f{fd}.f64 = _d; }}"]
        if mn.endswith("ux") and a:
            out.append(f"ctx.r{a}.u32 = {ea};")
        return out
    if mn in ("stfsx", "stfsux", "stfdx", "stfdux"):
        a, b = _reg(ops[1]), _reg(ops[2])
        ea = f"{_base_expr(a)} + {_rv(b, 'u32')}"
        fs = _freg(ops[0])
        if mn.startswith("stfs"):
            out = [f"{{ float _f = (float)ctx.f{fs}.f64; uint32_t _t; __builtin_memcpy(&_t,&_f,4); PPC_STORE_U32({ea}, _t); }}"]
        else:
            out = [f"{{ double _d = ctx.f{fs}.f64; uint64_t _t; __builtin_memcpy(&_t,&_d,8); PPC_STORE_U64({ea}, _t); }}"]
        if mn.endswith("ux") and a:
            out.append(f"ctx.r{a}.u32 = {ea};")
        return out

    # ---- not translated yet — honest trap (never a silent wrong result) --
    return [f'PPC_UNIMPLEMENTED("{mn} {op_str}");']


# ---------------------------------------------------------------------------
# whole-function emission
# ---------------------------------------------------------------------------
def recompile_function(body, base_va, func_va, size,
                       img=None, img_base=0, is_exec=None):
    """Emit the C++ for one guest function. Returns (cpp_text, called, stats).

    When `img` (the whole loaded image) is supplied, `bctr` switch tables are
    recovered from it (RexGlue function_scanner) so indirect jumps become real
    C++ switches; `is_exec(addr)` validates candidate table targets.
    """
    n = size - (size % WORD)
    func_lo, func_hi = func_va, func_va + n
    insns = list(disasm_ppc.listing(body, func_va, count=n // WORD))

    # pass 1: collect intra-function branch targets -> labels
    labels = set()
    for va, raw, mn, op_str in insns:
        if mn.startswith("b"):
            toks = re.findall(r"0x[0-9A-Fa-f]+", op_str)
            if toks:
                t = int(toks[-1], 16)
                if func_lo <= t < func_hi:
                    labels.add(t)

    # pass 1b: recover bctr jump tables from the image (RexGlue function_scanner).
    # Any in-function case target becomes a block label so the switch can goto it.
    jump_tables = {}
    if img is not None:
        def _rd(a, w):
            o = a - img_base
            return int.from_bytes(img[o:o + w], "big") if 0 <= o and o + w <= len(img) else None
        _isx = is_exec or (lambda a: True)
        try:
            jump_tables = jumptable.detect_jump_tables(
                insns, func_lo, func_hi,
                lambda a: _rd(a, 1), lambda a: _rd(a, 2), lambda a: _rd(a, 4), _isx)
        except Exception:
            jump_tables = {}
        for _ir, _tgts in jump_tables.values():
            for _t in _tgts:
                if func_lo <= _t < func_hi:
                    labels.add(_t)

    # pass 2: emit
    called = set()
    lines = []
    # The reference emits a weak `__imp__` alias here for host-override; that uses
    # __attribute__((alias)) (an ELF feature). We keep the model in the macro
    # (PPC_FUNC_IMPL) so the generated body is portable and compiles as-is.
    lines.append(f"PPC_FUNC_IMPL(sub_{func_va:08X}) {{")
    lines.append("\tPPC_FUNC_PROLOGUE();")
    lines.append("\tuint32_t ea{};")
    lines.append("\tPPCRegister temp{};")   # scratch for carry/rotate builder sequences
    total = unimpl = 0
    for va, raw, mn, op_str in insns:
        total += 1
        if va in labels:
            lines.append(f"loc_{va:08X}:")
        lines.append(f"\t// {mn} {op_str}".rstrip())
        stmts = emit_insn(va, mn, op_str, labels, func_lo, func_hi, raw, jump_tables)
        # safety net: a handler that failed to parse an operand emits a stray
        # "None" (e.g. ctx.rNone). Never ship broken C++ — fall back to an honest
        # trap so the byte stays accounted for and the file still compiles.
        if any("None" in s for s in stmts):
            stmts = [f'PPC_UNIMPLEMENTED("{mn} {op_str}");']
        for stmt in stmts:
            lines.append(f"\t{stmt}")
            if "PPC_UNIMPLEMENTED" in stmt:
                unimpl += 1
            m = re.search(r"sub_([0-9A-F]{8})\(ctx", stmt)
            if m:
                called.add(int(m.group(1), 16))
    lines.append("}")
    stats = {"total": total, "unimpl": unimpl,
             "translated": total - unimpl, "labels": len(labels)}
    return "\n".join(lines) + "\n", called, stats


# ---------------------------------------------------------------------------
# the minimal runtime header (our own rex/ppc.h, enough to compile the output)
# ---------------------------------------------------------------------------
def runtime_header():
    regs = "\n".join(f"    PPCRegister r{i};" for i in range(32))
    fprs = "\n".join(f"    PPCRegister f{i};" for i in range(32))
    vrs = "\n".join(f"    PPCVRegister v{i};" for i in range(128))
    crs = "\n".join(f"    PPCCRRegister cr{i};" for i in range(8))
    loads = "\n".join(
        f"#define PPC_LOAD_{w}(x)  ({sw}(*(volatile {t}*)(base + (uint32_t)(x))))"
        for w, t, sw in (("U8", "uint8_t", ""), ("U16", "uint16_t", "__builtin_bswap16"),
                         ("U32", "uint32_t", "__builtin_bswap32"),
                         ("U64", "uint64_t", "__builtin_bswap64")))
    stores = "\n".join(
        f"#define PPC_STORE_{w}(x,y)  (*(volatile {t}*)(base + (uint32_t)(x)) = ({sw}((({t})(y)))))"
        for w, t, sw in (("U8", "uint8_t", ""), ("U16", "uint16_t", "__builtin_bswap16"),
                         ("U32", "uint32_t", "__builtin_bswap32"),
                         ("U64", "uint64_t", "__builtin_bswap64")))
    # Documentation of the runtime symbols the ported builders reference (the
    # vector/memory helper prototypes + the whole-vector shuffle tables). In
    # BRIDGE mode these are real definitions from <rex/ppc/context.h|memory.h>;
    # kept here (comment-only) so the standalone contract is self-describing.
    port_helpers = "\n".join(
        m.RUNTIME_HELPERS for m in (port_memory, port_vector)
        if getattr(m, "RUNTIME_HELPERS", "").strip())
    return f"""// rex_ppc_min.h - PowerPC recompilation runtime (XboxTranspiler).
// When the vendored RexGlue kernel is on the include path (<rex/ppc/context.h>),
// this BRIDGES onto its PPCContext so the recompiled code and the kernel share
// one runtime ABI (the register types are byte-compatible - ours were modeled on
// RexGlue's). Otherwise it falls back to our own standalone context that compiles
// alone. Define REX_PPC_STANDALONE to force the standalone path.
#pragma once
#include <cstdint>
#include <cmath>     // ported FP builders (fabs/sqrt/frsp/fsel/…)
#include <climits>   // ported FP convert builders (saturation bounds)

#if __has_include(<rex/ppc/context.h>) && !defined(REX_PPC_STANDALONE)
// ---- BRIDGE: unify onto the vendored RexGlue PPCContext ---------------------
#define REX_PPC_BRIDGE 1
#include <rex/ppc/context.h>       // PPCContext(+kernel_state), reg types, PPC_FUNC, PPCFuncMapping
using PPCVRegister = rex::VRegister;
#else
// ---- STANDALONE: our own minimal context -----------------------------------
union PPCRegister {{
    int8_t s8; uint8_t u8; int16_t s16; uint16_t u16;
    int32_t s32; uint32_t u32; int64_t s64; uint64_t u64; float f32; double f64;
}};
struct PPCXERRegister {{ uint8_t so, ov, ca; }};
struct PPCCRRegister {{
    uint8_t lt, gt, eq, so;
    template <class T> void compare(T a, T b, const PPCXERRegister& xer) {{
        lt = a < b; gt = a > b; eq = a == b; so = xer.so;
    }}
}};
union alignas(16) PPCVRegister {{
    int8_t s8[16]; uint8_t u8[16]; int16_t s16[8]; uint16_t u16[8];
    int32_t s32[4]; uint32_t u32[4]; int64_t s64[2]; uint64_t u64[2];
    float f32[4]; double f64[2];
}};
struct alignas(64) PPCContext {{
{regs}
    uint64_t lr;
    PPCRegister ctr;
    PPCXERRegister xer;
    PPCRegister reserved;   // lwarx/stwcx reservation value (atomics)
{crs}
{fprs}
{vrs}
}};
using PPCFunc = void(PPCContext& ctx, uint8_t* base);
#define PPC_FUNC(x)        void x(PPCContext& __restrict ctx, uint8_t* base)
#define PPC_FUNC_IMPL(x)   PPC_FUNC(x)
#define PPC_WEAK_FUNC(x)   PPC_FUNC(x)
#define PPC_FUNC_PROLOGUE()  do {{ (void)ctx; (void)base; }} while (0)
#define PPC_UNIMPLEMENTED(s) do {{ /* trap: {{s}} not yet translated */ }} while (0)
#endif

// ---- shared helpers (PPCVRegister is defined in both branches) --------------
static inline uint32_t ppc_rotl32(uint32_t v, uint32_t n) {{
    n &= 31; return n ? ((v << n) | (v >> (32 - n))) : v;
}}
static inline uint64_t ppc_rotl64(uint64_t v, uint32_t n) {{
    n &= 63; return n ? ((v << n) | (v >> (64 - n))) : v;
}}
// PowerPC big-endian bit numbering (bit 0 = MSB): mask bits [mb, me] inclusive,
// wrapping when mb > me.
static inline uint32_t ppc_mask32(int mb, int me) {{
    uint32_t begin = 0xFFFFFFFFu >> mb;
    uint32_t end = (me >= 31) ? 0xFFFFFFFFu : (0xFFFFFFFFu << (31 - me));
    return (mb <= me) ? (begin & end) : (begin | end);
}}
static inline uint32_t ppc_rlwinm(uint32_t v, int sh, int mb, int me) {{
    return ppc_rotl32(v, sh) & ppc_mask32(mb, me);
}}
static inline uint32_t ppc_rlwimi(uint32_t cur, uint32_t v, int sh, int mb, int me) {{
    uint32_t m = ppc_mask32(mb, me);
    return (cur & ~m) | (ppc_rotl32(v, sh) & m);
}}
// AltiVec (128-bit) helpers. NOTE: element order follows the register's native
// lanes; full big-endian vector-load correctness is validated on real runs.
static inline void ppc_vmove(PPCVRegister& d, const PPCVRegister& a) {{
    d.u64[0] = a.u64[0]; d.u64[1] = a.u64[1];
}}
{_VEC_HELPERS}

// ---- indirect calls + memory access ----------------------------------------
// In bridge mode, override RexGlue's macros with the forms our generated code
// emits (1-arg PPC_UNIMPLEMENTED; non-extern-C PPC_FUNC_IMPL; our indirect call).
#if defined(REX_PPC_BRIDGE)
#undef PPC_UNIMPLEMENTED
#define PPC_UNIMPLEMENTED(s) do {{ /* {{s}} */ }} while (0)
#undef PPC_FUNC_IMPL
#define PPC_FUNC_IMPL(x) PPC_FUNC(x)
#undef PPC_CALL_INDIRECT_FUNC
#endif
extern "C" void ppc_call_indirect(PPCContext& ctx, uint8_t* base, uint32_t addr);
#define PPC_CALL_INDIRECT_FUNC(x)  ppc_call_indirect(ctx, base, (x))

{loads}
{stores}

// ---- runtime symbols referenced by the ported RexGlue builders -------------
// (bridge mode: real definitions come from <rex/ppc/context.h> + <memory.h>)
{port_helpers}
"""


def _emit_forward_decls(called):
    """Forward-declare called guest functions so a single .cpp compiles alone."""
    return "\n".join(f"extern PPC_FUNC(sub_{a:08X});" for a in sorted(called))


def recompile_image(path, limit=None, jobs=None, skip=None, extra=None):
    """Discover every function and recompile the whole image (RexGlue-style).
    Returns (cpp_text, mapping_text, agg). Sizes come from .pdata; functions with
    no .pdata extent are sized by the gap to the next function (linear sweep).

    `skip` is a set of guest addresses NOT to recompile — import thunks whose
    bodies are meaningless (patched at load) and are instead provided as host
    shims. They stay in the declarations and the mapping so callers still resolve.
    """
    skip = set(skip or ())
    import ppcdiscover
    import parallel
    funcs, _sources, x, img, _secs = ppcdiscover.discover(path, extra=extra)
    base = x.image_base or 0
    addrs = sorted(funcs)
    # fill missing sizes from the gap to the next function start
    sizes = {}
    for i, va in enumerate(addrs):
        s = funcs[va]
        if not s:
            s = (addrs[i + 1] - va) if i + 1 < len(addrs) else 0x100
            s = min(s, 0x4000)
        sizes[va] = s

    chosen = addrs[:limit] if limit else addrs
    emit_addrs = [a for a in chosen if a not in skip]   # import thunks skipped

    # Executable-range predicate for jump-table target validation: a candidate
    # case target must be word-aligned and land inside the discovered code span.
    _code_lo = addrs[0] if addrs else base
    _code_hi = (addrs[-1] + sizes[addrs[-1]]) if addrs else base
    def _is_exec(a):
        return (a & 3) == 0 and _code_lo <= a < _code_hi

    try:
        import progress as _pg
        if not _pg.active():
            _pg = None
    except Exception:
        _pg = None

    def _one(va):
        fbody = img[va - base: va - base + sizes[va]]
        if not fbody:
            return None
        r = recompile_function(fbody, va, va, sizes[va],      # (cpp, called, st)
                               img=img, img_base=base, is_exec=_is_exec)
        if _pg:
            _, _, st = r
            flag = ("  (%d unimpl)" % st["unimpl"]) if st["unimpl"] else ""
            _pg.detail("sub_%08X   %d/%d instrs%s"
                       % (va, st["translated"], st["total"], flag))
        return r

    # Drive the recompile through the shared thread-pool fan-out (parallel.tmap),
    # exactly like ledger/conduct do. tmap AUTO-drives the progress UI (auto_op +
    # auto_bar per completion) AND keeps the MAIN thread waiting on futures — so
    # the GIL is free for the window's paint thread and it never starves. Each
    # worker uses a thread-local capstone (disasm_ppc._md), so the fan-out is safe.
    results = parallel.tmap(_one, emit_addrs, jobs=jobs, label="Recompiling PowerPC")

    all_called = set()
    parts = []
    agg = {"funcs": 0, "total": 0, "translated": 0, "unimpl": 0,
           "module": x.module_name(), "discovered": len(funcs),
           "lo": (chosen[0] if chosen else 0), "hi": (chosen[-1] if chosen else 0)}
    for r in results:                          # tmap preserves input order
        if r is None:
            continue
        cpp, called, st = r
        parts.append(cpp)
        all_called |= called
        agg["funcs"] += 1
        for k in ("total", "translated", "unimpl"):
            agg[k] += st[k]

    # forward-declare EVERY function (chosen ones are defined later in the file;
    # C++ needs the declaration before an earlier caller uses it) plus any
    # external call targets not emitted here.
    decls = sorted(set(chosen) | all_called)
    externs = "\n".join(f"PPC_FUNC(sub_{a:08X});" for a in decls)
    body = '#include "rex_ppc_min.h"\n\n' + externs + "\n\n" + "\n".join(parts)

    mapping = ["#include \"rex_ppc_min.h\"", "",
               "struct PPCFuncMapping { uint32_t addr; PPCFunc* func; };",
               "PPCFuncMapping PPCFuncMappings[] = {"]
    for va in chosen:
        mapping.append(f"\t{{ 0x{va:08X}, sub_{va:08X} }},")
    mapping += ["\t{ 0, nullptr },", "};", ""]

    # expose the raw pieces so a caller (ppcproject) can lay them out as a real
    # multi-file source tree instead of one giant translation unit.
    agg["parts"] = parts                       # per-function C++, in address order
    agg["decls"] = decls                       # every function address to declare
    agg["chosen"] = chosen                     # all functions (for the map)
    agg["skipped"] = sorted(a for a in chosen if a in skip)   # import thunks (host-provided)
    return body, "\n".join(mapping), agg


def main():
    ap = argparse.ArgumentParser(description="PowerPC -> C++ static recompiler (PoC)")
    ap.add_argument("path", nargs="?")
    ap.add_argument("--vaddr", type=lambda x: int(x, 0))
    ap.add_argument("--size", type=lambda x: int(x, 0), default=0x100)
    ap.add_argument("--section", default=None)
    ap.add_argument("--raw", action="store_true")
    ap.add_argument("-o", "--out", default=None)
    ap.add_argument("--limit", type=int, default=None, help="cap functions in --all")
    ap.add_argument("--emit-runtime", metavar="H", help="write rex_ppc_min.h and exit")
    ap.add_argument("--all", action="store_true",
                    help="discover + recompile the WHOLE image (RexGlue-style)")
    ap.add_argument("--check", action="store_true", help="compile the output with g++")
    import parallel
    jobs = parallel.resolve_jobs(sys.argv)      # -j N / $WIN95RE_JOBS / all cores
    args = ap.parse_args()

    if args.emit_runtime:
        with open(args.emit_runtime, "w") as f:
            f.write(runtime_header())
        print("wrote runtime header:", args.emit_runtime)
        return

    if args.all:
        if not args.path:
            ap.error("--all needs a path")
        body, mapping, agg = recompile_image(args.path, limit=args.limit, jobs=jobs)
        pct = 100.0 * agg["translated"] / max(1, agg["total"])
        print(f"whole-image recompile: {agg['funcs']} functions, "
              f"{agg['translated']}/{agg['total']} instrs translated "
              f"({pct:.1f}%), {agg['unimpl']} unimplemented")
        outname = "(not written)"
        if args.out:
            with open(args.out, "w") as f:
                f.write(body)
            with open(args.out.replace(".cpp", "_init.cpp"), "w") as f:
                f.write(mapping)
            outname = os.path.basename(args.out)
            print("wrote", args.out, "+ mapping table")
        # end-of-run project status report -> the UI Details pane (+ the ding)
        try:
            import progress
            progress.report(
                "Recompiled %s  -  %.1f%% translated" % (agg["module"], pct),
                ["File:          %s" % agg["module"],
                 "Address range: 0x%08X - 0x%08X" % (agg["lo"], agg["hi"]),
                 "Functions:     %s recompiled  (of %s discovered)"
                 % (f"{agg['funcs']:,}", f"{agg['discovered']:,}"),
                 "Instructions:  %s total" % f"{agg['total']:,}",
                 "Translated:    %s   (%.1f%% live)" % (f"{agg['translated']:,}", pct),
                 "Unimplemented: %s   (VMX128 + rarer forms)" % f"{agg['unimpl']:,}",
                 "Output C++:    %s  (+ mapping table)" % outname])
        except Exception:
            pass
        if args.check:
            _check_compiles(body)
        return

    if not (args.path and args.vaddr):
        ap.error("need path + --vaddr (or --all / --emit-runtime)")

    body, sec_base, _entry = disasm_ppc._code_section(args.path, args.section, args.raw)
    off = args.vaddr - sec_base
    fbody = body[off:off + args.size]
    cpp, called, stats = recompile_function(fbody, sec_base, args.vaddr, args.size)

    header = ('#include "rex_ppc_min.h"\n\n' + _emit_forward_decls(called) + "\n\n")
    full = header + cpp
    if args.out:
        with open(args.out, "w") as f:
            f.write(full)
    print(f"recompiled sub_{args.vaddr:08X}: {stats['translated']}/{stats['total']} "
          f"instrs translated, {stats['unimpl']} unimplemented, "
          f"{stats['labels']} labels, {len(called)} calls")
    if args.out:
        print("wrote", args.out)

    if args.check:
        _check_compiles(full)


def _check_compiles(full):
    gpp = "/c/msys64/mingw64/bin/g++.exe"
    if not os.path.exists(gpp):
        gpp = "g++"
    with tempfile.TemporaryDirectory() as td:
        with open(os.path.join(td, "rex_ppc_min.h"), "w") as f:
            f.write(runtime_header())
        cpp = os.path.join(td, "f.cpp")
        with open(cpp, "w") as f:
            f.write(full)
        r = subprocess.run([gpp, "-std=c++20", "-c", "-I", td, "-o",
                            os.path.join(td, "f.o"), cpp],
                           capture_output=True, text=True)
        if r.returncode == 0:
            print("COMPILE: OK (g++ -c accepted the generated C++)")
        else:
            print("COMPILE: FAILED\n" + r.stderr[:1500])
            sys.exit(1)


if __name__ == "__main__":
    main()
