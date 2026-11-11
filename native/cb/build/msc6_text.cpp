// The rewrites of the decompiler's C text that msc6.hpp declares: C in, C out, no state.
#include <algorithm>
#include <cctype>
#include <format>
#include <functional>
#include <limits>

#include "cb/build/msc6.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::build::msc6 {

namespace wine16 = cb::symbols::wine16;

namespace {

// ---- scanning helpers -----------------------------------------------------------------------------------------------

constexpr std::size_t npos = std::string_view::npos;

bool is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
bool is_space(char c) { return text::is_space(c); }

// What may stand just before a name for it to be that name: (?<![\w.]) and its two cousins.
enum class Before {
    NotWordOrDot,      // (?<![\w.])
    NotWordDotArrow,   // (?<![\w.>])
    Boundary,          // \b (the name begins with a word character)
};

bool before_ok(std::string_view s, std::size_t p, Before rule) {
    if (p == 0) return true;
    const char c = s[p - 1];
    switch (rule) {
        case Before::NotWordOrDot: return !is_word(c) && c != '.';
        case Before::NotWordDotArrow: return !is_word(c) && c != '.' && c != '>';
        case Before::Boundary: return !is_word(c);
    }
    return false;
}

// `name` as a whole word at or after `from`: the rule before it, a word boundary after it, and optionally not followed (past
// white space) by '[' -- (?!\s*\[).
std::size_t find_word(std::string_view s, std::string_view name, std::size_t from, Before rule, bool not_indexed = false) {
    if (name.empty()) return npos;
    for (std::size_t p = s.find(name, from); p != npos; p = s.find(name, p + 1)) {
        if (!before_ok(s, p, rule)) continue;
        const std::size_t after = p + name.size();
        if (after < s.size() && is_word(s[after])) continue;
        if (not_indexed) {
            std::size_t q = after;
            while (q < s.size() && is_space(s[q])) ++q;
            if (q < s.size() && s[q] == '[') continue;
        }
        return p;
    }
    return npos;
}

std::size_t count_word(std::string_view s, std::string_view name, Before rule, bool not_indexed = false) {
    std::size_t n = 0;
    for (std::size_t p = find_word(s, name, 0, rule, not_indexed); p != npos; p = find_word(s, name, p + name.size(), rule, not_indexed)) ++n;
    return n;
}

// Every whole word `name` (rule before it) made `replacement`.
std::string replace_word(std::string_view s, std::string_view name, Before rule, std::string_view replacement) {
    std::string out;
    std::size_t i = 0;
    for (std::size_t p = find_word(s, name, 0, rule); p != npos; p = find_word(s, name, i, rule)) {
        out.append(s.substr(i, p - i));
        out.append(replacement);
        i = p + name.size();
    }
    out.append(s.substr(i));
    return out;
}

// `LO\s*,\s*HI\b` (with the rule before LO) at p: the position after it, or npos.
std::size_t pair_at(std::string_view s, std::size_t p, std::string_view lo, std::string_view hi) {
    if (s.compare(p, lo.size(), lo) != 0) return npos;
    std::size_t q = p + lo.size();
    while (q < s.size() && is_space(s[q])) ++q;
    if (q >= s.size() || s[q] != ',') return npos;
    ++q;
    while (q < s.size() && is_space(s[q])) ++q;
    if (s.compare(q, hi.size(), hi) != 0) return npos;
    q += hi.size();
    if (q < s.size() && is_word(s[q])) return npos;
    return q;
}

// Every `LO , HI` made `replacement`.
std::string replace_pair(std::string_view s, std::string_view lo, std::string_view hi, Before rule, std::string_view replacement) {
    std::string out;
    std::size_t i = 0;
    for (std::size_t p = s.find(lo, 0); p != npos; p = s.find(lo, i > p ? i : p + 1)) {
        if (p < i || !before_ok(s, p, rule)) continue;
        const std::size_t q = pair_at(s, p, lo, hi);
        if (q == npos) continue;
        out.append(s.substr(i, p - i));
        out.append(replacement);
        i = q;
    }
    out.append(s.substr(i));
    return out;
}

std::size_t count_pairs(std::string_view s, std::string_view lo, std::string_view hi, Before rule) {
    std::size_t n = 0, i = 0;
    for (std::size_t p = s.find(lo, 0); p != npos; p = s.find(lo, i > p ? i : p + 1)) {
        if (p < i || !before_ok(s, p, rule)) continue;
        const std::size_t q = pair_at(s, p, lo, hi);
        if (q == npos) continue;
        ++n;
        i = q;
    }
    return n;
}

// A declaration `type name;` as ^\s*\w+\s+NAME\s*; finds it (with the line start `from` ^ may be at).
struct Declaration {
    std::size_t start = 0;                 // where the match begins: a line start, then the white space
    std::size_t type_begin = 0, type_end = 0;
    std::size_t name_at = 0;
    std::size_t semicolon = 0;
};

std::optional<Declaration> find_declaration(std::string_view s, std::string_view name, std::size_t from) {
    for (std::size_t p = from; p <= s.size(); ++p) {
        if (!(p == 0 || s[p - 1] == '\n')) continue;
        std::size_t i = p;
        while (i < s.size() && is_space(s[i])) ++i;
        const std::size_t type_begin = i;
        while (i < s.size() && is_word(s[i])) ++i;
        if (i == type_begin) continue;
        const std::size_t type_end = i;
        while (i < s.size() && is_space(s[i])) ++i;
        if (i == type_end) continue;
        if (s.compare(i, name.size(), name) != 0) continue;
        std::size_t j = i + name.size();
        while (j < s.size() && is_space(s[j])) ++j;
        if (j >= s.size() || s[j] != ';') continue;
        return Declaration{p, type_begin, type_end, i, j};
    }
    return std::nullopt;
}

bool has_declaration(std::string_view s, std::string_view name) { return find_declaration(s, name, 0).has_value(); }

// Every declaration line of `name` (`^\s*\w+\s+NAME\s*;[ \t]*\n`) gone.
std::string remove_declarations(std::string_view decls, std::string_view name) {
    std::string out;
    std::size_t i = 0;
    while (true) {
        const auto d = find_declaration(decls, name, i);
        if (!d) break;
        std::size_t end = d->semicolon + 1;
        while (end < decls.size() && (decls[end] == ' ' || decls[end] == '\t')) ++end;
        if (end >= decls.size() || decls[end] != '\n') {   // (no line end: not this one; the next line start may do)
            out.append(decls.substr(i, d->start + 1 - i));
            i = d->start + 1;
            continue;
        }
        out.append(decls.substr(i, d->start - i));
        i = end + 1;
    }
    out.append(decls.substr(i));
    return out;
}

// Every declaration of `name` given `type` instead: `^(\s*)\w+(\s+)NAME\s*;` -> `\1TYPE\2NAME;`.
std::string retype_declarations(std::string_view decls, std::string_view name, std::string_view type) {
    std::string out;
    std::size_t i = 0;
    while (true) {
        const auto d = find_declaration(decls, name, i);
        if (!d) break;
        out.append(decls.substr(i, d->type_begin - i));
        out.append(type);
        out.append(decls.substr(d->type_end, d->name_at - d->type_end));
        out.append(name);
        out.push_back(';');
        i = d->semicolon + 1;
    }
    out.append(decls.substr(i));
    return out;
}

// ---- C text ----------------------------------------------------------------------------------------------------------

// The index past the parenthesis that closes the one before `i` (string and character literals skipped), or npos.
std::size_t close_paren(std::string_view text, std::size_t i) {
    int depth = 1;
    while (i < text.size()) {
        const char c = text[i];
        if (c == '"' || c == '\'') {
            std::size_t j = i + 1;
            while (j < text.size() && text[j] != c) j += text[j] == '\\' ? 2 : 1;
            i = j + 1;
            continue;
        }
        if (c == '(') ++depth;
        if (c == ')') --depth;
        ++i;
        if (depth == 0) return i;
    }
    return npos;
}

// An argument list cut at its top-level commas.
std::vector<std::string> split_arguments(std::string_view inner) {
    std::vector<std::string> out;
    int depth = 0;
    std::size_t start = 0, i = 0;
    while (i < inner.size()) {
        const char c = inner[i];
        if (c == '"' || c == '\'') {
            std::size_t j = i + 1;
            while (j < inner.size() && inner[j] != c) j += inner[j] == '\\' ? 2 : 1;
            i = j + 1;
            continue;
        }
        if (c == '(' || c == '[' || c == '{') ++depth;
        if (c == ')' || c == ']' || c == '}') --depth;
        if (c == ',' && depth == 0) {
            out.emplace_back(inner.substr(start, i - start));
            start = i + 1;
        }
        ++i;
    }
    if (!text::trim(inner).empty()) out.emplace_back(inner.substr(std::min(start, inner.size())));
    return out;
}

bool in_set(std::string_view s, std::initializer_list<std::string_view> set) {
    return std::find(set.begin(), set.end(), s) != set.end();
}

// An offset followed by the segment register it is in (right-to-left order: the offset is pushed last, so it comes first) is
// one far pointer, as the source had it.
std::vector<std::string> far_pairs(const std::vector<std::string>& args) {
    static const Regex low_word(R"(^\s*\(\s*u?int2\s*\)\s*(.+?)\s*$)");
    static const Regex high_word(R"(^\s*\(\s*u?int2\s*\)\s*\(\s*\(\s*u?int4\s*\)\s*(.+?)\s*>>\s*(?:0x10|16)\s*\)\s*$)");
    static const Regex cast_prefix(R"(^\s*\(\s*u?int2\s*\)\s*)");
    std::vector<std::string> out;
    std::size_t k = 0;
    while (k < args.size()) {
        std::optional<RegexMatch> lo, hi;
        if (k + 1 < args.size()) lo = low_word.match(args[k]);
        if (lo) hi = high_word.match(args[k + 1]);
        if (lo && hi && text::trim(lo->group(1), "() ") == text::trim(hi->group(1), "() ")) {
            out.push_back(" " + std::string(lo->group(1)));   // a 4-byte value split into its words: the value
            k += 2;
            continue;
        }
        if (k + 1 < args.size() && in_set(text::trim(args[k + 1]), {"unaff_SS", "codebreaker_ss()", "unaff_DS", "codebreaker_ds()"})) {
            const std::string off(text::trim(cast_prefix.replace_first(args[k], "")));
            out.push_back(" (void __far *)" + (off.starts_with("&") ? off : "(char __near *)(" + off + ")"));
            k += 2;
        } else {
            out.push_back(args[k]);
            ++k;
        }
    }
    return out;
}

// A parenthesis-led call `name(` at or after `from`: where the name begins, where the arguments do, and the name.
struct CallStart {
    std::size_t begin = 0;
    std::size_t args = 0;
    std::string name;
};

std::optional<CallStart> next_call(std::string_view text, std::size_t from) {
    static const Regex call(R"(\b(\w+)\s*\()");
    const auto m = call.search(text, from);
    if (!m) return std::nullopt;
    return CallStart{m->start(), m->end(), std::string(m->group(1))};
}

std::string call_join(const std::vector<std::string>& args) {
    std::string out;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (i) out += ", ";
        out += text::trim(args[i]);
    }
    return out;
}

