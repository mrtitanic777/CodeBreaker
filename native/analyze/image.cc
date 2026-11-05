// image.cc -- the decoding half of codebreaker/analyze/image.py: functions followed from their
// start (fall through, direct branches, recognised jump tables, never past the next function),
// and discover(): the starts a module gives away by its calls and by the addresses it holds.
#include "image.h"

#include <algorithm>
#include <cstdio>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace cba {

CsLib CS;

template <typename T>
static bool get(HMODULE m, const char *name, T &slot, char *err, size_t errlen) {
    slot = reinterpret_cast<T>(GetProcAddress(m, name));
    if (!slot) snprintf(err, errlen, "no %s in capstone.dll", name);
    return slot != nullptr;
}

bool load_capstone(const wchar_t *dll, char *err, size_t errlen) {
    HMODULE c = LoadLibraryW(dll);       // the module Python's capstone already loaded
    if (!c) {
        snprintf(err, errlen, "cannot load capstone.dll (error %lu)", GetLastError());
        return false;
    }
    return get(c, "cs_open", CS.open, err, errlen) && get(c, "cs_option", CS.option, err, errlen) &&
           get(c, "cs_disasm", CS.disasm, err, errlen) && get(c, "cs_free", CS.free, err, errlen) &&
           get(c, "cs_reg_name", CS.reg_name, err, errlen) && get(c, "cs_regs_access", CS.regs_access, err, errlen) &&
           get(c, "cs_close", CS.close, err, errlen);
}

static const int64_t MAX_TABLE = 1024;

static std::string family(const std::string &r) {
    static const std::unordered_map<std::string, std::string> F = [] {
        std::unordered_map<std::string, std::string> m;
        const char *rows[][2] = {{"eax", "rax eax ax al ah"}, {"ecx", "rcx ecx cx cl ch"}, {"edx", "rdx edx dx dl dh"},
                                 {"ebx", "rbx ebx bx bl bh"}, {"esp", "rsp esp sp spl"}, {"ebp", "rbp ebp bp bpl"},
                                 {"esi", "rsi esi si sil"}, {"edi", "rdi edi di dil"}};
        for (auto &row : rows) {
            std::string names = row[1], cur;
            for (size_t i = 0; i <= names.size(); i++) {
                if (i == names.size() || names[i] == ' ') {
                    if (!cur.empty()) m[cur] = row[0];
                    cur.clear();
                } else cur += names[i];
            }
        }
        return m;
    }();
    auto it = F.find(r);
    return it == F.end() ? r : it->second;
}

static bool is_return(const std::string &mn) {
    return mn == "ret" || mn == "retf" || mn == "retn" || mn == "lret" || mn == "iret" || mn == "iretd";
}

static bool ends_with(const std::string &s, const char *t) {
    size_t n = strlen(t);
    return s.size() >= n && s.compare(s.size() - n, n, t) == 0;
}

static bool one_of(const std::string &s, const char *list) {   // list: space-separated words
    const char *p = list;
    while (*p) {
        const char *q = p;
        while (*q && *q != ' ') q++;
        if ((size_t)(q - p) == s.size() && s.compare(0, s.size(), p, q - p) == 0) return true;
        p = *q ? q + 1 : q;
    }
    return false;
}

static const char *NEVER = "arpl daa das aaa aas aam aad into bound salc xlatb les lds lcall ljmp iretd iret int1 "
                           "insb insd insw outsb outsd outsw cmc hlt";
static const char *PRIVILEGED = "in out cli sti lgdt lidt ltr lmsw clts invd wbinvd";

static int64_t signed_of(int64_t value, int size) {
    int bits = 8 * size;
    if (bits <= 0 || bits > 63) return value;
    return (value >> (bits - 1)) ? value - ((int64_t)1 << bits) : value;
}

std::string Image::label(int64_t va) const {
    auto n = name_text.find(va);
    if (n != name_text.end() && !n->second.empty()) return n->second;
    auto i = import_text.find(va);
    if (i != import_text.end() && !i->second.empty()) return i->second;
    char b[32];
    snprintf(b, sizeof b, "%llx", (long long)va);
    return b;
}

