// cb-unity: alone
#include "emulate/m6809.hpp"

#include <format>
#include <unordered_map>

#include "core/error.hpp"

namespace cb::emulate::m6809 {

namespace dm = decompile::m6809;

// ---- the bus --------------------------------------------------------------------------------------------------------

Bus::Bus() : ram_(kMemorySize, 0) {}

Bus::Bus(ByteView image) : ram_(kMemorySize, 0) {
    if (image.size() > kMemorySize) throw UsageError("a 6809 image is at most 64 KB");
    std::copy(image.begin(), image.end(), ram_.begin());
}

void Bus::map(std::uint16_t lo, std::uint16_t hi, ReadFn read, WriteFn write) { ranges_.push_back({lo, hi, std::move(read), std::move(write), true}); }
void Bus::map_quiet(std::uint16_t lo, std::uint16_t hi, ReadFn read, WriteFn write) { ranges_.push_back({lo, hi, std::move(read), std::move(write), false}); }

std::uint8_t Bus::read(std::uint16_t address) {
    for (const Range& r : ranges_) {
        if (address < r.lo || address > r.hi) continue;
        const std::uint8_t v = r.read ? r.read(address) : 0;
        if (r.logged) log_.push_back({DeviceAccess::Kind::Read, address, v});
        return v;
    }
    return ram_[address];
}

void Bus::write(std::uint16_t address, std::uint8_t value) {
    for (const Range& r : ranges_) {
        if (address < r.lo || address > r.hi) continue;
        if (r.logged) log_.push_back({DeviceAccess::Kind::Write, address, value});
        if (r.write) r.write(address, value);
        return;
    }
    ram_[address] = value;
}

// ---- the CPU --------------------------------------------------------------------------------------------------------

namespace {

enum class O : std::uint8_t {
    Neg, Com, Lsr, Ror, Asr, Lsl, Rol, Dec, Inc, Tst, Clr, Jmp, Nop, Sync, Lbra, Lbsr, Daa, Orcc, Andcc, Sex, Exg, Tfr, Leax, Leay, Leas, Leau, Pshs, Puls, Pshu, Pulu,
    Rts, Abx, Rti, Cwai, Mul, Swi, Swi2, Swi3, Bra, Brn, Bhi, Bls, Bcc, Bcs, Bne, Beq, Bvc, Bvs, Bpl, Bmi, Bge, Blt, Bgt, Ble, Bsr, Jsr,
    Suba, Cmpa, Sbca, Anda, Bita, Lda, Sta, Eora, Adca, Ora, Adda, Subb, Cmpb, Sbcb, Andb, Bitb, Ldb, Stb, Eorb, Adcb, Orb, Addb,
    Subd, Addd, Cmpd, Ldd, Std, Cmpx, Ldx, Stx, Cmpy, Ldy, Sty, Cmpu, Ldu, Stu, Cmps, Lds, Sts,
    Nega, Coma, Lsra, Rora, Asra, Lsla, Rola, Deca, Inca, Tsta, Clra, Negb, Comb, Lsrb, Rorb, Asrb, Lslb, Rolb, Decb, Incb, Tstb, Clrb, Unknown,
};

O op_of(std::string_view m) {
    static const std::unordered_map<std::string_view, O> table = {
        {"NEG", O::Neg}, {"COM", O::Com}, {"LSR", O::Lsr}, {"ROR", O::Ror}, {"ASR", O::Asr}, {"LSL", O::Lsl}, {"ROL", O::Rol}, {"DEC", O::Dec}, {"INC", O::Inc}, {"TST", O::Tst},
        {"CLR", O::Clr}, {"JMP", O::Jmp}, {"NOP", O::Nop}, {"SYNC", O::Sync}, {"LBRA", O::Lbra}, {"LBSR", O::Lbsr}, {"DAA", O::Daa}, {"ORCC", O::Orcc}, {"ANDCC", O::Andcc},
        {"SEX", O::Sex}, {"EXG", O::Exg}, {"TFR", O::Tfr}, {"LEAX", O::Leax}, {"LEAY", O::Leay}, {"LEAS", O::Leas}, {"LEAU", O::Leau}, {"PSHS", O::Pshs}, {"PULS", O::Puls},
        {"PSHU", O::Pshu}, {"PULU", O::Pulu}, {"RTS", O::Rts}, {"ABX", O::Abx}, {"RTI", O::Rti}, {"CWAI", O::Cwai}, {"MUL", O::Mul}, {"SWI", O::Swi}, {"SWI2", O::Swi2},
        {"SWI3", O::Swi3}, {"BRA", O::Bra}, {"BRN", O::Brn}, {"BHI", O::Bhi}, {"BLS", O::Bls}, {"BCC", O::Bcc}, {"BCS", O::Bcs}, {"BNE", O::Bne}, {"BEQ", O::Beq},
        {"BVC", O::Bvc}, {"BVS", O::Bvs}, {"BPL", O::Bpl}, {"BMI", O::Bmi}, {"BGE", O::Bge}, {"BLT", O::Blt}, {"BGT", O::Bgt}, {"BLE", O::Ble}, {"BSR", O::Bsr}, {"JSR", O::Jsr},
        {"LBRN", O::Brn}, {"LBHI", O::Bhi}, {"LBLS", O::Bls}, {"LBCC", O::Bcc}, {"LBCS", O::Bcs}, {"LBNE", O::Bne}, {"LBEQ", O::Beq}, {"LBVC", O::Bvc}, {"LBVS", O::Bvs},
        {"LBPL", O::Bpl}, {"LBMI", O::Bmi}, {"LBGE", O::Bge}, {"LBLT", O::Blt}, {"LBGT", O::Bgt}, {"LBLE", O::Ble},
        {"SUBA", O::Suba}, {"CMPA", O::Cmpa}, {"SBCA", O::Sbca}, {"ANDA", O::Anda}, {"BITA", O::Bita}, {"LDA", O::Lda}, {"STA", O::Sta}, {"EORA", O::Eora}, {"ADCA", O::Adca},
        {"ORA", O::Ora}, {"ADDA", O::Adda}, {"SUBB", O::Subb}, {"CMPB", O::Cmpb}, {"SBCB", O::Sbcb}, {"ANDB", O::Andb}, {"BITB", O::Bitb}, {"LDB", O::Ldb}, {"STB", O::Stb},
        {"EORB", O::Eorb}, {"ADCB", O::Adcb}, {"ORB", O::Orb}, {"ADDB", O::Addb}, {"SUBD", O::Subd}, {"ADDD", O::Addd}, {"CMPD", O::Cmpd}, {"LDD", O::Ldd}, {"STD", O::Std},
        {"CMPX", O::Cmpx}, {"LDX", O::Ldx}, {"STX", O::Stx}, {"CMPY", O::Cmpy}, {"LDY", O::Ldy}, {"STY", O::Sty}, {"CMPU", O::Cmpu}, {"LDU", O::Ldu}, {"STU", O::Stu},
        {"CMPS", O::Cmps}, {"LDS", O::Lds}, {"STS", O::Sts}, {"NEGA", O::Nega}, {"COMA", O::Coma}, {"LSRA", O::Lsra}, {"RORA", O::Rora}, {"ASRA", O::Asra}, {"LSLA", O::Lsla},
        {"ROLA", O::Rola}, {"DECA", O::Deca}, {"INCA", O::Inca}, {"TSTA", O::Tsta}, {"CLRA", O::Clra}, {"NEGB", O::Negb}, {"COMB", O::Comb}, {"LSRB", O::Lsrb}, {"RORB", O::Rorb},
        {"ASRB", O::Asrb}, {"LSLB", O::Lslb}, {"ROLB", O::Rolb}, {"DECB", O::Decb}, {"INCB", O::Incb}, {"TSTB", O::Tstb}, {"CLRB", O::Clrb}};
    const auto it = table.find(m);
    return it == table.end() ? O::Unknown : it->second;
}

struct Exec {
    Registers& r;
    Bus& bus;