const Regex& import_pattern() {
    static const Regex re(R"(^extern [^;\n]*__stdcall\s+([A-Z][A-Z0-9]*)_(\d+)\s*\(\s*\);[ \t]*$)", RegexFlags::Multiline);
    return re;
}

// ---- the head of a definition: `(.*?\b\w+\s*\()([\w\s,]*)(\)\s*\n)(.*?)(\{.*)` ----------------------------------------------

struct Definition {
    std::string head, params, close, decls, body;
};

std::optional<Definition> split_definition(std::string_view text) {
    static const Regex re(R"((.*?\b\w+\s*\()([\w\s,]*)(\)\s*\n)(.*?)(\{.*))", RegexFlags::DotAll);
    const auto m = re.match(text);
    if (!m) return std::nullopt;
    return Definition{std::string(m->group(1)), std::string(m->group(2)), std::string(m->group(3)), std::string(m->group(4)), std::string(m->group(5))};
}

std::vector<std::string> parameter_names(std::string_view list) {
    std::vector<std::string> out;
    for (const std::string_view p : text::split(list, ',')) {
        if (!text::trim(p).empty()) out.emplace_back(text::trim(p));
    }
    return out;
}

std::string join_commas(const std::vector<std::string>& parts) { return text::join(parts, ","); }

}  // namespace

