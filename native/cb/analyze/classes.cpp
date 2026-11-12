#include "cb/analyze/classes.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/analyze/dispatch.hpp"
#include "cb/analyze/strings.hpp"
#include "cb/core/text.hpp"

namespace cb::analyze::classes {

namespace {

constexpr std::uint32_t kNoLevel = 0xFFFFFFFFu;

// A hexadecimal number as the report writes it: no prefix, a sign for a negative one.
std::string cl_hex(std::int64_t v) { return std::format("{}{:x}", v < 0 ? "-" : "", v < 0 ? -v : v); }

// The word at `va` as an address-sized number.
std::optional<Va> cl_word(const Image& image, Va va) {
    const std::optional<std::uint32_t> w = image.u32(va);
    if (!w) return std::nullopt;
    return static_cast<Va>(*w);
}

std::int64_t cl_le32(const Bytes& data, std::size_t at) {
    return static_cast<std::int64_t>(data[at]) | (static_cast<std::int64_t>(data[at + 1]) << 8) | (static_cast<std::int64_t>(data[at + 2]) << 16) |
           (static_cast<std::int64_t>(data[at + 3]) << 24);
}

// Is this section one the loader or the file owns (resources, relocations; with `exports`, the export and import tables too)?
bool cl_is_file_section(std::string_view name, bool exports) {
    return name == ".rsrc" || name == ".reloc" || (exports && (name == ".edata" || name == ".idata"));
}

// The instruction of function `fn` at `addr`, or null.
const Instruction* cl_instruction_at(const Function& fn, Va addr) {
    const auto it = std::lower_bound(fn.insns.begin(), fn.insns.end(), addr, [](const Instruction& i, Va a) { return i.addr < a; });
    return it != fn.insns.end() && it->addr == addr ? &*it : nullptr;
}

// The address a call or jump reads its destination from (`call [slot]`) when that is an import, else its direct target.
std::optional<Va> cl_destination(const Image& image, const Instruction& ins) {
    if (ins.target) return ins.target;
    if (ins.operands.empty() || ins.operands[0].kind != Operand::Kind::Memory || !ins.operands[0].base.empty() || !ins.operands[0].index.empty())
        return std::nullopt;
    const Va slot = ins.operands[0].value;
    return image.is_import(slot) ? std::optional<Va>(slot) : std::nullopt;
}

// (address, value) for every aligned word of the module's data that holds an address inside the module.
std::vector<std::pair<Va, Va>> cl_aligned_pointers(const Image& image) {
    std::vector<std::pair<Va, Va>> out;
    const std::vector<Span>& spans = image.spans();
    if (spans.empty()) return out;
    const Va lo = spans.front().va, hi = spans.back().va + static_cast<Va>(spans.back().data.size());
    for (const Span& s : spans) {
        if (s.executable || cl_is_file_section(s.name, true)) continue;
        for (Va off = 0; off < s.initialised - 3; off += 4) {
            const Va v = cl_le32(s.data, static_cast<std::size_t>(off));
            if (lo <= v && v < hi) out.emplace_back(s.va + off, v);
        }
    }
    return out;
}

}  // namespace

// ---- virtual tables ---------------------------------------------------------------------------------------------------------

std::vector<Vtable> find_vtables(const Inputs& in) {
    Image& image = in.image;
    std::vector<Vtable> out;
    if (image.bits() != 32) return out;
    const std::set<Va> functions(image.functions().begin(), image.functions().end());
    auto is_function = [&](const std::optional<Va>& v) { return v && functions.contains(*v); };
    std::set<Va> named;   // addresses the code names
    for (const call_graph::Reference& r : in.graph.refs)
        if (r.how == call_graph::Access::AddressTaken) named.insert(r.address);
    struct Store {
        Va function;
        Va at;
        std::int64_t offset;
    };
    std::map<Va, std::vector<Store>> stores;   // table -> who installs it
    std::map<Va, std::vector<Va>> statics;     // table -> objects in the data that start with it
    for (const call_graph::Reference& r : in.graph.refs) {
        const Instruction* ins = image.covering(r.instruction);
        const Va va = r.address;
        if (ins == nullptr || r.how != call_graph::Access::AddressTaken || ins->mnemonic != "mov" || ins->operands.size() != 2 ||
            ins->operands[1].kind != Operand::Kind::Immediate || ins->operands[1].value != va || !is_function(cl_word(image, va)))
            continue;
        const Operand& to = ins->operands[0];
        if (to.kind == Operand::Kind::Memory && !to.base.empty()) {   // mov [reg+N], offset table
            stores[va].push_back({r.function, ins->addr, to.value});
        } else if (to.kind == Operand::Kind::Register) {              // mov reg, offset table / mov [reg2+N], reg
            const Function& fn = image.function(r.function);
            const auto here = std::lower_bound(fn.insns.begin(), fn.insns.end(), ins->addr, [](const Instruction& i, Va a) { return i.addr < a; });
            const std::string family_of = std::string(family(to.reg));
            for (auto q = here + 1; q != fn.insns.end() && q < here + 4; ++q) {
                if (q->mnemonic == "mov" && q->operands.size() == 2 && q->operands[0].kind == Operand::Kind::Memory && !q->operands[0].base.empty() &&
                    q->operands[1].kind == Operand::Kind::Register && q->operands[1].reg == to.reg) {
                    stores[va].push_back({r.function, q->addr, q->operands[0].value});
                    break;
                }
                if (q->writes.contains(family_of) || q->flow != Flow::None) break;
            }
        }
    }
    if (image.has_pointers() && !image.pointer_list().empty()) {   // an object in the data that starts with one
        for (const auto& [site, pointer] : image.pointer_list()) {
            const std::optional<Va>& t = pointer.target;
            if (t && !image.is_code(site) && is_function(cl_word(image, *t)) && image.pointer_at(*t) != nullptr && is_function(cl_word(image, *t + 4)) &&
                is_function(cl_word(image, *t + 8)) && site != *t - 4 && site != *t + 4)
                statics[*t].push_back(site);
        }
    }
    std::set<Va> all;
    for (const auto& [at, who] : stores) all.insert(at);
    for (const auto& [at, objects] : statics) all.insert(at);
    for (const Va va : all) {
        std::vector<Va> slots;
        Va at = va;
        for (;;) {
            const std::optional<Va> t = cl_word(image, at);
            if (!is_function(t) || (!slots.empty() && (named.contains(at) || statics.contains(at))) || (image.has_pointers() && image.pointer_at(at) == nullptr)) break;
            slots.push_back(*t);
            at += 4;
        }
        if (slots.empty()) continue;
        if (!stores.contains(va)) {
            bool any = false;
            for (Va a = va + 4; a < at; a += 4)
                if (statics.contains(a) || stores.contains(a)) any = true;
            if (slots.size() < 3 || any) continue;
        }
        std::vector<std::optional<int>> removes;
        for (std::size_t k = 0; k < slots.size() && k < 7; ++k) removes.push_back(in.conventions.at(slots[k]).removes);
        auto shape = [&](std::size_t from, std::initializer_list<int> want) {
            if (from + want.size() > removes.size()) return false;
            std::size_t i = from;
            for (const int w : want) {
                if (!removes[i] || *removes[i] != w) return false;
                ++i;
            }
            return true;
        };
        Vtable v;
        v.at = va;
        v.slots = slots;
        v.com = shape(0, {12, 4, 4});
        v.dispatch = v.com && shape(3, {8, 16, 24, 36});
        std::set<Va> installers;
        std::set<std::int64_t> offsets;
        if (const auto it = stores.find(va); it != stores.end()) {
            for (const Store& s : it->second) {
                installers.insert(s.function);
                offsets.insert(s.offset);
            }
        }
        v.installed_by.assign(installers.begin(), installers.end());
        v.at_offsets.assign(offsets.begin(), offsets.end());
        if (const auto it = statics.find(va); it != statics.end()) {
            v.static_objects = it->second;
            std::sort(v.static_objects.begin(), v.static_objects.end());
        }
        out.push_back(std::move(v));
    }
    return out;
}

// ---- run-time type information ----------------------------------------------------------------------------------------------

std::optional<std::string> decorated_name(std::string_view name) {
    if (!name.starts_with(".?A") || name.size() < 6) return std::nullopt;
    const std::string_view rest = name.substr(4);
    std::vector<std::string_view> parts;
    for (const std::string_view piece : text::split(rest, '@'))
        if (!piece.empty()) parts.push_back(piece);
    auto is_name_char = [](char c) {
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '?' || c == '$' || c == '<' || c == '>' || c == ',';
    };
    const bool all = !parts.empty() && std::all_of(parts.begin(), parts.end(), [&](std::string_view p) { return std::all_of(p.begin(), p.end(), is_name_char); });
    if (!all) return std::string(rest);
    std::string out;
    for (auto it = parts.rbegin(); it != parts.rend(); ++it) {
        if (it != parts.rbegin()) out += "::";
        out += *it;
    }
    return out;
}

std::optional<std::string> type_name(const Image& image, Va descriptor) {
    if (descriptor == 0 || image.span(descriptor) == nullptr) return std::nullopt;
    const std::optional<strings::Found> s = strings::string_at(image, descriptor + 8, 4);
    if (s && s->text.starts_with(".?A")) return decorated_name(s->text);
    return std::nullopt;
}

std::optional<Rtti> rtti(const Image& image, Va table) {
    const std::optional<Va> locator = cl_word(image, table - 4);
    if (!locator || *locator == 0 || image.span(*locator) == nullptr || (image.is_code(*locator) && image.owner(*locator))) return std::nullopt;
    const std::optional<Va> signature = cl_word(image, *locator), descriptor = cl_word(image, *locator + 12), hierarchy = cl_word(image, *locator + 16);
    if (!signature || *signature != 0 || !descriptor) return std::nullopt;
    const std::optional<std::string> name = type_name(image, *descriptor);
    if (!name || name->empty()) return std::nullopt;
    Rtti out{*name, {}};
    Va count = 0, array = 0;
    if (hierarchy && *hierarchy != 0 && image.span(*hierarchy) != nullptr) {
        count = cl_word(image, *hierarchy + 8).value_or(0);
        array = cl_word(image, *hierarchy + 12).value_or(0);
    }
    for (Va k = 1; k < std::min<Va>(count, 64); ++k) {   // entry 0 is the class itself
        if (array == 0) break;
        const Va entry = cl_word(image, array + 4 * k).value_or(0);
        const std::optional<std::string> base = type_name(image, cl_word(image, entry).value_or(0));
        if (base && !base->empty()) out.bases.push_back(*base);
    }
    return out;
}

// ---- MFC --------------------------------------------------------------------------------------------------------------------

std::optional<Va> returned_address(const Image& image, Va va) {
    const Bytes b = image.read(va, 6);
    if (b.size() == 6 && (b[0] == 0xB8 || b[0] == 0xA1) && b[5] == 0xC3) {
        const std::int64_t x = cl_le32(b, 1);
        if ((b[0] == 0xB8 && image.span(x) != nullptr) || (b[0] == 0xA1 && image.is_import(x))) return x;
    }
    return std::nullopt;
}

std::map<Va, Va> returns_address(const Image& image) {
    std::map<Va, Va> out;
    for (const Va f : image.functions())
        if (const auto v = returned_address(image, f)) out[f] = *v;
    return out;
}

std::string class_of_import(std::string_view name) {
    // ?classCWnd@CWnd@@2UCRuntimeClass@@B: the word after "?class", up to the first "@" (the shortest run of word characters)
    const std::size_t at = name.find("?class");
    if (at != std::string_view::npos) {
        std::size_t end = at + 6;
        while (end < name.size() && name[end] != '@' && (std::isalnum(static_cast<unsigned char>(name[end])) || name[end] == '_')) ++end;
        if (end > at + 6 && end < name.size() && name[end] == '@') return std::string(name.substr(at + 6, end - at - 6));
    }
    const std::size_t bang = name.rfind('!');
    return std::string(bang == std::string_view::npos ? name : name.substr(bang + 1));
}

std::vector<RuntimeClass> find_runtime_classes(const Image& image) {
    std::vector<RuntimeClass> out;
    if (image.bits() != 32) return out;
    struct Found {
        RuntimeClass c;
        Va base = 0;
    };
    std::vector<Va> order;
    std::map<Va, Found> found;
    for (const auto& [at, v] : cl_aligned_pointers(image)) {
        const std::optional<strings::Found> s = strings::string_at(image, v, 2);
        if (!s || s->encoding != strings::Encoding::Ascii) continue;
        // [A-Za-z_][A-Za-z0-9_]{1,63}, whole
        const std::string& name = s->text;
        auto word = [](char c, bool first) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || (!first && c >= '0' && c <= '9'); };
        if (name.size() < 2 || name.size() > 64 || !word(name[0], true) || !std::all_of(name.begin() + 1, name.end(), [&](char c) { return word(c, false); })) continue;
        const std::optional<Va> size = cl_word(image, at + 4), schema = cl_word(image, at + 8), create = cl_word(image, at + 12), base = cl_word(image, at + 16);
        if (!size || !schema || !create || !base || !(4 <= *size && *size <= 0x10000) || !(*schema <= 0x1000 || *schema == 0xFFFF)) continue;
        if (*create != 0 && !image.is_code(*create)) continue;
        Found f;
        f.c.at = at;
        f.c.name = name;
        f.c.size = *size;
        if (*schema != 0xFFFF) f.c.schema = *schema;
        f.c.creatable = *create != 0;
        f.base = *base;
        if (!found.contains(at)) order.push_back(at);
        found[at] = std::move(f);
    }
    std::map<Va, bool> linked;
    for (const Va at : order) {
        Found& f = found[at];
        Va b = f.base;
        if (b != 0 && image.is_code(b)) {   // the DLL build: a function returns the base's record
            const auto r = returned_address(image, b);
            b = (r && *r != 0) ? *r : b;
        }
        if (found.contains(b)) f.c.base = found[b].c.name;
        else if (image.is_import(b)) f.c.base = class_of_import(image.import_name(b));
        else f.c.base = std::nullopt;
        linked[at] = b == 0 || f.c.base.has_value();
    }
    std::set<std::string> bases;
    for (const Va at : order)
        if (found[at].c.base && !found[at].c.base->empty()) bases.insert(*found[at].c.base);
    for (const Va at : order) {
        const RuntimeClass& c = found[at].c;
        if (linked[at] && ((c.base && !c.base->empty()) || bases.contains(c.name))) out.push_back(c);
    }
    return out;
}

