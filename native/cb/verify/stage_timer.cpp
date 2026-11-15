// cb/verify/stage_timer.cpp -- see stage_timer.hpp.
#include "cb/verify/stage_timer.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <map>
#include <mutex>
#include <vector>

#include "cb/core/fs.hpp"

namespace cb::verify::stage_timer {

namespace {

struct Total {
    double seconds = 0;
    std::uint64_t calls = 0;
};

struct Table {
    std::mutex lock;
    std::map<std::string, Total, std::less<>> stages;
};

Table& table() {
    static Table t;
    return t;
}

void print_at_exit() {
    const std::string text = report();
    if (!text.empty()) std::fputs(text.c_str(), stderr);
}

}  // namespace

bool enabled() {
    static const bool on = [] {
        const std::optional<std::string> v = fs::get_env("CB_PROFILE");
        const bool set = v && !v->empty();
        if (set) {
            table();   // (built before the exit handler is registered, so that it is still there when the handler runs)
            std::atexit(print_at_exit);
        }
        return set;
    }();
    return on;
}

void add(std::string_view name, double seconds) {
    Table& t = table();
    const std::lock_guard guard(t.lock);
    Total& total = t.stages.try_emplace(std::string(name)).first->second;
    total.seconds += seconds;
    ++total.calls;
}

std::string report() {
    Table& t = table();
    const std::lock_guard guard(t.lock);
    std::vector<std::pair<std::string, Total>> rows(t.stages.begin(), t.stages.end());
    std::ranges::stable_sort(rows, [](const auto& a, const auto& b) { return a.second.seconds > b.second.seconds; });
    std::string out;
    for (const auto& [name, total] : rows) out += std::format("profile {:<28} {:9.3f} s {:8} calls\n", name, total.seconds, total.calls);
    return out;
}

void reset() {
    Table& t = table();
    const std::lock_guard guard(t.lock);
    t.stages.clear();
}

}  // namespace cb::verify::stage_timer
