// codebreaker/recompile/powerpc.cpp -- typed C++ port of codebreaker.recompile.powerpc (see powerpc.hpp).

#include "codebreaker/recompile/powerpc.hpp"

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstring>
#include <exception>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "codebreaker/base/converted.hpp"
#include "codebreaker/base/parallel.hpp"
#include "codebreaker/recompile/builders/arith.hpp"
#include "codebreaker/recompile/builders/floating.hpp"
#include "codebreaker/recompile/builders/logical.hpp"
#include "codebreaker/recompile/builders/memory.hpp"
#include "codebreaker/recompile/builders/system.hpp"
#include "codebreaker/recompile/builders/vector.hpp"
#include "codebreaker/recompile/disassemble.hpp"
#include "codebreaker/recompile/discover.hpp"
#include "codebreaker/recompile/jumptable.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::recompile::powerpc {

namespace powerpc_image = codebreaker::formats::powerpc_image;
namespace builders = codebreaker::recompile::builders;
namespace floating = builders::floating;

// -- the module's location --

namespace {
// ROOT as the import derives it: three folders above this module (codebreaker/recompile/powerpc.py). The C++ source sits
// six folders below the checkout (native/typed/cpp/codebreaker/recompile/powerpc.cpp).
py::str default_root_() {
    py::str here = py::os::path::abspath(py::str(__FILE__));
    for (int i = 0; i < 6; ++i) here = py::os::path::dirname(here);
    return here;
}
}  // namespace

py::str ROOT;

void set_root(const py::str& root) { ROOT = root; }

namespace {
const bool root_set_ = (set_root(default_root_()), true);
}

// AltiVec (128-bit) op helpers emitted into the runtime header. Kept as a plain
// string (normal C++ braces) so the f-string runtime_header() need not escape
// them. Lane order follows the register's native layout.
const py::str _VEC_HELPERS = py::str(
    "\n"
    "static inline int16_t _rex_sath(int32_t v){ return v>32767?32767:(v<-32768?-32768:(int16_t)v); }\n"
    "#define REX_VBIN8(NAME,OP)  static inline void NAME(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<16;i++)d.u8[i]=(uint8_t)(OP);}\n"
    "#define REX_VBIN16(NAME,OP) static inline void NAME(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<8;i++)d.u16[i]=(uint16_t)(OP);}\n"
    "#define REX_VBIN32(NAME,OP) static inline void NAME(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<4;i++)d.u32[i]=(uint32_t)(OP);}\n"
    "REX_VBIN8(ppc_vaddubm, a.u8[i]+b.u8[i])\n"
    "REX_VBIN16(ppc_vadduhm, a.u16[i]+b.u16[i])\n"
    "REX_VBIN32(ppc_vadduwm, a.u32[i]+b.u32[i])\n"
    "REX_VBIN8(ppc_vsububm, a.u8[i]-b.u8[i])\n"
    "REX_VBIN16(ppc_vsubuhm, a.u16[i]-b.u16[i])\n"
    "REX_VBIN32(ppc_vsubuwm, a.u32[i]-b.u32[i])\n"
    "REX_VBIN16(ppc_vslh, a.u16[i]<<(b.u16[i]&15))\n"
    "REX_VBIN16(ppc_vsrh, a.u16[i]>>(b.u16[i]&15))\n"
    "REX_VBIN32(ppc_vslw, a.u32[i]<<(b.u32[i]&31))\n"
    "REX_VBIN32(ppc_vsrw, a.u32[i]>>(b.u32[i]&31))\n"
    "static inline void ppc_vsrah(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<8;i++)d.s16[i]=(int16_t)(a.s16[i]>>(b.u16[i]&15));}\n"
    "static inline void ppc_vsraw(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<4;i++)d.s32[i]=(int32_t)(a.s32[i]>>(b.u32[i]&31));}\n"
    "static inline void ppc_vaddshs(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<8;i++)d.s16[i]=_rex_sath((int32_t)a.s16[i]+b.s16[i]);}\n"
    "static inline void ppc_vsubshs(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<8;i++)d.s16[i]=_rex_sath((int32_t)a.s16[i]-b.s16[i]);}\n"
    "static inline void ppc_vand(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){d.u64[0]=a.u64[0]&b.u64[0];d.u64[1]=a.u64[1]&b.u64[1];}\n"
    "static inline void ppc_vor (PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){d.u64[0]=a.u64[0]|b.u64[0];d.u64[1]=a.u64[1]|b.u64[1];}\n"
    "static inline void ppc_vxor(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){d.u64[0]=a.u64[0]^b.u64[0];d.u64[1]=a.u64[1]^b.u64[1];}\n"
    "static inline void ppc_vandc(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){d.u64[0]=a.u64[0]&~b.u64[0];d.u64[1]=a.u64[1]&~b.u64[1];}\n"
    "static inline void ppc_vnor(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){d.u64[0]=~(a.u64[0]|b.u64[0]);d.u64[1]=~(a.u64[1]|b.u64[1]);}\n"
    "static inline void ppc_vmrghb(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<8;i++){t.u8[2*i]=a.u8[i];t.u8[2*i+1]=b.u8[i];}d=t;}\n"
    "static inline void ppc_vmrglb(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<8;i++){t.u8[2*i]=a.u8[8+i];t.u8[2*i+1]=b.u8[8+i];}d=t;}\n"
    "static inline void ppc_vmrghh(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<4;i++){t.u16[2*i]=a.u16[i];t.u16[2*i+1]=b.u16[i];}d=t;}\n"
    "static inline void ppc_vmrglh(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<4;i++){t.u16[2*i]=a.u16[4+i];t.u16[2*i+1]=b.u16[4+i];}d=t;}\n"
    "static inline void ppc_vmrghw(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<2;i++){t.u32[2*i]=a.u32[i];t.u32[2*i+1]=b.u32[i];}d=t;}\n"
    "static inline void ppc_vmrglw(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<2;i++){t.u32[2*i]=a.u32[2+i];t.u32[2*i+1]=b.u32[2+i];}d=t;}\n"
    "static inline void ppc_vperm(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b,const PPCVRegister&c){PPCVRegister t;for(int i=0;i<16;i++){int s=c.u8[i]&0x1F;t.u8[i]=s<16?a.u8[s]:b.u8[s-16];}d=t;}\n"
    "static inline void ppc_vsel(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b,const PPCVRegister&c){d.u64[0]=(a.u64[0]&~c.u64[0])|(b.u64[0]&c.u64[0]);d.u64[1]=(a.u64[1]&~c.u64[1])|(b.u64[1]&c.u64[1]);}\n"
    "static inline void ppc_vspltw(PPCVRegister&d,const PPCVRegister&a,int e){uint32_t v=a.u32[e&3];for(int i=0;i<4;i++)d.u32[i]=v;}\n"
    "static inline void ppc_vsplth(PPCVRegister&d,const PPCVRegister&a,int e){uint16_t v=a.u16[e&7];for(int i=0;i<8;i++)d.u16[i]=v;}\n"
    "static inline void ppc_vspltb(PPCVRegister&d,const PPCVRegister&a,int e){uint8_t v=a.u8[e&15];for(int i=0;i<16;i++)d.u8[i]=v;}\n");

const py::dict<py::str, py::str> _COND = {
    {py::str("beq"), py::str("{cr}.eq")}, {py::str("bne"), py::str("!{cr}.eq")}, {py::str("blt"), py::str("{cr}.lt")},
    {py::str("bge"), py::str("!{cr}.lt")}, {py::str("bgt"), py::str("{cr}.gt")}, {py::str("ble"), py::str("!{cr}.gt")},
    {py::str("bso"), py::str("{cr}.so")}, {py::str("bns"), py::str("!{cr}.so")}, {py::str("bun"), py::str("{cr}.so")},
    {py::str("bnu"), py::str("!{cr}.so")}};

const py::dict<py::str, int64_t> _CRFIELD_BIT = {
    {py::str("lt"), 0}, {py::str("gt"), 1}, {py::str("eq"), 2}, {py::str("un"), 3}, {py::str("so"), 3}};

const py::set<py::str> _VEC3 = {
    py::str("vaddubm"), py::str("vadduhm"), py::str("vadduwm"), py::str("vsububm"), py::str("vsubuhm"), py::str("vsubuwm"),
    py::str("vslh"), py::str("vsrh"), py::str("vslw"), py::str("vsrw"), py::str("vsrah"), py::str("vsraw"),
    py::str("vaddshs"), py::str("vsubshs"), py::str("vand"), py::str("vor"), py::str("vxor"), py::str("vandc"), py::str("vnor"),
    py::str("vmrghb"), py::str("vmrglb"), py::str("vmrghh"), py::str("vmrglh"), py::str("vmrghw"), py::str("vmrglw")};

// ---------------------------------------------------------------------------------------------------------------------
// operand tokens
// ---------------------------------------------------------------------------------------------------------------------

