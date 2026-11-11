// codebreaker/formats/le.hpp -- COMPAT shim for the old typed port of codebreaker.formats.le (deleted when its last py:: consumer is
// refactored). The reader is native/cb/formats/le.hpp (cb::formats::le::Module and friends); this header only converts the records
// the old callers use and keeps the old function names. No logic. `corpus_files` needs the corpus index of codebreaker.base.corpus
// and the detector, which are not refactored yet: it stays here, tied to them.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/le.hpp"
#include "cb/formats/read.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/detect.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::le {

namespace cle = cb::formats::le;
using Place = std::tuple<int64_t, int64_t>;  // (object, offset)

struct Object {
    int64_t idx = 0;  // 1-based
    int64_t vsize = 0, base = 0, flags = 0, pmidx = 0, pcnt = 0;
    bool exec = false, read = false, write = false;
};

struct InternalFixup {
    int64_t obj = 0;
    std::optional<int64_t> target;  // None: a 16-bit selector
    int64_t srctype = 0;
};
struct ImportFixup {
    int64_t module = 0;
    int64_t ordinal = 0;
};
using Fixup = std::variant<InternalFixup, ImportFixup>;

struct Header {
    py::str signature;
    int64_t cpu = 0, os = 0, module_version = 0, module_flags = 0, pages = 0, entry_object = 0, entry = 0, stack_object = 0, stack = 0, page_size = 0,
            last_page_bytes = 0, fixup_section_size = 0, loader_section_size = 0, objects = 0, imported_modules = 0, data_pages_offset = 0, device_id = 0,
            ddk_version = 0;
};

struct Ddb {
    int64_t obj = 0, rva = 0, version = 0, device_id = 0, major = 0, minor = 0, flags = 0;
    py::str name;
    int64_t init_order = 0, control_proc = 0;
};

struct FixupRecord {
    int64_t source = 0, source_flags = 0, flags = 0;
    py::str kind;                       // 'internal', 'import', 'import-name', 'entry'
    std::optional<int64_t> obj;         // internal
    std::optional<int64_t> target;      // internal (None: a selector)
    std::optional<int64_t> module;      // import, import-name
    std::optional<int64_t> ordinal;     // import
    std::optional<int64_t> name;        // import-name
    std::optional<int64_t> entry;       // entry
    std::optional<int64_t> addend;
    py::list<int64_t> sources;
    py::bytes raw;
};

struct Descriptor {
    int64_t obj = 0, offset = 0;
    py::str name;
    int64_t next = 0, kit_version = 0, device_id = 0, major = 0, minor = 0, flags = 0, init_order = 0;
    std::optional<Place> control, v86_api, pm_api;
    int64_t v86_api_address = 0, pm_api_address = 0;
    std::optional<Place> reference_data, service_table;
    int64_t service_count = 0;
    int64_t control_raw = 0, v86_api_raw = 0, pm_api_raw = 0, service_table_raw = 0, reference_data_raw = 0;
    int64_t size = 0;  // 0x50 or 0x34
    py::bytes bytes;
};

struct ImageObject {
    int64_t vsize = 0, base = 0, flags = 0, first_page = 0, page_count = 0;
    std::optional<py::bytes> reserved;  // absent: four zero bytes
};
using NameList = py::list<std::tuple<py::str, int64_t>>;
using NameTable = std::variant<NameList, py::bytes>;
struct Moved {
    int64_t debug_info = 0, resource_data = 0, from = 0;
};
struct Image {
    py::bytes stub, header;
    py::list<ImageObject> objects;
    py::list<std::tuple<int64_t, py::bytes>> pages;  // (kind, bytes)
    NameTable resident, nonresident;
    py::bytes entry_table;
    py::dict<int64_t, py::list<py::bytes>> fixups;  // {page: [record bytes]}
    py::list<py::str> import_modules;
    py::bytes import_names;
    std::optional<int64_t> align;
    std::optional<int64_t> last_page_bytes;
    std::optional<std::tuple<py::bytes, py::bytes>> gaps;
    py::bytes after;
    Moved moved;
    std::optional<py::bytes> page_map;
    std::optional<py::bytes> tables_tail;
    std::optional<int64_t> data_pages;
    std::optional<py::dict<py::str, int64_t>> in_pages;
    std::optional<py::bytes> fill_from;
};

