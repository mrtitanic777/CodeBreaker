// questions.cc -- the LE reader's answers, in the shapes codebreaker/formats/le.py gives them, read by
// cb::formats::le (native/cb/formats/le.hpp).
//
// An image's questions take its header's offset (lfa) first; the image is read again from the
// bytes and lfa on every call, as LEFile keeps nothing that is not in them.  read_image and
// write_image carry an image's parts (cb::formats::le::Parts) as the dict le.py builds and changes.
#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "../core/cb_bridge.h"
#include "../core/text.h"
#include "cb/formats/le.hpp"
#include "cb/formats/read.hpp"

namespace cbf::le {
namespace {

namespace L = cb::formats::le;

L::Module image(Call &c) { return L::Module::parse(owned(c.data), uint64_t(c.int_arg(0))); }

void object_dict(Out &o, const L::Object &ob) {
    o.begin_dict();
    o.item("idx", ob.index);
    o.item("vsize", ob.vsize);
    o.item("base", ob.base);
    o.item("flags", ob.flags);
    o.item("pmidx", ob.first_page);
    o.item("pcnt", ob.page_count);
    o.key("exec");
    o.boolean(ob.executable());
    o.key("read");
    o.boolean(ob.readable());
    o.key("write");
    o.boolean(ob.writable());
    o.end_dict();
}

void place(Out &o, const L::Place &p) {
    o.begin_tuple();
    o.integer(p.object);
    o.integer(p.offset);
    o.end_tuple();
}

void maybe_place(Out &o, const std::optional<L::Place> &p) {
    if (p) place(o, *p);
    else o.none();
}

std::optional<L::Place> place_of(const Value &v) {
    if (v.is_none()) return std::nullopt;
    if (v.items.size() != 2) raise("ValueError", "not enough values to unpack");
    return L::Place{uint32_t(v.items[0].as_int()), v.items[1].as_int()};
}

// LEFile(data, lfa): every attribute it sets but d (the caller's own bytes)
void open(Call &c) {
    L::Module lf = image(c);
    const L::Header &h = lf.header();
    const int64_t lfa = int64_t(lf.lfa());
    Out &o = c.out;
    o.begin_dict();
    o.item("lfa", lfa);
    o.key("sig");
    o.bytes(Bytes(std::string(lf.signature())));
    o.item("cpu", h.cpu);
    o.item("ostype", h.os);
    o.item("mpages", h.pages);
    o.item("eip_obj", h.eip_object);
    o.item("eip", h.eip);
    o.item("pagesize", h.page_size);
    o.item("last_page_bytes", h.last_page_bytes);
    o.item("objtab", lfa + h.object_table);
    o.item("nobj", h.objects);
    o.item("pagemap", lfa + h.page_map);
    o.item("resnames", lfa + h.resident_names);
    o.item("entrytab", lfa + h.entry_table);
    o.item("fixup_page", lfa + h.fixup_pages);
    o.item("fixup_rec", lfa + h.fixup_records);
    o.item("datapages", h.data_pages);
    o.key("_page_order");
    o.none();
    o.key("objects");
    o.begin_list();
    for (const L::Object &ob : lf.objects()) object_dict(o, ob);
    o.end_list();
    o.end_dict();
}

void from_buffer(Call &c) { c.out.integer(int64_t(L::Module::from_buffer(owned(c.data)).lfa())); }

// page_to_obj -> (the object's idx or None, offset): the shim hands back the object itself
void page_to_obj(Call &c) {
    L::Module lf = image(c);
    const int64_t pg = c.int_arg(1);
    auto r = pg < 0 ? std::nullopt : lf.page_to_object(uint32_t(pg));
    c.out.begin_tuple();
    if (r) {
        c.out.integer(r->first->index);
        c.out.integer(int64_t(r->second));
    } else {
        c.out.none();
        c.out.integer(0);
    }
    c.out.end_tuple();
}

void fixups(Call &c) {
    Out &o = c.out;
    o.begin_dict();
    for (const auto &[at, f] : image(c).fixups()) {
        place(o, at);
        o.begin_dict();
        if (auto *imp = std::get_if<L::ImportFixup>(&f)) {
            o.item_text("kind", "import");
            o.item("module", imp->module);
            o.item("ordinal", imp->ordinal);
        } else {
            const auto &in = std::get<L::InternalFixup>(f);
            o.item_text("kind", "internal");
            o.item("obj", in.object);
            o.key("target");
            put_opt(o, in.target);
            o.item("srctype", in.source_type);
        }
        o.end_dict();
    }
    o.end_dict();
}

void import_modules(Call &c) {
    c.out.begin_list();
    for (const std::string &n : image(c).import_modules()) c.out.text(n);
    c.out.end_list();
}

void resident_name(Call &c) { c.out.text(image(c).resident_name()); }
void description(Call &c) { c.out.text(image(c).description()); }

void header(Call &c) {
    L::Module lf = image(c);
    const L::Header &h = lf.header();
    Out &o = c.out;
    o.begin_dict();
    o.item_text("signature", std::string(lf.signature()));
    o.item("cpu", h.cpu);
    o.item("os", h.os);
    o.item("module_version", h.version);
    o.item("module_flags", h.module_flags);
    o.item("pages", h.pages);
    o.item("entry_object", h.eip_object);
    o.item("entry", h.eip);
    o.item("stack_object", h.esp_object);
    o.item("stack", h.esp);
    o.item("page_size", h.page_size);
    o.item("last_page_bytes", h.last_page_bytes);
    o.item("fixup_section_size", h.fixup_size);
    o.item("loader_section_size", h.loader_size);
    o.item("objects", h.objects);
    o.item("imported_modules", h.import_module_count);
    o.item("data_pages_offset", h.data_pages);
    o.item("device_id", h.device_id);
    o.item("ddk_version", h.kit_version);
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
    o.item("obj", r->object);
    o.item("rva", r->rva);
    o.item("version", r->version);
    o.item("device_id", r->device_id);
    o.item("major", r->major);
    o.item("minor", r->minor);
    o.item("flags", r->flags);
    o.item_text("name", r->name);
    o.item("init_order", r->init_order);
    o.item("control_proc", r->control_proc);
    o.end_dict();
}

// object_data(lf, obj) / LEFile.object_bytes(obj): obj is the object's dict
void object_data_q(Call &c) {
    L::Module lf = image(c);
    const Value &obj = c.arg(1);
    L::Object ob;
    ob.first_page = uint32_t(key_int(obj, "pmidx"));
    ob.page_count = uint32_t(key_int(obj, "pcnt"));
    ob.vsize = uint32_t(key_int(obj, "vsize"));
    put(c.out, lf.object_data(ob));
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

// the W3 directory at `lfa` (W3File): the container the file's own header points at
L::Container w3_at(cb::ByteView d, int64_t lfa) {
    auto k = L::container(d);
    if (!k || k->kind != L::Container::Kind::W3 || int64_t(k->at) != lfa) raise("ValueError", "no W3 directory at 0x" + hex(uint64_t(lfa)));
    return *k;
}

// a W3 directory's members that are LE/LX modules: (name, its header)
std::vector<std::pair<std::string, int64_t>> w3_modules(cb::ByteView d, const L::Container &k) {
    std::vector<std::pair<std::string, int64_t>> out;
    for (const L::ContainerMember &m : k.members)
        if (cb::formats::starts_with(d, m.offset, "LE") || cb::formats::starts_with(d, m.offset, "LX")) out.emplace_back(m.name, m.offset);
    return out;
}

void w3(Call &c) {
    const cb::ByteView d = view(c.data);
    const int64_t lfa = c.int_arg(0);
    const L::Container k = w3_at(d, lfa);
    Out &o = c.out;
    o.begin_dict();
    o.item("version", k.version);
    o.item("count", cb::formats::get<uint32_t>(d, uint64_t(lfa) + 4, "le"));
    o.key("entries");
    o.begin_list();
    for (const L::ContainerMember &e : k.members) {
        o.begin_dict();
        o.item_text("name", e.name);
        o.item("off", e.offset);
        o.item("size", e.size);
        o.end_dict();
    }
    o.end_list();
    o.end_dict();
}

void w3_modules_q(Call &c) {
    const cb::ByteView d = view(c.data);
    name_offsets(c.out, w3_modules(d, w3_at(d, c.int_arg(0))));
}

// the modules of a file: a W3 directory's, or the file's own LE/LX module named `basename`
std::vector<std::pair<std::string, int64_t>> vxds(cb::ByteView d, const std::string &basename) {
    const uint64_t lfa = cb::formats::get<uint32_t>(d, 0x3c, "le");
    if (cb::formats::starts_with(d, lfa, "W3")) return w3_modules(d, w3_at(d, int64_t(lfa)));
    if (cb::formats::starts_with(d, lfa, "LE") || cb::formats::starts_with(d, lfa, "LX")) return {{basename, int64_t(lfa)}};
    raise("ValueError", "not an LE/LX/W3 image");
}

// open_vxds(path) -> [(name, lfa)]: the shim opens each LEFile
void open_vxds(Call &c) { name_offsets(c.out, vxds(view(c.data), c.str_arg(0))); }

// members(path) -> (the expanded bytes, or None for the file's own, [(name, lfa)])
void members(Call &c) {
    Out &o = c.out;
    const cb::ByteView d = view(c.data);
    o.begin_tuple();
    if (!L::container(d)) {
        o.none();
        name_offsets(o, vxds(d, c.str_arg(0)));
    } else {
        const cb::Bytes expanded = L::expand(d);
        auto k = L::container(expanded);
        if (!k) raise("TypeError", "'NoneType' object is not subscriptable");
        if (expanded.size() == d.size() && std::equal(expanded.begin(), expanded.end(), d.begin())) o.none();
        else put(o, expanded);
        name_offsets(o, w3_modules(expanded, *k));
    }
    o.end_tuple();
}

// header_fields(lf): every field of the header, by le.py's names, in the header's order
void header_fields_q(Call &c) {
    const L::Header h = image(c).header();
    const std::pair<const char *, int64_t> fields[] = {
        {"byte_order", h.byte_order}, {"word_order", h.word_order}, {"level", h.level}, {"cpu", h.cpu}, {"os", h.os},
        {"version", h.version}, {"module_flags", h.module_flags}, {"pages", h.pages}, {"eip_object", h.eip_object},
        {"eip", h.eip}, {"esp_object", h.esp_object}, {"esp", h.esp}, {"page_size", h.page_size},
        {"last_page_bytes", h.last_page_bytes}, {"fixup_size", h.fixup_size}, {"loader_size", h.loader_size},
        {"object_table", h.object_table}, {"objects", h.objects}, {"page_map", h.page_map}, {"resources", h.resources},
        {"resident_names", h.resident_names}, {"entry_table", h.entry_table}, {"fixup_pages", h.fixup_pages},
        {"fixup_records", h.fixup_records}, {"import_modules", h.import_modules},
        {"import_module_count", h.import_module_count}, {"import_names", h.import_names}, {"data_pages", h.data_pages},
        {"preload_pages", h.preload_pages}, {"nonresident_names", h.nonresident_names},
        {"nonresident_size", h.nonresident_size}, {"debug_info", h.debug_info}, {"debug_size", h.debug_size},
        {"resource_data", h.resource_data}, {"resource_size", h.resource_size}, {"device_id", h.device_id},
        {"kit_version", h.kit_version}};
    Out &o = c.out;
    o.begin_dict();
    for (const auto &[name, v] : fields) o.item(name, v);
    o.end_dict();
}

void is_member_q(Call &c) { c.out.boolean(image(c).is_member()); }

void page_map_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const L::PageEntry &e : image(c).page_map()) {
        o.begin_tuple();
        o.integer(e.number);
        o.integer(e.kind);
        o.end_tuple();
    }
    o.end_list();
}

void file_offset_q(Call &c) {
    const int64_t obj = c.int_arg(1);
    put_opt(c.out, obj < 0 ? std::nullopt : image(c).file_offset(uint32_t(obj), c.int_arg(2)));
}

void pairs(Out &o, const std::vector<L::NameOrdinal> &ns) {
    o.begin_list();
    for (const L::NameOrdinal &n : ns) {
        o.begin_tuple();
        o.text(n.name);
        o.integer(n.ordinal);
        o.end_tuple();
    }
    o.end_list();
}

void names_at_q(Call &c) {
    std::optional<uint64_t> end;
    if (c.argc() > 1 && !c.arg(1).is_none()) end = uint64_t(c.arg(1).as_int());
    pairs(c.out, L::names_at(view(c.data), uint64_t(c.int_arg(0)), end));
}

void names_q(Call &c) {
    const auto [res, non] = image(c).names();
    c.out.begin_tuple();
    pairs(c.out, res);
    pairs(c.out, non);
    c.out.end_tuple();
}

void entries_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const L::EntryPoint &e : image(c).entries()) {
        o.begin_tuple();
        o.integer(e.ordinal);
        o.integer(e.object);
        o.integer(e.offset);
        o.integer(e.flags);
        o.end_tuple();
    }
    o.end_list();
}

