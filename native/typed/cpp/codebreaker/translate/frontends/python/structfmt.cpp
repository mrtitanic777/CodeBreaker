// codebreaker/translate/frontends/python/structfmt.cpp -- C++ from codebreaker.translate.frontends.python.structfmt.
//
// structfmt.py -- a struct module format string, read the way CPython reads it.
//
//     parse('<12H') -> [('H', 2, 'int', 0, 65535)] * 12          (code, size, kind, lo, hi)
//
// Only the standard-size forms the C++ support header implements are accepted: a byte-order prefix of
// < > ! = (or none / @, native: Windows x64, where l and L are 4 bytes), and the codes x c b B ? h H i I l L
// q Q f d s. Anything else is None (the call is refused).

#include "codebreaker/translate/frontends/python/structfmt.hpp"

namespace codebreaker::translate::frontends::python::structfmt {

namespace {

// code -> (size, signed)
const py::dict<py::str, std::tuple<int64_t, bool>> _INT = {
    {py::str("b"), {1, true}}, {py::str("B"), {1, false}}, {py::str("h"), {2, true}}, {py::str("H"), {2, false}},
    {py::str("i"), {4, true}}, {py::str("I"), {4, false}}, {py::str("l"), {4, true}}, {py::str("L"), {4, false}},
    {py::str("q"), {8, true}}, {py::str("Q"), {8, false}}};

// int(count) for a run of decimal digits
int64_t int_of(const py::str& digits) {
    int64_t n = 0;
    for (char32_t c : digits.u32()) {
        int64_t d = static_cast<int64_t>(c - U'0');
        if (n > (INT64_MAX - d) / 10) throw py::OverflowError("count too large for int64");
        n = n * 10 + d;
    }
    return n;
}

// `ch in chars` for a one-character str
bool in_(const py::str& ch, const char* chars) {
    return py::str(chars).contains(ch);
}

}  // namespace

std::optional<py::list<Item>> parse(const py::str& fmt_in) {
    py::str fmt = fmt_in;
    py::str order("@");
    // fmt[:1] in '<>!=@' -- true for '' too, whose fmt[0] then raises IndexError
    if (in_(fmt.slice_to(1), "<>!=@")) {
        if (fmt.empty()) throw py::IndexError("string index out of range");
        order = fmt[0];
        fmt = fmt.slice_from(1);
    }
    py::list<Item> items;
    static const py::re::Pattern item_rx = py::re::compile(py::str("\\s*(\\d*)\\s*([a-zA-Z?])"));
    for (const std::tuple<py::str, py::str>& cc : item_rx.findall_groups<py::str, py::str>(fmt)) {
        const py::str& count = std::get<0>(cc);
        const py::str& code = std::get<1>(cc);
        int64_t n = !count.empty() ? int_of(count) : 1;
        if (code == "x") {
            items.append(Item(py::str("x"), n, py::str("pad"), 0, 0));
        } else if (code == "s") {
            items.append(Item(py::str("s"), n, py::str("bytes"), n, static_cast<uint64_t>(n)));
        } else if (code == "c") {
            for (int64_t k = 0; k < n; ++k) items.append(Item(py::str("c"), 1, py::str("bytes"), 1, 1));
        } else if (code == "?") {
            for (int64_t k = 0; k < n; ++k) items.append(Item(py::str("?"), 1, py::str("bool"), 0, 1));
        } else if (in_(code, "fd")) {
            for (int64_t k = 0; k < n; ++k) items.append(Item(code, code == "f" ? 4 : 8, py::str("float"), 0, 0));
        } else if (_INT.contains(code)) {
            auto [size, is_signed] = _INT.at(code);
            int64_t lo;
            uint64_t hi;
            if (is_signed) {
                // -(1 << (8 * size - 1)), (1 << (8 * size - 1)) - 1
                lo = size == 8 ? INT64_MIN : -(INT64_C(1) << (8 * size - 1));
                hi = (UINT64_C(1) << (8 * size - 1)) - 1;
            } else {
                lo = 0;
                hi = size == 8 ? UINT64_MAX : (UINT64_C(1) << (8 * size)) - 1;
            }
            for (int64_t k = 0; k < n; ++k) items.append(Item(code, size, py::str("int"), lo, hi));
        } else {
            return std::nullopt;
        }
    }
    if (!py::re::sub(py::str("[\\s\\d]|[a-zA-Z?]"), py::str(""), fmt).empty()) {
        return std::nullopt;
    }
    if (order == "@") {
        bool any_wide = false;
        for (const Item& i : items) {
            if (in_(std::get<0>(i), "hHiIlLqQfd")) { any_wide = true; break; }
        }
        if (any_wide) {
            // native alignment pads between items; the support header implements standard sizes only
            bool any_other = false;
            for (const Item& i : items) {
                if (!in_(std::get<0>(i), "xcbB?s")) { any_other = true; break; }
            }
            if (any_other && py::len(items) > 1) return std::nullopt;
        }
    }
    return items;
}

int64_t size(const py::list<Item>& items) {
    int64_t total = 0;
    for (const Item& i : items) total += std::get<1>(i);
    return total;
}

// the items that are values (pads are not)
py::list<Item> values(const py::list<Item>& items) {
    py::list<Item> result;
    for (const Item& i : items) {
        if (std::get<0>(i) != "x") result.append(i);
    }
    return result;
}

}  // namespace codebreaker::translate::frontends::python::structfmt
