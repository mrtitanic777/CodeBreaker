// codebreaker/emulate/shadow.cpp -- C++ port of codebreaker.emulate.shadow (see the header).
//
// Which values of a run are made from its inputs, and from which: a run's symbolic shadow, kept up beside the
// emulator instruction by instruction.  Before each instruction runs, its p-code is evaluated twice over at once --
// concretely, from the registers and memory as they are at that moment, and symbolically for the values that come from
// an input.  Only the symbolic half is kept.
//
// Concrete values are 64 bits here (Python's are unbounded): a varnode wider than 8 bytes raises OverflowError, which the
// shadow's own guard turns into `lost` ("shadow: OverflowError") as it does any exception.

#include "codebreaker/emulate/shadow.hpp"

#include <algorithm>
#include <map>
#include <unordered_map>

#include "codebreaker/emulate/windows16.hpp"

namespace codebreaker::emulate::shadow {

namespace S = symbolic;
using pcode::Input;
using pcode::Varnode;
using symbolic::ExprP;

namespace {

uint64_t mask_of(int64_t size) {
    if (size < 0) throw py::ValueError("negative shift count");
    if (size > 8) throw py::OverflowError("a value wider than 8 bytes");
    return size == 8 ? ~0ULL : ((1ULL << (8 * size)) - 1);
}

// _sx: v as a signed number of `size` bytes
int64_t sx(uint64_t v, int64_t size) {
    int64_t bits = 8 * size;
    if (bits <= 0) throw py::ValueError("negative shift count");
    if (size > 8) throw py::OverflowError("a value wider than 8 bytes");
    if (bits == 64) return static_cast<int64_t>(v);
    return ((v >> (bits - 1)) & 1) ? static_cast<int64_t>(v) - (int64_t(1) << bits) : static_cast<int64_t>(v);
}

ExprP bv(uint64_t c, const ExprP& s, int64_t size) { return s ? s : S::const_(static_cast<int64_t>(c), 8 * size); }

// A value of `size` bytes from its bytes: each (expr, k) -- byte k of expr -- or None for a concrete byte (of `conc`).
// All of one expression in order: that expression.
ExprP join(const std::vector<std::optional<ByteOf>>& entries, uint64_t conc, int64_t size) {
    if (entries.empty()) throw py::IndexError("list index out of range");
    const std::optional<ByteOf>& first = entries[0];
    bool all = first.has_value();
    for (size_t k = 0; all && k < entries.size(); k++) {
        const auto& e = entries[k];
        all = e.has_value() && std::get<0>(*e) == std::get<0>(*first) && std::get<1>(*e) == std::get<1>(*first) + static_cast<int64_t>(k);
    }
    if (all) return S::extract(std::get<0>(*first), 8 * std::get<1>(*first), 8 * size);
    ExprP out;
    for (int64_t k = 0; k < size; k++) {
        const auto& e = entries[static_cast<size_t>(k)];
        ExprP byte = e ? S::extract(std::get<0>(*e), 8 * std::get<1>(*e), 8)
                       : S::const_(static_cast<int64_t>(k < 8 ? (conc >> (8 * k)) & 0xFF : 0), 8);
        out = !out ? byte : S::concat(byte, out);
    }
    return out;
}

const char* const BINARY_OPS[][2] = {{"INT_ADD", "add"},   {"INT_SUB", "sub"},   {"INT_AND", "and"},   {"INT_OR", "or"},
                                     {"INT_XOR", "xor"},   {"INT_MULT", "mul"},  {"INT_DIV", "udiv"},  {"INT_REM", "urem"},
                                     {"INT_SDIV", "sdiv"}, {"INT_SREM", "srem"}};
struct CompareOp {
    const char* opc;
    const char* how;
    bool flip;
};
const CompareOp COMPARE_OPS[] = {{"INT_EQUAL", "eq", false}, {"INT_NOTEQUAL", "eq", true}, {"INT_LESS", "ult", false},
                                 {"INT_LESSEQUAL", "ule", false}, {"INT_SLESS", "slt", false}, {"INT_SLESSEQUAL", "sle", false}};

using Value = std::pair<uint64_t, ExprP>;   // (concrete, symbolic or None)

ExprP raw_expr(const char* op, int64_t bits, std::vector<ExprP> args) {
    auto e = std::make_shared<S::Expr>();
    e->op = py::str(op);
    e->bits = bits;
    e->args = std::move(args);
    return e;
}

}  // namespace

// ---- module functions ---------------------------------------------------------------------------------------------
std::optional<py::str> mode(const py::str& cpu) {
    py::str lang = cpu;
    if (lang.startswith(py::str("x86:LE:16"))) return py::str("x86-16-real");
    if (lang.startswith(py::str("x86:LE:32"))) return py::str("x86-32-flat");
    if (lang.startswith(py::str("PowerPC:BE"))) return py::str("ppc-be-64");
    return std::nullopt;
}

int64_t real_mode(Uc&, int64_t sel) { return sel * 16; }

// ---- PowerPCView -------------------------------------------------------------------------------------------------
PowerPCView::PowerPCView() { page = powerpc::PAGE; }

bool PowerPCView::known(const py::str& name) const {
    for (const char* n : {"LR", "CTR", "XER", "xer_so", "xer_ov", "xer_ca"})
        if (name == py::str(n)) return true;
    py::str c1 = name.slice(0, 1);
    if ((c1.empty() || c1 == py::str("r") || c1 == py::str("f")) && name.slice_from(1).isdigit()) return true;
    return name.slice(0, 2) == py::str("cr") && name.slice_from(2).isdigit();
}

uint64_t PowerPCView::reg(const powerpc::Machine& m, const py::str& name) const {
    if (name.empty()) throw py::IndexError("string index out of range");
    std::string c = name.slice(0, 1).utf8();
    if (c == "r") return m.r[py::int_parse(name.slice_from(1))];
    if (c == "f") return m.f[py::int_parse(name.slice_from(1))];
    if (name.slice(0, 2) == py::str("cr")) {
        int64_t shift = 28 - 4 * py::int_parse(name.slice_from(2));
        if (shift < 0) throw py::ValueError("negative shift count");
        return shift >= 64 ? 0 : (static_cast<uint64_t>(m.cr) >> shift) & 0xF;
    }
    if (name == py::str("LR")) return static_cast<uint64_t>(m.lr);
    if (name == py::str("CTR")) return m.ctr;
    if (name == py::str("xer_ca")) return static_cast<uint64_t>(m.ca);
    if (name == py::str("XER")) return static_cast<uint64_t>(m.ca) << 29;
    return 0;   // (SO, OV: the interpreter never sets them)
}

py::bytes PowerPCView::mem(const powerpc::Machine& m, int64_t at, int64_t size) const {
    at &= 0xFFFFFFFF;
    int64_t off = at & (page - 1);
    if (off + size <= page) {   // (within one page: one slice)
        if (auto p = m.pages.get(at >> 12)) return p->view().slice(off, off + size);   // page = m.pages.get(...)
        if (auto s = m.shared.get(at >> 12)) return s->slice(off, off + size);          // ... or m.shared.get(...)
        return py::bytes::zeros(size);                                                    // a page not made reads as zeros
    }
    std::string out;
    for (int64_t i = 0; i < size; i++) out += mem(m, at + i, 1).raw();
    return py::bytes(out);
}

py::bytes PowerPCView::code(const powerpc::Machine& m, int64_t address) const { return mem(m, address, 4); }

// ---- Shadow -------------------------------------------------------------------------------------------------------
Shadow::Shadow(std::shared_ptr<pcode::Lifter> lifter_, const PowerPCView& view_, const InputsFn& inputs_,
               const std::optional<std::tuple<int64_t, int64_t>>& span_, const SegBaseFn& segbase_,
               const py::list<std::tuple<py::str, int64_t, int64_t>>& result_, const py::list<py::str>& clobbered_,
               int64_t ret_bits_)
    : lifter(lifter_), view(view_), inputs(inputs_), span(span_), result(result_), clobbered(clobbered_), ret_bits(ret_bits_) {
    segbase_fn_ = segbase_ ? segbase_ : SegBaseFn([](powerpc::Machine&, int64_t) { return int64_t(0); });
    little = view.endian == py::str("little");
    mask = view.mask;
    for (const auto& kv : lifter->regs.items()) {
        const py::str& name = std::get<0>(kv);
        int64_t off = std::get<0>(std::get<1>(kv)), size = std::get<1>(std::get<1>(kv));
        if (!view.known(name)) continue;
        for (int64_t k = 0; k < size; k++) places.set(off + k, std::make_tuple(name, 8 * (little ? k : size - 1 - k)));
    }
    for (const auto& kv : lifter->regs.items()) named.set(std::get<0>(kv), std::get<0>(std::get<1>(kv)));
}

// The addresses of a value's bytes, its least significant first.
py::list<int64_t> Shadow::_bytes(int64_t at, int64_t size) const {
    py::list<int64_t> out;
    if (little) {
        for (int64_t a = at; a < at + size; a++) out.append(a);
    } else {
        for (int64_t a = at + size - 1; a > at - 1; a--) out.append(a);
    }
    return out;
}

// The base `sel` names (read once, until the environment next answers a call: an allocation makes descriptors).
int64_t Shadow::segbase(powerpc::Machine& m, int64_t sel) {
    auto got = bases.get(sel);
    if (!got) {
        int64_t b = segbase_fn_(m, sel);
        bases.set(sel, b);
        return b;
    }
    return *got;
}

// ---- inputs
ExprP Shadow::var(const py::str& name, int64_t bits, const py::str& kind,
                  const std::variant<py::none_t, int64_t, py::str>& where, int64_t value) {
    if (!vars.contains(name)) {
        Var v;
        v.kind = kind;
        v.where = where;
        v.bits = bits;
        v.value = value;
        vars.set(name, v);
        sym_.set(name, S::var(name, bits));
    }
    return sym_.at(name);
}

// The run is about to start: argument k, `bits` wide, is (where, value) -- where a memory address, or a register's
// name; or (where, value, k) to name it argument k (an input named `k` itself, of kind "reg", when k is a string).
void Shadow::enter(const py::list<Place>& places_, int64_t bits) {
    int64_t n = 0;
    for (const Place& item : places_) {
        std::variant<int64_t, py::str> k = item.k ? *item.k : std::variant<int64_t, py::str>(n);
        n++;
        ExprP v;
        if (std::holds_alternative<int64_t>(k)) {
            int64_t ki = std::get<int64_t>(k);
            v = var(py::percent(py::str("a%d"), ki), bits, py::str("arg"), ki, item.value);
        } else {
            const py::str& ks = std::get<py::str>(k);
            v = var(ks, bits, py::str("reg"), ks, item.value);
        }
        if (std::holds_alternative<py::str>(item.where)) {
            auto [off, size] = lifter->regs.at(std::get<py::str>(item.where));
            int64_t j = 0;
            for (int64_t b : _bytes(off, size)) {
                if (j < bits / 8) reg.set(b, ByteOf(v, j));
                j++;
            }
        } else {
            int64_t j = 0;
            for (int64_t b : _bytes(std::get<int64_t>(item.where), bits / 8)) {
                mem.set(b, ByteOf(v, j));
                j++;
            }
        }
    }
}

// Registers the environment set (not an instruction): no longer made from anything.
void Shadow::forget(const py::list<py::str>& names) {
    if (!off) {
        try {
            _forget_regs(names);
        } catch (const py::Exception& e) {
            lost.add(py::str("shadow: ") + py::str(e.type()));
            off = true;
        }
    }
}

void Shadow::_forget_regs(const py::list<py::str>& names) {
    for (const py::str& n : names) {
        auto o = named.get(n);
        if (!o) continue;
        int64_t size = std::get<1>(lifter->regs.at(n));
        for (int64_t b = *o; b < *o + size; b++) reg.pop(b, ByteOf());
    }
}

// (what the runner calls: nothing here may change the run -- whatever goes wrong in the shadow ends the shadow, never
// the run)
void Shadow::after_call(powerpc::Machine& m, bool symbolic_result, int64_t value, const std::optional<int64_t>& args_at,
                        int64_t nargs, const std::optional<py::str>& why, int64_t word) {
    try {
        _after_call(m, symbolic_result, value, args_at, nargs, why, word);
    } catch (const py::Exception& e) {
        lost.add(py::str("shadow: ") + py::str(e.type()));
        off = true;
    }
}

// Before the instruction at `address` (its segment based at `cs_base`) runs.
void Shadow::step(powerpc::Machine& m, int64_t address, int64_t cs_base) {
    if (!off) {
        try {
            _step(m, address, cs_base);
        } catch (const py::Exception& e) {
            lost.add(py::str("shadow: ") + py::str(e.type()));
            off = true;
        }
    }
}

// A call the environment answered (an imported function, or a callee not run): the `clobbered` registers were set,
// the result ones to its result -- `ret`, an input, when `symbolic_result`. Arguments made from the inputs (`nargs`
// words of `word` bytes from `args_at`), when the result is made from them (`why`): the path may be short of what it
// depends on.
void Shadow::_after_call(powerpc::Machine&, bool symbolic_result, int64_t value, const std::optional<int64_t>& args_at,
                         int64_t nargs, const std::optional<py::str>& why, int64_t word) {
    pending.reset();
    bases.clear();
    if (off) return;
    if (why && !why->empty() && args_at) {
        for (int64_t b = *args_at; b < *args_at + word * nargs; b++) {
            if (mem.contains(b)) {
                lost.add(*why);
                break;
            }
        }
    }
    _forget_regs(clobbered);
    if (symbolic_result) {
        uint64_t keep = ret_bits >= 64 ? ~0ULL : ((1ULL << ret_bits) - 1);
        ExprP r = var(py::str("ret"), ret_bits, py::str("ret"), py::none_t{}, static_cast<int64_t>(static_cast<uint64_t>(value) & keep));
        for (const auto& item : result) {
            const py::str& name = std::get<0>(item);
            int64_t k = std::get<1>(item), j = std::get<2>(item);
            auto [off_, size] = lifter->regs.at(name);
            reg.set(little ? off_ + k : off_ + size - 1 - k, ByteOf(r, j));
        }
    }
}

// Memory the environment wrote (not an instruction): no longer an input, nor made from one.
void Shadow::written(int64_t at, int64_t size) {
    for (int64_t b = at; b < at + size; b++) {
        mem.pop(b, ByteOf());
        touched.add(b);
    }
}

// ---- one instruction ----------------------------------------------------------------------------------------------
void Shadow::_step(powerpc::Machine& m, int64_t address, int64_t cs_base) {
    if (pending) {
        Pending pend = *pending;
        pending.reset();
        if (address == pend.target || address == pend.fall) {
            bool went = address == pend.target;
            trail.append(Step(pend.site, went));
            if (pend.entry) {
                if (went != pend.guess) lost.add(py::str("disagree"));   // (the specification and the processor differ here: the processor's way kept)
                PathEntry pe;
                pe.site = pend.site;
                pe.taken = went;
                pe.cond = pend.entry;
                pe.kind = py::str("jump");
                path.append(pe);
            }
        }
    }
    steps += 1;
    py::bytes raw;
    try {
        raw = view.code(m, address);
    } catch (const py::Exception&) {
        return;
    }
    int64_t off_ = address - cs_base;
    auto key = std::make_tuple(raw, off_);
    std::shared_ptr<const pcode::Lifted> ins;
    ins = lifted.get(key);
    if (!ins) {
        ins = lifter->lift(raw, off_);
        if (!ins) {
            _unknown(py::str("undecoded"));
            return;
        }
        lifted.set(key, ins);
    }
    bool reads_hit = false;
    if (!reg.empty())
        for (int64_t b : ins->reads)
            if (reg.contains(b)) {
                reads_hit = true;
                break;
            }
    if (!ins->full && !(!reg.empty() && reads_hit)) {
        if (!reg.empty())
            for (int64_t b : ins->writes) reg.pop(b, ByteOf());
        return;
    }
    try {
        _run(m, address, cs_base, *ins);
    } catch (const _Stop&) {
    }
    if (path.size() >= _CAP_PATH) {
        lost.add(py::str("path-capped"));
        off = true;
    }
}

// An instruction not followed: what it writes is no longer known to be made from anything.
void Shadow::_unknown(const py::str& why) {
    lost.add(why);
    reg.clear();
}

void Shadow::_run(powerpc::Machine& uc, int64_t address, int64_t cs_base, const pcode::Lifted& ins) {
    struct LocalByte {
        uint64_t c;
        std::optional<ByteOf> e;
    };
    std::map<int64_t, LocalByte> local_reg, local_mem;
    struct Unique {
        int64_t size;
        uint64_t c;
        ExprP s;
    };
    std::map<int64_t, Unique> uniq;
    std::map<std::string, uint64_t> conc;
    auto& regs = reg;
    int64_t amask = view.addresses;

    auto concrete_reg = [&](int64_t b) -> uint64_t {
        auto place = places.get(b);
        if (!place) return 0;
        const py::str& r = std::get<0>(*place);
        int64_t shift = std::get<1>(*place);
        auto it = conc.find(r.utf8());
        if (it == conc.end()) it = conc.emplace(r.utf8(), view.reg(uc, r)).first;
        return shift >= 64 ? 0 : (it->second >> shift) & 0xFF;
    };

    auto load = [&](int64_t at, int64_t size) -> Value {
        py::bytes data;
        try {
            data = view.mem(uc, at, size);
        } catch (const py::Exception&) {
            data = py::bytes::zeros(size);
        }
        if (size > 8) throw py::OverflowError("a value wider than 8 bytes");
        uint64_t c = 0;   // int.from_bytes(data, view.endian)
        for (int64_t i = 0; i < data.size(); i++) {
            int64_t b = data[i];
            if (little) c |= static_cast<uint64_t>(b) << (8 * i);
            else c = (c << 8) | static_cast<uint64_t>(b);
        }
        std::vector<std::optional<ByteOf>> ents;
        bool any_s = false;
        int64_t k = 0;
        for (int64_t b : _bytes(at, size)) {
            std::optional<ByteOf> e;
            auto lm = local_mem.find(b);
            if (lm != local_mem.end()) {
                c = (c & ~(0xFFULL << (8 * k))) | (lm->second.c << (8 * k));
                e = lm->second.e;
            } else {
                e = mem.get(b);
                if (!e && !touched.contains(b) && inputs(b)) {
                    e = ByteOf(var(py::percent(py::str("m%x"), b), 8, py::str("mem"), b, data[b - at]), 0);
                    mem.set(b, *e);
                }
            }
            ents.push_back(e);
            any_s = any_s || e.has_value();
            k++;
        }
        return {c, any_s ? join(ents, c, size) : ExprP()};
    };

    auto read = [&](const Input& in) -> Value {
        if (!std::holds_alternative<Varnode>(in)) throw py::TypeError("cannot unpack a non-varnode input");
        const Varnode& vn = std::get<Varnode>(in);
        const std::string space = vn.space.utf8();
        int64_t off_ = vn.offset, size = vn.size;
        if (space == "c") return {static_cast<uint64_t>(off_) & mask_of(size), ExprP()};
        if (space == "u") {
            auto got = uniq.find(off_);
            if (got != uniq.end() && got->second.size == size) return {got->second.c, got->second.s};
            if (got != uniq.end() && got->second.size > size)   // (the low part of a wider one)
                return {got->second.c & mask_of(size), got->second.s ? S::extract(got->second.s, 0, 8 * size) : ExprP()};
            return {0, ExprP()};
        }
        if (space == "r") {
            uint64_t c = 0;
            std::vector<std::optional<ByteOf>> ents;
            bool any_s = false;
            int64_t k = 0;
            if (size > 8) throw py::OverflowError("a value wider than 8 bytes");
            for (int64_t b : _bytes(off_, size)) {
                uint64_t cb;
                std::optional<ByteOf> e;
                auto lr = local_reg.find(b);
                if (lr != local_reg.end()) {
                    cb = lr->second.c;
                    e = lr->second.e;
                } else {
                    cb = concrete_reg(b);
                    e = regs.get(b);
                }
                c |= cb << (8 * k);
                ents.push_back(e);
                any_s = any_s || e.has_value();
                k++;
            }
            return {c, any_s ? join(ents, c, size) : ExprP()};
        }
        return load(off_, size);   // (ram named directly)
    };

    auto store = [&](int64_t at, int64_t size, uint64_t c, const ExprP& s) {
        int64_t k = 0;
        for (int64_t b : _bytes(at, size)) {
            local_mem[b] = {k < 8 ? (c >> (8 * k)) & 0xFF : 0, s ? std::optional<ByteOf>(ByteOf(s, k)) : std::nullopt};
            k++;
        }
    };

    auto write = [&](const Varnode& vn, uint64_t c, const ExprP& s) {
        const std::string space = vn.space.utf8();
        c &= mask_of(vn.size);
        if (space == "u") {
            uniq[vn.offset] = {vn.size, c, s};
        } else if (space == "r") {
            int64_t k = 0;
            for (int64_t b : _bytes(vn.offset, vn.size)) {
                local_reg[b] = {k < 8 ? (c >> (8 * k)) & 0xFF : 0, s ? std::optional<ByteOf>(ByteOf(s, k)) : std::nullopt};
                k++;
            }
        } else if (space == "m") {
            store(vn.offset, vn.size, c, s);
        }
    };

    auto pin = [&](uint64_t c, const ExprP& s) {
        if (s) {
            PathEntry pe;
            pe.site = address;
            pe.taken = true;
            pe.cond = S::compare(py::str("eq"), s, S::const_(static_cast<int64_t>(c), S::bits(s)));
            pe.kind = py::str("pin");
            path.append(pe);
            pins += 1;
        }
    };

    auto vn_of = [](const Input& in) -> const Varnode& {
        if (!std::holds_alternative<Varnode>(in)) throw py::TypeError("cannot unpack a non-varnode input");
        return std::get<Varnode>(in);
    };

    const auto& ops = ins.ops;
    int64_t i = 0, n = ops.size();
    bool done = false;
    while (i < n && !done) {
        const pcode::Op& op = ops[i];
        i++;
        const std::string opc = op.opcode.utf8();
        const auto& args = op.inputs;
        if (opc == "COPY") {
            auto [c, s] = read(args[0]);
            write(*op.out, c, s);
        } else if (opc == "LOAD") {
            auto [ac, asym] = read(args[1]);
            pin(ac, asym);
            auto [c, s] = load(static_cast<int64_t>(ac) & amask, op.out->size);
            write(*op.out, c, s);
        } else if (opc == "STORE") {
            auto [ac, asym] = read(args[1]);
            pin(ac, asym);
            auto [c, s] = read(args[2]);
            int64_t sz = vn_of(args[2]).size;
            store(static_cast<int64_t>(ac) & amask, sz, c & mask_of(sz), s);
        } else if (opc == "CALLOTHER") {
            const py::str& name = std::get<py::str>(args[0]);
            if (name == py::str("segment")) {
                auto [sc, ss] = read(args[1]);
                auto [oc, os_] = read(args[2]);
                pin(sc, ss);
                int64_t base = segbase(uc, static_cast<int64_t>(sc));
                ExprP s;
                uint64_t c;
                if (mask == 0xFFFF) {   // (real or protected 16-bit: base + 16-bit offset)
                    c = static_cast<uint64_t>(base) + (oc & 0xFFFF);
                    if (os_) {
                        ExprP off16 = S::extract(os_, 0, 16);
                        int64_t bits = 8 * op.out->size;
                        s = bits > 16 ? S::binary(py::str("add"), S::zext(off16, bits), S::const_(base, bits)) : off16;
                    }
                } else {   // (flat: the offset is the address)
                    c = static_cast<uint64_t>(base) + oc;
                    if (os_) {
                        int64_t bits = 8 * op.out->size;
                        s = S::bits(os_) == bits ? os_ : S::bits(os_) < bits ? S::zext(os_, bits) : S::extract(os_, 0, bits);
                        if (base) s = S::binary(py::str("add"), s, S::const_(base, bits));
                    }
                }
                write(*op.out, c, s);
            } else {
                bool sym = false;
                for (size_t a = 1; a < args.size(); a++)
                    if (std::holds_alternative<Varnode>(args[a]) && read(args[a]).second) sym = true;
                if (sym) lost.add(py::str("userop:") + name);
                if (op.out) write(*op.out, 0, ExprP());
            }
        } else if (opc == "CBRANCH") {
            auto [cc, cs] = read(args[1]);
            bool taken = cc != 0;
            const Varnode& tgt = vn_of(args[0]);
            if (tgt.space == py::str("c")) {   // (a branch inside the instruction's p-code)
                if (cs) {
                    PathEntry pe;
                    pe.site = address;
                    pe.taken = taken;
                    pe.cond = S::nonzero(cs);
                    pe.kind = py::str("inner");
                    path.append(pe);
                }
                if (taken) i = i - 1 + sx(static_cast<uint64_t>(tgt.offset), tgt.size);
            } else {
                int64_t target = cs_base + (tgt.offset & mask), fall = address + ins.length;
                if (target == fall || target == address) {   // (a string instruction's count run out, or its compare: its loop is the instruction's own)
                    if (cs) {
                        PathEntry pe;
                        pe.site = address;
                        pe.taken = taken;
                        pe.cond = S::nonzero(cs);
                        pe.kind = py::str("inner");
                        path.append(pe);
                    }
                } else {   // (the instruction's jump: resolved next step)
                    Pending p;
                    p.site = address;
                    p.target = target;
                    p.fall = fall;
                    p.entry = cs ? S::nonzero(cs) : ExprP();
                    p.guess = taken;
                    pending = p;
                }
                if (taken) done = true;
            }
        } else if (opc == "BRANCH") {
            const Varnode& tgt = vn_of(args[0]);
            if (tgt.space == py::str("c")) i = i - 1 + sx(static_cast<uint64_t>(tgt.offset), tgt.size);
            else done = true;
        } else if (opc == "BRANCHIND" || opc == "CALLIND" || opc == "RETURN") {
            auto [c, s] = read(args[0]);
            pin(c, s);
            done = true;
        } else if (opc == "CALL") {
            done = true;
        } else {
            std::vector<Value> vals;
            for (const Input& a : args) vals.push_back(read(a));
            auto [c, s] = _arith(opc, op.out ? op.out->size : 0, vals, args);
            if (op.out) write(*op.out, c, s);
        }
    }
    // what the instruction left: kept where it is made from an input, forgotten elsewhere
    for (const auto& kv : local_reg) {
        if (!kv.second.e) regs.pop(kv.first, ByteOf());
        else regs.set(kv.first, *kv.second.e);
    }
    for (const auto& kv : local_mem) {
        touched.add(kv.first);
        if (!kv.second.e) mem.pop(kv.first, ByteOf());
        else mem.set(kv.first, *kv.second.e);
    }
}

// One p-code operation of values (concrete, symbolic or None): -> (concrete, symbolic or None).
std::pair<uint64_t, ExprP> Shadow::_arith(const std::string& opc, int64_t osize, const std::vector<std::pair<uint64_t, ExprP>>& vals,
                                          const py::list<Input>& args) {
    if (vals.empty()) throw py::IndexError("list index out of range");
    auto vsize = [&](size_t i) -> int64_t {
        const Input& in = args[static_cast<int64_t>(i)];
        if (!std::holds_alternative<Varnode>(in)) throw py::TypeError("cannot unpack a non-varnode input");
        return std::get<Varnode>(in).size;
    };
    uint64_t ac = vals[0].first;
    const ExprP& a_s = vals[0].second;
    int64_t asize = vsize(0);
    uint64_t m = osize ? mask_of(osize) : 0;
    bool sym = false;
    for (const auto& v : vals) sym = sym || v.second != nullptr;
    uint64_t bc = 0;
    ExprP b_s;
    int64_t bsize = 0;
    if (vals.size() > 1) {
        bc = vals[1].first;
        b_s = vals[1].second;
        bsize = vsize(1);
    }
    // --- the concrete half
    uint64_t c;
    auto sdiv_sign = [&](int64_t x, int64_t y, bool rem) -> uint64_t {
        uint64_t ux = x < 0 ? 0 - static_cast<uint64_t>(x) : static_cast<uint64_t>(x);
        uint64_t uy = y < 0 ? 0 - static_cast<uint64_t>(y) : static_cast<uint64_t>(y);
        if (rem) {
            uint64_t r = ux % uy;
            return (x >= 0 ? r : 0 - r) & m;
        }
        uint64_t q = ux / uy;
        return ((x < 0) == (y < 0) ? q : 0 - q) & m;
    };
    if (opc == "INT_ADD") c = (ac + bc) & m;
    else if (opc == "INT_SUB") c = (ac - bc) & m;
    else if (opc == "INT_AND") c = ac & bc;
    else if (opc == "INT_OR") c = ac | bc;
    else if (opc == "INT_XOR") c = ac ^ bc;
    else if (opc == "INT_MULT") c = (ac * bc) & m;
    else if (opc == "INT_DIV") c = bc ? (ac / bc) & m : 0;
    else if (opc == "INT_REM") c = bc ? (ac % bc) & m : 0;
    else if (opc == "INT_SDIV") {
        int64_t x = sx(ac, asize), y = sx(bc, bsize);
        c = y ? sdiv_sign(x, y, false) : 0;
    } else if (opc == "INT_SREM") {
        int64_t x = sx(ac, asize), y = sx(bc, bsize);
        c = y ? sdiv_sign(x, y, true) : 0;
    } else if (opc == "INT_EQUAL") c = ac == bc;
    else if (opc == "INT_NOTEQUAL") c = ac != bc;
    else if (opc == "INT_LESS") c = ac < bc;
    else if (opc == "INT_LESSEQUAL") c = ac <= bc;
    else if (opc == "INT_SLESS") c = sx(ac, asize) < sx(bc, bsize);
    else if (opc == "INT_SLESSEQUAL") c = sx(ac, asize) <= sx(bc, bsize);
    else if (opc == "INT_ZEXT") c = ac;
    else if (opc == "INT_SEXT") c = static_cast<uint64_t>(sx(ac, asize)) & m;
    else if (opc == "INT_CARRY") {
        uint64_t sum = ac + bc;
        c = (sum < ac) || sum > mask_of(asize);
    } else if (opc == "INT_SCARRY" || opc == "INT_SBORROW") {
        int64_t x = sx(ac, asize), y = sx(bc, asize);
        bool overflow;
        if (asize >= 8) {   // (64 bits: the signed sum or difference wraps exactly when the operands' signs say so)
            int64_t r = static_cast<int64_t>(opc == "INT_SCARRY" ? static_cast<uint64_t>(x) + static_cast<uint64_t>(y)
                                                                 : static_cast<uint64_t>(x) - static_cast<uint64_t>(y));
            overflow = opc == "INT_SCARRY" ? ((x >= 0) == (y >= 0) && (r >= 0) != (x >= 0))
                                           : ((x >= 0) != (y >= 0) && (r >= 0) != (x >= 0));
        } else {
            int64_t r = opc == "INT_SCARRY" ? x + y : x - y;
            int64_t half = int64_t(1) << (8 * asize - 1);
            overflow = !(-half <= r && r < half);
        }
        c = overflow;
    } else if (opc == "INT_2COMP") c = (0 - ac) & m;
    else if (opc == "INT_NEGATE") c = (~ac) & m;
    else if (opc == "INT_LEFT") c = bc < static_cast<uint64_t>(8 * osize) ? (ac << bc) & m : 0;
    else if (opc == "INT_RIGHT") c = bc < static_cast<uint64_t>(8 * asize) ? (ac >> bc) & m : 0;
    else if (opc == "INT_SRIGHT") {
        int64_t x = sx(ac, asize);
        uint64_t sh = std::min<uint64_t>(bc, 8 * asize);
        c = static_cast<uint64_t>(sh >= 64 ? (x < 0 ? -1 : 0) : (x >> sh)) & m;
    } else if (opc == "BOOL_NEGATE") c = !ac;
    else if (opc == "BOOL_AND") c = ac && bc;
    else if (opc == "BOOL_OR") c = ac || bc;
    else if (opc == "BOOL_XOR") c = (ac != 0) != (bc != 0);
    else if (opc == "PIECE") {
        if (bsize >= 8 || 8 * bsize >= 64) throw py::OverflowError("a value wider than 8 bytes");
        c = ((ac << (8 * bsize)) | bc) & m;
    } else if (opc == "SUBPIECE") c = (bc * 8 >= 64 ? 0 : (ac >> (8 * bc))) & m;
    else if (opc == "POPCOUNT") {
        uint64_t x = ac;
        int64_t cnt = 0;
        while (x) {
            cnt += x & 1;
            x >>= 1;
        }
        c = static_cast<uint64_t>(cnt) & m;
    } else if (opc == "LZCOUNT") {
        int64_t bl = 0;
        uint64_t x = ac;
        while (x) {
            bl++;
            x >>= 1;
        }
        c = static_cast<uint64_t>(8 * asize - bl) & m;
    } else {   // (floating point and the rest: not followed)
        if (sym) lost.add(py::str("op:") + py::str(opc));
        return {0, ExprP()};
    }
    if (!sym) return {c, ExprP()};
    // --- the symbolic half
    ExprP A = bv(ac, a_s, asize);
    ExprP B;
    if (vals.size() > 1) B = bv(bc, b_s, bsize);
    ExprP s;
    const char* binary_name = nullptr;
    for (const auto& pr : BINARY_OPS)
        if (opc == pr[0]) binary_name = pr[1];
    const CompareOp* cmp = nullptr;
    for (const auto& co : COMPARE_OPS)
        if (opc == co.opc) cmp = &co;
    if (binary_name) {
        s = S::binary(py::str(binary_name), A, B);
    } else if (cmp) {
        ExprP c2 = S::compare(py::str(cmp->how), A, B);
        s = S::to_bool(cmp->flip ? S::negate(c2) : c2);
    } else if (opc == "INT_ZEXT") {
        s = S::zext(A, 8 * osize);
    } else if (opc == "INT_SEXT") {
        s = S::sext(A, 8 * osize);
    } else if (opc == "INT_CARRY") {
        s = S::to_bool(S::compare(py::str("ult"), S::binary(py::str("add"), A, B), A));
    } else if (opc == "INT_SCARRY" || opc == "INT_SBORROW") {
        ExprP r = S::binary(py::str(opc == "INT_SCARRY" ? "add" : "sub"), A, B);
        ExprP x = opc == "INT_SCARRY"
                      ? S::binary(py::str("and"), S::binary(py::str("xor"), A, r), S::binary(py::str("xor"), B, r))
                      : S::binary(py::str("and"), S::binary(py::str("xor"), A, B), S::binary(py::str("xor"), A, r));
        s = S::to_bool(S::nonzero(S::extract(x, 8 * asize - 1, 1)));
    } else if (opc == "INT_2COMP") {
        s = S::unary(py::str("neg"), A);
    } else if (opc == "INT_NEGATE") {
        s = S::unary(py::str("not"), A);
    } else if (opc == "INT_LEFT" || opc == "INT_RIGHT" || opc == "INT_SRIGHT") {
        int64_t width = 8 * asize;
        const char* kind = opc == "INT_LEFT" ? "shl" : opc == "INT_RIGHT" ? "lshr" : "ashr";
        ExprP amount, big;
        if (bsize > asize) {
            amount = S::extract(B, 0, width);
            big = S::compare(py::str("ule"), S::const_(width, 8 * bsize), B);
        } else {
            amount = S::zext(B, width);
        }
        s = S::binary(py::str(kind), A, amount);
        if (big) {
            ExprP over = std::string(kind) == "ashr" ? S::binary(py::str("ashr"), A, S::const_(width - 1, width)) : S::const_(0, width);
            s = raw_expr("ite", width, {big, over, s});
        }
    } else if (opc == "BOOL_NEGATE") {
        s = S::to_bool(S::negate(S::nonzero(A)));
    } else if (opc == "BOOL_AND" || opc == "BOOL_OR" || opc == "BOOL_XOR") {
        const char* how = opc == "BOOL_AND" ? "band" : opc == "BOOL_OR" ? "bor" : "bxor";
        s = S::to_bool(S::logic(py::str(how), S::nonzero(A), S::nonzero(B)));
    } else if (opc == "PIECE") {
        s = S::concat(A, B);
    } else if (opc == "SUBPIECE") {
        int64_t lo = 8 * static_cast<int64_t>(bc);
        int64_t width = std::min(8 * asize - lo, 8 * osize);
        s = S::zext(S::extract(A, lo, width), 8 * osize);
    } else if (opc == "POPCOUNT") {
        s = raw_expr("popcount", 8 * osize, {A});
    } else {
        lost.add(py::str("op:") + py::str(opc));
        return {c, ExprP()};
    }
    if (S::bits(s) != 8 * osize)   // (an output narrower or wider than made)
        s = S::bits(s) > 8 * osize ? S::extract(s, 0, 8 * osize) : S::zext(s, 8 * osize);
    return {c, s};
}

ShadowPPC::ShadowPPC(std::shared_ptr<pcode::Lifter> lifter_, const InputsFn& inputs_, const std::optional<std::tuple<int64_t, int64_t>>& span_)
    : Shadow(lifter_, PowerPCView(), inputs_, span_, nullptr,
             [] {
                 py::list<std::tuple<py::str, int64_t, int64_t>> r;
                 for (int64_t k = 0; k < 8; k++) r.append(std::make_tuple(py::str("r3"), k, k));
                 return r;
             }(),
             py::list<py::str>{py::str("r3")}, 64) {}

// ---- the x86 shadows ----------------------------------------------------------------------------------------------
std::shared_ptr<native_shadow::Shadow> Shadow16(const std::shared_ptr<pcode::Lifter>& lifter, SegBase segbase,
                                                const Inputs& inputs, const Span& span) {
    if (segbase != &windows16::selector_base && segbase != &real_mode)
        throw py::ValueError("Shadow16: a selector's base is the GDT's or real mode's");
    native_shadow::require();
    return std::make_shared<native_shadow::Shadow>(16, lifter, inputs, span, segbase == &real_mode);
}

std::shared_ptr<native_shadow::Shadow> Shadow32(const std::shared_ptr<pcode::Lifter>& lifter, const Inputs& inputs,
                                                const Span& span) {
    native_shadow::require();
    return std::make_shared<native_shadow::Shadow>(32, lifter, inputs, span);
}

std::shared_ptr<native_shadow::Shadow> ShadowLE(const std::shared_ptr<pcode::Lifter>& lifter, const Inputs& inputs,
                                                const Span& span) {
    native_shadow::require();
    return std::make_shared<native_shadow::Shadow>(33, lifter, inputs, span);
}

}  // namespace codebreaker::emulate::shadow
