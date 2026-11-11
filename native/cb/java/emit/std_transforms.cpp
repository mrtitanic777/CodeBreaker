#include "cb/java/emit/std_transforms.hpp"

#include <algorithm>
#include <array>
#include <vector>

#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::emit::std_transforms {

namespace {

bool is_word(char c) {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

// `word` -> `replacement` wherever it stands as a whole word.
std::string replace_word(std::string_view s, std::string_view word, std::string_view replacement) {
    std::string out;
    out.reserve(s.size());
    std::size_t pos = 0;
    while (true) {
        const std::size_t at = s.find(word, pos);
        if (at == std::string_view::npos) break;
        const std::size_t after = at + word.size();
        const bool starts = at == 0 || !is_word(s[at - 1]);
        const bool ends = after >= s.size() || !is_word(s[after]);
        out.append(s.substr(pos, at - pos));
        out.append(starts && ends ? replacement : word);
        pos = after;
    }
    out.append(s.substr(pos));
    return out;
}

bool has_word(std::string_view s, std::string_view word) {
    std::size_t pos = 0;
    while ((pos = s.find(word, pos)) != std::string_view::npos) {
        const std::size_t after = pos + word.size();
        if ((pos == 0 || !is_word(s[pos - 1])) && (after >= s.size() || !is_word(s[after]))) return true;
        pos = after;
    }
    return false;
}

// A standard-library container type: `[const ][std::]map|set|vector|list|deque<...>` with the template argument
// list nested at most one level deep, then `::[const_]iterator`.
constexpr const char* kIterType =
    R"((?:const\s+)?(?:std::)?(?:map|set|vector|list|deque)<[^>]*(?:<[^>]*>)*>\s*::\s*(?:const_)?iterator)";

// for ( <iterator type> name   ->   for(auto name
const Regex& iter_for() {
    static const Regex re(std::string(R"(for\s*\(\s*)") + kIterType + R"(\s+(\w+))");
    return re;
}

// <indent><iterator type> name =   ->   <indent>auto name =        (at the start of a line)
const Regex& iter_decl() {
    static const Regex re(std::string(R"(^(\s*))") + kIterType + R"(\s+(\w+\s*=))", RegexFlags::Multiline);
    return re;
}

const Regex& static_const_int() {
    static const Regex re(R"(\bstatic\s+const\s+int\b)");
    return re;
}

// Names that are macros on a target and so cannot be enum constants.
constexpr auto kReservedRefs = std::to_array<std::string_view>({
    "TRUE", "FALSE", "NULL", "NAN", "INFINITY", "ERROR", "DELETE", "IN", "OUT", "OPTIONAL", "CONST", "VOID", "NEAR",
    "FAR", "THIS", "INTERFACE", "DOMAIN",
});

}  // namespace

std::string map_outside_literals(std::string_view text, const std::function<std::string(std::string_view)>& f) {
    std::string out;
    std::size_t last = 0;
    for (const textutil::Span& lit : textutil::literal_spans(text)) {
        out += f(text.substr(last, lit.begin - last));
        out.append(text.substr(lit.begin, lit.end - lit.begin));
        last = lit.end;
    }
    out += f(text.substr(last));
    return out;
}

std::string annotate_first(std::string_view text, std::string_view pattern, std::string_view annotation) {
    if (pattern.empty()) return std::string(text);   // nothing to look for, so nothing is found
    std::size_t pos = 0;
    while (pos <= text.size()) {
        const std::size_t nl = text.find('\n', pos);
        const std::size_t end = nl == std::string_view::npos ? text.size() : nl;
        const std::string_view line = text.substr(pos, end - pos);
        const std::size_t at = line.find(pattern);
        if (at != std::string_view::npos && line.find("//", at + pattern.size()) == std::string_view::npos) {
            std::string out(text.substr(0, pos));
            out.append(text::rtrim(line));
            out += "  ";
            out.append(annotation);
            out.append(text.substr(end));
            return out;
        }
        if (nl == std::string_view::npos) break;
        pos = nl + 1;
    }
    return std::string(text);
}

std::string sanitize_reserved_members(std::string_view text) {
    std::string out;
    out.reserve(text.size());
    std::size_t pos = 0;
    while (true) {
        const std::size_t at = text.find("::", pos);
        if (at == std::string_view::npos) break;
        const std::size_t name_begin = at + 2;
        std::size_t name_end = name_begin;
        while (name_end < text.size() && is_word(text[name_end])) ++name_end;
        const std::string_view name = text.substr(name_begin, name_end - name_begin);
        out.append(text.substr(pos, name_end - pos));
        if (std::find(kReservedRefs.begin(), kReservedRefs.end(), name) != kReservedRefs.end()) out += '_';
        pos = name_end;
    }
    out.append(text.substr(pos));
    return out;
}

std::string apply_std_transforms(std::string_view text_in, int cpp_year) {
    std::string text(text_in);
    if (cpp_year < 2011) return text;

    const bool had_null = has_word(text, "NULL");
    text = map_outside_literals(text, [](std::string_view piece) { return replace_word(piece, "NULL", "nullptr"); });
    if (had_null) text = annotate_first(text, "nullptr", "// [C++11] nullptr replaces NULL");

    const bool had_iter = iter_for().test(text) || iter_decl().test(text);
    text = iter_for().replace_all(text, [](const RegexMatch& m) { return "for(auto " + std::string(m[1]); });
    text = iter_decl().replace_all(
        text, [](const RegexMatch& m) { return std::string(m[1]) + "auto " + std::string(m[2]); });
    if (had_iter) text = annotate_first(text, "auto ", "// [C++11] auto type deduction");

    const bool had_const = static_const_int().test(text);
    text = map_outside_literals(
        text, [](std::string_view piece) { return static_const_int().replace_all(piece, "static constexpr int"); });
    if (had_const) text = annotate_first(text, "constexpr", "// [C++11] constexpr replaces const");

    return text;
}

}  // namespace cb::java::emit::std_transforms
