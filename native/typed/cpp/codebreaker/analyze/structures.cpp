// codebreaker/analyze/structures.cpp -- typed C++ from codebreaker.analyze.structures (see the header).
//
// Structures recovered from how pointers are used: a pointer is followed from where it comes into view through register
// copies, `lea` and the frame slots, every access `[pointer + offset]` is a field, and pointers one handed on as another
// are unified. Roots are ordered and compared by their Python repr (the Python sorts by repr()).
#include "codebreaker/analyze/structures.hpp"

#include <algorithm>
#include <map>
#include <set>

#include "codebreaker/analyze/call_graph.hpp"
#include "codebreaker/analyze/conventions.hpp"
#include "codebreaker/analyze/image_bridge.hpp"

namespace codebreaker::analyze::structures {

using image::Function;
using image::Image;
using image::Op;

namespace {

// _ALLOCATORS: the number of the argument holding the size, or -1 for None
const std::map<std::string, int64_t>& allocators() {
    static const std::map<std::string, int64_t> t = {{"LocalAlloc", 1}, {"GlobalAlloc", 1}, {"HeapAlloc", 2},     {"malloc", 0},
                                                      {"calloc", -1},    {"??2@YAPAXI@Z", 0}, {"LOCALALLOC", 0},    {"GLOBALALLOC", -1},
                                                      {"CoTaskMemAlloc", 0}, {"SHAlloc", 0},  {"VirtualAlloc", 1}};
    return t;
}

std::string hex_lower(int64_t v) {
    char b[32];
    std::snprintf(b, sizeof b, "%llx", static_cast<unsigned long long>(v < 0 ? -v : v));
    return (v < 0 ? "-" : "") + std::string(b);
}

Root make_root(const char* kind, int64_t addr, int64_t index = 0) {
    Root r;
    r.kind = py::str(kind);
    r.addr = addr;
    r.index = index;
    return r;
}

std::string repr(const Root& r) {
    if (r.kind == "arg") return "('arg', " + std::to_string(r.addr) + ", " + std::to_string(r.index) + ")";
    if (r.kind == "field") return "('field', " + repr(*r.inner) + ", " + std::to_string(r.index) + ")";
    return "('" + r.kind.utf8() + "', " + std::to_string(r.addr) + ")";
}

struct Held {   // (root, delta)
    Root root;
    int64_t delta = 0;
};
using Hold = std::optional<Held>;

// a field while it is being collected
struct RawField {
    int64_t size = 0, reads = 0, writes = 0, element = 0;
    std::set<std::string> notes;
    std::set<int64_t> stores, users;
};
struct RootFields {
    Root root;
    std::vector<int64_t> order;
    std::map<int64_t, RawField> fields;
    bool empty() const { return fields.empty(); }
};
struct Collected {
    std::vector<std::string> order;                          // roots in the order fields first saw them
    std::map<std::string, RootFields> by_root;               // by repr
    std::vector<std::pair<Root, Root>> edges;
    std::map<std::string, int64_t> sizes;
    std::map<std::string, std::set<std::string>> uses;
    std::vector<std::pair<Root, int64_t>> passed;

