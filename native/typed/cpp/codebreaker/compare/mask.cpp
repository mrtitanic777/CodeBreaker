// codebreaker/compare/mask.cpp -- typed C++ port of codebreaker.compare.mask.
//
// mask.py -- the parts of a function left out when it is compared with another build of it.
//
//     mask.of(offsets, size)                  a byte mask: 1 for every offset in `offsets`
//     mask.blank(code, offsets)               the bytes with those offsets set to zero
//     mask.same(a, b, *masks)                 the same length, and the same outside every mask
//     mask.differing(a, b, *masks)            the offsets where they differ outside every mask
//     mask.same_words(a, b, ignore)           words: the same outside the bits `ignore` leaves out
//     mask.distance(a, b, ma, mb, unit=1)     how far apart, unit by unit (a byte, or a word)
//
// Two builds of the same code differ where a loader or linker filled something in: a relocated
// address, the distance of a call, a far call the linker rewrote. Every comparison of one function
// with another leaves those places out, and does it here.

#include "codebreaker/compare/mask.hpp"

#include <algorithm>
#include <cstdlib>

namespace codebreaker::compare::mask {

namespace {

// verify/verdicts.py: a byte comparison's words
const char* const IDENTICAL = "identical";
const char* const SAME_SIZE = "same size";
const char* const DIFFERENT = "different";

int64_t mask_len(const Mask& m) {
    return std::visit([](const auto& v) -> int64_t { return py::len(v); }, m);
}

// m[i] (IndexError past the end, as Python's)
int64_t mask_at(const Mask& m, int64_t i) {
    return std::visit([i](const auto& v) -> int64_t { return v[i]; }, m);
}

// `any(m[i] for m in masks if m)`: whether some mask leaves byte i out
bool left_out(const py::list<MaybeMask>& masks, int64_t i) {
    for (const MaybeMask& m : masks) {
        if (m && mask_len(*m) > 0 && mask_at(*m, i) != 0) return true;
    }
    return false;
}

}  // namespace

// A byte mask `size` long that leaves out every offset in `offsets` (those outside it ignored).
py::bytearray of(const py::list<int64_t>& offsets, int64_t size) {
    py::bytearray out = py::bytearray::zeros(size);
    for (int64_t o : offsets) {
        if (0 <= o && o < size) {
            out[o] = 1;
        }
    }
    return out;
}

py::bytearray of(const py::set<int64_t>& offsets, int64_t size) {
    py::bytearray out = py::bytearray::zeros(size);
    for (int64_t o : offsets) {
        if (0 <= o && o < size) {
            out[o] = 1;
        }
    }
    return out;
}

// `code` with the bytes at `offsets` set to zero: what is left to compare, or to digest.
py::bytes blank(const py::bytes& code, py::list<int64_t> offsets) {
    py::bytearray out = py::bytearray(code);
    for (int64_t o : offsets) {
        if (0 <= o && o < py::len(out)) {
            out[o] = 0;
        }
    }
    return py::bytes(out);
}

py::bytes blank(const py::bytes& code, const py::set<int64_t>& offsets) {
    py::bytearray out = py::bytearray(code);
    for (int64_t o : offsets) {
        if (0 <= o && o < py::len(out)) {
            out[o] = 0;
        }
    }
    return py::bytes(out);
}

// Whether `a` and `b` are as long as each other and the same at every byte no mask leaves out.
bool same(const py::bytes& a, const py::bytes& b, const py::list<MaybeMask>& masks) {
    if (py::len(a) != py::len(b)) {
        return false;
    }
    for (int64_t i = 0; i < py::len(a); ++i) {
        if (a[i] != b[i] && !left_out(masks, i)) {
            return false;
        }
    }
    return true;
}

// The offsets (up to the shorter one's length) where `a` and `b` differ and no mask leaves the
// byte out.
py::list<int64_t> differing(const py::bytes& a, const py::bytes& b, const py::list<MaybeMask>& masks) {
    py::list<int64_t> out;
    int64_t n = std::min(py::len(a), py::len(b));
    for (int64_t i = 0; i < n; ++i) {
        if (a[i] != b[i] && !left_out(masks, i)) {
            out.append(i);
        }
    }
    return out;
}

// Whether two runs of words are the same outside the bits `ignore` leaves out of each word.
bool same_words(const py::list<int64_t>& a, const py::list<int64_t>& b, const py::list<int64_t>& ignore) {
    int64_t n = std::min({py::len(a), py::len(b), py::len(ignore)});
    for (int64_t i = 0; i < n; ++i) {
        if ((a[i] ^ b[i]) & ~ignore[i]) {
            return false;
        }
    }
    return true;
}

// How far `a` is from `b`, unit by unit (a byte, or a word of `unit` bytes), outside what the
// masks leave out. -> (verdict, units that differ, units compared, the first that differs or -1)
std::tuple<py::str, int64_t, int64_t, int64_t> distance(const py::bytes& a, const py::bytes& b,
                                                        const MaybeMask& ma, const MaybeMask& mb,
                                                        int64_t unit, const py::str& byteorder) {
    int64_t la = py::len(a);
    int64_t lb = py::len(b);
    int64_t n = py::floordiv(std::min(la, lb), unit);
    int64_t diff = 0;
    int64_t first = -1;
    if (unit == 1) {
        py::list<MaybeMask> both{ma, mb};
        py::list<int64_t> found = differing(a, b, both);
        diff = py::len(found);
        first = found.empty() ? -1 : found[0];
    } else {
        if (unit < 0) {
            throw py::ValueError("negative shift count");   // (1 << 8 * unit)
        }
        for (int64_t i = 0; i < n; ++i) {
            // the bits of word i left out: ma[i] | mb[i] (a missing or empty mask leaves none)
            int64_t ignore = (ma && mask_len(*ma) > 0 ? mask_at(*ma, i) : 0) |
                             (mb && mask_len(*mb) > 0 ? mask_at(*mb, i) : 0);
            bool little = byteorder == py::str("little");
            if (!little && !(byteorder == py::str("big"))) {
                throw py::ValueError("byteorder must be either 'little' or 'big'");
            }
            // (int.from_bytes of both words, xor, & ~ignore & full -- done byte by byte so that a word
            // wider than 64 bits needs no wider integer; bytes of `ignore` past its 8th are its sign)
            bool word_differs = false;
            for (int64_t k = 0; k < unit && !word_differs; ++k) {   // k: the byte's significance
                int64_t j = little ? k : unit - 1 - k;
                int64_t x = a[unit * i + j] ^ b[unit * i + j];
                int64_t ign = k < 8 ? static_cast<int64_t>((static_cast<uint64_t>(ignore) >> (8 * k)) & 0xFF)
                                    : (ignore < 0 ? 0xFF : 0);
                if (x & ~ign & 0xFF) {
                    word_differs = true;
                }
            }
            if (word_differs) {
                diff += 1;
                first = first < 0 ? i : first;
            }
        }
    }
    int64_t extra = py::floordiv(std::llabs(la - lb), unit);
    diff += extra;
    if (first < 0 && extra) {
        first = n;
    }
    const char* verdict = diff == 0 ? IDENTICAL : la == lb ? SAME_SIZE : DIFFERENT;     // (verify/verdicts.py)
    return {py::str(verdict), diff, py::floordiv(std::max(la, lb), unit), first};
}

}  // namespace codebreaker::compare::mask
