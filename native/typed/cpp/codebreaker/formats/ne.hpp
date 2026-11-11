// codebreaker/formats/ne.hpp -- COMPAT shim for the old typed port of codebreaker.formats.ne (deleted when its last py:: consumer is
// refactored). The reader is native/cb/formats/ne.hpp (cb::formats::ne::Module); this header only converts. No logic.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/ne.hpp"
#include "cb/formats/read.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::ne {

inline constexpr int64_t PRELOAD = 0x0040;  // segment / resource flag: loaded at start (else on demand)

struct Header {
    int64_t linker_ver = 0, linker_rev = 0, entry_off = 0, entry_len = 0, crc = 0, flags = 0, autodata_seg = 0, heap = 0, stack = 0, csip = 0,
            sssp = 0, n_segs = 0, n_modrefs = 0, nonres_size = 0, seg_tab = 0, rsrc_tab = 0, resname_tab = 0, mod_tab = 0, imp_tab = 0,
            nonres_off = 0, n_movable = 0, align_shift = 0, n_resources = 0, exetype = 0;
    std::optional<int64_t> expect_winver;
};

struct Segment {
    int64_t idx = 0, file_off = 0, length = 0, flags = 0, minalloc = 0;
    py::str kind;  // 'DATA' | 'CODE'
};

struct Relocation {
    int64_t src = 0, rel_type = 0;
    bool additive = false;
    int64_t offset = 0;
    py::str kind;  // INTERNAL, IMPORTORD, IMPORTNAME or OSFIXUP
    std::optional<int64_t> seg, entry, module, ordinal, name_off, ftype;
};

struct Entry {
    int64_t ordinal = 0;
    py::str kind;  // 'UNUSED' | 'MOVEABLE' | 'FIXED'
    std::optional<int64_t> seg, offset, flags;
};

struct Resource {
    py::str type, id;
    int64_t off = 0, length = 0, flags = 0;
};

inline Header header_of(const cb::formats::ne::Header& h) {
    Header o;
    o.linker_ver = h.linker_version, o.linker_rev = h.linker_revision, o.entry_off = h.entry_table_offset, o.entry_len = h.entry_table_length;
    o.crc = h.crc, o.flags = h.flags, o.autodata_seg = h.autodata_segment, o.heap = h.heap_size, o.stack = h.stack_size, o.csip = h.cs_ip;
    o.sssp = h.ss_sp, o.n_segs = h.segment_count, o.n_modrefs = h.module_ref_count, o.nonres_size = h.nonresident_size;
    o.seg_tab = h.segment_table, o.rsrc_tab = h.resource_table, o.resname_tab = h.resident_names_table, o.mod_tab = h.module_ref_table;
    o.imp_tab = h.imported_names_table, o.nonres_off = h.nonresident_offset, o.n_movable = h.movable_entries, o.align_shift = h.alignment_shift;
    o.n_resources = h.resource_segments, o.exetype = h.target_os;
    if (h.expected_windows_version) o.expect_winver = *h.expected_windows_version;
    return o;
}

class NEFile {
  public:
    int64_t base = 0;  // byte added to segment sector offsets (nonzero for embedded modules)
    py::str path;
    py::bytes data;
    int64_t ne_off = 0;
    Header h;

    explicit NEFile(const py::str& path_) : path(path_) {
        py::file f = py::open_rb(path_);
        data = f.read();
        f.close();
        cb::compat::guard([&] { m_ = std::make_shared<cb::formats::ne::Module>(cb::formats::ne::Module::open(cb::compat::from_py(data))); });
        adopt();
    }
    virtual ~NEFile() = default;

    static NEFile from_buffer(const py::bytes& data, int64_t ne_off, const py::str& path = py::str("<buffer>")) {
        NEFile f;
        f.path = path;
        f.data = data;
        cb::compat::guard([&] {
            f.m_ = std::make_shared<cb::formats::ne::Module>(cb::formats::ne::Module::parse(cb::compat::from_py(data), static_cast<uint64_t>(ne_off)));
        });
        f.adopt();
        return f;
    }

