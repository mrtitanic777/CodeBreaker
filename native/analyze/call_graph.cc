// call_graph.cc -- who calls whom, what refers to what, what the exports reach (was the core of
// codebreaker/analyze/call_graph.py, _build). The answer is JSON; analyze/call_graph.py makes
// its Graph from it.
#include "call_graph.h"

#include <algorithm>
#include <cstring>
#include <map>
#include <unordered_map>

#include "json.h"
#include "pyset.h"

namespace cba {

static const char *READS = "cmp test push bt call jmp lcall ljmp fld fild fcomp fcom fadd fsub fmul fdiv ficom "
                           "ficomp bound div idiv mul";
static const char *PURE_WRITES = "mov pop fstp fst fistp fist lea movzx movsx sidt sgdt fstcw fnstcw fnstsw fstsw";

static bool word_in(const std::string &s, const char *list) {
    const char *p = list;
    while (*p) {
        const char *q = p;
        while (*q && *q != ' ') q++;
        if ((size_t)(q - p) == s.size() && s.compare(0, s.size(), p, q - p) == 0) return true;
        p = *q ? q + 1 : q;
    }
    return false;
}

struct Ref {
    int64_t fn, ins, va;
    const char *how;
};

// (address, how) for every operand of one instruction that refers to a place in the module
static void references(Image &img, const Ins &ins, std::vector<std::pair<int64_t, const char *>> &out) {
    bool has_known = img.has_pointers;
    PySet known;
    if (has_known) {
        for (int64_t a = ins.addr + 1; a < ins.addr + ins.size - (img.bits == 32 ? 3 : 1); a++) {
            auto p = img.pointers.find(a);
            if (p != img.pointers.end() && p->second.has_target) known.add(p->second.target);
        }
    }
    if (has_known && known.empty() && img.bits == 32) return;
    for (size_t k = 0; k < ins.ops.size(); k++) {
        const Op &op = ins.ops[k];
        if (op.kind == 'i') {
            if (ins.flow != F_NONE || ins.has_target) continue;
            if (img.bits == 32) {
                if (has_known ? known.contains(op.value) : (op.size == 4 && img.is_data(op.value)))
                    out.emplace_back(op.value, "a");
            }
        } else if (op.kind == 'm' && op.base.empty()) {
            int64_t va;
            if (img.bits == 32) {
                va = op.value;
                if (!(has_known ? known.contains(va) : img.span(va) != nullptr)) continue;
            } else {
                if (!(op.seg.empty() || op.seg == "ds") || !img.data_segment || !op.index.empty()) continue;
                va = img.data_segment + (op.value & 0xFFFF);
                if (!img.span(va)) continue;
            }
            if (ins.mn == "lea") out.emplace_back(va, "a");
            else if (k || word_in(ins.mn, READS)) out.emplace_back(va, "r");
            else
                out.emplace_back(va, (word_in(ins.mn, PURE_WRITES) || ins.mn.compare(0, 3, "set") == 0) ? "w" : "rw");
        } else if (op.kind == 'm' && img.bits == 32) {           // [reg + ADDRESS]: a table indexed from a register
            int64_t va = op.value & 0xFFFFFFFF;
            if (has_known ? known.contains(va) : (va >= 0x10000 && img.span(va) != nullptr))
                out.emplace_back(va, ins.mn == "lea" ? "a" : (k || word_in(ins.mn, READS)) ? "r" : "rw");
        }
    }
    if (img.bits == 16 && has_known && !known.empty()) {         // a far address or a segment written into the code
        for (int64_t va : known.items())
            if (ins.flow == F_NONE || !ins.has_target) out.emplace_back(va, "a");
    }
}

static bool destination(const Image &img, const Ins &ins, int64_t &dest) {
    if (ins.has_target) {
        dest = ins.target;
        return true;
    }
    if (!ins.ops.empty() && ins.ops[0].kind == 'm' && ins.ops[0].base.empty() && ins.ops[0].index.empty() &&
        img.imports.count(ins.ops[0].value)) {
        dest = ins.ops[0].value;
        return true;
    }
    return false;
}

// Tarjan without recursion, over the callee sets in Python's iteration order -> components, callees first
static std::vector<std::vector<int64_t>> strongly_connected(const std::vector<int64_t> &nodes,
                                                            const std::unordered_map<int64_t, std::vector<int64_t>> &edges) {
    std::unordered_map<int64_t, int64_t> index, low;
    std::unordered_map<int64_t, bool> on;
    std::vector<int64_t> stack;
    std::vector<std::vector<int64_t>> out;
    int64_t counter = 0;
    static const std::vector<int64_t> none;
    auto succ = [&](int64_t v) -> const std::vector<int64_t> & {
        auto it = edges.find(v);
        return it == edges.end() ? none : it->second;
    };
    for (int64_t root : nodes) {
        if (index.count(root)) continue;
        std::vector<std::pair<int64_t, size_t>> work{{root, 0}};
        index[root] = low[root] = counter++;
        stack.push_back(root);
        on[root] = true;
        while (!work.empty()) {
            int64_t v = work.back().first;
            const std::vector<int64_t> &ws = succ(v);
            bool descended = false;
            while (work.back().second < ws.size()) {
                int64_t w = ws[work.back().second++];
                if (!index.count(w)) {
                    index[w] = low[w] = counter++;
                    stack.push_back(w);
                    on[w] = true;
                    work.emplace_back(w, 0);
                    descended = true;
                    break;
                }
                if (on[w]) low[v] = std::min(low[v], index[w]);
            }
            if (descended) continue;
            work.pop_back();
            if (!work.empty()) {
                int64_t u = work.back().first;
                low[u] = std::min(low[u], low[v]);
            }
            if (low[v] == index[v]) {
                std::vector<int64_t> comp;
                while (true) {
                    int64_t w = stack.back();
                    stack.pop_back();
                    on[w] = false;
                    comp.push_back(w);
                    if (w == v) break;
                }
                out.push_back(std::move(comp));
            }
        }
    }
    return out;
}

std::string call_graph(Image &img) {
    std::unordered_set<int64_t> funcs(img.functions.begin(), img.functions.end());
    std::unordered_map<int64_t, PySet> direct;
    std::unordered_map<int64_t, std::vector<int64_t>> direct_items;
    // insertion-ordered maps: key order of first insertion
    std::vector<int64_t> callers_order, users_order, taken_order;
    std::unordered_map<int64_t, std::set<int64_t>> callers, users;
    std::unordered_map<int64_t, std::vector<std::string>> taken;   // each entry already JSON
    std::vector<Ref> refs;
    std::string calls_json = "[";
    bool first_fn = true;
    std::vector<std::pair<int64_t, const char *>> found;
    auto take = [&](int64_t va, const std::string &entry) {
        auto it = taken.find(va);
        if (it == taken.end()) {
            taken_order.push_back(va);
            it = taken.emplace(va, std::vector<std::string>()).first;
        }
        it->second.push_back(entry);
    };
    for (int64_t start : std::vector<int64_t>(img.functions)) {
        const Function &fn = img.function(start);
        PySet &edges = direct[fn.start];
        if (!first_fn) calls_json += ',';
        first_fn = false;
        calls_json += '[';
        put_int(calls_json, fn.start);
        calls_json += ",[";
        bool first_call = true;
        std::vector<Image::Call> &mine = img.calls[fn.start];
        mine.clear();
        auto call = [&](int64_t site, const char *kind, bool has, int64_t dest) {
            mine.push_back({site, kind, has, dest});
            if (!first_call) calls_json += ',';
            first_call = false;
            calls_json += '[';
            put_int(calls_json, site);
            calls_json += ",\"";
            calls_json += kind;
            calls_json += "\",";
            if (has) put_int(calls_json, dest);
            else calls_json += "null";
            calls_json += ']';
        };
        for (const Ins &ins : fn.insns) {
            if (ins.flow == F_CALL || ins.flow == F_JMP) {
                int64_t dest = 0;
                bool has = destination(img, ins, dest);
                if (has && img.imports.count(dest)) {
                    call(ins.addr, ins.flow == F_CALL ? "import" : "tail", true, dest);
                    if (!users.count(dest)) users_order.push_back(dest);
                    users[dest].insert(fn.start);
                } else if (!has) {
                    bool table_site = false;
                    for (const Table &t : fn.tables)
                        if (t.site == ins.addr) table_site = true;
                    if (ins.flow == F_CALL || fn.tables.empty() || !table_site) call(ins.addr, "indirect", false, 0);
                } else if (funcs.count(dest) &&
                           (ins.flow == F_CALL || (dest != fn.start && !(fn.start < dest && dest < fn.limit)))) {
                    call(ins.addr, ins.flow == F_CALL ? "call" : "tail", true, dest);
                    edges.add(dest);
                    if (!callers.count(dest)) callers_order.push_back(dest);
                    callers[dest].insert(fn.start);
                }
            }
            found.clear();
            references(img, ins, found);
            for (auto &r : found) {
                if (img.imports.count(r.first)) continue;
                refs.push_back({fn.start, ins.addr, r.first, r.second});
                if (funcs.count(r.first) && !std::strcmp(r.second, "a")) {
                    std::string e = "[\"code\",";
                    put_int(e, fn.start);
                    e += ',';
                    put_int(e, ins.addr);
                    e += ']';
                    take(r.first, e);
                }
            }
        }
        calls_json += "]]";
    }
    calls_json += ']';
    img.graph_built = true;
    img.refs.clear();
    for (const Ref &r : refs) img.refs.push_back({r.fn, r.ins, r.va, r.how});
    // function addresses held in data
    std::set<int64_t> data_taken;
    if (img.has_pointers) {
        for (auto &p : img.pointer_list) {
            int64_t site = p.first;
            const Pointer &ptr = img.pointers[site];
            int64_t own;
            if (ptr.has_target && funcs.count(ptr.target) && (!img.is_code(site) || !img.owner(site, own))) {
                std::string e = "[\"data\",null,";
                put_int(e, site);
                e += ']';
                take(ptr.target, e);
                data_taken.insert(ptr.target);
            }
        }
    } else {
        for (const Span &s : img.spans) {
            if (s.exec) continue;
            for (int64_t off = 0; off < s.init - 3; off += 4) {
                const unsigned char *b = (const unsigned char *)s.data.data() + off;
                int64_t v = (int64_t)((uint32_t)b[0] | ((uint32_t)b[1] << 8) | ((uint32_t)b[2] << 16) | ((uint32_t)b[3] << 24));
                if (funcs.count(v)) {
                    std::string e = "[\"data\",null,";
                    put_int(e, s.va + off);
                    e += ']';
                    take(v, e);
                    data_taken.insert(v);
                }
            }
        }
    }
    for (auto &kv : direct) direct_items[kv.first] = kv.second.items();
    auto comps = strongly_connected(img.functions, direct_items);
    // what the exports reach: calls, tail jumps, and addresses taken by reached code
    std::unordered_map<int64_t, std::vector<int64_t>> takes;
    for (const Ref &r : refs)
        if (!std::strcmp(r.how, "a") && funcs.count(r.va)) takes[r.fn].push_back(r.va);
    auto reach = [&](const std::vector<int64_t> &roots) {
        std::set<int64_t> seen;
        std::vector<int64_t> work;
        for (int64_t r : roots)
            if (funcs.count(r) && seen.insert(r).second) work.push_back(r);
        while (!work.empty()) {
            int64_t f = work.back();
            work.pop_back();
            auto d = direct_items.find(f);
            if (d != direct_items.end())
                for (int64_t t : d->second)
                    if (seen.insert(t).second) work.push_back(t);
            auto k = takes.find(f);
            if (k != takes.end())
                for (int64_t t : k->second)
                    if (seen.insert(t).second) work.push_back(t);
        }
        return seen;
    };
    std::set<int64_t> reachable = reach(img.roots);
    std::vector<int64_t> with_tables = img.roots;
    for (int64_t f : taken_order)
        if (data_taken.count(f)) with_tables.push_back(f);
    std::set<int64_t> reachable_tables = reach(with_tables);
    std::unordered_set<int64_t> roots(img.roots.begin(), img.roots.end());

    std::string o = "{\"calls\":";
    o += calls_json;
    auto put_list = [&](const std::vector<int64_t> &v) {
        o += '[';
        for (size_t k = 0; k < v.size(); k++) {
            if (k) o += ',';
            put_int(o, v[k]);
        }
        o += ']';
    };
    auto put_set = [&](const std::set<int64_t> &v) { put_list(std::vector<int64_t>(v.begin(), v.end())); };
    auto put_keyed = [&](const std::vector<int64_t> &order, std::unordered_map<int64_t, std::set<int64_t>> &m) {
        o += '[';
        for (size_t k = 0; k < order.size(); k++) {
            if (k) o += ',';
            o += '[';
            put_int(o, order[k]);
            o += ',';
            put_set(m[order[k]]);
            o += ']';
        }
        o += ']';
    };
    o += ",\"callers\":";
    put_keyed(callers_order, callers);
    o += ",\"users\":";
    put_keyed(users_order, users);
    o += ",\"refs\":[";
    for (size_t k = 0; k < refs.size(); k++) {
        if (k) o += ',';
        o += '[';
        put_int(o, refs[k].fn), o += ',', put_int(o, refs[k].ins), o += ',', put_int(o, refs[k].va);
        o += ",\"";
        o += refs[k].how;
        o += "\"]";
    }
    o += "],\"taken\":[";
    for (size_t k = 0; k < taken_order.size(); k++) {
        if (k) o += ',';
        o += '[';
        put_int(o, taken_order[k]);
        o += ",[";
        auto &es = taken[taken_order[k]];
        for (size_t j = 0; j < es.size(); j++) {
            if (j) o += ',';
            o += es[j];
        }
        o += "]]";
    }
    o += "],\"order\":[";
    bool first = true;
    for (auto &c : comps)
        for (int64_t f : c) {
            if (!first) o += ',';
            first = false;
            put_int(o, f);
        }
    o += "],\"cycles\":[";
    first = true;
    for (auto &c : comps) {
        bool self_loop = direct[c[0]].contains(c[0]);
        if (c.size() > 1 || self_loop) {
            if (!first) o += ',';
            first = false;
            std::vector<int64_t> s(c);
            std::sort(s.begin(), s.end());
            put_list(s);
        }
    }
    o += "],\"reachable\":";
    put_set(reachable);
    o += ",\"reachable_with_tables\":";
    put_set(reachable_tables);
    o += ",\"unreferenced\":[";
    first = true;
    for (int64_t f : img.functions)
        if (!callers.count(f) && !taken.count(f) && !roots.count(f)) {
            if (!first) o += ',';
            first = false;
            put_int(o, f);
        }
    o += "]}";
    return o;
}

}  // namespace cba
