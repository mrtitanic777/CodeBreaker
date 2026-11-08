// codebreaker/formats/pe.cpp -- typed C++ from codebreaker.formats.pe (see pe.hpp).
//
// Calls the native reader (native/formats/pe/pe.cc, cbf::pe::Image) directly and shapes its results as
// native/formats/pe/questions.cc shaped them for the Python bridge, but into typed structs.
#include "pe.hpp"

#include <new>

#include "../../../../formats/core/bytes.h"
#include "../../../../formats/core/text.h"
#include "../../../../formats/pe/pe.h"

namespace codebreaker::formats::pe {

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
// bytes decoded latin1 (the reader's text), as py::str
py::str L(cbf::Bytes b) { return T(cbf::latin1(b)); }

cbf::Bytes view(const py::bytes& b) { return cbf::Bytes(b.raw()); }

// the image read again from the data, as the DLL does on every question
cbf::pe::Image opened(const PEFile& f) { return cbf::pe::Image(view(f.data), "<buffer>"); }

}  // namespace

PEFile::PEFile(const py::str& path_) : path(path_) {
    // with open(path, "rb") as f: self.data = f.read()
    py::file f = py::open_rb(path_);
    data = f.read();
    f.close();
    guard([&] {
        cbf::pe::Image im(view(data), path_.utf8());
        e_lfanew = im.e_lfanew;
        machine = im.machine;
        n_sections = im.n_sections;
        timestamp = im.timestamp;
        opt_size = im.opt_size;
        characteristics = im.characteristics;
        magic = im.magic;
        is_plus = im.is_plus;
        entry = im.entry;
        image_base = im.image_base;
        n_datadir = im.n_datadir;
        for (const cbf::pe::DataDirectory& d : im.datadirs) datadirs.append(std::make_tuple<int64_t, int64_t>(d.va, d.size));
        for (const cbf::pe::Section& s : im.sections) {
            Section out;
            out.name = L(s.name);
            out.vsize = s.vsize;
            out.vaddr = s.vaddr;
            out.rawsize = s.rawsize;
            out.rawptr = s.rawptr;
            out.chars = s.chars;
            sections.append(out);
        }
    });
}

py::str PEFile::machine_name() const {
    return guard([&] { return T(opened(*this).machine_name()); });
}

bool PEFile::is_dll() const {
    return guard([&] { return opened(*this).is_dll(); });
}

std::optional<int64_t> PEFile::_rva_to_off(int64_t rva) const {
    return guard([&] { return opened(*this).rva_to_off(rva); });
}

// Sum of raw sizes of executable/code sections.
int64_t PEFile::code_bytes() const {
    return guard([&] { return static_cast<int64_t>(opened(*this).code_bytes()); });
}

// The export directory's internal name, or '' if not exported.
py::str PEFile::module_name() const {
    return guard([&] { return T(opened(*this).module_name()); });
}

// {name: rva} for every export that has a name.
py::dict<py::str, int64_t> PEFile::export_addrs() const {
    return guard([&] {
        py::dict<py::str, int64_t> out;
        for (const cbf::pe::Export& e : opened(*this).exports()) {
            if (!e.name || e.name->empty()) continue;
            out.set(L(*e.name), static_cast<int64_t>(e.rva));   // a name met again keeps its place, takes the last rva
        }
        return out;
    });
}

// [(ordinal, name_or_None, rva)] from the export directory.
py::list<std::tuple<int64_t, std::optional<py::str>, int64_t>> PEFile::exports() const {
    return guard([&] {
        py::list<std::tuple<int64_t, std::optional<py::str>, int64_t>> out;
        for (const cbf::pe::Export& e : opened(*this).exports())
            out.append(std::make_tuple(e.ordinal, e.name ? std::optional<py::str>(L(*e.name)) : std::nullopt,
                                       static_cast<int64_t>(e.rva)));
        return out;
    });
}

// {iat_slot_va: (dll, name)} from the import directory (data dir 1).
py::dict<uint64_t, std::tuple<py::str, py::str>> PEFile::imports() const {
    return guard([&] {
        py::dict<uint64_t, std::tuple<py::str, py::str>> out;
        for (const cbf::pe::Import& i : opened(*this).imports()) out.set(i.slot, std::make_tuple(T(i.dll), T(i.name)));
        return out;
    });
}

// [(va_start, bytes, name)] for executable sections.
py::list<std::tuple<uint64_t, py::bytes, py::str>> PEFile::code_sections() const {
    return guard([&] {
        cbf::pe::Image im = opened(*this);
        py::list<std::tuple<uint64_t, py::bytes, py::str>> out;
        for (const auto& vs : im.code_sections()) {
            cbf::Bytes raw = im.data.slice(vs.second.rawptr, int64_t(vs.second.rawptr) + vs.second.rawsize);
            out.append(std::make_tuple(vs.first, py::bytes(raw.str()), L(vs.second.name)));
        }
        return out;
    });
}

