// codebreaker/recompile/builders/vector.cpp -- C++17 port of codebreaker.recompile.builders.vector (see vector.hpp).

#include "codebreaker/recompile/builders/vector.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
#include <tuple>
#include <vector>

namespace codebreaker::recompile::builders::vector {

namespace {

using Number = std::optional<int64_t>;
using S = std::string;

// "%d" of a number that is None when the operand token was not of the kind asked for: the TypeError Python raises
S D(const Number& n) {
    if (!n) throw py::TypeError("%d format: a real number is required, not NoneType");
    return std::to_string(*n);
}
S D(int64_t n) { return std::to_string(n); }

// the number, or the TypeError Python raises for arithmetic on None
int64_t need(const Number& n) {
    if (!n) throw py::TypeError("unsupported operand type(s): 'NoneType'");
    return *n;
}

// "%X" of an int (Python writes a negative number as "-" and the magnitude)
S X(int64_t n) {
    char buf[40];
    if (n < 0) std::snprintf(buf, sizeof buf, "-%llX", static_cast<unsigned long long>(0ULL - static_cast<uint64_t>(n)));
    else std::snprintf(buf, sizeof buf, "%llX", static_cast<unsigned long long>(n));
    return buf;
}

// Python's `template % (args...)` for the %d / %X / %s conversions: the args are already formatted, in order
S fmt(const char* t, std::initializer_list<S> args) {
    S out;
    auto it = args.begin();
    for (const char* p = t; *p; ++p) {
        if (*p == '%' && (p[1] == 'd' || p[1] == 'X' || p[1] == 's')) {
            out += *it++;
            ++p;
        } else {
            out += *p;
        }
    }
    return out;
}

// the statements of one instruction, built up line by line
struct Lines {
    py::list<py::str> v;
    void add(const S& s) { v.append(py::str(s)); }
};

// `u != 0` for an immediate that is None when the token was not a number
bool nonzero(const Number& u) { return !u || *u != 0; }

// "%d" % (1 << u), u a shift count: ValueError for a negative count (a huge count is accepted up to a
// limit; Python would build the big integer)
S pow2_text(int64_t u) {
    if (u < 0) throw py::ValueError("negative shift count");
    if (u > 100000) throw py::OverflowError("shift count too large");
    std::vector<int> digits{1};   // little-endian decimal digits
    for (int64_t i = 0; i < u; ++i) {
        int carry = 0;
        for (auto& dg : digits) {
            int x = dg * 2 + carry;
            dg = x % 10;
            carry = x / 10;
        }
        if (carry) digits.push_back(carry);
    }
    S out;
    for (auto it = digits.rbegin(); it != digits.rend(); ++it) out += static_cast<char>('0' + *it);
    return out;
}

// struct.unpack("<I", struct.pack("<f", 2.0 ** (-u)))[0]
int64_t scale_bits(int64_t u) {
    int64_t e = -u;
    if (e > 1023) throw py::OverflowError("(34, 'Numerical result out of range')");
    double v = e < -1074 ? 0.0 : std::ldexp(1.0, static_cast<int>(e));
    float f = static_cast<float>(v);
    if (std::isinf(f)) throw py::OverflowError("float too large to pack with f format");
    uint32_t bits;
    std::memcpy(&bits, &f, sizeof bits);
    return bits;
}

const S FLUSH = "ctx.fpscr.enableFlushMode();";

// emit_vec_fp_binary
S vfp2(const S& op, const Number& d, const Number& a, const Number& b) {
    return fmt("simde_mm_store_ps(ctx.v%d.f32, simde_mm_%s_ps(simde_mm_load_ps(ctx.v%d.f32), simde_mm_load_ps(ctx.v%d.f32)));",
               {D(d), op, D(a), D(b)});
}

// emit_vec_int_binary (a,b already in emitted order; swapped callers pass b,a)
S vint2(const S& op, const S& ty, const Number& d, const Number& a, const Number& b) {
    return fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.%s, simde_mm_%s("
               "simde_mm_load_si128((simde__m128i*)ctx.v%d.%s), "
               "simde_mm_load_si128((simde__m128i*)ctx.v%d.%s)));",
               {D(d), ty, op, D(a), ty, D(b), ty});
}

// a store of rex::<helper>(load vA, load vB) into vD, all three as `ty` lanes
S rex3(const S& ty, const S& helper, const Number& d, const Number& a, const Number& b) {
    return fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.%s, rex::%s("
               "simde_mm_load_si128((simde__m128i*)ctx.v%d.%s), "
               "simde_mm_load_si128((simde__m128i*)ctx.v%d.%s)));",
               {D(d), ty, helper, D(a), ty, D(b), ty});
}

// the ctx.cr6.setFromMask(...) statement of a record-form integer compare
S cr6_si128(const Number& d, const S& ty, const S& mask) {
    return fmt("ctx.cr6.setFromMask(simde_mm_load_si128((simde__m128i*)ctx.v%d.%s), %s);", {D(d), ty, mask});
}

}  // namespace

std::optional<py::list<py::str>> emit(const py::str& mn_s, const py::list<py::str>& ops,
                                      const OperandNumber& _reg, const OperandNumber& _vreg, const OperandNumber& _freg,
                                      const OperandNumber& _imm, const OperandNumber& _cr, const MemOperand& _mem,
                                      const RegisterValue& _rv, const BaseExpression& _base_expr, const AddressExpression& _addr,
                                      int64_t va, int64_t WORD) {
    (void)_reg; (void)_freg; (void)_cr; (void)_mem; (void)_rv; (void)_base_expr; (void)_addr; (void)va; (void)WORD;
    const S mn = mn_s.utf8();

    // a record form is a mnemonic ending in '.'
    const bool rec = !mn.empty() && mn.back() == '.';
    const S core = rec ? mn.substr(0, mn.size() - 1) : mn;

    // _r(k): ops[k] as a vector register (IndexError when the operand is missing)
    auto _r = [&](int64_t k) -> Number { return _vreg(ops[k]); };
    // the immediate of operand k
    auto im = [&](int64_t k) -> Number { return _imm(ops[k]); };
    auto is = [&](std::initializer_list<const char*> names) {
        for (const char* n : names)
            if (core == n) return true;
        return false;
    };

    // =======================================================================
    // Vector Floating Point Arithmetic
    // =======================================================================
    if (is({"vaddfp", "vaddfp128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o; o.add(FLUSH); o.add(vfp2("add", d, a, b));
        return o.v;
    }
    if (is({"vsubfp", "vsubfp128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o; o.add(FLUSH); o.add(vfp2("sub", d, a, b));
        return o.v;
    }
    if (core == "vmulfp128") {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o; o.add(FLUSH); o.add(vfp2("mul", d, a, b));
        return o.v;
    }
    if (is({"vmaddfp", "vmaddfp128", "vmaddcfp128"})) {
        Number d = _r(0), a = _r(1), c = _r(2), b = _r(3);
        Lines o; o.add(FLUSH);
        o.add(fmt("simde_mm_store_ps(ctx.v%d.f32, simde_mm_add_ps(simde_mm_mul_ps("
                  "simde_mm_load_ps(ctx.v%d.f32), simde_mm_load_ps(ctx.v%d.f32)), "
                  "simde_mm_load_ps(ctx.v%d.f32)));", {D(d), D(a), D(c), D(b)}));
        return o.v;
    }
    if (is({"vnmsubfp", "vnmsubfp128"})) {
        Number d = _r(0), a = _r(1), c = _r(2), b = _r(3);
        Lines o; o.add(FLUSH);
        o.add(fmt("simde_mm_store_ps(ctx.v%d.f32, simde_mm_xor_ps(simde_mm_sub_ps("
                  "simde_mm_mul_ps(simde_mm_load_ps(ctx.v%d.f32), simde_mm_load_ps(ctx.v%d.f32)), "
                  "simde_mm_load_ps(ctx.v%d.f32)), "
                  "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));", {D(d), D(a), D(c), D(b)}));
        return o.v;
    }
    if (is({"vmaxfp", "vmaxfp128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o; o.add(FLUSH); o.add(vfp2("max", d, a, b));
        return o.v;
    }
    if (is({"vminfp", "vminfp128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o; o.add(FLUSH); o.add(vfp2("min", d, a, b));
        return o.v;
    }
    if (is({"vrefp", "vrefp128"})) {
        Number d = _r(0), a = _r(1);
        Lines o; o.add(FLUSH);
        o.add(fmt("simde_mm_store_ps(ctx.v%d.f32, simde_mm_div_ps(simde_mm_set1_ps(1), "
                  "simde_mm_load_ps(ctx.v%d.f32)));", {D(d), D(a)}));
        return o.v;
    }
    if (is({"vrsqrtefp", "vrsqrtefp128"})) {
        Number d = _r(0), a = _r(1);
        Lines o; o.add(FLUSH);
        o.add(fmt("simde_mm_store_ps(ctx.v%d.f32, simde_mm_div_ps(simde_mm_set1_ps(1), "
                  "simde_mm_sqrt_ps(simde_mm_load_ps(ctx.v%d.f32))));", {D(d), D(a)}));
        return o.v;
    }
    if (is({"vexptefp", "vexptefp128"})) {
        Number d = _r(0), a = _r(1);
        Lines o; o.add(FLUSH);
        for (int64_t i = 0; i < 4; ++i) o.add(fmt("ctx.v%d.f32[%d] = exp2f(ctx.v%d.f32[%d]);", {D(d), D(i), D(a), D(i)}));
        return o.v;
    }
    if (is({"vlogefp", "vlogefp128"})) {
        Number d = _r(0), a = _r(1);
        Lines o; o.add(FLUSH);
        for (int64_t i = 0; i < 4; ++i) o.add(fmt("ctx.v%d.f32[%d] = log2f(ctx.v%d.f32[%d]);", {D(d), D(i), D(a), D(i)}));
        return o.v;
    }

    // =======================================================================
    // Vector Dot Products (VMX128)
    // =======================================================================
    if (core == "vmsum3fp128" || core == "vmsum4fp128") {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o; o.add(FLUSH);
        o.add(fmt("simde_mm_store_ps(ctx.v%d.f32, simde_mm_dp_ps("
                  "simde_mm_load_ps(ctx.v%d.f32), simde_mm_load_ps(ctx.v%d.f32), %s));",
                  {D(d), D(a), D(b), core == "vmsum3fp128" ? "0xEF" : "0xFF"}));
        return o.v;
    }

    // =======================================================================
    // Vector Rounding
    // =======================================================================
    {
        const std::pair<const char*, const char*> round_modes[] = {
            {"vrfim", "SIMDE_MM_FROUND_TO_NEG_INF | SIMDE_MM_FROUND_NO_EXC"},
            {"vrfin", "SIMDE_MM_FROUND_TO_NEAREST_INT | SIMDE_MM_FROUND_NO_EXC"},
            {"vrfip", "SIMDE_MM_FROUND_TO_POS_INF | SIMDE_MM_FROUND_NO_EXC"},
            {"vrfiz", "SIMDE_MM_FROUND_TO_ZERO | SIMDE_MM_FROUND_NO_EXC"},
        };
        // (the "128" alias maps to its base form)
        const bool alias = core.size() > 3 && core.compare(core.size() - 3, 3, "128") == 0;
        for (const auto& e : round_modes) {
            if (core == e.first || (alias && core.substr(0, core.size() - 3) == e.first)) {
                Number d = _r(0), a = _r(1);
                Lines o; o.add(FLUSH);
                o.add(fmt("simde_mm_store_ps(ctx.v%d.f32, simde_mm_round_ps(simde_mm_load_ps(ctx.v%d.f32), %s));",
                          {D(d), D(a), e.second}));
                return o.v;
            }
        }
    }

    // =======================================================================
    // Vector Integer Arithmetic
    // =======================================================================
    {
        // mnemonic -> (simde op, lane type)
        static const std::tuple<const char*, const char*, const char*> intbin[] = {
            {"vaddsbs", "adds_epi8", "s8"},  {"vaddshs", "adds_epi16", "s16"},
            {"vaddubm", "add_epi8", "u8"},   {"vaddubs", "adds_epu8", "u8"},
            {"vadduhm", "add_epi16", "u16"}, {"vadduwm", "add_epi32", "u32"},
            {"vadduhs", "adds_epu16", "u16"},
            {"vsubsbs", "subs_epi8", "s8"},  {"vsububm", "sub_epi8", "u8"},
            {"vsububs", "subs_epu8", "u8"},  {"vsubuhs", "subs_epu16", "u16"},
            {"vsubuhm", "sub_epi16", "u16"}, {"vsubuwm", "sub_epi32", "u32"},
            {"vsubshs", "subs_epi16", "s16"},
            {"vmaxsw", "max_epi32", "s32"},  {"vmaxsh", "max_epi16", "s16"},
            {"vmaxsb", "max_epi8", "s8"},    {"vminsh", "min_epi16", "s16"},
            {"vminsb", "min_epi8", "s8"},    {"vminsw", "min_epi32", "s32"},
            {"vmaxuh", "max_epu16", "u16"},  {"vminuh", "min_epu16", "u16"},
            {"vmaxub", "max_epu8", "u8"},    {"vminub", "min_epu8", "u8"},
            {"vavgub", "avg_epu8", "u8"},    {"vavguh", "avg_epu16", "u16"},
        };
        for (const auto& e : intbin) {
            if (core == std::get<0>(e)) {
                Number d = _r(0), a = _r(1), b = _r(2);
                Lines o; o.add(vint2(std::get<1>(e), std::get<2>(e), d, a, b));
                return o.v;
            }
        }
    }

    if (core == "vaddsws") {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add("{");
        o.add(fmt("simde__m128i a = simde_mm_load_si128((simde__m128i*)ctx.v%d.u8);", {D(a)}));
        o.add(fmt("simde__m128i b = simde_mm_load_si128((simde__m128i*)ctx.v%d.u8);", {D(b)}));
        o.add("simde__m128i sum = simde_mm_add_epi32(a, b);");
        o.add("simde__m128i overflow = simde_mm_and_si128(simde_mm_xor_si128(a, sum), "
              "simde_mm_xor_si128(b, sum));");
        o.add("simde__m128i sat_val = simde_mm_xor_si128(simde_mm_srai_epi32(a, 31), "
              "simde_mm_set1_epi32(0x7FFFFFFF));");
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_blendv_epi8("
                  "sum, sat_val, overflow));", {D(d)}));
        o.add("}");
        return o.v;
    }

    if (core == "vadduws") {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u32, rex::simde_mm_adds_epu32("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)));", {D(d), D(a), D(b)}));
        return o.v;
    }

    if (core == "vsubsws") {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        for (int64_t i = 0; i < 4; ++i) {
            o.add(fmt("temp.s64 = int64_t(ctx.v%d.s32[%d]) - int64_t(ctx.v%d.s32[%d]);", {D(a), D(i), D(b), D(i)}));
            o.add(fmt("ctx.v%d.s32[%d] = temp.s64 > INT_MAX ? INT_MAX : "
                      "temp.s64 < INT_MIN ? INT_MIN : temp.s64;", {D(d), D(i)}));
        }
        return o.v;
    }

    if (core == "vsubuws") {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u32, simde_mm_sub_epi32("
                  "simde_mm_load_si128((simde__m128i*) ctx.v%d.u32), "
                  "simde_mm_min_epu32(simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32))));", {D(d), D(a), D(a), D(b)}));
        return o.v;
    }

    // =======================================================================
    // Vector Average (rex helpers for the signed forms)
    // =======================================================================
    if (core == "vavgsb") {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o; o.add(rex3("u8", "simde_mm_avg_epi8", d, a, b));
        return o.v;
    }
    if (core == "vavgsh") {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o; o.add(rex3("u16", "simde_mm_avg_epi16", d, a, b));
        return o.v;
    }
    if (core == "vavgsw") {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o; o.add(rex3("s32", "simde_mm_avg_epi32", d, a, b));
        return o.v;
    }

    // =======================================================================
    // Vector Logical
    // =======================================================================
    if (is({"vand", "vand128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o; o.add(vint2("and_si128", "u8", d, a, b));
        return o.v;
    }
    if (is({"vandc", "vandc128"})) {
        // vD = vA & ~vB : emit_vec_int_binary_swapped(andnot_si128, u8) -> op(vB, vA)
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o; o.add(vint2("andnot_si128", "u8", d, b, a));
        return o.v;
    }
    if (is({"vor", "vor128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        S body;
        if (a != b) {
            body = fmt("simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                       "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)));", {D(a), D(b)});
        } else {
            body = fmt("simde_mm_load_si128((simde__m128i*)ctx.v%d.u8));", {D(a)});
        }
        Lines o; o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, %s", {D(d), body}));
        return o.v;
    }
    if (is({"vxor", "vxor128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        S body;
        if (a != b) {
            body = fmt("simde_mm_xor_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                       "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)));", {D(a), D(b)});
        } else {
            body = "simde_mm_setzero_si128());";
        }
        Lines o; o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, %s", {D(d), body}));
        return o.v;
    }
    if (is({"vnor", "vnor128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_xor_si128("
                  "simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)), "
                  "simde_mm_set1_epi32(-1)));", {D(d), D(a), D(b)}));
        return o.v;
    }
    if (is({"vsel", "vsel128"})) {
        Number d = _r(0);
        Number a = _r(1), b = _r(2), c = _r(3);  // operands: vD, vA, vB, vC(mask)
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_or_si128("
                  "simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)), "
                  "simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8))));", {D(d), D(c), D(a), D(c), D(b)}));
        return o.v;
    }

    // =======================================================================
    // Vector Compare (record forms update CR6 via setFromMask)
    // =======================================================================
    if (is({"vcmpbfp", "vcmpbfp128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o; o.add(FLUSH);
        o.add(fmt("simde_mm_store_ps(vTemp.f32, simde_mm_and_ps(simde_mm_cmpgt_ps("
                  "simde_mm_load_ps(ctx.v%d.f32), simde_mm_load_ps(ctx.v%d.f32)), "
                  "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));", {D(a), D(b)}));
        o.add(fmt("simde_mm_store_ps(ctx.v%d.f32, simde_mm_and_ps(simde_mm_cmplt_ps("
                  "simde_mm_load_ps(ctx.v%d.f32), simde_mm_xor_ps(simde_mm_load_ps(ctx.v%d.f32), "
                  "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000))))), "
                  "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x40000000)))));", {D(d), D(a), D(b)}));
        o.add(fmt("simde_mm_store_ps(ctx.v%d.f32, simde_mm_or_ps(simde_mm_load_ps(vTemp.f32), "
                  "simde_mm_load_ps(ctx.v%d.f32)));", {D(d), D(d)}));
        if (rec) {
            o.add(fmt("ctx.cr6.setFromMask(simde_mm_castsi128_ps(simde_mm_or_si128("
                      "simde_mm_load_si128((simde__m128i*)ctx.v%d.f32), "
                      "simde_mm_slli_epi32(simde_mm_load_si128((simde__m128i*)ctx.v%d.f32), 1))), 0xF);",
                      {D(d), D(d)}));
        }
        return o.v;
    }

    {
        const std::pair<const char*, const char*> fcmp[] = {
            {"vcmpeqfp", "cmpeq"}, {"vcmpeqfp128", "cmpeq"}, {"vcmpgefp", "cmpge"}, {"vcmpgefp128", "cmpge"},
            {"vcmpgtfp", "cmpgt"}, {"vcmpgtfp128", "cmpgt"},
        };
        for (const auto& e : fcmp) {
            if (core == e.first) {
                Number d = _r(0);
                Number a = _r(1), b = _r(2);
                Lines o; o.add(FLUSH); o.add(vfp2(e.second, d, a, b));
                if (rec) o.add(fmt("ctx.cr6.setFromMask(simde_mm_load_ps(ctx.v%d.f32), 0xF);", {D(d)}));
                return o.v;
            }
        }
    }

    if (core == "vcmpequb") {
        Number d = _r(0);
        Number a = _r(1), b = _r(2);
        Lines o; o.add(vint2("cmpeq_epi8", "u8", d, a, b));
        if (rec) o.add(cr6_si128(d, "u8", "0xFFFF"));
        return o.v;
    }
    if (core == "vcmpequh") {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_cmpeq_epi16("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16)));", {D(d), D(a), D(b)}));
        if (rec) o.add(cr6_si128(d, "u16", "0xFFFF"));
        return o.v;
    }
    if (is({"vcmpequw", "vcmpequw128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_cmpeq_epi32("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)));", {D(d), D(a), D(b)}));
        if (rec) o.add(fmt("ctx.cr6.setFromMask(simde_mm_load_ps(ctx.v%d.f32), 0xF);", {D(d)}));
        return o.v;
    }
    if (core == "vcmpgtub") {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, rex::simde_mm_cmpgt_epu8("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)));", {D(d), D(a), D(b)}));
        if (rec) o.add(cr6_si128(d, "u8", "0xFFFF"));
        return o.v;
    }
    if (core == "vcmpgtuh") {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, rex::simde_mm_cmpgt_epu16("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16)));", {D(d), D(a), D(b)}));
        if (rec) o.add(cr6_si128(d, "u16", "0xFFFF"));
        return o.v;
    }
    if (core == "vcmpgtuw") {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u32, simde_mm_cmpgt_epi32("
                  "simde_mm_xor_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), "
                  "simde_mm_set1_epi32((int32_t)0x80000000)), "
                  "simde_mm_xor_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), "
                  "simde_mm_set1_epi32((int32_t)0x80000000))));", {D(d), D(a), D(b)}));
        if (rec) o.add(fmt("ctx.cr6.setFromMask(simde_mm_castsi128_ps("
                           "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)), 0xF);", {D(d)}));
        return o.v;
    }
    if (core == "vcmpgtsb") {
        Number d = _r(0);
        Number a = _r(1), b = _r(2);
        Lines o; o.add(vint2("cmpgt_epi8", "u8", d, a, b));
        if (rec) o.add(cr6_si128(d, "u8", "0xFFFF"));
        return o.v;
    }
    if (core == "vcmpgtsh") {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_cmpgt_epi16("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16)));", {D(d), D(a), D(b)}));
        if (rec) o.add(cr6_si128(d, "u16", "0xFFFF"));
        return o.v;
    }
    if (core == "vcmpgtsw") {
        Number d = _r(0);
        Number a = _r(1), b = _r(2);
        Lines o; o.add(vint2("cmpgt_epi32", "u32", d, a, b));
        if (rec) o.add(fmt("ctx.cr6.setFromMask(simde_mm_castsi128_ps("
                           "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)), 0xF);", {D(d)}));
        return o.v;
    }

    // =======================================================================
    // Vector Conversion
    // =======================================================================
    if (is({"vctsxs", "vcfpsxws128", "vctuxs", "vcfpuxws128"})) {
        const bool signed_form = core == "vctsxs" || core == "vcfpsxws128";
        Number d = _r(0), a = _r(1);
        Number u = im(2);
        S inner;
        if (nonzero(u)) {
            S scale = pow2_text(need(u));   // (1 << u)
            inner = fmt("simde_mm_mul_ps(simde_mm_load_ps(ctx.v%d.f32), simde_mm_set1_ps(%d))", {D(a), scale});
        } else {
            inner = fmt("simde_mm_load_ps(ctx.v%d.f32)", {D(a)});
        }
        Lines o; o.add(FLUSH);
        if (signed_form)
            o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.s32, rex::simde_mm_vctsxs(%s));", {D(d), inner}));
        else
            o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u32, rex::simde_mm_vctuxs(%s));", {D(d), inner}));
        return o.v;
    }
    if (is({"vcfsx", "vcsxwfp128", "vcfux", "vcuxwfp128"})) {
        const bool signed_form = core == "vcfsx" || core == "vcsxwfp128";
        Number d = _r(0), a = _r(1);
        Number u = im(2);
        Lines o; o.add(FLUSH);
        if (nonzero(u)) {
            int64_t bits = scale_bits(need(u));
            if (signed_form)
                o.add(fmt("simde_mm_store_ps(ctx.v%d.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps("
                          "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)), "
                          "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x%X)))));", {D(d), D(a), X(bits)}));
            else
                o.add(fmt("simde_mm_store_ps(ctx.v%d.f32, simde_mm_mul_ps(rex::simde_mm_cvtepu32_ps_("
                          "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)), "
                          "simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x%X)))));", {D(d), D(a), X(bits)}));
        } else {
            if (signed_form)
                o.add(fmt("simde_mm_store_ps(ctx.v%d.f32, simde_mm_cvtepi32_ps("
                          "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)));", {D(d), D(a)}));
            else
                o.add(fmt("simde_mm_store_ps(ctx.v%d.f32, rex::simde_mm_cvtepu32_ps_("
                          "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32)));", {D(d), D(a)}));
        }
        return o.v;
    }

    // =======================================================================
    // Vector Merge (emit_vec_int_binary_swapped)
    // =======================================================================
    {
        static const std::tuple<const char*, const char*, const char*> merge[] = {
            {"vmrghb", "unpackhi_epi8", "u8"},  {"vmrghh", "unpackhi_epi16", "u16"},
            {"vmrghw", "unpackhi_epi32", "u32"},
            {"vmrglb", "unpacklo_epi8", "u8"},  {"vmrglh", "unpacklo_epi16", "u16"},
            {"vmrglw", "unpacklo_epi32", "u32"},
        };
        // (the "128" alias maps to its base form)
        const bool alias = core.size() > 3 && core.compare(core.size() - 3, 3, "128") == 0;
        for (const auto& e : merge) {
            if (core == std::get<0>(e) || (alias && core.substr(0, core.size() - 3) == std::get<0>(e))) {
                Number d = _r(0), b = _r(2), a = _r(1);
                Lines o; o.add(vint2(std::get<1>(e), std::get<2>(e), d, b, a));  // swapped
                return o.v;
            }
        }
    }

    // =======================================================================
    // Vector Permute
    // =======================================================================
    if (is({"vperm", "vperm128"})) {
        Number d = _r(0), a = _r(1), b = _r(2), c = _r(3);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, rex::simde_mm_perm_epi8_("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8)));", {D(d), D(a), D(b), D(c)}));
        return o.v;
    }
    if (core == "vpermwi128") {
        Number d = _r(0), a = _r(1);
        int64_t imm = need(im(2));
        int64_t x = 3 - (imm & 0x3);
        int64_t y = 3 - ((imm >> 2) & 0x3);
        int64_t z = 3 - ((imm >> 4) & 0x3);
        int64_t w = 3 - ((imm >> 6) & 0x3);
        int64_t perm = x | (y << 2) | (z << 4) | (w << 6);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u32, simde_mm_shuffle_epi32("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), 0x%X));", {D(d), D(a), X(perm)}));
        return o.v;
    }
    if (core == "vrlimi128") {
        Number d = _r(0), a = _r(1);
        Number blend = im(2);
        Number idx = im(3);
        const int64_t shuffles[4] = {(3 << 6) | (2 << 4) | (1 << 2) | 0,
                                     (2 << 6) | (1 << 4) | (0 << 2) | 3,
                                     (1 << 6) | (0 << 4) | (3 << 2) | 2,
                                     (0 << 6) | (3 << 4) | (2 << 2) | 1};
        // shuffles[idx]: a list index (negative counts from the end; IndexError out of range)
        int64_t k = need(idx);
        if (k < -4 || k >= 4) throw py::IndexError("list index out of range");
        int64_t shuffle = shuffles[k < 0 ? k + 4 : k];
        Lines o;
        o.add(fmt("simde_mm_store_ps(ctx.v%d.f32, simde_mm_blend_ps("
                  "simde_mm_load_ps(ctx.v%d.f32), simde_mm_permute_ps("
                  "simde_mm_load_ps(ctx.v%d.f32), %d), %d));", {D(d), D(d), D(a), D(shuffle), D(blend)}));
        return o.v;
    }

    // =======================================================================
    // Vector Shift / Rotate
    // =======================================================================
    if (core == "vsl" || core == "vslo" || core == "vslo128" || core == "vsro" || core == "vsro128" || core == "vsr") {
        Number d = _r(0), a = _r(1), b = _r(2);
        const char* helper = core == "vsl" ? "simde_mm_vsl"
                             : (core == "vslo" || core == "vslo128") ? "simde_mm_vslo"
                             : (core == "vsr") ? "simde_mm_vsr" : "simde_mm_vsro";
        Lines o; o.add(rex3("u8", helper, d, a, b));
        return o.v;
    }
    if (is({"vsldoi", "vsldoi128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Number sh = im(3);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_alignr_epi8("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), %d));", {D(d), D(a), D(b), D(16 - need(sh))}));
        return o.v;
    }
    // per-lane shifts: "ctx.v<d>.<dty>[i] = ctx.v<a>.<dty>[i] <op> (ctx.v<b>.<bty>[bi] & <mask>);"
    {
        struct LaneShift { const char* mn; const char* dty; const char* op; const char* bty; const char* mask; int lanes; int bstride; };
        static const LaneShift lane_shifts[] = {
            {"vslb", "u8", "<<", "u8", "0x7", 16, 1},    {"vslh", "u16", "<<", "u16", "0xF", 8, 1},
            {"vsrh", "u16", ">>", "u16", "0xF", 8, 1},   {"vsrb", "u8", ">>", "u8", "0x7", 16, 1},
            {"vsrab", "s8", ">>", "u8", "0x7", 16, 1},   {"vsrah", "s16", ">>", "u16", "0xF", 8, 1},
            {"vslw", "u32", "<<", "u8", "0x1F", 4, 4},   {"vslw128", "u32", "<<", "u8", "0x1F", 4, 4},
            {"vsraw", "s32", ">>", "u8", "0x1F", 4, 4},  {"vsraw128", "s32", ">>", "u8", "0x1F", 4, 4},
            {"vsrw", "u32", ">>", "u8", "0x1F", 4, 4},   {"vsrw128", "u32", ">>", "u8", "0x1F", 4, 4},
        };
        for (const auto& e : lane_shifts) {
            if (core == e.mn) {
                Number d = _r(0), a = _r(1), b = _r(2);
                Lines o;
                for (int64_t i = 0; i < e.lanes; ++i)
                    o.add(fmt("ctx.v%d.%s[%d] = ctx.v%d.%s[%d] %s (ctx.v%d.%s[%d] & %s);",
                              {D(d), e.dty, D(i), D(a), e.dty, D(i), e.op, D(b), e.bty, D(i * e.bstride), e.mask}));
                return o.v;
            }
        }
    }
    if (core == "vrlh") {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        for (int64_t i = 0; i < 8; ++i) {
            o.add(fmt("{ uint16_t sh = ctx.v%d.u16[%d] & 0xF;", {D(b), D(i)}));
            o.add(fmt("ctx.v%d.u16[%d] = (ctx.v%d.u16[%d] << sh) | "
                      "(sh ? (ctx.v%d.u16[%d] >> (16 - sh)) : 0); }", {D(d), D(i), D(a), D(i), D(a), D(i)}));
        }
        return o.v;
    }
    if (is({"vrlw", "vrlw128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        for (int64_t i = 0; i < 4; ++i) {
            o.add(fmt("{ uint32_t sh = ctx.v%d.u32[%d] & 0x1F;", {D(b), D(i)}));
            o.add(fmt("ctx.v%d.u32[%d] = (ctx.v%d.u32[%d] << sh) | "
                      "(sh ? (ctx.v%d.u32[%d] >> (32 - sh)) : 0); }", {D(d), D(i), D(a), D(i), D(a), D(i)}));
        }
        return o.v;
    }

    // =======================================================================
    // Vector Splat  (element-reversal math reproduced verbatim)
    // =======================================================================
    if (core == "vspltb") {
        Number d = _r(0), a = _r(1);
        int64_t perm = 15 - need(im(2));
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_shuffle_epi8("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u8), "
                  "simde_mm_set1_epi8(char(0x%X))));", {D(d), D(a), X(perm)}));
        return o.v;
    }
    if (core == "vsplth") {
        Number d = _r(0), a = _r(1);
        int64_t perm = 7 - need(im(2));
        perm = (perm * 2) | ((perm * 2 + 1) * 256);   // (perm*2) | ((perm*2+1) << 8)
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u16, simde_mm_shuffle_epi8("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), "
                  "simde_mm_set1_epi16(short(0x%X))));", {D(d), D(a), X(perm)}));
        return o.v;
    }
    if (is({"vspltw", "vspltw128"})) {
        Number d = _r(0), a = _r(1);
        int64_t perm = 3 - need(im(2));
        perm |= (perm * 4) | (perm * 16) | (perm * 64);   // (perm << 2) | (perm << 4) | (perm << 6)
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u32, simde_mm_shuffle_epi32("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), 0x%X));", {D(d), D(a), X(perm)}));
        return o.v;
    }
    if (is({"vspltisb", "vspltish", "vspltisw", "vspltisw128"})) {
        Number d = _r(0);
        int64_t s = need(im(1)) & 0x1F;
        if (s & 0x10) s -= 0x20;
        Lines o;
        if (core == "vspltisb")
            o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, "
                      "simde_mm_set1_epi8(char(0x%X)));", {D(d), X(s & 0xFF)}));
        else if (core == "vspltish")
            o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.s16, "
                      "simde_mm_set1_epi16(short(0x%X)));", {D(d), X(s & 0xFFFF)}));
        else
            o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u32, "
                      "simde_mm_set1_epi32(int(0x%X)));", {D(d), X(s & 0xFFFFFFFF)}));
        return o.v;
    }

    // =======================================================================
    // Vector Pack
    // =======================================================================
    if (is({"vpkshus", "vpkshus128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_packus_epi16("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.s16), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.s16)));", {D(d), D(b), D(a)}));
        return o.v;
    }
    if (is({"vpkuhum", "vpkuhum128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u8, simde_mm_packus_epi16("
                  "simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), "
                  "simde_mm_set1_epi16(0xFF)), "
                  "simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u16), "
                  "simde_mm_set1_epi16(0xFF))));", {D(d), D(b), D(a)}));
        return o.v;
    }
    if (is({"vpkuwum", "vpkuwum128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u16, simde_mm_packus_epi32("
                  "simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), "
                  "simde_mm_set1_epi32(0xFFFF)), "
                  "simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v%d.u32), "
                  "simde_mm_set1_epi32(0xFFFF))));", {D(d), D(b), D(a)}));
        return o.v;
    }
    if (is({"vpkshss", "vpkshss128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.s8, simde_mm_packs_epi16("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.s16), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.s16)));", {D(d), D(b), D(a)}));
        return o.v;
    }
    if (is({"vpkswss", "vpkswss128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.s16, simde_mm_packs_epi32("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.s32), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.s32)));", {D(d), D(b), D(a)}));
        return o.v;
    }
    if (is({"vpkswus", "vpkswus128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.u16, simde_mm_packus_epi32("
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.s32), "
                  "simde_mm_load_si128((simde__m128i*)ctx.v%d.s32)));", {D(d), D(b), D(a)}));
        return o.v;
    }
    if (is({"vpkuhus", "vpkuhus128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        for (int64_t i = 0; i < 8; ++i) {
            o.add(fmt("ctx.v%d.u8[%d] = ctx.v%d.u16[%d] > 0xFF ? 0xFF : (uint8_t)ctx.v%d.u16[%d];",
                      {D(d), D(15 - i), D(a), D(7 - i), D(a), D(7 - i)}));
            o.add(fmt("ctx.v%d.u8[%d] = ctx.v%d.u16[%d] > 0xFF ? 0xFF : (uint8_t)ctx.v%d.u16[%d];",
                      {D(d), D(7 - i), D(b), D(7 - i), D(b), D(7 - i)}));
        }
        return o.v;
    }
    if (is({"vpkuwus", "vpkuwus128"})) {
        Number d = _r(0), a = _r(1), b = _r(2);
        Lines o;
        for (int64_t i = 0; i < 4; ++i) {
            o.add(fmt("ctx.v%d.u16[%d] = ctx.v%d.u32[%d] > 0xFFFF ? 0xFFFF : (uint16_t)ctx.v%d.u32[%d];",
                      {D(d), D(7 - i), D(a), D(3 - i), D(a), D(3 - i)}));
            o.add(fmt("ctx.v%d.u16[%d] = ctx.v%d.u32[%d] > 0xFFFF ? 0xFFFF : (uint16_t)ctx.v%d.u32[%d];",
                      {D(d), D(3 - i), D(b), D(3 - i), D(b), D(3 - i)}));
        }
        return o.v;
    }

    if (core == "vpkd3d128") return _emit_vpkd3d128(ops, _vreg, _imm);

    // =======================================================================
    // Vector Unpack
    // =======================================================================
    if (core == "vupkd3d128") return _emit_vupkd3d128(ops, _vreg, _imm);

    if (core == "vupkhsb" || core == "vupkhsb128") {
        // VMX128 misID fixup: VUPKHSB128 with operands[2]==0x60 -> VUPKHSH128
        if (core == "vupkhsb128" && ops.size() > 2 && im(2) == Number(0x60)) {
            Number d = _vreg(ops[0]), a = _vreg(ops[1]);
            return _emit_vupkhsh(d, a);
        }
        Number d = _vreg(ops[0]), a = _vreg(ops[1]);
        return _emit_vupkhsb(d, a);
    }
    if (core == "vupkhsh" || core == "vupkhsh128") {
        Number d = _vreg(ops[0]), a = _vreg(ops[1]);
        return _emit_vupkhsh(d, a);
    }
    if (core == "vupklsb" || core == "vupklsb128") {
        if (core == "vupklsb128" && ops.size() > 2 && im(2) == Number(0x60)) {
            Number d = _vreg(ops[0]), a = _vreg(ops[1]);
            return _emit_vupklsh(d, a);
        }
        Number d = _vreg(ops[0]), a = _vreg(ops[1]);
        return _emit_vupklsb(d, a);
    }
    if (core == "vupklsh" || core == "vupklsh128") {
        Number d = _vreg(ops[0]), a = _vreg(ops[1]);
        return _emit_vupklsh(d, a);
    }

    return std::nullopt;
}