struct Container {
    py::str kind;  // 'W3' or 'W4'
    int64_t at = 0, version = 0;
    std::optional<py::bytes> reserved;                        // W3 only
    py::list<std::tuple<py::str, int64_t, int64_t>> members;  // (name, offset of the member's LE header, size)
};

inline constexpr int64_t HEADER_SIZE = 0xC4;
inline constexpr int64_t MODULE_STATIC = 0x00000000, MODULE_DYNAMIC = 0x00008000, MODULE_OLD = 0x00010000;  // module flags of a driver

// ---- conversions ----------------------------------------------------------------------------------------------------------------
namespace conv {
inline Place place(const cle::Place& p) { return Place(p.object, p.offset); }
inline cle::Place place(const Place& p) { return cle::Place{static_cast<uint32_t>(std::get<0>(p)), std::get<1>(p)}; }
inline std::optional<Place> place(const std::optional<cle::Place>& p) { return p ? std::optional<Place>(place(*p)) : std::nullopt; }
inline Object object(const cle::Object& o) {
    return Object{o.index, o.vsize, o.base, o.flags, o.first_page, o.page_count, o.executable(), o.readable(), o.writable()};
}
inline cle::Object object(const Object& o) {
    return cle::Object{static_cast<uint32_t>(o.idx), static_cast<uint32_t>(o.vsize), static_cast<uint32_t>(o.base), static_cast<uint32_t>(o.flags),
                       static_cast<uint32_t>(o.pmidx), static_cast<uint32_t>(o.pcnt)};
}
inline Fixup fixup(const cle::Fixup& f) {
    if (auto* i = std::get_if<cle::InternalFixup>(&f)) {
        InternalFixup o;
        o.obj = i->object, o.srctype = i->source_type;
        if (i->target) o.target = *i->target;
        return o;
    }
    const auto& m = std::get<cle::ImportFixup>(f);
    return ImportFixup{m.module, m.ordinal};
}
inline NameList names(const std::vector<cle::NameOrdinal>& v) {
    NameList out;
    for (const auto& n : v) out.append(std::make_tuple(cb::compat::to_py(n.name), static_cast<int64_t>(n.ordinal)));
    return out;
}
inline std::vector<cle::NameOrdinal> names(const NameList& l) {
    std::vector<cle::NameOrdinal> out;
    for (const auto& t : l) out.push_back({cb::compat::from_py(std::get<0>(t)), static_cast<uint16_t>(std::get<1>(t))});
    return out;
}
inline FixupRecord record(const cle::FixupRecord& r) {
    FixupRecord o;
    o.source = r.source, o.source_flags = r.source_flags, o.flags = r.flags;
    switch (r.kind) {
        case cle::FixupRecord::Kind::Internal:
            o.kind = py::str("internal"), o.obj = r.first;
            if (r.target) o.target = *r.target;
            break;
        case cle::FixupRecord::Kind::Import: o.kind = py::str("import"), o.module = r.first, o.ordinal = r.ordinal; break;
        case cle::FixupRecord::Kind::ImportName: o.kind = py::str("import-name"), o.module = r.first, o.name = r.name; break;
        case cle::FixupRecord::Kind::Entry: o.kind = py::str("entry"), o.entry = r.first; break;
    }
    if (r.addend) o.addend = *r.addend;
    for (int16_t s : r.sources) o.sources.append(s);
    o.raw = cb::compat::to_py(cb::ByteView(r.raw));
    return o;
}
inline cle::FixupRecord record(const FixupRecord& r) {
    cle::FixupRecord o;
    o.source = static_cast<uint8_t>(r.source), o.source_flags = static_cast<uint8_t>(r.source_flags), o.flags = static_cast<uint8_t>(r.flags);
    const std::string k = r.kind.utf8();
    if (k == "internal") o.kind = cle::FixupRecord::Kind::Internal, o.first = static_cast<uint32_t>(r.obj.value_or(0));
    else if (k == "import") o.kind = cle::FixupRecord::Kind::Import, o.first = static_cast<uint32_t>(r.module.value_or(0));
    else if (k == "import-name") o.kind = cle::FixupRecord::Kind::ImportName, o.first = static_cast<uint32_t>(r.module.value_or(0));
    else o.kind = cle::FixupRecord::Kind::Entry, o.first = static_cast<uint32_t>(r.entry.value_or(0));
    if (r.target) o.target = static_cast<uint32_t>(*r.target);
    o.ordinal = static_cast<uint32_t>(r.ordinal.value_or(0)), o.name = static_cast<uint32_t>(r.name.value_or(0));
    if (r.addend) o.addend = static_cast<uint32_t>(*r.addend);
    for (int64_t s : r.sources.vec()) o.sources.push_back(static_cast<int16_t>(s));
    o.raw = cb::compat::from_py(r.raw);
    return o;
}
inline Ddb ddb(const cle::Ddb& d) {
    return Ddb{d.object, d.rva, d.version, d.device_id, d.major, d.minor, d.flags, cb::compat::to_py(d.name), d.init_order, d.control_proc};
}
inline Descriptor descriptor(const cle::Descriptor& d) {
    Descriptor o;
    o.obj = d.where.object, o.offset = d.where.offset;
    o.name = cb::compat::to_py(d.name);
    o.next = d.next, o.kit_version = d.kit_version, o.device_id = d.device_id, o.major = d.major, o.minor = d.minor, o.flags = d.flags;
    o.init_order = d.init_order;
    o.control = place(d.at_control), o.v86_api = place(d.at_v86_api), o.pm_api = place(d.at_pm_api);
    o.v86_api_address = d.v86_api_address, o.pm_api_address = d.pm_api_address;
    o.reference_data = place(d.at_reference_data), o.service_table = place(d.at_service_table);
    o.service_count = d.service_count;
    o.control_raw = d.control, o.v86_api_raw = d.v86_api, o.pm_api_raw = d.pm_api, o.service_table_raw = d.service_table, o.reference_data_raw = d.reference_data;
    o.size = static_cast<int64_t>(d.size);
    o.bytes = cb::compat::to_py(cb::ByteView(d.bytes));
    return o;
}
inline cle::Descriptor descriptor(const Descriptor& d) {  // what service_table() needs from a descriptor the caller holds
    cle::Descriptor o;
    o.where = cle::Place{static_cast<uint32_t>(d.obj), d.offset};
    if (d.service_table) o.at_service_table = place(*d.service_table);
    o.service_count = static_cast<uint32_t>(d.service_count);
    return o;
}
inline Container container(const cle::Container& c) {
    Container o;
    o.kind = py::str(c.kind == cle::Container::Kind::W3 ? "W3" : "W4");
    o.at = static_cast<int64_t>(c.at), o.version = c.version;
    if (c.kind == cle::Container::Kind::W3) o.reserved = cb::compat::to_py(cb::ByteView(c.reserved));
    for (const auto& m : c.members) o.members.append(std::make_tuple(cb::compat::to_py(m.name), static_cast<int64_t>(m.offset), static_cast<int64_t>(m.size)));
    return o;
}
}  // namespace conv