void entry_table_size_q(Call &c) { c.out.integer(int64_t(image(c).entry_table_size())); }

void exports_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const L::Export &e : image(c).exports()) {
        o.begin_tuple();
        o.text(e.name.value_or(""));
        o.integer(e.ordinal);
        o.integer(e.object);
        o.integer(e.offset);
        o.end_tuple();
    }
    o.end_list();
}

const char *record_kind(L::FixupRecord::Kind k) {
    switch (k) {
    case L::FixupRecord::Kind::Internal: return "internal";
    case L::FixupRecord::Kind::Import: return "import";
    case L::FixupRecord::Kind::ImportName: return "import-name";
    default: return "entry";
    }
}

void fixup_records_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const L::FixupRecord &r : image(c).fixup_records()) {
        o.begin_tuple();
        o.integer(r.page);
        o.begin_dict();
        o.item("source", r.source);
        o.item("source_flags", r.source_flags);
        o.item("flags", r.flags);
        o.item_text("kind", record_kind(r.kind));
        switch (r.kind) {
        case L::FixupRecord::Kind::Internal:
            o.item("obj", r.first);
            o.key("target");
            put_opt(o, r.target);
            break;
        case L::FixupRecord::Kind::Import:
            o.item("module", r.first);
            o.item("ordinal", r.ordinal);
            break;
        case L::FixupRecord::Kind::ImportName:
            o.item("module", r.first);
            o.item("name", r.name);
            break;
        case L::FixupRecord::Kind::Entry:
            o.item("entry", r.first);
            break;
        }
        if (r.addend) o.item("addend", *r.addend);
        o.key("sources");
        o.begin_list();
        for (int16_t s : r.sources) o.integer(s);
        o.end_list();
        o.key("raw");
        put(o, r.raw);
        o.end_dict();
        o.end_tuple();
    }
    o.end_list();
}