// ---------------------------------------------------------------------------
// Unpack (simple, si128) helpers
// ---------------------------------------------------------------------------
py::list<py::str> _emit_vupkhsb(const std::optional<int64_t>& d, const std::optional<int64_t>& a) {
    Lines o;
    o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.s16, simde_mm_cvtepi8_epi16("
              "simde_mm_unpackhi_epi64(simde_mm_load_si128((simde__m128i*)ctx.v%d.s8), "
              "simde_mm_load_si128((simde__m128i*)ctx.v%d.s8))));", {D(d), D(a), D(a)}));
    return o.v;
}

py::list<py::str> _emit_vupkhsh(const std::optional<int64_t>& d, const std::optional<int64_t>& a) {
    Lines o;
    o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.s32, simde_mm_cvtepi16_epi32("
              "simde_mm_unpackhi_epi64(simde_mm_load_si128((simde__m128i*)ctx.v%d.s16), "
              "simde_mm_load_si128((simde__m128i*)ctx.v%d.s16))));", {D(d), D(a), D(a)}));
    return o.v;
}

py::list<py::str> _emit_vupklsb(const std::optional<int64_t>& d, const std::optional<int64_t>& a) {
    Lines o;
    o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.s32, simde_mm_cvtepi8_epi16("
              "simde_mm_load_si128((simde__m128i*)ctx.v%d.s16)));", {D(d), D(a)}));
    return o.v;
}

