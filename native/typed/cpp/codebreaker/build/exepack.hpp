// codebreaker/build/exepack.hpp -- C++ from codebreaker.build.exepack.
#pragma once

#include "py.hpp"

namespace codebreaker::build::exepack {

// exepack.py -- store a real-mode program the way LINK /EXEPACK did, and read how a stored one was.
//
//     Layout l = exepack::layout(stored_module, cs, ip);     how the original was packed: .fills, .stub_code
//     auto [module, entry] = exepack::pack(image, relocs, entry, stack, fills, stub_code);
//
// The format (formats/dos.unpack_exepack undoes it): the stored load module is the program's
// bytes with stretches of one repeated byte ("fills") taken out, followed by a paragraph-aligned
// segment the file's entry point names. That segment starts with eight words -- the program's IP,
// CS, a scratch word, the segment's own size, SP, SS, the program's size in paragraphs, "RB" --
// then the unpacker's code and its message ("Packed file is corrupt"), then the relocation table
// as sixteen lists, one per 64 KB, each a count and that many offsets.
//
// The packed stream is read backwards by the unpacker, from just below that segment: each command
// is a length word and a command byte (B0 fill, B2 copy; bit 0 marks the last), a fill preceded by
// its one byte, a copy by its bytes. The program's first bytes, up to the first fill, are stored
// as they are -- no command is spent on them -- and FF pads the stream to the paragraph.
//
// Which stretches are fills is the linker's choice, made from how the program was written: LINK
// packed what the object file gave it as ITERATED data (`db 218 dup (0)`), not every run of a
// byte -- a run written out byte by byte stays as it is. The fills of a rebuilt program are
// therefore stated: they are the `dup`s of its source, listed beside it (layout() reads them from
// the original).

extern const py::bytes MESSAGE;
extern const int64_t FILL;
extern const int64_t COPY;

// A fill: (offset in the program, length, the byte).
using Fill = std::tuple<int64_t, int64_t, int64_t>;

class Layout {
public:
    py::list<Fill> fills;     // [(offset, length, byte)] in the program, ascending
    py::bytes stub_code;      // the unpacker's code and message: what follows the header words
    int64_t size = 0;         // the program's size in bytes (paragraphs * 16)
    int64_t words = 0;        // 8, or 9 in the later header (a skip count before "RB")

    Layout(const py::list<Fill>& fills, const py::bytes& stub_code, int64_t size, int64_t words);
};

// How a stored load module was packed. ValueError when it is not EXEPACK (struct.error, IndexError
// as Python raises them for a module too short to hold what it claims).
Layout layout(const py::bytes& module, int64_t cs, int64_t ip);

// The program `image` (its relocation offsets in the order the table lists them, entry (cs, ip),
// stack (ss, sp), all relative to the program) stored packed, with a header of `words` words.
// -> (stored load module, (cs, ip, ss, sp) of the file's header): the unpacker runs from its own
// copy behind the program, with a stack of 80h bytes there.
std::tuple<py::bytes, std::tuple<int64_t, int64_t, int64_t, int64_t>> pack(
    const py::bytes& image, const py::list<int64_t>& relocs, const std::tuple<int64_t, int64_t>& entry,
    const std::tuple<int64_t, int64_t>& stack, const py::list<Fill>& fills, const py::bytes& stub_code, int64_t words = 8);

}  // namespace codebreaker::build::exepack