// ---- dialect and pascal order ------------------------------------------------------------------------------------------

std::string dialect(std::string_view header) {
    static constexpr std::string_view kSaid[] = {
        "#define __far _far", "#define __near _near", "#define __stdcall _pascal", "#define __cdecl _cdecl",
        "#define codebreaker_ss() 0", "#define codebreaker_ds() 0", "#define codebreaker_cs() 0",
        // OpenWatcom's spellings of what Microsoft C 6 has with one underscore, and the far-pointer macros OpenWatcom's i86.h
        // gives (here without a header)
        "#define __based _based", "#define __segname _segname", "#define __export _export", "#define __loadds _loadds",
        "#define __interrupt _interrupt", "#define __saveregs _saveregs",
        // (OpenWatcom's _Packed: the frame structure c_text writes is of words -- no padding to lose)
        "#define _Packed",
        // (Microsoft's _fastcall: arguments in AX, DX, BX -- describe's __fastcall16 model)
        "#define __fastcall _fastcall", "#define __fastcall16near __near __fastcall", "#define __fastcall16far __far __fastcall",
        "#define MK_FP(s,o) ((void _far *)(((unsigned long)(unsigned)(s) << 16) | (unsigned)(o)))",
        "#define FP_OFF(p) ((unsigned)(unsigned long)(void _far *)(p))",
        "#define FP_SEG(p) ((unsigned)((unsigned long)(void _far *)(p) >> 16))"};
    std::string out;
    for (const std::string_view line : kSaid) {
        out.append(line);
        out.push_back('\n');
    }
    for (const std::string_view line : text::split(header, '\n')) {
        const std::string_view stripped = text::trim(line);
        constexpr std::string_view kIntrinsic = "uint2 codebreaker_";
        const bool segment_function = stripped.starts_with(kIntrinsic) && stripped.size() >= kIntrinsic.size() + 1 &&
                                      (stripped[kIntrinsic.size()] == 's' || stripped[kIntrinsic.size()] == 'd' || stripped[kIntrinsic.size()] == 'c') &&
                                      stripped.substr(kIntrinsic.size() + 1).starts_with("s(void);");
        if (line.find("#pragma aux") != npos || segment_function) continue;
        std::string kept = text::replace_all(line, "unsigned long long", "unsigned long");
        kept = text::replace_all(kept, "long long", "long");
        kept = text::replace_all(kept, "long double", "double");
        out += kept;
        out.push_back('\n');
    }
    return out;
}

std::set<std::string> pascal_names(std::span<const std::string> texts) {
    // A line that is not a directive, with __stdcall (and the 16-bit far/near forms of it) before a name and its parenthesis.
    static const Regex declared(R"(^(?!\s*#)[^\n]*?__stdcall(?:16(?:far|near))?\b[^;(){}=\n]*?\b(\w+)\s*\()", RegexFlags::Multiline);
    std::set<std::string> out;
    for (const std::string& t : texts) {
        declared.for_each_match(t, [&](const RegexMatch& m) {
            out.emplace(m.group(1));
            return true;
        });
    }
    return out;
}

std::string pascal_order(std::string_view text, const std::set<std::string>& names) {
    std::string out;
    std::size_t i = 0;
    while (true) {
        const auto call = next_call(text, i);
        if (!call) {
            out.append(text.substr(i));
            return out;
        }
        const std::size_t end = close_paren(text, call->args);
        if (!names.contains(call->name) || end == npos) {
            out.append(text.substr(i, call->args - i));
            i = call->args;
            continue;
        }
        std::vector<std::string> args;
        for (const std::string& a : split_arguments(text.substr(call->args, end - 1 - call->args))) {
            args.push_back(pascal_order(a, names));
        }
        args = far_pairs(args);
        std::reverse(args.begin(), args.end());
        out.append(text.substr(i, call->args - i));
        out.append(call_join(args));
        out.push_back(')');
        i = end;
    }
}

// ---- the Windows SDK --------------------------------------------------------------------------------------------------

std::string cdecl_imports(std::string_view header, std::string_view sdk, const OrdinalName& ordinal_name) {
    static const Regex declared_cdecl(R"(\bCDECL\s+(\w+)\s*\()");
    std::set<std::string> c_functions;
    declared_cdecl.for_each_match(sdk, [&](const RegexMatch& m) {
        c_functions.emplace(m.group(1));
        return true;
    });
    return import_pattern().replace_all(header, [&](const RegexMatch& m) -> std::string {
        const auto ordinal = text::parse_int<std::uint32_t>(m.group(2));
        const std::string name = ordinal ? ordinal_name(m.group(1), *ordinal).value_or("") : std::string();
        if (name.starts_with("_") || c_functions.contains(name)) return text::replace_all(m.whole(), "__stdcall", "__cdecl");
        return std::string(m.whole());
    });
}

ExportLookup lookup_in(const wine16::Specs& specs) {
    return [&specs](std::string_view module, std::uint32_t ordinal) -> const wine16::Export* {
        const wine16::Exports& exports = specs.exports(module);
        const auto it = exports.find(ordinal);
        return it == exports.end() ? nullptr : &it->second;
    };
}

