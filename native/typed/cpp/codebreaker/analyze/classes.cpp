// codebreaker/analyze/classes.cpp -- typed C++ from codebreaker.analyze.classes (see the header).
//
// C++ in a module: virtual tables, classes and their hierarchy, MFC's class records and message maps, and
// exception-handling tables. call_graph / strings are reached through analyze/image_bridge.hpp.
#include "codebreaker/analyze/classes.hpp"

#include <algorithm>
#include <map>
#include <set>

#include "codebreaker/analyze/conventions.hpp"
#include "codebreaker/analyze/dispatch.hpp"
#include "codebreaker/analyze/image_bridge.hpp"
#include "codebreaker/analyze/structures.hpp"
#include "py_os.hpp"

namespace codebreaker::analyze::classes {

using image::Function;
using image::Image;
using image::Op;

namespace {

std::string hexs(int64_t v) {
    char b[32];
    std::snprintf(b, sizeof b, "%llx", static_cast<unsigned long long>(v < 0 ? -v : v));
    return (v < 0 ? "-" : "") + std::string(b);
}

bool is_data_name(const std::optional<py::str>& n, bool four) {
    if (!n) return false;
    return *n == ".rsrc" || *n == ".reloc" || (!four ? false : (*n == ".edata" || *n == ".idata"));
}

int64_t le32(const std::string& d, size_t at) {
    return static_cast<int64_t>(static_cast<unsigned char>(d[at])) | (static_cast<int64_t>(static_cast<unsigned char>(d[at + 1])) << 8) |
           (static_cast<int64_t>(static_cast<unsigned char>(d[at + 2])) << 16) | (static_cast<int64_t>(static_cast<unsigned char>(d[at + 3])) << 24);
}

bool flow_is(const image::Ins& i, const char* f) { return i.flow && *i.flow == f; }

// the real instruction of a reference (the call graph keeps only its address and size)
const image::Ins& real_ins(Image& img, const call_graph::Reference& r) {
    image::FunctionP fn = img.function(r.function);
    return *fn->insns[fn->index.at(r.instruction->addr)];
}

bool is_none_or_zero_u32(const std::optional<int64_t>& v) { return !v || *v == 0; }

}  // namespace

// ---- virtual tables -------------------------------------------------------------------------------------------------
std::shared_ptr<py::list<Vtable>> _vtables(Image& img) {
    auto out = std::make_shared<py::list<Vtable>>();
    if (img.bits != 32) return out;
    auto graph = bridge::graph(img);
    auto facts = conventions::functions(img);
    std::set<int64_t> funcs(img.functions.vec().begin(), img.functions.vec().end());
    auto in_funcs = [&](const std::optional<int64_t>& v) { return v && funcs.count(*v); };
    std::set<int64_t> named;                                   // addresses the code names
    for (const auto& r : graph->refs.vec()) if (r.how == "a") named.insert(r.address);
    std::map<int64_t, std::vector<std::tuple<int64_t, int64_t, int64_t>>> stores;   // table -> who installs it
    std::map<int64_t, std::vector<int64_t>> static_;
    for (const auto& r : graph->refs.vec()) {
        const image::Ins& ins = real_ins(img, r);
        int64_t va = r.address;
        if (r.how != "a" || ins.mn != "mov" || ins.ops.size() != 2 || ins.ops[1].kind != "i" || ins.ops[1].value != va || !in_funcs(img.u32(va))) continue;
        if (ins.ops[0].kind == "m" && ins.ops[0].base) {            // mov [reg+N], offset table
            stores[va].emplace_back(r.function, ins.addr, ins.ops[0].disp);
        } else if (ins.ops[0].kind == "r") {                        // mov reg, offset table / mov [reg2+N], reg
            image::FunctionP fn = img.function(r.function);
            const py::str& reg = ins.ops[0].name;
            int64_t at = *fn->at(ins.addr);
            for (int64_t q = at + 1; q < std::min<int64_t>(at + 4, fn->insns.size()); q++) {
                const image::Ins& b = *fn->insns[q];
                if (b.mn == "mov" && b.ops.size() == 2 && b.ops[0].kind == "m" && b.ops[0].base && b.ops[1].kind == "r" && b.ops[1].name == reg) {
                    stores[va].emplace_back(r.function, b.addr, b.ops[0].disp);
                    break;
                }
                if (b.wr.contains(image::family(reg)) || b.flow) break;
            }
        }
    }
    if (img.pointers && !img.pointers->empty()) {                   // an object in the data that starts with one
        for (const auto& kv : img.pointers->items()) {
            int64_t site = std::get<0>(kv);
            const auto& t = std::get<1>(std::get<1>(kv));
            if (t && !img.is_code(site) && in_funcs(img.u32(*t)) && img.pointers->contains(*t) && in_funcs(img.u32(*t + 4)) &&
                in_funcs(img.u32(*t + 8)) && site != *t - 4 && site != *t + 4) {
                static_[*t].push_back(site);
            }
        }
    }
    std::set<int64_t> all;
    for (auto& kv : stores) all.insert(kv.first);
    for (auto& kv : static_) all.insert(kv.first);
    for (int64_t va : all) {
        std::vector<int64_t> slots;
        int64_t at = va;
        while (true) {
            auto t = img.u32(at);
            if (!in_funcs(t) || (!slots.empty() && (named.count(at) || static_.count(at))) || (img.pointers && !img.pointers->contains(at))) break;
            slots.push_back(*t);
            at += 4;
        }
        if (slots.empty()) continue;
        if (!stores.count(va)) {
            bool any = false;
            for (int64_t a = va + 4; a < at; a += 4) if (static_.count(a) || stores.count(a)) any = true;
            if (slots.size() < 3 || any) continue;
        }
        std::vector<std::optional<int64_t>> removes;
        for (size_t k = 0; k < slots.size() && k < 7; k++) removes.push_back(facts->at(slots[k]).removes);
        auto eq = [&](size_t from, std::initializer_list<int64_t> want) {
            std::vector<std::optional<int64_t>> got(removes.begin() + std::min(from, removes.size()), removes.begin() + std::min(from + want.size(), removes.size()));
            if (got.size() != want.size()) return false;
            size_t i = 0;
            for (int64_t w : want) { if (!got[i] || *got[i] != w) return false; i++; }
            return true;
        };
        bool com = eq(0, {12, 4, 4});
        bool dual = com && eq(3, {8, 16, 24, 36});
        Vtable v;
        v.at = va;
        for (int64_t s : slots) v.slots.append(s);
        std::set<int64_t> inst, offs;
        if (stores.count(va)) for (const auto& s : stores.at(va)) { inst.insert(std::get<0>(s)); offs.insert(std::get<2>(s)); }
        for (int64_t f : inst) v.installed_by.append(f);
        for (int64_t o : offs) v.at_offsets.append(o);
        std::vector<int64_t> so = static_.count(va) ? static_.at(va) : std::vector<int64_t>();
        std::sort(so.begin(), so.end());
        for (int64_t s : so) v.static_objects.append(s);
        v.com = com;
        v.dispatch = dual;
        out->append(v);
    }
    return out;
}

std::shared_ptr<py::list<Vtable>> vtables(Image& img) {
    return img.memo<py::list<Vtable>>(py::str("classes.vtables"), [](Image& i) { return _vtables(i); });
}

// ---- run-time type information --------------------------------------------------------------------------------------
std::optional<py::str> _decorated(const py::str& name) {
    if (!name.startswith(py::str(".?A")) || name.size() < 6) return std::nullopt;
    py::str rest = name.slice_from(4);
    std::vector<py::str> parts;
    {
        const std::u32string& u = rest.u32();
        size_t from = 0;
        for (;;) {
            size_t p = u.find(U'@', from);
            std::u32string piece = u.substr(from, p == std::u32string::npos ? std::u32string::npos : p - from);
            if (!piece.empty()) parts.emplace_back(piece);
            if (p == std::u32string::npos) break;
            from = p + 1;
        }
    }
    bool all = !parts.empty();
    for (const py::str& p : parts) {
        if (!py::re::match(py::str("[\\w?$<>,]+$"), p)) { all = false; break; }
    }
    if (!all) return rest;
    std::vector<py::str> rev(parts.rbegin(), parts.rend());
    py::str out;
    for (size_t i = 0; i < rev.size(); i++) out = out + (i ? py::str("::") : py::str()) + rev[i];
    return out;
}

std::optional<py::str> _type_name(Image& img, int64_t descriptor) {
    if (!descriptor || img.span(descriptor) == nullptr) return std::nullopt;
    auto s = bridge::string_at(img, descriptor + 8, 4);
    if (s && std::get<0>(*s).startswith(py::str(".?A"))) return _decorated(std::get<0>(*s));
    return std::nullopt;
}

std::optional<std::tuple<py::str, py::list<py::str>>> _rtti(Image& img, int64_t table) {
    auto locator = img.u32(table - 4);
    if (!locator || *locator == 0 || img.span(*locator) == nullptr || (img.is_code(*locator) && img.owner(*locator))) return std::nullopt;
    auto signature = img.u32(*locator), descriptor = img.u32(*locator + 12), hierarchy = img.u32(*locator + 16);
    if (!signature || *signature != 0 || !descriptor) return std::nullopt;
    auto name = _type_name(img, *descriptor);
    if (!name || name->size() == 0) return std::nullopt;
    py::list<py::str> bases;
    int64_t count = 0, array = 0;
    if (hierarchy && *hierarchy && img.span(*hierarchy)) {
        count = img.u32(*hierarchy + 8).value_or(0);
        array = img.u32(*hierarchy + 12).value_or(0);
    }
    for (int64_t k = 1; k < std::min<int64_t>(count, 64); k++) {   // entry 0 is the class itself
        std::optional<py::str> base;
        if (array) base = _type_name(img, img.u32(img.u32(array + 4 * k).value_or(0)).value_or(0));
        if (base && base->size() != 0) bases.append(*base);
    }
    return std::make_tuple(*name, bases);
}

// ---- MFC ------------------------------------------------------------------------------------------------------------
std::optional<int64_t> _returned(const Image& img, int64_t va) {
    py::bytes b = img.read(va, 6);
    if (b.size() == 6 && (b[0] == 0xB8 || b[0] == 0xA1) && b[5] == 0xC3) {
        int64_t x = le32(b.raw(), 1);
        if ((b[0] == 0xB8 && img.span(x)) || (b[0] == 0xA1 && img.imports.contains(x))) return x;
    }
    return std::nullopt;
}

std::shared_ptr<py::dict<int64_t, int64_t>> _returns_address(Image& img) {
    return img.memo<py::dict<int64_t, int64_t>>(py::str("classes.getters"), [](Image& i) {
        auto d = std::make_shared<py::dict<int64_t, int64_t>>();
        for (int64_t f : i.functions.vec()) {
            auto v = _returned(i, f);
            if (v) d->set(f, *v);
        }
        return d;
    });
}

py::str _class_of_import(const py::str& name) {
    auto m = py::re::search(py::str("\\?class(\\w+?)@"), name);
    if (m) return m->group(1);
    const std::u32string& u = name.u32();
    size_t p = u.rfind(U'!');
    return py::str(p == std::u32string::npos ? u : u.substr(p + 1));
}

namespace {

// (address, value) for every aligned word of the module's data that holds an address
std::vector<std::pair<int64_t, int64_t>> aligned_pointers(const Image& img) {
    std::vector<std::pair<int64_t, int64_t>> out;
    const auto& sp = img.spans.vec();
    int64_t lo = std::get<0>(sp.front()), hi = std::get<0>(sp.back()) + std::get<1>(sp.back()).size();
    for (const native::Span& s : sp) {
        if (std::get<2>(s) || is_data_name(std::get<4>(s), true)) continue;
        const std::string& d = std::get<1>(s).raw();
        for (int64_t off = 0; off < std::get<5>(s) - 3; off += 4) {
            int64_t v = le32(d, static_cast<size_t>(off));
            if (lo <= v && v < hi) out.emplace_back(std::get<0>(s) + off, v);
        }
    }
    return out;
}

}  // namespace

std::shared_ptr<py::dict<int64_t, RuntimeClass>> _runtime_classes(Image& img) {
    auto out = std::make_shared<py::dict<int64_t, RuntimeClass>>();
    if (img.bits != 32) return out;
    struct Found { RuntimeClass c; int64_t base; };
    std::vector<int64_t> order;
    std::map<int64_t, Found> found;
    for (const auto& av : aligned_pointers(img)) {
        int64_t at = av.first, v = av.second;
        auto s = bridge::string_at(img, v, 2);
        if (!s || !(std::get<1>(*s) == "A") || !py::re::match(py::str("[A-Za-z_][A-Za-z0-9_]{1,63}$"), std::get<0>(*s))) continue;
        auto size = img.u32(at + 4), schema = img.u32(at + 8), create = img.u32(at + 12), base = img.u32(at + 16);
        if (!size || !schema || !create || !base || !(4 <= *size && *size <= 0x10000) || !(*schema <= 0x1000 || *schema == 0xFFFF)) continue;
        if (*create && !img.is_code(*create)) continue;
        Found f;
        f.c.at = at;
        f.c.name = std::get<0>(*s);
        f.c.size = *size;
        if (*schema != 0xFFFF) f.c.schema = *schema;
        f.c.creatable = *create != 0;
        f.base = *base;
        if (!found.count(at)) order.push_back(at);
        found[at] = f;
    }
    std::map<int64_t, bool> linked;
    for (int64_t at : order) {
        Found& f = found[at];
        int64_t b = f.base;
        if (b && img.is_code(b)) {   // the DLL build: a function returns the base's record
            auto r = _returned(img, b);
            b = (r && *r) ? *r : b;
        }
        if (found.count(b)) f.c.base = found[b].c.name;
        else if (img.imports.contains(b)) f.c.base = _class_of_import(img.imports.at(b));
        else f.c.base = std::nullopt;
        linked[at] = b == 0 || f.c.base.has_value();
    }
    std::set<std::string> bases;
    for (int64_t at : order) if (found[at].c.base && found[at].c.base->size() != 0) bases.insert(found[at].c.base->utf8());
    for (int64_t at : order) {
        const RuntimeClass& c = found[at].c;
        if (linked[at] && ((c.base && c.base->size() != 0) || bases.count(c.name.utf8()))) out->set(at, c);
    }
    return out;
}

std::shared_ptr<py::dict<int64_t, RuntimeClass>> runtime_classes(Image& img) {
    return img.memo<py::dict<int64_t, RuntimeClass>>(py::str("classes.runtime"), [](Image& i) { return _runtime_classes(i); });
}

py::str _ids(int64_t first, int64_t last) {
    return first == last ? py::str(std::to_string(first)) : py::str(std::to_string(first) + ", " + std::to_string(last));
}

py::str _macro(const Image& img, const MapEntry& e) {
    auto [msg, code, ident, last, sig, fn] = e;
    std::string handler = img.label(fn).utf8();
    std::string range = last != ident ? "_RANGE" : "";
    std::string ids = _ids(ident, last).utf8();
    if (msg == 0x111) {
        if (code == 0xFFFFFFFFll || code == 0xFFFF) return py::str("ON_UPDATE_COMMAND_UI" + range + "(" + ids + ", " + handler + ")");
        if (code == 0) return py::str("ON_COMMAND" + range + "(" + ids + ", " + handler + ")");
        return py::str("ON_CONTROL" + range + "(0x" + hexs(code) + ", " + ids + ", " + handler + ")");
    }
    if (msg == 0x4E) {
        int64_t c = (code >> 31) ? code - (int64_t(1) << 32) : code;
        return py::str("ON_NOTIFY" + range + "(" + std::to_string(c) + ", " + ids + ", " + handler + ")");
    }
    if (msg == 0xC000) return py::str("ON_REGISTERED_MESSAGE(*(UINT *)0x" + hexs(sig > 0x10000 ? sig : ident) + ", " + handler + ")");
    auto name = dispatch::MESSAGES.get(msg);
    if (name) {
        if (!(sig == 10 || sig == 11 || sig == 12)) return py::str("ON_" + name->utf8() + "() -> " + handler);
        return py::str("ON_MESSAGE(" + name->utf8() + ", " + handler + ")");
    }
    std::string h = hexs(msg);
    if (h.size() < 4) h = std::string(4 - h.size(), '0') + h;
    return py::str("ON_MESSAGE(0x" + h + ", " + handler + ")");
}

std::shared_ptr<py::list<MessageMap>> _message_maps(Image& img) {
    auto out = std::make_shared<py::list<MessageMap>>();
    if (img.bits != 32) return out;
    std::set<int64_t> funcs(img.functions.vec().begin(), img.functions.vec().end());
    auto getters = _returns_address(img);
    std::map<int64_t, std::vector<int64_t>> holders;        // address a word of data holds -> where
    for (const auto& av : aligned_pointers(img)) holders[av.second].push_back(av.first);
    std::map<int64_t, std::vector<int64_t>> by_table;
    for (const Vtable& vt : vtables(img)->vec()) {
        for (int64_t f : vt.slots.vec()) if (getters->contains(f)) by_table[getters->at(f)].push_back(vt.at);
    }
    int64_t skip = -1;
    for (const native::Span& s : img.spans.vec()) {
        if (std::get<2>(s) || is_data_name(std::get<4>(s), true)) continue;
        int64_t base = std::get<0>(s), held = std::get<5>(s);
        const std::string& data = std::get<1>(s).raw();
        int64_t off = 0;
        while (off + 48 <= held) {
            if (base + off < skip || !funcs.count(le32(data, static_cast<size_t>(off + 20)))) {
                off += 4;
                continue;
            }
            std::vector<MapEntry> entries;
            int64_t p = off;
            bool broke = false;
            while (p + 24 <= held) {
                int64_t e[6];
                for (int k = 0; k < 6; k++) e[k] = le32(data, static_cast<size_t>(p + 4 * k));
                if (e[0] == 0 && e[5] == 0 && e[4] == 0) { broke = true; break; }
                if (!funcs.count(e[5]) || e[0] > 0xFFFF || !(e[4] <= 0x80 || img.span(e[4])) || e[2] > 0xFFFF || e[3] > 0xFFFF ||
                    !(e[1] <= 0xFFFF || e[1] >= 0xFFFFF000ll)) {
                    entries.clear();
                    broke = true;
                    break;
                }
                entries.emplace_back(e[0], e[1], e[2], e[3], e[4], e[5]);
                p += 24;
            }
            if (!broke) entries.clear();
            int64_t at = base + off;
            std::vector<int64_t> owners;
            auto hit = holders.find(at);
            if (hit != holders.end()) for (int64_t h : hit->second) if (h - 4 >= base) owners.push_back(h - 4);
            if (!entries.empty() && (!owners.empty() || entries.size() >= 3)) {
                MessageMap rec;
                rec.at = at;
                for (const auto& e : entries) rec.entries.append(e);
                if (!owners.empty()) {
                    rec.map = owners[0];
                    std::vector<int64_t> ct = by_table.count(owners[0]) ? by_table[owners[0]] : std::vector<int64_t>();
                    std::sort(ct.begin(), ct.end());
                    for (int64_t t : ct) rec.class_tables.append(t);
                    auto b = img.u32(owners[0]);
                    std::optional<int64_t> bm = b;
                    if (b && getters->contains(*b)) bm = getters->at(*b);
                    if (bm && *bm != 0) rec.base_map = bm;
                }
                out->append(rec);
                skip = base + (p - off) + off + 24;
                off = p + 24 - ((p + 24) % 4);
                continue;
            }
            off += 4;
        }
    }
    return out;
}

std::shared_ptr<py::list<MessageMap>> message_maps(Image& img) {
    return img.memo<py::list<MessageMap>>(py::str("classes.message_maps"), [](Image& i) { return _message_maps(i); });
}

// ---- the hierarchy --------------------------------------------------------------------------------------------------
std::shared_ptr<py::list<Class>> _hierarchy(Image& img) {
    auto result = std::make_shared<py::list<Class>>();
    auto tables = vtables(img);
    if (tables->empty()) return result;
    auto runtime = runtime_classes(img);
    auto getters = _returns_address(img);
    std::map<int64_t, const Vtable*> by_at;
    for (const Vtable& t : tables->vec()) by_at[t.at] = &t;
    std::map<int64_t, structures::Structure> layouts;
    for (const structures::Structure& s : structures::recover(img)->vec()) {
        auto f0 = s.fields.get(0);
        if (!f0) continue;
        for (int64_t v : f0->stores) if (by_at.count(v) && !layouts.count(v)) layouts[v] = s;
    }
    std::map<int64_t, MessageMap> maps;
    for (const MessageMap& m : message_maps(img)->vec()) for (int64_t t : m.class_tables.vec()) maps[t] = m;
    struct Work { Class c; std::optional<py::str> name; };
    std::vector<int64_t> order;
    std::map<int64_t, Work> out;
    for (const Vtable& t : tables->vec()) {
        Work w;
        Class& c = w.c;
        c.vtable = t.at;
        c.slots = t.slots.size();
        c.methods = t.slots;
        c.installed_by = t.installed_by;
        c.com = t.com;
        auto info = _rtti(img, t.at);
        int64_t s0 = t.slots[0];
        if (info) {
            w.name = std::get<0>(*info);
            c.bases = std::get<1>(*info);
            c.evidence.append(py::str("run-time type information"));
        } else if (getters->contains(s0) && runtime->contains(getters->at(s0))) {
            RuntimeClass rc = runtime->at(getters->at(s0));
            w.name = rc.name;
            c.size = rc.size;
            py::list<py::str> chain;
            std::set<std::string> seen = {rc.name.utf8()};
            std::map<std::string, RuntimeClass> by_name;
            for (const RuntimeClass& r : runtime->values()) by_name[r.name.utf8()] = r;
            std::optional<py::str> b = rc.base;
            while (b && b->size() != 0 && !seen.count(b->utf8())) {
                chain.append(*b);
                seen.insert(b->utf8());
                auto bn = by_name.find(b->utf8());
                b = bn != by_name.end() ? bn->second.base : std::nullopt;
            }
            c.bases = chain;
            c.evidence.append(py::str("MFC class record at " + hexs(rc.at)));
        }
        auto lit = layouts.find(t.at);
        if (lit != layouts.end()) {
            c.layout = lit->second.name;
            c.fields = lit->second.fields.size();
            if (!c.size) {
                c.size = lit->second.size;
                c.size_exact = lit->second.exact;
            }
        }
        auto mit = maps.find(t.at);
        if (mit != maps.end()) c.message_map = mit->second.at;
        if (!out.count(t.at)) order.push_back(t.at);
        out[t.at] = w;
    }
    // inheritance the binary does not state: shared leading slots, and the order tables are installed
    std::map<int64_t, std::vector<int64_t>> installs;
    for (const Vtable& t : tables->vec()) for (int64_t f : t.installed_by.vec()) installs[f].push_back(t.at);
    auto name_or = [&](int64_t at) { return out[at].name && out[at].name->size() != 0 ? *out[at].name : py::str("class_" + hexs(at)); };
    for (int64_t ti = 0; ti < tables->size(); ti++) {
        const Vtable& t = (*tables)[ti];
        Work& w = out[t.at];
        if (!w.c.bases.empty()) continue;
        std::optional<std::pair<std::pair<int64_t, int64_t>, int64_t>> best;
        for (int64_t ui = 0; ui < tables->size(); ui++) {
            const Vtable& u = (*tables)[ui];
            if (ui == ti || u.slots.size() >= t.slots.size() || u.slots.size() < 2) continue;
            int64_t same = 0;
            for (int64_t k = 0; k < std::min(u.slots.size(), t.slots.size()); k++) if (u.slots[k] == t.slots[k]) same++;
            if (same >= std::max<int64_t>(2, u.slots.size() / 2) && (!best || std::make_pair(same, u.slots.size()) > best->first)) {
                best = std::make_pair(std::make_pair(same, u.slots.size()), u.at);
            }
        }
        std::set<int64_t> together;
        for (int64_t f : t.installed_by.vec()) {
            for (int64_t o : installs[f]) if (o != t.at && by_at.at(o)->slots.size() <= t.slots.size()) together.insert(o);
        }
        if (best) {
            w.c.bases = py::list<py::str>{name_or(best->second)};
            w.c.evidence.append(py::str("probably: " + std::to_string(best->first.first) + " of its first " + std::to_string(best->first.second) +
                                        " virtual functions are those of the table at " + hexs(best->second)));
        } else if (together.size() == 1) {
            int64_t o = *together.begin();
            w.c.bases = py::list<py::str>{name_or(o)};
            w.c.evidence.append(py::str("probably: installed in the same object, by the same function, as the table at " + hexs(o)));
        }
    }
    std::vector<int64_t> sorted_order = order;
    std::sort(sorted_order.begin(), sorted_order.end());
    for (int64_t at : sorted_order) {
        Work& w = out[at];
        w.c.name = (w.name && w.name->size() != 0) ? *w.name : py::str("class_" + hexs(w.c.vtable));
        result->append(w.c);
    }
    return result;
}

std::shared_ptr<py::list<Class>> hierarchy(Image& img) {
    return img.memo<py::list<Class>>(py::str("classes.hierarchy"), [](Image& i) { return _hierarchy(i); });
}

// ---- exception handling ---------------------------------------------------------------------------------------------
py::list<CxxTable> _cxx_tables(Image& img) {
    py::list<CxxTable> out;
    const auto& sp = img.spans.vec();
    int64_t lo = std::get<0>(sp.front()), hi = std::get<0>(sp.back()) + std::get<1>(sp.back()).size();
    auto inside = [&](int64_t v) { return lo <= v && v < hi; };
    for (const native::Span& s : sp) {
        if (is_data_name(std::get<4>(s), false)) continue;
        int64_t base = std::get<0>(s), held = std::get<5>(s);
        const std::string& data = std::get<1>(s).raw();
        for (int64_t magic : {0x19930520ll, 0x19930521ll, 0x19930522ll}) {
            std::string pat(4, '\0');
            for (int k = 0; k < 4; k++) pat[static_cast<size_t>(k)] = static_cast<char>((magic >> (8 * k)) & 0xFF);
            size_t pos = 0;
            while (true) {
                size_t f = data.find(pat, pos);
                if (f == std::string::npos || static_cast<int64_t>(f) + 4 > held) break;
                pos = f;
                int64_t at = base + static_cast<int64_t>(pos);
                pos += 4;
                auto states = img.u32(at + 4), unwind = img.u32(at + 8), ntry = img.u32(at + 12), trymap = img.u32(at + 16);
                if (!states || !unwind || !ntry || !trymap || *states > 4096 || *ntry > 256 || (*states && !inside(*unwind)) ||
                    (*ntry && !inside(*trymap)) || !(*states || *ntry)) {
                    continue;
                }
                int64_t cleanups = 0;
                for (int64_t k = 0; k < *states; k++) if (img.u32(*unwind + 8 * k + 4).value_or(0)) cleanups++;
                py::list<CxxTry> tries;
                bool ok = true;
                for (int64_t k = 0; k < *ntry && ok; k++) {
                    int64_t v[5];
                    for (int q = 0; q < 5; q++) v[q] = img.u32(*trymap + 20 * k + 4 * q).value_or(0);
                    CxxTry t;
                    t.low = v[0];
                    t.high = v[1];
                    for (int64_t c = 0; c < std::min<int64_t>(v[3], 64); c++) {
                        auto ptype = img.u32(v[4] + 16 * c + 4), handler = img.u32(v[4] + 16 * c + 12);
                        if (!handler || !img.is_code(*handler)) { ok = false; break; }
                        py::str ty;
                        auto tn = ptype ? _type_name(img, *ptype) : std::nullopt;
                        if (tn && tn->size() != 0) ty = *tn;
                        else ty = py::str((!ptype || *ptype == 0) ? "..." : "?");
                        t.catches.append(std::make_tuple(ty, *handler));
                    }
                    if (ok) tries.append(t);
                }
                if (ok) {
                    CxxTable ct;
                    ct.at = at;
                    ct.states = *states;
                    ct.cleanups = cleanups;
                    ct.tries = tries;
                    out.append(ct);
                }
            }
        }
    }
    return out;
}

py::list<Scope> _scope_table(const Image& img, int64_t at) {
    py::list<Scope> rows;
    for (int64_t k = 0; k < 64; k++) {
        auto level = img.u32(at + 12 * k), flt = img.u32(at + 12 * k + 4), handler = img.u32(at + 12 * k + 8);
        if (!handler || !img.is_code(*handler) || !(level && (*level == 0xFFFFFFFFll || *level < k)) || !(flt && (*flt == 0 || img.is_code(*flt)))) break;
        std::optional<int64_t> lv = *level == 0xFFFFFFFFll ? std::nullopt : level;
        std::optional<int64_t> fl = *flt ? flt : std::nullopt;
        rows.append(Scope(lv, fl, *handler));
    }
    return rows;
}

std::shared_ptr<ExceptionTables> _exception_tables(Image& img) {
    auto res = std::make_shared<ExceptionTables>();
    if (img.bits != 32) return res;
    auto graph = bridge::graph(img);
    res->cxx = _cxx_tables(img);
    std::map<int64_t, int64_t> by_table;
    for (const CxxTable& t : res->cxx.vec()) by_table[t.at] = t.at;
    std::map<int64_t, int64_t> thunks;   // the stub that hands the table to the run-time
    for (const auto& r : graph->refs.vec()) {
        if (r.how == "a" && by_table.count(r.address) && real_ins(img, r).mn == "mov") {
            thunks[r.function] = r.address;
            thunks[r.instruction->addr] = r.address;   // the stub may sit inside its function's bytes
        }
    }
    std::set<int64_t> helpers;   // functions that set a frame up for their caller
    auto each = img.each();
    for (const image::FunctionP& fnp : each.vec()) {
        const Function& fn = *fnp;
        bool a = false, b = false;
        if (fn.insns.size() <= 14) {
            for (const image::InsP& i : fn.insns.vec()) {
                if (i->ops.size() == 2 && i->mn == "mov" && i->ops[0].kind == "m" && i->ops[0].seg && *i->ops[0].seg == "fs" && i->ops[0].disp == 0) a = true;
                if (i->mn == "push" && !i->ops.empty() && i->ops[0].kind == "r" && i->ops[0].name == "eax") b = true;
            }
        }
        if (fn.insns.size() <= 14 && a && b) {
            helpers.insert(fn.start);
        } else if (fn.insns.size() == 1 && flow_is(*fn.insns[0], "jmp")) {
            auto d = image::destination(img, *fn.insns[0]);
            auto nm = d ? img.imports.get(*d) : std::nullopt;
            if (nm && nm->contains(py::str("EH_prolog"))) helpers.insert(fn.start);   // the compiler's own, imported from its run-time
        }
    }
    for (const auto& kv : img.imports.items()) if (std::get<1>(kv).contains(py::str("EH_prolog"))) helpers.insert(std::get<0>(kv));
    for (const image::FunctionP& fnp : each.vec()) {
        const Function& fn = *fnp;
        if (helpers.count(fn.start)) continue;
        size_t nh = std::min<int64_t>(3, fn.insns.size());
        if (nh >= 2) {
            const image::Ins& h0 = *fn.insns[0];
            const image::Ins& h1 = *fn.insns[1];
            if (h0.mn == "mov" && !h0.ops.empty() && h0.ops[0].kind == "r" && h0.ops[0].name == "eax" && h0.ops.size() > 1 && h0.ops[1].kind == "i" &&
                flow_is(h1, "call")) {
                auto d = image::destination(img, h1);
                if (d && helpers.count(*d)) {
                    ExceptionFrame fr;
                    fr.function = fn.start;
                    fr.handler = h0.ops[1].value;
                    if (thunks.count(fr.handler)) fr.cxx = thunks[fr.handler];
                    res->frames.append(fr);
                    continue;
                }
            }
        }
        std::vector<int64_t> pushes;
        for (int64_t i = 0; i < std::min<int64_t>(24, fn.insns.size()); i++) {
            const image::Ins& ins = *fn.insns[i];
            if (ins.mn == "push" && !ins.ops.empty() && ins.ops[0].kind == "i") pushes.push_back(ins.ops[0].value);
            bool fsop = false;
            for (const Op& op : ins.ops.vec()) if (op.kind == "m" && op.seg && *op.seg == "fs" && op.disp == 0 && !op.base) fsop = true;
            if (fsop && ins.ops[0].kind == "m" && ins.mn == "mov") {
                std::optional<int64_t> handler;
                for (auto it = pushes.rbegin(); it != pushes.rend(); ++it) {
                    if (img.is_code(*it) || img.imports.contains(*it)) { handler = *it; break; }
                }
                if (!handler) break;
                ExceptionFrame fr;
                fr.function = fn.start;
                fr.handler = *handler;
                if (thunks.count(*handler)) {
                    fr.cxx = thunks[*handler];
                } else {
                    std::vector<int64_t> data;
                    for (int64_t p : pushes) if (p != *handler && img.span(p) && !(img.is_code(p) && img.names.contains(p))) data.push_back(p);
                    py::list<Scope> rows;
                    if (!data.empty()) rows = _scope_table(img, data.back());
                    if (!rows.empty()) {
                        fr.scope_table = data.back();
                        fr.scopes = rows;
                    }
                }
                res->frames.append(fr);
                break;
            }
            if (ins.flow && (flow_is(ins, "jmp") || flow_is(ins, "ret"))) break;
        }
    }
    return res;
}

std::shared_ptr<ExceptionTables> exception_tables(Image& img) {
    return img.memo<ExceptionTables>(py::str("classes.exceptions"), [](Image& i) { return _exception_tables(i); });
}

// ---- analyse --------------------------------------------------------------------------------------------------------
Analysis analyse(Image& img) {
    auto tables = vtables(img);
    auto tree = hierarchy(img);
    auto runtime = runtime_classes(img);
    auto maps = message_maps(img);
    auto eh = exception_tables(img);
    int64_t thiscall = 0;
    for (const conventions::Fact& f : conventions::functions(img)->values()) if (f.convention && *f.convention == "thiscall") thiscall++;
    Analysis a;
    for (const Class& c : tree->vec()) {
        ClassFact row;
        row.name = c.name;
        row.vtable = c.vtable;
        row.virtual_functions = c.slots;
        for (int64_t i = 0; i < std::min<int64_t>(64, c.methods.size()); i++) row.methods.append(img.label(c.methods[i]));
        row.bases = c.bases;
        row.evidence = c.evidence;
        row.size = c.size;
        for (int64_t f : c.installed_by.vec()) row.constructors_or_destructors.append(img.label(f));
        row.com_interface = c.com;
        row.layout = c.layout;
        row.fields = c.fields;
        row.message_map = c.message_map;
        row.size_exact = c.size_exact;
        a.facts.append(AnyFact(row));
    }
    for (const RuntimeClass& rc : runtime->values()) {
        MfcClassFact f;
        f.record = rc;
        a.facts.append(AnyFact(f));
    }
    auto class_name_for = [&](const MessageMap& m) -> std::optional<py::str> {
        for (const Class& c : tree->vec()) if (m.class_tables.contains(c.vtable)) return c.name;
        return std::nullopt;
    };
    for (const MessageMap& m : maps->vec()) {
        MessageMapFact f;
        f.at = m.at;
        f.class_ = class_name_for(m);
        for (const MapEntry& e : m.entries.vec()) f.entries.append(_macro(img, e));
        f.base_map = m.base_map;
        a.facts.append(AnyFact(f));
    }
    for (const CxxTable& t : eh->cxx.vec()) {
        CxxTableFact f;
        for (const ExceptionFrame& fr : eh->frames.vec()) {
            if (fr.cxx && *fr.cxx == t.at) { f.addr = fr.function; break; }
        }
        f.at = t.at;
        f.states = t.states;
        f.cleanups = t.cleanups;
        for (const CxxTry& b : t.tries.vec()) {
            TryBlockFact tb;
            tb.states = py::str(std::to_string(b.low) + ".." + std::to_string(b.high));
            for (const auto& c : b.catches.vec()) tb.catches.append(py::str("catch (" + std::get<0>(c).utf8() + ") at " + hexs(std::get<1>(c))));
            f.try_blocks.append(tb);
        }
        a.facts.append(AnyFact(f));
    }
    for (const ExceptionFrame& fr : eh->frames.vec()) {
        if (fr.cxx) continue;
        ExceptionFrameFact row;
        row.addr = fr.function;
        row.handler = img.label(fr.handler);
        if (fr.scopes && !fr.scopes->empty()) {
            py::list<ScopeFact> sc;
            for (const Scope& s : fr.scopes->vec()) {
                ScopeFact sf;
                sf.enclosing = std::get<0>(s);
                sf.kind = py::str(std::get<1>(s) ? "__except" : "__finally");
                sf.filter = std::get<1>(s);
                sf.handler = std::get<2>(s);
                sc.append(sf);
            }
            row.scopes = sc;
        }
        a.facts.append(AnyFact(row));
    }
    int64_t named = 0, stated = 0, inferred = 0, layouts = 0;
    for (const Class& c : tree->vec()) {
        bool probably = !c.evidence.empty() && c.evidence[0].startswith(py::str("probably"));
        if (!c.evidence.empty() && !probably) { named++; if (!c.bases.empty()) stated++; }
        if (probably) inferred++;
        if (c.layout) layouts++;
    }
    int64_t vf = 0, com = 0, mentries = 0, tied = 0, trys = 0, catches = 0, scoped = 0, scopes = 0;
    std::set<int64_t> ctors;
    for (const Vtable& t : tables->vec()) {
        vf += t.slots.size();
        if (t.com) com++;
        for (int64_t f : t.installed_by.vec()) ctors.insert(f);
    }
    for (const MessageMap& m : maps->vec()) { mentries += m.entries.size(); if (!m.class_tables.empty()) tied++; }
    for (const CxxTable& t : eh->cxx.vec()) {
        trys += t.tries.size();
        for (const CxxTry& b : t.tries.vec()) catches += b.catches.size();
    }
    for (const ExceptionFrame& fr : eh->frames.vec()) {
        if (fr.scopes && !fr.scopes->empty()) scoped++;
        if (fr.scopes) scopes += fr.scopes->size();
    }
    auto put = [&](const char* k, int64_t v) { a.counts.set(py::str(k), v); };
    put("virtual_tables", tables->size());
    put("virtual_functions", vf);
    put("com_shaped_tables", com);
    put("constructors_or_destructors", static_cast<int64_t>(ctors.size()));
    put("classes_named_by_the_binary", named);
    put("classes_with_stated_bases", stated);
    put("classes_with_inferred_base", inferred);
    put("classes_with_instance_layout", layouts);
    put("mfc_class_records", runtime->size());
    put("message_maps", maps->size());
    put("message_map_entries", mentries);
    put("message_maps_tied_to_a_class", tied);
    put("this_in_ecx_functions", thiscall);
    put("cxx_exception_tables", eh->cxx.size());
    put("try_blocks", trys);
    put("catch_handlers", catches);
    put("exception_frames", eh->frames.size());
    put("frames_with_scope_table", scoped);
    put("except_or_finally_scopes", scopes);
    for (int64_t q = 0; q < std::min<int64_t>(10, tree->size()); q++) {
        const Class& c = (*tree)[q];
        std::string bases;
        if (!c.bases.empty()) {
            bases = " : ";
            for (int64_t i = 0; i < std::min<int64_t>(3, c.bases.size()); i++) bases += (i ? ", " : "") + c.bases[i].utf8();
        }
        std::string size = (c.size && *c.size) ? std::to_string(*c.size) + " bytes" : "size unknown";
        std::string ev = !c.evidence.empty() ? "  [" + c.evidence[0].utf8() + "]" : "";
        a.text.append(py::str("class " + c.name.utf8() + bases + "  table " + hexs(c.vtable) + ", " + std::to_string(c.slots) + " virtual, " + size + ev));
    }
    for (int64_t q = 0; q < std::min<int64_t>(3, maps->size()); q++) {
        const MessageMap& m = (*maps)[q];
        auto nm = class_name_for(m);
        a.text.append(py::str("message map " + hexs(m.at) + (nm ? " of " + nm->utf8() : std::string()) + ":"));
        for (int64_t i = 0; i < std::min<int64_t>(8, m.entries.size()); i++) a.text.append(py::str("    " + _macro(img, m.entries[i]).utf8()));
    }
    return a;
}

}  // namespace codebreaker::analyze::classes
