// questions.cc -- the NE reader's answers, in the shapes codebreaker/formats/ne.py gives them.
//
// A module's questions take its place first -- (ne_off, base, overlay_at or None) -- then the
// method's own arguments: the module is read again from those on every call (NEFile keeps
// nothing else), and overlay_at not None makes it a boot image's module (BootImageModule).
#include "../core/questions.h"
#include "../core/text.h"
#include "ne.h"

namespace cbf::ne {
namespace {

constexpr size_t kArgs = 3;   // the module's place, before a method's own arguments

Module module(Call &c) {
    Module m(c.data, c.int_arg(0));
    m.base = c.int_arg(1);
    if (!c.arg(2).is_none()) {
        m.boot = true;
        m.overlay_at = c.int_arg(2);
    }
    return m;
}

void header_dict(Out &o, const Header &h) {
    o.begin_dict();
    o.item("linker_ver", h.linker_ver);
    o.item("linker_rev", h.linker_rev);
    o.item("entry_off", h.entry_off);
    o.item("entry_len", h.entry_len);
    o.item("crc", h.crc);
    o.item("flags", h.flags);
    o.item("autodata_seg", h.autodata_seg);
    o.item("heap", h.heap);
    o.item("stack", h.stack);
    o.item("csip", h.csip);
    o.item("sssp", h.sssp);
    o.item("n_segs", h.n_segs);
    o.item("n_modrefs", h.n_modrefs);
    o.item("nonres_size", h.nonres_size);
    o.item("seg_tab", h.seg_tab);
    o.item("rsrc_tab", h.rsrc_tab);
    o.item("resname_tab", h.resname_tab);
    o.item("mod_tab", h.mod_tab);
    o.item("imp_tab", h.imp_tab);
    o.item("nonres_off", h.nonres_off);
    o.item("n_movable", h.n_movable);
    o.item("align_shift", h.align_shift);
    o.item("n_resources", h.n_resources);
    o.item("exetype", h.exetype);
    o.key("expect_winver");
    if (h.expect_winver) o.integer(*h.expect_winver);
    else o.none();
    o.end_dict();
}

// NEFile(path): {ne_off, h}
void open(Call &c) {
    Module m = Module::open(c.data);
    c.out.begin_dict();
    c.out.item("ne_off", m.ne_off);
    c.out.key("h");
    header_dict(c.out, m.h);
    c.out.end_dict();
}

// from_buffer(data, ne_off): the header, after its signature check
void from_buffer(Call &c) {
    int64_t off = c.int_arg(0);
    if (!c.data.slice_is(off, off + 2, "NE", 2)) raise("ValueError", "no NE at 0x" + hex(uint64_t(off)));
    header_dict(c.out, Module(c.data, off).h);
}

void header(Call &c) { header_dict(c.out, Module(c.data, c.int_arg(0)).h); }
void u16(Call &c) { c.out.integer(c.data.u16(c.int_arg(kArgs))); }
void u32(Call &c) { c.out.integer(c.data.u32(c.int_arg(kArgs))); }

void names(Out &o, const std::vector<Name> &ns) {
    o.begin_list();
    for (const Name &n : ns) {
        o.begin_tuple();
        o.latin1(n.name);
        o.integer(n.ordinal);
        o.end_tuple();
    }
    o.end_list();
}

void read_name_table(Call &c) { names(c.out, module(c).read_name_table(c.int_arg(kArgs))); }
void resident_names(Call &c) { names(c.out, module(c).resident_names()); }
void nonresident_names(Call &c) { names(c.out, module(c).nonresident_names()); }
void module_name(Call &c) { c.out.text(module(c).module_name()); }

void module_refs(Call &c) {
    c.out.begin_list();
    for (const std::string &s : module(c).module_refs()) c.out.text(s);
    c.out.end_list();
}

void imported_name(Call &c) { c.out.text(module(c).imported_name(c.int_arg(kArgs))); }

void segments(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const Segment &s : module(c).segments()) {
        o.begin_dict();
        o.item("idx", s.idx);
        o.item("file_off", s.file_off);
        o.item("length", s.length);
        o.item("flags", s.flags);
        o.item("minalloc", s.minalloc);
        o.item_text("kind", s.is_data() ? "DATA" : "CODE");
        o.end_dict();
    }
    o.end_list();
}

// relocations(seg): seg is the segment's dict as segments() gave it (or as the caller made it)
int64_t key_int(const Value &d, const char *k) {
    const Value *v = d.get(k);
    if (!v) raise("KeyError", std::string("'") + k + "'");
    return v->as_int();
}

void relocations(Call &c) {
    const Value &seg = c.arg(kArgs);
    if (seg.kind != Value::Dict) raise("TypeError", "a segment that is not a dict");
    Module m = module(c);
    const int64_t flags = key_int(seg, "flags");
    std::vector<Relocation> recs;
    if (flags & kRelocInfo) recs = m.relocations(uint16_t(flags), key_int(seg, "file_off"), key_int(seg, "length"));
    Out &o = c.out;
    o.begin_list();
    for (const Relocation &r : recs) {
        o.begin_dict();
        o.item("src", r.src);
        o.item("rel_type", r.rel_type);
        o.key("additive");
        o.boolean(r.additive());
        o.item("offset", r.offset);
        switch (r.kind) {
        case Relocation::Internal:
            o.item_text("kind", "INTERNAL");
            o.item("seg", r.seg);
            o.item("entry", r.entry);
            break;
        case Relocation::ImportOrdinal:
            o.item_text("kind", "IMPORTORD");
            o.item("module", r.module);
            o.item("ordinal", r.ordinal);
            break;
        case Relocation::ImportName:
            o.item_text("kind", "IMPORTNAME");
            o.item("module", r.module);
            o.item("name_off", r.name_off);
            break;
        case Relocation::OsFixup:
            o.item_text("kind", "OSFIXUP");
            o.item("ftype", r.ftype);
            break;
        }
        o.end_dict();
    }
    o.end_list();
}

void entries(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const Entry &e : module(c).entries()) {
        o.begin_dict();
        o.item("ordinal", e.ordinal);
        if (e.kind == Entry::Unused) {
            o.item_text("kind", "UNUSED");
        } else {
            o.item_text("kind", e.kind == Entry::Moveable ? "MOVEABLE" : "FIXED");
            o.item("seg", e.seg);
            o.item("offset", e.offset);
            o.item("flags", e.flags);
        }
        o.end_dict();
    }
    o.end_list();
}

