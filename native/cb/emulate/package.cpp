// cb/emulate/package.cpp -- see package.hpp.
#include "cb/emulate/package.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"

namespace cb::emulate {

void RunnerTable::add(std::string kind, RunnerName runner) {
    for (auto& entry : entries_) {
        if (entry.first == kind) {
            entry.second = std::move(runner);
            return;
        }
    }
    entries_.emplace_back(std::move(kind), std::move(runner));
}

const RunnerName* RunnerTable::find(std::string_view kind) const {
    auto it = std::find_if(entries_.begin(), entries_.end(), [&](const auto& e) { return e.first == kind; });
    return it == entries_.end() ? nullptr : &it->second;
}

const RunnerName& RunnerTable::at(std::string_view kind) const {
    if (const RunnerName* r = find(kind)) return *r;
    throw UsageError(std::format("no runner for kind '{}'", kind));
}

std::vector<std::string> RunnerTable::kinds() const {
    std::vector<std::string> out;
    for (const auto& e : entries_) out.push_back(e.first);
    return out;
}

}  // namespace cb::emulate