namespace {

using Num = std::optional<int64_t>;

bool ascii_only(const std::u32string& t) {
    for (char32_t c : t)
        if (c >= 0x80) return false;
    return true;
}
bool is_digit(char32_t c) { return c >= '0' && c <= '9'; }
bool is_hex(char32_t c) { return is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

int64_t digits_to_int(const std::u32string& t, size_t a, size_t b) {
    if (b - a <= 18) {
        int64_t v = 0;
        for (size_t i = a; i < b; ++i) v = v * 10 + static_cast<int64_t>(t[i] - '0');
        return v;
    }
    return py::int_parse(py::str(t.substr(a, b - a)));
}

// re.fullmatch(r"<prefix>(\d+)", tok.strip()) -> the number
Num prefixed(const py::str& tok_, char32_t prefix, const py::re::Pattern& re) {
    const py::str tok = tok_.strip();
    const std::u32string& t = tok.u32();
    if (ascii_only(t)) {
        if (t.size() < 2 || t[0] != prefix) return std::nullopt;
        for (size_t i = 1; i < t.size(); ++i)
            if (!is_digit(t[i])) return std::nullopt;
        return digits_to_int(t, 1, t.size());
    }
    const auto m = re.fullmatch(tok);
    if (!m) return std::nullopt;
    return py::int_parse(m->group(1));
}

// the number, or the TypeError Python raises for arithmetic on None
int64_t need(const Num& n) {
    if (!n) throw py::TypeError("unsupported operand type(s) for arithmetic: 'NoneType'");
    return *n;
}

// f"{n}" for a number that is None when an operand token was not of the kind asked for
std::string N(const Num& n) { return n ? std::to_string(*n) : std::string("None"); }
std::string N(int64_t n) { return std::to_string(n); }

std::string hex_(uint64_t v, const char* fmt) {
    char buf[40];
    std::snprintf(buf, sizeof buf, fmt, static_cast<unsigned long long>(v));
    return buf;
}
// f"{v:08X}"
std::string x8(uint64_t v) { return hex_(v, "%08llX"); }

// (_imm(tok) << 16) & 0xFFFFFFFF
uint64_t shl16_32(const Num& imm) { return (static_cast<uint64_t>(need(imm)) << 16) & 0xFFFFFFFFull; }
// _imm(tok) & 0xFFFF
uint64_t low16(const Num& imm) { return static_cast<uint64_t>(need(imm)) & 0xFFFFull; }

py::list<py::str> L(const std::vector<std::string>& v) {
    py::list<py::str> out;
    for (const std::string& s : v) out.append(py::str(s));
    return out;
}

}  // namespace

std::optional<int64_t> _reg(const py::str& tok) {
    static const py::re::Pattern re("r(\\d+)", 0);
    return prefixed(tok, U'r', re);
}

std::optional<int64_t> _reg_or_zero(const py::str& tok) { return tok.strip() == py::str("0") ? Num(0) : _reg(tok); }

std::optional<int64_t> _imm(const py::str& tok) {
    try {
        return py::int_parse(tok.strip(), 0);
    } catch (const py::ValueError&) {
        return std::nullopt;
    }
}

std::optional<std::tuple<int64_t, int64_t>> _mem(const py::str& tok_) {
    const py::str tok = tok_.strip();
    const std::u32string& t = tok.u32();
    py::str disp_text, base_text;
    if (ascii_only(t)) {
        // (-?(?:0x)?[0-9A-Fa-f]+)\((r\d+|0)\)   as a whole
        size_t i = 0;
        if (i < t.size() && t[i] == U'-') ++i;
        size_t end = 0;
        bool ok = false;
        if (i + 1 < t.size() && t[i] == U'0' && t[i + 1] == U'x') {
            size_t k = i + 2;
            while (k < t.size() && is_hex(t[k])) ++k;
            if (k > i + 2 && k < t.size() && t[k] == U'(') {
                end = k;
                ok = true;
            }
        }
        if (!ok) {
            size_t k = i;
            while (k < t.size() && is_hex(t[k])) ++k;
            if (k > i && k < t.size() && t[k] == U'(') {
                end = k;
                ok = true;
            }
        }
        if (!ok) return std::nullopt;
        const size_t j = end + 1;
        size_t close = 0;
        if (j < t.size() && t[j] == U'r') {
            size_t k = j + 1;
            while (k < t.size() && is_digit(t[k])) ++k;
            if (k == j + 1) return std::nullopt;
            close = k;
        } else if (j < t.size() && t[j] == U'0') {
            close = j + 1;
        } else {
            return std::nullopt;
        }
        if (close + 1 != t.size() || t[close] != U')') return std::nullopt;
        disp_text = py::str(t.substr(0, end));
        base_text = py::str(t.substr(j, close - j));
    } else {
        static const py::re::Pattern re("(-?(?:0x)?[0-9A-Fa-f]+)\\((r\\d+|0)\\)", 0);
        const auto m = re.fullmatch(tok);
        if (!m) return std::nullopt;
        disp_text = m->group(1);
        base_text = m->group(2);
    }
    const int64_t disp = py::int_parse(disp_text, 0);
    const int64_t base = base_text == py::str("0") ? 0 : py::int_parse(base_text.slice_from(1));
    return std::make_tuple(disp, base);
}

std::optional<int64_t> _cr(const py::str& tok_) {
    const py::str tok = tok_.strip();
    const std::u32string& t = tok.u32();
    if (ascii_only(t)) {
        // cr(\d)   or   cr(\d)(lt|gt|eq|un|so)
        if (t.size() < 3 || t[0] != U'c' || t[1] != U'r' || !is_digit(t[2])) return std::nullopt;
        const int64_t field = t[2] - '0';
        if (t.size() == 3) return field;
        if (t.size() == 5) {
            const std::u32string w = t.substr(3);
            for (const char32_t* name : {U"lt", U"gt", U"eq", U"un", U"so"}) {
                if (w == name) return field * 4 + _CRFIELD_BIT.at(py::str(std::u32string(name)));
            }
        }
        return std::nullopt;
    }
    static const py::re::Pattern re1("cr(\\d)", 0), re2("cr(\\d)(lt|gt|eq|un|so)", 0);
    if (const auto m = re1.fullmatch(tok)) return py::int_parse(m->group(1));
    if (const auto m = re2.fullmatch(tok)) return py::int_parse(m->group(1)) * 4 + _CRFIELD_BIT.at(m->group(2));
    return std::nullopt;
}

py::list<py::str> _split(const py::str& op_str) {
    py::list<py::str> out;
    if (op_str.strip().empty()) return out;
    const std::u32string& t = op_str.u32();
    size_t a = 0;
    for (;;) {
        const size_t b = t.find(U',', a);
        out.append(py::str(t.substr(a, b == std::u32string::npos ? std::u32string::npos : b - a)).strip());
        if (b == std::u32string::npos) break;
        a = b + 1;
    }
    return out;
}

py::str _rv(const std::optional<int64_t>& n, const py::str& field) { return py::str("ctx.r" + N(n) + "." + field.utf8()); }

py::str _base_expr(const std::optional<int64_t>& base) {
    return (base && *base == 0) ? py::str("0") : py::str("ctx.r" + N(base) + ".u32");
}

py::str _addr(int64_t disp, int64_t base) {
    const py::str b = _base_expr(base);
    if (b == py::str("0")) return py::str(N(disp));
    return py::str(b.utf8() + " + " + N(disp));
}

std::optional<int64_t> _freg(const py::str& tok) {
    static const py::re::Pattern re("f(\\d+)", 0);
    return prefixed(tok, U'f', re);
}

std::optional<int64_t> _vreg(const py::str& tok) {
    static const py::re::Pattern re("v(\\d+)", 0);
    return prefixed(tok, U'v', re);
}

// ---------------------------------------------------------------------------------------------------------------------
// the per-instruction emitter
// ---------------------------------------------------------------------------------------------------------------------

namespace {

std::string R(const Num& n, const char* field = "u64") { return "ctx.r" + N(n) + "." + field; }   // _rv
std::string F(const Num& n) { return "ctx.f" + N(n); }
std::string V(const Num& n) { return "ctx.v" + N(n); }
std::string BE(const Num& base) { return _base_expr(base).utf8(); }

// the operand helpers the builders are handed
const floating::OperandNumber& op_reg0() {
    static const floating::OperandNumber f = [](const py::str& t) { return _reg_or_zero(t); };
    return f;
}
const floating::OperandNumber& op_vreg() {
    static const floating::OperandNumber f = [](const py::str& t) { return _vreg(t); };
    return f;
}
const floating::OperandNumber& op_freg() {
    static const floating::OperandNumber f = [](const py::str& t) { return _freg(t); };
    return f;
}
const floating::OperandNumber& op_imm() {
    static const floating::OperandNumber f = [](const py::str& t) { return _imm(t); };
    return f;
}
const floating::OperandNumber& op_cr() {
    static const floating::OperandNumber f = [](const py::str& t) { return _cr(t); };
    return f;
}
const floating::MemOperand& op_mem() {
    static const floating::MemOperand f = [](const py::str& t) { return _mem(t); };
    return f;
}
const floating::RegisterValue& op_rv() {
    static const floating::RegisterValue f = [](int64_t n, const py::str& field) { return _rv(n, field); };
    return f;
}
const floating::BaseExpression& op_base_expr() {
    static const floating::BaseExpression f = [](int64_t b) { return _base_expr(b); };
    return f;
}
const floating::AddressExpression& op_addr() {
    static const floating::AddressExpression f = [](int64_t d, int64_t b) { return _addr(d, b); };
    return f;
}

// re.findall(r"0x[0-9A-Fa-f]+", text)[-1] as an int, or None
Num last_hex_token(const py::str& text) {
    const std::u32string& t = text.u32();
    Num out;
    size_t i = 0;
    while (i < t.size()) {
        if (t[i] == U'0' && i + 2 < t.size() && t[i + 1] == U'x' && is_hex(t[i + 2])) {
            size_t k = i + 2;
            while (k < t.size() && is_hex(t[k])) ++k;
            out = py::int_parse(py::str(t.substr(i, k - i)), 16);
            i = k;
        } else {
            ++i;
        }
    }
    return out;
}

bool starts(const std::string& s, const char* p) { return s.compare(0, std::strlen(p), p) == 0; }
bool ends(const std::string& s, const char* p) {
    const size_t n = std::strlen(p);
    return s.size() >= n && s.compare(s.size() - n, n, p) == 0;
}
std::string rstrip_chars(std::string s, const char* chars) {
    while (!s.empty() && std::strchr(chars, s.back())) s.pop_back();
    return s;
}
bool in(const std::string& s, std::initializer_list<const char*> names) {
    for (const char* n : names)
        if (s == n) return true;
    return false;
}

}  // namespace

py::list<py::str> emit_insn(int64_t va, const py::str& mn_text, const py::str& op_str, const py::set<int64_t>& labels, int64_t func_lo,
                            int64_t func_hi, int64_t raw, const std::optional<JumpTables>& jump_tables) {
    (void)labels;
    py::list<py::str> ops = _split(op_str);
    const std::string mn = mn_text.utf8();
    const std::string ostr = op_str.utf8();

    // Capstone leaves out an implicit cr0 on compares ("cmplwi r10,2"); the builders read
    // [crD, rA, rB|imm], so it is put back ("cmplwi cr0,r10,2").
    if (starts(mn, "cmp") && !ops.empty() && !_cr(ops[0])) {
        py::list<py::str> with;
        with.append(py::str("cr0"));
        with.extend(ops);
        ops = with;
    }

    // ---- atomics: load-reserve / store-conditional --------------------
    // MUST come before the record-form ('.') handling: stwcx./stdcx. set CR0 as
    // part of their own semantics, and if these aren't implemented every guest
    // spinlock / refcount / atomic-increment loop spins forever. The reservation's value is
    // kept in ctx.reserved, and the conditional store is a compare-and-swap.
    const std::string amn = rstrip_chars(mn, ".");
    if (in(amn, {"lwarx", "ldarx", "stwcx", "stdcx"})) {
        const Num a = _reg(ops[1]), b = _reg(ops[2]), d = _reg(ops[0]);
        const std::string ea = (!a || *a == 0) ? R(b, "u32") : "ctx.r" + N(a) + ".u32 + " + R(b, "u32");
        if (amn == "lwarx")
            return L({"{ uint32_t _ea = " + ea + "; ctx.reserved.u32 = *(volatile uint32_t*)(base + _ea); " + "ctx.r" + N(d) +
                      ".u64 = __builtin_bswap32(ctx.reserved.u32); }"});
        if (amn == "ldarx")
            return L({"{ uint32_t _ea = " + ea + "; ctx.reserved.u64 = *(volatile uint64_t*)(base + _ea); " + "ctx.r" + N(d) +
                      ".u64 = __builtin_bswap64(ctx.reserved.u64); }"});
        if (amn == "stwcx")
            return L({"{ uint32_t _ea = " + ea + "; ctx.cr0.lt = ctx.cr0.gt = 0; ctx.cr0.so = ctx.xer.so; " +
                      "ctx.cr0.eq = __sync_bool_compare_and_swap((uint32_t*)(base + _ea), " + "ctx.reserved.u32, __builtin_bswap32(ctx.r" +
                      N(d) + ".u32)); }"});
        // stdcx.
        return L({"{ uint32_t _ea = " + ea + "; ctx.cr0.lt = ctx.cr0.gt = 0; ctx.cr0.so = ctx.xer.so; " +
                  "ctx.cr0.eq = __sync_bool_compare_and_swap((uint64_t*)(base + _ea), " + "ctx.reserved.u64, __builtin_bswap64(ctx.r" +
                  N(d) + ".u64)); }"});
    }

    // ---- generic integer record form (Rc bit / trailing '.') ----------------
    // Handled BEFORE the builder chain: strip the '.', emit the base op through
    // the normal path, then append the CR0 compare exactly once. Kept ahead of
    // the ports because the arithmetic port defensively strips '.' itself (and
    // would otherwise return a base op with no CR0 update, silently dropping the
    // record semantics). FP (CR1) and vector (CR6) manage their own condition
    // field inside their ports, so their dotted forms are NOT intercepted here.
    if (ends(mn, ".") && mn_text.size() > 1 && !starts(mn, "f") && !starts(mn, "v")) {
        py::list<py::str> base = emit_insn(va, mn_text.slice(0, mn_text.size() - 1), op_str, labels, func_lo, func_hi, raw, jump_tables);
        const Num d = !ops.empty() ? _reg(ops[0]) : Num();
        // Don't double-set CR0: some builders (andi./andis.) already emit
        // their own cr0 update. Only append when the base didn't touch cr0.
        bool already_cr0 = false, unimpl = false;
        for (const py::str& s : base) {
            if (s.contains(py::str("cr0"))) already_cr0 = true;
            if (s.contains(py::str("UNIMPLEMENTED"))) unimpl = true;
        }
        if (d && !already_cr0 && !unimpl) base.append(py::str("ctx.cr0.compare<int32_t>(ctx.r" + N(d) + ".s32, 0, ctx.xer);"));
        return base;
    }

    // vsel128's selector is its destination: the builders read it as the fourth operand
    if (mn == "vsel128" && ops.size() == 3) {
        py::list<py::str> four = ops.copy();
        four.append(ops[0]);
        ops = four;
    }

    // ---- the builders (recompile/builders/) ----------------------------------
    // The complete instruction set: scalar FP, integer arith+compare, logical/
    // rotate/shift, SPR/trap/CR-logical, loads/stores/atomics, and VMX/VMX128.
    // Each module's emit() returns a C++ stmt list or None. Builders that emit
    // their own condition update (vcmp*. via CR6, the FP compares) are reached
    // here directly; base integer ops arrive dotless (the record wrapper above
    // already stripped and will append CR0).
    // (each module's emit takes the same helpers under its own type names)
#define CB_PORT(M, RV) [&]() { return M::emit(mn_text, ops, op_reg0(), op_vreg(), op_freg(), op_imm(), op_cr(), op_mem(), RV, op_base_expr(), op_addr(), va, raw); }
    // (logical, system and memory take the register-value helper with an optional register number)
    static const builders::logical::RegisterValue rv_opt = [](const std::optional<int64_t>& n, const py::str& f) { return _rv(n, f); };
    const std::vector<std::function<std::optional<py::list<py::str>>()>> port_modules = {
        CB_PORT(builders::floating, op_rv()), CB_PORT(builders::arith, op_rv()),  CB_PORT(builders::logical, rv_opt),
        CB_PORT(builders::system, rv_opt),    CB_PORT(builders::memory, rv_opt),  CB_PORT(builders::vector, op_rv())};
#undef CB_PORT
    for (const auto& port : port_modules) {
        std::optional<py::list<py::str>> r;
        try {
            r = port();
        } catch (const std::exception&) {
            // Capstone writes some operands in another shape than the builders
            // expect (implicit cr0 on compares, symbolic
            // CR bits like "cr1eq"). If a builder mis-indexes on that, don't
            // crash the whole recompile - fall through to the next module and
            // ultimately to the format-tuned hand-written handlers below.
            r = std::nullopt;
        }
        if (r) return *r;
    }

    // ---- floating point ----------------------------------------------
    if (mn == "fmr") return L({F(_freg(ops[0])) + ".f64 = " + F(_freg(ops[1])) + ".f64;"});
    {
        static const std::vector<std::tuple<const char*, const char*, const char*>> FADD = {
            {"fadds", "float", "+"}, {"fsubs", "float", "-"}, {"fmuls", "float", "*"}, {"fdivs", "float", "/"},
            {"fadd", "double", "+"}, {"fsub", "double", "-"}, {"fmul", "double", "*"}, {"fdiv", "double", "/"}};
        for (const auto& e : FADD) {
            if (mn == std::get<0>(e)) {
                const Num a = _freg(ops[1]), b = _freg(ops[2]);
                return L({F(_freg(ops[0])) + ".f64 = (" + std::get<1>(e) + ")(" + F(a) + ".f64 " + std::get<2>(e) + " " + F(b) + ".f64);"});
            }
        }
    }
    if (mn == "fcmpu" || mn == "fcmpo") {
        const Num c = _cr(ops[0]);
        return L({"ctx.cr" + N(c ? *c : 0) + ".compare<double>(" + F(_freg(ops[1])) + ".f64, " + F(_freg(ops[2])) + ".f64, ctx.xer);"});
    }
    if (in(mn, {"lfs", "lfsu", "lfd", "lfdu"})) {
        const auto mref = _mem(ops[1]);
        if (mref) {
            const auto [disp, base] = *mref;
            const Num fd = _freg(ops[0]);
            if (starts(mn, "lfs"))
                return L({"{ uint32_t _t = PPC_LOAD_U32(" + _addr(disp, base).utf8() + "); float _f; " +
                          "__builtin_memcpy(&_f, &_t, 4); " + F(fd) + ".f64 = _f; }"});
            return L({"{ uint64_t _t = PPC_LOAD_U64(" + _addr(disp, base).utf8() + "); double _d; " + "__builtin_memcpy(&_d, &_t, 8); " +
                      F(fd) + ".f64 = _d; }"});
        }
    }
    if (in(mn, {"stfs", "stfsu", "stfd", "stfdu"})) {
        const auto mref = _mem(ops[1]);
        if (mref) {
            const auto [disp, base] = *mref;
            const Num fs = _freg(ops[0]);
            if (starts(mn, "stfs"))
                return L({"{ float _f = (float)" + F(fs) + ".f64; uint32_t _t; " + "__builtin_memcpy(&_t, &_f, 4); PPC_STORE_U32(" +
                          _addr(disp, base).utf8() + ", _t); }"});
            return L({"{ double _d = " + F(fs) + ".f64; uint64_t _t; " + "__builtin_memcpy(&_t, &_d, 8); PPC_STORE_U64(" +
                      _addr(disp, base).utf8() + ", _t); }"});
        }
    }

    // ---- a few more integer ops --------------------------------------
    if (mn == "srawi")
        return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = (int32_t)(" + R(_reg(ops[1]), "s32") + " >> " + N(_imm(ops[2])) + ");"});
    if (mn == "divw")
        return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = (int32_t)(" + R(_reg(ops[1]), "s32") + " / " + R(_reg(ops[2]), "s32") + ");"});
    if (mn == "divwu")
        return L({"ctx.r" + N(_reg(ops[0])) + ".u64 = (uint32_t)(" + R(_reg(ops[1]), "u32") + " / " + R(_reg(ops[2]), "u32") + ");"});
    if (mn == "andi") return L({R(_reg(ops[0])) + " = " + R(_reg(ops[1])) + " & " + std::to_string(low16(_imm(ops[2]))) + "u;"});
    if (in(mn, {"tw", "twi", "twui", "trap", "twlgei", "twllei", "tweqi"})) return L({"/* tw (conditional trap) - elided in PoC */"});

    // ---- sign extension -----------------------------------------------
    if (mn == "extsb") return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = (int8_t)" + R(_reg(ops[1]), "u8") + ";"});
    if (mn == "extsh") return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = (int16_t)" + R(_reg(ops[1]), "u16") + ";"});
    if (mn == "extsw") return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = (int32_t)" + R(_reg(ops[1]), "u32") + ";"});
    if (mn == "cntlzw") {
        const std::string s = R(_reg(ops[1]), "u32");
        return L({"ctx.r" + N(_reg(ops[0])) + ".u64 = " + s + " ? (uint32_t)__builtin_clz(" + s + ") : 32;"});
    }
    if (mn == "cntlzd") {
        const std::string s = R(_reg(ops[1]), "u64");
        return L({"ctx.r" + N(_reg(ops[0])) + ".u64 = " + s + " ? (uint32_t)__builtin_clzll(" + s + ") : 64;"});
    }

    // ---- rotate-and-mask (general forms) ------------------------------
    if (mn == "rlwinm") {
        const Num d = _reg(ops[0]), s = _reg(ops[1]), sh = _imm(ops[2]), mb = _imm(ops[3]), me = _imm(ops[4]);
        return L({"ctx.r" + N(d) + ".u64 = ppc_rlwinm(" + R(s, "u32") + ", " + N(sh) + ", " + N(mb) + ", " + N(me) + ");"});
    }
    if (mn == "rlwimi") {
        const Num d = _reg(ops[0]), s = _reg(ops[1]), sh = _imm(ops[2]), mb = _imm(ops[3]), me = _imm(ops[4]);
        return L({"ctx.r" + N(d) + ".u64 = ppc_rlwimi(" + R(d, "u32") + ", " + R(s, "u32") + ", " + N(sh) + ", " + N(mb) + ", " + N(me) + ");"});
    }

    // ---- 64-bit shifts / clears ---------------------------------------
    if (mn == "clrldi")
        return L({"ctx.r" + N(_reg(ops[0])) + ".u64 = " + R(_reg(ops[1]), "u64") + " & (~0ull >> " + N(_imm(ops[2])) + ");"});
    if (mn == "sldi") return L({"ctx.r" + N(_reg(ops[0])) + ".u64 = " + R(_reg(ops[1]), "u64") + " << " + N(_imm(ops[2])) + ";"});
    if (mn == "srdi") return L({"ctx.r" + N(_reg(ops[0])) + ".u64 = " + R(_reg(ops[1]), "u64") + " >> " + N(_imm(ops[2])) + ";"});
    if (mn == "sradi") return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = " + R(_reg(ops[1]), "s64") + " >> " + N(_imm(ops[2])) + ";"});
    if (mn == "sld") {
        const Num d = _reg(ops[0]), s = _reg(ops[1]), b = _reg(ops[2]);
        return L({"ctx.r" + N(d) + ".u64 = (" + R(b, "u32") + " & 0x40) ? 0 : (" + R(s, "u64") + " << (" + R(b, "u32") + " & 0x3F));"});
    }
    if (mn == "srd") {
        const Num d = _reg(ops[0]), s = _reg(ops[1]), b = _reg(ops[2]);
        return L({"ctx.r" + N(d) + ".u64 = (" + R(b, "u32") + " & 0x40) ? 0 : (" + R(s, "u64") + " >> (" + R(b, "u32") + " & 0x3F));"});
    }
    if (mn == "slw") {
        const Num d = _reg(ops[0]), s = _reg(ops[1]), b = _reg(ops[2]);
        return L({"ctx.r" + N(d) + ".u64 = (" + R(b, "u32") + " & 0x20) ? 0 : (uint32_t)(" + R(s, "u32") + " << (" + R(b, "u32") + " & 0x1F));"});
    }
    if (mn == "srw") {
        const Num d = _reg(ops[0]), s = _reg(ops[1]), b = _reg(ops[2]);
        return L({"ctx.r" + N(d) + ".u64 = (" + R(b, "u32") + " & 0x20) ? 0 : (" + R(s, "u32") + " >> (" + R(b, "u32") + " & 0x1F));"});
    }
    if (mn == "sraw") {
        const Num d = _reg(ops[0]), s = _reg(ops[1]), b = _reg(ops[2]);
        return L({"ctx.r" + N(d) + ".s64 = (int32_t)(" + R(s, "s32") + " >> (" + R(b, "u32") + " & 0x1F));"});
    }

    // ---- more integer arithmetic --------------------------------------
    if (mn == "subfic")
        return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = (int64_t)" + N(_imm(ops[2])) + " - " + R(_reg(ops[1]), "s64") + ";"});
    if (mn == "addic" || mn == "subfic_")
        return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = " + R(_reg(ops[1]), "s64") + " + " + N(_imm(ops[2])) + ";"});
    if (mn == "addc") {   // add carrying (sets XER[CA])
        const Num d = _reg(ops[0]), a = _reg(ops[1]), b = _reg(ops[2]);
        return L({"{ uint64_t _s = (uint64_t)" + R(a, "u32") + " + " + R(b, "u32") + "; " + "ctx.r" + N(d) +
                  ".u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }"});
    }
    if (mn == "adde") {   // add extended (uses XER[CA])
        const Num d = _reg(ops[0]), a = _reg(ops[1]), b = _reg(ops[2]);
        return L({"{ uint64_t _s = (uint64_t)" + R(a, "u32") + " + " + R(b, "u32") + " + ctx.xer.ca; " + "ctx.r" + N(d) +
                  ".u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }"});
    }
    if (mn == "subfc") {
        const Num d = _reg(ops[0]), a = _reg(ops[1]), b = _reg(ops[2]);
        return L({"{ uint64_t _s = (uint64_t)" + R(b, "u32") + " + (uint32_t)(~" + R(a, "u32") + ") + 1; " + "ctx.r" + N(d) +
                  ".u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }"});
    }
    if (mn == "subfe") {
        const Num d = _reg(ops[0]), a = _reg(ops[1]), b = _reg(ops[2]);
        return L({"{ uint64_t _s = (uint64_t)" + R(b, "u32") + " + (uint32_t)(~" + R(a, "u32") + ") + ctx.xer.ca; " + "ctx.r" + N(d) +
                  ".u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }"});
    }
    if (mn == "nand") return L({R(_reg(ops[0])) + " = ~(" + R(_reg(ops[1])) + " & " + R(_reg(ops[2])) + ");"});
    if (mn == "andc") return L({R(_reg(ops[0])) + " = " + R(_reg(ops[1])) + " & ~" + R(_reg(ops[2])) + ";"});
    if (mn == "orc") return L({R(_reg(ops[0])) + " = " + R(_reg(ops[1])) + " | ~" + R(_reg(ops[2])) + ";"});
    if (mn == "nor") return L({R(_reg(ops[0])) + " = ~(" + R(_reg(ops[1])) + " | " + R(_reg(ops[2])) + ");"});
    if (mn == "eqv") return L({R(_reg(ops[0])) + " = ~(" + R(_reg(ops[1])) + " ^ " + R(_reg(ops[2])) + ");"});
    if (mn == "mulld") return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = " + R(_reg(ops[1]), "s64") + " * " + R(_reg(ops[2]), "s64") + ";"});
    if (mn == "divd") return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = " + R(_reg(ops[1]), "s64") + " / " + R(_reg(ops[2]), "s64") + ";"});
    if (mn == "divdu") return L({"ctx.r" + N(_reg(ops[0])) + ".u64 = " + R(_reg(ops[1]), "u64") + " / " + R(_reg(ops[2]), "u64") + ";"});

    // ---- more floating point ------------------------------------------
    if (mn == "fneg") return L({F(_freg(ops[0])) + ".f64 = -" + F(_freg(ops[1])) + ".f64;"});
    if (mn == "fabs") return L({F(_freg(ops[0])) + ".f64 = __builtin_fabs(" + F(_freg(ops[1])) + ".f64);"});
    if (mn == "fnabs") return L({F(_freg(ops[0])) + ".f64 = -__builtin_fabs(" + F(_freg(ops[1])) + ".f64);"});
    if (mn == "frsp") return L({F(_freg(ops[0])) + ".f64 = (float)" + F(_freg(ops[1])) + ".f64;"});
    if (mn == "fcfid") return L({F(_freg(ops[0])) + ".f64 = (double)(int64_t)" + F(_freg(ops[1])) + ".u64;"});
    if (mn == "fctiwz" || mn == "fctiw") return L({F(_freg(ops[0])) + ".u64 = (uint32_t)(int32_t)" + F(_freg(ops[1])) + ".f64;"});
    if (mn == "fctidz" || mn == "fctid") return L({F(_freg(ops[0])) + ".u64 = (uint64_t)(int64_t)" + F(_freg(ops[1])) + ".f64;"});
    if (mn == "fsqrts") return L({F(_freg(ops[0])) + ".f64 = (float)__builtin_sqrt(" + F(_freg(ops[1])) + ".f64);"});
    if (mn == "fsqrt") return L({F(_freg(ops[0])) + ".f64 = __builtin_sqrt(" + F(_freg(ops[1])) + ".f64);"});
    if (mn == "fres") return L({F(_freg(ops[0])) + ".f64 = (float)(1.0 / " + F(_freg(ops[1])) + ".f64);"});
    {
        static const std::vector<std::tuple<const char*, const char*, const char*>> FMA = {
            {"fmadds", "(float)", "+"}, {"fmsubs", "(float)", "-"}, {"fnmadds", "-(float)", "+"}, {"fnmsubs", "-(float)", "-"},
            {"fmadd", "", "+"},         {"fmsub", "", "-"},         {"fnmadd", "-", "+"},         {"fnmsub", "-", "-"}};
        for (const auto& e : FMA) {
            if (mn == std::get<0>(e)) {
                const Num a = _freg(ops[1]), c = _freg(ops[2]), b = _freg(ops[3]);
                return L({F(_freg(ops[0])) + ".f64 = " + std::get<1>(e) + "(" + F(a) + ".f64 * " + F(c) + ".f64 " + std::get<2>(e) + " " + F(b) +
                          ".f64);"});
            }
        }
    }
    if (mn == "stfiwx")
        return L({"PPC_STORE_U32(" + R(_reg(ops[1]), "u32") + " + " + R(_reg(ops[2]), "u32") + ", (uint32_t)" + F(_freg(ops[0])) + ".u64);"});

    // ---- cache / sync hints -> no-ops (correct: they are hints/barriers) --
    if (in(mn, {"dcbt", "dcbtst", "dcbst", "dcbf", "dcbi", "icbi", "sync", "isync", "eieio", "lwsync", "ptesync", "tlbsync", "dcbz",
                "dcbz128", "dcbzl"}))
        return L({"/* " + mn + " (cache/sync hint) */"});
    // supervisor MSR operations: the MSR is not modelled -- writes do nothing, reads give 0.
    if (in(mn, {"mtmsr", "mtmsrd", "mtmsree"})) return L({"/* " + mn + " (MSR write, not modeled) */"});
    if (mn == "mfmsr") return L({R(_reg(ops[0])) + " = 0;"});

    // ---- AltiVec vectors (element-wise; helpers in the runtime header) -----
    if (_VEC3.contains(mn_text))
        return L({"ppc_" + mn + "(" + V(_vreg(ops[0])) + ", " + V(_vreg(ops[1])) + ", " + V(_vreg(ops[2])) + ");"});
    if (mn == "vmr") return L({"ppc_vmove(" + V(_vreg(ops[0])) + ", " + V(_vreg(ops[1])) + ");"});
    if (mn == "vperm" || mn == "vsel")
        return L({"ppc_" + mn + "(" + V(_vreg(ops[0])) + ", " + V(_vreg(ops[1])) + ", " + V(_vreg(ops[2])) + ", " + V(_vreg(ops[3])) + ");"});
    if (mn == "vspltw" || mn == "vsplth" || mn == "vspltb")
        return L({"ppc_" + mn + "(" + V(_vreg(ops[0])) + ", " + V(_vreg(ops[1])) + ", " + N(_imm(ops[2])) + ");"});
    if (mn == "vspltisw" || mn == "vspltish" || mn == "vspltisb") {
        const Num d = _vreg(ops[0]), imm = _imm(ops[1]);
        const char* fld = mn == "vspltisw" ? "s32" : mn == "vspltish" ? "s16" : "s8";
        const int n = mn == "vspltisw" ? 4 : mn == "vspltish" ? 8 : 16;
        return L({"for (int _i=0;_i<" + std::to_string(n) + ";_i++) ctx.v" + N(d) + "." + fld + "[_i] = " + N(imm) + ";"});
    }
    if (mn == "lvx" || mn == "lvxl" || mn == "lvx128") {
        const Num a = _reg(ops[1]), b = _reg(ops[2]);
        return L({"{ uint32_t _ea = (" + BE(a) + " + " + R(b, "u32") + ") & ~0xFu; " + "for (int _i=0;_i<16;_i++) ctx.v" + N(_vreg(ops[0])) +
                  ".u8[_i] = PPC_LOAD_U8(_ea+_i); }"});
    }
    if (mn == "stvx" || mn == "stvxl" || mn == "stvx128") {
        const Num a = _reg(ops[1]), b = _reg(ops[2]);
        return L({"{ uint32_t _ea = (" + BE(a) + " + " + R(b, "u32") + ") & ~0xFu; " + "for (int _i=0;_i<16;_i++) PPC_STORE_U8(_ea+_i, ctx.v" +
                  N(_vreg(ops[0])) + ".u8[_i]); }"});
    }

    auto target = [&]() -> Num { return last_hex_token(op_str); };

    auto call_or_goto = [&](const Num& tgt_, const std::optional<std::string>& conditional_pred) -> py::list<py::str> {
        // in-function -> goto label; out-of-function -> tail-call another function
        const int64_t tgt = need(tgt_);
        std::string stmt;
        if (func_lo <= tgt && tgt < func_hi) stmt = "goto loc_" + x8(static_cast<uint64_t>(tgt)) + ";";
        else stmt = "sub_" + x8(static_cast<uint64_t>(tgt)) + "(ctx, base); return;";
        if (conditional_pred) return L({"if (" + *conditional_pred + ") { " + stmt + " }"});
        return L({stmt});
    };

    // ---- moves / SPRs -------------------------------------------------
    if (mn == "mflr") return L({R(_reg(ops[0])) + " = ctx.lr;"});
    if (mn == "mtlr") return L({"ctx.lr = " + R(_reg(ops[0])) + ";"});
    if (mn == "mfctr") return L({R(_reg(ops[0])) + " = ctx.ctr.u64;"});
    if (mn == "mtctr") return L({"ctx.ctr.u64 = " + R(_reg(ops[0])) + ";"});
    if (mn == "mr") return L({R(_reg(ops[0])) + " = " + R(_reg(ops[1])) + ";"});
    if (mn == "nop") return L({"/* nop */"});

    // ---- immediates / address building --------------------------------
    if (mn == "li") return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = " + N(_imm(ops[1])) + ";"});
    if (mn == "lis") return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = (int32_t)0x" + x8(shl16_32(_imm(ops[1]))) + ";"});

    // ---- ALU ----------------------------------------------------------
    if (mn == "add" || mn == "addc") return L({R(_reg(ops[0])) + " = " + R(_reg(ops[1])) + " + " + R(_reg(ops[2])) + ";"});
    if (mn == "addi") return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = " + R(_reg(ops[1]), "s64") + " + " + N(_imm(ops[2])) + ";"});
    if (mn == "addis")
        return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = " + R(_reg(ops[1]), "s64") + " + (int32_t)0x" + x8(shl16_32(_imm(ops[2]))) + ";"});
    if (mn == "subf")   // subf rD,rA,rB = rB - rA
        return L({R(_reg(ops[0])) + " = " + R(_reg(ops[2])) + " - " + R(_reg(ops[1])) + ";"});
    if (mn == "sub")    // extended: sub rD,rB,rA
        return L({R(_reg(ops[0])) + " = " + R(_reg(ops[1])) + " - " + R(_reg(ops[2])) + ";"});
    if (mn == "neg") return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = -" + R(_reg(ops[1]), "s64") + ";"});
    if (mn == "mullw")
        return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = (int32_t)(" + R(_reg(ops[1]), "s32") + " * " + R(_reg(ops[2]), "s32") + ");"});
    if (mn == "mulli")
        return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = (int32_t)(" + R(_reg(ops[1]), "s32") + " * " + N(_imm(ops[2])) + ");"});
    if (mn == "mulhw")
        return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = (int32_t)(((int64_t)" + R(_reg(ops[1]), "s32") + " * (int64_t)" + R(_reg(ops[2]), "s32") +
                  ") >> 32);"});
    if (mn == "mulhwu")
        return L({"ctx.r" + N(_reg(ops[0])) + ".u64 = (uint32_t)(((uint64_t)" + R(_reg(ops[1]), "u32") + " * (uint64_t)" + R(_reg(ops[2]), "u32") +
                  ") >> 32);"});

    // ---- logical ------------------------------------------------------
    if (mn == "or") return L({R(_reg(ops[0])) + " = " + R(_reg(ops[1])) + " | " + R(_reg(ops[2])) + ";"});
    if (mn == "and") return L({R(_reg(ops[0])) + " = " + R(_reg(ops[1])) + " & " + R(_reg(ops[2])) + ";"});
    if (mn == "xor") return L({R(_reg(ops[0])) + " = " + R(_reg(ops[1])) + " ^ " + R(_reg(ops[2])) + ";"});
    if (mn == "ori") return L({R(_reg(ops[0])) + " = " + R(_reg(ops[1])) + " | " + std::to_string(low16(_imm(ops[2]))) + "u;"});
    if (mn == "oris") return L({R(_reg(ops[0])) + " = " + R(_reg(ops[1])) + " | 0x" + x8(shl16_32(_imm(ops[2]))) + "u;"});
    if (mn == "slwi") return L({"ctx.r" + N(_reg(ops[0])) + ".u64 = (uint32_t)(" + R(_reg(ops[1]), "u32") + " << " + N(_imm(ops[2])) + ");"});
    if (mn == "srwi") return L({"ctx.r" + N(_reg(ops[0])) + ".u64 = " + R(_reg(ops[1]), "u32") + " >> " + N(_imm(ops[2])) + ";"});
    if (mn == "rotlwi") return L({"ctx.r" + N(_reg(ops[0])) + ".u64 = ppc_rotl32(" + R(_reg(ops[1]), "u32") + ", " + N(_imm(ops[2])) + ");"});
    if (mn == "clrlwi")
        return L({"ctx.r" + N(_reg(ops[0])) + ".u64 = " + R(_reg(ops[1]), "u32") + " & (0xFFFFFFFFu >> " + N(_imm(ops[2])) + ");"});

    // ---- loads --------------------------------------------------------
    {
        static const std::vector<std::pair<const char*, const char*>> LD = {{"lbz", "U8"},   {"lhz", "U16"},  {"lha", "U16"}, {"lwz", "U32"},
                                                                            {"ld", "U64"},   {"lwzu", "U32"}, {"lbzu", "U8"},  {"lhzu", "U16"}};
        for (const auto& e : LD) {
            if (mn != e.first) continue;
            const auto mref = _mem(ops[1]);
            if (!mref) return L({"PPC_UNIMPLEMENTED(\"" + mn + " " + ostr + "\");"});
            const auto [disp, base] = *mref;
            std::vector<std::string> out = {"ctx.r" + N(_reg(ops[0])) + ".u64 = PPC_LOAD_" + e.second + "(" + _addr(disp, base).utf8() + ");"};
            if (ends(mn, "u") && base) out.push_back("ctx.r" + N(base) + ".u32 = " + _addr(disp, base).utf8() + ";");   // update form
            return L(out);
        }
        static const std::vector<std::pair<const char*, const char*>> LDX = {{"lwzx", "U32"}, {"lbzx", "U8"}, {"lhzx", "U16"}, {"ldx", "U64"}};
        for (const auto& e : LDX) {
            if (mn == e.first)
                return L({"ctx.r" + N(_reg(ops[0])) + ".u64 = PPC_LOAD_" + e.second + "(" + R(_reg(ops[1]), "u32") + " + " + R(_reg(ops[2]), "u32") + ");"});
        }
    }

    // ---- stores -------------------------------------------------------
    {
        static const std::vector<std::pair<const char*, const char*>> ST = {{"stb", "U8"},   {"sth", "U16"},   {"stw", "U32"}, {"std", "U64"},
                                                                            {"stwu", "U32"}, {"stbu", "U8"},   {"sthu", "U16"}};
        for (const auto& e : ST) {
            if (mn != e.first) continue;
            const auto mref = _mem(ops[1]);
            if (!mref) return L({"PPC_UNIMPLEMENTED(\"" + mn + " " + ostr + "\");"});
            const auto [disp, base] = *mref;
            const Num rs = _reg(ops[0]);
            if (ends(mn, "u") && base)   // store-with-update
                return L({"ea = " + _addr(disp, base).utf8() + ";", std::string("PPC_STORE_") + e.second + "(ea, ctx.r" + N(rs) + ".u32);",
                          "ctx.r" + N(base) + ".u32 = ea;"});
            return L({std::string("PPC_STORE_") + e.second + "(" + _addr(disp, base).utf8() + ", ctx.r" + N(rs) + ".u32);"});
        }
        static const std::vector<std::pair<const char*, const char*>> STX = {{"stwx", "U32"}, {"stbx", "U8"}, {"sthx", "U16"}, {"stdx", "U64"}};
        for (const auto& e : STX) {
            if (mn == e.first)
                return L({std::string("PPC_STORE_") + e.second + "(" + R(_reg(ops[1]), "u32") + " + " + R(_reg(ops[2]), "u32") + ", ctx.r" +
                          N(_reg(ops[0])) + ".u32);"});
        }
    }

    // ---- compares -----------------------------------------------------
    if (in(mn, {"cmpw", "cmpwi", "cmplw", "cmplwi", "cmpd", "cmpdi", "cmpld", "cmpldi"})) {
        const bool is_signed = !starts(mn, "cmpl");
        const bool wide = starts(mn, "cmpd") || starts(mn, "cmpld");
        const char* ty = is_signed ? (wide ? "int64_t" : "int32_t") : (wide ? "uint64_t" : "uint32_t");
        const char* fld = is_signed ? (wide ? "s64" : "s32") : (wide ? "u64" : "u32");
        // operands may lead with crN or default to cr0
        int64_t idx = 0;
        int64_t cr = 0;
        if (!ops.empty() && _cr(ops[0])) {
            cr = *_cr(ops[0]);
            idx = 1;
        }
        const std::string a = R(_reg(ops[idx]), fld);
        const py::str& b_tok = ops[idx + 1];
        const std::string b = _reg(b_tok) ? R(_reg(b_tok), fld) : N(_imm(b_tok));
        return L({"ctx.cr" + N(cr) + ".compare<" + ty + ">(" + a + ", " + b + ", ctx.xer);"});
    }

    // ---- branches -----------------------------------------------------
    if (mn == "blr") return L({"return;"});
    if (mn == "bctr") {
        if (jump_tables && jump_tables->contains(va)) {
            // A recovered switch table (jumptable.py): emit a real
            // switch on the index register instead of shattering control flow
            // through an indirect call. Local cases goto the block label;
            // out-of-function cases tail-call. Unknown index -> the honest
            // indirect call (the table's default/out-of-range path).
            const auto& jt = jump_tables->at(va);
            const int64_t idx_reg = std::get<0>(jt);
            const py::list<int64_t>& targets = std::get<1>(jt);
            std::vector<std::string> out = {"switch (ctx.r" + N(idx_reg) + ".u32) {"};
            int64_t i = 0;
            for (int64_t t : targets) {
                if (func_lo <= t && t < func_hi) out.push_back("case " + N(i) + "u: goto loc_" + x8(static_cast<uint64_t>(t)) + ";");
                else out.push_back("case " + N(i) + "u: sub_" + x8(static_cast<uint64_t>(t)) + "(ctx, base); return;");
                ++i;
            }
            out.push_back("default: PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32); return;");
            out.push_back("}");
            return L(out);
        }
        return L({"PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32); return;"});
    }
    if (mn == "bctrl") return L({"ctx.lr = 0x" + x8(static_cast<uint64_t>(va + WORD)) + ";", "PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);"});
    if (mn == "blrl")   // call through LR (LR becomes the return address)
        return L({"{ uint32_t _to = (uint32_t)ctx.lr; ctx.lr = 0x" + x8(static_cast<uint64_t>(va + WORD)) + "; PPC_CALL_INDIRECT_FUNC(_to); }"});
    // conditional jumps through CTR (beqctr ...), and returns that count CTR down (bdzlr, bdnzlr)
    const std::string mn_nohint = rstrip_chars(mn, "+-");
    {
        static const std::vector<std::pair<const char*, const char*>> cond_ctr = {
            {"beqctr", "eq"}, {"bnectr", "!eq"}, {"bltctr", "lt"}, {"bgectr", "!lt"}, {"bgtctr", "gt"}, {"blectr", "!gt"}};
        for (const auto& e : cond_ctr) {
            if (mn_nohint != e.first) continue;
            const std::string m = e.second;
            const Num c = !ops.empty() ? _cr(ops[0]) : Num();
            const int64_t cr = c ? *c : 0;
            const std::string pred = m[0] == '!' ? "!ctx.cr" + N(cr) + "." + m.substr(1) : "ctx.cr" + N(cr) + "." + m;
            return L({"if (" + pred + ") { PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32); return; }"});
        }
    }
    if (mn_nohint == "bdzlr" || mn_nohint == "bdnzlr") {
        const char* test = starts(mn, "bdz") ? "==" : "!=";
        return L({"ctx.ctr.u64 -= 1;", std::string("if (ctx.ctr.u64 ") + test + " 0) return;"});
    }
    if (mn == "b" || mn == "ba") return call_or_goto(target(), std::nullopt);
    if (mn == "bl" || mn == "bla") {
        const Num tgt = target();
        if (!tgt) throw py::TypeError("unsupported format string passed to NoneType.__format__");
        return L({"ctx.lr = 0x" + x8(static_cast<uint64_t>(va + WORD)) + ";", "sub_" + x8(static_cast<uint64_t>(*tgt)) + "(ctx, base);"});
    }
    const std::string base_mn = mn_nohint;            // strip branch-hint suffix
    if (_COND.contains(py::str(base_mn))) {
        int64_t cr = 0;
        if (!ops.empty() && _cr(ops[0])) cr = *_cr(ops[0]);
        std::string pred = _COND.at(py::str(base_mn)).utf8();
        const std::string field = "ctx.cr" + N(cr);
        for (size_t p = pred.find("{cr}"); p != std::string::npos; p = pred.find("{cr}")) pred.replace(p, 4, field);
        return call_or_goto(target(), pred);
    }
    if (in(base_mn, {"beqlr", "bnelr", "bltlr", "bgelr", "bgtlr", "blelr"})) {
        const std::string m = base_mn == "beqlr" ? "eq" : base_mn == "bnelr" ? "!eq" : base_mn == "bltlr" ? "lt" : base_mn == "bgelr" ? "!lt"
                                                         : base_mn == "bgtlr" ? "gt" : "!gt";
        const Num c = !ops.empty() ? _cr(ops[0]) : Num();
        const int64_t cr = c ? *c : 0;
        const std::string pred = m[0] == '!' ? "!ctx.cr" + N(cr) + "." + m.substr(1) : "ctx.cr" + N(cr) + "." + m;
        return L({"if (" + pred + ") return;"});
    }

    // ---- CTR-decrement branches (bdnz/bdz) ----------------------------
    if (base_mn == "bdnz" || base_mn == "bdz") {
        const std::string pred = base_mn == "bdnz" ? "ctx.ctr.u64 != 0" : "ctx.ctr.u64 == 0";
        py::list<py::str> out = L({"ctx.ctr.u64 -= 1;"});
        out.extend(call_or_goto(target(), pred));
        return out;
    }
    // ...and with a condition bit too: bdnzf/bdnzt/bdzf/bdzt <bit>, <target>; the bit is written
    // "eq" (in cr0) or "4*crN+eq"
    if (in(base_mn, {"bdnzf", "bdnzt", "bdzf", "bdzt"}) && ops.size() == 2) {
        static const py::re::Pattern re("(?:4\\*cr(\\d)\\+)?(lt|gt|eq|so|un)", 0);
        const auto m = re.fullmatch(ops[0].replace(py::str(" "), py::str("")));
        if (m) {
            const auto g1 = m->group_opt(1);
            const int64_t field = g1 ? py::int_parse(*g1) : 0;
            const py::str g2 = m->group(2);
            const std::string bit = g2 == py::str("un") ? "so" : g2.utf8();
            const std::string ctr = starts(base_mn, "bdnz") ? "ctx.ctr.u64 != 0" : "ctx.ctr.u64 == 0";
            const std::string cond = (ends(base_mn, "t") ? "" : "!") + std::string("ctx.cr") + N(field) + "." + bit;
            py::list<py::str> out = L({"ctx.ctr.u64 -= 1;"});
            out.extend(call_or_goto(Num(py::int_parse(ops[1], 0)), ctr + " && " + cond));
            return out;
        }
    }

    // ---- integer + carry (extend/negate carrying) ---------------------
    if (mn == "addze") {   // rD = rA + XER[CA]
        const Num d = _reg(ops[0]), a = _reg(ops[1]);
        return L({"{ uint64_t _s = (uint64_t)" + R(a, "u32") + " + ctx.xer.ca; " + "ctx.r" + N(d) + ".u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }"});
    }
    if (mn == "addme") {   // rD = rA + XER[CA] - 1
        const Num d = _reg(ops[0]), a = _reg(ops[1]);
        return L({"{ uint64_t _s = (uint64_t)" + R(a, "u32") + " + ctx.xer.ca + 0xFFFFFFFFull; " + "ctx.r" + N(d) +
                  ".u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }"});
    }
    if (mn == "subfze") {   // rD = ~rA + XER[CA]
        const Num d = _reg(ops[0]), a = _reg(ops[1]);
        return L({"{ uint64_t _s = (uint64_t)(uint32_t)(~" + R(a, "u32") + ") + ctx.xer.ca; " + "ctx.r" + N(d) +
                  ".u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }"});
    }
    if (mn == "subfme") {   // rD = ~rA + XER[CA] - 1
        const Num d = _reg(ops[0]), a = _reg(ops[1]);
        return L({"{ uint64_t _s = (uint64_t)(uint32_t)(~" + R(a, "u32") + ") + ctx.xer.ca + 0xFFFFFFFFull; " + "ctx.r" + N(d) +
                  ".u64 = (uint32_t)_s; ctx.xer.ca = (uint8_t)(_s >> 32); }"});
    }

    // ---- logical immediates -------------------------------------------
    if (mn == "xori") return L({R(_reg(ops[0])) + " = " + R(_reg(ops[1])) + " ^ " + std::to_string(low16(_imm(ops[2]))) + "u;"});
    if (mn == "xoris") return L({R(_reg(ops[0])) + " = " + R(_reg(ops[1])) + " ^ 0x" + x8(shl16_32(_imm(ops[2]))) + "u;"});
    if (mn == "andis") return L({"ctx.r" + N(_reg(ops[0])) + ".u64 = " + R(_reg(ops[1]), "u64") + " & 0x" + x8(shl16_32(_imm(ops[2]))) + "ull;"});

    // ---- 64-bit rotate + mask (rldic family) --------------------------
    if (in(mn, {"rldicl", "rldicr", "rldic", "rldimi"})) {
        const Num d = _reg(ops[0]), s = _reg(ops[1]), sh = _imm(ops[2]);
        const Num n = _imm(ops[3]);
        const std::string rot = "ppc_rotl64(" + R(s, "u64") + ", " + N(sh) + ")";
        if (mn == "rldicl")   // clear left: keep bits [n..63]
            return L({"ctx.r" + N(d) + ".u64 = " + rot + " & (~0ull >> " + N(n) + ");"});
        if (mn == "rldicr")   // clear right: keep bits [0..n]
            return L({"ctx.r" + N(d) + ".u64 = " + rot + " & (~0ull << (63 - " + N(n) + "));"});
        if (mn == "rldic")    // mask [n .. 63-sh]
            return L({"ctx.r" + N(d) + ".u64 = " + rot + " & ((~0ull >> " + N(n) + ") & (~0ull << " + N(sh) + "));"});
        // rldimi: insert rot into rD under mask [n .. 63-sh]
        return L({"{ uint64_t _m = (~0ull >> " + N(n) + ") & (~0ull << " + N(sh) + "); " + "ctx.r" + N(d) + ".u64 = (" + rot + " & _m) | (ctx.r" + N(d) +
                  ".u64 & ~_m); }"});
    }

    // ---- load word algebraic (sign-extend 32->64) ---------------------
    if (mn == "lwa") {
        const auto mref = _mem(ops[1]);
        if (mref) {
            const auto [disp, base] = *mref;
            return L({"ctx.r" + N(_reg(ops[0])) + ".s64 = (int32_t)PPC_LOAD_U32(" + _addr(disp, base).utf8() + ");"});
        }
    }
    if (mn == "lwax" || mn == "lwaux") {
        const Num a = _reg(ops[1]), b = _reg(ops[2]);
        std::vector<std::string> out = {"ctx.r" + N(_reg(ops[0])) + ".s64 = (int32_t)PPC_LOAD_U32(" + BE(a) + " + " + R(b, "u32") + ");"};
        if (mn == "lwaux" && a && *a) out.push_back("ctx.r" + N(a) + ".u32 = " + BE(a) + " + " + R(b, "u32") + ";");
        return L(out);
    }

    // ---- float indexed / update loads & stores ------------------------
    if (in(mn, {"lfsx", "lfsux", "lfdx", "lfdux"})) {
        const Num a = _reg(ops[1]), b = _reg(ops[2]);
        const std::string ea = BE(a) + " + " + R(b, "u32");
        const Num fd = _freg(ops[0]);
        std::vector<std::string> out;
        if (starts(mn, "lfs"))
            out.push_back("{ uint32_t _t = PPC_LOAD_U32(" + ea + "); float _f; __builtin_memcpy(&_f,&_t,4); " + F(fd) + ".f64 = _f; }");
        else
            out.push_back("{ uint64_t _t = PPC_LOAD_U64(" + ea + "); double _d; __builtin_memcpy(&_d,&_t,8); " + F(fd) + ".f64 = _d; }");
        if (ends(mn, "ux") && a && *a) out.push_back("ctx.r" + N(a) + ".u32 = " + ea + ";");
        return L(out);
    }
    if (in(mn, {"stfsx", "stfsux", "stfdx", "stfdux"})) {
        const Num a = _reg(ops[1]), b = _reg(ops[2]);
        const std::string ea = BE(a) + " + " + R(b, "u32");
        const Num fs = _freg(ops[0]);
        std::vector<std::string> out;
        if (starts(mn, "stfs"))
            out.push_back("{ float _f = (float)" + F(fs) + ".f64; uint32_t _t; __builtin_memcpy(&_t,&_f,4); PPC_STORE_U32(" + ea + ", _t); }");
        else
            out.push_back("{ double _d = " + F(fs) + ".f64; uint64_t _t; __builtin_memcpy(&_t,&_d,8); PPC_STORE_U64(" + ea + ", _t); }");
        if (ends(mn, "ux") && a && *a) out.push_back("ctx.r" + N(a) + ".u32 = " + ea + ";");
        return L(out);
    }

    // ---- not translated yet — honest trap (never a silent wrong result) --
    return L({"PPC_UNIMPLEMENTED(\"" + mn + " " + ostr + "\");"});
}

// ---------------------------------------------------------------------------------------------------------------------
// whole-function emission
// ---------------------------------------------------------------------------------------------------------------------

namespace {

// int.from_bytes(raw, "big") of a (possibly short) byte string
int64_t be_value(const std::string& s) {
    int64_t v = 0;
    for (unsigned char c : s) v = (v << 8) | c;
    return v;
}

// re.search(r"sub_([0-9A-F]{8})\(ctx", stmt): the address, or none
std::optional<int64_t> called_in(const std::string& stmt) {
    for (size_t p = stmt.find("sub_"); p != std::string::npos; p = stmt.find("sub_", p + 1)) {
        if (p + 4 + 8 + 4 > stmt.size()) continue;
        bool ok = true;
        int64_t v = 0;
        for (size_t k = 0; k < 8; ++k) {
            const char c = stmt[p + 4 + k];
            int d;
            if (c >= '0' && c <= '9') d = c - '0';
            else if (c >= 'A' && c <= 'F') d = c - 'A' + 10;
            else {
                ok = false;
                break;
            }
            v = v * 16 + d;
        }
        if (ok && stmt.compare(p + 12, 4, "(ctx") == 0) return v;
    }
    return std::nullopt;
}

}  // namespace

std::tuple<py::str, py::set<int64_t>, Stats> recompile_function(const py::bytes& body, int64_t base_va, int64_t func_va, int64_t size,
                                                                const py::bytes* img, int64_t img_base,
                                                                const std::function<bool(int64_t)>& is_exec) {
    (void)base_va;
    const int64_t n = size - py::mod(size, WORD);
    const int64_t func_lo = func_va, func_hi = func_va + n;
    const auto listing = disassemble::listing(body, func_va, 0, py::floordiv(n, WORD));
    std::vector<std::tuple<int64_t, int64_t, py::str, py::str>> insns;   // (va, raw word, mnemonic, operands)
    for (const auto& row : listing) insns.emplace_back(std::get<0>(row), be_value(std::get<1>(row).raw()), std::get<2>(row), std::get<3>(row));

    // pass 1: collect intra-function branch targets -> labels
    py::set<int64_t> labels;
    for (const auto& row : insns) {
        if (std::get<2>(row).startswith(py::str("b"))) {
            const Num t = last_hex_token(std::get<3>(row));
            if (t && func_lo <= *t && *t < func_hi) labels.add(*t);
        }
    }

    // pass 1b: recover bctr jump tables from the image (jumptable.py).
    // Any in-function case target becomes a block label so the switch can goto it.
    JumpTables jump_tables;
    if (img) {
        const std::string& bytes_ = img->raw();
        auto rd = [&](int64_t a, int64_t w) -> std::optional<int64_t> {
            const int64_t o = a - img_base;
            if (0 <= o && o + w <= static_cast<int64_t>(bytes_.size())) return be_value(bytes_.substr(static_cast<size_t>(o), static_cast<size_t>(w)));
            return std::nullopt;
        };
        const std::function<bool(int64_t)> isx = is_exec ? is_exec : [](int64_t) { return true; };
        try {
            py::list<jumptable::Insn> jt_insns;
            for (const auto& row : insns)
                jt_insns.append(jumptable::Insn(std::get<0>(row), std::variant<int64_t, py::bytes>(std::get<1>(row)), std::get<2>(row), std::get<3>(row)));
            jump_tables = jumptable::detect_jump_tables(
                jt_insns, func_lo, func_hi, [&](int64_t a) { return rd(a, 1); }, [&](int64_t a) { return rd(a, 2); },
                [&](int64_t a) { return rd(a, 4); }, isx);
        } catch (const std::exception&) {
            jump_tables = JumpTables();
        }
        for (const auto& kv : jump_tables.items())
            for (int64_t t : std::get<1>(std::get<1>(kv)))
                if (func_lo <= t && t < func_hi) labels.add(t);
    }

    // pass 2: emit
    py::set<int64_t> called;
    std::vector<std::string> lines;
    // (A weak `__imp__` alias for the host to override would need __attribute__((alias)), an
    // ELF feature; the macro PPC_FUNC_IMPL carries the model instead, so the body is portable.)
    lines.push_back("PPC_FUNC_IMPL(sub_" + x8(static_cast<uint64_t>(func_va)) + ") {");
    lines.push_back("\tPPC_FUNC_PROLOGUE();");
    lines.push_back("\tuint32_t ea{};");
    lines.push_back("\tPPCRegister temp{};");   // scratch for carry/rotate builder sequences
    int64_t total = 0, unimpl = 0;
    for (const auto& row : insns) {
        const int64_t va = std::get<0>(row), raw = std::get<1>(row);
        const py::str& mn = std::get<2>(row);
        const py::str& op_str = std::get<3>(row);
        ++total;
        if (labels.contains(va)) lines.push_back("loc_" + x8(static_cast<uint64_t>(va)) + ":");
        {
            std::string c = "\t// " + mn.utf8() + " " + op_str.utf8();
            while (!c.empty() && (c.back() == ' ' || c.back() == '\t' || c.back() == '\n' || c.back() == '\r' || c.back() == '\v' || c.back() == '\f'))
                c.pop_back();
            lines.push_back(c);
        }
        py::list<py::str> stmts = emit_insn(va, mn, op_str, labels, func_lo, func_hi, raw, jump_tables.empty() ? std::optional<JumpTables>() : std::optional<JumpTables>(jump_tables));
        // safety net: a handler that failed to parse an operand emits a stray
        // "None" (e.g. ctx.rNone). Never ship broken C++ — fall back to an honest
        // trap so the byte stays accounted for and the file still compiles.
        bool stray = false;
        for (const py::str& s : stmts)
            if (s.contains(py::str("None"))) stray = true;
        if (stray) stmts = L({"PPC_UNIMPLEMENTED(\"" + mn.utf8() + " " + op_str.utf8() + "\");"});
        for (const py::str& stmt_ : stmts) {
            const std::string stmt = stmt_.utf8();
            lines.push_back("\t" + stmt);
            if (stmt.find("PPC_UNIMPLEMENTED") != std::string::npos) ++unimpl;
            if (const auto c = called_in(stmt)) called.add(*c);
        }
    }
    lines.push_back("}");
    bool vtemp = false;
    for (const std::string& l : lines)
        if (l.find("vTemp") != std::string::npos) vtemp = true;
    if (vtemp) lines.insert(lines.begin() + 4, "\tPPCVRegister vTemp{};");   // (the scratch vector some vector builders use)
    Stats stats;
    stats.total = total;
    stats.unimpl = unimpl;
    stats.translated = total - unimpl;
    stats.labels = labels.size();
    std::string text;
    for (const std::string& l : lines) {
        text += l;
        text += '\n';
    }
    return {py::str(text), called, stats};
}

// ---------------------------------------------------------------------------------------------------------------------
// the runtime header: enough of rex/ppc.h for the output to compile on its own
// ---------------------------------------------------------------------------------------------------------------------

namespace {
py::str vec_helpers() { return _VEC_HELPERS; }
}  // namespace

py::str runtime_header() {
    auto lines = [](const char* fmt, int count) {
        std::string s;
        char buf[128];
        for (int i = 0; i < count; ++i) {
            std::snprintf(buf, sizeof buf, fmt, i);
            if (i) s += '\n';
            s += buf;
        }
        return py::str(s);
    };
    const py::str regs = lines("    PPCRegister r%d;", 32);
    const py::str fprs = lines("    PPCRegister f%d;", 32);
    const py::str vrs = lines("    PPCVRegister v%d;", 128);
    const py::str crs = lines("    PPCCRRegister cr%d;", 8);
    py::list<py::str> load_lines, store_lines;
    for (const auto& e : {std::make_tuple("U8", "uint8_t", ""), std::make_tuple("U16", "uint16_t", "__builtin_bswap16"),
                          std::make_tuple("U32", "uint32_t", "__builtin_bswap32"), std::make_tuple("U64", "uint64_t", "__builtin_bswap64")}) {
        const std::string w = std::get<0>(e), t = std::get<1>(e), sw = std::get<2>(e);
        load_lines.append(py::str("#define PPC_LOAD_" + w + "(x)  (" + sw + "(*(volatile " + t + "*)(base + (uint32_t)(x))))"));
        store_lines.append(py::str("#define PPC_STORE_" + w + "(x,y)  (*(volatile " + t + "*)(base + (uint32_t)(x)) = (" + sw + "(((" + t + ")(y)))))"));
    }
    const py::str loads = py::join(py::str("\n"), load_lines);
    const py::str stores = py::join(py::str("\n"), store_lines);
    // Documentation of the runtime symbols the builders reference (the
    // vector/memory helper prototypes + the whole-vector shuffle tables). In
    // BRIDGE mode these are real definitions from <rex/ppc/context.h|memory.h>;
    // kept here (comment-only) so the standalone contract is self-describing.
    py::list<py::str> helper_parts;
    for (const py::str* h : {&builders::memory::RUNTIME_HELPERS, &builders::vector::RUNTIME_HELPERS})
        if (!h->strip().empty()) helper_parts.append(*h);
    const py::str port_helpers = py::join(py::str("\n"), helper_parts);
    py::str out;
    out += py::str(
        "// rex_ppc_min.h - the runtime of CodeBreaker's recompiled PowerPC code.\n"
        "// When the rexglue kernel (native/rexglue/kernel) is on the include path (<rex/ppc/context.h>),\n"
        "// this BRIDGES onto its PPCContext so the recompiled code and the kernel share\n"
        "// one runtime ABI (the register types are byte-compatible - ours were modeled on\n"
        "// the kernel's). Otherwise it falls back to a standalone context of its own that compiles\n"
        "// alone. Define REX_PPC_STANDALONE to force the standalone path.\n"
        "#pragma once\n"
        "#include <cstdint>\n"
        "#include <cmath>     // the floating-point builders (fabs/sqrt/frsp/fsel ...)\n"
        "#include <climits>   // the conversion builders (saturation bounds)\n"
        "\n"
        "#if __has_include(<rex/ppc/context.h>) && !defined(REX_PPC_STANDALONE)\n"
        "// ---- BRIDGE: the rexglue kernel's PPCContext --------------------------------\n"
        "#define REX_PPC_BRIDGE 1\n"
        "#include <rex/ppc/context.h>       // PPCContext(+kernel_state), reg types, PPC_FUNC, PPCFuncMapping\n"
        "using PPCVRegister = rex::VRegister;\n"
        "#else\n"
        "// ---- STANDALONE: a minimal context of its own -------------------------------\n"
        "union PPCRegister {\n"
        "    int8_t s8; uint8_t u8; int16_t s16; uint16_t u16;\n"
        "    int32_t s32; uint32_t u32; int64_t s64; uint64_t u64; float f32; double f64;\n"
        "};\n"
        "struct PPCXERRegister { uint8_t so, ov, ca; };\n"
        "struct PPCCRRegister {\n"
        "    uint8_t lt, gt, eq, so;\n"
        "    template <class T> void compare(T a, T b, const PPCXERRegister& xer) {\n"
        "        lt = a < b; gt = a > b; eq = a == b; so = xer.so;\n"
        "    }\n"
        "};\n"
        "union alignas(16) PPCVRegister {\n"
        "    int8_t s8[16]; uint8_t u8[16]; int16_t s16[8]; uint16_t u16[8];\n"
        "    int32_t s32[4]; uint32_t u32[4]; int64_t s64[2]; uint64_t u64[2];\n"
        "    float f32[4]; double f64[2];\n"
        "};\n"
        "struct alignas(64) PPCContext {\n");
    out += regs;
    out += py::str(
        "\n"
        "    uint64_t lr;\n"
        "    PPCRegister ctr;\n"
        "    PPCXERRegister xer;\n"
        "    PPCRegister reserved;   // lwarx/stwcx reservation value (atomics)\n");
    out += crs;
    out += py::str(
        "\n");
    out += fprs;
    out += py::str(
        "\n");
    out += vrs;
    out += py::str(
        "\n"
        "};\n"
        "using PPCFunc = void(PPCContext& ctx, uint8_t* base);\n"
        "#define PPC_FUNC(x)        void x(PPCContext& __restrict ctx, uint8_t* base)\n"
        "#define PPC_FUNC_IMPL(x)   PPC_FUNC(x)\n"
        "#define PPC_WEAK_FUNC(x)   PPC_FUNC(x)\n"
        "#define PPC_FUNC_PROLOGUE()  do { (void)ctx; (void)base; } while (0)\n"
        "#define PPC_UNIMPLEMENTED(s) do { /* trap: {s} not yet translated */ } while (0)\n"
        "#endif\n"
        "\n"
        "// ---- shared helpers (PPCVRegister is defined in both branches) --------------\n"
        "static inline uint32_t ppc_rotl32(uint32_t v, uint32_t n) {\n"
        "    n &= 31; return n ? ((v << n) | (v >> (32 - n))) : v;\n"
        "}\n"
        "static inline uint64_t ppc_rotl64(uint64_t v, uint32_t n) {\n"
        "    n &= 63; return n ? ((v << n) | (v >> (64 - n))) : v;\n"
        "}\n"
        "// PowerPC big-endian bit numbering (bit 0 = MSB): mask bits [mb, me] inclusive,\n"
        "// wrapping when mb > me.\n"
        "static inline uint32_t ppc_mask32(int mb, int me) {\n"
        "    uint32_t begin = 0xFFFFFFFFu >> mb;\n"
        "    uint32_t end = (me >= 31) ? 0xFFFFFFFFu : (0xFFFFFFFFu << (31 - me));\n"
        "    return (mb <= me) ? (begin & end) : (begin | end);\n"
        "}\n"
        "static inline uint32_t ppc_rlwinm(uint32_t v, int sh, int mb, int me) {\n"
        "    return ppc_rotl32(v, sh) & ppc_mask32(mb, me);\n"
        "}\n"
        "static inline uint32_t ppc_rlwimi(uint32_t cur, uint32_t v, int sh, int mb, int me) {\n"
        "    uint32_t m = ppc_mask32(mb, me);\n"
        "    return (cur & ~m) | (ppc_rotl32(v, sh) & m);\n"
        "}\n"
        "// AltiVec (128-bit) helpers. NOTE: element order follows the register's native\n"
        "// lanes; full big-endian vector-load correctness is validated on real runs.\n"
        "static inline void ppc_vmove(PPCVRegister& d, const PPCVRegister& a) {\n"
        "    d.u64[0] = a.u64[0]; d.u64[1] = a.u64[1];\n"
        "}\n");
    out += vec_helpers();
    out += py::str(
        "\n"
        "\n"
        "// ---- indirect calls + memory access ----------------------------------------\n"
        "// In bridge mode, the kernel's macros are replaced by the forms the generated code\n"
        "// uses (1-argument PPC_UNIMPLEMENTED; non-extern-C PPC_FUNC_IMPL; its indirect call).\n"
        "#if defined(REX_PPC_BRIDGE)\n"
        "#undef PPC_UNIMPLEMENTED\n"
        "#define PPC_UNIMPLEMENTED(s) do { /* {s} */ } while (0)\n"
        "#undef PPC_FUNC_IMPL\n"
        "#define PPC_FUNC_IMPL(x) PPC_FUNC(x)\n"
        "#undef PPC_CALL_INDIRECT_FUNC\n"
        "#endif\n"
        "extern \"C\" void ppc_call_indirect(PPCContext& ctx, uint8_t* base, uint32_t addr);\n"
        "#define PPC_CALL_INDIRECT_FUNC(x)  ppc_call_indirect(ctx, base, (x))\n"
        "\n");
    out += loads;
    out += py::str(
        "\n");
    out += stores;
    out += py::str(
        "\n"
        "\n"
        "// ---- runtime symbols the builders reference --------------------------------\n"
        "// (bridge mode: real definitions come from <rex/ppc/context.h> + <memory.h>)\n");
    out += port_helpers;
    out += py::str(
        "\n");
    return out;
}

py::str _emit_forward_decls(const py::set<int64_t>& called) {
    std::vector<int64_t> v;
    for (int64_t a : called) v.push_back(a);
    std::sort(v.begin(), v.end());
    std::string s;
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) s += '\n';
        s += "extern PPC_FUNC(sub_" + x8(static_cast<uint64_t>(v[i])) + ");";
    }
    return py::str(s);
}