    std::uint16_t read16(std::uint16_t a) { return static_cast<std::uint16_t>(bus.read(a) << 8 | bus.read(static_cast<std::uint16_t>(a + 1))); }
    void write16(std::uint16_t a, std::uint16_t v) {
        bus.write(a, static_cast<std::uint8_t>(v >> 8));
        bus.write(static_cast<std::uint16_t>(a + 1), static_cast<std::uint8_t>(v));
    }
    std::uint16_t d() const { return static_cast<std::uint16_t>(r.a << 8 | r.b); }
    void set_d(std::uint16_t v) { r.a = static_cast<std::uint8_t>(v >> 8); r.b = static_cast<std::uint8_t>(v); }
    std::uint16_t& index_reg(unsigned i) { return i == 0 ? r.x : i == 1 ? r.y : i == 2 ? r.u : r.s; }

    void flag(std::uint8_t bit, bool on) { r.cc = on ? (r.cc | bit) : (r.cc & ~bit); }
    void nz8(std::uint8_t v) { flag(cc::N, v & 0x80); flag(cc::Z, v == 0); }
    void nz16(std::uint16_t v) { flag(cc::N, v & 0x8000); flag(cc::Z, v == 0); }

    std::uint8_t add8(std::uint8_t a, std::uint8_t b, unsigned carry) {
        const unsigned s = a + b + carry;
        const std::uint8_t res = static_cast<std::uint8_t>(s);
        flag(cc::H, ((a ^ b ^ res) & 0x10) != 0);
        nz8(res);
        flag(cc::V, ((a ^ res) & (b ^ res) & 0x80) != 0);
        flag(cc::C, s > 0xFF);
        return res;
    }
    std::uint8_t sub8(std::uint8_t a, std::uint8_t b, unsigned borrow) {
        const int s = a - b - static_cast<int>(borrow);
        const std::uint8_t res = static_cast<std::uint8_t>(s);
        nz8(res);
        flag(cc::V, ((a ^ b) & (a ^ res) & 0x80) != 0);
        flag(cc::C, s < 0);
        return res;
    }
    std::uint16_t add16(std::uint16_t a, std::uint16_t b) {
        const unsigned s = a + b;
        const std::uint16_t res = static_cast<std::uint16_t>(s);
        nz16(res);
        flag(cc::V, ((a ^ res) & (b ^ res) & 0x8000) != 0);
        flag(cc::C, s > 0xFFFF);
        return res;
    }
    std::uint16_t sub16(std::uint16_t a, std::uint16_t b) {
        const int s = a - b;
        const std::uint16_t res = static_cast<std::uint16_t>(s);
        nz16(res);
        flag(cc::V, ((a ^ b) & (a ^ res) & 0x8000) != 0);
        flag(cc::C, s < 0);
        return res;
    }
    void logic(std::uint8_t v) { nz8(v); flag(cc::V, false); }