// a fix-up record as fixup_records gives it (or as a driver build made it)
L::FixupRecord record_of(const Value &rec) {
    L::FixupRecord r;
    const int64_t fl = key_int(rec, "flags");
    const Value *sf = maybe(rec, "source_flags");
    const int64_t src = key_int(rec, "source"), sfl = sf ? sf->as_int() : 0;
    if (src < 0 || src > 255 || sfl < 0 || sfl > 255 || fl < 0 || fl > 255) raise("ValueError", "byte must be in range(0, 256)");
    r.source = uint8_t(src);
    r.source_flags = uint8_t(sfl);
    r.flags = uint8_t(fl);
    for (const Value &s : need(rec, "sources").items) {
        const int64_t v = s.as_int();
        if (v < -0x8000 || v > 0x7FFF) raise("struct.error", "short format requires -32768 <= number <= 32767");
        r.sources.push_back(int16_t(v));
    }
    const std::string kind = need(rec, "kind").as_str();
    auto u32_of = [](const Value &v) {
        const int64_t x = v.as_int();
        if (x < 0) raise("OverflowError", "can't convert negative int to unsigned");
        if (x > 0xFFFFFFFFLL) raise("OverflowError", "int too big to convert");
        return uint32_t(x);
    };
    if (kind == "internal") {
        r.kind = L::FixupRecord::Kind::Internal;
        r.first = u32_of(need(rec, "obj"));
        if (r.source != 2) r.target = u32_of(need(rec, "target"));
    } else if (kind == "import") {
        r.kind = L::FixupRecord::Kind::Import;
        r.first = u32_of(need(rec, "module"));
        r.ordinal = u32_of(need(rec, "ordinal"));
    } else if (kind == "import-name") {
        r.kind = L::FixupRecord::Kind::ImportName;
        r.first = u32_of(need(rec, "module"));
        r.name = u32_of(need(rec, "name"));
    } else if (kind == "entry") {
        r.kind = L::FixupRecord::Kind::Entry;
        r.first = u32_of(need(rec, "entry"));
    } else {
        raise("KeyError", "'" + kind + "'");
    }
    if (fl & 0x04) r.addend = u32_of(need(rec, "addend"));
    return r;
}

