#include "cb/port/csource.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <functional>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"

namespace cb::port::csource {

namespace {

bool is_word(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_';
}
bool is_ident_start(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) != 0 || c == '_';
}
bool is_blank(char c) {
    return c == ' ' || c == '\t';
}

// The index just after the optional whitespace at `at`.
std::size_t skip_space(std::string_view s, std::size_t at) {
    while (at < s.size() && text::is_space(s[at]))
        ++at;
    return at;
}

// Where the word `word` occurs as a whole word at or after `from`, or npos.
std::size_t find_word(std::string_view s, std::string_view word, std::size_t from) {
    if (word.empty())
        return std::string_view::npos;
    for (std::size_t at = s.find(word, from); at != std::string_view::npos; at = s.find(word, at + 1)) {
        const bool before = at == 0 || !is_word(s[at - 1]);
        const bool after = at + word.size() >= s.size() || !is_word(s[at + word.size()]);
        if (before && after)
            return at;
    }
    return std::string_view::npos;
}

// Reads the arguments of the call whose '(' is at `paren`.
std::optional<std::vector<Span>> read_args(std::string_view line, std::size_t paren, bool partial) {
    std::size_t i = paren + 1, start = i;
    int depth = 1;
    std::vector<Span> spans;
    while (i < line.size() && depth != 0) {
        const char c = line[i];
        if (c == '(' || c == '[' || c == '{') {
            ++depth;
        } else if (c == ')' || c == ']' || c == '}') {
            --depth;
        }
        if ((c == ',' && depth == 1) || depth == 0) {
            spans.push_back({start, i});
            start = i + 1;
        }
        ++i;
    }
    if (depth == 0 || (partial && depth >= 1 && !spans.empty()))
        return spans;
    return std::nullopt;
}

} // namespace

std::string collapse_space(std::string_view s) {
    std::string out;
    bool in_space = false;
    for (char c : s) {
        if (text::is_space(c)) {
            if (!in_space)
                out += ' ';
            in_space = true;
        } else {
            out += c;
            in_space = false;
        }
    }
    return out;
}

// ---- vocabulary ---------------------------------------------------------------------------------------

bool is_keyword(std::string_view word) {
    static constexpr std::array<std::string_view, 32> kWords = {
        "auto",   "break",  "case",     "char",   "const",    "continue", "default",  "do",
        "double", "else",   "enum",     "extern", "float",    "for",      "goto",     "if",
        "int",    "long",   "register", "return", "short",    "signed",   "sizeof",   "static",
        "struct", "switch", "typedef",  "union",  "unsigned", "void",     "volatile", "while"};
    return std::find(kWords.begin(), kWords.end(), word) != kWords.end();
}

bool is_type_word(std::string_view word) {
    static constexpr std::array<std::string_view, 24> kWords = {
        "int",   "char",     "long",  "short",   "unsigned", "signed",  "float",    "double",
        "void",  "struct",   "union", "enum",    "register", "static",  "extern",   "auto",
        "const", "volatile", "FILE",  "int16_t", "uint16_t", "int32_t", "uint32_t", "int8_t"};
    if (std::find(kWords.begin(), kWords.end(), word) != kWords.end())
        return true;
    return word == "uint8_t" || word == "intptr_t" || word == "uintptr_t";
}

std::vector<std::string> split_lines(std::string_view text) {
    std::vector<std::string> out;
    for (std::string_view l : text::split(text, '\n'))
        out.emplace_back(l);
    return out;
}

std::string join_lines(const std::vector<std::string>& lines) {
    return text::join(lines, "\n");
}

// ---- patterns that carry a name -----------------------------------------------------------------------

std::string regex_escape(std::string_view name) {
    std::string escaped;
    for (char c : name) {
        const auto u = static_cast<unsigned char>(c);
        if (u < 0x80 && !is_word(c))
            escaped += '\\';
        escaped += c;
    }
    return escaped;
}

Regex name_regex(std::string_view pattern, std::string_view name) {
    return Regex(text::replace_all(pattern, "@@", regex_escape(name)));
}

Regex name_regex(std::string_view pattern, std::string_view first, std::string_view second) {
    return Regex(
        text::replace_all(text::replace_all(pattern, "@1@", regex_escape(first)), "@2@", regex_escape(second)));
}

// ---- structure ----------------------------------------------------------------------------------------

std::vector<int> depths(const std::vector<std::string>& code_lines) {
    std::vector<int> out;
    out.reserve(code_lines.size());
    int depth = 0;
    for (const std::string& l : code_lines) {
        out.push_back(depth);
        for (char c : l)
            depth += (c == '{') - (c == '}');
    }
    return out;
}

