// codebreaker/base/ctext.cpp -- C++ from codebreaker.base.ctext.
//
// ctext.py -- C text with its strings, character constants and comments blanked: what is code, at the
// offsets it has, for the regular expressions and counts that must not look inside a literal.
//
//     ctext.blank(text)               what is inside each comment, string and character constant made
//                                     spaces -- the quotes and a comment's newlines kept, so every offset
//                                     and every line is the same; one left open runs to the end of its
//                                     line (a string) or of the text (a comment)
//     ctext.blank(text, whole=True)   each closed one made spaces whole, its quotes and newlines too (a
//                                     quote with no partner on its line is left as it is: an apostrophe)
//     ctext.blank(text, whole=True, across_lines=True)
//                                     the same, but a string or character constant runs on across lines
//                                     to its partner (how exact/batch's brace count has always read them)
//     ctext.sub_code(text, rx, fn)    re.sub of what is code alone (not in strings or comments)

#include "codebreaker/base/ctext.hpp"

#include <list>
#include <tuple>
#include <unordered_map>

namespace codebreaker::base::ctext {

const py::re::Pattern _OPEN = py::re::compile(py::str("/\\*[\\s\\S]*?(?:\\*/|\\Z)|\"(?:\\\\[\\s\\S]|[^\"\\\\\\n])*\"?|'(?:\\\\[\\s\\S]|[^'\\\\\\n])*'?"));
const py::re::Pattern _CLOSED = py::re::compile(py::str("/\\*[\\s\\S]*?\\*/|\"(?:\\\\[\\s\\S]|[^\"\\\\\\n])*\"|'(?:\\\\[\\s\\S]|[^'\\\\\\n])*'"));
const py::re::Pattern _ACROSS = py::re::compile(py::str("/\\*[\\s\\S]*?\\*/|\"(?:\\\\[\\s\\S]|[^\"\\\\])*\"|'(?:\\\\[\\s\\S]|[^'\\\\])*'"));

py::str _inside(const py::re::Match& m) {
    py::str t = m.group(0);
    if (t.startswith(py::str("/*"))) {                       // (a comment: its newlines kept)
        return py::re::sub(py::str("[^\\n]"), py::str(" "), t);
    }
    py::str q = t[0];
    int64_t end = py::len(t) > 1 && t[-1] == q ? py::len(t) - 1 : py::len(t);
    return q + py::repeat(py::str(" "), end - 1) + t.slice_from(end);   // (a string: inside it spaces, its quotes kept)
}

namespace {

// functools.lru_cache(maxsize=16384) over (text, whole, across_lines)
using Key = std::tuple<py::str, bool, bool>;
struct KeyHash {
    size_t operator()(const Key& k) const {
        return py::str_hash()(std::get<0>(k)) * 4 + (std::get<1>(k) ? 2 : 0) + (std::get<2>(k) ? 1 : 0);
    }
};
constexpr size_t CACHE_MAX = 16384;
using Order = std::list<std::pair<Key, py::str>>;     // most recently used first

Order& order_() {
    static Order o;
    return o;
}
// (hash of the key -> its entries: a lookup hashes the text it is given, no copy of it is made to look it up)
std::unordered_multimap<size_t, Order::iterator>& index_() {
    static std::unordered_multimap<size_t, Order::iterator> m;
    return m;
}
size_t key_hash(const py::str& text, bool whole, bool across_lines) {
    return py::str_hash()(text) * 4 + (whole ? 2 : 0) + (across_lines ? 1 : 0);
}

py::str blank_uncached(const py::str& text, bool whole, bool across_lines) {
    if (whole) {
        return (across_lines ? _ACROSS : _CLOSED).sub(
            [](const py::re::Match& m) { return py::repeat(py::str(" "), py::len(m.group(0))); }, text);
    }
    return _OPEN.sub(_inside, text);
}

}  // namespace

// the text with its comments, strings and character constants blanked, every offset the same: inside
// them made spaces (the quotes and newlines kept), or all of each closed one (`whole`; with
// `across_lines` a string or character constant may close on a later line). A text seen before is
// given as it was -- the passes ask often.
py::str blank(const py::str& text, bool whole, bool across_lines) {
    size_t h = key_hash(text, whole, across_lines);
    auto range = index_().equal_range(h);
    for (auto it = range.first; it != range.second; ++it) {
        const Key& k = it->second->first;
        if (std::get<1>(k) == whole && std::get<2>(k) == across_lines && std::get<0>(k) == text) {
            order_().splice(order_().begin(), order_(), it->second);
            return it->second->second;
        }
    }
    py::str result = blank_uncached(text, whole, across_lines);
    order_().emplace_front(Key(text, whole, across_lines), result);
    index_().emplace(h, order_().begin());
    if (order_().size() > CACHE_MAX) {
        const Key& old = order_().back().first;
        auto r = index_().equal_range(key_hash(std::get<0>(old), std::get<1>(old), std::get<2>(old)));
        for (auto it = r.first; it != r.second; ++it)
            if (it->second == std::prev(order_().end())) {
                index_().erase(it);
                break;
            }
        order_().pop_back();
    }
    return result;
}

// re.sub of what is code alone (not in strings or comments)
py::str sub_code(const py::str& text, const py::re::Pattern& rx, const std::function<py::str(const py::re::Match&)>& fn) {
    py::str code = blank(text);
    py::str out;
    int64_t last = 0;
    for (const py::re::Match& m : rx.finditer(code)) {
        out += text.slice(last, m.start());
        out += fn(m);
        last = m.end();
    }
    return out + text.slice_from(last);
}

}  // namespace codebreaker::base::ctext
