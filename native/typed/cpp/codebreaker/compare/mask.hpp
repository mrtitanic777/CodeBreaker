// codebreaker/compare/mask.hpp -- typed C++ port of codebreaker.compare.mask.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::compare::mask {

// A mask comes in one of two forms. For code read a byte at a time it says, byte by byte, whether
// the byte is left out: a sequence of flags, any non-zero value meaning left out (py::bytes holds
// `of`'s bytearray; a py::list<int64_t> holds any other flag sequence). For code made of
// fixed-size words it says, word by word, which bits of the word are left out
// (py::list<int64_t>). Python's "sequence of whatever" is this variant; None is std::nullopt.
using Mask = std::variant<py::bytes, py::list<int64_t>>;
using MaybeMask = std::optional<Mask>;

// A byte mask `size` long that leaves out every offset in `offsets` (those outside it ignored).
py::bytearray of(const py::list<int64_t>& offsets, int64_t size);
py::bytearray of(const py::set<int64_t>& offsets, int64_t size);

// `code` with the bytes at `offsets` set to zero: what is left to compare, or to digest.
py::bytes blank(const py::bytes& code, py::list<int64_t> offsets);
py::bytes blank(const py::bytes& code, const py::set<int64_t>& offsets);

// Whether `a` and `b` are as long as each other and the same at every byte no mask leaves out.
bool same(const py::bytes& a, const py::bytes& b, const py::list<MaybeMask>& masks);

// The offsets (up to the shorter one's length) where `a` and `b` differ and no mask leaves the
// byte out.
py::list<int64_t> differing(const py::bytes& a, const py::bytes& b, const py::list<MaybeMask>& masks);

// Whether two runs of words are the same outside the bits `ignore` leaves out of each word.
bool same_words(const py::list<int64_t>& a, const py::list<int64_t>& b, const py::list<int64_t>& ignore);

// How far `a` is from `b`, unit by unit (a byte, or a word of `unit` bytes), outside what the
// masks leave out: for bytes, ma[i] / mb[i] say whether byte i is left out (a relocated address,
// a call's distance); for words, they are the bits of word i that are left out. When the lengths
// differ the whole tail counts as differing, so a longer or shorter piece is always farther than
// one of the right length.
// -> (verdict, units that differ, units compared, the first that differs or -1)
std::tuple<py::str, int64_t, int64_t, int64_t> distance(const py::bytes& a, const py::bytes& b,
                                                        const MaybeMask& ma = std::nullopt,
                                                        const MaybeMask& mb = std::nullopt,
                                                        int64_t unit = 1,
                                                        const py::str& byteorder = py::str("little"));

}  // namespace codebreaker::compare::mask