py::list<py::str> _emit_vupklsh(const std::optional<int64_t>& d, const std::optional<int64_t>& a) {
    Lines o;
    o.add(fmt("simde_mm_store_si128((simde__m128i*)ctx.v%d.s32, simde_mm_cvtepi16_epi32("
              "simde_mm_load_si128((simde__m128i*)ctx.v%d.s16)));", {D(d), D(a)}));
    return o.v;
}

// ---------------------------------------------------------------------------
// vpkd3d128 : D3D pack (scalar). operands[0]=vD, [1]=vB(src), [2]=type,
//             [3]=mask, [4]=shift.  Emits "ctx.fpscr.enableFlushMode();" first.
// ---------------------------------------------------------------------------
py::list<py::str> _emit_vpkd3d128(const py::list<py::str>& ops, const OperandNumber& _vreg, const OperandNumber& _imm) {
    Number d = _vreg(ops[0]);
    Number s = _vreg(ops[1]);
    Number typ = _imm(ops[2]);
    Lines out;
    out.add(FLUSH);
    auto is_typ = [&](int64_t v) { return typ && *typ == v; };

    if (is_typ(0)) {  // D3D color
        Number mask = _imm(ops[3]);
        Number shift = _imm(ops[4]);
        const int64_t indices[4] = {3, 0, 1, 2};
        for (int64_t i = 0; i < 4; ++i) {
            out.add(fmt("vTemp.u32[%d] = 0x404000FF;", {D(i)}));
            out.add(fmt("vTemp.f32[%d] = !(ctx.v%d.f32[%d] >= 3.0f) ? 3.0f : (ctx.v%d.f32[%d] > "
                        "vTemp.f32[%d] ? vTemp.f32[%d] : ctx.v%d.f32[%d]);",
                        {D(i), D(s), D(i), D(s), D(i), D(i), D(i), D(s), D(i)}));
            out.add(fmt("temp.u32 %s= uint32_t(vTemp.u8[%d]) << %d;", {i == 0 ? "" : "|", D(i * 4), D(indices[i] * 8)}));
        }
        if (mask == Number(3) && shift == Number(3)) {
            out.add(fmt("ctx.v%d.u32[0] = 0;", {D(d)}));
        } else {
            out.add(fmt("ctx.v%d.u32[%d] = temp.u32;", {D(d), D(shift)}));
            if (need(mask) >= 2 && *shift < 3) out.add(fmt("ctx.v%d.u32[%d] = 0;", {D(d), D(*shift + 1)}));
        }
        return out.v;
    }

    if (is_typ(1)) {  // NORMSHORT2
        Number sh = _imm(ops[4]);
        out.add(fmt("temp.s32 = ctx.v%d.s32[3] - 0x40400000;", {D(s)}));
        out.add("temp.s32 = temp.s32 > 32767 ? 32767 : (temp.s32 < -32767 ? -32767 : temp.s32);");
        out.add("vTemp.u32[0] = uint32_t(uint16_t(temp.s32)) << 16;");
        out.add(fmt("temp.s32 = ctx.v%d.s32[2] - 0x40400000;", {D(s)}));
        out.add("temp.s32 = temp.s32 > 32767 ? 32767 : (temp.s32 < -32767 ? -32767 : temp.s32);");
        out.add("vTemp.u32[0] |= uint16_t(temp.s32);");
        out.add(fmt("ctx.v%d.u32[%d] = vTemp.u32[0];", {D(d), D(sh)}));
        return out.v;
    }

    if (is_typ(2)) {  // NORMPACKED32 (2:10:10:10)
        Number sh = _imm(ops[4]);
        out.add("vTemp.u32[0] = 0;");
        out.add(fmt("temp.f32 = ctx.v%d.f32[3] < kPack2101010_Min10 ? kPack2101010_Min10 : "
                    "(ctx.v%d.f32[3] > kPack2101010_Max10 ? kPack2101010_Max10 : ctx.v%d.f32[3]);", {D(s), D(s), D(s)}));
        out.add("vTemp.u32[0] = temp.u32 & 0x3FF;");
        out.add(fmt("temp.f32 = ctx.v%d.f32[2] < kPack2101010_Min10 ? kPack2101010_Min10 : "
                    "(ctx.v%d.f32[2] > kPack2101010_Max10 ? kPack2101010_Max10 : ctx.v%d.f32[2]);", {D(s), D(s), D(s)}));
        out.add("vTemp.u32[0] |= (temp.u32 & 0x3FF) << 10;");
        out.add(fmt("temp.f32 = ctx.v%d.f32[1] < kPack2101010_Min10 ? kPack2101010_Min10 : "
                    "(ctx.v%d.f32[1] > kPack2101010_Max10 ? kPack2101010_Max10 : ctx.v%d.f32[1]);", {D(s), D(s), D(s)}));
        out.add("vTemp.u32[0] |= (temp.u32 & 0x3FF) << 20;");
        out.add(fmt("temp.f32 = ctx.v%d.f32[0] < kPack2101010_Min2 ? kPack2101010_Min2 : "
                    "(ctx.v%d.f32[0] > kPack2101010_Max2 ? kPack2101010_Max2 : ctx.v%d.f32[0]);", {D(s), D(s), D(s)}));
        out.add("vTemp.u32[0] |= (temp.u32 & 0x3) << 30;");
        out.add(fmt("ctx.v%d.u32[%d] = vTemp.u32[0];", {D(d), D(sh)}));
        return out.v;
    }

    if (is_typ(3) || is_typ(5)) {  // FLOAT16_2 (case 3) / FLOAT16_4 (case 5)
        const bool two = is_typ(3);
        int64_t n = two ? 2 : 4;
        Number sh = _imm(ops[4]);
        for (int64_t i = 0; i < n; ++i) {
            int64_t src = 3 - i;
            int64_t dst = two ? (1 - i) + (2 * need(sh)) : (3 - i) + (2 * need(sh));
            out.add(fmt("temp.u32 = (ctx.v%d.u32[%d]&0x7FFFFFFF);", {D(s), D(src)}));
            out.add(fmt("vTemp.u8[0] = (temp.f32 != temp.f32) || (temp.f32 > 65504.0f) ? 0xFF : "
                        "((ctx.v%d.u32[%d]&0x7f800000)>>23);", {D(s), D(src)}));
            out.add(fmt("temp.u16 = vTemp.u8[0] != 0xFF ? ((ctx.v%d.u32[%d]&0x7FE000)>>13) : 0x0;", {D(s), D(src)}));
            out.add(fmt("ctx.v%d.u16[%d] = vTemp.u8[0] != 0xFF ? (vTemp.u8[0] > 0x70 ? "
                        "(((vTemp.u8[0]-0x70)<<10)+temp.u16) : (0x71-vTemp.u8[0] > 31 ? 0x0 : "
                        "((0x400+temp.u16)>>(0x71-vTemp.u8[0])))) : 0x7FFF;", {D(d), D(dst)}));
            out.add(fmt("ctx.v%d.u16[%d] |= ((ctx.v%d.u32[%d]&0x80000000)>>16);", {D(d), D(dst), D(s), D(src)}));
        }
        return out.v;
    }

    if (is_typ(4)) {  // NORMSHORT4
        Number sh = _imm(ops[4]);
        for (int64_t i = 0; i < 4; ++i) {
            int64_t src = 3 - i;
            int64_t dst = (3 - i) + (2 * need(sh));
            out.add(fmt("temp.s32 = ctx.v%d.s32[%d] - 0x40400000;", {D(s), D(src)}));
            out.add("temp.s32 = temp.s32 > 32767 ? 32767 : (temp.s32 < -32767 ? -32767 : temp.s32);");
            out.add(fmt("ctx.v%d.u16[%d] = uint16_t(temp.s32);", {D(d), D(dst)}));
        }
        return out.v;
    }

    if (is_typ(6)) {  // NORMPACKED64 (4:20:20:20)
        Number sh = _imm(ops[4]);
        out.add("vTemp.u64[0] = 0;");
        out.add(fmt("temp.s32 = int32_t(ctx.v%d.f32[0]);", {D(s)}));
        out.add("vTemp.u64[0] = uint64_t(temp.s32 & 0xFFFFF);");
        out.add(fmt("temp.s32 = int32_t(ctx.v%d.f32[1]);", {D(s)}));
        out.add("vTemp.u64[0] |= uint64_t(temp.s32 & 0xFFFFF) << 20;");
        out.add(fmt("temp.s32 = int32_t(ctx.v%d.f32[2]);", {D(s)}));
        out.add("vTemp.u64[0] |= uint64_t(temp.s32 & 0xFFFFF) << 40;");
        out.add(fmt("temp.s32 = int32_t(ctx.v%d.f32[3]);", {D(s)}));
        out.add("vTemp.u64[0] |= uint64_t(temp.s32 & 0xF) << 60;");
        out.add(fmt("ctx.v%d.u64[%d] = vTemp.u64[0];", {D(d), D(need(sh) >> 1)}));
        return out.v;
    }

    out.add("__builtin_debugtrap();");
    return out.v;
}

