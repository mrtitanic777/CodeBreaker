// codebreaker/decompile/describe.cpp -- typed C++ of codebreaker.decompile.describe (see describe.hpp).

#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/emulate/capstone_x86.hpp"
#include "codebreaker/platforms/package.hpp"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <algorithm>
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <deque>
#include <filesystem>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include <capstone/capstone.h>

#include "codebreaker/analyze/conventions.hpp"
#include "codebreaker/analyze/image.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/data.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/formats/nes.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "codebreaker/symbols/driver_services.hpp"
#include "codebreaker/symbols/names.hpp"
#include "codebreaker/symbols/ordinals.hpp"
#include "py_stdlib.hpp"

// verify/lanes/win32.hpp is declared here by the one function this file calls (LANES["pe"].functions(path)): the header
// itself reaches the whole decompiler.
namespace codebreaker::verify::lanes::win32 {
py::list<int64_t> functions(const py::str& lane, const py::str& path);
}

namespace codebreaker::decompile::describe {

using std::string;

namespace {

// ---- small helpers ---------------------------------------------------------------------------------------------
string fmt(const char* f, ...) {
    va_list ap;
    va_start(ap, f);
    char buf[2048];
    int n = vsnprintf(buf, sizeof buf, f, ap);
    va_end(ap);
    if (n < 0) return string();
    if (n < static_cast<int>(sizeof buf)) return string(buf, static_cast<size_t>(n));
    string big(static_cast<size_t>(n) + 1, '\0');
    va_start(ap, f);
    vsnprintf(&big[0], big.size(), f, ap);
    va_end(ap);
    big.resize(static_cast<size_t>(n));
    return big;
}
// Python's "%x" (a negative number is shown with a minus sign)
string hx(int64_t v) {
    char b[32];
    if (v < 0) snprintf(b, sizeof b, "-%llx", static_cast<unsigned long long>(-v));
    else snprintf(b, sizeof b, "%llx", static_cast<unsigned long long>(v));
    return b;
}
string HX(int64_t v, int width) {   // "%0<width>X"
    char b[40];
    snprintf(b, sizeof b, "%0*llX", width, static_cast<unsigned long long>(v));
    return b;
}
inline bool starts(const string& s, const char* p) { size_t n = std::strlen(p); return s.size() >= n && s.compare(0, n, p) == 0; }
inline bool contains(const string& s, const char* p) { return s.find(p) != string::npos; }
inline bool eq_any(const string& s, std::initializer_list<const char*> xs) {
    for (const char* x : xs) if (s == x) return true;
    return false;
}
// r"0x[0-9a-f]+" fullmatch
bool is_hex_lit(const string& s) {
    if (s.size() < 3 || s[0] != '0' || s[1] != 'x') return false;
    for (size_t i = 2; i < s.size(); i++) {
        char c = s[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) return false;
    }
    return true;
}
bool is_dec_lit(const string& s) {   // r"\d+" (ASCII digits: capstone's text)
    if (s.empty()) return false;
    for (char c : s) if (c < '0' || c > '9') return false;
    return true;
}
// r"(0x[0-9a-f]+|\d+)"
inline bool is_num_lit(const string& s) { return is_hex_lit(s) || is_dec_lit(s); }
// r"-?(0x[0-9a-f]+|\d+)"
inline bool is_signed_num_lit(const string& s) { return is_num_lit(!s.empty() && s[0] == '-' ? s.substr(1) : s); }
// int(s, base), or nullopt for a ValueError
std::optional<int64_t> try_int(const string& s, int64_t base) {
    try {
        return py::int_parse(py::str(s), base);
    } catch (const py::ValueError&) {
        return std::nullopt;
    }
}
int64_t int_of(const string& s, int64_t base) { return py::int_parse(py::str(s), base); }
// s.split(",")[0]
string before_comma(const string& s) {
    size_t k = s.find(',');
    return k == string::npos ? s : s.substr(0, k);
}
// [x.strip() for x in s.split(",")]
std::vector<string> split_strip_commas(const string& s) {
    std::vector<string> out;
    size_t at = 0;
    while (true) {
        size_t k = s.find(',', at);
        string part = s.substr(at, k == string::npos ? string::npos : k - at);
        size_t a = 0, b = part.size();
        while (a < b && (part[a] == ' ' || part[a] == '\t' || part[a] == '\n' || part[a] == '\r')) a++;
        while (b > a && (part[b - 1] == ' ' || part[b - 1] == '\t' || part[b - 1] == '\n' || part[b - 1] == '\r')) b--;
        out.push_back(part.substr(a, b - a));
        if (k == string::npos) break;
        at = k + 1;
    }
    return out;
}
// s.split(sep)
std::vector<string> split_on(const string& s, const string& sep) {
    std::vector<string> out;
    size_t at = 0;
    while (true) {
        size_t k = s.find(sep, at);
        out.push_back(s.substr(at, k == string::npos ? string::npos : k - at));
        if (k == string::npos) break;
        at = k + sep.size();
    }
    return out;
}
// re.sub(r"\W", "_", s)
py::str ident_py(const py::str& s) { return py::re::sub(py::str("\\W"), py::str("_"), s); }
string ident(const string& s) { return ident_py(py::str(s)).utf8(); }
// re.fullmatch(r"[A-Za-z_]\w*", s)
bool c_name(const py::str& s) { return py::re::fullmatch(py::str("[A-Za-z_]\\w*"), s).has_value(); }

std::optional<string> getenv_s(const char* name) {
    auto v = py::os::getenv(py::str(name));
    if (!v) return std::nullopt;
    return v->utf8();
}
bool env_is(const char* name, const char* value) {
    auto v = getenv_s(name);
    return v && *v == value;
}
bool env_not(const char* name, const char* value) { return !env_is(name, value); }

// os.path.splitext(p)[0]
string splitext_root(const string& p) {
    size_t sep = p.find_last_of("/\\");
    size_t dot = p.rfind('.');
    if (dot != string::npos && (sep == string::npos || dot > sep)) {
        size_t fi = (sep == string::npos ? 0 : sep + 1);
        while (fi < dot) {
            if (p[fi] != '.') return p.substr(0, dot);
            fi++;
        }
    }
    return p;
}

// ---- the capstone decoder: platforms.decoder(16, detail=...) ----------------------------------------------------------
std::shared_ptr<platforms::Decoder> md_(bool detail) { return platforms::decoder(16, py::str("x86"), detail); }

// i.reg_name(id)
string reg_name_s(int64_t id) {
    const char* s = md_(true)->reg_name(static_cast<uint32_t>(id));
    return s ? string(s) : string();
}

Insn make_insn(const platforms::Decoder& md, const cs_insn& c, bool detail) {
    Insn out;
    out.address = c.address;
    out.size = c.size;
    out.mn = c.mnemonic;
    out.os = c.op_str;
    out.mnemonic = py::str(out.mn);
    out.op_str = py::str(out.os);
    out.bytes = py::bytes(reinterpret_cast<const char*>(c.bytes), c.size);
    if (!detail || !c.detail) return out;
    const cs_x86& x = c.detail->x86;
    for (int k = 0; k < 4; ++k) out.prefix.append(x.prefix[k]);
    for (int k = 0; k < x.op_count; ++k) {
        const cs_x86_op& o = x.operands[k];
        inline_asm::Operand op;
        op.type = o.type;
        op.reg = static_cast<int64_t>(static_cast<uint32_t>(o.reg));
        op.imm = o.imm;
        op.mem.segment = static_cast<int64_t>(static_cast<uint32_t>(o.mem.segment));
        op.mem.base = static_cast<int64_t>(static_cast<uint32_t>(o.mem.base));
        op.mem.index = static_cast<int64_t>(static_cast<uint32_t>(o.mem.index));
        op.mem.scale = o.mem.scale;
        op.mem.disp = o.mem.disp;
        out.operands.append(op);
        out.op_size.append(o.size);
        out.op_access.append(o.access);
    }
    for (int k = 0; k < c.detail->groups_count; ++k) out.groups.append(c.detail->groups[k]);
    if (auto access = md.regs_access(c)) {
        out.regs_ok = true;
        for (uint16_t r : std::get<0>(*access)) out.regs_read.append(r);
        for (uint16_t r : std::get<1>(*access)) out.regs_write.append(r);
    }
    return out;
}

// md.disasm(code, address) (count 0: to the end), as a vector; `count` > 0: only that many
std::vector<Insn> disasm_v(bool detail, const string& code, int64_t address, size_t count = 0) {
    std::shared_ptr<platforms::Decoder> md = md_(detail);
    std::vector<Insn> out;
    md->each(code, address, count, [&](const cs_insn& c) {
        out.push_back(make_insn(*md, c, detail));
        return true;
    });
    return out;
}
std::vector<Insn> disasm_bytes(bool detail, const py::bytes& code, int64_t address, size_t count = 0) {
    return disasm_v(detail, code.raw(), address, count);
}
// code[a:b] as text (Python's clamping of a slice)
string slice_s(const py::bytes& code, int64_t a, int64_t b) {
    int64_t n = code.size();
    if (a < 0) a = std::max<int64_t>(0, a + n);
    if (b < 0) b = std::max<int64_t>(0, b + n);
    a = std::min(a, n);
    b = std::min(b, n);
    if (b <= a) return string();
    return code.raw().substr(static_cast<size_t>(a), static_cast<size_t>(b - a));
}

class PlainDecoder : public Decoder {
  public:
    explicit PlainDecoder(bool detail) : detail_(detail) {}
    py::list<Insn> disasm(const py::bytes& code, int64_t address) const override {
        return py::list<Insn>(disasm_bytes(detail_, code, address));
    }
    py::str reg_name(int64_t reg) const override { return py::str(reg_name_s(reg)); }

  private:
    bool detail_;
};

// ---- the instructions of a code segment, each decoded once -------------------------------------------------------
// The instruction at an offset of a code segment, decoded once. Finding functions, their returns and the stubs in front
// of them walks the same bytes again and again, one instruction at a time. Kept per segment (the Python holds the bytes
// object itself so its id is not reused; here the segment's own copy is held and compared).
class Dec {
  public:
    explicit Dec(const py::bytes* code) : code_(code) {}
    const py::bytes& code() const { return *code_; }
    // next(md.disasm(bytes(code[at:at + 16]), at), None)
    std::shared_ptr<const Insn> one(int64_t at) {
        std::lock_guard<std::mutex> hold(m_);
        auto it = seen_.find(at);
        if (it != seen_.end()) return it->second;
        std::vector<Insn> got = disasm_v(false, slice_s(*code_, at, at + 16), at, 1);
        std::shared_ptr<const Insn> ins = got.empty() ? nullptr : std::make_shared<const Insn>(std::move(got[0]));
        seen_[at] = ins;
        return ins;
    }

  private:
    const py::bytes* code_;
    std::unordered_map<int64_t, std::shared_ptr<const Insn>> seen_;
    std::mutex m_;
};
struct Held {
    py::bytes copy;
    std::unique_ptr<Dec> dec;
};
std::mutex& held_mutex() {
    static std::mutex m;
    return m;
}
std::map<std::pair<const void*, size_t>, std::unique_ptr<Held>>& held_map() {
    static std::map<std::pair<const void*, size_t>, std::unique_ptr<Held>> m;
    return m;
}
// the kept decodings of this segment's bytes (made when there are none)
Dec& held(const py::bytes& code) {
    std::lock_guard<std::mutex> hold(held_mutex());
    auto key = std::make_pair(static_cast<const void*>(code.raw().data()), code.raw().size());
    auto& slot = held_map()[key];
    if (slot && slot->copy.raw() == code.raw()) return *slot->dec;
    slot.reset(new Held());
    slot->copy = code;
    slot->dec.reset(new Dec(&slot->copy));
    return *slot->dec;
}
void held_clear() {
    std::lock_guard<std::mutex> hold(held_mutex());
    held_map().clear();
}

// the register sets the Python tests against (capstone's numbers)
bool hits(const py::list<int64_t>& regs, std::initializer_list<int> set) {
    for (int64_t r : regs)
        for (int s : set)
            if (r == s) return true;
    return false;
}
bool hits_ax(const py::list<int64_t>& regs) { return hits(regs, {X86_REG_AX, X86_REG_AL, X86_REG_AH, X86_REG_EAX}); }
bool hits_dx(const py::list<int64_t>& regs) { return hits(regs, {X86_REG_DX, X86_REG_DL, X86_REG_DH, X86_REG_EDX}); }

// The first operand of an instruction is an immediate: `i.operands and i.operands[0].type == CS_OP_IMM`
inline bool imm_first(const Insn& i) { return i.operands.size() > 0 && i.operands.vec()[0].type == X86_OP_IMM; }
inline int64_t first_imm(const Insn& i) { return i.operands.vec()[0].imm; }
inline int64_t first_size(const Insn& i) { return i.op_size.size() > 0 ? i.op_size.vec()[0] : 0; }

}  // namespace

void set_capstone_dll(const py::str& path) { emulate::capstone_x86::load_capstone(path); }

std::shared_ptr<const Decoder> decoder16(bool detail) {
    static std::shared_ptr<const Decoder> plain = std::make_shared<PlainDecoder>(false);
    static std::shared_ptr<const Decoder> rich = std::make_shared<PlainDecoder>(true);
    md_(detail);   // (ImportError here, not at the first instruction, when there is no capstone)
    return detail ? rich : plain;
}

std::shared_ptr<const Insn> _one(const py::bytes& code, int64_t at) { return held(code).one(at); }

// ---- the 16-bit address space ---------------------------------------------------------------------------------
const py::str ARCH = py::str("x86:LE:16:Protected Mode:default");        // platforms.named("ne").cpu
const py::str LE_ARCH = py::str("x86:LE:32:default:windows");            // platforms.named("le").cpu
const py::str DOS_ARCH = py::str("x86:LE:16:Real Mode:default");         // platforms.named("dos").cpu

// The decompiler's language for a PE image of this machine type (its platform's), or None.
std::optional<py::str> _arch(int64_t machine) {
    if (machine == 0x014C) return py::str("x86:LE:32:default:windows");     // platforms.for_machine: pe
    if (machine == 0x01F2) return py::str("PowerPC:BE:64:Xenon:homed");    // xbox360
    return std::nullopt;
}

py::dict<py::str, py::dict<py::str, std::tuple<py::str, int64_t, std::optional<int64_t>>>> BY_NUMBER;
py::dict<py::str, py::set<py::str>> IN_ECX;

int64_t selector(int64_t seg) { return FIRST_SELECTOR + 8 * (seg - 1); }
int64_t address(int64_t seg, int64_t off) { return (selector(seg) << 16) + off; }
namespace {
// Python's floor division and modulo of ints
int64_t fdiv(int64_t a, int64_t b) {
    int64_t q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) q--;
    return q;
}
int64_t fmod_(int64_t a, int64_t b) { return a - fdiv(a, b) * b; }
}  // namespace
SegOff where(int64_t va) { return SegOff{fdiv((va >> 16) - FIRST_SELECTOR, 8) + 1, va & 0xFFFF}; }
int64_t le_address(int64_t obj, int64_t off) { return LE_BASE + (obj - 1) * LE_STRIDE + off; }

bool inside(const Description& d, int64_t va) {
    SegOff w = where(va);
    int64_t seg = std::get<0>(w), off = std::get<1>(w);
    return fmod_((va >> 16) - FIRST_SELECTOR, 8) == 0 && off < d.code.get(seg, 0);
}

// ---- Description ------------------------------------------------------------------------------------------------
Description::Description(const py::str& path_, const py::dict<int64_t, py::str>& names_, const py::dict<int64_t, SegOff>& where_,
                         const py::list<py::str>& imports_)
    : path(path_), names(names_), where(where_), imports(imports_) {}

py::str Description::write(const std::optional<py::str>& path_) {
    if (path_ && !path_->empty()) path = *path_;
    py::file f = py::open(path, py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));
    string text;
    for (const py::str& l : lines) {
        text += l.utf8();
        text += "\n";
    }
    if (lines.empty()) text = "\n";
    f.write(py::str(text));
    f.close();
    return path;
}

py::list<py::list<py::str>> Description::records(const py::list<py::str>& kinds) const {
    py::list<py::list<py::str>> out;
    for (const py::str& ln : lines) {
        py::list<py::str> parts;
        string s = ln.utf8();
        for (const string& p : split_on(s, "\t")) parts.append(py::str(p));
        if (kinds.contains(parts[0])) out.append(parts);
    }
    return out;
}