    RootFields& root(const Root& r) {
        std::string k = repr(r);
        auto it = by_root.find(k);
        if (it == by_root.end()) {
            order.push_back(k);
            it = by_root.emplace(k, RootFields()).first;
            it->second.root = r;
        }
        return it->second;
    }
    RawField& field(const Root& r, int64_t off, int64_t size) {
        RootFields& rf = root(r);
        auto it = rf.fields.find(off);
        if (it == rf.fields.end()) {
            rf.order.push_back(off);
            it = rf.fields.emplace(off, RawField()).first;
        }
        it->second.size = std::max(it->second.size, size);
        return it->second;
    }
};

bool in_list(const py::list<py::str>& l, const char* s) {
    for (const py::str& x : l.vec()) if (x == s) return true;
    return false;
}

std::string last_after_bang(const py::str& s) {
    const std::u32string& u = s.u32();
    size_t p = u.rfind(U'!');
    return py::str(p == std::u32string::npos ? u : u.substr(p + 1)).utf8();
}

bool flow_is(const image::Ins& i, const char* f) { return i.flow && *i.flow == f; }

// Follow pointers through one function. Adds to `c` and to `edges`.
void scan(Image& img, const Function& fn, const conventions::Fact& fact, call_graph::Graph& graph, Collected& c) {
    const int64_t word = img.bits == 32 ? 4 : 2;
    const std::string conv = fact.convention ? fact.convention->utf8() : std::string();
    const bool far = conv.size() >= 3 && conv.compare(conv.size() - 3, 3, "far") == 0;
    const int64_t first = img.bits == 32 ? 2 * word : (far ? 6 : 4);
    const bool framed = fact.frame;
    std::set<int64_t> labels;
    for (const image::InsP& i : fn.insns.vec()) if ((flow_is(*i, "jcc") || flow_is(*i, "jmp")) && i->target) labels.insert(*i->target);
    std::map<std::string, Held> holds;
    std::map<std::pair<std::string, int64_t>, Held> slots;
    std::vector<Hold> pushed;
    if (in_list(fact.reg_args, "ecx") && img.bits == 32) {
        Root t = make_root("this", fn.start);
        holds["ecx"] = Held{t, 0};
    }
    std::map<int64_t, std::pair<std::string, std::optional<int64_t>>> calls;
    for (const auto& cl : graph.calls.at(fn.start).vec()) calls[cl.site] = {cl.kind.utf8(), cl.destination};
    std::optional<int64_t> depth = framed ? std::nullopt : std::optional<int64_t>(0);
    std::map<std::string, std::pair<Root, int64_t>> pending;   // register -> (root, offset) of the field it was loaded from

    auto fam = [](const py::str& r) { return image::family(r).utf8(); };
    auto bound = [&](const Op& op) -> const Held* {
        if (!op.base) return nullptr;
        auto it = holds.find(fam(*op.base));
        return it == holds.end() ? nullptr : &it->second;
    };
    // which argument slot a memory operand is, or none
    auto argument = [&](const Op& op) -> std::optional<int64_t> {
        if (!op.base || op.index) return std::nullopt;
        std::string f = fam(*op.base);
        if (framed && f == "ebp" && op.disp >= first) return (op.disp - first) / word;
        if (depth && f == "esp" && img.bits == 32 && op.disp >= *depth + word) return (op.disp - *depth - word) / word;
        return std::nullopt;
    };
    auto flo_div = [](int64_t a, int64_t b) { return a / b; };   // (the operands are non-negative here)
    (void)flo_div;

    for (int64_t i = 0; i < fn.insns.size(); i++) {
        const image::Ins& ins = *fn.insns[i];
        if (labels.count(ins.addr)) {
            for (const char* r : {"eax", "ecx", "edx"}) {
                if (!(std::string(r) == "ecx" && i == 0)) holds.erase(r);
            }
            pending.clear();
            if (depth && *depth) depth = std::nullopt;
        }
        const std::string mn = ins.mn.utf8();
        const py::list<Op>& ops = ins.ops;
        // every access through a followed pointer is a field
        for (int64_t k = 0; k < ops.size(); k++) {
            const Op& op = ops[k];
            if (op.kind != "m") continue;
            if (op.base && pending.count(fam(*op.base)) && mn != "lea") {   // a loaded field used as an address
                auto pe = pending.at(fam(*op.base));
                c.field(pe.first, pe.second, word).notes.insert("pointer");
                Root child;
                child.kind = py::str("field");
                child.inner = std::make_shared<Root>(pe.first);
                child.index = pe.second;
                holds[fam(*op.base)] = Held{child, 0};
                pending.erase(fam(*op.base));
            }
            const Held* bp = bound(op);
            if (!bp) continue;
            Held b = *bp;
            int64_t off = b.delta + op.disp;
            if (!(0 <= off && off < MAX_OFFSET)) continue;
            if (mn == "lea") {
                if (off) c.field(b.root, off, 0).notes.insert("address taken");
                continue;
            }
            RawField& f = c.field(b.root, off, op.size);
            f.users.insert(fn.start);
            bool write = k == 0 && !in_list(call_graph::READS, mn.c_str());
            (write ? f.writes : f.reads) += 1;
            if (write && !in_list(call_graph::PURE_WRITES, mn.c_str())) f.reads += 1;
            if (op.index) {
                f.notes.insert("array");
                f.element = std::max(f.element, op.scale > 1 ? op.scale : op.size);
            }
            if (mn[0] == 'f' && mn != "fwait") f.notes.insert("float");
            if (flow_is(ins, "call")) f.notes.insert("called");
            if (write && mn == "mov" && ops.size() == 2 && ops[1].kind == "i" && ops[1].size == word) f.stores.insert(ops[1].value);
            if (write && mn == "mov" && ops.size() == 2 && ops[1].kind == "r" && (op.size == word || op.size == 4)) {
                auto src = holds.find(fam(ops[1].name));   // a followed pointer stored in a field
                if (src != holds.end() && src->second.delta == 0) {
                    Root child;
                    child.kind = py::str("field");
                    child.inner = std::make_shared<Root>(b.root);
                    child.index = off;
                    c.edges.emplace_back(src->second.root, child);
                }
            }
        }
        // calls: what is handed on, and what comes back
        if (flow_is(ins, "call")) {
            std::string kind = "indirect";
            std::optional<int64_t> dest;
            auto cit = calls.find(ins.addr);
            if (cit != calls.end()) { kind = cit->second.first; dest = cit->second.second; }
            size_t from = pushed.size() > 8 ? pushed.size() - 8 : 0;
            std::vector<Hold> tail(pushed.begin() + from, pushed.end());
            std::reverse(tail.begin(), tail.end());
            if (kind == "call" && dest) {
                for (size_t j = 0; j < tail.size(); j++) {
                    if (tail[j] && tail[j]->delta == 0) c.edges.emplace_back(tail[j]->root, make_root("arg", *dest, static_cast<int64_t>(j)));
                }
                auto e = holds.find("ecx");
                if (e != holds.end() && e->second.delta == 0 && img.bits == 32) c.passed.emplace_back(e->second.root, *dest);
            } else if (kind == "import") {
                std::string api = last_after_bang(img.imports.at(*dest));
                for (size_t j = 0; j < tail.size(); j++) {
                    if (tail[j] && tail[j]->delta == 0) {
                        c.root(tail[j]->root);
                        c.uses[repr(tail[j]->root)].insert(api + " argument " + std::to_string(j));
                    }
                }
            }
            for (const char* r : {"eax", "ecx", "edx"}) {
                holds.erase(r);
                pending.erase(r);
            }
            if (kind == "import") {
                std::string api = last_after_bang(img.imports.at(*dest));
                std::string stem = api;
                if (api.size() >= 1 && (api.back() == 'A' || api.back() == 'W') && allocators().count(api.substr(0, api.size() - 1))) stem = api.substr(0, api.size() - 1);
                auto al = allocators().find(stem);
                if (al != allocators().end() && al->second >= 0) {
                    py::list<Op> args = image::arguments(fn, i, al->second + 1);
                    int64_t k = al->second;
                    if (k < args.size() && args[k].kind == "i" && 0 < args[k].value && args[k].value < MAX_OFFSET) {
                        Root root = make_root("alloc", ins.addr);
                        holds["eax"] = Held{root, 0};
                        c.sizes[repr(root)] = args[k].value;
                        c.root(root);
                    }
                }
            }
            if (kind == "call" && dest) holds["eax"] = Held{make_root("returns", *dest), 0};
            else if (kind == "import" && !holds.count("eax")) holds["eax"] = Held{make_root("result", ins.addr), 0};
            pushed.clear();
            continue;
        }
        if (flow_is(ins, "ret")) {
            auto b = holds.find("eax");
            if (b != holds.end() && b->second.delta == 0 && !(b->second.root == make_root("returns", fn.start))) {
                c.edges.emplace_back(b->second.root, make_root("returns", fn.start));
            }
            continue;
        }
        if (mn == "push") {
            Hold b;
            if (!ops.empty() && ops[0].kind == "r") {
                auto h = holds.find(fam(ops[0].name));
                if (h != holds.end()) b = h->second;
            } else if (!ops.empty() && ops[0].kind == "m") {
                auto a = argument(ops[0]);
                if (a) {
                    b = Held{make_root("arg", fn.start, *a), 0};
                } else {
                    auto s = slots.find({ops[0].base ? fam(*ops[0].base) : std::string(), ops[0].disp});
                    if (s != slots.end()) b = s->second;
                }
            }
            pushed.push_back(b);
            if (depth) *depth += word;
            continue;
        }
        if (depth) {
            auto d = conventions::stack_delta(ins);
            if (!d) depth = std::nullopt;
            else *depth += *d;
        }
        if (ops.empty() || !(ops[0].kind == "r" || ops[0].kind == "m")) continue;
        // where pointers go
        if (ops[0].kind == "r") {
            std::string dst = fam(ops[0].name);
            Hold nw;
            if ((mn == "mov" || mn == "les" || mn == "lds") && ops.size() == 2) {
                const Op& src = ops[1];
                if (src.kind == "r") {
                    auto h = holds.find(fam(src.name));
                    if (h != holds.end()) nw = h->second;
                    if (!nw && pending.count(fam(src.name)) && dst != fam(src.name)) {
                        pending[dst] = pending.at(fam(src.name));
                        holds.erase(dst);
                        continue;
                    }
                } else if (src.kind == "m") {
                    auto a = argument(src);
                    if (a) {
                        nw = Held{make_root("arg", fn.start, *a), 0};
                    } else if (!src.base && !src.index) {
                        int64_t va = img.bits == 32 ? src.disp : img.data_segment + (src.disp & 0xFFFF);
                        if (img.span(va) != nullptr && !img.imports.contains(va) && (src.size == word || src.size == 4)) nw = Held{make_root("global", va), 0};
                    } else if (src.base && (fam(*src.base) == "ebp" || fam(*src.base) == "esp") && slots.count({fam(*src.base), src.disp})) {
                        nw = slots.at({fam(*src.base), src.disp});
                    } else if (bound(src) != nullptr && (src.size == word || src.size == 4) && !src.index) {
                        Held b = *bound(src);
                        if (0 <= b.delta + src.disp && b.delta + src.disp < MAX_OFFSET) {
                            pending[dst] = {b.root, b.delta + src.disp};
                            holds.erase(dst);
                            continue;
                        }
                    }
                }
            } else if (mn == "lea" && ops.size() == 2 && ops[1].kind == "m" && !ops[1].index && bound(ops[1]) != nullptr) {
                Held b = *bound(ops[1]);
                nw = Held{b.root, b.delta + ops[1].disp};
            }
            if (ins.wr.contains(py::str(dst)) || nw) {
                pending.erase(dst);
                if (nw) holds[dst] = *nw;
                else holds.erase(dst);
            }
            for (const py::str& r : ins.wr) {
                std::string rs = r.utf8();
                if (rs != dst && holds.count(rs) && rs != "esp" && rs != "ebp") holds.erase(rs);
            }
        } else if (mn == "mov" && ops[0].base && (fam(*ops[0].base) == "ebp" || fam(*ops[0].base) == "esp") && !ops[0].index && ops.size() == 2) {
            std::pair<std::string, int64_t> key = {fam(*ops[0].base), ops[0].disp};
            Hold b;
            if (ops[1].kind == "r") {
                auto h = holds.find(fam(ops[1].name));
                if (h != holds.end()) b = h->second;
            }
            if (b) slots[key] = *b;
            else slots.erase(key);
        } else if (mn == "mov" && !ops[0].base && !ops[0].index && ops.size() == 2 && ops[1].kind == "r") {
            auto h = holds.find(fam(ops[1].name));   // a followed pointer stored in a global
            int64_t va = img.bits == 32 ? ops[0].disp : img.data_segment + (ops[0].disp & 0xFFFF);
            if (h != holds.end() && h->second.delta == 0 && img.span(va) != nullptr && (!ops[0].seg || *ops[0].seg == "ds")) {
                c.edges.emplace_back(h->second.root, make_root("global", va));
            }
        } else {
            for (const py::str& r : ins.wr) {
                std::string rs = r.utf8();
                if (holds.count(rs) && rs != "esp" && rs != "ebp") holds.erase(rs);
                pending.erase(rs);
            }
        }
    }
}

}  // namespace

// ---- Root ---------------------------------------------------------------------------------------------------------------
bool Root::operator==(const Root& o) const { return repr(*this) == repr(o); }
bool Root::operator<(const Root& o) const { return repr(*this) < repr(o); }

py::str _describe(const Image& img, const Root& root) {
    if (root.kind == "arg") return py::str("argument " + std::to_string(root.index) + " of " + img.label(root.addr).utf8());
    if (root.kind == "this") return py::str("this of " + img.label(root.addr).utf8());
    if (root.kind == "global") return py::str("what global " + hex_lower(root.addr) + " points at");
    if (root.kind == "alloc") return py::str("allocated at " + hex_lower(root.addr));
    if (root.kind == "returns") return py::str("what " + img.label(root.addr).utf8() + " returns");
    if (root.kind == "result") return py::str("result of the call at " + hex_lower(root.addr));
    return py::str("field +" + hex_lower(root.index) + " of (" + _describe(img, *root.inner).utf8() + ")");
}

std::shared_ptr<py::list<Structure>> _recover(Image& img) {
    auto graph = bridge::graph(img);
    auto facts = conventions::functions(img);
    Collected c;
    for (const image::FunctionP& fn : img.each().vec()) {
        if (!fn->insns.empty()) scan(img, *fn, facts->at(fn->start), *graph, c);
    }
    for (const auto& pd : c.passed) {   // `this` handed on in ecx
        if (in_list(facts->at(pd.second).reg_args, "ecx")) c.edges.emplace_back(pd.first, make_root("this", pd.second));
    }
    // Which pointers are one structure. A pointer handed on is the pointer received -- unless the receiver takes pointers
    // from many places, or leads to no field at all, or the two are known to differ in size.
    std::map<std::string, std::pair<Root, Root>> uniq;   // by the repr of the pair: sorted(set(...), key=repr)
    for (const auto& e : c.edges) {
        if (e.first == e.second) continue;
        uniq.emplace("(" + repr(e.first) + ", " + repr(e.second) + ")", e);
    }
    std::vector<std::pair<Root, Root>> edges;
    for (auto& kv : uniq) edges.push_back(kv.second);
    std::map<std::string, std::set<std::string>> sources;
    for (const auto& e : edges) sources[repr(e.second)].insert(repr(e.first));
    std::set<std::string> useful;
    for (const std::string& k : c.order) if (!c.by_root.at(k).empty()) useful.insert(k);
    for (bool grew = true; grew;) {
        grew = false;
        for (const auto& e : edges) {
            std::string a = repr(e.first), b = repr(e.second);
            if (useful.count(b) && !useful.count(a)) { useful.insert(a); grew = true; }
        }
    }
    std::map<std::string, std::string> parent;
    auto find = [&](const std::string& x) {
        std::string p = parent.emplace(x, x).first->second;
        while (p != parent[p]) {
            parent[p] = parent[parent[p]];
            p = parent[p];
        }
        parent[x] = p;
        return p;
    };
    struct Extent { std::optional<int64_t> exact; int64_t end = 0; };
    std::map<std::string, Extent> extent;   // set -> [exact size or None, end of its last field]
    for (const std::string& k : c.order) {
        int64_t end = 0;
        for (auto& of : c.by_root.at(k).fields) end = std::max(end, of.first + of.second.size);
        auto sz = c.sizes.find(k);
        extent[k] = Extent{sz == c.sizes.end() ? std::nullopt : std::optional<int64_t>(sz->second), end};
    }
    for (const auto& e : edges) {
        std::string a = repr(e.first), b = repr(e.second);
        if (static_cast<int64_t>(sources[b].size()) > GENERIC || !useful.count(b) || !useful.count(a)) continue;
        std::string ra = find(a), rb = find(b);
        if (ra == rb) continue;
        Extent ea = extent.count(ra) ? extent[ra] : Extent(), eb = extent.count(rb) ? extent[rb] : Extent();
        bool ta = ea.exact && *ea.exact, tb = eb.exact && *eb.exact;
        if (ta && tb && *ea.exact != *eb.exact) continue;
        std::optional<int64_t> exact = ta ? ea.exact : eb.exact;
        if (exact && *exact && std::max(ea.end, eb.end) > *exact) continue;
        std::string keep = ra < rb ? ra : rb, gone = ra < rb ? rb : ra;
        parent[gone] = keep;
        extent[keep] = Extent{exact, std::max(ea.end, eb.end)};
    }
    std::vector<std::string> group_order;
    std::map<std::string, std::vector<std::string>> groups;
    for (const std::string& k : c.order) {
        std::string rep = find(k);
        if (!groups.count(rep)) group_order.push_back(rep);
        groups[rep].push_back(k);
    }
    std::vector<Structure> out;
    for (const std::string& rep : group_order) {
        const std::vector<std::string>& roots = groups[rep];
        std::vector<int64_t> merged_order;
        std::map<int64_t, RawField> merged;
        for (const std::string& r : roots) {
            const RootFields& rf = c.by_root.at(r);
            for (int64_t off : rf.order) {
                const RawField& f = rf.fields.at(off);
                if (!merged.count(off)) merged_order.push_back(off);
                RawField& m = merged[off];
                m.size = std::max(m.size, f.size);
                m.reads += f.reads;
                m.writes += f.writes;
                m.notes.insert(f.notes.begin(), f.notes.end());
                m.stores.insert(f.stores.begin(), f.stores.end());
                m.users.insert(f.users.begin(), f.users.end());
                m.element = std::max(m.element, f.element);
            }
        }
        std::vector<int64_t> keep_order;
        for (int64_t off : merged_order) if (merged[off].size || !merged[off].notes.empty()) keep_order.push_back(off);
        std::vector<int64_t> exact;
        for (const std::string& r : roots) if (c.sizes.count(r)) exact.push_back(c.sizes.at(r));
        if (keep_order.size() < 2 && !(!keep_order.empty() && !exact.empty())) continue;
        int64_t low = 0;
        for (int64_t off : keep_order) low = std::max(low, off + std::max<int64_t>(merged[off].size, 1));
        std::set<std::string> uses;
        for (const std::string& r : roots) if (c.uses.count(r)) uses.insert(c.uses.at(r).begin(), c.uses.at(r).end());
        Structure s;
        std::vector<std::string> sorted_roots = roots;
        std::sort(sorted_roots.begin(), sorted_roots.end());
        for (const std::string& r : sorted_roots) s.roots.append(c.by_root.at(r).root);
        std::set<int64_t> fns;
        for (int64_t off : keep_order) {
            const RawField& m = merged[off];
            Field f;
            f.size = m.size;
            f.reads = m.reads;
            f.writes = m.writes;
            for (const auto& n : m.notes) f.notes.add(py::str(n));
            for (int64_t v : m.stores) f.stores.add(v);
            for (int64_t v : m.users) { f.users.add(v); fns.insert(v); }
            f.element = m.element;
            s.fields.set(off, f);
        }
        s.exact = !exact.empty();
        s.size = exact.empty() ? low : *std::max_element(exact.begin(), exact.end());
        for (const std::string& u : uses) s.passed_to.append(py::str(u));
        for (int64_t f : fns) s.functions.append(f);
        out.push_back(s);
    }
    std::stable_sort(out.begin(), out.end(), [](const Structure& a, const Structure& b) {
        if (a.fields.size() != b.fields.size()) return a.fields.size() > b.fields.size();
        return repr(a.roots[0]) < repr(b.roots[0]);
    });
    auto res = std::make_shared<py::list<Structure>>();
    int64_t n = 0;
    for (Structure& s : out) {
        s.name = py::str("struct_" + std::to_string(++n));
        res->append(s);
    }
    return res;
}

std::shared_ptr<py::list<Structure>> recover(Image& img) {
    return img.memo<py::list<Structure>>(py::str("structures"), [](Image& i) { return _recover(i); });
}

py::str field_type(const Field& f, int64_t) {
    auto name_of = [](int64_t size, bool wide) -> std::string {
        switch (size) {
            case 1: return "unsigned char";
            case 2: return "unsigned short";
            case 4: return "unsigned long";
            case 8: return wide ? "" : "double";
            case 10: return wide ? "" : "long double";
            default: return "";
        }
    };
    const py::set<py::str>& notes = f.notes;
    auto has = [&](const char* n) { return notes.contains(py::str(n)); };
    std::string base = name_of(f.size, false);
    if (base.empty()) base = "unsigned char";
    if (has("called")) return py::str("function pointer");
    if (has("float")) return py::str(f.size == 4 ? "float" : "double");
    if (has("pointer")) return py::str("pointer");
    if (has("array")) {
        std::string e = name_of(f.element ? f.element : f.size, true);
        if (e.empty()) e = "unsigned char";
        return py::str(e + "[]");
    }
    if (has("address taken") && !f.size) return py::str("embedded object");
    return py::str(base);
}

py::str layout(const Image&, const Structure& s) {
    std::vector<std::string> lines;
    lines.push_back("struct " + s.name.utf8() + " {" + (s.exact ? "    /* " + std::to_string(s.size) + " bytes */" : std::string()));
    int64_t at = 0;
    std::vector<int64_t> offs = s.fields.keys();
    std::sort(offs.begin(), offs.end());
    for (int64_t off : offs) {
        const Field& f = s.fields.at(off);
        if (off > at) lines.push_back("    unsigned char gap_" + hex_lower(at) + "[" + std::to_string(off - at) + "];");
        if (off < at) continue;   // bytes of the field before it, accessed singly
        if (!f.size && f.notes.contains(py::str("address taken"))) {
            bool inside = false;
            for (int64_t o : offs) if (off < o && o < off + 4) inside = true;
            if (inside) continue;
        }
        std::string t = field_type(f).utf8();
        int64_t size = f.size ? f.size : 1;
        std::string name = "f_" + hex_lower(off);
        std::string decl;
        if (t == "function pointer") decl = "void (*" + name + ")()";
        else if (t == "pointer") decl = "void *" + name;
        else if (t == "embedded object") decl = "unsigned char " + name + "[1]";
        else if (t.size() >= 2 && t.compare(t.size() - 2, 2, "[]") == 0) decl = t.substr(0, t.size() - 2) + " " + name + "[1]";
        else decl = t + " " + name;
        std::string head = decl + ";";
        if (head.size() < 34) head += std::string(34 - head.size(), ' ');
        std::set<std::string> extra;
        for (const py::str& n : f.notes) {
            if (!(n == "pointer" || n == "called" || n == "float")) extra.insert(n.utf8());
        }
        std::string tail;
        if (!extra.empty()) {
            tail = ", ";
            bool first = true;
            for (const std::string& n : extra) { if (!first) tail += ", "; first = false; tail += n; }
        }
        lines.push_back("    " + head + " /* +" + hex_lower(off) + "  read " + std::to_string(f.reads) + ", written " + std::to_string(f.writes) + tail + " */");
        at = off + size;
    }
    lines.push_back("};");
    std::string out;
    for (size_t i = 0; i < lines.size(); i++) out += (i ? "\n" : "") + lines[i];
    return py::str(out);
}

Analysis analyse(Image& img) {
    auto found = recover(img);
    Analysis a;
    for (const Structure& s : found->vec()) {
        StructureFact f;
        f.name = s.name;
        f.size = s.size;
        f.exact_size = s.exact;
        std::vector<int64_t> offs = s.fields.keys();
        std::sort(offs.begin(), offs.end());
        for (int64_t o : offs) {
            const Field& fl = s.fields.at(o);
            FieldFact ff;
            ff.size = fl.size;
            ff.type = field_type(fl);
            ff.reads = fl.reads;
            ff.writes = fl.writes;
            std::vector<std::string> notes;
            for (const py::str& n : fl.notes) notes.push_back(n.utf8());
            std::sort(notes.begin(), notes.end());
            for (const std::string& n : notes) ff.notes.append(py::str(n));
            f.fields.set(py::str("+" + hex_lower(o)), ff);
        }
        for (int64_t i = 0; i < std::min<int64_t>(12, s.roots.size()); i++) f.seen_as.append(_describe(img, s.roots[i]));
        f.pointers_unified = s.roots.size();
        for (int64_t i = 0; i < std::min<int64_t>(40, s.functions.size()); i++) f.functions.append(img.label(s.functions[i]));
        for (int64_t i = 0; i < std::min<int64_t>(12, s.passed_to.size()); i++) f.passed_to.append(s.passed_to[i]);
        a.facts.append(f);
    }
    auto put = [&](const char* k, int64_t v) { a.counts.set(py::str(k), v); };
    int64_t nf = 0, exact = 0, ptr = 0, called = 0, arr = 0, thisn = 0, glob = 0, multi = 0, largest = 0;
    std::set<int64_t> touching;
    for (const Structure& s : found->vec()) {
        nf += s.fields.size();
        if (s.exact) exact++;
        for (const Field& f : s.fields.values()) {
            if (f.notes.contains(py::str("pointer"))) ptr++;
            if (f.notes.contains(py::str("called"))) called++;
            if (f.notes.contains(py::str("array"))) arr++;
        }
        bool t = false, g = false;
        for (const Root& r : s.roots.vec()) { if (r.kind == "this") t = true; if (r.kind == "global") g = true; }
        if (t) thisn++;
        if (g) glob++;
        if (s.functions.size() > 1) multi++;
        largest = std::max<int64_t>(largest, s.fields.size());
        for (int64_t f : s.functions.vec()) touching.insert(f);
    }
    put("structures", found->size());
    put("fields", nf);
    put("with_exact_size", exact);
    put("pointer_fields", ptr);
    put("function_pointer_fields", called);
    put("array_fields", arr);
    put("seen_through_this", thisn);
    put("seen_through_global_pointer", glob);
    put("unified_across_functions", multi);
    put("largest_fields", largest);
    put("functions_touching_a_structure", static_cast<int64_t>(touching.size()));
    for (int64_t q = 0; q < std::min<int64_t>(3, found->size()); q++) {
        const Structure& s = (*found)[q];
        std::string seen;
        for (int64_t i = 0; i < std::min<int64_t>(3, s.roots.size()); i++) seen += (i ? "; " : "") + _describe(img, s.roots[i]).utf8();
        a.text.append(py::str(s.name.utf8() + ": " + std::to_string(s.fields.size()) + " fields, " + std::to_string(s.size) + " bytes" +
                              (s.exact ? "" : " at least") + ", used by " + std::to_string(s.functions.size()) + " functions, seen as " + seen));
    }
    if (!found->empty()) {
        std::string lay = layout(img, (*found)[0]).utf8();
        size_t pos = 0;
        int n = 0;
        while (n < 24) {
            size_t nl = lay.find('\n', pos);
            a.text.append(py::str(lay.substr(pos, nl == std::string::npos ? std::string::npos : nl - pos)));
            n++;
            if (nl == std::string::npos) break;
            pos = nl + 1;
        }
    }
    return a;
}

}  // namespace codebreaker::analyze::structures

size_t std::hash<codebreaker::analyze::structures::Root>::operator()(const codebreaker::analyze::structures::Root& r) const {
    using codebreaker::analyze::structures::Root;
    size_t h = std::hash<std::string>()(r.kind.utf8());
    h ^= std::hash<int64_t>()(r.addr) + 0x9e3779b97f4a7c15ull + (h << 6) + (h >> 2);
    h ^= std::hash<int64_t>()(r.index) + 0x9e3779b97f4a7c15ull + (h << 6) + (h >> 2);
    if (r.inner) h ^= (*this)(*r.inner) + 0x9e3779b97f4a7c15ull + (h << 6) + (h >> 2);
    return h;
}
