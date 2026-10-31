// codebreaker/formats/ne.hpp -- C++ from codebreaker.formats.ne by CodeBreaker's Python front end.
#pragma once

#include "py.hpp"

namespace codebreaker::formats::ne {

class NEFile;
struct Entry;
struct HeaderRecord;

struct Entry {
    std::optional<int64_t> ordinal;
    std::optional<py::str> kind;
    std::optional<int64_t> seg;
    std::optional<int64_t> offset;
    std::optional<int64_t> flags;
};
template <class F> void for_each_field(const Entry& r, F&& f) {
    if (r.ordinal) f("ordinal", *r.ordinal);
    if (r.kind) f("kind", *r.kind);
    if (r.seg) f("seg", *r.seg);
    if (r.offset) f("offset", *r.offset);
    if (r.flags) f("flags", *r.flags);
}

struct HeaderRecord {
    int64_t linker_ver = 0;
    int64_t linker_rev = 0;
    int64_t entry_off = 0;
    int64_t entry_len = 0;
    int64_t crc = 0;
    int64_t flags = 0;
    int64_t autodata_seg = 0;
    int64_t heap = 0;
    int64_t stack = 0;
    int64_t csip = 0;
    int64_t sssp = 0;
    int64_t n_segs = 0;
    int64_t n_modrefs = 0;
    int64_t nonres_size = 0;
    int64_t seg_tab = 0;
    int64_t rsrc_tab = 0;
    int64_t resname_tab = 0;
    int64_t mod_tab = 0;
    int64_t imp_tab = 0;
    int64_t nonres_off = 0;
    int64_t n_movable = 0;
    int64_t align_shift = 0;
    int64_t n_resources = 0;
    int64_t exetype = 0;
    std::optional<int64_t> expect_winver;
};
template <class F> void for_each_field(const HeaderRecord& r, F&& f) {
    f("linker_ver", r.linker_ver);
    f("linker_rev", r.linker_rev);
    f("entry_off", r.entry_off);
    f("entry_len", r.entry_len);
    f("crc", r.crc);
    f("flags", r.flags);
    f("autodata_seg", r.autodata_seg);
    f("heap", r.heap);
    f("stack", r.stack);
    f("csip", r.csip);
    f("sssp", r.sssp);
    f("n_segs", r.n_segs);
    f("n_modrefs", r.n_modrefs);
    f("nonres_size", r.nonres_size);
    f("seg_tab", r.seg_tab);
    f("rsrc_tab", r.rsrc_tab);
    f("resname_tab", r.resname_tab);
    f("mod_tab", r.mod_tab);
    f("imp_tab", r.imp_tab);
    f("nonres_off", r.nonres_off);
    f("n_movable", r.n_movable);
    f("align_shift", r.align_shift);
    f("n_resources", r.n_resources);
    f("exetype", r.exetype);
    f("expect_winver", r.expect_winver);
}

class NEFile : public py::object {
public:
    py::str path;
    py::bytes data;
    int64_t ne_off = 0;
    HeaderRecord h;
    static constexpr int64_t base = 0;

    explicit NEFile(const py::str& path);
    int64_t u16(int64_t off);
    int64_t u32(int64_t off);
    void _parse_header();
    py::list<std::tuple<py::str, int64_t>> _read_name_table(int64_t file_off);
    py::list<std::tuple<py::str, int64_t>> resident_names();
    py::list<std::tuple<py::str, int64_t>> nonresident_names();
    py::str module_name();
    py::list<py::str> module_refs();
    py::str imported_name(int64_t off);
    py::list<int64_t> export_ordinals();
    py::list<Entry> entries();
    py::str _rsrc_string(int64_t base, int64_t off);

    // Left in Python (the front end refused them):
    //   from_buffer -- dynamic:decorator classmethod
    //   segments -- int-range:field file_off in [0, None]
    //   relocations -- int-range:argument off in [0, 9223372036854841343]
    //   resources -- int-range:field off in [0, None]
};

extern const int64_t PRELOAD;

py::list<std::tuple<int64_t, py::str>> embedded(const py::bytes& data, int64_t start = 64);

// Left in Python (the front end refused them):
//   open_boot_image -- dynamic:next
//   _tables_end -- untyped:parameter nef (no annotation, no call site)
//   standalone -- syntax:nested function

}  // namespace codebreaker::formats::ne