std::string id_range(std::int64_t first, std::int64_t last) { return first == last ? std::to_string(first) : std::to_string(first) + ", " + std::to_string(last); }

std::string macro_of(const Image& image, const MapEntry& e) {
    const std::string handler = image.label(e.handler);
    const std::string range = e.last_id != e.id ? "_RANGE" : "";
    const std::string ids = id_range(e.id, e.last_id);
    if (e.message == 0x111) {
        if (e.code == 0xFFFFFFFFLL || e.code == 0xFFFF) return "ON_UPDATE_COMMAND_UI" + range + "(" + ids + ", " + handler + ")";
        if (e.code == 0) return "ON_COMMAND" + range + "(" + ids + ", " + handler + ")";
        return "ON_CONTROL" + range + "(0x" + cl_hex(e.code) + ", " + ids + ", " + handler + ")";
    }
    if (e.message == 0x4E) {
        const std::int64_t c = (e.code >> 31) != 0 ? e.code - (std::int64_t(1) << 32) : e.code;   // (a notification code is signed)
        return "ON_NOTIFY" + range + "(" + std::to_string(c) + ", " + ids + ", " + handler + ")";
    }
    if (e.message == 0xC000) return "ON_REGISTERED_MESSAGE(*(UINT *)0x" + cl_hex(e.signature > 0x10000 ? e.signature : e.id) + ", " + handler + ")";
    if (const auto name = dispatch::message_name(e.message)) {
        if (!(e.signature == 10 || e.signature == 11 || e.signature == 12)) return "ON_" + std::string(*name) + "() -> " + handler;
        return "ON_MESSAGE(" + std::string(*name) + ", " + handler + ")";
    }
    std::string h = cl_hex(e.message);
    if (h.size() < 4) h = std::string(4 - h.size(), '0') + h;
    return "ON_MESSAGE(0x" + h + ", " + handler + ")";
}

