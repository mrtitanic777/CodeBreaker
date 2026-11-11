// questions.cc -- the NE reader's answers, in the shapes codebreaker/formats/ne.py gives them, read by
// cb::formats::ne (native/cb/formats/ne.hpp).
//
// A module's questions take its place first -- (ne_off, base, overlay_at or None) -- then the
// method's own arguments: the module is read again from those on every call (NEFile keeps
// nothing else), and overlay_at not None makes it a boot image's module (BootImageModule).
#include <string>
#include <variant>
#include <vector>

#include "../core/cb_bridge.h"
#include "../core/text.h"
#include "cb/formats/ne.hpp"
#include "cb/formats/read.hpp"

namespace cbf::ne {
namespace {

namespace N = cb::formats::ne;

constexpr size_t kArgs = 3;   // the module's place, before a method's own arguments

N::Module module(Call &c) {
    const uint64_t ne_off = uint64_t(c.int_arg(0));
    N::Module m = c.arg(2).is_none() ? N::Module::parse(owned(c.data), ne_off)
                                     : N::Module::parse_boot_module(owned(c.data), ne_off, uint64_t(c.int_arg(2)));
    m.set_base(uint64_t(c.int_arg(1)));
    return m;
}

void header_dict(Out &o, const N::Header &h) {
    o.begin_dict();
    o.item("linker_ver", h.linker_version);
    o.item("linker_rev", h.linker_revision);
    o.item("entry_off", h.entry_table_offset);
    o.item("entry_len", h.entry_table_length);
    o.item("crc", h.crc);
    o.item("flags", h.flags);
    o.item("autodata_seg", h.autodata_segment);
    o.item("heap", h.heap_size);
    o.item("stack", h.stack_size);
    o.item("csip", h.cs_ip);
    o.item("sssp", h.ss_sp);
    o.item("n_segs", h.segment_count);
    o.item("n_modrefs", h.module_ref_count);
    o.item("nonres_size", h.nonresident_size);
    o.item("seg_tab", h.segment_table);
    o.item("rsrc_tab", h.resource_table);
    o.item("resname_tab", h.resident_names_table);
    o.item("mod_tab", h.module_ref_table);
    o.item("imp_tab", h.imported_names_table);
    o.item("nonres_off", h.nonresident_offset);
    o.item("n_movable", h.movable_entries);
    o.item("align_shift", h.alignment_shift);
    o.item("n_resources", h.resource_segments);
    o.item("exetype", h.target_os);
    o.key("expect_winver");
    put_opt(o, h.expected_windows_version);
    o.end_dict();
}

// NEFile(path): {ne_off, h}
void open(Call &c) {
    N::Module m = N::Module::open(owned(c.data));
    c.out.begin_dict();
    c.out.item("ne_off", int64_t(m.ne_offset()));
    c.out.key("h");
    header_dict(c.out, m.header());
    c.out.end_dict();
}

// from_buffer(data, ne_off): the header, after its signature check
void from_buffer(Call &c) { header_dict(c.out, N::Module::parse(owned(c.data), uint64_t(c.int_arg(0))).header()); }
void header(Call &c) { header_dict(c.out, N::Module::parse(owned(c.data), uint64_t(c.int_arg(0))).header()); }
// u16(off) / u32(off): struct.unpack_from's offset, a negative one counting from the end of the file
template <class T>
void value_at(Call &c) {
    int64_t off = c.int_arg(kArgs);
    if (off < 0) off += c.data.size();
    if (off < 0) raise("struct.error", "offset " + dec(c.int_arg(kArgs)) + " out of range for " + dec(c.data.size()) + "-byte buffer");
    c.out.integer(cb::formats::get<T>(view(c.data), uint64_t(off), "ne"));
}
void u16(Call &c) { value_at<uint16_t>(c); }
void u32(Call &c) { value_at<uint32_t>(c); }

void names(Out &o, const std::vector<N::Name> &ns) {
    o.begin_list();
    for (const N::Name &n : ns) {
        o.begin_tuple();
        o.text(n.name);
        o.integer(n.ordinal);
        o.end_tuple();
    }
    o.end_list();
}

void read_name_table(Call &c) { names(c.out, module(c).read_name_table(uint64_t(c.int_arg(kArgs)))); }
void resident_names(Call &c) { names(c.out, module(c).resident_names()); }
void nonresident_names(Call &c) { names(c.out, module(c).nonresident_names()); }
void module_name(Call &c) { c.out.text(module(c).module_name()); }

void module_refs(Call &c) {
    c.out.begin_list();
    for (const std::string &s : module(c).module_refs()) c.out.text(s);
    c.out.end_list();
}

void imported_name(Call &c) { c.out.text(module(c).imported_name(uint64_t(c.int_arg(kArgs)))); }

void segments(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const N::Segment &s : module(c).segments()) {
        o.begin_dict();
        o.item("idx", s.index);
        o.item("file_off", int64_t(s.file_offset));
        o.item("length", s.length);
        o.item("flags", s.flags);
        o.item("minalloc", s.min_alloc);
        o.item_text("kind", s.is_data() ? "DATA" : "CODE");
        o.end_dict();
    }
    o.end_list();
}

// relocations(seg): seg is the segment's dict as segments() gave it (or as the caller made it)
void relocations(Call &c) {
    const Value &seg = c.arg(kArgs);
    if (seg.kind != Value::Dict) raise("TypeError", "a segment that is not a dict");
    N::Module m = module(c);
    N::Segment s;
    s.flags = uint16_t(key_int(seg, "flags"));
    std::vector<N::Relocation> recs;
    if (s.flags & N::kRelocInfo) {
        s.file_offset = uint64_t(key_int(seg, "file_off"));
        s.length = uint32_t(key_int(seg, "length"));
        recs = m.relocations(s);
    }
    Out &o = c.out;
    o.begin_list();
    for (const N::Relocation &r : recs) {
        o.begin_dict();
        o.item("src", r.source);
        o.item("rel_type", r.flags);
        o.key("additive");
        o.boolean(r.additive());
        o.item("offset", r.offset);
        if (auto *a = std::get_if<N::InternalRef>(&r.target)) {
            o.item_text("kind", "INTERNAL");
            o.item("seg", a->segment);
            o.item("entry", a->entry);
        } else if (auto *b = std::get_if<N::ImportOrdinalRef>(&r.target)) {
            o.item_text("kind", "IMPORTORD");
            o.item("module", b->module);
            o.item("ordinal", b->ordinal);
        } else if (auto *n = std::get_if<N::ImportNameRef>(&r.target)) {
            o.item_text("kind", "IMPORTNAME");
            o.item("module", n->module);
            o.item("name_off", n->name_offset);
        } else {
            o.item_text("kind", "OSFIXUP");
            o.item("ftype", std::get<N::OsFixupRef>(r.target).fixup_type);
        }
        o.end_dict();
    }
    o.end_list();
}

void entries(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const N::Entry &e : module(c).entries()) {
        o.begin_dict();
        o.item("ordinal", e.ordinal);
        if (e.kind == N::Entry::Kind::Unused) {
            o.item_text("kind", "UNUSED");
        } else {
            o.item_text("kind", e.kind == N::Entry::Kind::Moveable ? "MOVEABLE" : "FIXED");
            o.item("seg", e.segment);
            o.item("offset", e.offset);
            o.item("flags", e.flags);
        }
        o.end_dict();
    }
    o.end_list();
}

