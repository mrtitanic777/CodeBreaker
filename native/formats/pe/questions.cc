// questions.cc -- the PE reader's answers, in the shapes codebreaker/formats/pe.py gives them, read by
// cb::formats::pe (native/cb/formats/pe.hpp).
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../core/cb_bridge.h"
#include "../core/text.h"
#include "cb/formats/pe.hpp"
#include "cb/formats/read.hpp"

namespace cbf::pe {
namespace {

namespace P = cb::formats::pe;

P::Image open(Call &c) { return P::Image::parse(owned(c.data), c.str_arg(0, "<buffer>")); }
// the questions asked of an image already opened: its path is not needed again
P::Image opened(Call &c) { return P::Image::parse(owned(c.data), "<buffer>"); }

std::string version(int64_t major, int64_t minor) { return dec(major) + "." + dec(minor); }

void section_dict(Out &o, const P::Section &s) {
    o.begin_dict();
    o.item_text("name", s.name);
    o.item("vsize", s.virtual_size);
    o.item("vaddr", s.virtual_address);
    o.item("rawsize", s.raw_size);
    o.item("rawptr", s.raw_ptr);
    o.item("chars", s.characteristics);
    o.end_dict();
}

// PEFile.__init__: every attribute it sets
void parse(Call &c) {
    P::Image im = open(c);
    const P::Header &h = im.header();
    Out &o = c.out;
    o.begin_dict();
    o.item("e_lfanew", h.e_lfanew);
    o.item("machine", h.machine);
    o.item("n_sections", h.n_sections);
    o.item("timestamp", h.timestamp);
    o.item("opt_size", h.opt_size);
    o.item("characteristics", h.characteristics);
    o.item("magic", h.magic);
    o.key("is_plus");
    o.boolean(h.is_plus);
    o.item("entry", h.entry_rva);
    o.key("image_base");
    o.uinteger(h.image_base);
    o.item("n_datadir", h.n_datadirs);
    o.key("datadirs");
    o.begin_list();
    for (const P::DataDirectory &d : im.data_directories()) {
        o.begin_tuple();
        o.integer(d.rva);
        o.integer(d.size);
        o.end_tuple();
    }
    o.end_list();
    o.key("sections");
    o.begin_list();
    for (const P::Section &s : im.sections()) section_dict(o, s);
    o.end_list();
    o.end_dict();
}

void machine_name(Call &c) { c.out.text(opened(c).machine_name()); }
void is_dll(Call &c) { c.out.boolean(opened(c).header().is_dll()); }
void rva_to_off(Call &c) {
    const int64_t rva = c.int_arg(0);
    put_opt(c.out, rva < 0 ? std::nullopt : opened(c).rva_to_offset(uint64_t(rva)));
}
void code_bytes(Call &c) { c.out.uinteger(opened(c).code_bytes()); }
void module_name(Call &c) { c.out.text(opened(c).module_name()); }

void exports(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const P::Export &e : opened(c).exports()) {
        o.begin_tuple();
        o.uinteger(e.ordinal);
        put_opt_text(o, e.name);
        o.integer(e.rva);
        o.end_tuple();
    }
    o.end_list();
}

// {name: rva} of the named exports: a name met again keeps its first place and its last rva
void export_addrs(Call &c) {
    std::vector<std::pair<std::string, uint32_t>> out;
    std::unordered_map<std::string, size_t> where;
    for (const P::Export &e : opened(c).exports()) {
        if (!e.name || e.name->empty()) continue;
        auto it = where.find(*e.name);
        if (it != where.end()) {
            out[it->second].second = e.rva;
        } else {
            where[*e.name] = out.size();
            out.emplace_back(*e.name, e.rva);
        }
    }
    Out &o = c.out;
    o.begin_dict();
    for (auto &kv : out) {
        o.text(kv.first);
        o.integer(kv.second);
    }
    o.end_dict();
}

void imports(Call &c) {
    Out &o = c.out;
    o.begin_dict();
    for (const P::Import &i : opened(c).imports()) {
        o.uinteger(i.slot_va);
        o.begin_tuple();
        o.text(i.dll);
        o.text(i.name);
        o.end_tuple();
    }
    o.end_dict();
}

void code_sections(Call &c) {
    P::Image im = opened(c);
    Out &o = c.out;
    o.begin_list();
    for (const auto &cs : im.code_sections()) {
        o.begin_tuple();
        o.uinteger(cs.va);
        put(o, cb::formats::clamped(im.data(), cs.section->raw_ptr, cs.section->raw_size));   // data[rawptr:rawptr+rawsize]
        o.text(cs.section->name);
        o.end_tuple();
    }
    o.end_list();
}

void base_relocs(Call &c) {
    Out &o = c.out;
    o.begin_set();
    for (uint64_t r : opened(c).base_relocs()) o.uinteger(r);
    o.end_set();
}

void build_meta(Call &c) {
    P::Image im = opened(c);
    const P::Header &h = im.header();
    const uint64_t opt = uint64_t(h.e_lfanew) + 4 + 20;
    const uint64_t subsys_off = opt + (h.is_plus ? 72 : 68);
    const cb::ByteView d = im.data();
    Out &o = c.out;
    o.begin_dict();
    o.item_text("machine", im.machine_name());
    o.item_text("magic", h.is_plus ? "PE32+" : "PE32");
    o.item_text("kind", h.is_dll() ? "DLL" : "EXE");
    o.key("image_base");
    o.uinteger(h.image_base);
    o.item("entry_rva", h.entry_rva);
    o.key("entry_va");
    if (h.entry_rva) {
        if (h.entry_rva > UINT64_MAX - h.image_base) o.uinteger65(h.image_base + h.entry_rva);   // past 64 bits, as Python adds
        else o.uinteger(h.image_base + h.entry_rva);
    } else {
        o.integer(0);
    }
    o.item("timestamp", h.timestamp);
    o.item_text("linker_version", version(cb::formats::get<uint8_t>(d, opt + 2, "pe"), cb::formats::get<uint8_t>(d, opt + 3, "pe")));
    const uint16_t sub = cb::formats::get<uint16_t>(d, subsys_off, "pe");
    const char *label = P::subsystem_label(sub);
    o.item_text("subsystem", label ? label : "0x" + hex(sub));
    o.end_dict();
}

void debug_codeview(Call &c) {
    auto cv = opened(c).debug_codeview();
    Out &o = c.out;
    if (!cv) {
        o.none();
        return;
    }
    o.begin_dict();
    if (cv->kind == P::CodeView::Kind::Rsds) {
        o.item_text("format", "RSDS");
        o.item_text("guid", cv->guid);
    } else {
        o.item_text("format", "NB10");
        o.item("signature", cv->signature);
    }
    o.item("age", cv->age);
    o.item_text("pdb", cv->pdb);
    o.end_dict();
}

std::optional<int64_t> opt_int(const Value &v) {
    if (v.is_none()) return std::nullopt;
    return v.as_int();
}

// _u(fmt, off): a little-endian value of the image, 0 where it does not lie in the file
void u(Call &c) {
    const std::string fmt = c.str_arg(0);
    if (fmt.size() != 1 || std::string("BHIQ").find(fmt[0]) == std::string::npos) raise("struct.error", "bad char in struct format");
    const auto off = opt_int(c.arg(1));
    const cb::ByteView d = view(c.data);
    uint64_t v = 0;
    if (off && *off >= 0) {
        const uint64_t at = uint64_t(*off);
        switch (fmt[0]) {
        case 'B': v = cb::formats::get_or_zero<uint8_t>(d, at); break;
        case 'H': v = cb::formats::get_or_zero<uint16_t>(d, at); break;
        case 'I': v = cb::formats::get_or_zero<uint32_t>(d, at); break;
        default: v = cb::formats::get_or_zero<uint64_t>(d, at); break;
        }
    }
    c.out.uinteger(v);
}

// _text(off, limit): the zero-terminated latin1 string there, at most `limit` bytes; "" outside the file
void text(Call &c) {
    const auto off = opt_int(c.arg(0));
    const int64_t limit = c.int_arg(1, 512);
    if (!off || *off < 0 || *off >= c.data.size() || limit <= 0) {
        c.out.text("");
        return;
    }
    c.out.text(cb::formats::cstring(view(c.data), uint64_t(*off), uint64_t(limit)));
}

void file_offset(Call &c) {
    const int64_t rva = c.int_arg(0);
    put_opt(c.out, rva < 0 ? std::nullopt : opened(c).file_offset(uint64_t(rva)));
}

void optional_header(Call &c) {
    const P::OptionalHeader h = opened(c).optional_header();
    Out &o = c.out;
    o.begin_dict();
    o.item("magic", h.magic);
    o.item_text("linker_version", version(h.linker_major, h.linker_minor));
    o.item("code_size", h.code_size);
    o.item("initialized_data_size", h.initialized_data_size);
    o.item("uninitialized_data_size", h.uninitialized_data_size);
    o.item("entry_point", h.entry_point);
    o.item("code_base", h.code_base);
    o.key("image_base");
    o.uinteger(h.image_base);
    o.item("section_alignment", h.section_alignment);
    o.item("file_alignment", h.file_alignment);
    o.item_text("os_version", version(h.os_version.major, h.os_version.minor));
    o.item_text("image_version", version(h.image_version.major, h.image_version.minor));
    o.item_text("subsystem_version", version(h.subsystem_version.major, h.subsystem_version.minor));
    o.item("image_size", h.image_size);
    o.item("headers_size", h.headers_size);
    o.item("checksum", h.checksum);
    o.item("subsystem", h.subsystem);
    const char *label = P::subsystem_label(h.subsystem);
    o.item_text("subsystem_name", label ? label : dec(h.subsystem));
    o.item("dll_characteristics", h.dll_characteristics);
    o.end_dict();
}

void directory(Call &c) {
    const int64_t k = c.int_arg(0);
    const P::DataDirectory d = k < 0 ? P::DataDirectory{} : opened(c).directory(size_t(k));
    c.out.begin_tuple();
    c.out.integer(d.rva);
    c.out.integer(d.size);
    c.out.end_tuple();
}

void export_table(Call &c) {
    const P::ExportTable t = opened(c).export_table();
    Out &o = c.out;
    o.begin_dict();
    o.item_text("module", t.module);
    o.item("ordinal_base", t.ordinal_base);
    o.item("timestamp", t.timestamp);
    o.key("symbols");
    o.begin_list();
    for (const P::ExportSymbol &s : t.symbols) {
        o.begin_dict();
        o.item_text("name", s.name.value_or(""));
        o.key("ordinal");
        o.uinteger(s.ordinal);
        o.item("hint", s.hint ? int64_t(*s.hint) : -1);
        o.item("rva", s.rva);
        o.item_text("forward", s.forward.value_or(""));
        o.end_dict();
    }
    o.end_list();
    o.end_dict();
}

void import_table(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const P::ImportModule &m : opened(c).import_table(c.bool_arg(0))) {
        o.begin_dict();
        o.item_text("module", m.module);
        o.key("symbols");
        o.begin_list();
        for (const P::ImportSymbol &s : m.symbols) {
            o.begin_dict();
            o.item_text("name", s.name.value_or(""));
            o.item("ordinal", s.ordinal ? int64_t(*s.ordinal) : -1);
            o.item("hint", s.hint ? int64_t(*s.hint) : -1);
            o.key("slot");
            o.uinteger(s.slot_rva);
            o.end_dict();
        }
        o.end_list();
        o.end_dict();
    }
    o.end_list();
}

