// conventions.cc -- how each function is called and what kind of function it is (was the core
// of codebreaker/analyze/conventions.py, _functions and its helpers). One fact per function, as
// JSON objects whose keys come in the order the Python put them.
#include "conventions.h"

#include <algorithm>
#include <map>
#include <regex>
#include <set>

#include "call_graph.h"
#include "json.h"
#include "pyset.h"

namespace cba {

namespace {

const char *INPUTS[] = {"eax", "ecx", "edx", "ebx", "esi", "edi"};

bool is_input(const std::string &r) {
    for (const char *i : INPUTS)
        if (r == i) return true;
    return false;
}

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

int64_t sgn(int64_t value, int size) {
    int bits = 8 * size;
    if (bits <= 0 || bits > 63) return value;
    return (value >> (bits - 1)) ? value - ((int64_t)1 << bits) : value;
}

bool op_eq(const Op &a, const Op &b) {
    return a.kind == b.kind && a.reg == b.reg && a.base == b.base && a.index == b.index && a.seg == b.seg &&
           a.value == b.value && a.scale == b.scale && a.size == b.size;
}

bool is_esp_imm(const Ins &ins) {   // add/sub esp, K
    return ins.ops.size() == 2 && ins.ops[0].kind == 'r' && fam(ins.ops[0].reg) == "esp" && ins.ops[1].kind == 'i';
}

int last_field(const Op &op) { return op.size; }   // ops[0][-1]: every operand ends with its size

// stack_delta: bytes `ins` adds to what is pushed; false when it is not plain
bool stack_delta(const Ins &ins, int64_t &d) {
    if (ins.mn == "push") {
        d = ins.ops.empty() ? 4 : last_field(ins.ops[0]);
        return true;
    }
    if (ins.mn == "pop") {
        d = -(ins.ops.empty() ? 4 : last_field(ins.ops[0]));
        return true;
    }
    if ((ins.mn == "add" || ins.mn == "sub") && is_esp_imm(ins)) {
        int64_t n = sgn(ins.ops[1].value, ins.ops[1].size);
        d = ins.mn == "sub" ? n : -n;
        return true;
    }
    d = 0;
    return !ins.wr.count("esp");
}

struct Frame {
    bool framed = false;
    int64_t local = 0;
    std::vector<std::string> saved;
};

Frame frame_of(const Function &fn) {
    Frame f;
    std::vector<std::string> saved;
    for (size_t k = 0; k < fn.insns.size() && k < 12; k++) {
        const Ins &ins = fn.insns[k];
        const std::string &mn = ins.mn;
        const auto &ops = ins.ops;
        if (mn == "push" && !ops.empty() && ops[0].kind == 'r') {
            std::string r = fam(ops[0].reg);
            if ((r == "ebx" || r == "esi" || r == "edi" || (r == "ebp" && !f.framed)) &&
                std::find(saved.begin(), saved.end(), r) == saved.end()) {
                saved.push_back(r);
                continue;
            }
            if (r == "ds" || r == "es") continue;
            break;
        }
        if (mn == "mov" && ops.size() == 2 && ops[0].kind == 'r' && ops[1].kind == 'r' && fam(ops[0].reg) == "ebp" &&
            fam(ops[1].reg) == "esp")
            f.framed = true;
        else if (mn == "enter" && !ops.empty())
            f.framed = true, f.local = ops[0].value;
        else if ((mn == "sub" || mn == "add") && is_esp_imm(ins)) {
            int64_t n = sgn(ops[1].value, ops[1].size);
            f.local = std::max(f.local, mn == "sub" ? n : -n);
        } else if (!(mn == "inc" || mn == "nop" || mn == "pop" ||
                     (mn == "mov" && !ops.empty() && ops[0].kind == 'r' &&
                      (fam(ops[0].reg) == "eax" || fam(ops[0].reg) == "edi" || fam(ops[0].reg) == "ds" ||
                       fam(ops[0].reg) == "es"))))
            break;                                       // past the entry sequence
    }
    f.local = std::max<int64_t>(f.local, 0);
    for (auto &r : saved)
        if (r != "ebp") f.saved.push_back(r);
    return f;
}

std::map<int64_t, int64_t> argument_slots(const Function &fn, bool framed, int64_t first, int64_t word) {
    std::map<int64_t, int64_t> slots;
    auto widest = [&](int64_t off, int64_t size) {
        auto it = slots.find(off);
        slots[off] = std::max(it == slots.end() ? 0 : it->second, size);
    };
    if (framed) {
        for (const Ins &ins : fn.insns)
            for (const Op &op : ins.ops)
                if (op.kind == 'm' && !op.base.empty() && fam(op.base) == "ebp" && op.index.empty() && op.value >= first)
                    widest(op.value - first, op.size);
        return slots;
    }
    int64_t depth = 0;                                   // no frame: follow the stack through the entry code
    for (const Ins &ins : fn.insns) {
        if (ins.flow == F_CALL || ins.flow == F_JMP || ins.flow == F_RET) break;
        for (const Op &op : ins.ops)
            if (op.kind == 'm' && !op.base.empty() && fam(op.base) == "esp" && op.index.empty() &&
                op.value >= depth + first - word)
                widest(op.value - depth - (first - word), op.size);
        int64_t d;
        if (!stack_delta(ins, d)) break;
        depth += d;
    }
    return slots;
}

bool sets_constant(const Ins &ins) {
    const auto &ops = ins.ops;
    if (ops.size() != 2 || ops[0].kind != 'r') return false;
    if (ins.mn == "xor" || ins.mn == "sub" || ins.mn == "sbb") return op_eq(ops[0], ops[1]);
    if (ops[1].kind != 'i') return false;
    return (ins.mn == "or" && sgn(ops[1].value, ops[1].size) == -1) || (ins.mn == "and" && ops[1].value == 0);
}

std::unordered_map<int64_t, size_t> index_of(const Function &fn) {
    std::unordered_map<int64_t, size_t> idx;
    for (size_t k = 0; k < fn.insns.size(); k++) idx[fn.insns[k].addr] = k;
    return idx;
}

std::set<std::string> register_inputs(const Function &fn, const std::unordered_map<int64_t, size_t> &index) {
    std::set<std::string> found;
    std::set<std::pair<size_t, std::set<std::string>>> seen;
    struct Item {
        bool has;
        size_t i;
        std::set<std::string> written;
        int depth;
    };
    std::vector<Item> work{{true, 0, {}, 0}};
    int budget = 90;
    while (!work.empty() && budget > 0) {
        Item it = std::move(work.back());
        work.pop_back();
        bool has = it.has;
        size_t i = it.i;
        std::set<std::string> written = std::move(it.written);
        int depth = it.depth;
        while (has && i < fn.insns.size() && budget > 0) {
            auto key = std::make_pair(i, written);
            if (seen.count(key)) break;
            seen.insert(key);
            budget--;
            const Ins &ins = fn.insns[i];
            const auto &ops = ins.ops;
            bool zeroing = sets_constant(ins);
            if ((ins.mn != "push" && !zeroing) || (ins.mn == "push" && !ops.empty() && ops[0].kind == 'm')) {
                for (auto &r : ins.rd)
                    if (is_input(r) && !written.count(r)) found.insert(r);
            }
            if (ins.flow == F_CALL) written.insert({"eax", "ecx", "edx"});
            else if (ins.flow == F_RET) break;
            for (auto &r : ins.wr)
                if (is_input(r)) written.insert(r);
            if (ins.mn == "pop" && !ops.empty() && ops[0].kind == 'r') written.insert(fam(ops[0].reg));
            if (ins.flow == F_JMP) {
                has = false;
                if (ins.has_target) {
                    auto f = index.find(ins.target);
                    if (f != index.end()) has = true, i = f->second;
                }
                continue;
            }
            if (ins.flow == F_JCC && ins.has_target && depth < 3) {
                auto f = index.find(ins.target);
                if (f != index.end()) work.push_back({true, f->second, written, depth + 1});
            }
            auto nx = index.find(ins.addr + ins.size);
            has = nx != index.end();
            if (has) i = nx->second;
        }
    }
    return found;
}

enum Kind { K_UNKNOWN, K_CALL, K_VALUE, K_CONST };
struct RetKind {
    Kind kind = K_UNKNOWN;
    int64_t value = 0;
    bool wide = false, flt = false;
};

bool starts(const std::string &s, const char *p) { return s.compare(0, strlen(p), p) == 0; }

void returns_of(const Function &fn, std::vector<int64_t> &pops, std::vector<RetKind> &kinds) {
    std::unordered_set<int64_t> labels;
    for (const Ins &ins : fn.insns)
        if ((ins.flow == F_JCC || ins.flow == F_JMP) && ins.has_target) labels.insert(ins.target);
    for (int i = 0; i < (int)fn.insns.size(); i++) {
        const Ins &ins = fn.insns[i];
        if (ins.flow != F_RET) continue;
        pops.push_back(ins.ops.empty() ? 0 : ins.ops[0].value);
        RetKind k;
        int flt = -1;                                   // None
        for (int j = i - 1; j > std::max(-1, i - 12); j--) {
            const Ins &b = fn.insns[j];
            const std::string &mn = b.mn;
            if (mn[0] == 'f' && flt < 0 && mn != "fwait" && mn != "fnclex" && mn != "fldcw" && mn != "fnstcw" && mn != "fstcw")
                flt = !(starts(mn, "fstp") || starts(mn, "fistp") || mn == "fcompp" || mn == "fucompp");
            if (b.wr.count("edx") && b.flow != F_CALL && mn != "pop" && mn != "cdq" && mn != "cwd") k.wide = true;
            if (b.flow == F_CALL) {
                k.kind = K_CALL;
                break;
            }
            if (b.wr.count("eax") && mn != "pop") {
                const auto &ops = b.ops;
                if ((mn == "xor" || mn == "sub") && ops.size() == 2 && op_eq(ops[0], ops[1])) k.kind = K_CONST, k.value = 0;
                else if (mn == "mov" && ops.size() > 1 && ops[1].kind == 'i') k.kind = K_CONST, k.value = ops[1].value;
                else if (mn == "or" && ops.size() > 1 && ops[1].kind == 'i' && sgn(ops[1].value, ops[1].size) == -1)
                    k.kind = K_CONST, k.value = -1;
                else k.kind = K_VALUE;
                break;
            }
            if (b.flow == F_JMP || b.flow == F_RET || (labels.count(b.addr) && j < i - 1)) break;
        }
        k.flt = flt > 0;
        kinds.push_back(k);
    }
}

bool destination(const Image &img, const Ins &ins, int64_t &dest) {
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

std::string upper_ascii(std::string s) {
    for (auto &c : s)
        if (c >= 'a' && c <= 'z') c = (char)(c - 32);
    return s;
}

const char *callback(const std::string &name, const std::vector<std::string> &apis, bool has_pop, int64_t pop) {
    static const std::regex names[] = {
        std::regex("(WND|WINDOW|MDICHILD|FRAME|SUBCLASS)PROC$|WNDPROC", std::regex::icase),
        std::regex("(DLG|DIALOG)(PROC|FN)?$|DLGPROC", std::regex::icase),
        std::regex("HOOK(PROC)?$", std::regex::icase),
        std::regex("ENUM[A-Za-z0-9_]*PROC$", std::regex::icase)};
    static const char *kinds[] = {"window procedure", "dialog procedure", "hook procedure", "enumeration procedure"};
    for (int k = 0; k < 4; k++)
        if (!name.empty() && !starts(name, "sub_") && std::regex_search(name, names[k])) return kinds[k];
    static const std::map<std::string, const char *> CALLS = {
        {"DEFWINDOWPROC", "window procedure"}, {"DEFMDICHILDPROC", "window procedure"},
        {"DEFFRAMEPROC", "window procedure"},  {"CALLWINDOWPROC", "window procedure"},
        {"DEFDLGPROC", "dialog procedure"},     {"ENDDIALOG", "dialog procedure"},
        {"CALLNEXTHOOKEX", "hook procedure"},   {"DEFHOOKPROC", "hook procedure"}};
    for (const std::string &api : apis) {
        std::string key;
        size_t bang = api.rfind('!');
        if (bang != std::string::npos) {
            key = upper_ascii(api.substr(bang + 1));
            while (!key.empty() && (key.back() == 'A' || key.back() == 'W')) key.pop_back();
        }
        auto it = CALLS.find(key);
        if (it == CALLS.end()) continue;
        const char *kind = it->second;
        if (has_pop && (pop == 10 || pop == 16 || (!strcmp(kind, "hook procedure") && (pop == 8 || pop == 12)))) return kind;
    }
    return nullptr;
}

bool va_list_api(const std::string &a) {
    static const std::regex rx("(^|!)(w?vs?n?w?printf|wvsprintf[AW]?|_vsn?w?printf|vfw?printf|_?vsprintf|FormatMessage[AW])$",
                               std::regex::icase);
    return std::regex_search(a, rx);
}

std::string py_slice(const std::string &s, int a, int b) {   // s[a:b] for negative a, b
    int n = (int)s.size();
    int lo = std::max(0, n + a), hi = std::max(0, n + b);
    return lo < hi ? s.substr(lo, hi - lo) : std::string();
}

const char *string_op(const std::string &mn) {
    if (!starts(mn, "rep")) return nullptr;
    std::string k = py_slice(mn, -5, -1);
    if (k == "movs") return "copy";
    if (k == "stos") return "fill";
    if (k == "scas") return "scan";
    if (k == "cmps") return "compare";
    if (k == "lods") return "load";
    return nullptr;
}

void put_list(std::string &o, const std::vector<int64_t> &v) {
    o += '[';
    for (size_t k = 0; k < v.size(); k++) {
        if (k) o += ',';
        put_int(o, v[k]);
    }
    o += ']';
}

void put_strs(std::string &o, const std::vector<std::string> &v) {
    o += '[';
    for (size_t k = 0; k < v.size(); k++) {
        if (k) o += ',';
        put_ustr(o, v[k]);
    }
    o += ']';
}

int64_t floordiv(int64_t a, int64_t b) {
    int64_t q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) q--;
    return q;
}

}  // namespace

std::string conventions(Image &img) {
    if (!img.graph_built) call_graph(img);
    const int64_t word = img.bits == 32 ? 4 : 2;
    // what callers do with the result, what they remove after the call, what they load before it
    std::unordered_map<int64_t, std::pair<int64_t, int64_t>> uses;
    std::unordered_map<int64_t, std::map<int64_t, int64_t>> cleanup;
    struct Loads {
        int64_t sites = 0, ecx = 0, edx = 0;
    };
    std::unordered_map<int64_t, Loads> loads;
    std::unordered_map<int64_t, std::unordered_map<int64_t, size_t>> indexes;
    for (int64_t f : img.functions) {
        const Function &fn = img.function(f);
        auto &index = indexes[f] = index_of(fn);
        for (const Image::Call &c : img.calls[f]) {
            if (strcmp(c.kind, "call")) continue;
            size_t i = index.at(c.site);
            auto &tally = uses[c.dest];
            for (size_t k = i + 1; k < fn.insns.size() && k < i + 5; k++) {
                const Ins &b = fn.insns[k];
                if (b.rd.count("eax") && !sets_constant(b) && b.flow != F_RET) {
                    tally.first++;
                    break;
                }
                if (b.wr.count("eax") || b.flow == F_CALL) {
                    tally.second++;
                    break;
                }
                if (b.flow != F_NONE) break;
            }
            size_t j = i + 1;
            int64_t n = 0;
            while (j < fn.insns.size()) {
                const Ins &b = fn.insns[j];
                if (b.mn == "add" && is_esp_imm(b)) n += sgn(b.ops[1].value, b.ops[1].size);
                else if (b.mn == "pop" && !b.ops.empty() && b.ops[0].kind == 'r' &&
                         (fam(b.ops[0].reg) == "ecx" || fam(b.ops[0].reg) == "ebx") && n < 2 * word) {
                    n += word;
                    j++;
                    continue;
                }
                break;
            }
            cleanup[c.dest][n]++;
            if (img.bits == 32) {
                bool ecx = false, edx = false;
                for (size_t k = i; k > (i >= 3 ? i - 3 : 0);) {
                    k--;
                    const Ins &b = fn.insns[k];
                    if (b.flow != F_NONE) break;
                    if (b.wr.count("ecx")) ecx = true;
                    if (b.wr.count("edx")) edx = true;
                }
                Loads &v = loads[c.dest];
                v.sites++;
                v.ecx += ecx;
                v.edx += edx;
            }
        }
    }
    std::string o = "[";
    bool first_fact = true;
    for (int64_t start : std::vector<int64_t>(img.functions)) {
        const Function &fn = img.function(start);
        if (!first_fact) o += ',';
        first_fact = false;
        o += "{\"addr\":";
        put_int(o, fn.start);
        o += ",\"kind\":\"convention\",\"name\":";
        auto nm = img.name_text.find(fn.start);
        char hex[32];
        snprintf(hex, sizeof hex, "%llx", (long long)fn.start);
        std::string name = nm != img.name_text.end() ? nm->second : std::string(hex);
        put_ustr(o, name);
        if (fn.insns.empty()) {
            o += ",\"shape\":\"not code\",\"convention\":null,\"args\":null,\"arg_source\":null}";
            continue;
        }
        const auto &index = indexes[start];
        std::vector<int64_t> pops;
        std::vector<RetKind> ret_kinds;
        returns_of(fn, pops, ret_kinds);
        bool far = false;
        if (img.bits == 16)
            for (const Ins &i : fn.insns)
                if (i.mn == "retf" || i.mn == "lret") far = true;
        Frame fr = frame_of(fn);
        int64_t first = img.bits == 32 ? 2 * word : (far ? 6 : 4);
        auto slots = argument_slots(fn, fr.framed, first, word);
        std::set<std::string> regset = register_inputs(fn, index);
        {   // _pushed_inputs
            std::vector<const Ins *> real;
            for (const Ins &i : fn.insns)
                if (i.mn != "nop" && i.mn != "int3") real.push_back(&i);
            auto lv = loads.find(fn.start);
            for (size_t k = 0; k < real.size() && k < 4; k++) {
                const Ins &ins = *real[k];
                if (ins.mn == "push" && !ins.ops.empty() && ins.ops[0].kind == 'i') continue;
                if (ins.mn != "push" || ins.ops.empty() || ins.ops[0].kind != 'r') break;
                std::string r = fam(ins.ops[0].reg);
                if ((r == "ecx" || r == "edx") && lv != loads.end() && lv->second.sites >= 2 &&
                    (r == "ecx" ? lv->second.ecx : lv->second.edx) * 10 >= lv->second.sites * 9)
                    regset.insert(r);
            }
        }
        for (auto &s : fr.saved) regset.erase(s);
        regset.erase("ebp");
        regset.erase("esp");
        std::vector<std::string> regs(regset.begin(), regset.end());
        const auto &calls = img.calls[fn.start];
        std::set<std::string> apiset;
        for (auto &c : calls)
            if (c.has_dest && img.imports.count(c.dest)) apiset.insert(img.import_text[c.dest]);
        std::vector<std::string> apis(apiset.begin(), apiset.end());
        std::vector<const Ins *> real;
        for (const Ins &i : fn.insns)
            if (i.mn != "nop" && i.mn != "int3") real.push_back(&i);
        // shape
        std::string shape, extra;
        int64_t d0;
        if (real.size() == 1 && real[0]->flow == F_JMP && destination(img, *real[0], d0) && img.imports.count(d0)) {
            shape = "import thunk";
            extra = ",\"thunk_of\":";
            put_ustr(extra, img.import_text[d0]);
        } else if (real.size() == 1 && real[0]->flow == F_JMP && real[0]->has_target && img.names.count(real[0]->target)) {
            shape = "jump thunk";
            extra = ",\"thunk_of\":";
            put_ustr(extra, img.label(real[0]->target));
        } else if (real.size() == 1 && real[0]->flow == F_RET) shape = "empty";
        else if (real.empty()) shape = "empty";
        else if (real.size() <= 3 && real.back()->flow == F_RET && !ret_kinds.empty() && ret_kinds[0].kind == K_CONST &&
                 real.size() == 2 && real[0]->flow == F_NONE) {
            shape = "constant";
            extra = ",\"constant\":";
            put_int(extra, ret_kinds[0].value);
        } else {
            bool any = false;
            for (auto &c : calls)
                if (!strcmp(c.kind, "call") || !strcmp(c.kind, "import") || !strcmp(c.kind, "indirect")) any = true;
            shape = any ? "calls" : "leaf";
        }
        o += extra;
        o += ",\"shape\":";
        put_str(o, shape);
        std::set<std::string> tails;
        for (auto &c : calls)
            if (!strcmp(c.kind, "tail")) tails.insert(img.label(c.dest));
        if (!tails.empty()) {
            o += ",\"tail_calls\":";
            put_strs(o, std::vector<std::string>(tails.begin(), tails.end()));
        }
        // convention and arguments
        bool has_pop = !pops.empty();
        int64_t pop = 0;
        if (has_pop) {   // max(set(pops), key=pops.count): the first most common, in the set's order
            PySet ps;
            for (int64_t p : pops) ps.add(p);
            int64_t best = -1;
            for (int64_t v : ps.items()) {
                int64_t c = std::count(pops.begin(), pops.end(), v);
                if (c > best) best = c, pop = v;
            }
        }
        bool has_ecx = regset.count("ecx") > 0, has_edx = regset.count("edx") > 0;
        std::string conv;
        bool has_conv = has_pop;
        if (img.bits == 32) {
            bool others = false;
            for (auto &r : regs)
                if (r != "ecx" && r != "edx") others = true;
            if (!has_pop) {
            } else if (has_ecx && has_edx) conv = "fastcall";
            else if (has_ecx && !others) conv = "thiscall";
            else if (!regs.empty()) conv = "register";
            else conv = pop ? "stdcall" : "cdecl";
        } else if (has_pop)
            conv = std::string(pop ? "pascal" : "cdecl") + (far ? " far" : " near");
        o += ",\"convention\":";
        if (has_conv) put_str(o, conv);
        else o += "null";
        o += ",\"reg_args\":";
        put_strs(o, regs);
        o += ",\"removes\":";
        if (has_pop) put_int(o, pop);
        else o += "null";
        bool has_args = false;
        int64_t args = 0;
        std::string source;
        auto cl = cleanup.find(fn.start);
        if (has_pop && pop) has_args = true, args = floordiv(pop, word), source = "return";
        else if (has_pop && pop == 0) {
            int64_t best_n = 0, best_c = -1;
            if (cl != cleanup.end())
                for (auto &kv : cl->second)
                    if (kv.first > 0 && (kv.second > best_c || (kv.second == best_c && kv.first < best_n)))
                        best_c = kv.second, best_n = kv.first;
            if (best_c >= 0) has_args = true, args = floordiv(best_n, word), source = "callers";
            else if (!slots.empty()) {
                auto last = *slots.rbegin();
                has_args = true, args = floordiv(last.first + std::max(last.second, word), word), source = "frame";
            } else {
                int64_t zero = 0;
                if (cl != cleanup.end()) {
                    auto z = cl->second.find(0);
                    if (z != cl->second.end()) zero = z->second;
                }
                if (zero >= 2 || shape == "empty" || shape == "constant")
                    has_args = true, args = 0, source = (cl != cleanup.end() && !cl->second.empty()) ? "callers" : "shape";
            }
        }
        // (args and arg_source are written below, after variadic may have changed them)
        std::string slots_json;
        if (!slots.empty()) {
            slots_json = ",\"arg_slots\":{";
            bool first_slot = true;
            for (auto &kv : slots) {
                if (!first_slot) slots_json += ',';
                first_slot = false;
                slots_json += "\"+";
                put_int(slots_json, kv.first);
                slots_json += "\":";
                put_int(slots_json, kv.second);
            }
            slots_json += '}';
        }
        std::string rest = slots_json;
        rest += ",\"frame\":";
        rest += fr.framed ? "true" : "false";
        rest += ",\"locals\":";
        put_int(rest, fr.local);
        rest += ",\"saved\":";
        put_strs(rest, fr.saved);
        // result
        auto us = uses.find(fn.start);
        int64_t used = us == uses.end() ? 0 : us->second.first, ignored = us == uses.end() ? 0 : us->second.second;
        size_t consts = 0, sets = 0, explicit_ = 0;
        bool only01 = true, any_flt = false, any_wide = false;
        for (auto &k : ret_kinds) {
            if (k.kind == K_CONST) {
                consts++;
                if (k.value != 0 && k.value != 1) only01 = false;
            }
            if (k.kind != K_UNKNOWN) sets++;
            if (k.kind != K_UNKNOWN && k.kind != K_CALL) explicit_++;
            any_flt |= k.flt;
            any_wide |= k.wide;
        }
        const char *result = nullptr;
        if (any_flt && !explicit_) result = "float";
        else if (used) result = img.bits == 16 && any_wide ? "wide" : "value";
        else if (ignored >= 2 && !explicit_) result = "none";
        else if (!ret_kinds.empty() && consts == ret_kinds.size() && only01) result = "flag";
        else if (sets && !ignored) result = img.bits == 16 && any_wide ? "wide" : "value";
        rest += ",\"result\":";
        if (result) put_str(rest, result);
        else rest += "null";
        rest += ",\"result_evidence\":{\"callers_use\":";
        put_int(rest, used);
        rest += ",\"callers_ignore\":";
        put_int(rest, ignored);
        rest += '}';
        // what the body does
        int64_t loops = 0, floats = 0;
        std::set<std::string> strops;
        for (const Ins &i : fn.insns) {
            if ((i.flow == F_JCC || i.flow == F_JMP) && i.has_target && fn.start <= i.target && i.target <= i.addr) loops++;
            if (const char *s = string_op(i.mn)) strops.insert(s);
            if (i.mn[0] == 'f' && i.mn != "fwait") floats++;
        }
        if (loops) rest += ",\"loops\":", put_int(rest, loops);
        if (!strops.empty()) rest += ",\"string_ops\":", put_strs(rest, std::vector<std::string>(strops.begin(), strops.end()));
        if (floats) {
            rest += ",\"float_instructions\":";
            put_int(rest, floats);
            std::set<int64_t> fl;
            if (fr.framed)
                for (const Ins &i : fn.insns) {
                    const std::string &mn = i.mn;
                    if (!(mn == "fld" || mn == "fadd" || mn == "fsub" || mn == "fmul" || mn == "fdiv" || mn == "fcomp" ||
                          mn == "fcom"))
                        continue;
                    for (const Op &op : i.ops)
                        if (op.kind == 'm' && !op.base.empty() && fam(op.base) == "ebp" && op.value >= first &&
                            (op.size == 4 || op.size == 8))
                            fl.insert(op.value - first);
                }
            if (!fl.empty()) rest += ",\"float_args\":", put_list(rest, std::vector<int64_t>(fl.begin(), fl.end()));
        }
        auto ebp_slot = [&](const Op &op) {
            return op.kind == 'm' && !op.base.empty() && fam(op.base) == "ebp" && op.value >= first;
        };
        bool takes_slot = false;
        if (fr.framed)
            for (const Ins &i : fn.insns)
                if (i.ops.size() == 2 && i.mn == "lea" && ebp_slot(i.ops[1])) takes_slot = true;
        std::vector<int64_t> removed;
        if (has_pop && pop == 0 && cl != cleanup.end())
            for (auto &kv : cl->second)
                if (kv.first > 0) removed.push_back(kv.first);
        bool has_listed = false;
        int64_t listed = 0;
        if (fr.framed && has_pop && pop == 0) {
            std::set<int64_t> direct, taken;
            for (const Ins &i : fn.insns) {
                if (i.mn != "lea")
                    for (const Op &op : i.ops)
                        if (ebp_slot(op) && op.index.empty()) direct.insert(op.value - first);
                if (i.mn == "lea" && i.ops.size() == 2 && ebp_slot(i.ops[1]) && i.ops[1].index.empty())
                    taken.insert(i.ops[1].value - first);
            }
            for (int64_t at : taken) {
                bool beyond = false;
                for (int64_t d : direct)
                    if (d >= at) beyond = true;
                if (at >= word && direct.count(at - word) && !beyond) has_listed = true, listed = at;
            }
        }
        bool va_api = false;
        for (auto &a : apis)
            if (va_list_api(a)) va_api = true;
        bool variadic = false;
        if ((takes_slot && (va_api || removed.size() >= 2)) || removed.size() >= 3 || (has_listed && shape == "calls")) {
            variadic = true;
            if (has_listed) has_args = true, args = floordiv(listed, word), source = "frame (named arguments)";
            else if (!removed.empty()) {
                int64_t m = floordiv(removed[0], word), other = (has_args && args) ? args : 99;
                has_args = true, args = std::min(m, other), source = "callers (at least)";
            }
        }
        o += ",\"args\":";
        if (has_args) put_int(o, args);
        else o += "null";
        o += ",\"arg_source\":";
        if (has_args) put_str(o, source);
        else o += "null";
        o += rest;
        if (variadic) o += ",\"variadic\":true";
        if (img.bits == 16) {   // _entry16
            std::string head = img.read(fn.start, 8), twelve = img.read(fn.start, 12);
            std::vector<const char *> notes;
            if (head.compare(0, 3, "\x1e\x58\x90", 3) == 0 || head.compare(0, 3, "\x8c\xd8\x90", 3) == 0) {
                notes.push_back("exported entry (the loader patches in the data segment)");
                head = head.size() > 3 ? head.substr(3) : std::string();
            }
            if (!head.empty() && (unsigned char)head[0] == 0xB8 && twelve.find("\x8e\xd8") != std::string::npos)
                notes.push_back("loads its own data segment");
            if (head.compare(0, 4, "\x45\x55\x8b\xec", 4) == 0) notes.push_back("far frame marked for the stack walker");
            if (!notes.empty()) {
                o += ",\"entry\":[";
                for (size_t k = 0; k < notes.size(); k++) {
                    if (k) o += ',';
                    put_str(o, notes[k]);
                }
                o += ']';
            }
        }
        if (const char *cb = callback(name, apis, has_pop, pop)) {
            o += ",\"procedure\":";
            put_str(o, cb);
        }
        o += '}';
    }
    o += ']';
    return o;
}

}  // namespace cba
