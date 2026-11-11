#include "cb/xna/fixups.hpp"

#include <algorithm>
#include <cctype>
#include <iterator>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::xna::fixups {
namespace {

// These patterns run over whole decompiled files (tens of thousands of lines): they cannot loop for long, so the step
// budget is lifted far above what a large file needs.
constexpr std::uint64_t kSteps = std::uint64_t{1} << 40;

// An identifier here is the compiler's: ASCII letters, digits, `_`, and any byte of a UTF-8 sequence (an
// obfuscator renames members to whatever letters it likes).
#define CB_ID_START "[A-Za-z_\\x80-\\xff]"
#define CB_ID_CHAR "[A-Za-z0-9_\\x80-\\xff]"
#define CB_ID_DOTTED "[A-Za-z0-9_.\\x80-\\xff]"

// An lvalue: a name with an optional index, then `.name[index]` ... (an index holds no brackets of its own).
#define CB_LVALUE                                                                                           \
    "(?:" CB_ID_START CB_ID_CHAR "*(?:\\s*\\[[^\\[\\]]*\\])?)"                                              \
    "(?:\\s*\\.\\s*" CB_ID_START CB_ID_CHAR "*(?:\\s*\\[[^\\[\\]]*\\])?)*"

// `(Type)(ref lvalue)`: the lvalue is group 1.
const Regex& ref_cast_pattern() {
    static const Regex re("\\(\\s*" CB_ID_START CB_ID_DOTTED "*\\s*\\)\\s*\\(\\s*ref\\s+(" CB_LVALUE ")\\s*\\)",
                          RegexFlags::None, kSteps);
    return re;
}

// A call of the constructor ILSpy names `_002Ector`, three forms, each ending at the call's open parenthesis:
//   base._002Ector(  /  this._002Ector(            group `base`
//   ((Base)this)._002Ector(                       (neither group)
//   v._002Ector(  /  (v)._002Ector(                group `var`
const Regex& constructor_call_pattern() {
    static const Regex re("(?<base>base|this)\\._002Ector\\s*\\("
                          "|\\(\\(" CB_ID_START CB_ID_DOTTED "*\\)this\\)\\._002Ector\\s*\\("
                          "|\\(?(?<var>" CB_ID_START CB_ID_CHAR "*)\\)?\\._002Ector\\s*\\(",
                          RegexFlags::None, kSteps);
    return re;
}

const Regex& private_struct_pattern() {
    static const Regex re("\\bprivate(\\s+struct\\s+" CB_ID_CHAR "+)", RegexFlags::None, kSteps);
    return re;
}

// The text inside the parentheses that open at `open_paren`, and the position after the closing one; nullopt when
// they never close.
struct Matched {
    std::string_view inside;
    std::size_t after = 0;
};
std::optional<Matched> matching(std::string_view text, std::size_t open_paren) {
    int depth = 0;
    for (std::size_t i = open_paren; i < text.size(); ++i) {
        if (text[i] == '(') {
            ++depth;
        } else if (text[i] == ')') {
            if (--depth == 0) return Matched{text.substr(open_paren + 1, i - open_paren - 1), i + 1};
        }
    }
    return std::nullopt;
}

// The position of the `{` that encloses `pos` (the nearest one before it that its own `}` has not closed).
std::optional<std::size_t> enclosing_brace(std::string_view text, std::size_t pos) {
    int depth = 0;
    for (std::size_t i = pos; i-- > 0;) {
        if (text[i] == '}') {
            ++depth;
        } else if (text[i] == '{') {
            if (depth == 0) return i;
            --depth;
        }
    }
    return std::nullopt;
}

// Words that open a parenthesised statement: `if (a) {` has a brace after a `)` and is not a constructor.
bool is_statement_keyword(std::string_view word) {
    static constexpr std::string_view kWords[] = {"if", "while", "for", "foreach", "using", "lock", "switch", "catch", "fixed"};
    return std::find(std::begin(kWords), std::end(kWords), word) != std::end(kWords);
}

// The word before the parenthesis that the `)` at `close` closes (`if` for `if (a)`), or empty.
std::string_view word_before_group(std::string_view text, std::size_t close) {
    int depth = 0;
    for (std::size_t i = close + 1; i-- > 0;) {
        if (text[i] == ')') {
            ++depth;
        } else if (text[i] == '(' && --depth == 0) {
            std::size_t end = i;
            while (end > 0 && text::is_space(text[end - 1])) --end;
            std::size_t start = end;
            while (start > 0 && (std::isalnum(static_cast<unsigned char>(text[start - 1])) || text[start - 1] == '_')) --start;
            while (start < end && text[start] >= '0' && text[start] <= '9') ++start;
            return text.substr(start, end - start);
        }
    }
    return {};
}

bool is_blank(char c) { return c == ' ' || c == '\t'; }
bool is_id_start(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }
bool is_type_char(char c) {
    return is_id_start(c) || (c >= '0' && c <= '9') || c == '<' || c == '>' || c == ',' || c == '.' || c == '[' ||
           c == ']' || c == ' ';
}

// The type a line declares `var` with, if it is a declaration of the decompiler's one-per-line shape:
// `<blanks><type> <blanks>var<blanks>` then `=` (not `==`) or `;`. The type is the shortest one that fits.
std::optional<std::string_view> declared_on_line(std::string_view line, std::string_view var) {
    std::size_t i = 0;
    while (i < line.size() && is_blank(line[i])) ++i;
    if (i >= line.size() || !is_id_start(line[i])) return std::nullopt;
    for (std::size_t end = i + 1; end <= line.size(); ++end) {   // the type is line[i, end)
        if (end < line.size() && is_blank(line[end])) {
            std::size_t v = end;
            while (v < line.size() && is_blank(line[v])) ++v;
            if (line.substr(v, var.size()) == var) {
                std::size_t a = v + var.size();
                while (a < line.size() && is_blank(line[a])) ++a;
                if (a < line.size() && ((line[a] == '=' && !(a + 1 < line.size() && line[a + 1] == '=')) || line[a] == ';'))
                    return text::trim(line.substr(i, end - i));
            }
        }
        if (end >= line.size() || !is_type_char(line[end])) break;   // the type cannot grow past this byte
    }
    return std::nullopt;
}

// The type of the nearest declaration of `var` before `before`.
std::optional<std::string> declared_type(std::string_view text, std::string_view var, std::size_t before) {
    text = text.substr(0, before);
    std::size_t line_end = text.size();
    for (;;) {
        const std::size_t nl = line_end == 0 ? std::string_view::npos : text.rfind('\n', line_end - 1);
        const std::size_t line_start = nl == std::string_view::npos ? 0 : nl + 1;
        if (const auto type = declared_on_line(text.substr(line_start, line_end - line_start), var))
            return std::string(*type);
        if (line_start == 0) return std::nullopt;
        line_end = line_start - 1;
    }
}

// An edit of the text: [start, end) becomes `with`.
struct Edit {
    std::size_t start = 0;
    std::size_t end = 0;
    std::string with;
};

}  // namespace

