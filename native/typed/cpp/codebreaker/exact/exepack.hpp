// codebreaker/exact/exepack.hpp -- C++ from codebreaker.exact.exepack.
//
// exepack.py -- what a file packed by EXEPACK (or LINK /EXEPACK) keeps of the program it packed.
//
// The packed file's code segment begins with EXEPACK's header: the program's own entry and stack (real
// CS:IP, SS:SP), the length it unpacks to, the signature "RB". After the unpacker's code and its message
// ("Packed file is corrupt") come the program's relocations: sixteen groups, one for each 64K of the image,
// a count and that many offsets - in the order the linker listed them.
//
//     exepack::read(file bytes) -> Packed or null (not packed)
//         .ip .cs .sp .ss      the program's entry and stack (paragraphs from the image's start)
//         .dest                the unpacked length in paragraphs
//         .relocs              [image address] of each relocation, in the table's order
#pragma once

#include "py.hpp"

namespace codebreaker::exact::exepack {

class Packed : public py::object {
public:
    int64_t ip = 0;
    int64_t cs = 0;
    int64_t mem = 0;
    int64_t size = 0;
    int64_t sp = 0;
    int64_t ss = 0;
    int64_t dest = 0;
    py::list<int64_t> relocs;
};

extern const py::bytes SIG;
extern const py::bytes MSG;

// The packed program's header and relocations; null when the file is not EXEPACK-packed.
// Raises py::StructError (struct.error) when the relocation table runs off the end of the file.
py::ref<Packed> read(const py::bytes& b);

}  // namespace codebreaker::exact::exepack
