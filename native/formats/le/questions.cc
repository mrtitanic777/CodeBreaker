// questions.cc -- the LE reader's answers, in the shapes codebreaker/formats/le.py gives them.
//
// An image's questions take its header's offset (lfa) first; the image is read again from the
// bytes and lfa on every call, as LEFile keeps nothing that is not in them.
#include "../core/questions.h"
#include "../core/text.h"
#include "le.h"

namespace cbf::le {
namespace {

Image image(Call &c) { return Image(c.data, c.int_arg(0)); }

void object_dict(Out &o, const Object &ob) {
    o.begin_dict();
    o.item("idx", ob.idx);
    o.item("vsize", ob.vsize);
    o.item("base", ob.base);
    o.item("flags", ob.flags);
    o.item("pmidx", ob.pmidx);
    o.item("pcnt", ob.pcnt);
    o.key("exec");
    o.boolean(ob.exec());
    o.key("read");
    o.boolean(ob.read());
    o.key("write");
    o.boolean(ob.write());
    o.end_dict();
}

void place(Out &o, const Place &p) {
    o.begin_tuple();
    o.integer(p.first);
    o.integer(p.second);
    o.end_tuple();
}

void maybe_place(Out &o, const std::optional<Place> &p) {
    if (p) place(o, *p);
    else o.none();
}

// LEFile(data, lfa): every attribute it sets but d (the caller's own bytes)
void open(Call &c) {
    Image lf = image(c);
    Out &o = c.out;
    o.begin_dict();
    o.item("lfa", lf.lfa);
    o.key("sig");
    o.bytes(lf.sig);
    o.item("cpu", lf.cpu);
    o.item("ostype", lf.ostype);
    o.item("mpages", lf.mpages);
    o.item("eip_obj", lf.eip_obj);
    o.item("eip", lf.eip);
    o.item("pagesize", lf.pagesize);
    o.item("last_page_bytes", lf.last_page_bytes);
    o.item("objtab", lf.objtab);
    o.item("nobj", lf.nobj);
    o.item("pagemap", lf.pagemap);
    o.item("resnames", lf.resnames);
    o.item("entrytab", lf.entrytab);
    o.item("fixup_page", lf.fixup_page);
    o.item("fixup_rec", lf.fixup_rec);
    o.item("datapages", lf.datapages);
    o.key("_page_order");
    o.none();
    o.key("objects");
    o.begin_list();
    for (const Object &ob : lf.objects) object_dict(o, ob);
    o.end_list();
    o.end_dict();
}

void from_buffer(Call &c) { c.out.integer(Image::from_buffer(c.data).lfa); }

// page_to_obj -> (the object's idx or None, offset): the shim hands back the object itself
void page_to_obj(Call &c) {
    Image lf = image(c);
    auto [obj, off] = lf.page_to_obj(c.int_arg(1));
    c.out.begin_tuple();
    if (obj) c.out.integer(obj->idx);
    else c.out.none();
    c.out.integer(off);
    c.out.end_tuple();
}

void fixups(Call &c) {
    std::vector<size_t> shared;
    auto fx = image(c).fixups(&shared);
    Out &o = c.out;
    std::map<size_t, size_t> memo;                       // a record's dict, written once (one object)
    o.begin_dict();
    for (size_t k = 0; k < fx.size(); ++k) {
        place(o, fx[k].first);
        auto it = memo.find(shared[k]);
        if (it != memo.end()) {
            o.again(it->second);
            continue;
        }
        const Fixup &f = fx[k].second;
        o.begin_dict();
        if (f.import) {
            o.item_text("kind", "import");
            o.item("module", f.obj_or_module);
            o.item("ordinal", f.ordinal);
        } else {
            o.item_text("kind", "internal");
            o.item("obj", f.obj_or_module);
            o.key("target");
            if (f.target) o.integer(*f.target);
            else o.none();
            o.item("srctype", f.srctype);
        }
        o.end_dict();
        memo[shared[k]] = o.memo();
    }
    o.end_dict();
}

void import_modules(Call &c) {
    c.out.begin_list();
    for (auto &n : image(c).import_modules()) c.out.text(n);
    c.out.end_list();
}

void resident_name(Call &c) { c.out.text(image(c).resident_name()); }
void description(Call &c) { c.out.text(image(c).description()); }

void header(Call &c) {
    Image lf = image(c);
    auto u = [&](int64_t o) { return int64_t(lf.header_u(o, 4)); };
    auto h = [&](int64_t o) { return int64_t(lf.header_u(o, 2)); };
    Out &o = c.out;
    o.begin_dict();
    o.key("signature");
    o.latin1(lf.sig);
    o.item("cpu", lf.cpu);
    o.item("os", lf.ostype);
    o.item("module_version", u(0x0c));
    o.item("module_flags", u(0x10));
    o.item("pages", lf.mpages);
    o.item("entry_object", lf.eip_obj);
    o.item("entry", lf.eip);
    o.item("stack_object", u(0x20));
    o.item("stack", u(0x24));
    o.item("page_size", lf.pagesize);
    o.item("last_page_bytes", lf.last_page_bytes);
    o.item("fixup_section_size", u(0x30));
    o.item("loader_section_size", u(0x38));
    o.item("objects", lf.nobj);
    o.item("imported_modules", u(0x74));
    o.item("data_pages_offset", lf.datapages);
    o.item("device_id", h(0xC0));
    o.item("ddk_version", h(0xC2));
    o.end_dict();
}

void ddb(Call &c) {
    auto r = image(c).ddb();
    Out &o = c.out;
    if (!r) {
        o.none();
        return;
    }
    o.begin_dict();
    o.item("obj", r->obj);
    o.item("rva", r->rva);
    o.item("version", r->version);
    o.item("device_id", r->device_id);
    o.item("major", r->major);
    o.item("minor", r->minor);
    o.item("flags", r->flags);
    o.key("name");
    o.latin1(Bytes(r->name));
    o.item("init_order", r->init_order);
    o.item("control_proc", r->control_proc);
    o.end_dict();
}

int64_t key_int(const Value &d, const char *k) {
    if (d.kind != Value::Dict) raise("TypeError", "an object that is not a dict");
    const Value *v = d.get(k);
    if (!v) raise("KeyError", std::string("'") + k + "'");
    return v->as_int();
}

// object_data(lf, obj) / LEFile.object_bytes(obj)
void object_data_q(Call &c) {
    Image lf = image(c);
    const Value &obj = c.arg(1);
    int64_t pmidx = key_int(obj, "pmidx"), pcnt = key_int(obj, "pcnt"), vsize = key_int(obj, "vsize");
    c.out.bytes(Bytes(object_data(lf, pmidx, pcnt, vsize)));
}

void w3(Call &c) {
    W3 w(c.data, c.int_arg(0));
    Out &o = c.out;
    o.begin_dict();
    o.item("version", w.version);
    o.item("count", w.count);
    o.key("entries");
    o.begin_list();
    for (const W3Entry &e : w.entries) {
        o.begin_dict();
        o.item_text("name", e.name);
        o.item("off", e.off);
        o.item("size", e.size);
        o.end_dict();
    }
    o.end_list();
    o.end_dict();
}

void name_offsets(Out &o, const std::vector<std::pair<std::string, int64_t>> &mods) {
    o.begin_list();
    for (auto &m : mods) {
        o.begin_tuple();
        o.text(m.first);
        o.integer(m.second);
        o.end_tuple();
    }
    o.end_list();
}

void w3_modules(Call &c) { name_offsets(c.out, W3(c.data, c.int_arg(0)).modules()); }

std::vector<std::pair<std::string, int64_t>> vxds(Bytes d, const std::string &basename) {
    int64_t lfa = d.u32(0x3c);
    Bytes sig = d.slice(lfa, lfa + 2);
    if (sig.is("W3", 2)) return W3(d, lfa).modules();
    if (sig.is("LE", 2) || sig.is("LX", 2)) return {{basename, lfa}};
    raise("ValueError", "not an LE/LX/W3 image");
}

// open_vxds(path) -> [(name, lfa)]: the shim opens each LEFile
void open_vxds(Call &c) { name_offsets(c.out, vxds(c.data, c.str_arg(0))); }

// members(path) -> (the expanded bytes, or None for the file's own, [(name, lfa)])
void members(Call &c) {
    Out &o = c.out;
    auto cont = container(c.data);
    o.begin_tuple();
    if (!cont) {
        o.none();
        name_offsets(o, vxds(c.data, c.str_arg(0)));
    } else {
        std::string data = expand(c.data);
        Bytes d(data);
        auto c2 = container(d);
        if (!c2) raise("TypeError", "'NoneType' object is not subscriptable");
        std::vector<std::pair<std::string, int64_t>> mods;
        for (auto &m : c2->members) {
            Bytes s = d.slice(std::get<1>(m), int64_t(std::get<1>(m)) + 2);
            if (s.is("LE", 2) || s.is("LX", 2)) mods.emplace_back(std::get<0>(m), std::get<1>(m));
        }
        if (data == c.data.str()) o.none();
        else o.bytes(d);
        name_offsets(o, mods);
    }
    o.end_tuple();
}

void header_fields_q(Call &c) {
    Out &o = c.out;
    o.begin_dict();
    for (auto &kv : header_fields(image(c))) o.item(kv.first, int64_t(kv.second));
    o.end_dict();
}

void is_member_q(Call &c) { c.out.boolean(is_member(image(c))); }

void page_map_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (auto &e : page_map(image(c))) {
        o.begin_tuple();
        o.integer(e.number);
        o.integer(e.kind);
        o.end_tuple();
    }
    o.end_list();
}

void file_offset_q(Call &c) {
    auto r = file_offset(image(c), c.int_arg(1), c.int_arg(2));
    if (r) c.out.integer(*r);
    else c.out.none();
}

void pairs(Out &o, const std::vector<std::pair<std::string, uint16_t>> &ns) {
    o.begin_list();
    for (auto &n : ns) {
        o.begin_tuple();
        o.text(n.first);
        o.integer(n.second);
        o.end_tuple();
    }
    o.end_list();
}

void names_at_q(Call &c) {
    std::optional<int64_t> end;
    if (c.argc() > 1 && !c.arg(1).is_none()) end = c.arg(1).as_int();
    pairs(c.out, names_at(c.data, c.int_arg(0), end));
}

void names_q(Call &c) {
    auto [res, non] = names(image(c));
    c.out.begin_tuple();
    pairs(c.out, res);
    pairs(c.out, non);
    c.out.end_tuple();
}

void entries_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (auto &e : entries(image(c))) {
        o.begin_tuple();
        o.integer(e.ordinal);
        o.integer(e.obj);
        o.integer(e.off);
        o.integer(e.flags);
        o.end_tuple();
    }
    o.end_list();
}