std::vector<MessageMap> find_message_maps(const Image& image, std::span<const Vtable> vtables) {
    std::vector<MessageMap> out;
    if (image.bits() != 32) return out;
    const std::set<Va> functions(image.functions().begin(), image.functions().end());
    const std::map<Va, Va> getters = returns_address(image);
    std::map<Va, std::vector<Va>> holders;   // the address a word of data holds -> where
    for (const auto& [at, v] : cl_aligned_pointers(image)) holders[v].push_back(at);
    std::map<Va, std::vector<Va>> by_table;   // the record a getter returns -> the tables whose functions return it
    for (const Vtable& vt : vtables)
        for (const Va f : vt.slots)
            if (const auto g = getters.find(f); g != getters.end()) by_table[g->second].push_back(vt.at);
    Va skip = -1;
    for (const Span& s : image.spans()) {
        if (s.executable || cl_is_file_section(s.name, true)) continue;
        const Va base = s.va, held = s.initialised;
        Va off = 0;
        while (off + 48 <= held) {
            if (base + off < skip || !functions.contains(cl_le32(s.data, static_cast<std::size_t>(off + 20)))) {
                off += 4;
                continue;
            }
            std::vector<MapEntry> entries;
            Va p = off;
            bool broke = false;
            while (p + 24 <= held) {
                std::int64_t e[6];
                for (int k = 0; k < 6; ++k) e[k] = cl_le32(s.data, static_cast<std::size_t>(p + 4 * k));
                if (e[0] == 0 && e[5] == 0 && e[4] == 0) {   // the zero entry that ends the map
                    broke = true;
                    break;
                }
                if (!functions.contains(e[5]) || e[0] > 0xFFFF || !(e[4] <= 0x80 || image.span(e[4]) != nullptr) || e[2] > 0xFFFF || e[3] > 0xFFFF ||
                    !(e[1] <= 0xFFFF || e[1] >= 0xFFFFF000LL)) {
                    entries.clear();
                    broke = true;
                    break;
                }
                entries.push_back({e[0], e[1], e[2], e[3], e[4], e[5]});
                p += 24;
            }
            if (!broke) entries.clear();
            const Va at = base + off;
            std::vector<Va> owners;
            if (const auto hit = holders.find(at); hit != holders.end())
                for (const Va h : hit->second)
                    if (h - 4 >= base) owners.push_back(h - 4);
            if (!entries.empty() && (!owners.empty() || entries.size() >= 3)) {
                MessageMap rec;
                rec.at = at;
                rec.entries = std::move(entries);
                if (!owners.empty()) {
                    rec.map = owners[0];
                    if (const auto ct = by_table.find(owners[0]); ct != by_table.end()) {
                        rec.class_tables = ct->second;
                        std::sort(rec.class_tables.begin(), rec.class_tables.end());
                    }
                    const std::optional<Va> b = cl_word(image, owners[0]);
                    std::optional<Va> base_map = b;
                    if (b) {
                        if (const auto g = getters.find(*b); g != getters.end()) base_map = g->second;
                    }
                    if (base_map && *base_map != 0) rec.base_map = base_map;
                }
                out.push_back(std::move(rec));
                skip = base + (p - off) + off + 24;
                off = p + 24 - ((p + 24) % 4);
                continue;
            }
            off += 4;
        }
    }
    return out;
}

