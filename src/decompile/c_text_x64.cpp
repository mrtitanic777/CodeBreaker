// decompile/c_text_x64.cpp -- an x64 module's decompiled functions made compilable by Visual C++'s x64 compiler (see c_text.hpp).
//
// Each function is compiled on its own and linked into an image of its own, which is run beside the original (verify/lanes/win64):
// so what the rebuilt code reaches that it does not have itself is the original's, at the original's address. A global is a
// place in the original's data (`uRam0000000140034400` is `*(uint8 *)0x140034400`), an import is a call through the original's
// import slot, and a function that is not rebuilt -- the runtime library's, one the decompiler did not write -- is called where
// the original has it. Everything the functions share is declared in the one header every function is compiled against.
#include <algorithm>
#include <format>
#include <functional>
#include <map>
#include <optional>
#include <regex>
#include <set>

#include "core/text.hpp"
#include "decompile/c_text.hpp"
#include "decompile/c_text_internal.hpp"
#include "decompile/cpp_text_x64.hpp"
#include "formats/pe.hpp"
#include "symbols/cpp_classes_x64.hpp"

namespace cb::decompile::c_text {

namespace {

// The heading of a function's text, as a declaration: `void sub_001010(xunknown8 param_1,int4 param_2);`. Empty for a text that has
// no definition (a function whose decompile failed is a comment).
std::string declaration_of(std::string_view text) {
    const std::size_t open = text.find('(');
    const std::size_t brace = text.find('{');
    if (open == std::string_view::npos || brace == std::string_view::npos || brace < open) return {};
    std::string_view head = text.substr(0, brace);
    // (comments before the heading: the decompiler's notes)
    while (head.starts_with("/*")) {
        const std::size_t end = head.find("*/");
        if (end == std::string_view::npos) return {};
        head = text::trim(head.substr(end + 2));
    }
    const std::size_t close = head.rfind(')');
    if (close == std::string_view::npos) return {};
    return text::join(text::split_ws(head.substr(0, close + 1)), " ") + ";";
}

bool is_word(char c) { return c == '_' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

// What the x64 C needs beyond the 32-bit module's types and helpers: the odd-sized unknowns the decompiler names, a software
// interrupt as what it does (`int 29h` is __fastfail; any other a breakpoint), and the bus-lock markers around an atomic access
// (one thread runs here: the access alone is what it does).
constexpr std::string_view kX64Text = R"CBR(
typedef unsigned long long unkbyte5; typedef long long unkint5; typedef unsigned long long unkuint5;
typedef unsigned long long unkbyte6; typedef long long unkint6; typedef unsigned long long unkuint6;
typedef unsigned long long unkbyte7; typedef long long unkint7; typedef unsigned long long unkuint7;
__declspec(noreturn) void __fastfail(unsigned int);
void __debugbreak(void);
#pragma intrinsic(__fastfail, __debugbreak)
static uint8 __cb_int29(uint8 code) { __fastfail((unsigned int)code); }
static uint8 __cb_int3() { __debugbreak(); return 0; }
#define swi(n) ((n) == 0x29 ? (code *)__cb_int29 : (code *)__cb_int3)
void *_AddressOfReturnAddress(void);
void __movsb(unsigned char *, unsigned char const *, unsigned long long);
unsigned long long _umul128(unsigned long long, unsigned long long, unsigned long long *);
#pragma intrinsic(_AddressOfReturnAddress, __movsb, _umul128)
/* a * b for a 64-bit a and a 128-bit b held in 16 bytes, to 128 bits: its low (part 0) or high (part 8) 64 bits */
static uint8 __cb_mul128(uint8 a, const void *b, int part) { uint8 hi; uint8 lo = _umul128(a, ((const uint8 *)b)[0], &hi); hi += a * ((const uint8 *)b)[1]; return part ? hi : lo; }
static void __cb_mul128_set(void *d, uint8 a, const void *b) { uint8 lo = __cb_mul128(a, b, 0); uint8 hi = __cb_mul128(a, b, 8); ((uint8 *)d)[0] = lo; ((uint8 *)d)[1] = hi; }
/* a 128-bit value as two 64-bit halves: a 16-byte variable's bytes, a 64-bit value widened, shifted; the product of two, to 128 bits */
typedef struct { uint8 lo, hi; } __cb_u128;
static __cb_u128 __cb_ld128(const void *p) { __cb_u128 r; r.lo = ((const uint8 *)p)[0]; r.hi = ((const uint8 *)p)[1]; return r; }
static __cb_u128 __cb_zx128(uint8 v) { __cb_u128 r; r.lo = v; r.hi = 0; return r; }
static __cb_u128 __cb_shl128(__cb_u128 a, int n) { __cb_u128 r; if (n == 0) return a; if (n >= 128) { r.lo = 0; r.hi = 0; } else if (n >= 64) { r.lo = 0; r.hi = a.lo << (n - 64); } else { r.lo = a.lo << n; r.hi = a.hi << n | a.lo >> (64 - n); } return r; }
static __cb_u128 __cb_sx128(int8 v) { __cb_u128 r; r.lo = (uint8)v; r.hi = v < 0 ? ~(uint8)0 : 0; return r; }
static __cb_u128 __cb_shr128(__cb_u128 a, int n) { __cb_u128 r; if (n == 0) return a; if (n >= 128) { r.lo = 0; r.hi = 0; } else if (n >= 64) { r.hi = 0; r.lo = a.hi >> (n - 64); } else { r.hi = a.hi >> n; r.lo = a.lo >> n | a.hi << (64 - n); } return r; }
static uint8 __cb_bytes128(__cb_u128 a, int at) { return __cb_shr128(a, 8 * at).lo; }
static uint8 __cb_mulx128(__cb_u128 a, __cb_u128 b, int part) { uint8 hi; uint8 lo = _umul128(a.lo, b.lo, &hi); hi += a.lo * b.hi + a.hi * b.lo; return part ? hi : lo; }
static uint8 __cb_rd3(const void *p) { return (uint8)*(const uint2 *)p | ((uint8)((const uint1 *)p)[2] << 16); }
static uint8 __cb_rd5(const void *p) { return (uint8)*(const uint4 *)p | ((uint8)((const uint1 *)p)[4] << 32); }
static uint8 __cb_rd6(const void *p) { return (uint8)*(const uint4 *)p | ((uint8)*(const uint2 *)((const uint1 *)p + 4) << 32); }
static uint8 __cb_rd7(const void *p) { return __cb_rd6(p) | ((uint8)((const uint1 *)p)[6] << 48); }
static uint8 __cb_wr3(void *p, uint8 v) { *(uint2 *)p = (uint2)v; ((uint1 *)p)[2] = (uint1)(v >> 16); return v; }
static uint8 __cb_wr5(void *p, uint8 v) { *(uint4 *)p = (uint4)v; ((uint1 *)p)[4] = (uint1)(v >> 32); return v; }
static uint8 __cb_wr6(void *p, uint8 v) { *(uint4 *)p = (uint4)v; *(uint2 *)((uint1 *)p + 4) = (uint2)(v >> 32); return v; }
static uint8 __cb_wr7(void *p, uint8 v) { __cb_wr6(p, v); ((uint1 *)p)[6] = (uint1)(v >> 48); return v; }
unsigned long long __rdtsc(void);
unsigned long long __rdtscp(unsigned int *);
unsigned long long __popcnt64(unsigned long long);
unsigned int _mm_getcsr(void);
void _xrstor(void const *, unsigned long long);
unsigned int _mm_crc32_u8(unsigned int, unsigned char);
unsigned int _mm_crc32_u16(unsigned int, unsigned short);
unsigned int _mm_crc32_u32(unsigned int, unsigned int);
unsigned long long _mm_crc32_u64(unsigned long long, unsigned long long);
#pragma intrinsic(__rdtsc, __rdtscp, __popcnt64, _mm_getcsr, _xrstor, _mm_crc32_u8, _mm_crc32_u16, _mm_crc32_u32, _mm_crc32_u64)
#define rdtsc() __rdtsc()
static uint8 __cb_rdtscp(void) { unsigned int aux; return __rdtscp(&aux); }
#define rdtscp() __cb_rdtscp()
#define POPCOUNT(x) ((uint8)__popcnt64((unsigned long long)(x)))
#define MXCSR ((uint4)_mm_getcsr())
#define xrstor(p, mask) _xrstor((void const *)(p), (unsigned long long)(mask))
/* the instructions the decompiler names but C has no word for: a system call (`syscall; ret`, run as the original's is), an
   address monitor's set-up (it changes nothing a function's result shows) */
#pragma section(".text", read, execute)
__declspec(allocate(".text")) static const unsigned char __cb_syscall_code[] = {0x0f, 0x05, 0xc3};
#define syscall() (((void (*)(void))(void *)__cb_syscall_code)())
#define monitorx() ((void)0)
unsigned int _rdpid_u32(void);
#pragma intrinsic(_rdpid_u32)
#define readPID() ((uint8)_rdpid_u32())
#define LOCK()
#define UNLOCK()
)CBR";

// What a C++ program's header adds around the C one: compiled as C++ (a method's file), the C declarations are C's (a function
// declared without its parameters takes any arguments, as in C), and the classes follow them.
constexpr std::string_view kCppOpen = R"CBR(#ifdef __cplusplus
extern "C" {
#define CB_ARGS ...
#else
#define CB_ARGS
#endif
)CBR";

// `v._4_4_`, the decompiler's name for bytes of a variable (4 bytes at offset 4 of v), as C: `(*(uint4 *)((uint1 *)&v + 4))`. A size
// that is no C type (3, 5, 6, 7 bytes) is read and written by the header's helpers (`__cb_rd6(p)`, `__cb_wr6(p, value)`): the
// bytes of the variable's own memory in either case. The variable may be an element (`a[0]._1_3_`).
std::string partial_access(std::string s) {
    static const std::regex piece(R"(\b([A-Za-z_]\w*(?:\[[^\]\n]*\])*)\._(\d+)_(\d+)_)");
    std::string out;
    std::size_t at = 0;
    for (auto it = std::sregex_iterator(s.begin(), s.end(), piece); it != std::sregex_iterator(); ++it) {
        const std::string size = (*it)[3].str();
        const std::size_t begin = static_cast<std::size_t>(it->position());
        if (begin < at) continue;
        const std::size_t end = begin + static_cast<std::size_t>(it->length());
        if (size == "1" || size == "2" || size == "4" || size == "8") {
            out.append(s, at, begin - at);
            // (the bytes read as a pointer and followed: `*axStack_38._8_8_`)
            const bool followed = begin > 0 && s[begin - 1] == '*' && (begin < 2 || (!is_word(s[begin - 2]) && s[begin - 2] != ')'));
            out += std::format("(*(uint{} *{})((uint1 *)&{} + {}))", size, followed ? "*" : "", (*it)[1].str(), (*it)[2].str());
            at = end;
        } else if (size == "3" || size == "5" || size == "6" || size == "7") {
            out.append(s, at, begin - at);
            const std::string place = std::format("((uint1 *)&{} + {})", (*it)[1].str(), (*it)[2].str());
            std::size_t eq = end;
            while (eq < s.size() && s[eq] == ' ') ++eq;
            const std::size_t semi = s.find(';', eq);
            if (eq + 1 < s.size() && s[eq] == '=' && s[eq + 1] == ' ' && semi != std::string::npos) {   // (an assignment: a statement)
                out += std::format("__cb_wr{}({}, {})", size, place, partial_access(s.substr(eq + 2, semi - eq - 2)));
                at = semi;
            } else {
                out += std::format("__cb_rd{}({})", size, place);
                at = end;
            }
        } else {   // (more than 8 bytes: copied, `a._4_12_ = b._4_12_;`)
            static const std::regex copy(R"(^ = ([A-Za-z_]\w*(?:\[[^\]\n]*\])*)\._(\d+)_(\d+)_;)");
            std::smatch c;
            const std::size_t line_end = s.find('\n', end);
            const std::string rest = s.substr(end, line_end == std::string::npos ? std::string::npos : line_end - end);
            if (!std::regex_search(rest, c, copy) || c[3].str() != size) continue;
            out.append(s, at, begin - at);
            out += std::format("__movsb((uint1 *)&{} + {}, (uint1 *)&{} + {}, {})", (*it)[1].str(), (*it)[2].str(), c[1].str(), c[2].str(), size);
            at = end + static_cast<std::size_t>(c.length()) - 1;
        }
    }
    out.append(s, at, std::string::npos);
    return out;
}

// A text without the decompiler's notes before its heading (`/* WARNING: ... */`).
std::string_view past_notes(std::string_view text) {
    text = text::trim(text);
    while (text.starts_with("/*")) {
        const std::size_t end = text.find("*/");
        if (end == std::string_view::npos) break;
        text = text::trim(text.substr(end + 2));
    }
    return text;
}

// The address of a part of a structure the decompiler has no field for (`&p->field_0x10`, `&p[3].field_0x25`): the structure's
// address and the offset, which is how the memory is reached (the C's structure has the fields the function reads and writes, and
// the bytes between them).
std::string address_of_unnamed_fields(const std::string& text) {
    // (cast to the structure first: `&((S *)p)->field_0x8`; or read as a character: `p->field_0x20 == 'a'`)
    static const std::regex cast(R"(&\(\((\w+) \*\)(\w+)\)->field_0x([0-9a-f]+)\b)");
    static const std::regex as_char(R"(\b(\w+)->field_0x([0-9a-f]+)( (?:==|!=) '[^']{1,2}'))");
    const std::string s = std::regex_replace(std::regex_replace(text, cast, "((xunknown1 *)((uint1 *)$2 + 0x$3))"), as_char,
                                             "(*(char *)((uint1 *)$1 + 0x$2))$3");
    static const std::regex piece(R"(&(\w+)((?:\[[^\]\n]*\])*)(->|\.)field_0x([0-9a-f]+)\b)");
    std::string out;
    std::size_t at = 0;
    for (auto it = std::sregex_iterator(s.begin(), s.end(), piece); it != std::sregex_iterator(); ++it) {
        out.append(s, at, static_cast<std::size_t>(it->position()) - at);
        const std::string base = (*it)[1].str() + (*it)[2].str();
        out += std::format("((xunknown1 *)((uint1 *){} + 0x{}))", (*it)[3].str() == "->" ? base : "&" + base, (*it)[4].str());
        at = static_cast<std::size_t>(it->position() + it->length());
    }
    out.append(s, at, std::string::npos);
    // (and such a part read or written: the byte there -- the decompiler's field_0x is an undefined byte)
    if (out.find("field_0x") == std::string::npos) return out;
    static const std::regex cast_use(R"(\(\((\w+) \*\)(\w+)\)->field_0x([0-9a-f]+)\b)");
    const std::string t = std::regex_replace(out, cast_use, "(*(xunknown1 *)((uint1 *)$2 + 0x$3))");
    static const std::regex use(R"(\b(\w+)((?:\[[^\]\n]*\])*)(->|\.)field_0x([0-9a-f]+)\b)");
    std::string done;
    at = 0;
    for (auto it = std::sregex_iterator(t.begin(), t.end(), use); it != std::sregex_iterator(); ++it) {
        const std::size_t p = static_cast<std::size_t>(it->position());
        if (p > 0 && (t[p - 1] == '.' || t[p - 1] == '>')) continue;   // (a member's part: `a->b.field_0x4` stays)
        done.append(t, at, p - at);
        const std::string base = (*it)[1].str() + (*it)[2].str();
        done += std::format("(*(xunknown1 *)((uint1 *){} + 0x{}))", (*it)[3].str() == "->" ? base : "&" + base, (*it)[4].str());
        at = static_cast<std::size_t>(it->position() + it->length());
    }
    done.append(t, at, std::string::npos);
    return done;
}

// The decompiler's 128-bit product of a 64-bit value and a 16-byte variable (`SUB168(ZEXT816(2) * axVar2,8)`, `axVar2 = ZEXT816(2) * axVar2;`)
// and the name it gives a stack argument it stores to (`_param_7`, which is `param_7`).
std::string wide_products(const std::string& s) {
    static const std::regex part(R"(SUB168\(ZEXT816\(([^()]*(?:\([^()]*\))?[^()]*)\) \* (\w+),(0|8)\))");
    static const std::regex whole(R"((?:^|\n)(\s*)(\w+) = ZEXT816\(([^()\n]*(?:\([^()\n]*\))?[^()\n]*)\) \* (\w+);)");
    static const std::regex stack_arg(R"(\b_(param_\d+)\b)");
    std::string out = std::regex_replace(s, part, "__cb_mul128($1, $2, $3)");
    out = std::regex_replace(out, whole, "\n$1__cb_mul128_set($2, $3, $4);");
    return std::regex_replace(out, stack_arg, "$1");
}

// A local variable the function declares: its type as written, its size in bytes, and whether it is one value (not an array).
struct Local {
    std::string type;
    std::int64_t size = 0;
    bool scalar = true;
};

// The locals a function's text declares (`  xunknown4 xStack_30;`, `  uint1 *puVar2;`, `  xunknown1 axVar2 [16];`), by name: those
// of a type whose size the name says, and pointers.
std::map<std::string, Local> locals_of(const std::string& s) {
    static const std::regex sized(R"(^\s*((?:xunknown|uint|int|unkbyte|unkint|unkuint)([1-8]))\s+(\w+)\s*(?:\[(\d+)\])?;\s*$)");
    static const std::regex pointer(R"(^\s*(\w+\s*\*+)\s*(\w+);\s*$)");
    std::map<std::string, Local> out;
    for (const std::string_view line : text::split(s, '\n')) {
        std::cmatch m;
        if (std::regex_match(line.data(), line.data() + line.size(), m, sized)) {
            const std::int64_t one = std::stoll(m[2].str());
            out[m[3].str()] = m[4].matched ? Local{m[1].str(), one * std::stoll(m[4].str()), false} : Local{m[1].str(), one, true};
        } else if (std::regex_match(line.data(), line.data() + line.size(), m, pointer) && m[1].str() != "return" &&
                   !m[1].str().starts_with("goto")) {
            out[m[2].str()] = Local{m[1].str(), 8, true};
        }
    }
    // (and the parameters the heading states: `s_f_param_1 *param_1`, `uint4 param_2`)
    const std::string_view head = past_notes(s);
    const std::size_t open = head.find('('), brace = head.find('{');
    const std::size_t close = head.rfind(')', brace);
    if (open != std::string_view::npos && brace != std::string_view::npos && close != std::string_view::npos && open < close) {
        for (const std::string_view param : text::split(head.substr(open + 1, close - open - 1), ',')) {
            const std::string one = text::join(text::split_ws(param), " ") + ";";
            std::cmatch m;
            if (std::regex_match(one.data(), one.data() + one.size(), m, sized) && !m[4].matched)
                out.try_emplace(m[3].str(), Local{m[1].str(), std::stoll(m[2].str()), true});
            else if (std::regex_match(one.data(), one.data() + one.size(), m, pointer))
                out.try_emplace(m[2].str(), Local{m[1].str(), 8, true});
        }
    }
    return out;
}

// Where a stack variable is, from its name: `xStack_30` at -0x30 from the frame's top, `uStackX_8` (the caller's) at +0x8.
std::optional<std::int64_t> stack_place(std::string_view name) {
    const std::size_t at = name.rfind("Stack");
    if (at == std::string_view::npos) return std::nullopt;
    std::string_view rest = name.substr(at + 5);
    const bool above = rest.starts_with("X");
    if (above) rest.remove_prefix(1);
    if (!rest.starts_with("_") || rest.size() < 2) return std::nullopt;
    const std::optional<std::int64_t> n = text::parse_int<std::int64_t>(rest.substr(1), 16);
    if (!n) return std::nullopt;
    return above ? *n : -*n;
}

// `_xStack_30`, the decompiler's name for the bytes of stack variables side by side read or written as one (`_xStack_30 =
// CONCAT44(1,x)` writes xStack_30 and xStack_2c): each variable written its part of the value, or the value read as the parts put
// together. The width is the one the text gives it (CONCAT24: 6 bytes), else 8; the variables must lie end to end over it.
std::string stack_pieces(const std::string& s) {
    static const std::regex piece(R"((^|[^\w])_(\w*StackX?_[0-9a-f]+)\b)");
    if (s.find("Stack") == std::string::npos) return s;
    std::set<std::string> names;
    for (auto it = std::sregex_iterator(s.begin(), s.end(), piece); it != std::sregex_iterator(); ++it) names.insert((*it)[2].str());
    if (names.empty()) return s;
    const std::map<std::string, Local> locals = locals_of(s);
    std::map<std::int64_t, std::string> by_place;
    for (const auto& [n, l] : locals)
        if (const auto at = stack_place(n)) by_place[*at] = n;
    std::string out = s;
    for (const std::string& name : names) {
        if (!locals.contains(name)) continue;
        const std::int64_t start = *stack_place(name);
        std::int64_t width = 8;
        static const std::regex concat(R"(_(\w+) = (?:\(\w+\))?CONCAT([1-8])([1-8])\()");
        for (auto it = std::sregex_iterator(s.begin(), s.end(), concat); it != std::sregex_iterator(); ++it)
            if ((*it)[1].str() == name) width = std::stoll((*it)[2].str()) + std::stoll((*it)[3].str());
        if (width > 8) continue;
        // the variables over [start, start + width), end to end
        std::vector<std::pair<std::string, std::int64_t>> parts;   // (name, offset in the value)
        std::int64_t at = start;
        bool whole = true;
        while (at < start + width) {
            const auto p = by_place.find(at);
            if (p == by_place.end() || !locals.at(p->second).scalar || at + locals.at(p->second).size > start + width) {
                whole = false;
                break;
            }
            parts.emplace_back(p->second, at - start);
            at += locals.at(p->second).size;
        }
        if (!whole || parts.size() < 2) continue;
        const std::string word = "_" + name;
        std::string written;
        std::size_t from = 0;
        for (std::size_t k = out.find(word); k != std::string::npos; k = out.find(word, from)) {
            const std::size_t end = k + word.size();
            written.append(out, from, k - from);
            from = end;
            if ((k > 0 && is_word(out[k - 1])) || (end < out.size() && is_word(out[end]))) {
                written.append(word);
                continue;
            }
            const std::size_t line_end = out.find('\n', end);
            const std::string_view rest = std::string_view(out).substr(end, (line_end == std::string::npos ? out.size() : line_end) - end);
            if (rest.starts_with(" = ") && rest.ends_with(";")) {   // (written: each part its bytes of the value)
                std::string set = std::format("{{ uint8 __cb_piece = (uint8)({});", rest.substr(3, rest.size() - 4));
                for (const auto& [part, offset] : parts)
                    set += std::format(" {} = ({})(__cb_piece >> {});", part, locals.at(part).type, offset * 8);
                written += set + " }";
                from = end + rest.size();
            } else {   // (read: the parts put together)
                std::string value = "(";
                for (std::size_t i = 0; i < parts.size(); ++i)
                    value += std::format("{}((uint8)(uint{}){} << {})", i ? " | " : "", locals.at(parts[i].first).size, parts[i].first, parts[i].second * 8);
                written += value + ")";
            }
        }
        written.append(out, from, std::string::npos);
        out = std::move(written);
    }
    return out;
}

// The decompiler's 128-bit products left after wide_products (`SUB168(axVar1 * (ZEXT416(x) << 0x20),8)`, `SUB168(axVar2 * axVar3,0)`):
// each operand a 16-byte variable, a value widened (ZEXTn16) or one of those shifted, the product the header's __cb_mulx128.
std::string products_128(const std::string& s) {
    if (s.find("SUB16") == std::string::npos) return s;
    std::set<std::string> wide;   // the 16-byte variables
    for (const auto& [n, l] : locals_of(s))
        if (l.size == 16) wide.insert(n);
    const auto skip = [&](std::size_t i) {
        while (i < s.size() && s[i] == ' ') ++i;
        return i;
    };
    const auto closing = [&](std::size_t open) -> std::size_t {   // the `)` matching s[open]
        int depth = 0;
        for (std::size_t k = open; k < s.size(); ++k) {
            if (s[k] == '(') ++depth;
            else if (s[k] == ')' && --depth == 0) return k;
        }
        return std::string::npos;
    };
    // one operand from i: its C, and where it ends
    std::function<std::optional<std::pair<std::string, std::size_t>>(std::size_t)> operand = [&](std::size_t i) -> std::optional<std::pair<std::string, std::size_t>> {
        i = skip(i);
        for (const std::string_view zext : {"ZEXT116(", "ZEXT216(", "ZEXT416(", "ZEXT816(", "SEXT116(", "SEXT216(", "SEXT416(", "SEXT816("}) {
            if (s.compare(i, zext.size(), zext) != 0) continue;
            const std::size_t close = closing(i + zext.size() - 1);
            if (close == std::string::npos) return std::nullopt;
            const std::string inner = s.substr(i + zext.size(), close - i - zext.size());
            if (zext[0] == 'S')   // (sign-extended from its own width: a byte, a word, a dword or a qword)
                return std::pair{std::format("__cb_sx128((int8)(int{})({}))", zext[4] - '0', inner), close + 1};
            return std::pair{std::format("__cb_zx128((uint8)({}))", inner), close + 1};
        }
        if (i < s.size() && s[i] == '(') {
            const auto inner = operand(i + 1);
            if (!inner) return std::nullopt;
            std::size_t k = skip(inner->second);
            if (s.compare(k, 2, "<<") == 0) {
                const std::size_t close = s.find(')', k);
                if (close == std::string::npos) return std::nullopt;
                const std::string_view count = text::trim(std::string_view(s).substr(k + 2, close - k - 2));
                if (!text::parse_int<std::int64_t>(count, 0)) return std::nullopt;
                return std::pair{std::format("__cb_shl128({}, {})", inner->first, count), close + 1};
            }
            if (k < s.size() && s[k] == ')') return std::pair{inner->first, k + 1};
            return std::nullopt;
        }
        std::size_t e = i;
        while (e < s.size() && is_word(s[e])) ++e;
        const std::string name = s.substr(i, e - i);
        if (!wide.contains(name)) return std::nullopt;
        return std::pair{std::format("__cb_ld128({})", name), e};
    };
    std::string out;
    std::size_t at = 0;
    for (std::size_t k = s.find("SUB16"); k != std::string::npos; k = s.find("SUB16", k + 1)) {
        if (k < at || (k > 0 && is_word(s[k - 1])) || k + 7 > s.size() || s[k + 6] != '(') continue;
        const char width = s[k + 5];   // (the bytes taken: SUB161 one, SUB168 eight)
        if (width != '1' && width != '2' && width != '4' && width != '8') continue;
        const auto a = operand(k + 7);
        if (!a) continue;
        const std::size_t star = skip(a->second);
        if (star < s.size() && s.compare(star, 2, ">>") == 0) {   // (bytes of a 16-byte value shifted right: `SUB161(axVar8 >> 7,0)`)
            const std::size_t comma = s.find(',', star);
            const std::size_t close = comma == std::string::npos ? comma : s.find(')', comma);
            if (close == std::string::npos) continue;
            const std::string_view count = text::trim(std::string_view(s).substr(star + 2, comma - star - 2));
            const std::string_view offset = text::trim(std::string_view(s).substr(comma + 1, close - comma - 1));
            const auto n = text::parse_int<std::int64_t>(count, 0);
            const auto o = text::parse_int<std::int64_t>(offset, 0);
            if (!n || !o || *o < 0 || *o > 15) continue;
            out.append(s, at, k - at);
            out += std::format("((uint{})__cb_bytes128(__cb_shr128({}, {}), {}))", width, a->first, *n, *o);
            at = close + 1;
            continue;
        }
        if (width != '8' || star >= s.size() || s[star] != '*') continue;
        const auto b = operand(star + 1);
        if (!b) continue;
        const std::size_t comma = skip(b->second);
        if (comma >= s.size() || s[comma] != ',') continue;
        const std::size_t close = s.find(')', comma);
        if (close == std::string::npos) continue;
        const std::string_view part = text::trim(std::string_view(s).substr(comma + 1, close - comma - 1));
        if (part != "0" && part != "8") continue;
        out.append(s, at, k - at);
        out += std::format("__cb_mulx128({}, {}, {})", a->first, b->first, part);
        at = close + 1;
    }
    out.append(s, at, std::string::npos);
    return out;
}

// A 16-byte variable given a 128-bit product whole (`axVar2 = ZEXT416(uVar6 - 1) * ZEXT816(0x18);`): its two halves, each the
// product's (products_128 writes them).
std::string wide_assignments(const std::string& s) {
    if (s.find(" * ") == std::string::npos || s.find("EXT") == std::string::npos) return s;
    std::set<std::string> wide;
    for (const auto& [n, l] : locals_of(s))
        if (l.size == 16 && !l.scalar) wide.insert(n);
    if (wide.empty()) return s;
    static const std::regex assign(R"(^(\s*)(\w+) = ((?:[ZS]EXT\d+16\(|\w+ \* ).* \* .*|[ZS]EXT\d+16\(.*);$)");
    std::vector<std::string> out;
    for (const std::string_view full : text::split(s, '\n')) {
        std::string line(full);
        std::smatch m;
        if (std::regex_match(line, m, assign) && wide.contains(m[2].str()) && m[3].str().find(" * ") != std::string::npos)
            line = std::format("{0}*(uint8 *){1} = SUB168({2},0); *(uint8 *)((uint1 *){1} + 8) = SUB168({2},8);", m[1].str(), m[2].str(), m[3].str());
        out.push_back(std::move(line));
    }
    return text::join(out, "\n");
}

// A local the decompiler declares as a 64-bit number but reaches through as a pointer (`xStack_338->f_0`, `xStack_350[1]`, the
// stack slot holding a pointer at those places): declared as the pointer the text makes of it (`xStack_338 = (S *)...`).
std::string pointer_locals(const std::string& s) {
    if (s.find("->") == std::string::npos && s.find('[') == std::string::npos) return s;
    static const std::regex decl(R"((^|\n)(\s*)(?:xunknown8|uint8|int8) (\w+);)");
    std::string out = s;
    for (auto it = std::sregex_iterator(s.begin(), s.end(), decl); it != std::sregex_iterator(); ++it) {
        const std::string name = (*it)[3].str();
        const std::regex arrow("\\b" + name + "->");
        const std::regex index("\\b" + name + "\\[");
        const bool through = std::regex_search(s, arrow);
        if (!through && !std::regex_search(s, index)) continue;
        const std::regex cast("\\b" + name + " =\\s*\\((\\w+) \\*\\)");
        std::string type;
        for (auto c = std::sregex_iterator(s.begin(), s.end(), cast); c != std::sregex_iterator(); ++c) {
            const std::string t = (*c)[1].str();
            if (!through || t.starts_with("s_")) {
                type = t;
                break;
            }
        }
        if (type.empty()) {   // (or given a parameter or local that is that pointer: `xStack_e0 = param_1;`)
            static const std::regex typed_pointer(R"(^(\w+) \*$)");
            const std::regex given("\\b" + name + " = (\\w+);");
            const std::map<std::string, Local> locals = locals_of(s);
            for (auto g = std::sregex_iterator(s.begin(), s.end(), given); g != std::sregex_iterator() && type.empty(); ++g) {
                const auto l = locals.find((*g)[1].str());
                std::smatch t;
                if (l != locals.end() && std::regex_match(l->second.type, t, typed_pointer) && (!through || t[1].str().starts_with("s_")))
                    type = t[1].str();
            }
        }
        if (type.empty()) continue;
        const std::string was = (*it)[0].str();
        const std::size_t k = out.find(was);
        if (k == std::string::npos) continue;
        out.replace(k, was.size(), std::format("{}{}{} *{};", (*it)[1].str(), (*it)[2].str(), type, name));
    }
    return out;
}

// The function's name and the number of parameters it states (`void` states none).
std::pair<std::string, int> name_and_arity(std::string_view text) {
    const std::string_view head = past_notes(text);
    const std::size_t open = head.find('(');
    if (open == std::string_view::npos) return {};
    std::size_t name_end = open;
    while (name_end > 0 && head[name_end - 1] == ' ') --name_end;
    std::size_t name_start = name_end;
    while (name_start > 0 && is_word(head[name_start - 1])) --name_start;
    int depth = 0, commas = 0;
    std::size_t k = open;
    for (; k < head.size(); ++k) {
        if (head[k] == '(') ++depth;
        else if (head[k] == ')' && --depth == 0) break;
        else if (head[k] == ',' && depth == 1) ++commas;
    }
    const std::string_view params = text::trim(head.substr(open + 1, k - open - 1));
    const int arity = params.empty() || params == "void" ? 0 : commas + 1;
    return {std::string(head.substr(name_start, name_end - name_start)), arity};
}

// A function that calls itself with other arguments than it has parameters (the original's call left the rest in whatever the
// caller's registers and stack held) calls through a pointer to a function without a prototype, as every other call does.
std::string unprototyped_self_calls(const std::string& s) {
    const auto [name, arity] = name_and_arity(s);
    if (name.empty()) return s;
    const std::size_t body = s.find('{');
    if (body == std::string::npos) return s;
    std::string out = s.substr(0, body);
    std::size_t at = body;
    for (std::size_t k = s.find(name + "(", body); k != std::string::npos; k = s.find(name + "(", at)) {
        out.append(s, at, k - at);
        at = k;
        const bool word_start = k == 0 || !is_word(s[k - 1]);
        int depth = 0, commas = 0;
        std::size_t e = k + name.size();
        bool any = false;
        for (; e < s.size(); ++e) {
            if (s[e] == '(') ++depth;
            else if (s[e] == ')' && --depth == 0) break;
            else if (s[e] == ',' && depth == 1) ++commas;
            else if (depth == 1 && s[e] != ' ' && s[e] != '\n') any = true;
        }
        const int args = any ? commas + 1 : 0;
        if (word_start && args != arity) {
            out += "((uint8 (*)())" + name + ")";
            at = k + name.size();
        } else {
            out.append(s, k, name.size());
            at = k + name.size();
        }
    }
    out.append(s, at, std::string::npos);
    return out;
}

// A label the decompiler writes twice (two copies of a block it reached from more than one place): the later ones renamed, so the
// gotos -- written before the first -- go to the first.
std::string single_labels(const std::string& s) {
    if (s.find("code_r0x") == std::string::npos) return s;
    std::vector<std::string_view> lines = text::split(s, '\n');
    std::map<std::string, int> seen;
    std::vector<std::string> out;
    for (const std::string_view line : lines) {
        const std::string_view t = text::trim(line);
        if (t.starts_with("code_r0x") && t.back() == ':' && t.find(' ') == std::string_view::npos) {
            const std::string label(t.substr(0, t.size() - 1));
            if (const int n = ++seen[label]; n > 1) {
                out.push_back(std::string(line.substr(0, line.find('c'))) + std::format("{}_{}:", label, n));
                continue;
            }
        }
        out.emplace_back(line);
    }
    return text::join(out, "\n");
}

// A label at the end of a block (`code_r0x00014001134f:` before the `}`) needs a statement after it, before C23.
std::string close_labels(const std::string& s) {
    const std::vector<std::string_view> lines = text::split(s, '\n');
    std::vector<std::string> out;
    for (std::size_t k = 0; k < lines.size(); ++k) {
        std::string line(lines[k]);
        const std::string_view t = text::trim(lines[k]);
        const bool label = t.find(' ') == std::string_view::npos || t.starts_with("case ");   // (a case's label too)
        if (t.size() > 1 && t.back() == ':' && label) {
            std::size_t next = k + 1;
            while (next < lines.size() && text::trim(lines[next]).empty()) ++next;
            if (next < lines.size() && text::trim(lines[next]).starts_with("}")) line += ";";
        }
        out.push_back(std::move(line));
    }
    return text::join(out, "\n");
}

// An array variable the decompiler assigns whole (`axVar2 = *paxVar4;`): C has no array assignment, the bytes are moved.
std::string array_assignments(const std::string& s) {
    static const std::regex decl(R"(^\s*(?:xunknown|uint|int)([1248])\s+(\w+)\s*\[(\d+)\];)");
    const std::vector<std::string_view> lines = text::split(s, '\n');
    std::map<std::string, std::int64_t> arrays;
    for (const std::string_view line : lines) {
        std::cmatch m;
        if (std::regex_search(line.data(), line.data() + line.size(), m, decl)) arrays[m[2].str()] = std::stoll(m[1].str()) * std::stoll(m[3].str());
    }
    if (arrays.empty()) return s;
    std::vector<std::string> out;
    for (const std::string_view full : lines) {
        std::string line(full);
        const std::string_view t = text::trim(full);
        const std::size_t eq = t.find(" = ");
        if (eq != std::string_view::npos && t.back() == ';') {
            const std::string place(t.substr(0, eq)), value(t.substr(eq + 3, t.size() - eq - 4));
            static const std::regex filled(R"(^\((?:\w+)\s*\[\d+\]\)(\w+)$|^ZEXT\d+(?:16|32)\((.+)\)$)");
            std::smatch f;
            const auto a = arrays.find(place);
            const auto balanced = [](const std::string& e) {   // (`ZEXT816(a) * (b)` is not one value widened)
                int depth = 0;
                for (const char c : e)
                    if ((depth += c == '(' ? 1 : c == ')' ? -1 : 0) < 0) return false;
                return depth == 0;
            };
            if (a != arrays.end() && std::regex_match(value, f, filled) && (f[1].matched || balanced(f[2].str())) &&
                (a->second == 1 || a->second == 2 || a->second == 4 || a->second == 8 || a->second == 16)) {
                // (`axStack_58 = (xunknown1 [8])0x0;`, `axStack_98 = ZEXT816(0);`: the value in its low bytes, zeros above)
                const std::string indent = line.substr(0, line.find_first_not_of(' '));
                const std::int64_t low = std::min<std::int64_t>(a->second, 8);
                line = std::format("{}*(uint{} *){} = (uint{})({});", indent, low, a->first, low, f[1].matched ? f[1].str() : f[2].str());
                if (a->second == 16) line += std::format(" *(uint8 *)((uint1 *){} + 8) = 0;", a->first);
            } else if (a != arrays.end())
                line = std::format("{}__movsb((uint1 *){}, (uint1 *)&({}), {});", line.substr(0, line.find_first_not_of(' ')), a->first,
                                   value, a->second);
            // (an array given whole to a place that is one: `*param_2 = axVar2;` with param_2 a pointer to 16 bytes)
            else if (const auto b = arrays.find(value); b != arrays.end() && place.starts_with("*") &&
                                                                    place.find_first_of("=;") == std::string::npos)
                line = std::format("{}__movsb((uint1 *)&({}), (uint1 *){}, {});", line.substr(0, line.find_first_not_of(' ')), place,
                                   b->first, b->second);
        }
        out.push_back(std::move(line));
    }
    return text::join(out, "\n");
}

// Every identifier of a text that is followed by `(`: what it calls (and casts and keywords, which the caller passes over).
std::set<std::string> called_words(std::string_view s) {
    std::set<std::string> out;
    for (std::size_t k = 0; k < s.size();) {
        if (!is_word(s[k]) || (k > 0 && is_word(s[k - 1]))) {
            ++k;
            continue;
        }
        std::size_t e = k;
        while (e < s.size() && is_word(s[e])) ++e;
        std::size_t after = e;
        while (after < s.size() && (s[after] == ' ' || s[after] == '\n' || s[after] == '\r' || s[after] == '\t')) ++after;
        if (after < s.size() && s[after] == '(') out.emplace(s.substr(k, e - k));
        k = e;
    }
    return out;
}

// Every identifier of a text.
std::set<std::string> words_of(std::string_view s) {
    std::set<std::string> out;
    for (std::size_t k = 0; k < s.size();) {
        if (!is_word(s[k]) || (k > 0 && is_word(s[k - 1]))) {
            ++k;
            continue;
        }
        std::size_t e = k;
        while (e < s.size() && is_word(s[e])) ++e;
        out.emplace(s.substr(k, e - k));
        k = e;
    }
    return out;
}

}  // namespace

// `crc32(crc, value)`, the instruction, as wide as its value is (`xunknown1 xVar8`: a byte; `*pxVar1`, pxVar1 an `xunknown8 *`: 8
// bytes): the intrinsic of that width.
std::string crc32_widths(const std::string& s) {
    if (s.find("crc32(") == std::string::npos) return s;
    const std::map<std::string, Local> locals = locals_of(s);
    std::string out;
    std::size_t at = 0;
    for (std::size_t k = s.find("crc32("); k != std::string::npos; k = s.find("crc32(", k + 1)) {
        if (k < at || (k > 0 && (is_word(s[k - 1]) || s[k - 1] == '.'))) continue;
        int depth = 0;
        std::size_t comma = std::string::npos, close = std::string::npos;
        for (std::size_t e = k + 5; e < s.size(); ++e) {
            if (s[e] == '(') ++depth;
            else if (s[e] == ')' && --depth == 0) { close = e; break; }
            else if (s[e] == ',' && depth == 1) comma = e;
        }
        if (comma == std::string::npos || close == std::string::npos) continue;
        std::string value(text::trim(std::string_view(s).substr(comma + 1, close - comma - 1)));
        const bool through = value.starts_with("*");
        const auto l = locals.find(through ? value.substr(1) : value);
        if (l == locals.end()) continue;
        std::string type = l->second.type;
        if (through) {   // (the pointer's element)
            if (!type.ends_with(" *")) continue;
            type = type.substr(0, type.size() - 2);
        } else if (!l->second.scalar || type.ends_with("*")) {
            continue;
        }
        const char digit = type.empty() ? '0' : type.back();
        static const std::map<char, std::pair<std::string_view, std::string_view>> kWidth = {
            {'1', {"u8", "unsigned char"}}, {'2', {"u16", "unsigned short"}}, {'4', {"u32", "unsigned int"}}, {'8', {"u64", "unsigned long long"}}};
        const auto w = kWidth.find(digit);
        if (w == kWidth.end()) continue;
        out.append(s, at, k - at);
        out += std::format("((uint8)_mm_crc32_{}(({})({}), ({})({})))", w->second.first, digit == '8' ? "unsigned long long" : "unsigned int",
                           text::trim(std::string_view(s).substr(k + 6, comma - k - 6)), w->second.second, value);
        at = close + 1;
    }
    out.append(s, at, std::string::npos);
    return out;
}

// A function the decompiler finds to return a pointer to an array writes it as no C declares one (`xunknown1 (*) [16] strrchr(...)`):
// written to return a pointer to the array's first element, the same address.
std::string array_pointer_result(std::string s) {
    const std::string_view head = past_notes(s);
    static const std::regex result(R"(^(\w+) \(\*\) \[\d+\] (\w+)\()");
    std::cmatch m;
    if (!std::regex_search(head.data(), head.data() + head.size(), m, result)) return s;
    const std::size_t at = s.size() - head.size();
    return s.replace(at, static_cast<std::size_t>(m.length()), m[1].str() + " *" + m[2].str() + "(");
}

std::string x64_function_text(std::string_view text) {
    return crc32_widths(array_pointer_result(pointer_locals(single_labels(close_labels(array_assignments(products_128(wide_assignments(wide_products(unprototyped_self_calls(address_of_unnamed_fields(
        stack_pieces(partial_access(std::string(text::trim(text)))))))))))))));
}

ModuleX64 compilable_x64(Services& services, const std::filesystem::path& module, const std::filesystem::path& outdir) {
    const formats::pe::Image image = formats::pe::Image::open(module);
    Decompiled got = services.raw(module, {}, outdir);
    OrderedMap<std::string, std::string> out;
    std::vector<std::string> order;
    std::map<Va, std::set<std::int64_t>> sizes;
    std::vector<StructNote> structs;
    std::vector<std::string> decls;
    std::set<std::string> own;
    ModuleX64 m;
    for (const std::string& n : got.order) {
        const std::string* text = got.texts.find(n);
        if (text == nullptr) continue;
        if (const auto it = got.notes.find(n); it != got.notes.end()) {
            for (const core16::GlobalNote& g : it->second.globals) sizes[g.address].insert(g.bytes);
            structs.insert(structs.end(), it->second.structs.begin(), it->second.structs.end());
        }
        std::string body = x64_function_text(*text);
        const std::string declared = declaration_of(body);
        // (no C: left to the original -- nor C the decompiler says it could not recover: a jump through a table or an import's slot
        // it did not follow is a call through a pointer nothing sets)
        if (declared.empty() || internal::has(body, "UNRECOVERED_JUMPTABLE")) continue;
        // Every function leaves something in RAX, and the decompiler may find a caller using it where it found the function to return
        // nothing: a `void` function is written to return uint8, its result not compared.
        // (the decompiler's notes may come before the heading)
        if (const std::string_view heading = past_notes(body); heading.starts_with("void ")) {
            body = std::string(body.substr(0, body.size() - heading.size())) + "uint8 " + std::string(heading.substr(5));
            m.novalue.insert(n);
        }
        own.insert(n);
        // (declared without its parameters: a call the decompiler wrote with another number of arguments still compiles, as the
        // original's call did)
        const std::string head = declaration_of(body);
        decls.push_back(head.substr(0, head.find('(')) + "();");
        out.insert_or_assign(n, std::move(body));
        order.push_back(n);
    }
    std::map<std::string, std::uint64_t> slots;   // import name -> its slot
    for (const formats::pe::Import& i : image.imports())
        if (!i.name.empty() && i.name[0] != '#' && i.name != "?") slots.try_emplace(text::sanitize_identifier(i.name), i.slot_va);
    // (a delay-loaded import is called through its own slot, which holds the original's stub until the helper fills it in)
    for (const formats::pe::ImportModule& delayed : image.import_table(true))
        for (const formats::pe::ImportSymbol& s : delayed.symbols)
            if (s.name) slots.try_emplace(text::sanitize_identifier(*s.name), image.header().image_base + s.slot_rva);
    std::map<std::string, Va> elsewhere;          // a function of the module the C calls by name that is not rebuilt
    for (const auto& [va, name] : got.described)
        if (!own.contains(name)) elsewhere.try_emplace(name, va);

    // An imported variable (ntdll's NlsMbCodePageTag): the decompiler gives it the type of what it does not know (`code`) and
    // compares its value (`NlsMbCodePageTag != (code)0x0`, the first byte there). Its slot holds the variable's address: the C reads
    // the byte at it -- unless some function calls it, then it is a function after all.
    std::set<std::string> data_imports;
    {
        static const std::regex compared(R"(\b(\w+) ([!=]=) \(code\))");
        std::set<std::string> called;
        for (const std::string& n : order)
            for (const std::string& w : called_words(out.at(n))) called.insert(w);
        for (const std::string& n : order) {
            const std::string& t = out.at(n);
            for (auto it = std::sregex_iterator(t.begin(), t.end(), compared); it != std::sregex_iterator(); ++it)
                if (const std::string w = (*it)[1].str(); slots.contains(w) && !called.contains(w)) data_imports.insert(w);
        }
        for (const std::string& n : order) {
            std::string& t = out.at(n);
            if (t.find("(code)") == std::string::npos) continue;
            std::string done;
            std::size_t at = 0;
            for (auto it = std::sregex_iterator(t.begin(), t.end(), compared); it != std::sregex_iterator(); ++it) {
                if (!data_imports.contains((*it)[1].str())) continue;
                done.append(t, at, static_cast<std::size_t>(it->position()) - at);
                done += (*it)[1].str() + " " + (*it)[2].str() + " (uint1)";
                at = static_cast<std::size_t>(it->position() + it->length());
            }
            done.append(t, at, std::string::npos);
            t = std::move(done);
        }
    }
    // `_NlsAnsiCodePage`, `_xRam00000001800c7208`: the decompiler's name for a global wider than the symbol at its address (its
    // note: "Globals starting with '_' overlap smaller symbols at the same address") -- for an import the variable its slot points
    // at. Read as wide as what the text compares it with or gives it to (`uVar1 == _NlsAnsiCodePage`, uVar1 a uint2).
    for (const std::string& n : order) {
        std::string& t = out.at(n);
        if (t.find("Globals starting with '_'") == std::string::npos) continue;
        static const std::regex wide(R"((^|[^\w])_(\w+)\b)");
        static const std::regex ram_name(R"(^[a-z]*Ram([0-9a-f]{16})$)");
        static const std::regex before(R"((\w+) (?:==|!=|=) $)");
        static const std::regex after(R"(^ (?:==|!=) (\w+))");
        const std::map<std::string, Local> locals = locals_of(t);
        const auto type_of = [&](const std::string& word) -> std::string {
            const auto l = locals.find(word);
            return l != locals.end() && l->second.scalar && l->second.size <= 8 ? l->second.type : std::string();
        };
        std::string done;
        std::size_t at = 0;
        for (auto it = std::sregex_iterator(t.begin(), t.end(), wide); it != std::sregex_iterator(); ++it) {
            const std::string w = (*it)[2].str();
            std::smatch r;
            const bool is_ram = std::regex_match(w, r, ram_name);
            const auto slot = slots.find(w);
            if (!is_ram && slot == slots.end()) continue;
            const std::size_t begin = static_cast<std::size_t>(it->position()) + (*it)[1].length();
            const std::size_t end = begin + 1 + w.size();
            const std::size_t line_start = t.rfind('\n', begin) == std::string::npos ? 0 : t.rfind('\n', begin) + 1;
            const std::string left = t.substr(line_start, begin - line_start), right = t.substr(end, t.find('\n', end) - end);
            std::smatch other;
            std::string type;
            if (std::regex_search(left, other, before) && other.suffix().length() == 0) type = type_of(other[1].str());
            if (type.empty() && std::regex_search(right, other, after)) type = type_of(other[1].str());
            if (type.empty()) continue;
            done.append(t, at, begin - at);
            done += is_ram ? std::format("(*({} *)0x{})", type, r[1].str()) : std::format("(*({} *)*(uint1 **)0x{:x})", type, slot->second);
            at = end;
        }
        done.append(t, at, std::string::npos);
        t = std::move(done);
    }
    static const std::regex ram(R"(\b([a-z]*)Ram([0-9a-f]{16})\b)");
    static const std::regex func(R"(\bfunc_0x([0-9a-f]{8,16})\b)");   // (as many digits as the address needs)
    std::set<std::pair<std::string, std::string>> rams;
    std::set<std::string> funcs, called_through, calls;
    bool stack_pointer = false;   // (the C reads the stack pointer, as the decompiler names it)
    for (const std::string& n : order) {
        const std::string& t = out.at(n);
        for (auto it = std::sregex_iterator(t.begin(), t.end(), ram); it != std::sregex_iterator(); ++it) {
            rams.emplace((*it)[1].str(), (*it)[2].str());
            // (a global called through holds a function's address)
            const std::size_t end = static_cast<std::size_t>(it->position() + it->length());
            if (it->position() >= 2 && t.compare(static_cast<std::size_t>(it->position()) - 2, 2, "(*") == 0 && t.compare(end, 1, ")") == 0) {
                std::size_t open = end + 1;   // (the arguments may begin on the next line)
                while (open < t.size() && (t[open] == ' ' || t[open] == 10 || t[open] == 13)) ++open;
                if (open < t.size() && t[open] == '(') called_through.insert(it->str());
            }
        }
        for (auto it = std::sregex_iterator(t.begin(), t.end(), func); it != std::sregex_iterator(); ++it) funcs.insert((*it)[1].str());
        for (const std::string& w : called_words(t)) calls.insert(w);
        // (a function of the module the C does not call but passes on, by its name)
        for (const std::string& w : words_of(t)) {
            if (elsewhere.contains(w) || slots.contains(w)) calls.insert(w);   // (an import passed on is the pointer its slot holds)
            if (w == "register0x00000020") stack_pointer = true;
        }
    }
    std::vector<std::string> macros;
    for (const auto& [prefix, digits] : rams) {
        const Va at = *text::parse_int<Va>(digits, 16);
        std::int64_t size = 8;
        if (const auto it = sizes.find(at); it != sizes.end()) {
            std::int64_t widest = 0;
            for (const std::int64_t s : it->second)
                if (s <= 8) widest = std::max(widest, s);
            if (widest != 0) size = widest;
        }
        const std::string name = prefix + "Ram" + digits;
        macros.push_back(std::format("#define {} (*({} *)0x{})", name, internal::ram_type(prefix, size, called_through.contains(name)), digits));
    }
    for (const std::string& digits : funcs)
        if (!own.contains("func_0x" + digits)) macros.push_back(std::format("#define func_0x{} ((code *)0x{})", digits, digits));
    // (the stack pointer a function reaches its frame by after a stack probe: where the function was entered, which is the
    // closest the rebuilt function has to the original's)
    if (stack_pointer) macros.push_back("#define register0x00000020 ((int8)_AddressOfReturnAddress())");
    // (a function of the module named as one Visual C++ makes intrinsic -- ntdll's own wcslen, memset -- is called as the module's,
    // with whatever arguments the decompiler gives it)
    static const std::set<std::string> kIntrinsic = {"memcmp", "memcpy", "memset", "strcat", "strcmp", "strcpy", "strlen", "wcscat",
                                                     "wcscmp", "wcscpy", "wcslen", "_strset", "_wcsset", "abs", "labs", "llabs", "_abs64",
                                                     "_rotl", "_rotr", "_lrotl", "_lrotr", "_rotl64", "_rotr64", "_byteswap_ushort",
                                                     "_byteswap_ulong", "_byteswap_uint64"};
    std::vector<std::string> not_intrinsic;   // (after the declarations, which the pragma's own would otherwise contradict)
    for (const std::string& n : own)
        if (kIntrinsic.contains(n)) not_intrinsic.push_back(std::format("#pragma function({})", n));
    for (const std::string& w : calls) {
        if (own.contains(w)) continue;
        if (const auto s = slots.find(w); s != slots.end()) {
            macros.push_back(data_imports.contains(w) ? std::format("#define {} (**(uint1 **)0x{:x})", w, s->second)
                                                      : std::format("#define {} (*(code **)0x{:x})", w, s->second));
            m.outside.emplace(w, s->second);
        } else if (const auto e = elsewhere.find(w); e != elsewhere.end()) {
            macros.push_back(std::format("#define {} ((code *)0x{:x})", w, static_cast<std::uint64_t>(e->second)));
            m.outside.emplace(w, static_cast<std::uint64_t>(e->second));
        }
    }
    // A C++ program (run-time type information names its classes): the functions the evidence makes methods are written as the
    // classes' members, compiled as C++; the rest stay C.
    const symbols::cpp_classes_x64::Classes classes = symbols::cpp_classes_x64::find_classes(image);
    cpp_text_x64::Recovered cpp;
    if (!classes.classes.empty()) cpp = cpp_text_x64::recover({&classes, &got.described, &got.by_va, &out, structs});
    const bool cplusplus = !cpp.methods.empty();
    std::map<std::string, std::string> renamed;   // C name -> the member's qualified name
    if (cplusplus) {
        OrderedMap<std::string, std::string> written;
        for (std::string& n : order) {
            std::string text = out.at(n);
            if (const auto method = cpp.methods.find(n); method != cpp.methods.end()) {
                renamed[n] = method->second.qualified;
                text = cpp.texts.at(n);
            }
            const std::string key = renamed.contains(n) ? renamed.at(n) : n;
            written.insert_or_assign(key, std::move(text));
            n = key;
        }
        out = std::move(written);
        std::set<std::string> novalue;
        for (const std::string& n : m.novalue) novalue.insert(renamed.contains(n) ? renamed.at(n) : n);
        m.novalue = std::move(novalue);
        for (std::string& d : decls) d = d.substr(0, d.size() - 3) + "(CB_ARGS);";
        m.classes = cpp.classes;
        m.methods = renamed;
    }
    std::string c_types = text::replace_all(internal::types_text(), "typedef int code();",
                                            cplusplus ? "#ifdef __cplusplus\ntypedef uint8 code(...);\n#else\ntypedef uint8 code();\n#endif" : "typedef uint8 code();");
    // (C++ has its own bool, one byte as C's typedef is)
    if (cplusplus) c_types = text::replace_all(c_types, "typedef unsigned char bool;", "\n#ifndef __cplusplus\ntypedef unsigned char bool;\n#endif\n");
    const std::string types = c_types + std::string(internal::helpers_text()) + internal::x86_helpers() + std::string(kX64Text);
    std::string header = types + internal::lines_of(internal::struct_decls(structs, types)) + text::join(macros, "\n") + "\n" +
                         text::join(decls, "\n") + "\n" + (not_intrinsic.empty() ? std::string() : text::join(not_intrinsic, "\n") + "\n");
    if (cplusplus) header = std::string(kCppOpen) + header + "#ifdef __cplusplus\n}\n" + cpp.declarations + "#endif\n";
    OrderedMap<std::uint64_t, std::string> by_address;
    for (const auto& [va, n] : got.by_va)
        if (own.contains(n)) by_address.insert_or_assign(static_cast<std::uint64_t>(va), renamed.contains(n) ? renamed.at(n) : n);
    m.made = translate::l2::Made::of(text::lower(module.filename().string()), header, out, order, by_address);
    m.image_base = image.header().image_base;
    return m;
}

}  // namespace cb::decompile::c_text
