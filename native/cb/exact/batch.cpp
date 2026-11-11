#include "cb/exact/batch.hpp"

#include <algorithm>
#include <cctype>
#include <unordered_set>

#include "cb/core/text.hpp"

namespace cb::exact::batch {
namespace {

// The words C reserves; the extern scan must not take them for the names a declaration gives.
const std::unordered_set<std::string_view>& keywords() {
    static const std::unordered_set<std::string_view> k = {
        "extern", "int", "char", "long", "short", "unsigned", "signed", "struct", "union", "double", "float",
        "register", "static", "void", "auto", "typedef", "enum", "sizeof", "return", "if", "else", "while", "for",
        "do", "switch", "case", "default", "break", "continue", "goto"};
    return k;
}

constexpr std::string_view kSuffixMark = "__Q";

bool is_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }
bool is_ident_start(char c) { return std::isalpha(static_cast<unsigned char>(c)) != 0 || c == '_'; }

// `kw` at the start of s as a whole word (what `kw\b` matches there).
bool starts_with_word(std::string_view s, std::string_view kw) {
    return s.starts_with(kw) && (s.size() == kw.size() || !is_word(s[kw.size()]));
}

// The end of the run of word characters that begins at `from`.
std::size_t word_end(std::string_view s, std::size_t from) {
    while (from < s.size() && is_word(s[from])) ++from;
    return from;
}

// Is the whole line a file comment of the form  /* WORD: text without a star */ ?
bool is_file_comment(std::string_view s) {
    if (!s.starts_with("/* ") || !s.ends_with("*/") || s.size() < 5) return false;
    const std::size_t word_from = 3, word_to = word_end(s, word_from);
    if (word_to == word_from || s.substr(word_to, 2) != ": ") return false;
    const std::string_view middle = s.substr(word_to + 2);   // text then "*/"
    return middle.size() >= 2 && middle.find('*') == middle.size() - 2;
}

// Where a quoted literal ('"' or '\'') that begins at `open` ends, if it closes: `\` takes the next character, and
// (when `newline_ends`) a raw newline ends the attempt; a literal that never closes is not one.
std::optional<std::size_t> literal_end(std::string_view s, std::size_t open, bool newline_ends) {
    const char quote = s[open];
    for (std::size_t j = open + 1; j < s.size();) {
        const char c = s[j];
        if (c == quote) return j + 1;
        if (c == '\\') {
            if (j + 1 >= s.size() || (newline_ends && s[j + 1] == '\n')) return std::nullopt;
            j += 2;
        } else if (newline_ends && c == '\n') {
            return std::nullopt;
        } else {
            ++j;
        }
    }
    return std::nullopt;
}

// The number of `c` in the C text outside its comments, strings and character constants. A string or constant runs
// on across lines to its partner; one with no partner (an apostrophe) is just a character.
std::size_t count_in_code(std::string_view text, std::string_view chars, std::size_t* second = nullptr) {
    std::size_t first_count = 0, second_count = 0;
    for (std::size_t i = 0; i < text.size();) {
        const char c = text[i];
        if (c == '/' && i + 1 < text.size() && text[i + 1] == '*') {
            if (const std::size_t close = text.find("*/", i + 2); close != std::string_view::npos) {
                i = close + 2;
                continue;
            }
        } else if (c == '"' || c == '\'') {
            if (const auto end = literal_end(text, i, false)) {
                i = *end;
                continue;
            }
        }
        if (c == chars[0]) ++first_count;
        if (chars.size() > 1 && c == chars[1]) ++second_count;
        ++i;
    }
    if (second != nullptr) *second = second_count;
    return first_count;
}

// `name(` with the name starting at the beginning of s: the name.
std::optional<std::string_view> call_at(std::string_view s) {
    if (s.empty() || !is_ident_start(s[0])) return std::nullopt;
    const std::size_t end = word_end(s, 0);
    if (end < s.size() && s[end] == '(') return s.substr(0, end);
    return std::nullopt;
}

// The function a candidate defines: its name at the start of a line, with the type perhaps on the line before. A
// line that begins extern, struct or static is not where the match starts, but its *next* line may be.
std::optional<std::string_view> defined_function(std::string_view body) {
    for (std::size_t p = 0; p <= body.size();) {
        const std::string_view rest = body.substr(p);
        const bool declaration = starts_with_word(rest, "extern") || starts_with_word(rest, "struct") || starts_with_word(rest, "static");
        if (!declaration) {
            const std::size_t eol = rest.find('\n');
            const bool type_line = eol != std::string_view::npos && eol > 0 &&
                                   std::ranges::all_of(rest.substr(0, eol), [](char c) { return is_word(c) || c == ' ' || c == '*'; });
            if (type_line) {
                if (const auto name = call_at(rest.substr(eol + 1))) return name;
            } else if (const auto name = call_at(rest)) {
                return name;
            }
        }
        const std::size_t nl = body.find('\n', p);
        if (nl == std::string_view::npos) break;
        p = nl + 1;
    }
    return std::nullopt;
}

// `struct NAME` taken out of an extern declaration's text (each becomes a space).
std::string without_struct_names(std::string_view l) {
    std::string out;
    for (std::size_t i = 0; i < l.size();) {
        if ((i == 0 || !is_word(l[i - 1])) && l.substr(i).starts_with("struct")) {
            std::size_t j = i + 6, ws = j;
            while (j < l.size() && text::is_space(l[j])) ++j;
            if (j > ws && j < l.size() && is_word(l[j])) {
                out += ' ';
                i = word_end(l, j);
                continue;
            }
        }
        out += l[i++];
    }
    return out;
}

// The names an extern declaration's text (up to its ';') gives: each identifier followed, past any space, by one of
// ( [ , ) ; or the end of the text.
void names_declared(std::string_view l, std::set<std::string>& names) {
    for (std::size_t i = 0; i < l.size();) {
        if (!is_ident_start(l[i])) {
            ++i;
            continue;
        }
        const std::size_t end = word_end(l, i);
        std::size_t after = end;
        while (after < l.size() && text::is_space(l[after])) ++after;
        const bool at_end = after == l.size() || (after + 1 == l.size() && l[after] == '\n');
        const bool ends_name = at_end || std::string_view("([,);").find(l[after]) != std::string_view::npos;
        if (ends_name) {
            if (const std::string_view n = l.substr(i, end - i); !keywords().contains(n)) names.emplace(n);
            i = after;
        } else {
            ++i;   // (a start inside the word fails the same way)
        }
    }
}

// Apply `rename` to every identifier-like run of word characters in `piece`.
template <class F>
void map_words(std::string_view piece, std::string& out, F&& fn) {
    for (std::size_t i = 0; i < piece.size();) {
        if (!is_word(piece[i])) {
            out += piece[i++];
            continue;
        }
        const std::size_t end = word_end(piece, i);
        fn(piece.substr(i, end - i), out);
        i = end;
    }
}

}  // namespace