void entry_table_size_q(Call &c) { c.out.integer(entry_table_size(image(c))); }

void exports_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (auto &e : exports(image(c))) {
        o.begin_tuple();
        o.text(e.name);
        o.integer(e.ordinal);
        o.integer(e.obj);
        o.integer(e.off);
        o.end_tuple();
    }
    o.end_list();
}

void fixup_records_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const FixupRecord &r : fixup_records(image(c))) {
        o.begin_tuple();
        o.integer(r.page);
        o.begin_dict();
        o.item("source", r.source);
        o.item("source_flags", r.source_flags);
        o.item("flags", r.flags);
        switch (r.kind) {
        case FixupRecord::Internal:
            o.item_text("kind", "internal");
            o.item("obj", r.first);
            o.key("target");
            if (r.target) o.uinteger(*r.target);
            else o.none();
            break;
        case FixupRecord::Import:
            o.item_text("kind", "import");
            o.item("module", r.first);
            o.key("ordinal");
            o.uinteger(r.ordinal);
            break;
        case FixupRecord::ImportName:
            o.item_text("kind", "import-name");
            o.item("module", r.first);
            o.key("name");
            o.uinteger(r.name);
            break;
        case FixupRecord::Entry:
            o.item_text("kind", "entry");
            o.item("entry", r.first);
            break;
        }
        if (r.addend) {
            o.key("addend");
            o.uinteger(*r.addend);
        }
        o.key("sources");
        o.begin_list();
        for (int64_t s : r.sources) o.integer(s);
        o.end_list();
        o.key("raw");
        o.bytes(r.raw);
        o.end_dict();
        o.end_tuple();
    }
    o.end_list();
}

