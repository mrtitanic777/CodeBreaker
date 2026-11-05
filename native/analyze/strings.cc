// strings.cc -- a module's strings: where they are, who uses them, the ones assembled on the
// stack, and the format strings handed to printf-like functions (was the core of
// codebreaker/analyze/strings.py: _table, _uses, stack_strings with _assemble, _formats with
// _formatter, literal and image.arguments/definition). Answers are JSON in the Python's order.
//
// A string is a run of printable characters ending in a zero byte (or of 16-bit characters
// ending in a zero word). One the code refers to is believed from two characters up; one nothing
// refers to is listed only from five (four in 16-bit code), and only outside the code.
#include "strings.h"

#include <algorithm>
#include <cstring>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

#include "call_graph.h"
#include "json.h"
#include "pyset.h"

namespace cba {

namespace {

bool printable(unsigned char c) { return c >= 0x20 && c < 0x7F; }
bool texty(unsigned char c) { return printable(c) || c == 9 || c == 10 || c == 13; }

// _ascii / _wide: the string starting at `start`, if it is one of at least `least` characters
bool ascii_at(const std::string &d, int64_t start, int64_t least, std::string &out) {
    int64_t end = start, n = (int64_t)d.size();
    while (end < n && texty((unsigned char)d[end])) end++;
    if (end - start >= least && end < n && d[end] == 0) {
        out.assign(d, start, end - start);
        return true;
    }
    return false;
}

bool wide_at(const std::string &d, int64_t start, int64_t least, std::string &out) {
    int64_t end = start, n = (int64_t)d.size();
    while (end + 1 < n && d[end + 1] == 0 && texty((unsigned char)d[end])) end += 2;
    if ((end - start) / 2 >= least && end + 1 < n && d[end] == 0 && d[end + 1] == 0) {
        out.clear();
        for (int64_t k = start; k < end; k += 2) out += d[k];
        return true;
    }
    return false;
}

// strings.at
bool string_at(const Image &img, int64_t va, int64_t least, std::string &text, char &kind) {
    const Span *s = img.span(va);
    if (!s || va - s->va >= s->init) return false;
    int64_t off = va - s->va;
    const std::string &d = s->data;
    if (img.bits == 32 && wide_at(d, off, least, text)) {
        kind = 'W';
        return true;
    }
    if (ascii_at(d, off, least, text) &&
        !(text.size() < 4 && off && printable((unsigned char)d[off - 1]))) {   // not the tail of a longer one
        kind = 'A';
        return true;
    }
    return false;
}

void ensure_graph(Image &img) {
    if (!img.graph_built) call_graph(img);
}

void build_table(Image &img) {
    ensure_graph(img);
    img.strings.clear();
    img.string_at.clear();
    auto put = [&](int64_t va, const std::string &t, char k, bool ref, bool replace) {
        auto it = img.string_at.find(va);
        if (it != img.string_at.end()) {
            if (replace) img.strings[it->second] = {va, t, k, ref};
            return;
        }
        img.string_at[va] = img.strings.size();
        img.strings.push_back({va, t, k, ref});
    };
    PySet referenced;
    for (const Image::Ref &r : img.refs)
        if (!std::strcmp(r.how, "a")) referenced.add(r.va);
    if (img.has_pointers && !img.pointer_list.empty()) {
        PySet held;
        for (auto &p : img.pointer_list) {
            const Pointer &ptr = img.pointers[p.first];
            if (ptr.has_target && !img.is_code(p.first)) held.add(ptr.target);
        }
        referenced.merge(held);
    }
    std::string text;
    char kind;
    for (int64_t va : referenced.items()) {
        if (img.name_text.count(va)) continue;
        if (string_at(img, va, 2, text, kind)) put(va, text, kind, true, true);
    }
    for (const Span &s : img.spans) {                     // what nothing refers to
        if (s.exec || s.name == ".rsrc" || s.name == ".reloc" || s.name == ".edata" || s.name == ".idata") continue;
        const std::string &d = s.data;
        int64_t off = 0, held = std::min<int64_t>(s.init, (int64_t)d.size());
        while (off < held) {
            if (printable((unsigned char)d[off])) {
                bool found = ascii_at(d, off, img.bits == 32 ? 5 : 4, text);
                int size = 1;
                if (!found && img.bits == 32 && off + 1 < held && d[off + 1] == 0) {
                    found = wide_at(d, off, 5, text);
                    size = 2;
                }
                if (found) {
                    put(s.va + off, text, size == 1 ? 'A' : 'W', false, false);
                    off += ((int64_t)text.size() + 1) * size;
                    continue;
                }
                while (off < held && printable((unsigned char)d[off])) off++;   // the rest of a run that is not one
            }
            off++;
        }
    }
    img.strings_built = true;
}

}  // namespace

const Image::Str *in_table(const Image &img, int64_t va) {
    auto it = img.string_at.find(va);
    return it == img.string_at.end() ? nullptr : &img.strings[it->second];
}

void ensure_table(Image &img) {
    if (!img.strings_built) build_table(img);
}

namespace {

std::string table_json(const Image &img) {
    std::string o = "[";
    for (size_t k = 0; k < img.strings.size(); k++) {
        const Image::Str &s = img.strings[k];
        if (k) o += ',';
        o += '[', put_int(o, s.va), o += ',', put_str(o, s.text), o += s.kind == 'W' ? ",\"W\"," : ",\"A\",";
        o += s.referenced ? "true]" : "false]";
    }
    return o + "]";
}

bool near_ds(const Function &fn, size_t i) {   // the data segment pushed or stored beside instruction i
    size_t lo = i >= 2 ? i - 2 : 0, hi = std::min(fn.insns.size(), i + 3);
    for (size_t k = lo; k < hi; k++)
        for (const Op &op : fn.insns[k].ops)
            if (op.kind == 'r' && op.reg == "ds") return true;
    return false;
}

std::string uses_json(Image &img) {
    ensure_table(img);
    std::vector<int64_t> order;
    std::unordered_map<int64_t, std::string> rows;   // function -> its rows, JSON
    auto row = [&](int64_t f, int64_t site, int64_t va, const std::string &text) {
        auto it = rows.find(f);
        if (it == rows.end()) {
            order.push_back(f);
            it = rows.emplace(f, std::string()).first;
        } else it->second += ',';
        std::string &o = it->second;
        o += '[', put_int(o, site), o += ',', put_int(o, va), o += ',', put_str(o, text), o += ']';
    };
    if (img.bits == 32) {
        for (const Image::Ref &r : img.refs)
            if (!std::strcmp(r.how, "a"))
                if (const Image::Str *s = in_table(img, r.va)) row(r.fn, r.ins, r.va, s->text);
    } else {
        int64_t lo = img.data_segment;                    // 16-bit: an offset into the data segment, beside DS
        for (int64_t start : std::vector<int64_t>(img.functions)) {
            const Function &fn = img.function(start);
            for (size_t i = 0; i < fn.insns.size(); i++) {
                const Ins &ins = fn.insns[i];
                if ((ins.mn != "push" && ins.mn != "mov" && ins.mn != "lea") || ins.flow != F_NONE || ins.ops.empty())
                    continue;
                const Op &op = ins.ops.back();
                bool has = false;
                int64_t va = 0;
                if (op.kind == 'i' && op.size == 2 && op.value >= 0x10) has = true, va = lo + op.value;
                else if (op.kind == 'm' && ins.mn == "lea" && op.base.empty() && op.index.empty())
                    has = true, va = lo + (op.value & 0xFFFF);
                if (!has || !lo) continue;
                auto it = img.string_at.find(va);
                if (it == img.string_at.end() || !(ins.mn == "lea" || near_ds(fn, i))) continue;
                Image::Str &s = img.strings[it->second];
                row(fn.start, ins.addr, va, s.text);
                s.referenced = true;
            }
        }
    }
    std::string o = "[";
    for (size_t k = 0; k < order.size(); k++) {
        if (k) o += ',';
        o += '[', put_int(o, order[k]), o += ",[", o += rows[order[k]], o += "]]";
    }
    return o + "]";
}

bool has_letters(const std::string &t) {   // re.search("[A-Za-z]{2}", t)
    auto alpha = [](char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); };
    for (size_t k = 0; k + 1 < t.size(); k++)
        if (alpha(t[k]) && alpha(t[k + 1])) return true;
    return false;
}

struct Piece { std::string base; int64_t off; std::string text; bool wide; };

// _assemble: printable runs in {(base, offset): (byte, store number)}
std::vector<Piece> assemble(const std::map<std::pair<std::string, int64_t>, std::pair<int, int>> &stores, size_t least = 4) {
    std::vector<Piece> out;
    std::vector<std::pair<std::pair<std::string, int64_t>, std::pair<int, int>>> keys(stores.begin(), stores.end());
    size_t i = 0;
    while (i < keys.size()) {
        const std::string &base = keys[i].first.first;
        int64_t off = keys[i].first.second;
        size_t j = i;
        while (j + 1 < keys.size() && keys[j + 1].first.first == base && keys[j + 1].first.second == keys[j].first.second + 1)
            j++;
        std::vector<std::pair<int, int>> run;
        for (size_t k = i; k <= j; k++) run.push_back(keys[k].second);
        size_t start = 0;
        for (size_t k = 0; k <= run.size(); k++) {
            if (k == run.size() || !printable((unsigned char)run[k].first)) {
                std::string text;
                std::set<int> numbers;
                for (size_t x = start; x < k; x++) text += (char)run[x].first, numbers.insert(run[x].second);
                if (k - start >= least && numbers.size() >= 2 && has_letters(text))
                    out.push_back({base, off + (int64_t)start, text, false});
                start = k + 1;
            }
        }
        size_t k = 0;                                     // 16-bit characters: letter, 0, letter, 0
        while (k + 1 < run.size()) {
            size_t m = k;
            while (m + 1 < run.size() && printable((unsigned char)run[m].first) && run[m + 1].first == 0) m += 2;
            if ((m - k) / 2 >= 3) {
                std::string text;
                for (size_t x = k; x < m; x += 2) text += (char)run[x].first;
                if (has_letters(text)) out.push_back({base, off + (int64_t)k, text, true});
            }
            k = std::max(m, k + 1);
        }
        i = j + 1;
    }
    return out;
}

std::string stack_json(Image &img) {
    std::string o = "[";
    bool first = true;
    for (int64_t start : std::vector<int64_t>(img.functions)) {
        const Function &fn = img.function(start);
        std::map<std::pair<std::string, int64_t>, std::pair<int, int>> stores;
        int n = 0;
        for (const Ins &ins : fn.insns) {
            const auto &ops = ins.ops;
            if (ins.mn != "mov" || ops.size() != 2 || ops[0].kind != 'm' || ops[1].kind != 'i' || ops[0].base.empty() ||
                !ops[0].index.empty())
                continue;
            std::string base = fam(ops[0].base);
            if (base != "ebp" && base != "esp") continue;
            n++;
            for (int k = 0; k < ops[0].size; k++) {
                int64_t v = ops[1].value;
                int byte = 8 * k < 64 ? (int)((v >> (8 * k)) & 0xFF) : (v < 0 ? 0xFF : 0);
                stores[{base, ops[0].value + k}] = {byte, n};
            }
        }
        if (stores.size() < 4) continue;
        for (const Piece &p : assemble(stores)) {
            if (!first) o += ',';
            first = false;
            char where[64];
            snprintf(where, sizeof where, "%s0x%llx", p.off < 0 ? "-" : "+", (unsigned long long)(p.off < 0 ? -p.off : p.off));
            o += '[', put_int(o, fn.start), o += ',', put_str(o, p.base + where), o += ',', put_str(o, p.text);
            o += p.wide ? ",true]" : ",false]";
        }
    }
    return o + "]";
}

// ---- format strings ---------------------------------------------------------------------------
const std::pair<const char *, int> FORMATTERS[] = {
    {"wsprintf", 1}, {"wvsprintf", 1}, {"sprintf", 1}, {"swprintf", 1}, {"vsprintf", 1}, {"printf", 0},
    {"wprintf", 0}, {"fprintf", 1}, {"fwprintf", 1}, {"_snprintf", 2}, {"_snwprintf", 2}, {"sscanf", 1},
    {"swscanf", 1}, {"fscanf", 1}, {"_vsnprintf", 2}, {"wnsprintf", 2}, {"_wsprintf", 1}, {"_cprintf", 0}};
const char *NO_LIST[] = {"wvsprintf", "vsprintf", "_vsnprintf"};   // the arguments are elsewhere (a va_list)

std::string lower_ascii(std::string s) {
    for (char &c : s)
        if (c >= 'A' && c <= 'Z') c = (char)(c - 'A' + 'a');
    return s;
}

int formatter_position(const std::string &s) {
    for (auto &f : FORMATTERS)
        if (s == f.first) return f.second;
    return -1;
}

std::string replace_all(std::string s, const std::string &what) {
    size_t p = 0;
    while ((p = s.find(what, p)) != std::string::npos) s.erase(p, what.size());
    return s;
}

// _formatter: (position of the format argument, takes a wide format) for a printf-like import
bool formatter(const std::string &name, int &pos, bool &wide) {
    size_t bang = name.rfind('!');
    std::string bare = bang == std::string::npos ? name : name.substr(bang + 1);
    for (unsigned char c : bare)
        if (c >= 0x80) return false;                      // no such name lower-cases into the table
    std::string stem = bare;
    if ((bare.empty() || bare.back() == 'A' || bare.back() == 'W') &&
        formatter_position(lower_ascii(bare.substr(0, bare.empty() ? 0 : bare.size() - 1))) >= 0)
        stem = bare.substr(0, bare.size() - 1);
    pos = formatter_position(lower_ascii(stem));
    if (pos < 0) return false;
    std::string rest = replace_all(replace_all(replace_all(lower_ascii(stem), "wsprintf"), "wvsprintf"), "wnsprintf");
    wide = (!bare.empty() && bare.back() == 'W') || rest.find('w') != std::string::npos;
    return true;
}

bool listed(const std::string &api) {
    size_t bang = api.rfind('!');
    std::string bare = lower_ascii(bang == std::string::npos ? api : api.substr(bang + 1));
    for (const char *n : NO_LIST)
        if (bare.compare(0, strlen(n), n) == 0) return false;
    return true;
}

bool is_digit(char c) { return c >= '0' && c <= '9'; }

// _SPEC.search: a conversion anywhere in the text (every part before the conversion character
// is drawn from characters no conversion is, so the first greedy reading decides)
bool has_spec(const std::string &t) {
    static const char *CONV = "diouxXeEfgGcCsSpn%";
    for (size_t p = 0; p < t.size(); p++) {
        if (t[p] != '%') continue;
        size_t q = p + 1;
        while (q < t.size() && strchr("-+ 0#", t[q]) && t[q]) q++;
        if (q < t.size() && t[q] == '*') q++;
        else while (q < t.size() && is_digit(t[q])) q++;
        if (q + 1 < t.size() && t[q] == '.' && (t[q + 1] == '*' || is_digit(t[q + 1]))) {
            q += 1;
            if (t[q] == '*') q++;
            else while (q < t.size() && is_digit(t[q])) q++;
        }
        static const char *LENGTHS[] = {"hh", "h", "ll", "l", "L", "w", "I64", "I32"};
        for (const char *l : LENGTHS) {
            size_t n = strlen(l);
            if (t.compare(q, n, l) == 0 && q + n < t.size() && t[q + n] && strchr(CONV, t[q + n])) return true;
        }
        if (q < t.size() && t[q] && strchr(CONV, t[q])) return true;
    }
    return false;
}

}  // namespace

