// codebreaker/verify/gate16/contract16.cpp -- typed C++ of codebreaker.verify.gate16.contract16 (see contract16.hpp).
//
// The calls of each function rebuilt from C beside the original's: per callee, the bytes the caller takes off the stack after
// the call. The original's callers say how each callee is to be called -- one that removes its own arguments is called without
// the caller taking them off -- and a rebuilt function that takes off what the callee already removed (or leaves what it does not)
// leaves the stack wrong for everything after it.
//
// The isolated check (isolated16.py) answers a call as the description says the callee does, so it agrees with any belief the
// description has; this reads both modules' instructions as they are and so sees a wrong belief.
//
// The Python keeps each described module through pickle (cache "contract16_described"); here describe16 runs again (it writes
// the same description.txt either way).

#include "codebreaker/verify/gate16/contract16.hpp"

#include <algorithm>
#include <cstring>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <capstone/capstone.h>

#include "codebreaker/decompile/describe.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::verify::gate16::contract16 {

namespace describe = codebreaker::decompile::describe;
using describe::Insn;
using std::string;

namespace {

const py::set<py::str> _VARIADIC = {py::str("USER_420"), py::str("wsprintf"), py::str("USER_421"), py::str("wvsprintf")};   // (USER.420 wsprintf: its own count)
// (functions that take their arguments in registers -- DOS3Call, NetBIOSCall: AH, DS:DX, ES:DI, CX loaded just before the call,
// and the carry flag back. C sets none of them: a C caller of one is not the original's. VER's sub_3_0BD6 renames a file through
// DOS3Call; its C called it with nothing loaded, and the isolated check, answering DOS3Call alike on both sides, saw no difference)
const py::set<py::str> _IN_REGISTERS = {py::str("KERNEL_102"), py::str("DOS3CALL"), py::str("KERNEL_103"), py::str("NETBIOSCALL")};

bool starts(const string& s, const char* p) { size_t n = std::strlen(p); return s.size() >= n && s.compare(0, n, p) == 0; }
bool is_hex_lit(const string& s) {
    if (s.size() < 3 || s[0] != '0' || s[1] != 'x') return false;
    for (size_t i = 2; i < s.size(); i++) {
        char c = s[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) return false;
    }
    return true;
}
bool is_dec_lit(const string& s) {
    if (s.empty()) return false;
    for (char c : s) if (c < '0' || c > '9') return false;
    return true;
}
bool is_num_lit(const string& s) { return is_hex_lit(s) || is_dec_lit(s); }
using SegOffKey = std::pair<int64_t, int64_t>;
// re.fullmatch(r"-?\d+", s)
bool is_signed_num_lit_dec(const string& s) { return is_dec_lit(!s.empty() && s[0] == '-' ? s.substr(1) : s); }
// Python's repr of a sorted(set, key=str) list of byte counts (int | str)
string repr_taken(const std::set<TakenOff>& xs) {
    std::vector<std::pair<string, TakenOff>> items;
    for (const TakenOff& x : xs) {
        string key = std::holds_alternative<int64_t>(x) ? std::to_string(std::get<int64_t>(x)) : std::get<py::str>(x).utf8();
        items.emplace_back(key, x);
    }
    std::stable_sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    string out = "[";
    for (size_t k = 0; k < items.size(); k++) {
        if (k) out += ", ";
        out += std::holds_alternative<int64_t>(items[k].second) ? items[k].first : repr(std::get<py::str>(items[k].second)).utf8();
    }
    return out + "]";
}
string repr_ints(const std::set<int64_t>& xs) {
    string out = "[";
    bool first = true;
    for (int64_t x : xs) {
        if (!first) out += ", ";
        first = false;
        out += std::to_string(x);
    }
    return out + "]";
}
bool is_signed_num_lit(const string& s) { return is_num_lit(!s.empty() && s[0] == '-' ? s.substr(1) : s); }
int64_t int_of(const string& s, int64_t base) { return py::int_parse(py::str(s), base); }
std::vector<string> split_on(const string& s, const string& sep) {
    std::vector<string> out;
    size_t at = 0;
    while (true) {
        size_t k = s.find(sep, at);
        out.push_back(s.substr(at, k == string::npos ? string::npos : k - at));
        if (k == string::npos) break;
        at = k + sep.size();
    }
    return out;
}
// i.mnemonic.replace("notrack ", "")
string plain_mnemonic(const Insn& i) {
    string m = i.mn;
    size_t k;
    while ((k = m.find("notrack ")) != string::npos) m.erase(k, 8);
    return m;
}
bool imm_first(const Insn& i) { return i.operands.size() > 0 && i.operands.vec()[0].type == X86_OP_IMM; }
int64_t first_imm(const Insn& i) { return i.operands.vec()[0].imm; }
int64_t first_size(const Insn& i) { return i.op_size.size() > 0 ? i.op_size.vec()[0] : 0; }
bool writes_sp(const Insn& i) {
    for (int64_t w : i.regs_write) if (w == X86_REG_SP) return true;
    return false;
}

// the registers of ("near", seg, off) / ("far", address) of a direct call, or None (through a pointer)
std::optional<CalleeKey> callee_key(const Insn& i, int64_t seg) {
    string mnemonic = plain_mnemonic(i);
    if (mnemonic == "call" && is_num_lit(i.os)) return CalleeKey{py::str("near"), seg, int_of(i.os, 0) & 0xFFFF};
    if (mnemonic == "lcall") {
        std::vector<string> parts = split_on(i.os, ", ");
        if (parts.size() == 2 && is_num_lit(parts[0]) && is_num_lit(parts[1]))
            return CalleeKey{py::str("far"), (int_of(parts[0], 0) << 16) | int_of(parts[1], 0), 0};
    }
    return std::nullopt;
}

// {index of a call: (bytes pushed and not yet taken off, whether that is all of them)} -- the pushes since the prologue, less what
// was popped, taken off (`add sp, n`) or removed by a callee that removes its own (removes(key)). A call's pushes are not all its
// own: the bytes before them can be an enclosing call's -- `GlobalAlloc(0x2042, GlobalSize(h))` pushes the flags, then GlobalSize's
// handle -- and counted back only to the call before, USER's GETCLIPBOARDDATA "pushed four" for GlobalAlloc's six; counted back to a
// join, a call taking no arguments was given the next call's (USER's sub_9_0907 pushes sub_9_0930's far pointer before calling
// sub_1_8270, whose result is sub_9_0930's first argument). A join of paths (a jump's target) has what every jump to it had, if they
// agree; else (or one not seen: a jump back, a table's) the count starts over there, as no longer all of them.
std::map<size_t, std::pair<int64_t, bool>> stack_of(const std::vector<Insn>& ins, int64_t seg, int64_t prologue,
                                                    const std::function<std::optional<int64_t>(const CalleeKey&)>& removes) {
    std::map<size_t, std::pair<int64_t, bool>> out;
    std::map<int64_t, std::vector<std::pair<std::vector<int64_t>, bool>>> saved;
    std::vector<int64_t> pend;
    bool exact = false, live = false;
    auto pop_words = [&](int64_t n) {
        while (n > 0 && !pend.empty()) {
            int64_t top = pend.back();
            pend.pop_back();
            n -= top ? top : 2;
        }
    };
    auto sum_of = [](const std::vector<int64_t>& p) {
        int64_t t = 0;
        for (int64_t x : p) t += x;
        return t;
    };
    for (size_t k = 0; k < ins.size(); k++) {
        const Insn& i = ins[k];
        int64_t a = static_cast<int64_t>(i.address);
        if (a < prologue) continue;
        if (a == prologue) {
            pend.clear();
            exact = true;
            live = true;
        } else if (saved.count(a) || !live) {
            std::vector<std::pair<std::vector<int64_t>, bool>> seen;
            if (saved.count(a)) {
                seen = saved[a];
                saved.erase(a);
            }
            if (live) seen.push_back({pend, exact});
            bool all_exact = true;
            std::set<int64_t> sums;
            for (const auto& pe : seen) {
                if (!pe.second) all_exact = false;
                sums.insert(sum_of(pe.first));
            }
            if (!seen.empty() && all_exact && sums.size() == 1) {
                pend = seen[0].first;
                exact = true;
            } else {
                pend.clear();
                exact = false;
            }
            live = true;
        }
        string m = plain_mnemonic(i);
        if (m == "push") {
            pend.push_back(first_size(i) == 4 && i.operands.size() > 0 ? 4 : 2);
            if (i.os == "cs" && k + 1 < ins.size() && ins[k + 1].mn == "call") pend.back() = 0;   // (`push cs / call near`: the return address's half, no argument)
            continue;
        }
        if (m == "pushf" || m == "pushfw") {
            pend.push_back(2);
            continue;
        }
        if (m == "pusha" || m == "pushaw") {
            pend.push_back(16);
            continue;
        }
        if (m == "pop" || m == "popf" || m == "popfw" || m == "popa" || m == "popaw") {
            pop_words(starts(m, "popa") ? 16 : 2);
            continue;
        }
        if ((m == "add" || m == "sub") && starts(i.os, "sp, ") && is_num_lit(i.os.substr(4))) {
            int64_t n = int_of(i.os.substr(4), 0);
            if (m == "sub") pend.push_back(n);   // (room for a structure passed by value)
            else pop_words(n);
            continue;
        }
        if (i.group(describe::CS_GRP_CALL)) {
            if (!pend.empty() && pend.back() == 0) pend.pop_back();
            out[k] = {sum_of(pend), exact};
            std::optional<CalleeKey> key = callee_key(i, seg);
            if (!key) {
                const Insn* nxt = k + 1 < ins.size() ? &ins[k + 1] : nullptr;
                if (!(nxt && nxt->mn == "add" && starts(nxt->os, "sp"))) {
                    pend.clear();   // (through a pointer: it may remove what was pushed, or not)
                    exact = false;
                }
                continue;
            }
            pop_words(removes(*key).value_or(0));
            continue;
        }
        if (i.group(describe::CS_GRP_JUMP)) {
            if (imm_first(i)) {
                int64_t t = first_imm(i) & 0xFFFF;
                if (t > a) saved[t].push_back({pend, exact});
            }
            if (m == "jmp" || m == "ljmp") live = false;
            continue;
        }
        if (i.group(describe::CS_GRP_RET) || m == "ret" || m == "retf" || m == "iret" || m == "int3") {
            live = false;
            continue;
        }
        if (!i.regs_ok) {
            pend.clear();
            exact = false;
            continue;
        }
        if (writes_sp(i)) {   // (`mov sp, bp`, LEAVE: the frame's end)
            pend.clear();
            exact = false;
        }
    }
    return out;
}

// (description, {address: name}, {address: bytes it removes -- None: not known})
struct Described {
    std::shared_ptr<describe::Description> d;
    std::map<int64_t, py::str> names;
    std::map<int64_t, std::optional<int64_t>> pops;
};

// `keep`: the description is kept (base/cache.py) by the module's bytes and the source of what describes it -- the original's is the
// same for every build of it, and a build made again the same as before is too: describing the two was most of the check. (Here the
// description is made again: see the top.)
Described described(const py::str& path, const py::str& where, bool keep) {
    (void)keep;
    Described out;
    out.d = describe::describe16(path, where);
    py::file f = py::open(py::os::path::join(where, py::str("description.txt")), py::str("r"), py::str("utf-8"));
    py::str text = f.read_text();
    f.close();
    string all = text.utf8();
    size_t at = 0;
    while (at < all.size()) {
        size_t nl = all.find('\n', at);
        string line = all.substr(at, nl == string::npos ? string::npos : nl - at);
        at = nl == string::npos ? all.size() : nl + 1;
        std::vector<string> p = split_on(line, "\t");
        if ((p[0] == "X" || p[0] == "F") && p.size() > 2) out.names[int_of(p[1], 16)] = py::str(p[2]);
        if ((p[0] == "X" || p[0] == "F") && p.size() > 3 && is_signed_num_lit_dec(p[3])) {
            int64_t v = int_of(p[3], 10);
            out.pops[int_of(p[1], 16)] = v >= 0 ? std::optional<int64_t>(v) : std::nullopt;
        }
    }
    return out;
}

// removes(key) for _calls: what the callee at a call's target removes, as described -- or, one the describer did not find (the
// build's functions are found as the original's are, and not all of them: USER's sub_1_2293 rebuilt at 1:5810 was not, and every call
// to it "took none"), as its own nearest return says
std::function<std::optional<int64_t>(const CalleeKey&)> removes_of(const Described& d) {
    return [&d](const CalleeKey& key) -> std::optional<int64_t> {
        int64_t va = key.kind == "far" ? key.a : describe::address(key.a, key.b);
        auto it = d.pops.find(va);
        if (it != d.pops.end()) return it->second;
        auto w = describe::where(va);
        int64_t s = std::get<0>(w), o = std::get<1>(w);
        auto img = d.d->images.get(s);
        if (!img) return std::nullopt;
        int64_t pop = std::get<1>(describe::returns(*img, o, 600, key.kind == "far" ? std::optional<py::str>(py::str("far")) : std::nullopt));
        return pop >= 0 ? std::optional<int64_t>(pop) : std::nullopt;
    };
}

}  // namespace

