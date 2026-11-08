// codebreaker/emulate/m6502.cpp -- typed C++ port of codebreaker.emulate.m6502. See m6502.hpp.

#include "codebreaker/emulate/m6502.hpp"

#include "codebreaker/decompile/m6502.hpp"

namespace codebreaker::emulate::m6502 {

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

// the register a mnemonic's third letter names (LDA -> a, STX -> x, TAY -> y)
int64_t& reg_of(CPU& c, const py::str& m) {
    switch (m.u32()[2]) {
        case U'A': return c.a;
        case U'X': return c.x;
        default: return c.y;       // 'Y'
    }
}

}  // namespace

// ---- Bus

Bus::Bus(const std::optional<py::bytearray>& memory) : mem(memory ? *memory : py::bytearray::zeros(0x10000)) {}

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

// ---- CPU

CPU::CPU(const Memory& memory, bool decimal_) : m(memory), decimal(decimal_) {}

// ---- memory helpers
int64_t CPU::rd(int64_t a) { return mem_get(m, a & 0xFFFF); }

void CPU::wr(int64_t a, int64_t v) { mem_set(m, a & 0xFFFF, v & 0xFF); }

int64_t CPU::rd16(int64_t a) {
    int64_t lo = rd(a);
    return lo | (rd(a + 1) << 8);
}

int64_t CPU::rd16_zp(int64_t a) {                  // a pointer in page zero wraps within it
    int64_t lo = rd(a & 0xFF);
    return lo | (rd((a + 1) & 0xFF) << 8);
}

void CPU::push(int64_t v) {
    wr(0x100 | s, v);
    s = (s - 1) & 0xFF;
}

int64_t CPU::pull() {
    s = (s + 1) & 0xFF;
    return rd(0x100 | s);
}

void CPU::flag(int64_t f, bool on) { p = on ? (p | f) : (p & ~f); }

int64_t CPU::nz(int64_t v) {
    v &= 0xFF;
    flag(_Z, v == 0);
    flag(_N, (v & 0x80) != 0);
    return v;
}

// ---- operands
int64_t CPU::address(const py::str& mode) {
    int64_t pc_ = pc;
    if (mode == "zp") return rd(pc_ + 1);
    if (mode == "zpx") return (rd(pc_ + 1) + x) & 0xFF;
    if (mode == "zpy") return (rd(pc_ + 1) + y) & 0xFF;
    if (mode == "abs") return rd16(pc_ + 1);
    if (mode == "abx") return (rd16(pc_ + 1) + x) & 0xFFFF;
    if (mode == "aby") return (rd16(pc_ + 1) + y) & 0xFFFF;
    if (mode == "izx") return rd16_zp(rd(pc_ + 1) + x);
    if (mode == "izy") return (rd16_zp(rd(pc_ + 1)) + y) & 0xFFFF;
    if (mode == "ind") {                           // the page-wrap of the original chip
        int64_t ptr = rd16(pc_ + 1);
        int64_t lo = rd(ptr);
        return lo | (rd((ptr & 0xFF00) | ((ptr + 1) & 0xFF)) << 8);
    }
    throw py::ValueError(mode);
}

int64_t CPU::load(const py::str& mode) {
    if (mode == "imm") return rd(pc + 1);
    if (mode == "acc") return a;
    return rd(address(mode));
}

void CPU::store_back(const py::str& mode, std::optional<int64_t> addr, int64_t v) {
    if (mode == "acc") a = v & 0xFF;
    else wr(*addr, v);
}

// ---- arithmetic
void CPU::adc(int64_t v) {
    int64_t c = p & _C;
    if (decimal && (p & _D)) {
        int64_t lo = (a & 0x0F) + (v & 0x0F) + c;
        int64_t hi = (a >> 4) + (v >> 4) + (lo > 9 ? 1 : 0);
        int64_t binary = a + v + c;
        flag(_Z, (binary & 0xFF) == 0);
        flag(_N, (hi & 0x08) != 0);
        flag(_V, (~(a ^ v) & (a ^ (hi << 4)) & 0x80) != 0);
        if (lo > 9) lo += 6;
        if (hi > 9) hi += 6;
        flag(_C, hi > 15);
        a = ((hi << 4) | (lo & 0x0F)) & 0xFF;
        return;
    }
    int64_t r = a + v + c;
    flag(_C, r > 0xFF);
    flag(_V, (~(a ^ v) & (a ^ r) & 0x80) != 0);
    a = nz(r);
}