LineIndex::LineIndex(std::string_view text) : starts_{0} {
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '\n')
            starts_.push_back(i + 1);
    }
}

std::size_t LineIndex::line_of(std::size_t offset) const {
    return static_cast<std::size_t>(std::upper_bound(starts_.begin(), starts_.end(), offset) - starts_.begin()) - 1;
}

std::string apply_edits(std::string_view text, std::vector<Edit> edits) {
    std::sort(edits.begin(), edits.end(), std::greater<>());
    std::string out(text);
    for (const Edit& e : edits) {
        const std::size_t start = std::min(e.start, out.size());
        const std::size_t end = std::min(std::max(e.end, start), out.size());
        out.replace(start, end - start, e.text);
    }
    return out;
}

namespace {

// The line of the brace that closes the one opened on line `open` (the line count if it never closes).
std::size_t closing_line(const std::vector<std::string>& code, std::size_t open) {
    int depth = 0;
    std::size_t k = open;
    while (k < code.size()) {
        for (char c : code[k])
            depth += (c == '{') - (c == '}');
        if (depth == 0)
            break;
        ++k;
    }
    return k;
}

bool starts_with_brace(const std::string& code_line) {
    return text::ltrim(code_line).starts_with('{');
}

// The old-style definitions of a file: `name(a, b)` at the top, its parameters declared on the lines after, then
// the '{' (the type of the function may be on the line before the name).
std::vector<FunctionSpan> old_style_definitions(const std::vector<std::string>& code) {
    static const Regex head(R"(^(?<ret>[\w\s*]*?)(?<name>[A-Za-z_]\w*)\s*\((?<params>[^()]*)\)\s*$)");
    static const Regex ret_is_struct(R"(\s*(?:static\s+)?(?:struct|union)\s+\w+\s*\**\s*)");
    static const Regex type_line(
        R"(\s*(?:static\s+)?(?:(?:unsigned|signed|long|short|int|char|void|float|double|struct\s+\w+|union\s+\w+)\s*)+\**\s*|\s*(?:static\s+)?[A-Z_][A-Z0-9_]*\s*\*+\s*)");
    std::vector<FunctionSpan> out;
    const std::vector<int> ds = depths(code);
    std::size_t i = 0;
    while (i < code.size()) {
        std::optional<RegexMatch> m;
        std::string trimmed;
        if (ds[i] == 0 && !text::ltrim(code[i]).starts_with('#')) {
            trimmed = std::string(text::rtrim(code[i]));
            m = head.match(trimmed);
        }
        if (!m || is_keyword(m->named("name").value())) {
            ++i;
            continue;
        }
        const std::string_view ret = m->named("ret").value();
        // The words of the return type must be type words (or `*`), unless it is `struct tag *` as a whole.
        bool all_type_words = true;
        for (std::size_t a = 0; a < ret.size() && all_type_words;) {
            if (is_word(ret[a])) {
                std::size_t b = a;
                while (b < ret.size() && is_word(ret[b]))
                    ++b;
                const std::string_view w = ret.substr(a, b - a);
                if (w != "struct" && w != "union" && !is_type_word(w))
                    all_type_words = false;
                a = b;
            } else {
                ++a;
            }
        }
        if (!all_type_words && !ret_is_struct.full_match(ret)) {
            ++i;
            continue;
        }
        // The parameters are bare names (an old-style head), and not just `void`.
        bool bare_names = true;
        std::size_t named = 0;
        std::string only;
        for (std::string_view p : text::split(m->named("params").value(), ',')) {
            p = text::trim(p);
            if (p.empty())
                continue;
            ++named;
            only = std::string(p);
            if (!text::is_identifier(p))
                bare_names = false;
        }
        if (!bare_names || (named == 1 && only == "void")) {
            ++i;
            continue;
        }
        std::size_t first = i;
        if (text::trim(ret).empty() && i > 0 && ds[i - 1] == 0 && type_line.full_match(code[i - 1]) &&
            !text::trim(code[i - 1]).empty()) {
            first = i - 1; // the type on the line before: char *\nf(a)
        }
        std::size_t j = i + 1;
        while (j < code.size() && !starts_with_brace(code[j])) {
            const std::string_view s = text::trim(code[j]);
            if (!s.empty() && !s.ends_with(';'))
                break;
            ++j;
        }
        if (j >= code.size() || !starts_with_brace(code[j])) {
            ++i;
            continue;
        }
        const std::size_t k = closing_line(code, j);
        out.push_back({std::string(m->named("name").value()), first, j, k});
        i = k + 1;
    }
    return out;
}

} // namespace

