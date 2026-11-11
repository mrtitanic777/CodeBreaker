// cb/port/complaints.cpp -- see complaints.hpp.

#include "cb/port/complaints.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace cb::port::pre_ansi {

namespace {

#include "cb/port/complaints_table.inc"

struct Compiled {
    Complaint kind;
    std::vector<Regex> patterns;
};

const std::vector<Compiled>& compiled() {
    static const std::vector<Compiled> table = [] {
        std::vector<Compiled> out;
        for (const auto& [kind, patterns] : kSays) {
            Compiled c{kind, {}};
            for (const std::string_view p : patterns) c.patterns.emplace_back(p);
            out.push_back(std::move(c));
        }
        return out;
    }();
    return table;
}

}  // namespace

const std::vector<ComplaintPatterns>& complaint_table() {
    static const std::vector<ComplaintPatterns> table = [] {
        std::vector<ComplaintPatterns> out;
        for (const auto& [kind, patterns] : kSays) out.push_back({kind, patterns});
        return out;
    }();
    return table;
}

std::optional<Said> said(std::string_view text) {
    for (const Compiled& c : compiled()) {
        for (const Regex& rx : c.patterns) {
            if (auto m = rx.search(text)) return Said{c.kind, std::move(*m)};
        }
    }
    return std::nullopt;
}

std::string_view cast_pattern() {
    return R"re(\(\s*(?:(?:struct|union)\s+\w+|\w+)(?:\s+\w+)*\s*\**\s*\)|\(\s*[\w\s]+\**\s*\(\s*\*\s*\)\s*\([^()]*\)\s*\))re";
}

}  // namespace cb::port::pre_ansi
