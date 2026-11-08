// codebreaker/emulate/sm83.cpp -- typed C++ port of codebreaker.emulate.sm83. See sm83.hpp.

#include "codebreaker/emulate/sm83.hpp"

namespace codebreaker::emulate::sm83 {

namespace {

namespace z80 = ::codebreaker::emulate::z80;

int64_t mem_get(Memory& m, int64_t a) {
    if (auto* ba = std::get_if<py::bytearray>(&m)) return (*ba)[a];
    return std::get<py::ref<Bus>>(m)->getitem(a);
}

void mem_set(Memory& m, int64_t a, int64_t v) {
    if (auto* ba = std::get_if<py::bytearray>(&m)) {
        (*ba)[a] = v;
        return;
    }
    std::get<py::ref<Bus>>(m)->setitem(a, v);
}

py::set<py::str> names(std::initializer_list<const char*> xs) {
    py::set<py::str> out;
    for (const char* x : xs) out.add(py::str(x));
    return out;
}

// calling a reader or writer that is None
int64_t call_read(const ReadFn& r, CPU& c, const table::Insn& d) {
    if (!r) throw py::TypeError("'NoneType' object is not callable");
    return r(c, d);
}

void call_write(const WriteFn& w, CPU& c, const table::Insn& d, int64_t v) {
    if (!w) throw py::TypeError("'NoneType' object is not callable");
    w(c, d, v);
}

// 1 << bit (Python's ints are unbounded: a bit past the 64th is a mask no byte reaches, 0 here)
int64_t bit_mask(int64_t bit) {
    if (bit < 0) throw py::ValueError("negative shift count");
    return bit >= 63 ? 0 : (int64_t(1) << bit);
}

int64_t target_of(const table::Insn& d) {
    if (!d.target) throw py::TypeError("instruction has no target");
    return *d.target;
}

py::dict<py::str, Handler> _HANDLERS;

}  // namespace

const py::dict<py::str, std::tuple<py::str, py::str>> _PAIRS = {
    {py::str("AF"), {py::str("a"), py::str("f")}},
    {py::str("BC"), {py::str("b"), py::str("c")}},
    {py::str("DE"), {py::str("d"), py::str("e")}},
    {py::str("HL"), {py::str("h"), py::str("l")}},
};
const py::set<py::str> _REG8 = names({"A", "B", "C", "D", "E", "H", "L"});

// ---- CPU

CPU::CPU(const Memory& memory) : m(memory) {}

int64_t CPU::rd(int64_t a_) { return mem_get(m, a_ & 0xFFFF); }

void CPU::wr(int64_t a_, int64_t v) { mem_set(m, a_ & 0xFFFF, v & 0xFF); }

void CPU::push(int64_t v) {
    sp = (sp - 2) & 0xFFFF;
    wr(sp, v);
    wr(sp + 1, v >> 8);
}

int64_t CPU::pop() {
    int64_t v = rd(sp) | (rd(sp + 1) << 8);
    sp = (sp + 2) & 0xFFFF;
    return v;
}

namespace {

// getattr(cpu, name) / setattr(cpu, name, v) of the attributes that hold numbers; name is lower case
int64_t* attr_ref(CPU& c, const py::str& n) {
    if (n == "a") return &c.a;
    if (n == "f") return &c.f;
    if (n == "b") return &c.b;
    if (n == "c") return &c.c;
    if (n == "d") return &c.d;
    if (n == "e") return &c.e;
    if (n == "h") return &c.h;
    if (n == "l") return &c.l;
    if (n == "sp") return &c.sp;
    if (n == "pc") return &c.pc;
    if (n == "steps") return &c.steps;
    return nullptr;
}

int64_t attr_get(CPU& c, const py::str& n) {
    if (int64_t* p = attr_ref(c, n)) return *p;
    if (n == "ime") return c.ime ? 1 : 0;
    if (n == "halted") return c.halted ? 1 : 0;
    if (c._extra.contains(n)) return c._extra.at(n);
    throw py::AttributeError(py::percent(py::str("'CPU' object has no attribute %s"), py::repr(n)).utf8());
}

void attr_set(CPU& c, const py::str& n, int64_t v) {
    if (int64_t* p = attr_ref(c, n)) {
        *p = v;
    } else if (n == "ime") {
        c.ime = v != 0;
    } else if (n == "halted") {
        c.halted = v != 0;
    } else {
        c._extra.set(n, v);
    }
}

}  // namespace

int64_t CPU::get(const py::str& name) {
    if (_PAIRS.contains(name)) {
        const auto& hl = _PAIRS.at(name);
        return (attr_get(*this, std::get<0>(hl)) << 8) | attr_get(*this, std::get<1>(hl));
    }
    return attr_get(*this, name.lower());
}

void CPU::set(const py::str& name, int64_t v) {
    if (_PAIRS.contains(name)) {
        const auto& hl = _PAIRS.at(name);
        attr_set(*this, std::get<0>(hl), (v >> 8) & 0xFF);
        attr_set(*this, std::get<1>(hl), v & (std::get<1>(hl) == "f" ? 0xF0 : 0xFF));
    } else if (name == "SP") {
        sp = v & 0xFFFF;
    } else {
        attr_set(*this, name.lower(), v & 0xFF);
    }
}

bool CPU::cond(const py::str& cc) {
    // {"NZ": ..., "Z": ..., "NC": ..., "C": ...}[cc]
    if (cc == "NZ") return !(f & FZ);
    if (cc == "Z") return (f & FZ) != 0;
    if (cc == "NC") return !(f & FC);
    if (cc == "C") return (f & FC) != 0;
    throw py::KeyError(py::repr(cc));
}

namespace {

// bytes(self.rd(pc + i) for i in range(n)) of step(): read one by one, through the bus
py::bytes bytes_at(CPU& c, int64_t pc, int64_t n) {
    std::string s;
    for (int64_t i = 0; i < n; ++i) s.push_back(static_cast<char>(c.rd(pc + i)));
    return py::bytes(s);
}

}  // namespace

void CPU::step() {
    if (halted) throw Stop(py::str("HALT"));
    int64_t pc_ = pc;
    std::optional<table::Insn> cached = _decoded.get(pc_);
    if (!cached || !(cached->bytes == bytes_at(*this, pc_, cached->size))) {
        std::optional<table::Insn> dec = table::decode(bytes_at(*this, pc_, 3), 0, pc_);
        if (!dec) throw Stop(py::percent(py::str("no instruction at %04X"), pc_));
        cached = *dec;
        _decoded.set(pc_, *cached);
    }
    const table::Insn d = *cached;
    pc = (pc_ + d.size) & 0xFFFF;
    _handler(d.pattern)(*this, d);
    steps += 1;
}

// ---- operands: (read, write, consumes a value?) for each written form

std::tuple<ReadFn, WriteFn, bool> _operand(const py::str& form, int64_t k) {
    if (_REG8.contains(form) || form == "BC" || form == "DE" || form == "HL" || form == "SP" || form == "AF") {
        return {[form](CPU& c, const table::Insn&) { return c.get(form); },
                [form](CPU& c, const table::Insn&, int64_t v) { c.set(form, v); }, false};
    }
    if (form == "n" || form == "nn" || form == "s") {
        return {[k](CPU&, const table::Insn& d) { return d.values[k]; }, WriteFn(), true};
    }
    if (form == "(BC)" || form == "(DE)" || form == "(HL)") {
        py::str pair = form.slice(1, 3);
        return {[pair](CPU& c, const table::Insn&) { return c.rd(c.get(pair)); },
                [pair](CPU& c, const table::Insn&, int64_t v) { c.wr(c.get(pair), v); }, false};
    }
    if (form == "(nn)" || form == "(h)") {
        return {[k](CPU& c, const table::Insn& d) { return c.rd(d.values[k]); },
                [k](CPU& c, const table::Insn& d, int64_t v) { c.wr(d.values[k], v); }, true};
    }
    if (form == "(C)") {
        return {[](CPU& c, const table::Insn&) { return c.rd(0xFF00 + c.c); },
                [](CPU& c, const table::Insn&, int64_t v) { c.wr(0xFF00 + c.c, v); }, false};
    }
    if (form == "(HL+)" || form == "(HL-)") {
        int64_t step = form.u32()[3] == U'+' ? 1 : -1;
        return {[step](CPU& c, const table::Insn&) {
                    int64_t hl = c.get(py::str("HL"));
                    c.set(py::str("HL"), hl + step);
                    return c.rd(hl);
                },
                [step](CPU& c, const table::Insn&, int64_t v) {
                    int64_t hl = c.get(py::str("HL"));
                    c.set(py::str("HL"), hl + step);
                    c.wr(hl, v);
                },
                false};
    }
    if (form.isdigit()) {
        return {[form](CPU&, const table::Insn&) { return py::int_parse(form, 10); }, WriteFn(), false};
    }
    return {ReadFn(), WriteFn(), false};
}

// ---- the handlers

Handler _handler(const py::str& pattern) {
    std::optional<Handler> h = _HANDLERS.get(pattern);
    if (!h) {
        h = _compile(pattern);
        _HANDLERS.set(pattern, *h);
    }
    return *h;
}

void _alu(CPU& c, const py::str& op, int64_t v) {
    int64_t a = c.a, cy = (c.f & FC) ? 1 : 0;
    if (op == "ADD" || op == "ADC") {
        int64_t k = op == "ADC" ? cy : 0;
        int64_t r = a + v + k;
        c.f = ((r & 0xFF) == 0 ? FZ : 0) | (((a & 0xF) + (v & 0xF) + k) > 0xF ? FH : 0) | (r > 0xFF ? FC : 0);
        c.a = r & 0xFF;
    } else if (op == "SUB" || op == "SBC" || op == "CP") {
        int64_t k = op == "SBC" ? cy : 0;
        int64_t r = a - v - k;
        c.f = FN | ((r & 0xFF) == 0 ? FZ : 0) | (((a & 0xF) - (v & 0xF) - k) < 0 ? FH : 0) | (r < 0 ? FC : 0);
        if (op != "CP") c.a = r & 0xFF;
    } else if (op == "AND") {
        c.a &= v;
        c.f = (c.a == 0 ? FZ : 0) | FH;
    } else if (op == "XOR") {
        c.a ^= v;
        c.f = c.a == 0 ? FZ : 0;
    } else {
        c.a |= v;
        c.f = c.a == 0 ? FZ : 0;
    }
}

int64_t _rot(CPU& c, const py::str& op, int64_t v) {
    int64_t cy = (c.f & FC) ? 1 : 0;
    int64_t r, co;
    if (op == "RLC") {
        r = ((v << 1) | (v >> 7)) & 0xFF;
        co = v >> 7;
    } else if (op == "RRC") {
        r = ((v >> 1) | (v << 7)) & 0xFF;
        co = v & 1;
    } else if (op == "RL") {
        r = ((v << 1) | cy) & 0xFF;
        co = v >> 7;
    } else if (op == "RR") {
        r = (v >> 1) | (cy << 7);
        co = v & 1;
    } else if (op == "SLA") {
        r = (v << 1) & 0xFF;
        co = v >> 7;
    } else if (op == "SRA") {
        r = (v >> 1) | (v & 0x80);
        co = v & 1;
    } else if (op == "SWAP") {
        r = ((v << 4) | (v >> 4)) & 0xFF;
        co = 0;
    } else {
        r = v >> 1;
        co = v & 1;
    }
    c.f = (r == 0 ? FZ : 0) | (co ? FC : 0);
    return r;
}

int64_t _sp_plus(CPU& c, int64_t e) {
    int64_t sp = c.sp;
    c.f = (((sp & 0xF) + (e & 0xF)) > 0xF ? FH : 0) | (((sp & 0xFF) + (e & 0xFF)) > 0xFF ? FC : 0);
    return (sp + e) & 0xFFFF;
}

Handler _compile(const py::str& pattern) {
    const auto parts = pattern.partition(py::str(" "));   // m, _, rest = pattern.partition(" ")
    const py::str m = std::get<0>(parts);
    const py::str rest = std::get<2>(parts);
    py::list<py::str> forms = rest.size() ? py::split(rest, py::str(",")) : py::list<py::str>();
    py::list<ReadFn> rd;
    py::list<WriteFn> wr;
    int64_t k = 0;
    for (const py::str& f : forms) {
        if (table::CC.contains(f) && (m == "JP" || m == "JR" || m == "CALL" || m == "RET")) {
            rd.append(ReadFn());
            wr.append(WriteFn());
            continue;
        }
        auto [r, w, takes] = _operand(f, k);
        rd.append(r);
        wr.append(w);
        k += takes ? 1 : 0;
    }

    if (m == "NOP") {
        return [](CPU&, const table::Insn&) {};
    }
    if (m == "STOP") {
        return [](CPU&, const table::Insn&) { throw Stop(py::str("STOP")); };
    }
    if (pattern == "LD (nn),SP") {
        return [](CPU& c, const table::Insn& d) {
            c.wr(d.values[0], c.sp);
            c.wr(d.values[0] + 1, c.sp >> 8);
        };
    }
    if (pattern == "LD HL,SP+s") {
        return [](CPU& c, const table::Insn& d) { c.set(py::str("HL"), _sp_plus(c, d.values[0])); };
    }
    if (pattern == "ADD SP,s") {
        return [](CPU& c, const table::Insn& d) { c.sp = _sp_plus(c, d.values[0]); };
    }
    if (m == "LD" || m == "LDH") {
        ReadFn r1 = rd[1];
        WriteFn w0 = wr[0];
        return [r1, w0](CPU& c, const table::Insn& d) {
            int64_t v = call_read(r1, c, d);
            call_write(w0, c, d, v);
        };
    }
    if (m == "PUSH") {
        return [forms](CPU& c, const table::Insn&) { c.push(c.get(forms[0])); };
    }
    if (m == "POP") {
        return [forms](CPU& c, const table::Insn&) {
            py::str name = forms[0];
            c.set(name, c.pop());
        };
    }
    if (m == "ADD" || m == "ADC" || m == "SUB" || m == "SBC" || m == "AND" || m == "XOR" || m == "OR" || m == "CP") {
        if (pattern.startswith(py::str("ADD HL,"))) {
            py::str src = forms[1];
            return [src](CPU& c, const table::Insn&) {
                int64_t a = c.get(py::str("HL")), b = c.get(src);
                int64_t r = a + b;
                c.f = (c.f & FZ) | (((a & 0xFFF) + (b & 0xFFF)) > 0xFFF ? FH : 0) | (r > 0xFFFF ? FC : 0);
                c.set(py::str("HL"), r);
            };
        }
        ReadFn src = rd[-1];
        return [m, src](CPU& c, const table::Insn& d) { _alu(c, m, call_read(src, c, d)); };
    }
    if (m == "INC" || m == "DEC") {
        py::str f = forms[0];
        if (f == "BC" || f == "DE" || f == "HL" || f == "SP") {
            int64_t delta = m == "INC" ? 1 : -1;
            return [f, delta](CPU& c, const table::Insn&) { c.set(f, (c.get(f) + delta) & 0xFFFF); };
        }
        ReadFn r0 = rd[0];
        WriteFn w0 = wr[0];
        return [m, r0, w0](CPU& c, const table::Insn& d) {
            int64_t v = call_read(r0, c, d);
            int64_t r;
            if (m == "INC") {
                r = (v + 1) & 0xFF;
                c.f = (c.f & FC) | (r == 0 ? FZ : 0) | ((v & 0xF) == 0xF ? FH : 0);
            } else {
                r = (v - 1) & 0xFF;
                c.f = (c.f & FC) | FN | (r == 0 ? FZ : 0) | ((v & 0xF) == 0 ? FH : 0);
            }
            call_write(w0, c, d, r);
        };
    }
    if (m == "RLCA" || m == "RRCA" || m == "RLA" || m == "RRA") {
        py::str op = m.slice_to(-1);
        return [op](CPU& c, const table::Insn&) {
            c.a = _rot(c, op, c.a);
            c.f &= FC;                      // Z is always cleared
        };
    }
    if (table::ROT.contains(m)) {
        ReadFn r0 = rd[0];
        WriteFn w0 = wr[0];
        return [m, r0, w0](CPU& c, const table::Insn& d) {
            int64_t v = _rot(c, m, call_read(r0, c, d));
            call_write(w0, c, d, v);
        };
    }
    if (m == "BIT") {
        int64_t bit = py::int_parse(forms[0], 10);
        ReadFn r1 = rd[1];
        return [bit, r1](CPU& c, const table::Insn& d) {
            int64_t keep = c.f & FC;
            int64_t v = call_read(r1, c, d);
            c.f = keep | FH | ((v & bit_mask(bit)) ? 0 : FZ);
        };
    }
    if (m == "SET" || m == "RES") {
        int64_t bit = py::int_parse(forms[0], 10);
        ReadFn r1 = rd[1];
        WriteFn w1 = wr[1];
        return [m, bit, r1, w1](CPU& c, const table::Insn& d) {
            int64_t v = call_read(r1, c, d);
            v = m == "SET" ? (v | bit_mask(bit)) : (v & ~bit_mask(bit));
            call_write(w1, c, d, v);
        };
    }
    if (m == "JP" || m == "JR" || m == "CALL") {
        if (pattern == "JP HL") {
            return [](CPU& c, const table::Insn&) { c.pc = c.get(py::str("HL")); };
        }
        std::optional<py::str> cc;
        if (forms.size() == 2) cc = forms[0];
        return [m, cc](CPU& c, const table::Insn& d) {
            if (!cc || c.cond(*cc)) {
                if (m == "CALL") c.push(c.pc);
                c.pc = target_of(d);
            }
        };
    }
    if (m == "RET") {
        std::optional<py::str> cc;
        if (forms.size()) cc = forms[0];
        return [cc](CPU& c, const table::Insn&) {
            if (!cc || c.cond(*cc)) c.pc = c.pop();
        };
    }
    if (m == "RETI") {
        return [](CPU& c, const table::Insn&) {
            c.ime = true;
            c.pc = c.pop();
        };
    }
    if (m == "RST") {
        return [](CPU& c, const table::Insn& d) {
            c.push(c.pc);
            c.pc = target_of(d);
        };
    }
    if (m == "HALT") {
        return [](CPU& c, const table::Insn&) { c.halted = true; };
    }
    if (m == "DI" || m == "EI") {
        bool on = m == "EI";
        return [on](CPU& c, const table::Insn&) { c.ime = on; };
    }
    if (m == "CPL") {
        return [](CPU& c, const table::Insn&) {
            c.a ^= 0xFF;
            c.f = (c.f & (FZ | FC)) | FN | FH;
        };
    }
    if (m == "SCF") {
        return [](CPU& c, const table::Insn&) { c.f = (c.f & FZ) | FC; };
    }
    if (m == "CCF") {
        return [](CPU& c, const table::Insn&) { c.f = (c.f & FZ) | ((c.f & FC) ? 0 : FC); };
    }
    if (m == "DAA") {
        return [](CPU& c, const table::Insn&) {
            int64_t a = c.a, f = c.f;
            int64_t carry = f & FC;
            if (f & FN) {
                if (f & FH) a = (a - 0x06) & 0xFF;
                if (carry) a = (a - 0x60) & 0xFF;
            } else {
                if ((f & FH) || (a & 0x0F) > 9) a += 0x06;
                if (carry || a > 0x9F) {
                    a += 0x60;
                    carry = FC;
                }
            }
            c.a = a & 0xFF;
            c.f = (c.a == 0 ? FZ : 0) | (f & FN) | carry;
        };
    }
    throw py::ValueError(py::percent(py::str("no handler for %r"), pattern));
}

RunRecord run(const Memory& memory, int64_t entry, int64_t steps, int64_t sp, const Regs& regs) {
    CPU cpu(memory);
    if (regs.a) cpu.a = *regs.a;
    if (regs.f) cpu.f = *regs.f;
    if (regs.b) cpu.b = *regs.b;
    if (regs.c) cpu.c = *regs.c;
    if (regs.d) cpu.d = *regs.d;
    if (regs.e) cpu.e = *regs.e;
    if (regs.h) cpu.h = *regs.h;
    if (regs.l) cpu.l = *regs.l;
    if (regs.ime) cpu.ime = *regs.ime;
    if (regs.halted) cpu.halted = *regs.halted;
    cpu.sp = sp;
    cpu.push(RETURN);
    cpu.pc = entry;
    py::str end("limit");
    try {
        while (cpu.steps < steps) {
            if (cpu.pc == RETURN) {
                end = py::str("return");
                break;
            }
            cpu.step();
        }
    } catch (const Stop& e) {
        end = py::percent(py::str("stopped: %s"), py::str(e.what()));
    }
    RunRecord out;
    out.end = end;
    out.a = cpu.a;
    out.f = cpu.f;
    out.bc = cpu.get(py::str("BC"));
    out.de = cpu.get(py::str("DE"));
    out.hl = cpu.get(py::str("HL"));
    out.sp = cpu.sp;
    out.steps = cpu.steps;
    if (const auto* bus = std::get_if<py::ref<Bus>>(&memory)) out.io = py::list<IoAccess>((*bus)->log.vec());   // list(getattr(memory, "log", []))
    return out;
}

emu_run::Run record(const RunRecord& out) {
    py::dict<py::str, emu_run::Register> results;
    results.set(py::str("A"), emu_run::Register(out.a));
    results.set(py::str("F"), emu_run::Register(out.f));
    results.set(py::str("BC"), emu_run::Register(out.bc));
    results.set(py::str("DE"), emu_run::Register(out.de));
    results.set(py::str("HL"), emu_run::Register(out.hl));
    results.set(py::str("SP"), emu_run::Register(out.sp));
    py::list<emu_run::IO> io;
    for (const IoAccess& t : out.io) {
        emu_run::IO a;
        a.kind = std::get<0>(t);
        a.port = std::get<1>(t);
        a.value = std::get<2>(t);
        io.append(a);
    }
    return emu_run::Run(out.end, results, py::list<emu_run::Write>(), false, py::list<emu_run::Call>(), io, std::nullopt, std::nullopt,
                        py::list<int64_t>(), out.steps, out.end == "limit", py::str("little"), std::make_shared<RunRecord>(out));
}

emu_run::Run recorded(const Memory& memory, int64_t entry, const RunOptions& kw) {
    return record(run(memory, entry, kw.steps, kw.sp, kw.regs));
}

}  // namespace codebreaker::emulate::sm83
