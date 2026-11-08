// codebreaker/formats/xbox/pck.hpp -- C++ port of codebreaker.formats.xbox.pck.
//
// Minecraft LCE .pck container parser / extractor.
//
// Layout (all integers big-endian, all names UTF-16BE):
//
//     uint32 version                       observed: 3
//     uint32 property_count
//       property_count x {
//           uint32 id
//           uint32 name_len                in UTF-16 code units
//           char16 name[name_len]
//           uint32 zero
//       }
//     uint32 entry_count
//       entry_count x {
//           uint32 size                    payload length in bytes
//           uint32 type                    content-type tag
//           uint32 name_len
//           char16 name[name_len]
//           uint32 zero
//       }
//     payload blobs, in table order
//
// Tutorial.pck has property_count = 0, which is why a parser that assumes the
// property table is absent still appears to work on it.
//
// Notes on the port:
//   * the Python's `if __name__ == '__main__':` block is main(sys_argv) (the program name first);
//   * a short file raises py::StructError with struct.error's own message (the CLI prints it);
//   * names are decoded as UTF-16BE with 'replace' (py::bytes::decode has no utf-16 codec).
#pragma once

#include "py.hpp"

namespace codebreaker::formats::xbox::pck {

extern const py::str DESCRIPTION;

// One entry of the table (the Python's dict(size=, type=, name=)).
struct Entry {
    int64_t size = 0;
    int64_t type = 0;
    py::str name;
};

// parse()'s dict: dict(version=, props=, entries=, table_end=, data=)
struct Parsed {
    int64_t version = 0;
    py::list<std::tuple<int64_t, py::str>> props;
    py::list<Entry> entries;
    int64_t table_end = 0;
    py::bytes data;
};

// Read {uint32 a, uint32 len, utf16be name, uint32 zero} at p. -> (a, name, the position after the record)
std::tuple<int64_t, py::str, int64_t> _rec(const py::bytes& d, int64_t p);

Parsed parse(const py::str& path);

// Every payload as (name, bytes), and where reading stopped. -> ([(name, blob)], end, file size)
//
// Entry sizes in the table are exact (verified 122/122 against PNG spans),
// but payloads are separated by variable padding -- observed 4, 16 and 316
// bytes, not a simple alignment.  So rather than predict the padding, walk
// forward from the previous payload and skip the gap: PNG payloads are
// located by signature, everything else by skipping NUL padding.
std::tuple<py::list<std::tuple<py::str, py::bytes>>, int64_t, int64_t> payloads(const py::str& path);

// Write every payload under outdir. -> (files written, end, file size)
std::tuple<int64_t, int64_t, int64_t> extract(const py::str& path, const py::str& outdir);

// The script: `pck [-x] file.pck ...` (sys_argv as sys.argv: the program name first).
void main(const py::list<py::str>& sys_argv);

}  // namespace codebreaker::formats::xbox::pck