std::tuple<py::str, py::str, Agg> recompile_image(const py::str& path, std::optional<int64_t> limit, std::optional<int64_t> jobs,
                                                  const std::optional<py::set<int64_t>>& skip_in, const std::optional<py::list<int64_t>>& extra) {
    py::set<int64_t> skip;
    if (skip_in) skip = py::set<int64_t>::from(*skip_in);
    const auto found = discover::discover(path, true, extra);
    const py::dict<int64_t, int64_t>& funcs = std::get<0>(found);
    const py::ref<powerpc_image::Image> image = std::get<2>(found);
    const auto loaded = disassemble::image(path);
    const py::bytes& img_bytes = std::get<0>(loaded);
    const int64_t base = std::get<1>(loaded);
    const std::string& img = img_bytes.raw();
    std::vector<int64_t> addrs;
    for (const auto& kv : funcs.items()) addrs.push_back(std::get<0>(kv));
    std::sort(addrs.begin(), addrs.end());

    // Bytes from `va` through its first blr / bctr / plain branch. A function with no .pdata
    // length runs at least that far: the register save and restore runs have an entry every
    // word (__savegprlr_14, _15 ...), so the gap to the next start is a single instruction, but
    // each entry runs on to the run's end.
    auto word = [&img](int64_t o) -> int64_t {
        const int64_t n = static_cast<int64_t>(img.size());
        if (o >= 0 && o + 4 <= n) {
            const unsigned char* p = reinterpret_cast<const unsigned char*>(img.data()) + o;
            return (static_cast<int64_t>(p[0]) << 24) | (static_cast<int64_t>(p[1]) << 16) | (static_cast<int64_t>(p[2]) << 8) | p[3];
        }
        int64_t a = o, b = o + 4;
        auto norm = [n](int64_t i) { return i < 0 ? std::max<int64_t>(i + n, 0) : std::min(i, n); };
        a = norm(a);
        b = norm(b);
        int64_t v = 0;
        for (int64_t i = a; i < b; ++i) v = (v << 8) | static_cast<unsigned char>(img[static_cast<size_t>(i)]);
        return v;
    };
    auto to_end = [&](int64_t va) -> int64_t {
        const int64_t off = va - base;
        for (int64_t k = off; k < std::min<int64_t>(static_cast<int64_t>(img.size()), off + 0x4000) - 3; k += 4) {
            const int64_t w = word(k);
            if (w == 0x4E800020 || w == 0x4E800420 || ((w >> 26) == 18 && !(w & 1))) return k + 4 - off;
        }
        return 0x100;
    };
    auto size_of = [&](int64_t va, std::optional<int64_t> nxt) -> int64_t {
        const int64_t gap = std::min(nxt ? (*nxt - va) : int64_t(0x100), int64_t(0x4000));
        return gap <= 0x40 ? std::max(gap, to_end(va)) : gap;
    };
    // a function with no .pdata length runs to the next start; the import thunks are starts too
    // (a static initializer just before the thunk table would otherwise take the table in)
    std::vector<int64_t> bounds(addrs);
    for (const auto& kv : image->imports.items()) bounds.push_back(std::get<0>(kv));
    std::sort(bounds.begin(), bounds.end());
    bounds.erase(std::unique(bounds.begin(), bounds.end()), bounds.end());
    auto next_start = [&bounds](int64_t va) -> std::optional<int64_t> {
        const auto it = std::upper_bound(bounds.begin(), bounds.end(), va);
        if (it == bounds.end()) return std::nullopt;
        return *it;
    };
    std::unordered_map<int64_t, int64_t> sizes;
    for (int64_t va : addrs) {
        const int64_t listed = funcs.at(va);
        sizes[va] = listed ? listed : size_of(va, next_start(va));
    }
    const bool limited = limit && *limit != 0;
    auto head = [&](int64_t count) {
        std::vector<int64_t> v;
        // addrs[:limit] (a negative limit counts from the end, as a Python slice does)
        const int64_t n = static_cast<int64_t>(addrs.size());
        int64_t end = count < 0 ? std::max<int64_t>(count + n, 0) : std::min(count, n);
        v.assign(addrs.begin(), addrs.begin() + end);
        return v;
    };
    std::vector<int64_t> chosen = limited ? head(*limit) : addrs;
    std::vector<int64_t> emit_addrs;
    for (int64_t a : chosen)
        if (!skip.contains(a)) emit_addrs.push_back(a);
    const int64_t code_lo = addrs.empty() ? base : addrs.front();
    const int64_t code_hi = addrs.empty() ? base : addrs.back() + sizes.at(addrs.back());   // (a function's end)
    auto is_exec_fn = [code_lo, code_hi](int64_t a) { return (a & 3) == 0 && code_lo <= a && a < code_hi; };

    using Result = std::tuple<py::str, py::set<int64_t>, Stats>;
    auto one = [&](int64_t va) -> std::optional<Result> {
        const int64_t off = va - base;
        const int64_t sz = sizes.at(va);
        // img[va - base : va - base + size]
        py::bytes fbody = img_bytes.slice(off, off + sz);
        if (fbody.empty()) return std::nullopt;
        return recompile_function(fbody, va, va, sz, &img_bytes, base, is_exec_fn);
    };
    // Recompile, then recompile what the code calls that is not yet a function -- the targets of
    // tail jumps (a `b` into another function: the register restore runs are entered that way,
    // one entry per register count) -- until nothing called is missing.
    std::unordered_map<int64_t, std::optional<Result>> results;
    std::vector<int64_t> pending(emit_addrs);
    int64_t rounds = 0;
    const int64_t lo = std::get<0>(image->code), hi = std::get<1>(image->code);
    while (!pending.empty() && rounds < 8) {
        // (the functions are recompiled on a pool of threads, each decoding with its own Capstone)
        std::vector<std::optional<Result>> got(pending.size());
        const int64_t workers = std::max<int64_t>(1, std::min<int64_t>(jobs && *jobs ? *jobs : base::parallel::workers(), static_cast<int64_t>(pending.size())));
        if (workers <= 1) {
            for (size_t i = 0; i < pending.size(); ++i) got[i] = one(pending[i]);
        } else {
            std::atomic<size_t> next{0};
            std::mutex m;
            std::exception_ptr first;
            size_t first_at = pending.size();
            std::vector<std::thread> pool;
            for (int64_t w = 0; w < workers; ++w) {
                pool.emplace_back([&]() {
                    for (;;) {
                        const size_t i = next.fetch_add(1);
                        if (i >= pending.size()) return;
                        try {
                            got[i] = one(pending[i]);
                        } catch (...) {
                            std::lock_guard<std::mutex> lock(m);
                            if (i < first_at) {
                                first_at = i;
                                first = std::current_exception();
                            }
                        }
                    }
                });
            }
            for (std::thread& t : pool) t.join();
            if (first) std::rethrow_exception(first);
        }
        for (size_t i = 0; i < pending.size(); ++i) results[pending[i]] = got[i];
        std::vector<int64_t> missing;
        if (!limited) {
            std::unordered_map<int64_t, bool> seen;
            for (const auto& kv : results) {
                if (!kv.second) continue;
                for (int64_t a : std::get<1>(*kv.second)) {
                    if (seen.count(a) || results.count(a) || skip.contains(a) || !(lo <= a && a < hi) || image->imports.contains(a)) continue;
                    seen[a] = true;
                    missing.push_back(a);
                }
            }
            std::sort(missing.begin(), missing.end());
        }
        if (!missing.empty()) {
            std::vector<int64_t> merged(bounds);
            for (const auto& kv : results) merged.push_back(kv.first);
            for (int64_t a : missing) merged.push_back(a);
            std::sort(merged.begin(), merged.end());
            merged.erase(std::unique(merged.begin(), merged.end()), merged.end());
            bounds = merged;
            for (int64_t a : missing) sizes[a] = size_of(a, next_start(a));
            chosen.insert(chosen.end(), missing.begin(), missing.end());
            std::sort(chosen.begin(), chosen.end());
            chosen.erase(std::unique(chosen.begin(), chosen.end()), chosen.end());
        }
        pending = missing;
        ++rounds;
    }
    py::set<int64_t> all_called;
    Agg agg;
    agg.discovered = funcs.size();
    agg.tail_entries = static_cast<int64_t>(chosen.size()) - static_cast<int64_t>(limited ? head(*limit).size() : addrs.size());
    agg.lo = chosen.empty() ? 0 : chosen.front();
    agg.hi = chosen.empty() ? 0 : chosen.back();
    std::vector<int64_t> order;
    for (const auto& kv : results) order.push_back(kv.first);
    std::sort(order.begin(), order.end());
    for (int64_t va : order) {
        const auto& r = results.at(va);
        if (!r) continue;
        agg.parts.append(std::get<0>(*r));
        for (int64_t a : std::get<1>(*r)) all_called.add(a);
        ++agg.funcs;
        const Stats& st = std::get<2>(*r);
        agg.total += st.total;
        agg.translated += st.translated;
        agg.unimpl += st.unimpl;
    }
    // every function is declared before any is defined (an earlier one calls a later one)
    std::vector<int64_t> decls(chosen);
    for (int64_t a : all_called) decls.push_back(a);
    std::sort(decls.begin(), decls.end());
    decls.erase(std::unique(decls.begin(), decls.end()), decls.end());
    std::string body = "#include \"rex_ppc_min.h\"\n\n";
    for (size_t i = 0; i < decls.size(); ++i) {
        if (i) body += '\n';
        body += "PPC_FUNC(sub_" + x8(static_cast<uint64_t>(decls[i])) + ");";
    }
    body += "\n\n";
    for (int64_t i = 0; i < agg.parts.size(); ++i) {
        if (i) body += '\n';
        body += agg.parts[i].utf8();
    }
    std::string mapping = "#include \"rex_ppc_min.h\"\n\nstruct PPCFuncMapping { uint32_t addr; PPCFunc* func; };\nPPCFuncMapping PPCFuncMappings[] = {";
    for (int64_t va : chosen) mapping += "\n\t{ 0x" + x8(static_cast<uint64_t>(va)) + ", sub_" + x8(static_cast<uint64_t>(va)) + " },";
    mapping += "\n\t{ 0, nullptr },\n};\n";
    for (int64_t a : decls) agg.decls.append(a);
    for (int64_t a : chosen) agg.chosen.append(a);
    for (int64_t a : chosen)
        if (skip.contains(a)) agg.skipped.append(a);
    agg.image = image;
    return {py::str(body), py::str(mapping), agg};
}