void encode_fixup_q(Call &c) { put(c.out, L::encode_fixup(record_of(c.arg(0)))); }

void references_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const L::Reference &r : image(c).references()) {
        o.begin_tuple();
        o.integer(r.at.object);
        o.integer(r.at.offset);
        o.integer(r.source);
        o.integer(r.target_object);
        o.integer(r.target_offset);
        o.end_tuple();
    }
    o.end_list();
}

void pointers_q(Call &c) {
    Out &o = c.out;
    o.begin_dict();
    for (const auto &[from, to] : image(c).pointers()) {
        place(o, from);
        place(o, to);
    }
    o.end_dict();
}

void plausible_q(Call &c) {
    const int64_t at = c.int_arg(0);
    c.out.boolean(at >= 0 && L::plausible_descriptor(view(c.data), uint64_t(at)));
}

void descriptor_out(Out &o, const L::Descriptor &d) {
    o.begin_dict();
    o.item("obj", d.where.object);
    o.item("offset", d.where.offset);
    o.item_text("name", d.name);
    o.item("next", d.next);
    o.item("kit_version", d.kit_version);
    o.item("device_id", d.device_id);
    o.item("major", d.major);
    o.item("minor", d.minor);
    o.item("flags", d.flags);
    o.item("init_order", d.init_order);
    o.key("control");
    maybe_place(o, d.at_control);
    o.key("v86_api");
    maybe_place(o, d.at_v86_api);
    o.key("pm_api");
    maybe_place(o, d.at_pm_api);
    o.item("v86_api_address", d.v86_api_address);
    o.item("pm_api_address", d.pm_api_address);
    o.key("reference_data");
    maybe_place(o, d.at_reference_data);
    o.key("service_table");
    maybe_place(o, d.at_service_table);
    o.item("service_count", d.service_count);
    o.item("control_raw", d.control);
    o.item("v86_api_raw", d.v86_api);
    o.item("pm_api_raw", d.pm_api);
    o.item("service_table_raw", d.service_table);
    o.item("reference_data_raw", d.reference_data);
    o.item("size", int64_t(d.size));
    o.key("bytes");
    put(o, d.bytes);
    o.end_dict();
}

