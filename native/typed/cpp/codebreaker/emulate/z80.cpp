// codebreaker/emulate/z80.cpp -- typed C++ port of codebreaker.emulate.z80. See z80.hpp.

#include "codebreaker/emulate/z80.hpp"

namespace codebreaker::emulate::z80 {

namespace {

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

// the Bus behind the memory, or null for a plain bytearray (the Python's hasattr(c.m, "inp"))
Bus* bus_of(CPU& c) {
    if (auto* b = std::get_if<py::ref<Bus>>(&c.m)) return b->get();
    return nullptr;
}

py::set<py::str> names(std::initializer_list<const char*> xs) {
    py::set<py::str> out;
    for (const char* x : xs) out.add(py::str(x));
    return out;
}

const py::set<py::str> _REG8 = names({"A", "B", "C", "D", "E", "H", "L", "IXH", "IXL", "IYH", "IYL", "I", "R"});
const py::set<py::str> _REG16 = names({"AF", "BC", "DE", "HL", "SP", "IX", "IY"});

py::list<int64_t> make_sz() {
    py::list<int64_t> out;
    for (int64_t v = 0; v < 256; ++v) out.append((v & (FS | FY | FX)) | (v == 0 ? FZ : 0));
    return out;
}

const py::list<int64_t> _SZ = make_sz();

py::list<int64_t> make_szp() {
    py::list<int64_t> out;
    for (int64_t v = 0; v < 256; ++v) out.append(_SZ[v] | _parity(v));
    return out;
}

const py::list<int64_t> _SZP = make_szp();

py::dict<py::str, Handler> _HANDLERS;

}  // namespace

// ---- Bus

Bus::Bus(const std::optional<py::bytearray>& memory, const PortIn& inp_, const PortOut& outp_)
    : mem(memory ? *memory : py::bytearray::zeros(0x10000)), _inp(inp_), _outp(outp_) {}

void Bus::map(int64_t lo, int64_t hi, const IoRead& read, const IoWrite& write) {
    io.append(IoRange(lo, hi, read, write));
}

int64_t Bus::getitem(int64_t a) {
    for (const IoRange& r : io) {
        int64_t lo = std::get<0>(r), hi = std::get<1>(r);
        const IoRead& read = std::get<2>(r);
        if (lo <= a && a <= hi) {
            int64_t v = read ? (read(a) & 0xFF) : 0;
            log.append(IoAccess(py::str("r"), a, v));
            return v;
        }
    }
    return mem[a];
}

void Bus::setitem(int64_t a, int64_t v) {
    for (const IoRange& r : io) {
        int64_t lo = std::get<0>(r), hi = std::get<1>(r);
        const IoWrite& write = std::get<3>(r);
        if (lo <= a && a <= hi) {
            log.append(IoAccess(py::str("w"), a, v & 0xFF));
            if (write) write(a, v & 0xFF);
            return;
        }
    }
    mem[a] = v & 0xFF;
}

int64_t Bus::inp(int64_t port) {
    int64_t v = (_inp ? _inp(port) : 0xFF) & 0xFF;
    log.append(IoAccess(py::str("in"), port & 0xFF, v));
    return v;
}

void Bus::outp(int64_t port, int64_t v) {
    log.append(IoAccess(py::str("out"), port & 0xFF, v & 0xFF));
    if (_outp) _outp(port, v & 0xFF);
}

int64_t _parity(int64_t v) {
    v ^= v >> 4;
    v ^= v >> 2;
    v ^= v >> 1;
    return (v & 1) ? 0 : FP;
}

// ---- CPU

CPU::CPU(const Memory& memory) : m(memory) {}

// ---- memory
int64_t CPU::rd(int64_t a_) { return mem_get(m, a_ & 0xFFFF); }

void CPU::wr(int64_t a_, int64_t v) { mem_set(m, a_ & 0xFFFF, v & 0xFF); }

int64_t CPU::rd16(int64_t a_) {
    int64_t lo = rd(a_);
    int64_t hi = rd(a_ + 1);
    return lo | (hi << 8);
}

void CPU::wr16(int64_t a_, int64_t v) {
    wr(a_, v);
    wr(a_ + 1, v >> 8);
}

void CPU::push(int64_t v) {
    sp = (sp - 2) & 0xFFFF;
    wr16(sp, v);
}

int64_t CPU::pop() {
    int64_t v = rd16(sp);
    sp = (sp + 2) & 0xFFFF;
    return v;
}

// ---- register pairs and halves
int64_t& CPU::field(const py::str& name) {
    if (name == "A") return a;
    if (name == "F") return f;
    if (name == "B") return b;
    if (name == "C") return c;
    if (name == "D") return d;
    if (name == "E") return e;
    if (name == "H") return h;
    if (name == "L") return l;
    if (name == "SP") return sp;
    if (name == "IX") return ix;
    if (name == "IY") return iy;
    if (name == "I") return i;
    if (name == "R") return r;
    throw py::KeyError(py::repr(name));
}

namespace {

// (high, low) registers of a pair name, or null
bool pair_of(CPU& c, const py::str& name, int64_t*& hi, int64_t*& lo) {
    if (name == "AF") { hi = &c.a; lo = &c.f; return true; }
    if (name == "BC") { hi = &c.b; lo = &c.c; return true; }
    if (name == "DE") { hi = &c.d; lo = &c.e; return true; }
    if (name == "HL") { hi = &c.h; lo = &c.l; return true; }
    return false;
}

bool is_index_half(const py::str& name) {
    return name == "IXH" || name == "IXL" || name == "IYH" || name == "IYL";
}

}  // namespace

int64_t CPU::get(const py::str& name) {
    int64_t *hi = nullptr, *lo = nullptr;
    if (pair_of(*this, name, hi, lo)) return (*hi << 8) | *lo;
    if (is_index_half(name)) {
        int64_t v = name.u32()[1] == U'X' ? ix : iy;
        return name.u32()[2] == U'H' ? v >> 8 : v & 0xFF;
    }
    return field(name);
}

void CPU::set(const py::str& name, int64_t v) {
    int64_t *hi = nullptr, *lo = nullptr;
    if (pair_of(*this, name, hi, lo)) {
        *hi = (v >> 8) & 0xFF;
        *lo = v & 0xFF;
    } else if (is_index_half(name)) {
        int64_t& fld = name.u32()[1] == U'X' ? ix : iy;
        int64_t old = fld;
        fld = name.u32()[2] == U'H' ? (((v & 0xFF) << 8) | (old & 0xFF)) : ((old & 0xFF00) | (v & 0xFF));
    } else {
        int64_t& fld = field(name);
        fld = v & ((name == "SP" || name == "IX" || name == "IY") ? 0xFFFF : 0xFF);
    }
}

bool CPU::cond(const py::str& cc) {
    int64_t fl = f;
    if (cc == "NZ") return !(fl & FZ);
    if (cc == "Z") return (fl & FZ) != 0;
    if (cc == "NC") return !(fl & FC);
    if (cc == "C") return (fl & FC) != 0;
    if (cc == "PO") return !(fl & FP);
    if (cc == "PE") return (fl & FP) != 0;
    if (cc == "P") return !(fl & FS);
    if (cc == "M") return (fl & FS) != 0;
    throw py::KeyError(py::repr(cc));
}

// ---- one instruction
py::bytes CPU::_bytes_at(int64_t pc_, int64_t n) {
    std::string s;
    for (int64_t k = 0; k < n; ++k) s.push_back(static_cast<char>(rd(pc_ + k)));
    return py::bytes(s);
}

void CPU::step() {
    if (halted) throw Stop(py::str("HALT"));
    int64_t pc_ = pc;
    std::optional<Decoded> cached = _decoded.get(pc_);
    if (!cached || !(cached->bytes == _bytes_at(pc_, cached->bytes.size()))) {
        py::bytes code = _bytes_at(pc_, 4);
        std::optional<table::Insn> dec = table::decode(code, 0, pc_);
        if (!dec) throw Stop(py::percent(py::str("undecodable at %04X"), pc_));
        cached = Decoded{*dec, dec->bytes};
        _decoded.set(pc_, *cached);
    }
    const table::Insn& insn = cached->insn;
    unsigned char first = insn.bytes.size() ? static_cast<unsigned char>(insn.bytes.raw()[0]) : 0;
    bool prefixed = insn.bytes.size() && (first == 0xDD || first == 0xFD || first == 0xCB || first == 0xED);
    r = (r & 0x80) | ((r + (prefixed ? 2 : 1)) & 0x7F);
    pc = (pc_ + insn.size) & 0xFFFF;
    _handler(insn.pattern)(*this, insn);
    steps += 1;
}

// ---- operands: (read, write) for each written form ----------------------------------------------

std::tuple<ReadFn, WriteFn, bool> _operand(const py::str& form, int64_t k) {
    if (_REG8.contains(form) || _REG16.contains(form)) {
        return {[form](CPU& c, const table::Insn&) { return c.get(form); },
                [form](CPU& c, const table::Insn&, int64_t v) { c.set(form, v); }, false};
    }
    if (form == "n" || form == "nn") {
        return {[k](CPU&, const table::Insn& d) { return d.values[k]; }, WriteFn(), true};
    }
    if (form == "(BC)" || form == "(DE)" || form == "(HL)") {
        py::str pair = form.slice(1, 3);
        return {[pair](CPU& c, const table::Insn&) { return c.rd(c.get(pair)); },
                [pair](CPU& c, const table::Insn&, int64_t v) { c.wr(c.get(pair), v); }, false};
    }
    if (form == "(nn)") {
        return {[k](CPU& c, const table::Insn& d) { return c.rd(d.values[k]); },
                [k](CPU& c, const table::Insn& d, int64_t v) { c.wr(d.values[k], v); }, true};
    }
    // \((I[XY])\+d\)$
    if (form.size() == 6 && form.startswith(py::str("(I")) && (form.u32()[2] == U'X' || form.u32()[2] == U'Y') &&
        form.endswith(py::str("+d)"))) {
        py::str reg = form.slice(1, 3);
        auto addr = [reg, k](CPU& c, const table::Insn& d) { return (c.get(reg) + d.values[k]) & 0xFFFF; };
        return {[addr](CPU& c, const table::Insn& d) { return c.rd(addr(c, d)); },
                [addr](CPU& c, const table::Insn& d, int64_t v) { c.wr(addr(c, d), v); }, true};
    }
    if (form.isdigit()) {
        int64_t n = py::int_parse(form, 10);
        return {[n](CPU&, const table::Insn&) { return n; }, WriteFn(), false};
    }
    // (C) (n) (SP) (IX) AF' and the like: read by their instruction's own handler
    return {ReadFn(), WriteFn(), form == "(n)"};
}

std::tuple<py::str, py::list<Operand>> _operands(const py::str& pattern) {
    auto [m, sep, rest] = pattern.partition(py::str(" "));
    py::list<py::str> forms = rest.size() ? py::split(rest, py::str(",")) : py::list<py::str>();
    py::list<Operand> out;
    int64_t k = 0;
    for (const py::str& f : forms) {
        ReadFn r;
        WriteFn w;
        bool takes = false;
        bool is_cc = table::CC.contains(f) && (m == "JP" || m == "JR" || m == "CALL" || m == "RET");
        if (!is_cc) std::tie(r, w, takes) = _operand(f, k);
        out.append(Operand{f, r, w});
        k += takes ? 1 : 0;
    }
    return {m, out};
}

// ---- arithmetic with flags ---------------------------------------------------------------------------

int64_t _add8(CPU& c, int64_t a, int64_t b, int64_t carry) {
    int64_t r = a + b + carry;
    int64_t f = _SZ[r & 0xFF] | ((a ^ b ^ r) & FH) | (r > 0xFF ? FC : 0);
    f |= (~(a ^ b) & (a ^ r) & 0x80) ? FP : 0;
    c.f = f;
    return r & 0xFF;
}

int64_t _sub8(CPU& c, int64_t a, int64_t b, int64_t carry, bool keep) {
    (void)keep;
    int64_t r = a - b - carry;
    int64_t f = _SZ[r & 0xFF] | FN | ((a ^ b ^ r) & FH) | (r < 0 ? FC : 0);
    f |= ((a ^ b) & (a ^ r) & 0x80) ? FP : 0;
    c.f = f;
    return r & 0xFF;
}

void _alu(CPU& c, const py::str& op, int64_t v) {
    int64_t a = c.a;
    if (op == "ADD") {
        c.a = _add8(c, a, v);
    } else if (op == "ADC") {
        c.a = _add8(c, a, v, c.f & FC);
    } else if (op == "SUB") {
        c.a = _sub8(c, a, v);
    } else if (op == "SBC") {
        c.a = _sub8(c, a, v, c.f & FC);
    } else if (op == "AND") {
        c.a = a & v;
        c.f = _SZP[c.a] | FH;
    } else if (op == "XOR") {
        c.a = a ^ v;
        c.f = _SZP[c.a];
    } else if (op == "OR") {
        c.a = a | v;
        c.f = _SZP[c.a];
    } else {  // CP: the flags of A - v, bits 5 and 3 from v
        _sub8(c, a, v);
        c.f = (c.f & ~(FY | FX)) | (v & (FY | FX));
    }
}

int64_t _rot(CPU& c, const py::str& op, int64_t v) {
    int64_t cy = c.f & FC;
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
    } else if (op == "SLL") {
        r = ((v << 1) | 1) & 0xFF;
        co = v >> 7;
    } else {  // SRL
        r = v >> 1;
        co = v & 1;
    }
    c.f = _SZP[r] | (co ? FC : 0);
    return r;
}

