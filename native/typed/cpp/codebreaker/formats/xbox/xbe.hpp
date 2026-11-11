// codebreaker/formats/xbox/xbe.hpp -- typed C++ from codebreaker.formats.xbox.xbe.
//
// Original Xbox XBE executable reader -- the OG counterpart to xex.py.
//
// Parses the XBE header, certificate (title, title id, region, ratings, media),
// section table, and decrypts the XOR-obfuscated entry point and kernel-thunk
// address by trying the retail / debug / chihiro keys and keeping whichever lands
// inside the image. (OG Xbox XISO discs are already read by `gdf` -- its base-offset
// scan includes the 0x18300000 redump base.)
//
// XBE header (little-endian):
//   0x104 base addr   0x10C size of image   0x118 cert addr(VA)   0x11C section count
//   0x120 section headers(VA)   0x128 entry(^key)   0x148 kernel thunk(^key)
// Certificate: 0x08 title id  0x0C title name(40 UTF-16LE)  0xA0 region  0xA4 ratings
// Section header (0x38): flags, VA, vsize, raw addr, raw size, name VA.
//
//     to_pe(path)     the program as a 32-bit PE image, for the x86 lane (see below)
//
// The original Xbox runs x86 code, so its programs go down the 32-bit Windows lane once they are
// in the shape that lane reads. to_pe writes that image: the same bytes at the same addresses
// (each section at its address, the XBE's own header at the base), the entry point, and the
// kernel's functions as imports. The kernel thunk table already has the shape of a PE import
// address table -- one word per import, the kernel's ordinal with the top bit set, a zero at the
// end -- so it becomes the import table of `xboxkrnl.exe`, whose ordinals symbols/ordinal_names.json
// names (the kernel's export map). The image is written once per distinct file, named by its
// contents.
//
// Typing notes:
//   * The header / certificate reader, the section table and the PE image maker are formats.dll's (native/formats/xbox/
//     assets.cc), linked in and asked through cbf_call.  The Python's XBE copies the header dict into its attributes;
//     here they are the fields of the class (the dict's own names: `_entry_raw`, `_thunk_raw`).
//   * Replaces the hooks `xbe.XBE` of the formats.xbox package and `set_xbe_to_pe` of the platforms (to_pe).
//   * main() takes sys.argv[1:].
#pragma once

#include <cstdint>
#include <tuple>

#include "py.hpp"

namespace codebreaker::formats::xbox::xbe {

extern const py::bytes MAGIC;
extern const py::dict<py::str, int64_t> ENTRY_KEYS;
extern const py::dict<py::str, int64_t> THUNK_KEYS;
extern const py::dict<int64_t, py::str> REGION;
extern const py::dict<int64_t, py::str> MEDIA;
extern const py::str DESCRIPTION;

// the answer of xbe.parse: the header and certificate
struct Header {
    int64_t base = 0;
    int64_t size_of_image = 0;
    int64_t size_of_headers = 0;
    int64_t cert_addr = 0;
    int64_t section_count = 0;
    int64_t section_hdr_addr = 0;
    int64_t init_flags = 0;
    int64_t _entry_raw = 0;
    int64_t _thunk_raw = 0;
    int64_t cert_size = 0;
    int64_t title_id = 0;
    py::str title_name;
    int64_t allowed_media = 0;
    int64_t region = 0;
    int64_t ratings = 0;
    int64_t disk_number = 0;
    int64_t version = 0;
};
template <class F> void for_each_field_mut(Header& h, F&& f) {
    f("base", h.base, true);
    f("size_of_image", h.size_of_image, true);
    f("size_of_headers", h.size_of_headers, true);
    f("cert_addr", h.cert_addr, true);
    f("section_count", h.section_count, true);
    f("section_hdr_addr", h.section_hdr_addr, true);
    f("init_flags", h.init_flags, true);
    f("_entry_raw", h._entry_raw, true);
    f("_thunk_raw", h._thunk_raw, true);
    f("cert_size", h.cert_size, true);
    f("title_id", h.title_id, true);
    f("title_name", h.title_name, true);
    f("allowed_media", h.allowed_media, true);
    f("region", h.region, true);
    f("ratings", h.ratings, true);
    f("disk_number", h.disk_number, true);
    f("version", h.version, true);
}

// one row of the section table
struct Section {
    py::str name;
    int64_t flags = 0;
    int64_t vaddr = 0;
    int64_t vsize = 0;
    int64_t raw_addr = 0;
    int64_t raw_size = 0;
};
template <class F> void for_each_field_mut(Section& s, F&& f) {
    f("name", s.name, true);
    f("flags", s.flags, true);
    f("vaddr", s.vaddr, true);
    f("vsize", s.vsize, true);
    f("raw_addr", s.raw_addr, true);
    f("raw_size", s.raw_size, true);
}

class XBE {
  public:
    py::bytes d;
    int64_t base = 0, size_of_image = 0, size_of_headers = 0, cert_addr = 0, section_count = 0, section_hdr_addr = 0,
            init_flags = 0, _entry_raw = 0, _thunk_raw = 0, cert_size = 0, title_id = 0;
    py::str title_name;
    int64_t allowed_media = 0, region = 0, ratings = 0, disk_number = 0, version = 0;

    explicit XBE(const py::bytes& data);

    // Title id -> 'AA-nnn' (two publisher letters + 3-digit game number).
    py::str title_id_str() const;
    std::tuple<int64_t, py::str> _decrypt(int64_t raw, const py::dict<py::str, int64_t>& keys) const;
    std::tuple<int64_t, py::str> entry_point() const;
    std::tuple<int64_t, py::str> kernel_thunk() const;
    py::list<Section> sections() const;
    py::str region_str() const;
    py::str media_str() const;
};

// The XBE at `path` as a PE image (written once, kept under state/unpacked/xbe). -> its path
py::str to_pe(const py::str& path);

// The bytes of the PE image for an XBE (see the module's notes; formats.dll's).
py::bytes pe_image(const XBE& x);

// the command line without the program name (sys.argv[1:]); prints to stdout
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::xbe