// ---- the module ------------------------------------------------------------------------------------------------------------------
class W3File;

class LEFile {
  public:
    LEFile(const py::bytes& data, int64_t lfa) : LEFile(std::make_shared<const py::bytes>(data), lfa) {}
    // (the bytes shared, as members of a container are)
    LEFile(std::shared_ptr<const py::bytes> data, int64_t lfa) : data_(std::move(data)) {
        cb::compat::guard([&] { m_ = std::make_shared<cle::Module>(cle::Module::parse(cb::compat::from_py(*data_), static_cast<uint64_t>(lfa))); });
        adopt();
    }
    static LEFile from_buffer(const py::bytes& data) {
        return cb::compat::guard([&] {
            auto m = cle::Module::from_buffer(cb::compat::from_py(data));
            return LEFile(data, static_cast<int64_t>(m.lfa()));
        });
    }
    static LEFile from_file(const py::str& path) {
        py::file f = py::open_rb(path);
        py::bytes b = f.read();
        f.close();
        return from_buffer(b);
    }

    const py::bytes& d() const { return *data_; }
    std::shared_ptr<const py::bytes> shared_data() const { return data_; }

    int64_t lfa = 0;
    py::bytes sig;
    int64_t cpu = 0, ostype = 0, mpages = 0, eip_obj = 0, eip = 0, pagesize = 0, last_page_bytes = 0, objtab = 0, nobj = 0, pagemap = 0, resnames = 0,
            entrytab = 0, fixup_page = 0, fixup_rec = 0, datapages = 0;
    py::list<Object> objects;

