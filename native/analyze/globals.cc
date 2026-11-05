// globals.cc -- the module's global variables, typed from how the code uses them, and the
// tables of pointers its data holds (was the core of codebreaker/analyze/globals.py: _variables,
// _type, _operand, _dereferenced, _pointer_tables, _slot_kind). Answers are JSON in the
// Python's order; a variable's sets come as lists (Python makes them sets again).
#include "globals.h"

#include <algorithm>
#include <cstring>
#include <map>
#include <set>
#include <unordered_map>

#include "call_graph.h"
#include "json.h"
#include "strings.h"

namespace cba {

namespace {

struct Var {
    int64_t at = 0, size = 0, taken = 0, element = 0, extent = 0;
    std::set<int64_t> readers, writers, values;
    std::set<std::string> notes;
    bool writable = false;
    bool uninitialised = false, has_label = false;
    uint64_t held = 0;
    std::string label, type;
};

const char *word(int64_t size) {   // _WORD
    switch (size) {
    case 1: return "unsigned char";
    case 2: return "unsigned short";
    case 4: return "unsigned long";
    case 8: return "double";
    case 10: return "long double";
    }
    return nullptr;
}

// _operand: the memory operand of ins that names va -> its position, or -1
int operand(const Image &img, const Ins &ins, int64_t va) {
    for (size_t k = 0; k < ins.ops.size(); k++) {
        const Op &op = ins.ops[k];
        if (op.kind == 'm' && op.base.empty()) {
            int64_t at = img.bits == 32 ? op.value : img.data_segment + (op.value & 0xFFFF);
            if (at == va) return (int)k;
        }
    }
    return -1;
}

// _dereferenced: is reg, loaded at fn.insns[i], used as an address before it is overwritten?
bool dereferenced(const Function &fn, size_t i, const std::string &reg) {
    std::string f = fam(reg);
    for (size_t j = i + 1; j < fn.insns.size() && j < i + 9; j++) {
        const Ins &b = fn.insns[j];
        for (const Op &op : b.ops)
            if (op.kind == 'm' && ((!op.base.empty() && fam(op.base) == f) || (!op.index.empty() && fam(op.index) == f)))
                return true;
        if (b.wr.count(f) || b.flow == F_JMP || b.flow == F_RET ||
            (b.flow == F_CALL && (f == "eax" || f == "ecx" || f == "edx")))
            return false;
    }
    return false;
}

std::string fmt(const char *f, const std::string &a, int64_t n) {
    char b[160];
    snprintf(b, sizeof b, f, a.c_str(), (long long)n);
    return b;
}

std::string type_of(const Image &img, const Var &g, const Image::Str *string, int64_t size) {
    const auto &notes = g.notes;
    char b[96];
    if (string && g.writers.empty()) {
        snprintf(b, sizeof b, "%s[%d]", string->kind == 'W' ? "wchar_t" : "char", (int)string->text.size() + 1);
        return b;
    }
    if (notes.count("called")) return "function pointer";
    if (notes.count("far pointer")) return "far pointer";
    if (notes.count("string pointer")) return "char *";
    if (notes.count("float")) return size == 4 ? "float" : size == 8 ? "double" : size == 10 ? "long double" : "float";
    for (const std::string &n : notes)                     // sorted: the first "result of"
        if (n.compare(0, 10, "result of ") == 0) return n;
    if (notes.count("array")) {
        const char *w = word(g.element ? g.element : size);
        return std::string(w ? w : "unsigned char") + "[]";
    }
    if (notes.count("pointer")) return "pointer";
    if (notes.count("counter")) return fmt("counter (%s%lld bytes)", "", size);
    if (notes.count("flags") && g.values.size() >= 2) return fmt("flags (%s%lld bytes)", "", size);
    if (g.taken && g.readers.empty() && g.writers.empty())
        return fmt("object passed by address (up to %s%lld bytes)", "", g.extent);
    const char *w = word(size);
    return w ? w : fmt("%s%lld bytes", "", size);
}

std::string variables_json(Image &img) {
    if (!img.graph_built) call_graph(img);
    ensure_table(img);
    std::vector<int64_t> order;
    std::unordered_map<int64_t, Var> out;
    for (const Image::Ref &r : img.refs) {
        int64_t va = r.va;
        if (img.name_text.count(va) || img.imports.count(va)) continue;
        const Span *s = img.span(va);
        if (!s || (s->exec && img.covering(va))) continue;          // an instruction, not data
        auto it = out.find(va);
        if (it == out.end()) {
            order.push_back(va);
            it = out.emplace(va, Var()).first;
            it->second.at = va;
            it->second.writable = s->writable;
        }
        Var &g = it->second;
        const Function &fn = img.function(r.fn);
        size_t i = 0;
        while (i < fn.insns.size() && fn.insns[i].addr != r.ins) i++;
        const Ins &ins = fn.insns[i];
        int k = operand(img, ins, va);
        std::string how = r.how;
        if (how == "a") {
            g.taken++;
            continue;
        }
        if (k < 0) {                                               // [reg + ADDRESS]: indexed from a register
            g.notes.insert("array");
            for (const Op &o : ins.ops)
                if (o.kind == 'm' && !o.base.empty()) g.element = std::max<int64_t>(g.element, !o.index.empty() ? o.scale : o.size);
            (how == "r" ? g.readers : g.writers).insert(r.fn);
            continue;
        }
        const Op &op = ins.ops[k];
        g.size = std::max<int64_t>(g.size, op.size);
        if (how.find('r') != std::string::npos) g.readers.insert(r.fn);
        if (how.find('w') != std::string::npos) g.writers.insert(r.fn);
        if (!op.index.empty()) {
            g.notes.insert("array");
            g.element = std::max<int64_t>(g.element, op.scale > 1 ? op.scale : op.size);
        }
        const std::string &mn = ins.mn;
        const auto &ops = ins.ops;
        if (ins.flow == F_CALL) g.notes.insert("called");
        else if (!mn.empty() && mn[0] == 'f' && mn != "fwait")
            g.notes.insert(mn.compare(0, 2, "fi") == 0 ? "integer in the FPU" : "float");
        else if (mn == "les" || mn == "lds") g.notes.insert("far pointer");
        else if (mn == "inc" || mn == "dec" ||
                 ((mn == "add" || mn == "sub") && ops.size() == 2 && ops[1].kind == 'i' && ops[1].value == 1 && k == 0))
            g.notes.insert("counter");
        else if ((mn == "mov" || mn == "cmp" || mn == "test" || mn == "or" || mn == "and") && ops.size() == 2 &&
                 ops[1].kind == 'i' && k == 0) {
            if (!(img.has_pointers && !img.pointers.empty() && img.pointers.count(ins.addr + ins.size - op.size)))
                g.values.insert(ops[1].value);
            if (mn == "or" || mn == "and" || mn == "test") g.notes.insert("flags");
        }
        if (mn == "mov" && k == 1 && ops[0].kind == 'r' && dereferenced(fn, i, ops[0].reg)) g.notes.insert("pointer");
        if (mn == "mov" && k == 0 && ops.size() > 1 && ops[1].kind == 'r' && fam(ops[1].reg) == "eax") {
            Arg src;
            if (definition(fn, (int)i, ops[1].reg, 14, src) && src.kind == 'c' && src.has_value &&
                img.imports.count(src.value)) {
                std::string api = img.import_text.count(src.value) ? img.import_text.at(src.value) : std::string();
                size_t bang = api.rfind('!');
                g.notes.insert("result of " + (bang == std::string::npos ? api : api.substr(bang + 1)));
            }
        }
    }
    std::vector<int64_t> known(order.begin(), order.end());
    for (const Image::Str &s : img.strings) known.push_back(s.va);
    std::sort(known.begin(), known.end());
    known.erase(std::unique(known.begin(), known.end()), known.end());
    std::string o = "[";
    for (size_t n = 0; n < order.size(); n++) {
        Var &g = out[order[n]];
        int64_t va = g.at;
        const Span *s = img.span(va);
        auto kt = std::upper_bound(known.begin(), known.end(), va);
        int64_t end = s->va + (int64_t)s->data.size();
        g.extent = std::min<int64_t>((kt != known.end() && *kt < end ? *kt : end) - va, 0x10000);
        int64_t size = g.size ? g.size : g.element ? g.element : (img.bits == 32 ? 4 : 2);
        if (!(va - s->va < s->init)) g.uninitialised = true;
        else {
            std::string raw = img.read(va, std::min<int64_t>(size, 8));
            g.held = 0;
            for (size_t b = 0; b < raw.size(); b++) g.held |= (uint64_t)(unsigned char)raw[b] << (8 * b);
        }
        auto p = img.pointers.find(va);
        if (img.has_pointers && p != img.pointers.end() && p->second.has_target) {
            int64_t t = p->second.target;
            g.has_label = true;
            g.label = "&" + img.label(t);
            if (img.name_text.count(t) || img.imports.count(t)) g.notes.insert("called");
            else if (in_table(img, t)) g.notes.insert("string pointer");
            else g.notes.insert("pointer");
        }
        g.type = type_of(img, g, in_table(img, va), size);
        g.size = size;
        if (n) o += ',';
        o += "{\"at\":", put_int(o, va), o += ",\"size\":", put_int(o, g.size), o += ",\"readers\":[";
        bool first = true;
        for (int64_t v : g.readers) o += first ? "" : ",", put_int(o, v), first = false;
        o += "],\"writers\":[", first = true;
        for (int64_t v : g.writers) o += first ? "" : ",", put_int(o, v), first = false;
        o += "],\"taken\":", put_int(o, g.taken), o += ",\"notes\":[", first = true;
        for (const std::string &v : g.notes) o += first ? "" : ",", put_ustr(o, v), first = false;
        o += "],\"values\":[", first = true;
        for (int64_t v : g.values) o += first ? "" : ",", put_int(o, v), first = false;
        o += "],\"writable\":", o += g.writable ? "true" : "false";
        o += ",\"element\":", put_int(o, g.element), o += ",\"extent\":", put_int(o, g.extent), o += ",\"initial\":";
        if (g.has_label) put_ustr(o, g.label);
        else if (g.uninitialised) o += "\"uninitialised\"";
        else {
            char b[32];
            snprintf(b, sizeof b, "%llu", (unsigned long long)g.held);
            o += b;
        }
        o += ",\"type\":", put_ustr(o, g.type), o += '}';
    }
    return o + "]";
}

const char *slot_kind(const Image &img, int64_t t) {   // _slot_kind
    if (img.name_text.count(t)) return "function";
    if (img.imports.count(t)) return "import";
    if (in_table(img, t)) return "string";
    if (img.is_code(t)) return "code";
    return "data";
}

std::string tables_json(Image &img) {
    ensure_table(img);
    const int64_t wordsize = 4;
    std::map<int64_t, int64_t> sites;
    if (img.has_pointers) {
        for (auto &pl : img.pointer_list) {
            int64_t site = pl.first;
            const Pointer &p = img.pointers[site];
            if (!p.has_target || (p.kind != 0 && p.kind != 1) ||
                (img.is_code(site) && (img.kind == 'n' || img.covering(site))))
                continue;
            sites[site] = p.target;
        }
    } else {
        for (const Span &s : img.spans) {
            if (s.exec) continue;
            for (int64_t off = 0; off < s.init - 3; off += 4) {
                uint64_t v = 0;
                for (int b = 0; b < 4 && off + b < (int64_t)s.data.size(); b++)
                    v |= (uint64_t)(unsigned char)s.data[off + b] << (8 * b);
                const Image::Str *t = in_table(img, (int64_t)v);
                if (img.name_text.count((int64_t)v) || (t && t->referenced)) sites[s.va + off] = (int64_t)v;
            }
        }
    }
    std::vector<int64_t> order;
    for (auto &kv : sites) order.push_back(kv.first);
    struct T { int64_t at, count, stride; std::vector<int64_t> targets; std::vector<const char *> kinds; const char *shape; };
    std::vector<T> tables;
    std::set<int64_t> used;
    size_t i = 0;
    while (i < order.size()) {
        size_t j = i;
        while (j + 1 < order.size() && order[j + 1] == order[j] + wordsize) j++;
        if (j > i) {
            T t{order[i], (int64_t)(j - i + 1), wordsize, {}, {}, "run"};
            bool all_code = true;
            for (size_t k = i; k <= j; k++) {
                t.targets.push_back(sites[order[k]]);
                t.kinds.push_back(slot_kind(img, sites[order[k]]));
                if (std::strcmp(t.kinds.back(), "code")) all_code = false;
            }
            if (!all_code) tables.push_back(t);   // all-code runs are jump tables (see dispatch.py)
            for (size_t k = i; k <= j; k++) used.insert(order[k]);
        }
        i = j + 1;
    }
    std::vector<int64_t> singles;
    for (int64_t a : order)
        if (!used.count(a)) singles.push_back(a);
    i = 0;
    while (i + 2 < singles.size()) {                                // the same field of an array of structures
        int64_t stride = singles[i + 1] - singles[i];
        size_t j = i + 1;
        while (8 <= stride && stride <= 128 && j + 1 < singles.size() && singles[j + 1] - singles[j] == stride) j++;
        if (j - i >= 2 && 8 <= stride && stride <= 128) {
            T t{singles[i], (int64_t)(j - i + 1), stride, {}, {}, "records"};
            for (size_t k = i; k <= j; k++) {
                t.targets.push_back(sites[singles[k]]);
                t.kinds.push_back(slot_kind(img, sites[singles[k]]));
            }
            tables.push_back(t);
            i = j + 1;
        } else i++;
    }
    std::stable_sort(tables.begin(), tables.end(), [](const T &a, const T &b) { return a.at < b.at; });
    std::string o = "[";
    for (size_t n = 0; n < tables.size(); n++) {
        const T &t = tables[n];
        if (n) o += ',';
        o += "{\"at\":", put_int(o, t.at), o += ",\"count\":", put_int(o, t.count), o += ",\"stride\":";
        put_int(o, t.stride), o += ",\"targets\":[";
        for (size_t k = 0; k < t.targets.size(); k++) o += k ? "," : "", put_int(o, t.targets[k]);
        o += "],\"kinds\":[";
        for (size_t k = 0; k < t.kinds.size(); k++) o += k ? ",\"" : "\"", o += t.kinds[k], o += '"';
        o += "],\"shape\":\"", o += t.shape, o += "\"}";
    }
    return o + "]";
}

}  // namespace

bool globals_answer(Image &img, const char *what, std::string &out) {
    if (!std::strcmp(what, "variables")) out = variables_json(img);
    else if (!std::strcmp(what, "tables")) out = tables_json(img);
    else return false;
    return true;
}

}  // namespace cba