void encode_fixup_q(Call &c) { c.out.bytes(Bytes(encode_fixup(c.arg(0)))); }

void references_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (auto &r : references(image(c))) {
        o.begin_tuple();
        o.integer(r.obj);
        o.integer(r.off);
        o.integer(r.source);
        o.integer(r.target_obj);
        o.integer(r.target_off);
        o.end_tuple();
    }
    o.end_list();
}

void pointers_q(Call &c) {
    Out &o = c.out;
    o.begin_dict();
    for (auto &kv : pointers(image(c))) {
        place(o, kv.first);
        place(o, kv.second);
    }
    o.end_dict();
}

void plausible_q(Call &c) {
    Bytes b = c.data;
    int64_t at = c.int_arg(0);
    bool ok = false;
    if (!(at < 0 || at + 0x38 > b.size())) {
        Bytes name = b.slice(at + 0x0C, at + 0x14);
        int version = b.at(at + 4) | (b.at(at + 5) << 8);
        bool printable = true;
        for (uint8_t ch : name) printable = printable && ch >= 32 && ch < 127;
        ok = 0x0300 <= version && version <= 0x04FF && b_isalnum(name.slice(0, 1)) && printable;
    }
    c.out.boolean(ok);
}

void descriptor_out(Out &o, const Descriptor &d) {
    static const char *kAddr[] = {"control", "v86_api", "pm_api", "service_table", "reference_data"};
    o.begin_dict();
    o.item("obj", d.obj);
    o.item("offset", d.offset);
    o.item_text("name", d.name);
    for (auto &f : d.fields) {
        o.key(f.first);
        auto it = d.addresses.find(f.first);
        if (it != d.addresses.end()) maybe_place(o, it->second);
        else o.uinteger(f.second);
    }
    for (const char *f : kAddr) {
        o.key((std::string(f) + "_raw").c_str());
        o.uinteger(d.field(f));
    }
    o.item("size", d.size);
    o.key("bytes");
    o.bytes(d.bytes);
    o.end_dict();
}

