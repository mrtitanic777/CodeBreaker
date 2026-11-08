// codebreaker/source/assembly.hpp -- C++17 port of codebreaker.source.assembly.
//
// assembly.py -- what the assembly-source writers share (drivers, real-mode programs).
//
//     assembly::operands(text)                  a decoder's operand text in the assembler's spelling
//     assembly::data_lines(bytes)               bytes as `db` lines, text as text
//     assembly::probe(candidates, ...)          which instruction texts the assembler turns back into
//                                               the original's bytes
//
// An instruction is written as its mnemonic only when the assembler turns that text back into the
// bytes the original has -- assemblers choose between equivalent encodings, and a different choice
// moves everything behind it. That is checked rather than assumed: every distinct instruction text
// is assembled once, at a place of its own in a probe file, and its bytes are read back from the
// object file. What the probe has established is remembered (state/assembly_probes.json), so a
// text is assembled once however many programs use it.
//
// Typing notes:
//   * A probe candidate is (text, the original bytes outside patched fields, a mask with 1 for each byte of a
//     patched field): `Candidate`.  `labels` and `near` (any iterable of str in the Python; callers pass sets)
//     are py::set<py::str>.
//   * culprits() is a template over the key an instruction piece carries (the callers use an address, an int,
//     or an (object, address) pair): `Key` is any type with ==, hash and a py::set.  A piece is
//     (offset, length, mask or None, key or None, is a jump inside the segment).
//   * The file of what is known (assembly_probes.json) holds booleans; a value of another kind is read as its
//     truth and written back as true/false.  JSON is written as json.dump(known, f, indent=0, sort_keys=True).
//   * `_PROBE_LOCK` is a std::mutex.  _TEXT (rb"[\x20-\x26\x28-\x7e]{5,}") is applied by hand in data_lines:
//     the runs of printable bytes other than the quote itself that are five or more long.
#pragma once

#include <algorithm>
#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::source::assembly {

extern const py::list<py::str> CONDITIONS;
extern const py::dict<py::str, py::str> SPELLING;
extern const py::str STRING_OPS;   // the pattern, as the Python has it (with its re.compile flags: none)
extern const py::str HEX_PATTERN;   // (the Python's _HEX: that name is a macro of the C library's ctype.h)

// Assemble one file with OpenWatcom's assembler. -> (object file or None, its messages)
std::tuple<std::optional<py::str>, py::str> assemble(const py::str& source, const py::str& work);

// Link with OpenWatcom's linker from the directives in `lines`. -> (file or None, messages)
std::tuple<std::optional<py::str>, py::str> link(const py::list<py::str>& lines, const py::str& out, const py::str& work);

// Hexadecimal numbers the way the assembler reads them (0x1f -> 01Fh).
py::str operands(const py::str& text);

py::str mnemonic(const py::str& name);

// Bytes as `db` lines; runs of printable characters as quoted text.
py::list<py::str> data_lines(const py::bytes& data, int64_t per_line = 16);

using Candidate = std::tuple<py::str, py::bytes, py::bytes>;

py::str _key(const Candidate& candidate, const py::str& mode);

// {key: did the text assemble back} as state/assembly_probes.json has it ({} when there is none or it is damaged)
py::dict<py::str, bool> _known();

// One probe at a time: they share the file of what is known.
py::set<Candidate> probe(const py::list<Candidate>& candidates, const py::set<py::str>& labels, const py::str& outdir,
                         const py::str& mode = py::str("flat32"), const py::set<py::str>& near = py::set<py::str>());

// Which candidates assemble to their original bytes. A candidate is (text, the original
// bytes outside patched fields, a mask with 1 for each byte of a patched field); `labels` are
// the names the texts may mention (`near` those used as call or jump targets). `mode` is
// "flat32" (32-bit code, flat model) or "real16". -> the set of candidates that do.
py::set<Candidate> _probe(const py::list<Candidate>& candidates, const py::set<py::str>& labels, const py::str& outdir,
                          const py::str& mode, const py::set<py::str>& near);

// {offset: bytes} of the data records of the last segment an object file defines, joined
// where one record continues the one before.
py::dict<int64_t, py::bytes> _last_segment_data(const py::bytes& obj);

// One piece of the original: (offset, length, mask or None, key or None, is a jump inside the segment)
template <class Key>
using Piece = std::tuple<int64_t, int64_t, std::optional<py::bytes>, std::optional<Key>, bool>;

// Which instructions did not come back as they were. `pieces` covers the original in
// order: (offset, length, mask or None, key or None, is a jump inside the segment); a piece
// with a key is an instruction written as its mnemonic. The built bytes are walked piece by
// piece, so that an instruction that came back longer or shorter is found itself, and not
// the first place behind it where the bytes no longer line up. A jump that only changed
// because what it jumps over changed size is blamed only when nothing else is.
// -> the set of keys to write as bytes instead.
template <class Key>
py::set<Key> culprits(const py::bytes& original, const py::bytes& built, const py::list<Piece<Key>>& pieces) {
    auto same = [&](int64_t pos, const Piece<Key>& piece) -> bool {
        const int64_t off = std::get<0>(piece), n = std::get<1>(piece);
        const std::optional<py::bytes>& mask = std::get<2>(piece);
        if (pos < 0 || pos + n > built.size()) {
            return false;
        }
        py::bytes a = built.slice(pos, pos + n);
        py::bytes b = original.slice(off, off + n);
        if (!mask) {
            return a == b;
        }
        const int64_t m = std::min({a.size(), b.size(), mask->size()});
        for (int64_t i = 0; i < m; ++i) {
            if (!(*mask)[i] && a[i] != b[i]) {
                return false;
            }
        }
        return true;
    };

    int64_t pos = 0;
    py::set<Key> bad, jumps;
    const int64_t count = pieces.size();
    int64_t k = 0;
    std::vector<int64_t> order;   // range(-8, 17) sorted by abs (stable)
    for (int64_t d = -8; d < 17; ++d) {
        order.push_back(d);
    }
    std::stable_sort(order.begin(), order.end(), [](int64_t x, int64_t y) { return py::abs(x) < py::abs(y); });
    while (k < count) {
        const Piece<Key>& piece = pieces[k];
        if (same(pos, piece)) {
            pos += std::get<1>(piece);
            ++k;
            continue;
        }
        if (std::get<3>(piece)) {
            (std::get<4>(piece) ? jumps : bad).add(*std::get<3>(piece));
        }
        // where does the built file continue? try the sizes this piece may have come back with
        int64_t ahead = 0;
        for (int64_t j = k + 1; j < std::min<int64_t>(k + 5, count); ++j) {
            if (std::get<1>(pieces[j]) >= 2) {
                ++ahead;
            }
        }
        if (ahead == 0) {
            ahead = std::min<int64_t>(k + 2, count) - std::min<int64_t>(k + 1, count);
        }
        bool found = false;
        for (int64_t d : order) {
            const int64_t p = pos + std::get<1>(piece) + d;
            if (p < pos) {
                continue;
            }
            int64_t q = p;
            bool all = true;
            const int64_t upto = std::min<int64_t>(k + 1 + std::max<int64_t>(1, ahead), count);
            for (int64_t j = k + 1; j < upto; ++j) {
                if (!same(q, pieces[j])) {
                    all = false;
                    break;
                }
                q += std::get<1>(pieces[j]);
            }
            if (all) {
                pos = p;
                found = true;
                break;
            }
        }
        if (!found) {
            break;   // lost: what was found so far is the lesson
        }
        ++k;
    }
    return !bad.empty() ? bad : jumps;
}

}  // namespace codebreaker::source::assembly