void descriptor_q(Call &c) {
    auto d = image(c).descriptor();
    if (d) descriptor_out(c.out, *d);
    else c.out.none();
}

// service_table(lf, d=None): of the descriptor the caller holds (its dict), else of the image's own
void service_table_q(Call &c) {
    L::Module lf = image(c);
    const Value &given = c.arg(1);
    std::vector<std::optional<L::Place>> table;
    if (given.truthy()) {
        if (given.kind != Value::Dict) raise("TypeError", "a descriptor that is not a dict");
        const Value &t = need(given, "service_table");
        if (t.truthy()) {
            const int64_t count = key_int(given, "service_count");
            L::Descriptor d;
            if (0 < count && count < 0x1000) {
                d.at_service_table = place_of(t);
                d.service_count = uint32_t(count);
            }
            table = lf.service_table(d);
        }
    } else {
        table = lf.service_table();
    }
    Out &o = c.out;
    o.begin_list();
    for (const auto &p : table) maybe_place(o, p);
    o.end_list();
}

void entry_points_q(Call &c) {
    L::Module lf = image(c);
    auto d = lf.descriptor();
    Out &o = c.out;
    o.begin_dict();
    if (d) {
        const std::pair<const char *, const std::optional<L::Place> *> named[] = {
            {"control", &d->at_control}, {"v86_api", &d->at_v86_api}, {"pm_api", &d->at_pm_api}};
        for (const auto &[name, p] : named)
            if (*p) {
                o.key(name);
                place(o, **p);
            }
        const auto st = lf.service_table(*d);
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
    for (const L::ServiceCall &s : image(c).service_calls()) {
        o.begin_tuple();
        o.integer(s.object);
        o.integer(int64_t(s.offset));
        o.integer(s.device);
        o.integer(s.service);
        o.boolean(s.jump);
        o.end_tuple();
    }
    o.end_list();
}

// ---- an image's parts: read_image's dict, and back -------------------------------------------------------------

void name_table_out(Out &o, const L::NameTable &t) {
    if (const cb::Bytes *stored = std::get_if<cb::Bytes>(&t)) put(o, *stored);
    else pairs(o, std::get<std::vector<L::NameOrdinal>>(t));
}

void read_image_q(Call &c) {
    const L::Parts p = image(c).parts();
    Out &o = c.out;
    o.begin_dict();
    o.key("stub");
    put(o, p.stub);
    o.key("header");
    put(o, p.header);
    o.key("objects");
    o.begin_list();
    for (const L::ObjectEntry &ob : p.objects) {
        o.begin_dict();
        o.item("vsize", ob.vsize);
        o.item("base", ob.base);
        o.item("flags", ob.flags);
        o.item("first_page", ob.first_page);
        o.item("page_count", ob.page_count);
        o.key("reserved");
        if (ob.reserved) o.bytes(Bytes(ob.reserved->data(), 4));
        else o.bytes(Bytes());
        o.end_dict();
    }
    o.end_list();
    o.key("pages");
    o.begin_list();
    for (const L::StoredPage &pg : p.pages) {
        o.begin_tuple();
        o.integer(pg.kind);
        put(o, pg.body);
        o.end_tuple();
    }
    o.end_list();
    o.key("resident");
    name_table_out(o, p.resident);
    o.key("nonresident");
    name_table_out(o, p.nonresident);
    o.key("entry_table");
    put(o, p.entry_table);
    o.key("fixups");
    o.begin_dict();
    for (const auto &[page, recs] : p.fixups) {
        o.integer(page);
        o.begin_list();
        for (const cb::Bytes &r : recs) put(o, r);
        o.end_list();
    }
    o.end_dict();
    o.key("import_modules");
    o.begin_list();
    for (const std::string &m : p.import_modules) o.text(m);
    o.end_list();
    o.key("import_names");
    put(o, p.import_names);
    o.key("align");
    put_opt(o, p.align);
    o.key("last_page_bytes");
    put_opt(o, p.last_page_bytes);
    o.key("gaps");
    o.begin_tuple();
    put(o, p.gaps ? p.gaps->first : cb::Bytes());
    put(o, p.gaps ? p.gaps->second : cb::Bytes());
    o.end_tuple();
    o.key("after");
    put(o, p.after);
    o.key("moved");
    o.begin_dict();
    o.item("debug_info", p.moved.debug_info);
    o.item("resource_data", p.moved.resource_data);
    o.item("from", p.moved.from);
    o.end_dict();
    o.end_dict();
}

uint32_t u32_field(const Value &v) {
    const int64_t x = v.as_int();
    if (x < 0 || x > 0xFFFFFFFFLL) raise("struct.error", "argument out of range");
    return uint32_t(x);
}

std::vector<L::ObjectEntry> objects_of(const Value &objects) {
    std::vector<L::ObjectEntry> out;
    for (const Value &o : objects.items) {
        L::ObjectEntry e;
        e.vsize = u32_field(need(o, "vsize"));
        e.base = u32_field(need(o, "base"));
        e.flags = u32_field(need(o, "flags"));
        e.first_page = u32_field(need(o, "first_page"));
        e.page_count = u32_field(need(o, "page_count"));
        if (const Value *r = o.get("reserved")) {
            const cb::Bytes b = blob(*r);
            if (b.size() != 4) raise("ValueError", "an object's reserved field is 4 bytes");
            e.reserved = std::array<uint8_t, 4>{b[0], b[1], b[2], b[3]};
        }
        out.push_back(e);
    }
    return out;
}

L::NameTable name_table_of(const Value &pairs_v) {
    if (pairs_v.kind == Value::Bytes) return blob(pairs_v);
    std::vector<L::NameOrdinal> out;
    for (const Value &p : pairs_v.items) {
        const int64_t ord = item(p, 1).as_int();
        if (ord < 0 || ord > 0xFFFF) raise("struct.error", "'H' format requires 0 <= number <= 65535");
        out.push_back({item(p, 0).as_str(), uint16_t(ord)});
    }
    return out;
}

L::Parts parts_of(const Value &image_v) {
    L::Parts p;
    p.stub = blob(need(image_v, "stub"));
    p.header = blob(need(image_v, "header"));
    p.objects = objects_of(need(image_v, "objects"));
    for (const Value &pg : need(image_v, "pages").items) {
        const int64_t kind = item(pg, 0).as_int();
        if (kind < 0 || kind > 255) raise("OverflowError", "int too big to convert");
        p.pages.push_back({uint8_t(kind), blob(item(pg, 1))});
    }
    p.resident = name_table_of(need(image_v, "resident"));
    const Value &non = need(image_v, "nonresident");
    p.nonresident = non.truthy() ? name_table_of(non) : L::NameTable(std::vector<L::NameOrdinal>{});
    p.entry_table = blob(need(image_v, "entry_table"));
    const Value &fx = need(image_v, "fixups");
    for (size_t k = 0; k + 1 < fx.items.size(); k += 2) {
        std::vector<cb::Bytes> recs;
        for (const Value &r : fx.items[k + 1].items) recs.push_back(blob(r));
        p.fixups.insert_or_assign(u32_field(fx.items[k]), std::move(recs));
    }
    for (const Value &m : need(image_v, "import_modules").items) p.import_modules.push_back(m.as_str());
    p.import_names = blob(need(image_v, "import_names"));
    if (const Value *v = maybe(image_v, "align")) p.align = u32_field(*v);
    if (const Value *v = maybe(image_v, "last_page_bytes")) p.last_page_bytes = u32_field(*v);
    if (const Value *g = maybe(image_v, "gaps"); g && g->truthy()) p.gaps.emplace(blob(item(*g, 0)), blob(item(*g, 1)));
    p.after = blob(need(image_v, "after"));
    const Value &moved = need(image_v, "moved");
    p.moved.debug_info = u32_field(need(moved, "debug_info"));
    p.moved.resource_data = u32_field(need(moved, "resource_data"));
    p.moved.from = u32_field(need(moved, "from"));
    if (const Value *v = maybe(image_v, "page_map")) p.page_map = blob(*v);
    if (const Value *v = maybe(image_v, "tables_tail"); v && v->truthy()) p.tables_tail = blob(*v);
    if (const Value *v = maybe(image_v, "data_pages"); v && v->truthy()) p.data_pages = u32_field(*v);
    if (const Value *v = maybe(image_v, "in_pages")) {
        cb::OrderedMap<std::string, int64_t> in;
        for (size_t k = 0; k + 1 < v->items.size(); k += 2) in.insert_or_assign(v->items[k].as_str(), v->items[k + 1].as_int());
        p.in_pages = std::move(in);
    }
    if (const Value *v = maybe(image_v, "fill_from"); v && v->truthy()) p.fill_from = blob(*v);
    return p;
}

void write_image_q(Call &c) { put(c.out, L::write_parts(parts_of(c.arg(0)))); }

void container_q(Call &c) {
    auto k = L::container(view(c.data));
    Out &o = c.out;
    if (!k) {
        o.none();
        return;
    }
    const bool w3 = k->kind == L::Container::Kind::W3;
    o.begin_dict();
    o.item_text("kind", w3 ? "W3" : "W4");
    o.item("at", int64_t(k->at));
    o.item("version", k->version);
    if (w3) {
        o.key("reserved");
        put(o, k->reserved);
    }
    o.key("members");
    o.begin_list();
    for (const L::ContainerMember &m : k->members) {
        o.begin_tuple();
        o.text(m.name);
        o.integer(m.offset);
        o.integer(m.size);
        o.end_tuple();
    }
    o.end_list();
    o.end_dict();
}

void object_table_q(Call &c) { put(c.out, L::object_table(objects_of(c.arg(0)))); }
void name_table_q(Call &c) { put(c.out, L::name_table(name_table_of(c.arg(0)))); }

void fixup_tables_q(Call &c) {
    const Value &fx = c.arg(0);
    if (fx.kind != Value::Dict) raise("TypeError", "fix-ups that are not a dict");
    std::map<uint32_t, std::vector<cb::Bytes>> m;
    for (size_t k = 0; k + 1 < fx.items.size(); k += 2) {
        std::vector<cb::Bytes> recs;
        for (const Value &r : fx.items[k + 1].items) recs.push_back(blob(r));
        m[u32_field(fx.items[k])] = std::move(recs);
    }
    const auto [table, records] = L::fixup_tables(m, u32_field(c.arg(1)));
    c.out.begin_tuple();
    put(c.out, table);
    put(c.out, records);
    c.out.end_tuple();
}

void pack_directory_q(Call &c) {
    const Value &v = c.arg(0);
    L::Container k;
    const int64_t version = key_int(v, "version");
    if (version < 0 || version > 0xFFFF) raise("struct.error", "'H' format requires 0 <= number <= 65535");
    k.version = uint16_t(version);
    k.reserved = blob(need(v, "reserved"));
    for (const Value &m : need(v, "members").items)
        k.members.push_back({item(m, 0).as_str(), u32_field(item(m, 1)), u32_field(item(m, 2))});
    put(c.out, L::pack_directory(k));
}

void expand_page_q(Call &c) { put(c.out, L::expand_page(view(c.data), size_t(c.int_arg(0, 0x2000)))); }
void compress_page_q(Call &c) { put(c.out, L::compress_page(view(c.data))); }
void expand_q(Call &c) { put(c.out, L::expand(view(c.data))); }
void compress_q(Call &c) { put(c.out, L::compress(view(c.data), view(c.arg(0).as_bytes()))); }

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
    add("le.w3_modules", w3_modules_q, "list[(str, int)]");
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
    add("le._name_table", name_table_q, "bytes");
    add("le.fixup_tables", fixup_tables_q, "(bytes, bytes)");
    add("le.expand_page", expand_page_q, "bytes");
    add("le.compress_page", compress_page_q, "bytes");
    add("le.expand", expand_q, "bytes");
    add("le.compress", compress_q, "bytes");
}

}  // namespace cbf::le
