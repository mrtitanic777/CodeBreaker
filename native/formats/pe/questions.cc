// questions.cc -- the PE reader's answers, in the shapes codebreaker/formats/pe.py gives them.
#include <unordered_map>

#include "../core/questions.h"
#include "../core/text.h"
#include "pe.h"

namespace cbf::pe {
namespace {

Image open(Call &c) { return Image(c.data, c.str_arg(0, "<buffer>")); }
// the questions asked of an image already opened: its path is not needed again
Image opened(Call &c) { return Image(c.data, "<buffer>"); }

void put_offset(Out &o, std::optional<int64_t> off) {
    if (off) o.integer(*off);
    else o.none();
}

void section_dict(Out &o, const Section &s) {
    o.begin_dict();
    o.key("name");
    o.latin1(s.name);
    o.item("vsize", s.vsize);
    o.item("vaddr", s.vaddr);
    o.item("rawsize", s.rawsize);
    o.item("rawptr", s.rawptr);
    o.item("chars", s.chars);
    o.end_dict();
}

// PEFile.__init__: every attribute it sets
void parse(Call &c) {
    Image im = open(c);
    Out &o = c.out;
    o.begin_dict();
    o.item("e_lfanew", im.e_lfanew);
    o.item("machine", im.machine);
    o.item("n_sections", im.n_sections);
    o.item("timestamp", im.timestamp);
    o.item("opt_size", im.opt_size);
    o.item("characteristics", im.characteristics);
    o.item("magic", im.magic);
    o.key("is_plus");
    o.boolean(im.is_plus);
    o.item("entry", im.entry);
    o.key("image_base");
    o.uinteger(im.image_base);
    o.item("n_datadir", im.n_datadir);
    o.key("datadirs");
    o.begin_list();
    for (const DataDirectory &d : im.datadirs) {
        o.begin_tuple();
        o.integer(d.va);
        o.integer(d.size);
        o.end_tuple();
    }
    o.end_list();
    o.key("sections");
    o.begin_list();
    for (const Section &s : im.sections) section_dict(o, s);
    o.end_list();
    o.end_dict();
}

void machine_name(Call &c) { c.out.text(opened(c).machine_name()); }
void is_dll(Call &c) { c.out.boolean(opened(c).is_dll()); }
void rva_to_off(Call &c) { put_offset(c.out, opened(c).rva_to_off(c.int_arg(0))); }
void code_bytes(Call &c) { c.out.uinteger(opened(c).code_bytes()); }
void module_name(Call &c) { c.out.text(opened(c).module_name()); }

void exports(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const Export &e : opened(c).exports()) {
        o.begin_tuple();
        o.integer(e.ordinal);
        if (e.name) o.latin1(*e.name);
        else o.none();
        o.integer(e.rva);
        o.end_tuple();
    }
    o.end_list();
}

// {name: rva} of the named exports: a name met again keeps its first place and its last rva
void export_addrs(Call &c) {
    std::vector<std::pair<std::string, uint32_t>> out;
    std::unordered_map<std::string, size_t> where;
    for (const Export &e : opened(c).exports()) {
        if (!e.name || e.name->empty()) continue;
        std::string n = latin1(*e.name);
        auto it = where.find(n);
        if (it != where.end()) {
            out[it->second].second = e.rva;
        } else {
            where[n] = out.size();
            out.emplace_back(n, e.rva);
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
    for (const Import &i : opened(c).imports()) {
        o.uinteger(i.slot);
        o.begin_tuple();
        o.text(i.dll);
        o.text(i.name);
        o.end_tuple();
    }
    o.end_dict();
}

void code_sections(Call &c) {
    Image im = opened(c);
    Out &o = c.out;
    o.begin_list();
    for (auto &vs : im.code_sections()) {
        o.begin_tuple();
        o.uinteger(vs.first);
        o.bytes(im.data.slice(vs.second.rawptr, int64_t(vs.second.rawptr) + vs.second.rawsize));
        o.latin1(vs.second.name);
        o.end_tuple();
    }
    o.end_list();
}

void base_relocs(Call &c) {
    Out &o = c.out;
    o.begin_set();
    for (int64_t r : opened(c).base_relocs()) o.integer(r);
    o.end_set();
}

void build_meta(Call &c) {
    Image im = opened(c);
    const int64_t opt = int64_t(im.e_lfanew) + 4 + 20;
    const int64_t subsys_off = opt + (im.is_plus ? 72 : 68);
    Out &o = c.out;
    o.begin_dict();
    o.item_text("machine", im.machine_name());
    o.item_text("magic", im.is_plus ? "PE32+" : "PE32");
    o.item_text("kind", im.is_dll() ? "DLL" : "EXE");
    o.key("image_base");
    o.uinteger(im.image_base);
    o.item("entry_rva", im.entry);
    o.key("entry_va");
    if (im.entry) {
        if (im.entry > UINT64_MAX - im.image_base) o.uinteger65(im.image_base + im.entry);   // past 64 bits, as Python adds
        else o.uinteger(im.image_base + im.entry);
    } else {
        o.integer(0);
    }
    o.item("timestamp", im.timestamp);
    o.item_text("linker_version", dec(im.data.at(opt + 2)) + "." + dec(im.data.at(opt + 3)));
    uint16_t sub = im.data.slice(subsys_off, subsys_off + 2).exact(2).u16(0);
    const char *label = subsystem_label(sub);
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
    o.item_text("format", cv->format);
    if (cv->format == "RSDS") {
        o.item_text("guid", cv->guid);
        o.item("age", cv->age);
    } else {
        o.item("signature", cv->signature);
        o.item("age", cv->age);
    }
    o.key("pdb");
    o.latin1(cv->pdb);
    o.end_dict();
}

std::optional<int64_t> opt_int(const Value &v) {
    if (v.is_none()) return std::nullopt;
    return v.as_int();
}

void u(Call &c) {
    std::string fmt = c.str_arg(0);
    if (fmt.size() != 1 || std::string("BHIQ").find(fmt[0]) == std::string::npos) raise("struct.error", "bad char in struct format");
    c.out.uinteger(opened(c).u(fmt[0], opt_int(c.arg(1))));
}
void text(Call &c) { c.out.text(opened(c).text(opt_int(c.arg(0)), c.int_arg(1, 512))); }
void file_offset(Call &c) { put_offset(c.out, opened(c).file_offset(c.int_arg(0))); }

void optional_header(Call &c) {
    Image im = opened(c);
    const int64_t opt = int64_t(im.e_lfanew) + 24;
    auto u16 = [&](int64_t o) { return int64_t(im.u('H', opt + o)); };
    auto u32 = [&](int64_t o) { return int64_t(im.u('I', opt + o)); };
    auto ver = [&](int64_t a, int64_t b) { return dec(a) + "." + dec(b); };
    Out &o = c.out;
    o.begin_dict();
    o.item("magic", im.magic);
    o.item_text("linker_version", ver(int64_t(im.u('B', opt + 2)), int64_t(im.u('B', opt + 3))));
    o.item("code_size", u32(4));
    o.item("initialized_data_size", u32(8));
    o.item("uninitialized_data_size", u32(12));
    o.item("entry_point", im.entry);
    o.item("code_base", u32(20));
    o.key("image_base");
    o.uinteger(im.image_base);
    o.item("section_alignment", u32(32));
    o.item("file_alignment", u32(36));
    o.item_text("os_version", ver(u16(40), u16(42)));
    o.item_text("image_version", ver(u16(44), u16(46)));
    o.item_text("subsystem_version", ver(u16(48), u16(50)));
    o.item("image_size", u32(56));
    o.item("headers_size", u32(60));
    o.item("checksum", u32(64));
    o.item("subsystem", u16(68));
    const char *label = subsystem_label(uint32_t(u16(68)));
    o.item_text("subsystem_name", label ? label : dec(u16(68)));
    o.item("dll_characteristics", u16(70));
    o.end_dict();
}

void directory(Call &c) {
    DataDirectory d = opened(c).directory(c.int_arg(0));
    c.out.begin_tuple();
    c.out.integer(d.va);
    c.out.integer(d.size);
    c.out.end_tuple();
}

void export_table(Call &c) {
    ExportTable t = opened(c).export_table();
    Out &o = c.out;
    o.begin_dict();
    o.item_text("module", t.module);
    o.item("ordinal_base", t.ordinal_base);
    o.item("timestamp", t.timestamp);
    o.key("symbols");
    o.begin_list();
    for (const ExportSymbol &s : t.symbols) {
        o.begin_dict();
        o.item_text("name", s.name);
        o.item("ordinal", s.ordinal);
        o.item("hint", s.hint);
        o.item("rva", s.rva);
        o.item_text("forward", s.forward);
        o.end_dict();
    }
    o.end_list();
    o.end_dict();
}

void import_table(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const ImportModule &m : opened(c).import_table(c.bool_arg(0))) {
        o.begin_dict();
        o.item_text("module", m.module);
        o.key("symbols");
        o.begin_list();
        for (const ImportSymbol &s : m.symbols) {
            o.begin_dict();
            o.item_text("name", s.name);
            o.item("ordinal", s.ordinal);
            o.item("hint", s.hint);
            o.item("slot", s.slot);
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
    for (const ResourceType &t : opened(c).resource_tree()) {
        o.begin_dict();
        o.key("type");
        if (t.name) o.text(*t.name);
        else o.integer(*t.id);
        o.key("id");
        if (t.id) o.integer(*t.id);
        else o.none();
        o.key("items");
        o.begin_list();
        for (const ResourceItem &it : t.items) {
            o.begin_dict();
            o.item_text("name", *it.name_text);
            o.key("id");
            if (it.id) o.integer(*it.id);
            else o.none();
            o.item("language", it.language);
            o.item("rva", it.rva);
            o.key("offset");
            put_offset(o, it.offset);
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
    for (auto &b : opened(c).reloc_blocks()) {
        o.begin_tuple();
        o.integer(b.first);
        o.integer(b.second);
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
    for (const DebugEntry &e : opened(c).debug_entries()) {
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
    for (const Certificate &e : opened(c).certificates()) {
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
    o.item_text("runtime_version", dec(r->major) + "." + dec(r->minor));
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
    for (const RichEntry &e : r->entries) {
        o.begin_tuple();
        o.integer(e.product);
        o.integer(e.build);
        o.integer(e.count);
        o.end_tuple();
    }
    o.end_list();
    o.end_dict();
}

void dotnet(Call &c) { c.out.boolean(is_dotnet(c.str_arg(0))); }

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
