// codebreaker/formats/xbox/nand.hpp -- C++ from codebreaker.formats.xbox.nand.
#pragma once

#include "py.hpp"

namespace codebreaker::formats::xbox::nand {

// nand.py -- the flash file system of an Xbox 360 NAND dump (16 MB, small-block, with spare bytes).
//
//     fs = nand::Flash(raw_bytes)
//     fs.files                        {name: (first block, size, time)}
//     fs.read(name)                   a file's bytes
//     nand::is_nand(path)
//
// A raw dump is pages of 0x200 data bytes and 0x10 spare bytes, 32 pages to a block (0x4000 data
// bytes). A block's spare bytes give its id (bytes 2 and 1) and type (byte 12, low six bits; $30 is
// the file system's root). Blocks are found by id, not position (bad blocks are remapped). A root
// block's even pages hold the allocation table (a big-endian 16-bit next-block number per block:
// $1FFF ends a file, $1FFB reserved, $1FFE free), its odd pages sixteen 0x20-byte directory
// entries: a name (22 bytes, zero padded), the first block (16 bits), the size and a time (32 bits
// each). Older copies of the root stay in the flash; the one with the highest sequence number
// (spare bytes 0, 3, 4 and 6) is current. Layout read from a dashboard 17559 console's dump.

extern const int64_t PAGE;
extern const int64_t SPARE;
extern const int64_t PAGES_PER_BLOCK;
extern const int64_t BLOCK;
extern const int64_t ROOT_TYPE;
extern const int64_t END;

// A raw dump with spare bytes: its size a whole number of pages-with-spares, starting with the
// flash header's magic ($FF4F) and Microsoft's copyright.
bool is_nand(const py::str& path);

class Flash {
public:
    py::bytes raw;
    py::dict<int64_t, int64_t> by_id;
    int64_t root = 0;
    py::list<int64_t> alloc;
    py::dict<py::str, std::tuple<int64_t, int64_t, int64_t>> files;   // name -> (first block, size, time)

    // ValueError when the size is not a whole number of pages-with-spares or no root block exists.
    explicit Flash(const py::bytes& raw);

    py::bytes _page(int64_t p) const;
    py::bytes _spare(int64_t block) const;
    py::list<int64_t> chain(const py::str& name) const;   // KeyError for an unknown name
    py::bytes read(const py::str& name) const;
};

}  // namespace codebreaker::formats::xbox::nand
