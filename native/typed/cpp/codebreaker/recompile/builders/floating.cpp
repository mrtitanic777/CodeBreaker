// codebreaker/recompile/builders/floating.cpp -- C++17 port of codebreaker.recompile.builders.floating (see floating.hpp).

#include "codebreaker/recompile/builders/floating.hpp"

namespace codebreaker::recompile::builders::floating {

namespace {

using Number = std::optional<int64_t>;

// f"{n}" for a number that is None when the operand token was not of the kind asked for
py::str num(const Number& n) { return n ? py::to_str(*n) : py::str("None"); }

// "ctx.f<n>"
py::str F(const Number& n) { return py::str("ctx.f") + num(n); }

// the statements of one builder: the flush-mode switch first, then the instruction's own
py::list<py::str> with_flush(const py::str& statement) {
    // All scalar FP ops first disable flush mode (FPU mode: denormals preserved).
    return py::list<py::str>{py::str("ctx.fpscr.disableFlushMode();"), statement};
}

}  // namespace

std::optional<py::list<py::str>> emit(const py::str& mn, const py::list<py::str>& ops,
                                      const OperandNumber& _reg, const OperandNumber& _vreg, const OperandNumber& _freg,
                                      const OperandNumber& _imm, const OperandNumber& _cr, const MemOperand& _mem,
                                      const RegisterValue& _rv, const BaseExpression& _base_expr, const AddressExpression& _addr,
                                      int64_t va, int64_t WORD) {
    (void)_reg; (void)_vreg; (void)_imm; (void)_mem; (void)_rv; (void)_base_expr; (void)_addr; (void)va; (void)WORD;
    const std::string m = mn.utf8();

    // fD, fA (operands 0 and 1), evaluated in the order the Python does
    auto freg2 = [&]() {
        Number d = _freg(ops[0]);
        Number a = _freg(ops[1]);
        return std::make_tuple(d, a);
    };
    // fD, fA, fB (operands 0, 1, 2)
    auto freg3 = [&]() {
        Number d = _freg(ops[0]);
        Number a = _freg(ops[1]);
        Number b = _freg(ops[2]);
        return std::make_tuple(d, a, b);
    };
    // fD, fA, fC, fB (operands 0, 1, 2, 3)
    auto freg4 = [&]() {
        Number d = _freg(ops[0]);
        Number a = _freg(ops[1]);
        Number c = _freg(ops[2]);
        Number b = _freg(ops[3]);
        return std::make_tuple(d, a, c, b);
    };

    // ---- Sign Manipulation -------------------------------------------------
    if (m == "fabs") {
        auto [d, a] = freg2();
        return with_flush(F(d) + ".u64 = " + F(a) + ".u64 & ~0x8000000000000000;");
    }
    if (m == "fnabs") {
        auto [d, a] = freg2();
        return with_flush(F(d) + ".u64 = " + F(a) + ".u64 | 0x8000000000000000;");
    }
    if (m == "fneg") {
        auto [d, a] = freg2();
        return with_flush(F(d) + ".u64 = " + F(a) + ".u64 ^ 0x8000000000000000;");
    }

    // ---- Move and Conversion ----------------------------------------------
    if (m == "fmr") {
        auto [d, a] = freg2();
        return with_flush(F(d) + ".f64 = " + F(a) + ".f64;");
    }
    if (m == "fcfid") {
        auto [d, a] = freg2();
        return with_flush(F(d) + ".f64 = double(" + F(a) + ".s64);");
    }
    if (m == "fctid") {
        auto [d, a] = freg2();
        return with_flush(F(d) + ".s64 = std::isnan(" + F(a) + ".f64) ? int64_t(0x8000000000000000ULL) : (" + F(a) +
                          ".f64 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvtsd_si64(simde_mm_load_sd(&" + F(a) + ".f64));");
    }
    if (m == "fctidz") {
        auto [d, a] = freg2();
        return with_flush(F(d) + ".s64 = std::isnan(" + F(a) + ".f64) ? int64_t(0x8000000000000000ULL) : (" + F(a) +
                          ".f64 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&" + F(a) + ".f64));");
    }
    if (m == "fctiwz") {
        auto [d, a] = freg2();
        return with_flush(F(d) + ".s64 = std::isnan(" + F(a) + ".f64) ? int64_t(0x80000000U) : (" + F(a) +
                          ".f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&" + F(a) + ".f64));");
    }
    if (m == "frsp") {
        auto [d, a] = freg2();
        return with_flush(F(d) + ".f64 = double(float(" + F(a) + ".f64));");
    }

    // ---- Comparison --------------------------------------------------------
    // fcmpu: operands[0]=crfD, operands[1]=fA, operands[2]=fB.
    // fcmpo is translated as fcmpu (the same statements).
    if (m == "fcmpu" || m == "fcmpo") {
        Number c = _cr(ops[0]);
        Number a = _freg(ops[1]);
        Number b = _freg(ops[2]);
        return with_flush(py::str("ctx.cr") + num(c) + ".compare(" + F(a) + ".f64, " + F(b) + ".f64);");
    }

    // ---- Addition ----------------------------------------------------------
    if (m == "fadd") {
        auto [d, a, b] = freg3();
        return with_flush(F(d) + ".f64 = " + F(a) + ".f64 + " + F(b) + ".f64;");
    }
    if (m == "fadds") {
        auto [d, a, b] = freg3();
        return with_flush(F(d) + ".f64 = double(float(" + F(a) + ".f64 + " + F(b) + ".f64));");
    }

    // ---- Subtraction -------------------------------------------------------
    if (m == "fsub") {
        auto [d, a, b] = freg3();
        return with_flush(F(d) + ".f64 = " + F(a) + ".f64 - " + F(b) + ".f64;");
    }
    if (m == "fsubs") {
        auto [d, a, b] = freg3();
        return with_flush(F(d) + ".f64 = double(float(" + F(a) + ".f64 - " + F(b) + ".f64));");
    }

    // ---- Multiplication ----------------------------------------------------
    if (m == "fmul") {
        auto [d, a, b] = freg3();
        return with_flush(F(d) + ".f64 = " + F(a) + ".f64 * " + F(b) + ".f64;");
    }
    if (m == "fmuls") {
        auto [d, a, b] = freg3();
        return with_flush(F(d) + ".f64 = double(float(" + F(a) + ".f64 * " + F(b) + ".f64));");
    }

    // ---- Division ----------------------------------------------------------
    if (m == "fdiv") {
        auto [d, a, b] = freg3();
        return with_flush(F(d) + ".f64 = " + F(a) + ".f64 / " + F(b) + ".f64;");
    }
    if (m == "fdivs") {
        auto [d, a, b] = freg3();
        return with_flush(F(d) + ".f64 = double(float(" + F(a) + ".f64 / " + F(b) + ".f64));");
    }

    // ---- Fused Multiply-Add ------------------------------------------------
    // operands[1]*operands[2] (+/-) operands[3]  (index order reproduced verbatim)
    if (m == "fmadd") {
        auto [d, a, c, b] = freg4();
        return with_flush(F(d) + ".f64 = " + F(a) + ".f64 * " + F(c) + ".f64 + " + F(b) + ".f64;");
    }
    if (m == "fmadds") {
        auto [d, a, c, b] = freg4();
        return with_flush(F(d) + ".f64 = double(float(" + F(a) + ".f64 * " + F(c) + ".f64 + " + F(b) + ".f64));");
    }
    if (m == "fmsub") {
        auto [d, a, c, b] = freg4();
        return with_flush(F(d) + ".f64 = " + F(a) + ".f64 * " + F(c) + ".f64 - " + F(b) + ".f64;");
    }
    if (m == "fmsubs") {
        auto [d, a, c, b] = freg4();
        return with_flush(F(d) + ".f64 = double(float(" + F(a) + ".f64 * " + F(c) + ".f64 - " + F(b) + ".f64));");
    }
    if (m == "fnmadd") {
        auto [d, a, c, b] = freg4();
        return with_flush(F(d) + ".f64 = -(" + F(a) + ".f64 * " + F(c) + ".f64 + " + F(b) + ".f64);");
    }
    if (m == "fnmadds") {
        auto [d, a, c, b] = freg4();
        return with_flush(F(d) + ".f64 = double(float(-(" + F(a) + ".f64 * " + F(c) + ".f64 + " + F(b) + ".f64)));");
    }
    if (m == "fnmsub") {
        auto [d, a, c, b] = freg4();
        return with_flush(F(d) + ".f64 = -(" + F(a) + ".f64 * " + F(c) + ".f64 - " + F(b) + ".f64);");
    }
    if (m == "fnmsubs") {
        auto [d, a, c, b] = freg4();
        return with_flush(F(d) + ".f64 = double(float(-(" + F(a) + ".f64 * " + F(c) + ".f64 - " + F(b) + ".f64)));");
    }

    // ---- Reciprocal and Square Root ---------------------------------------
    if (m == "fres") {
        auto [d, a] = freg2();
        return with_flush(F(d) + ".f64 = double(float(1.0 / " + F(a) + ".f64));");
    }
    if (m == "frsqrte") {
        auto [d, a] = freg2();
        return with_flush(F(d) + ".f64 = 1.0 / sqrt(" + F(a) + ".f64);");
    }
    if (m == "fsqrt") {
        auto [d, a] = freg2();
        return with_flush(F(d) + ".f64 = sqrt(" + F(a) + ".f64);");
    }
    if (m == "fsqrts") {
        auto [d, a] = freg2();
        return with_flush(F(d) + ".f64 = double(float(sqrt(" + F(a) + ".f64)));");
    }

    // ---- Selection ---------------------------------------------------------
    // operands[1] >= 0.0 ? operands[2] : operands[3]
    if (m == "fsel") {
        auto [d, a, c, b] = freg4();
        return with_flush(F(d) + ".f64 = " + F(a) + ".f64 >= 0.0 ? " + F(c) + ".f64 : " + F(b) + ".f64;");
    }

    return std::nullopt;
}

const py::str RUNTIME_HELPERS = py::str("#include <cmath>\n#include <climits>\n");

const py::list<py::str> MNEMONICS = {
    py::str("fabs"), py::str("fnabs"), py::str("fneg"),
    py::str("fmr"), py::str("fcfid"), py::str("fctid"), py::str("fctidz"), py::str("fctiwz"), py::str("frsp"),
    py::str("fcmpu"), py::str("fcmpo"),
    py::str("fadd"), py::str("fadds"),
    py::str("fsub"), py::str("fsubs"),
    py::str("fmul"), py::str("fmuls"),
    py::str("fdiv"), py::str("fdivs"),
    py::str("fmadd"), py::str("fmadds"), py::str("fmsub"), py::str("fmsubs"),
    py::str("fnmadd"), py::str("fnmadds"), py::str("fnmsub"), py::str("fnmsubs"),
    py::str("fres"), py::str("frsqrte"), py::str("fsqrt"), py::str("fsqrts"),
    py::str("fsel"),
};

}  // namespace codebreaker::recompile::builders::floating
