// cb/exact/emu/xenix.hpp -- run a program of XENIX for the 8086 (Altos ACS 8600, 1982-83) under emulation.
//
// Microsoft's first C linker and librarian for MS-DOS (LINK 2.40, LIB 2.40, 1983-84) were not compiled on MS-DOS:
// their code is that of the C compiler of XENIX for the 8086 - the Portable C Compiler line: /lib/cpp, /lib/c0,
// /lib/c1, /lib/c2, /bin/as, /bin/ld. A XENIX system with that compiler survives for the Altos ACS 8600 (Bitsavers;
// binaries/altos-8600-xenix, git-ignored). This runs one of its programs, the way the DOS executive runs an MS-DOS
// one and isis an ISIS-II one: enough of the machine and of the system to compile a file. The machine is Unicorn
// (the prebuilt unicorn.dll beside the program, cb/core/unicorn_api.hpp).
//
// The program file is a Version 7 a.out for the 8086: 16 bytes of header (magic 0109h: text and data in segments of
// their own; 0107h and 0108h: in one), text, data. A system call is `out 8800h, ax` - the Altos traps the port - with
// the call's number in AX and its arguments in BX, CX, SI, DI; the result comes back in AX (and BX), with the carry
// flag set and an error number in AX if it failed. The numbers are those of Version 7 UNIX. A program of XENIX 2.3
// instead starts with a jump over a row of `jmp .`, and makes a system call by a far call to the one at 0002 (a far
// call to the one at 0004 asks for the stack to be grown down to BX); that is caught too.
//
// `root` is the directory holding the XENIX files (lib/c0 ...); a path the program names is looked for in `work`
// first (so /tmp and the files being compiled live there), then in `root`. A path that names a place outside both
// ("..", a drive, a backslash, a Windows device name) is not found. Files are kept in memory while a program has
// them open and written back at close (and before any call that looks at the host by name), and when the run ends.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::exact::emu::xenix {

inline constexpr std::uint16_t kCodeSegment = 0x1000;   // where the text is loaded
inline constexpr std::uint16_t kDataSegment = 0x3000;   // where the data is, for a program with a segment of its own for it

// The registers a hook can read and write. (Flags is the whole EFLAGS; the carry flag is bit 0.)
enum class Reg { Ax, Bx, Cx, Dx, Si, Di, Bp, Sp, Ip, Cs, Ds, Es, Ss, Flags };

// The emulated machine, as a hook sees it. Addresses are linear (segment * 16 + offset) in the machine's 1 MiB.
// Reads and writes of memory that is not there throw EmulationError.
class Machine {
  public:
    virtual ~Machine() = default;
    virtual std::uint32_t reg(Reg r) const = 0;
    virtual void set_reg(Reg r, std::uint32_t value) = 0;
    virtual Bytes read(std::uint32_t address, std::size_t size) const = 0;
    virtual void write(std::uint32_t address, ByteView data) = 0;
    virtual void stop() = 0;
};

// Called, with the machine and the two segments, each time the instruction at its address (an offset in the text) is
// about to be done: to watch a program at work, or to change what it does by changing its registers.
using Hook = std::function<void(Machine&, std::uint16_t cs, std::uint16_t ds)>;

// A system call as traced.
struct SysCall {
    std::uint16_t number = 0, bx = 0, cx = 0, si = 0, di = 0;
    friend bool operator==(const SysCall&, const SysCall&) = default;
};

struct Spec {
    std::filesystem::path root;
    std::filesystem::path work;
    std::string program;                      // a path in the XENIX system: "lib/c0"
    std::vector<std::string> args;            // the program's arguments (bytes; argv[0] is `program`)
    Bytes stdin_data;                         // what fd 0 reads
    bool trace = false;                       // keep the system calls in Result::calls
    std::uint64_t limit = 400'000'000;        // instructions
    std::map<std::uint32_t, Hook> hooks;      // by offset in the text
};

struct Result {
    Bytes console;                            // what the program wrote to fd 1 and 2
    std::optional<std::uint16_t> status;      // its exit status (absent: it did not exit)
    std::vector<SysCall> calls;               // the system calls, if traced
    std::optional<std::string> error;         // why the run stopped short, if it did: the machine faulted, a call this
                                              // does not provide, the limit
};

// Run `spec.program`. Throws NotFoundError if the file is not there, FormatError (TruncatedError if the file is
// shorter than its header says) if it is not an a.out for the 8086, UsageError if the arguments do not fit in the
// program's stack. A fault of the machine or of the program is not an exception: it ends the run, and is `error`.
Result run(const Spec& spec);

}  // namespace cb::exact::emu::xenix