std::vector<FunctionSpan> definitions_any(const std::vector<std::string>& code) {
    static const Regex head(R"(^[\w\s*]*?\b([A-Za-z_]\w*)\s*\([^;]*\)\s*$)");
    std::vector<FunctionSpan> out;
    const std::vector<int> ds = depths(code);
    std::size_t i = 0;
    while (i < code.size()) {
        std::optional<RegexMatch> m;
        if (ds[i] == 0)
            m = head.match(code[i]);
        if (m && !is_keyword(m->group(1)) && i + 1 < code.size() && starts_with_brace(code[i + 1])) {
            const std::size_t k = closing_line(code, i + 1);
            out.push_back({std::string(m->group(1)), i, i + 1, k});
            i = k + 1;
        } else {
            ++i;
        }
    }
    return out;
}

SourceIndex::SourceIndex(std::string text) : text_(std::move(text)) {
    code_ = base::ctext::blank(text_);
    code_lines_ = split_lines(code_);
    functions_ = definitions_any(code_lines_);
    for (FunctionSpan& f : old_style_definitions(code_lines_))
        functions_.push_back(std::move(f));
}

const FunctionSpan* SourceIndex::enclosing(std::size_t line) const {
    if (line == 0)
        return nullptr;
    for (const FunctionSpan& f : functions_) { // (new style first; an old-style one, kept so)
        if (f.first <= line - 1 && line - 1 <= f.last)
            return &f;
    }
    return nullptr;
}

bool SourceIndex::declares_local(const FunctionSpan& fn, std::string_view var) const {
    std::string body;
    for (std::size_t i = fn.first; i <= fn.last && i < code_lines_.size(); ++i) {
        body += code_lines_[i];
        body += '\n';
    }
    return name_regex(R"(\b(?:u?int16_t|int|unsigned|intptr_t)\s+(?:[^;(){}]*,\s*)?@@\b\s*[;,)=])", var)
        .search(body)
        .has_value();
}

// ---- calls --------------------------------------------------------------------------------------------

std::optional<std::vector<Span>> call_args(std::string_view line, std::string_view fn, bool partial) {
    for (std::size_t at = find_word(line, fn, 0); at != std::string_view::npos; at = find_word(line, fn, at + 1)) {
        const std::size_t paren = skip_space(line, at + fn.size());
        if (paren < line.size() && line[paren] == '(')
            return read_args(line, paren, partial);
    }
    return std::nullopt;
}

std::optional<CallAt> call_at(std::string_view line, std::size_t col) {
    std::optional<CallAt> best;
    std::size_t i = 0;
    while (i < line.size()) {
        if (!is_word(line[i])) {
            ++i;
            continue;
        }
        std::size_t j = i;
        while (j < line.size() && is_word(line[j]))
            ++j;
        const std::string_view word = line.substr(i, j - i);
        const std::size_t paren = skip_space(line, j);
        if (is_ident_start(word.front()) && paren < line.size() && line[paren] == '(') {
            if (!is_keyword(word)) {
                if (auto spans = read_args(line, paren, true)) {
                    for (std::size_t k = 0; k < spans->size(); ++k) {
                        const Span s = (*spans)[k];
                        if (s.start <= col && col <= s.end &&
                            (!best || s.end - s.start < best->span.end - best->span.start)) {
                            best = CallAt{std::string(word), k, s};
                        }
                    }
                }
            }
            i = paren + 1; // (the next call is looked for after this one's '(')
        } else {
            i = j;
        }
    }
    return best;
}

std::size_t object_before(std::string_view line, std::size_t at) {
    std::size_t i = at;
    for (;;) {
        while (i > 0 && is_blank(line[i - 1]))
            --i;
        if (i > 0 && (line[i - 1] == ')' || line[i - 1] == ']')) {
            const char close = line[i - 1];
            const char open = close == ')' ? '(' : '[';
            int depth = 0;
            std::size_t k = i - 1;
            bool balanced = false;
            for (;;) {
                depth += (line[k] == close) - (line[k] == open);
                if (depth == 0) {
                    balanced = true;
                    break;
                }
                if (k == 0)
                    break;
                --k;
            }
            // DIVERGES: an unbalanced bracket gave the Python an index of -1 and then indexed from the end of the
            // line; here the object is taken to start at the line's start.
            if (!balanced)
                return 0;
            i = k;
            if (open == '(' && !(i > 0 && is_word(line[i - 1])))
                return i; // a parenthesized object: the whole
            continue;
        }
        // the name that ends here (its first letter: a word of digits is not a name)
        std::size_t start = i;
        while (start > 0 && is_word(line[start - 1]))
            --start;
        while (start < i && !is_ident_start(line[start]))
            ++start;
        if (start == i)
            return i;
        i = start;
        if (line.substr(0, i).ends_with("->")) {
            i -= 2;
            continue;
        }
        if (line.substr(0, i).ends_with(".")) {
            i -= 1;
            continue;
        }
        return i;
    }
}