    int64_t u16(int64_t off) const { return static_cast<int64_t>(cb::formats::get<uint16_t>(m().data(), static_cast<uint64_t>(off), "ne")); }
    int64_t u32(int64_t off) const { return static_cast<int64_t>(cb::formats::get<uint32_t>(m().data(), static_cast<uint64_t>(off), "ne")); }

    py::list<std::tuple<py::str, int64_t>> resident_names() const { return names_of(m().resident_names()); }
    py::list<std::tuple<py::str, int64_t>> nonresident_names() const { return names_of(m().nonresident_names()); }
    py::str module_name() const { return cb::compat::to_py(m().module_name()); }
    py::list<py::str> module_refs() const { return cb::compat::guard([&] { return cb::compat::to_py(m().module_refs()); }); }
    py::str imported_name(int64_t off) const { return cb::compat::to_py(m().imported_name(static_cast<uint64_t>(off))); }
    py::list<Segment> segments() const {
        return cb::compat::guard([&] {
            py::list<Segment> out;
            for (const auto& s : m().segments()) out.append(Segment{s.index, static_cast<int64_t>(s.file_offset), s.length, s.flags, s.min_alloc, py::str(s.is_data() ? "DATA" : "CODE")});
            return out;
        });
    }
    py::list<Relocation> relocations(const Segment& seg) const {
        cb::formats::ne::Segment s;
        s.file_offset = static_cast<uint64_t>(seg.file_off), s.length = static_cast<uint32_t>(seg.length), s.flags = static_cast<uint16_t>(seg.flags);
        py::list<Relocation> out;
        for (const auto& r : m().relocations(s)) {
            Relocation o;
            o.src = r.source, o.rel_type = r.flags, o.additive = r.additive(), o.offset = r.offset;
            if (auto* a = std::get_if<cb::formats::ne::InternalRef>(&r.target)) o.kind = py::str("INTERNAL"), o.seg = a->segment, o.entry = a->entry;
            else if (auto* b = std::get_if<cb::formats::ne::ImportOrdinalRef>(&r.target)) o.kind = py::str("IMPORTORD"), o.module = b->module, o.ordinal = b->ordinal;
            else if (auto* c = std::get_if<cb::formats::ne::ImportNameRef>(&r.target)) o.kind = py::str("IMPORTNAME"), o.module = c->module, o.name_off = c->name_offset;
            else o.kind = py::str("OSFIXUP"), o.ftype = std::get<cb::formats::ne::OsFixupRef>(r.target).fixup_type;
            out.append(o);
        }
        return out;
    }
    py::list<int64_t> export_ordinals() const {
        return cb::compat::guard([&] {
            py::list<int64_t> out;
            for (uint32_t o : m().export_ordinals()) out.append(static_cast<int64_t>(o));
            return out;
        });
    }
    py::list<Entry> entries() const {
        return cb::compat::guard([&] {
            py::list<Entry> out;
            for (const auto& e : m().entries()) {
                Entry o;
                o.ordinal = e.ordinal;
                o.kind = py::str(e.kind == cb::formats::ne::Entry::Kind::Unused ? "UNUSED" : e.kind == cb::formats::ne::Entry::Kind::Moveable ? "MOVEABLE" : "FIXED");
                if (e.kind != cb::formats::ne::Entry::Kind::Unused) o.seg = e.segment, o.offset = e.offset, o.flags = e.flags;
                out.append(o);
            }
            return out;
        });
    }
    py::list<Resource> resources() const {
        return cb::compat::guard([&] {
            py::list<Resource> out;
            for (const auto& r : m().resources())
                out.append(Resource{cb::compat::to_py(r.type), cb::compat::to_py(r.id), static_cast<int64_t>(r.file_offset), static_cast<int64_t>(r.length), r.flags});
            return out;
        });
    }

    // the overlay's place when this is a boot image's module (BootImageModule), else None
    virtual std::optional<int64_t> overlay_place() const { return std::nullopt; }