Image::~Image() {
    fn_.clear();
    if (md_) CS.close(&md_);
}

bool Image::open_decoder(std::string &err) {
    if (!CS.open) {
        err = "capstone not loaded";
        return false;
    }
    if (CS.open(CS_ARCH_X86, bits == 32 ? CS_MODE_32 : CS_MODE_16, &md_) != CS_ERR_OK) {
        err = "cs_open failed";
        return false;
    }
    CS.option(md_, CS_OPT_DETAIL, CS_OPT_ON);
    return true;
}

void Image::finish() {
    std::sort(spans.begin(), spans.end(), [](const Span &a, const Span &b) { return a.va < b.va; });
    starts_.clear();
    for (auto &s : spans) starts_.push_back(s.va);
    std::sort(functions.begin(), functions.end());
    for (auto f : functions) names.insert(f);
    for (auto &p : pointer_list) pointers[p.first] = p.second;
}

const Span *Image::span(int64_t va) const {
    auto it = std::upper_bound(starts_.begin(), starts_.end(), va);
    if (it == starts_.begin()) return nullptr;
    const Span &s = spans[(it - starts_.begin()) - 1];
    return va < s.va + (int64_t)s.data.size() ? &s : nullptr;
}

std::string Image::read(int64_t va, int64_t n) const {
    const Span *s = span(va);
    if (!s) return std::string();
    int64_t off = va - s->va, size = (int64_t)s->data.size();
    int64_t end = std::min(off + n, size);
    return end > off ? s->data.substr((size_t)off, (size_t)(end - off)) : std::string();
}

bool Image::u16(int64_t va, int64_t &v) const {
    std::string b = read(va, 2);
    if (b.size() != 2) return false;
    v = (uint8_t)b[0] | ((int64_t)(uint8_t)b[1] << 8);
    return true;
}

bool Image::u32(int64_t va, int64_t &v) const {
    std::string b = read(va, 4);
    if (b.size() != 4) return false;
    v = (int64_t)((uint32_t)(uint8_t)b[0] | ((uint32_t)(uint8_t)b[1] << 8) | ((uint32_t)(uint8_t)b[2] << 16) |
                  ((uint32_t)(uint8_t)b[3] << 24));
    return true;
}

bool Image::is_code(int64_t va) const {
    const Span *s = span(va);
    return s && s->exec;
}

bool Image::is_data(int64_t va) const { return span(va) != nullptr && !imports.count(va); }

bool Image::owner(int64_t va, int64_t &start) const {
    auto it = std::upper_bound(functions.begin(), functions.end(), va);
    if (it == functions.begin()) return false;
    int64_t f = *(it - 1);
    auto l = limits.find(f);
    if (l != limits.end() && va < l->second) {
        start = f;
        return true;
    }
    return false;
}

const std::string &Image::reg(unsigned rid) {
    auto it = regs_.find(rid);
    if (it != regs_.end()) return it->second;
    const char *n = CS.reg_name(md_, rid);
    return regs_[rid] = (n && *n) ? n : "?";
}