std::optional<SplitSource> split(std::string_view source) {
    std::vector<std::string_view> pre, body;
    for (std::string_view line : text::split(source, '\n')) {
        const std::string_view s = text::trim(line);
        if (body.empty() && (s.starts_with("#include") || is_file_comment(s) || s.empty())) {
            pre.push_back(line);
            continue;
        }
        if (s.starts_with('#')) return std::nullopt;
        body.push_back(line);
    }
    const std::string joined = text::join(body, "\n");
    std::size_t closing_braces = 0, closing_parens = 0;
    const std::size_t opening_braces = count_in_code(joined, "{}", &closing_braces);
    const std::size_t opening_parens = count_in_code(joined, "()", &closing_parens);
    if (opening_braces != closing_braces || opening_parens != closing_parens) return std::nullopt;
    std::vector<std::string_view> kept;
    for (std::string_view l : pre)
        if (!text::trim(l).empty()) kept.push_back(l);
    return SplitSource{text::join(kept, "\n"), joined};
}

std::set<std::string> own_names(std::string_view body) {
    std::set<std::string> names;
    if (const auto f = defined_function(body)) names.emplace(*f);

    // What it declares extern: `extern` at a line's start, up to the first semicolon (a declaration may run over
    // several lines; the scan resumes at the first line start after the semicolon).
    for (std::size_t p = 0; p < body.size();) {
        const std::string_view rest = body.substr(p);
        std::size_t scan_from = p;
        if (starts_with_word(rest, "extern")) {
            if (const std::size_t semi = rest.find(';'); semi != std::string_view::npos) {
                names_declared(without_struct_names(rest.substr(6, semi - 6)), names);
                scan_from = p + semi;
            }
        }
        const std::size_t nl = body.find('\n', scan_from);
        p = nl == std::string_view::npos ? body.size() : nl + 1;
    }

    // The structs it defines: struct NAME {
    for (std::size_t i = 0; i < body.size(); ++i) {
        if ((i != 0 && is_word(body[i - 1])) || !body.substr(i).starts_with("struct")) continue;
        std::size_t j = i + 6;
        const std::size_t ws = j;
        while (j < body.size() && text::is_space(body[j])) ++j;
        if (j == ws || j >= body.size() || !is_word(body[j])) continue;
        const std::size_t end = word_end(body, j);
        std::size_t after = end;
        while (after < body.size() && text::is_space(body[after])) ++after;
        if (after < body.size() && body[after] == '{') names.emplace(body.substr(j, end - j));
    }
    return names;
}

