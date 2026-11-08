// codebreaker/recompile/builders/memory.cpp -- C++17 port of codebreaker.recompile.builders.memory (see memory.hpp).

#include "codebreaker/recompile/builders/memory.hpp"

#include <string>

namespace codebreaker::recompile::builders::memory {

namespace {

using Number = std::optional<int64_t>;
using Pair = std::tuple<py::str, py::str>;
using Triple = std::tuple<py::str, py::str, py::str>;

// f"{n}" for a number that is None when the operand token was not of the kind asked for
py::str num(const Number& n) { return n ? py::to_str(*n) : py::str("None"); }

// the number, or the TypeError Python raises for arithmetic on None
int64_t need(const Number& n) {
    if (!n) throw py::TypeError("unsupported operand type(s): 'NoneType'");
    return *n;
}

// def i32(x): x &= 0xFFFFFFFF; return x - 0x100000000 if x & 0x80000000 else x
int64_t i32(int64_t x) {
    x &= 0xFFFFFFFF;
    return (x & 0x80000000) ? x - 0x100000000 : x;
}

// `a != 0` for a register that is None when the operand token was not a register
bool nonzero(const Number& a) { return !a || *a != 0; }

// C++: if (operands[base] != 0) print("{}.u32 + "); print(disp)
py::str dform_addr(const Number& a, int64_t disp) {
    return (nonzero(a) ? py::str("ctx.r") + num(a) + ".u32 + " : py::str("")) + py::to_str(disp);
}

// C++: if (operands[rA] != 0) print("{}.u32 + "); print("{}.u32", rB)
py::str xform_addr(const Number& a, const Number& b) {
    return (nonzero(a) ? py::str("ctx.r") + num(a) + ".u32 + " : py::str("")) + "ctx.r" + num(b) + ".u32";
}

py::list<py::str> lines(std::initializer_list<py::str> l) { return py::list<py::str>(l); }

// d, a, b = f(ops[0]), _reg(ops[1]), _reg(ops[2])  (the destination or source in `first`'s kind)
struct Regs3 {
    Number d, a, b;
};
Regs3 regs3(const OperandNumber& first, const OperandNumber& _reg, const py::list<py::str>& ops) {
    Regs3 r;
    r.d = first(ops[0]);
    r.a = _reg(ops[1]);
    r.b = _reg(ops[2]);
    return r;
}

// d = f(ops[0]); disp, a = _mem(ops[1])   (TypeError when _mem could not read the operand: it unpacks None)
struct DForm {
    Number d;
    int64_t disp = 0;
    int64_t a = 0;
};
DForm dform(const OperandNumber& first, const MemOperand& _mem, const py::list<py::str>& ops) {
    DForm r;
    r.d = first(ops[0]);
    auto m = _mem(ops[1]);
    if (!m) throw py::TypeError("cannot unpack non-iterable NoneType object");
    r.disp = std::get<0>(*m);
    r.a = std::get<1>(*m);
    return r;
}

// inner = (f"ctx.r{a}.u32 + " if a != 0 else "") + f"ctx.r{b}.u32"
py::str inner_of(const Regs3& r) { return xform_addr(r.a, r.b); }

}  // namespace

std::optional<py::list<py::str>> emit(const py::str& mn_s, const py::list<py::str>& ops,
                                      const OperandNumber& _reg, const OperandNumber& _vreg, const OperandNumber& _freg,
                                      const OperandNumber& _imm, const OperandNumber& _cr, const MemOperand& _mem,
                                      const RegisterValue& _rv, const BaseExpression& _base_expr, const AddressExpression& _addr,
                                      int64_t va, int64_t WORD) {
    (void)_cr; (void)_rv; (void)_base_expr; (void)_addr; (void)va; (void)WORD;
    const std::string mn = mn_s.utf8();

    // -----------------------------------------------------------------------
    // Load Immediate family (L* category; not real memory ops)
    // -----------------------------------------------------------------------
    if (mn == "li") {
        Number d = _reg(ops[0]);
        return lines({py::str("ctx.r") + num(d) + ".s64 = " + py::to_str(i32(need(_imm(ops[1])))) + ";"});
    }

    if (mn == "lis") {
        // C++: rD.s64 = static_cast<int32_t>(imm << 16); (+ mmio-base tracking, dropped)
        Number d = _reg(ops[0]);
        int64_t imm = need(_imm(ops[1])) & 0xFFFFFFFF;
        return lines({py::str("ctx.r") + num(d) + ".s64 = " + py::to_str(i32(imm << 16)) + ";"});
    }

    // -----------------------------------------------------------------------
    // D-form loads: rD.u64 = MACRO(rA + D)          (emit_load_d_form)
    // -----------------------------------------------------------------------
    {
        const std::pair<const char*, const char*> LD_D[] = {{"lbz", "PPC_LOAD_U8"}, {"lhz", "PPC_LOAD_U16"},
                                                            {"lwz", "PPC_LOAD_U32"}, {"ld", "PPC_LOAD_U64"}};
        for (const auto& e : LD_D) {
            if (mn == e.first) {
                DForm f = dform(_reg, _mem, ops);
                return lines({py::str("ctx.r") + num(f.d) + ".u64 = " + e.second + "(" + dform_addr(f.a, f.disp) + ");"});
            }
        }
    }

    // -----------------------------------------------------------------------
    // X-form loads: rD.u64 = MACRO(rA + rB)         (emit_load_x_form)
    // -----------------------------------------------------------------------
    {
        const std::pair<const char*, const char*> LD_X[] = {{"lbzx", "PPC_LOAD_U8"}, {"lhzx", "PPC_LOAD_U16"},
                                                            {"lwzx", "PPC_LOAD_U32"}, {"ldx", "PPC_LOAD_U64"}};
        for (const auto& e : LD_X) {
            if (mn == e.first) {
                Regs3 r = regs3(_reg, _reg, ops);
                return lines({py::str("ctx.r") + num(r.d) + ".u64 = " + e.second + "(" + xform_addr(r.a, r.b) + ");"});
            }
        }
    }

    // -----------------------------------------------------------------------
    // D-form load w/ update: ea=D+rA; rD.u64=MACRO(ea); rA.u32=ea  (emitLoadWithUpdate)
    // -----------------------------------------------------------------------
    {
        const std::pair<const char*, const char*> LDU_D[] = {{"lbzu", "PPC_LOAD_U8"}, {"lhzu", "PPC_LOAD_U16"},
                                                             {"lwzu", "PPC_LOAD_U32"}, {"ldu", "PPC_LOAD_U64"}};
        for (const auto& e : LDU_D) {
            if (mn == e.first) {
                DForm f = dform(_reg, _mem, ops);
                return lines({py::str("ea = ") + py::to_str(f.disp) + " + ctx.r" + py::to_str(f.a) + ".u32;",
                              py::str("ctx.r") + num(f.d) + ".u64 = " + e.second + "(ea);",
                              py::str("ctx.r") + py::to_str(f.a) + ".u32 = ea;"});
            }
        }
    }

    // -----------------------------------------------------------------------
    // X-form load w/ update: ea=rA+rB; rD.u64=MACRO(ea); rA.u32=ea (emitLoadXFormWithUpdate)
    // -----------------------------------------------------------------------
    {
        const std::pair<const char*, const char*> LDU_X[] = {{"lbzux", "PPC_LOAD_U8"}, {"lhzux", "PPC_LOAD_U16"},
                                                             {"lwzux", "PPC_LOAD_U32"}, {"ldux", "PPC_LOAD_U64"}};
        for (const auto& e : LDU_X) {
            if (mn == e.first) {
                Regs3 r = regs3(_reg, _reg, ops);
                return lines({py::str("ea = ctx.r") + num(r.a) + ".u32 + ctx.r" + num(r.b) + ".u32;",
                              py::str("ctx.r") + num(r.d) + ".u64 = " + e.second + "(ea);",
                              py::str("ctx.r") + num(r.a) + ".u32 = ea;"});
            }
        }
    }

    // -----------------------------------------------------------------------
    // Sign-extending D-form loads: rD.s64 = CAST(MACRO(rA + D)) (emitSignExtendLoadDForm)
    // -----------------------------------------------------------------------
    {
        const Triple LDA_D[] = {{"lha", "int16_t", "PPC_LOAD_U16"}, {"lwa", "int32_t", "PPC_LOAD_U32"}};
        for (const auto& e : LDA_D) {
            if (mn == std::get<0>(e).utf8()) {
                const py::str& cast = std::get<1>(e);
                const py::str& macro = std::get<2>(e);
                DForm f = dform(_reg, _mem, ops);
                return lines({py::str("ctx.r") + num(f.d) + ".s64 = " + cast + "(" + macro + "(" + dform_addr(f.a, f.disp) + "));"});
            }
        }
    }

    // -----------------------------------------------------------------------
    // Sign-extending X-form loads: rD.s64 = CAST(MACRO(rA + rB)) (emitSignExtendLoadXForm)
    // -----------------------------------------------------------------------
    {
        const Triple LDA_X[] = {{"lhax", "int16_t", "PPC_LOAD_U16"}, {"lwax", "int32_t", "PPC_LOAD_U32"}};
        for (const auto& e : LDA_X) {
            if (mn == std::get<0>(e).utf8()) {
                const py::str& cast = std::get<1>(e);
                const py::str& macro = std::get<2>(e);
                Regs3 r = regs3(_reg, _reg, ops);
                return lines({py::str("ctx.r") + num(r.d) + ".s64 = " + cast + "(" + macro + "(" + xform_addr(r.a, r.b) + "));"});
            }
        }
    }

    // -----------------------------------------------------------------------
    // Algebraic loads with update (explicit builders)
    // -----------------------------------------------------------------------
    if (mn == "lhau") {   // D-form: ea=D+rA; rD.s64=int16_t(LOAD16(ea)); rA.u32=ea
        DForm f = dform(_reg, _mem, ops);
        return lines({py::str("ea = ") + py::to_str(f.disp) + " + ctx.r" + py::to_str(f.a) + ".u32;",
                      py::str("ctx.r") + num(f.d) + ".s64 = int16_t(PPC_LOAD_U16(ea));",
                      py::str("ctx.r") + py::to_str(f.a) + ".u32 = ea;"});
    }

    if (mn == "lhaux") {   // X-form: ea=rA+rB; rD.s64=int16_t(LOAD16(ea)); rA.u32=ea
        Regs3 r = regs3(_reg, _reg, ops);
        return lines({py::str("ea = ctx.r") + num(r.a) + ".u32 + ctx.r" + num(r.b) + ".u32;",
                      py::str("ctx.r") + num(r.d) + ".s64 = int16_t(PPC_LOAD_U16(ea));",
                      py::str("ctx.r") + num(r.a) + ".u32 = ea;"});
    }

    if (mn == "lwaux") {   // X-form: ea=rA+rB; rD.s64=int32_t(LOAD32(ea)); rA.u32=ea
        Regs3 r = regs3(_reg, _reg, ops);
        return lines({py::str("ea = ctx.r") + num(r.a) + ".u32 + ctx.r" + num(r.b) + ".u32;",
                      py::str("ctx.r") + num(r.d) + ".s64 = int32_t(PPC_LOAD_U32(ea));",
                      py::str("ctx.r") + num(r.a) + ".u32 = ea;"});
    }

    // -----------------------------------------------------------------------
    // Byte-reverse indexed loads (X-form). Non-MMIO -> PPC_LOAD_*.
    // -----------------------------------------------------------------------
    {
        const Triple LBR[] = {{"lhbrx", "__builtin_bswap16", "PPC_LOAD_U16"}, {"lwbrx", "__builtin_bswap32", "PPC_LOAD_U32"}};
        for (const auto& e : LBR) {
            if (mn == std::get<0>(e).utf8()) {
                const py::str& bswap = std::get<1>(e);
                const py::str& macro = std::get<2>(e);
                Regs3 r = regs3(_reg, _reg, ops);
                return lines({py::str("ctx.r") + num(r.d) + ".u64 = " + bswap + "(" + macro + "(" + xform_addr(r.a, r.b) + "));"});
            }
        }
    }

    // -----------------------------------------------------------------------
    // Atomic load-and-reserve (X-form). (emitAtomicLoadReserve)
    //   ea = [rA +] rB; reserved.field = *(T*)PPC_RAW_ADDR(ea); rD.u64 = bswap(reserved.field)
    // -----------------------------------------------------------------------
    {
        struct Larx { const char* mn; const char* ptr; const char* bswap; const char* field; };
        const Larx LARX[] = {{"lwarx", "uint32_t", "__builtin_bswap32", "u32"}, {"ldarx", "uint64_t", "__builtin_bswap64", "u64"}};
        for (const auto& e : LARX) {
            if (mn == e.mn) {
                Regs3 r = regs3(_reg, _reg, ops);
                return lines({py::str("ea = ") + xform_addr(r.a, r.b) + ";",
                              py::str("ctx.reserved.") + e.field + " = *(" + e.ptr + "*)PPC_RAW_ADDR(ea);",
                              py::str("ctx.r") + num(r.d) + ".u64 = " + e.bswap + "(ctx.reserved." + e.field + ");"});
            }
        }
    }

    // -----------------------------------------------------------------------
    // Floating-point loads (all emit disableFlushMode first)
    // -----------------------------------------------------------------------
    if (mn == "lfd") {   // D-form: fD.u64 = PPC_LOAD_U64(rA + D)
        DForm f = dform(_freg, _mem, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("ctx.f") + num(f.d) + ".u64 = PPC_LOAD_U64(" + dform_addr(f.a, f.disp) + ");"});
    }

