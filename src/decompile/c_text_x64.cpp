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

std::set<std::string> words_of(std::string_view s);

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
void _mm_setcsr(unsigned int);
void _xrstor(void const *, unsigned long long);
unsigned int _mm_crc32_u8(unsigned int, unsigned char);
unsigned int _mm_crc32_u16(unsigned int, unsigned short);
unsigned int _mm_crc32_u32(unsigned int, unsigned int);
unsigned long long _mm_crc32_u64(unsigned long long, unsigned long long);
#pragma intrinsic(__rdtsc, __rdtscp, __popcnt64, _mm_getcsr, _mm_setcsr, _xrstor, _mm_crc32_u8, _mm_crc32_u16, _mm_crc32_u32, _mm_crc32_u64)
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
/* a system call stub's call (`mov r10, rcx; mov eax, N; syscall; ret`): the arguments in the registers and on the stack where the
   caller left them, the number in EAX -- `return system_call(0xf, param_1, param_2, param_3, param_4);` jumps to the stub's code,
   which the function has (system_call_0xf, in its .text) */
#define system_call(n, ...) (((uint8 (*)())(void *)system_call_##n)(__VA_ARGS__))
/* a number the decompiler gives a structure as its value (`s = (S)(x);`): the structure whose first bytes are the number's */
static void *__cb_bits(uint8 v) { static uint8 b[8]; int i; b[0] = v; for (i = 1; i < 8; ++i) b[i] = 0; return b; }
/* the thread's environment block: the GS segment's base, which the block holds at 0x30 */
unsigned long long __readgsqword(unsigned long);
#pragma intrinsic(__readgsqword)
#define monitorx() ((void)0)
#define monitor() ((void)0)
/* a wait for a store to the monitored line (or a time out): what the function leaves is the same */
#define mwaitx() ((void)0)
#define mwait() ((void)0)
unsigned int _rdpid_u32(void);
#pragma intrinsic(_rdpid_u32)
#define readPID() ((uint8)_rdpid_u32())
/* the instructions Visual C++ has an intrinsic for: a random number and whether one was given, a performance counter, the limit
   of a segment, the processor's state saved, a breakpoint the bytes the decompiler could not read make */
int _rdrand64_step(unsigned long long *);
int _rdseed64_step(unsigned long long *);
unsigned long long __readpmc(unsigned long);
unsigned long __segmentlimit(unsigned long);
void _xsave(void *, unsigned long long);
void _xsavec(void *, unsigned long long);
void _xsaveopt(void *, unsigned long long);
void _xsaves(void *, unsigned long long);
void _xsave64(void *, unsigned long long);
void _xsavec64(void *, unsigned long long);
void _xsaveopt64(void *, unsigned long long);
void _xsaves64(void *, unsigned long long);
void _xrstor64(void const *, unsigned long long);
void __ud2(void);
void __cpuidex(int *, int, int);
#pragma intrinsic(_rdrand64_step, _rdseed64_step, __readpmc, __segmentlimit, _xsave, _xsavec, _xsaveopt, _xsaves, _xsave64, _xsavec64, _xsaveopt64, _xsaves64, _xrstor64, __ud2, __cpuidex)
static int __cb_random_valid;
static uint8 __cb_rdrand(void) { unsigned long long v = 0; __cb_random_valid = _rdrand64_step(&v); return v; }
static uint8 __cb_rdseed(void) { unsigned long long v = 0; __cb_random_valid = _rdseed64_step(&v); return v; }
#define rdrand() __cb_rdrand()
#define rdrandIsValid() __cb_random_valid
#define rdseed() __cb_rdseed()
#define rdseedIsValid() __cb_random_valid
#define rdpmc(counter) ((uint8)__readpmc((unsigned long)(counter)))
#define SegmentLimit(selector) ((uint4)__segmentlimit((unsigned long)(selector)))
#define xsave(p, mask) _xsave((void *)(p), (unsigned long long)(mask))
#define xsavec(p, mask) _xsavec((void *)(p), (unsigned long long)(mask))
#define xsaveopt(p, mask) _xsaveopt((void *)(p), (unsigned long long)(mask))
#define xsaves(p, mask) _xsaves((void *)(p), (unsigned long long)(mask))
#define xsave64(p, mask) _xsave64((void *)(p), (unsigned long long)(mask))
#define xsavec64(p, mask) _xsavec64((void *)(p), (unsigned long long)(mask))
#define xsaveopt64(p, mask) _xsaveopt64((void *)(p), (unsigned long long)(mask))
#define xsaves64(p, mask) _xsaves64((void *)(p), (unsigned long long)(mask))
#define xrstor64(p, mask) _xrstor64((void const *)(p), (unsigned long long)(mask))
#define halt_baddata() __ud2()
/* SSE operations on 16-byte values (each an array, or a pointer to one followed) whose result the decompiler gives a 16-byte
   variable: the 16 bytes made, which the variable is given */
