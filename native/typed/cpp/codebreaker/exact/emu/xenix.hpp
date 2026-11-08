// codebreaker/exact/emu/xenix.hpp -- C++ port of codebreaker.exact.emu.xenix.
//
// xenixrun.py - run a program of XENIX for the 8086 (Altos ACS 8600, 1982-83) under emulation.
//
//   python xenixrun.py ROOT WORKDIR PROGRAM [ARG ..]
//
// Microsoft's first C linker and librarian for MS-DOS (LINK 2.40, LIB 2.40, 1983-84) were not
// compiled on MS-DOS: their code is that of the C compiler of XENIX for the 8086 - the Portable C
// Compiler line: /lib/cpp, /lib/c0, /lib/c1, /lib/c2, /bin/as, /bin/ld. A XENIX system with that
// compiler survives for the Altos ACS 8600 (Bitsavers; binaries/altos-8600-xenix, git-ignored).
// This runs one of its programs, the way dosrun.py runs an MS-DOS one and isis.py an ISIS-II one:
// enough of the machine and of the system to compile a file.
//
// The program file is a Version 7 a.out for the 8086: 16 bytes of header (magic 0109h: text and data
// in segments of their own; 0107h: in one), text, data. A system call is `out 8800h, ax` - the
// Altos traps the port - with the call's number in AX and its arguments in BX, CX, SI, DI; the
// result comes back in AX (and BX), with the carry flag set and an error number in AX if it failed.
// The numbers are those of Version 7 UNIX.
//
// ROOT is the directory holding the XENIX files (lib/c0 ...); a path the program names is looked
// for in WORKDIR first (so /tmp and the files being compiled live there), then in ROOT.
//
// Unicorn: the Python imports the `unicorn` package. Here the engine is Unicorn's C API in the
// unicorn.dll that package carries; nothing of it is linked, the caller hands over the DLL's path once
// (load_unicorn) -- the way native/formats and native/dosexec do. A run without it raises RuntimeError.
#pragma once

#include <functional>
#include <ostream>
#include <variant>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::emu::xenix {

// SystemExit(message): a program that is not an a.out file.
using py::SystemExit;
// unicorn.UcError: the engine refused (a memory access outside what is mapped ...).
struct UcError : py::Exception {
    explicit UcError(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "UcError"; }
};
// next(...) of an exhausted generator: no free file descriptor.
using py::StopIteration;

extern const int64_t CS;
extern const int64_t DS;
// (ENOENT EBADF ENOTTY EINVAL carry an underscore: <errno.h> defines macros of those names)
extern const int64_t ENOENT_;
extern const int64_t EBADF_;
extern const int64_t ENOTTY_;
extern const int64_t EINVAL_;

// A system call as traced: (number, BX, CX, SI, DI).
using Call = std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t>;

class Result {
  public:
    py::bytes console;                  // output of fd 1 and 2
    std::optional<int64_t> status;      // exit status (None: it did not exit)
    py::list<Call> calls;               // the traced system calls
    std::optional<py::str> error;       // why the run stopped short, or None

    Result() = default;
};

// The emulated machine, as a hook sees it (the Python hands over the unicorn object). Register ids are
// Unicorn's own (UC_X86_REG_*); memory is the machine's linear space.
class Machine {
  public:
    virtual ~Machine() = default;
    virtual int64_t reg_read(int64_t reg) = 0;
    virtual void reg_write(int64_t reg, int64_t value) = 0;
    virtual py::bytes mem_read(int64_t address, int64_t size) = 0;
    virtual void mem_write(int64_t address, const py::bytes& data) = 0;
    virtual void emu_stop() = 0;
};

// A function called with the machine and the two segments each time the instruction at its
// address is about to be done.
using Hook = std::function<void(Machine&, int64_t cs, int64_t ds)>;

// An argument of the program: text (encoded as UTF-8) or bytes.
using Arg = std::variant<py::str, py::bytes>;

// Hand over the path of unicorn.dll (the one Python's `unicorn` package carries). RuntimeError if it
// cannot be loaded.
void load_unicorn(const py::str& dll_path);

// -> Result (console output of fd 1 and 2, exit status)
Result run(const py::str& root, const py::str& work, const py::str& prog, const py::list<Arg>& args,
           const py::bytes& stdin_ = py::bytes(), bool trace = false, int64_t limit = 400000000,
           const py::dict<int64_t, Hook>& hooks = py::dict<int64_t, Hook>());

// The command line: argv as sys.argv is (the program's name first). Writes to `out`.
void main(const py::list<py::str>& argv, std::ostream& out);

}  // namespace codebreaker::exact::emu::xenix
