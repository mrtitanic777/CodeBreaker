// codebreaker/source/le_writer.hpp -- typed C++ of codebreaker.source.le_writer
//
// le_writer.py -- a virtual device driver (LE, and the W3 bundle VMM32.VXD) as assembly source that builds back byte for byte.
//
//     python -m codebreaker le-source <driver> [--out DIR]
//
//     Result r = le_writer::reconstruct(path, outdir);
//     r.source, r.image, r.same, r.instructions, r.raw, r.rounds
//
// driver_writer.py writes a driver as the driver kit's macro source, built by wasm and wlink: that is the
// driver as it was written, and it comes back the same in structure. This writes the FILE, the way
// pe_writer and ne_writer do: one NASM source assembled flat (-f bin). The headers and tables are a
// section of their own; each object is a section at its first page's place in the file (start=) whose
// addresses are the object's offsets (vstart=0), so a label is what a fix-up names. Every fix-up record's
// place is written as its label less the start of its page, every internal target as the label it names,
// every 32-bit offset the file stores at a fix-up's place (the target, as LINK386 and the driver linker
// leave it) as that label, and the fix-up page table as the distances of each page's records from the
// first. An object whose pages are not in the file in order, or not all there, is written as its pages'
// bytes in the order the file has them.
//
// Code is what is reached from the entry point, the device descriptor's control procedure, the entry
// table and every internal fix-up that names a place in an executable object; an instruction is a
// mnemonic where NASM gives back its bytes (checked each round from the assembler's map), its bytes
// otherwise, and always its bytes when a fix-up patches a place inside it (that place labelled).
// A W3 bundle (VMM32.VXD) is its header and each of its drivers, each written as above, its labels named
// by the driver.
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * An instruction is pe_writer::Insn (capstone's, with detail). A place is (object, offset): `Place`.
//   * An object's dict (the LE's own keys plus whole, start, size, bits) is `Obj`; a fix-up site's value (the
//     record's index, its width, the record) is `Site`, the record being named by its index in `Driver::records`.
//   * The "raw" set holds the instructions written as bytes: a Key (the driver's tag, object, offset).
//   * `Driver` is the Python's `_Driver`: it holds the file's bytes by reference (the Writer's), so a Writer is
//     not copied.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "codebreaker/formats/le.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "codebreaker/source/pe_writer.hpp"
#include "codebreaker/translate/l2.hpp"
#include "py.hpp"

namespace codebreaker::source::le_writer {

namespace le = codebreaker::formats::le;
using Insn = pe_writer::Insn;
using Place = std::tuple<int64_t, int64_t>;                       // (object, offset)
using Key = std::tuple<py::str, int64_t, int64_t>;                // (driver tag, object, offset)

extern const py::set<py::str> _STOP;
extern const py::set<int64_t> _SEGS;
extern const py::dict<int64_t, int64_t> _WIDTH;
extern const py::list<py::str> _PRELUDE;

// One object of the LE with what the writer knows of its place in the file.
struct Obj {
    le::Object o;
    bool whole = false;               // all its pages are in the file, in order
    std::optional<int64_t> start;     // where in the file (None unless whole)
    int64_t size = 0;
    int64_t bits = 0;                 // 16 or 32
};

// What a fix-up patches at one place: (index of its record, width).
struct Site {
    int64_t k = 0, w = 0;
};

// The target a relative branch names as its operand.
std::optional<int64_t> _target(const Insn& i);

// The instruction in NASM's spelling, or None where only its bytes will do.
std::optional<py::str> _nasm(const Insn& i, int64_t bits, const std::function<py::str(int64_t)>& ref);

// One LE image inside the file (the whole file, or one driver of a W3 bundle).
class Driver {
  public:
    const py::bytes& d;
    py::str tag;
    le::LEFile lf;
    py::list<std::tuple<int64_t, int64_t>> pm;
    int64_t last = 0;
    std::vector<Obj> objs;
    py::dict<int64_t, int64_t> by_idx;                             // object number -> its place in `objs`
    py::list<std::tuple<int64_t, le::FixupRecord>> records;
    py::dict<Place, Site> sites;
    py::dict<Place, Insn> insns;
    py::dict<Place, int64_t> covered;
    py::set<Place> starts;
    py::dict<Place, py::str> placed;

    Driver(const py::bytes& data, const std::shared_ptr<const py::bytes>& shared, int64_t lfa, const py::str& tag);

    int64_t _page_len(int64_t n) const;
    py::str name(int64_t oi, int64_t a) const;
    bool has_obj(int64_t oi) const { return by_idx.contains(oi); }
    const Obj& obj(int64_t oi) const { return objs[static_cast<size_t>(by_idx.at(oi))]; }
    void _decode();
    // Where each function begins, for the fragment each is in the file's L2 tree: the entry points and every place a
    // call lands -- a near call inside an object, or one into another object through a fix-up -- where an
    // instruction was found.
    py::set<Place> _function_starts(const py::list<Place>& entries) const;
};

class Writer {
  public:
    py::str path;
    std::shared_ptr<const py::bytes> data_;
    const py::bytes& d;
    bool w3 = false;
    std::vector<std::unique_ptr<Driver>> drivers;
    translate::l2::Dict l2;
    bool _mark_all = true;

    explicit Writer(const py::str& path);
    Writer(const Writer&) = delete;
    Writer& operator=(const Writer&) = delete;

    int64_t insn_count() const;
    void _place(Driver& dr, const py::set<Key>& raw);
    py::str ref(const Driver& dr, int64_t oi, int64_t a) const;
    std::tuple<py::str, py::dict<int64_t, Key>> source(const py::set<Key>& raw = py::set<Key>(), bool mark_all = true, bool exact = true);
    void _object(image_writer::Assembly& L, const Driver& dr, const Obj& o, const py::set<Key>& raw, py::dict<int64_t, Key>& lines_of) const;
    // How the bytes at a fix-up's place are written: the label of the target the file holds there.
    py::str _site_value(const Driver& dr, int64_t oi, int64_t at, const le::FixupRecord& rec, int64_t w, const py::bytes& b) const;
    py::list<py::str> _text(const Driver& dr, int64_t oi, const Insn& i, bool raw, int64_t bits) const;
    void _data(image_writer::Assembly& L, const Driver& dr, int64_t oi, int64_t base, int64_t a, int64_t end) const;
    // The fix-up records, page by page: each place as its label less its page's start, each internal target as its label.
    void _fixrec(image_writer::Assembly& L, const Driver& dr, const py::list<int64_t>& offs) const;
};

py::set<Key> _differing(const Writer& w, const py::bytes& data, const py::dict<py::str, std::tuple<int64_t, int64_t>>& syms, const py::set<Key>& raw);

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

Result reconstruct(const py::str& path, const py::str& outdir, const std::function<void(const py::str&)>& log = nullptr);

// python -m codebreaker le-source <driver> [--out DIR]
int64_t main(const py::list<py::str>& argv);

extern const py::str DOC;

}  // namespace codebreaker::source::le_writer
