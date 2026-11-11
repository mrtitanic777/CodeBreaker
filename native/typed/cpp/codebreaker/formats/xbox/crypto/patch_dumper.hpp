// codebreaker/formats/xbox/crypto/patch_dumper.hpp -- C++ port of codebreaker.formats.xbox.crypto.patch_dumper.
#pragma once

#include <functional>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::crypto::patch_dumper {

// One decoded instruction, as the disassembler returns it (capstone's mnemonic and op_str).
struct Instruction {
    py::str mnemonic;
    py::str op_str;
};

// md.disasm(code, address): the instructions in `code` taken to start at `address`.
// The Python main() gets its decoder from platforms.decoder(32, arch="ppc"): so does main(paths); main(md, paths) takes
// another PowerPC decoder (an override).
using Decoder = std::function<py::list<Instruction>(const py::bytes&, int64_t)>;

// The files main() reads and writes (hard-coded in the script).
extern const char* const PATCHES_PATH;
extern const char* const OUTPUT_PATH;

// Dump the xeBuild patch file as MAKEPATCH assembly. The paths default to the script's own.
void main(const py::str& patches_path = py::str(PATCHES_PATH), const py::str& output_path = py::str(OUTPUT_PATH));
void main(const Decoder& md, const py::str& patches_path = py::str(PATCHES_PATH),
          const py::str& output_path = py::str(OUTPUT_PATH));

}  // namespace codebreaker::formats::xbox::crypto::patch_dumper
