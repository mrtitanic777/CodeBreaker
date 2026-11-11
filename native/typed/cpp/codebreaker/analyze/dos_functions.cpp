// codebreaker/analyze/dos_functions.cpp -- typed C++ of codebreaker.analyze.dos_functions (see dos_functions.hpp).

#include "codebreaker/analyze/dos_functions.hpp"
#include "codebreaker/emulate/capstone_x86.hpp"
#include "codebreaker/platforms/package.hpp"

#include <algorithm>
#include <cstring>
#include <string>
#include <unordered_set>

#include <capstone/capstone.h>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/symbols/dos_interrupts.hpp"
#include "py_stdlib.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#undef near   // (windows.h makes `near` and `far` empty macros; the module uses both as names)
#undef far
#endif

namespace codebreaker::analyze::dos_functions {

namespace dos = codebreaker::formats::dos;

const py::list<int64_t> _PREFIX = {0x26, 0x2E, 0x36, 0x3E, 0x64, 0x65, 0x66, 0x67, 0xF0, 0xF2, 0xF3};
const py::dict<py::str, std::tuple<py::str, py::str>> _HALVES = {
    {py::str("ax"), {py::str("al"), py::str("ah")}},
    {py::str("bx"), {py::str("bl"), py::str("bh")}},
    {py::str("cx"), {py::str("cl"), py::str("ch")}},
    {py::str("dx"), {py::str("dl"), py::str("dh")}}};
const py::dict<py::str, py::str> _HALF = {{py::str("al"), py::str("ax")}, {py::str("ah"), py::str("ax")},
                                          {py::str("bl"), py::str("bx")}, {py::str("bh"), py::str("bx")},
                                          {py::str("cl"), py::str("cx")}, {py::str("ch"), py::str("cx")},
                                          {py::str("dl"), py::str("dx")}, {py::str("dh"), py::str("dx")}};
const py::list<py::str> _WHOLE = {py::str("si"), py::str("di"), py::str("bp"), py::str("ds"), py::str("es")};
const py::list<py::str> _VOLATILE = {py::str("al"), py::str("ah"), py::str("bl"), py::str("bh"), py::str("cl"), py::str("ch"),
                                     py::str("dl"), py::str("dh"), py::str("si"), py::str("di"), py::str("bp"), py::str("es")};
const py::set<std::tuple<int64_t, std::optional<int64_t>>> _ENDS_PROGRAM = [] {
    py::set<std::tuple<int64_t, std::optional<int64_t>>> s;
    s.add({0x20, std::nullopt});
    s.add({0x27, std::nullopt});
    s.add({0x21, 0x4C});
    s.add({0x21, 0x00});
    s.add({0x21, 0x31});
    return s;
}();
const py::list<int64_t> _ODD = {0x00, 0x27, 0x2F, 0x37, 0x3F, 0x5C, 0x62, 0x63, 0x6C, 0x6D, 0x6E, 0x6F, 0xD6, 0xF1};

namespace {

// ---- register names, the way _HALVES / _HALF / _WHOLE / _VOLATILE state them ---------------------
// (std::string forms of the tables above: the hot paths avoid building a py::str per register)
bool is_halves_(const std::string& r) { return r == "ax" || r == "bx" || r == "cx" || r == "dx"; }
std::string lo_of_(const std::string& r) { return std::string(1, r[0]) + "l"; }
std::string hi_of_(const std::string& r) { return std::string(1, r[0]) + "h"; }
bool is_half_(const std::string& r) {
    return r == "al" || r == "ah" || r == "bl" || r == "bh" || r == "cl" || r == "ch" || r == "dl" || r == "dh";
}
bool is_whole_(const std::string& r) { return r == "si" || r == "di" || r == "bp" || r == "ds" || r == "es"; }

// ---- the 16-bit capstone decoder: platforms.decoder(16, detail=True) ------------------------------------
std::shared_ptr<platforms::Decoder> md_() { return platforms::decoder(16, py::str("x86"), true); }

// One operand: capstone keeps reg, imm and mem in one union; all three views are given as it holds them.
struct COperand {
    int64_t type = 0, reg = 0, imm = 0, size = 0;
    int64_t segment = 0, base = 0, index = 0, disp = 0;   // op.mem
};

// One decoded instruction (capstone's CsInsn).
struct Decoded {
    int64_t size = 0;
    std::string mnemonic;
    std::vector<COperand> ops;
    cs_insn insn{};
    cs_detail detail{};
};

// next(md.disasm(raw, address, 1), None)
bool disasm_one(const py::bytes& raw, uint64_t address, Decoded& out) {
    std::shared_ptr<platforms::Decoder> md = md_();
    size_t n = 0;
    cs_insn* insn = md->raw(reinterpret_cast<const uint8_t*>(raw.raw().data()), static_cast<size_t>(raw.size()), static_cast<int64_t>(address), 1, n);
    if (n == 0) return false;
    out.size = insn->size;
    out.mnemonic = insn->mnemonic;
    out.insn = *insn;
    out.detail = *insn->detail;
    out.insn.detail = &out.detail;
    out.ops.clear();
    const cs_x86& x = insn->detail->x86;
    for (int i = 0; i < x.op_count; ++i) {
        const cs_x86_op& o = x.operands[i];
        COperand c;
        c.type = o.type;
        c.reg = static_cast<int64_t>(static_cast<uint32_t>(o.reg));
        c.imm = o.imm;
        c.size = o.size;
        c.segment = static_cast<int64_t>(static_cast<uint32_t>(o.mem.segment));
        c.base = static_cast<int64_t>(static_cast<uint32_t>(o.mem.base));
        c.index = static_cast<int64_t>(static_cast<uint32_t>(o.mem.index));
        c.disp = o.mem.disp;
        out.ops.push_back(c);
    }
    md->release(insn, n);
    return true;
}

// got.reg_name(id): "" for None
std::string reg_name_(int64_t id) {
    const char* s = md_()->reg_name(static_cast<uint32_t>(id));
    return s ? std::string(s) : std::string();
}

// got.regs_access()[1]: the registers written
std::vector<int64_t> regs_written_(Decoded& d) {
    auto access = md_()->regs_access(d.insn);
    if (!access) throw py::RuntimeError("capstone: cs_regs_access failed");
    std::vector<int64_t> out;
    for (uint16_t r : std::get<1>(*access)) out.push_back(r);
    return out;
}

constexpr int64_t OP_REG = X86_OP_REG, OP_IMM = X86_OP_IMM, OP_MEM = X86_OP_MEM;

const py::bytes kPush55 = py::bytes("\x55\x8B\xEC", 3);
const py::bytes kPush55b = py::bytes("\x55\x89\xE5", 3);
const py::bytes kEnter = py::bytes("\xC8", 1);

bool in_prefix_(int64_t b) {
    return b == 0x26 || b == 0x2E || b == 0x36 || b == 0x3E || b == 0x64 || b == 0x65 || b == 0x66 || b == 0x67 || b == 0xF0 ||
           b == 0xF2 || b == 0xF3;
}
bool in_odd_(int64_t b) {
    return b == 0x00 || b == 0x27 || b == 0x2F || b == 0x37 || b == 0x3F || b == 0x5C || b == 0x62 || b == 0x63 || b == 0x6C ||
           b == 0x6D || b == 0x6E || b == 0x6F || b == 0xD6 || b == 0xF1;
}

// (number, ah) in _ENDS_PROGRAM
bool ends_program_(int64_t number, const std::optional<int64_t>& ah) {
    if (!ah) return number == 0x20 || number == 0x27;
    return number == 0x21 && (*ah == 0x4C || *ah == 0x00 || *ah == 0x31);
}

std::optional<int64_t> reg_get_(const Regs& regs, const std::string& r) {
    auto it = regs.consts.find(r);
    if (it == regs.consts.end()) return std::nullopt;
    return it->second;
}

[[noreturn]] void none_times_(const char* what) { throw py::TypeError(std::string("unsupported operand type(s) for ") + what); }

}  // namespace

void set_capstone_dll(const py::str& path) { emulate::capstone_x86::load_capstone(path); }

// ---- Function, Interrupt -------------------------------------------------------------------------

Function::Function(int64_t address, const py::str& name, const py::str& distance, const py::str& evidence, int64_t cs, bool sure)
    : address(address), name(name), distance(distance), evidence(evidence), cs(cs), sure(sure) {}

py::str Function::repr() const {
    return py::percent(py::str("Function(%s at %#x, %s, %s)"), name, address, distance, evidence);
}

Interrupt::Interrupt(int64_t address, int64_t number, const std::optional<int64_t>& ah, const std::optional<int64_t>& al,
                     const py::ref<Function>& function)
    : address(address), number(number), ah(ah), al(al), function(function) {}

py::str flow_name(Flow f) {
    switch (f) {
        case Flow::Next: return "next";
        case Flow::Call: return "call";
        case Flow::Jump: return "jump";
        case Flow::Branch: return "branch";
        case Flow::Stop: return "stop";
    }
    return "";
}

// ---- Analysis ------------------------------------------------------------------------------------

Analysis::Analysis(const dos::Program& p) : program(p) {
    mark_.assign(static_cast<size_t>(p.image.size()), 0);    // 1 instruction start, 2 inside one, 3 jump table
    guess_.assign(static_cast<size_t>(p.image.size()), 0);   // reached only from a guessed function start
    relocs_ = py::set<int64_t>();
    for (int64_t r : p.relocations) relocs_.add(r);
}

uint8_t& Analysis::m_(int64_t i) {
    int64_t n = static_cast<int64_t>(mark_.size());
    int64_t j = i < 0 ? i + n : i;
    if (j < 0 || j >= n) throw py::IndexError("bytearray index out of range");
    return mark_[static_cast<size_t>(j)];
}

uint8_t Analysis::g_(int64_t i) const {
    int64_t n = static_cast<int64_t>(guess_.size());
    int64_t j = i < 0 ? i + n : i;
    if (j < 0 || j >= n) throw py::IndexError("bytearray index out of range");
    return guess_[static_cast<size_t>(j)];
}

int64_t Analysis::ds_linear_() const {
    if (!program.ds) none_times_("*: 'NoneType' and 'int'");
    return *program.ds * 16;
}

py::bytes Analysis::_mark_bytes() const { return py::bytes(std::string(mark_.begin(), mark_.end())); }
py::bytes Analysis::_guess_bytes() const { return py::bytes(std::string(guess_.begin(), guess_.end())); }

std::tuple<int64_t, int64_t> Analysis::_cache_counts() const {
    int64_t n = 0;
    for (const auto& kv : cache_) n += kv.second.has_value() ? 1 : 0;
    return {static_cast<int64_t>(cache_.size()), n};
}

// reading instructions ----------------------------------------------------------------------
std::optional<Ins> Analysis::_decode(int64_t va, int64_t cs) {
    auto cached = cache_.find(va);
    if (cached != cache_.end()) return cached->second;
    const dos::Program& p = program;
    py::bytes raw = p.inside(va) ? p.image.slice(va - p.base, va - p.base + 16) : py::bytes();
    Decoded got;
    bool have = false;
    {
        py::bytes head = raw.slice(0, 2);
        bool skip = head == py::bytes("\x00\x00", 2) || head == py::bytes("\xFF\xFF", 2) || head.empty();
        if (!skip) have = disasm_one(raw, static_cast<uint64_t>(va & 0xFFFF), got);
    }
    std::optional<Ins> ins;
    if (have && p.inside(va + got.size - 1)) {
        Ins r;
        r.size = got.size;
        r.flow = Flow::Next;
        r.far = false;
        r.cs = cs;
        int64_t k = 0;
        while (k < got.size - 1 && in_prefix_(raw[k])) k += 1;
        int64_t op = raw[k], nxt = va + got.size;
        bool odd = in_odd_(op);
        if (!odd) {
            for (int64_t i = 0; i < k; ++i) {
                if (raw[i] == 0x64 || raw[i] == 0x65) odd = true;
            }
        }
        if (!odd) odd = op == 0x0F && raw[k + 1] < 0x10;
        r.odd = odd;
        auto near = [&](int64_t rel) -> int64_t {  // stays inside the 64 KB of its segment
            int64_t t = nxt + rel;
            return (cs <= t && t < cs + 0x10000) ? t : cs + ((t - cs) & 0xFFFF);
        };
        if (op == 0xE8) {
            r.flow = Flow::Call;
            r.targets = {near(py::unpack_from1<int64_t>("<h", raw, k + 1))};
        } else if (op == 0xE9) {
            r.flow = Flow::Jump;
            r.targets = {near(py::unpack_from1<int64_t>("<h", raw, k + 1))};
        } else if (op == 0xEB) {
            r.flow = Flow::Jump;
            r.targets = {near(py::unpack_from1<int64_t>("<b", raw, k + 1))};
        } else if ((0x70 <= op && op <= 0x7F) || (0xE0 <= op && op <= 0xE3)) {
            r.flow = Flow::Branch;
            r.targets = {near(py::unpack_from1<int64_t>("<b", raw, k + 1))};
        } else if (op == 0x0F && 0x80 <= raw[k + 1] && raw[k + 1] <= 0x8F) {
            r.flow = Flow::Branch;
            r.targets = {near(py::unpack_from1<int64_t>("<h", raw, k + 2))};
        } else if (op == 0x9A || op == 0xEA) {
            auto [off, seg] = py::unpack_from<int64_t, int64_t>("<HH", raw, k + 1);
            r.flow = op == 0x9A ? Flow::Call : Flow::Jump;
            r.far = true;
            r.targets = {seg * 16 + off};
            if (!relocs_.empty() && !relocs_.contains(va + k + 3)) r.targets.clear();  // an absolute segment: not this program
        } else if (op == 0xC3 || op == 0xC2 || op == 0xCB || op == 0xCA || op == 0xCF) {
            r.flow = Flow::Stop;
            py::str kind = op == 0xCF ? py::str("iret") : (op == 0xC3 || op == 0xC2) ? py::str("near") : py::str("far");
            int64_t removed = (op == 0xC2 || op == 0xCA) ? py::unpack_from1<int64_t>("<H", raw, k + 1) : 0;
            r.ret = std::make_tuple(kind, removed);
        } else if (op == 0xCD || op == 0xCC || op == 0xCE) {
            r.number = op == 0xCD ? raw[k + 1] : op == 0xCC ? 3 : 4;
        } else if (op == 0xF4) {
            r.flow = Flow::Stop;
        } else if (op == 0xFF && ((raw[k + 1] >> 3) & 7) >= 2 && ((raw[k + 1] >> 3) & 7) <= 5) {
            int64_t kind = (raw[k + 1] >> 3) & 7;
            r.flow = (kind == 2 || kind == 3) ? Flow::Call : Flow::Jump;
            if ((kind == 2 || kind == 4) && got.ops.size() == 1 && got.ops[0].type == OP_MEM) {
                const COperand& m = got.ops[0];
                bool indexed = m.index != 0 || m.base == X86_REG_BX || m.base == X86_REG_SI || m.base == X86_REG_DI;
                if (indexed && m.disp != 0 && (m.segment == 0 || m.segment == X86_REG_CS || m.segment == X86_REG_DS)) {
                    r.table = (m.segment == X86_REG_CS ? cs : ds_linear_()) + (m.disp & 0xFFFF);
                }
            }
            if (got.ops.size() == 1 && got.ops[0].type == OP_MEM) {  // through one pointer variable
                const COperand& m = got.ops[0];
                if (m.index == 0 && m.base == 0 && (m.segment == 0 || m.segment == X86_REG_CS || m.segment == X86_REG_DS)) {
                    r.variable = (m.segment == X86_REG_CS ? cs : ds_linear_()) + (m.disp & 0xFFFF);
                    r.far = kind == 3 || kind == 5;
                }
            }
        }
        ins = r;
    }
    cache_[va] = ins;
    return ins;
}

bool Analysis::_looks_like_code(int64_t va, int64_t cs, int64_t limit, bool strict) {
    int64_t base = program.base;
    std::unordered_set<int64_t> seen;
    std::vector<int64_t> work{va};
    bool ended = false;
    if (strict) {
        py::bytes head = program.image.slice(va - base, va - base + 8);
        bool all_text = true;
        for (int64_t i = 0; i < head.size(); ++i) {
            if (!(0x20 <= head.byte_(i) && head.byte_(i) < 0x7F)) all_text = false;
        }
        if (all_text) return false;
    }
    while (!work.empty() && static_cast<int64_t>(seen.size()) < limit) {
        int64_t a = work.back();
        work.pop_back();
        while (seen.find(a) == seen.end()) {
            std::optional<Ins> ins = _decode(a, cs);
            if (!ins || m_(a - base) == 2 || m_(a - base) == 3 || (strict && ins->odd)) return false;
            seen.insert(a);
            if (m_(a - base) == 1) {
                ended = true;
                break;
            }
            if (ins->flow == Flow::Stop || (ins->number && ends_program_(*ins->number, std::nullopt))) {
                ended = ended || ins->ret.has_value() || ins->number.has_value();
                break;
            }
            if ((ins->flow == Flow::Jump || ins->flow == Flow::Branch) && !ins->far) {
                for (int64_t t : ins->targets) {
                    if (!program.inside(t)) return false;
                    work.push_back(t);
                }
            }
            if (ins->flow == Flow::Jump) {
                ended = ended || (ins->targets.empty() && !strict);
                break;
            }
            a += ins->size;
        }
    }
    return ended && static_cast<int64_t>(seen.size()) >= (strict ? 2 : 3);
}

// following a function -----------------------------------------------------------------------
py::ref<Function> Analysis::_add(int64_t va, const py::str& distance, const py::str& evidence, int64_t cs,
                                 const std::optional<py::str>& name, bool sure) {
    if (functions.contains(va) || !program.inside(va) || m_(va - program.base) == 2 || m_(va - program.base) == 3) return nullptr;
    py::str nm = (name && !name->empty()) ? *name : py::percent(py::str("sub_%05X"), va - program.base);
    py::ref<Function> f = std::make_shared<Function>(va, nm, distance, evidence, cs, sure);
    functions.set(va, f);
    fn_set_.insert(va);
    queue_.push_back(f);
    return f;
}

py::list<int64_t> Analysis::_table(int64_t at, int64_t cs) {
    const dos::Program& p = program;
    py::list<int64_t> out;
    int64_t stop = at + 512;
    while (at + 2 <= stop && p.inside(at + 1) && (m_(at - p.base) == 0 || m_(at - p.base) == 3) &&
           (m_(at + 1 - p.base) == 0 || m_(at + 1 - p.base) == 3)) {
        std::optional<int64_t> w = p.word(at);
        if (!w) none_times_("+: 'int' and 'NoneType'");
        int64_t t = cs + *w;
        bool known = _labels.contains(t) || functions.contains(t) || (p.inside(t) && m_(t - p.base) == 1);
        if (!p.inside(t) || t == cs || t == at || !(known || _looks_like_code(t, cs))) break;
        out.append(t);
        stop = t > at ? std::min(stop, t) : stop;
        at += 2;
    }
    return out;
}

int64_t Analysis::_far_cs(int64_t va, const Ins& ins) const {
    py::bytes raw = program.image.slice(va - program.base, va - program.base + ins.size);
    return py::unpack_from1<int64_t>("<H", raw, ins.size - 2) * 16;
}

std::optional<int64_t> Analysis::_word(const Regs& regs, const py::str& reg) {
    std::string r = reg.utf8();
    std::optional<int64_t> lo = reg_get_(regs, lo_of_(r)), hi = reg_get_(regs, hi_of_(r));
    if (!lo || !hi) return std::nullopt;
    return (*hi << 8) | *lo;
}

void Analysis::_follow(const py::ref<Function>& fp) {
    Function& f = *fp;
    const dos::Program& p = program;
    bool guess = !f.sure;
    std::unordered_set<int64_t> seen;
    std::vector<int64_t> work{f.address};
    std::vector<std::tuple<py::str, int64_t>> returns;
    while (!work.empty()) {
        int64_t a = work.back();
        work.pop_back();
        Regs regs;
        while (seen.find(a) == seen.end() && p.inside(a)) {
            std::optional<Ins> insn = _decode(a, f.cs);
            int64_t i = a - p.base;
            if (!insn || m_(i) == 2 || m_(i) == 3) break;  // runs into data, or into the middle of code
            const Ins& ins = *insn;
            seen.insert(a);
            if (m_(i) == 0) {
                m_(i) = 1;
                for (int64_t j = i + 1; j < i + ins.size; ++j) m_(j) = 2;
                if (guess) {
                    for (int64_t j = i; j < i + ins.size && j < static_cast<int64_t>(guess_.size()); ++j) guess_[static_cast<size_t>(j)] = 1;
                }
            } else if (!guess && g_(i)) {
                for (int64_t j = i; j < i + ins.size && j < static_cast<int64_t>(guess_.size()); ++j) guess_[static_cast<size_t>(j)] = 0;
            }
            bool ends = false;
            if (ins.number) {
                sites_[a] = *ins.number;
                std::optional<int64_t> ds = reg_get_(regs, "ds");
                std::optional<int64_t> off = _word(regs, py::str("dx"));
                std::optional<int64_t> ah = reg_get_(regs, "ah"), al = reg_get_(regs, "al");
                if (*ins.number == 0x21 && ah == 0x25 && al && ds && off) {
                    if (!vectors.contains(*ds * 16 + *off)) vectors.set(*ds * 16 + *off, *al);
                }
                ends = ends_program_(*ins.number, *ins.number == 0x21 ? ah : std::nullopt);
            }
            std::optional<std::tuple<int64_t, int64_t>> vector = _constants(a, ins, regs);
            if (vector) {
                if (!vectors.contains(std::get<0>(*vector))) vectors.set(std::get<0>(*vector), std::get<1>(*vector));
            }
            if (ins.ret) returns.push_back(*ins.ret);
            if (ins.table && !tables.contains(*ins.table) && p.inside(*ins.table)) {
                py::list<int64_t> targets = _table(*ins.table, f.cs);
                if (targets.size() >= 2) {
                    tables.set(*ins.table, targets.size());
                    for (int64_t j = *ins.table - p.base; j < *ins.table - p.base + 2 * targets.size(); ++j) m_(j) = 3;
                    if (ins.flow == Flow::Jump) {
                        _labels.update(targets);
                        for (int64_t t : targets) work.push_back(t);
                    } else {
                        for (int64_t t : targets) _add(t, py::str("near"), py::str("table"), f.cs, std::nullopt, f.sure);
                    }
                }
            }
            if (ins.variable && p.inside(*ins.variable + (ins.far ? 3 : 1)) && m_(*ins.variable - p.base) == 0) {
                int64_t at = *ins.variable;
                int64_t t, cs;
                if (!ins.far) {
                    std::optional<int64_t> w = p.word(at);
                    if (!w) none_times_("+: 'int' and 'NoneType'");
                    t = f.cs + *w;
                    cs = f.cs;
                } else {
                    std::optional<int64_t> w2 = p.word(at + 2);
                    if (!w2) none_times_("*: 'NoneType' and 'int'");
                    cs = *w2 * 16;
                    if (relocs_.empty() || relocs_.contains(at + 2)) {
                        std::optional<int64_t> w = p.word(at);
                        if (!w) none_times_("+: 'int' and 'NoneType'");
                        t = cs + *w;
                    } else {
                        t = -1;
                    }
                }
                if (t != cs && p.inside(t) && (m_(t - p.base) == 1 || _looks_like_code(t, cs))) {
                    _add(t, ins.far ? py::str("far") : py::str("near"), py::str("variable"), cs, std::nullopt, f.sure);
                }
            }
            if (ins.flow == Flow::Call) {
                for (int64_t t : ins.targets) {
                    if (t != a + ins.size) {  // `call $+3` only fetches the address
                        _add(t, ins.far ? py::str("far") : py::str("near"), py::str("called"), ins.far ? _far_cs(a, ins) : f.cs,
                             std::nullopt, f.sure);
                    }
                }
            } else if (ins.flow == Flow::Jump || ins.flow == Flow::Branch) {
                for (int64_t t : ins.targets) {
                    if (ins.far) {
                        _add(t, py::str("far"), py::str("called"), _far_cs(a, ins), std::nullopt, f.sure);
                    } else if (p.inside(t) && !functions.contains(t)) {
                        _labels.add(t);
                        work.push_back(t);
                    }
                }
            }
            if (ins.flow == Flow::Stop || ins.flow == Flow::Jump || ends) break;
            a += ins.size;
        }
    }
    int64_t total = 0;
    for (int64_t a : seen) total += cache_.at(a)->size;
    f.size = total;
    int64_t far = 0;
    for (const auto& r : returns) {
        if (std::get<0>(r) != "near") far += 1;
    }
    int64_t near = static_cast<int64_t>(returns.size()) - far;
    if (far != near) f.distance = far > near ? py::str("far") : py::str("near");
    bool any_iret = false;
    for (const auto& r : returns) {
        if (std::get<0>(r) == "iret") any_iret = true;
    }
    f.interrupt = any_iret && near == 0;
    py::set<int64_t> removed;
    for (const auto& r : returns) {
        if ((std::get<0>(r) != "near") == (f.distance == "far")) removed.add(std::get<1>(r));
    }
    f.removes = removed.size() == 1 ? *removed.begin() : -1;
}

// constants in registers, along a straight run of instructions -----------------------------------
std::optional<std::tuple<int64_t, int64_t>> Analysis::_constants(int64_t va, const Ins& ins, Regs& regs) {
    const dos::Program& p = program;
    py::bytes raw = p.image.slice(va - p.base, va - p.base + ins.size);
    Decoded got;
    if (!disasm_one(raw, static_cast<uint64_t>(va & 0xFFFF), got)) throw py::StopIteration();
    const std::string& m = got.mnemonic;
    std::vector<COperand>& ops = got.ops;

    auto put = [&](const std::string& reg, std::optional<int64_t> value) {
        if (is_halves_(reg)) {
            std::string lo = lo_of_(reg), hi = hi_of_(reg);
            if (!value) {
                regs.consts.erase(lo);
                regs.consts.erase(hi);
            } else {
                regs.consts[lo] = *value & 0xFF;
                regs.consts[hi] = (*value >> 8) & 0xFF;
            }
        } else if (is_whole_(reg) || is_half_(reg)) {
            if (!value) regs.consts.erase(reg);
            else regs.consts[reg] = *value;
        }
    };
    auto get = [&](const std::string& reg) -> std::optional<int64_t> {
        if (is_halves_(reg)) return _word(regs, py::str(reg));
        if (reg == "cs") return ins.cs >> 4;
        return reg_get_(regs, reg);
    };

    if (m == "push" && ops.size() == 1) {
        if (ops[0].type == OP_REG) regs.pushed = get(reg_name_(ops[0].reg));
        else if (ops[0].type == OP_IMM) regs.pushed = ops[0].imm & 0xFFFF;
        else regs.pushed = std::nullopt;
        return std::nullopt;
    }
    std::optional<int64_t> pushed = regs.pushed;
    regs.pushed = std::nullopt;
    if (m == "pop" && ops.size() == 1 && ops[0].type == OP_REG) {
        put(reg_name_(ops[0].reg), pushed);
        return std::nullopt;
    }
    if (ins.flow == Flow::Call || ins.number) {
        for (const py::str& r : _VOLATILE) regs.consts.erase(r.utf8());
        return std::nullopt;
    }
    if (ops.size() == 2 && ops[0].type == OP_REG) {
        std::string dst = reg_name_(ops[0].reg);
        const COperand& src = ops[1];
        if (m == "mov" && src.type == OP_IMM) {
            put(dst, src.imm & (ops[0].size == 1 ? 0xFF : 0xFFFF));
            return std::nullopt;
        }
        if (m == "mov" && src.type == OP_REG) {
            put(dst, get(reg_name_(src.reg)));
            return std::nullopt;
        }
        if (m == "lea" && src.base == 0 && src.index == 0) {
            put(dst, src.disp & 0xFFFF);
            return std::nullopt;
        }
        if ((m == "xor" || m == "sub") && src.type == OP_REG && src.reg == ops[0].reg) {
            put(dst, 0);
            return std::nullopt;
        }
    }
    if (m == "mov" && ops.size() == 2 && ops[0].type == OP_MEM && ops[0].size == 2 && ops[0].base == 0 && ops[0].index == 0) {
        const char* seg = ops[0].segment == X86_REG_ES ? "es" : ops[0].segment == 0 ? "ds" : nullptr;
        int64_t at = ops[0].disp & 0xFFFF;
        std::optional<int64_t> segv = seg ? reg_get_(regs, seg) : std::nullopt;
        if (seg && segv == 0 && at < 0x400) {
            std::optional<int64_t> value = ops[1].type == OP_IMM ? std::optional<int64_t>(ops[1].imm & 0xFFFF) : get(reg_name_(ops[1].reg));
            if (!value) regs.table.erase(at);
            else regs.table[at] = *value;
            auto prev = regs.table.find(at - 2);
            if (at % 4 == 2 && prev != regs.table.end() && value) {
                return std::make_tuple(*value * 16 + prev->second, at / 4);
            }
        }
        return std::nullopt;
    }
    for (int64_t r : regs_written_(got)) {
        std::string name = reg_name_(r);
        put((name.size() > 0 && name[0] == 'e' && is_halves_(name.substr(1))) ? name.substr(1) : name, std::nullopt);
    }
    return std::nullopt;
}

// the whole program ----------------------------------------------------------------------------
Analysis& Analysis::run() {
    const dos::Program& p = program;
    for (const auto& e : p.entries) {
        int64_t va = std::get<0>(e);
        int64_t seg = p.segments.get(va, (va >> 4) & 0xF000);
        _add(va, std::get<2>(e), py::str("entry"), seg * 16, std::get<1>(e));
    }
    _drain();
    _guesses();
    _name_interrupts();
    return *this;
}

void Analysis::_drain() {
    while (!queue_.empty()) {
        while (!queue_.empty()) {
            py::ref<Function> f = queue_.back();
            queue_.pop_back();
            _follow(f);
        }
        std::vector<std::tuple<int64_t, int64_t>> vs = vectors.items();
        std::sort(vs.begin(), vs.end());
        for (const auto& kv : vs) {  // handlers the code just read installs
            int64_t va = std::get<0>(kv), number = std::get<1>(kv);
            if (!functions.contains(va) && program.inside(va)) {
                int64_t cs = _segment_of(va);
                if (_looks_like_code(va, cs)) {
                    _add(va, py::str("far"), py::str("vector"), cs,
                         py::percent(py::str("int_%02X_handler_%05X"), number, va - program.base));
                }
            }
        }
    }
}

int64_t Analysis::_segment_of(int64_t va) {
    auto it = fn_set_.upper_bound(va);   // the nearest function at or before va
    if (it != fn_set_.begin()) {
        --it;
        int64_t best = *it;
        const Function& f = *functions.at(best);
        if (va - f.cs < 0x10000) return f.cs;
    }
    return va & ~static_cast<int64_t>(0xF);
}

void Analysis::_guesses() {
    const dos::Program& p = program;
    py::set<int64_t> tried;
    while (true) {
        int64_t before = functions.size();
        std::unordered_set<int64_t> bases;
        for (const auto& f : functions.values()) bases.insert(f->cs);
        for (int64_t site : p.relocations) {  // far pointers in data
            int64_t i = site - p.base;
            if (i < 2 || m_(i) == 1 || m_(i) == 2 || m_(i - 2) == 1 || m_(i - 2) == 2) continue;
            std::optional<int64_t> w1 = p.word(site), w2 = p.word(site - 2);
            if (!w1) none_times_("*: 'NoneType' and 'int'");
            int64_t seg = *w1;
            if (!w2) none_times_("*: 'NoneType' and 'int'");
            int64_t off = *w2;
            int64_t t = seg * 16 + off;
            if (bases.count(seg * 16) && p.inside(t) && !functions.contains(t) && m_(t - p.base) == 0 &&
                _looks_like_code(t, seg * 16)) {
                _add(t, py::str("far"), py::str("pointer"), seg * 16, std::nullopt, false);
            }
        }
        _drain();
        for (const auto& span : p.spans) {  // prologues in bytes nothing reaches
            int64_t lo = std::get<0>(span), n = std::get<1>(span);
            int64_t i = lo - p.base, end = lo - p.base + n;
            while (true) {
                int64_t j = -1;
                for (int64_t x : {p.image.find(kPush55, i, end), p.image.find(kPush55b, i, end), p.image.find(kEnter, i, end)}) {
                    if (x >= 0 && (j < 0 || x < j)) j = x;
                }
                if (j < 0) break;
                i = j + 1;
                int64_t va = p.base + j;
                if (m_(j) != 0) continue;
                if (p.image[j] == 0xC8) {
                    bool bad = p.image.slice(j + 3, j + 4) != py::bytes("\x00", 1) || (p.image[j + 1] & 1) || p.image[j + 2] >= 0x10;
                    if (bad) continue;  // `enter`: an even frame under 4 KB, no nesting
                }
                int64_t cs = _segment_of(va);
                if (_looks_like_code(va, cs)) {
                    _add(va, py::str("near"), py::str("prologue"), cs, std::nullopt, false);
                    _drain();
                }
            }
        }
        _after_returns(tried);
        if (functions.size() == before) break;
    }
}

void Analysis::_after_returns(py::set<int64_t>& tried) {
    const dos::Program& p = program;
    while (true) {
        std::vector<int64_t> ends;
        for (const auto& kv : cache_) {
            const std::optional<Ins>& ins = kv.second;
            if (ins && (ins->flow == Flow::Stop || ins->flow == Flow::Jump) && m_(kv.first - p.base) == 1) {
                ends.push_back(kv.first + ins->size);
            }
        }
        std::sort(ends.begin(), ends.end());
        bool found = false;
        for (int64_t va : ends) {
            int64_t stop = va + 4;
            while (va < stop && p.inside(va) && m_(va - p.base) == 0 &&
                   (p.image[va - p.base] == 0x90 || p.image[va - p.base] == 0xCC || p.image[va - p.base] == 0)) {
                va += 1;  // padding between routines
            }
            if (tried.contains(va) || !p.inside(va) || m_(va - p.base) != 0 || functions.contains(va)) continue;
            tried.add(va);
            int64_t cs = _segment_of(va);
            if (_looks_like_code(va, cs, 1500, true)) {
                _add(va, py::str("near"), py::str("follows"), cs, std::nullopt, false);
                _drain();
                found = true;
            }
        }
        if (!found) break;
    }
}

void Analysis::_name_interrupts() {
    const dos::Program& p = program;
    std::vector<int64_t> starts;
    for (int64_t k : functions.keys()) starts.push_back(k);
    std::sort(starts.begin(), starts.end());
    std::unordered_map<int64_t, int64_t> before;   // instruction -> the one that falls into it
    for (const auto& kv : cache_) {
        const std::optional<Ins>& ins = kv.second;
        if (ins && ins->flow == Flow::Next && !ins->number && m_(kv.first - p.base) == 1) before[kv.first + ins->size] = kv.first;
    }
    for (const auto& site : sites_) {
        int64_t va = site.first;
        std::vector<int64_t> run;
        int64_t a = va;
        while (!_labels.contains(a) && !functions.contains(a) && before.count(a) && run.size() < 64) {
            a = before[a];
            run.push_back(a);
        }
        Regs regs;
        for (auto it = run.rbegin(); it != run.rend(); ++it) _constants(*it, *cache_.at(*it), regs);
        int64_t k = static_cast<int64_t>(std::upper_bound(starts.begin(), starts.end(), va) - starts.begin()) - 1;
        interrupts.append(Interrupt(va, site.second, reg_get_(regs, "ah"), reg_get_(regs, "al"),
                                    k >= 0 ? functions.at(starts[static_cast<size_t>(k)]) : py::ref<Function>()));
    }
}

py::dict<py::str, int64_t> Analysis::census() const {
    const dos::Program& p = program;
    py::dict<py::str, int64_t> out;
    out.set(py::str("code"), 0);
    out.set(py::str("guessed code"), 0);
    out.set(py::str("jump tables"), 0);
    out.set(py::str("not classified"), 0);
    out.set(py::str("total"), 0);
    int64_t code = 0, guessed = 0, tab = 0, none = 0, total = 0;
    for (const auto& span : p.spans) {
        int64_t lo = std::get<0>(span), n = std::get<1>(span);
        for (int64_t i = lo - p.base; i < lo - p.base + n; ++i) {
            int64_t m = mark_.at(static_cast<size_t>(i));
            total += 1;
            if (m == 3) tab += 1;
            else if (m == 0) none += 1;
            else if (guess_.at(static_cast<size_t>(i))) guessed += 1;
            else code += 1;
        }
    }
    out.set(py::str("code"), code);
    out.set(py::str("guessed code"), guessed);
    out.set(py::str("jump tables"), tab);
    out.set(py::str("not classified"), none);
    out.set(py::str("total"), total);
    return out;
}

Analysis discover(const dos::Program& program) {
    Analysis a(program);
    a.run();
    return a;
}

// (Python 3.13+ strips the docstring's common indentation, so main.__doc__ is this text)
const py::str MAIN_DOC =
    "python -m codebreaker dos-functions <program> [--all]\n"
    "\n"
    "The functions found in a real-mode program (a DOS .EXE or .COM, a device driver, IO.SYS):\n"
    "how many by which evidence, how much of the program is code, and the interrupt services it\n"
    "calls. --all lists every function.";

int64_t main(const py::list<py::str>& argv) {
    if (argv.empty()) {
        py::print(MAIN_DOC);
        return 2;
    }
    Analysis a = discover(dos::read(codebreaker::base::corpus::resolve(argv[0])));
    const dos::Program& p = a.program;
    py::str stored;
    if (p.packer) {
        stored = py::percent(py::str(", stored with %s (%s)"), *p.packer, p.unpacked ? py::str("unpacked") : py::str("NOT unpacked"));
    } else {
        stored = py::str("");
    }
    int64_t loaded = 0;
    for (const auto& s : p.spans) loaded += std::get<1>(s);
    py::print(py::percent(py::str("%s: %s%s, %d bytes loaded"), p.name, p.kind, stored, loaded));
    py::dict<py::str, int64_t> by;
    for (const auto& f : a.functions.values()) by.set(f->evidence, by.get(f->evidence, 0) + 1);
    std::vector<std::tuple<py::str, int64_t>> kv = by.items();
    std::sort(kv.begin(), kv.end());
    py::list<py::str> parts;
    for (const auto& e : kv) parts.append(py::percent(py::str("%d %s"), std::get<1>(e), std::get<0>(e)));
    int64_t guesses = 0;
    for (const auto& f : a.functions.values()) guesses += f->sure ? 0 : 1;
    py::print(py::percent(py::str("%d functions: %s; %d of them rest on a guess"), a.functions.size(), py::join(py::str(", "), parts),
                          guesses));
    py::list<py::str> cparts;
    for (const auto& e : a.census().items()) cparts.append(py::percent(py::str("%s %d"), std::get<0>(e), std::get<1>(e)));
    py::print(py::str("bytes: ") + py::join(py::str(", "), cparts));
    int64_t exact = 0;
    for (const Interrupt& i : a.interrupts) {
        if (codebreaker::symbols::dos_interrupts::name(i.number, i.ah, i.al).exact) exact += 1;
    }
    py::print(py::percent(py::str("%d interrupt calls, %d with the service named"), a.interrupts.size(), exact));
    for (const py::str& note : p.notes) py::print(py::str("note: ") + note);
    bool all = false;
    for (const py::str& x : argv) {
        if (x == "--all") all = true;
    }
    if (all) {
        std::vector<int64_t> vas = a.functions.keys();
        std::sort(vas.begin(), vas.end());
        for (int64_t va : vas) {
            const Function& f = *a.functions.at(va);
            py::print(py::percent(py::str("  %05X %-26s %-4s removes %2d  %5d bytes  %s%s"), va, f.name, f.distance, f.removes,
                                  f.size, f.evidence, f.sure ? py::str("") : py::str(" (guess)")));
        }
    }
    return 0;
}

}  // namespace codebreaker::analyze::dos_functions