Ins Image::convert(const cs_insn &ci) {
    Ins ins;
    ins.addr = (int64_t)ci.address;
    ins.size = ci.size;
    ins.mn = ci.mnemonic;
    std::string text = std::string(ci.mnemonic) + " " + ci.op_str;
    size_t a = text.find_first_not_of(" \t\r\n\f\v"), b = text.find_last_not_of(" \t\r\n\f\v");
    ins.text = a == std::string::npos ? std::string() : text.substr(a, b - a + 1);
    uint64_t wide = bits == 32 ? 0xFFFFFFFFull : 0xFFFFull;
    const cs_x86 &x = ci.detail->x86;
    for (int k = 0; k < x.op_count; k++) {
        const cs_x86_op &o = x.operands[k];
        Op op;
        op.size = o.size;
        if (o.type == X86_OP_REG) {
            op.kind = 'r';
            op.reg = reg(o.reg);
        } else if (o.type == X86_OP_IMM) {
            op.kind = 'i';
            uint64_t mask = (o.size == 1 || o.size == 2 || o.size == 4) ? ((1ull << (8 * o.size)) - 1) : wide;
            op.value = (int64_t)((uint64_t)o.imm & mask);
        } else if (o.type == X86_OP_MEM) {
            op.kind = 'm';
            if (o.mem.base) op.base = reg(o.mem.base);
            if (o.mem.index) op.index = reg(o.mem.index);
            op.scale = o.mem.scale;
            op.value = o.mem.base ? o.mem.disp : (int64_t)((uint64_t)o.mem.disp & wide);
            if (o.mem.segment) op.seg = reg(o.mem.segment);
        } else continue;
        ins.ops.push_back(op);
    }
    if (ci.id != 0) {
        cs_regs rd, wr;
        uint8_t nr = 0, nw = 0;
        if (CS.regs_access(md_, &ci, rd, &nr, wr, &nw) == CS_ERR_OK) {
            for (int k = 0; k < nr; k++) ins.rd.insert(family(reg(rd[k])));
            for (int k = 0; k < nw; k++) ins.wr.insert(family(reg(wr[k])));
        }
    }
    const std::string &mn = ins.mn;
    if (is_return(mn)) ins.flow = F_RET;
    else if (mn == "call" || mn == "lcall" || mn == "jmp" || mn == "ljmp" || (!mn.empty() && mn[0] == 'j') ||
             one_of(mn, "loop loope loopne jcxz jecxz")) {
        ins.flow = ends_with(mn, "call") ? F_CALL : ends_with(mn, "jmp") ? F_JMP : F_JCC;
        int n = ci.size;
        const uint8_t *code = ci.bytes;
        if (bits == 16 && n >= 5 && (code[n - 5] == 0x9A || code[n - 5] == 0xEA) && mn[0] == 'l') {
            ins.has_target = true;
            ins.target = ((int64_t)(code[n - 2] | (code[n - 1] << 8)) << 16) | (code[n - 4] | (code[n - 3] << 8));
        } else if (!ins.ops.empty() && ins.ops[0].kind == 'i') {
            ins.has_target = true;
            ins.target = bits == 32 ? ins.ops[0].value : ((ins.addr & ~(int64_t)0xFFFF) | (ins.ops[0].value & 0xFFFF));
        }
    }
    return ins;
}

Function Image::linear(int64_t va, int n) {
    Function fn;
    fn.start = fn.limit = fn.end = va;
    const Span *s = span(va);
    if (!s || n <= 0) return fn;
    int64_t off = va - s->va;
    int64_t len = std::min<int64_t>(16 * (int64_t)n, (int64_t)s->data.size() - off);
    cs_insn *all = nullptr;
    size_t got = CS.disasm(md_, (const uint8_t *)s->data.data() + off, (size_t)len, (uint64_t)va, (size_t)n, &all);
    for (size_t k = 0; k < got; k++) fn.insns.push_back(convert(all[k]));
    if (all) CS.free(all, got);
    return fn;
}

const Function &Image::function(int64_t start) {
    auto it = fn_.find(start);
    if (it == fn_.end()) it = fn_.emplace(start, decode(start)).first;
    return *it->second;
}

const Ins *Image::covering(int64_t va) {
    int64_t f;
    if (!owner(va, f)) return nullptr;
    const Function &fn = function(f);
    size_t lo = 0, hi = fn.insns.size();
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (fn.insns[mid].addr <= va) lo = mid + 1;
        else hi = mid;
    }
    if (lo && va < fn.insns[lo - 1].addr + fn.insns[lo - 1].size) return &fn.insns[lo - 1];
    return nullptr;
}

