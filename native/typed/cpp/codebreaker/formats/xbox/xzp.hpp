// codebreaker/formats/xbox/xzp.hpp -- typed C++ from codebreaker.formats.xbox.xzp.
//
// XUIZ (.xzp) container parser / extractor -- Xbox 360 UI / texture packs.
//
// Two layouts share the 'XUIZ' magic, told apart by the version word at 0x04:
//
//   v1  Minecraft LCE:                       v3  Xbox dashboard / XUI:
//     0x00 'XUIZ'                              0x00 'XUIZ'
//     0x04 uint32be version = 1                0x04 uint32be version = 3
//     0x08 uint32be file_size                  0x08 uint32be file_size
//     0x0C uint32be ? (1)                       0x0C uint32be ? (0)
//     0x10 uint32be base_rel (rel 0x16)         0x10 uint32be base_rel (rel 0x16)
//     0x14 uint16   ?                           0x14 uint16   entry_count
//     0x17 table (count implied):               0x16 table (entry_count entries):
//        uint24be size                             uint32be size
//        uint32be offset (rel 0x16+base_rel)       uint32be offset (rel 0x16+base_rel)
//        uint16le name_len (UTF-16 units)          uint8    name_len (bytes)
//        char16le name  (LITTLE-endian)            char     name  (latin-1)
//     payloads                                  payloads
//
// Both put the payload base at 0x16 + (uint32be @0x10). v1 has a one-byte skew
// (table @0x17, offsets @0x16) that must be preserved; v3 has no skew and an
// explicit entry count. v3 names carry backslash paths with leading '..\\' -- the
// extractor strips '..'/absolute components so extraction can't escape the outdir.
//
// Verified: v1 Minecraft TexturePack.xzp (PNG sig + IEND exact); v3 dashboard
// packs controlp/xam/skin/gamercrd (18/29/33/9 PNG signatures all valid).
//
// Typing notes:
//   * parse() / payloads() are formats.dll's (native/formats/xbox/assets.cc), linked in and asked through cbf_call.  The
//     parse dict (the answer's keys plus 'data', the file's bytes) is the record Parsed; an entry the record Entry.
//   * The Python takes a path or the bytes of a container (`isinstance(path, (bytes, bytearray))`): both are overloads.
//     Given bytes, the reader is told the label '<bytes>' (what its messages name the file).
//   * The module's `if __name__ == '__main__'` block is main(argv) (argv = sys.argv[1:]); its sys.exit(n) is py::SystemExit(n).
//   * payloads(const py::str&) replaces the hook `xzp_payloads` of formats::xbox::volumes.
#pragma once

#include <cstdint>

#include "py.hpp"

namespace codebreaker::formats::xbox::xzp {

extern const py::str DESCRIPTION;
inline constexpr int64_t BASE_REL = 0x16;   // payload base is relative to this
inline constexpr int64_t TBL = 0x17;        // entry table starts here
extern const py::bytes PNG;
extern const py::bytes IEND;

// one stored file of the container
struct Entry {
    int64_t size = 0;
    int64_t offset = 0;
    py::str name;
};
template <class F> void for_each_field_mut(Entry& e, F&& f) {
    f("size", e.size, true);
    f("offset", e.offset, true);
    f("name", e.name, true);
}

// what parse() gives: the container's header and table, and its bytes
struct Parsed {
    int64_t version = 0;
    int64_t file_size = 0;
    int64_t base = 0;
    int64_t table_end = 0;
    py::list<Entry> entries;
    py::bytes data;                    // r['data'] = d (not in the reader's answer)
};
template <class F> void for_each_field_mut(Parsed& r, F&& f) {
    f("version", r.version, true);
    f("file_size", r.file_size, true);
    f("base", r.base, true);
    f("table_end", r.table_end, true);
    f("entries", r.entries, true);
    f("data", r.data, false);
}

// XUIZ comes in two layouts, distinguished by the version word at 0x04:
//   v1  Minecraft LCE      table @0x17, [u24be size][u32be off][u16le nlen][utf16le name]
//   v3  Xbox dashboard/XUI table @0x16, [u32be size][u32be off][u8 nlen][latin1 name],
//                          with an explicit u16 entry count at 0x14
// Both put the payload base at 0x16 + (u32be @0x10).
Parsed parse(const py::str& path);
Parsed parse(const py::bytes& data);

// (the number of PNG entries, of those with a PNG signature, of those with a closing IEND)
std::tuple<int64_t, int64_t, int64_t> verify(const Parsed& r);

// Every stored file as (relative path, bytes), names made safe (no '..', no absolute parts).
py::list<std::tuple<py::str, py::bytes>> payloads(const py::str& path);
py::list<std::tuple<py::str, py::bytes>> payloads(const py::bytes& data);

int64_t extract(const py::str& path, const py::str& outdir);
int64_t extract(const py::bytes& data, const py::str& outdir);

// Pack a folder of files into a XUIZ (.xzp) container. Clones the 0x16-byte
// header from a template .xzp (preserving its unknown fields), then rebuilds
// the entry table and payloads. Inverse of parse().
py::str build(const py::str& folder, const py::str& out_path, const py::str& template_path);
py::str build(const py::str& folder, const py::str& out_path, const py::bytes& template_data);

// the module's command line (sys.argv[1:]): `pack <folder> <out.xzp> <template.xzp>`, or paths and -x
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::xzp