void descriptor_q(Call &c) {
    auto d = descriptor(image(c));
    if (d) descriptor_out(c.out, *d);
    else c.out.none();
}

std::optional<Place> place_of(const Value &v) {
    if (v.is_none()) return std::nullopt;
    if (v.items.size() != 2) raise("ValueError", "not enough values to unpack");
    return Place{v.items[0].as_int(), v.items[1].as_int()};
}

void service_table_q(Call &c) {
    Image lf = image(c);
    const Value &given = c.arg(1);
    std::optional<Place> st;
    int64_t count = 0;
    if (given.truthy()) {                                // a descriptor the caller holds
        if (given.kind != Value::Dict) raise("TypeError", "a descriptor that is not a dict");
        const Value *t = given.get("service_table");
        if (!t) raise("KeyError", "'service_table'");
        if (!t->truthy()) {
            c.out.begin_list();
            c.out.end_list();
            return;
        }
        const Value *n = given.get("service_count");
        if (!n) raise("KeyError", "'service_count'");
        count = n->as_int();
        if (0 < count && count < 0x1000) st = place_of(*t);
    } else {
        auto d = descriptor(lf);
        if (d) {
            st = d->addresses.at("service_table");
            count = int64_t(d->field("service_count"));
        }
    }
    Out &o = c.out;
    o.begin_list();
    for (auto &p : service_table(lf, st, count)) maybe_place(o, p);
    o.end_list();
}

