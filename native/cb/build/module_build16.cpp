// cb/build/module_build16.cpp -- link a 16-bit Windows module (an NE image) from its reconstruction.
//
// Each function is compiled into a code segment named for the original segment it came from; the module's data is C (build/data), the
// functions that cannot be C are assembly (module_asm16), and the linker is told the original's segments, options and exports. A
// function the compiler rejects, or whose segment does not fit, becomes a stand-in and the module is linked again.

#include <algorithm>
#include <cstdio>
#include <format>
#include <map>
#include <set>

#include "cb/base/cache.hpp"
#include "cb/build/module_internal.hpp"
#include "cb/build/msc6.hpp"
#include "cb/core/fs.hpp"
#include <nlohmann/json.hpp>

#include "cb/core/json.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/describe.hpp"
#include "cb/translate/l2.hpp"

namespace cb::build::module {

using namespace internal;

namespace {

namespace desc = decompile::describe;
namespace ct = decompile::c_text;

// The linker's options for a segment as the original's flags say it was loaded: {flag bit, when set, when clear}.
struct SegmentOption {
    std::uint16_t bit;
    const char* set;
    const char* clear;
};
constexpr SegmentOption kSegmentOptions[] = {{0x40, "preload", "loadoncall"}, {0x10, "moveable", "fixed"}, {0x1000, "discardable", ""}, {0x20, "shared", "nonshared"}};

// A name of the first `n` characters... of a call's text: how many arguments the call at `open` (the index of its `(`) passes.
std::size_t count_call_arguments(std::string_view text, std::size_t open) {
    std::size_t depth = 0, arguments = 0;
    bool any = false;
    char quote = 0;
    for (std::size_t i = open; i < text.size(); ++i) {
        const char c = text[i];
        if (quote != 0) {
            if (c == '\\') ++i;
            else if (c == quote) quote = 0;
            continue;
        }
        if (c == '"' || c == '\'') {
            quote = c;
            any = true;
        } else if (c == '(' || c == '[' || c == '{') {
            if (++depth > 1) any = true;
        } else if (c == ')' || c == ']' || c == '}') {
            if (--depth == 0) return any ? arguments + 1 : 0;
        } else if (c == ',' && depth == 1) {
            ++arguments;
        } else if (!text::is_space(c)) {
            any = true;
        }
    }
    return any ? arguments + 1 : 0;
}

std::string escape_regex(std::string_view s) {
    std::string out;
    for (char c : s) {
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')) out += '\\';
        out += c;
    }
    return out;
}

bool is_hex_lower(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }
bool is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

// A call the decompiler left as an address (`func_0x6e001000`) goes to the function reconstructed at that address -- when `lookup` says
// it can be reached; every other is left as it was.
std::string name_addressed_calls(std::string_view text, const std::function<std::optional<std::string>(std::uint64_t)>& lookup) {
    constexpr std::string_view kPrefix = "func_0x";
    std::string out;
    std::size_t at = 0;
    while (at < text.size()) {
        const std::size_t found = text.find(kPrefix, at);
        if (found == std::string_view::npos) break;
        const std::size_t digits = found + kPrefix.size();
        const bool boundary_before = found == 0 || !is_word(text[found - 1]);
        const bool eight = digits + 8 <= text.size() && std::all_of(text.begin() + static_cast<std::ptrdiff_t>(digits), text.begin() + static_cast<std::ptrdiff_t>(digits + 8), is_hex_lower);
        const bool boundary_after = digits + 8 >= text.size() || !is_word(text[digits + 8]);
        out.append(text.substr(at, found - at));
        if (boundary_before && eight && boundary_after) {
            const auto va = text::parse_int<std::uint64_t>(text.substr(digits, 8), 16);
            const std::optional<std::string> target = va ? lookup(*va) : std::nullopt;
            out += target ? *target : std::string(text.substr(found, kPrefix.size() + 8));
            at = digits + 8;
        } else {
            out += kPrefix;
            at = digits;
        }
    }
    out.append(text.substr(at));
    return out;
}

// What the build of one 16-bit module holds from step to step.
struct Ctx16 {
    const Services& services;
    Built& b;
    const stdfs::path& path;
    const ct::Module16& m;
    const stdfs::path& folder;
    formats::ne::Module ne;
    functions::Compiler cc;
    stdfs::path wlink, wasm;
    Env env;
    Segments segs;
    OrderedMap<std::uint64_t, std::string> by_va;
    OrderedMap<std::string, std::string> mtexts;   // the module's texts
    std::string mheader;
    Places at;
    std::vector<std::string> order;
    std::vector<std::string> cflags;               // the module's options, without the segment's
    std::map<std::string, std::string> stems;      // {name: object stem}
    std::map<std::uint32_t, std::vector<std::string>> by_segment;
    std::map<std::string, std::uint32_t> segment_of;
    std::map<std::string, std::string> texts;      // each function's text, its calls by address named
    std::map<std::int64_t, Bytes> carried;         // every data segment of the module the build carries
    std::uint32_t autodata = 0;
    std::set<std::string> forced;                  // functions rebuilt from their instructions
    std::map<std::string, PublicSymbol> publics;
    definitions::Interface iface;

