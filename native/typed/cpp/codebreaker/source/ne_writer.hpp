// codebreaker/source/ne_writer.hpp -- typed C++ of codebreaker.source.ne_writer
//
// ne_writer.py -- a 16-bit Windows (or OS/2) program as assembly source that builds back into it, byte for byte.
//
//     python -m codebreaker ne-source <program> [--out DIR] [--keep-stub]
//
//     Result r = ne_writer::reconstruct(path, outdir);
//     r.source, r.image, r.same, r.instructions, r.raw, r.rounds
//
// The way pe_writer writes a 32-bit image, for the segmented kind: one NASM source assembled flat (-f bin),
// each segment a section at its place in the file (start=) whose addresses are its own offsets (vstart=0),
// followed by its relocation table. Whatever the linker made is written as what it refers to, not as a
// number: the segment table's sectors and lengths as expressions of the segments' places, every relocation
// chain (LINK writes, in each place a relocation patches, where the next place of the same target is) as the
// label of the next place, every record's first place as its label, and an internal reference's offset as
// the label in its segment. So the file is the linker's output as written down, and stands without the linker
// that made it -- whichever version that was.
//
// Code is what is reached from the entry table, from the program's start, and from every internal reference
// into a code segment; what is not reached is data. An instruction is written as its mnemonic where NASM
// gives back the original's bytes (checked every round from the assembler's map, as pe_writer does); one that
// holds a place a relocation patches is written as its bytes, with those places as expressions and its
// mnemonic beside it. The DOS program in front (the stub) comes from its own source when that builds back.
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * An instruction is pe_writer::Insn (capstone's, with detail). A place is (segment, offset): `Place`.
//   * A segment's dict (the NE's own keys plus relocs, table, code) is `Seg`; a relocation chain's site value
//     (record index, width, chained, next) is `Site`.
//   * The "raw" set holds the instructions written as bytes, each a Place.
//   * `_stub` is pe_writer::_stub, which takes the file's bytes and where its new header is (the Python passed a
//     stand-in object carrying those two fields).
//   * reconstruct's `stub` is None (look for the stub's source), False (keep its bytes) or a path:
//     pe_writer::StubArg.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/formats/ne.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "codebreaker/source/pe_writer.hpp"
#include "codebreaker/translate/l2.hpp"
#include "py.hpp"

namespace codebreaker::source::ne_writer {

namespace ne = codebreaker::formats::ne;
using Insn = pe_writer::Insn;
using Place = std::tuple<int64_t, int64_t>;   // (segment, offset)

extern const py::set<py::str> _STOP;
extern const py::set<int64_t> _SEGS;
extern const py::dict<int64_t, int64_t> _WIDTH;   // bytes a relocation patches, by source type
extern const py::list<py::str> _PRELUDE;

// A segment of the file with its relocations.
struct Seg {
    int64_t idx = 0, file_off = 0, length = 0, flags = 0, minalloc = 0;
    py::str kind;
    py::list<ne::Relocation> relocs;
    int64_t table = 0;       // bytes of its relocation table (0 when it has none)
    bool code = false;
};

// What a relocation patches at one place: (record index, width, chained, next place of the chain).
struct Site {
    int64_t k = 0, w = 0;
    bool chained = false;
    int64_t nxt = 0;
};

class Writer {
  public:
    py::str path;
    ne::NEFile ne;
    py::bytes d;
    int64_t shift = 0;
    std::vector<Seg> segs;
    py::dict<int64_t, int64_t> by_idx;           // segment number -> its place in `segs`
    py::dict<Place, Site> sites;                 // every place a relocation patches
    py::dict<Place, Insn> insns;
    py::dict<Place, int64_t> covered;
    py::set<Place> starts;
    py::dict<Place, py::str> placed;
    py::set<Place> _raw;
    translate::l2::Dict l2;

    explicit Writer(const py::str& path);

    const Seg& seg(int64_t idx) const { return segs[static_cast<size_t>(by_idx.at(idx))]; }
    bool has_seg(int64_t idx) const { return by_idx.contains(idx); }

    // code
    void _decode();
    // Where each function begins, for the fragment each is in the file's L2 tree: the entry points and every place
    // a call lands -- a near call, or a far call through a relocation to a segment of the module's own -- where an
    // instruction was found.
    py::set<Place> _function_starts(const py::list<Place>& entries) const;
    static std::optional<int64_t> _target(const Insn& i);

    // names
    py::str label(int64_t si, int64_t a) const;
    py::str ref(int64_t si, int64_t a) const;
    py::set<Place> wanted(bool mark_all) const;

    // source
    py::list<py::str> text(int64_t si, const Insn& i, bool raw) const;
    std::optional<py::str> _nasm(int64_t si, const Insn& i, const std::optional<int64_t>& tgt) const;
    std::tuple<py::str, py::dict<int64_t, Place>> source(const py::set<Place>& raw = py::set<Place>(), bool mark_all = true,
                                                         const std::optional<py::str>& stub = std::nullopt, bool exact = true);
    // Bytes of the file outside the segments; the segment table's sectors and lengths as expressions.
    void _data_plain(image_writer::Assembly& L, int64_t a, int64_t end, int64_t segtab) const;
    void _segment(image_writer::Assembly& L, const Seg& s, py::dict<int64_t, Place>& lines_of) const;
    void _data(image_writer::Assembly& L, int64_t si, int64_t base, int64_t a, int64_t end) const;
};

py::set<Place> _differing(const Writer& w, const py::bytes& data, const py::dict<py::str, std::tuple<int64_t, int64_t>>& syms,
                          const py::set<Place>& raw);

struct Result {
    py::str source;
    std::optional<py::str> image;
    bool same = false;
    int64_t rounds = 0;
    int64_t raw = 0;
    py::str log;
    int64_t instructions = 0;

    image_writer::FlatReport report() const;
};

Result reconstruct(const py::str& path, const py::str& outdir, const pe_writer::StubArg& stub = std::monostate(),
                   const std::function<void(const py::str&)>& log = nullptr);

// python -m codebreaker ne-source <program> [--out DIR] [--keep-stub]
int64_t main(const py::list<py::str>& argv);

extern const py::str DOC;

}  // namespace codebreaker::source::ne_writer