// ---- NE: finding functions and how they return ----------------------------------------------------------------------
namespace {

// Offsets inside one code segment that two or more `call rel16` sites agree on. A stray 0xE8 in data does not
// coincide with another, so an agreed target is a real function start.
std::vector<int64_t> near_call_targets(const py::bytes& code) {
    std::map<int64_t, std::set<int64_t>> sites;
    int64_t n = code.size();
    for (int64_t i = 0; i < n - 2; i++) {
        if (code.byte_(i) == 0xE8) {
            int16_t rel = static_cast<int16_t>(code.byte_(i + 1) | (code.byte_(i + 2) << 8));
            int64_t tgt = (i + 3 + rel) & 0xFFFF;
            if (tgt < n) sites[tgt].insert(i);
        }
    }
    std::vector<int64_t> out;
    for (auto& kv : sites)
        if (kv.second.size() >= 2) out.push_back(kv.first);
    return out;
}

// The entry sequences of Win16 functions (the compilers' and the hand-written ones' alike)
const char* const kPrologues[] = {"\x55\x8b\xec",       // push bp; mov bp, sp
                                  "\x45\x55\x8b\xec",   // inc bp; push bp; mov bp, sp   (a far frame)
                                  "\x1e\x58\x90",       // push ds; pop ax; nop          (exported, as in the file)
                                  "\x8c\xd8\x90"};      // mov ax, ds; nop
const size_t kPrologueLen[] = {3, 4, 3, 3};

bool starts_at(const py::bytes& code, int64_t o, const char* p, size_t n) {
    if (o < 0 || static_cast<size_t>(o) + n > static_cast<size_t>(code.size())) return false;
    return std::memcmp(code.raw().data() + o, p, n) == 0;
}

bool is_prologue(const py::bytes& code, int64_t o) {
    for (int k = 0; k < 4; k++)
        if (starts_at(code, o, kPrologues[k], kPrologueLen[k])) return true;
    int64_t n = code.size();
    if (o >= 0 && o < n && code.byte_(o) == 0xC8 && o + 4 <= n && (code.byte_(o + 3) == 0 || code.byte_(o + 3) == 1)) return true;   // enter n, 0|1
    return o >= 0 && o < n && code.byte_(o) == 0xB8 && starts_at(code, o + 3, "\x45\x55\x8b\xec", 4);   // mov ax, DGROUP; inc bp ...
}

bool is_end(const string& m) { return m == "ret" || m == "retf" || m == "iret" || m == "jmp" || m == "ljmp"; }

std::vector<int64_t> prologues_after_ends_i(Dec& dec) {
    const py::bytes& code = dec.code();
    std::vector<int64_t> out;
    int64_t at = 0, n = code.size();
    while (at < n) {
        auto ins = dec.one(at);
        if (!ins) {
            at += 1;
            continue;
        }
        at += ins->size;
        if (is_end(ins->mn)) {
            int64_t p = at;
            while (p < n && (code.byte_(p) == 0x00 || code.byte_(p) == 0x90 || code.byte_(p) == 0xCC)) p++;
            if (p < n && is_prologue(code, p)) out.push_back(p);
        }
    }
    return out;
}

// Near call targets in the code reached from `start` (fall-through and jumps followed).
std::set<int64_t> calls_followed_i(Dec& dec, int64_t start, int64_t limit = 20000) {
    const py::bytes& code = dec.code();
    std::set<int64_t> calls, seen;
    std::vector<int64_t> work{start};
    int64_t n = code.size();
    while (!work.empty() && static_cast<int64_t>(seen.size()) < limit) {
        int64_t at = work.back();
        work.pop_back();
        while (0 <= at && at < n && !seen.count(at)) {
            auto ins = dec.one(at);
            if (!ins) break;
            seen.insert(at);
            int64_t b0 = ins->bytes.byte_(0);
            if (b0 == 0xE8 && ins->size == 3) {
                int16_t rel = static_cast<int16_t>(ins->bytes.byte_(1) | (ins->bytes.byte_(2) << 8));
                int64_t t = (at + 3 + rel) & 0xFFFF;
                if (t < n) calls.insert(t);
            } else if (b0 == 0xEB || b0 == 0xE9 || (0x70 <= b0 && b0 <= 0x7F) || (0xE0 <= b0 && b0 <= 0xE3) ||
                       (b0 == 0x0F && 0x80 <= ins->bytes.byte_(1) && ins->bytes.byte_(1) <= 0x8F)) {
                auto v = try_int(ins->os, 16);
                if (v) {
                    int64_t t = *v & 0xFFFF;
                    if (t < n) work.push_back(t);
                }
            }
            if (is_end(ins->mn)) break;
            at += ins->size;
        }
    }
    return calls;
}

// How the function at `off` returns: ("far" | "near" | None, bytes of arguments removed).
//
// Read from the function's own first return instruction: `retf` is a far function, `ret` a near one, and the operand is
// what it removes. That is the function's own statement of how it must be called; where it was found (entry table, a
// relocation, a near call) only says how SOMEONE calls it. None when no return is met within `limit` instructions.
std::tuple<std::optional<string>, int64_t> returns_i(Dec& dec, int64_t off, int64_t limit, const std::optional<string>& want) {
    const py::bytes& code = dec.code();
    int64_t len = code.size();
    // The code is followed, not read straight down: functions share epilogues (Windows exports beside each other jump
    // into one `retf 8` that may lie BEFORE them), and straight down from a jump is often someone else's code and
    // someone else's return. The nearest return by the paths the function takes is its own.
    // (one instruction at a time, every path abreast: the return fewest instructions away)
    std::deque<int64_t> work{off};
    std::set<int64_t> seen;
    int64_t n = 0;
    while (!work.empty() && n < limit) {
        int64_t at = work.front();
        work.pop_front();
        if (!(0 <= at && at < len) || seen.count(at)) continue;
        seen.insert(at);
        auto insp = dec.one(at);
        if (!insp) continue;
        const Insn& ins = *insp;
        n++;
        if (ins.mn == "ret" || ins.mn == "retf" || ins.mn == "lret") {
            string kind = ins.mn == "ret" ? "near" : "far";
            int64_t pop = ins.os.empty() ? 0 : int_of(ins.os, 0);
            if ((pop & 1) || pop > 0x7E) continue;   // (no function removes an odd count or so many: bytes that are not code, decoded)
            if (!want || kind == *want) return {kind, pop};
            continue;
        }
        std::optional<int64_t> target;
        if (ins.os.size() > 2 && ins.os.compare(0, 2, "0x") == 0) {
            bool alnum = true;
            for (size_t k = 2; k < ins.os.size(); k++) {
                unsigned char c = static_cast<unsigned char>(ins.os[k]);
                if (!std::isalnum(c)) alnum = false;
            }
            if (alnum) target = int_of(ins.os, 0);
        }
        if (ins.mn == "push" && target && off < *target && *target < len) {
            // a return address it pushes -- the stubs push the place of a `retf N` (KRNL386's LoadResource: push 13Dh ...
            // jmp worker; at 13Dh, retf 4) -- only when a return is what is there: a constant pushed as an argument is not
            // one (USER sub_24_00F6's `push 98Bh` was followed into the function after it, to a `ret 0EEBh` in its bytes,
            // and the dialog builder was called with two words fewer than it takes)
            // (or a few straight instructions before it: GetMessage's is `mov ax, -1` / `retf 0Ah`; or a shared tail with
            // a frame of its own: GetWindowText pushes AB36h, `push bp / mov bp, sp ...`, which ends in its `retf 8`)
            if (is_prologue(code, *target)) work.push_back(*target);
            std::vector<Insn> there = disasm_v(false, slice_s(code, *target, *target + 16), *target, 4);
            int64_t k = 0;
            for (const Insn& t : there) {
                if (t.mn == "ret" || t.mn == "retf") {
                    work.push_back(*target);
                    break;
                }
                if (k >= 2 || starts(t.mn, "j") || t.mn == "call" || t.mn == "lcall") break;
                k++;
            }
        }
        if (ins.mn == "jmp") {
            if (target) work.push_back(*target);   // (through a table or register: not followed)
            continue;
        }
        if (ins.mn == "ljmp" || ins.mn == "iret") continue;   // (a far jump leaves: what is after it is someone else's)
        if ((starts(ins.mn, "j") || eq_any(ins.mn, {"loop", "loope", "loopne", "jcxz"})) && target) work.push_back(*target);
        work.push_back(static_cast<int64_t>(ins.address) + ins.size);
    }
    return {std::nullopt, -1};
}

// Places in the module's code segments a far pointer names whose offset is a plain number beside a selector fix-up:
// `call far` / `jmp far` (9A/EA, the offset inline), a pointer pushed (`push seg X` / `push n`), or stored as two words
// side by side (`mov [bp+d], n` / `mov [bp+d+2], seg X`, either order). Such a place is a function -- often one nothing
// calls by a near call and the module does not export: a callback (USER's 1:3452, passed to an enumeration from 1:36E8).
// Not seen, it was taken for the tail of the function before it.
std::set<SegOff> far_pointer_targets(const formats::ne::NEFile& nef, const formats::ne::Segment& s, const py::bytes& code,
                                     const std::map<int64_t, formats::ne::Segment>& segs) {
    std::set<SegOff> out;
    int64_t n = code.size();
    auto u16 = [&](int64_t at) { return code.byte_(at) | (code.byte_(at + 1) << 8); };
    for (const auto& place : build::data::_places(nef, s, code)) {
        int64_t o = std::get<0>(place), src = std::get<1>(place);
        const formats::ne::Relocation& r = std::get<3>(place);
        if (src != 2 || r.kind != "INTERNAL" || !r.seg || *r.seg == 0xFF) continue;
        int64_t t = *r.seg;
        auto sit = segs.find(t);
        if (sit == segs.end() || sit->second.kind != "CODE") continue;
        std::vector<int64_t> offs;
        if (o >= 3 && (code[o - 3] == 0x9A || code[o - 3] == 0xEA)) offs.push_back(u16(o - 2));   // call/jmp far off:seg
        if (o >= 1 && code[o - 1] == 0x68 && o + 5 <= n && code[o + 2] == 0x68) offs.push_back(u16(o + 3));   // push seg / push off
        if (o >= 3 && code[o - 3] == 0xC7 && code[o - 2] == 0x46) {   // mov [bp+d], seg ...
            int64_t d = code[o - 1];
            if (o >= 8 && code[o - 8] == 0xC7 && code[o - 7] == 0x46 && code[o - 6] == ((d - 2) & 0xFF))
                offs.push_back(u16(o - 5));   // ... after mov [bp+d-2], off
            if (o + 7 <= n && code[o + 2] == 0xC7 && code[o + 3] == 0x46 && code[o + 4] == ((d - 2) & 0xFF))
                offs.push_back(u16(o + 5));   // ... before it
        }
        for (int64_t v : offs)
            if (0 <= v && v < sit->second.length) out.insert({t, v});
    }
    return out;
}

}  // namespace

py::list<int64_t> prologues_after_ends(const py::bytes& code) {
    py::list<int64_t> out;
    for (int64_t p : prologues_after_ends_i(held(code))) out.append(p);
    return out;
}

std::tuple<std::optional<py::str>, int64_t> returns(const py::bytes& code, int64_t off, int64_t limit,
                                                    const std::optional<py::str>& want) {
    std::optional<string> w;
    if (want) w = want->utf8();
    auto [kind, pop] = returns_i(held(code), off, limit, w);
    std::optional<py::str> k;
    if (kind) k = py::str(*kind);
    return {k, pop};
}

// {(segment, offset): "far" | "near"} -- every function start that can be established.
py::dict<SegOff, py::str> functions(const formats::ne::NEFile& nef) {
    py::list<formats::ne::Segment> seglist = nef.segments();
    std::map<int64_t, formats::ne::Segment> segs;
    for (const auto& s : seglist) segs[s.idx] = s;
    py::dict<SegOff, py::str> out;
    for (const auto& e : nef.entries()) {
        if ((e.kind == "FIXED" || e.kind == "MOVEABLE") && e.seg) {
            auto it = segs.find(*e.seg);
            if (it != segs.end() && it->second.kind == "CODE") out.set(SegOff{*e.seg, e.offset.value_or(0)}, py::str("far"));
        }
    }
    // the module's entry point (a library's LibEntry: what Windows runs when it loads it -- it makes the local heap and
    // sets the module up) is a function, called far by the loader
    int64_t cs = nef.h.csip >> 16, ip = nef.h.csip & 0xFFFF;
    if (cs) {
        auto it = segs.find(cs);
        if (it != segs.end() && it->second.kind == "CODE") out.set(SegOff{cs, ip}, py::str("far"));
    }
    std::set<int64_t> used;   // (segments some export starts in: below)
    for (const auto& k : out.keys()) used.insert(std::get<0>(k));
    for (const auto& s : seglist) {
        if (s.kind != "CODE") continue;
        for (const auto& r : nef.relocations(s)) {
            if (r.kind == "INTERNAL" && r.seg && *r.seg != 0xFF) {
                auto it = segs.find(*r.seg);
                if (it != segs.end() && it->second.kind == "CODE" && r.src == 3) out.setdefault(SegOff{*r.seg, r.entry.value_or(0)}, py::str("far"));
            }
        }
        py::bytes code = nef.data.slice(s.file_off, s.file_off + s.length);
        for (int64_t off : near_call_targets(code)) out.setdefault(SegOff{s.idx, off}, py::str("near"));
        for (const SegOff& place : far_pointer_targets(nef, s, code, segs)) out.setdefault(place, py::str("far"));
        // a function nothing visibly reaches (through a table, a pointer computed or kept in data): its entry sequence
        // right after another function's end. Taken for that one's tail, it was erased with it when that one was rebuilt
        // from C (USER had 95)
        Dec& dec = held(code);
        for (int64_t off : prologues_after_ends_i(dec)) {
            auto how = std::get<0>(returns_i(dec, off, 600, std::nullopt));
            out.setdefault(SegOff{s.idx, off}, py::str(how ? *how : string("far")));
        }
    }
    // a code segment nothing above names a function in (reached by its number from outside, or dead) still is one of the
    // module's segments: what it holds starts at 0 -- far or near as how it is reached says (a far fix-up, a near call:
    // above), else as its own return says.
    // (SUWIN's 7:0000 is a near helper -- `ret`, called `call 0` -- and taken for far, its C caller called it far and lost
    // a word to the stack on every call: ten in a loop, then a `retf` into nothing, Illegal Instruction)
    for (const auto& s : seglist) {
        if (s.kind == "CODE" && s.length && !used.count(s.idx) && !out.contains(SegOff{s.idx, 0})) {
            py::bytes code = s.file_off ? nef.data.slice(s.file_off, s.file_off + s.length) : py::bytes();
            auto how = std::get<0>(returns_i(held(code), 0, 600, std::nullopt));
            out.set(SegOff{s.idx, 0}, py::str(how ? *how : string("far")));
        }
    }
    // what the code calls, followed from every start known -- one call is enough when it is in code that is followed (a
    // stray E8 in data is not)
    std::map<int64_t, py::bytes> codes;
    for (const auto& s : seglist)
        if (s.kind == "CODE" && s.file_off) codes[s.idx] = nef.data.slice(s.file_off, s.file_off + s.length);
    std::set<SegOff> done;
    while (true) {
        std::vector<SegOff> fresh;
        for (const auto& k : out.keys())
            if (!done.count(k) && codes.count(std::get<0>(k))) fresh.push_back(k);
        if (fresh.empty()) break;
        for (const SegOff& k : fresh) {
            done.insert(k);
            int64_t sg = std::get<0>(k), o = std::get<1>(k);
            for (int64_t t : calls_followed_i(held(codes[sg]), o)) out.setdefault(SegOff{sg, t}, py::str("near"));
        }
    }
    return out;
}

// The guard: entry sequences after an end that are not among `known` {(segment, offset)} -- functions the description
// does not have, so that the build would take them for the tail of the function before.
py::list<SegOff> hidden_functions(const formats::ne::NEFile& nef, const py::set<SegOff>& known) {
    py::list<SegOff> out;
    for (const auto& s : nef.segments()) {
        if (s.kind != "CODE" || !s.file_off) continue;
        py::bytes code = nef.data.slice(s.file_off, s.file_off + s.length);
        for (int64_t o : prologues_after_ends_i(held(code)))
            if (!known.contains(SegOff{s.idx, o})) out.append(SegOff{s.idx, o});
    }
    return out;
}