void resource_tree(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const P::ResourceType &t : opened(c).resource_tree()) {
        o.begin_dict();
        o.key("type");
        if (t.name) o.text(*t.name);
        else o.integer(t.id.value_or(0));
        o.key("id");
        put_opt(o, t.id);
        o.key("items");
        o.begin_list();
        for (const P::ResourceItem &it : t.items) {
            o.begin_dict();
            o.item_text("name", it.name ? *it.name : "#" + std::to_string(it.id.value_or(0)));
            o.key("id");
            put_opt(o, it.id);
            o.item("language", it.language);
            o.item("rva", it.rva);
            o.key("offset");
            put_opt(o, it.file_offset);
            o.item("size", it.size);
            o.end_dict();
        }
        o.end_list();
        o.end_dict();
    }
    o.end_list();
}

void reloc_blocks(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const P::RelocBlock &b : opened(c).reloc_blocks()) {
        o.begin_tuple();
        o.integer(b.page_rva);
        o.integer(b.entries);
        o.end_tuple();
    }
    o.end_list();
}

void tls(Call &c) {
    auto t = opened(c).tls();
    Out &o = c.out;
    if (!t) {
        o.none();
        return;
    }
    o.begin_dict();
    o.key("start_va");
    o.uinteger(t->start_va);
    o.key("end_va");
    o.uinteger(t->end_va);
    o.key("index_va");
    o.uinteger(t->index_va);
    o.key("callbacks_va");
    o.uinteger(t->callbacks_va);
    o.end_dict();
}