Model build_model(const Inputs& in) {
    Model m;
    m.vtables = find_vtables(in);
    m.getters = returns_address(in.image);
    m.runtime = find_runtime_classes(in.image);
    m.maps = find_message_maps(in.image, m.vtables);
    return m;
}

// ---- the hierarchy ----------------------------------------------------------------------------------------------------------

std::vector<Class> hierarchy(const Image& image, const Model& model, std::span<const structures::Structure> structures) {
    std::vector<Class> result;
    if (model.vtables.empty()) return result;
    std::map<Va, const RuntimeClass*> runtime_at;
    std::map<std::string, const RuntimeClass*> runtime_named;
    for (const RuntimeClass& r : model.runtime) {
        runtime_at[r.at] = &r;
        runtime_named[r.name] = &r;
    }
    std::map<Va, const Vtable*> by_at;
    for (const Vtable& t : model.vtables) by_at[t.at] = &t;
    std::map<Va, const structures::Structure*> layouts;   // the structure whose first field is given a table's address
    for (const structures::Structure& s : structures) {
        const auto f0 = s.fields.find(0);
        if (f0 == s.fields.end()) continue;
        for (const Va v : f0->second.stores)
            if (by_at.contains(v) && !layouts.contains(v)) layouts[v] = &s;
    }
    std::map<Va, const MessageMap*> maps;
    for (const MessageMap& m : model.maps)
        for (const Va t : m.class_tables) maps[t] = &m;
    struct Work {
        Class c;
        std::optional<std::string> name;
    };
    std::vector<Va> order;
    std::map<Va, Work> out;
    for (const Vtable& t : model.vtables) {
        Work w;
        Class& c = w.c;
        c.vtable = t.at;
        c.slots = static_cast<std::int64_t>(t.slots.size());
        c.methods = t.slots;
        c.installed_by = t.installed_by;
        c.com = t.com;
        const std::optional<Rtti> info = rtti(image, t.at);
        const Va s0 = t.slots[0];
        const auto getter = model.getters.find(s0);
        if (info) {
            w.name = info->name;
            c.bases = info->bases;
            c.evidence.push_back("run-time type information");
        } else if (getter != model.getters.end() && runtime_at.contains(getter->second)) {
            const RuntimeClass& rc = *runtime_at.at(getter->second);
            w.name = rc.name;
            c.size = rc.size;
            std::set<std::string> seen = {rc.name};
            std::optional<std::string> b = rc.base;
            while (b && !b->empty() && !seen.contains(*b)) {
                c.bases.push_back(*b);
                seen.insert(*b);
                const auto next = runtime_named.find(*b);
                b = next != runtime_named.end() ? next->second->base : std::nullopt;
            }
            c.evidence.push_back("MFC class record at " + cl_hex(rc.at));
        }
        if (const auto lit = layouts.find(t.at); lit != layouts.end()) {
            c.layout = lit->second->name;
            c.fields = static_cast<std::int64_t>(lit->second->fields.size());
            if (!c.size) {
                c.size = lit->second->size;
                c.size_exact = lit->second->exact;
            }
        }
        if (const auto mit = maps.find(t.at); mit != maps.end()) c.message_map = mit->second->at;
        if (!out.contains(t.at)) order.push_back(t.at);
        out[t.at] = std::move(w);
    }
    // inheritance the binary does not state: shared leading slots, and the order tables are installed
    std::map<Va, std::vector<Va>> installs;
    for (const Vtable& t : model.vtables)
        for (const Va f : t.installed_by) installs[f].push_back(t.at);
    auto name_or = [&](Va at) { return out[at].name && !out[at].name->empty() ? *out[at].name : "class_" + cl_hex(at); };
    for (std::size_t ti = 0; ti < model.vtables.size(); ++ti) {
        const Vtable& t = model.vtables[ti];
        Work& w = out[t.at];
        if (!w.c.bases.empty()) continue;
        std::optional<std::pair<std::pair<std::int64_t, std::int64_t>, Va>> best;
        for (std::size_t ui = 0; ui < model.vtables.size(); ++ui) {
            const Vtable& u = model.vtables[ui];
            if (ui == ti || u.slots.size() >= t.slots.size() || u.slots.size() < 2) continue;
            std::int64_t same = 0;
            for (std::size_t k = 0; k < std::min(u.slots.size(), t.slots.size()); ++k)
                if (u.slots[k] == t.slots[k]) ++same;
            const std::pair<std::int64_t, std::int64_t> score(same, static_cast<std::int64_t>(u.slots.size()));
            if (same >= std::max<std::int64_t>(2, static_cast<std::int64_t>(u.slots.size()) / 2) && (!best || score > best->first)) best = std::make_pair(score, u.at);
        }
        std::set<Va> together;
        for (const Va f : t.installed_by)
            for (const Va o : installs[f])
                if (o != t.at && by_at.at(o)->slots.size() <= t.slots.size()) together.insert(o);
        if (best) {
            w.c.bases = {name_or(best->second)};
            w.c.evidence.push_back(std::format("probably: {} of its first {} virtual functions are those of the table at {}", best->first.first, best->first.second,
                                               cl_hex(best->second)));
        } else if (together.size() == 1) {
            const Va o = *together.begin();
            w.c.bases = {name_or(o)};
            w.c.evidence.push_back("probably: installed in the same object, by the same function, as the table at " + cl_hex(o));
        }
    }
    std::sort(order.begin(), order.end());
    for (const Va at : order) {
        Work& w = out[at];
        w.c.name = (w.name && !w.name->empty()) ? *w.name : "class_" + cl_hex(w.c.vtable);
        result.push_back(std::move(w.c));
    }
    return result;
}