    py::bytes object_bytes(const Object& obj) const {
        return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(m_->object_data(conv::object(obj)))); });
    }
    std::tuple<std::optional<Object>, int64_t> page_to_obj(int64_t pg) const {
        auto r = pg < 0 ? std::nullopt : m_->page_to_object(static_cast<uint32_t>(pg));
        if (!r) return std::make_tuple(std::optional<Object>(), int64_t{0});
        return std::make_tuple(std::optional<Object>(conv::object(*r->first)), static_cast<int64_t>(r->second));
    }
    py::dict<Place, Fixup> fixups() const {
        return cb::compat::guard([&] {
            py::dict<Place, Fixup> out;
            for (const auto& kv : m_->fixups()) out.set(conv::place(kv.first), conv::fixup(kv.second));
            return out;
        });
    }
    py::list<py::str> import_modules() const { return cb::compat::guard([&] { return cb::compat::to_py(m_->import_modules()); }); }
    py::str resident_name() const { return cb::compat::guard([&] { return cb::compat::to_py(m_->resident_name()); }); }
    Header header() const {
        const auto& h = m_->header();
        Header o;
        o.signature = cb::compat::to_py(std::string(m_->signature()));
        o.cpu = h.cpu, o.os = h.os, o.module_version = h.version, o.module_flags = h.module_flags, o.pages = h.pages, o.entry_object = h.eip_object, o.entry = h.eip;
        o.stack_object = h.esp_object, o.stack = h.esp, o.page_size = h.page_size, o.last_page_bytes = h.last_page_bytes, o.fixup_section_size = h.fixup_size;
        o.loader_section_size = h.loader_size, o.objects = h.objects, o.imported_modules = h.import_module_count, o.data_pages_offset = h.data_pages;
        o.device_id = h.device_id, o.ddk_version = h.kit_version;
        return o;
    }
    py::str description() const { return cb::compat::guard([&] { return cb::compat::to_py(m_->description()); }); }
    std::optional<Ddb> ddb() const {
        return cb::compat::guard([&]() -> std::optional<Ddb> {
            auto d = m_->ddb();
            return d ? std::optional<Ddb>(conv::ddb(*d)) : std::nullopt;
        });
    }

    const cle::Module& module() const { return *m_; }

  private:
    void adopt() {
        const auto& h = m_->header();
        lfa = static_cast<int64_t>(m_->lfa());
        sig = py::bytes(std::string(m_->signature()));
        cpu = h.cpu, ostype = h.os, mpages = h.pages, eip_obj = h.eip_object, eip = h.eip, pagesize = h.page_size, last_page_bytes = h.last_page_bytes;
        nobj = h.objects, datapages = h.data_pages;
        objtab = lfa + h.object_table, pagemap = lfa + h.page_map, resnames = lfa + h.resident_names, entrytab = lfa + h.entry_table;
        fixup_page = lfa + h.fixup_pages, fixup_rec = lfa + h.fixup_records;
        for (const auto& o : m_->objects()) objects.append(conv::object(o));
    }
    std::shared_ptr<const py::bytes> data_;
    std::shared_ptr<cle::Module> m_;
};

struct W3Entry {
    py::str name;
    int64_t off = 0, size = 0;
};

class W3File {
  public:
    W3File(const py::bytes& data, int64_t lfa_) : data_(std::make_shared<const py::bytes>(data)), lfa(lfa_) {
        cb::compat::guard([&] {
            auto c = cle::container(cb::compat::from_py(data));
            if (!c || c->kind != cle::Container::Kind::W3) throw cb::FormatError("le", static_cast<uint64_t>(lfa_), "not a W3 container");
            version = c->version;
            count = static_cast<int64_t>(c->members.size());
            for (const auto& m : c->members) entries.append(W3Entry{cb::compat::to_py(m.name), m.offset, m.size});
        });
    }
    py::list<std::tuple<py::str, LEFile>> modules() const {
        py::list<std::tuple<py::str, LEFile>> out;
        for (const auto& e : entries) {
            const auto d = cb::compat::from_py(*data_);
            if (cb::formats::starts_with(d, static_cast<uint64_t>(e.off), "LE") || cb::formats::starts_with(d, static_cast<uint64_t>(e.off), "LX"))
                out.append(std::make_tuple(e.name, LEFile(data_, e.off)));
        }
        return out;
    }
    static W3File from_file(const py::str& path) {
        py::file f = py::open_rb(path);
        py::bytes b = f.read();
        f.close();
        const cb::Bytes d = cb::compat::from_py(b);
        return W3File(b, static_cast<int64_t>(cb::formats::get<uint32_t>(d, 0x3C, "le")));
    }

