#include "formats/nid_data.hpp"

#include <algorithm>
#include <charconv>
#include <cstddef>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

namespace cb::formats::nids {

// Written by CMake (embed_nids.cmake) into the build folder: one entry per config/nids/*.nids.
struct EmbeddedNids {
    const char* file;
    const char* text;
};
extern const EmbeddedNids kEmbeddedNids[];
extern const std::size_t kEmbeddedNidsCount;

namespace {

std::string_view file_text(std::string_view file) {
    for (std::size_t i = 0; i < kEmbeddedNidsCount; ++i)
        if (file == kEmbeddedNids[i].file) return kEmbeddedNids[i].text;
    return {};
}

// Call `row(key, name, extra)` for each row of `section`.
template <class F>
void each_row(std::string_view body, std::string_view section, F&& row) {
    bool in = false;
    while (!body.empty()) {
        const std::size_t nl = body.find('\n');
        std::string_view line = body.substr(0, nl);
        body = nl == std::string_view::npos ? std::string_view() : body.substr(nl + 1);
        if (!line.empty() && line.back() == '\r') line.remove_suffix(1);
        if (line.empty() || line.front() == '#') continue;
        if (line.front() == '[') {
            in = line.substr(1, line.size() - 2) == section;
            continue;
        }
        if (!in) continue;
        const std::size_t a = line.find(' ');
        if (a == std::string_view::npos) continue;
        std::string_view key = line.substr(0, a), rest = line.substr(a + 1), extra;
        const std::size_t b = rest.find(' ');
        if (b != std::string_view::npos) {
            extra = rest.substr(b + 1);
            rest = rest.substr(0, b);
        }
        row(key, rest, extra);
    }
}

std::uint64_t number(std::string_view s) {
    int base = 10;
    if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        base = 16;
        s.remove_prefix(2);
    }
    std::uint64_t v = 0;
    std::from_chars(s.data(), s.data() + s.size(), v, base);
    return v;
}

template <class T>
const T& cached(std::string_view file, std::string_view section, bool is_text, T (*build)(std::string_view, std::string_view)) {
    static std::mutex lock;
    static std::map<std::tuple<std::string, std::string, bool>, std::unique_ptr<T>> tables;
    std::lock_guard<std::mutex> g(lock);
    auto& slot = tables[{std::string(file), std::string(section), is_text}];
    if (!slot) slot = std::make_unique<T>(build(file_text(file), section));
    return *slot;
}

}  // namespace

const Row* NumericTable::find(std::uint64_t key) const {
    const auto it = std::lower_bound(rows_.begin(), rows_.end(), key, [](const Row& r, std::uint64_t v) { return r.key < v; });
    return it != rows_.end() && it->key == key ? &*it : nullptr;
}

const TextRow* TextTable::find(std::string_view key) const {
    const auto it = std::lower_bound(rows_.begin(), rows_.end(), key, [](const TextRow& r, std::string_view v) { return r.key < v; });
    return it != rows_.end() && it->key == key ? &*it : nullptr;
}

const NumericTable& numeric(std::string_view file, std::string_view section) {
    return cached<NumericTable>(file, section, false, [](std::string_view body, std::string_view sec) {
        NumericTable t;
        each_row(body, sec, [&](std::string_view k, std::string_view name, std::string_view extra) { t.rows_.push_back({number(k), name, extra}); });
        std::stable_sort(t.rows_.begin(), t.rows_.end(), [](const Row& a, const Row& b) { return a.key < b.key; });
        return t;
    });
}

const TextTable& text(std::string_view file, std::string_view section) {
    return cached<TextTable>(file, section, true, [](std::string_view body, std::string_view sec) {
        TextTable t;
        each_row(body, sec, [&](std::string_view k, std::string_view name, std::string_view) { t.rows_.push_back({k, name}); });
        std::stable_sort(t.rows_.begin(), t.rows_.end(), [](const TextRow& a, const TextRow& b) { return a.key < b.key; });
        return t;
    });
}

}  // namespace cb::formats::nids