std::string fam(const std::string &r) {   // image.family
    static const char *rows[][2] = {{"rax", "eax"}, {"eax", "eax"}, {"ax", "eax"}, {"al", "eax"}, {"ah", "eax"},
                                    {"rcx", "ecx"}, {"ecx", "ecx"}, {"cx", "ecx"}, {"cl", "ecx"}, {"ch", "ecx"},
                                    {"rdx", "edx"}, {"edx", "edx"}, {"dx", "edx"}, {"dl", "edx"}, {"dh", "edx"},
                                    {"rbx", "ebx"}, {"ebx", "ebx"}, {"bx", "ebx"}, {"bl", "ebx"}, {"bh", "ebx"},
                                    {"rsp", "esp"}, {"esp", "esp"}, {"sp", "esp"}, {"spl", "esp"},
                                    {"rbp", "ebp"}, {"ebp", "ebp"}, {"bp", "ebp"}, {"bpl", "ebp"},
                                    {"rsi", "esi"}, {"esi", "esi"}, {"si", "esi"}, {"sil", "esi"},
                                    {"rdi", "edi"}, {"edi", "edi"}, {"di", "edi"}, {"dil", "edi"}};
    for (auto &row : rows)
        if (r == row[0]) return row[1];
    return r;
}

// an argument as image.arguments gives it: an operand, ("lea", ...), ("call", ...)
Arg of_op(const Op &op) {
    Arg a;
    a.kind = op.kind;
    a.reg = op.reg;
    a.value = op.value;
    return a;
}