int64_t _inc8(CPU& c, int64_t v) {
    int64_t r = (v + 1) & 0xFF;
    c.f = (c.f & FC) | _SZ[r] | ((v & 0x0F) == 0x0F ? FH : 0) | (v == 0x7F ? FP : 0);
    return r;
}

int64_t _dec8(CPU& c, int64_t v) {
    int64_t r = (v - 1) & 0xFF;
    c.f = (c.f & FC) | _SZ[r] | FN | ((v & 0x0F) == 0 ? FH : 0) | (v == 0x80 ? FP : 0);
    return r;
}

int64_t _add16(CPU& c, int64_t a, int64_t b) {
    int64_t r = a + b;
    c.f = (c.f & (FS | FZ | FP)) | (((a ^ b ^ r) >> 8) & FH) | ((r >> 8) & (FY | FX)) | (r > 0xFFFF ? FC : 0);
    return r & 0xFFFF;
}

int64_t _adc16(CPU& c, int64_t a, int64_t b) {
    int64_t cy = c.f & FC;
    int64_t r = a + b + cy;
    int64_t f = ((r & 0x8000) ? FS : 0) | ((r & 0xFFFF) == 0 ? FZ : 0) | (((a ^ b ^ r) >> 8) & FH);
    f |= ((r >> 8) & (FY | FX)) | (r > 0xFFFF ? FC : 0);
    f |= (~(a ^ b) & (a ^ r) & 0x8000) ? FP : 0;
    c.f = f;
    return r & 0xFFFF;
}

