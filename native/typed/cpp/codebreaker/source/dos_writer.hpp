// codebreaker/source/dos_writer.hpp -- typed C++ of codebreaker.source.dos_writer
//
// dos_writer.py -- a real-mode program as assembly source that builds back into it.
//
//     python -m codebreaker dos-source <program> [--out DIR]
//
//     Result r = dos_writer::reconstruct(path, outdir);
//     r.source, r.image, r.same, r.notes, r.instructions, r.raw, r.rounds
//
// The real-mode base of the system was written in assembly, so assembly is what its source is.
// The file has one segment for every segment the program itself names (each word its relocation
// table marks holds one, and so do the entry point and the stack), and every such word is
// written as the segment it names (`dw seg S0605`), so the program can be assembled and linked
// at any address. Code is what analyze/dos_functions.py found by following the program; the rest
// is written as data, text as text. Interrupt calls carry the name of the service.
//
// What is NOT recovered, because a real-mode image does not state it: which data an offset
// inside a segment refers to. `mov ax, ds:[1234h]` stays a number, so data cannot be moved within
// its segment without changing those by hand.
//
// An instruction is written as its mnemonic only when the assembler turns that text back into the
// original's bytes (source/assembly.py checks each one); the file is then assembled and linked
// and the result compared with the original: every byte of the load module, the relocation
// table, the entry point and the stack. Kinds of file: MZ programs (unpacked first when the file
// stores them packed -- the rebuilt program is the unpacked one), .COM files, and device drivers
// that are plain images.
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * The dict every reconstruct path answers is the record `Result`. A key the Python sets only on some paths
//     is a std::optional (file, file_same, facts, equivalent, windows) or an empty list (remarks, notes); the
//     counts the Python reads back with .get(key, 0) start at 0. "source" is one path, or (for IO.SYS and the
//     driver+vxd file) a list of them: a std::variant. "parts" keeps what the Python reads of each part (`Part`),
//     which is a dos_writer result for a real-mode program and a driver_writer one for the VxD.
//   * An instruction (`Ins`) is the Python's 5-tuple (bytes, mnemonic, operands, target, relocated word at).
//   * `facts` (what laying the file out takes from the original, kept in <stem>.build.json) is the record `Facts`;
//     its "order" is the relocation offsets EXEPACK stored (a list of ints) or the table's (offset, segment)
//     pairs of any other file: a std::variant. The build.json is written and read by hand, as json.dump(indent=1)
//     writes it (py::json_parse reads it back).
//   * The capstone instruction (`pe_writer::Insn`) carries the groups and the first operand dos_writer reads.
//   * `log` (None or a callable) is a std::function, null for None.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

#include "codebreaker/analyze/dos_functions.hpp"
#include "codebreaker/build/definitions.hpp"
#include "codebreaker/build/exepack.hpp"
#include "codebreaker/formats/dos.hpp"
#include "codebreaker/source/assembly.hpp"
#include "codebreaker/translate/l2.hpp"
#include "py.hpp"

namespace codebreaker::source::dos_writer {

namespace dos = codebreaker::formats::dos;
namespace dos_functions = codebreaker::analyze::dos_functions;

extern const int64_t _MARK;           // stands in for a relocated segment word while decoding
extern const int64_t _MAX_ROUNDS;

// One instruction as the writer keeps it: (bytes, mnemonic, operands, target, relocated word at).
struct Ins {
    py::bytes data;
    py::str mnemonic;
    py::str ops;
    std::optional<int64_t> target;
    std::optional<int64_t> where;
};

class Writer {
  public:
    dos::Program p;
    py::bytes img;
    int64_t origin = 0;                       // a .COM file sits behind its program prefix
    py::set<int64_t> rel;
    bool flat = false;                        // no header of its own: the bytes as they load
    std::optional<int64_t> stack_in;          // the stack's segment, when it is in the load module
    int64_t top = 0;
    py::list<int64_t> segments;
    dos_functions::Analysis analysis;
    py::dict<int64_t, py::str> names;
    std::optional<int64_t> entry;
    py::set<int64_t> labels;
    py::dict<int64_t, Ins> ins;               // offset in the image -> the instruction
    py::dict<int64_t, py::str> calls;         // offset of an `int` -> its name
    py::set<int64_t> crossing;                // jumps across a place the program was divided at: written as bytes
    translate::l2::Dict l2;

