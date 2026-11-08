// codebreaker/recompile/builders/arith.cpp -- C++17 port of codebreaker.recompile.builders.arith (see arith.hpp).

#include "codebreaker/recompile/builders/arith.hpp"

#include <map>
#include <string>
#include <tuple>

namespace codebreaker::recompile::builders::arith {

namespace {

using Number = std::optional<int64_t>;

// f"{n}" for a number that is None when the operand token was not of the kind asked for
py::str num(const Number& n) { return n ? py::to_str(*n) : py::str("None"); }

// "ctx.r<n>"
py::str R(const Number& n) { return py::str("ctx.r") + num(n); }

// the number, or the TypeError Python raises for arithmetic on None
int64_t need(const Number& n) {
    if (!n) throw py::TypeError("unsupported operand type(s): 'NoneType'");
    return *n;
}

// the low 32 bits of x as a signed 32-bit number (the Python's i32)
int64_t i32(int64_t x) {
    return static_cast<int64_t>(static_cast<int32_t>(static_cast<uint32_t>(static_cast<uint64_t>(x) & 0xFFFFFFFFull)));
}

// the low 32 bits of x as an unsigned number (the Python's u32)
int64_t u32(int64_t x) { return static_cast<int64_t>(static_cast<uint64_t>(x) & 0xFFFFFFFFull); }

py::list<py::str> lines(std::initializer_list<py::str> l) { return py::list<py::str>(l); }

}  // namespace

std::optional<py::list<py::str>> emit(const py::str& mn, const py::list<py::str>& ops,
                                      const OperandNumber& _reg, const OperandNumber& _vreg, const OperandNumber& _freg,
                                      const OperandNumber& _imm, const OperandNumber& _cr, const MemOperand& _mem,
                                      const RegisterValue& _rv, const BaseExpression& _base_expr, const AddressExpression& _addr,
                                      int64_t va, int64_t WORD) {
    (void)_vreg; (void)_freg; (void)_mem; (void)_rv; (void)_base_expr; (void)_addr; (void)va; (void)WORD;

    // a record form's CR0 update is powerpc.py's (see the module's notes): never emitted here
    std::string full = mn.utf8();
    const std::string m = (!full.empty() && full.back() == '.') ? full.substr(0, full.size() - 1) : full;

    // rD, rA, rB (operands 0, 1, 2), evaluated in the order the Python does
    auto reg3 = [&]() {
        Number d = _reg(ops[0]);
        Number a = _reg(ops[1]);
        Number b = _reg(ops[2]);
        return std::make_tuple(d, a, b);
    };
    // rD, rA (operands 0 and 1)
    auto reg2 = [&]() {
        Number d = _reg(ops[0]);
        Number a = _reg(ops[1]);
        return std::make_tuple(d, a);
    };

    // =======================================================================
    // ARITHMETIC (arithmetic.cpp)
    // =======================================================================

    // ---- Addition --------------------------------------------------------
    if (m == "add") {  // build_add
        auto [d, a, b] = reg3();
        return lines({R(d) + ".u64 = " + R(a) + ".u64 + " + R(b) + ".u64;"});
    }

    if (m == "adde") {  // build_adde
        auto [d, a, b] = reg3();
        return lines({
            py::str("temp.u8 = (") + R(a) + ".u32 + " + R(b) + ".u32 < " + R(a) + ".u32) | " +
                "(" + R(a) + ".u32 + " + R(b) + ".u32 + ctx.xer.ca < ctx.xer.ca);",
            R(d) + ".u64 = " + R(a) + ".u64 + " + R(b) + ".u64 + ctx.xer.ca;",
            py::str("ctx.xer.ca = temp.u8;"),
        });
    }

    if (m == "addi") {  // build_addi  (no record form)
        auto [d, a] = reg2();
        int64_t simm = i32(need(_imm(ops[2])));
        py::str lhs = (!a || *a != 0) ? R(a) + ".s64 + " : py::str("");
        return lines({R(d) + ".s64 = " + lhs + py::to_str(simm) + ";"});
    }

    if (m == "addic") {  // build_addic
        auto [d, a] = reg2();
        int64_t simm = i32(need(_imm(ops[2])));
        int64_t notimm = static_cast<int64_t>((~static_cast<uint64_t>(simm)) & 0xFFFFFFFFull);   // C++: ~operands[2] (uint32_t, unsigned print)
        return lines({
            py::str("ctx.xer.ca = ") + R(a) + ".u32 > " + py::to_str(notimm) + ";",
            R(d) + ".s64 = " + R(a) + ".s64 + " + py::to_str(simm) + ";",
        });
    }

    if (m == "addis") {  // build_addis  (no record form)
        auto [d, a] = reg2();
        // C++: static_cast<int32_t>(operands[2] << 16); the shift is done modulo 2**64, whose low 32 bits are the Python's
        int64_t imm = i32(static_cast<int64_t>(static_cast<uint64_t>(need(_imm(ops[2]))) << 16));
        py::str lhs = (!a || *a != 0) ? R(a) + ".s64 + " : py::str("");
        return lines({R(d) + ".s64 = " + lhs + py::to_str(imm) + ";"});
    }

    if (m == "addze") {  // build_addze
        auto [d, a] = reg2();
        return lines({
            py::str("temp.s64 = ") + R(a) + ".s64 + ctx.xer.ca;",
            py::str("ctx.xer.ca = temp.u32 < ") + R(a) + ".u32;",
            R(d) + ".s64 = temp.s64;",
        });
    }

    if (m == "addme") {  // build_addme
        auto [d, a] = reg2();
        return lines({
            py::str("temp.u8 = (") + R(a) + ".u32 + 0xFFFFFFFFu < " + R(a) + ".u32) | " +
                "(" + R(a) + ".u32 + 0xFFFFFFFFu + ctx.xer.ca < ctx.xer.ca);",
            R(d) + ".u64 = " + R(a) + ".u64 + ctx.xer.ca + 0xFFFFFFFFFFFFFFFFull;",
            py::str("ctx.xer.ca = temp.u8;"),
        });
    }

    if (m == "addc") {  // build_addc
        auto [d, a, b] = reg3();
        return lines({
            py::str("ctx.xer.ca = ") + R(a) + ".u32 + " + R(b) + ".u32 < " + R(a) + ".u32;",
            R(d) + ".u64 = " + R(a) + ".u64 + " + R(b) + ".u64;",
        });
    }

    // ---- Division (safe: 0 on div-by-zero / INT_MIN/-1) ------------------
    if (m == "divd") {  // build_divd
        auto [d, a, b] = reg3();
        return lines({
            R(d) + ".s64 = (" + R(b) + ".s64 && !(" + R(a) + ".s64 == INT64_MIN && " +
                R(b) + ".s64 == -1)) ? " + R(a) + ".s64 / " + R(b) + ".s64 : 0;"
        });
    }

    if (m == "divdu") {  // build_divdu
        auto [d, a, b] = reg3();
        return lines({R(d) + ".u64 = " + R(b) + ".u64 ? " + R(a) + ".u64 / " + R(b) + ".u64 : 0;"});
    }

    if (m == "divw") {  // build_divw
        auto [d, a, b] = reg3();
        return lines({
            R(d) + ".u64 = uint32_t((" + R(b) + ".s32 && !(" + R(a) + ".s32 == INT32_MIN && " +
                R(b) + ".s32 == -1)) ? " + R(a) + ".s32 / " + R(b) + ".s32 : 0);"
        });
    }

    if (m == "divwu") {  // build_divwu
        auto [d, a, b] = reg3();
        return lines({R(d) + ".u64 = uint32_t(" + R(b) + ".u32 ? " + R(a) + ".u32 / " + R(b) + ".u32 : 0);"});
    }

    // ---- Multiplication --------------------------------------------------
    if (m == "mulhw") {  // build_mulhw
        auto [d, a, b] = reg3();
        return lines({R(d) + ".s64 = (int64_t(" + R(a) + ".s32) * int64_t(" + R(b) + ".s32)) >> 32;"});
    }

    if (m == "mulhwu") {  // build_mulhwu
        auto [d, a, b] = reg3();
        return lines({R(d) + ".u64 = (uint64_t(" + R(a) + ".u32) * uint64_t(" + R(b) + ".u32)) >> 32;"});
    }

    if (m == "mulld") {  // build_mulld
        auto [d, a, b] = reg3();
        return lines({R(d) + ".s64 = static_cast<int64_t>(" + R(a) + ".u64 * " + R(b) + ".u64);"});
    }

    if (m == "mulli") {  // build_mulli  (no record form)
        auto [d, a] = reg2();
        int64_t simm = i32(need(_imm(ops[2])));
        return lines({
            R(d) + ".s64 = static_cast<int64_t>(" + R(a) + ".u64 * " +
                "static_cast<uint64_t>(" + py::to_str(simm) + "));"
        });
    }

    if (m == "mullw") {  // build_mullw
        auto [d, a, b] = reg3();
        return lines({R(d) + ".s64 = int64_t(" + R(a) + ".s32) * int64_t(" + R(b) + ".s32);"});
    }

    if (m == "mulhd") {  // build_mulhd
        auto [d, a, b] = reg3();
        return lines({
            R(d) + ".s64 = static_cast<int64_t>((static_cast<__int128>(" +
                "static_cast<int64_t>(" + R(a) + ".s64)) * static_cast<__int128>(" +
                "static_cast<int64_t>(" + R(b) + ".s64))) >> 64);"
        });
    }

    if (m == "mulhdu") {  // build_mulhdu
        auto [d, a, b] = reg3();
        return lines({
            R(d) + ".u64 = static_cast<uint64_t>((static_cast<__uint128_t>(" + R(a) + ".u64) * " +
                "static_cast<__uint128_t>(" + R(b) + ".u64)) >> 64);"
        });
    }

    // ---- Negation --------------------------------------------------------
    if (m == "neg") {  // build_neg
        auto [d, a] = reg2();
        return lines({R(d) + ".s64 = static_cast<int64_t>(-" + R(a) + ".u64);"});
    }

    // ---- Subtraction (subtract-from: rD = rB - rA) -----------------------
    if (m == "subf") {  // build_subf
        auto [d, a, b] = reg3();
        return lines({R(d) + ".u64 = " + R(b) + ".u64 - " + R(a) + ".u64;"});
    }

    if (m == "subfc") {  // build_subfc
        auto [d, a, b] = reg3();
        return lines({
            py::str("ctx.xer.ca = ") + R(b) + ".u32 >= " + R(a) + ".u32;",
            R(d) + ".u64 = " + R(b) + ".u64 - " + R(a) + ".u64;",
        });
    }

    if (m == "subfe") {  // build_subfe
        auto [d, a, b] = reg3();
        return lines({
            py::str("temp.u8 = (~") + R(a) + ".u32 + " + R(b) + ".u32 < ~" + R(a) + ".u32) | " +
                "(~" + R(a) + ".u32 + " + R(b) + ".u32 + ctx.xer.ca < ctx.xer.ca);",
            R(d) + ".u64 = ~" + R(a) + ".u64 + " + R(b) + ".u64 + ctx.xer.ca;",
            py::str("ctx.xer.ca = temp.u8;"),
        });
    }

    if (m == "subfic") {  // build_subfic  (no record form)
        auto [d, a] = reg2();
        int64_t simm = i32(need(_imm(ops[2])));
        return lines({
            py::str("ctx.xer.ca = ") + R(a) + ".u32 <= " + py::to_str(u32(simm)) + ";",      // C++: <= operands[2] (uint32_t)
            R(d) + ".u64 = static_cast<uint64_t>(" + py::to_str(simm) + ") - " + R(a) + ".u64;",
        });
    }

    if (m == "subfze") {  // build_subfze
        auto [d, a] = reg2();
        return lines({
            py::str("temp.u8 = ~") + R(a) + ".u32 + ctx.xer.ca < ~" + R(a) + ".u32;",
            R(d) + ".u64 = ~" + R(a) + ".u64 + ctx.xer.ca;",
            py::str("ctx.xer.ca = temp.u8;"),
        });
    }

    if (m == "subfme") {  // build_subfme
        auto [d, a] = reg2();
        return lines({
            py::str("temp.u8 = (~") + R(a) + ".u32 + 0xFFFFFFFFu < ~" + R(a) + ".u32) | " +
                "(~" + R(a) + ".u32 + 0xFFFFFFFFu + ctx.xer.ca < ctx.xer.ca);",
            R(d) + ".u64 = ~" + R(a) + ".u64 + ctx.xer.ca + 0xFFFFFFFFFFFFFFFFull;",
            py::str("ctx.xer.ca = temp.u8;"),
        });
    }

    // =======================================================================
    // COMPARISON (comparison.cpp)  -- crD.compare<T>(rA.field, rB.field|imm, xer)
    // =======================================================================

    // emitCompareRegister(ctx, type, field): reg-to-reg   (mnemonic -> (C++ type, register field))
    static const std::map<std::string, std::tuple<std::string, std::string>> CMP_REG = {
        {"cmpd",  {"int64_t",  "s64"}},
        {"cmpld", {"uint64_t", "u64"}},
        {"cmplw", {"uint32_t", "u32"}},
        {"cmpw",  {"int32_t",  "s32"}},
    };
    auto reg_form = CMP_REG.find(m);
    if (reg_form != CMP_REG.end()) {
        const std::string& typ = std::get<0>(reg_form->second);
        const std::string& fld = std::get<1>(reg_form->second);
        Number cr = _cr(ops[0]);
        Number a = _reg(ops[1]);
        Number b = _reg(ops[2]);
        return lines({py::str("ctx.cr") + num(cr) + ".compare<" + typ + ">(" + R(a) + "." + fld + ", " + R(b) + "." + fld + ", ctx.xer);"});
    }

    // emitCompareImmediate(ctx, type, field, sign_extend): reg-to-imm
    //   sign_extend True  -> imm = static_cast<int32_t>(operands[2])  (signed print)
    //   sign_extend False -> imm = operands[2]                        (uint32_t print)
    static const std::map<std::string, std::tuple<std::string, std::string, bool>> CMP_IMM = {
        {"cmpdi",  {"int64_t",  "s64", true}},
        {"cmpldi", {"uint64_t", "u64", false}},
        {"cmplwi", {"uint32_t", "u32", false}},
        {"cmpwi",  {"int32_t",  "s32", true}},
    };
    auto imm_form = CMP_IMM.find(m);
    if (imm_form != CMP_IMM.end()) {
        const std::string& typ = std::get<0>(imm_form->second);
        const std::string& fld = std::get<1>(imm_form->second);
        bool sx = std::get<2>(imm_form->second);
        Number cr = _cr(ops[0]);
        Number a = _reg(ops[1]);
        int64_t v = need(_imm(ops[2]));
        int64_t imm = sx ? i32(v) : u32(v);
        return lines({py::str("ctx.cr") + num(cr) + ".compare<" + typ + ">(" + R(a) + "." + fld + ", " + py::to_str(imm) + ", ctx.xer);"});
    }

    return std::nullopt;
}

const py::str RUNTIME_HELPERS = py::str("");

const py::list<py::str> MNEMONICS = {
    // addition
    py::str("add"), py::str("adde"), py::str("addi"), py::str("addic"), py::str("addis"), py::str("addze"), py::str("addme"), py::str("addc"),
    // division
    py::str("divd"), py::str("divdu"), py::str("divw"), py::str("divwu"),
    // multiplication
    py::str("mulhw"), py::str("mulhwu"), py::str("mulld"), py::str("mulli"), py::str("mullw"), py::str("mulhd"), py::str("mulhdu"),
    // negation
    py::str("neg"),
    // subtraction
    py::str("subf"), py::str("subfc"), py::str("subfe"), py::str("subfic"), py::str("subfze"), py::str("subfme"),
    // comparison (reg-reg)
    py::str("cmpd"), py::str("cmpld"), py::str("cmplw"), py::str("cmpw"),
    // comparison (reg-imm)
    py::str("cmpdi"), py::str("cmpldi"), py::str("cmplwi"), py::str("cmpwi"),
};

}  // namespace codebreaker::recompile::builders::arith
