// decompile/function_in.cpp -- one function cut out of a whole module's C (decompiler.hpp: function_in).
#include <algorithm>
#include <cctype>
#include <optional>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

#include "core/text.hpp"
#include "decompile/decompiler.hpp"

namespace cb::decompile::decompiler {
namespace {

// Does a function header line name the place `place` is (hex, or segment:offset)? Compared as numbers: sub_00132 is 132, sub_1_0116 is
// 1:0116, func_0x7f021036 is 7f021036.
bool function_in_same_place(std::string_view place, const std::string& header) {
    static const std::regex pattern(R"(\b(?:sub|func|FUN)_(?:0x)?([0-9a-fA-F]+)(?:_([0-9a-fA-F]+))?\s*\()");
    std::smatch m;
    if (!std::regex_search(header, m, pattern)) return false;
    try {
        const std::string addr(place);
        if (const std::size_t colon = addr.find(':'); colon != std::string::npos) {
            if (!m[2].matched) return false;
            return std::stoull(m[1].str(), nullptr, 16) == std::stoull(addr.substr(0, colon), nullptr, 16) &&
                   std::stoull(m[2].str(), nullptr, 16) == std::stoull(addr.substr(colon + 1), nullptr, 16);
        }
        return !m[2].matched && std::stoull(m[1].str(), nullptr, 16) == std::stoull(addr, nullptr, 16);
    } catch (const std::exception&) {
        return false;
    }
}

std::string function_in_escaped(std::string_view s) {
    std::string out;
    for (const char c : s) {
        if (std::string_view(R"(\^$.|?*+()[]{})").find(c) != std::string_view::npos) out.push_back('\\');
        out.push_back(c);
    }
    return out;
}

}  // namespace

std::optional<std::string> function_in(std::string_view module_c, std::string_view place) {
    const bool by_address = !place.empty() && std::all_of(place.begin(), place.end(), [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || c == ':'; });
    std::optional<std::regex> named;
    if (!by_address) named.emplace(R"(\b)" + function_in_escaped(place) + R"(\s*\()", std::regex::icase);
    std::vector<std::string> lines;
    for (std::size_t from = 0; from < module_c.size();) {
        std::size_t to = module_c.find_first_of("\r\n", from);
        if (to == std::string_view::npos) to = module_c.size();
        lines.emplace_back(module_c.substr(from, to - from));
        if (to < module_c.size() && module_c[to] == '\r' && to + 1 < module_c.size() && module_c[to + 1] == '\n') ++to;
        from = to + 1;
    }
    for (std::size_t i = 0; i < lines.size(); ++i) {
        const std::string& line = lines[i];
        if (line.empty() || std::isspace(static_cast<unsigned char>(line[0])) || line.find('(') == std::string::npos) continue;
        const std::string_view right = text::trim(line);
        const bool opens = right.ends_with("{") || (i + 1 < lines.size() && (text::trim(lines[i + 1]) == "{" || text::trim(lines[i + 1]).empty()));
        if (!opens) continue;
        if (!(named ? std::regex_search(line, *named) : function_in_same_place(place, line))) continue;
        std::size_t start = i;
        while (start > 0 && (lines[start - 1].starts_with("/*") || lines[start - 1].starts_with(" *") || lines[start - 1].starts_with("//"))) --start;
        std::size_t end = i + 1;
        while (end < lines.size() && lines[end] != "}") ++end;
        std::string out;
        for (std::size_t k = start; k <= end && k < lines.size(); ++k) {
            if (k > start) out += '\n';
            out += lines[k];
        }
        return out;
    }
    return std::nullopt;
}

}  // namespace cb::decompile::decompiler