// ---------------------------------------------------------------------------------------------------------------------
// checking the output with a C++ compiler
// ---------------------------------------------------------------------------------------------------------------------

std::optional<py::str> compiler() {
    const std::optional<py::str> found = py::settings::get_opt_str(py::str("gpp"));
    if (found && !found->empty() && py::os::path::isfile(*found)) return found;
    return py::shutil::which(py::str("g++"));
}

py::list<py::str> kernel_includes() {
    const py::str root = py::os::path::join(ROOT, py::str("native"), py::str("rexglue"), py::str("kernel"));
    if (!py::os::path::isdir(root)) return py::list<py::str>();
    py::list<py::str> out;
    for (const char* d : {"shim", "include", "thirdparty/fmt/include", "thirdparty/simde", "thirdparty", "thirdparty/mspack", "thirdparty/o1heap",
                          "thirdparty/crypto"}) {
        py::str p = root;
        for (const py::str& part : py::split(py::str(d), py::str("/"))) p = py::os::path::join(p, part);
        out.append(p);
    }
    return out;
}

std::tuple<std::optional<bool>, py::str> check_compiles(const py::str& full, bool bridge) {
    const std::optional<py::str> gpp = compiler();
    if (!gpp) return {std::nullopt, py::str("no C++ compiler (settings.json: gpp, or g++ on PATH)")};
    const py::list<py::str> inc = bridge ? kernel_includes() : py::list<py::str>();
    py::tempfile::TemporaryDirectory td;
    {
        py::file f = py::open(py::os::path::join(td.name, py::str("rex_ppc_min.h")), py::str("w"));
        f.write(runtime_header());
        f.close();
    }
    const py::str cpp = py::os::path::join(td.name, py::str("f.cpp"));
    {
        py::file f = py::open(cpp, py::str("w"));
        f.write(full);
        f.close();
    }
    py::list<py::str> cmd;
    cmd.append(*gpp);
    cmd.append(py::str(inc.empty() ? "-std=c++20" : "-std=c++23"));
    cmd.append(py::str("-c"));
    cmd.append(py::str("-w"));
    cmd.append(py::str("-I"));
    cmd.append(td.name);
    for (const py::str& d : inc) {
        cmd.append(py::str("-I"));
        cmd.append(d);
    }
    if (!inc.empty()) cmd.append(py::str("-DFMT_HEADER_ONLY"));
    else cmd.append(py::str("-DREX_PPC_STANDALONE"));
    cmd.append(py::str("-o"));
    cmd.append(py::os::path::join(td.name, py::str("f.o")));
    cmd.append(cpp);
    const py::subprocess::CompletedProcess r = py::subprocess::run(cmd);
    const py::str err = r.stderr_ ? *r.stderr_ : py::str();
    return {r.returncode == 0, err.slice(0, std::min<int64_t>(3000, err.size()))};
}