void Image::discover() {
    std::vector<int64_t> pending = functions;
    while (true) {
        std::set<int64_t> fresh;
        for (int64_t start : pending)
            for (const Ins &ins : function(start).insns)
                if (ins.flow == F_CALL && ins.has_target && !names.count(ins.target) && !imports.count(ins.target) &&
                    is_code(ins.target))
                    fresh.insert(ins.target);
        if (fresh.empty()) {
            fresh = held_code_addresses();
            if (fresh.empty()) break;
        }
        std::set<int64_t> redo(fresh);
        for (int64_t va : fresh) {
            names.insert(va);
            order.push_back(va);
            auto pos = std::lower_bound(functions.begin(), functions.end(), va);
            size_t i = pos - functions.begin();
            const Span *s = span(va);
            int64_t end = s->va + (int64_t)s->data.size();
            if (kind == 'n') end = std::min(end, s->va + std::max<int64_t>(s->init, 1));
            bool has_next = i < functions.size();
            limits[va] = (has_next && functions[i] < end) ? functions[i] : end;
            if (i && limits[functions[i - 1]] > va) {
                int64_t prev = functions[i - 1];
                limits[prev] = va;
                fn_.erase(prev);
                redo.insert(prev);
            }
            functions.insert(functions.begin() + i, va);
        }
        discovered.insert(fresh.begin(), fresh.end());
        pending.assign(redo.begin(), redo.end());
    }
}

std::set<int64_t> Image::held_code_addresses() {
    std::set<int64_t> fresh;
    if (!has_pointers || pointers.empty() || bits != 32) return fresh;
    for (auto &entry : pointer_list) {
        int64_t site = entry.first;
        const Pointer &p = pointers[site];
        int64_t t = p.target;
        if (p.kind != 0 || !p.has_target || names.count(t) || imports.count(t) || !is_code(t)) continue;
        if (covering(t)) continue;                      // a label inside a function, or not code at all
        if (is_code(site)) {
            const Ins *ins = covering(site);
            if (!ins) {
                std::vector<int64_t> run{site};
                for (int step : {-4, 4}) {
                    int64_t at = site + step;
                    while (pointers.count(at) && run.size() < 256) {
                        run.push_back(at);
                        at += step;
                    }
                }
                bool known = false;
                for (int64_t a : run) {
                    const Pointer &q = pointers[a];
                    if (q.has_target && names.count(q.target)) known = true;
                }
                if (run.size() < 2 || !known) continue;
            } else {
                bool held = false;
                for (const Op &op : ins->ops)
                    if (op.kind == 'i' && op.value == t) held = true;
                if (!held || ins->flow != F_NONE) continue;
            }
        }
        if (looks_like_code(t)) fresh.insert(t);
    }
    return fresh;
}

bool Image::looks_like_code(int64_t va, int most) {
    const Span *s = span(va);
    std::string head = read(va, 8);
    if (!s || head.size() < 8) return false;
    if ((head[0] == 0 && head[1] == 0) || ((uint8_t)head[0] == 0xCC && (uint8_t)head[1] == 0xCC) ||
        ((uint8_t)head[0] == 0x90 && (uint8_t)head[1] == 0x90))
        return false;
    bool printable = true;
    for (int k = 0; k < 4; k++)
        if (!(0x20 <= (uint8_t)head[k] && (uint8_t)head[k] < 0x7F)) printable = false;
    if (printable) return false;
    int64_t off = va - s->va;
    std::string chunk = s->data.substr((size_t)off, (size_t)(16 * most));
    cs_insn *all = nullptr;
    size_t got = CS.disasm(md_, (const uint8_t *)chunk.data(), chunk.size(), (uint64_t)va, 0, &all);
    int n = 0;
    int result = 0;                                    // 0: ran out, 1 true, 2 false
    for (size_t k = 0; k < got && !result; k++) {
        const cs_insn &ci = all[k];
        std::string mn = ci.mnemonic;
        size_t sp = mn.find_last_of(" \t");
        std::string last = sp == std::string::npos ? mn : mn.substr(sp + 1);
        if (one_of(last, NEVER) || (flat && one_of(mn, PRIVILEGED)) || (ci.size >= 2 && ci.bytes[0] == 0 && ci.bytes[1] == 0)) {
            result = 2;
            break;
        }
        if (has_pointers && !pointers.empty()) {   // an address the loader writes must be an operand
            const cs_x86_encoding &e = ci.detail->x86.encoding;
            int64_t fit1 = e.disp_size == 4 ? (int64_t)ci.address + e.disp_offset : -1;
            int64_t fit2 = e.imm_size == 4 ? (int64_t)ci.address + e.imm_offset : -1;
            bool bad = false;
            for (int64_t a = (int64_t)ci.address; a < (int64_t)ci.address + ci.size; a++)
                if (pointers.count(a) && a != fit1 && a != fit2) bad = true;
            if (bad) {
                result = 2;
                break;
            }
        }
        n++;
        if (is_return(mn) || mn == "jmp") {
            result = mn != "retf" ? 1 : 2;
            break;
        }
        if (n >= most) result = 1;
    }
    if (all) CS.free(all, got);
    return result == 1;
}