// ---------------------------------------------------------------------------
// vupkd3d128 : D3D unpack (scalar). operands[0]=vD, [1]=vB(src), [2]=type.
//              switch is on (operands[2] >> 2).
// ---------------------------------------------------------------------------
py::list<py::str> _emit_vupkd3d128(const py::list<py::str>& ops, const OperandNumber& _vreg, const OperandNumber& _imm) {
    Number d = _vreg(ops[0]);
    Number s = _vreg(ops[1]);
    int64_t sel = need(_imm(ops[2])) >> 2;
    Lines out;

    if (sel == 0) {  // D3D color
        const int64_t indices[4] = {3, 0, 1, 2};
        for (int64_t i = 0; i < 4; ++i)
            out.add(fmt("vTemp.u32[%d] = ctx.v%d.u8[%d] | 0x3F800000;", {D(i), D(s), D(indices[i])}));
        out.add(fmt("ctx.v%d = vTemp;", {D(d)}));
        return out.v;
    }

    if (sel == 1) {  // NORMSHORT2
        for (int64_t i = 0; i < 2; ++i) {
            out.add("temp.f32 = 3.0f;");
            out.add(fmt("temp.s32 += ctx.v%d.s16[%d];", {D(s), D(1 - i)}));
            out.add(fmt("vTemp.f32[%d] = temp.f32;", {D(3 - i)}));
        }
        out.add("vTemp.f32[1] = 0.0f;");
        out.add("vTemp.f32[0] = 1.0f;");
        out.add(fmt("ctx.v%d = vTemp;", {D(d)}));
        return out.v;
    }

    if (sel == 2) {  // NORMPACKED32
        out.add(fmt("temp.s32 = (ctx.v%d.s32[0] << 22) >> 22;", {D(s)}));
        out.add("vTemp.s32[0] = temp.s32 == -512 ? 0x7FC00000 : (temp.s32 + 0x40400000);");
        out.add(fmt("ctx.v%d.u32[3] = vTemp.u32[0];", {D(d)}));
        out.add(fmt("temp.s32 = (ctx.v%d.s32[0] << 12) >> 22;", {D(s)}));
        out.add("vTemp.s32[0] = temp.s32 + 0x40400000;");
        out.add(fmt("ctx.v%d.u32[2] = vTemp.u32[0];", {D(d)}));
        out.add(fmt("temp.s32 = (ctx.v%d.s32[0] << 2) >> 22;", {D(s)}));
        out.add("vTemp.s32[0] = temp.s32 + 0x40400000;");
        out.add(fmt("ctx.v%d.u32[1] = vTemp.u32[0];", {D(d)}));
        out.add(fmt("vTemp.u32[0] = (ctx.v%d.u32[0] >> 30) | 0x3F800000;", {D(s)}));
        out.add(fmt("ctx.v%d.u32[0] = vTemp.u32[0];", {D(d)}));
        return out.v;
    }

    if (sel == 3) {  // FLOAT16_2
        for (int64_t i = 0; i < 2; ++i) {
            int64_t src = 1 - i;
            int64_t dst = 3 - i;
            out.add(fmt("temp.u32 = ctx.v%d.u16[%d];", {D(s), D(src)}));
            out.add("vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | "
                    "(((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);");
            out.add("if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;");
            out.add(fmt("ctx.v%d.u32[%d] = vTemp.u32[0];", {D(d), D(dst)}));
        }
        out.add(fmt("ctx.v%d.f32[1] = 0.0f;", {D(d)}));
        out.add(fmt("ctx.v%d.f32[0] = 1.0f;", {D(d)}));
        return out.v;
    }

    if (sel == 4) {  // NORMSHORT4
        for (int64_t i = 0; i < 4; ++i) {
            int64_t src = 3 - i;
            int64_t dst = 3 - i;
            out.add("temp.f32 = 3.0f;");
            out.add(fmt("temp.s32 += ctx.v%d.s16[%d];", {D(s), D(src)}));
            out.add(fmt("ctx.v%d.f32[%d] = temp.f32;", {D(d), D(dst)}));
        }
        return out.v;
    }

    if (sel == 5) {  // FLOAT16_4
        for (int64_t i = 0; i < 4; ++i) {
            int64_t src = 3 - i;
            int64_t dst = 3 - i;
            out.add(fmt("temp.u32 = ctx.v%d.u16[%d];", {D(s), D(src)}));
            out.add("vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | "
                    "(((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);");
            out.add("if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;");
            out.add(fmt("ctx.v%d.u32[%d] = vTemp.u32[0];", {D(d), D(dst)}));
        }
        return out.v;
    }

    if (sel == 6) {  // NORMPACKED64
        out.add(fmt("vTemp.u64[0] = ctx.v%d.u64[1];", {D(s)}));
        out.add("temp.s32 = (int32_t(vTemp.u64[0] << 44) >> 44);");
        out.add(fmt("ctx.v%d.f32[0] = float(temp.s32);", {D(d)}));
        out.add("temp.s32 = (int32_t(vTemp.u64[0] << 24) >> 44);");
        out.add(fmt("ctx.v%d.f32[1] = float(temp.s32);", {D(d)}));
        out.add("temp.s32 = (int32_t(vTemp.u64[0] << 4) >> 44);");
        out.add(fmt("ctx.v%d.f32[2] = float(temp.s32);", {D(d)}));
        out.add(fmt("ctx.v%d.f32[3] = float(vTemp.u64[0] >> 60);", {D(d)}));
        return out.v;
    }

    out.add("__builtin_debugtrap();");
    return out.v;
}

