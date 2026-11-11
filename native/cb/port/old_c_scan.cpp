// cb/port/old_c_scan.cpp -- see old_c_scan.hpp.

#include "cb/port/old_c_scan.hpp"

#include <algorithm>

#include "cb/core/text.hpp"
#include "cb/port/c_source.hpp"

namespace cb::port::pre_ansi {

std::optional<std::uint64_t> c_integer(std::string_view text) {
    if (text.empty()) return std::nullopt;
    if (text.size() > 2 && text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) return text::parse_int<std::uint64_t>(text, 16);
    if (!std::all_of(text.begin(), text.end(), [](char c) { return c >= '0' && c <= '9'; })) return std::nullopt;
    const bool octal = text.size() > 1 && text[0] == '0' && std::all_of(text.begin(), text.end(), [](char c) { return c <= '7'; });
    return text::parse_int<std::uint64_t>(text, octal ? 8 : 10);
}

std::vector<std::string> file_names(const OrderedMap<std::string, std::string>& files) {
    std::vector<std::string> names;
    names.reserve(files.size());
    for (const auto& kv : files) names.push_back(kv.first);
    return names;
}

std::vector<std::string> sorted_names(const OrderedMap<std::string, std::string>& files) {
    std::vector<std::string> names;
    names.reserve(files.size());
    for (const auto& kv : files) names.push_back(kv.first);
    std::sort(names.begin(), names.end());
    return names;
}

std::string join_range(const std::vector<std::string>& lines, std::size_t first, std::size_t last) {
    std::string out;
    for (std::size_t i = first; i <= last && i < lines.size(); ++i) {
        if (i > first) out.push_back('\n');
        out += lines[i];
    }
    return out;
}

std::size_t line_of(std::string_view text, std::size_t offset) {
    return static_cast<std::size_t>(std::count(text.begin(), text.begin() + static_cast<std::ptrdiff_t>(std::min(offset, text.size())), '\n'));
}

std::string pointer_type_text(std::string_view type) {
    std::string s = text::replace_all(collapse_space(type), " *", "*");
    const std::size_t star = s.find('*');
    if (star != std::string::npos) s.insert(star, " ");
    return s;
}

std::map<std::string, std::string, std::less<>> pointer_targets(std::string_view code) {
    static const Regex declared(R"(\b((?:struct|union)\s+\w+)\s*\*\s*(\w+)\s*[;,)=])");
    std::map<std::string, std::string, std::less<>> out;
    declared.for_each_match(code, [&](const RegexMatch& m) {
        out[std::string(m.group(2))] = collapse_space(m.group(1));
        return true;
    });
    return out;
}

std::vector<CallSite> call_sites(std::string_view code, const std::function<bool(std::string_view)>& wanted, bool not_member) {
    std::vector<CallSite> out;
    std::size_t i = 0;
    while (i < code.size()) {
        if (!is_word_char(code[i])) {
            ++i;
            continue;
        }
        const std::size_t start = i;
        while (i < code.size() && is_word_char(code[i])) ++i;
        const std::string_view word = code.substr(start, i - start);
        if (not_member && start > 0 && (code[start - 1] == '.' || code[start - 1] == '>')) continue;
        std::size_t k = i;
        while (k < code.size() && text::is_space(code[k])) ++k;
        if (k < code.size() && code[k] == '(' && wanted(word)) out.push_back({start, word});
    }
    return out;
}

std::string apply_edits(std::string text, std::vector<Edit> edits, bool unique) {
    std::sort(edits.begin(), edits.end(), [](const Edit& a, const Edit& b) { return b < a; });
    if (unique) edits.erase(std::unique(edits.begin(), edits.end()), edits.end());
    for (const Edit& e : edits) text.replace(e.begin, e.end - e.begin, e.text);
    return text;
}

NamedRegex::NamedRegex(std::string pattern, RegexFlags flags) : pattern_(std::move(pattern)), flags_(flags) {}

const Regex& NamedRegex::with(std::string_view first, std::string_view second) {
    std::string key(first);
    key.push_back('\0');
    key.append(second);
    if (const auto it = cache_.find(key); it != cache_.end()) return it->second;
    std::string pattern = text::replace_all(pattern_, "@1@", regex_quote(first));
    pattern = text::replace_all(pattern, "@2@", regex_quote(second));
    return cache_.emplace(std::move(key), Regex(pattern, flags_)).first->second;
}

}  // namespace cb::port::pre_ansi