    const py::bytes& d() const { return *data_; }
    int64_t lfa = 0;
    int64_t version = 0, count = 0;
    py::list<W3Entry> entries;

  private:
    std::shared_ptr<const py::bytes> data_;
};

// ---- the module-level functions -------------------------------------------------------------------------------------------------
inline py::list<std::tuple<py::str, LEFile>> open_vxds(const py::str& path) {
    return cb::compat::guard([&] {
        py::file f = py::open_rb(path);
        py::bytes b = f.read();
        f.close();
        auto shared = std::make_shared<const py::bytes>(b);
        py::list<std::tuple<py::str, LEFile>> out;
        for (const auto& [name, mod] : cle::modules(cb::compat::from_py(b), cb::fs::from_text(path.utf8()).filename().string()))
            out.append(std::make_tuple(cb::compat::to_py(name), LEFile(shared, static_cast<int64_t>(mod.lfa()))));
        return out;
    });
}

inline py::dict<py::str, int64_t> header_fields(const LEFile& lf) {
    const auto& h = lf.module().header();
    py::dict<py::str, int64_t> out;
    auto put = [&](const char* k, int64_t v) { out.set(py::str(k), v); };
    put("byte_order", h.byte_order), put("word_order", h.word_order), put("level", h.level), put("cpu", h.cpu), put("os", h.os), put("version", h.version);
    put("module_flags", h.module_flags), put("pages", h.pages), put("eip_object", h.eip_object), put("eip", h.eip), put("esp_object", h.esp_object);
    put("esp", h.esp), put("page_size", h.page_size), put("last_page_bytes", h.last_page_bytes), put("fixup_size", h.fixup_size), put("loader_size", h.loader_size);
    put("object_table", h.object_table), put("objects", h.objects), put("page_map", h.page_map), put("resources", h.resources);
    put("resident_names", h.resident_names), put("entry_table", h.entry_table), put("fixup_pages", h.fixup_pages), put("fixup_records", h.fixup_records);
    put("import_modules", h.import_modules), put("import_module_count", h.import_module_count), put("import_names", h.import_names);
    put("data_pages", h.data_pages), put("preload_pages", h.preload_pages), put("nonresident_names", h.nonresident_names), put("nonresident_size", h.nonresident_size);
    put("debug_info", h.debug_info), put("debug_size", h.debug_size), put("resource_data", h.resource_data), put("resource_size", h.resource_size);
    put("device_id", h.device_id), put("kit_version", h.kit_version);
    return out;
}

inline bool is_member(const LEFile& lf) { return cb::compat::guard([&] { return lf.module().is_member(); }); }

inline py::list<std::tuple<int64_t, int64_t>> page_map(const LEFile& lf) {
    return cb::compat::guard([&] {
        py::list<std::tuple<int64_t, int64_t>> out;
        for (const auto& e : lf.module().page_map()) out.append(std::make_tuple<int64_t, int64_t>(e.number, e.kind));
        return out;
    });
}

inline py::bytes object_data(const LEFile& lf, const Object& obj) { return lf.object_bytes(obj); }

inline std::optional<int64_t> file_offset(const LEFile& lf, int64_t obj_index, int64_t off) {
    if (obj_index < 0) return std::nullopt;
    auto r = lf.module().file_offset(static_cast<uint32_t>(obj_index), off);
    return r ? std::optional<int64_t>(static_cast<int64_t>(*r)) : std::nullopt;
}

inline py::list<std::tuple<py::str, int64_t>> _names(const py::bytes& d, int64_t p, std::optional<int64_t> end = std::nullopt) {
    const cb::Bytes bytes = cb::compat::from_py(d);
    return conv::names(cle::names_at(bytes, static_cast<uint64_t>(p < 0 ? 0 : p), end ? std::optional<uint64_t>(static_cast<uint64_t>(*end)) : std::nullopt));
}

inline std::tuple<NameList, NameList> names(const LEFile& lf) {
    return cb::compat::guard([&] {
        auto n = lf.module().names();
        return std::make_tuple(conv::names(n.first), conv::names(n.second));
    });
}

inline py::list<std::tuple<int64_t, int64_t, int64_t, int64_t>> entries(const LEFile& lf) {
    return cb::compat::guard([&] {
        py::list<std::tuple<int64_t, int64_t, int64_t, int64_t>> out;
        for (const auto& e : lf.module().entries()) out.append(std::make_tuple<int64_t, int64_t, int64_t, int64_t>(e.ordinal, e.object, e.offset, e.flags));
        return out;
    });
}