namespace {

// How a call's arguments are said by Wine's .spec: its calls found in a text, whole or nested.
struct TypedCall {
    std::size_t begin = 0, args_begin = 0, end = 0;   // [begin, end) is the whole call
    std::string name;
    std::vector<std::string> args;
};

// The calls of the names in `typed` in `text`: each once, its nested calls too when `nested`.
std::vector<TypedCall> typed_calls(std::string_view text, const std::function<bool(const std::string&)>& wanted, bool nested) {
    std::vector<TypedCall> out;
    std::size_t i = 0;
    while (const auto call = next_call(text, i)) {
        i = call->args;   // (a call inside the arguments of another is found by going on from the parenthesis)
        if (!wanted(call->name)) continue;
        const std::size_t end = close_paren(text, call->args);
        if (end == npos) continue;
        out.push_back({call->begin, call->args, end, call->name, split_arguments(text.substr(call->args, end - 1 - call->args))});
        if (!nested) i = end;
    }
    return out;
}

std::uint32_t words_of(wine16::ArgKind kind) { return wine16::width(kind) / 2; }

}  // namespace

WineTyped wine_typing(std::string_view header, const OrderedMap<std::string, std::string>& texts, const ExportLookup& exports) {
    std::map<std::string, const wine16::Export*> typed;
    import_pattern().for_each_match(header, [&](const RegexMatch& m) {
        const auto ordinal = text::parse_int<std::uint32_t>(m.group(2));
        const wine16::Export* e = ordinal ? exports(m.group(1), *ordinal) : nullptr;
        if (e && e->args && (e->convention == wine16::Convention::Pascal || e->convention == wine16::Convention::Cdecl)) {
            typed[std::format("{}_{}", m.group(1), m.group(2))] = e;
        }
        return true;
    });
    const auto is_typed = [&](const std::string& name) { return typed.contains(name); };
    std::map<std::string, bool> fits;
    for (const auto& [name, t] : texts) {
        for (const TypedCall& c : typed_calls(t, is_typed, true)) {
            const wine16::Export& e = *typed.at(c.name);
            std::size_t words = 0;
            for (const wine16::ArgKind a : *e.args) words += words_of(a);
            const bool ok = c.args.size() == e.args->size() || c.args.size() == words;
            const auto seen = fits.find(c.name);
            fits[c.name] = (seen == fits.end() || seen->second) && ok;
        }
    }
    std::map<std::string, const wine16::Export*> use;
    for (const auto& [name, e] : typed) {
        const auto f = fits.find(name);
        if (f == fits.end() || f->second) use[name] = e;
    }
    const auto is_used = [&](const std::string& name) { return use.contains(name); };

    // `text` with each call of a typed import said one argument to a parameter
    std::function<std::string(std::string_view)> said = [&](std::string_view text) -> std::string {
        std::string out;
        std::size_t i = 0;
        for (const TypedCall& c : typed_calls(text, is_used, false)) {
            const wine16::Export& e = *use.at(c.name);
            std::vector<std::string> args;
            for (const std::string& a : c.args) args.push_back(said(text::trim(a)));
            if (args.size() != e.args->size()) {
                std::vector<std::string> joined;
                std::size_t k = 0;
                for (const wine16::ArgKind kind : *e.args) {
                    if (wine16::width(kind) == 4) {
                        const bool pascal = e.convention == wine16::Convention::Pascal;
                        const std::string& hi = pascal ? args[k] : args[k + 1];
                        const std::string& lo = pascal ? args[k + 1] : args[k];
                        const std::string v = std::format("(((uint4)(uint2)({}) << 16) | (uint2)({}))", hi, lo);
                        joined.push_back(wine16::is_pointer(kind) ? "(void __far *)" + v : v);
                        k += 2;
                    } else {
                        joined.push_back(args[k]);
                        ++k;
                    }
                }
                args = std::move(joined);
            }
            out.append(text.substr(i, c.args_begin - i));
            out.append(text::join(args, ", "));
            out.push_back(')');
            i = c.end;
        }
        out.append(text.substr(i));
        return out;
    };
    const auto declare = [&](const RegexMatch& m) -> std::string {
        const std::string name = std::format("{}_{}", m.group(1), m.group(2));
        const auto it = use.find(name);
        if (it == use.end()) return std::string(m.whole());
        const wine16::Export& e = *it->second;
        std::string params;
        for (const wine16::ArgKind a : *e.args) params += (params.empty() ? "" : ", ") + std::string(wine16::c_type(a));
        return std::format("extern {} __far {} {}({});", e.ret16 ? "uint2" : "uint4",
                           e.convention == wine16::Convention::Pascal ? "__stdcall" : "__cdecl", name, params.empty() ? "void" : params);
    };
    WineTyped out;
    out.header = import_pattern().replace_all(header, declare);
    for (const auto& [name, t] : texts) out.texts.insert_or_assign(name, said(t));
    return out;
}

std::string wine_definition(std::string_view text, const wine16::Export* exported) {
    if (!exported || !exported->args) return std::string(text);
    auto def = split_definition(text);
    if (!def) return std::string(text);
    const std::vector<std::string> params = parameter_names(def->params);
    std::vector<wine16::ArgKind> kinds = *exported->args;
    if (exported->convention == wine16::Convention::Pascal) std::reverse(kinds.begin(), kinds.end());
    std::size_t words = 0;
    for (const wine16::ArgKind k : kinds) words += words_of(k);
    if (params.size() != words || params.size() == kinds.size()) return std::string(text);
    std::vector<std::string> out;
    std::size_t k = 0;
    std::string body = def->body, decls = def->decls;
    for (const wine16::ArgKind kind : kinds) {
        if (wine16::width(kind) == 2) {
            out.push_back(params[k]);
            ++k;
            continue;
        }
        const std::string lo = params[k], hi = params[k + 1];
        k += 2;
        out.push_back(lo);
        const bool pointer = wine16::is_pointer(kind);
        const std::string whole = std::format("\x01{}\x01", k);   // (the pair, kept out of the way while its halves change)
        body = replace_pair(body, lo, hi, Before::NotWordOrDot, whole);
        body = replace_word(body, hi, Before::NotWordOrDot, pointer ? "FP_SEG(" + whole + ")" : "((uint2)((uint4)" + whole + " >> 16))");
        body = replace_word(body, lo, Before::NotWordOrDot, pointer ? "FP_OFF(" + whole + ")" : "((uint2)" + whole + ")");
        body = text::replace_all(body, whole, lo);
        decls = remove_declarations(decls, hi);
        decls = retype_declarations(decls, lo, wine16::c_type(kind));
    }
    return def->head + join_commas(out) + def->close + decls + body;
}