    const cb::formats::ne::Module& module() const { return m(); }

  protected:
    NEFile() = default;
    void adopt() {
        ne_off = static_cast<int64_t>(m_->ne_offset());
        base = static_cast<int64_t>(m_->base());
        h = header_of(m_->header());
    }
    const cb::formats::ne::Module& m() const {
        m_->set_base(static_cast<uint64_t>(base));  // `base` is public and may have been set by the caller
        return *m_;
    }
    std::shared_ptr<cb::formats::ne::Module> m_;

  private:
    static py::list<std::tuple<py::str, int64_t>> names_of(const std::vector<cb::formats::ne::Name>& names) {
        py::list<std::tuple<py::str, int64_t>> out;
        for (const auto& n : names) out.append(std::make_tuple(cb::compat::to_py(n.name), static_cast<int64_t>(n.ordinal)));
        return out;
    }
};

class BootImageModule : public NEFile {
  public:
    int64_t overlay_at = 0;

    static BootImageModule from_buffer(const py::bytes& data, int64_t ne_off, const py::str& path = py::str("<buffer>")) {
        BootImageModule f;
        f.path = path;
        f.data = data;
        cb::compat::guard([&] {
            f.m_ = std::make_shared<cb::formats::ne::Module>(cb::formats::ne::Module::parse(cb::compat::from_py(data), static_cast<uint64_t>(ne_off)));
        });
        f.adopt();
        return f;
    }
    std::optional<int64_t> overlay_place() const override { return overlay_at; }
    // where the overlay starts in `data` (the module then reads its on-demand segments from there)
    void set_overlay_at(int64_t at) {
        overlay_at = at;
        cb::compat::guard([&] {
            m_ = std::make_shared<cb::formats::ne::Module>(
                cb::formats::ne::Module::parse_boot_module(cb::compat::from_py(data), static_cast<uint64_t>(ne_off), static_cast<uint64_t>(at)));
        });
    }

  protected:
    BootImageModule() = default;
};

inline std::tuple<int64_t, int64_t, std::optional<int64_t>> where(const NEFile& m) {
    return std::make_tuple(m.ne_off, m.base, m.overlay_place());
}

inline py::list<std::tuple<int64_t, py::str>> embedded(const py::bytes& data, int64_t start = 0x40) {
    return cb::compat::guard([&] {
        py::list<std::tuple<int64_t, py::str>> out;
        const cb::Bytes d = cb::compat::from_py(data);
        for (const auto& [off, name] : cb::formats::ne::embedded(d, static_cast<uint64_t>(start))) out.append(std::make_tuple(static_cast<int64_t>(off), cb::compat::to_py(name)));
        return out;
    });
}

inline py::dict<py::str, BootImageModule> open_boot_image(const py::str& path, const std::optional<py::str>& overlay = std::nullopt) {
    return cb::compat::guard([&] {
        py::dict<py::str, BootImageModule> out;
        std::optional<std::filesystem::path> ovl;
        if (overlay && !overlay->utf8().empty()) ovl = cb::fs::from_text(overlay->utf8());
        const auto modules = cb::formats::ne::open_boot_image_files(cb::fs::from_text(path.utf8()), ovl);
        if (modules.empty()) return out;
        const py::bytes both = cb::compat::to_py(modules[0].second.data());  // resident file followed by the overlay
        for (const auto& [name, mod] : modules) {
            BootImageModule b = BootImageModule::from_buffer(both, static_cast<int64_t>(mod.ne_offset()), path);
            b.set_overlay_at(static_cast<int64_t>(*mod.overlay_at()));
            out.set(cb::compat::to_py(name), b);
        }
        return out;
    });
}

inline int64_t _tables_end(const NEFile& nef) { return cb::compat::guard([&] { return static_cast<int64_t>(nef.module().tables_end()); }); }

inline py::bytes standalone(const NEFile& nef) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(nef.module().standalone())); });
}

}  // namespace codebreaker::formats::ne