inline int64_t _entry_table_size(const LEFile& lf) { return static_cast<int64_t>(lf.module().entry_table_size()); }

inline py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> exports(const LEFile& lf) {
    return cb::compat::guard([&] {
        py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> out;
        for (const auto& e : lf.module().exports())
            out.append(std::make_tuple(cb::compat::to_py(e.name.value_or("")), static_cast<int64_t>(e.ordinal), static_cast<int64_t>(e.object), static_cast<int64_t>(e.offset)));
        return out;
    });
}

inline py::list<std::tuple<int64_t, FixupRecord>> fixup_records(const LEFile& lf) {
    return cb::compat::guard([&] {
        py::list<std::tuple<int64_t, FixupRecord>> out;
        for (const auto& r : lf.module().fixup_records()) out.append(std::make_tuple(static_cast<int64_t>(r.page), conv::record(r)));
        return out;
    });
}

inline py::bytes encode_fixup(const FixupRecord& rec) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cle::encode_fixup(conv::record(rec)))); });
}

inline py::list<std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t>> references(const LEFile& lf) {
    return cb::compat::guard([&] {
        py::list<std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t>> out;
        for (const auto& r : lf.module().references())
            out.append(std::make_tuple(static_cast<int64_t>(r.at.object), r.at.offset, static_cast<int64_t>(r.source), static_cast<int64_t>(r.target_object), r.target_offset));
        return out;
    });
}

inline py::dict<Place, Place> pointers(const LEFile& lf) {
    return cb::compat::guard([&] {
        py::dict<Place, Place> out;
        for (const auto& kv : lf.module().pointers()) out.set(conv::place(kv.first), conv::place(kv.second));
        return out;
    });
}

inline std::optional<Descriptor> descriptor(const LEFile& lf) {
    return cb::compat::guard([&]() -> std::optional<Descriptor> {
        auto d = lf.module().descriptor();
        return d ? std::optional<Descriptor>(conv::descriptor(*d)) : std::nullopt;
    });
}

inline py::list<std::optional<Place>> service_table(const LEFile& lf, const std::optional<Descriptor>& given = std::nullopt) {
    return cb::compat::guard([&] {
        py::list<std::optional<Place>> out;
        const auto table = given ? lf.module().service_table(conv::descriptor(*given)) : lf.module().service_table();
        for (const auto& p : table) out.append(conv::place(p));
        return out;
    });
}

inline py::dict<py::str, Place> entry_points(const LEFile& lf) {
    return cb::compat::guard([&] {
        py::dict<py::str, Place> out;
        auto d = lf.module().descriptor();
        if (!d) return out;
        if (d->at_control) out.set(py::str("control"), conv::place(*d->at_control));
        if (d->at_v86_api) out.set(py::str("v86_api"), conv::place(*d->at_v86_api));
        if (d->at_pm_api) out.set(py::str("pm_api"), conv::place(*d->at_pm_api));
        const auto table = lf.module().service_table(*d);
        for (size_t i = 0; i < table.size(); ++i)
            if (table[i]) out.set(py::str("service_" + std::to_string(i)), conv::place(*table[i]));
        return out;
    });
}

inline py::list<std::tuple<int64_t, int64_t, int64_t, int64_t, bool>> service_calls(const LEFile& lf) {
    return cb::compat::guard([&] {
        py::list<std::tuple<int64_t, int64_t, int64_t, int64_t, bool>> out;
        for (const auto& c : lf.module().service_calls())
            out.append(std::make_tuple(static_cast<int64_t>(c.object), static_cast<int64_t>(c.offset), static_cast<int64_t>(c.device), static_cast<int64_t>(c.service), c.jump));
        return out;
    });
}

