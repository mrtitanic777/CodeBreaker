// codebreaker/formats/ne.cpp -- typed C++ from codebreaker.formats.ne (see ne.hpp).
//
// Calls the native reader (native/formats/ne/ne.cc, cbf::ne::Module) directly and shapes its results as
// native/formats/ne/questions.cc shaped them for the Python bridge, but into typed structs.
#include "ne.hpp"

#include <new>

#include "../../../../formats/core/bytes.h"
#include "../../../../formats/core/text.h"
#include "../../../../formats/ne/ne.h"

namespace codebreaker::formats::ne {

namespace {

// ---- the reader's errors, as the exceptions Python raised --------------------------------------------
[[noreturn]] void rethrow(const cbf::PyError& e) {
    const std::string& k = e.kind;
    const std::string& m = e.message;
    if (k == "struct.error") throw py::StructError(m);
    if (k == "ValueError") throw py::ValueError(m);
    if (k == "IndexError") throw py::IndexError(m);
    if (k == "KeyError") throw py::KeyError(m);
    if (k == "TypeError") throw py::TypeError(m);
    if (k == "OverflowError") throw py::OverflowError(m);
    if (k == "MemoryError") throw py::MemoryError(m);
    if (k == "OSError") throw py::OSError(m);
    if (k == "StopIteration") throw py::StopIteration(m);
    if (k == "UnicodeDecodeError") throw py::UnicodeDecodeError(m);
    if (k == "AttributeError") throw py::AttributeError(m);
    if (k == "AssertionError") throw py::AssertionError(m);
    if (k == "ZeroDivisionError") throw py::ZeroDivisionError(m);
    throw py::RuntimeError(m);
}

// runs a call of the reader; its errors leave as py:: exceptions (a failed allocation is MemoryError, as in the DLL)
template <class F>
auto guard(F&& f) -> decltype(f()) {
    try {
        return f();
    } catch (const cbf::PyError& e) {
        rethrow(e);
    } catch (const cbf::Fallback& f2) {
        throw py::RuntimeError("formats.dll leaves this question to its caller: " + f2.reason);
    } catch (const std::bad_alloc&) {
        throw py::MemoryError(std::string());
    }
}

py::str T(const std::string& utf8) { return py::str(utf8); }

cbf::Bytes view(const py::bytes& b) { return cbf::Bytes(b.raw()); }

Header header_of(const cbf::ne::Header& h) {
    Header o;
    o.linker_ver = h.linker_ver;
    o.linker_rev = h.linker_rev;
    o.entry_off = h.entry_off;
    o.entry_len = h.entry_len;
    o.crc = h.crc;
    o.flags = h.flags;
    o.autodata_seg = h.autodata_seg;
    o.heap = h.heap;
    o.stack = h.stack;
    o.csip = h.csip;
    o.sssp = h.sssp;
    o.n_segs = h.n_segs;
    o.n_modrefs = h.n_modrefs;
    o.nonres_size = h.nonres_size;
    o.seg_tab = h.seg_tab;
    o.rsrc_tab = h.rsrc_tab;
    o.resname_tab = h.resname_tab;
    o.mod_tab = h.mod_tab;
    o.imp_tab = h.imp_tab;
    o.nonres_off = h.nonres_off;
    o.n_movable = h.n_movable;
    o.align_shift = h.align_shift;
    o.n_resources = h.n_resources;
    o.exetype = h.exetype;
    if (h.expect_winver) o.expect_winver = *h.expect_winver;
    return o;
}

// the module again from its place, as the DLL does on every question
cbf::ne::Module module(const NEFile& f) {
    cbf::ne::Module m(view(f.data), f.ne_off);
    m.base = f.base;
    if (auto overlay = f.overlay_place()) {
        m.boot = true;
        m.overlay_at = *overlay;
    }
    return m;
}

py::list<std::tuple<py::str, int64_t>> names_of(const std::vector<cbf::ne::Name>& ns) {
    py::list<std::tuple<py::str, int64_t>> out;
    for (const cbf::ne::Name& n : ns) out.append(std::make_tuple(T(cbf::latin1(n.name)), static_cast<int64_t>(n.ordinal)));
    return out;
}

// "NE" at ne_off, else ValueError, then the header (ne.from_buffer)
Header header_at_buffer(const py::bytes& data, int64_t off) {
    return guard([&] {
        cbf::Bytes d = view(data);
        if (!d.slice_is(off, off + 2, "NE", 2)) cbf::raise("ValueError", "no NE at 0x" + cbf::hex(uint64_t(off)));
        return header_of(cbf::ne::Module(d, off).h);
    });
}

}  // namespace

// ---- NEFile --------------------------------------------------------------------------------------------

NEFile::NEFile(const py::str& path_) : path(path_) {
    // self.data = open(path, "rb").read()
    py::file f = py::open_rb(path_);
    data = f.read();
    f.close();
    guard([&] {
        cbf::ne::Module m = cbf::ne::Module::open(view(data));
        ne_off = m.ne_off;
        h = header_of(m.h);
    });
}

void NEFile::init_buffer(NEFile& self, const py::bytes& data, int64_t ne_off, const py::str& path) {
    self.path = path;
    self.data = data;
    self.ne_off = ne_off;
    self.h = header_at_buffer(data, ne_off);
    self.base = ne_off;   // segment offsets are relative to this
}

NEFile NEFile::from_buffer(const py::bytes& data, int64_t ne_off, const py::str& path) {
    NEFile self;
    init_buffer(self, data, ne_off, path);
    return self;
}

void NEFile::_parse_header() {
    h = guard([&] { return header_of(cbf::ne::Module(view(data), ne_off).h); });
}

int64_t NEFile::u16(int64_t off) const {
    return guard([&] { return static_cast<int64_t>(view(data).u16(off)); });
}

int64_t NEFile::u32(int64_t off) const {
    return guard([&] { return static_cast<int64_t>(view(data).u32(off)); });
}

py::list<std::tuple<py::str, int64_t>> NEFile::_read_name_table(int64_t file_off) const {
    return guard([&] { return names_of(module(*this).read_name_table(file_off)); });
}

py::list<std::tuple<py::str, int64_t>> NEFile::resident_names() const {
    return guard([&] { return names_of(module(*this).resident_names()); });
}

py::list<std::tuple<py::str, int64_t>> NEFile::nonresident_names() const {
    return guard([&] { return names_of(module(*this).nonresident_names()); });
}

// This module's own name -- the first resident-name-table entry.
py::str NEFile::module_name() const {
    return guard([&] { return T(module(*this).module_name()); });
}

// Imported module names, in module-reference-table order (1-based).
py::list<py::str> NEFile::module_refs() const {
    return guard([&] {
        py::list<py::str> out;
        for (const std::string& s : module(*this).module_refs()) out.append(T(s));
        return out;
    });
}

py::str NEFile::imported_name(int64_t off) const {
    return guard([&] { return T(module(*this).imported_name(off)); });
}

py::list<Segment> NEFile::segments() const {
    return guard([&] {
        py::list<Segment> out;
        for (const cbf::ne::Segment& s : module(*this).segments()) {
            Segment o;
            o.idx = s.idx;
            o.file_off = s.file_off;
            o.length = s.length;
            o.flags = s.flags;
            o.minalloc = s.minalloc;
            o.kind = s.is_data() ? "DATA" : "CODE";
            out.append(o);
        }
        return out;
    });
}

// Parse per-segment relocation records (if RELOCINFO set).
py::list<Relocation> NEFile::relocations(const Segment& seg) const {
    return guard([&] {
        cbf::ne::Module m = module(*this);
        std::vector<cbf::ne::Relocation> recs;
        if (seg.flags & cbf::ne::kRelocInfo) recs = m.relocations(uint16_t(seg.flags), seg.file_off, seg.length);
        py::list<Relocation> out;
        for (const cbf::ne::Relocation& r : recs) {
            Relocation o;
            o.src = r.src;
            o.rel_type = r.rel_type;
            o.additive = r.additive();
            o.offset = r.offset;
            switch (r.kind) {
            case cbf::ne::Relocation::Internal:
                o.kind = "INTERNAL";
                o.seg = r.seg;
                o.entry = r.entry;
                break;
            case cbf::ne::Relocation::ImportOrdinal:
                o.kind = "IMPORTORD";
                o.module = r.module;
                o.ordinal = r.ordinal;
                break;
            case cbf::ne::Relocation::ImportName:
                o.kind = "IMPORTNAME";
                o.module = r.module;
                o.name_off = r.name_off;
                break;
            case cbf::ne::Relocation::OsFixup:
                o.kind = "OSFIXUP";
                o.ftype = r.ftype;
                break;
            }
            out.append(o);
        }
        return out;
    });
}

// The ordinals the entry table actually exports, in order (unused gaps skipped).
py::list<int64_t> NEFile::export_ordinals() const {
    return guard([&] {
        py::list<int64_t> out;
        for (int64_t v : module(*this).export_ordinals()) out.append(v);
        return out;
    });
}

// Parse entry-table bundles -> list of dict(ordinal, seg, offset, flags).
py::list<Entry> NEFile::entries() const {
    return guard([&] {
        py::list<Entry> out;
        for (const cbf::ne::Entry& e : module(*this).entries()) {
            Entry o;
            o.ordinal = e.ordinal;
            if (e.kind == cbf::ne::Entry::Unused) {
                o.kind = "UNUSED";
            } else {
                o.kind = e.kind == cbf::ne::Entry::Moveable ? "MOVEABLE" : "FIXED";
                o.seg = e.seg;
                o.offset = e.offset;
                o.flags = e.flags;
            }
            out.append(o);
        }
        return out;
    });
}

py::list<Resource> NEFile::resources() const {
    return guard([&] {
        py::list<Resource> out;
        for (const cbf::ne::Resource& r : module(*this).resources()) {
            Resource o;
            o.type = T(r.type);
            o.id = T(r.id);
            o.off = r.off;
            o.length = r.length;
            o.flags = r.flags;
            out.append(o);
        }
        return out;
    });
}

py::str NEFile::_rsrc_string(int64_t base_, int64_t off) const {
    return guard([&] { return T(module(*this).rsrc_string(base_, off)); });
}

// ---- BootImageModule -----------------------------------------------------------------------------------

BootImageModule BootImageModule::from_buffer(const py::bytes& data, int64_t ne_off, const py::str& path) {
    BootImageModule self;
    init_buffer(self, data, ne_off, path);
    return self;
}

// ---- module functions ----------------------------------------------------------------------------------

std::tuple<int64_t, int64_t, std::optional<int64_t>> where(const NEFile& m) {
    return std::make_tuple(m.ne_off, m.base, m.overlay_place());
}

std::tuple<py::bytes, std::tuple<int64_t, int64_t, std::optional<int64_t>>> _place(const NEFile& nef) {
    return std::make_tuple(nef.data, where(nef));
}

py::list<std::tuple<int64_t, py::str>> embedded(const py::bytes& data, int64_t start) {
    return guard([&] {
        py::list<std::tuple<int64_t, py::str>> out;
        for (const auto& e : cbf::ne::embedded(view(data), start)) out.append(std::make_tuple(e.first, T(e.second)));
        return out;
    });
}

py::dict<py::str, BootImageModule> open_boot_image(const py::str& path, const std::optional<py::str>& overlay_) {
    std::optional<py::str> overlay = overlay_;
    if (!overlay) {
        py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(path))).lower();
        py::str folder = py::os::path::dirname(py::os::path::abspath(path));
        for (const py::str& f : py::os::listdir(folder))
            if (f.lower() == stem + py::str(".ovl")) {
                overlay = py::os::path::join(folder, f);
                break;
            }
    }
    py::file rf = py::open_rb(path);
    py::bytes resident = rf.read();
    rf.close();
    py::bytes extra;
    if (overlay && !overlay->u32().empty()) {
        py::file of = py::open_rb(*overlay);
        extra = of.read();
        of.close();
    }
    py::dict<py::str, BootImageModule> out;
    for (const auto& e : embedded(resident)) {
        BootImageModule m = BootImageModule::from_buffer(py::bytes(resident.raw() + extra.raw()), std::get<0>(e), path);
        m.base = 0;
        m.overlay_at = resident.size();
        if (!out.contains(std::get<1>(e))) out.set(std::get<1>(e), m);   // out.setdefault(name, m)
    }
    return out;
}

// Offset just past the header and every table that belongs with it.
int64_t _tables_end(const NEFile& nef) {
    return guard([&] { return cbf::ne::tables_end(module(nef)); });
}

// The bytes of an ordinary one-file executable holding the module `nef`.
py::bytes standalone(const NEFile& nef) {
    return guard([&] { return py::bytes(cbf::ne::standalone(module(nef))); });
}

}  // namespace codebreaker::formats::ne