// [(callee, bytes taken off after it, argument bytes pushed for it -- negative: at least that, None: not known -- whether the
// frame's LEAVE follows)] of the code in [lo, hi). removes(key): the bytes a callee removes itself (its `retf n`), or None.
py::list<CallRow> _calls(const py::bytes& img, int64_t seg, int64_t lo, int64_t hi,
                         const std::function<std::optional<py::str>(const CalleeKey&)>& name_of,
                         const std::function<std::optional<int64_t>(const CalleeKey&)>& removes) {
    int64_t n = img.size();
    int64_t a = lo < 0 ? std::max<int64_t>(0, lo + n) : std::min(lo, n), b = hi < 0 ? std::max<int64_t>(0, hi + n) : std::min(hi, n);
    py::bytes window(b > a ? img.raw().substr(static_cast<size_t>(a), static_cast<size_t>(b - a)) : string());
    std::vector<Insn> ins = describe::decoder16(true)->disasm(window, lo).vec();
    py::list<CallRow> out;
    // (what the prologue pushes is saved registers, not arguments; and a push a jump lands on starts a new count -- as describe counts
    // the bytes pushed for a call through a pointer)
    std::set<int64_t> targets;
    for (const Insn& i : ins)
        if (i.group(describe::CS_GRP_JUMP) && imm_first(i)) targets.insert(first_imm(i) & 0xFFFF);
    int64_t prologue = lo;
    bool loaded = false;   // (DS set from AX: the Windows prolog done)
    for (size_t j = 0; j < std::min<size_t>(ins.size(), 16); j++) {
        const Insn& i = ins[j];
        const Insn* nxt_ = j + 1 < ins.size() ? &ins[j + 1] : nullptr;
        // (`mov ax, ds` and `mov ax, <data segment>` are the prolog's only before DS is loaded from AX: after it, GDI's sub_1_59D2
        // rebuilt -- `mov ds, ax / mov ax, ds / push ax / push 594h`, a far pointer to its data -- "pushed two bytes" for the four it
        // pushes)
        if (i.mn == "nop" || i.mn == "enter" || i.os == "bp, sp" || i.os == "ds, ax" || (!loaded && i.os == "ax, ds")
            // (and OpenWatcom's room for locals, `push ax` a word at a time -- counted as an argument, sub_3_7E58's allocator call
            // "pushed six bytes" for its two)
            || (i.mn == "push" && (i.os == "ax" || i.os == "cx" || i.os == "dx") && nxt_ && !nxt_->group(describe::CS_GRP_CALL)) ||
            ((i.mn == "push" || i.mn == "inc") && (i.os == "bp" || i.os == "ds" || i.os == "si" || i.os == "di")) || (i.mn == "pop" && i.os == "ax") ||
            (!loaded && i.mn == "mov" && starts(i.os, "ax, 0x")) || (i.mn == "sub" && starts(i.os, "sp,"))) {
            prologue = static_cast<int64_t>(i.address) + i.size;
            loaded = loaded || (i.mn == "mov" && i.os == "ds, ax");
        } else {
            break;
        }
    }
    targets.insert(prologue);
    std::map<size_t, std::pair<int64_t, bool>> stack_at = stack_of(ins, seg, prologue, removes);
    for (size_t k = 0; k < ins.size(); k++) {
        const Insn& i = ins[k];
        // (a target of 0 is printed `0`, not `0x0`: every call to a segment's first function -- SUWIN's helpers at 7:0000 and 9:0000 --
        // went unchecked, and a C caller passing one word of a far pointer's two was never seen)
        // (OpenWatcom's `3E E8`, a call with a DS prefix: Capstone's "notrack call")
        std::optional<CalleeKey> key = callee_key(i, seg);
        if (!key) continue;   // (through a pointer: the callee is not known here)
        std::optional<py::str> callee = name_of(*key);
        size_t after = k + 1;
        while (after < ins.size() && ins[after].mn == "nop") after++;   // (OpenWatcom's `nop` after a near call)
        const Insn* nxt = after < ins.size() ? &ins[after] : nullptr;
        int64_t total = 0;
        bool exact = false;   // (pushed and not yet taken off; False: counted only from a join on)
        auto st = stack_at.find(k);
        if (st != stack_at.end()) {
            total = st->second.first;
            exact = st->second.second;
        }
        int64_t off_n = 0;
        std::optional<py::str> off_s;   // a count read from a register: "?<operand>"
        size_t rest = after;   // (the first instruction past the cleanup)
        if (nxt && nxt->mn == "add" && starts(nxt->os, "sp, ")) {
            string amount = split_on(nxt->os, ", ")[1];
            if (is_signed_num_lit(amount)) off_n = int_of(amount, 0);
            else off_s = py::str("?" + amount);
            rest = after + 1;
        } else {
            // (or taken off by popping them into a register no one reads: `pop bx`, `pop cx` -- the original's way after a one-word cdecl
            // call; read as nothing, the build's `add sp, 2` after the same call "took off two bytes the original did not")
            for (size_t q = after; q < std::min(ins.size(), after + static_cast<size_t>(std::max<int64_t>(0, total / 2))); q++) {
                if (ins[q].mn == "pop" && (ins[q].os == "bx" || ins[q].os == "cx") && off_n < total) {
                    off_n += 2;
                    rest += 1;
                } else {
                    break;
                }
            }
        }
        bool at_leave = false;
        if (total) {
            // (and then the frame's LEAVE, nothing touching the stack before it: SP is set back from BP whatever was taken off -- Microsoft C 8
            // itself leaves a call's arguments there when the function ends next (SETUPX's 2:5A3C, `lcall _SUFORMATMESSAGEBOX / xor ax, ax /
            // leave`), OpenWatcom takes them off: the same. Told here; compared as the same.)
            for (size_t q = rest; q < std::min(ins.size(), rest + 12); q++) {
                const Insn& s = ins[q];
                if (s.mn == "leave" || (s.mn == "mov" && s.os == "sp, bp") || (s.mn == "lea" && starts(s.os, "sp, [bp"))) {   // (OpenWatcom's, before its pops)
                    at_leave = true;
                    break;
                }
                if (s.mn == "push" || s.mn == "pop" || s.mn == "pushf" || s.mn == "popf" || s.mn == "enter" || s.mn == "ret" || s.mn == "retf" ||
                    s.group(describe::CS_GRP_CALL) || s.group(describe::CS_GRP_JUMP))
                    break;
                if (!s.regs_ok) break;
                if (writes_sp(s)) break;
            }
        }
        if (callee && !callee->empty()) {
            // (the bytes that are this call's: what the callee removes itself, else what the caller takes off after it, else -- the frame's
            // LEAVE taking them -- all still pushed; a callee that removes none and is followed by no cleanup says nothing of its own)
            int64_t took = removes(*key).value_or(0);
            std::optional<int64_t> args;
            if (took > 0) args = took;
            else if (!off_s && off_n > 0) args = off_n;
            else if (at_leave) args = total;
            std::optional<int64_t> pushed;
            if (!args) {
                pushed = std::nullopt;
            } else if (*args <= total) {
                pushed = *args;
            } else {
                // (negative: at least that -- and none counted from a join is no count at all: USER's sub_6_1A72 jumps into its call to
                // sub_1_38E4 from three places, "pushing none")
                pushed = exact ? std::optional<int64_t>(total) : (total ? std::optional<int64_t>(-total) : std::nullopt);
            }
            TakenOff off = off_s ? TakenOff(*off_s) : TakenOff(off_n);
            out.append(CallRow(*callee, off, pushed, at_leave));
        }
    }
    return out;
}