void debug_entries(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const P::DebugEntry &e : opened(c).debug_entries()) {
        o.begin_dict();
        o.item("type", e.type);
        o.item("timestamp", e.timestamp);
        o.item("size", e.size);
        o.item("offset", e.offset);
        o.end_dict();
    }
    o.end_list();
}

void certificates(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const P::Certificate &e : opened(c).certificates()) {
        o.begin_dict();
        o.item("length", e.length);
        o.item("revision", e.revision);
        o.item("type", e.type);
        o.end_dict();
    }
    o.end_list();
}

void clr(Call &c) {
    auto r = opened(c).clr();
    Out &o = c.out;
    if (!r) {
        o.none();
        return;
    }
    o.begin_dict();
    o.item_text("runtime_version", version(r->runtime.major, r->runtime.minor));
    o.item("flags", r->flags);
    o.item("entry_token", r->entry_token);
    o.end_dict();
}

void rich(Call &c) {
    auto r = opened(c).rich();
    Out &o = c.out;
    if (!r) {
        o.none();
        return;
    }
    o.begin_dict();
    o.item("key", r->key);
    o.key("entries");
    o.begin_list();
    for (const P::RichEntry &e : r->entries) {
        o.begin_tuple();
        o.integer(e.product);
        o.integer(e.build);
        o.integer(e.count);
        o.end_tuple();
    }
    o.end_list();
    o.end_dict();
}