Rewritten ref_casts(std::string_view text) {
    Rewritten out{std::string(text), 0};
    if (text.find("ref") == std::string_view::npos) return out;
    for (;;) {   // nested: ((IDisposable)(Enumerator)(ref e))
        std::string next;
        std::size_t last = 0, n = 0;
        ref_cast_pattern().for_each_match(out.text, [&](const RegexMatch& m) {
            next.append(out.text, last, m.start() - last);
            next.append(m.group(1));
            last = m.end();
            ++n;
            return true;
        });
        if (n == 0) return out;
        next.append(out.text, last, std::string::npos);
        out.text = std::move(next);
        out.count += n;
    }
}

ConstructorsResult constructors(std::string_view text) {
    ConstructorsResult result{std::string(text), 0, {}};
    if (text.find("_002Ector") == std::string_view::npos) return result;

    std::vector<Edit> edits;
    std::size_t counted_to = 0, line = 1;   // line number of `counted_to`, so each match costs only the text since the last
    constructor_call_pattern().for_each_match(text, [&](const RegexMatch& m) {
        const std::size_t paren = m.end() - 1;   // every form ends at the call's open parenthesis
        const std::optional<Matched> args = matching(text, paren);
        if (!args) return true;
        line += static_cast<std::size_t>(std::count(text.begin() + static_cast<std::ptrdiff_t>(counted_to),
                                                    text.begin() + static_cast<std::ptrdiff_t>(m.start()), '\n'));
        counted_to = m.start();
        const auto base = m.named("base");
        const auto var = m.named("var");
        if ((base && !base->empty()) || !var) {
            // a base or this call: it is the constructor's initializer
            const std::string_view kind = (base && !base->empty()) ? *base : std::string_view("base");
            const std::optional<std::size_t> brace = enclosing_brace(text, m.start());
            const std::size_t close = brace && *brace > 0 ? text.rfind(')', *brace - 1) : std::string_view::npos;
            if (close == std::string_view::npos || close == 0) {
                result.left.push_back({line, "no constructor parameter list before it"});
                return true;
            }
            const std::string_view between = text.substr(close + 1, *brace - close - 1);
            const std::string_view before_group = word_before_group(text, close);
            if (!text::trim(between).empty() || before_group == "base" || before_group == "this") {   // `: base()` is there
                result.left.push_back({line, "the constructor already has an initializer"});
                return true;
            }
            if (is_statement_keyword(before_group)) {
                result.left.push_back({line, "it is inside a statement block, not at the top of a constructor"});
                return true;
            }
            edits.push_back({close + 1, close + 1, std::string(" : ").append(kind).append("(").append(args->inside).append(")")});
            std::size_t after = args->after;   // the call's statement ends at its `;`
            std::size_t semi = after;
            while (semi < text.size() && is_blank(text[semi])) ++semi;
            if (semi < text.size() && text[semi] == ';') after = semi + 1;
            const std::size_t nl_before = m.start() == 0 ? std::string_view::npos : text.rfind('\n', m.start() - 1);
            const std::size_t start = nl_before == std::string_view::npos ? 0 : nl_before + 1;
            const std::size_t nl_after = text.find('\n', after);   // the call may span lines: through the line its `)` is on
            const std::size_t end = nl_after == std::string_view::npos ? text.size() : nl_after;
            if (text::trim(text.substr(start, m.start() - start)).empty() && text::trim(text.substr(after, end - after)).empty())
                edits.push_back({start, std::min(end + 1, text.size()), ""});   // alone on its lines: they go
            else
                edits.push_back({m.start(), after, ""});   // it shares a line: only the call goes
        } else {
            const std::optional<std::string> type = declared_type(text, *var, m.start());
            if (!type) {
                result.left.push_back({line, std::string("no declaration of ").append(*var).append(" before it")});
                return true;
            }
            edits.push_back({m.start(), args->after,
                             std::string(*var).append(" = new ").append(*type).append("(").append(args->inside).append(")")});
        }
        return true;
    });

    // From the end backwards, so the positions stay right; an edit that overlaps one already made is skipped.
    std::stable_sort(edits.begin(), edits.end(), [](const Edit& a, const Edit& b) { return a.start > b.start; });
    std::size_t last = text.size() + 1;
    for (const Edit& e : edits) {
        if (e.end > last) continue;
        result.text.replace(e.start, e.end - e.start, e.with);
        last = e.start;
        ++result.count;
    }
    return result;
}

