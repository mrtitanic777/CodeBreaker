// codebreaker/invert/known_differences.hpp -- SHIM over cb/invert/known_differences.hpp (docs/CPP_DESIGN.md 6.3); deleted
// with its last py:: consumer. The old module's table KB is a mutable list: diagnose() reads it as it is.
#pragma once

#include <optional>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/invert/known_differences.hpp"

namespace codebreaker::invert::known_differences {

struct Entry {
    py::str id;
    py::str match;
    py::str cause;
    py::str fix;
    py::str msc6dec_construct;
    bool auto_ = false;
    py::str example;
};

namespace detail {
inline Entry entry_of(const cb::invert::DifferencePattern& p) {
    return {cb::compat::to_py(p.id), cb::compat::to_py(p.match), cb::compat::to_py(p.cause), cb::compat::to_py(p.fix),
            cb::compat::to_py(p.construct), p.automatic, cb::compat::to_py(p.example)};
}
inline cb::invert::DifferencePattern pattern_of(const Entry& e) {
    return {cb::compat::from_py(e.id),        cb::compat::from_py(e.match), cb::compat::from_py(e.cause),
            cb::compat::from_py(e.fix),       cb::compat::from_py(e.msc6dec_construct), e.auto_, cb::compat::from_py(e.example)};
}
}  // namespace detail

inline py::list<Entry> KB = [] {
    const cb::invert::KnownDifferences seeded;
    py::list<Entry> kb;
    for (const auto& p : seeded.patterns()) kb.append(detail::entry_of(p));
    return kb;
}();

// Match a localized divergence against the KB; return the matching pattern(s).
inline py::list<Entry> diagnose(const std::optional<py::str>& divergence_text) {
    return cb::compat::guard([&] {
        std::vector<cb::invert::DifferencePattern> table;
        for (const Entry& e : KB) table.push_back(detail::pattern_of(e));
        const cb::invert::KnownDifferences kb(std::move(table));
        py::list<Entry> out;
        for (const auto* p : kb.diagnose(divergence_text ? cb::compat::from_py(*divergence_text) : std::string())) {
            out.append(detail::entry_of(*p));
        }
        return out;
    });
}

// Append a newly-solved divergence pattern (id, match, cause, fix, ...).
inline Entry record(const Entry& pattern) {
    KB.append(pattern);
    return pattern;
}

// The module's `if __name__ == "__main__":` block: print the KB, each entry's id, cause and fix.
inline void main() {
    std::vector<cb::invert::DifferencePattern> table;
    for (const Entry& e : KB) table.push_back(detail::pattern_of(e));
    std::string text = cb::invert::KnownDifferences(std::move(table)).listing();
    text.pop_back();   // (print adds the last newline)
    py::print(cb::compat::to_py(text));
}

}  // namespace codebreaker::invert::known_differences
