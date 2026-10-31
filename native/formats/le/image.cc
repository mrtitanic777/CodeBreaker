// image.cc -- an LE image taken apart and written back (le.py's read_image / write_image and the
// small encoders they use: encode_fixup, object_table, fixup_tables, pack_directory, compress).
//
// read_image writes its answer straight out (it is one big dict); write_image reads the dict a
// caller hands over -- read_image's, or one a driver build changed -- with the Python's rules:
// a missing key is a KeyError, a value out of a field's range a struct.error.
#include <algorithm>
#include <map>

#include "../core/questions.h"
#include "../core/text.h"
#include "le.h"

namespace cbf::le {

namespace {
const Value &need(const Value &d, const char *key) {
    if (d.kind != Value::Dict) raise("TypeError", "a dict is required");
    const Value *v = d.get(key);
    if (!v) raise("KeyError", std::string("'") + key + "'");
    return *v;
}
const Value *maybe(const Value &d, const char *key) {   // d.get(key): None counts as absent
    const Value *v = d.get(key);
    return v && !v->is_none() ? v : nullptr;
}
std::string blob(const Value &v) {
    if (v.kind != Value::Bytes) raise("TypeError", "a bytes-like object is required");
    return v.s;
}
void put_le(std::string &out, uint64_t v, int width) {
    for (int k = 0; k < width; ++k) out += char(v >> (8 * k));
}
// struct.pack("<?", v) with its range check
void pack(std::string &out, int64_t v, int width, bool is_signed = false) {
    const int64_t lo = is_signed ? -(int64_t(1) << (8 * width - 1)) : 0;
    const int64_t hi = is_signed ? (int64_t(1) << (8 * width - 1)) - 1 : (width == 4 ? 0xFFFFFFFFLL : (int64_t(1) << (8 * width)) - 1);
    if (v < lo || v > hi) raise("struct.error", "argument out of range");
    put_le(out, uint64_t(v), width);
}
// int.to_bytes(width, "little" / "big") with its OverflowError
std::string to_bytes(int64_t v, int width, bool big = false) {
    if (v < 0) raise("OverflowError", "can't convert negative int to unsigned");
    if (width < 8 && v >= (int64_t(1) << (8 * width))) raise("OverflowError", "int too big to convert");
    std::string s;
    for (int k = 0; k < width; ++k) s += char(uint64_t(v) >> (8 * (big ? width - 1 - k : k)));
    return s;
}
void put_into(std::string &hdr, int64_t at, int64_t v) {   // struct.pack_into("<I", hdr, at, v)
    if (v < 0 || v > 0xFFFFFFFFLL) raise("struct.error", "'I' format requires 0 <= number <= 4294967295");
    for (int k = 0; k < 4; ++k) hdr[size_t(at + k)] = char(uint64_t(v) >> (8 * k));
}
std::string name_byte(size_t n) {                            // bytes([len(name)])
    if (n > 255) raise("ValueError", "bytes must be in range(0, 256)");
    return std::string(1, char(n));
}
}  // namespace

// encode_fixup(rec): the bytes of a fix-up record as fixup_records gives it
std::string encode_fixup(const Value &rec) {
    const int64_t fl = need(rec, "flags").as_int();
    const Value *sf = maybe(rec, "source_flags");
    const int64_t src = need(rec, "source").as_int() | (sf ? sf->as_int() : 0);
    const bool many = src & 0x20;
    if (src < 0 || src > 255 || fl < 0 || fl > 255) raise("ValueError", "byte must be in range(0, 256)");
    std::string out = to_bytes(src, 1) + to_bytes(fl, 1);   // bytearray([src, fl])
    const Value &sources = need(rec, "sources");
    if (many) out += name_byte(sources.items.size());
    else pack(out, py_at(sources.items, 0).as_int(), 2, true);
    const int wide = (fl & 0x40) ? 2 : 1;
    const std::string kind = need(rec, "kind").as_str();
    const char *first = kind == "internal" ? "obj" : kind == "import" ? "module" : kind == "import-name" ? "module"
                        : kind == "entry" ? "entry" : nullptr;
    if (!first) raise("KeyError", "'" + kind + "'");
    out += to_bytes(need(rec, first).as_int(), wide);
    if (kind == "internal" && need(rec, "source").as_int() != 2)
        out += to_bytes(need(rec, "target").as_int(), (fl & 0x10) ? 4 : 2);
    else if (kind == "import")
        out += to_bytes(need(rec, "ordinal").as_int(), (fl & 0x80) ? 1 : ((fl & 0x10) ? 4 : 2));
    else if (kind == "import-name")
        out += to_bytes(need(rec, "name").as_int(), (fl & 0x10) ? 4 : 2);
    if (fl & 0x04) out += to_bytes(need(rec, "addend").as_int(), (fl & 0x20) ? 4 : 2);
    if (many)
        for (const Value &s : sources.items) pack(out, s.as_int(), 2, true);
    return out;
}

// object_table(objects)
std::string object_table(const Value &objects) {
    std::string out;
    for (const Value &o : objects.items) {
        for (const char *k : {"vsize", "base", "flags", "first_page", "page_count"}) pack(out, need(o, k).as_int(), 4);
        const Value *r = o.get("reserved");
        out += r ? blob(*r) : std::string(4, '\0');
    }
    return out;
}

// fixup_tables(fixups, pages) -> (page table, records)
std::pair<std::string, std::string> fixup_tables(const std::map<int64_t, std::vector<std::string>> &fixups, int64_t pages) {
    std::string table, records;
    for (int64_t pg = 1; pg <= pages; ++pg) {
        pack(table, int64_t(records.size()), 4);
        auto it = fixups.find(pg);
        if (it != fixups.end())
            for (const std::string &r : it->second) records += r;
    }
    pack(table, int64_t(records.size()), 4);
    return {table, records};
}

// _name_table(pairs): bytes as stored, or built from [(name, ordinal)]
std::string name_table(const Value &pairs) {
    if (pairs.kind == Value::Bytes) return pairs.s;
    std::string out;
    for (const Value &p : pairs.items) {
        std::string n = to_latin1(py_at(p.items, 0).as_str());
        out += name_byte(n.size()) + n;
        pack(out, py_at(p.items, 1).as_int(), 2);
    }
    return out + '\0';
}

std::string pack_directory(const Value &c) {
    std::string out = "W3";
    pack(out, need(c, "version").as_int(), 2);
    const Value &members = need(c, "members");
    pack(out, int64_t(members.items.size()), 2);
    out += blob(need(c, "reserved"));
    for (const Value &m : members.items) {
        std::string n = to_latin1(py_at(m.items, 0).as_str());
        if (n.size() < 8) n.append(8 - n.size(), ' ');
        out += n;
        pack(out, py_at(m.items, 1).as_int(), 4);
        pack(out, py_at(m.items, 2).as_int(), 4);
    }
    return out;
}

std::string compress(Bytes data, Bytes like) {
    auto c = container(like);
    if (!c) raise("TypeError", "'NoneType' object is not subscriptable");
    const int64_t at = c->at;
    const uint16_t size = like.u16(at + 4);
    if (size == 0) raise("ValueError", "range() arg 3 must not be zero");
    Bytes body = data.slice(at);
    std::vector<std::string> pages;
    for (int64_t i = 0; i < body.size(); i += size) {
        std::string page = body.slice(i, i + size).str();
        if (int64_t(page.size()) < size) page.append(size_t(size - int64_t(page.size())), '\0');
        pages.push_back(compress_page(Bytes(page)));
    }
    std::string head = like.slice(0, at + 0x10).str();
    if (at + 6 + 2 > int64_t(head.size()) || pages.size() > 0xFFFF) raise("struct.error", "pack_into out of range");
    head[size_t(at + 6)] = char(pages.size());
    head[size_t(at + 7)] = char(pages.size() >> 8);
    int64_t pos = at + 0x10 + 4 * int64_t(pages.size());
    std::string table;
    for (const std::string &pg : pages) {
        pack(table, pos, 4);
        pos += int64_t(pg.size());
    }
    std::string out = head + table;
    for (const std::string &pg : pages) out += pg;
    return out;
}

// ---- read_image ------------------------------------------------------------------------------
static void names_out(Out &o, const std::vector<std::pair<std::string, uint16_t>> &ns) {
    o.begin_list();
    for (auto &n : ns) {
        o.begin_tuple();
        o.text(n.first);
        o.integer(n.second);
        o.end_tuple();
    }
    o.end_list();
}

void read_image(const Image &lf, Out &o) {
    const Bytes d = lf.d;
    auto h = header_fields(lf);
    auto H = [&h](const char *n) -> int64_t {
        for (auto &kv : h)
            if (std::string(kv.first) == n) return int64_t(kv.second);
        return 0;
    };
    const bool member = is_member(lf);
    auto pm = page_map(lf);
    int64_t last = 0;
    for (auto &e : pm) last = std::max(last, e.number);
    auto page = [&](int64_t number, uint8_t kind) -> Bytes {
        if (kind == 2 || kind == 3 || !number) return Bytes();
        int64_t off = int64_t(lf.datapages) + (number - 1) * int64_t(lf.pagesize);
        return d.slice(off, off + ((number == last && lf.last_page_bytes) ? lf.last_page_bytes : lf.pagesize));
    };
    std::vector<Bytes> bodies;
    for (auto &e : pm) bodies.push_back(page(e.number, e.kind));
    Bytes kept_last;
    bool kept = false;
    for (size_t k = 0; k < pm.size(); ++k)
        if (pm[k].kind != 2 && pm[k].kind != 3 && !bodies[k].empty()) {
            kept = true;
            kept_last = bodies[k];
        }
    const bool full_last = kept && kept_last.size() == lf.pagesize;
    std::map<int64_t, int64_t> page_of;                    // a page's number -> its first place in the map
    for (int64_t i = int64_t(pm.size()) - 1; i >= 0; --i)
        if (pm[size_t(i)].number) page_of[pm[size_t(i)].number] = i + 1;
    std::vector<int64_t> fix_order;                        // fixups: {page: [record bytes]}, in first-seen order
    std::map<int64_t, std::vector<Bytes>> fix;
    for (const FixupRecord &r : fixup_records(lf)) {
        auto it = page_of.find(r.page);
        int64_t key = it != page_of.end() ? it->second : r.page;
        if (!fix.count(key)) fix_order.push_back(key);
        fix[key].push_back(r.raw);
    }
    auto [res, non] = names(lf);
    const int64_t names_end = !non.empty() ? H("nonresident_names") + H("nonresident_size") : 0;
    const int64_t pages_end = int64_t(lf.datapages) +
        (last ? (last - 1) * int64_t(lf.pagesize) + (lf.last_page_bytes ? lf.last_page_bytes : lf.pagesize) : 0);
    int64_t align = 1;
    while (align < 0x1000 && int64_t(lf.datapages) % (align * 2) == 0) align *= 2;
    const int64_t ip = lf.lfa + H("import_names"), ie = lf.lfa + H("fixup_size") + H("fixup_pages");
    const int64_t ets = entry_table_size(lf);
    auto mods = lf.import_modules();

    o.begin_dict();
    o.key("stub");
    if (!member) o.bytes(d.slice(0, lf.lfa));
    else {
        std::string s = "MZ" + std::string(0x3A, '\0');
        put_le(s, 0x40, 4);
        o.bytes(Bytes(s));
    }
    o.key("header");
    o.bytes(d.slice(lf.lfa, lf.lfa + kHeaderSize));
    o.key("objects");
    o.begin_list();
    for (size_t i = 0; i < lf.objects.size(); ++i) {
        const Object &ob = lf.objects[i];
        o.begin_dict();
        o.item("vsize", ob.vsize);
        o.item("base", ob.base);
        o.item("flags", ob.flags);
        o.item("first_page", ob.pmidx);
        o.item("page_count", ob.pcnt);
        o.key("reserved");
        o.bytes(d.slice(lf.objtab + 24 * int64_t(i) + 20, lf.objtab + 24 * int64_t(i) + 24));
        o.end_dict();
    }
    o.end_list();
    o.key("pages");
    o.begin_list();
    for (size_t k = 0; k < pm.size(); ++k) {
        o.begin_tuple();
        o.integer(pm[k].kind);
        o.bytes(bodies[k]);
        o.end_tuple();
    }
    o.end_list();
    o.key("resident");
    names_out(o, res);
    o.key("nonresident");
    names_out(o, non);
    o.key("entry_table");
    o.bytes(d.slice(lf.entrytab, lf.entrytab + ets));
    o.key("fixups");
    o.begin_dict();
    for (int64_t k : fix_order) {
        o.integer(k);
        o.begin_list();
        for (Bytes r : fix[k]) o.bytes(r);
        o.end_list();
    }
    o.end_dict();
    o.key("import_modules");
    o.begin_list();
    for (auto &m : mods) o.text(m);
    o.end_list();
    o.key("import_names");
    o.bytes(H("import_names") && lf.lfa < ip && ip < ie && ie <= d.size() ? d.slice(ip, ie) : Bytes());
    o.item("align", align);
    o.key("last_page_bytes");
    if (!lf.last_page_bytes && full_last) o.integer(0);
    else o.none();
    o.key("gaps");
    o.begin_tuple();
    o.bytes(lf.lfa < ie && ie <= lf.datapages && !member ? d.slice(ie, lf.datapages) : Bytes());
    const int64_t nr = H("nonresident_names");
    o.bytes(!non.empty() && pages_end <= nr && nr < pages_end + 16 ? d.slice(pages_end, nr) : Bytes());
    o.end_tuple();
    o.key("after");
    o.bytes(member ? Bytes() : d.slice(std::max(pages_end, names_end)));
    o.key("moved");
    o.begin_dict();
    o.item("debug_info", H("debug_info"));
    o.item("resource_data", H("resource_data"));
    o.item("from", std::max(pages_end, names_end));
    o.end_dict();
    o.end_dict();
}

// ---- write_image -----------------------------------------------------------------------------
static const std::map<std::string, int> kFieldAt = {
    {"byte_order", 0x02}, {"word_order", 0x03}, {"level", 0x04}, {"cpu", 0x08}, {"os", 0x0A}, {"version", 0x0C},
    {"module_flags", 0x10}, {"pages", 0x14}, {"eip_object", 0x18}, {"eip", 0x1C}, {"esp_object", 0x20}, {"esp", 0x24},
    {"page_size", 0x28}, {"last_page_bytes", 0x2C}, {"fixup_size", 0x30}, {"loader_size", 0x38},
    {"object_table", 0x40}, {"objects", 0x44}, {"page_map", 0x48}, {"resources", 0x50}, {"resident_names", 0x58},
    {"entry_table", 0x5C}, {"fixup_pages", 0x68}, {"fixup_records", 0x6C}, {"import_modules", 0x70},
    {"import_module_count", 0x74}, {"import_names", 0x78}, {"data_pages", 0x80}, {"preload_pages", 0x84},
    {"nonresident_names", 0x88}, {"nonresident_size", 0x8C}, {"debug_info", 0x98}, {"debug_size", 0x9C},
    {"resource_data", 0xB8}, {"resource_size", 0xBC}, {"device_id", 0xC0}, {"kit_version", 0xC2}};

std::string write_image(const Value &image) {
    std::string hdr = blob(need(image, "header"));
    if (int64_t(hdr.size()) < kHeaderSize) hdr.append(size_t(kHeaderSize - int64_t(hdr.size())), '\0');
    const int64_t page_size = Bytes(hdr).u32(0x28);
    const std::string table = object_table(need(image, "objects"));
    const Value &kinds = need(image, "pages");
    const int64_t npages = int64_t(kinds.items.size());
    auto kind_of = [&](int64_t i) { return py_at(kinds.items[size_t(i)].items, 0).as_int(); };
    auto body_of = [&](int64_t i) { return blob(py_at(kinds.items[size_t(i)].items, 1)); };
    std::vector<int64_t> stored;
    for (int64_t i = 0; i < npages; ++i) {
        int64_t k = kind_of(i);
        if (k != 2 && k != 3 && !body_of(i).empty()) stored.push_back(i);
    }
    std::vector<std::pair<int64_t, int64_t>> number;       // {place in the map: page number}, in stored order
    std::string pmap;
    if (const Value *pm = maybe(image, "page_map")) {
        Bytes p = pm->as_bytes();
        for (int64_t i : stored) number.emplace_back(i, int64_t(p.slice(4 * i, 4 * i + 3).from_be()));
        pmap = pm->s;
    } else {
        std::map<int64_t, int64_t> num;
        for (size_t n = 0; n < stored.size(); ++n) {
            number.emplace_back(stored[n], int64_t(n) + 1);
            num[stored[n]] = int64_t(n) + 1;
        }
        for (int64_t i = 0; i < npages; ++i) {
            auto it = num.find(i);
            pmap += to_bytes(it != num.end() ? it->second : 0, 3, true);
            pmap += to_bytes(kind_of(i), 1);
        }
    }
    std::map<int64_t, int64_t> at;                          // {page number: place in the map}, last place wins
    for (auto &kv : number) at[kv.second] = kv.first;
    const int64_t top = at.empty() ? 0 : at.rbegin()->first;
    if (top > 0 && page_size * top > (int64_t(1) << 31)) fallback("an image past memory");
    std::string data;
    for (int64_t n = 1; n <= top; ++n) {
        auto it = at.find(n);
        if (it == at.end()) {
            data.append(size_t(std::max<int64_t>(page_size, 0)), '\0');
            continue;
        }
        std::string b = body_of(it->second);
        if (n != top && int64_t(b.size()) < page_size) b.append(size_t(page_size - int64_t(b.size())), '\0');
        data += b;
    }
    const int64_t last_len = top ? int64_t(body_of(at[top]).size()) : 0;
    const std::string resident = name_table(need(image, "resident"));
    std::map<int64_t, std::vector<std::string>> fix;
    const Value &fixups = need(image, "fixups");
    std::map<int64_t, int64_t> num_of;
    for (auto &kv : number) num_of[kv.first] = kv.second;
    for (size_t k = 0; k + 1 < fixups.items.size(); k += 2) {
        int64_t pg = fixups.items[k].as_int();
        auto it = num_of.find(pg - 1);
        std::vector<std::string> recs;
        for (const Value &r : fixups.items[k + 1].items) recs.push_back(blob(r));
        fix[it != num_of.end() ? it->second : pg] = recs;   // (a later page with the same number wins)
    }
    auto [fix_pages, fix_records] = fixup_tables(fix, npages);
    std::string modules;
    const Value &mods = need(image, "import_modules");
    for (const Value &m : mods.items) {
        std::string n = to_latin1(m.as_str());
        modules += name_byte(n.size()) + n;
    }
    const Value *tail_v = maybe(image, "tables_tail");
    const std::string tail = tail_v && tail_v->truthy() ? blob(*tail_v) : "";
    const std::string entry_table = blob(need(image, "entry_table"));
    const std::string import_names = blob(need(image, "import_names"));
    std::map<std::string, int64_t> where;
    int64_t pos = kHeaderSize;
    const std::pair<const char *, const std::string *> parts[] = {
        {"object_table", &table}, {"page_map", &pmap}, {"resident_names", &resident}, {"entry_table", &entry_table},
        {"fixup_pages", &fix_pages}, {"fixup_records", &fix_records}, {"import_modules", &modules},
        {"import_names", &import_names}, {"tables_tail", &tail}};
    for (auto &p : parts) {
        where[p.first] = pos;
        pos += int64_t(p.second->size());
    }
    const std::string stub = blob(need(image, "stub"));
    const int64_t base = int64_t(stub.size());
    const Value *dp = maybe(image, "data_pages");
    int64_t pages_at = dp && dp->truthy() ? dp->as_int() : 0;
    if (pages_at < base + pos) {
        const int64_t al = need(image, "align").as_int();
        if (al == 0) raise("ZeroDivisionError", "integer division or modulo by zero");
        pages_at = ((base + pos + al - 1) / al) * al;
    }
    const Value *lastv = maybe(image, "last_page_bytes");
    put_into(hdr, 0x14, npages);
    put_into(hdr, 0x2C, lastv ? lastv->as_int() : last_len);
    put_into(hdr, 0x30, int64_t(fix_pages.size() + fix_records.size() + modules.size() + import_names.size()));
    put_into(hdr, 0x38, where["fixup_pages"] - kHeaderSize);
    put_into(hdr, 0x40, where["object_table"]);
    put_into(hdr, 0x44, int64_t(need(image, "objects").items.size()));
    put_into(hdr, 0x48, where["page_map"]);
    put_into(hdr, 0x58, where["resident_names"]);
    put_into(hdr, 0x5C, where["entry_table"]);
    put_into(hdr, 0x68, where["fixup_pages"]);
    put_into(hdr, 0x6C, where["fixup_records"]);
    put_into(hdr, 0x70, where["import_modules"]);
    put_into(hdr, 0x74, int64_t(mods.items.size()));
    put_into(hdr, 0x78, where["import_names"]);
    put_into(hdr, 0x80, pages_at);
    std::string out = stub + hdr + table + pmap + resident + entry_table + fix_pages + fix_records + modules + import_names + tail;
    std::string gap, names_gap;
    if (const Value *g = maybe(image, "gaps"); g && g->truthy()) {
        gap = blob(py_at(g->items, 0));
        names_gap = blob(py_at(g->items, 1));
    }
    if (int64_t(gap.size()) == pages_at - int64_t(out.size())) out += gap;
    else if (pages_at < int64_t(out.size())) raise("ValueError", "negative count");
    else out.append(size_t(pages_at - int64_t(out.size())), '\0');
    out += data;
    const Value &nonres = need(image, "nonresident");
    const std::string non = nonres.truthy() ? name_table(nonres) : "";
    out += names_gap;
    const Value *in_pages = maybe(image, "in_pages");
    put_into(hdr, 0x8C, int64_t(non.size()));
    if (!(in_pages && in_pages->get("nonresident_names"))) {   // else already among the data pages
        put_into(hdr, 0x88, non.empty() ? 0 : int64_t(out.size()));
        out += non;
    }
    const Value &moved = need(image, "moved");
    const std::string after = blob(need(image, "after"));
    const int64_t shift = int64_t(out.size()) - need(moved, "from").as_int();
    for (auto &f : {std::make_pair(0x98, "debug_info"), std::make_pair(0xB8, "resource_data")}) {
        int64_t v = need(moved, f.second).as_int();
        put_into(hdr, f.first, v && !after.empty() ? v + shift : v);
    }
    if (in_pages)
        for (size_t k = 0; k + 1 < in_pages->items.size(); k += 2) {
            const std::string &key = in_pages->items[k].as_str();
            auto it = kFieldAt.find(key);
            if (it == kFieldAt.end()) raise("KeyError", "'" + key + "'");
            put_into(hdr, it->second, in_pages->items[k + 1].as_int() + pages_at - need(image, "data_pages").as_int());
        }
    out += after;
    if (const Value *fill = maybe(image, "fill_from"); fill && fill->truthy()) {
        Bytes f = fill->as_bytes();
        Bytes rest = f.slice(int64_t(out.size()));
        out.append(reinterpret_cast<const char *>(rest.data()), size_t(rest.size()));
    }
    if (base + kHeaderSize <= int64_t(out.size())) out.replace(size_t(base), size_t(kHeaderSize), hdr);
    else fallback("a header past the image");
    return out;
}

}  // namespace cbf::le