std::unique_ptr<Function> Image::decode(int64_t start) {
    auto fn = std::make_unique<Function>();
    fn->start = start;
    auto lim = limits.find(start);
    fn->limit = lim == limits.end() ? start : lim->second;
    std::map<int64_t, Ins> seen;
    std::vector<int64_t> work{start};
    const Span *s = span(start);
    while (!work.empty() && s) {
        int64_t at = work.back();
        work.pop_back();
        if (seen.count(at) || !(start <= at && at < fn->limit)) continue;
        bool has_at = true;
        while (has_at && at < fn->limit) {             // a run of code, read a piece at a time
            int64_t off = at - s->va, size = (int64_t)s->data.size();
            int64_t hi = std::min(std::min(off + 48, fn->limit - s->va), size);
            bool has_next = false;
            int64_t next = 0;
            if (hi > off) {
                cs_insn *all = nullptr;
                size_t got = CS.disasm(md_, (const uint8_t *)s->data.data() + off, (size_t)(hi - off), (uint64_t)at, 0, &all);
                for (size_t k = 0; k < got; k++) {
                    const cs_insn &ci = all[k];
                    if (seen.count((int64_t)ci.address)) {
                        has_next = false;
                        break;
                    }
                    Ins &ins = seen[(int64_t)ci.address] = convert(ci);
                    next = (int64_t)ci.address + ci.size;
                    has_next = true;
                    if (ins.flow == F_RET || ins.mn == "hlt" || ins.mn == "ud2") {
                        has_next = false;
                        break;
                    }
                    if (ins.flow == F_JMP) {
                        if (ins.has_target) work.push_back(ins.target);
                        else if (!ins.ops.empty()) {
                            Table t;
                            if (jump_table(*fn, ins, seen, t)) {
                                for (int64_t v : t.targets) work.push_back(v);
                                fn->tables.push_back(std::move(t));
                            }
                        }
                        has_next = false;
                        break;
                    }
                    if (ins.flow == F_JCC && ins.has_target) work.push_back(ins.target);
                }
                if (all) CS.free(all, got);
            }
            at = next;
            has_at = has_next;
        }
    }
    fn->insns.reserve(seen.size());
    for (auto &kv : seen) fn->insns.push_back(std::move(kv.second));
    fn->end = fn->insns.empty() ? start : fn->insns.back().addr + fn->insns.back().size;
    return fn;
}

static std::string after_mnemonic(const std::string &text) {   // text.split(None, 1)[1]
    const char *ws = " \t\r\n\f\v";
    size_t a = text.find_first_not_of(ws);
    if (a == std::string::npos) return std::string();
    size_t b = text.find_first_of(ws, a);
    if (b == std::string::npos) return std::string();
    size_t c = text.find_first_not_of(ws, b);
    return c == std::string::npos ? std::string() : text.substr(c);
}