    // The read-modify-write instructions on a byte.
    std::uint8_t rmw(O op, std::uint8_t v) {
        const unsigned c = r.cc & cc::C ? 1u : 0u;
        std::uint8_t res = v;
        switch (op) {
            case O::Neg: case O::Nega: case O::Negb: res = sub8(0, v, 0); break;
            case O::Com: case O::Coma: case O::Comb: res = static_cast<std::uint8_t>(~v); nz8(res); flag(cc::V, false); flag(cc::C, true); break;
            case O::Lsr: case O::Lsra: case O::Lsrb: flag(cc::C, v & 1); res = v >> 1; nz8(res); break;
            case O::Ror: case O::Rora: case O::Rorb: flag(cc::C, v & 1); res = static_cast<std::uint8_t>(c << 7 | v >> 1); nz8(res); break;
            case O::Asr: case O::Asra: case O::Asrb: flag(cc::C, v & 1); res = static_cast<std::uint8_t>((v >> 1) | (v & 0x80)); nz8(res); break;
            case O::Lsl: case O::Lsla: case O::Lslb: flag(cc::C, v & 0x80); res = static_cast<std::uint8_t>(v << 1); nz8(res); flag(cc::V, ((r.cc & cc::N) != 0) != ((r.cc & cc::C) != 0)); break;
            case O::Rol: case O::Rola: case O::Rolb: flag(cc::C, v & 0x80); res = static_cast<std::uint8_t>(v << 1 | c); nz8(res); flag(cc::V, ((r.cc & cc::N) != 0) != ((r.cc & cc::C) != 0)); break;
            case O::Dec: case O::Deca: case O::Decb: res = static_cast<std::uint8_t>(v - 1); nz8(res); flag(cc::V, v == 0x80); break;
            case O::Inc: case O::Inca: case O::Incb: res = static_cast<std::uint8_t>(v + 1); nz8(res); flag(cc::V, v == 0x7F); break;
            case O::Tst: case O::Tsta: case O::Tstb: nz8(v); flag(cc::V, false); break;
            default: res = 0; flag(cc::N, false); flag(cc::Z, true); flag(cc::V, false); flag(cc::C, false); break;   // CLR
        }
        return res;
    }