// Set of RVAs that carry an absolute 32-bit address (HIGHLOW, type 3).
py::set<int64_t> PEFile::base_relocs() const {
    return guard([&] {
        py::set<int64_t> out;
        for (int64_t r : opened(*this).base_relocs()) out.add(r);
        return out;
    });
}

// Structural build metadata: machine, bitness, image base, entry, link timestamp, linker version,
// subsystem, EXE/DLL. All from the headers.
BuildMeta PEFile::build_meta() const {
    return guard([&] {
        cbf::pe::Image im = opened(*this);
        const int64_t opt_at = int64_t(im.e_lfanew) + 4 + 20;
        const int64_t subsys_off = opt_at + (im.is_plus ? 72 : 68);
        BuildMeta o;
        o.machine = T(im.machine_name());
        o.magic = im.is_plus ? "PE32+" : "PE32";
        o.kind = im.is_dll() ? "DLL" : "EXE";
        o.image_base = im.image_base;
        o.entry_rva = im.entry;
        bool past_64_bits = false;       // entry_va = image_base + entry as Python adds them
        if (im.entry) {
            if (im.entry > UINT64_MAX - im.image_base) past_64_bits = true;
            else o.entry_va = im.image_base + im.entry;
        }
        o.timestamp = im.timestamp;
        o.linker_version = T(cbf::dec(im.data.at(opt_at + 2)) + "." + cbf::dec(im.data.at(opt_at + 3)));
        uint16_t sub = im.data.slice(subsys_off, subsys_off + 2).exact(2).u16(0);
        const char* label = cbf::pe::subsystem_label(sub);
        o.subsystem = T(label ? std::string(label) : "0x" + cbf::hex(sub));
        // (the DLL hands back a Python int here, which has no 64-bit limit; this struct does)
        if (past_64_bits) throw py::OverflowError("int too large for int64");
        return o;
    });
}

// The CodeView record from the DEBUG data directory (index 6), or None.
std::optional<CodeView> PEFile::debug_codeview() const {
    return guard([&]() -> std::optional<CodeView> {
        auto cv = opened(*this).debug_codeview();
        if (!cv) return std::nullopt;
        CodeView o;
        o.format = T(cv->format);
        if (cv->format == "RSDS") {
            o.guid = T(cv->guid);
        } else {
            o.signature = static_cast<int64_t>(cv->signature);
        }
        o.age = cv->age;
        o.pdb = L(cv->pdb);
        return o;
    });
}

// One little-endian value at a file offset, or 0 when it lies outside the file.
uint64_t PEFile::_u(const py::str& fmt, std::optional<int64_t> off) const {
    return guard([&] {
        std::string f = fmt.utf8();
        if (f.size() != 1 || std::string("BHIQ").find(f[0]) == std::string::npos)
            cbf::raise("struct.error", "bad char in struct format");
        return opened(*this).u(f[0], off);
    });
}

// The zero-terminated string at a file offset ('' when the offset is not in the file).
py::str PEFile::_text(std::optional<int64_t> off, int64_t limit) const {
    return guard([&] { return T(opened(*this).text(off, limit)); });
}

std::optional<int64_t> PEFile::file_offset(int64_t rva) const {
    return guard([&] { return opened(*this).file_offset(rva); });
}

// The optional header's fields by name (PE32 and PE32+).
OptionalHeader PEFile::optional_header() const {
    return guard([&] {
        cbf::pe::Image im = opened(*this);
        const int64_t opt_at = int64_t(im.e_lfanew) + 24;
        auto u16 = [&](int64_t o) { return int64_t(im.u('H', opt_at + o)); };
        auto u32 = [&](int64_t o) { return int64_t(im.u('I', opt_at + o)); };
        auto ver = [&](int64_t a, int64_t b) { return cbf::dec(a) + "." + cbf::dec(b); };
        OptionalHeader o;
        o.magic = im.magic;
        o.linker_version = T(ver(int64_t(im.u('B', opt_at + 2)), int64_t(im.u('B', opt_at + 3))));
        o.code_size = u32(4);
        o.initialized_data_size = u32(8);
        o.uninitialized_data_size = u32(12);
        o.entry_point = im.entry;
        o.code_base = u32(20);
        o.image_base = im.image_base;
        o.section_alignment = u32(32);
        o.file_alignment = u32(36);
        o.os_version = T(ver(u16(40), u16(42)));
        o.image_version = T(ver(u16(44), u16(46)));
        o.subsystem_version = T(ver(u16(48), u16(50)));
        o.image_size = u32(56);
        o.headers_size = u32(60);
        o.checksum = u32(64);
        o.subsystem = u16(68);
        const char* label = cbf::pe::subsystem_label(uint32_t(u16(68)));
        o.subsystem_name = T(label ? std::string(label) : cbf::dec(u16(68)));
        o.dll_characteristics = u16(70);
        return o;
    });
}

