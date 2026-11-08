// codebreaker/formats/powerpc_image.hpp -- typed C++ of codebreaker.formats.powerpc_image
//
// powerpc_image.py -- what a PowerPC PE image states about its own code.
//
//     auto img = powerpc_image::read(path);
//     img->base                       the address the image is built for
//     img->spans                      [(address, file offset, size)]: where the file's bytes sit in memory
//     img->functions                  sorted function starts
//     img->imports                    {address: name}: the stubs calls to other modules go through
//     img->inline_                    {address: name}: register save/restore runs entered by `bl`
//     img->code                       (first address, last address + 1) of the code: every executable section
//     img->extents                    {start: (size, prologue size, has an exception handler)}, from .pdata
//     img->entry                      the entry point's address (0 when there is none)
//     img->word(address)              one big-endian instruction word, or None
//     virtual_methods(*img)           {address: (class, slot)}: the virtual functions run-time type
//                                     information names (see below)
//
// Big-endian PowerPC code in a PE container (machine type 0x1F2), as produced by Microsoft's
// PowerPC compilers. Three things about such an image are stated by the image and need no guessing:
//
// Functions. The exception table (.pdata) has one record per function that has unwind information:
// its start and its length. Small leaf functions have no record, so every `bl` target inside the
// code section is a function start as well.
//
// Imports. A call to another module goes through a 16-byte stub in the code section: two record
// words naming the library (by index) and the ordinal, then `mtctr r11; bctr`. The loader rewrites
// the two words into a load of the function's address. A stub is named `import<library>_ord_<n>`.
//
// Prologue and epilogue helpers. Functions that save many registers do not do so inline: they call
// into a run of `std rN,-X(r1)` instructions that ends `stw r12,-8(r1); blr` (entering at the
// instruction for the first register they need), and leave by jumping into the matching run of
// `ld rN,-X(r1)` that ends `lwz r12,-8(r1); mtlr r12; blr`. The same exists for floating-point
// registers, relative to r12 (`stfd` / `lfd` runs ending `blr`). These runs are part of every
// caller's own prologue and epilogue, not functions in their own right: the ones entered by `bl`
// are listed in `inline_` so their instructions are read as the caller's; the general-register
// restore run is reached by a plain jump and simply continues the caller.
//
// Classes. Microsoft's C++ compilers leave run-time type information beside each virtual table of a
// class that has any: the word before the table points at a locator (signature 0) whose fourth
// word points at the class's type descriptor, which holds the class's decorated name (`.?AVCFoo@@`).
// Where that chain is intact, every function the table lists is a virtual function of that class,
// named for certain; the standard library's own classes are left out.
//
// Some files hold the image as it sits in memory (every section at its own address) rather than
// packed as a file on disk; their section headers still give file positions that are no longer
// true. Which of the two a file is, is decided by where the function table's entries point at code.
//
// Typing notes:
//   * The Python's `Image` is the struct below, handed around as py::ref<Image> (read caches one per path,
//     and virtual_methods caches its answer on the image).  The attributes the native reader's answer set
//     (`__dict__.update(answer)`) are its fields; `run_names` is std::nullopt for an image with no code
//     section (the answer then has no such key).
//   * The native reader (native/formats/win/powerpc_image.cc) is a question handler that builds pickled
//     values; its algorithm is written out in the .cpp on typed values.  Its exceptions (cbf::PyError) are
//     thrown as the py:: exception of the same Python type.
//   * "base" in the basefile's .json (a JSON number) is read as an integer; any other truthy kind of value
//     (the Python would carry a str or a float on to fail later) is a TypeError here.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::powerpc_image {

inline constexpr int64_t MACHINE = 0x01F2;

// The image: the attributes listed above (the Python gets them from formats.dll's powerpc.read).
struct Image {
    py::str path;
    int64_t base = 0;
    py::bytes data;
    bool memory_layout = false;
    py::list<std::tuple<int64_t, int64_t, int64_t>> spans;               // (address, file offset, size)
    py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> sections;   // (name, address, size, characteristics)
    py::list<int64_t> functions;
    py::dict<int64_t, py::str> imports;
    py::dict<int64_t, py::str> inline_;                                  // `inline` in the Python
    std::tuple<int64_t, int64_t> code{0, 0};
    py::dict<int64_t, std::tuple<int64_t, int64_t, bool>> extents;       // start -> (size, prologue size, has a handler)
    int64_t entry = 0;
    std::optional<py::dict<int64_t, py::str>> run_names;                 // {address: run name}; None without code
    std::optional<py::dict<int64_t, std::tuple<py::str, int64_t>>> _virtual;   // virtual_methods' cache

    // The file offset of an address, or None when no span holds it.
    std::optional<int64_t> offset(int64_t va) const;
    // One big-endian instruction word, or None.
    std::optional<int64_t> word(int64_t va) const;
    py::bytes bytes_at(int64_t va, int64_t size) const;
};

// -> Image for a PowerPC PE file (cached per path).
py::ref<Image> read(const py::str& path);

// {address: (class, slot)} -- each virtual function whose table still links, through its
// locator, to the class's type descriptor (see the module's notes). Big-endian words; the
// classes of the standard library are left out.
py::dict<int64_t, std::tuple<py::str, int64_t>> virtual_methods(Image& img);

// [(address, bytes)] -- the image as it sits in memory, for running it.
py::list<std::tuple<int64_t, py::bytes>> regions(const py::str& path);

}  // namespace codebreaker::formats::powerpc_image