// ---- exception handling -----------------------------------------------------------------------------------------------------

std::vector<CxxTable> find_cxx_tables(const Image& image) {
    std::vector<CxxTable> out;
    const std::vector<Span>& spans = image.spans();
    if (spans.empty()) return out;
    const Va lo = spans.front().va, hi = spans.back().va + static_cast<Va>(spans.back().data.size());
    auto inside = [&](Va v) { return lo <= v && v < hi; };
    for (const Span& s : spans) {
        if (cl_is_file_section(s.name, false)) continue;
        const Va base = s.va, held = s.initialised;
        for (const std::int64_t magic : {0x19930520LL, 0x19930521LL, 0x19930522LL}) {
            const std::uint8_t pattern[4] = {static_cast<std::uint8_t>(magic), static_cast<std::uint8_t>(magic >> 8), static_cast<std::uint8_t>(magic >> 16),
                                             static_cast<std::uint8_t>(magic >> 24)};
            auto from = s.data.begin();
            for (;;) {
                const auto f = std::search(from, s.data.end(), std::begin(pattern), std::end(pattern));
                if (f == s.data.end() || static_cast<Va>(f - s.data.begin()) + 4 > held) break;
                const Va pos = static_cast<Va>(f - s.data.begin());
                from = f + 4;
                const Va at = base + pos;
                const std::optional<Va> states = cl_word(image, at + 4), unwind = cl_word(image, at + 8), ntry = cl_word(image, at + 12), trymap = cl_word(image, at + 16);
                if (!states || !unwind || !ntry || !trymap || *states > 4096 || *ntry > 256 || (*states != 0 && !inside(*unwind)) || (*ntry != 0 && !inside(*trymap)) ||
                    !(*states != 0 || *ntry != 0))
                    continue;
                std::int64_t cleanups = 0;
                for (Va k = 0; k < *states; ++k)
                    if (cl_word(image, *unwind + 8 * k + 4).value_or(0) != 0) ++cleanups;
                std::vector<CxxTry> tries;
                bool ok = true;
                for (Va k = 0; k < *ntry && ok; ++k) {
                    Va v[5];
                    for (int q = 0; q < 5; ++q) v[q] = cl_word(image, *trymap + 20 * k + 4 * q).value_or(0);
                    CxxTry t;
                    t.low = v[0];
                    t.high = v[1];
                    for (Va c = 0; c < std::min<Va>(v[3], 64); ++c) {
                        const std::optional<Va> ptype = cl_word(image, v[4] + 16 * c + 4), handler = cl_word(image, v[4] + 16 * c + 12);
                        if (!handler || !image.is_code(*handler)) {
                            ok = false;
                            break;
                        }
                        const std::optional<std::string> named = ptype ? type_name(image, *ptype) : std::nullopt;
                        t.catches.push_back({named && !named->empty() ? *named : (!ptype || *ptype == 0) ? "..." : "?", *handler});
                    }
                    if (ok) tries.push_back(std::move(t));
                }
                if (ok) out.push_back({at, *states, cleanups, std::move(tries)});
            }
        }
    }
    return out;
}

