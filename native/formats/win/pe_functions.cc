// pe_functions.cc -- the 32-bit function carve: codebreaker/formats/pe_functions.py.
// One linear Capstone sweep per code section (the Capstone Python's package carries), the function
// starts (exports, the entry point, direct-call targets in code, standard prologues), the extents
// start-to-next-start, and the whole-section listing.  The Python PeCarve keeps the module's API:
// it asks for the decoded addresses, the call targets and the functions, and decodes an instruction
// itself (Capstone's Python object) only when a caller asks for that one.
#include <algorithm>
#include <cstdlib>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include "../core/build.h"
#include "../core/cs32.h"
#include "../core/questions.h"
#include "../core/text.h"

namespace cbf::pef {
namespace {

constexpr size_t kWindow = 1 << 16;

std::wstring widen(const std::string &utf8) {
    std::wstring w;
    for (uint32_t cp : codepoints(utf8)) {
        if (cp >= 0x10000) {
            cp -= 0x10000;
            w += wchar_t(0xD800 + (cp >> 10));
            w += wchar_t(0xDC00 + (cp & 0x3FF));
        } else {
            w += wchar_t(cp);
        }
    }
    return w;
}

// the target of `call imm`: the operand text "0x..." (the lite decoder of platforms read it so too)
bool call_target(const CsIns &ins, int64_t &t, bool detail) {
    if (detail) {                                       // PeCarve: operands[0] is an immediate
        t = ins.imm;
        return ins.mnemonic == "call" && ins.imm0;
    }
    if (ins.mnemonic != "call" || ins.op_str.size() < 3 || ins.op_str[0] != '0' || ins.op_str[1] != 'x') return false;
    char *end = nullptr;
    unsigned long long v = strtoull(ins.op_str.c_str() + 2, &end, 16);
    if (*end != 0) return false;                        // not a lone immediate
    t = static_cast<int64_t>(v);
    return true;
}

struct Sec {
    int64_t base;
    std::string data;
    int64_t end() const { return base + static_cast<int64_t>(data.size()); }
};

struct Walk {
    std::wstring dll;
    std::vector<Sec> secs;
    std::vector<std::pair<int64_t, std::string>> exports;   // in the dict's order
    std::unordered_set<int64_t> export_set;
    std::unordered_set<int64_t> decoded, called;
    bool update_called = true;

    const Sec *sec_of(int64_t va) const {
        for (auto &s : secs)
            if (s.base <= va && va < s.end()) return &s;
        return nullptr;
    }

    // md.disasm(data[off:], base + off), in windows: calls f(ins) for each; f returns false to stop
    template <class F>
    void instructions(const Sec &s, int64_t off, F f) {
        const uint8_t *p = reinterpret_cast<const uint8_t *>(s.data.data());
        int64_t n = static_cast<int64_t>(s.data.size());
        while (off < n) {
            size_t w = static_cast<size_t>(std::min<int64_t>(n - off, kWindow));
            std::vector<CsIns> run = cs32_disasm(dll, p + off, w, s.base + off, 0, update_called);
            if (run.empty()) return;
            for (auto &ins : run)
                if (!f(ins)) return;
            off = run.back().address - s.base + run.back().size;
        }
    }

    void sweep() {
        for (auto &s : secs) {
            int64_t n = static_cast<int64_t>(s.data.size()), off = 0;
            const uint8_t *p = reinterpret_cast<const uint8_t *>(s.data.data());
            while (off < n) {
                size_t w = static_cast<size_t>(std::min<int64_t>(n - off, kWindow));
                std::vector<CsIns> run = cs32_disasm(dll, p + off, w, s.base + off, 0, update_called);
                if (run.empty()) {
                    off += 1;
                    continue;
                }
                for (auto &ins : run) {
                    decoded.insert(ins.address);
                    int64_t t;
                    if (call_target(ins, t, update_called)) called.insert(t);
                }
                off = run.back().address - s.base + run.back().size;
            }
        }
    }