std::string renamed(std::string_view body, std::size_t k) {
    const std::set<std::string> names = own_names(body);
    if (names.empty()) return std::string(body);
    const std::string suffix = std::string(kSuffixMark) + std::to_string(k);
    const auto rename_words = [&](std::string_view piece, std::string& out) {
        map_words(piece, out, [&](std::string_view word, std::string& o) {
            o += word;
            if (names.contains(std::string(word))) o += suffix;
        });
    };
    std::string out;
    std::size_t piece_from = 0;
    const auto flush_piece = [&](std::size_t to) {   // (a piece that starts with a quote - an unclosed string - is left whole)
        const std::string_view piece = body.substr(piece_from, to - piece_from);
        if (piece.starts_with('"')) out += piece;
        else rename_words(piece, out);
    };
    for (std::size_t i = 0; i < body.size();) {
        if (body[i] == '"') {
            if (const auto end = literal_end(body, i, true)) {   // (not inside a string)
                flush_piece(i);
                out += body.substr(i, *end - i);
                piece_from = i = *end;
                continue;
            }
        }
        ++i;
    }
    flush_piece(body.size());
    return out;
}

std::string strip_suffix(std::string_view name) {
    const std::size_t at = name.rfind(kSuffixMark);
    if (at != std::string_view::npos) {
        const std::string_view digits = name.substr(at + kSuffixMark.size());
        if (!digits.empty() && std::ranges::all_of(digits, [](char c) { return c >= '0' && c <= '9'; })) return std::string(name.substr(0, at));
    }
    return std::string(name);
}

std::optional<BatchSource> build(const std::vector<std::string>& sources) {
    std::vector<std::optional<SplitSource>> parts;
    for (const std::string& s : sources) parts.push_back(split(s));
    std::vector<std::size_t> ok;
    for (std::size_t i = 0; i < parts.size(); ++i)
        if (parts[i]) ok.push_back(i);
    if (ok.size() < 2) return std::nullopt;
    const std::string pre = parts[ok.front()]->preamble;
    std::erase_if(ok, [&](std::size_t i) { return parts[i]->preamble != pre; });   // (a different file comment: not batched)
    if (ok.size() < 2) return std::nullopt;

    std::vector<std::string> lines;
    if (!pre.empty())
        for (std::string_view l : text::split(pre, '\n')) lines.emplace_back(l);
    BatchSource out;
    for (std::size_t k = 0; k < ok.size(); ++k) {
        const std::string body = renamed(parts[ok[k]]->body, k);
        const std::size_t first = lines.size() + 1;
        for (std::string_view l : text::split(body, '\n')) lines.emplace_back(l);
        lines.emplace_back();
        out.spans.push_back({first, lines.size()});
    }
    out.text = text::join(lines, "\n") + "\n";
    out.candidates = std::move(ok);
    return out;
}

std::vector<std::size_t> error_lines(std::string_view console) {
    std::set<std::size_t> lines;
    for (std::size_t i = 0; i + 3 < console.size(); ++i) {
        if (console[i] != '.' || !text::istarts_with(console.substr(i + 1), "c(")) continue;
        std::size_t j = i + 3, digits = j;
        while (j < console.size() && console[j] >= '0' && console[j] <= '9') ++j;
        if (j == digits || j >= console.size() || console[j] != ')') continue;
        const std::optional<std::size_t> line = text::parse_int<std::size_t>(console.substr(digits, j - digits));
        if (!line) continue;
        auto skip_space = [&](std::size_t from) {
            while (from < console.size() && text::is_space(console[from])) ++from;
            return from;
        };
        j = skip_space(j + 1);
        if (j >= console.size() || console[j] != ':') continue;
        j = skip_space(j + 1);
        if (text::istarts_with(console.substr(j), "warning")) continue;   // (3.00 names only warnings this way)
        lines.insert(*line);
    }
    return {lines.begin(), lines.end()};
}

std::optional<Compiled> compile_many(Toolchain& toolchain, const CompileOptions& options, const SymbolRenamer& rename,
                                     const std::vector<std::string>& sources, int rounds) {
    std::vector<std::size_t> want(sources.size());
    for (std::size_t i = 0; i < want.size(); ++i) want[i] = i;
    for (int round = 0; round < rounds; ++round) {
        std::vector<std::string> chosen;
        for (std::size_t i : want) chosen.push_back(sources[i]);
        const std::optional<BatchSource> b = build(chosen);
        if (!b) return std::nullopt;
        std::vector<std::size_t> idx;
        for (std::size_t i : b->candidates) idx.push_back(want[i]);
        const Bytes source = text::to_latin1(b->text);
        CompileOutcome outcome = toolchain.compile(source, options);
        if (outcome.object) {
            Compiled done;
            done.object = rename(*outcome.object, [](std::string_view n) { return strip_suffix(n); });
            for (std::size_t k = 0; k < idx.size(); ++k) done.placed.push_back({idx[k], k});
            return done;
        }
        const std::vector<std::size_t> bad = error_lines(outcome.console);
        if (bad.empty()) return std::nullopt;
        std::vector<std::size_t> kept;
        for (std::size_t k = 0; k < idx.size(); ++k) {
            const LineSpan span = b->spans[k];
            const bool blamed = std::ranges::any_of(bad, [&](std::size_t l) { return span.first <= l && l <= span.last; });
            if (!blamed) kept.push_back(idx[k]);
        }
        if (kept.size() == idx.size() || kept.size() < 2) return std::nullopt;
        want = std::move(kept);
    }
    return std::nullopt;
}

}  // namespace cb::exact::batch