std::string sdk_header(std::string_view header, const std::optional<std::string>& sdk) {
    if (sdk && !sdk->empty()) return *sdk + "\n" + std::string(header);
    return std::string(header);
}

Prototypes sdk_prototypes(std::string_view sdk) {
    static const Regex declared(R"(^([^\n#]*?)\bWINAPI\s+(\w+)\s*\(([^()]*)\)\s*;)", RegexFlags::Multiline);
    Prototypes out;
    declared.for_each_match(sdk, [&](const RegexMatch& m) {
        std::vector<std::string> params;
        for (const std::string_view p : text::split(m.group(3), ',')) params.emplace_back(text::trim(p));
        const bool variadic = std::find(params.begin(), params.end(), "...") != params.end();
        const bool none = params.size() == 1 && (params[0] == "void" || params[0] == "VOID" || params[0].empty());
        out.params[std::string(m.group(2))] = variadic ? std::nullopt : std::optional<std::vector<std::string>>(none ? std::vector<std::string>() : params);
        const std::string_view result = text::trim(m.group(1));
        if (result == "void" || result == "VOID") out.no_result.emplace(m.group(2));
        return true;
    });
    return out;
}

std::string sdk_calls(std::string_view text, const Prototypes& protos, const OrdinalName& ordinal_name) {
    static const Regex call(R"(\b([A-Z][A-Z0-9]*)_(\d+)\s*\()");
    static const Regex wide_type(R"(FAR\s*\*|\bLP|\bH?PSTR\b|\bLONG\b|\bDWORD\b|\bFARPROC\b|\bSEGPTR\b|\bCOLORREF\b|\bLRESULT\b)");
    static const Regex pointer_type(R"(FAR\s*\*|\bLP(?!ARAM\b)|\bH?PSTR\b|\bFARPROC\b|\bSEGPTR\b)");
    std::string out;
    std::size_t i = 0;
    while (true) {
        const auto m = call.search(text, i);
        if (!m) {
            out.append(text.substr(i));
            return out;
        }
        const std::size_t end = close_paren(text, m->end());
        const auto ordinal = text::parse_int<std::uint32_t>(m->group(2));
        const std::optional<std::string> name = ordinal ? ordinal_name(m->group(1), *ordinal) : std::nullopt;
        const std::vector<std::string>* params = nullptr;
        if (name && !name->empty()) {
            const auto it = protos.params.find(*name);
            if (it != protos.params.end() && it->second) params = &*it->second;
        }
        if (params && protos.no_result.contains(*name)) {
            // a value taken from a function of no value is not the API: only a statement of its own is
            const std::string_view before = text::rtrim(text.substr(0, m->start()));
            const bool statement_start = before.empty() || before.back() == ';' || before.back() == '{' || before.back() == '}';
            const bool statement_end = end != npos && text::ltrim(text.substr(end)).starts_with(';');
            if (!(statement_start && statement_end)) params = nullptr;
        }
        if (end == npos || !params) {
            out.append(text.substr(i, m->end() - i));
            i = m->end();
            continue;
        }
        std::vector<std::string> args;
        for (const std::string& a : split_arguments(text.substr(m->end(), end - 1 - m->end()))) {
            args.emplace_back(text::trim(sdk_calls(a, protos, ordinal_name)));
        }
        std::vector<bool> wide;
        std::size_t wides = 0;
        for (const std::string& p : *params) {
            wide.push_back(wide_type.test(p));
            wides += wide.back() ? 1 : 0;
        }
        std::vector<std::string> said;
        if (args.size() == params->size()) {
            said = args;
        } else if (args.size() == params->size() + wides) {
            std::size_t k = 0;
            for (std::size_t n = 0; n < params->size(); ++n) {
                if (wide[n]) {
                    const std::string& hi = args[k];
                    const std::string& lo = args[k + 1];
                    said.push_back(pointer_type.test((*params)[n]) ? std::format("MAKELP({}, {})", hi, lo) : std::format("MAKELONG({}, {})", lo, hi));
                    k += 2;
                } else {
                    said.push_back(args[k]);
                    ++k;
                }
            }
        } else {
            out.append(text.substr(i, m->end() - i));
            i = m->end();
            continue;
        }
        out.append(text.substr(i, m->start() - i));
        out.append(*name + "(" + text::join(said, ", ") + ")");
        i = end;
    }
}

// ---- temporaries --------------------------------------------------------------------------------------------------------

namespace {

const Regex& decl_line() {   // a local declared on a line of its own: group 1 is its name
    static const Regex re(R"(^\s+(?:[\w]+\s+)+\**\s*(\w+)\s*;\s*$)");
    return re;
}
const Regex& assign_line() {   // `name = expression;`: groups 1 indent, 2 name, 3 expression
    static const Regex re(R"(^(\s*)(\w+) = (.+);\s*$)");
    return re;
}

// The line of the declaration of `name`, or none.
std::optional<std::size_t> declaration_line(const std::vector<std::string>& lines, std::string_view name) {
    for (std::size_t i = 0; i < lines.size(); ++i) {
        const auto m = decl_line().match(lines[i]);
        if (m && m->group(1) == name) return i;
    }
    return std::nullopt;
}

std::size_t indent_of(std::string_view line) { return line.size() - text::ltrim(line).size(); }

// Lines a < b are in one block: nothing between them closes it, opens one, or is a label.
bool same_block(const std::vector<std::string>& lines, std::size_t a, std::size_t b) {
    const std::size_t indent = indent_of(lines[a]);
    for (std::size_t k = a + 1; k < b; ++k) {
        const std::string_view s = text::trim(lines[k]);
        if (s.empty()) continue;
        if (indent_of(lines[k]) != indent || s.ends_with('{') || s.ends_with(':') || s.starts_with('}')) return false;
    }
    return true;
}

bool is_number(std::string_view s) {   // -?(0x[0-9a-fA-F]+|\d+)
    if (s.starts_with('-')) s.remove_prefix(1);
    if (s.starts_with("0x")) {
        s.remove_prefix(2);
        return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return std::isxdigit(static_cast<unsigned char>(c)) != 0; });
    }
    return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return c >= '0' && c <= '9'; });
}