void export_ordinals(Call &c) {
    c.out.begin_list();
    for (int64_t v : module(c).export_ordinals()) c.out.integer(v);
    c.out.end_list();
}

void resources(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const Resource &r : module(c).resources()) {
        o.begin_dict();
        o.item_text("type", r.type);
        o.item_text("id", r.id);
        o.item("off", r.off);
        o.item("length", r.length);
        o.item("flags", r.flags);
        o.end_dict();
    }
    o.end_list();
}

void rsrc_string(Call &c) { c.out.text(module(c).rsrc_string(c.int_arg(kArgs), c.int_arg(kArgs + 1))); }

void embedded_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (auto &e : embedded(c.data, c.int_arg(0, 0x40))) {
        o.begin_tuple();
        o.integer(e.first);
        o.text(e.second);
        o.end_tuple();
    }
    o.end_list();
}

void tables_end_q(Call &c) { c.out.integer(tables_end(module(c))); }
void standalone_q(Call &c) { c.out.bytes(Bytes(standalone(module(c)))); }

}  // namespace

CBF_QUESTIONS(ne) {
    add("ne.open", open);
    add("ne.from_buffer", from_buffer);
    add("ne._parse_header", header);
    add("ne.u16", u16);
    add("ne.u32", u32);
    add("ne._read_name_table", read_name_table);
    add("ne.resident_names", resident_names);
    add("ne.nonresident_names", nonresident_names);
    add("ne.module_name", module_name);
    add("ne.module_refs", module_refs);
    add("ne.imported_name", imported_name);
    add("ne.segments", segments);
    add("ne.relocations", relocations);
    add("ne.entries", entries);
    add("ne.export_ordinals", export_ordinals);
    add("ne.resources", resources);
    add("ne._rsrc_string", rsrc_string);
    add("ne.embedded", embedded_q);
    add("ne._tables_end", tables_end_q);
    add("ne.standalone", standalone_q);
}

}  // namespace cbf::ne