const py::str RUNTIME_HELPERS = py::str(R"CBX(
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
)CBX");

const py::list<py::str> MNEMONICS = {
    // FP arithmetic
    py::str("vaddfp"), py::str("vaddfp128"), py::str("vsubfp"), py::str("vsubfp128"), py::str("vmulfp128"),
    py::str("vmaddfp"), py::str("vmaddfp128"), py::str("vmaddcfp128"), py::str("vnmsubfp"), py::str("vnmsubfp128"),
    py::str("vmaxfp"), py::str("vmaxfp128"), py::str("vminfp"), py::str("vminfp128"),
    py::str("vrefp"), py::str("vrefp128"), py::str("vrsqrtefp"), py::str("vrsqrtefp128"),
    py::str("vexptefp"), py::str("vexptefp128"), py::str("vlogefp"), py::str("vlogefp128"),
    // dot products
    py::str("vmsum3fp128"), py::str("vmsum4fp128"),
    // rounding
    py::str("vrfim"), py::str("vrfim128"), py::str("vrfin"), py::str("vrfin128"),
    py::str("vrfip"), py::str("vrfip128"), py::str("vrfiz"), py::str("vrfiz128"),
    // integer arithmetic / saturating
    py::str("vaddsbs"), py::str("vaddshs"), py::str("vaddsws"), py::str("vaddubm"), py::str("vaddubs"), py::str("vadduhm"),
    py::str("vadduwm"), py::str("vadduws"), py::str("vadduhs"),
    py::str("vsubsbs"), py::str("vsubsws"), py::str("vsububm"), py::str("vsububs"), py::str("vsubuhs"), py::str("vsubuws"),
    py::str("vsubuhm"), py::str("vsubuwm"), py::str("vsubshs"),
    py::str("vmaxsw"), py::str("vmaxsh"), py::str("vmaxsb"), py::str("vminsh"), py::str("vminsb"), py::str("vminsw"),
    py::str("vmaxuh"), py::str("vminuh"), py::str("vmaxub"), py::str("vminub"),
    // average
    py::str("vavgsb"), py::str("vavgsh"), py::str("vavgsw"), py::str("vavgub"), py::str("vavguh"),
    // logical
    py::str("vand"), py::str("vand128"), py::str("vandc"), py::str("vandc128"), py::str("vor"), py::str("vor128"),
    py::str("vxor"), py::str("vxor128"), py::str("vnor"), py::str("vnor128"), py::str("vsel"), py::str("vsel128"),
    // compare
    py::str("vcmpbfp"), py::str("vcmpbfp128"), py::str("vcmpeqfp"), py::str("vcmpeqfp128"),
    py::str("vcmpequb"), py::str("vcmpequh"), py::str("vcmpequw"), py::str("vcmpequw128"),
    py::str("vcmpgefp"), py::str("vcmpgefp128"), py::str("vcmpgtfp"), py::str("vcmpgtfp128"),
    py::str("vcmpgtub"), py::str("vcmpgtuh"), py::str("vcmpgtuw"),
    py::str("vcmpgtsb"), py::str("vcmpgtsh"), py::str("vcmpgtsw"),
    // conversion
    py::str("vctsxs"), py::str("vcfpsxws128"), py::str("vctuxs"), py::str("vcfpuxws128"),
    py::str("vcfsx"), py::str("vcsxwfp128"), py::str("vcfux"), py::str("vcuxwfp128"),
    // merge
    py::str("vmrghb"), py::str("vmrghh"), py::str("vmrghw"), py::str("vmrghw128"),
    py::str("vmrglb"), py::str("vmrglh"), py::str("vmrglw"), py::str("vmrglw128"),
    // permute
    py::str("vperm"), py::str("vperm128"), py::str("vpermwi128"), py::str("vrlimi128"),
    // shift / rotate
    py::str("vsl"), py::str("vslb"), py::str("vslh"), py::str("vsldoi"), py::str("vsldoi128"), py::str("vslw"), py::str("vslw128"),
    py::str("vslo"), py::str("vslo128"), py::str("vsr"), py::str("vsrh"), py::str("vsrb"), py::str("vsrab"), py::str("vsrah"),
    py::str("vsraw"), py::str("vsraw128"), py::str("vsrw"), py::str("vsrw128"), py::str("vsro"), py::str("vsro128"),
    py::str("vrlh"), py::str("vrlw"), py::str("vrlw128"),
    // splat
    py::str("vspltb"), py::str("vsplth"), py::str("vspltisb"), py::str("vspltish"), py::str("vspltisw"), py::str("vspltisw128"),
    py::str("vspltw"), py::str("vspltw128"),
    // pack
    py::str("vpkshus"), py::str("vpkshus128"), py::str("vpkuhum"), py::str("vpkuhum128"), py::str("vpkuhus"), py::str("vpkuhus128"),
    py::str("vpkuwum"), py::str("vpkuwum128"), py::str("vpkuwus"), py::str("vpkuwus128"),
    py::str("vpkshss"), py::str("vpkshss128"), py::str("vpkswss"), py::str("vpkswss128"), py::str("vpkswus"), py::str("vpkswus128"),
    py::str("vpkd3d128"),
    // unpack
    py::str("vupkd3d128"),
    py::str("vupkhsb"), py::str("vupkhsb128"), py::str("vupkhsh"), py::str("vupkhsh128"),
    py::str("vupklsb"), py::str("vupklsb128"), py::str("vupklsh"), py::str("vupklsh128"),
};

}  // namespace codebreaker::recompile::builders::vector
