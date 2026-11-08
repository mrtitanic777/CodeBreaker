// codebreaker/emulate/windows16.cpp -- C++ port of codebreaker.emulate.windows16 (see the header).
//
// Run a function of a 16-bit Windows module.  The module's segments are laid out in protected mode, each behind a
// selector of its own, with its relocations applied; every import is a far address in a segment of landing places, so a
// call to Windows is seen, named, and returned from with the arguments it was given.  Memory is kept by the hooks of
// native_win16 (cbtrace.dll); the events (an import entered, a callee not run, an interrupt) are this module's.
//
// Not carried over: struct() / _load_symbols() (a reader of the SDK symbol JSON that nothing uses).  The Python
// mutates the caller's `context` (_code_bases); here the run keeps its own copy of it.

#include "codebreaker/emulate/windows16.hpp"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <map>
#include <set>

#include "codebreaker/emulate/api_models.hpp"
#include "codebreaker/emulate/capstone_x86.hpp"
#include "codebreaker/emulate/native_win16.hpp"
#include "codebreaker/emulate/unicorn_uc.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "py_os.hpp"

namespace codebreaker::emulate::windows16 {

using native_trace::UcError;
using unicorn_uc::x86_reg;

namespace {

int64_t R(const char* name) { return x86_reg(py::str(name)).value(); }

// ---- the GDT -------------------------------------------------------------------------------------------------------
uint8_t u8(int64_t v) { return static_cast<uint8_t>(v & 0xFF); }

// bytearray slice assignment table[a:a+n] = x (past the end: appended)
void put_slice(std::string& table, int64_t a, int64_t n, const std::string& x) {
    int64_t len = static_cast<int64_t>(table.size());
    int64_t lo = std::min(a, len), hi = std::min(a + n, len);
    table.replace(static_cast<size_t>(lo), static_cast<size_t>(hi - lo), x);
}

// Map the GDT, set GDTR.  segs: (base, limit, is_code). -> the selectors (index 0 = null).
//
// DENSE GDT: real NE/stack/sentinel segments occupy indices 1..k; EVERY other index up to _N_GDT is filled with a
// present descriptor pointing at a shared scratch segment. Win16 code routinely `pop ds`/`pop es`/`retf` to selectors we
// didn't assign (a caller's saved DS, a movable-segment selector, a null arg); with a sparse GDT those loads raise #GP
// and Unicorn aborts the whole trace. Making every selector present turns those into harmless scratch-segment accesses so
// the trace continues and the behavior is still captured.
py::list<int64_t> _build_harness(Uc& uc, const std::vector<std::tuple<int64_t, int64_t, bool>>& segs) {
    try {
        uc.mem_map(SCRATCH_BASE, 0x10000);
    } catch (const UcError&) {   // already mapped
    }
    py::bytes scratch = _desc(SCRATCH_BASE, 0xFFFF, false);   // present, data r/w
    std::string gdt(8, '\0');                                  // null descriptor at index 0
    py::list<int64_t> sels{0};
    for (const auto& s : segs) {
        sels.append(_sel(sels.size()));
        gdt += _desc(std::get<0>(s), std::get<1>(s), std::get<2>(s)).raw();
    }
    while (static_cast<int64_t>(gdt.size()) / 8 < _N_GDT) gdt += scratch.raw();   // pad every remaining index with scratch
    uc.mem_map(GDT_ADDR, ((static_cast<int64_t>(gdt.size()) + _PAGE - 1) / _PAGE) * _PAGE);
    uc.mem_write(GDT_ADDR, py::bytes(gdt));
    uc.reg_write_descriptor(R("GDTR"), std::make_tuple(int64_t(0), GDT_ADDR, static_cast<int64_t>(gdt.size()) - 1, int64_t(0)));
    return sels;
}

struct Loaded16 {   // what a module is once loaded
    py::dict<int64_t, int64_t> sels, seg_bases;
    int64_t stack_sel = 0, sent_sel = 0;
};

// Map every NE segment into its 64K GDT window, build the GDT (segments + stack + import sentinel), and load the segment
// bytes.  sels is indexed by NE segment number (1-based).
Loaded16 _load_ne(Uc& uc, const formats::ne::NEFile& nef) {
    auto segs = nef.segments();
    Loaded16 out;
    // The module's segments sit high in the GDT, past every selector a plain number given as an input names (20h, 60h,
    // 100h ...): at index n those numbers were selectors of the module's own code, and a function handed one as a far
    // pointer read its own instructions -- which differ between the original and a rebuild.
    std::vector<std::tuple<int64_t, int64_t, bool>> harness_list(static_cast<size_t>(_MODULE_SLOT - 1),
                                                                 std::make_tuple(SCRATCH_BASE, int64_t(0xFFFF), false));
    for (const auto& s : segs) {
        int64_t base = segment_base(s.idx);
        out.seg_bases.set(s.idx, base);
        uc.mem_map(base, 0x10000);
        py::bytes body = nef.data.slice(s.file_off, s.file_off + s.length);
        if (!body.empty()) uc.mem_write(base, body);
        harness_list.emplace_back(base, 0xFFFF, s.kind == py::str("CODE"));
    }
    // stack + import-sentinel segments, at the same selectors whatever the module's segment count: a build with another
    // number of segments had another stack selector, and a word equal to one side's SS was "SS" there and a number on
    // the other
    uc.mem_map(STACK_BASE, 0x10000);
    uc.mem_map(SENT_BASE, 0x10000);
    try {
        uc.mem_map(SCRATCH_BASE, 0x10000);
    } catch (const UcError&) {
    }
    harness_list[static_cast<size_t>(_FIXED_SLOT - 1)] = {STACK_BASE, 0xFFFF, false};
    harness_list[static_cast<size_t>(_FIXED_SLOT)] = {SENT_BASE, 0xFFFF, true};   // code: far calls/rets land here
    if (static_cast<int64_t>(harness_list.size()) > _N_GDT - 24)   // (the top indices are the heaps')
        throw py::ValueError(py::percent(py::str("%d segments: more than the GDT holds"), segs.size()));
    py::list<int64_t> sels_all = _build_harness(uc, harness_list);
    for (const auto& s : segs) out.sels.set(s.idx, sels_all[_MODULE_SLOT + s.idx - 1]);
    out.stack_sel = sels_all[_FIXED_SLOT];
    out.sent_sel = sels_all[_FIXED_SLOT + 1];
    return out;
}

// Return namer(import_record) -> 'MODULE.FUNC' using the NE module-ref table.  (The Win16 ordinal->name oracle the
// Python asks for is not there -- emulator._load() takes a module -- so an ordinal is always 'MODULE.@ord'.)
std::function<py::str(const formats::ne::Relocation&)> _import_namer(const formats::ne::NEFile& nef) {
    py::list<py::str> modrefs;
    try {
        modrefs = nef.module_refs();
    } catch (const py::Exception&) {
    }
    return [&nef, modrefs](const formats::ne::Relocation& r) -> py::str {
        int64_t mi = r.module.value_or(0);
        py::str mod = mi && 0 < mi && mi <= modrefs.size() ? modrefs[mi - 1] : py::str("?");
        if (r.kind == py::str("IMPORTORD")) {
            if (!r.ordinal) throw py::TypeError("%d format: a real number is required, not NoneType");
            return mod + py::str(".@") + py::str(std::to_string(*r.ordinal));
        }
        if (r.kind == py::str("IMPORTNAME")) {
            try {
                return mod + py::str(".") + nef.imported_name(r.name_off.value_or(0));
            } catch (const py::Exception&) {
                return mod + py::str(".?");
            }
        }
        return mod + py::str(".OSFIXUP");
    };
}

// The value of one of KERNEL's exports that is a plain number, or None.
std::optional<int64_t> _kernel_constant(const py::str& api) {
    py::str up = api.upper();
    int64_t dot = up.find(py::str("."));
    py::str mod = dot >= 0 ? up.slice(0, dot) : up;
    py::str fn = dot >= 0 ? up.slice_from(dot + 1) : py::str("");
    if (mod != py::str("KERNEL")) return std::nullopt;
    if (fn == py::str("@113") || fn == py::str("__AHSHIFT")) return 3;
    if (fn == py::str("@114") || fn == py::str("__AHINCR")) return 8;
    return std::nullopt;
}

int64_t rd16(Uc& uc, int64_t a) {
    py::bytes b = uc.mem_read(a, 2);
    return b[0] | (b[1] << 8);
}
void wr16(Uc& uc, int64_t a, int64_t v) {
    std::string s(2, '\0');
    s[0] = static_cast<char>(v & 0xFF);
    s[1] = static_cast<char>((v >> 8) & 0xFF);
    uc.mem_write(a, py::bytes(s));
}

void _patch_chain(Uc& uc, int64_t base, const formats::ne::Relocation& r, int64_t tsel, int64_t toff) {
    int64_t src = r.src;
    int64_t off = r.offset;
    auto put = [&](int64_t a) {
        if (src == 2) wr16(uc, a, tsel);                                  // SEGMENT selector
        else if (src == 5) wr16(uc, a, toff);                             // OFFSET 16
        else if (src == 3) { wr16(uc, a, toff); wr16(uc, a + 2, tsel); }  // POINTER far 32 (off:sel)
        else if (src == 0) uc.mem_write(a, py::bytes(std::string(1, static_cast<char>(toff & 0xFF))));   // LOBYTE
        else wr16(uc, a, toff);                                           // best-effort
    };
    if (r.additive) {
        int64_t a = base + off;
        if (src == 2) wr16(uc, a, (rd16(uc, a) + tsel) & 0xFFFF);
        else if (src == 3) { wr16(uc, a, (rd16(uc, a) + toff) & 0xFFFF); wr16(uc, a + 2, (rd16(uc, a + 2) + tsel) & 0xFFFF); }
        else wr16(uc, a, (rd16(uc, a) + toff) & 0xFFFF);
        return;
    }
    int64_t seen = 0;
    while (off != 0xFFFF && seen < 4096) {   // walk the fixup chain
        int64_t a = base + off;
        int64_t nxt = rd16(uc, a);           // next-link is the low 16 bits
        put(a);
        off = nxt;
        seen++;
    }
}

// Apply per-segment fixups. INTERNAL refs resolve to (selector:offset) in our GDT; external IMPORT refs get a unique
// sentinel far address (sent_sel:slot) recorded in `imports` for stubbing. Handles the NE source types we see in
// practice: 2=SEGMENT(selector), 5=OFFSET(16), 3=POINTER(far 32), 0=LOBYTE; walks non-additive chains, adds for additive.
using Imports = std::map<int64_t, Hit>;
void _apply_relocs(Uc& uc, const formats::ne::NEFile& nef, const Loaded16& ld, Imports& imports) {
    std::optional<py::list<formats::ne::Entry>> ents;
    auto namer = _import_namer(nef);
    for (const auto& s : nef.segments()) {
        int64_t base = ld.seg_bases.at(s.idx);
        for (const auto& r : nef.relocations(s)) {
            // resolve the target (selector, offset)
            int64_t tsel, toff;
            if (r.kind == py::str("INTERNAL")) {
                int64_t tseg = r.seg.value();
                if (tseg == 0xFF) {   // moveable -> entry-table ordinal
                    if (!ents) ents = nef.entries();
                    int64_t k = r.entry.value();
                    std::optional<formats::ne::Entry> e;
                    if (0 < k && k <= ents->size()) e = (*ents)[k - 1];
                    if (!e || e->kind == py::str("UNUSED")) continue;
                    tsel = ld.sels.get(e->seg.value(), 0);
                    toff = e->offset.value();
                } else {
                    tsel = ld.sels.get(tseg, 0);
                    toff = r.entry.value();
                }
            } else if (r.kind != py::str("OSFIXUP") && r.src == 5 && _kernel_constant(namer(r))) {
                // KERNEL's plain numbers (__AHSHIFT 3, __AHINCR 8): written in as the loader does
                tsel = 0;
                toff = *_kernel_constant(namer(r));
            } else {   // IMPORTORD / IMPORTNAME / OSFIXUP
                int64_t slot = static_cast<int64_t>(imports.size()) + 1;
                toff = slot * _IMPORT_SENT_STRIDE;
                tsel = ld.sent_sel;
                Hit h;
                h.kind = r.kind;
                h.module = r.module;
                h.ordinal = r.ordinal;
                h.name_off = r.name_off;
                h.api = namer(r);
                imports[toff] = h;
            }
            _patch_chain(uc, base, r, tsel, toff);
        }
    }
}

// ---- what is kept per process --------------------------------------------------------------------------------------
std::map<std::string, py::bytes> _RELOCATED;
struct Known {
    std::shared_ptr<formats::ne::NEFile> nef;
    std::vector<std::pair<int64_t, py::bytes>> bodies;   // {base: fixed-up bytes}
    Imports imports;
};
std::map<std::tuple<std::string, int64_t, int64_t>, std::shared_ptr<Known>> _LOADED;   // (path, size, mtime) -> ...
std::map<std::tuple<int64_t, int64_t>, py::bytes> _TAILS;
std::map<std::tuple<std::string, int64_t>, int64_t> _RETF;

// ---- a 16-bit instruction, for the branch log and the fault note -----------------------------------------------------
struct Insn16 {
    int64_t address = 0, size = 0;
    std::string mnemonic, op_str;
    std::vector<cs_x86_op> operands;
    std::vector<int> regs_written;
    std::vector<std::string> reg_names_written;
};

// the first instruction of `code` at address `at`, or none
std::optional<Insn16> decode16(const std::string& code, int64_t at, bool detail) {
    const capstone_x86::Api& api = capstone_x86::api();
    csh h = capstone_x86::handle(16, detail);
    if (code.empty()) return std::nullopt;
    cs_insn* all = nullptr;
    size_t k = api.disasm(h, reinterpret_cast<const uint8_t*>(code.data()), code.size(), static_cast<uint64_t>(at), 1, &all);
    if (k == 0) return std::nullopt;
    Insn16 out;
    out.address = static_cast<int64_t>(all[0].address);
    out.size = all[0].size;
    out.mnemonic = all[0].mnemonic;
    out.op_str = all[0].op_str;
    if (detail && all[0].detail) {
        const cs_x86& x = all[0].detail->x86;
        for (int i = 0; i < x.op_count; i++) out.operands.push_back(x.operands[i]);
        cs_regs regs_read, regs_write;
        uint8_t nr = 0, nw = 0;
        if (api.regs_access(h, &all[0], regs_read, &nr, regs_write, &nw) == CS_ERR_OK) {
            for (int i = 0; i < nw; i++) {
                out.regs_written.push_back(regs_write[i]);
                const char* nm = api.reg_name(h, regs_write[i]);
                out.reg_names_written.push_back(nm ? nm : "");
            }
        } else {
            out.regs_written.push_back(-1);   // (regs_access failed: the Python's `except` gives no registers)
        }
    }
    api.free_(all, 1);
    return out;
}

std::string reg_name16(int reg) {
    const char* nm = capstone_x86::api().reg_name(capstone_x86::handle(16, true), static_cast<unsigned>(reg));
    return nm ? nm : "";
}

// An operand of a compared instruction: ("reg", (uc id, parent name), size), ("imm", value, size) or
// ("mem", (base, index, disp, segment), size).
struct Operand16 {
    std::string kind;
    std::optional<int64_t> reg;       // reg: Unicorn's id
    std::string parent;               // reg: the 32-bit register it is part of
    int64_t imm = 0;                  // imm
    std::optional<int64_t> base, index, seg;   // mem
    int64_t disp = 0;
    int64_t size = 0;
};

// capstone register name -> the 32-bit register it is part of: emulator._PARENT.get(name, name)
std::string parent_of(const std::string& name) {
    auto p = emulator::_PARENT.get(py::str(name));
    return p ? p->utf8() : name;
}

std::optional<int64_t> uc_reg(const std::string& name) { return emulator::_uc_reg(py::str(name)); }

struct Decoded16 {
    std::string kind = "other";              // "other" | "keep" | "jcc" | "cmp"
    int64_t target = 0, next = 0;            // jcc: the offset it jumps to, and the next instruction's
    std::string mnemonic;                    // cmp
    std::vector<Operand16> ops;
    int64_t size = 0;
    std::vector<std::string> written;        // the registers (32-bit parents) it writes
    bool has_load = false;                   // a load from memory into a register
    std::string load_reg;                    // (its parent register; empty: not one of _PARENT's)
    Operand16 load_src;
    int64_t load_size = 0;
};

bool starts_with(const std::string& s, const char* p) { return s.compare(0, std::strlen(p), p) == 0; }

std::optional<Operand16> operand16(const cs_x86_op& op) {
    Operand16 o;
    o.size = op.size;
    if (op.type == X86_OP_REG) {
        std::string name = reg_name16(op.reg);
        o.kind = "reg";
        o.reg = uc_reg(name);
        o.parent = parent_of(name);
        return o;
    }
    if (op.type == X86_OP_IMM) {
        o.kind = "imm";
        o.imm = op.imm;
        return o;
    }
    if (op.type == X86_OP_MEM) {
        const x86_op_mem& m = op.mem;
        std::string base = m.base ? reg_name16(m.base) : "";
        std::string seg = m.segment ? reg_name16(m.segment) : (base == "bp" || base == "sp" ? "ss" : "ds");
        o.kind = "mem";
        if (m.base) o.base = uc_reg(base);
        if (m.index) o.index = uc_reg(reg_name16(m.index));
        o.disp = m.disp;
        o.seg = uc_reg(seg);
        return o;
    }
    return std::nullopt;
}

// _decode16: as emulator._decode_branchy's -- ("other", ...) for anything it cannot make sense of
std::shared_ptr<const Decoded16> _decode16(const std::string& raw, int64_t off) {
    static std::map<std::pair<std::string, int64_t>, std::shared_ptr<const Decoded16>> memo;
    auto key = std::make_pair(raw, off);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;
    auto out = std::make_shared<Decoded16>();
    try {
        std::optional<Insn16> ins = decode16(raw, off, true);
        if (!ins) throw py::StopIteration();
        const std::string& mn = ins->mnemonic;
        Decoded16 d;
        // written = tuple({_PARENT.get(name, name) for r in regs_access()[1]}), () when that raises
        if (!(ins->regs_written.size() == 1 && ins->regs_written[0] == -1)) {
            std::set<std::string> w;
            for (const std::string& n : ins->reg_names_written) w.insert(parent_of(n));
            d.written.assign(w.begin(), w.end());
        }
        if ((mn == "mov" || mn == "movzx" || mn == "movsx") && ins->operands.size() == 2 && ins->operands[0].type == X86_OP_REG &&
            ins->operands[1].type == X86_OP_MEM) {
            auto src = operand16(ins->operands[1]);
            if (src) {
                d.has_load = true;
                auto p = emulator::_PARENT.get(py::str(reg_name16(ins->operands[0].reg)));
                d.load_reg = p ? p->utf8() : "";
                d.load_src = *src;
                d.load_size = ins->operands[1].size;
            }
        }
        bool flagless = false;
        for (const py::str& f : emulator::_FLAGLESS)
            if (f.utf8() == mn) flagless = true;
        if (flagless || starts_with(mn, "cmov") || starts_with(mn, "set")) {
            d.kind = "keep";
        } else if (starts_with(mn, "j") && mn != "jmp" && !ins->operands.empty() && ins->operands[0].type == X86_OP_IMM) {
            d.kind = "jcc";
            d.target = ins->operands[0].imm & 0xFFFF;
            d.next = off + ins->size;
            d.written.clear();
            d.has_load = false;
        } else {
            bool compares = false;
            for (const py::str& f : emulator::_COMPARES)
                if (f.utf8() == mn) compares = true;
            if (compares && (ins->operands.size() == 1 || ins->operands.size() == 2)) {
                std::vector<std::optional<Operand16>> ops;
                for (const auto& op : ins->operands) ops.push_back(operand16(op));
                bool all_ok = true;
                for (const auto& o : ops)
                    if (!o || (o->kind == "reg" && !o->reg)) all_ok = false;
                if (all_ok) {
                    if (ops.size() == 1) {   // dec/inc: compared with what it becomes
                        Operand16 imm;
                        imm.kind = "imm";
                        imm.imm = mn == "dec" ? 1 : -1;
                        imm.size = ops[0]->size;
                        ops.push_back(imm);
                    }
                    d.kind = "cmp";
                    d.mnemonic = mn;
                    for (const auto& o : ops) d.ops.push_back(*o);
                    d.size = ops[0]->size;
                    d.has_load = false;
                }
            }
            if (d.kind != "cmp") d.has_load = false;
        }
        *out = d;
    } catch (const py::Exception&) {
    }
    memo[key] = out;
    return out;
}

// A code hook for the offsets [lo, hi) of the segment at `cs_base`: appends (jump, taken, compare) to the branch log as
// emulator._branch_hook does, every address a linear one.
std::function<void(Uc&, int64_t, int64_t)> _branch_hook16(py::list<native_trace::BranchEntry> log, int64_t cs_base, int64_t lo,
                                                          int64_t hi) {
    struct State {
        struct Pending {
            int64_t site, target, next;
            std::optional<native_trace::Compare> cmp;
        };
        std::optional<Pending> pend;
        std::optional<native_trace::Compare> cmp;           // compare in force
        std::map<std::string, std::pair<int64_t, int64_t>> loaded;   // register -> (address, size) it was loaded from
        std::map<int64_t, int64_t> bases;
    };
    auto st = std::make_shared<State>();
    auto segbase = [st](Uc& uc, int64_t sel) -> int64_t {
        auto it = st->bases.find(sel);
        if (it == st->bases.end()) {   // (the descriptor's base, from the GDT)
            py::bytes d = uc.mem_read(GDT_ADDR + (sel >> 3) * 8, 8);
            it = st->bases.emplace(sel, d[2] | (d[3] << 8) | (d[4] << 16) | (d[7] << 24)).first;
        }
        return it->second;
    };
    auto reg_or0 = [](Uc& uc, const std::optional<int64_t>& r) -> int64_t { return r && *r ? uc.reg_read(*r) : 0; };
    auto address = [=](Uc& uc, const Operand16& mem) -> int64_t {
        int64_t off = (mem.disp + reg_or0(uc, mem.base) + reg_or0(uc, mem.index)) & 0xFFFF;
        if (!mem.seg) throw py::TypeError("an integer is required");
        return segbase(uc, uc.reg_read(*mem.seg)) + off;
    };
    auto value = [=](Uc& uc, const Operand16& op) -> std::pair<uint64_t, std::optional<uint64_t>> {
        uint64_t mask = op.size >= 8 ? ~0ULL : ((1ULL << (8 * op.size)) - 1);
        if (op.kind == "imm") return {static_cast<uint64_t>(op.imm) & mask, std::nullopt};
        if (op.kind == "reg") {
            auto at = st->loaded.find(op.parent);
            std::optional<uint64_t> where;
            if (at != st->loaded.end() && at->second.second == op.size) where = static_cast<uint64_t>(at->second.first);
            return {static_cast<uint64_t>(uc.reg_read(*op.reg)) & mask, where};
        }
        int64_t a = address(uc, op);
        py::bytes b = uc.mem_read(a, op.size);
        uint64_t v = 0;
        for (int64_t i = 0; i < b.size() && i < 8; i++) v |= static_cast<uint64_t>(b[i]) << (8 * i);
        return {v, static_cast<uint64_t>(a)};
    };
    return [=](Uc& uc, int64_t addr, int64_t) {
        if (st->pend) {
            State::Pending pend = *st->pend;
            st->pend.reset();
            if (log.size() < emulator::_BRANCH_CAP && (addr == pend.target || addr == pend.next))
                log.append(native_trace::BranchEntry(static_cast<uint64_t>(pend.site), addr == pend.target, pend.cmp));
        }
        py::bytes raw;
        try {
            raw = uc.mem_read(addr, 16);
        } catch (const py::Exception&) {
            raw = uc.mem_read(addr, (addr | 0xFFF) + 1 - addr);   // (the last bytes of a window)
        }
        std::shared_ptr<const Decoded16> d = _decode16(raw.raw(), addr - cs_base);
        if (d->kind == "cmp") {
            try {
                auto [a, at_a] = value(uc, d->ops[0]);
                auto [b, at_b] = value(uc, d->ops[1]);
                st->cmp = native_trace::Compare(py::str(d->mnemonic), static_cast<uint64_t>(d->size), a, b, at_a, at_b);
            } catch (const py::Exception&) {
                st->cmp.reset();
            }
        } else if (d->kind == "jcc") {
            if (lo <= d->target && d->target < hi && d->target != d->next)
                st->pend = State::Pending{addr, cs_base + d->target, cs_base + d->next, st->cmp};
        } else if (d->kind == "other") {
            st->cmp.reset();
        }
        for (const std::string& r : d->written) st->loaded.erase(r);
        if (d->has_load && !d->load_reg.empty()) {
            try {
                st->loaded[d->load_reg] = {address(uc, d->load_src), d->load_size};
            } catch (const py::Exception&) {
            }
        }
    };
}

// The bytes of arguments the recorded system's code at sel:offset removes: its first `retf n` (0 for a plain retf, or none
// found in the next 8K).
int64_t _retf_bytes(const Context& context, int64_t sel, int64_t offset) {
    auto base = context._code_bases.get(sel);
    if (!base || !context.snapshot || context.snapshot->empty()) return 0;
    int64_t at = *base - SNAP_BASE + offset;
    auto key = std::make_tuple(context.key ? context.key->utf8() : std::string("\x01None"), at);
    auto it = _RETF.find(key);
    if (it != _RETF.end()) return it->second;
    int64_t got = 0;
    {
        const std::string chunk = context.snapshot->slice(at, at + 0x2000).raw();
        const capstone_x86::Api& api = capstone_x86::api();
        csh h = capstone_x86::handle(16, false);
        cs_insn* all = nullptr;
        size_t k = chunk.empty() ? 0 : api.disasm(h, reinterpret_cast<const uint8_t*>(chunk.data()), chunk.size(),
                                                  static_cast<uint64_t>(offset), 0, &all);
        for (size_t i = 0; i < k; i++) {
            std::string mn = all[i].mnemonic;
            if (mn == "retf" || mn == "lret") {
                std::string ops = all[i].op_str;
                got = ops.empty() ? 0 : py::int_parse(py::str(ops), 0);
                break;
            }
        }
        if (all) api.free_(all, k);
    }
    _RETF[key] = got;
    return got;
}

// An LDT holding a recorded call's selectors (each index of it present: a selector not recorded names scratch).
// Returns {recorded selector: the one to load for it here}.
//
// With context.snapshot (the whole of memory at the call and context.ldt_at: where the LDT was and its limit) every
// selector the system had names its bytes there -- a selector a callee hands back as much as one among the arguments.
py::dict<int64_t, int64_t> _recorded_ldt(Uc& uc, Context& context, const std::optional<int64_t>& data_base) {
    py::dict<int64_t, std::tuple<py::bytes, bool>> rec = context.ldt.copy();
    const bool snap = context.snapshot && !context.snapshot->empty();
    std::string table;
    int64_t at = 0, n = 0;
    py::bytes snapshot = context.snapshot ? *context.snapshot : py::bytes();
    py::bytes scratch3 = _desc(SCRATCH_BASE, 0xFFFF, false, 3);
    if (snap) {
        uc.mem_map(SNAP_BASE, (snapshot.size() + 0xFFFF) & ~int64_t(0xFFFF));
        uc.mem_write(SNAP_BASE, snapshot);
        if (!context.ldt_at) throw py::KeyError("'ldt_at'");
        at = std::get<0>(*context.ldt_at);
        int64_t lim = std::get<1>(*context.ldt_at);
        n = (lim + 1) / 8;
        for (int64_t i = 0; i < n; i++) table += scratch3.raw();
        for (int64_t i = 1; i < n; i++) {
            py::bytes d = snapshot.slice(at + 8 * i, at + 8 * i + 8);
            if (d.size() < 8 || !((d[5] & 0x80) && (d[5] & 0x10))) continue;   // (not present, or not code/data)
            int64_t base = d[2] | (d[3] << 8) | (d[4] << 16) | (d[7] << 24);
            int64_t limit = d[0] | (d[1] << 8);
            if (base + limit >= snapshot.size()) continue;
            if (d[5] & 8) {
                // (code of the running system -- Windows', another module's, a callback: not run, but called as an import
                // is, in the sentinel segment, answered and returned from. Conforming, so a far call from the tracer's
                // level 0 to an RPL 3 selector is allowed; run where it was, a level-3 segment, it faulted on both sides)
                std::string e = _desc(SENT_BASE, 0xFFFF, true).raw();
                e[5] = static_cast<char>(0x80 | 0x10 | 0x0E);
                put_slice(table, 8 * i, 8, e);
                continue;
            }
            put_slice(table, 8 * i, 8, _desc(SNAP_BASE + base, limit, false, 3).raw());
        }
        rec = py::dict<int64_t, std::tuple<py::bytes, bool>>();
    } else {
        int64_t top = 1;
        for (int64_t s : rec.keys()) top = std::max(top, s >> 3);
        top = std::max(top, context.ss >> 3);
        top = std::max(top, context.data_sel.value_or(0) >> 3);
        for (int64_t i = 0; i < top + 1; i++) table += scratch3.raw();
    }
    int64_t need = std::max(context.ss, context.data_sel.value_or(0)) >> 3;
    if (static_cast<int64_t>(table.size()) / 8 <= need)
        for (int64_t i = 0; i < need + 1 - static_cast<int64_t>(table.size()) / 8; i++) table += scratch3.raw();

    auto put = [&](int64_t sel, int64_t base, bool code) { put_slice(table, 8 * (sel >> 3), 8, _desc(base, 0xFFFF, code, 3).raw()); };
    std::vector<int64_t> sorted_keys = rec.keys();
    std::sort(sorted_keys.begin(), sorted_keys.end());
    for (size_t k = 0; k < sorted_keys.size(); k++) {
        int64_t sel = sorted_keys[k];
        const auto& entry = rec.at(sel);
        if (!(sel & 4)) continue;
        int64_t base = FOREIGN_BASE + static_cast<int64_t>(k) * SEG_STRIDE;
        uc.mem_map(base, SEG_STRIDE);
        uc.mem_write(base, std::get<0>(entry).slice(0, SEG_STRIDE));
        put(sel, base, std::get<1>(entry));
    }
    put(context.ss, STACK_BASE, false);
    if (context.data_sel && *context.data_sel && data_base) put(*context.data_sel, *data_base, false);
    if (snap) {
        for (const auto& kv : context.own.items()) {
            // (the module's own code segments: a window each, where a far call is sent on by name)
            int64_t sel = std::get<0>(kv), sg = std::get<1>(kv);
            try {
                uc.mem_map(OWN_BASE + sg * SEG_STRIDE, SEG_STRIDE);
            } catch (const UcError&) {
            }
            std::string e = _desc(OWN_BASE + sg * SEG_STRIDE, 0xFFFF, true).raw();
            e[5] = static_cast<char>(0x80 | 0x10 | 0x0E);
            put_slice(table, 8 * (sel >> 3), 8, e);
        }
    }
    int64_t size = (static_cast<int64_t>(table.size()) + _PAGE - 1) / _PAGE * _PAGE;
    uc.mem_map(LDT_ADDR, size);
    uc.mem_write(LDT_ADDR, py::bytes(table));
    uc.reg_write_descriptor(R("LDTR"), std::make_tuple(int64_t(0), LDT_ADDR, static_cast<int64_t>(table.size()) - 1, int64_t(0x82)));
    if (snap) {
        py::dict<int64_t, int64_t> bases;
        for (int64_t i = 1; i < n; i++) {
            if ((snapshot[at + 8 * i + 5] & 0x98) == 0x98)
                bases.set((i << 3) | 7, SNAP_BASE + (snapshot[at + 8 * i + 2] | (snapshot[at + 8 * i + 3] << 8) |
                                                       (snapshot[at + 8 * i + 4] << 16) | (snapshot[at + 8 * i + 7] << 24)));
        }
        context._code_bases = bases;
    }
    py::dict<int64_t, int64_t> out;
    for (int64_t s : rec.keys())
        if (s & 4) out.set(s, s);
    if (snap) {
        out = py::dict<int64_t, int64_t>();
        for (int64_t i = 1; i < static_cast<int64_t>(table.size()) / 8; i++) out.set((i << 3) | 7, (i << 3) | 7);
    }
    out.set(context.ss, context.ss);
    if (context.data_sel && *context.data_sel) out.set(*context.data_sel, *context.data_sel);
    return out;
}

// The Hit of a call by "callback <name>" etc. is built where it is made.
std::string sha1_hex8(const std::string& text) { return py::hashlib::sha1(py::bytes(text)).hexdigest().slice(0, 8).utf8(); }
int64_t hex_int(const std::string& h) { return py::int_parse(py::str(h), 16); }

}  // namespace

// ---- module functions ------------------------------------------------------------------------------------------------
// Build one 8-byte GDT descriptor. 16-bit (D/B=0) byte-granular segment by default.
py::bytes _desc(int64_t base, int64_t limit, bool code, int64_t dpl, bool big) {
    std::string d(8, '\0');
    d[0] = static_cast<char>(u8(limit));
    d[1] = static_cast<char>(u8(limit >> 8));
    d[2] = static_cast<char>(u8(base));
    d[3] = static_cast<char>(u8(base >> 8));
    d[4] = static_cast<char>(u8(base >> 16));
    // access: P | DPL | S(1) | type ; code=0xA(exec/read) data=0x2(read/write)
    d[5] = static_cast<char>(0x80 | ((dpl & 3) << 5) | 0x10 | (code ? 0xA : 0x2));
    // flags nibble (G=0,D/B,L=0,AVL=0) | limit[19:16]
    d[6] = static_cast<char>(((big ? 0x4 : 0x0) << 4) | ((limit >> 16) & 0xF));
    d[7] = static_cast<char>(u8(base >> 24));
    return py::bytes(d);
}

// Where the module's segment `idx` (1-based) is laid out: its own 64K window.
int64_t segment_base(int64_t idx) { return SEG_ORIGIN + idx * SEG_STRIDE; }

// The base the descriptor for `sel` gives, read from the GDT as it is now (0 past its end).
int64_t selector_base(Uc& uc, int64_t sel) {
    py::bytes d;
    try {
        d = uc.mem_read(GDT_ADDR + (sel >> 3) * 8, 8);
    } catch (const py::Exception&) {
        return 0;
    }
    return d[2] | (d[3] << 8) | (d[4] << 16) | (d[7] << 24);
}

// The module's data segment as the loader leaves it -- its fix-ups applied (the selectors of far pointers kept in it) --
// the bytes a rebuilt module is given in place of its own, so both sides start from the same globals.
py::bytes relocated_data(const py::str& path) {
    auto it = _RELOCATED.find(path.utf8());
    if (it != _RELOCATED.end()) return it->second;
    formats::ne::NEFile nef(path);
    int64_t autod = nef.h.autodata_seg;
    std::optional<formats::ne::Segment> seg;
    for (const auto& s : nef.segments())
        if (s.idx == autod) {
            seg = s;
            break;
        }
    py::bytes out;
    if (seg && seg->file_off) {
        auto uc = unicorn_uc::open_x86(32);
        Loaded16 ld = _load_ne(*uc, nef);
        Imports imports;
        _apply_relocs(*uc, nef, ld, imports);
        out = uc->mem_read(ld.seg_bases.at(autod), seg->length);
    }
    _RELOCATED[path.utf8()] = out;
    return out;
}

// ---- the stub environment ---------------------------------------------------------------------------------------------
Ctx::Ctx(Uc& uc_) : uc(uc_) {}

int64_t Ctx::alloc_heap(int64_t size) {
    // take a GDT index near the top (the harness padded those with scratch; overwrite with a real data descriptor pointing
    // at a fresh linear window) and map that window.
    int64_t idx = _N_GDT - 8 - n;
    int64_t base = HEAP_BASE + n * HEAP_STRIDE;
    n += 1;
    try {
        uc.mem_map(base, ((size + 0xFFF) / 0x1000) * 0x1000);
    } catch (const UcError&) {
    }
    uc.mem_write(GDT_ADDR + idx * 8, _desc(base, 0xFFFF, false));
    int64_t sel = _sel(idx);
    heaps.set(sel, std::make_tuple(base, size));
    return sel;
}

namespace {
// Bare API function name, upper-cased (drop the 'MODULE.' prefix); resolve KERNEL @ordinals (the knowledge base's win16
// kernel_ordinal: KERNEL's numbers for the functions modelled, for when the call is reported by number, 'KERNEL.@15').
py::str _api(const Hit& hit) {
    py::str api = (hit.api ? *hit.api : py::str("")).upper();
    int64_t dot = api.find(py::str("."));
    py::str fn = dot >= 0 ? api.slice_from(dot + 1) : api;
    if (fn.startswith(py::str("@")) && api.startswith(py::str("KERNEL"))) {
        try {
            py::str num = py::str(std::to_string(py::int_parse(fn.slice_from(1))));   // str(int(fn[1:]))
            // knowledge.table("win16", "kernel_ordinal"): win16.json's "kernel_ordinals" part
            py::str file = py::os::path::join(knowledge::sources::_MODELS(), py::str("win16.json"));
            static std::optional<knowledge::sources::Rows> ords;
            if (!ords) ords = knowledge::sources::_part(py::str("kernel_ordinals"))(file);
            auto got = ords->get(num);
            if (got && got->kind == knowledge::sources::Json::Str) fn = got->s;
        } catch (const py::ValueError&) {
        }
    }
    return fn;
}
}  // namespace

StubResult default_stub(const Hit& hit, Uc*, HeapContext& ctx) {
    auto model = api_models::api_model(py::str("win16"), _api(hit));
    if (!model) return StubResult{0, std::nullopt, std::nullopt};   // unknown: benign 0 (stack may drift)
    py::list<int64_t> a = hit.args_stack ? *hit.args_stack : py::list<int64_t>();
    StubResult out;
    for (const char* reg : {"ax", "dx", "pop"}) {
        auto v = model->get(py::str(reg));
        if (!v) continue;
        int64_t value;
        if (v->kind == api_models::Json::Str) {
            if (v->s == py::str("heap")) {   // a new selector of the stand-in heap
                value = ctx.alloc_heap();
            } else if (v->s.startswith(py::str("arg"))) {   // the Nth word of the arguments, as pushed
                int64_t n = py::int_parse(v->s.slice_from(3));
                value = a.size() > n ? a[n] : 0;
            } else {
                throw py::TypeError("unsupported operand type(s) for &: 'str' and 'int'");
            }
        } else {
            value = v->i;
        }
        std::string r = reg;
        if (r == "ax") out.ax = value;
        else if (r == "dx") out.dx = value;
        else out.pop = value;
    }
    return out;
}

// ---- trace_function16 ---------------------------------------------------------------------------------------------------
Trace16 trace_function16(const py::str& path, const Trace16Options& options) {
    const int64_t CS = R("CS"), DS = R("DS"), SS = R("SS"), ES = R("ES"), ESP = R("ESP"), EIP = R("EIP"), EAX = R("EAX"),
                  EDX = R("EDX"), EBP = R("EBP"), EBX = R("EBX"), ECX = R("ECX"), ESI = R("ESI"), EDI = R("EDI");
    int64_t insn_cap = options.insn_cap;
    bool near_ = options.near_;
    native_shadow::Shadow* shadow = options.shadow;
    const bool has_context = options.context.has_value();
    Context context_copy = has_context ? *options.context : Context();   // (the run keeps its own copy: _code_bases is set on it)
    Context* context = has_context ? &context_copy : nullptr;

    // (a module is parsed and fixed up once per process: each run after the first writes the fixed-up segments as they
    // were -- walking every fix-up chain again was most of a run's setup)
    std::filesystem::path fs_path(path.utf8());
    std::error_code ec;
    int64_t st_size = static_cast<int64_t>(std::filesystem::file_size(fs_path, ec));
    if (ec) throw py::FileNotFoundError("[WinError 2] The system cannot find the file specified: " + py::repr(path).utf8());
    int64_t st_mtime = static_cast<int64_t>(std::filesystem::last_write_time(fs_path, ec).time_since_epoch().count());
    auto lkey = std::make_tuple(path.utf8(), st_size, st_mtime);
    std::shared_ptr<Known> known;
    if (auto it = _LOADED.find(lkey); it != _LOADED.end()) known = it->second;
    std::shared_ptr<formats::ne::NEFile> nef = known ? known->nef : std::make_shared<formats::ne::NEFile>(path);
    std::shared_ptr<Uc> ucp = unicorn_uc::open_x86(32);
    Uc& uc = *ucp;
    uc.speed_up();   // (the same reads, for less: emulate/fast_uc.py)
    Loaded16 ld = _load_ne(uc, *nef);
    Imports imports;
    if (known) {
        for (const auto& kv : known->bodies) uc.mem_write(kv.first, kv.second);
        imports = known->imports;
    } else {
        _apply_relocs(uc, *nef, ld, imports);
        auto k = std::make_shared<Known>();
        k->nef = nef;
        for (const auto& s : nef->segments()) {
            if (s.file_off && s.length) {
                int64_t b = ld.seg_bases.at(s.idx);
                k->bodies.emplace_back(b, uc.mem_read(b, s.length));
            }
        }
        k->imports = imports;
        _LOADED[lkey] = k;
    }

    std::optional<int64_t> ordinal = options.ordinal, seg = options.seg, offset = options.offset;
    Trace16 rec;
    if (ordinal) {
        auto ents = nef->entries();
        std::optional<formats::ne::Entry> e;
        if (0 < *ordinal && *ordinal <= ents.size()) e = ents[*ordinal - 1];
        if (!e || e->kind == py::str("UNUSED")) {
            Trace16 bad;
            bad.exit = py::str("bad-ordinal");
            bad.ordinal = ordinal;
            return bad;
        }
        seg = e->seg;
        offset = e->offset;
    }
    if (!seg) throw py::KeyError("None");
    int64_t cs_base = ld.seg_bases.at(*seg);
    int64_t autod = nef->h.autodata_seg;
    int64_t ds_sel = ld.sels.get(autod, ld.stack_sel);   // DGROUP; fall back to stack
    py::list<int64_t> args;
    if (options.args && !options.args->empty()) {
        for (int64_t a : *options.args) args.append(a & 0xFFFF);
    } else {
        for (int64_t i = 0; i < options.arity; i++) args.append(0);
    }
    std::optional<int64_t> data_base = ld.seg_bases.get(autod);   // seg_bases.get(auto)

    py::dict<int64_t, int64_t> alias;
    if (context) alias = _recorded_ldt(uc, *context, data_base);

    std::vector<std::pair<int64_t, int64_t>> arg_places;   // (where each argument word is pushed, its value)
    int64_t entry_sp = 0;
    auto le16 = [](int64_t v) {
        std::string s(2, '\0');
        s[0] = static_cast<char>(v & 0xFF);
        s[1] = static_cast<char>((v >> 8) & 0xFF);
        return py::bytes(s);
    };

    // the recorded call's own frame: its stack at its SP, its registers, its selectors
    auto enter_recorded = [&](int64_t sseg, int64_t soff) -> int64_t {
        std::string stack = context->stack.slice(0, 0x10000).raw();
        stack.resize(0x10000, '\0');   // .ljust(0x10000, b"\0")
        uc.mem_write(STACK_BASE, py::bytes(stack));
        int64_t sp = context->sp & 0xFFFF;
        if (near_) {
            uc.mem_write(STACK_BASE + sp, le16(NEAR_RET));
        } else {
            uc.mem_write(STACK_BASE + sp, le16(FAR_RET));
            uc.mem_write(STACK_BASE + sp + 2, le16(ld.sent_sel));
        }
        entry_sp = sp;
        const std::pair<int64_t, const char*> regmap[] = {{EAX, "AX"}, {EBX, "BX"}, {ECX, "CX"}, {EDX, "DX"},
                                                          {ESI, "SI"}, {EDI, "DI"}, {EBP, "BP"}};
        for (const auto& rk : regmap) {
            // (AX the tracer's mark, not the recorded one: a function that sets no result hands back what AX held, and the
            // recorded word read as a result)
            uc.reg_write(rk.first, std::string(rk.second) == "AX" ? ENTRY_AX : context->regs.get(py::str(rk.second), 0) & 0xFFFF);
        }
        // (SS the tracer's own: a stack selector must be of the running level, and the run's is 0; the recorded one, in the
        // LDT, names the same bytes for a pointer into the stack)
        uc.reg_write(SS, ld.stack_sel);
        uc.reg_write(ESP, sp);
        uc.reg_write(DS, context->ds ? alias.get(*context->ds, ds_sel) : ds_sel);
        uc.reg_write(ES, context->es ? alias.get(*context->es, ds_sel) : ds_sel);
        uc.reg_write(CS, ld.sels.at(sseg));
        uc.reg_write(EIP, soff);
        return soff;
    };

    // Set up a Pascal far frame for one call: push args left-to-right, then a far-ret sentinel, and point CS:IP at
    // (sseg:soff) with DS/ES = DGROUP. Returns the effective code offset.
    auto enter = [&](int64_t sseg, int64_t soff, const py::list<int64_t>& sargs) -> int64_t {
        if (context) return enter_recorded(sseg, soff);
        int64_t sp = 0xFFF0;
        arg_places.clear();
        for (int64_t a : sargs) {
            sp = (sp - 2) & 0xFFFF;
            uc.mem_write(STACK_BASE + sp, le16(a & 0xFFFF));
            arg_places.emplace_back(STACK_BASE + sp, a & 0xFFFF);
        }
        if (near_) {   // near call: just the return offset
            sp = (sp - 2) & 0xFFFF;
            uc.mem_write(STACK_BASE + sp, le16(NEAR_RET));
        } else {
            // a far return goes to a place of its own in the sentinel segment. (It used to go to offset 0 of the function's
            // own segment, which ran whatever was there.)
            sp = (sp - 2) & 0xFFFF;
            uc.mem_write(STACK_BASE + sp, le16(ld.sent_sel));
            sp = (sp - 2) & 0xFFFF;
            uc.mem_write(STACK_BASE + sp, le16(FAR_RET));
        }
        entry_sp = sp;
        uc.reg_write(SS, ld.stack_sel);
        uc.reg_write(ESP, sp);
        uc.reg_write(DS, ds_sel);
        uc.reg_write(ES, ds_sel);
        uc.reg_write(EAX, ENTRY_AX);   // (a function that sets no result returns it)
        if (options.bp) uc.reg_write(EBP, *options.bp & 0xFFFF);
        uc.reg_write(CS, ld.sels.at(sseg));
        uc.reg_write(EIP, soff);
        return soff;
    };

    if (options.data_image && !options.data_image->empty() && data_base) uc.mem_write(*data_base, options.data_image->slice(0, 0x10000));
    if (options.data_end && data_base) {
        // (past the original's data, the same on both sides: zeros to where its segment ends -- what the loader gives -- then
        // a pattern to 64K. A build's own data segment is laid out longer, and a read past the end -- a pointer near FFFFh --
        // found the build's bytes on one side and nothing on the other)
        int64_t base = *data_base;
        int64_t start = options.data_image && !options.data_image->empty() ? options.data_image->size() : std::get<0>(*options.data_end);
        for (int64_t page = (base + start) & ~int64_t(0xFFF); page < base + 0x10000; page += 0x1000) {
            try {
                uc.mem_map(page, 0x1000);
            } catch (const UcError&) {   // (mapped already)
            }
        }
        uc.mem_write(base + start, py::bytes::zeros(std::max<int64_t>(0, std::get<1>(*options.data_end) - start)));
        auto tkey = std::make_tuple(base, std::get<1>(*options.data_end));
        auto tit = _TAILS.find(tkey);
        if (tit == _TAILS.end()) {
            std::string tail;
            for (int64_t a = std::get<1>(*options.data_end); a < 0x10000; a++)
                tail.push_back(static_cast<char>((((base + a) * 0x9E3779B1LL) >> 13) & 0xFF));   // (emulator._pattern's)
            tit = _TAILS.emplace(tkey, py::bytes(tail)).first;
        }
        uc.mem_write(base + std::get<1>(*options.data_end), tit->second);
    }
    if (options.mem_init) {
        for (const auto& item : *options.mem_init) {
            const auto& where = std::get<0>(item);
            const py::bytes& data = std::get<1>(item);
            bool in_data = std::holds_alternative<std::tuple<py::str, int64_t>>(where);
            if (in_data && (!data_base || std::get<1>(std::get<std::tuple<py::str, int64_t>>(where)) + data.size() > 0x10000))
                continue;   // (no data segment, or past its end)
            int64_t at = in_data ? *data_base + std::get<1>(std::get<std::tuple<py::str, int64_t>>(where)) : std::get<int64_t>(where);
            try {
                uc.mem_write(at, data);
            } catch (const UcError&) {   // (nothing mapped there: not an input)
            }
        }
    }
    enter(*seg, *offset, args);

    rec.module = py::os::path::basename(path);
    rec.seg = *seg;
    rec.offset = *offset;
    rec.ordinal = ordinal;
    rec.input.args = args;
    rec.input.arity = args.size();
    rec.layout.seg_bases = ld.seg_bases.copy();
    rec.layout.ds_base = data_base;
    rec.layout.autodata_seg = autod;

    std::map<int64_t, int64_t> code_at;   // (selector -> base)
    for (const auto& kv : ld.seg_bases.items()) {
        int64_t i = std::get<0>(kv);
        if (ld.sels.contains(i) && i != autod) code_at[ld.sels.at(i) & ~int64_t(3)] = std::get<1>(kv);
    }
    py::dict<int64_t, std::tuple<py::str, int64_t, bool>> intercept;
    py::dict<core16::SegOff, py::str> places;
    if (options.callees) {
        for (const auto& kv : options.callees->items()) places.set(std::get<0>(kv), std::get<0>(std::get<1>(kv)));
        for (const auto& kv : options.callees->items()) {
            int64_t cseg = std::get<0>(std::get<0>(kv)), coff = std::get<1>(std::get<0>(kv));
            if (ld.seg_bases.contains(cseg) && !(cseg == *seg && coff == *offset)) intercept.set(ld.seg_bases.at(cseg) + coff, std::get<1>(kv));
        }
    }

    // A call into another of the module's functions, recorded and answered.
    auto answer = [&](int64_t address, int64_t n) {
        auto [name, pop, far] = intercept.at(address);
        if (pop < 0) {   // (how many it removes is not known: the stack after it is not either)
            rec.unsure = true;
            pop = 0;
        }
        int64_t spn = uc.reg_read(ESP) & 0xFFFF;
        int64_t ip = rd16(uc, STACK_BASE + spn);
        int64_t cs = far ? rd16(uc, STACK_BASE + spn + 2) : uc.reg_read(CS);
        int64_t first = spn + (far ? 4 : 2);
        py::list<int64_t> words;
        for (int64_t k = 0; k < pop / 2; k++) words.append(rd16(uc, STACK_BASE + ((first + 2 * k) & 0xFFFF)));
        CalleeCall cc;
        cc.name = name;
        cc.args = words;
        cc.sp = spn;
        cc.n = n;
        cc.ret_ip = ip;
        rec.calls_out.append(cc);
        // (the made-up result is made from the words as they compare between builds: a place in the stack, or the stack's or
        // data's selector, legitimately differs)
        int64_t ss_now = uc.reg_read(SS), ds_now = uc.reg_read(DS);
        std::string same = "[";
        for (int64_t i = 0; i < words.size(); i++) {
            int64_t w = words[i];
            std::optional<int64_t> nxt;
            if (i + 1 < words.size()) nxt = words[i + 1];
            std::optional<py::str> fn;
            if (nxt) fn = places.get(core16::SegOff((*nxt >> 3) - _MODULE_SLOT + 1, w));   // (a callback: the function, wherever it sits)
            std::string item;
            if (fn && !fn->empty()) item = py::repr(py::str("&") + *fn).utf8();
            else if (w == ss_now) item = "'SS'";
            else if (w == ds_now) item = "'DS'";
            else if ((nxt && *nxt == ss_now) || (0xE000 <= w && w < 0xFFF0)) item = "'stack'";
            else item = std::to_string(w);
            same += (i ? ", " : "") + item;
        }
        same += "]";
        int64_t h = hex_int(sha1_hex8(name.utf8() + same));
        // (a place in the caller's stack handed over, `&local, SS`, the callee fills: a word made up the same way. Left as
        // it was, the caller read what its frame held before -- another thing in each build)
        if (shadow) shadow->after_call(uc, false, 0, STACK_BASE + first, words.size(), py::str("callee"));
        for (int64_t i = 0; i + 1 < words.size(); i++) {
            if (words[i + 1] == ss_now) {
                uc.mem_write(STACK_BASE + words[i], le16(((h >> 7) ^ (0x9E37 * (i + 1))) & 0xFFFF));
                if (shadow) shadow->written(STACK_BASE + words[i], 2);
            }
        }
        uc.reg_write(EAX, h & 0xFFFF);
        uc.reg_write(EDX, (h >> 16) & 0xFFFF);
        uc.reg_write(ESP, (first + pop) & 0xFFFF);
        uc.reg_write(CS, cs);
        uc.reg_write(EIP, ip);
    };

    // import sentinel landing: a far call/jmp reached SENT -> stub a far return (AX=0). `n` is the instruction count then.
    auto sentinel = [&](int64_t address, int64_t n) {
        int64_t slot = address - SENT_BASE;
        int64_t spn = uc.reg_read(ESP) & 0xFFFF;
        int64_t ip = rd16(uc, STACK_BASE + spn);
        int64_t cs = rd16(uc, STACK_BASE + spn + 2);
        // capture the Pascal args pushed above the far-return addr (for caller reconstruction)
        py::list<int64_t> args_stack;
        for (int64_t k = 0; k < 12; k++) args_stack.append(rd16(uc, STACK_BASE + ((spn + 4 + 2 * k) & 0xFFFF)));
        Hit hit;
        if (auto it = imports.find(slot); it != imports.end()) hit = it->second;
        else hit.slot = slot;
        hit.pointed = py::dict<int64_t, py::bytes>();   // (dict(...) is a copy: a hit has its own, not the import record's)
        hit.args_stack = args_stack;
        hit.n = n;
        hit.ret_ip = ip;
        hit.ret_cs = cs;   // (the caller's own code segment)
        // (and what a far pointer into one of the module's code segments points at: up to 32 bytes, to the first zero -- a
        // string kept in the code, at its own offset in each build)
        for (int64_t k = 0; k + 1 < args_stack.size(); k++) {
            auto cb = code_at.find(args_stack[k + 1] & ~int64_t(3));
            if (cb != code_at.end()) {
                try {
                    std::string s = uc.mem_read(cb->second + args_stack[k], 32).raw();
                    hit.pointed.set(k, py::bytes(s.substr(0, s.find('\0'))));
                } catch (const UcError&) {
                }
            }
        }
        int64_t called = uc.reg_read(CS);
        StubResult res;
        bool have_res = false;
        if (context && called != ld.sent_sel) {
            called |= 3;   // (loaded as conforming code: RPL 0)
            // (a far call into the recorded system's code: named by where it went, the same in any build, and its arguments
            // removed as that code's own `retf n` removes them)
            int64_t size = _retf_bytes(*context, called, slot);
            Hit fh;
            fh.api = py::percent(py::str("far %04x:%04x"), called, slot);
            fh.slot = slot;
            fh.foreign = true;
            fh.args_stack = args_stack;
            fh.n = n;
            fh.ret_ip = ip;
            fh.arg_bytes = size;
            hit = fh;
            rec.imports_hit.append(hit);
            int64_t h = hex_int(sha1_hex8(hit.api->utf8()));
            res = StubResult{h & 0xFFFF, (h >> 16) & 0xFFFF, size};
            have_res = true;
        } else {
            rec.imports_hit.append(hit);
            int64_t idx = rec.imports_hit.size() - 1;
            if (options.import_stub) {
                std::optional<StubResult> r = options.import_stub(hit, uc);
                rec.imports_hit[idx] = hit;   // (the stub's notes on it: the Python's one dict)
                if (r) {
                    res = *r;
                    have_res = true;
                }
            }
        }
        int64_t ax = (have_res ? res.ax.value_or(0) : 0) & 0xFFFF;
        int64_t dx = (have_res ? res.dx.value_or(0) : 0) & 0xFFFF;
        int64_t pop = have_res ? res.pop.value_or(0) : 0;
        uc.reg_write(ESP, (spn + 4 + pop) & 0xFFFF);   // far-ret addr + callee's pascal args
        uc.reg_write(EAX, (dx << 16) | ax);
        uc.reg_write(EDX, dx);
        uc.reg_write(CS, cs);
        uc.reg_write(EIP, ip);
        if (shadow) {
            // (an unmodelled call's result is an input, `ret`; a modelled one's is made from its arguments, which the shadow
            // does not follow into the model)
            bool freed = hit.unmodelled.value_or(false);
            shadow->after_call(uc, freed, (dx << 16) | ax, STACK_BASE + ((spn + 4) & 0xFFFF), pop / 2,
                               freed ? std::nullopt : std::optional<py::str>(py::str("call:") + (hit.api ? *hit.api : py::str("None"))));
        }
    };

    // `int 21h` in a Windows module is DOS3Call by another door -- the same service, which Windows documents as the faster
    // way in; a module's own code picks one by a flag its start-up sets, and a rebuild in C calls DOS3Call. So it is answered
    // as that import is, recorded as that call. Any other interrupt, or a processor exception, ends the run as it did
    // without this hook.
    auto intr = [&](int64_t intno, int64_t n) {
        if (intno == 0x21 && options.import_stub) {
            Hit hit;
            hit.api = py::str("KERNEL.@102");
            hit.args_stack = py::list<int64_t>();
            hit.n = n;
            hit.int_ = 0x21;
            rec.imports_hit.append(hit);
            int64_t idx = rec.imports_hit.size() - 1;
            std::optional<StubResult> r = options.import_stub(hit, uc);
            rec.imports_hit[idx] = hit;
            StubResult res = r ? *r : StubResult();
            int64_t ax = res.ax.value_or(0) & 0xFFFF, dx = res.dx.value_or(0) & 0xFFFF;
            uc.reg_write(EAX, (dx << 16) | ax);
            uc.reg_write(EDX, dx);
            if (shadow) {
                bool freed = hit.unmodelled.value_or(false);
                shadow->after_call(uc, freed, (dx << 16) | ax, std::nullopt, 0, freed ? std::nullopt : std::optional<py::str>(py::str("call:KERNEL.@102")));
            }
            return;
        }
        rec.interrupt = intno;
        uc.emu_stop();
    };

    // The module's own code reached through a recorded far pointer (a context's run): that function of this side's build, by
    // name -- True when redirected.
    auto own = [&](int64_t address, int64_t n) -> bool {
        auto tgt = context->redirect.get(core16::SegOff((address - OWN_BASE) >> 16, address & 0xFFFF));
        if (!tgt) return false;
        Hit h;
        h.api = py::str("callback ") + std::get<2>(*tgt);
        h.n = n;
        rec.imports_hit.append(h);
        uc.reg_write(CS, ld.sels.at(std::get<0>(*tgt)));
        uc.reg_write(EIP, std::get<1>(*tgt));
        return true;
    };

    // The per-instruction and memory hooks are C++ (native/tracer: cbtrace.dll; emulate/native_win16), attached in
    // trace_function16's order; the events above are its callbacks, and a shadow (the path explorer's) is stepped there
    // too. The branch log and the ports' device model stay hooks of this module's.
    native_win16::require();
    native_win16::RunArgs ra;
    ra.eip_id = EIP;
    ra.near_ = near_;
    ra.fill = std::getenv("CB_GATE_ZERO") == nullptr;
    ra.cap = insn_cap;
    ra.cs_base = cs_base;
    ra.near_ret = cs_base + NEAR_RET;
    ra.far_ret = SENT_BASE + FAR_RET;
    ra.sent_lo = SENT_BASE;
    ra.sent_hi = SENT_BASE + 0x10000;
    ra.snap_base = SNAP_BASE;
    ra.gov = &emulator::_GOV;
    for (int64_t k : intercept.keys()) ra.intercept.append(static_cast<uint64_t>(k));
    ra.on_sentinel = sentinel;
    ra.on_intercept = answer;
    ra.on_intr = intr;
    ra.shadow = native_shadow::handle_of(shadow);
    if (context) ra.own = std::make_tuple(OWN_BASE, OWN_BASE + 0x1000000);
    ra.on_own = own;
    native_trace::Engine eng;
    eng.uch = uc.handle();
    eng.emu_start = [&uc](int64_t begin, int64_t until, int64_t count) { uc.emu_start(begin, until, 0, count); };
    auto nr = std::make_unique<native_win16::Run>(eng, ra);
    if (options.branches) {
        rec.branches = py::list<native_trace::BranchEntry>();
        uc.hook_add_code(_branch_hook16(*rec.branches, cs_base, std::get<0>(*options.branches), std::get<1>(*options.branches)),
                         cs_base + std::get<0>(*options.branches), cs_base + std::get<1>(*options.branches) - 1);
    }
    std::unique_ptr<ports16::Devices> devices;
    if (options.ports) {
        native_win16::Run* nrp = nr.get();
        devices = std::make_unique<ports16::Devices>([nrp]() { return nrp->count(); });
        struct Hooks : ports16::InsnHooks {
            Uc& uc;
            explicit Hooks(Uc& u) : uc(u) {}
            void hook_in(std::function<int64_t(int64_t, int64_t)> on_in) override {
                uc.hook_add_insn_in([on_in](Uc&, int64_t port, int64_t size) { return on_in(port, size); });
            }
            void hook_out(std::function<void(int64_t, int64_t, int64_t)> on_out) override {
                uc.hook_add_insn_out([on_out](Uc&, int64_t port, int64_t size, int64_t value) { on_out(port, size, value); });
            }
        } hooks(uc);
        ports16::hook(hooks, *devices);
    }

    // prelude: run setup calls first (sharing this uc's DGROUP state + the import_stub ctx), then discard their trace so only
    // the main function's memory/imports are captured.
    if (options.prelude) {
        for (const PreludeCall& pc : *options.prelude) {
            std::optional<int64_t> pseg = pc.seg, poff = pc.offset;
            if (pc.ordinal) {
                auto ents = nef->entries();
                formats::ne::Entry pe = ents[*pc.ordinal - 1];
                if (!pe.seg || !pe.offset) throw py::KeyError("'seg'");
                pseg = pe.seg;
                poff = pe.offset;
            }
            py::list<int64_t> pargs;
            for (int64_t a : pc.args) pargs.append(a & 0xFFFF);
            enter(pseg.value(), poff.value(), pargs);
            try {
                uc.emu_start(*poff, SENT_BASE, 0, insn_cap + 1);
            } catch (const UcError&) {
            }
            if (std::exception_ptr exc = nr->raised()) std::rethrow_exception(exc);   // (what a callback raised in it)
        }
        if (!options.prelude->empty()) {
            nr->reset();
            rec.mem_reads = py::list<native_trace::MemRead>();
            rec.mem_writes = py::list<native_trace::MemWrite>();
            rec.imports_hit = py::list<Hit>();
            rec.path = py::list<uint64_t>();
            rec.insn_count = 0;
            rec.prelude_ran = options.prelude->size();
            if (devices) devices->log.clear();   // (the devices keep their state)
            enter(*seg, *offset, args);
        }
    }

    // snapshot the register file the main function sees at entry (the implicit inputs behind the decompiler's in_/unaff_
    // pseudo-vars). Meaningful for an in-context run (real caller via prelude).
    Regs entry_regs;
    entry_regs.set(py::str("AX"), uc.reg_read(EAX) & 0xFFFF);
    entry_regs.set(py::str("BX"), uc.reg_read(EBX) & 0xFFFF);
    entry_regs.set(py::str("CX"), uc.reg_read(ECX) & 0xFFFF);
    entry_regs.set(py::str("DX"), uc.reg_read(EDX) & 0xFFFF);
    entry_regs.set(py::str("SI"), uc.reg_read(ESI) & 0xFFFF);
    entry_regs.set(py::str("DI"), uc.reg_read(EDI) & 0xFFFF);
    entry_regs.set(py::str("BP"), uc.reg_read(EBP) & 0xFFFF);
    entry_regs.set(py::str("CS"), uc.reg_read(CS));
    entry_regs.set(py::str("DS"), uc.reg_read(DS));
    entry_regs.set(py::str("ES"), uc.reg_read(ES));
    entry_regs.set(py::str("SS"), uc.reg_read(SS));
    rec.entry_regs = entry_regs;

    if (shadow) {
        py::list<native_shadow::Place> places_in;
        if (!context) {
            for (const auto& ap : arg_places) {
                native_shadow::Place p;
                p.where = ap.first;
                p.value = ap.second;
                places_in.append(p);
            }
        }
        shadow->enter(places_in);
    }
    std::optional<py::str> failed;
    try {
        uc.emu_start(*offset, SENT_BASE, 0, insn_cap + 1);
    } catch (const UcError& e) {
        failed = unicorn_uc::strerror(e.errno_);
    }
    {   // the record, then what a callback raised in the run
        native_win16::Win16Record wr;
        nr->collect(wr);
        if (wr.insn_count) rec.insn_count = *wr.insn_count;
        if (wr.truncated) rec.truncated = *wr.truncated;
        if (wr.returned) rec.returned = wr.returned;
        if (wr.limit_fault) rec.limit_fault = wr.limit_fault;
        if (wr.path) rec.path = *wr.path;
        if (wr.mem_reads) rec.mem_reads = *wr.mem_reads;
        if (wr.mem_writes) rec.mem_writes = *wr.mem_writes;
        if (wr.lazy_maps) rec.lazy_maps = wr.lazy_maps;
        rec.memops = wr.memops;
        rec.writes_seen = wr.writes_seen;
        rec._stuck = wr._stuck;
    }
    if (std::exception_ptr exc = nr->raised()) std::rethrow_exception(exc);
    if (!failed) {
        // (neither returned nor stopped by a hook: emu_start's own instruction count ran out -- just short of the cap, the
        // skipped calls counting too. That is a run cut off, as much as one the hook stops: "stopped" made a long loop in each
        // build look like an ending)
        rec.exit = rec.returned.value_or(false) ? py::str("ret") : py::str("truncated");
        if (rec.limit_fault.value_or(false)) rec.exit = py::str("exception: past the end of a segment");
        if (rec.interrupt)   // (hook_intr: as Unicorn says it unhooked)
            failed = py::str("Unhandled CPU exception (UC_ERR_EXCEPTION)");
    }
    if (failed) {
        rec.exit = py::str("exception: ") + *failed;
        // fault detail: disassemble the instruction at the faulting CS:EIP (helps triage which 16-bit construct the segmented
        // emulator still can't handle).
        try {
            int64_t feip = uc.reg_read(EIP) & 0xFFFF;
            int64_t fcs = uc.reg_read(CS);
            std::optional<int64_t> csbase;
            for (const auto& kv : ld.seg_bases.items()) {
                if (ld.sels.get(std::get<0>(kv), -1) == fcs) {
                    csbase = std::get<1>(kv);
                    break;
                }
            }
            if (csbase) {
                std::string fb = uc.mem_read(*csbase + feip, 16).raw();
                std::optional<Insn16> ins = decode16(fb, feip, false);
                Fault f;
                f.cs = fcs;
                f.eip = feip;
                if (ins) {
                    std::string text = ins->mnemonic + " " + ins->op_str;
                    f.insn = py::str(text).strip();
                }
                f.bytes = py::bytes(fb.substr(0, 8)).hex();
                rec.fault = f;
            }
        } catch (const py::Exception&) {
        }
    }
    rec.ret = uc.reg_read(EAX) & 0xFFFF;
    // how far the stack pointer moved: 2 or 4 for the return address, plus any arguments removed
    rec.sp_delta = ((uc.reg_read(ESP) & 0xFFFF) - entry_sp) & 0xFFFF;
    rec.code_base = cs_base;
    Regs exit_regs;
    exit_regs.set(py::str("SI"), uc.reg_read(ESI) & 0xFFFF);
    exit_regs.set(py::str("DI"), uc.reg_read(EDI) & 0xFFFF);
    exit_regs.set(py::str("DS"), uc.reg_read(DS));
    rec.exit_regs = exit_regs;
    rec.ret32 = ((uc.reg_read(EDX) & 0xFFFF) << 16) | *rec.ret;
    rec.n_imports = static_cast<int64_t>(imports.size());
    if (devices) rec.io = devices->log;
    return rec;
}

// ---- probe --------------------------------------------------------------------------------------------------------------
Trace16 probe(const py::str& dll, const ProbeOptions& options) {
    py::list<int64_t> args;
    bool have_args = options.args.has_value();
    if (have_args) {
        args = *options.args;
        if (options.cdecl_ && !args.empty()) {
            std::vector<int64_t> v = args.vec();
            std::reverse(v.begin(), v.end());
            args = py::list<int64_t>(v);
        }
    }
    std::shared_ptr<Ctx> ctx;   // ctx_box
    Stub stub = options.stub ? options.stub : Stub([](Hit& hit, Uc& uc, HeapContext& c) { return default_stub(hit, &uc, c); });
    auto wrapped = [&](Hit& hit, Uc& uc) -> std::optional<StubResult> {
        if (!ctx) ctx = std::make_shared<Ctx>(uc);
        StubResult res = stub(hit, uc, *ctx);
        hit.unmodelled = !res.pop.has_value();   // (its result: stub_ret, for the path explorer)
        if (!res.pop && options.stub_ret) {      // (not modelled: the result asked for)
            res.ax = *options.stub_ret & 0xFFFF;
            res.dx = (*options.stub_ret >> 16) & 0xFFFF;
        }
        if (!res.pop) {   // remove the arguments, as the real one would
            std::optional<int64_t> size = symbols::api_arguments::of(hit.api);
            hit.arg_bytes = size;
            if (size && *size) res.pop = *size;
        } else {
            hit.arg_bytes = res.pop;
        }
        return res;
    };
    Trace16Options kw;
    kw.ordinal = options.ordinal;
    kw.seg = options.seg;
    kw.offset = options.offset;
    kw.args = have_args ? std::optional<py::list<int64_t>>(args) : std::nullopt;
    kw.arity = have_args ? args.size() : 0;
    if (options.insn_cap && *options.insn_cap) kw.insn_cap = *options.insn_cap;
    kw.import_stub = wrapped;
    kw.prelude = options.prelude;
    kw.near_ = options.near_;
    kw.data_image = options.data_image;
    kw.callees = options.callees;
    kw.data_end = options.data_end;
    kw.context = options.context;
    kw.mem_init = options.mem_init;
    kw.branches = options.branches;
    kw.bp = options.bp;
    kw.ports = options.ports;
    kw.shadow = options.shadow;
    Trace16 rec = trace_function16(dll, kw);
    rec.heaps = std::make_tuple(HEAP_BASE, HEAP_STRIDE);
    if (ctx) {
        for (const auto& kv : ctx->heaps.items()) rec.heap_sels.set(std::get<0>(kv), std::get<0>(std::get<1>(kv)));
    }
    return rec;
}

// probe's record as the one run record (emulate/run.py): AX and DX:AX the results, every Windows call made with its hit (the
// detail: what the lane names it from), the devices' log, SI DI DS as they went in and came out, the linear addresses run in
// order.
run::Run record(const Trace16& rec) {
    Regs entry = rec.entry_regs ? *rec.entry_regs : Regs();
    Regs out = rec.exit_regs ? *rec.exit_regs : Regs();
    py::dict<py::str, run::Register> results;
    results.set(py::str("AX"), rec.ret ? run::Register(*rec.ret) : run::Register(py::none_t{}));
    results.set(py::str("DX:AX"), rec.ret32 ? run::Register(*rec.ret32) : run::Register(py::none_t{}));
    py::list<run::Write> writes;
    for (const auto& w : rec.mem_writes)
        writes.append(run::Write{static_cast<int64_t>(w.addr), static_cast<int64_t>(w.size), static_cast<int64_t>(w.value),
                                 static_cast<int64_t>(w.old), false});
    py::list<run::Call> calls;
    for (const Hit& h : rec.imports_hit) {
        // (the whole hit is Trace16::imports_hit, through run.raw: what fits the detail is the 16-bit tracer's note)
        py::dict<py::str, run::Word> note;
        if (h.api) note.set(py::str("api"), run::Word(*h.api));
        if (h.n) note.set(py::str("n"), run::Word(*h.n));
        calls.append(run::Call{h.api, h.args_stack, run::Detail(note)});
    }
    py::list<run::IO> io;
    if (rec.io) {
        for (const auto& e : *rec.io) io.append(run::IO{std::get<0>(e), std::get<1>(e), std::get<2>(e), std::get<3>(e), std::nullopt});
    }
    run::Preserved preserved;
    for (const auto& kv : out.items())
        preserved.set(std::get<0>(kv), std::make_tuple(entry.get(std::get<0>(kv)), std::optional<int64_t>(std::get<1>(kv))));
    py::list<int64_t> path;
    for (uint64_t a : rec.path) path.append(static_cast<int64_t>(a));
    bool bad = rec.exit && *rec.exit == py::str("bad-ordinal");
    return run::Run(rec.exit, results, writes, false, calls, io, rec.sp_delta, preserved, path,
                    bad ? std::nullopt : std::optional<int64_t>(rec.insn_count), rec.truncated, py::str("little"),
                    std::make_shared<Trace16>(rec));
}

// The platform's runner (platforms/win16.py emulator.run): probe(dll, **kw) as a Run.
run::Run recorded(const py::str& dll, const ProbeOptions& options) { return record(probe(dll, options)); }

}  // namespace codebreaker::emulate::windows16