std::vector<Scope> read_scope_table(const Image& image, Va at) {
    std::vector<Scope> rows;
    for (Va k = 0; k < 64; ++k) {
        const std::optional<Va> level = cl_word(image, at + 12 * k), filter = cl_word(image, at + 12 * k + 4), handler = cl_word(image, at + 12 * k + 8);
        if (!handler || !image.is_code(*handler) || !(level && (*level == kNoLevel || *level < k)) || !(filter && (*filter == 0 || image.is_code(*filter)))) break;
        rows.push_back({*level == kNoLevel ? std::nullopt : level, *filter != 0 ? filter : std::nullopt, *handler});
    }
    return rows;
}

ExceptionTables find_exception_tables(Image& image, const call_graph::Graph& graph) {
    ExceptionTables res;
    if (image.bits() != 32) return res;
    res.cxx = find_cxx_tables(image);
    std::set<Va> table_at;
    for (const CxxTable& t : res.cxx) table_at.insert(t.at);
    std::map<Va, Va> thunks;   // the stub that hands the table to the run-time
    for (const call_graph::Reference& r : graph.refs) {
        const Instruction* ins = table_at.contains(r.address) && r.how == call_graph::Access::AddressTaken ? image.covering(r.instruction) : nullptr;
        if (ins != nullptr && ins->mnemonic == "mov") {
            thunks[r.function] = r.address;
            thunks[r.instruction] = r.address;   // the stub may sit inside its function's bytes
        }
    }
    std::set<Va> helpers;   // functions that set a frame up for their caller
    const std::vector<Va> starts = image.functions();
    for (const Va start : starts) {
        const Function& fn = image.function(start);
        bool a = false, b = false;
        if (fn.insns.size() <= 14) {
            for (const Instruction& i : fn.insns) {
                if (i.operands.size() == 2 && i.mnemonic == "mov" && i.operands[0].kind == Operand::Kind::Memory && i.operands[0].seg == "fs" && i.operands[0].value == 0) a = true;
                if (i.mnemonic == "push" && !i.operands.empty() && i.operands[0].kind == Operand::Kind::Register && i.operands[0].reg == "eax") b = true;
            }
        }
        if (fn.insns.size() <= 14 && a && b) {
            helpers.insert(fn.start);
        } else if (fn.insns.size() == 1 && fn.insns[0].flow == Flow::Jump) {
            const auto d = cl_destination(image, fn.insns[0]);
            if (d && image.import_name(*d).find("EH_prolog") != std::string::npos) helpers.insert(fn.start);   // the compiler's own, imported from its run-time
        }
    }
    for (const auto& [va, name] : image.imports())
        if (name.find("EH_prolog") != std::string::npos) helpers.insert(va);
    for (const Va start : starts) {
        const Function& fn = image.function(start);
        if (helpers.contains(fn.start)) continue;
        if (fn.insns.size() >= 2) {
            const Instruction& h0 = fn.insns[0];
            const Instruction& h1 = fn.insns[1];
            if (h0.mnemonic == "mov" && !h0.operands.empty() && h0.operands[0].kind == Operand::Kind::Register && h0.operands[0].reg == "eax" && h0.operands.size() > 1 &&
                h0.operands[1].kind == Operand::Kind::Immediate && h1.flow == Flow::Call) {
                const auto d = cl_destination(image, h1);
                if (d && helpers.contains(*d)) {
                    ExceptionFrame fr;
                    fr.function = fn.start;
                    fr.handler = h0.operands[1].value;
                    if (const auto t = thunks.find(fr.handler); t != thunks.end()) fr.cxx = t->second;
                    res.frames.push_back(std::move(fr));
                    continue;
                }
            }
        }
        std::vector<Va> pushes;
        for (std::size_t i = 0; i < std::min<std::size_t>(24, fn.insns.size()); ++i) {
            const Instruction& ins = fn.insns[i];
            if (ins.mnemonic == "push" && !ins.operands.empty() && ins.operands[0].kind == Operand::Kind::Immediate) pushes.push_back(ins.operands[0].value);
            const bool fs_operand = std::any_of(ins.operands.begin(), ins.operands.end(), [](const Operand& op) {
                return op.kind == Operand::Kind::Memory && op.seg == "fs" && op.value == 0 && op.base.empty();
            });
            if (fs_operand && ins.operands[0].kind == Operand::Kind::Memory && ins.mnemonic == "mov") {
                std::optional<Va> handler;
                for (auto it = pushes.rbegin(); it != pushes.rend(); ++it) {
                    if (image.is_code(*it) || image.is_import(*it)) {
                        handler = *it;
                        break;
                    }
                }
                if (!handler) break;
                ExceptionFrame fr;
                fr.function = fn.start;
                fr.handler = *handler;
                if (const auto t = thunks.find(*handler); t != thunks.end()) {
                    fr.cxx = t->second;
                } else {
                    std::vector<Va> data;
                    for (const Va p : pushes)
                        if (p != *handler && image.span(p) != nullptr && !(image.is_code(p) && !image.function_name(p).empty())) data.push_back(p);
                    std::vector<Scope> rows;
                    if (!data.empty()) rows = read_scope_table(image, data.back());
                    if (!rows.empty()) {
                        fr.scope_table = data.back();
                        fr.scopes = std::move(rows);
                    }
                }
                res.frames.push_back(std::move(fr));
                break;
            }
            if (ins.flow == Flow::Jump || ins.flow == Flow::Return) break;
        }
    }
    return res;
}