typedef uint1 __cb_x16[16];
static __cb_x16 *__cb_x16_out(const uint1 *made) { static __cb_x16 out; int i; for (i = 0; i < 16; ++i) out[i] = made[i]; return &out; }
static __cb_x16 *__cb_shift16(const uint1 *a, int n, int lane, int right, int arith) {
    uint1 r[16]; int i, k;
    for (i = 0; i < 16; i += lane) {
        uint8 v = 0; for (k = 0; k < lane; ++k) v |= (uint8)a[i + k] << (8 * k);
        if (n >= 8 * lane) { v = arith && (v >> (8 * lane - 1)) ? ~(uint8)0 : 0; }
        else if (right && arith) { int8 sv = (int8)(v << (64 - 8 * lane)) >> (64 - 8 * lane); v = (uint8)(sv >> n); }
        else v = right ? v >> n : v << n;
        for (k = 0; k < lane; ++k) r[i + k] = (uint1)(v >> (8 * k));
    }
    return __cb_x16_out(r);
}
#define psraw(a, n) (*__cb_shift16((const uint1 *)(a), (int)(n), 2, 1, 1))
#define psrad(a, n) (*__cb_shift16((const uint1 *)(a), (int)(n), 4, 1, 1))
#define psrlw(a, n) (*__cb_shift16((const uint1 *)(a), (int)(n), 2, 1, 0))
#define psrld(a, n) (*__cb_shift16((const uint1 *)(a), (int)(n), 4, 1, 0))
#define psrlq(a, n) (*__cb_shift16((const uint1 *)(a), (int)(n), 8, 1, 0))
#define psllw(a, n) (*__cb_shift16((const uint1 *)(a), (int)(n), 2, 0, 0))
#define pslld(a, n) (*__cb_shift16((const uint1 *)(a), (int)(n), 4, 0, 0))
#define psllq(a, n) (*__cb_shift16((const uint1 *)(a), (int)(n), 8, 0, 0))
static __cb_x16 *__cb_pshufb(const uint1 *a, const uint1 *b) { uint1 r[16]; int i; for (i = 0; i < 16; ++i) r[i] = (b[i] & 0x80) ? 0 : a[b[i] & 15]; return __cb_x16_out(r); }
#define pshufb(a, b) (*__cb_pshufb((const uint1 *)(a), (const uint1 *)(b)))
static __cb_x16 *__cb_pmaddubsw(const uint1 *a, const uint1 *b) {
    uint1 r[16]; int i;
    for (i = 0; i < 8; ++i) {
        int v = (int)a[2 * i] * (int)(signed char)b[2 * i] + (int)a[2 * i + 1] * (int)(signed char)b[2 * i + 1];
        if (v > 32767) v = 32767; if (v < -32768) v = -32768;
        r[2 * i] = (uint1)v; r[2 * i + 1] = (uint1)((unsigned)v >> 8);
    }
    return __cb_x16_out(r);
}
#define pmaddubsw(a, b) (*__cb_pmaddubsw((const uint1 *)(a), (const uint1 *)(b)))
static __cb_x16 *__cb_psadbw(const uint1 *a, const uint1 *b) {
    uint1 r[16]; int i, h;
    for (i = 0; i < 16; ++i) r[i] = 0;
    for (h = 0; h < 16; h += 8) { unsigned sum = 0; for (i = h; i < h + 8; ++i) sum += a[i] > b[i] ? a[i] - b[i] : b[i] - a[i]; r[h] = (uint1)sum; r[h + 1] = (uint1)(sum >> 8); }
    return __cb_x16_out(r);
}
#define psadbw(a, b) (*__cb_psadbw((const uint1 *)(a), (const uint1 *)(b)))
static __cb_x16 *__cb_pmaddwd(const uint1 *a, const uint1 *b) {
    uint1 r[16]; int i, k;
    for (i = 0; i < 4; ++i) {
        int sum = 0;
        for (k = 0; k < 2; ++k) { const int at = 4 * i + 2 * k; sum += (int)(short)(a[at] | a[at + 1] << 8) * (int)(short)(b[at] | b[at + 1] << 8); }
        for (k = 0; k < 4; ++k) r[4 * i + k] = (uint1)((unsigned)sum >> (8 * k));
    }
    return __cb_x16_out(r);
}
#define pmaddwd(a, b) (*__cb_pmaddwd((const uint1 *)(a), (const uint1 *)(b)))
/* a cache line written back (it changes nothing the function's result shows), and the float constants C99 names */
void _mm_clflush(void const *);
void _mm_clflushopt(void const *);
void _mm_clwb(void const *);
#pragma intrinsic(_mm_clflush, _mm_clflushopt, _mm_clwb)
#define clflush(p) _mm_clflush((void const *)(p))
#define clflushopt(p) _mm_clflushopt((void const *)(p))
#define clwb(p) _mm_clwb((void const *)(p))
#ifndef INFINITY
#define INFINITY ((float)(1e+300 * 1e+300))
#endif
#ifndef NAN
#define NAN ((float)(INFINITY * 0.0F))
#endif
/* cpuid, as the decompiler reads it: the leaf's four registers in the order EAX, EBX, EDX, ECX (sub-leaf 0) */
static uint4 *__cb_cpuid(uint4 leaf) { static uint4 regs[4]; int r[4]; __cpuidex(r, (int)leaf, 0); regs[0] = (uint4)r[0]; regs[1] = (uint4)r[1]; regs[2] = (uint4)r[3]; regs[3] = (uint4)r[2]; return regs; }
#define cpuid(leaf) __cb_cpuid(leaf)
#define cpuid_basic_info(leaf) __cb_cpuid(leaf)
#define cpuid_Version_info(leaf) __cb_cpuid(leaf)
#define cpuid_cache_tlb_info(leaf) __cb_cpuid(leaf)
#define cpuid_serial_info(leaf) __cb_cpuid(leaf)
#define cpuid_Deterministic_Cache_Parameters_info(leaf) __cb_cpuid(leaf)
#define cpuid_MONITOR_MWAIT_Features_info(leaf) __cb_cpuid(leaf)
#define cpuid_Thermal_Power_Management_info(leaf) __cb_cpuid(leaf)
#define cpuid_Extended_Feature_Enumeration_info(leaf) __cb_cpuid(leaf)
#define cpuid_Direct_Cache_Access_info(leaf) __cb_cpuid(leaf)
#define cpuid_Architectural_Performance_Monitoring_info(leaf) __cb_cpuid(leaf)
#define cpuid_Extended_Topology_info(leaf) __cb_cpuid(leaf)
#define cpuid_Processor_Extended_States_info(leaf) __cb_cpuid(leaf)
#define cpuid_Quality_of_Service_info(leaf) __cb_cpuid(leaf)
#define cpuid_brand_part1_info(leaf) __cb_cpuid(leaf)
#define cpuid_brand_part2_info(leaf) __cb_cpuid(leaf)
#define cpuid_brand_part3_info(leaf) __cb_cpuid(leaf)
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
    static const std::regex piece(R"(\b([A-Za-z_]\w*(?:\[[^\]\n]*\]|(?:\.|->)[A-Za-z_]\w*)*)\._(\d+)_(\d+)_)");
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
    static const std::regex cast(R"(&\(\((\w+) \*\)(\w+|\([^()\n]*(?:\([^()\n]*\)[^()\n]*)*\))\)->field_0x([0-9a-f]+)\b)");
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
    static const std::regex byte(R"(^\s*(char|bool|byte)\s+(\w+)\s*(?:\[(\d+)\])?;\s*$)");   // (one byte each)
    static const std::regex pointer(R"(^\s*(\w+\s*\*+)\s*(\w+);\s*$)");
    std::map<std::string, Local> out;
    for (const std::string_view line : text::split(s, '\n')) {
        std::cmatch m;
        if (std::regex_match(line.data(), line.data() + line.size(), m, sized)) {
            const std::int64_t one = std::stoll(m[2].str());
            out[m[3].str()] = m[4].matched ? Local{m[1].str(), one * std::stoll(m[4].str()), false} : Local{m[1].str(), one, true};
        } else if (std::regex_match(line.data(), line.data() + line.size(), m, byte)) {
            out[m[2].str()] = m[3].matched ? Local{m[1].str(), std::stoll(m[3].str()), false} : Local{m[1].str(), 1, true};
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
    // (`xStack0000000000000000`: the place as a 64-bit offset from where the stack was at entry -- 0 is the return address's)
    if (rest.size() == 16 && std::ranges::all_of(rest, [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }))
        return static_cast<std::int64_t>(*text::parse_int<std::uint64_t>(rest, 16));
    const bool above = rest.starts_with("X");
    if (above) rest.remove_prefix(1);
    if (!rest.starts_with("_") || rest.size() < 2) return std::nullopt;
    const std::optional<std::int64_t> n = text::parse_int<std::int64_t>(rest.substr(1), 16);
    if (!n) return std::nullopt;
    return above ? *n : -*n;
}

// Every whole-word occurrence of `word` in s replaced by `with` (a word is letters, digits and `_`).
std::string replace_word(const std::string& s, const std::string& word, const std::string& with) {
    std::string out;
    std::size_t from = 0;
    for (std::size_t k = s.find(word); k != std::string::npos; k = s.find(word, k + word.size())) {
        const std::size_t end = k + word.size();
        if ((k > 0 && is_word(s[k - 1])) || (end < s.size() && is_word(s[end]))) continue;
        out.append(s, from, k - from);
        out += with;
        from = end;
    }
    out.append(s, from, std::string::npos);
    return out;
}

// The declarations of a function's text: the lines from its body's `{` to the first blank one (the decompiler declares every local
// there, before any statement), as [begin, end) offsets.
std::pair<std::size_t, std::size_t> declaration_block(const std::string& s) {
    const std::size_t at = s.size() - past_notes(s).size();
    const std::size_t brace = s.find("\n{", at);
    if (brace == std::string::npos) return {0, 0};
    const std::size_t begin = s.find('\n', brace + 1);
    if (begin == std::string::npos) return {0, 0};
    for (std::size_t k = begin + 1; k < s.size();) {
        const std::size_t e = std::min(s.find('\n', k), s.size());
        if (text::trim(std::string_view(s).substr(k, e - k)).empty()) return {begin + 1, k};
        k = e + 1;
    }
    return {begin + 1, s.size()};
}

// The stack variables a text declares, by where they are (`xunknown2 xStack_48;`, `s_foo xStack_40;`, `uint1 axStack_38 [4];`),
// whatever their type: a variable whose size locals_of does not know is still a variable there.
std::map<std::int64_t, std::string> declared_stack(const std::string& s) {
    static const std::regex decl(R"(^\s*\w[\w ]*\**\s*\b(\w*Stack(?:X?_[0-9a-f]+|[0-9a-f]{16}))\s*(?:\[\d+\])?;\s*$)");
    std::map<std::int64_t, std::string> out;
    const auto [begin, end] = declaration_block(s);
    for (const std::string_view line : text::split(std::string_view(s).substr(begin, end - begin), '\n')) {
        std::cmatch m;
        if (std::regex_match(line.data(), line.data() + line.size(), m, decl))
            if (const auto at = stack_place(m[1].str())) out[*at] = m[1].str();
    }
    return out;
}

// `_xStack_48 = 0;` where xStack_48 is two bytes and nothing the text names lies right after it (a gap, an array, a variable
// only partly in the value): the variables within the value's bytes are given one storage as wide as the value
// (`uint8 __cb_frame_xStack_48;`), each read and written as its bytes of it -- so the value written whole reaches each part, and
// a part's address (`&xStack_48`, handed to a callee) is that of the bytes after it too, as on the original's stack. Nothing when
// a variable lies partly inside: it cannot be both its own and the storage's.
std::optional<std::string> frame_storage(const std::string& s, const std::string& name, std::int64_t start, std::int64_t width,
                                         std::set<std::string>& merged) {
    const std::map<std::string, Local> locals = locals_of(s);
    const std::map<std::int64_t, std::string> declared = declared_stack(s);
    struct Part {
        std::string name;
        std::int64_t offset;
        Local local;
    };
    std::vector<Part> parts;
    for (const auto& [at, n] : declared) {
        const auto l = locals.find(n);
        if (at >= start + width) continue;
        if (l == locals.end()) {
            if (at < start) continue;   // (one before whose size is not known: taken as ending before)
            return std::nullopt;
        }
        if (at + l->second.size <= start) continue;
        if (at < start || at + l->second.size > start + width || merged.contains(n)) return std::nullopt;
        parts.push_back({n, at - start, l->second});
    }
    if (parts.empty() || parts.front().offset != 0 || parts.front().name != name) return std::nullopt;
    const std::string storage = "__cb_frame_" + name;
    const std::int64_t bytes = width <= 1 ? 1 : width <= 2 ? 2 : width <= 4 ? 4 : 8;
    static const std::regex decl(R"(^(\s*)\w[\w ]*\**\s*\b(\w+)\s*(?:\[\d+\])?;\s*$)");
    std::vector<std::string> lines;
    bool declared_storage = false;
    const auto [begin, end] = declaration_block(s);
    std::size_t offset = 0;
    for (const std::string_view line : text::split(s, '\n')) {
        const bool in_block = offset >= begin && offset < end;
        offset += line.size() + 1;
        std::cmatch m;
        if (in_block && std::regex_match(line.data(), line.data() + line.size(), m, decl) &&
            std::ranges::any_of(parts, [&](const Part& p) { return p.name == m[2].str(); })) {
            if (!declared_storage) lines.push_back(std::format("{}uint{} {};", m[1].str(), bytes, storage));
            declared_storage = true;
            continue;
        }
        lines.emplace_back(line);
    }
    std::string out = replace_word(text::join(lines, "\n"), "_" + name, storage);
    for (const Part& p : parts) {
        const Local& l = p.local;
        std::string place;
        if (l.scalar) {
            place = std::format("(*({} *)((uint1 *)&{} + {}))", l.type, storage, p.offset);
        } else {   // (an array: the array there, which is what its name was)
            const std::int64_t one = l.type.empty() ? 1 : std::max(l.type.back() - '0', 1);
            place = std::format("(*({} (*)[{}])((uint1 *)&{} + {}))", l.type, l.size / one, storage, p.offset);
            const std::regex as_number("\\((u?int[1248]|xunknown[1248])\\)" + p.name + "\\b");
            out = std::regex_replace(out, as_number, std::format("(*($1 *)((uint1 *)&{} + {}))", storage, p.offset));
        }
        out = replace_word(out, p.name, place);
        merged.insert(p.name);
    }
    return out;
}

// `stack0xffffffffffffff44`, the decompiler's name for bytes of the frame where no one variable is (part of one, part of the
// next), as a value of a CONCAT: those bytes, put together from the variables they are in.
std::string frame_reads(const std::string& s) {
    if (s.find("stack0x") == std::string::npos) return s;
    const std::map<std::string, Local> locals = locals_of(s);
    std::map<std::int64_t, std::pair<std::string, Local>> by_place;
    for (const auto& [n, l] : locals)
        if (const auto at = stack_place(n)) by_place[*at] = {n, l};
    std::map<std::string, std::int64_t> widths;   // a stack0x name -> its width, the CONCAT's digit for its side
    static const std::regex first(R"(CONCAT([1-8])([1-8])\((stack0x[0-9a-f]{16}),)");
    static const std::regex second(R"(CONCAT([1-8])([1-8])\([^,()]*(?:\([^()]*\))?[^,()]*,(stack0x[0-9a-f]{16})\))");
    for (auto it = std::sregex_iterator(s.begin(), s.end(), first); it != std::sregex_iterator(); ++it)
        widths.try_emplace((*it)[3].str(), std::stoll((*it)[1].str()));
    for (auto it = std::sregex_iterator(s.begin(), s.end(), second); it != std::sregex_iterator(); ++it)
        widths.try_emplace((*it)[3].str(), std::stoll((*it)[2].str()));
    std::string out = s;
    for (const auto& [word, width] : widths) {
        const std::int64_t at = static_cast<std::int64_t>(*text::parse_int<std::uint64_t>(std::string_view(word).substr(7), 16));
        std::string value;
        bool covered = true;
        for (std::int64_t b = at; b < at + width;) {
            auto l = by_place.upper_bound(b);
            if (l == by_place.begin()) {
                covered = false;
                break;
            }
            --l;
            const std::int64_t place = l->first, size = l->second.second.size;
            if (b >= place + size) {
                covered = false;
                break;
            }
            const std::int64_t take = std::min(place + size, at + width) - b;   // (the bytes of this variable in the value)
            const std::int64_t n = take >= 8 ? 8 : take >= 4 ? 4 : take >= 2 ? 2 : 1;
            value += std::format("{}((uint8)*(uint{} *)((uint1 *)&{} + {}) << {})", value.empty() ? "" : " | ", n, l->second.first, b - place,
                                 8 * (b - at));
            b += n;
        }
        if (covered) out = replace_word(out, word, "(" + value + ")");
    }
    return out;
}

// `_xStack_30`, the decompiler's name for the bytes of stack variables side by side read or written as one (`_xStack_30 =
// CONCAT44(1,x)` writes xStack_30 and xStack_2c): each variable written its part of the value, or the value read as the parts put
// together. The width is the one the text gives it (CONCAT24: 6 bytes), else 8; the variables must lie end to end over it.
std::string stack_pieces(const std::string& s) {
    static const std::regex piece(R"((^|[^\w])_(\w*Stack(?:X?_[0-9a-f]+|[0-9a-f]{16}))\b)");
    if (s.find("Stack") == std::string::npos) return s;
    std::set<std::string> names;
    for (auto it = std::sregex_iterator(s.begin(), s.end(), piece); it != std::sregex_iterator(); ++it) names.insert((*it)[2].str());
    if (names.empty()) return s;
    const std::map<std::string, Local> locals = locals_of(s);
    std::map<std::int64_t, std::string> by_place;
    for (const auto& [n, l] : locals)
        if (const auto at = stack_place(n)) by_place[*at] = n;
    std::string out = s;
    std::set<std::string> merged_names;   // (the locals frame_storage has put in a storage of their own)
    for (const std::string& name : names) {
        if (!locals.contains(name) || merged_names.contains(name)) continue;
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
        if (!whole || parts.size() < 2) {
            if (std::optional<std::string> merged = frame_storage(out, name, start, width, merged_names)) out = std::move(*merged);
            continue;
        }
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
    std::set<std::string> to_16;   // the pointers to 16 bytes the text declares (`xunknown1 (*param_4) [16]`)
    static const std::regex pointer_16(R"(\(\*(\w+)\) ?\[16\])");
    for (auto it = std::sregex_iterator(s.begin(), s.end(), pointer_16); it != std::sregex_iterator(); ++it) to_16.insert((*it)[1].str());
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
        // (a pointer to 16 bytes followed: `*param_4`, the parameter `xunknown1 (*param_4) [16]`)
        const bool followed = i < s.size() && s[i] == '*';
        std::size_t e = followed ? i + 1 : i;
        while (e < s.size() && is_word(s[e])) ++e;
        const std::string name = s.substr(followed ? i + 1 : i, e - i - (followed ? 1 : 0));
        if (followed ? !to_16.contains(name) : !wide.contains(name)) return std::nullopt;
        return std::pair{std::format("__cb_ld128({}{})", followed ? "*" : "", name), e};
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
        if (star < s.size() && s[star] == ',') {   // (bytes of a 16-byte value: `SUB161(*param_4,1)`)
            const std::size_t close = s.find(')', star);
            if (close == std::string::npos) continue;
            const auto o = text::parse_int<std::int64_t>(text::trim(std::string_view(s).substr(star + 1, close - star - 1)), 0);
            if (!o || *o < 0 || *o + (width - '0') > 16) continue;
            out.append(s, at, k - at);
            out += std::format("((uint{})__cb_bytes128({}, {}))", width, a->first, *o);
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
    static const std::regex assign(R"(^(\s*)(\w+) = (.* \* .*);$)");
    static const std::regex widened(R"(\b[ZS]EXT\d+16\()");
    std::vector<std::string> out;
    for (const std::string_view full : text::split(s, '\n')) {
        std::string line(full);
        std::smatch m;
        if (std::regex_match(line, m, assign) && wide.contains(m[2].str()) && std::regex_search(m[3].str(), widened)) {
            const std::string value = m[3].str();
            if (words_of(value).contains(m[2].str()))   // (`axVar1 = axVar1 * ZEXT416(u)`: both halves of the old value first)
                line = std::format("{0}{{ uint8 __cb_lo = SUB168({2},0); uint8 __cb_hi = SUB168({2},8); *(uint8 *){1} = __cb_lo; "
                                   "*(uint8 *)((uint1 *){1} + 8) = __cb_hi; }}",
                                   m[1].str(), m[2].str(), value);
            else
                line = std::format("{0}*(uint8 *){1} = SUB168({2},0); *(uint8 *)((uint1 *){1} + 8) = SUB168({2},8);", m[1].str(), m[2].str(), value);
        }
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
    static const std::regex cast_array(R"(^\*\((?:xunknown|uint|int)([1248]) \(\*\) ?\[(\d+)\]\))");
    if (arrays.empty() && s.find("(*) [") == std::string::npos) return s;
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
            // (a place cast to an array given whole: `*(xunknown1 (*) [4])(param_3 + 0x16) = <an array>;`)
            else if (std::smatch c; std::regex_search(place, c, cast_array) && c.position(0) == 0 && place.find_first_of("=;") == std::string::npos)
                line = std::format("{}__movsb((uint1 *)&({}), (uint1 *)&({}), {});", line.substr(0, line.find_first_not_of(' ')), place, value,
                                   std::stoll(c[2].str()) * std::max<std::int64_t>(c[1].str().back() - '0', 1));
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

// `MXCSR = x;`, the SSE control register written (ldmxcsr): the intrinsic that writes it (MXCSR itself reads it).
std::string mxcsr_writes(const std::string& s) {
    if (s.find("MXCSR = ") == std::string::npos) return s;
    static const std::regex write(R"(\bMXCSR = ([^;\n]+);)");
    return std::regex_replace(s, write, "_mm_setcsr((unsigned int)($1));");
}

// `int8 unaff_GS_OFFSET;`, the decompiler's name for the GS segment's base (the thread's environment block, which `gs:[0x30]` reads
// `*(unaff_GS_OFFSET + 0x30)`), declared with no value: given the base, which the block holds at 0x30.
std::string gs_base(const std::string& s) {
    if (s.find("_GS_OFFSET") == std::string::npos) return s;
    static const std::regex decl(R"((^|\n)(\s*)(\w[\w ]*?\s*\**)\s*\b((?:unaff|in)_GS_OFFSET);)");
    std::string out;
    std::size_t at = 0;
    for (auto it = std::sregex_iterator(s.begin(), s.end(), decl); it != std::sregex_iterator(); ++it) {
        out.append(s, at, static_cast<std::size_t>(it->position()) - at);
        const std::string type(text::trim((*it)[3].str()));
        out += std::format("{}{}{}{}{} = ({})__readgsqword(0x30);", (*it)[1].str(), (*it)[2].str(), type, type.ends_with("*") ? "" : " ",
                           (*it)[4].str(), type);
        at = static_cast<std::size_t>(it->position() + it->length());
    }
    out.append(s, at, std::string::npos);
    return out;
}

// The names a text declares as a structure held by value (`s_f_param_1 param_1`, `s_f_sVar2 sStack_80;`), not a pointer to one.
std::set<std::string> structure_values(const std::string& s) {
    static const std::regex value(R"((?:^|[\n(,])\s*(s_\w+)\s+(\w+)\s*(?=[;,)]))");
    std::set<std::string> out;
    const std::size_t body = s.find('{');
    for (auto it = std::sregex_iterator(s.begin(), s.end(), value); it != std::sregex_iterator(); ++it) {
        if (static_cast<std::size_t>(it->position()) > body && s.compare(static_cast<std::size_t>(it->position()), 1, "(") == 0) continue;
        out.insert((*it)[2].str());
    }
    return out;
}

// A number given a structure as its value (`sStack_80 = (S)((uint8)x << 0x20);`, as C has no such cast): the structure whose bytes
// are the number's (`*(S *)__cb_bits(..)`, the header's). And a structure the decompiler holds by value but the code uses as the
// pointer it is (`*(uint8 *)param_1`, param_1 a structure in a register): its bits as that pointer.
std::string structure_casts(const std::string& s) {
    if (s.find("(s_") == std::string::npos) return s;
    std::string out;
    std::size_t at = 0;
    for (std::size_t k = s.find("(s_"); k != std::string::npos; k = s.find("(s_", k + 1)) {
        if (k < at) continue;
        std::size_t e = k + 1;
        while (e < s.size() && is_word(s[e])) ++e;
        if (e >= s.size() || s[e] != ')') continue;   // (a pointer's cast: `(S *)`)
        std::size_t open = e + 1;
        while (open < s.size() && (s[open] == ' ' || s[open] == '\n')) ++open;
        if (open >= s.size() || s[open] != '(') continue;
        int depth = 0;
        std::size_t close = open;
        for (; close < s.size(); ++close) {
            if (s[close] == '(') ++depth;
            else if (s[close] == ')' && --depth == 0) break;
        }
        if (close >= s.size()) continue;
        out.append(s, at, k - at);
        out += std::format("(*({} *)__cb_bits((uint8){}))", s.substr(k + 1, e - k - 1), s.substr(open, close - open + 1));
        at = close + 1;
    }
    out.append(s, at, std::string::npos);
    const std::set<std::string> values = structure_values(out);
    for (const std::string& v : values) {
        const std::regex as_pointer("(\\(\\w+ \\*+\\))" + v + "\\b(?!\\.|->)");
        out = std::regex_replace(out, as_pointer, "$1*(uint8 *)&" + v);
    }
    return out;
}

// A function pointer moved by a number (`pcVar3 + -1`, which C does not do to a function's address): the address moved by bytes.
std::string code_pointer_arithmetic(const std::string& s) {
    if (s.find("code *") == std::string::npos) return s;
    std::string out = s;
    for (const auto& [n, l] : locals_of(s)) {
        if (text::join(text::split_ws(l.type), "") != "code*") continue;
        const std::regex moved("\\b" + n + " ([+-]) (-?(?:0x[0-9a-f]+|\\d+))\\b");
        out = std::regex_replace(out, moved, "(code *)((uint1 *)" + n + " $1 $2)");
    }
    return out;
}

// A local the decompiler declares as a number and follows as a pointer with no index or field (`*xStack_50 == uVar6`): followed
// as a pointer to what it is compared with or given to (a uint2 there: `*(uint2 *)xStack_50`), the local staying the number it is,
// so that the arithmetic on it (`xStack_50 + 1`) moves it by bytes as before.
std::string followed_numbers(const std::string& s) {
    const std::map<std::string, Local> locals = locals_of(s);
    std::string out = s;
    for (const auto& [n, l] : locals) {
        if (!l.scalar || l.size != 8 || l.type.find('*') != std::string::npos) continue;
        if (out.find("*" + n) == std::string::npos) continue;
        const std::regex follow("(^|[^\\w])\\*" + n + "\\b(?!\\s*[\\[(])");
        std::string done;
        std::size_t at = 0;
        bool any = false;
        for (auto it = std::sregex_iterator(out.begin(), out.end(), follow); it != std::sregex_iterator(); ++it) {
            const std::size_t begin = static_cast<std::size_t>(it->position()) + (*it)[1].length();
            const std::size_t end = static_cast<std::size_t>(it->position() + it->length());
            if (begin > 0 && out[begin - 1] == '*') continue;   // (a product: `a * *b` is written `a * *b`, `a**b` is not the decompiler's)
            // what it meets: `== W`, `!= W`, `= W` after; `W = ` or `W == ` before
            const std::size_t line_start = out.rfind('\n', begin) == std::string::npos ? 0 : out.rfind('\n', begin) + 1;
            const std::size_t line_end = std::min(out.find('\n', end), out.size());
            const std::string left = out.substr(line_start, begin - line_start), right = out.substr(end, line_end - end);
            static const std::regex after(R"(^\s*(?:==|!=|=)\s*(\w+))");
            static const std::regex before(R"((\w+)\s*(?:==|!=|=)\s*\(*$)");
            std::smatch m;
            std::string type = "xunknown8";
            const auto typed = [&](const std::string& w) {
                const auto o = locals.find(w);
                if (o != locals.end() && o->second.scalar && o->second.size <= 8 && o->second.type.find('*') == std::string::npos) type = o->second.type;
            };
            if (std::regex_search(right, m, after)) typed(m[1].str());
            if (type == "xunknown8" && std::regex_search(left, m, before)) typed(m[1].str());
            done.append(out, at, begin - at);
            done += std::format("(*({} *){})", type, n);
            at = end;
            any = true;
        }
        if (!any) continue;
        done.append(out, at, std::string::npos);
        out = std::move(done);
    }
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
    return mxcsr_writes(followed_numbers(code_pointer_arithmetic(structure_casts(gs_base(crc32_widths(array_pointer_result(pointer_locals(single_labels(close_labels(
        array_assignments(products_128(wide_assignments(wide_products(unprototyped_self_calls(address_of_unnamed_fields(
            frame_reads(stack_pieces(partial_access(std::string(text::trim(text)))))))))))))))))))));
}

// The number a system call stub puts in EAX (`mov r10, rcx; mov eax, N; ... syscall`): the last `mov eax, imm32` before the
// first `syscall` of its first 64 bytes.
std::optional<std::uint32_t> system_call_number(ByteView code) {
    std::optional<std::uint32_t> number;
    for (std::size_t k = 0; k + 1 < code.size(); ++k) {
        if (code[k] == 0x0F && code[k + 1] == 0x05) return number;
        if (code[k] == 0xB8 && k + 5 <= code.size()) {
            number = static_cast<std::uint32_t>(code[k + 1] | code[k + 2] << 8 | code[k + 3] << 16) | static_cast<std::uint32_t>(code[k + 4]) << 24;
            k += 4;   // (past the number: NtCreateLowBoxToken's is 0xb8, whose byte is the instruction's own)
        }
    }
    return std::nullopt;
}

// A system call stub's text (`xunknown8 NtClose(void)` ... `syscall(); return 0xf;`: the decompiler sees neither the arguments
// the call passes on nor what it gives back, and takes EAX, the number, for the result) written as what the stub does: it takes the
// four register arguments and gives back the call's result, the call made by the stub of its number (`system_call(0xf, ...)`).
// The rest stays: the test of how the system is entered, and the other way in (`int 2Eh`).
std::string system_call_text(const std::string& s, std::uint32_t number) {
    const std::string_view head = past_notes(s);
    const std::size_t at = s.size() - head.size();
    const std::size_t open = s.find('(', at), brace = s.find('{', at);
    if (open == std::string::npos || brace == std::string::npos || open > brace) return s;
    const std::size_t close = s.rfind(')', brace);
    if (close == std::string::npos || text::trim(std::string_view(s).substr(open + 1, close - open - 1)) != "void") return s;
    const std::string args = "param_1,param_2,param_3,param_4";
    std::string out = s.substr(0, open + 1) + "uint8 param_1,uint8 param_2,uint8 param_3,uint8 param_4" + s.substr(close);
    static const std::regex call_return(R"(syscall\(\);(\s*)return [^;\n]*;)");
    static const std::regex call(R"(\bsyscall\(\);)");
    const std::string made = std::format("system_call(0x{:x},{})", number, args);
    out = std::regex_replace(out, call_return, "return " + made + ";");
    out = std::regex_replace(out, call, made + ";");
    // (the stub's code, the function's own: `mov r10, rcx; mov eax, N; syscall; ret`)
    const std::size_t body = out.find("{\n", open);
    if (body == std::string::npos) return s;
    out.insert(body + 2, std::format("  __declspec(allocate(\".text\")) static const unsigned char system_call_0x{:x}[] = "
                                     "{{0x4c, 0x8b, 0xd1, 0xb8, 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x0f, 0x05, 0xc3}};\n",
                                     number, number & 0xFF, (number >> 8) & 0xFF, (number >> 16) & 0xFF, number >> 24));
    return out;
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
    std::map<std::string, Va> va_of;
    for (const auto& [va, name] : got.by_va) va_of.try_emplace(name, va);
    ModuleX64 m;
    for (const std::string& n : got.order) {
        const std::string* text = got.texts.find(n);
        if (text == nullptr) continue;
        if (const auto it = got.notes.find(n); it != got.notes.end()) {
            for (const core16::GlobalNote& g : it->second.globals) sizes[g.address].insert(g.bytes);
            structs.insert(structs.end(), it->second.structs.begin(), it->second.structs.end());
        }
        std::string body = x64_function_text(*text);
        if (body.find("syscall();") != std::string::npos)
            if (const auto va = va_of.find(n); va != va_of.end())
                if (const auto code = image.read_rva(va->second - image.header().image_base, 64))
                    if (const std::optional<std::uint32_t> number = system_call_number(*code)) {
                        body = system_call_text(body, *number);
                    }
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
    std::set<std::string> indexed;   // the globals the text indexes (`axRam..[1]`)
    std::set<std::string> funcs, called_through, calls;
    bool stack_pointer = false;   // (the C reads the stack pointer, as the decompiler names it)
    for (const std::string& n : order) {
        const std::string& t = out.at(n);
        for (auto it = std::sregex_iterator(t.begin(), t.end(), ram); it != std::sregex_iterator(); ++it) {
            rams.emplace((*it)[1].str(), (*it)[2].str());
            if (static_cast<std::size_t>(it->position() + it->length()) < t.size() && t[static_cast<std::size_t>(it->position() + it->length())] == '[')
                indexed.insert(it->str());
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
        // (an array the text indexes, `axRam00000001801d8a10[1]`: the bytes there, as many as the decompiler says it has)
        if (prefix == "ax" && indexed.contains(name)) {
            std::int64_t bytes = 16;
            if (const auto it = sizes.find(at); it != sizes.end()) bytes = std::max(bytes, *std::ranges::max_element(it->second));
            macros.push_back(std::format("#define {} (*(xunknown1 (*)[{}])0x{})", name, bytes, digits));
            continue;
        }
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
                                                     "_byteswap_ulong", "_byteswap_uint64", "strncmp", "strncpy", "strncat", "wcsncmp",
                                                     "wcsncpy", "wcsncat", "acos", "asin", "atan", "atan2", "ceil", "cos", "cosh", "exp",
                                                     "fabs", "floor", "fmod", "log", "log10", "pow", "sin", "sinh", "sqrt", "tan", "tanh"};
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