void entry_points_q(Call &c) {
    Image lf = image(c);
    auto d = descriptor(lf);
    Out &o = c.out;
    o.begin_dict();
    if (d) {
        for (const char *f : {"control", "v86_api", "pm_api"}) {
            auto p = d->addresses.at(f);
            if (p) {
                o.key(f);
                place(o, *p);
            }
        }
        auto st = service_table(lf, d->addresses.at("service_table"), int64_t(d->field("service_count")));
        for (size_t i = 0; i < st.size(); ++i)
            if (st[i]) {
                o.key(("service_" + std::to_string(i)).c_str());
                place(o, *st[i]);
            }
    }
    o.end_dict();
}

void service_calls_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (auto &s : service_calls(image(c))) {
        o.begin_tuple();
        o.integer(s.obj);
        o.integer(s.off);
        o.integer(s.device);
        o.integer(s.service);
        o.boolean(s.jump);
        o.end_tuple();
    }
    o.end_list();
}

void read_image_q(Call &c) { read_image(image(c), c.out); }
void write_image_q(Call &c) { c.out.bytes(Bytes(write_image(c.arg(0)))); }

void container_q(Call &c) {
    auto k = container(c.data);
    Out &o = c.out;
    if (!k) {
        o.none();
        return;
    }
    o.begin_dict();
    o.item_text("kind", k->kind);
    o.item("at", k->at);
    o.item("version", k->version);
    if (k->kind == "W3") {
        o.key("reserved");
        o.bytes(k->reserved);
    }
    o.key("members");
    o.begin_list();
    for (auto &m : k->members) {
        o.begin_tuple();
        o.text(std::get<0>(m));
        o.integer(std::get<1>(m));
        o.integer(std::get<2>(m));
        o.end_tuple();
    }
    o.end_list();
    o.end_dict();
}

void object_table_q(Call &c) { c.out.bytes(Bytes(object_table(c.arg(0)))); }

void fixup_tables_q(Call &c) {
    const Value &fx = c.arg(0);
    if (fx.kind != Value::Dict) raise("TypeError", "fix-ups that are not a dict");
    std::map<int64_t, std::vector<std::string>> m;
    for (size_t k = 0; k + 1 < fx.items.size(); k += 2) {
        std::vector<std::string> recs;
        for (const Value &r : fx.items[k + 1].items) recs.push_back(std::string(r.as_bytes().str()));
        m[fx.items[k].as_int()] = recs;
    }
    auto [table, records] = fixup_tables(m, c.int_arg(1));
    c.out.begin_tuple();
    c.out.bytes(Bytes(table));
    c.out.bytes(Bytes(records));
    c.out.end_tuple();
}

void pack_directory_q(Call &c) { c.out.bytes(Bytes(pack_directory(c.arg(0)))); }
void expand_page_q(Call &c) { c.out.bytes(Bytes(expand_page(c.data, c.int_arg(0, 0x2000)))); }
void compress_page_q(Call &c) { c.out.bytes(Bytes(compress_page(c.data))); }
void expand_q(Call &c) { c.out.bytes(Bytes(expand(c.data))); }
void compress_q(Call &c) { c.out.bytes(Bytes(compress(c.data, c.arg(0).as_bytes()))); }

}  // namespace