    void decode_from(int64_t va) {
        const Sec *s = sec_of(va);
        if (!s) return;
        instructions(*s, va - s->base, [&](const CsIns &ins) {
            if (decoded.count(ins.address) && ins.address != va) return false;
            int64_t t;
            if (update_called && call_target(ins, t, update_called)) called.insert(t);
            decoded.insert(ins.address);
            return true;
        });
    }
};

// pe_functions.carve(None, dll, sections [(va, bytes, name)], exports [(va, name)], image_base, entry,
//                    update_called) -> (decoded addresses, call targets, [(start, end, name)])
void carve(Call &c) {
    Walk w;
    w.dll = widen(c.str_arg(0));
    for (const Value &s : c.arg(1).items) w.secs.push_back({s[0].as_int(), s[1].s});
    for (const Value &e : c.arg(2).items) {
        w.exports.push_back({e[0].as_int(), e[1].as_str()});
        w.export_set.insert(e[0].as_int());
    }
    const int64_t image_base = c.int_arg(3), entry = c.int_arg(4);
    w.update_called = c.bool_arg(5);

    w.sweep();
    std::set<int64_t> starts;
    for (auto &e : w.exports) starts.insert(e.first);
    if (entry) starts.insert(image_base + entry);
    for (int64_t t : w.called)
        if (w.sec_of(t)) starts.insert(t);
    for (auto &s : w.secs) {
        int64_t n = static_cast<int64_t>(s.data.size());
        const uint8_t *d = reinterpret_cast<const uint8_t *>(s.data.data());
        for (int64_t i = 0; i < n - 4; ++i) {
            if (d[i] == 0x55 && d[i + 1] == 0x8b && d[i + 2] == 0xec) starts.insert(s.base + i);
            else if (d[i] == 0x8b && d[i + 1] == 0xff && d[i + 2] == 0x55 && d[i + 3] == 0x8b && d[i + 4] == 0xec) starts.insert(s.base + i);
        }
    }
    std::vector<int64_t> pending;
    for (int64_t s : starts)
        if (!w.decoded.count(s)) pending.push_back(s);
    for (int64_t s : pending)
        if (w.export_set.count(s) || w.called.count(s) || s == image_base + entry) w.decode_from(s);

    std::vector<int64_t> order;
    for (int64_t s : starts)
        if (w.decoded.count(s)) order.push_back(s);
    std::vector<int64_t> addrs(w.decoded.begin(), w.decoded.end());
    std::sort(addrs.begin(), addrs.end());
    std::vector<int64_t> targets(w.called.begin(), w.called.end());
    std::sort(targets.begin(), targets.end());

    Out &o = c.out;
    o.begin_tuple();
    o.begin_list();
    if (w.update_called)
        for (int64_t a : addrs) o.integer(a);
    o.end_list();
    o.begin_list();
    if (w.update_called)
        for (int64_t t : targets) o.integer(t);
    o.end_list();
    o.begin_list();
    for (size_t i = 0; i < order.size(); ++i) {
        const Sec *s = w.sec_of(order[i]);
        int64_t end = s->end();
        if (i + 1 < order.size() && order[i + 1] < end) end = order[i + 1];
        o.begin_tuple();
        o.integer(order[i]);
        o.integer(end);
        const std::string *name = nullptr;
        for (auto &e : w.exports)                     // a dict: the last name given for an address
            if (e.first == order[i]) name = &e.second;
        if (name) o.text(*name);
        else o.none();
        o.end_tuple();
    }
    o.end_list();
    o.end_tuple();
}

// pe_functions.disasm_whole32(code, dll, base) -> [(address, text)]
void disasm_whole32(Call &c) {
    const std::wstring dll = widen(c.str_arg(0));
    const int64_t base = c.int_arg(1);
    Bytes code = c.data;
    int64_t n = code.size(), pos = 0;
    Out &o = c.out;
    o.begin_list();
    while (pos < n) {
        std::vector<CsIns> r = cs32_disasm(dll, code.data() + pos, static_cast<size_t>(std::min<int64_t>(15, n - pos)), base + pos, 1, false);
        o.begin_tuple();
        o.integer(base + pos);
        if (r.empty()) {
            o.text("(bad)");
            pos += 1;
        } else {
            o.text(r[0].op_str.empty() ? r[0].mnemonic : r[0].mnemonic + " " + r[0].op_str);
            pos += r[0].size;
        }
        o.end_tuple();
    }
    o.end_list();
}

}  // namespace

CBF_QUESTIONS(pe_functions) {
    add("pe_functions.carve", carve);
    add("pe_functions.disasm_whole32", disasm_whole32);
}

}  // namespace cbf::pef