    bool taken(O op) const {
        const bool c = r.cc & cc::C, z = r.cc & cc::Z, n = r.cc & cc::N, v = r.cc & cc::V;
        switch (op) {
            case O::Bra: return true;
            case O::Brn: return false;
            case O::Bhi: return !c && !z;
            case O::Bls: return c || z;
            case O::Bcc: return !c;
            case O::Bcs: return c;
            case O::Bne: return !z;
            case O::Beq: return z;
            case O::Bvc: return !v;
            case O::Bvs: return v;
            case O::Bpl: return !n;
            case O::Bmi: return n;
            case O::Bge: return n == v;
            case O::Blt: return n != v;
            case O::Bgt: return !z && n == v;
            default: return z || n != v;   // BLE
        }
    }

    // The effective address of a direct, extended or indexed operand (the auto-increments happen here).
    std::uint16_t ea(const dm::Insn& i) {
        switch (i.mode) {
            case dm::Mode::Dir: return static_cast<std::uint16_t>(r.dp << 8 | i.value);
            case dm::Mode::Ext: return static_cast<std::uint16_t>(i.value);
            default: break;
        }
        const dm::Index& ix = i.index;
        std::uint16_t& base = index_reg(ix.reg);
        std::uint16_t a = 0;
        using K = dm::Index::Kind;
        switch (ix.kind) {
            case K::Offset5: case K::Offset8: case K::Offset16: a = static_cast<std::uint16_t>(base + ix.offset); break;
            case K::A: a = static_cast<std::uint16_t>(base + static_cast<std::int8_t>(r.a)); break;
            case K::B: a = static_cast<std::uint16_t>(base + static_cast<std::int8_t>(r.b)); break;
            case K::D: a = static_cast<std::uint16_t>(base + d()); break;
            case K::Inc1: a = base; base = static_cast<std::uint16_t>(base + 1); break;
            case K::Inc2: a = base; base = static_cast<std::uint16_t>(base + 2); break;
            case K::Dec1: base = static_cast<std::uint16_t>(base - 1); a = base; break;
            case K::Dec2: base = static_cast<std::uint16_t>(base - 2); a = base; break;
            case K::Zero: a = base; break;
            case K::Pc8: case K::Pc16: a = static_cast<std::uint16_t>(i.at + i.size + ix.offset); break;
            case K::Extended: a = static_cast<std::uint16_t>(ix.offset); break;
        }
        return ix.indirect ? read16(a) : a;
    }

    std::uint8_t operand8(const dm::Insn& i, std::uint16_t& address) {
        if (i.mode == dm::Mode::Imm8) return static_cast<std::uint8_t>(i.value);
        address = ea(i);
        return bus.read(address);
    }
    std::uint16_t operand16(const dm::Insn& i, std::uint16_t& address) {
        if (i.mode == dm::Mode::Imm16) return static_cast<std::uint16_t>(i.value);
        address = ea(i);
        return read16(address);
    }

    void push8(std::uint16_t& sp, std::uint8_t v) { sp = static_cast<std::uint16_t>(sp - 1); bus.write(sp, v); }
    void push16(std::uint16_t& sp, std::uint16_t v) { push8(sp, static_cast<std::uint8_t>(v)); push8(sp, static_cast<std::uint8_t>(v >> 8)); }
    std::uint8_t pull8(std::uint16_t& sp) { const std::uint8_t v = bus.read(sp); sp = static_cast<std::uint16_t>(sp + 1); return v; }
    std::uint16_t pull16(std::uint16_t& sp) { const std::uint8_t hi = pull8(sp); return static_cast<std::uint16_t>(hi << 8 | pull8(sp)); }

    std::uint16_t get_reg(unsigned code) {
        switch (code) {
            case 0: return d();
            case 1: return r.x;
            case 2: return r.y;
            case 3: return r.u;
            case 4: return r.s;
            case 5: return r.pc;
            case 8: return r.a;
            case 9: return r.b;
            case 10: return r.cc;
            default: return r.dp;
        }
    }
    void set_reg(unsigned code, std::uint16_t v) {
        switch (code) {
            case 0: set_d(v); break;
            case 1: r.x = v; break;
            case 2: r.y = v; break;
            case 3: r.u = v; break;
            case 4: r.s = v; break;
            case 5: r.pc = v; break;
            case 8: r.a = static_cast<std::uint8_t>(v); break;
            case 9: r.b = static_cast<std::uint8_t>(v); break;
            case 10: r.cc = static_cast<std::uint8_t>(v); break;
            default: r.dp = static_cast<std::uint8_t>(v); break;
        }
    }