    explicit Writer(const dos::Program& program);

    // A stretch of the program no relocated word divides can be longer than a segment may be (COMMAND.COM's
    // code is one such stretch of 75K): the original's segments were set up by the program itself, at run
    // time. For the assembler it is divided where it can be -- at a paragraph no jump or call reaches across
    // and no instruction lies over -- which changes no byte: everything inside refers to its own half by
    // label or by number.
    void _divide();
    int64_t segment_of(int64_t at) const;
    py::str label(int64_t at) const;
    void _decode();

    // text
    // A relocated word's value (relative to where the program is loaded) as an expression.
    py::str seg_name(int64_t value) const;
    py::str _seg_at(int64_t at) const;
    std::optional<py::str> mnemonic(int64_t at) const;
    py::list<py::str> _raw(int64_t at) const;

    struct Counts {
        int64_t instructions = 0, raw = 0, data = 0;
    };
    // -> (the file's text, counts)
    std::tuple<py::str, Counts> source(const std::optional<py::set<int64_t>>& good = std::nullopt);

    // The image as the pieces the source is made of, for assembly.culprits: each instruction written as its
    // mnemonic, each word that holds a segment (whatever it holds once built), and the runs of bytes between.
    py::list<assembly::Piece<int64_t>> pieces(const py::set<int64_t>& good) const;
    // {offset: (text, bytes outside the relocated word, mask)} of what can be probed.
    py::dict<int64_t, assembly::Candidate> candidates() const;
    py::list<py::str> link_lines(const py::str& obj) const;
};

// (bytes as loaded, relocation offsets, (cs, ip, ss, sp)) of a built file. `kind` is truthy for a header program:
// read plainly, as stored.
std::tuple<py::bytes, py::set<int64_t>, std::optional<std::tuple<int64_t, int64_t, int64_t, int64_t>>> _load_module(const py::str& path,
                                                                                                                  bool kind);

// What laying the file out takes from the original (kept in <stem>.build.json).
struct Facts {
    py::str file;
    std::optional<py::str> packer;
    bool overlay = false;
    std::optional<int64_t> min_alloc;
    std::optional<bool> full_last;
    std::optional<int64_t> words;
    // EXEPACK: the relocation offsets as it stored them; any other: (offset, segment) pairs
    std::variant<py::list<int64_t>, py::list<std::tuple<int64_t, int64_t>>> order = py::list<std::tuple<int64_t, int64_t>>();
    bool pklite_order = false;   // (PKLITE's facts name the keys in another order: its order comes before min_alloc)
};

// What the Windows module of a DOS+Windows file came to.
struct Windows {
    bool linked = false;
    int64_t functions = 0;
    py::list<py::str> stand_ins;
    py::list<py::str> undefined;
    py::list<build::definitions::Import> imports_extra;
    int64_t ran = 0, ran_same = 0;
};

// What one part of a combined file (IO.SYS's programs, a driver and its VxD) came to: what the Python reads of its result.
struct Part {
    py::str source;
    std::optional<py::str> image;
    bool linked = false;
    bool same = false;
    int64_t rounds = 0;
    int64_t instructions = 0, raw = 0, data = 0;
    py::list<py::str> notes;
};

// The dict reconstruct answers.
struct Result {
    std::variant<std::monostate, py::str, py::list<py::str>> source;
    std::optional<py::str> image;
    bool linked = false;
    bool same = false;
    int64_t rounds = 0;
    py::list<py::str> notes;
    py::str kind;
    int64_t instructions = 0, raw = 0, data = 0;
    py::dict<int64_t, py::ref<dos_functions::Function>> functions;
    std::vector<std::pair<py::str, Part>> parts;
    py::list<py::str> remarks;
    std::optional<Facts> facts;
    std::optional<bool> equivalent;
    std::optional<Windows> windows;
    std::optional<py::str> file;
    std::optional<bool> file_same;