// ---- declarations -------------------------------------------------------------------------------------

std::vector<std::string> split_top(std::string_view s, char sep) {
    std::vector<std::string> parts;
    std::string cur;
    int depth = 0;
    for (char c : s) {
        if (c == '(' || c == '[') {
            ++depth;
        } else if (c == ')' || c == ']') {
            --depth;
        }
        if (c == sep && depth == 0) {
            parts.push_back(std::move(cur));
            cur.clear();
        } else {
            cur += c;
        }
    }
    parts.push_back(std::move(cur));
    return parts;
}

namespace {

struct Token {
    std::size_t start, end;
};

// Every [A-Za-z_]\w* in the text, with no word boundary asked of its start (the "x1f" of "0x1f" is one).
std::vector<Token> identifier_tokens(std::string_view s) {
    std::vector<Token> out;
    for (std::size_t i = 0; i < s.size();) {
        if (is_ident_start(s[i])) {
            std::size_t j = i + 1;
            while (j < s.size() && is_word(s[j]))
                ++j;
            out.push_back({i, j});
            i = j;
        } else {
            ++i;
        }
    }
    return out;
}

} // namespace

std::optional<Declaration> declarators(std::string_view decl) {
    std::string_view d = text::trim(decl);
    while (d.ends_with(';'))
        d.remove_suffix(1);
    const std::vector<std::string> parts = split_top(d);
    const std::string& first = parts.front();
    const std::vector<Token> toks = identifier_tokens(first);
    std::optional<Token> name;
    for (std::size_t i = 0; i < toks.size(); ++i) {
        const std::string_view w = std::string_view(first).substr(toks[i].start, toks[i].end - toks[i].start);
        bool after_tag = false;
        if (i > 0) {
            const std::string_view prev =
                std::string_view(first).substr(toks[i - 1].start, toks[i - 1].end - toks[i - 1].start);
            after_tag = prev == "struct" || prev == "union" || prev == "enum";
        }
        if (is_type_word(w) || after_tag)
            continue;
        name = toks[i];
        break;
    }
    if (!name)
        return std::nullopt;
    std::size_t start = name->start;
    while (start > 0 &&
           (first[start - 1] == ' ' || first[start - 1] == '\t' || first[start - 1] == '*' || first[start - 1] == '('))
        --start;
    while (start < name->start && is_blank(first[start]))
        ++start;
    Declaration out;
    out.base = std::string(text::trim(std::string_view(first).substr(0, start)));
    out.declarators.push_back({first.substr(name->start, name->end - name->start),
                               std::string(text::trim(std::string_view(first).substr(start)))});
    for (std::size_t p = 1; p < parts.size(); ++p) {
        const std::vector<Token> t = identifier_tokens(parts[p]);
        if (t.empty())
            return std::nullopt;
        out.declarators.push_back(
            {parts[p].substr(t.front().start, t.front().end - t.front().start), std::string(text::trim(parts[p]))});
    }
    return out;
}

std::string abstract_type(std::string_view decl, std::string_view name) {
    static const Regex register_word(R"(\bregister\s+)");
    static const Regex array_tail(R"(\s*\[\s*\d*\s*\]$)");
    static const Regex space_star(R"(\s+\*)");
    const std::string no_register = register_word.replace_all(decl, "");
    // (struct bcd *bcd: the name, not the tag)
    const std::string no_name =
        name_regex(R"((?<!struct )(?<!union )(?<!enum )\b@@\b)", name).replace_first(no_register, "");
    const std::string t = array_tail.replace_all(text::trim(no_name), " *");
    return space_star.replace_all(t, " *");
}

// ---- how a name is used -------------------------------------------------------------------------------

