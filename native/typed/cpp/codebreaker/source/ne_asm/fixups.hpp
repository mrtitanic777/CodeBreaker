// codebreaker/source/ne_asm/fixups.hpp -- typed C++ of codebreaker.source.ne_asm.fixups.
//
// disasm.py -- Annotated 16-bit disassembly of NE segments.
//
// Extracts a segment's code, disassembles it with objdump (i8086), then walks the
// NE per-segment relocation chains to label every fixup site with the real target
// (module.ordinal for imports, seg:entry for internal refs). This turns the
// placeholder `call 0x0:0xffff` instructions into readable `call FAR KERNEL.91`.
//
// Usage:
//     python3 disasm.py FILE [seg_index] [--start 0xNN] [--count N]
//     python3 disasm.py FILE --entry ORDINAL      # disassemble one exported entry
//
// Typing notes:
//   * The Python's `if __name__ == "__main__"` block (it reads sys.argv) is main(argv) here; `argv` is
//     sys.argv[1:] (the file first), and the return is the exit code (0; the Python's sys.exit() after --entry).
//     A missing file argument is the IndexError the Python's `a[0]` raises.
//   * The tools print to stdout, as the Python's print does.
#pragma once

#include <optional>

#include "codebreaker/formats/ne.hpp"
#include "py.hpp"

namespace codebreaker::source::ne_asm::fixups {

// Open a normal MZ/NE file, or a raw embedded NE module (starts with 'NE').
formats::ne::NEFile open_ne(const py::str& path);

// offset -> human label, by walking each relocation chain.
py::dict<int64_t, py::str> build_fixup_map(const formats::ne::NEFile& ne, const formats::ne::Segment& seg,
                                           const py::list<py::str>& mods);

void disasm_segment(const py::str& path, int64_t seg_index = 1, int64_t start = 0, const std::optional<int64_t>& count = std::nullopt);

void disasm_entry(const py::str& path, int64_t ordinal);

// the script's command line: argv is sys.argv[1:]
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::source::ne_asm::fixups