// image.definition: what `reg` holds just before fn.insns[i], looking back through straight-line code
bool definition(const Function &fn, int i, const std::string &reg, int depth, Arg &out) {
    std::string f = fam(reg);
    for (int j = i - 1; j > std::max(-1, i - 1 - depth); j--) {
        const Ins &ins = fn.insns[j];
        if (ins.flow == F_JMP || ins.flow == F_RET) return false;
        if (ins.flow == F_CALL) {
            if (f == "eax") {
                out = Arg();
                out.kind = 'c';
                if (ins.has_target) out.has_value = true, out.value = ins.target;
                else if (!ins.ops.empty() && ins.ops[0].kind == 'm' && ins.ops[0].base.empty() && ins.ops[0].index.empty())
                    out.has_value = true, out.value = ins.ops[0].value;   // the slot it calls through
                return true;
            }
            if (f == "ecx" || f == "edx") return false;
            continue;
        }
        if (!ins.wr.count(f)) continue;
        const auto &ops = ins.ops;
        if (ops.size() == 2 && ops[0].kind == 'r' && fam(ops[0].reg) == f) {
            if (ins.mn == "mov" || ins.mn == "movzx") {
                if (ops[1].kind == 'r') {
                    if (definition(fn, j, ops[1].reg, depth - (i - j), out)) return true;
                    out = of_op(ops[1]);
                    return true;
                }
                out = of_op(ops[1]);
                return true;
            }
            if (ins.mn == "lea" && ops[1].kind == 'm') {
                out = Arg();
                out.kind = 'l';
                return true;
            }
            if ((ins.mn == "xor" || ins.mn == "sub") && ops[1].kind == 'r' && ops[1].reg == ops[0].reg) {
                out = Arg();
                out.kind = 'i';
                out.value = 0;
                return true;
            }
        }
        if (ins.mn == "pop" && j && fn.insns[j - 1].mn == "push") {   // push K / pop reg
            if (fn.insns[j - 1].ops.empty()) return false;
            out = of_op(fn.insns[j - 1].ops[0]);
            return true;
        }
        return false;
    }
    return false;
}