bool used_as_number(std::string_view code, std::string_view var) {
    return name_regex(R"(\b@@\s*(?:[*/%^|]=|<<=|>>=|[/%]|<<|>>|\*(?!\s*\))|&(?!&)|\|(?!\|)))", var).test(code) ||
           name_regex(R"((?:[\w)\]]\s*(?:[*/%]|<<|>>)|[*/%]=|<<=|>>=)\s*@@\b)", var).test(code) ||
           // (compared with a number; (off + n) & ~n)
           name_regex(R"(\(?\s*\b@@\s*(?:[+\-]=?[^;)]*)?\)?\s*(?:[<>]=?)\s*-?\s*(?:[1-9]|0\b))", var).test(code) ||
           name_regex(R"(\b@@\s*[+\-]\s*\w+\s*\)\s*[&|])", var).test(code) ||
           // (an index)
           name_regex(R"(\[\s*(?:\+\+|--)?\s*@@\s*(?:\+\+|--)?\s*\])", var).test(code);
}

namespace {

// Whether the text before a use of a name puts it where a test is: after `if (`, `while (`, `!`, `&&`, `||`, or at
// the start of a parenthesis that is not a call's.
bool test_comes_before(std::string_view before) {
    const std::string_view s = text::rtrim(before);
    if (s.ends_with('!') || s.ends_with("&&") || s.ends_with("||"))
        return true;
    if (!s.ends_with('('))
        return false;
    const std::string_view r = text::rtrim(s.substr(0, s.size() - 1));
    if (r.empty())
        return true;
    const char c = r.back();
    if (!is_word(c) && !text::is_space(c) && c != ')' && c != ']')
        return true; // (a call's argument is no test)
    for (std::string_view kw : {"if", "while", "for", "return"}) {
        if (r.ends_with(kw) && (r.size() == kw.size() || !is_word(r[r.size() - kw.size() - 1])))
            return true;
    }
    return false;
}

// Whether what follows a use closes a test: `)`, `&&` or `||`.
bool test_goes_after(std::string_view after) {
    const std::string_view s = text::ltrim(after);
    return s.starts_with(')') || s.starts_with("&&") || s.starts_with("||");
}

} // namespace

bool only_tested(std::string_view body, std::string_view name) {
    bool any = false;
    for (std::size_t at = find_word(body, name, 0); at != std::string_view::npos; at = find_word(body, name, at + 1)) {
        any = true;
        if (!test_comes_before(body.substr(0, at)) || !test_goes_after(body.substr(at + name.size())))
            return false;
    }
    return any;
}

StructMembers members(const std::vector<std::string_view>& texts) {
    static const Regex struct_head(R"(\b(struct|union)\s+(\w+)\s*\{)");
    StructMembers out;
    for (std::string_view t : texts) {
        const std::string code = base::ctext::blank(t);
        struct_head.for_each_match(code, [&](const RegexMatch& m) {
            int depth = 0;
            std::size_t k = m.end() - 1;
            while (k < code.size()) {
                depth += (code[k] == '{') - (code[k] == '}');
                if (depth == 0)
                    break;
                ++k;
            }
            const std::string owner = std::string(m.group(1)) + " " + std::string(m.group(2));
            for (std::string_view piece : text::split(std::string_view(code).substr(m.end(), k - m.end()), ';')) {
                if (text::trim(piece).empty())
                    continue;
                if (auto d = declarators(piece)) {
                    for (const NamedDeclarator& nd : d->declarators)
                        out[nd.name].push_back(owner);
                }
            }
            return true;
        });
    }
    return out;
}

std::size_t find_tag_body(std::string_view code, std::string_view keyword, std::string_view tag) {
    for (std::size_t at = find_word(code, keyword, 0); at != std::string_view::npos;
         at = find_word(code, keyword, at + 1)) {
        const std::size_t name_at = skip_space(code, at + keyword.size());
        if (name_at == at + keyword.size() || code.substr(name_at, tag.size()) != tag)
            continue;
        const std::size_t brace = skip_space(code, name_at + tag.size());
        if (brace < code.size() && code[brace] == '{')
            return brace + 1;
    }
    return std::string_view::npos;
}

// ---- types ----------------------------------------------------------------------------------------------

std::string ctype(std::string_view type, const TagSet& tags) {
    static const Regex aka(R"(\s*\(aka '[^']*'\))");
    static const Regex bare_word(R"((?<!struct )(?<!union )\b(\w+)\b)");
    std::string t = text::replace_all(text::replace_all(type, "__cdecl ", ""), "__cdecl", "");
    t = aka.replace_all(t, "");
    t = bare_word.replace_all(t, [&](const RegexMatch& m) {
        const std::string_view w = m.group(1);
        return tags.contains(w) ? "struct " + std::string(w) : std::string(w);
    });
    return std::string(text::trim(collapse_space(t)));
}

} // namespace cb::port::csource