// An expression that needs no parentheses where it goes: a name, a number, or a call of a name with no calls in it.
bool is_simple_expression(std::string_view s) {
    static const Regex call_of_name(R"([\w.]+\([^()]*\))");
    const auto word = [](std::string_view t) { return !t.empty() && std::all_of(t.begin(), t.end(), [](char c) { return is_word(c) || c == '.'; }); };
    std::string_view digits = s.starts_with('-') ? s.substr(1) : s;
    const bool decimal = !digits.empty() && std::all_of(digits.begin(), digits.end(), [](char c) { return c >= '0' && c <= '9'; });
    return word(s) || decimal || call_of_name.full_match(s).has_value();
}

// Cut the declaration of a name that is no longer used: `\n\s+(?:\w+\s+)+\**\s*NAME\s*;[ \t]*(?=\n)`, with NAME at `at`.
bool erase_declaration_at(std::string& s, std::size_t at, std::size_t name_size) {
    std::size_t end = at + name_size;
    while (end < s.size() && is_space(s[end])) ++end;
    if (end >= s.size() || s[end] != ';') return false;
    ++end;
    while (end < s.size() && (s[end] == ' ' || s[end] == '\t')) ++end;
    if (end >= s.size() || s[end] != '\n') return false;
    // backwards: \s*, \**, then the type words, each followed by white space
    std::size_t before_name = at;
    while (before_name > 0 && is_space(s[before_name - 1])) --before_name;
    std::size_t stars = before_name;
    while (stars > 0 && s[stars - 1] == '*') --stars;
    std::vector<std::size_t> word_starts;   // where each word of the type begins, nearest first
    std::size_t cursor = stars == before_name ? at : stars;   // (with no stars the space before the name ends the last word)
    while (true) {
        std::size_t ws = cursor;
        while (ws > 0 && is_space(s[ws - 1])) --ws;
        if (ws == cursor) break;   // (a word must be followed by white space)
        std::size_t w = ws;
        while (w > 0 && is_word(s[w - 1])) --w;
        if (w == ws) break;
        word_starts.push_back(w);
        cursor = w;
    }
    // the leftmost match starts at a '\n' with at least one white-space character before the first word
    for (auto it = word_starts.rbegin(); it != word_starts.rend(); ++it) {
        const std::size_t first = *it;
        std::size_t run = first;
        while (run > 0 && is_space(s[run - 1])) --run;
        for (std::size_t q = run; q + 1 < first; ++q) {
            if (s[q] == '\n') {
                s.erase(q, end - q);
                return true;
            }
        }
    }
    return false;
}

}  // namespace

std::string fold_temporaries(std::string_view text_in) {
    static const Regex loop_head(R"(^\s*(?:\}\s*)?(?:while|for|do)\b)");
    static const Regex declared_with_value(R"(^\s+(?:\w+\s+)+\**\s*\w+ = [^;]*;\s*$)");
    std::vector<std::string> lines;
    for (const std::string_view l : text::split(text_in, '\n')) lines.emplace_back(l);
    std::set<std::string> locals;
    std::size_t body = 0;
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const std::string& ln = lines[k];
        if (text::trim(ln) == "{" && body == 0) {
            body = k + 1;
            continue;
        }
        if (body != 0 && k >= body) {
            const auto m = decl_line().match(ln);
            if (m && ln.find('(') == npos && ln.find('=') == npos) {
                locals.emplace(m->group(1));
            } else if (declared_with_value.match(ln)) {
                continue;   // (one declared with its value: not a temporary)
            } else if (!text::trim(ln).empty()) {
                break;
            }
        }
    }
    bool changed = true;
    while (changed) {
        changed = false;
        for (std::size_t k = 0; k < lines.size(); ++k) {
            const auto m = assign_line().match(lines[k]);
            if (!m || !locals.contains(std::string(m->group(2)))) continue;
            const std::string name(m->group(2));
            const std::string expr(m->group(3));
            const std::size_t indent_width = m->group(1).size();
            const auto decl = declaration_line(lines, name);
            std::vector<std::size_t> uses;
            for (std::size_t j = 0; j < lines.size(); ++j) {
                if (j == k || (decl && j == *decl)) continue;
                if (find_word(lines[j], name, 0, Before::NotWordDotArrow, true) != npos) uses.push_back(j);
            }
            std::optional<std::size_t> next;
            for (std::size_t i = k + 1; i < lines.size(); ++i) {
                if (!text::trim(lines[i]).empty()) {
                    next = i;
                    break;
                }
            }
            if (uses.empty() || !next) continue;
            const auto again = assign_line().match(lines[*next]);
            const bool constant = is_number(text::trim(expr));
            std::size_t j = 0;
            if (again && again->group(2) == name && count_word(again->group(3), name, Before::NotWordDotArrow, true) == 1) {
                j = *next;   // `v = f(); v = g(v + 1);`: the first value dies there
            } else if (uses.size() == 1 && uses[0] == *next) {
                j = *next;
            } else if (constant && uses.size() == 1 && same_block(lines, k, uses[0])) {
                j = uses[0];   // a constant moves past what comes between
            } else {
                continue;
            }
            const auto target = assign_line().match(lines[j]);
            std::string head, tail;
            if (target && target->group(2) == name && j == *next && again) {
                head = lines[j].substr(0, target->start(3));
                tail = std::string(target->group(3)) + ";";
            } else {
                tail = lines[j];
            }
            if (count_word(tail, name, Before::NotWordDotArrow, true) != 1 || loop_head.match(lines[j]) ||
                text::ltrim(lines[j]).starts_with('}')) {
                continue;
            }
            if (indent_of(lines[j]) != indent_width) continue;
            const bool simple = is_simple_expression(text::trim(expr));
            const std::size_t at = find_word(tail, name, 0, Before::NotWordDotArrow, true);
            const std::string shown = simple ? expr : "(" + expr + ")";
            lines[j] = head + tail.substr(0, at) + shown + tail.substr(at + name.size());
            lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k));
            changed = true;
            break;
        }
    }
    std::string out = text::join(lines, "\n");
    for (const std::string& name : locals) {   // a declaration left with nothing to declare
        if (count_word(out, name, Before::NotWordDotArrow) == 1) {
            erase_declaration_at(out, find_word(out, name, 0, Before::NotWordDotArrow), name.size());
        }
    }
    return out;
}

