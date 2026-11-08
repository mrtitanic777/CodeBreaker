// codebreaker/exact/emu/dos.hpp -- C++ port of codebreaker.exact.emu.dos.
//
// dos.py - run a real-mode DOS program under Unicorn, in this process, with just enough of DOS behind
// INT 21h for the compilers, assemblers and linkers of the 1980s: files (handles and FCBs), memory
// blocks, console, EXEC, version, time. (From Microsoft-Assemblers-Compilers research/masm-1.00/
// reversing/tools/dosrun.py, which ran every period tool of those rebuilds.)
//
// It is an ORACLE and a TRACER, not part of the reconstruction: the original binary assembles real
// source here, and every basic block it executes is recorded, so the reconstruction can be checked
// against behaviour (which function runs for which statement; what the output is).
//
// As a module:  r = run(exe, workdir, cmdline) -> Result(exit, console, blocks, load_seg, files, steps, calls)
//
// `blocks` is the set of linear addresses of executed basic blocks; (addr - load_seg*16) is an
// offset into the program's load image (for an EXEPACK'd program: into the UNPACKED image, since
// the unpacker runs here too and unpacks in place).
//
// What is ported and how.  In Python, run() first offers a run to the C++ executive
// (native_dosexec.py -> native/dosexec); when that is not possible it runs the Python executive
// itself.  Here the C++ executive IS the executive: run() calls native_dosexec::run directly (the
// native/dosexec sources are linked in, no DLL, no ctypes).  A run the Python executive alone could
// do -- one with Python hooks of its own (instrument, instrument_when, driver) or a DPMI host object
// handed in by the caller -- has no C++ counterpart (those are Python callables over a Python
// `Uc`); run() takes none of them.  The MemFS, the arena (MCB) helpers, Result and the constants
// are ported whole.
#pragma once

#include <optional>
#include <tuple>

#include "codebreaker/exact/emu/dpmi.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::emu::dos {

inline constexpr int64_t PSP = 0x0100;   // PSP segment; the program loads at PSP + 0x10 = 0x0110
inline constexpr int64_t TOP = 0xA000;   // first segment past conventional memory
// (seconds a fast run may take: one that never ends); DOSRUN_FAST_LIMIT, read once as at import
extern const double FAST_LIMIT;
// int(os.environ.get("DOSRUN_MAX_BLOCKS", "60000000")), read once as at import: run's default max_blocks
extern const int64_t DEFAULT_MAX_BLOCKS;

// Result = collections.namedtuple("Result", "exit console blocks load_seg files steps calls")
struct Result {
    std::optional<int64_t> exit;
    py::bytes console;
    py::set<int64_t> blocks;
    int64_t load_seg = 0;
    std::optional<py::dict<py::str, py::bytes>> files;   // (always None)
    int64_t steps = 0;
    py::dict<int64_t, int64_t> calls;                    // collections.Counter: address -> count
};

// ---- the DOS memory arena, kept in guest RAM the way DOS keeps it -------------------------------
// Every block is preceded by a 16-byte arena header (memory control block) one paragraph before it:
//   +0 'M' (another block follows) or 'Z' (the last), +1 owner PSP (0 free, 8 DOS), +3 size in
//   paragraphs (not counting the header), +8 name. The next header is at mcb + 1 + size. Programs read
//   this chain themselves (a DOS extender's loader walks it from its PSP - 1 to size the memory it may
//   take), so it is the one source of truth: INT 21h 48h/49h/4Ah work on it directly, as DOS does.
inline constexpr int64_t ENVSEG = 0x00C0;     // the environment block (its header at ENVSEG - 1 is the first in the chain)
inline constexpr int64_t DOSSEG = 0x0080;     // INT 21h 52h's list of lists (first MCB at -2)
inline constexpr int64_t LOL_OFF = 0x0026;
inline constexpr int64_t LOL_SIZE = 0x40;
inline constexpr int64_t MCB_FREE = 0;
inline constexpr int64_t MCB_DOS = 8;

// a DOS memory error: .code is the DOS error code (7 arena trashed, 9 bad block).
using ArenaError = codebreaker::exact::emu::dpmi::ArenaError;

// (mcb segment, sig, owner, size)
using Mcb = std::tuple<int64_t, int64_t, int64_t, int64_t>;

void _mcb_write(dpmi::Engine& mu, int64_t seg, const py::bytes& sig, int64_t owner, int64_t size);
void _mcb_set(dpmi::Engine& mu, int64_t seg, const std::optional<py::bytes>& sig = std::nullopt,
              const std::optional<int64_t>& owner = std::nullopt, const std::optional<int64_t>& size = std::nullopt);