    void push_regs(std::uint16_t& sp, std::uint16_t other, unsigned mask) {
        if (mask & 0x80) push16(sp, r.pc);
        if (mask & 0x40) push16(sp, other);
        if (mask & 0x20) push16(sp, r.y);
        if (mask & 0x10) push16(sp, r.x);
        if (mask & 0x08) push8(sp, r.dp);
        if (mask & 0x04) push8(sp, r.b);
        if (mask & 0x02) push8(sp, r.a);
        if (mask & 0x01) push8(sp, r.cc);
    }
    void pull_regs(std::uint16_t& sp, std::uint16_t& other, unsigned mask) {
        if (mask & 0x01) r.cc = pull8(sp);
        if (mask & 0x02) r.a = pull8(sp);
        if (mask & 0x04) r.b = pull8(sp);
        if (mask & 0x08) r.dp = pull8(sp);
        if (mask & 0x10) r.x = pull16(sp);
        if (mask & 0x20) r.y = pull16(sp);
        if (mask & 0x40) other = pull16(sp);
        if (mask & 0x80) r.pc = pull16(sp);
    }
};

}  // namespace

std::optional<Fault> Cpu::step() {
    const std::optional<dm::Insn> found = dm::decode(bus_->memory(), regs.pc, regs.pc);
    if (!found) return Fault{regs.pc, std::format("undefined opcode {:02X} at {:04X}", bus_->memory()[regs.pc], regs.pc)};
    const dm::Insn& i = *found;
    const O op = op_of(i.mnemonic);
    Exec x{regs, *bus_};
    const std::uint16_t next = static_cast<std::uint16_t>(i.at + i.size);
    regs.pc = next;
    std::uint16_t a = 0;
    switch (op) {
        case O::Unknown: regs.pc = static_cast<std::uint16_t>(i.at); return Fault{regs.pc, std::format("not run: {} at {:04X}", i.mnemonic, i.at)};
        case O::Nop: break;
        case O::Sync: case O::Cwai: case O::Swi: case O::Swi2: case O::Swi3:
            regs.pc = static_cast<std::uint16_t>(i.at);
            return Fault{regs.pc, std::format("waits for an interrupt ({}) at {:04X}", i.mnemonic, i.at)};
        case O::Neg: case O::Com: case O::Lsr: case O::Ror: case O::Asr: case O::Lsl: case O::Rol: case O::Dec: case O::Inc: case O::Tst: case O::Clr: {
            a = x.ea(i);
            const std::uint8_t v = op == O::Clr ? std::uint8_t{0} : bus_->read(a);   // (CLR reads nothing it uses)
            const std::uint8_t res = x.rmw(op, v);
            if (op != O::Tst) bus_->write(a, res);
            break;
        }
        case O::Nega: case O::Coma: case O::Lsra: case O::Rora: case O::Asra: case O::Lsla: case O::Rola: case O::Deca: case O::Inca: case O::Tsta: case O::Clra:
            regs.a = x.rmw(op, regs.a);
            break;
        case O::Negb: case O::Comb: case O::Lsrb: case O::Rorb: case O::Asrb: case O::Lslb: case O::Rolb: case O::Decb: case O::Incb: case O::Tstb: case O::Clrb:
            regs.b = x.rmw(op, regs.b);
            break;
        case O::Jmp: regs.pc = x.ea(i); break;
        case O::Lbra: case O::Bra: case O::Brn: case O::Bhi: case O::Bls: case O::Bcc: case O::Bcs: case O::Bne: case O::Beq: case O::Bvc: case O::Bvs: case O::Bpl: case O::Bmi:
        case O::Bge: case O::Blt: case O::Bgt: case O::Ble:
            if (x.taken(op == O::Lbra ? O::Bra : op)) regs.pc = static_cast<std::uint16_t>(*i.target);
            break;
        case O::Bsr: case O::Lbsr: x.push16(regs.s, next); regs.pc = static_cast<std::uint16_t>(*i.target); break;
        case O::Jsr: a = x.ea(i); x.push16(regs.s, next); regs.pc = a; break;
        case O::Rts: regs.pc = x.pull16(regs.s); break;
        case O::Rti:
            regs.cc = x.pull8(regs.s);
            if (regs.cc & cc::E) {
                regs.a = x.pull8(regs.s);
                regs.b = x.pull8(regs.s);
                regs.dp = x.pull8(regs.s);
                regs.x = x.pull16(regs.s);
                regs.y = x.pull16(regs.s);
                regs.u = x.pull16(regs.s);
            }
            regs.pc = x.pull16(regs.s);
            break;
        case O::Daa: {
            unsigned adjust = 0;
            const unsigned hi = regs.a >> 4, lo = regs.a & 15;
            const bool c = regs.cc & cc::C;
            if ((regs.cc & cc::H) || lo > 9) adjust |= 0x06;
            if (c || hi > 9 || (hi > 8 && lo > 9)) adjust |= 0x60;
            const unsigned sum = regs.a + adjust;
            regs.a = static_cast<std::uint8_t>(sum);
            x.nz8(regs.a);
            x.flag(cc::C, c || sum > 0xFF);
            break;
        }
        case O::Orcc: regs.cc = static_cast<std::uint8_t>(regs.cc | i.value); break;
        case O::Andcc: regs.cc = static_cast<std::uint8_t>(regs.cc & i.value); break;
        case O::Sex: regs.a = (regs.b & 0x80) ? 0xFF : 0; x.nz16(x.d()); break;
        case O::Exg: {
            const unsigned p = i.value >> 4, q = i.value & 15;
            const std::uint16_t vp = x.get_reg(p), vq = x.get_reg(q);
            x.set_reg(p, vq);
            x.set_reg(q, vp);
            break;
        }
        case O::Tfr: x.set_reg(i.value & 15, x.get_reg(i.value >> 4)); break;
        case O::Leax: case O::Leay: case O::Leas: case O::Leau: {
            a = x.ea(i);
            if (op == O::Leax) { regs.x = a; x.flag(cc::Z, a == 0); }
            else if (op == O::Leay) { regs.y = a; x.flag(cc::Z, a == 0); }
            else if (op == O::Leas) regs.s = a;
            else regs.u = a;
            break;
        }
        case O::Pshs: x.push_regs(regs.s, regs.u, i.value); break;
        case O::Pshu: x.push_regs(regs.u, regs.s, i.value); break;
        case O::Puls: x.pull_regs(regs.s, regs.u, i.value); break;
        case O::Pulu: x.pull_regs(regs.u, regs.s, i.value); break;
        case O::Abx: regs.x = static_cast<std::uint16_t>(regs.x + regs.b); break;
        case O::Mul: {
            const std::uint16_t p = static_cast<std::uint16_t>(regs.a * regs.b);
            x.set_d(p);
            x.flag(cc::Z, p == 0);
            x.flag(cc::C, p & 0x80);
            break;
        }
        case O::Suba: case O::Cmpa: case O::Sbca: case O::Anda: case O::Bita: case O::Lda: case O::Eora: case O::Adca: case O::Ora: case O::Adda: {
            const std::uint8_t v = x.operand8(i, a);
            const unsigned c = regs.cc & cc::C ? 1u : 0u;
            switch (op) {
                case O::Suba: regs.a = x.sub8(regs.a, v, 0); break;
                case O::Cmpa: (void)x.sub8(regs.a, v, 0); break;
                case O::Sbca: regs.a = x.sub8(regs.a, v, c); break;
                case O::Anda: regs.a &= v; x.logic(regs.a); break;
                case O::Bita: x.logic(regs.a & v); break;
                case O::Lda: regs.a = v; x.logic(v); break;
                case O::Eora: regs.a ^= v; x.logic(regs.a); break;
                case O::Adca: regs.a = x.add8(regs.a, v, c); break;
                case O::Ora: regs.a |= v; x.logic(regs.a); break;
                default: regs.a = x.add8(regs.a, v, 0); break;
            }
            break;
        }
        case O::Subb: case O::Cmpb: case O::Sbcb: case O::Andb: case O::Bitb: case O::Ldb: case O::Eorb: case O::Adcb: case O::Orb: case O::Addb: {
            const std::uint8_t v = x.operand8(i, a);
            const unsigned c = regs.cc & cc::C ? 1u : 0u;
            switch (op) {
                case O::Subb: regs.b = x.sub8(regs.b, v, 0); break;
                case O::Cmpb: (void)x.sub8(regs.b, v, 0); break;
                case O::Sbcb: regs.b = x.sub8(regs.b, v, c); break;
                case O::Andb: regs.b &= v; x.logic(regs.b); break;
                case O::Bitb: x.logic(regs.b & v); break;
                case O::Ldb: regs.b = v; x.logic(v); break;
                case O::Eorb: regs.b ^= v; x.logic(regs.b); break;
                case O::Adcb: regs.b = x.add8(regs.b, v, c); break;
                case O::Orb: regs.b |= v; x.logic(regs.b); break;
                default: regs.b = x.add8(regs.b, v, 0); break;
            }
            break;
        }
        case O::Sta: a = x.ea(i); bus_->write(a, regs.a); x.logic(regs.a); break;
        case O::Stb: a = x.ea(i); bus_->write(a, regs.b); x.logic(regs.b); break;
        case O::Subd: x.set_d(x.sub16(x.d(), x.operand16(i, a))); break;
        case O::Addd: x.set_d(x.add16(x.d(), x.operand16(i, a))); break;
        case O::Cmpd: (void)x.sub16(x.d(), x.operand16(i, a)); break;
        case O::Cmpx: (void)x.sub16(regs.x, x.operand16(i, a)); break;
        case O::Cmpy: (void)x.sub16(regs.y, x.operand16(i, a)); break;
        case O::Cmpu: (void)x.sub16(regs.u, x.operand16(i, a)); break;
        case O::Cmps: (void)x.sub16(regs.s, x.operand16(i, a)); break;
        case O::Ldd: { const std::uint16_t v = x.operand16(i, a); x.set_d(v); x.nz16(v); x.flag(cc::V, false); break; }
        case O::Ldx: { const std::uint16_t v = x.operand16(i, a); regs.x = v; x.nz16(v); x.flag(cc::V, false); break; }
        case O::Ldy: { const std::uint16_t v = x.operand16(i, a); regs.y = v; x.nz16(v); x.flag(cc::V, false); break; }
        case O::Ldu: { const std::uint16_t v = x.operand16(i, a); regs.u = v; x.nz16(v); x.flag(cc::V, false); break; }
        case O::Lds: { const std::uint16_t v = x.operand16(i, a); regs.s = v; x.nz16(v); x.flag(cc::V, false); break; }
        case O::Std: a = x.ea(i); x.write16(a, x.d()); x.nz16(x.d()); x.flag(cc::V, false); break;
        case O::Stx: a = x.ea(i); x.write16(a, regs.x); x.nz16(regs.x); x.flag(cc::V, false); break;
        case O::Sty: a = x.ea(i); x.write16(a, regs.y); x.nz16(regs.y); x.flag(cc::V, false); break;
        case O::Stu: a = x.ea(i); x.write16(a, regs.u); x.nz16(regs.u); x.flag(cc::V, false); break;
        case O::Sts: a = x.ea(i); x.write16(a, regs.s); x.nz16(regs.s); x.flag(cc::V, false); break;
    }
    ++steps;
    return std::nullopt;
}

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string("?"));
        default: return "limit";
    }
}