    if (mn == "lfdx") {   // X-form: fD.u64 = PPC_LOAD_U64(rA + rB)
        Regs3 r = regs3(_freg, _reg, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("ctx.f") + num(r.d) + ".u64 = PPC_LOAD_U64(" + xform_addr(r.a, r.b) + ");"});
    }

    if (mn == "lfs") {   // D-form: temp.u32 = LOAD32(rA + D); fD.f64 = double(temp.f32)
        DForm f = dform(_freg, _mem, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("temp.u32 = PPC_LOAD_U32(") + dform_addr(f.a, f.disp) + ");",
                      py::str("ctx.f") + num(f.d) + ".f64 = double(temp.f32);"});
    }

    if (mn == "lfsx") {   // X-form: temp.u32 = LOAD32(rA + rB); fD.f64 = double(temp.f32)
        Regs3 r = regs3(_freg, _reg, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("temp.u32 = PPC_LOAD_U32(") + xform_addr(r.a, r.b) + ");",
                      py::str("ctx.f") + num(r.d) + ".f64 = double(temp.f32);"});
    }

    if (mn == "lfdu") {   // D-form update
        DForm f = dform(_freg, _mem, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("ea = ") + py::to_str(f.disp) + " + ctx.r" + py::to_str(f.a) + ".u32;",
                      py::str("ctx.f") + num(f.d) + ".u64 = PPC_LOAD_U64(ea);",
                      py::str("ctx.r") + py::to_str(f.a) + ".u32 = ea;"});
    }

    if (mn == "lfdux") {   // X-form update
        Regs3 r = regs3(_freg, _reg, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("ea = ctx.r") + num(r.a) + ".u32 + ctx.r" + num(r.b) + ".u32;",
                      py::str("ctx.f") + num(r.d) + ".u64 = PPC_LOAD_U64(ea);",
                      py::str("ctx.r") + num(r.a) + ".u32 = ea;"});
    }

    if (mn == "lfsu") {   // D-form update (single -> double)
        DForm f = dform(_freg, _mem, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("ea = ") + py::to_str(f.disp) + " + ctx.r" + py::to_str(f.a) + ".u32;",
                      py::str("temp.u32 = PPC_LOAD_U32(ea);"),
                      py::str("ctx.f") + num(f.d) + ".f64 = double(temp.f32);",
                      py::str("ctx.r") + py::to_str(f.a) + ".u32 = ea;"});
    }

    if (mn == "lfsux") {   // X-form update (single -> double)
        Regs3 r = regs3(_freg, _reg, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("ea = ctx.r") + num(r.a) + ".u32 + ctx.r" + num(r.b) + ".u32;",
                      py::str("temp.u32 = PPC_LOAD_U32(ea);"),
                      py::str("ctx.f") + num(r.d) + ".f64 = double(temp.f32);",
                      py::str("ctx.r") + num(r.a) + ".u32 = ea;"});
    }

    // -----------------------------------------------------------------------
    // D-form stores: MACRO(rA + D, rS.field)        (emit_store_d_form)
    // -----------------------------------------------------------------------
    {
        const Triple ST_D[] = {{"stb", "PPC_STORE_U8", "u8"}, {"sth", "PPC_STORE_U16", "u16"},
                               {"stw", "PPC_STORE_U32", "u32"}, {"std", "PPC_STORE_U64", "u64"}};
        for (const auto& e : ST_D) {
            if (mn == std::get<0>(e).utf8()) {
                const py::str& macro = std::get<1>(e);
                const py::str& field = std::get<2>(e);
                DForm f = dform(_reg, _mem, ops);
                return lines({macro + "(" + dform_addr(f.a, f.disp) + ", ctx.r" + num(f.d) + "." + field + ");"});
            }
        }
    }

    // -----------------------------------------------------------------------
    // X-form stores: MACRO(rA + rB, rS.field)       (emit_store_x_form)
    // -----------------------------------------------------------------------
    {
        const Triple ST_X[] = {{"stbx", "PPC_STORE_U8", "u8"}, {"sthx", "PPC_STORE_U16", "u16"},
                               {"stwx", "PPC_STORE_U32", "u32"}, {"stdx", "PPC_STORE_U64", "u64"}};
        for (const auto& e : ST_X) {
            if (mn == std::get<0>(e).utf8()) {
                const py::str& macro = std::get<1>(e);
                const py::str& field = std::get<2>(e);
                Regs3 r = regs3(_reg, _reg, ops);
                return lines({macro + "(" + xform_addr(r.a, r.b) + ", ctx.r" + num(r.d) + "." + field + ");"});
            }
        }
    }

    // -----------------------------------------------------------------------
    // D-form store w/ update: ea=D+rA; MACRO(ea, rS.field); rA.u32=ea (emitStoreWithUpdate)
    // -----------------------------------------------------------------------
    {
        const Triple STU_D[] = {{"stbu", "PPC_STORE_U8", "u8"}, {"sthu", "PPC_STORE_U16", "u16"},
                                {"stwu", "PPC_STORE_U32", "u32"}, {"stdu", "PPC_STORE_U64", "u64"}};
        for (const auto& e : STU_D) {
            if (mn == std::get<0>(e).utf8()) {
                const py::str& macro = std::get<1>(e);
                const py::str& field = std::get<2>(e);
                DForm f = dform(_reg, _mem, ops);
                return lines({py::str("ea = ") + py::to_str(f.disp) + " + ctx.r" + py::to_str(f.a) + ".u32;",
                              macro + "(ea, ctx.r" + num(f.d) + "." + field + ");",
                              py::str("ctx.r") + py::to_str(f.a) + ".u32 = ea;"});
            }
        }
    }

    // -----------------------------------------------------------------------
    // X-form store w/ update: ea=rA+rB; MACRO(ea, rS.field); rA.u32=ea (emitStoreXFormWithUpdate)
    //   (C++ selects mmio macro here; non-MMIO -> normal macro)
    // -----------------------------------------------------------------------
    {
        const Triple STU_X[] = {{"stbux", "PPC_STORE_U8", "u8"}, {"sthux", "PPC_STORE_U16", "u16"},
                                {"stwux", "PPC_STORE_U32", "u32"}, {"stdux", "PPC_STORE_U64", "u64"}};
        for (const auto& e : STU_X) {
            if (mn == std::get<0>(e).utf8()) {
                const py::str& macro = std::get<1>(e);
                const py::str& field = std::get<2>(e);
                Regs3 r = regs3(_reg, _reg, ops);
                return lines({py::str("ea = ctx.r") + num(r.a) + ".u32 + ctx.r" + num(r.b) + ".u32;",
                              macro + "(ea, ctx.r" + num(r.d) + "." + field + ");",
                              py::str("ctx.r") + num(r.a) + ".u32 = ea;"});
            }
        }
    }

    // -----------------------------------------------------------------------
    // Byte-reverse indexed stores (X-form). Non-MMIO -> PPC_STORE_*.
    // -----------------------------------------------------------------------
    {
        struct Sbr { const char* mn; const char* macro; const char* bswap; const char* field; };
        const Sbr SBR[] = {{"sthbrx", "PPC_STORE_U16", "__builtin_bswap16", "u16"},
                           {"stwbrx", "PPC_STORE_U32", "__builtin_bswap32", "u32"}};
        for (const auto& e : SBR) {
            if (mn == e.mn) {
                Regs3 r = regs3(_reg, _reg, ops);
                return lines({py::str(e.macro) + "(" + xform_addr(r.a, r.b) + ", " + e.bswap + "(ctx.r" + num(r.d) + "." + e.field + "));"});
            }
        }
    }

    // -----------------------------------------------------------------------
    // Atomic store-conditional (X-form). (emitAtomicStoreConditional)
    //   ea=[rA +]rB; cr0.lt=0; cr0.gt=0;
    //   cr0.eq = __sync_bool_compare_and_swap((T*)PPC_RAW_ADDR(ea), reserved.field, bswap(rS.field));
    //   cr0.so = xer.so;
    // Mnemonics carry the mandatory '.' record suffix; accept dotted and bare.
    // -----------------------------------------------------------------------
    {
        struct Stcx { const char* mn; const char* ptr; const char* bswap; const char* field; };
        const Stcx STCX[] = {{"stwcx.", "uint32_t", "__builtin_bswap32", "s32"},
                             {"stwcx", "uint32_t", "__builtin_bswap32", "s32"},
                             {"stdcx.", "uint64_t", "__builtin_bswap64", "s64"},
                             {"stdcx", "uint64_t", "__builtin_bswap64", "s64"}};
        for (const auto& e : STCX) {
            if (mn == e.mn) {
                Regs3 r = regs3(_reg, _reg, ops);
                return lines({py::str("ea = ") + xform_addr(r.a, r.b) + ";",
                              py::str("ctx.cr0.lt = 0;"),
                              py::str("ctx.cr0.gt = 0;"),
                              py::str("ctx.cr0.eq = __sync_bool_compare_and_swap(reinterpret_cast<") + e.ptr +
                                  "*>(PPC_RAW_ADDR(ea)), ctx.reserved." + e.field + ", " + e.bswap + "(ctx.r" + num(r.d) + "." + e.field + "));",
                              py::str("ctx.cr0.so = ctx.xer.so;")});
            }
        }
    }

    // -----------------------------------------------------------------------
    // Floating-point stores (all emit disableFlushMode first)
    // -----------------------------------------------------------------------
    if (mn == "stfd") {   // D-form: STORE64(rA + D, fS.u64)
        DForm f = dform(_freg, _mem, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("PPC_STORE_U64(") + dform_addr(f.a, f.disp) + ", ctx.f" + num(f.d) + ".u64);"});
    }

    if (mn == "stfdx") {   // X-form: STORE64(rA + rB, fS.u64)
        Regs3 r = regs3(_freg, _reg, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("PPC_STORE_U64(") + xform_addr(r.a, r.b) + ", ctx.f" + num(r.d) + ".u64);"});
    }

    if (mn == "stfdux") {   // X-form update (plain PPC_STORE_U64, no mmio in C++)
        Regs3 r = regs3(_freg, _reg, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("ea = ctx.r") + num(r.a) + ".u32 + ctx.r" + num(r.b) + ".u32;",
                      py::str("PPC_STORE_U64(ea, ctx.f") + num(r.d) + ".u64);",
                      py::str("ctx.r") + num(r.a) + ".u32 = ea;"});
    }

    if (mn == "stfiwx") {   // X-form: STORE32(rA + rB, fS.u32)
        Regs3 r = regs3(_freg, _reg, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("PPC_STORE_U32(") + xform_addr(r.a, r.b) + ", ctx.f" + num(r.d) + ".u32);"});
    }

    if (mn == "stfs") {   // D-form: temp.f32 = float(fS.f64); STORE32(rA + D, temp.u32)
        DForm f = dform(_freg, _mem, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("temp.f32 = float(ctx.f") + num(f.d) + ".f64);",
                      py::str("PPC_STORE_U32(") + dform_addr(f.a, f.disp) + ", temp.u32);"});
    }

    if (mn == "stfsx") {   // X-form: temp.f32 = float(fS.f64); STORE32(rA + rB, temp.u32)
        Regs3 r = regs3(_freg, _reg, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("temp.f32 = float(ctx.f") + num(r.d) + ".f64);",
                      py::str("PPC_STORE_U32(") + xform_addr(r.a, r.b) + ", temp.u32);"});
    }

    if (mn == "stfdu") {   // D-form update
        DForm f = dform(_freg, _mem, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("ea = ") + py::to_str(f.disp) + " + ctx.r" + py::to_str(f.a) + ".u32;",
                      py::str("PPC_STORE_U64(ea, ctx.f") + num(f.d) + ".u64);",
                      py::str("ctx.r") + py::to_str(f.a) + ".u32 = ea;"});
    }

    if (mn == "stfsu") {   // D-form update (double -> float)
        DForm f = dform(_freg, _mem, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("ea = ") + py::to_str(f.disp) + " + ctx.r" + py::to_str(f.a) + ".u32;",
                      py::str("temp.f32 = float(ctx.f") + num(f.d) + ".f64);",
                      py::str("PPC_STORE_U32(ea, temp.u32);"),
                      py::str("ctx.r") + py::to_str(f.a) + ".u32 = ea;"});
    }

    if (mn == "stfsux") {   // X-form update (double -> float); non-MMIO -> PPC_STORE_U32
        Regs3 r = regs3(_freg, _reg, ops);
        return lines({py::str("ctx.fpscr.disableFlushMode();"),
                      py::str("temp.f32 = float(ctx.f") + num(r.d) + ".f64);",
                      py::str("ea = ctx.r") + num(r.a) + ".u32 + ctx.r" + num(r.b) + ".u32;",
                      py::str("PPC_STORE_U32(ea, temp.u32);"),
                      py::str("ctx.r") + num(r.a) + ".u32 = ea;"});
    }

    // -----------------------------------------------------------------------
    // Vector loads. NOTE(tomc): endian handling reverses the WHOLE vector
    // (VectorMaskL shuffle), not individual elements. Ported verbatim.
    // dispatch: lvx/lvx128/lvxl128/lvebx/lvehx/lvewx/lvewx128 -> build_lvx
    // -----------------------------------------------------------------------
    if (mn == "lvx" || mn == "lvx128" || mn == "lvxl128" || mn == "lvebx" || mn == "lvehx" || mn == "lvewx" || mn == "lvewx128") {
        Regs3 r = regs3(_vreg, _reg, ops);
        py::str inner = inner_of(r);
        return lines({py::str("ea = (") + inner + ") & ~0xF;",
                      py::str("simde_mm_store_si128((simde__m128i*)ctx.v") + num(r.d) + ".u8, " +
                          "simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), " +
                          "simde_mm_load_si128((simde__m128i*)VectorMaskL)));"});
    }

    if (mn == "lvlx" || mn == "lvlx128") {
        Regs3 r = regs3(_vreg, _reg, ops);
        py::str inner = inner_of(r);
        return lines({py::str("temp.u32 = ") + inner + ";",
                      py::str("simde_mm_store_si128((simde__m128i*)ctx.v") + num(r.d) + ".u8, " +
                          "simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(temp.u32 & ~0xF)), " +
                          "simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));"});
    }

    if (mn == "lvrx" || mn == "lvrx128") {
        Regs3 r = regs3(_vreg, _reg, ops);
        py::str inner = inner_of(r);
        return lines({py::str("temp.u32 = ") + inner + ";",
                      py::str("simde_mm_store_si128((simde__m128i*)ctx.v") + num(r.d) + ".u8, temp.u32 & 0xF ? " +
                          "simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(temp.u32 & ~0xF)), " +
                          "simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : " +
                          "simde_mm_setzero_si128());"});
    }

    if (mn == "lvsl") {
        Regs3 r = regs3(_vreg, _reg, ops);
        py::str inner = inner_of(r);
        return lines({py::str("temp.u32 = ") + inner + ";",
                      py::str("simde_mm_store_si128((simde__m128i*)ctx.v") + num(r.d) + ".u8, " +
                          "simde_mm_load_si128((simde__m128i*)&VectorShiftTableL[(temp.u32 & 0xF) * 16]));"});
    }

    if (mn == "lvsr") {
        Regs3 r = regs3(_vreg, _reg, ops);
        py::str inner = inner_of(r);
        return lines({py::str("temp.u32 = ") + inner + ";",
                      py::str("simde_mm_store_si128((simde__m128i*)ctx.v") + num(r.d) + ".u8, " +
                          "simde_mm_load_si128((simde__m128i*)&VectorShiftTableR[(temp.u32 & 0xF) * 16]));"});
    }

    // -----------------------------------------------------------------------
    // Vector stores. NOTE: full-vector-reversal is accounted for in the
    // element index math (e.g. u16[7 - ...], u32[3 - ...], u8[15 - i]).
    // -----------------------------------------------------------------------
    if (mn == "stvehx") {
        Regs3 r = regs3(_vreg, _reg, ops);
        py::str inner = inner_of(r);
        return lines({py::str("ea = (") + inner + ") & ~0x1;",
                      py::str("PPC_STORE_U16(ea, ctx.v") + num(r.d) + ".u16[7 - ((ea & 0xF) >> 1)]);"});
    }

    if (mn == "stvewx" || mn == "stvewx128") {
        Regs3 r = regs3(_vreg, _reg, ops);
        py::str inner = inner_of(r);
        return lines({py::str("ea = (") + inner + ") & ~0x3;",
                      py::str("PPC_STORE_U32(ea, ctx.v") + num(r.d) + ".u32[3 - ((ea & 0xF) >> 2)]);"});
    }

    if (mn == "stvlx" || mn == "stvlx128" || mn == "stvlxl128") {
        Regs3 r = regs3(_vreg, _reg, ops);
        py::str inner = inner_of(r);
        return lines({py::str("ea = ") + inner + ";",
                      py::str("for (size_t i = 0; i < (16 - (ea & 0xF)); i++)"),
                      py::str("\tPPC_STORE_U8(ea + i, ctx.v") + num(r.d) + ".u8[15 - i]);"});
    }

    if (mn == "stvrx" || mn == "stvrx128") {
        Regs3 r = regs3(_vreg, _reg, ops);
        py::str inner = inner_of(r);
        return lines({py::str("ea = ") + inner + ";",
                      py::str("for (size_t i = 0; i < (ea & 0xF); i++)"),
                      py::str("\tPPC_STORE_U8(ea - i - 1, ctx.v") + num(r.d) + ".u8[i]);"});
    }

    if (mn == "stvx" || mn == "stvx128") {
        Regs3 r = regs3(_vreg, _reg, ops);
        py::str inner = inner_of(r);
        return lines({py::str("ea = (") + inner + ") & ~0xF;",
                      py::str("simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), ") +
                          "simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v" + num(r.d) + ".u8), " +
                          "simde_mm_load_si128((simde__m128i*)VectorMaskL)));"});
    }

    return std::nullopt;
}