// {name: why} for each named function whose calls take off different bytes than the original's calls to the same callee (map file
// and image of the build in `folder`).
py::dict<py::str, py::str> mismatches(const py::str& original, const py::str& folder, const py::str& image, const py::list<py::str>& names) {
    py::str tmp = py::tempfile::mkdtemp(std::nullopt, py::str("contract16_"));
    Described original_d, ours_d;
    try {
        original_d = described(original, py::os::path::join(tmp, py::str("o")), true);
        ours_d = described(image, py::os::path::join(tmp, py::str("u")), true);   // (a build made again: the same)
    } catch (...) {
        py::shutil::rmtree(tmp, true);
        throw;
    }
    py::shutil::rmtree(tmp, true);
    const Described& xo_pu = ours_d;
    std::map<int64_t, std::vector<int64_t>> starts;
    for (const auto& kv : original_d.d->names.items()) {
        auto w = describe::where(std::get<0>(kv));
        starts[std::get<0>(w)].push_back(std::get<1>(w));
    }
    for (auto& kv : starts) std::sort(kv.second.begin(), kv.second.end());
    std::map<py::str, std::tuple<int64_t, int64_t, int64_t>> orig_fn;
    std::map<SegOffKey, py::str> orig_at;
    for (const auto& kv : original_d.d->names.items()) {
        int64_t va = std::get<0>(kv);
        const py::str& n = std::get<1>(kv);
        auto w = describe::where(va);
        int64_t s = std::get<0>(w), o = std::get<1>(w);
        int64_t end = -1;
        for (int64_t x : starts[s]) if (x > o) { end = x; break; }
        if (end < 0) {
            auto im = original_d.d->images.get(s);
            end = im ? im->size() : 0;
        }
        orig_fn[n] = {s, o, end};
        orig_at[{s, o}] = n;
    }
    auto orig_name = [&](const CalleeKey& key) -> std::optional<py::str> {
        if (key.kind == "far") {
            auto it = original_d.names.find(key.a);
            if (it != original_d.names.end() && !it->second.empty()) return it->second;
            auto nm = original_d.d->names.get(key.a);
            if (nm && !nm->empty()) return nm;
            return std::nullopt;
        }
        auto it = orig_at.find({key.a, key.b});
        if (it == orig_at.end()) return std::nullopt;
        return it->second;
    };
    string text;
    try {
        py::file f = py::open(py::os::path::join(folder, py::str("m.map")), py::str("r"), std::nullopt, py::str("replace"));
        text = f.read_text().utf8();
        f.close();
    } catch (const py::OSError&) {
        return py::dict<py::str, py::str>();
    }
    std::vector<int64_t> sym_order;
    std::map<int64_t, std::vector<std::pair<int64_t, py::str>>> syms;
    py::re::Pattern line_pat(py::str("(?m)^([0-9a-f]{4}):([0-9a-f]{4})\\S*\\s+(\\S+)\\s*$"), 0);
    for (const auto& m : line_pat.finditer(py::str(text))) {
        int64_t s = py::int_parse(m.group(1), 16), o = py::int_parse(m.group(2), 16);
        py::str n = py::re::sub(py::str("^_"), py::str(""), py::re::sub(py::str("@\\d+$"), py::str(""), m.group(3)));
        if (!syms.count(s)) sym_order.push_back(s);
        syms[s].push_back({o, n});
    }
    std::map<SegOffKey, py::str> ours_at;
    for (int64_t s : sym_order) {
        auto& lst = syms[s];
        std::stable_sort(lst.begin(), lst.end(), [](const auto& a, const auto& b) { return a.first < b.first || (a.first == b.first && a.second < b.second); });
        for (const auto& on : lst) ours_at[{s, on.first}] = on.second;
    }
    auto ours_name = [&](const CalleeKey& key) -> std::optional<py::str> {
        if (key.kind == "far") {
            // (the build's own map first: the describer names a function of the build by where it is in the build -- SETUPX's allocator
            // sub_24_0F1E was "sub_24_052E" there, no call to it compared, and sub_3_7E58 calling it with no size was never seen. An import
            // has no place in the map: its name is the describer's)
            auto w = describe::where(key.a);
            auto it = ours_at.find({std::get<0>(w), std::get<1>(w)});
            if (it != ours_at.end() && !it->second.empty()) return it->second;
            auto x = xo_pu.names.find(key.a);
            if (x != xo_pu.names.end() && !x->second.empty()) return x->second;
            return std::nullopt;
        }
        auto it = ours_at.find({key.a, key.b});
        if (it == ours_at.end()) return std::nullopt;
        return it->second;
    };

    py::set<py::str> wanted;
    for (const py::str& n : names) wanted.add(n);
    py::dict<py::str, py::str> out;
    auto removes_o = removes_of(original_d);
    auto removes_u = removes_of(ours_d);
    for (int64_t s : sym_order) {
        auto& lst = syms[s];
        py::bytes img = ours_d.d->images.get(s, py::bytes());
        for (size_t k = 0; k < lst.size(); k++) {
            int64_t o = lst[k].first;
            const py::str& n = lst[k].second;
            if (!wanted.contains(n) || !orig_fn.count(n)) continue;
            int64_t hi = k + 1 < lst.size() ? lst[k + 1].first : img.size();
            auto [os_, oo, oe] = orig_fn[n];
            std::map<py::str, std::set<TakenOff>> a, b;
            std::map<py::str, std::set<int64_t>> pa, pb;
            std::map<py::str, int64_t> low;   // (the original's at-least counts)
            py::bytes oimg = original_d.d->images.get(os_, py::bytes());
            for (const CallRow& row : _calls(oimg, os_, oo, oe, orig_name, removes_o)) {
                const py::str& c = std::get<0>(row);
                a[c].insert(std::get<3>(row) ? TakenOff(py::str("leave")) : std::get<1>(row));
                const auto& p = std::get<2>(row);
                if (!p) continue;
                if (*p >= 0) pa[c].insert(*p);
                else low[c] = std::max(low[c], -*p);
            }
            for (const CallRow& row : _calls(img, s, o, hi, ours_name, removes_u)) {
                const py::str& c = std::get<0>(row);
                b[c].insert(std::get<3>(row) ? TakenOff(py::str("leave")) : std::get<1>(row));
                const auto& p = std::get<2>(row);
                if (p && *p >= 0) pb[c].insert(*p);   // (ours counted from a join: not compared)
            }
            std::vector<py::str> common;
            for (const auto& kv : a) if (b.count(kv.first)) common.push_back(kv.first);
            std::sort(common.begin(), common.end());
            for (const py::str& c : common) {
                if (_IN_REGISTERS.contains(c)) {
                    out.set(n, py::str("it calls " + c.utf8() + ", which takes its arguments in registers"));
                    break;
                }
                std::set<TakenOff> ta = a[c], tb = b[c];   // (before a LEAVE: whatever was taken off)
                ta.erase(TakenOff(py::str("leave")));
                tb.erase(TakenOff(py::str("leave")));
                if (!ta.empty() && !tb.empty() && ta != tb) {
                    // (each set may hold "leave" beside the byte counts -- and a count read from a register, "?bx": sorted as text, or the
                    // report itself fails. It did, on every USER build after the LEAVE rule came in, and with it the whole check)
                    out.set(n, py::str("after calling " + c.utf8() + " the original takes off " + repr_taken(a[c]) + " bytes, ours " + repr_taken(b[c])));
                    break;
                }
                // (and fewer argument bytes pushed than the original pushes: SUWIN's sub_8_0634 gave sub_8_0A6A one near pointer for its two
                // far ones -- the second, a 260-byte buffer the caller never reads, dropped -- and a path was written over its frame)
                // (or more: a call is the callee's, the same bytes from any caller -- unless the callee takes a varying number, `...`, as
                // wsprintf does)
                const std::set<int64_t>& pac = pa[c];
                const std::set<int64_t>& pbc = pb[c];
                if (!pac.empty() && !pbc.empty() &&
                    (*pbc.rbegin() < *pac.begin() || (pac != pbc && pac.size() == 1 && pbc.size() == 1 && !_VARIADIC.contains(c)))) {
                    out.set(n, py::str("it pushes " + repr_ints(pbc) + " argument bytes for " + c.utf8() + ", the original " + repr_ints(pac)));
                    break;
                }
            }
        }
    }
    return out;
}

}  // namespace codebreaker::verify::gate16::contract16