std::string join_far_parameters(std::string_view text_in) {
    const auto def = split_definition(text_in);
    if (!def) return std::string(text_in);
    std::vector<std::string> params = parameter_names(def->params);
    std::vector<std::pair<std::string, std::string>> joined;
    std::set<std::string> used;
    for (std::size_t i = 0; i + 1 < params.size(); ++i) {
        const std::string& lo = params[i];
        const std::string& hi = params[i + 1];
        if (used.contains(lo) || used.contains(hi)) continue;
        const std::size_t pairs = count_pairs(def->body, lo, hi, Before::Boundary);
        const std::size_t alone = count_word(def->body, lo, Before::NotWordOrDot) + count_word(def->body, hi, Before::NotWordOrDot);
        if (pairs && alone == 2 * pairs && has_declaration(def->decls, hi)) {
            joined.emplace_back(lo, hi);
            used.insert(lo);
            used.insert(hi);
        }
    }
    if (joined.empty()) return std::string(text_in);
    std::string body = def->body, decls = def->decls;
    for (const auto& [lo, hi] : joined) {
        params.erase(std::find(params.begin(), params.end(), hi));
        body = replace_pair(body, lo, hi, Before::Boundary, lo);
        decls = remove_declarations(decls, hi);
        decls = retype_declarations(decls, lo, "void __far *");
    }
    return def->head + join_commas(params) + def->close + decls + body;
}

// ---- the frame structure ----------------------------------------------------------------------------------------------

namespace {

std::optional<std::int64_t> size_of_type(std::string_view type) {
    static constexpr std::pair<std::string_view, std::int64_t> kSizes[] = {
        {"char", 1}, {"int1", 1}, {"uint1", 1}, {"xunknown1", 1}, {"bool", 1}, {"undefined", 1}, {"undefined1", 1},
        {"int2", 2}, {"uint2", 2}, {"xunknown2", 2}, {"undefined2", 2}, {"short", 2},
        {"int4", 4}, {"uint4", 4}, {"xunknown4", 4}, {"undefined4", 4}, {"long", 4}};
    for (const auto& [name, size] : kSizes) {
        if (name == type) return size;
    }
    return std::nullopt;
}

struct FrameMember {
    std::int64_t base = 0;   // from the frame pointer (negative: below it)
    std::string type;
    std::int64_t count = 1;
};

struct FrameVariable {
    std::string name, type;
    std::int64_t count = 1;
};

std::int64_t number_of(std::string_view s) { return text::parse_int<std::int64_t>(s, 0).value_or(0); }

}  // namespace