    // "%s" % r["source"]
    py::str source_text() const;
};

// Write a program's source, build it, and make it come back as the original (see the top).
Result reconstruct(const py::str& module_path, const py::str& outdir, const std::function<void(const py::str&)>& log = nullptr);

// A DOS program and a Windows module in one file (REGEDIT.EXE, XCOPY32.EXE, UNINSTAL.EXE): the DOS program written and
// built here (as a file laid out as the original's stub), the Windows module built whole by build/module.py (its own
// reconstruction, folder windows/), and the module put behind the program as the original has it (build/stubjoin.py).
Result _dos_and_windows(const py::str& module_path, const py::str& outdir, const std::function<void(const py::str&)>& log = nullptr);

// A real-mode driver and a VxD in one file (DRVSPACE.BIN): the driver written and built here, the VxD by the driver
// lane (source/driver_writer.py), and the file put together as the original is -- its MZ header (<stem>.hdr), the
// driver, the VxD laid out as Microsoft's LINK lays one out (build/vxdfile.py; its description and exports from
// <vxd>.def), its version resource (<vxd>.ver) and what the original carries behind that (<stem>.tail).
Result _driver_and_vxd(const py::str& module_path, const py::str& outdir, const std::function<void(const py::str&)>& log = nullptr);

py::str _fills_text(const build::exepack::Layout& layout);
py::list<build::exepack::Fill> _read_fills(const py::str& path);

// IO.SYS: each of its programs written, built and compared as any other (one source file each); SYSINIT then packed as
// LINK /EXEPACK stored it (build/exepack.py, the fills listed in sysinit.pack), and loader + kernel + packed SYSINIT
// joined into the file.
Result _kernel(const py::str& module_path, const py::str& outdir, const std::function<void(const py::str&)>& log = nullptr);

// A file kept beside the source: written from `make()` the first time, read after that (so an edited one is what is
// built). -> its bytes
py::bytes _beside(const py::str& path, const std::function<py::bytes()>& make);

// The built program as a file laid out as the original is (build/mzfile.py): its header's own data (<stem>.hdr), the load
// module -- packed as LINK /EXEPACK stored it when the original is (build/exepack.py: the fills in <stem>.pack, the
// unpacker from exepack.asm) -- and the bytes behind the load module (<stem>.ovl). result.file, result.file_same.
void _as_file(const dos::Program& p, const py::str& outdir, const py::str& stem, const py::str& linked, Result& result,
              const std::function<void(const py::str&)>& log = nullptr);

// What laying the file out takes from the original: written to the side files (<stem>.hdr, .ovl, .pack, the unpacker's own
// folder) and returned for <stem>.build.json.
Facts _facts(const dos::Program& p, const py::bytes& data, const py::str& outdir, const py::str& stem,
             const std::function<void(const py::str&)>& log = nullptr);

// The file, from what was linked (exe) and the folder: header data, packing, overlay.
py::bytes _compose(const py::str& outdir, const py::str& stem, const py::bytes& exe, const Facts& facts, Result& result);

void _as_pklite_file(const dos::Program& p, const py::bytes& data, const dos::Mz& oh, const py::bytes& exe, const dos::Mz& h,
                     const py::bytes& module, const py::str& outdir, const py::str& stem, const py::str& out, Result& result,
                     const std::function<void(const py::str&)>& log = nullptr);
py::bytes _compose_pklite(const py::str& outdir, const py::str& stem, const py::bytes& exe, const Facts& facts, Result& result);

// One real-mode program: its source written, assembled, linked and compared, round after round, until it builds back as
// the original (see the top).
Result _program(const dos::Program& p, const py::str& outdir, const py::str& stem, const std::function<void(const py::str&)>& log = nullptr);

// IO.SYS from its folder: its four programs built, SYSINIT packed, the three joined.
py::str _rebuild_kernel(const py::str& srcdir, const std::function<void(const py::str&)>& log = nullptr);

// Make the program from its folder alone (what dos-source wrote: <stem>.asm, .lnk, .build.json and the side files):
// assemble, link, set the header's entry and stack where the source cannot say them, lay the file out. -> the path of
// the file made. `desc`: the name of the .build.json to read (default: the first the folder has).
py::str rebuild(const py::str& srcdir, const std::function<void(const py::str&)>& log = nullptr,
                const std::optional<py::str>& desc = std::nullopt);

// python -m codebreaker dos-source <program> [--out DIR]
extern const py::str MAIN_DOC;
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::source::dos_writer