// ---------------------------------------------------------------------------------------------------------------------
// the command line
// ---------------------------------------------------------------------------------------------------------------------

namespace {
const py::str MAIN_DOC = py::str(
    "python -m codebreaker recompile <image or .xex> [--all] [--at ADDR --size N] [-o FILE]\n"
    "                                   [--limit N] [--check] [--runtime HEADER]\n"
    "\n"
    "--all without -o writes into the binary's project folder: Converted/<name>/src/<name>.cpp.");
}

int64_t main(const py::list<py::str>& argv) {
    // opt(name, conv, default): the value after `name` in argv
    auto find = [&](const char* name) -> std::optional<py::str> {
        if (!argv.contains(py::str(name))) return std::nullopt;
        return argv[argv.index(py::str(name)) + 1];
    };
    if (argv.contains(py::str("--runtime"))) {
        const py::str target = *find("--runtime");
        py::file f = py::open(target, py::str("w"));
        f.write(runtime_header());
        f.close();
        py::print(py::str("wrote"), target);
        return 0;
    }
    py::list<py::str> args;
    for (int64_t i = 0; i < argv.size(); ++i) {
        const py::str& a = argv[i];
        if (!a.startswith(py::str("-"))) {
            bool value_of_flag = false;
            if (i != 0) {
                const py::str& prev = argv[i - 1];
                for (const char* f : {"--at", "--size", "-o", "--limit", "-j"})
                    if (prev == py::str(f)) value_of_flag = true;
            }
            if (i == 0 || !value_of_flag) args.append(a);
        }
    }
    if (args.empty()) {
        py::print(MAIN_DOC);
        return 2;
    }
    const py::str path = args[0];
    std::optional<py::str> out = find("-o");
    py::str full;
    if (argv.contains(py::str("--all"))) {
        std::optional<int64_t> limit, jobs;
        if (const auto v = find("--limit")) limit = py::int_parse(*v);
        if (const auto v = find("-j")) jobs = py::int_parse(*v);
        const auto [body, mapping, agg] = recompile_image(path, limit, jobs);
        const double pct = 100.0 * static_cast<double>(agg.translated) / static_cast<double>(std::max<int64_t>(1, agg.total));
        py::print(py::percent(py::str("%d functions (%d found, %d tail-jump entries), %d/%d instructions translated (%.1f%%), %d not"), agg.funcs,
                              agg.discovered, agg.tail_entries, agg.translated, agg.total, pct, agg.unimpl));
        std::optional<base::converted::Project> proj;
        if (!out || out->empty()) {
            proj = base::converted::project(path);
            out = py::os::path::join(proj->src, proj->name().lower() + py::str(".cpp"));
        }
        {
            py::file f = py::open(*out, py::str("w"));
            f.write(body);
            f.close();
        }
        const py::str init_path = out->replace(py::str(".cpp"), py::str("_init.cpp"));
        {
            py::file f = py::open(init_path, py::str("w"));
            f.write(mapping);
            f.close();
        }
        if (proj) {
            py::list<py::str> outputs;
            outputs.append(*out);
            outputs.append(init_path);
            proj->record(py::str("recompile"), outputs, py::str("every function as C++ (one statement per guest instruction) and its mapping table"));
        }
        py::print(py::str("wrote"), *out, py::str("and its mapping table"));
        full = body;
    } else {
        std::optional<int64_t> at;
        if (const auto v = find("--at")) at = py::int_parse(*v, 0);
        if (!at) {
            py::print(MAIN_DOC);
            return 2;
        }
        int64_t size = 0x100;
        if (const auto v = find("--size")) size = py::int_parse(*v, 0);
        const auto [body, sec_base, entry] = disassemble::code_section(path);
        (void)entry;
        const int64_t off = *at - sec_base;
        const auto [cpp, called, stats] = recompile_function(body.slice(off, off + size), sec_base, *at, size);
        full = py::str("#include \"rex_ppc_min.h\"\n\n") + _emit_forward_decls(called) + py::str("\n\n") + cpp;
        if (out && !out->empty()) {
            py::file f = py::open(*out, py::str("w"));
            f.write(full);
            f.close();
        }
        py::print(py::percent(py::str("sub_%08X: %d/%d instructions translated, %d not, %d labels, %d calls"), *at, stats.translated, stats.total,
                              stats.unimpl, stats.labels, called.size()));
    }
    if (argv.contains(py::str("--check"))) {
        const auto [ok, why] = check_compiles(full);
        if (ok && *ok) py::print(py::str("compiles"));
        else if (ok) py::print(py::str("does not compile:\n") + why);
        else py::print(why);
        return (ok && *ok) ? 0 : 1;
    }
    return 0;
}

}  // namespace codebreaker::recompile::powerpc