void export_ordinals(Call &c) {
    c.out.begin_list();
    for (uint32_t v : module(c).export_ordinals()) c.out.integer(v);
    c.out.end_list();
}

void resources(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const N::Resource &r : module(c).resources()) {
        o.begin_dict();
        o.item_text("type", r.type);
        o.item_text("id", r.id);
        o.item("off", int64_t(r.file_offset));
        o.item("length", int64_t(r.length));
        o.item("flags", r.flags);
        o.end_dict();
    }
    o.end_list();
}

void rsrc_string(Call &c) {
    c.out.text(module(c).resource_string(uint64_t(c.int_arg(kArgs)), uint64_t(c.int_arg(kArgs + 1))));
}

void embedded_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (auto &e : N::embedded(view(c.data), uint64_t(c.int_arg(0, 0x40)))) {
        o.begin_tuple();
        o.integer(int64_t(e.first));
        o.text(e.second);
        o.end_tuple();
    }
    o.end_list();
}

void tables_end_q(Call &c) { c.out.integer(int64_t(module(c).tables_end())); }
void standalone_q(Call &c) { put(c.out, module(c).standalone()); }

}  // namespace

CBF_QUESTIONS(ne) {
    add("ne.open", open, "NEOpen{ne_off: u32, h: NEHeader{linker_ver: u8, linker_rev: u8, entry_off: u16, entry_len: u16, crc: u32, flags: u16, autodata_seg: u16, heap: u16, stack: u16, csip: u32, sssp: u32, n_segs: u16, n_modrefs: u16, nonres_size: u16, seg_tab: u16, rsrc_tab: u16, resname_tab: u16, mod_tab: u16, imp_tab: u16, nonres_off: u32, n_movable: u16, align_shift: u16, n_resources: u16, exetype: u8, expect_winver: u16?}}");
    add("ne.from_buffer", from_buffer, "NEHeader{linker_ver: u8, linker_rev: u8, entry_off: u16, entry_len: u16, crc: u32, flags: u16, autodata_seg: u16, heap: u16, stack: u16, csip: u32, sssp: u32, n_segs: u16, n_modrefs: u16, nonres_size: u16, seg_tab: u16, rsrc_tab: u16, resname_tab: u16, mod_tab: u16, imp_tab: u16, nonres_off: u32, n_movable: u16, align_shift: u16, n_resources: u16, exetype: u8, expect_winver: u16?}");
    add("ne._parse_header", header, "NEHeader{linker_ver: u8, linker_rev: u8, entry_off: u16, entry_len: u16, crc: u32, flags: u16, autodata_seg: u16, heap: u16, stack: u16, csip: u32, sssp: u32, n_segs: u16, n_modrefs: u16, nonres_size: u16, seg_tab: u16, rsrc_tab: u16, resname_tab: u16, mod_tab: u16, imp_tab: u16, nonres_off: u32, n_movable: u16, align_shift: u16, n_resources: u16, exetype: u8, expect_winver: u16?}");
    add("ne.u16", u16, "u16");
    add("ne.u32", u32, "u32");
    add("ne._read_name_table", read_name_table, "list[(str, u16)]");
    add("ne.resident_names", resident_names, "list[(str, u16)]");
    add("ne.nonresident_names", nonresident_names, "list[(str, u16)]");
    add("ne.module_name", module_name, "str");
    add("ne.module_refs", module_refs, "list[str]");
    add("ne.imported_name", imported_name, "str");
    add("ne.segments", segments, "list[Segment{idx: u16, file_off: u32, length: u32, flags: u16, minalloc: u32, kind: str}]");
    add("ne.relocations", relocations, "list[Relocation{src: u8, rel_type: u8, additive: bool, offset: u16, kind: str, seg?: u8, entry?: u16, module?: u16, ordinal?: u16, name_off?: u16, ftype?: u16}]");
    add("ne.entries", entries, "list[Entry{ordinal: u16, kind: str, seg?: u8, offset?: u16, flags?: u8}]");
    add("ne.export_ordinals", export_ordinals, "list[u16]");
    add("ne.resources", resources, "list[NEResource{type: str, id: str, off: u32, length: u32, flags: u16}]");
    add("ne._rsrc_string", rsrc_string, "str");
    add("ne.embedded", embedded_q, "list[(u32, str)]");
    add("ne._tables_end", tables_end_q, "u32");
    add("ne.standalone", standalone_q, "bytes");
}

}  // namespace cbf::ne