// (rva, size) of one data directory; (0, 0) when the image has none at that index.
std::tuple<int64_t, int64_t> PEFile::directory(int64_t index) const {
    return guard([&] {
        cbf::pe::DataDirectory d = opened(*this).directory(index);
        return std::make_tuple<int64_t, int64_t>(d.va, d.size);
    });
}

ExportTable PEFile::export_table() const {
    return guard([&] {
        cbf::pe::ExportTable t = opened(*this).export_table();
        ExportTable o;
        o.module = T(t.module);
        o.ordinal_base = t.ordinal_base;
        o.timestamp = t.timestamp;
        for (const cbf::pe::ExportSymbol& s : t.symbols) {
            ExportSymbol e;
            e.name = T(s.name);
            e.ordinal = s.ordinal;
            e.hint = s.hint;
            e.rva = s.rva;
            e.forward = T(s.forward);
            o.symbols.append(e);
        }
        return o;
    });
}

py::list<ImportModule> PEFile::import_table(bool delay) const {
    return guard([&] {
        py::list<ImportModule> out;
        for (const cbf::pe::ImportModule& m : opened(*this).import_table(delay)) {
            ImportModule o;
            o.module = T(m.module);
            for (const cbf::pe::ImportSymbol& s : m.symbols) {
                ImportSymbol e;
                e.name = T(s.name);
                e.ordinal = s.ordinal;
                e.hint = s.hint;
                e.slot = s.slot;
                o.symbols.append(e);
            }
            out.append(o);
        }
        return out;
    });
}

py::list<ResourceType> PEFile::resource_tree() const {
    return guard([&] {
        py::list<ResourceType> out;
        for (const cbf::pe::ResourceType& t : opened(*this).resource_tree()) {
            ResourceType o;
            if (t.name) o.type = T(*t.name);
            else o.type = static_cast<int64_t>(*t.id);
            if (t.id) o.id = static_cast<int64_t>(*t.id);
            for (const cbf::pe::ResourceItem& it : t.items) {
                ResourceItem e;
                e.name = T(*it.name_text);
                if (it.id) e.id = static_cast<int64_t>(*it.id);
                e.language = it.language;
                e.rva = it.rva;
                e.offset = it.offset;
                e.size = it.size;
                o.items.append(e);
            }
            out.append(o);
        }
        return out;
    });
}

// [(page_rva, entries)] -- the blocks of the base relocation directory.
py::list<std::tuple<int64_t, int64_t>> PEFile::reloc_blocks() const {
    return guard([&] {
        py::list<std::tuple<int64_t, int64_t>> out;
        for (const auto& b : opened(*this).reloc_blocks()) out.append(std::make_tuple<int64_t, int64_t>(b.first, b.second));
        return out;
    });
}

// The thread-local storage directory's addresses, or None.
std::optional<Tls> PEFile::tls() const {
    return guard([&]() -> std::optional<Tls> {
        auto t = opened(*this).tls();
        if (!t) return std::nullopt;
        return Tls{t->start_va, t->end_va, t->index_va, t->callbacks_va};
    });
}

py::list<DebugEntry> PEFile::debug_entries() const {
    return guard([&] {
        py::list<DebugEntry> out;
        for (const cbf::pe::DebugEntry& e : opened(*this).debug_entries()) out.append(DebugEntry{e.type, e.timestamp, e.size, e.offset});
        return out;
    });
}

py::list<Certificate> PEFile::certificates() const {
    return guard([&] {
        py::list<Certificate> out;
        for (const cbf::pe::Certificate& e : opened(*this).certificates()) out.append(Certificate{e.length, e.revision, e.type});
        return out;
    });
}

// The .NET runtime header, or None for a native image.
std::optional<Clr> PEFile::clr() const {
    return guard([&]() -> std::optional<Clr> {
        auto r = opened(*this).clr();
        if (!r) return std::nullopt;
        Clr o;
        o.runtime_version = T(cbf::dec(r->major) + "." + cbf::dec(r->minor));
        o.flags = r->flags;
        o.entry_token = r->entry_token;
        return o;
    });
}

// The linker's 'Rich' record between the DOS stub and the PE header.
std::optional<Rich> PEFile::rich() const {
    return guard([&]() -> std::optional<Rich> {
        auto r = opened(*this).rich();
        if (!r) return std::nullopt;
        Rich o;
        o.key = r->key;
        for (const cbf::pe::RichEntry& e : r->entries)
            o.entries.append(std::make_tuple<int64_t, int64_t, int64_t>(e.product, e.build, e.count));
        return o;
    });
}

PEFile open_pe(const py::str& path) { return PEFile(path); }

bool is_dotnet(const py::str& path) {
    return guard([&] { return cbf::pe::is_dotnet(path.utf8()); });
}

}  // namespace codebreaker::formats::pe
