#include "cb/build/asm_text.hpp"

#include <charconv>

#include "cb/core/text.hpp"

namespace cb::build {

std::string_view strip_comment(std::string_view line) {
    bool quoted = false;
    for (std::size_t i = 0; i < line.size(); ++i) {
        if (line[i] == '"') {
            quoted = !quoted;
        } else if (line[i] == ';' && !quoted) {
            return line.substr(0, i);
        }
    }
    return line;
}

std::vector<std::string_view> split_items(std::string_view text, bool track_parens) {
    std::vector<std::string_view> out;
    bool quoted = false;
    int depth = 0;
    std::size_t start = 0;
    for (std::size_t i = 0; i < text.size(); ++i) {
        const char c = text[i];
        if (c == '"') quoted = !quoted;
        if (!quoted && track_parens) {
            if (c == '(') ++depth;
            if (c == ')') --depth;
        }
        if (c == ',' && !quoted && depth == 0) {
            out.push_back(text::trim(text.substr(start, i - start)));
            start = i + 1;
        }
    }
    const std::string_view last = text::trim(text.substr(start));
    if (!last.empty()) out.push_back(last);
    return out;
}

std::optional<std::int64_t> parse_number(std::string_view text) {
    text = text::trim(text);
    bool negative = false;
    if (!text.empty() && (text.front() == '-' || text.front() == '+')) {
        negative = text.front() == '-';
        text.remove_prefix(1);
    }
    int base = 10;
    if (text.size() > 2 && text[0] == '0') {
        const char p = static_cast<char>(text[1] | 0x20);
        if (p == 'x') base = 16;
        else if (p == 'b') base = 2;
        else if (p == 'o') base = 8;
        if (base != 10) text.remove_prefix(2);
    }
    if (text.empty() || text.front() == '-' || text.front() == '+') return std::nullopt;
    unsigned long long magnitude = 0;
    const auto [end, ec] = std::from_chars(text.data(), text.data() + text.size(), magnitude, base);
    if (ec != std::errc() || end != text.data() + text.size()) return std::nullopt;
    if (magnitude > static_cast<unsigned long long>(INT64_MAX)) return std::nullopt;
    const auto v = static_cast<std::int64_t>(magnitude);
    return negative ? -v : v;
}

}  // namespace cb::build