    functions::FunctionBuilder& builder() const { return *services.builder; }
    void evidence(const std::string& n, const std::string& text) const {
        if (const auto it = b.origins.find(n); it != b.origins.end()) it->second.evidence.push_back(text);
    }
    // The compiler for code the build adds beside the functions: an entry stand-in, the interrupt routines, stand-ins.
    std::string extra(const std::string& stem, const std::string& text) const {
        std::vector<std::string> flags = cflags;
        flags.push_back("-nc=XCODE");
        if (stem == "data") flags.push_back("-dCODEBREAKER_DATA");   // (data.c defines the segments it declares)
        return builder().compile(cc, flags, folder, stem, text);
    }
    std::vector<std::string> flags_for(std::uint32_t seg) const {
        std::vector<std::string> flags = cflags;
        flags.push_back(std::format("-nt=CODE{}", seg));
        return flags;
    }
    ByteView segment_bytes(const formats::ne::Segment& s) const {
        const ByteView file = ne.data();
        const std::size_t from = std::min<std::size_t>(s.file_offset, file.size());
        return file.subspan(from, std::min<std::size_t>(s.length, file.size() - from));
    }
};

// A function with a path that returns without setting AX leaves AX as it came in: the source had a path with no value, and no caller reads one.
// The decompiler writes it `return in_AX;` with `in_AX` declared and never set. When that is all `in_AX` is in the text, the path returns 0 (an
// unspecified result) and the declaration goes -- no register parameter is invented; the gate then says, per function, whether it matters.
std::string unspecified_entry_result(std::string text) {
    const std::string_view name = "in_AX";
    const auto word_at = [&](std::size_t at) {
        const auto is_word = [](char ch) { return ch == '_' || (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'); };
        return (at == 0 || !is_word(text[at - 1])) && (at + name.size() >= text.size() || !is_word(text[at + name.size()]));
    };
    std::vector<std::size_t> uses;
    for (std::size_t at = text.find(name); at != std::string::npos; at = text.find(name, at + 1)) {
        if (word_at(at)) uses.push_back(at);
    }
    if (uses.size() < 2) return text;   // (a declaration and at least one return)
    std::size_t declared = std::string::npos;
    std::vector<std::size_t> returns;
    for (const std::size_t at : uses) {
        const std::size_t line = text.rfind('\n', at);
        const std::string before = text.substr(line == std::string::npos ? 0 : line + 1, at - (line == std::string::npos ? 0 : line + 1));
        const std::string after = text.substr(at + name.size(), text.find('\n', at) == std::string::npos ? std::string::npos : text.find('\n', at) - at - name.size());
        if (after == ";" && declared == std::string::npos && text::trim(before).find(' ') == std::string::npos && !text::trim(before).empty()) {
            declared = line == std::string::npos ? 0 : line;
        } else if (text::trim(before) == "return" && after == ";") {
            returns.push_back(at);
        } else {
            return text;   // (read or set some other way)
        }
    }
    if (declared == std::string::npos || returns.empty() || returns.size() + 1 != uses.size()) return text;
    for (auto it = returns.rbegin(); it != returns.rend(); ++it)
        text.replace(*it, name.size() + 1, "0; /* the original left AX as it came in: no caller reads it */");
    // (the declaration's line: from its newline to the next)
    const std::size_t dl_end = text.find('\n', declared + 1);
    if (dl_end != std::string::npos) text.erase(declared, dl_end - declared);
    return text;
}

// Compile each function into the code segment it came from.
void compile_functions(Ctx16& c) {
    Built& b = c.b;
    for (const auto& [place, name] : c.at) {
        c.by_segment[place.seg].push_back(name);
        c.segment_of[name] = place.seg;
    }
    // A call the decompiler left as an address goes to the function reconstructed at that address -- when it can be reached from this segment.
    for (const auto& [name, seg] : c.segment_of) {
        c.texts[name] = name_addressed_calls(c.mtexts.at(name), [&](std::uint64_t va) -> std::optional<std::string> {
            const std::string* target = c.by_va.find(va);
            if (target == nullptr) return std::nullopt;
            const auto where = c.segment_of.find(*target);
            if (!c.m.near.contains(*target) || (where != c.segment_of.end() && where->second == seg)) return *target;
            return std::nullopt;
        });
        c.texts[name] = unspecified_entry_result(std::move(c.texts[name]));
        // (CB_TRACE_WNDPROC=<name>: that function, a window procedure of five words, writes 0B3h and then its message, wParam and window words to port 0E9h as it is
        // entered -- the same bytes its instructions' build writes (asm16.cpp) -- so a boot of each can be compared call by call)
        if (env_text("CB_TRACE_WNDPROC") == name) {
            std::string& t = c.texts[name];
            const std::size_t open = t.find('{');
            const std::size_t blank = open == std::string::npos ? std::string::npos : t.find("\n\n", open);   // (the blank line after the declarations)
            if (blank != std::string::npos) {
                std::string probe;
                for (const char* w : {"0xB3", "a4", "a3", "a5"}) {
                    if (w[0] == '0') {
                        probe += std::string("  codebreaker_dbg(") + w + ");\n";
                    } else {
                        probe += std::string("  codebreaker_dbg((unsigned char)") + w + ");\n  codebreaker_dbg((unsigned char)((unsigned)" + w + " >> 8));\n";
                    }
                }
                t.insert(blank + 2, probe);
                t.insert(0, "void codebreaker_dbg(unsigned char);\n#pragma aux codebreaker_dbg = \"out 0e9h, al\" parm [al] modify [];\n");
            }
        }
    }
    std::vector<functions::Group> groups;
    for (const auto& [seg, names] : c.by_segment) groups.push_back({c.flags_for(seg), names, c.folder / std::format("s{:02d}", seg)});
    const std::vector<functions::Compiled> compiled =
        c.builder().compile_groups(c.cc, c.mheader, functions::Texts(c.texts.begin(), c.texts.end()), groups);
    std::size_t i = 0;
    for (const auto& [seg, names] : c.by_segment) {
        const functions::Compiled& done = compiled[i++];
        for (const auto& [name, stem] : done.stems) c.stems[name] = std::format("s{:02d}/{}", seg, stem);
        for (const auto& [name, why] : done.dropped) b.rejected.insert_or_assign(name, why);
    }
    fs::write_text(c.folder / "m.h", c.mheader);   // for what is compiled beside the functions
}

// The module's data as C, linked first so that it starts at offset 0 of the automatic data segment.
std::shared_ptr<const data::Data> make_data(Ctx16& c) {
    // every data segment besides the automatic one is carried, whether or not the C reads it: it is part of the module (and code written as
    // its own instructions refers to it)
    for (const auto& [n, sg] : c.segs) {
        if (!sg.is_data() || n == c.autodata || c.carried.contains(n)) continue;
        Bytes bytes;
        if (sg.file_offset != 0) {
            const ByteView s = c.segment_bytes(sg);
            bytes.assign(s.begin(), s.end());
        } else {
            bytes.assign(std::min<std::size_t>(sg.min_alloc != 0 ? sg.min_alloc : 0x10000, 0x10000), 0);
        }
        c.b.carried_segments[n] = bytes;
        c.carried[n] = std::move(bytes);
    }
    std::map<std::uint32_t, std::string> in_segment;   // a code segment -> a function in it (its selector)
    for (const auto& [place, name] : c.at) in_segment.emplace(place.seg, name);
    data::Names16 names;
    names.function_at = [&](data::Address16 a) -> std::optional<std::string> {
        const auto it = c.at.find(Place{a.segment, a.offset});
        return it == c.at.end() ? std::nullopt : std::optional<std::string>(it->second);
    };
    names.segment_symbol = [&](std::uint16_t s) -> std::optional<std::string> {
        const auto it = in_segment.find(s);
        return it == in_segment.end() ? std::nullopt : std::optional<std::string>(it->second);
    };
    std::set<std::uint16_t> carried_keys;
    std::map<std::uint16_t, Bytes> carried_bytes;
    for (const auto& [n, bytes] : c.carried) {
        carried_keys.insert(static_cast<std::uint16_t>(n));
        carried_bytes[static_cast<std::uint16_t>(n)] = bytes;
    }
    auto d = std::make_shared<data::Data>(data::data16(c.ne, names, carried_keys));
    const std::string far = data::far_segments(carried_bytes, &c.ne, names);
    if (const std::string why = c.extra("data", d->source + far); !why.empty()) {   // the code still needs a data segment to exist
        c.b.data_error = "the data does not compile: " + why;
        c.extra("data", "char codebreaker_data = 1;\n" + far);
    }
    return d;
}

// Which functions cannot be C: the origin says assembly, the original does something C cannot say, the C was run and differed ...
void decide_forced(Ctx16& c) {
    Built& b = c.b;
    const ct::Module16& m = c.m;
    // A function written in assembly is rebuilt as assembly (analyze/origin says which, from what its code does), not as a C translation of
    // it -- where its instructions can be written so that they assemble back to the original's bytes; the C stays only where that cannot be done.
    std::map<std::int64_t, std::string> by_va;
    for (const auto& [va, name] : c.by_va) by_va[static_cast<std::int64_t>(va)] = name;
    b.origins.clear();
    if (c.services.origins16) {
        for (const auto& [va, origin] : c.services.origins16(c.path, by_va)) b.origins[origin.name] = origin;
    }
    for (const std::string& n : c.order) {
        const auto o = b.origins.find(n);
        if (c.stems.contains(n) && o != b.origins.end() && o->second.language == analyze::origin::Language::Assembly) c.forced.insert(n);
    }
    // A function of a 32-bit code segment (NE flag 2000h: GDI's 36 and 38, its flat thunks, which the thunk compiler wrote as use32 assembly)
    // is its instructions: the C compiler here makes 16-bit code, which would run as 32-bit code in that segment
    for (const std::string& n : c.order) {
        const auto sg = c.segment_of.find(n);
        if (!c.stems.contains(n) || c.forced.contains(n) || sg == c.segment_of.end()) continue;
        const auto info = c.segs.find(sg->second);
        if (info != c.segs.end() && !info->second.is_data() && (info->second.flags & 0x2000)) {
            c.forced.insert(n);
            c.evidence(n, std::format("in a 32-bit code segment ({}): not 16-bit C", sg->second));
        }
    }
    // C with no result for a function whose instructions leave one in AX: the decompiler saw nothing in the module read it (an export: Windows
    // reads it -- KEYBOARD's Inquire returns the size it copied), and compiled C leaves its own AX, not the original's
    std::map<std::uint32_t, std::vector<std::uint32_t>> starts;
    for (const auto& [place, name] : c.at) starts[place.seg].push_back(place.off);
    for (const std::string& n : m.novalue) {
        if (!c.stems.contains(n) || c.forced.contains(n)) continue;
        const auto where = c.segment_of.find(n);
        if (where == c.segment_of.end()) continue;
        const auto place = std::find_if(c.at.begin(), c.at.end(), [&](const auto& e) { return e.second == n; });
        const auto info = c.segs.find(where->second);
        if (place == c.at.end() || info == c.segs.end() || info->second.file_offset == 0) continue;
        const ByteView code = c.segment_bytes(info->second);
        std::uint32_t end = static_cast<std::uint32_t>(code.size());
        for (std::uint32_t o : starts[where->second]) {
            if (o > place->first.off) end = std::min(end, o);
        }
        if (const auto ax = result_in_ax(code, place->first.off, end)) {
            c.evidence(n, std::format("leaves a result in AX ({:04x}) its C does not return (not trusted as C)", *ax));
            c.forced.insert(n);
        }
    }
    // C one of whose decompiled calls gives the caller's CS as an argument (a far call whose arguments it read a word off): until that is put
    // right, such a function is rebuilt from its own instructions rather than from C that would pass the wrong arguments.
    static const Regex unaff_cs(R"(\w+\([^;{}]*\bunaff_CS\b[^;{}]*\))");
    std::vector<std::string> untrusted;
    for (const std::string& n : c.order) {
        const std::string* text = c.mtexts.find(n);
        if (c.stems.contains(n) && text != nullptr && unaff_cs.test(*text)) untrusted.push_back(n);
    }
    for (const std::string& n : untrusted) {
        if (!c.forced.contains(n)) c.evidence(n, "its decompiled calls pass the caller's CS (not trusted as C)");
    }
    c.forced.insert(untrusted.begin(), untrusted.end());
    // ... and C that reads ES as the function was entered: no compiled C takes ES in. The original read ES as a callee left it -- USER's
    // sub_1_7CD4 returns `mov ax, es` after KERNEL's GetCodeInfo (its unwritten second result, the module); the C returned the ES it came in
    // with, SetWindowsHook stored that as the hook's module, and calling the hook faulted (1:8093, selector D400) on Setup's License page.
    static const Regex unaff_es(R"((?<![\w.])unaff_ES\b(?!\s*=[^=]))");
    for (const std::string& n : c.order) {
        const std::string* text = c.mtexts.find(n);
        if (c.forced.contains(n) || !c.stems.contains(n) || text == nullptr) continue;
        if (unaff_es.test(after_brace(*text))) {
            c.forced.insert(n);
            c.evidence(n, "its C reads ES as the function was entered (not trusted as C)");
        }
    }
    const std::map<std::string, std::string> mtexts_by_name(c.mtexts.begin(), c.mtexts.end());
    for (const auto& [name, why] : registers_c_cannot_carry(c.ne, c.segs, c.at, mtexts_by_name)) {
        if (!c.forced.contains(name) && c.stems.contains(name)) {
            c.forced.insert(name);
            c.evidence(name, why + " (not trusted as C)");
        }
    }
    // ... and C one of whose calls passes clearly fewer (or more) words than the callee takes off (its `ret N`): the decompiler kept to the
    // pushes it could see, and the stack would come back wrong
    std::map<std::string, std::size_t> arity;   // {function: words its callers push}
    if (m.description) {
        for (const std::string& line : m.description->lines) {
            const std::vector<std::string_view> parts = text::split(line, '\t');
            if (parts.size() > 4 && parts[0] == "F" && parts[4].starts_with("__stdcall16")) {
                const auto bytes = text::parse_int<std::int64_t>(parts[3]);
                if (bytes && *bytes > 0) arity[std::string(parts[2])] = static_cast<std::size_t>(*bytes) / 2;
            }
        }
    }
    if (!arity.empty()) {
        std::vector<std::string> escaped;
        for (const auto& [name, words] : arity) escaped.push_back(escape_regex(name));
        const Regex arity_call("\\b(" + text::join(escaped, "|") + ")\\s*\\(");   // (once for the module)
        for (const std::string& n : c.order) {
            const std::string* text = c.mtexts.find(n);
            if (c.forced.contains(n) || !c.stems.contains(n) || text == nullptr) continue;
            const std::string_view body = after_brace(*text);
            arity_call.for_each_match(body, [&](const RegexMatch& call) {
                const std::size_t passed = count_call_arguments(body, call.end() - 1);
                const std::size_t words = arity.at(std::string(call.group(1)));
                if (passed * 2 < words || passed > words) {
                    c.evidence(n, std::format("its call of {} passes {} arguments for {} words (not trusted as C)", call.group(1), passed, words));
                    c.forced.insert(n);
                    return false;
                }
                return true;
            });
        }
    }
    // ... and C that reads a variable nothing in it sets (the compiler's flow check): a value the original took from a register on entry or from
    // a buffer it had filled through a pointer, which the C reads as whatever happens to be there -- right or wrong by where the build happens
    // to put things
    {
        std::vector<std::string> left;
        for (const std::string& n : c.order) {
            if (!c.forced.contains(n) && c.stems.contains(n) && c.texts.contains(n)) left.push_back(n);
        }
        parallel::ThreadPool pool(std::max<std::size_t>(1, std::min<std::size_t>(left.size(), static_cast<std::size_t>(parallel::workers()))));
        const std::vector<std::vector<std::string>> unset = parallel::parallel_map(pool, left, [&](const std::string& n) {
            return c.builder().never_set(c.cc, c.cflags, c.folder / std::format("s{:02d}", c.segment_of.at(n)), n, c.texts.at(n));
        });
        for (std::size_t i = 0; i < left.size(); ++i) {
            if (unset[i].empty()) continue;
            c.evidence(left[i], std::format("its C reads {}, which it never sets (not trusted as C)", text::join(unset[i], ", ")));
            c.forced.insert(left[i]);
        }
    }
    // (for finding which rebuilt functions make a module misbehave: CB_ASM_ONLY=all rebuilds every function from its own instructions; a comma
    // list, those named; CB_ASM_SEGMENTS=1,3 those in the segments named)
    {
        const std::string only = env_text("CB_ASM_ONLY");
        std::set<std::uint32_t> segs_only;
        for (std::string_view x : text::split(env_text("CB_ASM_SEGMENTS"), ',')) {
            if (const auto n = text::parse_int<std::uint32_t>(text::trim(x))) segs_only.insert(*n);
        }
        if (!only.empty() || !segs_only.empty()) {
            std::set<std::string> named;
            if (only != "all") {
                for (std::string_view x : text::split(only, ',')) named.emplace(x);
            }
            for (const std::string& n : c.order) {
                const auto sg = c.segment_of.find(n);
                if (c.stems.contains(n) && (only == "all" || named.contains(n) || (sg != c.segment_of.end() && segs_only.contains(sg->second)))) c.forced.insert(n);
            }
        }
    }
    for (const std::string& n : near_pointer_tables(c.ne, c.at)) {
        // (its address is a number in the data segment: it must stay where it is)
        if (c.stems.contains(n) && !c.forced.contains(n)) {
            c.forced.insert(n);
            c.evidence(n, "its address is in a table of near code pointers in the data segment (called through it) -- kept as its instructions, at its place");
        }
    }
    for (const std::string& n : register_argument_calls(c.ne, c.at)) {
        // (a call through a pointer that takes an argument in a register: C cannot say it)
        if (c.stems.contains(n) && !c.forced.contains(n)) {
            c.forced.insert(n);
            c.evidence(n, "it loads AX for a call through a pointer (a VxD's API entry: the service in AX, the arguments read from its frame) -- kept as its instructions");
        }
    }
    {
        std::string kept_text;
        std::error_code ec;
        if (!c.services.kept_as_asm_file.empty() && stdfs::is_regular_file(c.services.kept_as_asm_file, ec)) kept_text = read_text_lossy(c.services.kept_as_asm_file);
        for (const auto& [name, why] : kept_as_asm(kept_text, b.name)) {   // (C a whole run of the module showed wrong)
            // (CB_FORCE_C names are C whatever this file says: an experiment's way to try a kept function without editing the file)
            const std::string force_c_text = env_text("CB_FORCE_C");
            const std::vector<std::string_view> force_c = text::split(force_c_text, ',');
            if (c.stems.contains(name) && std::find(force_c.begin(), force_c.end(), std::string_view(name)) == force_c.end()) {
                c.forced.insert(name);
                c.evidence(name, "kept as its instructions (build/kept_as_asm.txt): " + why);
            }
        }
    }
    for (const auto& [name, why] : b.gated) {   // (C that, run, did not do what the original does)
        if (c.stems.contains(name)) {
            c.forced.insert(name);
            c.evidence(name, "its C, run beside the original, differs (not trusted as C): " + why.substr(0, 160));
        }
    }
    for (std::string_view x : text::split(env_text("CB_FORCE_C"), ',')) c.forced.erase(std::string(x));   // (and these as C whatever origin says)
    if (const std::optional<std::string> keep_c = env_value("CB_ASM_KEEP_C")) {   // (and the other way: C for these alone)
        std::set<std::string> kept;
        for (std::string_view x : text::split(*keep_c, ',')) kept.emplace(x);
        for (const std::string& n : c.order) {
            if (c.stems.contains(n) && !kept.contains(n)) c.forced.insert(n);
        }
    }
}

// Where a self-loading module cuts its segments after start-up, and which functions find no room below it.
bool loads_itself(ByteView head) {
    if (head.size() < 6) return false;
    const std::int64_t pages = static_cast<std::int64_t>(head[4]) | static_cast<std::int64_t>(head[5]) << 8;
    // (signed: a file shorter than 511 bytes is a stub alone)
    return pages * 512 >= static_cast<std::int64_t>(head.size()) - 511;
}

// Compile the function again without the Windows prologue when the original's has none, and keep that object if it is smaller. True: it is.
bool refit_without_prologue(Ctx16& c, std::int64_t seg, std::uint32_t at, const std::string& name, OrderedMap<std::string, std::uint32_t>& size) {
    const std::uint32_t sg = static_cast<std::uint32_t>(seg);
    if (decompile::describe::windows_far_prologue(c.segment_bytes(c.segs.at(sg)), at)) return false;
    const std::string& stem = c.stems.at(name);
    const std::size_t slash = stem.find('/');
    if (slash == std::string::npos) return false;
    const stdfs::path dir = c.folder / stem.substr(0, slash);
    const std::string leaf = stem.substr(slash + 1);
    const auto code_size = [&]() -> std::optional<std::uint32_t> {
        try {
            return static_cast<std::uint32_t>(msc6::object_code(fs::read_file(c.folder / (stem + ".obj"))).code.size());
        } catch (const IoError&) {
            return std::nullopt;
        }
    };
    std::vector<std::string> flags = c.flags_for(sg);
    flags.push_back("-zW");
    const std::optional<std::uint32_t> was = code_size();
    if (!c.builder().compile(c.cc, flags, dir, leaf, c.texts.at(name)).empty()) {
        c.builder().compile(c.cc, c.flags_for(sg), dir, leaf, c.texts.at(name));   // (the object as it was)
        return false;
    }
    const std::optional<std::uint32_t> now = code_size();
    if (!now || (was && *now >= *was)) {
        c.builder().compile(c.cc, c.flags_for(sg), dir, leaf, c.texts.at(name));
        return false;
    }
    size.insert_or_assign(name, *now);
    return true;
}

void decide_resident(Ctx16& c, bool self_loading) {
    Built& b = c.b;
    b.resident_limit.clear();
    if (self_loading) {
        // A module that loads itself keeps only part of a segment after start-up: KERNEL cuts its segment 1 to A940h (GlobalReAlloc of CS at
        // 1:B567), the init code past it thrown away. A function from below that point that finds no room below it would go after the frame,
        // past the end -- KRNL386's C fault decoder (1:A296) at 1:C602 faulted on every fault it was to decode ("Invalid DPMI return from
        // 0367:C608"). The packing the frame will do is done here first, and only such a one stays as its instructions; one from past the
        // point (init code) may go after the frame.
        for (const auto& [n, s] : c.segs) {
            if (s.is_data()) continue;
            const auto limit = resident_limit_of(c.segment_bytes(s));
            if (limit && *limit != 0 && *limit < s.length) b.resident_limit[n] = *limit;
        }
        // (the data such a segment's C reads through CS is linked after its frame: past the cut, thrown away with the init code -- a function that
        // reads it stays as its instructions, which read the original's bytes where they are)
        for (const std::string& n : c.order) {
            const auto sg = c.segment_of.find(n);
            if (sg != c.segment_of.end() && b.resident_limit.contains(sg->second) && c.stems.contains(n) && !c.forced.contains(n) &&
                reads_code_data(c.texts.at(n), sg->second)) {
                c.forced.insert(n);
                c.evidence(n, std::format("it reads data beside its code through CS, in a segment cut after start-up ({:04x})", b.resident_limit.at(sg->second)));
            }
        }
    }
    for (const auto& [n, s] : c.segs) {
        // A FIXED code segment is no bigger than the original's: Windows puts a fixed segment as low in memory as it can -- below 1 MB, where every
        // task's PSP and task database must go too. USER's segment 1 grown from BA2Eh to C6DEh with the functions rebuilt from C that found no room
        // in their places and went after the frame, Setup could no longer start the program after its Welcome page: "There is not enough memory to
        // run Setup". (At C63Ch it still could; C778h -- the same build with sub_1_3514's C made 300 bytes longer, nothing else -- could not.) So a
        // function with no room in the frame stays as its instructions. (CB_FIXED_GROWS=1: let them grow, as before)
        if (!s.is_data() && !(s.flags & 0x0010) && env_text("CB_FIXED_GROWS") != "1") {
            const std::int64_t current = b.resident_limit.contains(n) ? b.resident_limit.at(n) : static_cast<std::int64_t>(s.length);
            b.resident_limit[n] = std::min<std::int64_t>(current, s.length);
        }
    }
    if (b.resident_limit.empty()) return;
    std::map<std::uint32_t, std::vector<std::pair<std::uint32_t, std::string>>> starts_of;
    for (const auto& [place, name] : c.at) starts_of[place.seg].emplace_back(place.off, name);   // (ascending)
    for (const auto& [sg, limit64] : b.resident_limit) {
        const std::uint32_t limit = static_cast<std::uint32_t>(limit64);
        const auto& ord = starts_of[static_cast<std::uint32_t>(sg)];
        OrderedMap<std::string, std::uint32_t> size;
        for (const auto& [off, nm] : ord) {
            if (c.forced.contains(nm) || !c.stems.contains(nm) || c.stems.at(nm).starts_with("asm/")) continue;
            try {
                size.insert_or_assign(nm, static_cast<std::uint32_t>(msc6::object_code(fs::read_file(c.folder / (c.stems.at(nm) + ".obj"))).code.size()));
            } catch (const IoError&) {
                // (no object for it: it has no size, and takes no part in the packing)
            }
        }
        std::set<std::string> refitted;
        while (true) {
            std::vector<HoleSpec> holes;
            std::map<std::string, std::uint32_t> own;
            for (std::size_t k = 0; k < ord.size(); ++k) {
                if (!size.contains(ord[k].second)) continue;
                holes.push_back({ord[k].first, k + 1 < ord.size() ? ord[k + 1].first : c.segs.at(static_cast<std::uint32_t>(sg)).length, ord[k].second});
                own[ord[k].second] = ord[k].first;
            }
            const Packed packed = pack_holes(holes, size, c.m.near, {}, limit, !c.b.byte_aligned_code);
            std::vector<std::string> stuck;
            for (const std::vector<std::string>* group : {&packed.no_room_far, &packed.no_room_near}) {
                for (const std::string& nm : *group) {
                    if (own.at(nm) < limit) stuck.push_back(nm);
                }
            }
            if (stuck.empty()) break;
            // A function whose original has no Windows prologue (a driver's stub: `xor ax, ax / retf`) is compiled without the one the compiler
            // writes for a far function (-zW), once, before it is given up: the prologue is what made it too big for its place.
            std::vector<std::string> give_up;
            for (const std::string& nm : stuck) {
                if (!refitted.insert(nm).second || !refit_without_prologue(c, sg, own.at(nm), nm, size)) give_up.push_back(nm);
            }
            if (give_up.empty()) continue;
            for (const std::string& nm : give_up) {
                c.forced.insert(nm);
                size.erase(nm);
                c.evidence(nm, limit < c.segs.at(static_cast<std::uint32_t>(sg)).length
                                   ? std::format("its C finds no room below where segment {} is cut ({:04x}) in a module that loads itself", sg, limit)
                                   : std::format("its C finds no room in fixed segment {}, which must stay the original's size ({:04x} bytes)", sg, limit));
            }
        }
    }
}

// The text of the assembly beside, read once for as long as it does not change; and the symbols it reaches near (`near ptr NAME`).
class NearReach {
public:
    explicit NearReach(stdfs::path folder) : folder_(std::move(folder)) {}
    bool reaches(const std::string& symbol) {
        const std::string text = asm_text();
        if (!cached_ || cached_->first != text) {
            static const Regex near_ptr(R"(near ptr ([\w@$?]+))");
            std::set<std::string> found;
            near_ptr.for_each_match(text, [&](const RegexMatch& m) {
                found.emplace(m.group(1));
                return true;
            });
            cached_ = std::make_pair(text, std::move(found));   // (found again only when the assembly has changed since)
        }
        return cached_->second.contains(symbol);
    }

private:
    struct Read {
        std::uintmax_t size = 0;
        stdfs::file_time_type time;
        std::string text;
    };
    std::string asm_text() {
        std::vector<std::string> parts;
        const stdfs::path dir = folder_ / "asm";
        std::error_code ec;
        if (stdfs::is_directory(dir, ec)) {
            std::vector<stdfs::path> files;
            for (const auto& entry : stdfs::directory_iterator(dir, ec)) {
                if (entry.path().extension() == ".asm") files.push_back(entry.path());
            }
            std::sort(files.begin(), files.end());   // (the order of the names)
            for (const stdfs::path& p : files) {
                const std::uintmax_t size = stdfs::file_size(p, ec);
                const auto time = cb::fs::last_write_time(p, ec);
                Read& r = read_[p.string()];
                if (r.size != size || r.time != time || r.text.empty()) r = {size, time, read_text_lossy(p)};
                parts.push_back(r.text);
            }
        }
        return text::join(parts, "\n");
    }
    stdfs::path folder_;
    std::map<std::string, Read> read_;   // each file read once
    std::optional<std::pair<std::string, std::set<std::string>>> cached_;
};

}  // namespace

std::optional<stdfs::path> ModuleBuilder::build16(Built& b, const stdfs::path& original, const ct::Module16& m, const stdfs::path& folder) const {
    const Services& s = services_;
    const watcom::OpenWatcom& ow = s.open_watcom;
    const stdfs::path wcc = ow.tool("wcc"), wlink = ow.tool("wlink");   // (16-bit Windows: its compiler and packager)
    if (wcc.empty() || wlink.empty()) throw UsageError("OpenWatcom's 16-bit compiler is not set up (settings.json: openwatcom)");
    if (!m.description) throw UsageError("a 16-bit module is built with its description");
    const Env env = ow.environment(watcom::Target::Win16);
    Ctx16 c{.services = s, .b = b, .path = original, .m = m, .folder = folder, .ne = formats::ne::Module::open_file(original), .cc = {wcc, env}, .wlink = wlink,
            .wasm = ow.tool("wasm"), .env = env};
    c.iface = s.interface_of(original);
    for (const formats::ne::Segment& sg : c.ne.segments()) c.segs[sg.index] = sg;
    c.by_va = m.made.by_va();
    c.mtexts = m.made.texts();
    c.mheader = m.made.header();
    for (const auto& [va, name] : c.by_va) {
        const auto place = m.where.find(static_cast<core16::Va>(va));
        if (place != m.where.end()) c.at[place->second] = name;
    }
    c.order = m.made.order();
    c.carried = m.segments;
    c.autodata = c.ne.header().autodata_segment;
    b.with_text = c.order.size();
    // the guard: a function the description does not have would be taken for the tail of the one before it, and erased with it if that one is
    // rebuilt from C -- never silently
    {
        std::set<core16::SegOff> known;
        for (const auto& [place, name] : c.at) known.insert(place);
        b.hidden = decompile::describe::hidden_functions(c.ne, known);
    }
    // (the medium model; MCODE: see the link order; size: a function rebuilt from C goes back into its place when it fits)
    for (const std::string& f : functions::cflags16()) {
        if (f != "-ms") c.cflags.push_back(f);
    }
    for (const char* f : {"-mm", "-fpi87", "-os", "-nc=MCODE"}) c.cflags.push_back(f);
    b.byte_aligned_code = true;   // (-os: the code segment of each object is byte-aligned)

    compile_functions(c);
    const std::shared_ptr<const data::Data> d = make_data(c);
    b.data = d;
    c.publics = publics_of(s, folder, c.stems);
    decide_forced(c);
    const bool self_loading = loads_itself(c.ne.data());
    decide_resident(c, self_loading);

    NearReach near_reach(folder);
    std::set<std::string> fresh;   // (the folders beside whose header is written)
    // A far function rebuilt from C, compiled into a code segment of its own beside its segment (CODE<n>X) -- for one that does not fit back
    // into its place. -> its stem, or nullopt. Not in a module that loads itself (KRNL386: DOS loads the whole file and its own start sets up its
    // segments and entries -- only the ones the original has; an export moved to a segment of ours is a "Call to Undefined Dynalink").
    const ElsewhereFn elsewhere = [&](const std::string& name) -> std::optional<std::string> {
        static const Regex call_name(R"(\b(\w+)\s*\()");
        const auto seg = c.segment_of.find(name);
        if (seg == c.segment_of.end() || m.near.contains(name) || self_loading) return std::nullopt;
        const std::string& text = c.texts.at(name);
        bool calls_near = false;
        call_name.for_each_match(after_brace(text), [&](const RegexMatch& call) {
            if (m.near.contains(std::string(call.group(1)))) calls_near = true;
            return !calls_near;
        });
        // it calls near ones, which only its own segment reaches (or reads its segment's own bytes through CS)
        static const Regex code_read(R"(\bcodebreaker_code_\d+\b)");
        if (calls_near || text.find("code __near *") != std::string::npos || text.find("codebreaker_cs()") != std::string::npos || code_read.test(text)) return std::nullopt;
        const auto published = c.publics.find(name);
        const std::string sym = published != c.publics.end() ? published->second.symbol : "_" + name;
        if (near_reach.reaches(sym)) return std::nullopt;   // hand-written code beside it reaches it near
        const stdfs::path sub = folder / std::format("s{:02d}x", seg->second);
        std::error_code ec;
        stdfs::create_directories(sub, ec);
        if (fresh.insert(sub.string()).second) c.builder().write_header(sub, c.mheader);   // (the header beside the function, as in its own folder -- this build's, not one left from before)
        const std::string stem = stdfs::path(c.stems.at(name)).filename().string();
        std::vector<std::string> flags = c.cflags;
        flags.push_back(std::format("-nt=CODE{}X", seg->second));
        flags.push_back("-nc=XCODE");
        const std::string why = c.builder().compile(c.cc, flags, sub, stem, text);
        if (!why.empty()) b.elsewhere_why[name] = why;
        return why.empty() ? std::optional<std::string>(std::format("s{:02d}x/{}", seg->second, stem)) : std::nullopt;
    };

    b.near_names = m.near;   // (for placing them: a near one stays in its segment)
    const Assembler assembler(s, c.wasm, env, folder);
    AssemblyJob job{s, b, c.ne, c.at, c.segs, c.stems, c.publics, folder, assembler, c.carried, c.forced, elsewhere};
    const std::map<std::string, asm16::Import> asm_imports = assemble16(job);
    for (const auto& [name, why] : m.failed) {   // no C, and not rebuilt from its instructions
        if (!b.assembled.contains(name) && !b.rejected.contains(name)) b.rejected.insert_or_assign(name, why);
    }

    const definitions::Interface& iface = c.iface;
    std::vector<std::string> lines = {std::string("format windows") + (iface.library ? " dll" : ""), "name m.dll",
                                      "option quiet, map, nodefaultlibs, caseexact, packcode=1, packdata=0",
                                      std::format("option modname='{}'", iface.name)};
    const formats::ne::Header& header = c.ne.header();
    if (header.heap_size != 0) lines.push_back(std::format("option heapsize={}", header.heap_size));
    if (header.stack_size != 0) lines.push_back(std::format("option stack={}", header.stack_size));   // (the linker refuses a size of 0)
    if (header.autodata_segment == 0) {
        // (no automatic data segment in the original -- WIN87EM, SYSTEM.DRV: their functions run on the caller's DS. The build's own little DGROUP
        // made one, and the loader set it up)
        lines.push_back("option noautodata");
    }
    if (!iface.description.empty()) lines.push_back("option description '" + text::replace_all(iface.description, "'", " ") + "'");
    // what the original's header says of it, as its linker was told: the Windows version it expects (3.10: Windows treats a 3.0 module the old
    // way), protected mode only, and no application type (KERNEL's)
    {
        const ByteView file = c.ne.data();
        const std::size_t ne_at = c.ne.ne_offset();
        const std::uint32_t expected = ne_at + 0x40 <= file.size() ? static_cast<std::uint32_t>(file[ne_at + 0x3E]) | static_cast<std::uint32_t>(file[ne_at + 0x3F]) << 8 : 0;
        if (expected != 0) lines.push_back(std::format("option version={}.{}", expected >> 8, expected & 0xFF));
        if (header.flags & 0x0008) lines.push_back("option protmode");
        if (!(header.flags & 0x0300)) lines.push_back("option noapptype");
    }
    lines.push_back("libpath " + fs::to_text(ow.lib_dir(watcom::Target::Win16)));   // lib286/win: the real library, not the stub above it
    const std::uint32_t cs = header.cs_ip >> 16, ip = header.cs_ip & 0xFFFF;
    const auto entry = cs != 0 ? c.at.find(Place{cs, ip}) : c.at.end();
    const bool entry_rebuilt = entry != c.at.end() && c.publics.contains(entry->second);
    if (entry_rebuilt) {
        lines.push_back("option start=" + quote_symbol(c.publics.at(entry->second).symbol));
        b.entry = entry->second + ", the function at the original's entry point";
    } else if (cs != 0) {
        c.extra("entry", "int2 __far __stdcall codebreaker_entry()\n{\n  return 1;\n}\n");
        lines.push_back("option start='_codebreaker_entry'");
        b.entry = std::format("a stand-in that reports success (the original's entry point {}:{:04x} has no reconstruction)", cs, ip);
    } else {
        b.entry = "none, as in the original";
    }
    lines.push_back("file data.obj");   // first: the data segment starts with it
    std::set<std::string> used_c = b.placed_c;
    used_c.insert(b.moved_c.begin(), b.moved_c.end());
    const bool uses_interrupts = std::any_of(used_c.begin(), used_c.end(), [&](const std::string& n) {
        const std::string* text = c.mtexts.find(n);
        return text != nullptr && text->find("codebreaker_int") != std::string::npos;
    });
    if (!m.interrupts.empty() && uses_interrupts) {
        // the interrupts the module's C takes (none, with every function rebuilt from its instructions: then nothing is added to the original's segments)
        std::set<std::uint32_t> vectors;
        for (const ct::Interrupt& i : m.interrupts) vectors.insert(i.vector);
        const ct::InterruptRoutines routines = ct::interrupt_routines(vectors);
        fs::write_text(folder / "interrupts.asm", routines.assembly);
        const ToolRun r = run_tool(c.wasm, {"-q", "interrupts.asm"}, folder, env, std::chrono::minutes(10));
        const std::string why = c.extra("interrupt_registers", text::replace_all(routines.c, "#include \"m.h\"\n", ""));
        if (r.code == 0 && why.empty()) {
            lines.push_back("file interrupts.obj");
            lines.push_back("file interrupt_registers.obj");
        } else {
            b.error = "the interrupt routines: " + (!why.empty() ? why : trimmed_tail(!r.out.empty() ? r.out : r.err, 200));
        }
    }
    for (const std::string& stem : b.lead_ins) lines.push_back("file " + stem + ".obj");   // each code segment starts with its lead-in
    {
        const std::set<std::string> lead_set(b.lead_ins.begin(), b.lead_ins.end());
        std::set<std::string> seen;
        for (const std::string& n : c.order) {   // (a run of functions written in assembly is one object)
            const auto stem = c.stems.find(n);
            if (stem == c.stems.end()) continue;
            if (seen.insert(stem->second).second && !lead_set.contains(stem->second)) lines.push_back("file " + stem->second + ".obj");
        }
    }
    // the original's data that the C of a segment reads through CS (c_text: codebreaker_code_<n>), after the segment's code -- for a segment with
    // any such function rebuilt from C
    b.code_data.clear();
    for (const auto& [seg, c_text] : m.code_data) {
        std::vector<std::string> readers;
        if (const auto names = c.by_segment.find(static_cast<std::uint32_t>(seg)); names != c.by_segment.end()) {
            for (const std::string& n : names->second) {
                const auto stem = c.stems.find(n);
                const auto text = c.texts.find(n);
                if (stem != c.stems.end() && !stem->second.starts_with("asm/") && text != c.texts.end() && reads_code_data(text->second, seg)) readers.push_back(n);
            }
        }
        if (readers.empty()) continue;
        const std::string why = c.builder().compile(c.cc, c.flags_for(static_cast<std::uint32_t>(seg)), folder, std::format("code{:02d}", seg), c_text);
        if (!why.empty()) {
            if (b.error.empty()) b.error = std::format("the data code segment {}'s C reads through CS: {}", seg, why);
        } else {
            lines.push_back(std::format("file code{:02d}.obj", seg));
            b.code_data.push_back({seg, readers});
        }
    }
    if (cs != 0 && !entry_rebuilt) lines.push_back("file entry.obj");
    for (const auto& [seg, names] : c.by_segment) {   // each code segment loaded as the original's was
        const auto info = c.segs.find(seg);
        const std::uint32_t flags = info != c.segs.end() ? info->second.flags : 0x1D10;
        std::vector<std::string> opts;
        for (const SegmentOption& o : kSegmentOptions) {
            const char* pick = (flags & o.bit) ? o.set : o.clear;
            if (*pick != 0) opts.push_back(pick);
        }
        lines.push_back(std::format("segment 'CODE{}' {}", seg, text::join(opts, " ")));
    }
    if (const auto auto_seg = c.segs.find(c.autodata); auto_seg != c.segs.end()) {   // the data segment, loaded as the original's
        const std::uint32_t flags = auto_seg->second.flags;
        std::vector<std::string> how;
        for (int i = 0; i < 2; ++i) how.push_back((flags & kSegmentOptions[i].bit) ? kSegmentOptions[i].set : kSegmentOptions[i].clear);
        lines.push_back("segment class 'DATA' " + text::join(how, " "));
        lines.push_back("segment class 'BSS' " + text::join(how, " "));
    }
    // the original's DOS part (its stub -- KRNL386's is its real-mode start, which finds the NE header by its own length and runs from the whole file
    // DOS loads)
    if (const auto stub = write_dos_stub(original, folder)) {
        lines.push_back("option stub=" + *stub);
        lines.push_back("option stubverbatim");
    }
    // its file alignment, and -- where its loader fixes are all by segment number, chained, as the original linker wrote KRNL386's (which relocates
    // itself, before there is an entry table to go through) -- ours the same
    lines.push_back(std::format("option alignment={}", 1u << header.alignment_shift));
    if (segment_number_fixups(c.ne)) lines.push_back("option selectorfixups");
    // ... and where the original's was: its code segments first, by their numbers (segment n is the original's segment n -- what reaches one by its
    // number finds it), what the build adds (CODE<n>X, the runtime's _TEXT) after them
    std::vector<std::uint32_t> code_numbers, data_numbers;   // (data_numbers: the ones that are not code)
    for (const auto& [n, sg] : c.segs) (sg.is_data() ? data_numbers : code_numbers).push_back(n);
    const bool all_rebuilt = !code_numbers.empty() && std::all_of(code_numbers.begin(), code_numbers.end(), [&](std::uint32_t n) { return c.by_segment.contains(n); });
    if (all_rebuilt) {
        std::vector<std::string> segment_names;
        for (std::uint32_t n : code_numbers) segment_names.push_back(std::format("segment CODE{}", n));
        std::string order_line = "order clname MCODE " + text::join(segment_names, " ");
        // the data segments after them, each at its number as there: the automatic one (its classes) and those carried as they were
        // (CODEBREAKER_SEGMENT_<n>, class FAR_DATA). Anything the build adds goes after all of the original's -- a runtime helper's code put before a
        // data segment renumbers it and every selector after it (USER's segment 44, its second data segment, became 46 behind the long-division routine)
        const std::uint32_t max_code = code_numbers.back();
        if (!data_numbers.empty() && data_numbers.front() == max_code + 1) {
            std::vector<std::uint32_t> far_segments;
            for (std::uint32_t n : data_numbers)
                if (n != c.autodata) far_segments.push_back(n);
            const bool in_data = std::find(data_numbers.begin(), data_numbers.end(), c.autodata) != data_numbers.end();
            const bool all_below = std::all_of(far_segments.begin(), far_segments.end(), [&](std::uint32_t n) { return n < c.autodata; });
            const bool all_above = std::all_of(far_segments.begin(), far_segments.end(), [&](std::uint32_t n) { return n > c.autodata; });
            if (far_segments.empty() || (std::is_sorted(far_segments.begin(), far_segments.end()) && (!in_data || all_below || all_above))) {
                std::vector<std::string> pieces;
                for (std::uint32_t n : data_numbers) {
                    if (n == c.autodata) {
                        pieces.push_back("clname BEGDATA clname DATA clname BSS clname STACK");
                    } else if (n == far_segments[0]) {
                        std::vector<std::string> names;
                        for (std::uint32_t k : far_segments) names.push_back(std::format("segment CODEBREAKER_SEGMENT_{}", k));
                        pieces.push_back("clname FAR_DATA " + text::join(names, " "));
                    }
                }
                order_line += " " + text::join(pieces, " ");
            }
        }
        lines.push_back(order_line + " clname XCODE clname CODE");   // (the runtime's CODE last)
    } else {
        std::vector<std::string> missing;
        for (std::uint32_t n : code_numbers)
            if (!c.by_segment.contains(n)) missing.push_back(std::to_string(n));
        b.layout = "segments renumbered: the original's code segments [" + text::join(missing, ", ") + "] are not all rebuilt";
    }
    // the module's resources
    if (!resources::read_module(original).resources.empty()) {
        b.resources = resources::rebuild(ow, original, folder);
        if (b.resources->link_res) lines.push_back("option resource=" + b.resources->link_res->filename().string());
    }
    std::map<std::string, asm16::Import> imports;
    for (const auto& [symbol, imported] : m.imports) imports[symbol] = {imported.first, static_cast<std::uint16_t>(imported.second)};
    for (const auto& [symbol, imported] : d->imports) imports.try_emplace(symbol, asm16::Import{imported.module, imported.by});
    for (const auto& [symbol, imported] : asm_imports) imports.try_emplace(symbol, imported);
    // a C name compiled as __stdcall with an open argument list is the symbol `_name`
    for (const auto& [symbol, imported] : imports) {
        const std::string what = std::holds_alternative<std::uint16_t>(imported.which) ? std::to_string(std::get<std::uint16_t>(imported.which))
                                                                                         : quote_symbol(std::get<std::string>(imported.which));
        lines.push_back(std::format("import _{} {}.{}", symbol, imported.module, what));
    }

    // The module's constants -- entries of segment 0xFE, KERNEL's __AHINCR, __0040H and the like: values, not places. Each is a public EQU in
    // constants.asm, which the linker exports as a constant entry.
    std::map<std::uint32_t, definitions::Export> constants;
    for (const definitions::Export& e : iface.exports) {
        if (const auto* far = std::get_if<Far16>(&e.address); far != nullptr && far->seg == 0xFE) constants[e.ordinal] = e;
    }
    const auto const_symbol = [](const definitions::Export& e) {
        return e.name && text::is_identifier(*e.name) ? *e.name : std::format("constant_{}", e.ordinal);
    };
    if (!constants.empty()) {
        std::vector<std::string> src = {std::format("; {} -- the values it exports as constants (entry segment 0FEh)", iface.name), ""};
        for (const auto& [ordinal, e] : constants) {   // (by ordinal)
            src.push_back("public " + const_symbol(e));
            src.push_back(std::format("{} equ 0{:X}h", const_symbol(e), std::get<Far16>(e.address).off));
        }
        src.push_back("end");
        src.push_back("");
        fs::write_text(folder / "constants.asm", text::join(src, "\n"));
        if (run_tool(c.wasm, {"-q", "constants.asm"}, folder, env, std::chrono::minutes(10)).code == 0) lines.push_back("file constants.obj");
        else constants.clear();
    }
    const definitions::SymbolAt symbol_at = [&](const definitions::Export& e) -> std::optional<std::string> {
        if (constants.contains(e.ordinal)) return const_symbol(e);
        const auto* far = std::get_if<Far16>(&e.address);
        if (far == nullptr) return std::nullopt;
        if (const auto exported = d->exported.find({far->seg, far->off}); exported != d->exported.end()) return "_" + exported->second;   // exported data: its part of the segment
        const auto fn = c.at.find(Place{far->seg, far->off});
        if (fn == c.at.end() || !c.publics.contains(fn->second)) return std::nullopt;
        return c.publics.at(fn->second).symbol;
    };
    {
        definitions::Directives directives = definitions::export_directives(iface, symbol_at);
        lines.insert(lines.end(), directives.lines.begin(), directives.lines.end());
        b.exports_missing = std::move(directives.missing);
    }
    lines.push_back("library clibm.lib");
    const auto stand_in = [&](const std::string& symbol) { return stand_in_from_header(c.mheader, symbol); };
    const auto compile_extra = [&](const std::string& stem, const std::string& text) { return c.extra(stem, text); };
    std::optional<stdfs::path> image = link_resolving(b, wlink, env, folder, lines, stand_in, compile_extra);
    // As the function-by-function build does (build/functions): a function that reaches near for something in another segment (the original called it
    // far; the text has lost that) cannot be linked, and a segment whose rebuilt functions come to more than 64K cannot be either. Those functions become
    // stand-ins -- the largest first, for a segment too big -- and the module is linked again.
    static const Regex file_apart(R"(file (s\d+/f\d+)\.obj.*?not in the same segment)");
    static const Regex size_over(R"(size of segment CODE(\d+) exceeds 64k by (\d+) bytes)");
    std::map<std::string, std::string> by_stem;
    for (const auto& [name, stem] : c.stems) by_stem[stem] = name;
    const auto object_size = [&](const std::string& n) { return fs::file_size(folder / (c.stems.at(n) + ".obj")); };
    for (int attempt = 0; attempt < 120 && !image; ++attempt) {
        const auto apart = file_apart.search(b.error);
        const auto over = size_over.search(b.error);
        std::vector<std::pair<std::string, std::string>> swap;
        if (apart && by_stem.contains(std::string(apart->group(1))) && !b.rejected.contains(by_stem.at(std::string(apart->group(1))))) {
            std::string line;
            for (std::string_view ln : text::lines(b.error)) {
                if (ln.find(apart->group(1)) != std::string_view::npos) {
                    line = ln;
                    break;
                }
            }
            swap.emplace_back(by_stem.at(std::string(apart->group(1))), "reaches near for something in another segment: " + trimmed_tail(line, 160));
        } else if (over) {
            const std::uint32_t seg = text::parse_int<std::uint32_t>(over->group(1)).value_or(0);
            const std::uint64_t need = text::parse_int<std::uint64_t>(over->group(2)).value_or(0) + 512;
            std::uint64_t freed = 0;
            // First the far ones, largest first, go to the segment beside (CODE<n>X): they are called far, from anywhere. Only what that cannot free
            // becomes stand-ins.
            bool moved_now = false;
            std::vector<std::pair<std::uint64_t, std::string>> cands;
            const auto in_seg = c.by_segment.find(seg);
            const std::vector<std::string> names = in_seg != c.by_segment.end() ? in_seg->second : std::vector<std::string>();
            for (const std::string& n : names) {
                if (c.stems.contains(n) && !b.rejected.contains(n) && !m.near.contains(n) && !b.moved_c.contains(n) && !b.assembled.contains(n) &&
                    !c.stems.at(n).starts_with("asm/"))
                    cands.emplace_back(object_size(n), n);
            }
            const auto largest_first = [](const auto& x, const auto& y) { return x.first > y.first || (x.first == y.first && y.second < x.second); };
            std::sort(cands.begin(), cands.end(), largest_first);
            for (const auto& [bytes, n] : cands) {
                if (freed >= need) break;
                const std::string old = "file " + c.stems.at(n) + ".obj";
                const auto at_line = std::find(lines.begin(), lines.end(), old);
                if (at_line == lines.end()) continue;
                const std::optional<std::string> moved = elsewhere(n);
                if (moved && !moved->empty()) {
                    *at_line = "file " + *moved + ".obj";
                    c.stems[n] = *moved;
                    b.moved_c.insert(n);
                    freed += bytes;
                    moved_now = true;
                }
            }
            if (moved_now) {
                b.error.clear();
                b.absent.clear();
                image = link_resolving(b, wlink, env, folder, lines, stand_in, compile_extra);
                continue;
            }
            cands.clear();
            for (const std::string& n : names) {
                if (c.stems.contains(n) && !b.rejected.contains(n)) cands.emplace_back(object_size(n), n);
            }
            std::sort(cands.begin(), cands.end(), largest_first);
            for (const auto& [bytes, n] : cands) {
                if (freed >= need) break;
                swap.emplace_back(n, "too large: the functions of its segment do not fit in 64K when rebuilt");
                freed += bytes;
            }
        }
        if (swap.empty()) break;
        for (const auto& [name, why] : swap) {
            const std::string& stem = c.stems.at(name);
            const std::size_t slash = stem.find('/');
            if (!c.builder().compile(c.cc, c.flags_for(c.segment_of.at(name)), folder / stem.substr(0, slash), stem.substr(slash + 1), functions::stand_in(c.texts.at(name))).empty())
                return image;   // not even a stand-in compiles: give up
            b.rejected.insert_or_assign(name, why);
        }
        b.error.clear();
        b.absent.clear();
        image = link_resolving(b, wlink, env, folder, lines, stand_in, compile_extra);
    }
    b.stand_in_addresses.clear();
    for (const auto& [va, name] : c.by_va) {
        const auto place = m.where.find(static_cast<core16::Va>(va));
        if (b.rejected.contains(name) && place != m.where.end()) b.stand_in_addresses.insert(Address(place->second));
    }
    if (image && b.data_error.empty()) {   // is the data where the code expects it?
        std::string map_text;
        std::error_code ec;
        if (stdfs::is_regular_file(folder / "m.map", ec)) map_text = read_text_lossy(folder / "m.map");
        static const Regex group(R"(^DGROUP\s+([0-9a-fA-F]{4}):([0-9a-fA-F]{4}))", RegexFlags::Multiline);
        static const Regex mine(R"(^([0-9a-fA-F]{4}):([0-9a-fA-F]{4})[+*]?\s+_codebreaker_data\s*$)", RegexFlags::Multiline);
        const auto g = group.search(map_text);
        const auto k = mine.search(map_text);
        if (!(g && k && g->group(1) == k->group(1) && g->group(2) == k->group(2))) b.data_error = "the data did not land at the start of the automatic data segment";
    }
    // (each export declared as the back end wrote its function's heading from L2: m.declarations)
    const std::string file_name = original.filename().string();
    const std::string base = text::lower(original.stem().string());
    fs::write_text(folder / (base + ".def"), definitions::definition_file(iface, file_name));
    fs::write_text(folder / (base + "_public.h"), definitions::public_header(iface, file_name, [&](const definitions::Export& e) -> std::optional<std::string> {
        const auto* far = std::get_if<Far16>(&e.address);
        if (far == nullptr) return std::nullopt;
        const auto fn = c.at.find(Place{far->seg, far->off});
        if (fn == c.at.end()) return std::nullopt;
        const auto declared = m.declarations.find(fn->second);
        return declared == m.declarations.end() ? std::nullopt : std::optional<std::string>(declared->second);
    }));
    // the module as one L2 tree, whatever each function came back as (docs/IR_UNIFICATION.md step 4)
    {
        std::map<std::int64_t, std::int64_t> lengths;
        for (const auto& [n, sg] : c.segs) lengths[n] = sg.length;
        b.l2_arena = std::make_shared<Arena>();
        b.l2 = module_document(*b.l2_arena, b.name, m, b.assembled, c.stems, folder, lengths);
        json::save(folder / "module.l2.json", translate::l2::to_json(*b.l2), json::Style::PythonCompatible);
    }
    return image;
}

}  // namespace cb::build::module