Rewritten data_holders(std::string_view text) {
    Rewritten out{std::string(text), 0};
    if (text.find("/* Not supported: data(") == std::string_view::npos) return out;
    std::string next;
    std::size_t last = 0;
    private_struct_pattern().for_each_match(text, [&](const RegexMatch& m) {
        next.append(text, last, m.start() - last);
        next.append("internal").append(m.group(1));
        last = m.end();
        ++out.count;
        return true;
    });
    if (out.count == 0) return out;
    next.append(text, last, std::string::npos);
    out.text = std::move(next);
    return out;
}

Report apply(const std::filesystem::path& folder) {
    namespace stdfs = std::filesystem;
    if (!stdfs::is_directory(fs::long_path(folder))) throw NotFoundError(folder, 0, "not a folder");
    std::vector<std::pair<std::string, stdfs::path>> files;   // (path relative to the folder, path), sorted
    for (const stdfs::directory_entry& e : stdfs::recursive_directory_iterator(fs::long_path(folder))) {
        if (!e.is_regular_file() || e.path().extension() != ".cs") continue;
        files.emplace_back(fs::to_text(stdfs::relative(e.path(), fs::long_path(folder))), e.path());
    }
    std::sort(files.begin(), files.end());

    Report report;
    for (const auto& [relative, path] : files) {
        const Bytes raw = fs::read_file(path);
        constexpr std::string_view kBom = "\xEF\xBB\xBF";
        std::string_view text = as_text(raw);
        if (text.starts_with(kBom)) text.remove_prefix(kBom.size());

        Rewritten refs = ref_casts(text);
        ConstructorsResult ctors = constructors(refs.text);
        Rewritten holders = data_holders(ctors.text);
        report.ref_casts += refs.count;
        report.constructors += ctors.count;
        report.data_holders += holders.count;
        for (Unplaced& u : ctors.left) report.left.push_back({relative, u.line, std::move(u.why)});
        if (holders.text != text) fs::write_file_atomic(path, as_bytes(holders.text));
    }
    return report;
}

}  // namespace cb::xna::fixups

#undef CB_ID_START
#undef CB_ID_CHAR
#undef CB_ID_DOTTED
#undef CB_LVALUE
