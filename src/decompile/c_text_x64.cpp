// decompile/c_text_x64.cpp -- an x64 module's decompiled functions made compilable by Visual C++'s x64 compiler (see c_text.hpp).
//
// Each function is compiled on its own and linked into an image of its own, which is run beside the original (verify/lanes/win64):
// so what the rebuilt code reaches that it does not have itself is the original's, at the original's address. A global is a
// place in the original's data (`uRam0000000140034400` is `*(uint8 *)0x140034400`), an import is a call through the original's
// import slot, and a function that is not rebuilt -- the runtime library's, one the decompiler did not write -- is called where
// the original has it. Everything the functions share is declared in the one header every function is compiled against.
#include <algorithm>
#include <format>
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
static uint8 __cb_rd3(const void *p) { return (uint8)*(const uint2 *)p | ((uint8)((const uint1 *)p)[2] << 16); }
static uint8 __cb_rd5(const void *p) { return (uint8)*(const uint4 *)p | ((uint8)((const uint1 *)p)[4] << 32); }
static uint8 __cb_rd6(const void *p) { return (uint8)*(const uint4 *)p | ((uint8)*(const uint2 *)((const uint1 *)p + 4) << 32); }
static uint8 __cb_rd7(const void *p) { return __cb_rd6(p) | ((uint8)((const uint1 *)p)[6] << 48); }
static uint8 __cb_wr3(void *p, uint8 v) { *(uint2 *)p = (uint2)v; ((uint1 *)p)[2] = (uint1)(v >> 16); return v; }
static uint8 __cb_wr5(void *p, uint8 v) { *(uint4 *)p = (uint4)v; ((uint1 *)p)[4] = (uint1)(v >> 32); return v; }
static uint8 __cb_wr6(void *p, uint8 v) { *(uint4 *)p = (uint4)v; *(uint2 *)((uint1 *)p + 4) = (uint2)(v >> 32); return v; }
static uint8 __cb_wr7(void *p, uint8 v) { __cb_wr6(p, v); ((uint1 *)p)[6] = (uint1)(v >> 48); return v; }
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
            out += std::format("(*(uint{} *)((uint1 *)&{} + {}))", size, (*it)[1].str(), (*it)[2].str());
            at = end;
        } else if (size == "3" || size == "5" || size == "6" || size == "7") {
            out.append(s, at, begin - at);
            const std::string place = std::format("((uint1 *)&{} + {})", (*it)[1].str(), (*it)[2].str());
            std::size_t eq = end;
            while (eq < s.size() && s[eq] == ' ') ++eq;
            const std::size_t semi = s.find(';', eq);
            if (eq + 1 < s.size() && s[eq] == '=' && s[eq + 1] == ' ' && semi != std::string::npos) {   // (an assignment: a statement)
                out += std::format("__cb_wr{}({}, {})", size, place, s.substr(eq + 2, semi - eq - 2));
                at = semi;
            } else {
                out += std::format("__cb_rd{}({})", size, place);
                at = end;
            }
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
    return out;
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

// A label at the end of a block (`code_r0x00014001134f:` before the `}`) needs a statement after it, before C23.
std::string close_labels(const std::string& s) {
    const std::vector<std::string_view> lines = text::split(s, '\n');
    std::vector<std::string> out;
    for (std::size_t k = 0; k < lines.size(); ++k) {
        std::string line(lines[k]);
        const std::string_view t = text::trim(lines[k]);
        if (t.size() > 1 && t.back() == ':' && t.find(' ') == std::string_view::npos && !t.starts_with("default")) {
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
            if (const auto a = arrays.find(std::string(t.substr(0, eq))); a != arrays.end())
                line = std::format("{}__movsb((uint1 *){}, (uint1 *)&({}), {});", line.substr(0, line.find_first_not_of(' ')), a->first,
                                   std::string(t.substr(eq + 3, t.size() - eq - 4)), a->second);
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
        std::string body = close_labels(array_assignments(wide_products(unprototyped_self_calls(address_of_unnamed_fields(partial_access(std::string(text::trim(*text))))))));
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
    for (const std::string& w : calls) {
        if (own.contains(w)) continue;
        if (const auto s = slots.find(w); s != slots.end()) {
            macros.push_back(std::format("#define {} (*(code **)0x{:x})", w, s->second));
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
                         text::join(decls, "\n") + "\n";
    if (cplusplus) header = std::string(kCppOpen) + header + "#ifdef __cplusplus\n}\n" + cpp.declarations + "#endif\n";
    OrderedMap<std::uint64_t, std::string> by_address;
    for (const auto& [va, n] : got.by_va)
        if (own.contains(n)) by_address.insert_or_assign(static_cast<std::uint64_t>(va), renamed.contains(n) ? renamed.at(n) : n);
    m.made = translate::l2::Made::of(text::lower(module.filename().string()), header, out, order, by_address);
    m.image_base = image.header().image_base;
    return m;
}

}  // namespace cb::decompile::c_text
