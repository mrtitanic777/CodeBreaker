// cb/port/c_source.cpp -- see c_source.hpp.

#include "cb/port/c_source.hpp"

#include <algorithm>
#include <array>
#include <cctype>

#include "cb/base/ctext.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::port::pre_ansi {

namespace {

bool is_alpha_(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
bool is_digit_(char c) { return c >= '0' && c <= '9'; }
bool ident_start(char c) { return is_alpha_(c) || c == '_'; }

template <std::size_t N>
bool in_sorted(const std::array<std::string_view, N>& words, std::string_view w) {
    return std::binary_search(words.begin(), words.end(), w);
}

}  // namespace

// ---- vocabulary ------------------------------------------------------------------------------------------------

bool is_keyword(std::string_view w) {
    static constexpr std::array<std::string_view, 32> kWords = {
        "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern",
        "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
    return in_sorted(kWords, w);
}

bool is_type_word(std::string_view w) {
    static constexpr std::array<std::string_view, 26> kWords = {
        "FILE", "auto", "char", "const", "double", "enum", "extern", "float", "int", "int16_t", "int32_t", "int8_t",
        "intptr_t", "long", "register", "short", "signed", "static", "struct", "uint16_t", "uint32_t", "uint8_t",
        "uintptr_t", "union", "unsigned", "void"};
    // (volatile sorts after void: kept out of the array's order on purpose, tested separately)
    return in_sorted(kWords, w) || w == "volatile";
}

bool rule_enabled(std::string_view name) {
    // the rules the port keeps on by default: all of them (the set that is off is empty)
    bool on = true;
    const std::optional<std::string> env = fs::get_env("CB_PORT_RULES");
    if (!env) return on;
    std::string spec;
    for (const char c : *env)
        if (c != ' ') spec.push_back(c);
    for (std::string_view w : text::split(spec, ',')) {
        if (w == name || (w.size() == name.size() + 1 && w[0] == '+' && w.substr(1) == name) || w == "+all") {
            on = true;
        } else if ((w.size() == name.size() + 1 && w[0] == '-' && w.substr(1) == name) || w == "-all") {
            on = false;
        }
    }
    return on;
}

// ---- lines and braces --------------------------------------------------------------------------------------------

std::vector<std::string> split_lines(std::string_view text) {
    std::vector<std::string> out;
    for (std::string_view piece : text::split(text, '\n')) out.emplace_back(piece);
    return out;
}

std::string join_lines(const std::vector<std::string>& lines) { return text::join(lines, "\n"); }

std::vector<int> brace_depths(const std::vector<std::string>& code_lines) {
    std::vector<int> out;
    out.reserve(code_lines.size());
    int d = 0;
    for (const std::string& l : code_lines) {
        out.push_back(d);
        for (char c : l) d += (c == '{') - (c == '}');
    }
    return out;
}

std::size_t match_brace(std::string_view code, std::size_t open) {
    int d = 0;
    std::size_t k = open;
    while (k < code.size()) {
        d += (code[k] == '{') - (code[k] == '}');
        if (d == 0) break;
        ++k;
    }
    return k;
}

// ---- text helpers ------------------------------------------------------------------------------------------------

bool is_word_char(char c) { return is_alpha_(c) || is_digit_(c) || c == '_'; }

std::string collapse_space(std::string_view s) {
    std::string out;
    out.reserve(s.size());
    bool in_space = false;
    for (char c : s) {
        if (text::is_space(c)) {
            if (!in_space) out.push_back(' ');
            in_space = true;
        } else {
            out.push_back(c);
            in_space = false;
        }
    }
    return out;
}

std::size_t find_word(std::string_view text, std::string_view word, std::size_t from) {
    if (word.empty()) return std::string_view::npos;
    for (std::size_t at = text.find(word, from); at != std::string_view::npos; at = text.find(word, at + 1)) {
        const bool before = at == 0 || !is_word_char(text[at - 1]);
        const bool after = at + word.size() >= text.size() || !is_word_char(text[at + word.size()]);
        if (before && after) return at;
    }
    return std::string_view::npos;
}

std::string regex_quote(std::string_view s, bool spaces_flex) {
    std::string out;
    for (char c : s) {
        if (c == ' ' && spaces_flex) {
            out += "\\s+";
        } else if (is_word_char(c) || static_cast<unsigned char>(c) >= 0x80) {
            out.push_back(c);
        } else {
            out.push_back('\\');
            out.push_back(c);
        }
    }
    return out;
}

std::string_view indent_of(std::string_view line) { return line.substr(0, line.size() - text::ltrim(line).size()); }

// ---- declarations -------------------------------------------------------------------------------------------------

std::vector<std::string_view> split_top(std::string_view s, char sep) {
    std::vector<std::string_view> parts;
    int d = 0;
    std::size_t start = 0;
    for (std::size_t i = 0; i < s.size(); ++i) {
        const char c = s[i];
        if (c == '(' || c == '[') {
            ++d;
        } else if (c == ')' || c == ']') {
            --d;
        }
        if (c == sep && d == 0) {
            parts.push_back(s.substr(start, i - start));
            start = i + 1;
        }
    }
    parts.push_back(s.substr(start));
    return parts;
}

namespace {

struct Token {
    std::size_t start, end;
};

// The identifiers of a text, [A-Za-z_]\w* scanned left to right (a number's tail such as the x1f of 0x1f counts).
std::vector<Token> identifiers(std::string_view s) {
    std::vector<Token> out;
    std::size_t i = 0;
    while (i < s.size()) {
        if (!ident_start(s[i])) {
            ++i;
            continue;
        }
        std::size_t j = i + 1;
        while (j < s.size() && is_word_char(s[j])) ++j;
        out.push_back({i, j});
        i = j;
    }
    return out;
}

}  // namespace

std::optional<Declaration> declarators(std::string_view decl) { return declarators(decl, false); }

std::optional<Declaration> declarators(std::string_view decl, bool size_t_is_type) {
    std::string_view d = text::trim(decl);
    while (!d.empty() && d.back() == ';') d.remove_suffix(1);
    const std::vector<std::string_view> parts = split_top(d, ',');
    const std::string_view first = parts[0];
    const std::vector<Token> toks = identifiers(first);
    std::optional<Token> name;
    for (std::size_t i = 0; i < toks.size(); ++i) {
        const std::string_view w = first.substr(toks[i].start, toks[i].end - toks[i].start);
        if (is_type_word(w) || (size_t_is_type && w == "size_t")) continue;
        if (i > 0) {
            const std::string_view prev = first.substr(toks[i - 1].start, toks[i - 1].end - toks[i - 1].start);
            if (prev == "struct" || prev == "union" || prev == "enum") continue;
        }
        name = toks[i];
        break;
    }
    if (!name) return std::nullopt;
    std::size_t start = name->start;
    while (start > 0 && (first[start - 1] == ' ' || first[start - 1] == '\t' || first[start - 1] == '*' || first[start - 1] == '('))
        --start;
    while (start < name->start && (first[start] == ' ' || first[start] == '\t')) ++start;
    Declaration out;
    out.base = std::string(text::trim(first.substr(0, start)));
    out.names.push_back({std::string(first.substr(name->start, name->end - name->start)),
                         std::string(text::trim(first.substr(start)))});
    for (std::size_t i = 1; i < parts.size(); ++i) {
        const std::vector<Token> t = identifiers(parts[i]);
        if (t.empty()) return std::nullopt;
        out.names.push_back({std::string(parts[i].substr(t[0].start, t[0].end - t[0].start)),
                             std::string(text::trim(parts[i]))});
    }
    return out;
}

std::string array_param(std::string_view type, std::string_view name) {
    // T a[] or T a[][3]: the whole type is `<lead>NAME [..] ([..])*` with nothing after; the lead ends at the first
    // place where the name stands as a word, a bracket follows and the brackets run to the end.
    if (type.find('\n') != std::string_view::npos) return std::string(type);
    for (std::size_t at = find_word(type, name); at != std::string_view::npos; at = find_word(type, name, at + 1)) {
        std::size_t i = at + name.size();
        while (i < type.size() && text::is_space(type[i])) ++i;
        if (i >= type.size() || type[i] != '[') continue;
        // the first bracket pair, then any number of further ones, then only whitespace
        auto skip_pair = [&](std::size_t from) -> std::optional<std::size_t> {
            const std::size_t close = type.find(']', from + 1);
            if (close == std::string_view::npos) return std::nullopt;
            return close + 1;
        };
        const auto first_end = skip_pair(i);
        if (!first_end) continue;
        std::size_t rest_begin = *first_end, pos = *first_end;
        bool ok = true;
        while (true) {
            std::size_t j = pos;
            while (j < type.size() && text::is_space(type[j])) ++j;
            if (j >= type.size()) {
                pos = j;
                break;
            }
            if (type[j] != '[') {
                ok = false;
                break;
            }
            const auto e = skip_pair(j);
            if (!e) {
                ok = false;
                break;
            }
            pos = *e;
        }
        if (!ok) continue;
        const std::string_view lead = type.substr(0, at);
        const std::string_view rest = text::trim(type.substr(rest_begin));
        if (!rest.empty()) return std::string(lead) + "(*" + std::string(name) + ")" + std::string(rest);
        return std::string(lead) + "*" + std::string(name);
    }
    return std::string(type);
}

// ---- functions ----------------------------------------------------------------------------------------------------

std::string Fn::proto() const {
    std::string ps;
    for (std::size_t i = 0; i < params.size(); ++i) {
        if (i) ps += ", ";
        ps += array_param(params[i].type, params[i].name);
    }
    if (ps.empty()) ps = "void";
    const std::string r = returns_void ? "void" : (ret.empty() ? "int" : ret);
    const std::string sep = r.ends_with('*') ? "" : " ";
    const std::string lead = (is_static ? "static " : "") + r + sep + name;
    if (varargs) return lead + "(" + ps + ", ...)";
    if (loose) return lead + "()";
    return lead + "(" + ps + ")";
}

namespace {

// `static`-less return text made only of type words, stars and (tagged) structures
bool ret_is_type(std::string_view ret) {
    static const Regex tagged(R"(\s*(?:static\s+)?(?:struct|union)\s+\w+\s*\**\s*)");
    // every word of it (except struct and union) is a type word or a star
    std::size_t i = 0;
    bool all = true;
    while (i < ret.size()) {
        if (ret[i] == '*') {
            ++i;
        } else if (is_word_char(ret[i])) {
            std::size_t j = i;
            while (j < ret.size() && is_word_char(ret[j])) ++j;
            const std::string_view w = ret.substr(i, j - i);
            if (w != "struct" && w != "union" && !is_type_word(w)) all = false;
            i = j;
        } else {
            ++i;
        }
    }
    return all || tagged.full_match(ret).has_value();
}

}  // namespace

std::vector<Definition> definitions(const std::vector<std::string>& lines, const std::vector<std::string>& code) {
    static const Regex head(R"(^([\w\s*]*?)([A-Za-z_]\w*)\s*\(([^()]*)\)\s*$)");
    // a type standing alone on the line before the name: char *\nf(a)  (or one of the library's: FILE *)
    static const Regex type_line(
        R"(\s*(?:static\s+)?(?:(?:unsigned|signed|long|short|int|char|void|float|double|struct\s+\w+|union\s+\w+)\s*)+\**\s*|\s*(?:static\s+)?[A-Z_][A-Z0-9_]*\s*\*+\s*)");
    static const Regex storage_words(R"(\b(?:register|auto|static|extern|const|volatile)\b)");
    static const Regex param_name(R"([A-Za-z_]\w*)");

    std::vector<Definition> out;
    const std::vector<int> ds = brace_depths(code);
    std::size_t i = 0;
    while (i < lines.size()) {
        std::optional<RegexMatch> m;
        if (ds[i] == 0 && !text::ltrim(code[i]).starts_with('#')) m = head.match(text::rtrim(code[i]));
        if (!m || is_keyword(m->group(2)) || !ret_is_type(m->group(1))) {
            ++i;
            continue;
        }
        std::vector<std::string> params;
        for (std::string_view p : text::split(m->group(3), ',')) {
            p = text::trim(p);
            if (!p.empty()) params.emplace_back(p);
        }
        const bool names_only = std::all_of(params.begin(), params.end(), [&](const std::string& p) { return param_name.full_match(p).has_value(); });
        if (!names_only || (params.size() == 1 && params[0] == "void")) {
            ++i;
            continue;
        }
        std::size_t first = i;
        if (text::trim(m->group(1)).empty() && i > 0 && ds[i - 1] == 0 && !text::trim(code[i - 1]).empty() &&
            type_line.full_match(code[i - 1]))
            first = i - 1;
        std::size_t j = i + 1;
        std::vector<std::string> decls;
        while (j < lines.size() && !text::trim(code[j]).starts_with('{')) {
            const std::string_view s = text::trim(code[j]);
            if (!s.empty() && !s.ends_with(';')) break;
            if (!s.empty()) decls.emplace_back(s);
            ++j;
        }
        if (j >= lines.size() || !text::trim(code[j]).starts_with('{')) {
            ++i;
            continue;
        }
        std::map<std::string, std::string> types;
        for (const std::string& d : decls) {
            auto r = declarators(d);
            if (!r) continue;
            std::string base = r->base;
            if (text::trim(storage_words.replace_all(base, "")).empty()) base = std::string(text::trim(base + " int"));
            for (const Declarator& dn : r->names) types[dn.name] = std::string(text::trim(base + " " + dn.text));
        }
        std::string ret(first == i ? text::trim(m->group(1)) : text::trim(code[first]));
        const bool is_static = ret.starts_with("static") && (ret.size() == 6 || !is_word_char(ret[6]));
        if (ret.starts_with("static")) ret = std::string(text::ltrim(std::string_view(ret).substr(6)));
        ret = std::string(text::trim(ret));
        Fn fn;
        fn.name = std::string(m->group(2));
        fn.ret = ret;
        fn.is_static = is_static;
        fn.line = i + 1;
        for (const std::string& p : params) {
            auto t = types.find(p);
            fn.params.push_back({p, t != types.end() ? t->second : "int " + p});
            if (t == types.end()) fn.implicit.insert(p);
        }
        std::size_t k = j;
        int d = 0;
        while (k < lines.size()) {
            for (char c : code[k]) d += (c == '{') - (c == '}');
            if (d == 0) break;
            ++k;
        }
        k = std::min(k, lines.size() - 1);   // (a file that never closes the brace: its last line)
        out.push_back({std::move(fn), first, j, k});
        i = k + 1;
    }
    return out;
}

std::vector<NamedDefinition> definitions_any(const std::vector<std::string>& lines, const std::vector<std::string>& code) {
    static const Regex head(R"(^[\w\s*]*?\b([A-Za-z_]\w*)\s*\([^;]*\)\s*$)");
    std::vector<NamedDefinition> out;
    const std::vector<int> ds = brace_depths(code);
    std::size_t i = 0;
    while (i < lines.size()) {
        std::optional<RegexMatch> m;
        if (ds[i] == 0) m = head.match(code[i]);
        if (m && !is_keyword(m->group(1)) && i + 1 < lines.size() && text::trim(code[i + 1]).starts_with('{')) {
            const std::size_t j = i + 1;
            std::size_t k = j;
            int d = 0;
            while (k < lines.size()) {
                for (char c : code[k]) d += (c == '{') - (c == '}');
                if (d == 0) break;
                ++k;
            }
            k = std::min(k, lines.size() - 1);
            out.push_back({std::string(m->group(1)), i, j, k});
            i = k + 1;
        } else {
            ++i;
        }
    }
    return out;
}

// ---- calls ---------------------------------------------------------------------------------------------------------

namespace {

// The place just past the '(' of the first call of `fn` at or after `from`: the name a whole word, then optional
// whitespace, then '('. Sets `match_start` to the name's place.
std::size_t find_call(std::string_view text, std::string_view fn, std::size_t from, std::size_t& match_start) {
    for (std::size_t at = find_word(text, fn, from); at != std::string_view::npos; at = find_word(text, fn, at + 1)) {
        std::size_t i = at + fn.size();
        while (i < text.size() && text::is_space(text[i])) ++i;
        if (i < text.size() && text[i] == '(') {
            match_start = at;
            return i + 1;
        }
    }
    return std::string_view::npos;
}

}  // namespace

std::optional<std::vector<Span>> call_args(std::string_view line, std::string_view fn, bool partial) {
    std::size_t at = 0;
    const std::size_t after = find_call(line, fn, 0, at);
    if (after == std::string_view::npos) return std::nullopt;
    std::size_t i = after, start = after;
    int d = 1;
    std::vector<Span> spans;
    while (i < line.size() && d != 0) {
        const char c = line[i];
        if (c == '(' || c == '[' || c == '{') {
            ++d;
        } else if (c == ')' || c == ']' || c == '}') {
            --d;
        }
        if ((c == ',' && d == 1) || d == 0) {
            spans.push_back({start, i});
            start = i + 1;
        }
        ++i;
    }
    if (d == 0 || (partial && d >= 1 && !spans.empty())) return spans;
    return std::nullopt;
}

std::vector<std::size_t> call_starts(std::string_view text, std::string_view fn) {
    std::vector<std::size_t> out;
    std::size_t from = 0;
    while (true) {
        std::size_t at = 0;
        const std::size_t after = find_call(text, fn, from, at);
        if (after == std::string_view::npos) break;
        out.push_back(at);
        from = after;
    }
    return out;
}

std::vector<std::vector<Span>> calls_of(std::string_view body, std::string_view fn) {
    std::vector<std::vector<Span>> out;
    std::size_t from = 0;
    while (true) {
        std::size_t at = 0;
        const std::size_t after = find_call(body, fn, from, at);
        if (after == std::string_view::npos) break;
        from = after;   // (the next call is looked for from the end of this match: calls inside the arguments count)
        std::size_t i = after, start = after;
        int d = 1;
        std::vector<Span> spans;
        while (i < body.size() && d != 0) {
            const char c = body[i];
            if (c == '(' || c == '[') {
                ++d;
            } else if (c == ')' || c == ']') {
                --d;
            }
            if ((c == ',' && d == 1) || d == 0) {
                spans.push_back({start, i});
                start = i + 1;
            }
            ++i;
        }
        out.push_back(std::move(spans));
    }
    return out;
}

std::optional<std::size_t> object_before(std::string_view line, std::size_t at) {
    std::size_t i = at;
    while (true) {
        while (i > 0 && (line[i - 1] == ' ' || line[i - 1] == '\t')) --i;
        if (i > 0 && (line[i - 1] == ')' || line[i - 1] == ']')) {
            const char close = line[i - 1];
            const char open = close == ')' ? '(' : '[';
            int d = 0;
            std::size_t k = i - 1;
            bool found = false;
            while (true) {
                d += (line[k] == close) - (line[k] == open);
                if (d == 0) {
                    found = true;
                    break;
                }
                if (k == 0) break;
                --k;
            }
            if (!found) return std::nullopt;
            i = k;
            if (open == '(' && !(i > 0 && is_word_char(line[i - 1]))) return i;   // a parenthesized object: the whole
            continue;
        }
        // the identifier that ends here
        std::size_t q = i;
        while (q > 0 && is_word_char(line[q - 1])) --q;
        while (q < i && is_digit_(line[q])) ++q;   // (an identifier starts with a letter or '_')
        if (q == i) return i;
        i = q;
        const std::string_view before = line.substr(0, i);
        if (before.ends_with("->")) {
            i -= 2;
            continue;
        }
        if (before.ends_with(".")) {
            i -= 1;
            continue;
        }
        return i;
    }
}

std::string abstract_type(std::string_view decl, std::string_view name) {
    static const Regex register_word(R"(\bregister\s+)");
    static const Regex array_tail(R"(\s*\[\s*\d*\s*\]$)");
    static const Regex space_star(R"(\s+\*)");
    const std::string no_register = register_word.replace_all(decl, "");
    // the first whole-word NAME that is not the tag of a struct, union or enum (struct bcd *bcd: the name, not the tag)
    std::string t = no_register;
    for (std::size_t at = find_word(no_register, name); at != std::string_view::npos; at = find_word(no_register, name, at + 1)) {
        const std::string_view before = std::string_view(no_register).substr(0, at);
        if (before.ends_with("struct ") || before.ends_with("union ") || before.ends_with("enum ")) continue;
        t = no_register.substr(0, at) + no_register.substr(at + name.size());
        break;
    }
    t = std::string(text::trim(t));
    t = array_tail.replace_first(t, " *");
    return space_star.replace_all(t, " *");
}

bool only_tested(std::string_view body, std::string_view name) {
    std::size_t uses = 0;
    for (std::size_t at = find_word(body, name); at != std::string_view::npos; at = find_word(body, name, at + 1)) {
        ++uses;
        // before it: `(` after a start, a keyword or an operator, or `!`, `&&`, `||`
        std::string_view p = text::rtrim(body.substr(0, at));
        bool before_ok = false;
        if (p.ends_with('!') || p.ends_with("&&") || p.ends_with("||")) {
            before_ok = true;
        } else if (p.ends_with('(')) {
            const std::string_view q = text::rtrim(p.substr(0, p.size() - 1));
            if (q.empty()) {
                before_ok = true;
            } else {
                const char last = q.back();
                if (!is_word_char(last) && !text::is_space(last) && last != ']' && last != ')') before_ok = true;
                for (std::string_view kw : {"if", "while", "for", "return"}) {
                    if (q.ends_with(kw) && (q.size() == kw.size() || !is_word_char(q[q.size() - kw.size() - 1]))) before_ok = true;
                }
            }
        }
        if (!before_ok) return false;
        const std::string_view rest = text::ltrim(body.substr(at + name.size()));
        if (!(rest.starts_with(')') || rest.starts_with("&&") || rest.starts_with("||"))) return false;
    }
    return uses > 0;
}

// ---- structures -----------------------------------------------------------------------------------------------------

OrderedMap<std::string, std::vector<std::string>> struct_members(const std::vector<std::string>& texts) {
    static const Regex head(R"(\b(struct|union)\s+(\w+)\s*\{)");
    OrderedMap<std::string, std::vector<std::string>> out;
    for (const std::string& t : texts) {
        const std::string code = base::ctext::blank(t);
        head.for_each_match(code, [&](const RegexMatch& m) {
            const std::size_t open = m.end() - 1;
            const std::size_t close = match_brace(code, open);
            const std::string_view body = std::string_view(code).substr(m.end(), close - m.end());
            for (std::string_view s : text::split(body, ';')) {
                if (text::trim(s).empty()) continue;
                const auto r = declarators(s);
                if (!r) continue;
                for (const Declarator& dn : r->names) out[dn.name].push_back(std::string(m.group(1)) + " " + std::string(m.group(2)));
            }
            return true;
        });
    }
    return out;
}

// ---- literals ---------------------------------------------------------------------------------------------------------

Bytes c_string_bytes(std::string_view literal) {
    Bytes out;
    std::size_t i = 0;
    while (i < literal.size()) {
        if (literal[i] != '"') {
            ++i;
            continue;
        }
        // one quoted part: up to the next quote that no backslash escapes (a backslash before a newline is no part)
        std::size_t j = i + 1;
        bool closed = false, broken = false;
        while (j < literal.size()) {
            if (literal[j] == '\\') {
                if (j + 1 >= literal.size() || literal[j + 1] == '\n') {
                    broken = true;
                    break;
                }
                j += 2;
            } else if (literal[j] == '"') {
                closed = true;
                break;
            } else {
                ++j;
            }
        }
        if (broken || !closed) {
            ++i;
            continue;
        }
        const Bytes part = text::to_latin1(literal.substr(i + 1, j - i - 1));
        for (std::size_t p = 0; p < part.size();) {
            if (part[p] != '\\') {
                out.push_back(part[p++]);
                continue;
            }
            ++p;
            if (p >= part.size()) throw ParseError("<string literal>", 1, static_cast<std::uint32_t>(i + 1), "a backslash ends the literal");
            const char e = static_cast<char>(part[p++]);
            switch (e) {
                case 'a': out.push_back(7); break;
                case 'b': out.push_back(8); break;
                case 'f': out.push_back(12); break;
                case 'n': out.push_back(10); break;
                case 'r': out.push_back(13); break;
                case 't': out.push_back(9); break;
                case 'v': out.push_back(11); break;
                case '\\': case '\'': case '"': case '?': out.push_back(static_cast<std::uint8_t>(e)); break;
                case 'x': {
                    unsigned v = 0;
                    int n = 0;
                    while (p < part.size() && n < 2 && std::isxdigit(part[p])) {
                        const char h = static_cast<char>(part[p++]);
                        v = v * 16 + static_cast<unsigned>(is_digit_(h) ? h - '0' : (h | 0x20) - 'a' + 10);
                        ++n;
                    }
                    if (n < 2) throw ParseError("<string literal>", 1, static_cast<std::uint32_t>(i + 1), "a \\x escape wants two hex digits");
                    out.push_back(static_cast<std::uint8_t>(v));
                    break;
                }
                default:
                    if (e >= '0' && e <= '7') {
                        unsigned v = static_cast<unsigned>(e - '0');
                        int n = 1;
                        while (p < part.size() && n < 3 && part[p] >= '0' && part[p] <= '7') {
                            v = v * 8 + (part[p++] - '0');
                            ++n;
                        }
                        out.push_back(static_cast<std::uint8_t>(v & 0xFF));
                    } else {
                        out.push_back('\\');   // (an escape C does not have: both characters kept, as the compilers' tables did)
                        out.push_back(static_cast<std::uint8_t>(e));
                    }
            }
        }
        i = j + 1;
    }
    return out;
}

}  // namespace cb::port::pre_ansi