// ---- taking apart and putting back -------------------------------------------------------------------------------------------------
inline Image read_image(const LEFile& lf) {
    return cb::compat::guard([&] {
        const cle::Parts p = lf.module().parts();
        Image o;
        o.stub = cb::compat::to_py(cb::ByteView(p.stub)), o.header = cb::compat::to_py(cb::ByteView(p.header));
        for (const auto& ob : p.objects) {
            ImageObject io{ob.vsize, ob.base, ob.flags, ob.first_page, ob.page_count, std::nullopt};
            if (ob.reserved) io.reserved = cb::compat::to_py(cb::ByteView(ob.reserved->data(), ob.reserved->size()));
            o.objects.append(io);
        }
        for (const auto& pg : p.pages) o.pages.append(std::make_tuple(static_cast<int64_t>(pg.kind), cb::compat::to_py(cb::ByteView(pg.body))));
        o.resident = std::holds_alternative<cb::Bytes>(p.resident) ? NameTable(cb::compat::to_py(cb::ByteView(std::get<cb::Bytes>(p.resident)))) : NameTable(conv::names(std::get<std::vector<cle::NameOrdinal>>(p.resident)));
        o.nonresident = std::holds_alternative<cb::Bytes>(p.nonresident) ? NameTable(cb::compat::to_py(cb::ByteView(std::get<cb::Bytes>(p.nonresident)))) : NameTable(conv::names(std::get<std::vector<cle::NameOrdinal>>(p.nonresident)));
        o.entry_table = cb::compat::to_py(cb::ByteView(p.entry_table));
        for (const auto& [page, records] : p.fixups) {
            py::list<py::bytes> l;
            for (const auto& r : records) l.append(cb::compat::to_py(cb::ByteView(r)));
            o.fixups.set(static_cast<int64_t>(page), l);
        }
        o.import_modules = cb::compat::to_py(p.import_modules);
        o.import_names = cb::compat::to_py(cb::ByteView(p.import_names));
        if (p.align) o.align = *p.align;
        if (p.last_page_bytes) o.last_page_bytes = *p.last_page_bytes;
        if (p.gaps) o.gaps = std::make_tuple(cb::compat::to_py(cb::ByteView(p.gaps->first)), cb::compat::to_py(cb::ByteView(p.gaps->second)));
        o.after = cb::compat::to_py(cb::ByteView(p.after));
        o.moved = Moved{p.moved.debug_info, p.moved.resource_data, p.moved.from};
        return o;
    });
}

inline cle::Parts parts_of(const Image& image) {
    cle::Parts p;
    p.stub = cb::compat::from_py(image.stub), p.header = cb::compat::from_py(image.header);
    for (const auto& o : image.objects) {
        cle::ObjectEntry e{static_cast<uint32_t>(o.vsize), static_cast<uint32_t>(o.base), static_cast<uint32_t>(o.flags), static_cast<uint32_t>(o.first_page),
                           static_cast<uint32_t>(o.page_count), std::nullopt};
        if (o.reserved) {
            const cb::Bytes r = cb::compat::from_py(*o.reserved);
            if (r.size() == 4) e.reserved = std::array<uint8_t, 4>{r[0], r[1], r[2], r[3]};
        }
        p.objects.push_back(e);
    }
    for (const auto& pg : image.pages) p.pages.push_back({static_cast<uint8_t>(std::get<0>(pg)), cb::compat::from_py(std::get<1>(pg))});
    auto table = [](const NameTable& t) -> cle::NameTable {
        if (auto* b = std::get_if<py::bytes>(&t)) return cb::compat::from_py(*b);
        return conv::names(std::get<NameList>(t));
    };
    p.resident = table(image.resident), p.nonresident = table(image.nonresident);
    p.entry_table = cb::compat::from_py(image.entry_table);
    for (const auto& kv : image.fixups.items()) {
        std::vector<cb::Bytes> records;
        for (const auto& r : std::get<1>(kv)) records.push_back(cb::compat::from_py(r));
        p.fixups.insert_or_assign(static_cast<uint32_t>(std::get<0>(kv)), std::move(records));
    }
    for (const auto& s : image.import_modules) p.import_modules.push_back(cb::compat::from_py(s));
    p.import_names = cb::compat::from_py(image.import_names);
    if (image.align) p.align = static_cast<uint32_t>(*image.align);
    if (image.last_page_bytes) p.last_page_bytes = static_cast<uint32_t>(*image.last_page_bytes);
    if (image.gaps) p.gaps = std::make_pair(cb::compat::from_py(std::get<0>(*image.gaps)), cb::compat::from_py(std::get<1>(*image.gaps)));
    p.after = cb::compat::from_py(image.after);
    p.moved = {static_cast<uint32_t>(image.moved.debug_info), static_cast<uint32_t>(image.moved.resource_data), static_cast<uint32_t>(image.moved.from)};
    if (image.page_map) p.page_map = cb::compat::from_py(*image.page_map);
    if (image.tables_tail) p.tables_tail = cb::compat::from_py(*image.tables_tail);
    if (image.data_pages) p.data_pages = static_cast<uint32_t>(*image.data_pages);
    if (image.in_pages) {
        cb::OrderedMap<std::string, int64_t> in;
        for (const auto& kv : image.in_pages->items()) in.insert_or_assign(cb::compat::from_py(std::get<0>(kv)), std::get<1>(kv));
        p.in_pages = std::move(in);
    }
    if (image.fill_from) p.fill_from = cb::compat::from_py(*image.fill_from);
    return p;
}