RunResult run(Bus& bus, std::uint16_t entry, const RunOptions& options) {
    Cpu cpu(bus);
    cpu.regs.a = options.a;
    cpu.regs.b = options.b;
    cpu.regs.dp = options.dp;
    cpu.regs.x = options.x;
    cpu.regs.y = options.y;
    cpu.regs.u = options.u;
    cpu.regs.s = static_cast<std::uint16_t>(options.s - 2);
    bus.write(cpu.regs.s, static_cast<std::uint8_t>(kReturnAddress >> 8));
    bus.write(static_cast<std::uint16_t>(cpu.regs.s + 1), static_cast<std::uint8_t>(kReturnAddress));
    cpu.regs.pc = entry;
    const std::size_t logged_before = bus.log().size();
    RunResult result;
    while (cpu.steps < options.max_steps) {
        if (cpu.regs.pc == kReturnAddress) {
            result.end = End::Return;
            break;
        }
        if (options.on_instruction) options.on_instruction(cpu.regs);
        if (const std::optional<Fault> fault = cpu.step()) {
            result.end = End::Stopped;
            result.fault = fault;
            break;
        }
    }
    if (result.end == End::Limit && cpu.regs.pc == kReturnAddress) result.end = End::Return;
    result.regs = cpu.regs;
    result.steps = cpu.steps;
    result.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(logged_before), bus.log().end());
    return result;
}

}  // namespace cb::emulate::m6809