const py::str RUNTIME_HELPERS = py::str(R"CBX(
// Provided by the runtime (native/rexglue/kernel; not redefined when it is included):
//   #include <simde/x86/sse4.1.h>   // simde_mm_shuffle_epi8, etc.
// extern const uint8_t VectorMaskL[];        // whole-vector byte-reverse mask
// extern const uint8_t VectorMaskR[];
// extern const uint8_t VectorShiftTableL[];  // lvsl table
// extern const uint8_t VectorShiftTableR[];  // lvsr table
// PPC_LOAD_U*/PPC_STORE_U*/PPC_RAW_ADDR come from the recompiler runtime header.
)CBX");

const py::list<py::str> MNEMONICS = {
    // load immediate
    py::str("li"), py::str("lis"),
    // d-form loads
    py::str("lbz"), py::str("lhz"), py::str("lwz"), py::str("ld"),
    // x-form loads
    py::str("lbzx"), py::str("lhzx"), py::str("lwzx"), py::str("ldx"),
    // d-form load w/ update
    py::str("lbzu"), py::str("lhzu"), py::str("lwzu"), py::str("ldu"),
    // x-form load w/ update
    py::str("lbzux"), py::str("lhzux"), py::str("lwzux"), py::str("ldux"),
    // algebraic loads
    py::str("lha"), py::str("lwa"), py::str("lhax"), py::str("lwax"), py::str("lhau"), py::str("lhaux"), py::str("lwaux"),
    // byte-reverse loads
    py::str("lhbrx"), py::str("lwbrx"),
    // atomic load-reserve
    py::str("lwarx"), py::str("ldarx"),
    // fp loads
    py::str("lfd"), py::str("lfdx"), py::str("lfs"), py::str("lfsx"), py::str("lfdu"), py::str("lfdux"), py::str("lfsu"), py::str("lfsux"),
    // d-form stores
    py::str("stb"), py::str("sth"), py::str("stw"), py::str("std"),
    // x-form stores
    py::str("stbx"), py::str("sthx"), py::str("stwx"), py::str("stdx"),
    // d-form store w/ update
    py::str("stbu"), py::str("sthu"), py::str("stwu"), py::str("stdu"),
    // x-form store w/ update
    py::str("stbux"), py::str("sthux"), py::str("stwux"), py::str("stdux"),
    // byte-reverse stores
    py::str("sthbrx"), py::str("stwbrx"),
    // atomic store-conditional (dotted + bare)
    py::str("stwcx."), py::str("stwcx"), py::str("stdcx."), py::str("stdcx"),
    // fp stores
    py::str("stfd"), py::str("stfdx"), py::str("stfdux"), py::str("stfiwx"), py::str("stfs"), py::str("stfsx"), py::str("stfdu"), py::str("stfsu"), py::str("stfsux"),
    // vector loads (+ VMX128 / element variants)
    py::str("lvx"), py::str("lvx128"), py::str("lvxl128"), py::str("lvebx"), py::str("lvehx"), py::str("lvewx"), py::str("lvewx128"),
    py::str("lvlx"), py::str("lvlx128"), py::str("lvrx"), py::str("lvrx128"), py::str("lvsl"), py::str("lvsr"),
    // vector stores (+ VMX128 variants)
    py::str("stvehx"), py::str("stvewx"), py::str("stvewx128"),
    py::str("stvlx"), py::str("stvlx128"), py::str("stvlxl128"), py::str("stvrx"), py::str("stvrx128"), py::str("stvx"), py::str("stvx128"),
};

}  // namespace codebreaker::recompile::builders::memory
