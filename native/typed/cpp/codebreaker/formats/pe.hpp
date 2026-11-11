// codebreaker/formats/pe.hpp -- COMPAT shim for the old typed port of codebreaker.formats.pe (deleted when its last py:: consumer is
// refactored). The reader is native/cb/formats/pe.hpp (cb::formats::pe::Image); this header only converts: the records the old
// callers use, and PEFile with its method names. No logic.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/pe.hpp"
#include "cb/formats/read.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::pe {

struct Section {
    py::str name;
    int64_t vsize = 0, vaddr = 0, rawsize = 0, rawptr = 0, chars = 0;
};

struct BuildMeta {
    py::str machine, magic, kind;
    uint64_t image_base = 0;
    int64_t entry_rva = 0;
    uint64_t entry_va = 0;
    int64_t timestamp = 0;
    py::str linker_version, subsystem;
};

struct CodeView {
    py::str format;
    std::optional<py::str> guid;       // RSDS
    std::optional<int64_t> signature;  // NB10
    int64_t age = 0;
    py::str pdb;
};

struct OptionalHeader {
    int64_t magic = 0;
    py::str linker_version;
    int64_t code_size = 0, initialized_data_size = 0, uninitialized_data_size = 0, entry_point = 0, code_base = 0;
    uint64_t image_base = 0;
    int64_t section_alignment = 0, file_alignment = 0;
    py::str os_version, image_version, subsystem_version;
    int64_t image_size = 0, headers_size = 0, checksum = 0, subsystem = 0;
    py::str subsystem_name;
    int64_t dll_characteristics = 0;
};

struct ExportSymbol {
    py::str name;
    int64_t ordinal = 0, hint = 0, rva = 0;
    py::str forward;
};
struct ExportTable {
    py::str module;
    int64_t ordinal_base = 0, timestamp = 0;
    py::list<ExportSymbol> symbols;
};

struct ImportSymbol {
    py::str name;
    int64_t ordinal = 0, hint = 0, slot = 0;
};
struct ImportModule {
    py::str module;
    py::list<ImportSymbol> symbols;
};

struct ResourceItem {
    py::str name;
    std::optional<int64_t> id;
    int64_t language = 0, rva = 0;
    std::optional<int64_t> offset;
    int64_t size = 0;
};
struct ResourceType {
    std::variant<py::str, int64_t> type;
    std::optional<int64_t> id;
    py::list<ResourceItem> items;
};

struct Tls {
    uint64_t start_va = 0, end_va = 0, index_va = 0, callbacks_va = 0;
};
struct DebugEntry {
    int64_t type = 0, timestamp = 0, size = 0, offset = 0;
};
struct Certificate {
    int64_t length = 0, revision = 0, type = 0;
};
struct Clr {
    py::str runtime_version;
    int64_t flags = 0, entry_token = 0;
};
struct Rich {
    int64_t key = 0;
    py::list<std::tuple<int64_t, int64_t, int64_t>> entries;  // (product, build, count)
};

class PEFile {
  public:
    py::str path;
    py::bytes data;
    int64_t e_lfanew = 0, machine = 0, n_sections = 0, timestamp = 0, opt_size = 0, characteristics = 0, magic = 0;
    bool is_plus = false;
    int64_t entry = 0;
    uint64_t image_base = 0;
    int64_t n_datadir = 0;
    py::list<std::tuple<int64_t, int64_t>> datadirs;  // (rva, size)
    py::list<Section> sections;