inline py::bytes _name_table(const NameTable& pairs) {
    return cb::compat::guard([&] {
        if (auto* b = std::get_if<py::bytes>(&pairs)) return *b;
        return cb::compat::to_py(cb::ByteView(cle::name_table(conv::names(std::get<NameList>(pairs)))));
    });
}

inline py::bytes object_table(const py::list<ImageObject>& objects) {
    Image tmp;
    tmp.objects = objects;
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cle::object_table(parts_of(tmp).objects))); });
}

inline std::tuple<py::bytes, py::bytes> fixup_tables(const py::dict<int64_t, py::list<py::bytes>>& fixups, int64_t pages) {
    return cb::compat::guard([&] {
        std::map<uint32_t, std::vector<cb::Bytes>> in;
        for (const auto& kv : fixups.items()) {
            std::vector<cb::Bytes> records;
            for (const auto& r : std::get<1>(kv)) records.push_back(cb::compat::from_py(r));
            in[static_cast<uint32_t>(std::get<0>(kv))] = std::move(records);
        }
        auto t = cle::fixup_tables(in, static_cast<uint32_t>(pages));
        return std::make_tuple(cb::compat::to_py(cb::ByteView(t.first)), cb::compat::to_py(cb::ByteView(t.second)));
    });
}

inline py::bytes write_image(const Image& image) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cle::write_parts(parts_of(image)))); });
}

inline std::optional<Container> container(const py::bytes& data) {
    return cb::compat::guard([&]() -> std::optional<Container> {
        auto c = cle::container(cb::compat::from_py(data));
        return c ? std::optional<Container>(conv::container(*c)) : std::nullopt;
    });
}

inline py::bytes pack_directory(const Container& c) {
    return cb::compat::guard([&] {
        cle::Container o;
        o.version = static_cast<uint16_t>(c.version);
        if (c.reserved) o.reserved = cb::compat::from_py(*c.reserved);
        for (const auto& m : c.members) o.members.push_back({cb::compat::from_py(std::get<0>(m)), static_cast<uint32_t>(std::get<1>(m)), static_cast<uint32_t>(std::get<2>(m))});
        return cb::compat::to_py(cb::ByteView(cle::pack_directory(o)));
    });
}

inline py::bytes expand_page(const py::bytes& buf, int64_t size = 0x2000) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cle::expand_page(cb::compat::from_py(buf), static_cast<size_t>(size)))); });
}
inline py::bytes compress_page(const py::bytes& page) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cle::compress_page(cb::compat::from_py(page)))); });
}
inline py::bytes expand(const py::bytes& data) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cle::expand(cb::compat::from_py(data)))); });
}
inline py::bytes compress(const py::bytes& data, const py::bytes& like) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cle::compress(cb::compat::from_py(data), cb::compat::from_py(like)))); });
}

inline py::list<std::tuple<py::str, LEFile>> members(const py::str& path) { return open_vxds(path); }

// COMPAT(py): remove when base.corpus is refactored (the corpus index and the detector it needs are the old ports').
inline py::list<py::str> corpus_files() {
    struct Entry {
        int64_t size;
        std::u32string lower;
        py::str path;
    };
    std::vector<Entry> found;
    for (const auto& kv : base::corpus::index().items()) {
        const py::list<py::str>& cands = std::get<1>(kv);
        if (cands.empty()) throw py::IndexError("list index out of range");
        const py::str& first = cands[0];
        if (detect::holds_drivers(first)) found.push_back({0, std::u32string(), first});
    }
    for (Entry& e : found) {
        e.size = py::os::path::getsize(e.path);
        e.lower = e.path.lower().u32();
    }
    std::stable_sort(found.begin(), found.end(), [](const Entry& a, const Entry& b) { return a.size != b.size ? a.size < b.size : a.lower < b.lower; });
    py::list<py::str> out;
    for (const Entry& e : found) out.append(e.path);
    return out;
}

}  // namespace codebreaker::formats::le
