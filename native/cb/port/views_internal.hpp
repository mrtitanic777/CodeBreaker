// cb/port/views_internal.hpp -- what views.cpp and views_offset.cpp share (not part of the port's interface).
#pragma once

#include <functional>
#include <map>
#include <utility>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/port/c_source.hpp"
#include "cb/port/csource.hpp"
#include "cb/port/struct_layout.hpp"

namespace cb::port::pre_ansi::views_detail {

// os.path.splitext(name)[0]: the name without its last extension (a leading dot is not one).
inline std::string stem_of(std::string_view name) {
    const std::size_t slash = name.find_last_of("/\\");
    const std::size_t base = slash == std::string_view::npos ? 0 : slash + 1;
    const std::size_t dot = name.rfind('.');
    if (dot == std::string_view::npos || dot <= base) return std::string(name);
    return std::string(name.substr(0, dot));
}

// Every run of white space one space.
inline std::string squeeze(std::string_view s) { return csource::collapse_space(s); }

// A regex with the program's name (an identifier) in it where the pattern says "@@".
inline Regex rx_name(std::string_view pattern, std::string_view name) { return csource::name_regex(pattern, name); }

// A regex with a structure's type in it where the pattern says "@T@": `struct s` is `struct\s+s`.
inline Regex rx_tag(std::string_view pattern, std::string_view tag) {
    return Regex(text::replace_all(pattern, "@T@", regex_quote(tag, true)));
}

// A map that keeps the order its keys were first given in, for keys that have no hash (a pair): re-assigning a key
// keeps its place. (What a Python dict does; the passes that visit their entries in this order say so.)
template <class K, class V>
class FirstSeenMap {
public:
    void assign(const K& key, V value) {
        const auto it = index_.find(key);
        if (it == index_.end()) {
            index_.emplace(key, items_.size());
            items_.emplace_back(key, std::move(value));
        } else {
            items_[it->second].second = std::move(value);
        }
    }
    auto begin() const { return items_.begin(); }
    auto end() const { return items_.end(); }
    std::size_t size() const { return items_.size(); }

private:
    std::vector<std::pair<K, V>> items_;
    std::map<K, std::size_t> index_;
};

// rx.replace_first with the replacement made from the match.
inline std::string replace_first_with(const Regex& rx, std::string_view subject, const std::function<std::string(const RegexMatch&)>& make) {
    const auto m = rx.search(subject);
    if (!m) return std::string(subject);
    std::string out(subject.substr(0, m->start()));
    out += make(*m);
    out += subject.substr(m->end());
    return out;
}

// The old layouts of the program: 16-bit pointers, far ones 4 bytes.
inline StructLayouts old_layouts_of(const Files& files) {
    std::vector<std::string> texts;
    for (const auto& kv : files) texts.push_back(kv.second);
    return lay_out_structs(texts, 2, 4);
}

// The key of a structure nested in another (`struct s.inner`): not a structure of the program's own.
inline bool is_nested(std::string_view key) { return key.find('.') != std::string_view::npos; }

inline bool has(std::string_view s, std::string_view part) { return s.find(part) != std::string_view::npos; }
inline bool has(std::string_view s, char c) { return s.find(c) != std::string_view::npos; }

inline const Member* member_named(const StructLayout& layout, std::string_view name) { return layout.find(name); }

// A line of `text` (the one the byte `at` is on), without its newline.
inline std::string_view line_at(std::string_view text, std::size_t at) {
    const std::size_t begin = text.rfind('\n', at == 0 ? 0 : at - 1);
    const std::size_t from = (at == 0 || begin == std::string_view::npos) ? 0 : begin + 1;
    const std::size_t end = text.find('\n', at);
    return text.substr(from, (end == std::string_view::npos ? text.size() : end) - from);
}

}  // namespace cb::port::pre_ansi::views_detail