void dotnet(Call &c) { c.out.boolean(P::is_dotnet_file(path_of(c.str_arg(0)))); }

}  // namespace

CBF_QUESTIONS(pe) {
    add("pe.parse", parse,
        "PEHeader{e_lfanew: u32, machine: u16, n_sections: u16, timestamp: u32, opt_size: u16, characteristics: u16, "
        "magic: u16, is_plus: bool, entry: u32, image_base: int, n_datadir: u32, datadirs: list[(u32, u32)], "
        "sections: list[Section{name: str, vsize: u32, vaddr: u32, rawsize: u32, rawptr: u32, chars: u32}]}");
    add("pe.machine_name", machine_name, "str");
    add("pe.is_dll", is_dll, "bool");
    add("pe._rva_to_off", rva_to_off, "int?");
    add("pe.code_bytes", code_bytes, "int");
    add("pe.module_name", module_name, "str");
    add("pe.export_addrs", export_addrs, "dict[str, u32]");
    add("pe.exports", exports, "list[(u32, str?, u32)]");
    add("pe.imports", imports, "dict[int, (str, str)]");
    add("pe.code_sections", code_sections, "list[(int, bytes, str)]");
    add("pe.base_relocs", base_relocs, "set[int]");
    add("pe.build_meta", build_meta,
        "BuildMeta{machine: str, magic: str, kind: str, image_base: int, entry_rva: int, entry_va: bigint, "
        "timestamp: int, linker_version: str, subsystem: str}");
    add("pe.debug_codeview", debug_codeview, "CodeView{format: str, guid?: str, signature?: int, age: int, pdb: str}?");
    add("pe._u", u, "int");
    add("pe._text", text, "str");
    add("pe.file_offset", file_offset, "int?");
    add("pe.optional_header", optional_header,
        "OptionalHeader{magic: int, linker_version: str, code_size: int, initialized_data_size: int, "
        "uninitialized_data_size: int, entry_point: int, code_base: int, image_base: int, section_alignment: int, "
        "file_alignment: int, os_version: str, image_version: str, subsystem_version: str, image_size: int, "
        "headers_size: int, checksum: int, subsystem: int, subsystem_name: str, dll_characteristics: int}");
    add("pe.directory", directory, "(u32, u32)");
    add("pe.export_table", export_table,
        "ExportTable{module: str, ordinal_base: u32, timestamp: u32, "
        "symbols: list[ExportSymbol{name: str, ordinal: u32, hint: i32, rva: u32, forward: str}]}");
    add("pe.import_table", import_table,
        "list[ImportModule{module: str, symbols: list[ImportSymbol{name: str, ordinal: i32, hint: i32, slot: u32}]}]");
    add("pe.resource_tree", resource_tree,
        "list[ResourceType{type: str|int, id: int?, "
        "items: list[ResourceItem{name: str, id: int?, language: int, rva: int, offset: int?, size: int}]}]");
    add("pe.reloc_blocks", reloc_blocks, "list[(u32, u32)]");
    add("pe.tls", tls, "Tls{start_va: int, end_va: int, index_va: int, callbacks_va: int}?");
    add("pe.debug_entries", debug_entries, "list[DebugEntry{type: u32, timestamp: u32, size: u32, offset: u32}]");
    add("pe.certificates", certificates, "list[Certificate{length: u32, revision: u16, type: u16}]");
    add("pe.clr", clr, "Clr{runtime_version: str, flags: u32, entry_token: u32}?");
    add("pe.rich", rich, "Rich{key: u32, entries: list[(u16, u16, u32)]}?");
    add("pe.is_dotnet", dotnet, "bool");
}

}  // namespace cbf::pe