// -> (sig byte, owner, size) of the header at `seg`; ArenaError(7) if it is not one.
std::tuple<int64_t, int64_t, int64_t> _mcb_read(dpmi::Engine& mu, int64_t seg);
// -> [(mcb segment, sig, owner, size)], the whole arena from `first` to the 'Z' header.
py::list<Mcb> mcb_chain(dpmi::Engine& mu, int64_t first);
// join each run of free blocks into one (DOS does this before it allocates or resizes).
void _mcb_coalesce(dpmi::Engine& mu, int64_t first);
int64_t _mcb_largest(dpmi::Engine& mu, int64_t first);
// block at header `seg` (size paragraphs) keeps `keep`; the rest becomes a free block behind it.
void _split(dpmi::Engine& mu, int64_t seg, int64_t sig, int64_t size, int64_t keep);
// -> the new block's segment, or None when no free block is large enough. The strategy is DOS's
// (INT 21h 58h): 0 first fit, 1 best fit (the smallest that fits), 2 last fit (the top of the last).
std::optional<int64_t> _mcb_alloc(dpmi::Engine& mu, int64_t first, int64_t paras, int64_t owner, int64_t strategy = 0);
// free the block at segment `block` (INT 21h 49h).
void _mcb_free(dpmi::Engine& mu, int64_t first, int64_t block);
// resize the block at segment `block` (INT 21h 4Ah) -> None, or the most it could have (error 8).
std::optional<int64_t> _mcb_resize(dpmi::Engine& mu, int64_t first, int64_t block, int64_t paras);

// a file of a MemFS: the operations the DOS calls use
class MemFile {
  public:
    explicit MemFile(const py::bytearray& buf) : buf(buf) {}
    py::bytearray buf;
    int64_t pos = 0;

    py::bytes read(std::optional<int64_t> n = std::nullopt);   // n None or negative: to the end
    int64_t write(const py::bytes& d);
    int64_t seek(int64_t off, int64_t whence = 0);
    int64_t tell();
    void truncate(std::optional<int64_t> size = std::nullopt);
    void flush();
    void close();
};

// the files a run sees, kept in memory: what the program writes stays here (its passes share them), what it
// reads and was not written comes from the disk - read once, kept for every run after (DISK) - except under the
// folders hidden (an include folder given whole each run). No file is written to the disk
class MemFS {
  public:
    // (path -> (mtime, size, bytes)) - the tools' own files
    static py::dict<py::str, std::tuple<double, int64_t, py::bytes>>& DISK();

    py::dict<py::str, py::bytearray> files;
    py::set<py::str> gone;
    py::list<py::str> hide;

    explicit MemFS(const py::list<py::str>& hide = py::list<py::str>());

    py::str key(const py::str& p) const;
    bool _hidden(const py::str& k) const;
    py::bytes disk(const py::str& p) const;
    bool isfile(const py::str& p) const;
    int64_t getsize(const py::str& p) const;
    py::bytes read(const py::str& p) const;
    // mode: "w" in it truncates; otherwise the file is brought in from the disk (FileNotFoundError if absent)
    MemFile open(const py::str& p, const py::str& mode = py::str("rb")) const;
    void put(const py::str& p, const py::bytes& b) const;
    void remove(const py::str& p) const;
    void replace(const py::str& a, const py::str& b) const;
};

// open(), retried: on a busy Windows machine (virus scanner, indexer) a file just created or
// closed can refuse to open for a moment (EINVAL / EACCES). A failed open here would reach the
// emulated program as a DOS error and change what it does, so wait instead of reporting it.
py::file ropen(const py::str& path, const py::str& mode = py::str("rb"));

// dos.run(exe, workdir, cmdline, stdin, max_blocks, trace_calls, env, psp, top, fast, fs, dpmi)
//   fs: a MemFS or nullptr (the disk itself); dpmi: a DPMI host is built for the run;
//   fast: no hook on every basic block (the blocks are not counted or kept - Result.blocks is empty); a run
//   that does not end is stopped by a time limit instead (FAST_LIMIT seconds).
// Raises what dos.run raises (RuntimeError, AssertionError, OSError ..., dpmi::UcError).
Result run(const py::str& exe, const py::str& workdir, const py::str& cmdline = py::str(""),
           const py::bytes& stdin_ = py::bytes(std::string()), int64_t max_blocks = DEFAULT_MAX_BLOCKS,
           bool trace_calls = false, const py::bytes& env = py::bytes(std::string("PATH=\0", 6)),
           std::optional<int64_t> psp = std::nullopt, std::optional<int64_t> top = std::nullopt, bool fast = false,
           const MemFS* fs = nullptr, bool dpmi = false);

// `python dos.py <EXE> <workdir> [cmdline]`: the text it prints (console, then the summary line).
py::str main_(const py::list<py::str>& argv);

}  // namespace codebreaker::exact::emu::dos
