// codebreaker/source/driver_writer.hpp -- typed C++ of codebreaker.source.driver_writer
//
// driver_writer.py -- a virtual device driver as assembly source that builds back into it.
//
//     driver_writer::Reconstruction r = driver_writer::reconstruct(path, outdir);
//     r.source, r.image, r.checks, r.same, r.raw, r.instructions, r.rounds
//
// A driver was written in assembly, so assembly is what its source is. What makes assembly of a driver SOURCE rather
// than a listing is that nothing in it is an address: every place the loader patches (a pointer to a variable, a call
// into another segment, an entry of a jump table) is written as the label it refers to, so the file can be assembled,
// linked at any address and changed. The image says exactly where those places are (its fix-up records).
//
// The file has one segment per object of the original, with the attributes that object had. Code is found by following
// it from the driver's entry points, from every place a call or jump across objects lands, and from pointers into code;
// everything not reached is written as data. Calls to the system and other drivers (`int 20h` followed by the service
// number) carry the service's name.
//
// An instruction is written as its mnemonic only when the assembler turns that text back into the bytes the original
// has -- which is checked, not assumed: every distinct instruction is first assembled on its own in a probe file; one
// that comes back different is written as its bytes, with the mnemonic beside it. Then the whole file is assembled and
// linked and each object compared with the original outside the patched places; an instruction at a place that still
// differs is written as bytes too, until nothing differs.
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * `_Object` is DriverObject, held by std::shared_ptr (the Python's objects are shared by name).  An instruction
//     (bytes, mnemonic, operands, signature, branch target or None) is `Insn`; `_one`'s answer ((insn, where control goes
//     next or None) or None) is std::optional<One>.  The memo of `_one` holds both a decoded answer and "nothing can be
//     decoded here" (None): std::optional<One> in a py::dict.
//   * The patched fields `fix` map an offset to (kind, target object, target offset).
//   * `platforms.decoder(32, detail=True)` (capstone) is the capstone.dll Python's package carries, loaded by path:
//     hand it to set_capstone_dll() once (a decode without it is an ImportError).
//   * `source`'s counts dict is `Counts`, its line map {source line: (object, offset)} a py::dict.  `self.l2` (the file
//     as one L2 tree, once written) is std::optional<translate::l2::Dict>; reconstruct writes it as module.l2.json
//     the way json.dump does (", " and ": " separators).
//   * reconstruct's result dict is the struct Reconstruction (keys the Python sets only on some paths are
//     std::optional).  `log` is a std::function taking the line.
//   * The struct.error a short window raises in `_one` (struct.pack_into) is raised as the runtime's py::StructError.
//   * The Python's main() prints through print(); here to stdout.  `_report` / `main` use the corpus's retail().
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/formats/le.hpp"
#include "codebreaker/source/assembly.hpp"
#include "codebreaker/translate/l2.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::driver_writer {

namespace le = codebreaker::formats::le;
using Place = le::Place;   // (object, offset)

// Names the capstone.dll to decode with (the one Python's capstone package loads).
void set_capstone_dll(const py::str& path);

extern const int64_t _MARK;          // stands in for a patched address while decoding
extern const int64_t _MAX_ROUNDS;
// opcodes common enough that one bad encoding does not mean every instance is bad; a rarer one the assembler rejects (a
// privileged or system instruction) is rejected wherever it appears, so all instances of it are written as bytes together
extern const py::set<py::str> _COMMON;

py::str _attributes(int64_t flags);

// one decoded instruction: (bytes, mnemonic, operands, "", branch target or None)
struct Insn {
    py::bytes data;
    py::str mnem, ops, sig;
    std::optional<int64_t> target;
};
// `_one`'s answer: the instruction, and where control goes on (the next offset, or None when it does not)
struct One {
    Insn ins;
    std::optional<int64_t> next;
};

struct DriverObject {
    int64_t i = 0, flags = 0, size = 0;
    py::bytes data;
    bool code = false, wide = false;         // wide: 32-bit code; otherwise real-mode start-up code
    py::set<int64_t> labels;
    py::dict<int64_t, std::tuple<int64_t, int64_t, int64_t>> fix;   // offset of a patched field -> (kind, target object, target offset)
    py::dict<int64_t, Insn> ins;             // offset -> decoded instruction (code only)
    py::dict<int64_t, std::tuple<int64_t, int64_t, bool>> services;   // offset of an `int 20h` -> (device, service, is a jump)
    py::bytes raw;
    py::dict<int64_t, std::optional<One>> memo;   // offset -> what decoding there gave (kept, so nothing is decoded twice)
    DriverObject(const le::LEFile& lf, const le::Object& obj);
};

// Is what was decoded really a piece of text? (a message table sits in an executable object, and letters decode as instructions)
bool _reads_as_text(const py::dict<int64_t, Insn>& found);

struct Counts {
    int64_t instructions = 0, raw = 0, data = 0;
};

class Writer {
  public:
    explicit Writer(const le::LEFile& lf);

    const le::LEFile* lf;
    std::optional<le::Descriptor> d;                                // `le.descriptor(lf) or {}`
    py::dict<int64_t, std::shared_ptr<DriverObject>> objects;
    py::dict<Place, py::str> names;
    std::optional<py::dict<Place, int64_t>> _within;
    std::optional<translate::l2::Dict> l2;                          // the file as one L2 tree, once written (source)

    // The name of a place. A place nothing can be written AT -- inside a patched field or an instruction, or outside its
    // object altogether -- is the nearest place before it that can, plus the distance.
    py::str label(int64_t o, int64_t a);
    // -- finding the code
    void _decode();
    // The instruction at `a`, decoded once; None when nothing can be decoded there.
    std::optional<One> _one(DriverObject& ob, int64_t a);
    // Decode from `start` until control cannot continue. -> ({offset: instruction}, [targets in this object], reached a clean end)
    std::tuple<py::dict<int64_t, Insn>, py::list<int64_t>, bool> _follow(DriverObject& ob, int64_t start);
    // -- writing
    // An instruction as the text an assembler takes, or None when it has no such text here (a service call, or more than
    // one patched field).
    std::optional<py::str> mnemonic(const DriverObject& ob, int64_t off);
    // An instruction as its bytes, each patched field as the label it holds.
    py::list<py::str> _raw(const DriverObject& ob, int64_t off);
    // {(object, offset): its label} of each place a function begins.
    py::dict<Place, py::str> function_starts();
    // -> (the assembly text, counts, {source line number: (object, offset)}). `good` is the set of (object, offset) of the
    // instructions to write as mnemonics; None writes every one that has a text.
    std::tuple<py::str, Counts, py::dict<int64_t, Place>> source(const std::optional<py::set<Place>>& good = std::nullopt);
    py::list<std::tuple<py::str, py::str>> classes() const;
    // -> the set of (object, offset) of instructions whose text assembles to the original's bytes.
    py::set<Place> probe(const py::str& outdir);
};

// Instructions written as mnemonics that did not come back as the original's bytes. -> (set of (object, offset), [objects that differ])
std::tuple<py::set<Place>, py::list<int64_t>> _differences(Writer& w, const le::LEFile& built, const py::set<Place>& good);
// Set the built file's LE module-flags field to the original's.
void _match_module_flags(const py::str& image, const le::LEFile& lf);
// Whether the LE has a non-resident name table inside its file.
bool _nonresident_names(const le::LEFile& lf);

// reconstruct()'s dict
struct Reconstruction {
    py::str source;
    std::optional<py::str> image;
    bool linked = false;
    int64_t rounds = 0;
    py::list<py::str> notes;
    std::optional<py::str> definitions;
    int64_t instructions = 0, raw = 0, data = 0;
    std::optional<py::dict<py::str, bool>> checks;
    std::optional<double> same;
    std::optional<py::list<int64_t>> differing_objects;
    std::optional<py::str> l2;
};
// Write a driver's source, build it, and make it come back as the original (see the top).
Reconstruction reconstruct(const py::str& module_path, const py::str& outdir, int64_t member = 0,
                           const std::function<void(const py::str&)>& log = nullptr);

bool _report(const py::str& name, const Reconstruction& r);

extern const py::str MAIN_DOC;
// python -m codebreaker driver-source <driver> [--out DIR]
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::source::driver_writer