int64_t _sbc16(CPU& c, int64_t a, int64_t b) {
    int64_t cy = c.f & FC;
    int64_t r = a - b - cy;
    int64_t f = FN | ((r & 0x8000) ? FS : 0) | ((r & 0xFFFF) == 0 ? FZ : 0) | (((a ^ b ^ r) >> 8) & FH);
    f |= ((r >> 8) & (FY | FX)) | (r < 0 ? FC : 0);
    f |= ((a ^ b) & (a ^ r) & 0x8000) ? FP : 0;
    c.f = f;
    return r & 0xFFFF;
}

// ---- the handlers ----------------------------------------------------------------------------------

Handler _handler(const py::str& pattern) {
    std::optional<Handler> h = _HANDLERS.get(pattern);
    if (!h) {
        h = _compile(pattern);
        _HANDLERS.set(pattern, *h);
    }
    return *h;
}

Handler _compile(const py::str& pattern) {
    std::tuple<py::str, py::list<Operand>> parsed = _operands(pattern);
    py::str m = std::get<0>(parsed);                 // (not a structured binding: the lambdas below capture it)
    py::list<Operand> ops = std::get<1>(parsed);
    py::list<py::str> forms;
    std::vector<ReadFn> rd;
    std::vector<WriteFn> wr;
    for (const Operand& o : ops) {
        forms.append(o.form);
        rd.push_back(o.read);
        wr.push_back(o.write);
    }
    auto form_is = [&](int64_t idx, const char* s) { return forms[idx] == s; };

    if (m == "NOP" || m == ".db") {                  // (.db: a lone DD/FD prefix or an ED opcode that does nothing)
        return [](CPU&, const table::Insn&) {};
    }
    if (m == "LD") {
        if (form_is(0, "A") && (form_is(1, "I") || form_is(1, "R"))) {
            py::str src = forms[1];
            return [src](CPU& c, const table::Insn&) {
                c.a = c.get(src);
                c.f = (c.f & FC) | _SZ[c.a] | (c.iff2 ? FP : 0);
            };
        }
        if (forms[0] == "(nn)" && _REG16.contains(forms[1])) {
            py::str src = forms[1];
            return [src](CPU& c, const table::Insn& d) { c.wr16(d.values[0], c.get(src)); };
        }
        if (forms[1] == "(nn)" && _REG16.contains(forms[0])) {
            py::str dst = forms[0];
            return [dst](CPU& c, const table::Insn& d) { c.set(dst, c.rd16(d.values[0])); };
        }
        if (forms[0].endswith(py::str("+d)")) && forms[1] == "n") {
            // LD (IX+d),n: the displacement is value 0, the byte value 1
            py::str reg = forms[0].slice(1, 3);
            return [reg](CPU& c, const table::Insn& d) { c.wr((c.get(reg) + d.values[0]) & 0xFFFF, d.values[1]); };
        }
        ReadFn r1 = rd[1];
        WriteFn w0 = wr[0];
        return [r1, w0](CPU& c, const table::Insn& d) { w0(c, d, r1(c, d)); };
    }
    if (m == "PUSH" || m == "POP") {
        py::str reg = forms[0];
        if (m == "PUSH") return [reg](CPU& c, const table::Insn&) { c.push(c.get(reg)); };
        return [reg](CPU& c, const table::Insn&) { c.set(reg, c.pop()); };
    }
    if (m == "EX") {
        if (forms == py::list<py::str>{py::str("AF"), py::str("AF'")}) {
            return [](CPU& c, const table::Insn&) {
                std::swap(c.a, c.a_);
                std::swap(c.f, c.f_);
            };
        }
        if (forms == py::list<py::str>{py::str("DE"), py::str("HL")}) {
            return [](CPU& c, const table::Insn&) {
                int64_t de = c.get(py::str("DE"));
                int64_t hl = c.get(py::str("HL"));
                c.set(py::str("DE"), hl);
                c.set(py::str("HL"), de);
            };
        }
        py::str reg = forms[1];   // EX (SP),HL/IX/IY
        return [reg](CPU& c, const table::Insn&) {
            int64_t v = c.rd16(c.sp);
            c.wr16(c.sp, c.get(reg));
            c.set(reg, v);
        };
    }
    if (m == "EXX") {
        return [](CPU& c, const table::Insn&) {
            std::swap(c.b, c.b_);
            std::swap(c.c, c.c_);
            std::swap(c.d, c.d_);
            std::swap(c.e, c.e_);
            std::swap(c.h, c.h_);
            std::swap(c.l, c.l_);
        };
    }
    if (m == "ADD" || m == "ADC" || m == "SUB" || m == "SBC" || m == "AND" || m == "XOR" || m == "OR" || m == "CP") {
        if ((forms[0] == "HL" || forms[0] == "IX" || forms[0] == "IY") && forms.size() == 2) {
            py::str dst = forms[0], src = forms[1];
            return [m, dst, src](CPU& c, const table::Insn&) {
                int64_t a = c.get(dst);
                int64_t b = c.get(src);
                c.set(dst, m == "ADD" ? _add16(c, a, b) : m == "ADC" ? _adc16(c, a, b) : _sbc16(c, a, b));
            };
        }
        ReadFn src = rd[rd.size() - 1];
        return [m, src](CPU& c, const table::Insn& d) { _alu(c, m, src(c, d)); };
    }
    if (m == "INC" || m == "DEC") {
        py::str f = forms[0];
        if (_REG16.contains(f)) {
            int64_t delta = m == "INC" ? 1 : -1;
            return [f, delta](CPU& c, const table::Insn&) { c.set(f, (c.get(f) + delta) & 0xFFFF); };
        }
        ReadFn r0 = rd[0];
        WriteFn w0 = wr[0];
        bool inc = m == "INC";
        return [r0, w0, inc](CPU& c, const table::Insn& d) {
            int64_t v = r0(c, d);
            w0(c, d, inc ? _inc8(c, v) : _dec8(c, v));
        };
    }
    if (m == "RLCA" || m == "RRCA" || m == "RLA" || m == "RRA") {
        py::str op = m.slice(0, -1);
        return [op](CPU& c, const table::Insn&) {
            int64_t keep = c.f & (FS | FZ | FP);
            c.a = _rot(c, op, c.a);
            c.f = keep | (c.f & FC) | (c.a & (FY | FX));
        };
    }
    if (table::ROT.contains(m)) {
        ReadFn r0 = rd[0];
        WriteFn w0 = wr[0];
        std::optional<py::str> copy;                  // DDCB undocumented: result also into a register
        if (forms.size() > 1) copy = forms[1];
        return [m, r0, w0, copy](CPU& c, const table::Insn& d) {
            int64_t v = _rot(c, m, r0(c, d));
            w0(c, d, v);
            if (copy) c.set(*copy, v);
        };
    }
    if (m == "BIT") {
        int64_t bit = py::int_parse(forms[0], 10);
        ReadFn r1 = rd[1];
        py::str operand = forms[1];
        bool indexed = operand.endswith(py::str("+d)")) || operand == "(HL)";
        return [bit, r1, operand, indexed](CPU& c, const table::Insn& d) {
            int64_t v = r1(c, d);
            int64_t t = v & (int64_t(1) << bit);
            int64_t f = (c.f & FC) | FH | (!t ? (FZ | FP) : 0) | (bit == 7 && t ? FS : 0);
            if (!indexed) {
                f |= v & (FY | FX);
            } else if (operand.endswith(py::str("+d)"))) {
                f |= ((c.get(operand.slice(1, 3)) + d.values[0]) >> 8) & (FY | FX);
            }
            c.f = f;
        };
    }
    if (m == "SET" || m == "RES") {
        int64_t bit = py::int_parse(forms[0], 10);
        ReadFn r1 = rd[1];
        WriteFn w1 = wr[1];
        std::optional<py::str> copy;
        if (forms.size() > 2) copy = forms[2];
        bool is_set = m == "SET";
        return [bit, r1, w1, copy, is_set](CPU& c, const table::Insn& d) {
            int64_t v = r1(c, d);
            v = is_set ? (v | (int64_t(1) << bit)) : (v & ~(int64_t(1) << bit));
            w1(c, d, v);
            if (copy) c.set(*copy, v);
        };
    }
    if (m == "JP" || m == "JR" || m == "CALL") {
        if (forms.size() && forms[0].startswith(py::str("("))) {      // JP (HL) / (IX) / (IY)
            py::str reg = forms[0].slice(1, -1);
            return [reg](CPU& c, const table::Insn&) { c.pc = c.get(reg); };
        }
        std::optional<py::str> cc;
        if (forms.size() == 2) cc = forms[0];
        bool is_call = m == "CALL";
        return [cc, is_call](CPU& c, const table::Insn& d) {
            if (!cc || c.cond(*cc)) {
                if (is_call) c.push(c.pc);
                c.pc = d.target.value();
            }
        };
    }
    if (m == "DJNZ") {
        return [](CPU& c, const table::Insn& d) {
            c.b = (c.b - 1) & 0xFF;
            if (c.b) c.pc = d.target.value();
        };
    }
    if (m == "RET") {
        std::optional<py::str> cc;
        if (forms.size()) cc = forms[0];
        return [cc](CPU& c, const table::Insn&) {
            if (!cc || c.cond(*cc)) c.pc = c.pop();
        };
    }
    if (m == "RETI" || m == "RETN") {
        return [](CPU& c, const table::Insn&) {
            c.iff1 = c.iff2;
            c.pc = c.pop();
        };
    }
    if (m == "RST") {
        return [](CPU& c, const table::Insn& d) {
            c.push(c.pc);
            c.pc = d.target.value();
        };
    }
    if (m == "HALT") {
        return [](CPU& c, const table::Insn&) { c.halted = true; };
    }
    if (m == "DI" || m == "EI") {
        bool on = m == "EI";
        return [on](CPU& c, const table::Insn&) { c.iff1 = c.iff2 = on; };
    }
    if (m == "IM") {
        int64_t mode = py::int_parse(forms[0], 10);
        return [mode](CPU& c, const table::Insn&) { c.im = mode; };
    }
    if (m == "CPL") {
        return [](CPU& c, const table::Insn&) {
            c.a ^= 0xFF;
            c.f = (c.f & (FS | FZ | FP | FC)) | FH | FN | (c.a & (FY | FX));
        };
    }
    if (m == "NEG") {
        return [](CPU& c, const table::Insn&) { c.a = _sub8(c, 0, c.a); };
    }
    if (m == "SCF") {
        return [](CPU& c, const table::Insn&) { c.f = (c.f & (FS | FZ | FP)) | FC | (c.a & (FY | FX)); };
    }
    if (m == "CCF") {
        return [](CPU& c, const table::Insn&) {
            c.f = ((c.f & (FS | FZ | FP | FC)) | ((c.f & FC) ? FH : 0) | (c.a & (FY | FX))) ^ FC;
        };
    }
    if (m == "DAA") {
        return [](CPU& c, const table::Insn&) {
            int64_t a = c.a, f = c.f;
            int64_t adjust = 0, carry = f & FC;
            if ((f & FH) || (a & 0x0F) > 9) adjust |= 0x06;
            if (carry || a > 0x99) {
                adjust |= 0x60;
                carry = FC;
            }
            int64_t r = ((f & FN) ? a - adjust : a + adjust) & 0xFF;
            int64_t h = (a ^ r) & FH;
            c.a = r;
            c.f = _SZP[r] | (f & FN) | h | carry;
        };
    }
    if (m == "RLD" || m == "RRD") {
        bool left = m == "RLD";
        return [left](CPU& c, const table::Insn&) {
            int64_t hl = c.get(py::str("HL"));
            int64_t v = c.rd(hl);
            if (left) {
                c.wr(hl, (v << 4) | (c.a & 0x0F));
                c.a = (c.a & 0xF0) | (v >> 4);
            } else {
                c.wr(hl, (v >> 4) | ((c.a & 0x0F) << 4));
                c.a = (c.a & 0xF0) | (v & 0x0F);
            }
            c.f = (c.f & FC) | _SZP[c.a];
        };
    }
    if (m == "IN") {
        if (forms == py::list<py::str>{py::str("A"), py::str("(n)")}) {
            return [](CPU& c, const table::Insn& d) {
                Bus* bus = bus_of(c);
                int64_t v = bus ? bus->inp(d.values[0] | (c.a << 8)) : 0xFF;
                c.a = v;
            };
        }
        std::optional<py::str> reg;
        if (!(forms[0] == "(C)")) reg = forms[0];
        return [reg](CPU& c, const table::Insn&) {
            Bus* bus = bus_of(c);
            int64_t v = bus ? bus->inp(c.get(py::str("BC"))) : 0xFF;
            if (reg) c.set(*reg, v);
            c.f = (c.f & FC) | _SZP[v];
        };
    }
    if (m == "OUT") {
        if (forms == py::list<py::str>{py::str("(n)"), py::str("A")}) {
            return [](CPU& c, const table::Insn& d) {
                Bus* bus = bus_of(c);
                if (bus) bus->outp(d.values[0] | (c.a << 8), c.a);
            };
        }
        py::str src = forms[1];
        return [src](CPU& c, const table::Insn&) {
            Bus* bus = bus_of(c);
            if (bus) bus->outp(c.get(py::str("BC")), src == "0" ? 0 : c.get(src));
        };
    }
    if (m == "LDI" || m == "LDD" || m == "LDIR" || m == "LDDR") {
        int64_t step = m.u32()[2] == U'I' ? 1 : -1;
        bool repeat = m.endswith(py::str("R"));
        return [step, repeat](CPU& c, const table::Insn&) {
            int64_t v = c.rd(c.get(py::str("HL")));
            c.wr(c.get(py::str("DE")), v);
            c.set(py::str("HL"), (c.get(py::str("HL")) + step) & 0xFFFF);
            c.set(py::str("DE"), (c.get(py::str("DE")) + step) & 0xFFFF);
            int64_t bc = (c.get(py::str("BC")) - 1) & 0xFFFF;
            c.set(py::str("BC"), bc);
            int64_t n = (v + c.a) & 0xFF;
            c.f = (c.f & (FS | FZ | FC)) | (bc ? FP : 0) | (n & FX) | ((n << 4) & FY);
            if (repeat && bc) c.pc = (c.pc - 2) & 0xFFFF;
        };
    }
    if (m == "CPI" || m == "CPD" || m == "CPIR" || m == "CPDR") {
        int64_t step = m.u32()[2] == U'I' ? 1 : -1;
        bool repeat = m.endswith(py::str("R"));
        return [step, repeat](CPU& c, const table::Insn&) {
            int64_t v = c.rd(c.get(py::str("HL")));
            int64_t r = (c.a - v) & 0xFF;
            int64_t h = (c.a ^ v ^ r) & FH;
            c.set(py::str("HL"), (c.get(py::str("HL")) + step) & 0xFFFF);
            int64_t bc = (c.get(py::str("BC")) - 1) & 0xFFFF;
            c.set(py::str("BC"), bc);
            int64_t n = (r - (h ? 1 : 0)) & 0xFF;
            c.f = (c.f & FC) | FN | (_SZ[r] & (FS | FZ)) | h | (bc ? FP : 0) | (n & FX) | ((n << 4) & FY);
            if (repeat && bc && r != 0) c.pc = (c.pc - 2) & 0xFFFF;
        };
    }
    if (m == "INI" || m == "IND" || m == "INIR" || m == "INDR" || m == "OUTI" || m == "OUTD" || m == "OTIR" ||
        m == "OTDR") {
        int64_t step = (m == "INI" || m == "INIR" || m == "OUTI" || m == "OTIR") ? 1 : -1;
        bool repeat = m.endswith(py::str("R")) && m != "OUTD";
        bool is_in = m.startswith(py::str("IN"));
        return [step, repeat, is_in](CPU& c, const table::Insn&) {
            int64_t hl = c.get(py::str("HL"));
            int64_t v;
            if (is_in) {
                Bus* bus = bus_of(c);
                v = bus ? bus->inp(c.get(py::str("BC"))) : 0xFF;
                c.wr(hl, v);
                c.b = (c.b - 1) & 0xFF;
            } else {
                v = c.rd(hl);
                c.b = (c.b - 1) & 0xFF;
                Bus* bus = bus_of(c);
                if (bus) bus->outp(c.get(py::str("BC")), v);
            }
            c.set(py::str("HL"), (hl + step) & 0xFFFF);
            c.f = _SZ[c.b] | ((v & 0x80) ? FN : 0) | (c.f & FC);
            if (repeat && c.b) c.pc = (c.pc - 2) & 0xFFFF;
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
    if (regs.ix) cpu.ix = *regs.ix;
    if (regs.iy) cpu.iy = *regs.iy;
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
    out.ix = cpu.ix;
    out.iy = cpu.iy;
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
    results.set(py::str("IX"), emu_run::Register(out.ix));
    results.set(py::str("IY"), emu_run::Register(out.iy));
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

}  // namespace codebreaker::emulate::z80
