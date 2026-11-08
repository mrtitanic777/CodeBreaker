// codebreaker/formats/dos.hpp -- typed C++ of codebreaker.formats.dos
//
// dos.py -- the ONE reader for real-mode programs: what DOS, or the boot sector, would load.
// Answered by the C++ (native/formats/dos: formats.dll); its Python reader is gone (docs/NATIVE.md, step 4).
//
//     namespace dos = codebreaker::formats::dos;
//     dos::kind(path)        "mz" | "com" | "driver" | "kernel" | None   (None: not a program -- e.g.
//                            COUNTRY.SYS and the LOGO*.SYS pictures carry a program's extension only)
//     dos::packer(path)      "EXEPACK" | "PKLITE" | "LZEXE" | "DIET" | "UPX" | "moves itself" | None
//     Program p = dos::read(path)   a Program: the bytes as they sit in memory once loaded (and unpacked)
//     dos::read(path, false) a program that moves itself read as it is stored: the
//                            moving is its own code, which its source has to have
//     dos::programs()        every real-mode program of the corpus, one path per name
//     dos::unpacked_file(p)  a packed program written back as a plain MZ file (`info --unpack`)
//
// Four kinds of file, one result:
//
//   mz      a header, a relocation table, a load module, sometimes an overlay behind it. The load
//           module goes to segment LOAD and every relocation (a word holding a segment) has LOAD
//           added. A load module stored compressed is unpacked first, so the Program holds the
//           program and not its unpacker: EXEPACK statically (a backwards run-length scheme with its
//           own relocation table), anything else by running the unpacker (in formats.dll, on Unicorn).
//           A program that is not compressed but starts by moving itself (COMMAND.COM) is run the
//           same way, up to where it continues in the moved copy.
//   com     no header: the file goes to LOAD:0100 behind a 256-byte program prefix, entry there.
//   driver  a device driver: a chain of device headers (next, attributes, strategy routine,
//           interrupt routine, name) at offset 0 -- of the file, or of an MZ load module.
//   kernel  IO.SYS / WINBOOT.SYS, three programs in one file: the first sectors are the loader the
//           boot sector starts at 0070:0200 (it moves itself to the top of memory); what follows is
//           the BIOS and DOS kernel, which the loader reads to 0070:0000; and the MZ header describes
//           only the last part, SYSINIT, with a "header size" that skips the other two.
//
// A Program's `image` is one flat stretch of real-mode memory starting at linear address `base`
// (linear = segment * 16 + offset); `spans` says which parts of it hold bytes from the file.
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * The readers are the C++ of native/formats/dos (dos.cc, unpack.cc, with core/bytes.cc, core/text.cc,
//     core/files.cc, core/uc.cc): linked, and called directly with typed results.  What the Python got
//     back as pickled values (dicts with fixed keys, tuples) these functions return as the structs / tuples
//     below.  An exception the reader raised comes back as the py:: exception of the same Python type (an
//     "UcError"/"ImportError" the Python bridge turned into RuntimeError is a RuntimeError here too).
//   * The unpacker runs on Unicorn, by the unicorn.dll the Python package carries: hand its path to
//     load_unicorn() once (as the xenix / dpmi ports do); a run without it is a RuntimeError.
//   * `Mz` is the dict _mz() answers (nreloc, header, min_alloc, max_alloc, ss, sp, ip, cs, reloc_off, size).
//   * `Device` and `Program` are the Python classes.  Program attributes the C++ sets only sometimes
//     (windows_at; in_kernel, which kernel_parts sets) are std::optional / bool defaults of "absent".
//     `ds` is std::optional<int64_t> (the data segment register; None when no `mov ds` is found).
//   * programs()' `extensions` default is the module's EXTENSIONS.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

#include "codebreaker/build/exepack.hpp"

namespace codebreaker::formats::dos {

extern const int64_t LOAD;                // the segment a program is loaded at
extern const int64_t KERNEL_SEGMENT;      // where the boot sector and the loader put IO.SYS
extern const int64_t LOADER_SEGMENT;      // where the loader moves itself with 640 KB of memory
extern const py::list<py::str> EXTENSIONS;
extern const int64_t EXEPACK_SEGMENT;     // where the EXEPACK unpacker's code is read (it runs behind SYSINIT)

// Unicorn's DLL (the one Python's `unicorn` package carries), handed over once: the unpackers need it.
void load_unicorn(const py::str& dll_path);

// The fields of an MZ header (_mz).  `size` is the end of the load module in the file.
struct Mz {
    int64_t nreloc = 0, header = 0, min_alloc = 0, max_alloc = 0, ss = 0, sp = 0, ip = 0, cs = 0, reloc_off = 0, size = 0;
};

// One device header of a driver.
class Device {
public:
    int64_t offset;
    std::optional<int64_t> next;      // the link to the next header (None: the chain ends)
    int64_t attributes, strategy, interrupt;
    py::str name;
    bool character;

    Device(int64_t offset, const std::optional<int64_t>& nxt, int64_t attributes, int64_t strategy, int64_t interrupt,
           const py::str& name);
    py::str repr() const;             // Device('NAME' at 0x0, strategy 0x0, interrupt 0x0)
};

using Span = std::tuple<int64_t, int64_t, py::str>;               // (linear address, length, label)
using EntryPoint = std::tuple<int64_t, py::str, py::str>;         // (linear address, name, "far" | "near")

// A real-mode program as loaded. See the top.
class Program {
public:
    py::str path, kind, name;
    std::optional<py::str> packer;    // how the load module was stored
    std::optional<py::str> unpacked;  // "static" | "emulated" when it was unpacked
    int64_t base;
    py::bytes image;
    py::list<Span> spans;             // which parts of the image hold bytes from the file
    py::list<EntryPoint> entries;
    py::dict<int64_t, int64_t> segments;   // entry address -> the code segment it runs with
    py::list<int64_t> relocations;    // linear addresses of words that hold a segment
    py::list<Device> devices;         // offsets relative to their segment
    int64_t device_segment;
    int64_t cs = 0, ip = 0, ss = 0, sp = 0;
    std::optional<int64_t> ds;        // the data segment register while the program runs
    py::bytes overlay;                // bytes of the file behind the load module
    py::list<py::str> notes;
    std::optional<int64_t> windows_at;   // set by the reader of a DOS program with a Windows module behind it
    bool in_kernel = false;           // SYSINIT of kernel_parts: built into IO.SYS, not a file of its own

    Program(const py::str& path, const py::str& kind);
    bool inside(int64_t linear) const;
    std::optional<int64_t> word(int64_t linear) const;
};

// ---- what a file is
// The fields of an MZ header, or None. A Windows module's DOS stub is None unless `stub`.
std::optional<Mz> _mz(const py::bytes& data, bool stub = false);

// The device headers chained from offset `at` of one segment's bytes. [] when what is there
// is not a device header: the routines must lie inside the image, a character device must
// have a printable name, and the chain must end in a link of offset FFFF.
py::list<Device> devices(const py::bytes& image, int64_t at = 0, const py::list<int64_t>& segments = py::list<int64_t>{0, 0xFFFF},
                         int64_t limit = 64);

// Where the BIOS/DOS part of an IO.SYS starts in the file: a sector boundary holding a jump
// followed at once by the chain of built-in devices (CON, AUX, PRN, ...) whose links name
// KERNEL_SEGMENT. None when the file is not laid out that way.  -> (start, first)
std::optional<std::tuple<int64_t, int64_t>> _kernel_body(const py::bytes& data, const Mz& h);

std::optional<py::str> _kind(const py::bytes& data, const py::str& name = py::str(""));

// Programs stored one after another in one file, each with its own MZ header: [(offset,
// bytes)], or [] for one alone. CodeView 4.1's CV.EXE is three -- a stub that checks the CPU and
// memory, the DOSX16 extender, and CodeView itself (448 KB, run in protected mode) -- and only
// the stub was taken apart: 7 functions of a debugger. (A program's overlays, stored after it the
// same way, come out as parts too.) Each part starts at the next byte, or after up to 15 bytes
// of zeros to a paragraph.   (the C++ answers each part as (offset, end))
py::list<std::tuple<int64_t, int64_t>> chain(const py::bytes& data);

// A real-mode device driver and a VxD in one file (DRVSPACE.BIN): an MZ header whose load
// module is the driver -- device headers at its start -- and whose new-header field points at
// an LE behind it, the VxD. -> (header size, LE offset), or None.
std::optional<std::tuple<int64_t, int64_t>> driver_and_vxd(const py::bytes& data);

// A DOS program and a Windows module in one file (REGEDIT.EXE, XCOPY32.EXE, UNINSTAL.EXE):
// the module's DOS stub is itself a real program -- run from DOS, it does the work there.
// -> (MZ fields, offset of the module's new header), or None.
std::optional<std::tuple<Mz, int64_t>> dos_and_windows(const py::bytes& data);

// The real-mode driver of a driver+vxd file, as a Program (a driver image).
Program driver_part(const py::str& path);

// What kind of real-mode program a file is, or None. See the top.
std::optional<py::str> kind(const py::str& path);

std::optional<py::str> _packer(const py::bytes& data, const Mz& h);

// How an MZ program's load module is stored compressed, or None when it is not (or the
// scheme is not one this knows by its marks).
std::optional<py::str> packer(const py::str& path);

// One path per program name of the corpus that kind() accepts, by name.
py::list<py::str> programs(const py::list<py::str>& extensions = EXTENSIONS);

// ---- EXEPACK
// (load module, [relocation offsets], cs, ip, ss, sp)
using Unpacked = std::tuple<py::bytes, py::list<int64_t>, int64_t, int64_t, int64_t, int64_t>;

// Undo EXEPACK on a load module. -> (load module, [relocation offsets], cs, ip, ss, sp)
//
// The packed module ends in a small segment (the entry's CS): a header of words -- the real
// IP, CS, (scratch), this segment's size, (in the 20-byte header one more word), the real SP,
// SS, the unpacked size in paragraphs, (in the 18- and 20-byte headers a padding count), and
// "RB" -- then the unpacker, its error message, and the relocation table. The data before it
// is read backwards from its end: skip 0xFF padding, then commands of
// a byte and a length word -- B0 fills `length` bytes with the byte before it, B2 copies
// `length` bytes -- written backwards from the unpacked size; bit 0 marks the last command.
// The relocation table is sixteen lists, one per 64 KB (segment 0x0000, 0x1000, ...), each a
// count and that many offsets.
Unpacked unpack_exepack(const py::bytes& module, int64_t cs, int64_t ip);

// Run the unpacker of a packed load module to the real entry point, in the emulator (twice, at
// two load segments: words that differ by the distance are the relocations). With `moved`, run a
// program that only moves itself until it continues in the moved copy. -> (load module with no
// load segment applied, [relocation offsets], cs, ip, ss, sp), the segments relative to the load
// segment. RuntimeError when no real entry point is reached or the two runs do not agree.
Unpacked unpack_emulated(const py::bytes& module, const py::list<int64_t>& relocations, int64_t cs, int64_t ip, int64_t ss,
                         int64_t sp, bool moved = false);

// A Program read from a packed MZ file, written back as a plain MZ file that DOS runs the
// same way: the unpacked load module with its relocation table (grouped per 64 KB, as EXEPACK
// kept it), the real entry and stack, room above the image for the stack, and whatever the file
// held behind its load module. -> bytes
py::bytes unpacked_file(const Program& p);

// The data segment a program sets up: the first `mov ds, reg` after its entry point, when
// the register was loaded with a relocated constant or copied from CS just before.
std::optional<int64_t> _data_segment(const Program& p, int64_t entry);

// Read a real-mode program. -> Program. ValueError when the file is not one. With
// run_movers False a program that moves itself is read as stored (see the top).
Program read(const py::str& path, bool run_movers = true);

// IO.SYS as the programs it is made of, each where it runs. -> (parts, layout):
// parts [(stem, Program)] -- the loader and the BIOS + DOS kernel, stored in the file as they
// run; SYSINIT as the MZ program it is (unpacked: the file stores it as LINK /EXEPACK did);
// and the EXEPACK unpacker's code -- and layout, how SYSINIT was packed (build/exepack.Layout).
// The file is: loader, kernel, SYSINIT packed. The loader's first bytes are the MZ header that
// describes the packed SYSINIT; to the loader they are data.
std::tuple<py::list<std::tuple<py::str, Program>>, build::exepack::Layout> kernel_parts(const py::str& path);

// PKLITE's entry code and decompressor (the first `start` bytes of the load module it
// stored) as a program of its own: it runs at the start of the program, in the program prefix's
// segment, then copies its decompressor to the top of memory.
Program pklite_head(const py::str& path, int64_t start);

// The EXEPACK unpacker's code (build/exepack.Layout.stub_code) as a program of its own:
// LINK put the same one behind every program it packed.
Program exepack_stub(const py::str& path, const build::exepack::Layout& layout);

}  // namespace codebreaker::formats::dos
