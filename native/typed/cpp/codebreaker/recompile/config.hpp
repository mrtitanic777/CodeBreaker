// codebreaker/recompile/config.hpp -- typed C++ port of codebreaker.recompile.config.
//
// config.py -- the XenonRecomp configuration of a title, from what CodeBreaker finds in it.
//
//     py::str text = config::make(xex_path, "ppc", "switch_tables.toml", std::nullopt);
//
// XenonRecomp takes a title's functions from its .pdata and follows the calls itself; what the
// configuration adds is what it cannot find:
//
//   * the compiler's register save/restore helpers (__savegprlr_14, __restgprlr_14, __savefpr_14,
//     __restfpr_14, __savevmx_14/_64, __restvmx_14/_64): each run is recompiled as one function
//     with an entry per register count;
//   * every function .pdata leaves out (discover.py: leaf functions, those reached only through a
//     pointer in the data or an address the code builds), with its length;
//   * the switch tables (XenonAnalyse's, written to their own file);
//   * the register settings, which must match the runtime's ppc_config.h (native/x360) since they
//     change PPCContext's layout;
//   * the title's hooks: `midasm_hook` tables from a hooks file, copied as they are.
//
// Typing notes:
//   * helpers() / jump_buffers() answer {config key: address}: a py::dict<py::str, int64_t> in the order the Python's
//     dict gets its keys.
//   * `switches` (a {table base: [case labels]} dict, or None) is std::optional<SwitchTables>; None and {} are the same.
//   * `code` is the image's code bytes (py::bytes); a slice of it with a start before the first byte reads as
//     Python's negative slice does.
//   * tomllib (read_switch_tables, the `analysed` text of write_switch_tables) has no port in the runtime: the file
//     formats read here (XenonAnalyse's and switches.write's: `[[switch]]` tables of integer keys and one array of
//     integers) are read by the small TOML reader at the top of config.cpp, which raises TOMLDecodeError (a ValueError)
//     for text it does not read.
//   * The Python's `switches` module is imported inside write_switch_tables; here codebreaker/recompile/switches.hpp.
//   * Python ints are unbounded; int64_t is the type (no real address comes near it).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/formats/powerpc_image.hpp"
#include "py.hpp"

namespace codebreaker::recompile::config {

// tomllib.TOMLDecodeError (a ValueError)
struct TOMLDecodeError : py::ValueError {
    TOMLDecodeError() : py::ValueError() {}
    explicit TOMLDecodeError(const std::string& m) : py::ValueError(m) {}
    const char* type() const override { return "TOMLDecodeError"; }
};

// The registers kept as locals of each function (the runtime is built with the same settings: x360.cmake
// reads them back from the generated ppc_config.h). LR stays in the context (backtraces and unwinding
// read it), and so do the non-volatile registers: a compiler-generated __finally block is called with
// `bl` and reads its parent's r30/r31, which a local of the parent would hide. CR, CTR and XER stay there too:
// a function entered by a branch from another can test the flags or count down the CTR its caller set
// (Up: `cmplwi cr6,r11,1; b 0x8212E5F4`, which starts with `bne cr6`).
extern const py::list<std::tuple<py::str, bool>> REGISTER_SETTINGS;

// {table base (the bctr): [case labels]}
using SwitchTables = py::dict<int64_t, py::list<int64_t>>;

// {config key: address} of the save/restore runs the image has.
py::dict<py::str, int64_t> helpers(const formats::powerpc_image::Image& img);

// The first word after the zero padding (or blr) that comes before `a`: a function's start.
int64_t _start_before(const py::bytes& code, int64_t lo, int64_t a);

// The CRT's setjmp and longjmp (the Xbox 360 jmp_buf: r1 at +0x90, CR at +0x130, LR at +0x134):
// setjmp stores r1 at 0x90(r3); longjmp loads LR from 0x134 and r1 from 0x90 of one register.
// XenonRecomp turns them into the host's setjmp/longjmp; without them a longjmp is a call that
// returns, and its caller goes on with registers it never restored.
py::dict<py::str, int64_t> jump_buffers(const formats::powerpc_image::Image& img, const py::bytes& code);

// {table base (the bctr): [case labels]} from a switch-table file (XenonAnalyse's format).
SwitchTables read_switch_tables(const py::str& path);

// The title's switch-table file: switches.py's tables (it walks back from every bctr), with
// XenonAnalyse's (`analysed`, its file's text) added for any bctr switches.py did not resolve.
// -> (switches.py's count, XenonAnalyse's added).
std::tuple<int64_t, int64_t> write_switch_tables(const py::str& path, const py::str& xex_path,
                                                 const std::optional<py::str>& analysed = std::nullopt);

// The address after the first blr, bctr or plain branch at or after `a`.
int64_t _end_of_run(const py::bytes& code, int64_t lo, int64_t a);

// [(address, size)] of the functions .pdata leaves out, sized to the next known start (a run
// of save/restore entries, and the import thunks, count as starts). A switch's case labels are
// never starts (an absolute switch table is data holding code addresses, which the pointer
// scan would take for functions), and the function that holds a switch runs to the end of
// its last case.
py::list<std::tuple<int64_t, int64_t>> extra_functions(const py::str& xex_path, const formats::powerpc_image::Image& img,
                                                       const std::optional<SwitchTables>& switches = std::nullopt);

// The configuration's text. `file_path` is the .xex as the configuration names it
// (default: xex_path); `hooks` is TOML text appended as it is (midasm_hook tables).
py::str make(const py::str& xex_path, const py::str& out_dir = py::str("ppc"),
             const py::str& switch_tables = py::str("switch_tables.toml"), const std::optional<py::str>& hooks = std::nullopt,
             const std::optional<py::str>& file_path = std::nullopt);

}  // namespace codebreaker::recompile::config