void CPU::sbc(int64_t v) {
    if (decimal && (p & _D)) {
        int64_t borrow = 1 - (p & _C);
        int64_t r = a - v - borrow;
        int64_t lo = (a & 0x0F) - (v & 0x0F) - borrow;
        int64_t hi = (a >> 4) - (v >> 4) - (lo < 0 ? 1 : 0);
        flag(_C, r >= 0);
        flag(_V, ((a ^ v) & (a ^ r) & 0x80) != 0);
        nz(r);
        if (lo < 0) lo -= 6;
        if (hi < 0) hi -= 6;
        a = ((hi << 4) | (lo & 0x0F)) & 0xFF;
        return;
    }
    adc(v ^ 0xFF);
}

void CPU::compare(int64_t reg, int64_t v) {
    flag(_C, reg >= v);
    nz(reg - v);
}

// ---- one instruction
void CPU::step() {
    int64_t op = rd(pc);
    if (!decompile::m6502::OPCODES.contains(op))
        throw Stop(py::percent(py::str("undocumented opcode %02X at %04X"), op, pc));
    const auto& entry = decompile::m6502::OPCODES.at(op);
    const py::str& m = std::get<0>(entry);
    const py::str& mode = std::get<1>(entry);
    int64_t size;
    if (mode == "imp" || mode == "acc") size = 1;
    else if (mode == "imm" || mode == "zp" || mode == "zpx" || mode == "zpy" || mode == "izx" || mode == "izy" || mode == "rel") size = 2;
    else if (mode == "abs" || mode == "abx" || mode == "aby" || mode == "ind") size = 3;
    else throw py::KeyError(py::repr(mode));
    int64_t nxt = (pc + size) & 0xFFFF;
    steps += 1;
    if (m == "LDA" || m == "LDX" || m == "LDY") {
        int64_t v = nz(load(mode));
        reg_of(*this, m) = v;
    } else if (m == "STA" || m == "STX" || m == "STY") {
        int64_t at = address(mode);
        wr(at, reg_of(*this, m));
    } else if (m == "ADC") {
        adc(load(mode));
    } else if (m == "SBC") {
        sbc(load(mode));
    } else if (m == "AND" || m == "ORA" || m == "EOR") {
        int64_t v = load(mode);
        a = nz(m == "AND" ? (a & v) : m == "ORA" ? (a | v) : (a ^ v));
    } else if (m == "CMP" || m == "CPX" || m == "CPY") {
        int64_t reg = m == "CMP" ? a : m == "CPX" ? x : y;
        compare(reg, load(mode));
    } else if (m == "BIT") {
        int64_t v = load(mode);
        flag(_Z, (a & v) == 0);
        flag(_N, (v & 0x80) != 0);
        flag(_V, (v & 0x40) != 0);
    } else if (m == "ASL" || m == "LSR" || m == "ROL" || m == "ROR") {
        std::optional<int64_t> addr;
        if (mode != "acc") addr = address(mode);
        int64_t v = mode == "acc" ? a : rd(*addr);
        int64_t c = p & _C;
        int64_t out, r;
        if (m == "ASL") {
            out = v & 0x80;
            r = v << 1;
        } else if (m == "LSR") {
            out = v & 1;
            r = v >> 1;
        } else if (m == "ROL") {
            out = v & 0x80;
            r = (v << 1) | c;
        } else {
            out = v & 1;
            r = (v >> 1) | (c << 7);
        }
        flag(_C, out != 0);
        store_back(mode, addr, nz(r));
    } else if (m == "INC" || m == "DEC") {
        int64_t addr = address(mode);
        wr(addr, nz(rd(addr) + (m == "INC" ? 1 : -1)));
    } else if (m == "INX" || m == "DEX") {
        x = nz(x + (m == "INX" ? 1 : -1));
    } else if (m == "INY" || m == "DEY") {
        y = nz(y + (m == "INY" ? 1 : -1));
    } else if (m == "TAX" || m == "TAY" || m == "TXA" || m == "TYA" || m == "TSX") {
        int64_t src = m == "TAX" || m == "TAY" ? a : m == "TXA" ? x : m == "TYA" ? y : s;
        if (m == "TSX") x = nz(src);
        else reg_of(*this, m) = nz(src);
    } else if (m == "TXS") {
        s = x;                                     // (no flags)
    } else if (m == "CLC" || m == "SEC" || m == "CLI" || m == "SEI" || m == "CLD" || m == "SED" || m == "CLV") {
        char32_t which = m.u32()[2];
        int64_t f = which == U'C' ? _C : which == U'I' ? _I : which == U'D' ? _D : _V;
        flag(f, m.u32()[0] == U'S');
    } else if (m == "PHA") {
        push(a);
    } else if (m == "PHP") {
        push(p | _B | _U);
    } else if (m == "PLA") {
        a = nz(pull());
    } else if (m == "PLP") {
        p = (pull() & ~_B) | _U;
    } else if (m == "BCC" || m == "BCS" || m == "BEQ" || m == "BMI" || m == "BNE" || m == "BPL" || m == "BVC" || m == "BVS") {
        int64_t bit = (m == "BCC" || m == "BCS") ? _C : (m == "BEQ" || m == "BNE") ? _Z : (m == "BMI" || m == "BPL") ? _N : _V;
        bool want = m == "BCS" || m == "BEQ" || m == "BMI" || m == "BVS";
        if (((p & bit) != 0) == want) {
            int64_t d = rd(pc + 1);
            nxt = (nxt + ((d & 0x80) ? d - 256 : d)) & 0xFFFF;
        }
    } else if (m == "JMP") {
        nxt = address(mode);
    } else if (m == "JSR") {
        int64_t ret = (pc + 2) & 0xFFFF;           // the address of its own last byte
        push(ret >> 8);
        push(ret & 0xFF);
        nxt = address("abs");
    } else if (m == "RTS") {
        int64_t lo = pull();
        int64_t hi = pull();
        nxt = (((hi << 8) | lo) + 1) & 0xFFFF;
    } else if (m == "RTI") {
        p = (pull() & ~_B) | _U;
        int64_t lo = pull();
        int64_t hi = pull();
        nxt = (hi << 8) | lo;
    } else if (m == "BRK") {
        int64_t ret = (pc + 2) & 0xFFFF;
        push(ret >> 8);
        push(ret & 0xFF);
        push(p | _B | _U);
        flag(_I, true);
        nxt = rd16(0xFFFE);
    } else if (m == "NOP") {
        // nothing
    } else {
        throw Stop(py::percent(py::str("no meaning for %s"), m));
    }
    pc = nxt;
}