// image.arguments: the operands of the n pushes before fn.insns[i], nearest first
std::vector<Arg> arguments(const Function &fn, int i, size_t n) {
    std::vector<Arg> out;
    for (int j = i - 1; j > std::max(-1, i - 60); j--) {
        if (out.size() >= n) break;
        const Ins &ins = fn.insns[j];
        if (ins.flow == F_CALL || ins.flow == F_JMP || ins.flow == F_RET || ins.mn == "pop" || ins.mn == "leave" ||
            ins.mn == "enter" || ins.mn == "pushal" || ins.mn == "popal")
            break;
        if (ins.mn == "push" && !ins.ops.empty()) {
            const Op &op = ins.ops[0];
            Arg a;
            if (op.kind != 'r' || !definition(fn, j, op.reg, 14, a)) a = of_op(op);
            out.push_back(a);
        } else if ((ins.mn == "add" || ins.mn == "sub") && !ins.ops.empty() && ins.ops[0].kind == 'r' &&
                   fam(ins.ops[0].reg) == "esp")
            break;
    }
    return out;
}

namespace {

// strings.literal: the string a call's k-th argument points at
const Image::Str *literal(const Image &img, const std::vector<Arg> &args, size_t k) {
    if (img.bits == 32) {
        if (k < args.size() && args[k].kind == 'i') return in_table(img, args[k].value);
        return nullptr;
    }
    size_t lo = 2 * k, hi = 2 * k + 1;               // 16-bit: a far pointer, two words, offset pushed last
    if (hi < args.size() && args[lo].kind == 'i') {
        const Arg &seg = args[hi];
        int64_t base;
        if (seg.kind == 'r' && seg.reg == "ds") base = img.data_segment;
        else if (seg.kind == 'i' && img.span(seg.value * 65536)) base = seg.value * 65536;
        else return nullptr;
        return in_table(img, base + args[lo].value);
    }
    return nullptr;
}

void put_row(std::string &o, int64_t f, int64_t site, const std::string &function, const Image::Str &s, bool wide,
             bool list) {
    o += '[', put_int(o, f), o += ',', put_int(o, site), o += ',', put_ustr(o, function), o += ',';
    put_str(o, s.text), o += wide ? ",true," : ",false,", o += list ? "true]" : "false]";
}

std::string formats_json(Image &img) {
    ensure_table(img);
    std::string rows;
    int64_t seen = 0;
    struct Site { int64_t f, site, va; };
    std::vector<std::pair<int64_t, int>> cand_order;
    std::map<std::pair<int64_t, int>, std::vector<Site>> candidates;
    for (int64_t f : std::vector<int64_t>(img.functions)) {
        const Function &fn = img.function(f);
        std::unordered_map<int64_t, int> index;
        for (size_t k = 0; k < fn.insns.size(); k++) index.emplace(fn.insns[k].addr, (int)k);
        for (const Image::Call &c : img.calls[f]) {
            if (!c.has_dest) continue;
            auto at = index.find(c.site);
            int i = at == index.end() ? 0 : at->second;
            if (img.imports.count(c.dest)) {
                auto name = img.import_text.find(c.dest);
                const std::string api = name == img.import_text.end() ? std::string() : name->second;
                int pos;
                bool wide;
                if (!formatter(api, pos, wide)) continue;
                seen++;
                auto args = arguments(fn, i, img.bits == 16 ? 2 * (pos + 1) : pos + 1);
                if (const Image::Str *s = literal(img, args, pos)) {
                    if (!rows.empty()) rows += ',';
                    put_row(rows, f, c.site, api, *s, wide || s->kind == 'W', listed(api));
                }
            } else if (!std::strcmp(c.kind, "call") && img.bits == 32) {   // the module's own printf-like functions
                auto args = arguments(fn, i, 4);
                for (size_t k = 0; k < args.size(); k++) {
                    const Image::Str *s = args[k].kind == 'i' ? in_table(img, args[k].value) : nullptr;
                    if (s && has_spec(replace_all(s->text, "%%"))) {
                        auto key = std::make_pair(c.dest, (int)k);
                        auto it = candidates.find(key);
                        if (it == candidates.end()) {
                            cand_order.push_back(key);
                            it = candidates.emplace(key, std::vector<Site>()).first;
                        }
                        it->second.push_back({f, c.site, args[k].value});
                    }
                }
            }
        }
    }
    std::string own;
    for (auto &key : cand_order) {
        const auto &sites = candidates[key];
        std::set<int64_t> distinct;
        for (auto &s : sites) distinct.insert(s.va);
        if (sites.size() < 2 || distinct.size() < 2) continue;
        if (!own.empty()) own += ',';
        own += '[', put_int(own, key.first), own += ',', put_int(own, key.second), own += ']';
        std::string label = img.label(key.first);
        for (auto &s : sites) {
            const Image::Str *t = in_table(img, s.va);
            if (!rows.empty()) rows += ',';
            put_row(rows, s.f, s.site, label, *t, t->kind == 'W', true);
        }
    }
    std::string o = "{\"rows\":[" + rows + "],\"sites\":";
    put_int(o, seen);
    return o + ",\"own\":[" + own + "]}";
}

}  // namespace

bool strings_answer(Image &img, const char *what, std::string &out) {
    if (!std::strcmp(what, "table")) {
        build_table(img);
        out = table_json(img);
    } else if (!std::strcmp(what, "uses")) out = uses_json(img);
    else if (!std::strcmp(what, "stack")) out = stack_json(img);
    else if (!std::strcmp(what, "formats")) out = formats_json(img);
    else return false;
    return true;
}

}  // namespace cba