// ---- how a function returns: jumps by return, forwards, arguments ----------------------------------------------------
namespace {

// Bytes a push puts on the 16-bit stack (2, or 4 for a 32-bit one), 0 for anything else.
int64_t push_bytes(const Insn& i) {
    if (i.mn != "push") return 0;
    return (starts(i.os, "e") || starts(i.os, "dword")) ? 4 : 2;
}
// Bytes a plain `ret` / `retf` takes off for the address it goes to (none when it also removes arguments: `retf 6` is a
// function's own return).
int64_t return_pops(const Insn& i) {
    if ((i.mn != "ret" && i.mn != "retf") || !i.os.empty()) return 0;
    return i.mn == "ret" ? 2 : 4;
}

bool jump_by_return_i(const std::vector<Insn>& ins, int64_t k, const std::set<int64_t>& targets) {
    int64_t want = return_pops(ins[static_cast<size_t>(k)]);
    if (!want || targets.count(static_cast<int64_t>(ins[static_cast<size_t>(k)].address))) return false;
    int64_t got = 0, j = k - 1;
    std::vector<string> regs;
    while (j >= 0 && got < want) {
        const Insn& pj = ins[static_cast<size_t>(j)];
        int64_t n = push_bytes(pj);
        if (!n || static_cast<int64_t>(pj.address) + pj.size != static_cast<int64_t>(ins[static_cast<size_t>(j) + 1].address)) return false;
        got += n;
        if (got < want && targets.count(static_cast<int64_t>(pj.address))) return false;
        regs.push_back(pj.os);
        j -= 1;
    }
    if (got != want) return false;
    // (what set each pushed register last, straight back from the pushes: a constant pushed is an address of its own --
    // `push seg / push off / retf`)
    for (const string& reg : regs) {
        bool digits = !reg.empty() && std::all_of(reg.begin(), reg.end(), [](char c) { return c >= '0' && c <= '9'; });
        if (starts(reg, "0x") || starts(reg, "word ptr") || starts(reg, "cs") || starts(reg, "ss") || starts(reg, "ds") ||
            starts(reg, "es") || digits)
            continue;
        bool found = false;
        for (int64_t qi = j; qi >= 0; qi--) {
            const Insn& q = ins[static_cast<size_t>(qi)];
            if (is_end(q.mn)) break;   // (what is before is not on the way in)
            if (!q.regs_ok) return false;
            bool wrote = false;
            for (int64_t w : q.regs_write)
                if (reg_name_s(w) == reg) wrote = true;
            if (wrote) {
                if (q.mn == "pop") return false;   // (a return address given back)
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

std::set<int64_t> jump_targets(const std::vector<Insn>& ins) {
    std::set<int64_t> t;
    for (const Insn& i : ins)
        if (i.group(CS_GRP_JUMP) && imm_first(i)) t.insert(first_imm(i) & 0xFFFF);
    return t;
}

}  // namespace

// Is ins[k] a return that is a jump -- the instructions straight before it push exactly what it takes off
// (`push dx / push ax / retf`, `push ax / ret`), none of them a place jumped to, and what they push is not a return
// address popped earlier? It goes to the address pushed, the stack as the function was entered: a tail call through a
// pointer. VER's sub_4_003E jumps so to the LZEXPAND function GetProcAddress gave it (`mov dx, [4] / mov ax, [2] ... push
// dx / push ax / retf`). USER's 1:81F5 is not one: `pop ax / pop dx / pop bx / pop cx ... push dx / push ax / retf` takes
// its arguments off by hand and returns to where it was called from -- what is pushed was last set by a pop, and is left
// a return (as is anything not found).
bool jump_by_return(const py::list<Insn>& ins, int64_t k, const py::set<int64_t>& targets) {
    std::set<int64_t> t;
    for (int64_t x : targets) t.insert(x);
    return jump_by_return_i(ins.vec(), k, t);
}

// [(address of a return that is a jump)] in every code segment (jump_by_return).
py::list<int64_t> return_jumps(const Description& d) {
    py::list<int64_t> out;
    std::vector<std::pair<int64_t, py::bytes>> imgs;
    for (const auto& kv : d.images.items()) imgs.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(imgs.begin(), imgs.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    for (const auto& [seg, img] : imgs) {
        if (!d.code.empty() && !d.code.contains(seg)) continue;
        std::vector<Insn> ins = disasm_bytes(true, img, 0);
        std::set<int64_t> targets = jump_targets(ins);
        for (size_t k = 0; k < ins.size(); k++) {
            const Insn& i = ins[k];
            if ((i.mn == "ret" || i.mn == "retf") && i.os.empty() && jump_by_return_i(ins, static_cast<int64_t>(k), targets))
                out.append(address(seg, static_cast<int64_t>(i.address)));
        }
    }
    return out;
}

namespace {

bool has_word_ptr_bp(const string& s) { return starts(s, "word ptr [bp + "); }

// "ax, 0x1f" / "ax, 31"
bool ax_const(const string& os) { return starts(os, "ax, ") && is_num_lit(os.substr(4)); }

}  // namespace

// Bytes of arguments a function takes off the stack itself at its start: its return address popped (one word near, two
// far), then more words popped, then the return address pushed back. -> bytes, or None when it does not start so.
std::optional<int64_t> popped_by_hand(const py::bytes& code, int64_t off, const py::str& kind_py) {
    string kind = kind_py.utf8();
    std::vector<Insn> ins = disasm_v(false, slice_s(code, off, off + 40), off);
    if (ins.size() > 20) ins.resize(20);
    int64_t words = kind == "far" ? 2 : 1;
    const std::vector<string> regs = {"ax", "bx", "cx", "dx", "si", "di", "bp", "es"};
    auto in_regs = [&](const string& s) { return std::find(regs.begin(), regs.end(), s) != regs.end(); };
    // (leaves the stack and the flow alone)
    auto quiet = [&](const Insn& i) {
        return !eq_any(i.mn, {"push", "pop", "call", "lcall", "ret", "retf", "jmp", "ljmp", "int", "enter", "leave", "pushf", "popf",
                              "pusha", "popa"}) &&
               !starts(i.mn, "j") && !contains(i.os, "sp");
    };
    size_t k = 0;
    while (k < ins.size() && k < 3 && quiet(ins[k])) k++;   // (IsIconic: `mov cl, 20h / test ax, ...` first)
    if (k < ins.size() && ins[k].mn == "call" && starts(ins[k].os, "0x") && !contains(ins[k].os, ",")) {
        // a helper does it for it (USER's PostQuitMessage, GetSysColor: `call 8128`, which pops its own return address, this
        // one's, and the arguments, pushes this one's back and `jmp bx` home): the helper's pops past its own return address
        // are this function's own
        int64_t helper = int_of(ins[k].os, 16) & 0xFFFF;
        if (helper != off) {
            std::vector<Insn> h = disasm_v(false, slice_s(code, helper, helper + 40), helper);
            if (h.size() > 20) h.resize(20);
            std::vector<string> hp;
            size_t j = 0;
            while (j < h.size() && h[j].mn == "pop" && in_regs(h[j].os)) {
                hp.push_back(h[j].os);
                j++;
            }
            if (static_cast<int64_t>(hp.size()) > 1 + words) {
                while (j < h.size() && j < 14 && quiet(h[j])) j++;
                std::vector<string> back;
                for (size_t q = j; q < std::min(h.size(), j + static_cast<size_t>(words)); q++)
                    if (h[q].mn == "push") back.push_back(h[q].os);
                bool home = false;
                for (size_t q = j + static_cast<size_t>(words); q < std::min(h.size(), j + static_cast<size_t>(words) + 6); q++)
                    if (h[q].mn == "jmp" && h[q].os == hp[0]) home = true;
                std::vector<string> want;   // list(reversed(hp[1:1 + words]))
                for (int64_t q = std::min<int64_t>(1 + words, static_cast<int64_t>(hp.size())) - 1; q >= 1; q--)
                    want.push_back(hp[static_cast<size_t>(q)]);
                if (back == want && home) return 2 * (static_cast<int64_t>(hp.size()) - 1 - words);
            }
        }
        return std::nullopt;
    }
    std::vector<string> pops;
    while (k < ins.size() && ins[k].mn == "pop" && in_regs(ins[k].os)) {
        pops.push_back(ins[k].os);
        k++;
    }
    if (static_cast<int64_t>(pops.size()) <= words) return std::nullopt;
    while (k < ins.size() && k < 12 && quiet(ins[k])) {   // (GetAsyncKeyState: `xor ah, ah` between)
        string first = before_comma(ins[k].os);
        for (int64_t q = 0; q < std::min<int64_t>(words, static_cast<int64_t>(pops.size())); q++)
            if (contains(first, pops[static_cast<size_t>(q)].c_str())) return std::nullopt;   // (its return address changed: not given back)
        k++;
    }
    std::vector<string> back;
    for (size_t q = k; q < std::min(ins.size(), k + static_cast<size_t>(words)); q++)
        if (ins[q].mn == "push") back.push_back(ins[q].os);
    // (the return address pushed back as it came: the last popped of it first)
    std::vector<string> want;
    for (int64_t q = std::min<int64_t>(words, static_cast<int64_t>(pops.size())) - 1; q >= 0; q--) want.push_back(pops[static_cast<size_t>(q)]);
    if (back != want) return std::nullopt;
    return 2 * (static_cast<int64_t>(pops.size()) - words);
}

// The prologue of a far function that Windows calls (exports, callbacks): `mov ax,ds` or `push ds; pop ax`, `nop`,
// `inc bp; push bp` -- the odd BP marking a far frame.
bool windows_far_prologue(const py::bytes& code, int64_t off) {
    string head = slice_s(code, off, off + 6);
    string h4 = head.substr(0, 4), h2 = head.substr(0, 2);
    return h4 == string("\x8c\xd8\x90\x45", 4) || h4 == string("\x1e\x58\x90\x45", 4) || h2 == string("\x45\x55", 2);
}

// The bytes of arguments a function reads -- through BP, after the prologue sets it (`enter`, or `push bp / mov bp, sp`)
// -- or None when it keeps no such frame (its reads are not known here).
std::optional<int64_t> args_read(const py::bytes& code, int64_t start, int64_t end, const py::str& kind_py) {
    string kind = kind_py.utf8();
    if (windows_far_prologue(code, start)) return std::nullopt;
    std::vector<Insn> ins = disasm_v(true, slice_s(code, start, std::min(end, start + 0x4000)), start);
    size_t k;
    if (!ins.empty() && ins[0].mn == "enter") {
        k = 1;
    } else if (ins.size() > 1 && ins[0].mn == "push" && ins[0].os == "bp" && ins[1].mn == "mov" && ins[1].os == "bp, sp") {
        k = 2;
    } else {
        return std::nullopt;
    }
    int64_t first = kind == "far" ? 6 : 4;   // (past the saved BP and the return address)
    int64_t most = 0;
    for (size_t q = k; q < ins.size(); q++) {
        const Insn& i = ins[q];
        if (eq_any(i.mn, {"mov", "lea", "pop", "xchg"}) && starts(i.os, "bp")) break;   // (BP made something else: no longer the frame)
        for (size_t o = 0; o < i.operands.size(); o++) {
            const inline_asm::Operand& op = i.operands.vec()[o];
            if (op.type == X86_OP_MEM && op.mem.base == X86_REG_BP && op.mem.index == 0 &&
                (op.mem.segment == 0 || op.mem.segment == X86_REG_SS) && op.mem.disp >= first) {
                // (a far pointer loaded or called through is four bytes, whatever Capstone says)
                int64_t size = i.mn == "lea" ? 2 : eq_any(i.mn, {"les", "lds", "lss", "lfs", "lgs", "lcall", "ljmp"}) ? 4 : i.op_size.vec()[o];
                most = std::max(most, op.mem.disp + std::max<int64_t>(size, 2) - first);
            }
        }
    }
    return most + (most & 1);
}

// What a function returns, by its instructions: 2 when on a way to one of its returns the last thing to set AX is its own
// (past the prologue's `mov ax, ds` / `push ds; pop ax` / `mov ax, DGROUP` ahead of `mov ds, ax`, and not a `pop ax` -- AX
// given back as it came) or a call (what the callee left there goes back as it came); 4 when DX is set there as well.
// 0 when no return is seen to carry anything. Walked back from each return through its straight run of instructions.
// 0 as well when a way to a return sets AX nowhere past that prologue: AX is then the data segment the prologue loaded -- a
// register as it came, not a result -- and a function that returns nothing on one way returns nothing at all (LZEXPAND's
// LZDONE: `dec word ptr [32h] / jne` straight to the return, the other way copying globals through AX) -- unless on
// another way AX is set and not read again before the return (LMOUSE31's CPLAPPLET: `mov ax, 0 / jmp` to the return; DS on
// its other way is a nonzero answer as well). `voided`, given a list, has what the other ways would have made it appended
// (results() keeps that where a caller reads AX after all), and `passes` is emptied.
int64_t result_bytes(const py::bytes& code, int64_t start, int64_t end, py::list<PassOn>* passes, py::list<int64_t>* voided) {
    std::vector<Insn> ins = disasm_v(true, slice_s(code, start, end), start);
    int64_t N = static_cast<int64_t>(ins.size());
    // (Windows' own prologue loads DS seventh or eighth: `mov ax, ds` or `push ds; pop ax`, `nop`, `inc bp; push bp; mov bp,
    // sp; push ds; mov ds, ax`)
    bool windows = windows_far_prologue(code, start);
    int64_t headk = -1;
    for (int64_t q = 0; q < std::min<int64_t>(N, windows ? 8 : 6); q++)
        if (ins[static_cast<size_t>(q)].mn == "mov" && ins[static_cast<size_t>(q)].os == "ds, ax") {
            headk = q;
            break;
        }
    int64_t prologue = headk >= 0 ? static_cast<int64_t>(ins[static_cast<size_t>(headk)].address) + ins[static_cast<size_t>(headk)].size : start;
    // (AX there is the data segment -- the caller's, which Windows makes the module's own, or DGROUP's `mov ax, seg` -- not
    // an argument loaded into DS)
    int64_t k = headk >= 0 ? headk : 0;
    bool ds_in_ax = headk >= 0 && (windows || (k > 0 && ins[static_cast<size_t>(k) - 1].mn == "mov" && ax_const(ins[static_cast<size_t>(k) - 1].os)));
    std::set<int64_t> targets = jump_targets(ins);
    // every way into each instruction: the one before it (unless that one jumps away or returns) and every jump to it
    std::map<int64_t, int64_t> at;
    for (int64_t q = 0; q < N; q++) at[static_cast<int64_t>(ins[static_cast<size_t>(q)].address)] = q;
    std::map<int64_t, std::vector<int64_t>> into;
    for (int64_t q = 0; q < N; q++) {
        const Insn& i = ins[static_cast<size_t>(q)];
        if (q + 1 < N && !is_end(i.mn) ) into[static_cast<int64_t>(ins[static_cast<size_t>(q) + 1].address)].push_back(static_cast<int64_t>(i.address));
        if (i.group(CS_GRP_JUMP) && imm_first(i)) into[first_imm(i) & 0xFFFF].push_back(static_cast<int64_t>(i.address));
    }
    auto into_of = [&](int64_t a) -> const std::vector<int64_t>& {
        static const std::vector<int64_t> none;
        auto it = into.find(a);
        return it == into.end() ? none : it->second;
    };
    // (a `pop dx` hands DX a value -- `push ax ... pop dx` -- unless the function saves DX itself, when it is DX given back)
    bool saves_dx = false;
    for (const Insn& i : ins)
        if (i.mn == "push" && (i.os == "dx" || i.os == "edx")) saves_dx = true;
    if (passes) {   // a jump on into another function's body: its result is that one's
        for (const Insn& i : ins)
            if (i.mn == "jmp" && imm_first(i) && !(start <= (first_imm(i) & 0xFFFF) && (first_imm(i) & 0xFFFF) < end))
                passes->append(PassOn(std::tuple<py::str, int64_t>(py::str("into"), first_imm(i) & 0xFFFF)));
    }
    auto reads_dx = [&](const Insn& p, const py::list<int64_t>& r) { return p.mn == "int" || hits_dx(r); };   // (an `int` reads what it is handed: DOS's DS:DX)

    auto read_after = [&](int64_t a) {
        // DX read from what sets AX on to the return, along the way it runs (a conditional jump's fall-through; a jump
        // followed): USER's 21:58E6 `mov ds, dx`, GDI's 4:168F `cmp dx, di` -- DX still in use there, not handed back
        for (int n = 0; n < 24; n++) {
            auto it = at.find(a);
            if (it == at.end()) return true;
            const Insn& p = ins[static_cast<size_t>(it->second)];
            if (p.mn == "ret" || p.mn == "retf") return false;
            if (!p.regs_ok) return true;
            if (reads_dx(p, p.regs_read)) return true;
            if (p.group(CS_GRP_CALL)) return true;
            a = (p.mn == "jmp" && imm_first(p)) ? (first_imm(p) & 0xFFFF) : static_cast<int64_t>(p.address) + p.size;
        }
        return true;
    };

    auto segment_in_dx = [&](int64_t a) {
        // Before what sets AX, along the straight run into it: the first thing to set DX is a segment register put there
        // (`mov dx, es`) -- the selector of a far pointer that goes back in DX:AX (KRNL386's `mov es, sel / mov dx, es / mov
        // ax, es:[off]`) -- or an argument nothing reads again before the return (SUWIN's 3:319C `mov ax, [bp+6] / mov dx,
        // [bp+8] / ... sub ax, cx / add ax, 1FEh / leave / retf`: a pointer into the segment it was given; a compiler writes
        // no DX it will not use). A number or a sum there is as often scratch, and is left out.
        auto pit = at.find(a);
        bool read;
        if (pit == at.end()) {
            read = true;
        } else {
            const Insn& p = ins[static_cast<size_t>(pit->second)];
            if (!p.regs_ok) read = true;
            else read = reads_dx(p, p.regs_read) || read_after(a);
        }
        for (int n = 0; n < 8; n++) {
            const std::vector<int64_t>& prev = into_of(a);
            if (prev.size() != 1 || !at.count(prev[0]) || prev[0] < prologue) return false;
            a = prev[0];
            const Insn& p = ins[static_cast<size_t>(at[a])];
            if (p.group(CS_GRP_CALL)) return false;
            if (!p.regs_ok) return false;
            if (hits_dx(p.regs_write)) {
                string src;
                if (p.mn == "mov") {
                    size_t kk = p.os.rfind(", ");
                    src = kk == string::npos ? p.os : p.os.substr(kk + 2);
                }
                return eq_any(src, {"es", "ds", "ss", "cs"}) || (has_word_ptr_bp(src) && !read);
            }
            read = read || reads_dx(p, p.regs_read);   // (DX read on the way: scratch)
        }
        return false;
    };

    // (`bare`: a way to a return sets no AX past the prologue; `evident`: on one, what sets AX last is not read again before
    // the return -- `mov ax, 0 / jmp done` -- a value set for the caller, where one read on the way -- `mov ax, [34h] / mov
    // [44h], ax` -- was a scratch copy)
    int64_t best = 0;
    bool bare = false, evident = false;
    struct Work {
        int64_t a;
        bool wide;
        int64_t steps;
        bool opaque, used;
    };
    for (int64_t k_ret = 0; k_ret < N; k_ret++) {
        const Insn& i = ins[static_cast<size_t>(k_ret)];
        if (i.mn != "ret" && i.mn != "retf") continue;
        if (jump_by_return_i(ins, k_ret, targets)) {
            // (a jump through the address pushed: what the function jumped to returns -- a call's result, passed on; DX:AX
            // here were the address, not a result)
            if (passes) passes->append(PassOn(py::none_t{}));
            else best = std::max<int64_t>(best, 2);
            continue;
        }
        // back from the return along every way into it, to what last set AX on each (DX set too makes it four bytes only
        // right at the end -- `mov ax, lo / mov dx, hi` -- or by what sets AX itself: DX on the way is as often a scratch
        // register)
        std::vector<Work> work;
        for (int64_t p : into_of(static_cast<int64_t>(i.address))) work.push_back({p, false, 0, false, false});
        std::set<std::tuple<int64_t, bool, bool, bool>> seen;
        while (!work.empty()) {
            Work w0 = work.back();
            work.pop_back();
            int64_t a = w0.a;
            bool wide = w0.wide, opaque = w0.opaque, used = w0.used;
            int64_t steps = w0.steps;
            if (at.count(a) && ds_in_ax && a < prologue && !opaque) bare = true;   // (into the prologue, AX still its DS)
            if (seen.count({a, wide, opaque, used}) || !at.count(a) || a < prologue) continue;
            seen.insert({a, wide, opaque, used});
            const Insn& p = ins[static_cast<size_t>(at[a])];
            // (an interrupt's results are not all in what Capstone says it writes)
            opaque = opaque || eq_any(p.mn, {"int", "into", "int3"});
            if (p.group(CS_GRP_CALL)) {
                if (passes) {   // (its size is the callee's: see results())
                    bool is_near_call = imm_first(p) && p.bytes.byte_(0) == 0xE8;
                    if (is_near_call) passes->append(PassOn(first_imm(p) & 0xFFFF));
                    else passes->append(PassOn(py::none_t{}));
                    best = std::max<int64_t>(best, wide ? 4 : 0);
                } else {
                    best = std::max<int64_t>(best, wide ? 4 : 2);   // the callee's result, passed on
                }
                continue;
            }
            if (!p.regs_ok) continue;
            const py::list<int64_t>& written = p.regs_write;
            if (hits_ax(written)) {
                if (p.mn != "pop") {
                    best = std::max<int64_t>(best, (wide || hits_dx(written) || segment_in_dx(static_cast<int64_t>(p.address))) ? 4 : 2);
                    evident = evident || !used;
                }
                continue;
            }
            bool wn = wide || (hits_dx(written) && steps < 3 && (p.mn != "pop" || !saves_dx));
            bool u = used || hits_ax(p.regs_read);
            int64_t ns = steps + ((p.mn == "pop" || p.mn == "leave" || p.mn == "push") ? 0 : 1);
            for (int64_t q : into_of(a)) work.push_back({q, wn, ns, opaque, u});
        }
    }
    if (bare && !evident && (best || (passes && !passes->empty()))) {
        if (voided) voided->append(best ? best : 2);
        if (passes) passes->clear();
        return 0;
    }
    return best;
}

namespace {
const std::map<string, int64_t> kUnsignedBound = {{"ja", 1}, {"jbe", 1}, {"jae", 0}, {"jb", 0}};   // index <= bound (1) or < bound (0)
}

// `J` lines: each switch jump through a table in its own code segment, `jmp word ptr cs:[reg + table]`, with the places its
// table sends to. The analysis finds the table by the range it can prove for the index -- and the range it proves can be
// wrong: USER's 1:03A5 indexes by a byte offset known to be even (the code tested the low four bits, then shifted), the
// analysis took odd offsets too, read garbage words as places, and followed them through half the segment (sub_1_032E:
// 1.5 GB). Here the table is read as the code bounds it: the compare that guards the jump (`cmp reg, n` / `ja`), the index
// a byte offset unless doubled before (`shl bx, 1` / `add bx, bx`) -- and no further than where the code it sends to begins.
py::list<py::str> jump_tables(const Description& d) {
    py::list<py::str> lines;
    for (const auto& kv : d.segment_bytes.items()) {
        int64_t sg = std::get<0>(kv);
        const py::bytes& codeb = std::get<1>(kv);
        const string& code = codeb.raw();
        size_t at = 0;
        while (true) {
            size_t f = code.find("\x2e\xff", at, 2);
            if (f == string::npos) break;
            at = f;
            std::vector<Insn> got = disasm_v(false, slice_s(codeb, static_cast<int64_t>(at), static_cast<int64_t>(at) + 8), static_cast<int64_t>(at), 1);
            const Insn* ins = got.empty() ? nullptr : &got[0];
            at += 1;
            // r"word ptr cs:\[(bx|si|di) \+ 0x([0-9a-f]+)\]"
            string reg;
            int64_t table = 0;
            bool mm = false;
            if (ins) {
                const string& os = ins->os;
                const string pre = "word ptr cs:[";
                if (starts(os, pre.c_str()) && os.size() > pre.size() + 2 + 5 && os.back() == ']') {
                    string r = os.substr(pre.size(), 2);
                    string rest = os.substr(pre.size() + 2);
                    if ((r == "bx" || r == "si" || r == "di") && starts(rest, " + 0x")) {
                        string hexd = rest.substr(5, rest.size() - 6);
                        if (!hexd.empty() && is_hex_lit("0x" + hexd)) {
                            reg = r;
                            table = int_of(hexd, 16);
                            mm = true;
                        }
                    }
                }
            }
            if (!ins || ins->mn != "jmp" || !mm) continue;
            // the guard: back from the jump, the last conditional branch on an unsigned compare
            std::vector<Insn> back;
            int64_t iaddr = static_cast<int64_t>(ins->address);
            for (int64_t start = std::max<int64_t>(0, iaddr - 24); start < iaddr; start++) {   // (a decode in step with the jump)
                std::vector<Insn> cand = disasm_v(false, slice_s(codeb, start, iaddr), start);
                if (!cand.empty() && static_cast<int64_t>(cand.back().address) + cand.back().size == iaddr) {
                    int64_t sum = 0;
                    for (const Insn& c : cand) sum += c.size;
                    if (sum == iaddr - start) {
                        back = cand;
                        break;
                    }
                }
            }
            if (back.empty()) continue;
            std::optional<int64_t> bound;
            bool doubled = false;
            for (int64_t k = static_cast<int64_t>(back.size()) - 1; k >= 0; k--) {
                const Insn& i = back[static_cast<size_t>(k)];
                if (((i.mn == "shl" || i.mn == "sal") && i.os == reg + ", 1") || (i.mn == "add" && i.os == reg + ", " + reg)) doubled = true;
                auto ub = kUnsignedBound.find(i.mn);
                if (ub != kUnsignedBound.end() && k > 0 && back[static_cast<size_t>(k) - 1].mn == "cmp") {
                    const string& cos = back[static_cast<size_t>(k) - 1].os;
                    // r"\w+, (0x[0-9a-f]+|\d+)"
                    size_t cm = cos.find(", ");
                    bool m2 = false;
                    if (cm != string::npos && cm > 0) {
                        bool word = true;
                        for (size_t q = 0; q < cm; q++) {
                            unsigned char c = static_cast<unsigned char>(cos[q]);
                            if (!(std::isalnum(c) || c == '_')) word = false;
                        }
                        m2 = word && is_num_lit(cos.substr(cm + 2));
                    }
                    if (m2) bound = int_of(cos.substr(cm + 2), 0) + ub->second - 1;   // (largest index)
                    break;
                }
            }
            if (!bound || *bound < 0 || *bound > 0x400) continue;
            int64_t count = doubled ? *bound + 1 : *bound / 2 + 1;
            std::vector<int64_t> targets;
            for (int64_t k = 0; k < count; k++) {
                int64_t p = table + 2 * k;
                if (p + 2 > static_cast<int64_t>(code.size())) break;
                int64_t t = static_cast<unsigned char>(code[static_cast<size_t>(p)]) | (static_cast<unsigned char>(code[static_cast<size_t>(p) + 1]) << 8);
                if ((table <= t && t < table + 2 * count) || t >= static_cast<int64_t>(code.size())) break;   // (into the table itself: past its end)
                targets.push_back(t);
            }
            if (!targets.empty()) {
                string joined;
                for (size_t q = 0; q < targets.size(); q++) {
                    if (q) joined += ",";
                    joined += hx(address(sg, targets[q]));
                }
                lines.append(py::str("J\t" + hx(address(sg, iaddr)) + "\t" + joined));
            }
        }
    }
    return lines;
}

// the bytes of arguments the caller takes off right after the call ins[j] -- `add sp, n`, or `pop bx` / `pop cx` a word
// each (never DX, where a result's high word comes back) -- or 0
static int64_t caller_cleanup_v(const std::vector<Insn>& ins, int64_t j) {
    if (j + 1 >= static_cast<int64_t>(ins.size())) return 0;
    const Insn& nxt = ins[static_cast<size_t>(j) + 1];
    if (nxt.mn == "add") {
        // r"sp, (0x[0-9a-f]+|\d+)"
        if (starts(nxt.os, "sp, ") && is_num_lit(nxt.os.substr(4))) {
            int64_t n = int_of(nxt.os.substr(4), 0);
            return (0 < n && n < 0x100 && n % 2 == 0) ? n : 0;
        }
    }
    int64_t words = 0;
    for (size_t q = static_cast<size_t>(j) + 1; q < std::min(ins.size(), static_cast<size_t>(j) + 4); q++) {
        if (ins[q].mn == "pop" && (ins[q].os == "bx" || ins[q].os == "cx")) words++;
        else break;
    }
    return 2 * words;
}
int64_t _caller_cleanup(const py::list<Insn>& ins, int64_t j) { return caller_cleanup_v(ins.vec(), j); }

namespace {

// nothing after the call ins[j] touches the stack before the frame's LEAVE
bool leave_follows(const std::vector<Insn>& ins, int64_t j) {
    for (size_t q = static_cast<size_t>(j) + 1; q < std::min(ins.size(), static_cast<size_t>(j) + 12); q++) {
        const Insn& p = ins[q];
        if (p.mn == "leave" || (p.mn == "mov" && p.os == "sp, bp")) return true;
        if (eq_any(p.mn, {"push", "pop", "pushf", "popf", "enter", "ret", "retf"}) || p.group(CS_GRP_CALL) || p.group(CS_GRP_JUMP)) return false;
        if (!p.regs_ok) return false;
        for (int64_t w : p.regs_write)
            if (w == X86_REG_SP) return false;
    }
    return false;
}

// the bytes pushed just before the call ins[j] (back to a jump target, the prologue, or anything else that moves SP);
// `push cs` of a near call to a far function not counted
int64_t pushed_before(const std::vector<Insn>& ins, int64_t j, const std::set<int64_t>& targets) {
    int64_t pushed = 0;
    int64_t n = j;
    if (n > 0 && ins[static_cast<size_t>(n) - 1].mn == "push" && ins[static_cast<size_t>(n) - 1].os == "cs") n -= 1;
    for (int64_t q = n - 1; q >= 0; q--) {
        const Insn& p = ins[static_cast<size_t>(q)];
        if (p.mn == "push") {
            pushed += first_size(p) == 4 ? 4 : 2;
            if (targets.count(static_cast<int64_t>(p.address))) break;
            continue;
        }
        if (p.group(CS_GRP_CALL) || p.group(CS_GRP_JUMP) || eq_any(p.mn, {"ret", "retf", "pop", "leave", "enter"}) ||
            targets.count(static_cast<int64_t>(p.address)))
            break;
        if (!p.regs_ok) break;
        bool sp = false;
        for (int64_t w : p.regs_write)
            if (w == X86_REG_SP) sp = true;
        if (sp) break;
    }
    return pushed;
}

}  // namespace

// `O` lines: for each call through a pointer (a driver's dispatch table, a callback), the words the code pushed for it just
// before -- the callee is unknown, so the analysis cannot tell what it takes, and drops what was pushed. Removed by the
// callee (Pascal: Windows' drivers and callbacks) unless the caller takes them off itself right after (`add sp, n`).
py::list<py::str> call_overrides(const Description& d) {
    std::vector<int64_t> segorder;
    std::map<int64_t, std::vector<std::pair<int64_t, int64_t>>> by_seg;
    for (int64_t va : d.names) {
        SegOff w = where(va);
        int64_t sg = std::get<0>(w), off = std::get<1>(w);
        if (!by_seg.count(sg)) segorder.push_back(sg);
        by_seg[sg].push_back({off, va});
    }
    py::list<py::str> lines;
    for (int64_t sg : segorder) {
        auto& lst = by_seg[sg];
        py::bytes code = d.segment_bytes.get(sg, py::bytes());
        std::sort(lst.begin(), lst.end());
        for (size_t k = 0; k < lst.size(); k++) {
            int64_t off = lst[k].first, va = lst[k].second;
            int64_t end = k + 1 < lst.size() ? lst[k + 1].first : code.size();
            std::vector<Insn> ins = disasm_v(true, slice_s(code, off, end), off);
            std::set<int64_t> targets = jump_targets(ins);
            // (what the prologue pushes is saved registers, not arguments: the count stops there)
            int64_t prologue = off;
            for (size_t q = 0; q < std::min<size_t>(ins.size(), 12); q++) {
                const Insn& i = ins[q];
                if (i.mn == "nop" || i.mn == "enter" || i.os == "ax, ds" || i.os == "bp, sp" || i.os == "ds, ax" ||
                    ((i.mn == "push" || i.mn == "inc") && eq_any(i.os, {"bp", "ds", "si", "di"})) || (i.mn == "pop" && i.os == "ax") ||
                    (i.mn == "mov" && starts(i.os, "ax, 0x")) || (i.mn == "sub" && starts(i.os, "sp,"))) {
                    prologue = static_cast<int64_t>(i.address) + i.size;
                } else {
                    break;
                }
            }
            targets.insert(prologue);
            for (size_t j = 0; j < ins.size(); j++) {
                const Insn& i = ins[j];
                if (!i.group(CS_GRP_CALL) || i.operands.empty()) continue;
                if (i.operands.vec()[0].type == X86_OP_IMM) {
                    // A direct call's callee is described -- but a C function's description says only that it removes nothing,
                    // not what it takes: its caller says that, in the cleanup right after the call (`add sp, 6`; at /Os `pop
                    // bx`/`pop cx` for each word). SUWIN's sub_2_0A92 passed sub_8_0F24 a far pointer and a character (`add sp,
                    // 6`); without this the call came out with no arguments at all.
                    // (CB_CALLER_ARGS=0 turns it off: on SUWIN as many functions are the original's bytes with it as without --
                    // 37 -- once each is compiled in its own source file; Setup on the 15 MINI.CAB modules rebuilt with it ran to
                    // the desktop)
                    int64_t took = env_not("CB_CALLER_ARGS", "0") ? caller_cleanup_v(ins, static_cast<int64_t>(j)) : 0;
                    if (!took && env_not("CB_CALLER_ARGS", "0") && i.mn == "call" && leave_follows(ins, static_cast<int64_t>(j))) {
                        // (or none: Microsoft C 8 leaves a call's arguments to the frame's LEAVE when the function ends there --
                        // SETUPX's 4:16D8 pushes sub_4_1470 six words and never takes them off. The pushes are the arguments, when
                        // the callee is one that leaves them to its caller: its own return says so)
                        int64_t target = first_imm(i) & 0xFFFF;
                        if (std::get<1>(returns(d.segment_bytes.get(sg, py::bytes()), target)) == 0) took = pushed_before(ins, static_cast<int64_t>(j), targets);
                    }
                    if (took) {
                        // (`push cs; call` is a far call to a function of the same file: the pushed CS is the return address's
                        // segment, not an argument)
                        bool is_far = i.mn == "lcall" || (j > 0 && ins[j - 1].mn == "push" && ins[j - 1].os == "cs");
                        lines.append(py::str("O\t" + hx(address(sg, static_cast<int64_t>(i.address))) + "\t" + hx(va) + "\t" + std::to_string(took) + "\t" +
                                             "__cdecl16" + (is_far ? "far" : "near")));
                    }
                    continue;
                }
                int64_t pushed = 0;
                for (int64_t q = static_cast<int64_t>(j) - 1; q >= 0; q--) {
                    const Insn& p = ins[static_cast<size_t>(q)];
                    if (p.mn == "push") {
                        pushed += (!p.operands.empty() && first_size(p) == 2) ? 2 : (!p.operands.empty() && first_size(p) == 4) ? 4 : 2;
                        if (targets.count(static_cast<int64_t>(p.address))) break;
                        continue;
                    }
                    if (p.group(CS_GRP_CALL) || p.group(CS_GRP_JUMP) || eq_any(p.mn, {"ret", "retf", "pop", "leave", "enter"}) ||
                        targets.count(static_cast<int64_t>(p.address)))
                        break;
                    if (!p.regs_ok) break;
                    bool sp = false;
                    for (int64_t w : p.regs_write)
                        if (w == X86_REG_SP) sp = true;
                    if (sp) break;
                }
                if (!pushed) continue;
                bool is_far = i.mn == "lcall" || (i.mn == "call" && contains(i.os, "dword"));
                const Insn* nxt = j + 1 < ins.size() ? &ins[j + 1] : nullptr;
                bool caller_pops = nxt && nxt->mn == "add" && starts(nxt->os, "sp,");
                string model = string(caller_pops ? "__cdecl16" : "__stdcall16") + (is_far ? "far" : "near");
                lines.append(py::str("O\t" + hx(address(sg, static_cast<int64_t>(i.address))) + "\t" + hx(va) + "\t" + std::to_string(pushed) + "\t" + model));
            }
        }
    }
    return lines;
}

// ---- who removes a function's arguments ---------------------------------------------------------------------------------
namespace {

// The direct call at ins[k] by its target place ("call 0x.." near, "lcall 0x.., 0x.." far into the module), or none.
// `any_far`: a far call to an import (selector >= 0xE000) is a none, as the callers write it.
std::optional<SegOff> direct_call_target(const Insn& i, int64_t seg) {
    if (i.mn == "call" && starts(i.os, "0x") && !contains(i.os, ",")) return SegOff{seg, int_of(i.os, 16) & 0xFFFF};
    if (i.mn == "lcall" && contains(i.os, ",") && starts(i.os, "0x")) {
        std::vector<string> parts = split_on(i.os, ", ");
        int64_t s = int_of(parts[0], 16), o = int_of(parts.at(1), 16);
        if (s < 0xE000) return where((s << 16) | o);
        return std::nullopt;
    }
    return std::nullopt;
}
bool is_direct_call(const Insn& i) {
    return (i.mn == "call" && starts(i.os, "0x") && !contains(i.os, ",")) || (i.mn == "lcall" && contains(i.os, ",") && starts(i.os, "0x"));
}

}  // namespace

// {(seg, off): "callee" | "caller"} -- who removes a function's arguments, as the original's own calls to it show: every
// call that pushed something before it and is followed by `add sp, n` says the caller does; every one that is not says the
// callee does. Only a function whose calls (two at least) all say the same is in it. The callers are the original program's
// own word on how each callee is called -- the callee's instructions can hide it (arguments taken off by hand, a shared
// epilogue).
py::dict<SegOff, py::str> callers_say(const Description& d) {
    std::vector<SegOff> order;
    std::map<SegOff, std::vector<string>> votes;
    for (const auto& kv : d.images.items()) {
        int64_t seg = std::get<0>(kv);
        std::vector<Insn> ins = disasm_bytes(false, std::get<1>(kv), 0);
        for (size_t k = 0; k + 1 < ins.size(); k++) {
            const Insn& i = ins[k];
            if (!is_direct_call(i)) continue;
            std::optional<SegOff> target = direct_call_target(i, seg);
            if (!target || k == 0 || ins[k - 1].mn != "push") continue;   // (nothing pushed just before: says nothing)
            const Insn& nxt = ins[k + 1];
            string who = (nxt.mn == "add" && starts(nxt.os, "sp, ")) ? "caller" : "callee";
            if (!votes.count(*target)) order.push_back(*target);
            votes[*target].push_back(who);
        }
    }
    py::dict<SegOff, py::str> out;
    for (const SegOff& t : order) {
        const auto& v = votes[t];
        bool same = true;
        for (const string& x : v) if (x != v[0]) same = false;
        if (v.size() >= 2 && same) out.set(t, py::str(v[0]));
    }
    return out;
}

// Bytes of arguments a thunk's caller has removed, for a thunk that puts more arguments under its return address and jumps
// on: `pop bx / pop dx` (the return address), `push`es (the words it adds), `push dx / push bx`, then `jmp` / `ljmp` to the
// function that does the work (USER's 1:0110 adds a 0 and goes to BeginPaint's body at 1:75C8). What the target removes, less
// the words the thunk added. -> bytes, or None when it is not such a thunk.
std::optional<int64_t> forwards(const Description& d, int64_t seg, int64_t off, const py::str& kind_py) {
    string kind = kind_py.utf8();
    py::bytes code;   // (relocated: a far jump's segment as the placeholder)
    {
        auto im = d.images.get(seg);
        if (im && !im->empty()) code = *im;
        else code = d.segment_bytes.get(seg, py::bytes());
    }
    Dec& dec = held(code);
    // (read along the way it runs: a near jump inside the segment followed, not listed -- USER's 1:2440 `pop ax / pop dx / xor
    // cx, cx / jmp 244C` into 1:2446's `push cx / push dx / push ax`, then on into the body they share, which ends `retf
    // 0Eh`: its callers push six words, not seven, and the C passed DS for a seventh)
    std::vector<Insn> ins;
    int64_t at = off;
    while (ins.size() < 30 && 0 <= at && at < code.size()) {
        auto ip = dec.one(at);
        if (!ip) break;
        const Insn& i = *ip;
        if (i.mn == "jmp" && is_hex_lit(i.os) && !ins.empty()) {
            at = int_of(i.os, 16) & 0xFFFF;
            Insn into;   // (where it went: the body, if it is next)
            into.mn = "into";
            into.mnemonic = py::str("into");
            into.address = static_cast<uint64_t>(at);
            into.size = 0;
            ins.push_back(into);
            continue;
        }
        ins.push_back(i);
        at = static_cast<int64_t>(i.address) + i.size;
    }
    // (the body's address first put in a register, the rest shared: Win95 USER's entries `mov cx, 2FAh / jmp 6Bh`, and at 6B
    // `pop eax / push 0 / push eax / jmp cx`)
    std::map<string, int64_t> loaded;
    size_t b = 0;   // ins = ins[b:]
    auto mov_const = [&](const Insn& i) {   // r"(ax|bx|cx|dx), 0x[0-9a-f]+"
        return i.mn == "mov" && i.os.size() > 4 && eq_any(i.os.substr(0, 4), {"ax, ", "bx, ", "cx, ", "dx, "}) && is_hex_lit(i.os.substr(4));
    };
    while (b < ins.size() && (ins[b].mn == "into" || mov_const(ins[b]))) {
        if (ins[b].mn == "mov") loaded[ins[b].os.substr(0, 2)] = int_of(ins[b].os.substr(4), 16);
        b++;
    }
    size_t n = ins.size() - b;
    auto at_i = [&](size_t k) -> const Insn& { return ins[b + k]; };
    int64_t words = kind == "far" ? 2 : 1;
    const std::vector<string> regs = {"ax", "bx", "cx", "dx", "si", "di", "es"};
    auto in_regs = [&](const string& s) { return std::find(regs.begin(), regs.end(), s) != regs.end(); };
    // (a far return address taken off whole, in one 32-bit register: Win95 USER's 5:0011 (lstrcmpi) `pop eax / push word
    // es:[90h] / push eax / jmp 6E1` -- its callers push four words, not the five the body's `retf 0Ah` removes, and the C
    // passed a sixth and a seventh)
    if (kind == "far" && n > 0 && at_i(0).mn == "pop" && eq_any(at_i(0).os, {"eax", "ebx", "ecx", "edx"})) {
        words = 1;
    } else {
        for (size_t q = 0; q < std::min<size_t>(n, static_cast<size_t>(words)); q++)
            if (at_i(q).mn != "pop" || !in_regs(at_i(q).os)) return std::nullopt;
    }
    if (static_cast<int64_t>(n) < 2 * words + 2) return std::nullopt;
    std::vector<string> ret;
    for (size_t q = 0; q < std::min<size_t>(n, static_cast<size_t>(words)); q++) ret.push_back(at_i(q).os);
    size_t k = static_cast<size_t>(words);
    int64_t added = 0, taken = 0;
    std::vector<string> wide = regs;
    for (const char* w : {"eax", "ebx", "ecx", "edx", "esi", "edi"}) wide.push_back(w);
    // (and the caller's arguments it takes off, before adding its own or among them -- USER's 1:014C pops one, pushes 0 and 1
    // or 5 (a `jcxz` choosing), puts the return address back, sets slots in the stack and goes to 1:3E1E: its callers' stack
    // came out four bytes short in the gate; Win95 USER.88 (EndDialog) `pop ecx / pop ax / cwde / push eax / push ecx`, its
    // int widened to a long, and USER's 8:43D6 `pop eax / pop bx / xor ecx, ecx / pop cx / push ecx`)
    const std::map<string, string> writes = {{"cbw", "ax"}, {"cwde", "eax"}, {"cwd", "dx"}, {"cdq", "edx"}};
    auto quiet = [&](const Insn& i) {   // (moves nothing on the stack)
        auto w = writes.find(i.mn);
        if (w != writes.end()) {   // (widening in a register)
            for (const string& r : ret)
                if (contains(w->second, r.c_str()) || contains(r, w->second.c_str())) return false;
            return true;
        }
        if (eq_any(i.mn, {"xor", "mov", "sub", "cmp", "test", "and", "or", "movzx", "movsx"}) && !contains(before_comma(i.os), "sp")) return true;
        if (eq_any(i.mn, {"jcxz", "je", "jne"}) && is_hex_lit(i.os)) {
            int64_t t = int_of(i.os, 16);
            return static_cast<int64_t>(i.address) < t && t <= static_cast<int64_t>(i.address) + 0x20;
        }
        return false;
    };
    while (k < n && (eq_any(at_i(k).mn, {"push", "pop", "into"}) || quiet(at_i(k))) && !(at_i(k).mn == "push" && at_i(k).os == ret.back())) {
        const Insn& q = at_i(k);
        if (q.mn == "into") {   // (a jump followed: it goes on there)
            k += 1;
            continue;
        }
        if (q.mn == "pop") {
            if (!std::count(wide.begin(), wide.end(), q.os) || std::count(ret.begin(), ret.end(), q.os)) return std::nullopt;
            taken += (starts(q.os, "e") && q.os.size() == 3) ? 2 : 1;
        } else if (q.mn == "push") {   // (a 32-bit push is two words: GDI.612's `push 0` is 66 68 00000000)
            bool pre66 = (q.bytes.size() > 0 && q.bytes.byte_(0) == 0x66) || (q.bytes.size() > 1 && q.bytes.byte_(1) == 0x66);
            added += pre66 ? 2 : 1;
        } else {
            string first = before_comma(q.os);
            for (const string& r : ret)
                if (contains(first, r.c_str())) return std::nullopt;   // (its return address changed)
        }
        k += 1;
    }
    std::vector<string> back;
    for (size_t q = k; q < std::min(n, k + static_cast<size_t>(words)); q++)
        if (at_i(q).mn == "push") back.push_back(at_i(q).os);
    std::vector<string> rev(ret.rbegin(), ret.rend());
    if (back != rev || !added || k + static_cast<size_t>(words) >= n) return std::nullopt;
    k += static_cast<size_t>(words);
    while (k + 1 < n && quiet(at_i(k))) k++;   // (slots set beneath, then the jump)
    if (k >= n) return std::nullopt;
    const Insn& j = at_i(k);
    int64_t tseg, toff;
    auto split_far = [&](const string& os) -> std::optional<std::pair<int64_t, int64_t>> {   // r"0x[0-9a-f]+(, |:)0x[0-9a-f]+"
        size_t cut = os.find(", ");
        size_t sepw = 2;
        if (cut == string::npos) {
            cut = os.find(':');
            sepw = 1;
        }
        if (cut == string::npos) return std::nullopt;
        string a = os.substr(0, cut), c = os.substr(cut + sepw);
        if (!is_hex_lit(a) || !is_hex_lit(c)) return std::nullopt;
        return std::make_pair(int_of(a, 16), int_of(c, 16));
    };
    auto far_ok = j.mn == "ljmp" ? split_far(j.os) : std::nullopt;
    if (far_ok) {
        int64_t s = far_ok->first, o = far_ok->second;
        if (s >= 0xE000) return std::nullopt;   // (another module's: not known here)
        SegOff w = where((s << 16) | o);
        tseg = std::get<0>(w);
        toff = std::get<1>(w);
    } else if (j.mn == "into") {   // (a near jump, followed: the body there)
        tseg = seg;
        toff = static_cast<int64_t>(j.address);
    } else if (j.mn == "jmp" && loaded.count(j.os)) {   // (to the address put in the register)
        tseg = seg;
        toff = loaded[j.os];
    } else if (eq_any(j.mn, {"mov", "enter", "push", "inc", "nop"})) {
        tseg = seg;   // (no jump: the body goes on right here)
        toff = static_cast<int64_t>(j.address);
    } else {
        return std::nullopt;
    }
    py::bytes target_code = d.segment_bytes.get(tseg, py::bytes());
    int64_t pop = std::get<1>(returns_i(held(target_code), toff, 600, kind));
    if (pop < 0) return std::nullopt;   // (None, or none met)
    return std::max<int64_t>(0, pop - 2 * added + 2 * taken);
}

// Does the function at `off` take arguments in AX, DX or BX (Microsoft C's _fastcall): its straight-line start (to the
// first jump, call or return) reads one of them (or AL, DL, BL) before it writes it. SUWIN's 7:0000 begins `push bp / mov
// bp, sp / push ax` -- its char argument, kept in its frame -- and its callers load AX just before the call.
bool register_arguments(const py::bytes& code, int64_t off) {
    static const std::map<string, string> kFastcall = {{"ax", "ax"}, {"al", "ax"}, {"ah", "ax"}, {"eax", "ax"}, {"dx", "dx"},
                                                      {"dl", "dx"}, {"dh", "dx"}, {"edx", "dx"}, {"bx", "bx"}, {"bl", "bx"},
                                                      {"bh", "bx"}, {"ebx", "bx"}};
    std::set<string> written;
    for (const Insn& i : disasm_v(true, slice_s(code, off, off + 64), off)) {
        if (i.group(CS_GRP_JUMP) || i.group(CS_GRP_CALL) || i.group(CS_GRP_RET) || eq_any(i.mn, {"int", "iret", "retf"})) return false;
        if (!i.regs_ok) return false;
        std::vector<string> ops = split_strip_commas(i.os);
        bool same = eq_any(i.mn, {"xor", "sub", "sbb"}) && ops.size() == 2 && ops[0] == ops[1];
        if (!same) {
            for (int64_t r : i.regs_read) {
                auto f = kFastcall.find(reg_name_s(r));
                if (f != kFastcall.end() && !written.count(f->second)) return true;
            }
        }
        for (int64_t r : i.regs_write) {
            auto f = kFastcall.find(reg_name_s(r));
            if (f != kFastcall.end()) written.insert(f->second);
        }
    }
    return false;
}

// Is the function at `off` a helper whose effect is on its caller's registers: a short straight run to a plain `ret` -- no
// frame, no branch, no call -- that leaves SI, DI or DS changed, registers a C function keeps for its caller? KEYBOARD's
// sub_9_0A6B, `push ax / push bx / mov ax, ds / add ax, 8 / mov ds, ax / xor si, si / pop bx / pop ax / ret`, moves OEMTOANSI's
// source pointer DS:SI on to the next 64K of a huge buffer. As a function of its own its C can only `return`, and the caller's
// C goes on through the old pointer (its copy past 64K went back over the start). Such a helper is decompiled as part of each
// caller -- the description's `inline` -- so the caller's C moves its own pointer.
bool register_helper(const py::bytes& code, int64_t off, const std::optional<int64_t>& end) {
    static const std::map<string, string> kKept = {{"si", "si"}, {"esi", "si"}, {"di", "di"}, {"edi", "di"}, {"ds", "ds"}};
    int64_t stop = !end ? off + 48 : std::min(*end, off + 48);
    std::vector<string> pushed;
    std::set<string> changed;
    int64_t k = 0;
    for (const Insn& i : disasm_v(true, slice_s(code, off, stop), off)) {
        if (k >= 16) return false;
        k++;
        if (i.mn == "ret" && i.os.empty()) return pushed.empty() && !changed.empty();
        if (i.group(CS_GRP_JUMP) || i.group(CS_GRP_CALL) || i.group(CS_GRP_RET) || i.group(CS_GRP_INT) ||
            eq_any(i.mn, {"retf", "iret", "enter", "leave", "int", "into"}))
            return false;
        if (!i.regs_ok) return false;
        std::set<string> names;
        for (int64_t r : i.regs_read) names.insert(reg_name_s(r));
        for (int64_t r : i.regs_write) names.insert(reg_name_s(r));
        std::set<string> frame = {"bp", "ebp", "sp", "esp"};
        if (i.mn == "push" || i.mn == "pop") {
            frame.erase("sp");
            frame.erase("esp");
        }
        for (const string& nm : names)
            if (frame.count(nm)) return false;   // (a frame, or its caller's: not this)
        if (i.mn == "push") {
            pushed.push_back(i.os);
            continue;
        }
        if (i.mn == "pop") {
            if (pushed.empty()) return false;
            string top = pushed.back();
            pushed.pop_back();
            if (top != i.os) return false;
            auto kp = kKept.find(i.os);
            if (kp != kKept.end()) changed.erase(kp->second);
            continue;
        }
        for (int64_t r : i.regs_write) {
            auto kp = kKept.find(reg_name_s(r));
            if (kp != kKept.end()) changed.insert(kp->second);
        }
    }
    return false;
}

// {(seg, off)} of `places` a call to which is followed, before anything sets it, by a read of AX (`or ax, ax`, `mov [x], ax`,
// `push ax`): what it leaves there is a result its caller uses.
py::set<SegOff> callers_read_ax(const Description& d, const py::set<SegOff>& places) {
    py::set<SegOff> out;
    for (const auto& kv : d.images.items()) {
        int64_t seg = std::get<0>(kv);
        if (!d.code.empty() && !d.code.contains(seg)) continue;
        std::vector<Insn> ins = disasm_bytes(true, std::get<1>(kv), 0);
        for (size_t k = 0; k + 1 < ins.size(); k++) {
            const Insn& i = ins[k];
            if (!is_direct_call(i)) continue;
            std::optional<SegOff> target = direct_call_target(i, seg);
            if (!target) continue;
            if (!places.contains(*target) || out.contains(*target)) continue;
            for (size_t q = k + 1; q < std::min(ins.size(), k + 8); q++) {
                const Insn& s = ins[q];
                if (s.group(CS_GRP_JUMP) || s.group(CS_GRP_CALL) || s.group(CS_GRP_RET) || eq_any(s.mn, {"int", "iret", "leave"})) break;
                std::vector<string> ops = split_strip_commas(s.os);
                if ((s.mn == "xor" || s.mn == "sub") && ops.size() == 2 && ops[0] == ops[1]) {
                    if (eq_any(ops[0], {"ax", "al", "ah", "eax"})) break;   // (cleared: written, not read)
                    continue;
                }
                if (!s.regs_ok) break;
                if (hits_ax(s.regs_read)) {
                    out.add(*target);
                    break;
                }
                if (hits_ax(s.regs_write)) break;
            }
        }
    }
    return out;
}

// {(seg, off)} of the functions a call to which is followed, before anything else could set it, by a read of DX -- the callee
// returns four bytes, DX:AX, whatever its own instructions let one tell: a compiler reads DX after a call only for a long or a
// far pointer it returned (SETUPX's 4:6C9C: `lcall SUREGCREATEKEY / or dx, ax`, the key's LONG result, read as an int and a DX
// nothing in the C had set). `edges`, given a list, has [(caller, callee)] added for each call whose AX and DX go back to the
// caller's own caller as they are (nothing writes either before the return, a jump to it followed): if the caller returns four
// bytes, so does the callee (SUREGQUERYVALUEEX hands back sub_31_002A's).
py::set<SegOff> callers_read_dx(const Description& d, py::list<std::tuple<SegOff, SegOff>>* edges) {
    py::set<SegOff> out;
    std::map<int64_t, std::vector<int64_t>> starts_;
    for (int64_t va : d.names) {
        SegOff w = where(va);
        starts_[std::get<0>(w)].push_back(std::get<1>(w));
    }
    for (auto& kv : starts_) std::sort(kv.second.begin(), kv.second.end());
    for (const auto& kv : d.images.items()) {
        int64_t seg = std::get<0>(kv);
        if (!d.code.empty() && !d.code.contains(seg)) continue;   // (a data segment: no calls in it)
        std::vector<Insn> ins = disasm_bytes(true, std::get<1>(kv), 0);
        std::map<int64_t, size_t> at;
        for (size_t q = 0; q < ins.size(); q++) at[static_cast<int64_t>(ins[q].address)] = q;
        for (size_t k = 0; k + 1 < ins.size(); k++) {
            const Insn& i = ins[k];
            if (edges && (i.mn == "call" || i.mn == "lcall") && starts_.count(seg)) {
                const auto& st = starts_[seg];
                auto ub = std::upper_bound(st.begin(), st.end(), static_cast<int64_t>(i.address));
                std::optional<int64_t> own = ub != st.begin() ? std::optional<int64_t>(*(ub - 1)) : std::nullopt;
                size_t n = k + 1;
                int64_t hops = 0;
                bool through = false;
                while (own && n < ins.size() && hops < 12) {
                    const Insn& q = ins[n];
                    if (q.mn == "ret" || q.mn == "retf") {
                        through = true;
                        break;
                    }
                    if (q.mn == "jmp" && starts(q.os, "0x") && at.count(int_of(q.os, 16))) {
                        n = at[int_of(q.os, 16)];
                        hops += 1;
                        continue;
                    }
                    if (q.group(CS_GRP_JUMP) || q.group(CS_GRP_CALL)) break;
                    if (!q.regs_ok) break;
                    if (hits_ax(q.regs_write) || hits_dx(q.regs_write)) break;
                    n += 1;
                    hops += 1;
                }
                if (through) {
                    std::optional<SegOff> tgt;
                    if (i.mn == "call" && starts(i.os, "0x") && !contains(i.os, ",")) tgt = SegOff{seg, int_of(i.os, 16) & 0xFFFF};
                    else if (i.mn == "lcall" && contains(i.os, ",") && starts(i.os, "0x")) tgt = direct_call_target(i, seg);
                    if (tgt) edges->append(std::make_tuple(SegOff{seg, *own}, *tgt));
                }
            }
            if (!is_direct_call(i)) continue;
            std::optional<SegOff> target = direct_call_target(i, seg);
            if (!target) continue;
            // (not when the caller set DX itself just before: what it reads may be its own, kept -- KEYBOARD's 9:0367 saves AX
            // and BX and leaves DX alone -- nor for a _fastcall callee, whose DX may be its argument)
            bool set_before = false;
            for (size_t q = k >= 6 ? k - 6 : 0; q < k; q++)
                if (ins[q].regs_ok && hits_dx(ins[q].regs_write)) set_before = true;
            if (set_before) continue;
            if (d.segment_bytes.contains(std::get<0>(*target)) &&
                register_arguments(d.segment_bytes.at(std::get<0>(*target)), std::get<1>(*target)))
                continue;
            for (size_t q = k + 1; q < std::min(ins.size(), k + 8); q++) {
                const Insn& s = ins[q];
                if (s.group(CS_GRP_JUMP) || s.group(CS_GRP_CALL) || s.group(CS_GRP_RET) || eq_any(s.mn, {"int", "iret", "leave"})) break;
                if (!s.regs_ok) break;
                if ((s.mn == "xor" && s.os == "dx, dx") || (s.mn == "sub" && s.os == "dx, dx")) break;   // (cleared: written, not read)
                if (hits_dx(s.regs_read)) {
                    out.add(*target);
                    break;
                }
                if (hits_dx(s.regs_write)) break;
            }
        }
    }
    return out;
}

// ---- _DxReturns ------------------------------------------------------------------------------------------------------
struct _DxReturns::Impl {
    struct Func {
        std::vector<Insn> ins;
        std::map<int64_t, size_t> at;
        std::map<int64_t, std::vector<int64_t>> into;
        int64_t end = 0;
        bool saves_dx = false, saves_ax = false;
    };
    std::map<SegOff, std::shared_ptr<Func>> code;   // null: None
};

_DxReturns::_DxReturns(const Description& d) : d_(&d), md_(decoder16(true)), impl_(std::make_shared<Impl>()) {
    for (int64_t va : d.names) {
        SegOff w = where(va);
        int64_t sg = std::get<0>(w), o = std::get<1>(w);
        if (!starts_.contains(sg)) starts_.set(sg, py::list<int64_t>());
        starts_.at(sg).append(o);
    }
    for (int64_t sg : starts_.keys()) {
        std::vector<int64_t>& v = starts_.at(sg).vec();
        std::sort(v.begin(), v.end());
    }
}

namespace {

bool saves_ax_of(const std::vector<Insn>& ins) {
    // Does it push AX as it was given -- before anything sets AX, on the way in from its start? (Not `push ebx / pop ax / pop
    // dx`, KRNL386's 2:4C91 handing back a LONG made in EBX.)
    for (const Insn& i : ins) {
        if (i.mn == "push" && i.os == "ax") return true;
        if (i.group(CS_GRP_JUMP) || i.group(CS_GRP_CALL) || i.group(CS_GRP_RET)) return false;
        if (!i.regs_ok) return false;
        if (hits_ax(i.regs_write)) return false;
    }
    return false;
}

// Is AX, on some way to a return, last set by a `pop ax` -- the caller's own AX given back? Then nothing comes back in DX:AX
// either, whatever a call before it did to DX: KEYBOARD's 9:0367 saves AX and BX, calls the keyboard VxD's entry through [20h]
// (a pointer, taken as handing back its DX) and returns them as they were; its callers read their own DX.
bool gives_back_ax(const std::vector<Insn>& ins, const std::map<int64_t, size_t>& at,
                   const std::map<int64_t, std::vector<int64_t>>& into, const std::vector<const Insn*>& ends) {
    std::vector<int64_t> work;
    std::set<int64_t> seen;
    auto into_of = [&](int64_t a) -> std::vector<int64_t> {
        auto it = into.find(a);
        return it == into.end() ? std::vector<int64_t>() : it->second;
    };
    for (const Insn* r : ends)
        for (int64_t x : into_of(static_cast<int64_t>(r->address))) work.push_back(x);
    while (!work.empty()) {
        int64_t a = work.back();
        work.pop_back();
        if (seen.count(a) || !at.count(a)) continue;
        seen.insert(a);
        const Insn& p = ins[at.at(a)];
        if (p.mn == "pop" && p.os == "ax") return true;
        if (p.group(CS_GRP_CALL) || p.mn == "int" || p.mn == "into") continue;   // (AX from a call: not the caller's)
        if (!p.regs_ok) continue;
        if (hits_ax(p.regs_write)) continue;
        for (int64_t x : into_of(a)) work.push_back(x);
    }
    return false;
}

}  // namespace

bool _DxReturns::every_way(const SegOff& place, const py::set<SegOff>& four) {
    // the function's instructions, kept
    std::shared_ptr<Impl::Func> f;
    auto cached = impl_->code.find(place);
    if (cached != impl_->code.end()) {
        f = cached->second;
    } else {
        int64_t sg = std::get<0>(place), off = std::get<1>(place);
        auto img = d_->images.get(sg);
        if (img) {
            std::vector<int64_t> later;
            if (starts_.contains(sg))
                for (int64_t o : starts_.at(sg)) if (o > off) later.push_back(o);
            int64_t end = !later.empty() ? later[0] : img->size();
            std::vector<Insn> ins = disasm_v(true, slice_s(*img, off, end), off);
            // (one that runs on into the next function's start, with no return or jump of its own at the end, is that code
            // too: SETUPX's 3:8246 is cut by a start found inside one of its instructions, 3:8259)
            for (int n = 0; n < 4; n++) {
                if (ins.empty() || is_end(ins.back().mn) || end - (static_cast<int64_t>(ins.back().address) + ins.back().size) >= 16 || end >= img->size()) break;
                std::vector<int64_t> l2;
                for (int64_t o : later) if (o > end) l2.push_back(o);
                later = l2;
                end = !later.empty() ? later[0] : img->size();
                ins = disasm_v(true, slice_s(*img, off, end), off);
            }
            f = std::make_shared<Impl::Func>();
            for (size_t k = 0; k < ins.size(); k++) f->at[static_cast<int64_t>(ins[k].address)] = k;
            for (size_t k = 0; k < ins.size(); k++) {
                const Insn& i = ins[k];
                if (k + 1 < ins.size() && !is_end(i.mn)) f->into[static_cast<int64_t>(ins[k + 1].address)].push_back(static_cast<int64_t>(i.address));
                if (i.group(CS_GRP_JUMP) && imm_first(i) && i.mn != "ljmp") f->into[first_imm(i) & 0xFFFF].push_back(static_cast<int64_t>(i.address));
            }
            f->end = end;
            for (const Insn& i : ins) if (i.mn == "push" && i.os == "dx") f->saves_dx = true;
            f->saves_ax = saves_ax_of(ins);
            f->ins = std::move(ins);
        }
        impl_->code[place] = f;
    }
    if (!f) return false;
    const std::vector<Insn>& ins = f->ins;
    int64_t sg = std::get<0>(place), off = std::get<1>(place);
    int64_t end = f->end;
    auto into_of = [&](int64_t a) -> std::vector<int64_t> {
        auto it = f->into.find(a);
        return it == f->into.end() ? std::vector<int64_t>() : it->second;
    };
    // (a direct call's or jump's target in the module, "out" for an import, none for one through a register or memory)
    enum Kind { None, Out, Place };
    struct Target {
        Kind kind = None;
        SegOff place{0, 0};
    };
    auto target_of = [&](const Insn& i) -> Target {
        Target t;
        if (!imm_first(i)) return t;
        if (i.mn == "lcall" || i.mn == "ljmp") {
            std::vector<string> parts = split_on(i.os, contains(i.os, ":") ? ":" : ", ");
            int64_t s = int_of(parts.at(0), 16), o = int_of(parts.at(1), 16);
            if (s >= 0xE000) {
                t.kind = Out;
            } else {
                t.kind = Place;
                t.place = where((s << 16) | o);
            }
            return t;
        }
        t.kind = Place;
        t.place = SegOff{sg, first_imm(i) & 0xFFFF};
        return t;
    };
    // (a call, or a jump on: its callee's DX -- an import's, or one through a pointer: SETUPX's 28:111C `call far es:[bx+4]`)
    auto handed_on = [&](const Insn& i) {
        Target t = target_of(i);
        return t.kind == Out || t.kind == None || four.contains(t.place);
    };
    // (the instructions reached from its start -- what lies past a jump on, a jump to an import (GETTICKCOUNT: `jmp far
    // KERNEL`), is not its; through a jump table everything may be)
    std::set<int64_t> reach;
    std::vector<int64_t> todo{off};
    while (!todo.empty()) {
        int64_t a = todo.back();
        todo.pop_back();
        if (reach.count(a) || !f->at.count(a)) continue;
        reach.insert(a);
        const Insn& i = ins[f->at.at(a)];
        if (i.group(CS_GRP_JUMP) && imm_first(i) && i.mn != "ljmp") {
            todo.push_back(first_imm(i) & 0xFFFF);
        } else if (i.group(CS_GRP_JUMP) && i.mn == "jmp") {
            reach.clear();   // (a jump through a table or register)
            for (const auto& kv : f->at) reach.insert(kv.first);
            break;
        }
        if (!is_end(i.mn)) todo.push_back(static_cast<int64_t>(i.address) + i.size);
    }
    std::vector<const Insn*> ends;
    for (size_t k = 0; k < ins.size(); k++) {
        const Insn& i = ins[k];
        if ((i.mn == "ret" || i.mn == "retf") && reach.count(static_cast<int64_t>(i.address)) &&
            !jump_by_return_i(ins, static_cast<int64_t>(k), std::set<int64_t>()))   // (a jump through a pointer: its callee's DX)
            ends.push_back(&i);
    }
    bool jumped_on = false;
    for (const Insn& i : ins) {   // (a jump on to another function: its result)
        if (!reach.count(static_cast<int64_t>(i.address))) continue;
        if (i.mn == "jmp" || i.mn == "ljmp") {
            Target t = target_of(i);
            if (t.kind == Out || (t.kind == Place && t.place != place && !(std::get<0>(t.place) == sg && off <= std::get<1>(t.place) && std::get<1>(t.place) < end))) {
                if (!handed_on(i)) return false;
                jumped_on = true;
            }
        }
    }
    if (ends.empty() && !jumped_on) return false;
    if (f->saves_ax && gives_back_ax(ins, f->at, f->into, ends)) return false;   // (no result at all: AX as it came)
    // work items: an address, or "not seen" (none)
    std::vector<std::optional<int64_t>> work;
    std::set<int64_t> seen;
    for (const Insn* r : ends) {
        std::vector<int64_t> in = into_of(static_cast<int64_t>(r->address));
        if (in.empty()) work.push_back(std::nullopt);
        else for (int64_t x : in) work.push_back(x);
    }
    while (!work.empty()) {
        std::optional<int64_t> w = work.back();
        work.pop_back();
        if (!w || !f->at.count(*w)) return false;   // (reached from what is not seen: DX unknown)
        int64_t a = *w;
        if (seen.count(a)) continue;
        seen.insert(a);
        const Insn& p = ins[f->at.at(a)];
        if (p.group(CS_GRP_CALL)) {
            if (handed_on(p)) continue;
            return false;   // (a two-byte callee: DX as it was)
        }
        if (p.mn == "int" || p.mn == "into") continue;
        if (!p.regs_ok) return false;
        if (hits_dx(p.regs_write)) {
            if (p.mn == "pop" && f->saves_dx) return false;   // (DX given back as it came)
            continue;
        }
        if (a == off) return false;   // (DX as the caller had it)
        std::vector<int64_t> in = into_of(a);
        if (in.empty()) work.push_back(std::nullopt);
        else for (int64_t x : in) work.push_back(x);
    }
    return true;
}

// {address: bytes it returns} for every function described: its own (result_bytes), or for one that passes on a callee's
// result, the callee's -- four bytes passed on are four bytes (a near callee in its segment; any other call counts as two).
py::dict<int64_t, int64_t> results(const Description& d) {
    std::vector<int64_t> segorder;
    std::map<int64_t, std::vector<int64_t>> by_seg;
    for (int64_t va : d.names) {
        SegOff w = where(va);
        int64_t sg = std::get<0>(w), off = std::get<1>(w);
        if (!by_seg.count(sg)) segorder.push_back(sg);
        by_seg[sg].push_back(off);
    }
    std::vector<SegOff> order;
    std::map<SegOff, int64_t> own, voided;
    std::map<SegOff, py::list<PassOn>> passes;
    for (int64_t sg : segorder) {
        py::bytes code = d.segment_bytes.get(sg, py::bytes());
        std::vector<int64_t> offs = by_seg[sg];
        std::sort(offs.begin(), offs.end());
        for (size_t k = 0; k < offs.size(); k++) {
            int64_t off = offs[k];
            py::list<PassOn> p;
            py::list<int64_t> v;
            SegOff key{sg, off};
            if (!own.count(key)) order.push_back(key);
            own[key] = result_bytes(code, off, k + 1 < offs.size() ? offs[k + 1] : code.size(), &p, &v);
            passes[key] = p;
            if (!v.empty()) voided[key] = v[0];
        }
    }
    if (!voided.empty() && !d.images.empty()) {
        // (one with a way that sets no AX, whose result a caller reads all the same: a result)
        py::set<SegOff> vs;
        for (const auto& kv : voided) vs.add(kv.first);
        for (const SegOff& place : callers_read_ax(d, vs)) own[place] = voided[place];
    }
    std::map<SegOff, int64_t> size = own;
    for (int round = 0; round < 8; round++) {   // (wrappers of wrappers)
        bool changed = false;
        for (const SegOff& place : order) {
            for (const PassOn& t : passes[place]) {
                int64_t got;
                if (std::holds_alternative<std::tuple<py::str, int64_t>>(t)) {   // ("into", offset): the function it runs on in
                    int64_t tt = std::get<1>(std::get<std::tuple<py::str, int64_t>>(t));
                    std::optional<int64_t> inside_;
                    for (int64_t o : by_seg[std::get<0>(place)])
                        if (o <= tt && (!inside_ || o > *inside_)) inside_ = o;
                    got = inside_ ? (size.count({std::get<0>(place), *inside_}) ? size[{std::get<0>(place), *inside_}] : 0) : 0;
                    if (!got) continue;
                } else {
                    if (std::holds_alternative<int64_t>(t)) {
                        SegOff k2{std::get<0>(place), std::get<int64_t>(t)};
                        got = size.count(k2) ? size[k2] : 0;
                    } else {
                        got = 2;
                    }
                    got = got ? got : 2;
                }
                if (got > size[place]) {
                    size[place] = got;
                    changed = true;
                }
            }
        }
        if (!changed) break;
    }
    // (off unless CB_CALLER_RESULTS=1 until Windows 95 Setup has been booted with it. With it on, USER's C once stopped
    // Setup -- "not enough memory to run Setup". That is now thought not a wrong result but what the C it lets USER have did
    // to fixed segment 1: grown past the original's, the low memory Setup needs for its program was gone. build/module.py now
    // keeps a fixed code segment at the original's size; the milestone boot, with the rule on and off, is to confirm it.
    // Narrowed meanwhile to a callee that hands back a DX of its own on every way -- see _DxReturns)
    if (env_is("CB_CALLER_RESULTS", "1") && !d.images.empty()) {
        py::list<std::tuple<SegOff, SegOff>> edges;
        std::set<SegOff> read, handed;
        for (const SegOff& p : callers_read_dx(d, &edges))
            if (size.count(p) && size[p] < 4) read.insert(p);   // (what the callers read after it)
        for (const auto& e : edges)
            if (size.count(std::get<1>(e)) && size[std::get<1>(e)] < 4) handed.insert(std::get<1>(e));
        _DxReturns own_dx(d);
        // (all taken as four-byte, then each that does not hand back a DX of its own under that dropped, until none is left to
        // drop: a wrapper's DX is its callee's, and the callee is four-byte only through the wrapper's callers -- USER's
        // 25:107F `call 1:1292 / leave / ret 8`, 1292 handing back GDI's GetTextExtent)
        std::set<SegOff> base;
        for (const auto& kv : size) if (kv.second >= 4) base.insert(kv.first);
        std::set<SegOff> made = read;
        for (const SegOff& x : handed) made.insert(x);
        for (size_t round = 0; round < made.size() + 1; round++) {
            std::set<SegOff> four = base;
            for (const SegOff& x : made) four.insert(x);
            py::set<SegOff> four_py;
            for (const SegOff& x : four) four_py.add(x);
            std::set<SegOff> keep;
            for (const SegOff& p : read) if (own_dx.every_way(p, four_py)) keep.insert(p);
            for (const auto& e : edges) {
                const SegOff& caller = std::get<0>(e);
                const SegOff& callee = std::get<1>(e);
                if (four.count(caller) && handed.count(callee) && own_dx.every_way(callee, four_py)) keep.insert(callee);
            }
            std::set<SegOff> kept;
            for (const SegOff& x : keep) if (made.count(x)) kept.insert(x);
            if (kept == made) break;
            made = kept;
        }
        for (const SegOff& p : made) size[p] = 4;
    }
    py::dict<int64_t, int64_t> out;
    for (int64_t va : d.names) {
        SegOff w = where(va);
        out.set(va, size.count(w) ? size[w] : 0);
    }
    return out;
}

// The description line for a function at `va`.
py::str model_line(const Description& d, int64_t va, const py::str& name, const py::str& dflt_py) {
    string dflt = dflt_py.utf8();
    SegOff w = where(va);
    int64_t seg = std::get<0>(w), off = std::get<1>(w);
    py::bytes code = d.segment_bytes.get(seg, py::bytes());
    if (d.far_called.contains(SegOff{seg, off})) dflt = "far";   // (found later, called far: see describe16)
    // How the original calls it is the evidence (an export, a far fix-up: far); the nearest return of that kind then says what it
    // removes. With no such evidence, its own nearest return says both.
    std::optional<string> kind;
    int64_t pop;
    if (dflt == "far" || windows_far_prologue(code, off)) {
        pop = std::get<1>(returns_i(held(code), off, 600, string("far")));
        kind = "far";
    } else {
        auto r = returns_i(held(code), off, 600, std::nullopt);
        kind = std::get<0>(r);
        pop = std::get<1>(r);
    }
    // A function that removes its own arguments says so in its return (`retf 6`); one that ends in a plain return leaves them
    // to the caller -- unless it took them off by hand first (USER's IsChild: `pop ax; pop dx` its return address, `pop bx;
    // pop cx` its two arguments, `push dx; push ax` the return address back, then a plain `retf`).
    string kd = kind ? *kind : dflt;
    auto fwd = forwards(d, seg, off, py::str(kd));
    if (fwd) {
        pop = *fwd;
        if (kd == "far") kind = "far";
    }
    // (and what it took off by hand at its start comes on top of what its return removes: USER's call-hook dispatcher 1:8005
    // pops one word by hand, then `retf 8` -- five words in all)
    kd = kind ? *kind : dflt;
    int64_t hand = popped_by_hand(code, off, py::str(kd)).value_or(0);
    if (hand && pop >= 0) pop += hand;
    string conv = pop > 0 ? "__stdcall16" : pop == 0 ? "__cdecl16" : "__stdcall16";
    // (CB_FASTCALL=0 turns it off. A far one only without the Windows prolog: under /Gw, which loads DS through AX, C 8 says
    // "illegal far _fastcall function" -- without it, as in SUWIN, a far _fastcall takes AX, DX, BX as a near one does: measured,
    // `retf` / `retf n`)
    if (env_not("CB_FASTCALL", "0") && (kd == "near" || !windows_far_prologue(code, off)) && register_arguments(code, off)) conv = "__fastcall16";
    // what it returns, as its instructions show (where it runs to: the next function's start)
    auto got_result = d.results.get(va);
    int64_t result;
    if (!got_result) {   // (one found later: on its own)
        std::optional<int64_t> later;
        for (int64_t v : d.names) {
            SegOff wv = where(v);
            if (std::get<0>(wv) == seg && std::get<1>(wv) > off && (!later || std::get<1>(wv) < *later)) later = std::get<1>(wv);
        }
        py::list<PassOn> ps;
        result = result_bytes(code, off, later ? *later : code.size(), &ps, nullptr);
        for (const PassOn& t : ps) {   // a callee's result passed on: its size
            if (std::holds_alternative<std::tuple<py::str, int64_t>>(t)) continue;
            int64_t got;
            if (std::holds_alternative<int64_t>(t) && d.segment_bytes.contains(seg)) got = d.results.get(address(seg, std::get<int64_t>(t)), 0);
            else got = 2;
            result = std::max(result, got ? got : 2);
        }
    } else {
        result = *got_result;
    }
    return py::str("F\t" + hx(va) + "\t" + name.utf8() + "\t" + std::to_string(pop) + "\t" + conv + (kind ? *kind : dflt) +
                   (result ? "\t" + std::to_string(result) : string()));
}

// ---- PE (and PowerPC) ---------------------------------------------------------------------------------------------------
namespace {

// codebreaker.knowledge's three asks, each a thin call into symbols.api_arguments / symbols.ordinals
std::optional<int64_t> k_argument_bytes_win16(const py::str& api) { return symbols::api_arguments::of(api); }
std::optional<int64_t> k_argument_bytes_win32(const py::str& api, const py::str& module) { return symbols::api_arguments::stack_bytes(module, api); }
std::optional<int64_t> k_declared_argument_bytes(const py::str& name) { return symbols::api_arguments::header_bytes(name); }
std::optional<py::str> k_import_name(const py::str& module, int64_t ordinal) { return symbols::ordinals::identifier(module, ordinal); }

std::string abspath_s(const py::str& p) { return py::os::path::abspath(p).utf8(); }

}  // namespace

// {va: name} -- an export's own name, else sub_<rva>. A name also used by an import of this module stays with the import
// (the call has to bind to it).
py::dict<int64_t, py::str> function_names(const py::str& module_path, const py::list<int64_t>& funcs) {
    formats::pe::PEFile pe = formats::pe::open_pe(module_path);
    py::set<py::str> imported;
    for (const py::str& k : build::watcom::import_map(module_path).keys()) imported.add(k);
    std::vector<std::pair<py::str, int64_t>> exports;
    for (const auto& kv : pe.export_addrs().items()) exports.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(exports.begin(), exports.end(), [](const auto& a, const auto& b) { return a.first < b.first || (a.first == b.first && a.second < b.second); });
    py::dict<int64_t, py::str> by_rva;
    for (const auto& nm_rva : exports)
        if (c_name(nm_rva.first) && !imported.contains(nm_rva.first)) by_rva.setdefault(nm_rva.second, nm_rva.first);
    py::dict<int64_t, py::str> out;
    for (int64_t va : funcs) {
        int64_t rva = va - static_cast<int64_t>(pe.image_base);
        out.set(va, by_rva.get(rva, py::str("sub_" + HX(rva, 6))));
    }
    return out;
}

// {address: linker name} for the functions a module exports under a name that is not a C name -- a C++ function's decorated
// name. In the C text such a function is sub_<address>; this is the symbol it must be linked as, and symbols/demangle.py makes
// it a declaration. Addresses are as in the description (16-bit: address(segment, offset)).
py::dict<int64_t, py::str> decorated_names(const py::str& module_path) {
    py::dict<int64_t, py::str> out;
    auto plain = [](const py::str& s) { return py::re::fullmatch(py::str("[A-Za-z_]\\w*"), s).has_value(); };   // plain.match(nm): `\Z` ends it
    auto lane = base::corpus::lane(module_path);
    if (lane && *lane == "ne") {
        formats::ne::NEFile nef(module_path);
        py::dict<int64_t, formats::ne::Entry> entries;
        for (const auto& e : nef.entries())
            if (e.kind == "FIXED" || e.kind == "MOVEABLE") entries.set(e.ordinal, e);
        auto names = nef.resident_names();
        auto non = nef.nonresident_names();
        std::vector<std::tuple<py::str, int64_t>> all;
        for (size_t k = 1; k < names.vec().size(); k++) all.push_back(names.vec()[k]);
        for (size_t k = 1; k < non.vec().size(); k++) all.push_back(non.vec()[k]);
        for (const auto& [nm, ordv] : all) {
            if (entries.contains(ordv) && !plain(nm)) {
                const formats::ne::Entry& e = entries.at(ordv);
                out.setdefault(address(e.seg.value_or(0), e.offset.value_or(0)), nm);
            }
        }
    } else {
        formats::pe::PEFile pe = formats::pe::open_pe(module_path);
        std::vector<std::pair<py::str, int64_t>> exports;
        for (const auto& kv : pe.export_addrs().items()) exports.emplace_back(std::get<0>(kv), std::get<1>(kv));
        std::sort(exports.begin(), exports.end(), [](const auto& a, const auto& b) { return a.first < b.first || (a.first == b.first && a.second < b.second); });
        for (const auto& nm_rva : exports)
            if (!plain(nm_rva.first)) out.setdefault(static_cast<int64_t>(pe.image_base) + nm_rva.second, nm_rva.first);
    }
    return out;
}

// {symbol: name} for a 16-bit module's imports by number: the symbol the C text and the link use (`USER_91`) and the
// function's real name (`GetDlgItem`), where the exporting module or its definition file gives one that is a C name.
py::dict<py::str, py::str> import_names16(const formats::ne::NEFile& nef) {
    py::list<py::str> refs = nef.module_refs();
    py::dict<py::str, std::optional<py::str>> got;
    for (const auto& s : nef.segments()) {
        for (const auto& r : nef.relocations(s)) {
            if (r.kind == "IMPORTORD" && r.module && 0 < *r.module && *r.module <= refs.size()) {
                const py::str& mod = refs[*r.module - 1];
                py::str symbol = ident_py(py::str(mod.utf8() + "_" + std::to_string(r.ordinal.value_or(0))));
                if (!got.contains(symbol)) got.set(symbol, k_import_name(mod, r.ordinal.value_or(0)));
            }
        }
    }
    py::dict<py::str, py::str> out;
    for (const auto& kv : got.items()) {
        const auto& name = std::get<1>(kv);
        if (name && !name->empty()) out.set(std::get<0>(kv), *name);
    }
    return out;
}

// Addresses of a PE module's functions. PowerPC images carry a table of them (.pdata: one record per function, its start and
// length); for x86 they are found by following the code.
py::list<int64_t> function_starts(const py::str& module_path) {
    if (formats::pe::open_pe(module_path).machine == formats::powerpc_image::MACHINE) return formats::powerpc_image::read(module_path)->functions;
    return verify::lanes::win32::functions(py::str("pe"), module_path);
}

namespace {

// A file of at least `size` zero bytes in `outdir`, for describing memory the file does not hold.
std::string zeros_file(const py::str& outdir, int64_t size) {
    py::os::makedirs(outdir, 0777, true);   // (the first time, before anything else is there)
    py::str path = py::os::path::join(outdir, py::str("zeros.bin"));
    std::error_code ec;
    bool have = py::os::path::isfile(path);
    if (!have || static_cast<int64_t>(std::filesystem::file_size(std::filesystem::u8path(path.utf8()), ec)) < size) {
        py::file f = py::open(path, py::str("wb"));
        f.write(py::bytes::zeros(std::max<int64_t>(size, 1 << 20)));
        f.close();
    }
    return abspath_s(path);
}

// Whether the register a call at fn.insns[i] goes through was last loaded, earlier in the function, with an import's address
// (`mov esi, [slot]`) or a constant (a function's address): the core then calls that function, with its own declaration.
bool register_holds_known(const analyze::image::Image& img, const analyze::image::Function& fn, int64_t i, const py::str& reg) {
    for (int64_t q = i - 1; q >= 0; q--) {
        const analyze::image::Ins& p = *fn.insns[q];
        if (!p.wr.contains(reg)) continue;
        if (p.mn == "mov" && p.ops.size() == 2) {
            const analyze::image::Op& src = p.ops[1];
            if (src.kind == "i") return true;
            if (src.kind == "m" && !src.base && !src.index) return img.imports.contains(src.disp);   // (an absolute address: no base, no index)
        }
        return false;
    }
    return false;
}

// `O <site> <function> <bytes> <model>` for each call through a pointer (a register, a variable; not an import's slot): the
// bytes the code pushed just before it, and who takes them off -- the caller (`add esp, N` right after: __cdecl) or the callee
// (__stdcall). The core otherwise calls through a pointer with no arguments at all: msjava.dll's `pcVar7 = cond ?
// CoTaskMemAlloc : CoTaskMemRealloc; (*pcVar7)();`, the original having pushed one or two, so the callee took off what the
// rebuild never pushed.
std::vector<string> pointer_calls32(const py::str& module_path, const py::list<int64_t>& funcs) {
    analyze::image::ImageP img;
    try {
        img = analyze::image::load(module_path);
    } catch (const std::exception&) {
        return {};
    }
    std::set<int64_t> wanted;
    for (int64_t f : funcs) wanted.insert(f);
    std::vector<string> out;
    for (const auto& fnp : img->each()) {
        const analyze::image::Function& fn = *fnp;
        if (!wanted.count(fn.start)) continue;
        // (the pushes a function opens with save registers: never a call's arguments)
        int64_t opening = 0;
        int64_t n = fn.insns.size();
        while (opening < n && fn.insns[opening]->mn == "push") opening++;
        for (int64_t i = 0; i < n; i++) {
            const analyze::image::Ins& ins = *fn.insns[i];
            if (!ins.flow || *ins.flow != "call" || ins.target || analyze::image::destination(*img, ins)) continue;
            if (!ins.ops.empty() && ins.ops[0].kind == "r" && register_holds_known(*img, fn, i, analyze::image::family(ins.ops[0].name)))
                continue;   // (`mov esi, [lstrcatA's slot] / call esi`: the core knows it)
            const analyze::image::Ins* nxt = i + 1 < n ? fn.insns[i + 1].get() : nullptr;
            if (nxt && nxt->mn == "add" && nxt->ops.size() == 2 && nxt->ops[0].kind == "r" && analyze::image::family(nxt->ops[0].name) == "esp" &&
                nxt->ops[1].kind == "i") {
                // the caller takes them off: exactly as many as it says
                out.push_back("O\t" + hx(ins.addr) + "\t" + hx(fn.start) + "\t" + std::to_string(nxt->ops[1].value) + "\t__cdecl");
                continue;
            }
            int64_t pushed = 0, k = i - 1;
            while (k >= std::max(opening, i - 40)) {
                const analyze::image::Ins& p = *fn.insns[k];
                if (p.mn == "push") {
                    pushed += 4;
                } else if (p.flow || eq_any(p.mn.utf8(), {"pop", "leave", "enter", "pushal", "popal"}) || p.wr.contains(py::str("esp"))) {
                    break;
                }
                k -= 1;
            }
            if (pushed) {
                // (a method called through its object's table -- `mov ecx, esi / call [eax+8]` -- takes the object in ECX beside what
                // was pushed: __thiscall, or it is lost)
                bool self = false;
                for (int64_t q = i - 1; q >= std::max<int64_t>(0, i - 12); q--) {
                    const analyze::image::Ins& p = *fn.insns[q];
                    if (p.flow) break;
                    if (p.wr.contains(py::str("ecx"))) {
                        self = true;
                        break;
                    }
                }
                out.push_back("O\t" + hx(ins.addr) + "\t" + hx(fn.start) + "\t" + std::to_string(pushed) + "\t" + (self ? "__thiscall" : "__stdcall"));
            }
        }
    }
    return out;
}

}  // namespace

// Write the description of a PE module for the native program: its processor, where its bytes sit in memory, and the facts
// about `funcs` (addresses) and its imports. -> (path of the description, {va: name})
std::tuple<py::str, py::dict<int64_t, py::str>> describe32(const py::str& module_path, const py::list<int64_t>& funcs, const py::str& outdir) {
    formats::pe::PEFile pe = formats::pe::open_pe(module_path);
    std::optional<py::str> arch = _arch(pe.machine);
    if (!arch || arch->empty()) {
        char b[32];
        snprintf(b, sizeof b, "%#llx", static_cast<unsigned long long>(pe.machine));
        throw py::ValueError(py::os::path::basename(module_path).utf8() + ": no language for machine type " + b);
    }
    bool x86 = arch->startswith(py::str("x86:"));
    py::dict<int64_t, py::str> names;
    if (x86) {
        names = function_names(module_path, funcs);
    } else {   // PowerPC: what run-time type information names
        symbols::names::Named own = symbols::names::powerpc_names(module_path);
        for (int64_t va : funcs) names.set(va, own.contains(va) ? std::get<0>(own.at(va)) : py::str("sub_" + HX(va, 8)));
    }
    std::vector<string> lines;
    lines.push_back("arch\t" + arch->utf8());
    string mpath = abspath_s(module_path);
    if (!x86) {
        // PowerPC: the image says where its bytes sit, which stubs are imports, and which register save/restore runs are part
        // of their callers (formats/powerpc_image.py)
        auto img = formats::powerpc_image::read(module_path);
        for (const auto& span : img->spans)
            lines.push_back("bytes\t" + hx(std::get<0>(span)) + "\t" + mpath + "\t" + hx(std::get<1>(span)) + "\t" + hx(std::get<2>(span)));
        std::vector<std::pair<int64_t, py::str>> imps;
        for (const auto& kv : img->imports.items()) imps.emplace_back(std::get<0>(kv), std::get<1>(kv));
        std::sort(imps.begin(), imps.end(), [](const auto& a, const auto& b) { return a.first < b.first || (a.first == b.first && a.second < b.second); });
        for (const auto& va_name : imps) lines.push_back("X\t" + hx(va_name.first) + "\t" + va_name.second.utf8() + "\t-1");
        std::vector<std::pair<int64_t, py::str>> inl;
        for (const auto& kv : img->inline_.items()) inl.emplace_back(std::get<0>(kv), std::get<1>(kv));
        std::sort(inl.begin(), inl.end(), [](const auto& a, const auto& b) { return a.first < b.first || (a.first == b.first && a.second < b.second); });
        for (const auto& va_name : inl) lines.push_back("F\t" + hx(va_name.first) + "\t" + va_name.second.utf8() + "\t-1\tinline");
    } else {
        for (const auto& sec : pe.sections) {
            int64_t size = std::min(sec.rawsize, sec.vsize ? sec.vsize : sec.rawsize);
            int64_t image_base = static_cast<int64_t>(pe.image_base);
            if (size && sec.rawptr)
                lines.push_back("bytes\t" + hx(image_base + sec.vaddr) + "\t" + mpath + "\t" + hx(sec.rawptr) + "\t" + hx(size));
            // The part of a section the file does not hold (.bss; a section's tail past its raw size) is zeros when the module
            // is loaded: described as such, its globals are the image's, and their sizes reach the C (undescribed, every one
            // was declared 4 bytes: setup32's `dec word ptr [40e36c]` stored over the word beside it).
            if ((sec.vsize ? sec.vsize : 0) > size)
                lines.push_back("bytes\t" + hx(image_base + sec.vaddr + size) + "\t" + zeros_file(outdir, sec.vsize - size) + "\t0\t" + hx(sec.vsize - size));
        }
    }
    std::shared_ptr<symbols::api_arguments::_Mod> mod;
    if (x86) mod = std::make_shared<symbols::api_arguments::_Mod>(module_path);
    // Functions that take an argument in ECX and nothing else in a register: a C++ member function's object. Read from the code
    // (ECX is used before anything is put in it), and said to the decompiler, which otherwise shows the object as a variable
    // nobody set -- in the function and, worse, not passed at all by its callers.
    std::set<int64_t> in_ecx, in_edx;
    if (x86) {
        try {
            auto img = analyze::image::load(module_path);
            auto found = analyze::conventions::functions(*img);
            for (const auto& kv : found->items()) {
                const auto& fact = std::get<1>(kv);
                if (fact.convention && *fact.convention == "thiscall") in_ecx.insert(std::get<0>(kv));
                if (fact.convention && *fact.convention == "fastcall") in_edx.insert(std::get<0>(kv));   // ECX and EDX
            }
        } catch (const std::exception& e) {
            // (said, not swallowed: without it no function takes ECX or EDX, and every C++ method of the module loses it --
            // msjava.dll's analysis stopped on a function of padding alone, and 116 of its functions passed EnterCriticalSection
            // garbage)
            const py::Exception* pe_ = dynamic_cast<const py::Exception*>(&e);
            fprintf(stderr, "describe32: %s: calling conventions not found (%s: %s); no function takes ECX or EDX\n",
                    py::os::path::basename(module_path).utf8().c_str(), pe_ ? pe_->type() : "Exception", e.what());
            in_ecx.clear();
            in_edx.clear();
        }
    }
    py::set<py::str> ecx_names;
    for (int64_t va : funcs)
        if (in_ecx.count(va)) ecx_names.add(names.at(va));
    IN_ECX.set(module_path, ecx_names);
    for (int64_t va : funcs) {
        std::optional<int64_t> pop;
        if (x86) {
            try {
                pop = symbols::api_arguments::_scan(*mod, va - static_cast<int64_t>(pe.image_base), 4);
            } catch (const py::ImportError&) {
                throw;   // (no capstone is not "no pop count")
            } catch (const std::exception&) {
                pop = std::nullopt;
            }
        }
        lines.push_back("F\t" + hx(va) + "\t" + names.at(va).utf8() + "\t" + std::to_string(pop ? *pop : -1) +
                        (in_ecx.count(va) && pop ? "\t__thiscall" : in_edx.count(va) && pop ? "\t__fastcall" : ""));
    }
    // An import by number is called by its real name when the module that exports it (or that module's definition file) gives
    // it one that is a C name nothing else here already has; how many bytes of arguments it takes is read from the export with
    // that number.
    py::dict<py::str, std::optional<std::tuple<py::str, int64_t>>> taken;
    if (x86) {
        for (const py::str& k : build::watcom::import_map(module_path).keys()) taken.set(k, std::nullopt);
        for (int64_t va : funcs) taken.set(names.at(va), std::nullopt);
    }
    py::dict<py::str, std::tuple<py::str, int64_t, std::optional<int64_t>>> numbered;
    BY_NUMBER.set(module_path, numbered);
    std::vector<std::pair<uint64_t, std::tuple<py::str, py::str>>> imports;
    if (x86)
        for (const auto& kv : pe.imports().items()) imports.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(imports.begin(), imports.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    for (const auto& slot_import : imports) {
        uint64_t slot = slot_import.first;
        const py::str& dll = std::get<0>(slot_import.second);
        const py::str& name = std::get<1>(slot_import.second);
        py::str stem = py::str(splitext_root(dll.utf8())).upper();
        if (name.empty() || name.startswith(py::str("#"))) {
            int64_t number = (!name.empty() && name.slice_from(1).isdigit()) ? py::int_parse(name.slice_from(1), 10) : 0;
            std::optional<py::str> real = k_import_name(stem, number);
            bool take = false;
            if (real && !real->empty()) {
                std::tuple<py::str, int64_t> want(stem, number);
                if (!taken.contains(*real)) taken.set(*real, std::optional<std::tuple<py::str, int64_t>>(want));
                auto cur = taken.at(*real);
                take = cur && *cur == want;
            }
            if (take) {
                std::optional<int64_t> pop = k_declared_argument_bytes(*real);
                if (!pop) pop = k_argument_bytes_win32(name, stem);
                lines.push_back("I\t" + hx(static_cast<int64_t>(slot)) + "\t" + real->utf8() + "\t" + std::to_string(pop ? *pop : -1));
                numbered.set(*real, std::make_tuple(stem, number, pop));
            } else {
                // no name for it -- but the module that exports it still says how many bytes of arguments it removes, and without
                // that the call is recovered with none
                std::optional<int64_t> pop = !name.empty() ? k_argument_bytes_win32(name, stem) : std::nullopt;
                py::str shown = name.empty() ? py::str("") : name.slice_from(1);
                if (name.empty()) shown = py::str("0");   // ((name or "#0")[1:])
                py::str id = ident_py(py::str(stem.utf8() + "_ord_" + shown.utf8()));
                lines.push_back("I\t" + hx(static_cast<int64_t>(slot)) + "\t" + id.utf8() + "\t" + std::to_string(pop ? *pop : -1));
                numbered.set(id, std::make_tuple(stem, number, pop));
            }
            continue;
        }
        std::optional<int64_t> pop = x86 ? k_argument_bytes_win32(name, stem) : std::nullopt;
        lines.push_back("I\t" + hx(static_cast<int64_t>(slot)) + "\t" + ident(name.utf8()) + "\t" + std::to_string(pop ? *pop : -1));
    }
    // (CB_POINTER_CALLS=1: say what each call through a pointer was pushed. Off: on the 32-bit sample it fixed no program and
    // made msjava.dll worse -- 280 unverified to 377 -- where a pointer call's pushes are not all its arguments; to be made
    // right before it is on)
    if (x86 && env_is("CB_POINTER_CALLS", "1"))
        for (const string& l : pointer_calls32(module_path, funcs)) lines.push_back(l);
    py::os::makedirs(outdir, 0777, true);
    py::str path = py::os::path::join(outdir, py::str("description.txt"));
    py::file f = py::open(path, py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));
    string text;
    for (const string& l : lines) text += l + "\n";
    f.write(py::str(text));
    f.close();
    return {path, names};
}

// ---- NE: the writers -------------------------------------------------------------------------------------------------
namespace {

// KERNEL's exports that are plain numbers, the same in every protected-mode Windows (by ordinal, and by name): a huge
// pointer's selector step
std::optional<int64_t> kernel_constant(const string& name) {
    if (name == "KERNEL_113" || name == "__AHSHIFT") return 3;
    if (name == "KERNEL_114" || name == "__AHINCR") return 8;
    return std::nullopt;
}
string bare(const string& name) {
    string up = py::str(name).upper().utf8();
    if (starts(up, "KERNEL_")) return up;
    size_t k = up.rfind('.');
    return k == string::npos ? up : up.substr(k + 1);
}

[[noreturn]] void struct_error(const char* what) { throw py::Exception(string("struct.error: ") + what); }
void put16(string& img, int64_t off, int64_t v) {
    if (v < 0 || v > 0xFFFF) struct_error("ushort format requires 0 <= number <= 65535");
    img[static_cast<size_t>(off)] = static_cast<char>(v & 0xFF);
    img[static_cast<size_t>(off) + 1] = static_cast<char>((v >> 8) & 0xFF);
}
int64_t get16(const string& img, int64_t off) {
    return static_cast<unsigned char>(img[static_cast<size_t>(off)]) | (static_cast<unsigned char>(img[static_cast<size_t>(off) + 1]) << 8);
}
void put32(string& img, int64_t off, int64_t v) {
    for (int k = 0; k < 4; k++) img[static_cast<size_t>(off) + k] = static_cast<char>((v >> (8 * k)) & 0xFF);
}
int64_t get32(const string& img, int64_t off) {
    int64_t v = 0;
    for (int k = 3; k >= 0; k--) v = (v << 8) | static_cast<unsigned char>(img[static_cast<size_t>(off) + k]);
    return v;
}

}  // namespace

// Write the description (and the relocated image it points at). -> Description
std::shared_ptr<Description> describe16(const py::str& module_path, const py::str& outdir) {
    held_clear();   // (another module's segments: let them go)
    formats::ne::NEFile nef(module_path);
    py::list<formats::ne::Segment> segs = nef.segments();
    py::dict<int64_t, formats::ne::Entry> entries;
    for (const auto& e : nef.entries())
        if (e.kind == "FIXED" || e.kind == "MOVEABLE") entries.set(e.ordinal, e);
    py::list<py::str> refs = nef.module_refs();
    std::map<int64_t, string> images;
    for (const auto& s : segs) {
        string data = s.file_off ? nef.data.slice(s.file_off, s.file_off + s.length).raw() : string();
        int64_t size = std::max<int64_t>(static_cast<int64_t>(data.size()), std::min<int64_t>(s.minalloc, 0x10000));
        data.append(static_cast<size_t>(size - static_cast<int64_t>(data.size())), '\0');
        images[s.idx] = data;
    }
    py::dict<py::str, int64_t> imports;   // name -> stand-in address
    py::list<py::str> order;
    auto extern_ = [&](const py::str& name) {
        if (!imports.contains(name)) {
            imports.set(name, (EXTERN_SELECTOR << 16) + 16 * order.size());
            order.append(name);
        }
        return imports.at(name);
    };
    py::dict<py::str, int64_t> selectors;   // import name -> its own made-up selector
    py::dict<py::str, py::str> used_as;     // import name -> "segment" | "offset"
    auto selector_of = [&](const py::str& name) {
        if (!selectors.contains(name)) selectors.set(name, IMPORT_SELECTOR + selectors.size());
        return selectors.at(name);
    };

    for (const auto& s : segs) {
        string& img = images[s.idx];
        for (const auto& r : nef.relocations(s)) {
            const string& kind = r.kind.utf8();
            int64_t tsel, toff;
            string nm;
            bool is_import = false;
            if (kind == "INTERNAL") {
                int64_t tseg;
                if (r.seg.value_or(0) == 0xFF) {   // by entry-table ordinal
                    if (!entries.contains(r.entry.value_or(0))) continue;
                    const formats::ne::Entry& e = entries.at(r.entry.value_or(0));
                    tseg = e.seg.value_or(0);
                    toff = e.offset.value_or(0);
                } else {
                    tseg = r.seg.value_or(0);
                    toff = r.entry.value_or(0);
                }
                tsel = selector(tseg);
            } else if (kind == "IMPORTORD" || kind == "IMPORTNAME") {
                int64_t module = r.module.value_or(0);
                string mod = (0 < module && module <= refs.size()) ? refs[module - 1].utf8() : string("?");
                nm = kind == "IMPORTORD" ? mod + "_" + std::to_string(r.ordinal.value_or(0)) : nef.imported_name(r.name_off.value_or(0)).utf8();
                int64_t a = extern_(ident_py(py::str(nm)));
                tsel = a >> 16;
                toff = a & 0xFFFF;
                is_import = true;
            } else {
                continue;   // floating-point emulator fixups
            }
            string nid = is_import ? ident(nm) : string();
            // Where the same target is needed in several places the file chains them: each place holds the offset of the next,
            // ending at 0xFFFF. An "additive" record is one place.
            int64_t off = r.offset, hops = 0;
            int64_t len = static_cast<int64_t>(img.size());
            while (off != 0xFFFF && off + 1 < len && hops < 0x10000) {
                int64_t nxt = get16(img, off);
                int64_t src = r.src;
                int64_t add = r.additive ? nxt : 0;
                if (src == 3 && off + 3 < len) {   // far pointer: offset, selector
                    put16(img, off, (toff + add) & 0xFFFF);
                    put16(img, off + 2, tsel);
                } else if (src == 2) {   // selector only
                    // an imported selector is a segment of its own: what is read through it is told apart, and named by its
                    // import (not one shared made-up selector)
                    int64_t sel;
                    if (is_import) {
                        sel = selector_of(py::str(nid));
                        used_as.set(py::str(nid), py::str("segment"));
                    } else {
                        sel = tsel;
                    }
                    put16(img, off, sel);
                } else if (src == 5 && is_import && kernel_constant(bare(nm))) {
                    // a number that is the same in every protected-mode Windows (__AHSHIFT 3, __AHINCR 8: a huge pointer's step):
                    // the number itself -- a made-up one would be folded into the arithmetic as if it were the value
                    put16(img, off, (*kernel_constant(bare(nm)) + add) & 0xFFFF);
                } else if (src == 5 && is_import) {
                    // an import's offset alone: a value KERNEL exports (__0040H, __WINFLAGS ...), which the loader writes in. A value
                    // of its own, so it is told apart
                    put16(img, off, (selector_of(py::str(nid)) + add) & 0xFFFF);
                    used_as.setdefault(py::str(nid), py::str("offset"));
                } else if (src == 5) {   // offset only
                    put16(img, off, (toff + add) & 0xFFFF);
                } else if (src == 0) {   // low byte
                    img[static_cast<size_t>(off)] = static_cast<char>((toff + add) & 0xFF);
                }
                if (r.additive) break;
                off = nxt;
                hops += 1;
            }
        }
    }

    py::os::makedirs(outdir, 0777, true);
    py::str blob_py = py::os::path::abspath(py::os::path::join(outdir, py::str("describe.bin")));
    string blob = blob_py.utf8();
    std::vector<string> lines;
    lines.push_back("arch\t" + ARCH.utf8());
    int64_t pos = 0;
    {
        py::file f = py::open(blob_py, py::str("wb"));
        for (const auto& s : segs) {
            const string& img = images[s.idx];
            if (img.empty()) continue;
            f.write(py::bytes(img));
            lines.push_back("bytes\t" + hx(address(s.idx, 0)) + "\t" + blob + "\t" + hx(pos) + "\t" + hx(static_cast<int64_t>(img.size())));
            pos += static_cast<int64_t>(img.size());
        }
        f.close();
    }
    int64_t data_seg = nef.h.autodata_seg;
    if (data_seg) lines.push_back("reg\tDS\t" + hx(selector(data_seg)));
    py::dict<SegOff, py::str> exported;
    {
        auto names1 = nef.resident_names();
        auto names2 = nef.nonresident_names();
        std::vector<std::tuple<py::str, int64_t>> all;
        for (size_t k = 1; k < names1.vec().size(); k++) all.push_back(names1.vec()[k]);
        for (size_t k = 1; k < names2.vec().size(); k++) all.push_back(names2.vec()[k]);
        for (const auto& [nm, ordv] : all) {
            if (entries.contains(ordv) && c_name(nm)) {
                const formats::ne::Entry& e = entries.at(ordv);
                exported.setdefault(SegOff{e.seg.value_or(0), e.offset.value_or(0)}, nm);
            }
        }
    }
    py::dict<int64_t, py::str> names;
    py::dict<int64_t, SegOff> where_map;
    std::vector<std::tuple<int64_t, py::str, py::str>> funcs;
    // A far call whose offset the linker wrote and whose selector the loader fills in (a fix-up of the selector word alone)
    // calls a far function: `call far seg:off` in the relocated code.
    std::map<int64_t, int64_t> code_sel;
    for (const auto& s : segs)
        if (s.kind == "CODE") code_sel[selector(s.idx)] = s.idx;
    py::set<SegOff> far_called;
    for (const auto& s : segs) {
        if (s.kind != "CODE") continue;
        const string& img = images[s.idx];
        size_t i = img.find('\x9a');
        while (i != string::npos && static_cast<int64_t>(i) < static_cast<int64_t>(img.size()) - 4) {
            int64_t o = get16(img, static_cast<int64_t>(i) + 1), sl = get16(img, static_cast<int64_t>(i) + 3);
            if (code_sel.count(sl)) far_called.add(SegOff{code_sel[sl], o});
            i = img.find('\x9a', i + 1);
        }
    }
    {
        std::vector<std::pair<SegOff, py::str>> found;
        for (const auto& kv : functions(nef).items()) found.emplace_back(std::get<0>(kv), std::get<1>(kv));
        std::sort(found.begin(), found.end(), [](const auto& a, const auto& b) { return a.first < b.first || (a.first == b.first && a.second < b.second); });
        for (const auto& kv : found) {
            int64_t seg = std::get<0>(kv.first), off = std::get<1>(kv.first);
            py::str dist = kv.second;
            if (far_called.contains(kv.first)) dist = py::str("far");
            int64_t va = address(seg, off);
            py::str nm = exported.get(kv.first, py::str(""));
            if (nm.empty()) nm = py::str("sub_" + std::to_string(seg) + "_" + HX(off, 4));
            if (imports.contains(nm)) nm = py::str("sub_" + std::to_string(seg) + "_" + HX(off, 4));
            names.set(va, nm);
            where_map.set(va, kv.first);
            funcs.emplace_back(va, nm, dist);
        }
    }
    auto d = std::make_shared<Description>(py::str(""), names, where_map, order);
    d->far_called = far_called;   // (seg, off) a far call reaches
    d->import_names = import_names16(nef);   // symbol -> the import's real name
    for (const auto& kv : selectors.items()) {   // made-up value -> (import symbol, its selector or its offset)
        const py::str& name = std::get<0>(kv);
        d->selector_imports.set(std::get<1>(kv), std::make_tuple(name, used_as.get(name, py::str("segment"))));
    }
    for (const auto& s : segs)
        if (s.kind == "CODE") d->code.set(s.idx, s.length);   // segment -> bytes of code
    for (const auto& s : segs) d->images.set(s.idx, py::bytes(images[s.idx]));   // segment -> its bytes, relocated
    d->data_segment = data_seg ? data_seg : 0;
    // (as the loader has them: the stored bytes, then zeros to the size it allocates -- a function's last instruction can end in
    // them: `retf 4` stored as CA 04, its 00 not stored)
    for (const auto& s : segs) {
        if (s.kind == "CODE" && s.file_off) {
            int64_t alloc = std::min<int64_t>(s.minalloc ? s.minalloc : 0x10000, 0x10000);
            string b = nef.data.slice(s.file_off, s.file_off + s.length).raw();
            b.append(static_cast<size_t>(std::max<int64_t>(0, alloc - s.length)), '\0');
            d->segment_bytes.set(s.idx, py::bytes(b));
        }
    }
    d->results = results(*d);
    d->inlined = py::set<int64_t>();   // helpers made part of their callers
    std::map<int64_t, std::vector<int64_t>> starts16;
    for (const auto& f : funcs) {
        SegOff w = where_map.at(std::get<0>(f));
        starts16[std::get<0>(w)].push_back(std::get<1>(w));
    }
    for (const auto& f : funcs) {
        int64_t va = std::get<0>(f);
        const py::str& nm = std::get<1>(f);
        const py::str& dist = std::get<2>(f);
        SegOff w = where_map.at(va);
        int64_t seg = std::get<0>(w), off = std::get<1>(w);
        std::optional<int64_t> later;
        for (int64_t o : starts16[seg]) if (o > off && (!later || o < *later)) later = o;
        if (dist == "near" && !exported.contains(w) && env_not("CB_INLINE_HELPERS", "0") &&
            register_helper(d->segment_bytes.get(seg, py::bytes()), off, later)) {
            d->inlined.add(va);
            lines.push_back("F\t" + hx(va) + "\t" + nm.utf8() + "\t-1\tinline");
            continue;
        }
        lines.push_back(model_line(*d, va, nm, dist).utf8());
    }
    if (!getenv_s("CB_NO_CALL_OVERRIDES") || getenv_s("CB_NO_CALL_OVERRIDES")->empty())
        for (const py::str& l : call_overrides(*d)) lines.push_back(l.utf8());
    for (const py::str& l : jump_tables(*d)) lines.push_back(l.utf8());
    // (each return that is a jump -- jump_by_return -- is a call through the address pushed, followed by a return: the
    // decompiler's CALL_RETURN flow override, `R <address>`)
    for (int64_t va : return_jumps(*d)) lines.push_back("R\t" + hx(va));
    for (const py::str& nm : order) {   // Windows API: far, and the callee removes its arguments --
        std::optional<int64_t> size = k_argument_bytes_win16(nm);   // how many bytes is read from the module that exports it
        lines.push_back("X\t" + hx(imports.at(nm)) + "\t" + nm.utf8() + "\t" + std::to_string(size ? *size : -1) + "\t__stdcall16far");
    }
    for (const string& l : lines) d->lines.append(py::str(l));
    d->write(py::os::path::join(outdir, py::str("description.txt")));
    return d;
}

// ---- Virtual device drivers (LE) ---------------------------------------------------------------------------------------

// Describe one LE image (`lf`, from formats.le) to the decompiler. -> Description
//
// A driver's objects are relocatable, so each is given an address of its own and the fix-ups the loader would apply are
// applied: a 32-bit reference to another place in the image gets that place's address. Ring-0 code calls the system and other
// drivers with `int 20h` followed by a service number -- six bytes the loader rewrites into a call; they are rewritten here into
// a call through a pointer that stands for that service, so the decompiler sees a named call instead of an interrupt and four
// bytes of nonsense.
std::shared_ptr<Description> describe_le(const formats::le::LEFile& lf, const py::str& name, const py::str& outdir) {
    std::map<int64_t, string> images;
    for (const auto& o : lf.objects) {
        string b = lf.object_bytes(o).raw();
        b.append(static_cast<size_t>(std::max<int64_t>(0, o.vsize - static_cast<int64_t>(b.size()))), '\0');
        images[o.idx] = b;
    }
    std::set<int64_t> execs;
    for (const auto& o : lf.objects)
        if (o.exec) execs.insert(o.idx);
    std::set<int64_t> starts_;
    py::dict<formats::le::Place, formats::le::Fixup> fixups;
    try {
        fixups = lf.fixups();
    } catch (const std::exception&) {
        fixups = py::dict<formats::le::Place, formats::le::Fixup>();
    }
    for (const auto& kv : fixups.items()) {
        int64_t obj = std::get<0>(std::get<0>(kv)), off = std::get<1>(std::get<0>(kv));
        auto it = images.find(obj);
        if (it == images.end()) continue;
        string& img = it->second;
        if (off < 0 || off + 4 > static_cast<int64_t>(img.size())) continue;
        const auto* fx = std::get_if<formats::le::InternalFixup>(&std::get<1>(kv));
        if (!fx || !fx->target || !images.count(fx->obj)) continue;
        int64_t target = le_address(fx->obj, *fx->target);
        if (fx->srctype == 7) {   // 32-bit offset
            put32(img, off, target & 0xFFFFFFFF);
        } else if (fx->srctype == 8) {   // 32-bit self-relative (a call or jump)
            put32(img, off, (target - (le_address(obj, off) + 4)) & 0xFFFFFFFF);
            if (execs.count(fx->obj)) starts_.insert(target);
        }
    }
    // service calls: CD 20 <service:16> <device:16>
    std::map<std::pair<int64_t, int64_t>, int64_t> services;
    std::vector<std::pair<int64_t, int64_t>> order;
    for (int64_t obj : execs) {
        string& img = images[obj];
        size_t i = img.find("\xCD\x20", 0, 2);
        while (i != string::npos && i + 6 <= img.size()) {
            int64_t svc = get16(img, static_cast<int64_t>(i) + 2), dev = get16(img, static_cast<int64_t>(i) + 4);
            if (0 < dev && dev < 0x600 && (svc & 0x7FFF) < 0x400) {
                auto key = std::make_pair(dev, svc & 0x7FFF);
                if (!services.count(key)) {
                    services[key] = LE_SLOTS + 4 * static_cast<int64_t>(order.size());
                    order.push_back(key);
                }
                img[i] = '\xFF';
                img[i + 1] = (svc & 0x8000) ? '\x25' : '\x15';   // jmp / call [slot]
                put32(img, static_cast<int64_t>(i) + 2, services[key]);
                i = img.find("\xCD\x20", i + 6, 2);
            } else {
                i = img.find("\xCD\x20", i + 2, 2);
            }
        }
    }
    // where functions start: the control procedure, self-relative targets, agreed near calls
    std::optional<formats::le::Ddb> ddb;
    try {
        ddb = lf.ddb();
    } catch (const std::exception&) {
        ddb = std::nullopt;
    }
    if (ddb) {
        auto it = images.find(ddb->obj);
        int64_t at = ddb->rva + 4 + 20;
        if (it != images.end() && at + 4 <= static_cast<int64_t>(it->second.size())) {
            int64_t cp = get32(it->second, at);
            bool in_exec = false;
            for (int64_t o : execs)
                if (le_address(o, 0) <= cp && cp < le_address(o, static_cast<int64_t>(images[o].size()))) in_exec = true;
            if (in_exec) starts_.insert(cp);
        }
    }
    for (int64_t obj : execs) {
        const string& img = images[obj];
        std::map<int64_t, std::set<int64_t>> sites;
        for (int64_t i = 0; i < static_cast<int64_t>(img.size()) - 4; i++) {
            if (static_cast<unsigned char>(img[static_cast<size_t>(i)]) == 0xE8) {
                int64_t rel = static_cast<int32_t>(static_cast<uint32_t>(get32(img, i + 1)));
                int64_t t = i + 5 + rel;
                if (0 <= t && t < static_cast<int64_t>(img.size())) sites[t].insert(i);
            }
        }
        for (const auto& kv : sites)
            if (kv.second.size() >= 2) starts_.insert(le_address(obj, kv.first));
    }
    py::os::makedirs(outdir, 0777, true);
    py::str blob_py = py::os::path::abspath(py::os::path::join(outdir, py::str("image_le.bin")));
    string blob = blob_py.utf8();
    std::vector<string> lines;
    lines.push_back("arch\t" + LE_ARCH.utf8());
    int64_t pos = 0;
    {
        py::file f = py::open(blob_py, py::str("wb"));
        for (const auto& kv : images) {
            if (!kv.second.empty()) {
                f.write(py::bytes(kv.second));
                lines.push_back("bytes\t" + hx(le_address(kv.first, 0)) + "\t" + blob + "\t" + hx(pos) + "\t" + hx(static_cast<int64_t>(kv.second.size())));
                pos += static_cast<int64_t>(kv.second.size());
            }
        }
        f.close();
    }
    py::dict<int64_t, py::str> names;
    for (int64_t va : starts_)
        names.set(va, py::str("sub_" + std::to_string(fdiv(va - LE_BASE, LE_STRIDE) + 1) + "_" + HX(fmod_(va - LE_BASE, LE_STRIDE), 5)));
    for (int64_t va : starts_) lines.push_back("F\t" + hx(va) + "\t" + names.at(va).utf8() + "\t-1");
    for (const auto& dev_svc : order)
        lines.push_back("I\t" + hx(services[dev_svc]) + "\t" + symbols::driver_services::label(dev_svc.first, dev_svc.second).utf8() + "\t-1");
    auto d = std::make_shared<Description>(py::os::path::join(outdir, py::str("description.txt")), names, py::dict<int64_t, SegOff>(),
                                           py::list<py::str>());
    for (int64_t obj : execs) d->spans.set(obj, std::make_tuple(le_address(obj, 0), static_cast<int64_t>(images[obj].size())));
    if (ddb && !ddb->name.empty()) d->device = ddb->name;
    else d->device = name;
    for (const string& l : lines) d->lines.append(py::str(l));
    d->write();
    return d;
}

// ---- DOS programs (MZ) -----------------------------------------------------------------------------------------------

// Describe a real-mode program to the decompiler. -> Description
//
// The reader (formats/dos.py) gives the bytes as they sit in memory -- unpacked when the file stores them packed, a .COM behind
// its program prefix, a device driver, the parts of IO.SYS each where it runs -- and the analysis (analyze/dos_functions.py)
// every function start that can be established, with whether it returns far or near and how many bytes its return removes. The
// Description also carries `program`, `analysis` and `interrupts` ({function name: [interrupt calls]}) so the interrupt calls in
// the output can be named.
std::shared_ptr<Description> describe_dos(const py::str& module_path, const py::str& outdir) {
    formats::dos::Program prog = formats::dos::read(module_path);
    auto found = std::make_shared<analyze::dos_functions::Analysis>(analyze::dos_functions::discover(prog));
    py::os::makedirs(outdir, 0777, true);
    py::str blob_py = py::os::path::abspath(py::os::path::join(outdir, py::str("image_dos.bin")));
    string blob = blob_py.utf8();
    {
        py::file f = py::open(blob_py, py::str("wb"));
        f.write(prog.image);
        f.close();
    }
    std::vector<string> lines;
    lines.push_back("arch\t" + DOS_ARCH.utf8());
    for (const auto& span : prog.spans) {
        int64_t lo = std::get<0>(span), n = std::get<1>(span);
        lines.push_back("bytes\t" + hx(lo) + "\t" + blob + "\t" + hx(lo - prog.base) + "\t" + hx(n));
    }
    if (!prog.ds) throw py::TypeError("%x format: an integer is required, not NoneType");
    lines.push_back("reg\tDS\t" + hx(*prog.ds));
    py::dict<int64_t, py::str> names;
    std::vector<int64_t> vas = found->functions.keys();
    std::sort(vas.begin(), vas.end());
    for (int64_t va : vas) {
        const py::ref<analyze::dos_functions::Function>& fn = found->functions.at(va);
        names.set(va, fn->name);
        lines.push_back("F\t" + hx(fn->address) + "\t" + fn->name.utf8() + "\t" + std::to_string(fn->removes) + "\t" +
                        (fn->removes > 0 ? "__stdcall16" : "__cdecl16") + fn->distance.utf8());
    }
    auto d = std::make_shared<Description>(py::os::path::join(outdir, py::str("description.txt")), names, py::dict<int64_t, SegOff>(),
                                           py::list<py::str>());
    int64_t k = 0;
    for (const auto& span : prog.spans) {
        d->spans.set(k + 1, std::make_tuple(std::get<0>(span), std::get<1>(span)));
        k++;
    }
    d->image = prog.image;
    d->base = prog.base;
    d->program = prog;
    d->analysis = found;
    for (const auto& call : found->interrupts) {
        if (call.function) {
            if (!d->interrupts.contains(call.function->name)) d->interrupts.set(call.function->name, py::list<analyze::dos_functions::Interrupt>());
            d->interrupts.at(call.function->name).append(call);
        }
    }
    for (const string& l : lines) d->lines.append(py::str(l));
    d->write();
    return d;
}

// ---- NES --------------------------------------------------------------------------------------------------------------

// Write the description of a NES cartridge for the native program: the 6502, the program ROM where the CPU sees it at power-on
// (formats/nes.py), and its functions -- the three vectors' and every JSR target. -> (path of the description, {va: name})
std::tuple<py::str, py::dict<int64_t, py::str>> describe_nes(const py::str& module_path, const py::str& outdir) {
    auto rom = formats::nes::read(module_path);
    py::os::makedirs(outdir, 0777, true);
    py::str image = py::os::path::join(outdir, py::str("cpu.bin"));
    {
        py::file f = py::open(image, py::str("wb"));
        f.write(py::bytes(rom->memory().view()).slice_from(0x8000));
        f.close();
    }
    py::dict<int64_t, py::str> names;
    names.set(rom->vectors.at(py::str("reset")), py::str("reset"));
    names.setdefault(rom->vectors.at(py::str("nmi")), py::str("nmi"));
    names.setdefault(rom->vectors.at(py::str("irq")), py::str("irq"));
    std::vector<string> lines;
    lines.push_back("arch\t6502:LE:16:NES:default");   // platforms.named("nes").cpu
    lines.push_back("bytes\t8000\t" + abspath_s(image) + "\t0\t8000");
    py::dict<int64_t, py::str> by_va;
    for (int64_t va : rom->functions()) {
        by_va.set(va, names.get(va, py::str("sub_" + HX(va, 4))));
        lines.push_back("F\t" + hx(va) + "\t" + by_va.at(va).utf8() + "\t-1");
    }
    py::str path = py::os::path::join(outdir, py::str("description.txt"));
    py::file f = py::open(path, py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));
    string text;
    for (const string& l : lines) text += l + "\n";
    f.write(py::str(text));
    f.close();
    return {path, by_va};
}

}  // namespace codebreaker::decompile::describe