CBF_QUESTIONS(le) {
    add("le.open", open, "LeOpen{lfa: int, sig: bytes, cpu: int, ostype: int, mpages: int, eip_obj: int, eip: int, pagesize: int, last_page_bytes: int, objtab: int, nobj: int, pagemap: int, resnames: int, entrytab: int, fixup_page: int, fixup_rec: int, datapages: int, _page_order: none, objects: list[LeObject{idx: int, vsize: int, base: int, flags: int, pmidx: int, pcnt: int, exec: bool, read: bool, write: bool}]}");
    add("le.from_buffer", from_buffer, "int");
    add("le.page_to_obj", page_to_obj, "(int?, int)");
    add("le.fixups", fixups);
    add("le.import_modules", import_modules, "list[str]");
    add("le.resident_name", resident_name, "str");
    add("le.header", header, "LeHeader{signature: str, cpu: int, os: int, module_version: int, module_flags: int, pages: int, entry_object: int, entry: int, stack_object: int, stack: int, page_size: int, last_page_bytes: int, fixup_section_size: int, loader_section_size: int, objects: int, imported_modules: int, data_pages_offset: int, device_id: int, ddk_version: int}");
    add("le.description", description, "str");
    add("le.ddb", ddb, "LeDdb{obj: int, rva: int, version: int, device_id: int, major: int, minor: int, flags: int, name: str, init_order: int, control_proc: int}?");
    add("le.object_bytes", object_data_q, "bytes");
    add("le.w3", w3, "W3{version: int, count: int, entries: list[W3Entry{name: str, off: int, size: int}]}");
    add("le.w3_modules", w3_modules, "list[(str, int)]");
    add("le.open_vxds", open_vxds, "list[(str, int)]");
    add("le.members", members, "(bytes?, list[(str, int)])");
    add("le.header_fields", header_fields_q, "dict[str, int]");
    add("le.is_member", is_member_q, "bool");
    add("le.page_map", page_map_q, "list[(u32, u8)]");
    add("le.object_data", object_data_q, "bytes");
    add("le.file_offset", file_offset_q, "int?");
    add("le._names", names_at_q, "list[(str, u16)]");
    add("le.names", names_q, "(list[(str, u16)], list[(str, u16)])");
    add("le.entries", entries_q, "list[(u16, u16, u32, u8)]");
    add("le._entry_table_size", entry_table_size_q, "u32");
    add("le.exports", exports_q, "list[(str, u16, u16, u32)]");
    add("le.fixup_records", fixup_records_q, "list[(int, LeFixupRecord{source: int, source_flags: int, flags: int, kind: str, obj?: int, target?: int?, module?: int, ordinal?: int, name?: int, entry?: int, addend?: int, sources: list[int], raw: bytes})]");
    add("le.encode_fixup", encode_fixup_q, "bytes");
    add("le.references", references_q, "list[(u16, u32, u32, u16, u32)]");
    add("le.pointers", pointers_q, "dict[(u16, u32), (u16, u32)]");
    add("le._plausible", plausible_q, "bool");
    add("le.descriptor", descriptor_q);
    add("le.service_table", service_table_q, "list[(u16, u32)?]");
    add("le.entry_points", entry_points_q, "dict[str, (u16, u32)]");
    add("le.service_calls", service_calls_q, "list[(u16, u32, u16, u16, bool)]");
    add("le.read_image", read_image_q);
    add("le.write_image", write_image_q, "bytes");
    add("le.container", container_q, "LeContainer{kind: str, at: int, version: int, reserved?: bytes, members: list[(str, int, int)]}?");
    add("le.pack_directory", pack_directory_q, "bytes");
    add("le.object_table", object_table_q, "bytes");
    add("le._name_table", [](Call &c) { c.out.bytes(Bytes(name_table(c.arg(0)))); }, "bytes");
    add("le.fixup_tables", fixup_tables_q, "(bytes, bytes)");
    add("le.expand_page", expand_page_q, "bytes");
    add("le.compress_page", compress_page_q, "bytes");
    add("le.expand", expand_q, "bytes");
    add("le.compress", compress_q, "bytes");
}

}  // namespace cbf::le