bool Image::jump_table(const Function &fn, const Ins &ins, const std::map<int64_t, Ins> &seen, Table &out) {
    const Op &op = ins.ops[0];
    if (op.kind != 'm') return false;
    int64_t table;
    int width;
    std::string r;
    if (bits == 32) {
        if (!op.base.empty() || op.index.empty() || op.scale != 4 || !is_data(op.value)) return false;
        table = op.value, width = 4, r = op.index;
    } else {
        r = !op.base.empty() ? op.base : op.index;
        if (op.seg != "cs" || r.empty() || op.size != 2) return false;
        table = (ins.addr & ~(int64_t)0xFFFF) | (op.value & 0xFFFF), width = 2;
    }
    std::vector<const Ins *> back;
    {
        auto it = seen.lower_bound(ins.addr);
        while (it != seen.begin() && back.size() < 10) {
            --it;
            back.push_back(&it->second);
        }
        std::reverse(back.begin(), back.end());
    }
    bool has_count = false, has_low = false, has_def = false, has_byte_table = false;
    int64_t count = 0, low = 0, def = 0, byte_table = 0;
    std::string selector = family(r);
    for (int i = (int)back.size() - 1; i >= 0; i--) {
        const Ins &b = *back[i];
        if ((b.mn == "mov" || b.mn == "movzx") && b.ops.size() == 2 && b.ops[0].kind == 'r' && b.ops[1].kind == 'm' &&
            family(b.ops[0].reg) == selector && b.ops[1].size == 1 && !has_byte_table && !b.ops[1].base.empty() &&
            bits == 32 && is_data(b.ops[1].value & 0xFFFFFFFF)) {
            has_byte_table = true;
            byte_table = b.ops[1].value & 0xFFFFFFFF;
            selector = family(b.ops[1].base);
        } else if (b.mn == "cmp" && b.ops.size() == 2 && b.ops[1].kind == 'i' && !has_count && i + 1 < (int)back.size() &&
                   one_of(back[i + 1]->mn, "ja jae jnbe jnb jg jge")) {
            has_count = true;
            count = b.ops[1].value + (one_of(back[i + 1]->mn, "ja jnbe jg") ? 1 : 0);
            has_def = back[i + 1]->has_target;
            def = back[i + 1]->target;
            std::string rest = after_mnemonic(b.text);
            size_t comma = rest.rfind(',');
            selector = comma == std::string::npos ? rest : rest.substr(0, comma);
            for (int k = std::max(0, i - 3); k < i; k++) {     // `sub reg, K` / `dec reg` before the bound
                const Ins &p = *back[k];
                if (!p.ops.empty() && p.ops[0].kind == 'r' && b.ops[0].kind == 'r' &&
                    family(p.ops[0].reg) == family(b.ops[0].reg)) {
                    if (p.mn == "sub" && p.ops.size() > 1 && p.ops[1].kind == 'i') has_low = true, low = p.ops[1].value;
                    else if (p.mn == "dec") has_low = true, low = 1;
                    else if (p.mn == "add" && p.ops.size() > 1 && p.ops[1].kind == 'i')
                        has_low = true, low = -signed_of(p.ops[1].value, p.ops[1].size);
                    else if (p.mn == "lea" && p.ops.size() > 1 && p.ops[1].kind == 'm' && !p.ops[1].base.empty() &&
                             p.ops[1].index.empty())
                        has_low = true, low = -p.ops[1].value;
                }
            }
            break;
        }
    }
    if (has_count && !(0 < count && count <= MAX_TABLE)) has_count = false;
    bool has_index = false, has_entries;
    std::vector<int> index;
    int64_t entries = 0;
    if (has_byte_table && has_count && count) {
        std::string raw = read(byte_table, count);
        for (unsigned char c : raw) index.push_back(c);
        has_index = true;
        has_entries = true;
        entries = (int64_t)index.size() == count ? (*std::max_element(index.begin(), index.end())) + 1 : 0;
    } else {
        has_entries = has_count;
        entries = count;
    }
    std::vector<int64_t> targets;
    int64_t n = has_entries ? entries : MAX_TABLE;
    for (int64_t i = 0; i < n; i++) {
        int64_t at = table + width * i, v;
        if (!(width == 4 ? u32(at, v) : u16(at, v))) break;
        if (width == 2) v |= ins.addr & ~(int64_t)0xFFFF;
        if (!(fn.start <= v && v < fn.limit) ||
            (!has_entries && i && (names.count(at) || seen.count(at))))
            break;
        targets.push_back(v);
    }
    if (targets.empty() || (has_entries && (int64_t)targets.size() != entries)) return false;
    out.site = ins.addr;
    out.table = table;
    out.targets = std::move(targets);
    out.low = has_low ? low : 0;
    out.selector = selector;
    out.has_default = has_def;
    out.def = def;
    out.has_bytes = has_index;
    out.bytes = std::move(index);
    out.bounded = has_count;
    return true;
}

}  // namespace cba
