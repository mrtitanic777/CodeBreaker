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

// `v._4_4_`, the decompiler's name for bytes of a variable (4 bytes at offset 4 of v), as C: `(*(uint4 *)((uint1 *)&v + 4))`. Sizes
// that are no C type (3, 5, 6, 7 bytes) are left as they are: the function does not compile.
std::string partial_access(std::string s) {
    static const std::regex piece(R"(\b([A-Za-z_]\w*)\._(\d+)_(\d+)_)");
    std::string out;
    std::size_t at = 0;
    for (auto it = std::sregex_iterator(s.begin(), s.end(), piece); it != std::sregex_iterator(); ++it) {
        const std::string size = (*it)[3].str();
        if (size != "1" && size != "2" && size != "4" && size != "8") continue;
        out.append(s, at, static_cast<std::size_t>(it->position()) - at);
        out += std::format("(*(uint{} *)((uint1 *)&{} + {}))", size, (*it)[1].str(), (*it)[2].str());
        at = static_cast<std::size_t>(it->position() + it->length());
    }
    out.append(s, at, std::string::npos);
    return out;
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
        while (after < s.size() && s[after] == ' ') ++after;
        if (after < s.size() && s[after] == '(') out.emplace(s.substr(k, e - k));
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
        std::string body = partial_access(std::string(text::trim(*text)));
        const std::string declared = declaration_of(body);
        // (no C: left to the original -- nor C the decompiler says it could not recover: a jump through a table or an import's slot
        // it did not follow is a call through a pointer nothing sets)
        if (declared.empty() || internal::has(body, "UNRECOVERED_JUMPTABLE")) continue;
        // Every function leaves something in RAX, and the decompiler may find a caller using it where it found the function to return
        // nothing: a `void` function is written to return uint8, its result not compared.
        if (body.starts_with("void ")) {
            body = "uint8 " + body.substr(5);
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
    for (const std::string& n : order) {
        const std::string& t = out.at(n);
        for (auto it = std::sregex_iterator(t.begin(), t.end(), ram); it != std::sregex_iterator(); ++it) {
            rams.emplace((*it)[1].str(), (*it)[2].str());
            // (a global called through holds a function's address)
            const std::size_t end = static_cast<std::size_t>(it->position() + it->length());
            if (it->position() >= 2 && t.compare(static_cast<std::size_t>(it->position()) - 2, 2, "(*") == 0 && t.compare(end, 2, ")(") == 0)
                called_through.insert(it->str());
        }
        for (auto it = std::sregex_iterator(t.begin(), t.end(), func); it != std::sregex_iterator(); ++it) funcs.insert((*it)[1].str());
        for (const std::string& w : called_words(t)) calls.insert(w);
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