// ---- the report -------------------------------------------------------------------------------------------------------------

Analysis analyse(const Inputs& in, std::span<const structures::Structure> structures) {
    const Image& image = in.image;
    const Model model = build_model(in);
    const std::vector<Class> tree = hierarchy(image, model, structures);
    const ExceptionTables eh = find_exception_tables(in.image, in.graph);
    std::int64_t thiscall = 0;
    for (const auto& [va, fact] : in.conventions)
        if (fact.convention == "thiscall") ++thiscall;
    Analysis a;
    for (const Class& c : tree) {
        ClassFact row;
        row.name = c.name;
        row.vtable = c.vtable;
        row.virtual_functions = c.slots;
        for (std::size_t i = 0; i < std::min<std::size_t>(64, c.methods.size()); ++i) row.methods.push_back(image.label(c.methods[i]));
        row.bases = c.bases;
        row.evidence = c.evidence;
        row.size = c.size;
        for (const Va f : c.installed_by) row.constructors_or_destructors.push_back(image.label(f));
        row.com_interface = c.com;
        row.layout = c.layout;
        row.fields = c.fields;
        row.message_map = c.message_map;
        row.size_exact = c.size_exact;
        a.facts.push_back(std::move(row));
    }
    for (const RuntimeClass& rc : model.runtime) a.facts.push_back(MfcClassFact{rc});
    auto class_name_for = [&](const MessageMap& m) -> std::optional<std::string> {
        for (const Class& c : tree)
            if (std::find(m.class_tables.begin(), m.class_tables.end(), c.vtable) != m.class_tables.end()) return c.name;
        return std::nullopt;
    };
    for (const MessageMap& m : model.maps) {
        MessageMapFact f;
        f.at = m.at;
        f.class_name = class_name_for(m);
        for (const MapEntry& e : m.entries) f.entries.push_back(macro_of(image, e));
        f.base_map = m.base_map;
        a.facts.push_back(std::move(f));
    }
    for (const CxxTable& t : eh.cxx) {
        CxxTableFact f;
        for (const ExceptionFrame& fr : eh.frames) {
            if (fr.cxx && *fr.cxx == t.at) {
                f.function = fr.function;
                break;
            }
        }
        f.at = t.at;
        f.states = t.states;
        f.cleanups = t.cleanups;
        for (const CxxTry& b : t.tries) {
            TryBlockFact tb;
            tb.states = std::format("{}..{}", b.low, b.high);
            for (const CxxCatch& c : b.catches) tb.catches.push_back("catch (" + c.type + ") at " + cl_hex(c.handler));
            f.try_blocks.push_back(std::move(tb));
        }
        a.facts.push_back(std::move(f));
    }
    for (const ExceptionFrame& fr : eh.frames) {
        if (fr.cxx) continue;
        ExceptionFrameFact row;
        row.function = fr.function;
        row.handler = image.label(fr.handler);
        if (fr.scopes && !fr.scopes->empty()) {
            std::vector<ScopeFact> sc;
            for (const Scope& s : *fr.scopes) sc.push_back({s.enclosing, s.filter ? "__except" : "__finally", s.filter, s.handler});
            row.scopes = std::move(sc);
        }
        a.facts.push_back(std::move(row));
    }
    std::int64_t named = 0, stated = 0, inferred = 0, layouts = 0;
    for (const Class& c : tree) {
        const bool probably = !c.evidence.empty() && c.evidence[0].starts_with("probably");
        if (!c.evidence.empty() && !probably) {
            ++named;
            if (!c.bases.empty()) ++stated;
        }
        if (probably) ++inferred;
        if (c.layout) ++layouts;
    }
    std::int64_t virtual_functions = 0, com = 0, entries = 0, tied = 0, tries = 0, catches = 0, scoped = 0, scopes = 0;
    std::set<Va> constructors;
    for (const Vtable& t : model.vtables) {
        virtual_functions += static_cast<std::int64_t>(t.slots.size());
        if (t.com) ++com;
        constructors.insert(t.installed_by.begin(), t.installed_by.end());
    }
    for (const MessageMap& m : model.maps) {
        entries += static_cast<std::int64_t>(m.entries.size());
        if (!m.class_tables.empty()) ++tied;
    }
    for (const CxxTable& t : eh.cxx) {
        tries += static_cast<std::int64_t>(t.tries.size());
        for (const CxxTry& b : t.tries) catches += static_cast<std::int64_t>(b.catches.size());
    }
    for (const ExceptionFrame& fr : eh.frames) {
        if (fr.scopes && !fr.scopes->empty()) ++scoped;
        if (fr.scopes) scopes += static_cast<std::int64_t>(fr.scopes->size());
    }
    auto put = [&](const char* key, std::int64_t value) { a.counts.insert_or_assign(key, value); };
    put("virtual_tables", static_cast<std::int64_t>(model.vtables.size()));
    put("virtual_functions", virtual_functions);
    put("com_shaped_tables", com);
    put("constructors_or_destructors", static_cast<std::int64_t>(constructors.size()));
    put("classes_named_by_the_binary", named);
    put("classes_with_stated_bases", stated);
    put("classes_with_inferred_base", inferred);
    put("classes_with_instance_layout", layouts);
    put("mfc_class_records", static_cast<std::int64_t>(model.runtime.size()));
    put("message_maps", static_cast<std::int64_t>(model.maps.size()));
    put("message_map_entries", entries);
    put("message_maps_tied_to_a_class", tied);
    put("this_in_ecx_functions", thiscall);
    put("cxx_exception_tables", static_cast<std::int64_t>(eh.cxx.size()));
    put("try_blocks", tries);
    put("catch_handlers", catches);
    put("exception_frames", static_cast<std::int64_t>(eh.frames.size()));
    put("frames_with_scope_table", scoped);
    put("except_or_finally_scopes", scopes);
    for (std::size_t q = 0; q < std::min<std::size_t>(10, tree.size()); ++q) {
        const Class& c = tree[q];
        std::string bases;
        if (!c.bases.empty()) {
            bases = " : ";
            for (std::size_t i = 0; i < std::min<std::size_t>(3, c.bases.size()); ++i) bases += (i != 0 ? ", " : "") + c.bases[i];
        }
        const std::string size = (c.size && *c.size != 0) ? std::to_string(*c.size) + " bytes" : "size unknown";
        const std::string evidence = !c.evidence.empty() ? "  [" + c.evidence[0] + "]" : "";
        a.text.push_back("class " + c.name + bases + "  table " + cl_hex(c.vtable) + ", " + std::to_string(c.slots) + " virtual, " + size + evidence);
    }
    for (std::size_t q = 0; q < std::min<std::size_t>(3, model.maps.size()); ++q) {
        const MessageMap& m = model.maps[q];
        const std::optional<std::string> name = class_name_for(m);
        a.text.push_back("message map " + cl_hex(m.at) + (name ? " of " + *name : std::string()) + ":");
        for (std::size_t i = 0; i < std::min<std::size_t>(8, m.entries.size()); ++i) a.text.push_back("    " + macro_of(image, m.entries[i]));
    }
    return a;
}

}  // namespace cb::analyze::classes