    explicit PEFile(const py::str& path_) : path(path_) {
        py::file f = py::open_rb(path_);
        data = f.read();
        f.close();
        cb::compat::guard([&] {
            img_ = std::make_shared<const cb::formats::pe::Image>(cb::formats::pe::Image::parse(cb::compat::from_py(data), path_.utf8()));
        });
        const cb::formats::pe::Header& h = img_->header();
        e_lfanew = h.e_lfanew, machine = h.machine, n_sections = h.n_sections, timestamp = h.timestamp, opt_size = h.opt_size;
        characteristics = h.characteristics, magic = h.magic, is_plus = h.is_plus, entry = h.entry_rva, image_base = h.image_base;
        n_datadir = h.n_datadirs;
        for (const auto& d : img_->data_directories()) datadirs.append(std::make_tuple<int64_t, int64_t>(d.rva, d.size));
        for (const auto& s : img_->sections())
            sections.append(Section{cb::compat::to_py(s.name), s.virtual_size, s.virtual_address, s.raw_size, s.raw_ptr, s.characteristics});
    }

    py::str machine_name() const { return cb::compat::to_py(img_->machine_name()); }
    bool is_dll() const { return img_->header().is_dll(); }
    std::optional<int64_t> _rva_to_off(int64_t rva) const { return opt_int(rva < 0 ? std::nullopt : img_->rva_to_offset(static_cast<uint64_t>(rva))); }
    int64_t code_bytes() const { return static_cast<int64_t>(img_->code_bytes()); }
    py::str module_name() const { return cb::compat::guard([&] { return cb::compat::to_py(img_->module_name()); }); }
    py::dict<py::str, int64_t> export_addrs() const {
        return cb::compat::guard([&] {
            py::dict<py::str, int64_t> out;
            for (const auto& e : img_->exports())
                if (e.name && !e.name->empty()) out.set(cb::compat::to_py(*e.name), static_cast<int64_t>(e.rva));
            return out;
        });
    }
    py::list<std::tuple<int64_t, std::optional<py::str>, int64_t>> exports() const {
        return cb::compat::guard([&] {
            py::list<std::tuple<int64_t, std::optional<py::str>, int64_t>> out;
            for (const auto& e : img_->exports())
                out.append(std::make_tuple(static_cast<int64_t>(e.ordinal), e.name ? std::optional<py::str>(cb::compat::to_py(*e.name)) : std::nullopt,
                                           static_cast<int64_t>(e.rva)));
            return out;
        });
    }
    py::dict<uint64_t, std::tuple<py::str, py::str>> imports() const {
        return cb::compat::guard([&] {
            py::dict<uint64_t, std::tuple<py::str, py::str>> out;
            for (const auto& i : img_->imports()) out.set(i.slot_va, std::make_tuple(cb::compat::to_py(i.dll), cb::compat::to_py(i.name)));
            return out;
        });
    }
    py::list<std::tuple<uint64_t, py::bytes, py::str>> code_sections() const {
        return cb::compat::guard([&] {
            py::list<std::tuple<uint64_t, py::bytes, py::str>> out;
            for (const auto& c : img_->code_sections())
                out.append(std::make_tuple(c.va, cb::compat::to_py(img_->section_data(*c.section)), cb::compat::to_py(c.section->name)));
            return out;
        });
    }
    py::set<int64_t> base_relocs() const {
        return cb::compat::guard([&] {
            py::set<int64_t> out;
            for (uint64_t r : img_->base_relocs()) out.add(static_cast<int64_t>(r));
            return out;
        });
    }
    BuildMeta build_meta() const {
        return cb::compat::guard([&] {
            const auto oh = img_->optional_header();
            const auto entry_va = img_->entry_va();
            BuildMeta o;
            o.machine = cb::compat::to_py(img_->machine_name());
            o.magic = cb::compat::to_py(is_plus ? "PE32+" : "PE32");
            o.kind = cb::compat::to_py(is_dll() ? "DLL" : "EXE");
            o.image_base = image_base;
            o.entry_rva = entry;
            o.entry_va = entry_va.value_or(0);
            o.timestamp = timestamp;
            o.linker_version = version_text(oh.linker_major, oh.linker_minor);
            o.subsystem = subsystem_text(oh.subsystem, true);
            return o;
        });
    }
    std::optional<CodeView> debug_codeview() const {
        return cb::compat::guard([&]() -> std::optional<CodeView> {
            auto cv = img_->debug_codeview();
            if (!cv) return std::nullopt;
            CodeView o;
            const bool rsds = cv->kind == cb::formats::pe::CodeView::Kind::Rsds;
            o.format = cb::compat::to_py(rsds ? "RSDS" : "NB10");
            if (rsds) o.guid = cb::compat::to_py(cv->guid);
            else o.signature = static_cast<int64_t>(cv->signature);
            o.age = cv->age;
            o.pdb = cb::compat::to_py(cv->pdb);
            return o;
        });
    }
    uint64_t _u(const py::str& fmt, std::optional<int64_t> off) const {
        const std::string f = fmt.utf8();
        if (f.size() != 1 || std::string("BHIQ").find(f[0]) == std::string::npos) throw py::StructError("bad char in struct format");
        if (!off || *off < 0) return 0;
        const auto d = img_->data();
        const auto at = static_cast<uint64_t>(*off);
        return f[0] == 'B' ? cb::formats::get_or_zero<uint8_t>(d, at) : f[0] == 'H' ? cb::formats::get_or_zero<uint16_t>(d, at)
               : f[0] == 'I' ? cb::formats::get_or_zero<uint32_t>(d, at) : cb::formats::get_or_zero<uint64_t>(d, at);
    }
    py::str _text(std::optional<int64_t> off, int64_t limit = 512) const {
        if (!off || *off < 0) return py::str("");
        return cb::compat::to_py(cb::formats::cstring(img_->data(), static_cast<uint64_t>(*off), static_cast<uint64_t>(limit)));
    }
    std::optional<int64_t> file_offset(int64_t rva) const { return opt_int(rva < 0 ? std::nullopt : img_->file_offset(static_cast<uint64_t>(rva))); }
    OptionalHeader optional_header() const {
        const auto h = img_->optional_header();
        OptionalHeader o;
        o.magic = h.magic;
        o.linker_version = version_text(h.linker_major, h.linker_minor);
        o.code_size = h.code_size, o.initialized_data_size = h.initialized_data_size, o.uninitialized_data_size = h.uninitialized_data_size;
        o.entry_point = h.entry_point, o.code_base = h.code_base, o.image_base = h.image_base;
        o.section_alignment = h.section_alignment, o.file_alignment = h.file_alignment;
        o.os_version = version_text(h.os_version.major, h.os_version.minor);
        o.image_version = version_text(h.image_version.major, h.image_version.minor);
        o.subsystem_version = version_text(h.subsystem_version.major, h.subsystem_version.minor);
        o.image_size = h.image_size, o.headers_size = h.headers_size, o.checksum = h.checksum, o.subsystem = h.subsystem;
        o.subsystem_name = subsystem_text(h.subsystem, false);
        o.dll_characteristics = h.dll_characteristics;
        return o;
    }
    std::tuple<int64_t, int64_t> directory(int64_t index) const {
        const auto d = index < 0 ? cb::formats::pe::DataDirectory{} : img_->directory(static_cast<size_t>(index));
        return std::make_tuple<int64_t, int64_t>(d.rva, d.size);
    }
    ExportTable export_table() const {
        const auto t = img_->export_table();
        ExportTable o;
        o.module = cb::compat::to_py(t.module);
        o.ordinal_base = t.ordinal_base, o.timestamp = t.timestamp;
        for (const auto& s : t.symbols)
            o.symbols.append(ExportSymbol{cb::compat::to_py(s.name.value_or("")), static_cast<int64_t>(s.ordinal), s.hint ? static_cast<int64_t>(*s.hint) : -1,
                                          s.rva, cb::compat::to_py(s.forward.value_or(""))});
        return o;
    }
    py::list<ImportModule> import_table(bool delay = false) const {
        py::list<ImportModule> out;
        for (const auto& m : img_->import_table(delay)) {
            ImportModule o;
            o.module = cb::compat::to_py(m.module);
            for (const auto& s : m.symbols)
                o.symbols.append(ImportSymbol{cb::compat::to_py(s.name.value_or("")), s.ordinal ? static_cast<int64_t>(*s.ordinal) : -1,
                                              s.hint ? static_cast<int64_t>(*s.hint) : -1, static_cast<int64_t>(s.slot_rva)});
            out.append(o);
        }
        return out;
    }
    py::list<ResourceType> resource_tree() const {
        py::list<ResourceType> out;
        for (const auto& t : img_->resource_tree()) {
            ResourceType o;
            if (t.name) o.type = cb::compat::to_py(*t.name);
            else o.type = static_cast<int64_t>(*t.id);
            if (t.id) o.id = static_cast<int64_t>(*t.id);
            for (const auto& it : t.items) {
                ResourceItem e;
                e.name = it.name ? cb::compat::to_py(*it.name) : py::str("#" + std::to_string(*it.id));
                if (it.id) e.id = static_cast<int64_t>(*it.id);
                e.language = it.language, e.rva = it.rva, e.size = it.size;
                if (it.file_offset) e.offset = static_cast<int64_t>(*it.file_offset);
                o.items.append(e);
            }
            out.append(o);
        }
        return out;
    }
    py::list<std::tuple<int64_t, int64_t>> reloc_blocks() const {
        py::list<std::tuple<int64_t, int64_t>> out;
        for (const auto& b : img_->reloc_blocks()) out.append(std::make_tuple<int64_t, int64_t>(b.page_rva, b.entries));
        return out;
    }
    std::optional<Tls> tls() const {
        auto t = img_->tls();
        if (!t) return std::nullopt;
        return Tls{t->start_va, t->end_va, t->index_va, t->callbacks_va};
    }
    py::list<DebugEntry> debug_entries() const {
        py::list<DebugEntry> out;
        for (const auto& e : img_->debug_entries()) out.append(DebugEntry{e.type, e.timestamp, e.size, e.offset});
        return out;
    }
    py::list<Certificate> certificates() const {
        py::list<Certificate> out;
        for (const auto& c : img_->certificates()) out.append(Certificate{c.length, c.revision, c.type});
        return out;
    }
    std::optional<Clr> clr() const {
        auto c = img_->clr();
        if (!c) return std::nullopt;
        return Clr{version_text(c->runtime.major, c->runtime.minor), c->flags, c->entry_token};
    }
    std::optional<Rich> rich() const {
        auto r = img_->rich();
        if (!r) return std::nullopt;
        Rich o;
        o.key = r->key;
        for (const auto& e : r->entries) o.entries.append(std::make_tuple<int64_t, int64_t, int64_t>(e.product, e.build, e.count));
        return o;
    }

  private:
    static std::optional<int64_t> opt_int(const std::optional<uint64_t>& v) { return v ? std::optional<int64_t>(static_cast<int64_t>(*v)) : std::nullopt; }
    static py::str version_text(int64_t major, int64_t minor) { return py::str(std::to_string(major) + "." + std::to_string(minor)); }
    // `hex_when_unnamed`: build_meta writes 0xNNNN, optional_header the decimal number
    static py::str subsystem_text(uint32_t sub, bool hex_when_unnamed) {
        if (const char* label = cb::formats::pe::subsystem_label(sub)) return py::str(label);
        if (!hex_when_unnamed) return py::str(std::to_string(sub));
        char buf[16];
        std::snprintf(buf, sizeof buf, "0x%x", sub);
        return py::str(buf);
    }
    std::shared_ptr<const cb::formats::pe::Image> img_;
};

inline PEFile open_pe(const py::str& path) { return PEFile(path); }

inline bool is_dotnet(const py::str& path) { return cb::formats::pe::is_dotnet_file(cb::fs::from_text(path.utf8())); }

}  // namespace codebreaker::formats::pe