std::string split_frame(std::string_view text_in) {
    static const Regex frame(R"(\n[ \t]*_Packed struct \{\n((?:[ \t]*[\w ]+\s+\w+\s*(?:\[\d+\])?\s*;\n)+)[ \t]*\} codebreaker_frame;)");
    static const Regex member(R"(([\w ]+?)\s+(\w+?_([0-9a-f]+))\s*(?:\[(\d+)\])?\s*;)");
    static const Regex at_form(R"((&)?\(\*\((\w+) \*\)\(\(uint1 \*\)codebreaker_frame\.(\w+) \+ (0x[0-9a-f]+|\d+)\)\))");
    static const Regex index_form(R"(codebreaker_frame\.(\w+)\[(0x[0-9a-f]+|\d+)\])");
    static const Regex ptr_form(R"(\(codebreaker_frame\.(\w+) \+ (-?(?:0x[0-9a-f]+|\d+))\))");
    static const Regex bare_form(R"(codebreaker_frame\.(\w+)\b)");
    const std::string whole(text_in);
    const auto fm = frame.search(whole);
    if (!fm) return whole;
    std::map<std::string, FrameMember> members;
    member.for_each_match(fm->group(1), [&](const RegexMatch& m) {
        const auto offset = text::parse_int<std::int64_t>(m.group(3), 16);
        members[std::string(m.group(2))] = FrameMember{-offset.value_or(0), std::string(text::trim(m.group(1))),
                                                       m.has(4) ? text::parse_int<std::int64_t>(m.group(4)).value_or(1) : 1};
        return true;
    });
    if (members.empty()) return whole;
    const std::string body = whole.substr(fm->end());
    std::map<std::int64_t, std::int64_t> scalars;
    std::set<std::int64_t> starts;
    bool lost = false;
    at_form.for_each_match(body, [&](const RegexMatch& m) {
        const auto mem = members.find(std::string(m.group(3)));
        const auto size = size_of_type(m.group(2));
        if (mem == members.end() || !size) {
            lost = true;
            return false;
        }
        const std::int64_t at = mem->second.base + number_of(m.group(4));
        scalars[at] = std::max(scalars.contains(at) ? scalars[at] : 0, *size);
        if (m.has(1)) starts.insert(at);
        return true;
    });
    if (lost) return whole;
    index_form.for_each_match(body, [&](const RegexMatch& m) {
        const auto mem = members.find(std::string(m.group(1)));
        if (mem == members.end()) {
            lost = true;
            return false;
        }
        const std::int64_t unit = size_of_type(mem->second.type).value_or(1);
        const std::int64_t at = mem->second.base + number_of(m.group(2)) * unit;
        scalars[at] = std::max(scalars.contains(at) ? scalars[at] : 0, unit);
        return true;
    });
    if (lost) return whole;
    ptr_form.for_each_match(body, [&](const RegexMatch& m) {   // (a member's address moved on: a place of its own)
        const auto mem = members.find(std::string(m.group(1)));
        if (mem == members.end()) {
            lost = true;
            return false;
        }
        starts.insert(mem->second.base + number_of(m.group(2)));
        return true;
    });
    if (lost) return whole;
    const std::string rest = ptr_form.replace_all(index_form.replace_all(at_form.replace_all(body, ""), ""), "");
    bare_form.for_each_match(rest, [&](const RegexMatch& m) {
        const auto mem = members.find(std::string(m.group(1)));
        if (mem == members.end()) {
            lost = true;
            return false;
        }
        starts.insert(mem->second.base);
        return true;
    });
    if (lost) return whole;
    std::int64_t high = std::numeric_limits<std::int64_t>::min();
    for (const auto& [name, mem] : members) high = std::max(high, mem.base + size_of_type(mem.type).value_or(1) * mem.count);
    std::set<std::int64_t> point_set(starts);
    for (const auto& [at, size] : scalars) point_set.insert(at);
    const std::vector<std::int64_t> points(point_set.begin(), point_set.end());
    if (points.empty()) return whole;
    std::map<std::int64_t, FrameVariable> var;
    for (std::size_t i = 0; i < points.size(); ++i) {
        const std::int64_t at = points[i];
        const std::int64_t end = i + 1 < points.size() ? points[i + 1] : high;
        const std::int64_t span = end - at;
        const std::string name = at < 0 ? std::format("local_{:x}", -at) : std::format("local_p{:x}", at);
        if (starts.contains(at) && (!scalars.contains(at) || span > scalars[at])) {
            var[at] = {name, "xunknown1", span};   // a buffer: its address is passed on
        } else {
            const std::int64_t size = scalars.contains(at) ? scalars[at] : span;
            if (size > span) return whole;   // (reads that overlap: not locals)
            const std::string type = size == 2 ? "int2" : size == 4 ? "int4" : "xunknown1";
            var[at] = {name, type, (size == 1 || size == 2 || size == 4) ? 1 : span};
        }
    }
    // the variable holding `at`, and how far into it
    const auto place = [&](std::int64_t at) {
        const auto it = std::upper_bound(points.begin(), points.end(), at);
        const std::int64_t b = *(it - 1);
        return std::pair<const FrameVariable&, std::int64_t>(var.at(b), at - b);
    };
    const auto at_text = [&](const RegexMatch& m) {
        const FrameMember& mem = members.at(std::string(m.group(3)));
        const std::string typ(m.group(2));
        const auto [v, d] = place(mem.base + number_of(m.group(4)));
        std::string ref;
        if (d == 0 && v.count == 1 && v.type == typ) {
            ref = v.name;
        } else if (d == 0 && v.count == 1 && size_of_type(v.type) == size_of_type(typ)) {
            ref = std::format("(*({} *)&{})", typ, v.name);
        } else {
            ref = std::format("(*({} *)((uint1 *)&{} + {}))", typ, v.name, d);
        }
        return m.has(1) ? "&" + ref : ref;
    };
    const auto index_text = [&](const RegexMatch& m) {
        const FrameMember& mem = members.at(std::string(m.group(1)));
        const auto [v, d] = place(mem.base + number_of(m.group(2)) * size_of_type(mem.type).value_or(1));
        return d == 0 && v.count == 1 && v.type == mem.type ? v.name : std::format("(*({} *)((uint1 *)&{} + {}))", mem.type, v.name, d);
    };
    const auto address_text = [&](const FrameMember& mem, std::int64_t at) {
        const auto [v, d] = place(at);
        return d ? std::format("(({} *)((uint1 *)&{} + {}))", mem.type, v.name, d) : std::format("(({} *)&{})", mem.type, v.name);
    };
    const auto bare_text = [&](const RegexMatch& m) {
        const FrameMember& mem = members.at(std::string(m.group(1)));
        return address_text(mem, mem.base);
    };
    const auto ptr_text = [&](const RegexMatch& m) {
        const FrameMember& mem = members.at(std::string(m.group(1)));
        return address_text(mem, mem.base + number_of(m.group(2)));
    };
    const std::string rewritten = bare_form.replace_all(
        ptr_form.replace_all(index_form.replace_all(at_form.replace_all(body, std::function<std::string(const RegexMatch&)>(at_text)),
                                                    std::function<std::string(const RegexMatch&)>(index_text)),
                             std::function<std::string(const RegexMatch&)>(ptr_text)),
        std::function<std::string(const RegexMatch&)>(bare_text));
    std::string decls;
    for (auto it = var.rbegin(); it != var.rend(); ++it) {   // nearest the frame pointer first
        const FrameVariable& v = it->second;
        decls += std::format("\n  {} {}{};", v.type, v.name, v.count > 1 ? std::format("[{}]", v.count) : std::string());
    }
    return whole.substr(0, fm->start()) + decls + rewritten;
}

DsGlobals ds_globals(std::string_view text_in) {
    static const Regex deref(R"(\*\s*\(\s*((?:unsigned |signed )?\w+)\s*\*\s*\)\s*(0x[0-9a-fA-F]+|\d+)\b)");
    std::set<std::string> seen;
    DsGlobals out;
    out.text = deref.replace_all(text_in, [&](const RegexMatch& m) -> std::string {
        const std::string type(m.group(1));
        const std::int64_t at = number_of(m.group(2));
        if (at < 0x10 || type == "void" || type == "code") return std::string(m.whole());
        const std::string name = std::format("codebreaker_d{:04x}", at);
        seen.insert(name);
        return std::format("(*({} *){})", type, name);
    });
    for (const std::string& n : seen) out.declarations += std::format("extern char {}[];\n", n);
    return out;
}

}  // namespace cb::build::msc6
