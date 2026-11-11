// codebreaker/base/ctext.hpp -- SHIM over cb/base/ctext.hpp (docs/CPP_DESIGN.md 6.3): the old types in, the new
// functions called, the old types out. Deleted with its last py:: consumer.
//
// Old callers hold C text as py::str, whose offsets count code points; the new blanking works on UTF-8 bytes and
// writes one space for every byte of a character inside a literal. For text with such characters the shim puts one
// space back for each, so that the blanked text has the length and offsets of the original in the old sense too.
#pragma once

#include <functional>
#include <string>

#include "cb/base/ctext.hpp"
#include "cb/compat/py_bridge.hpp"
#include "py_os.hpp"

namespace codebreaker::base::ctext {

namespace detail {

// The blanked UTF-8 text as a py::str of the original's length: a multi-byte character that was blanked becomes one space.
inline py::str as_original_length(const py::str& original, const std::string& utf8, const std::string& blanked) {
    if (static_cast<std::size_t>(original.size()) == utf8.size()) return cb::compat::to_py(blanked);   // ASCII
    std::u32string out;
    std::size_t at = 0;
    while (at < utf8.size()) {
        const unsigned char lead = static_cast<unsigned char>(utf8[at]);
        const std::size_t length = lead < 0x80 ? 1 : lead >= 0xF0 ? 4 : lead >= 0xE0 ? 3 : 2;
        const std::string_view was(utf8.data() + at, length), now(blanked.data() + at, length);
        if (length == 1 || was == now) {
            out += cb::compat::to_py(std::string(now)).u32();
        } else {
            out += U' ';
        }
        at += length;
    }
    return py::str(out);
}

}  // namespace detail

inline py::str blank(const py::str& text, bool whole = false, bool across_lines = false) {
    const std::string utf8 = cb::compat::from_py(text);
    const cb::base::ctext::Mode mode = !whole ? cb::base::ctext::Mode::InsideLiterals
                                       : across_lines ? cb::base::ctext::Mode::WholeClosedAcrossLines
                                                      : cb::base::ctext::Mode::WholeClosed;
    return detail::as_original_length(text, utf8, cb::base::ctext::blank(utf8, mode));
}

// re.sub of what is code alone (not in strings or comments): the pattern is the old layer's, so it is run here.
inline py::str sub_code(const py::str& text, const py::re::Pattern& rx, const std::function<py::str(const py::re::Match&)>& fn) {
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