py::dict<py::str, int64_t> CPU::flags() const {
    return py::dict<py::str, int64_t>{{py::str("N"), (p >> 7) & 1}, {py::str("V"), (p >> 6) & 1}, {py::str("B"), (p >> 4) & 1},
                                      {py::str("D"), (p >> 3) & 1}, {py::str("I"), (p >> 2) & 1}, {py::str("Z"), (p >> 1) & 1},
                                      {py::str("C"), p & 1}};
}

RunRecord run(const Memory& memory, int64_t entry, int64_t a, int64_t x, int64_t y, int64_t p, int64_t steps, bool decimal) {
    CPU cpu(memory, decimal);
    cpu.a = a & 0xFF;
    cpu.x = x & 0xFF;
    cpu.y = y & 0xFF;
    cpu.p = p;
    int64_t ret = RETURN - 1;
    cpu.push(ret >> 8);
    cpu.push(ret & 0xFF);
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
    out.x = cpu.x;
    out.y = cpu.y;
    out.p = cpu.p;
    out.s = cpu.s;
    out.steps = cpu.steps;
    if (const auto* bus = std::get_if<py::ref<Bus>>(&memory)) out.io = py::list<IoAccess>((*bus)->log.vec());   // list(getattr(memory, "log", []))
    return out;
}

emu_run::Run record(const RunRecord& out) {
    py::dict<py::str, emu_run::Register> results;
    results.set(py::str("A"), emu_run::Register(out.a));
    results.set(py::str("X"), emu_run::Register(out.x));
    results.set(py::str("Y"), emu_run::Register(out.y));
    results.set(py::str("P"), emu_run::Register(out.p));
    results.set(py::str("S"), emu_run::Register(out.s));
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
    return record(run(memory, entry, kw.a, kw.x, kw.y, kw.p, kw.steps, kw.decimal));
}

}  // namespace codebreaker::emulate::m6502
