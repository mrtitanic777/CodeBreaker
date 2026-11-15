#include "cb/emulate/w65816.hpp"

#include <algorithm>
#include <format>
#include <string_view>

#include "cb/core/error.hpp"
#include "cb/decompile/w65816.hpp"

namespace cb::emulate::w65816 {

namespace dec = cb::decompile::w65816;
using Mode = dec::Mode;

namespace {

constexpr std::size_t kLogLimit = 200000;   // accesses kept of a run (a DMA of 64 KB is that many writes)

enum class Op : std::uint8_t {
    Adc, And, Asl, Bcc, Bcs, Beq, Bit, Bmi, Bne, Bpl, Bra, Brk, Brl, Bvc, Bvs, Clc, Cld, Cli, Clv, Cmp, Cop, Cpx, Cpy, Dec, Dex, Dey, Eor, Inc, Inx, Iny,
    Jml, Jmp, Jsl, Jsr, Lda, Ldx, Ldy, Lsr, Mvn, Mvp, Nop, Ora, Pea, Pei, Per, Pha, Phb, Phd, Phk, Php, Phx, Phy, Pla, Plb, Pld, Plp, Plx, Ply, Rep, Rol,
    Ror, Rti, Rtl, Rts, Sbc, Sec, Sed, Sei, Sep, Sta, Stp, Stx, Sty, Stz, Tax, Tay, Tcd, Tcs, Tdc, Trb, Tsb, Tsc, Tsx, Txa, Txs, Txy, Tya, Tyx, Wai, Wdm, Xba, Xce,
};

constexpr std::string_view kNames[] = {
    "ADC", "AND", "ASL", "BCC", "BCS", "BEQ", "BIT", "BMI", "BNE", "BPL", "BRA", "BRK", "BRL", "BVC", "BVS", "CLC", "CLD", "CLI", "CLV", "CMP", "COP", "CPX", "CPY", "DEC", "DEX", "DEY", "EOR", "INC", "INX", "INY",
    "JML", "JMP", "JSL", "JSR", "LDA", "LDX", "LDY", "LSR", "MVN", "MVP", "NOP", "ORA", "PEA", "PEI", "PER", "PHA", "PHB", "PHD", "PHK", "PHP", "PHX", "PHY", "PLA", "PLB", "PLD", "PLP", "PLX", "PLY", "REP", "ROL",
    "ROR", "RTI", "RTL", "RTS", "SBC", "SEC", "SED", "SEI", "SEP", "STA", "STP", "STX", "STY", "STZ", "TAX", "TAY", "TCD", "TCS", "TDC", "TRB", "TSB", "TSC", "TSX", "TXA", "TXS", "TXY", "TYA", "TYX", "WAI", "WDM", "XBA", "XCE",
};
static_assert(std::size(kNames) == static_cast<std::size_t>(Op::Xce) + 1);

struct Entry {
    Op op = Op::Nop;
    Mode mode = Mode::Imp;
};

// The 256 opcodes resolved once from the decoder's table (the one description the disassembler and the assembler read too).
const std::array<Entry, 256>& table() {
    static const std::array<Entry, 256> t = [] {
        std::array<Entry, 256> out{};
        for (std::size_t code = 0; code < 256; ++code) {
            const dec::OpcodeEntry& e = dec::opcode_table()[code];
            const auto it = std::find(std::begin(kNames), std::end(kNames), e.mnemonic);
            CB_ASSERT(it != std::end(kNames), "an opcode of the table the interpreter does not know");
            out[code] = {static_cast<Op>(it - std::begin(kNames)), e.mode};
        }
        return out;
    }();
    return t;
}

}  // namespace

// ---- Machine -------------------------------------------------------------------------------------------------------

std::string Access::text() const {
    std::string name = formats::snes::register_name(address);
    if (name.empty()) name = std::format("IO_{:04X}", address);
    return std::format("{} {} {:02x}", write ? "w" : "r", name, value);
}

Machine::Machine(const formats::snes::Rom& rom) : rom_(rom) {}

std::uint8_t Machine::read(std::uint32_t address) {
    address &= 0xFFFFFF;
    const std::uint32_t bank = address >> 16, off = address & 0xFFFF;
    if (bank == 0x7E) return wram[off];
    if (bank == 0x7F) return wram[0x10000 + off];
    const bool system = (bank & 0x40) == 0;   // banks $00-$3F and $80-$BF
    if (system && off < 0x2000) return wram[off];
    if (system && off < 0x8000) {
        if (off >= 0x2100 && off < 0x2200) return hardware_read(static_cast<std::uint16_t>(off));
        if (off == 0x4016 || off == 0x4017 || (off >= 0x4200 && off < 0x4220) || (off >= 0x4300 && off < 0x4380)) return hardware_read(static_cast<std::uint16_t>(off));
        return 0;
    }
    const std::optional<std::uint32_t> at = rom_.offset(address);
    return at ? rom_.data[*at] : 0;
}

void Machine::write(std::uint32_t address, std::uint8_t value) {
    address &= 0xFFFFFF;
    const std::uint32_t bank = address >> 16, off = address & 0xFFFF;
    if (bank == 0x7E) {
        wram[off] = value;
        return;
    }
    if (bank == 0x7F) {
        wram[0x10000 + off] = value;
        return;
    }
    const bool system = (bank & 0x40) == 0;
    if (system && off < 0x2000) {
        wram[off] = value;
    } else if (system && off < 0x8000) {
        if (off >= 0x2100 && off < 0x2200) hardware_write(static_cast<std::uint16_t>(off), value);
        else if (off == 0x4016 || off == 0x4017 || (off >= 0x4200 && off < 0x4220) || (off >= 0x4300 && off < 0x4380)) hardware_write(static_cast<std::uint16_t>(off), value);
    }
}

std::uint8_t Machine::hardware_read(std::uint16_t address) {
    std::uint8_t value = 0;
    switch (address) {
        case 0x213E:
        case 0x213F: value = 0x01; break;   // the PPU chips' versions
        case 0x2140: value = 0xAA; break;   // the sound CPU's ready signature
        case 0x2141: value = 0xBB; break;
        case 0x2180: value = wram[wram_address_ & 0x1FFFF]; wram_address_ = (wram_address_ + 1) & 0x1FFFF; break;
        case 0x4210: value = (nmi_reads_++ & 1) ? 0x02 : 0x82; break;
        case 0x4212: value = (status_reads_++ & 1) ? 0x00 : 0x80; break;
        case 0x4214: value = static_cast<std::uint8_t>(quotient_); break;
        case 0x4215: value = static_cast<std::uint8_t>(quotient_ >> 8); break;
        case 0x4216: value = static_cast<std::uint8_t>(remainder_ != 0 || product_ == 0 ? remainder_ : product_); break;
        case 0x4217: value = static_cast<std::uint8_t>((remainder_ != 0 || product_ == 0 ? remainder_ : product_) >> 8); break;
        default:
            if (address >= 0x4300 && address < 0x4380) value = dma_regs_[address - 0x4300];
            break;
    }
    if (log_.size() < kLogLimit) log_.push_back({false, address, value});
    return value;
}

void Machine::hardware_write(std::uint16_t address, std::uint8_t value) {
    if (log_.size() < kLogLimit) log_.push_back({true, address, value});
    switch (address) {
        case 0x2180: wram[wram_address_ & 0x1FFFF] = value; wram_address_ = (wram_address_ + 1) & 0x1FFFF; break;
        case 0x2181: wram_address_ = (wram_address_ & 0x1FF00) | value; break;
        case 0x2182: wram_address_ = (wram_address_ & 0x100FF) | static_cast<std::uint32_t>(value) << 8; break;
        case 0x2183: wram_address_ = (wram_address_ & 0x0FFFF) | static_cast<std::uint32_t>(value & 1) << 16; break;
        case 0x4202: multiplicand_ = value; break;
        case 0x4203:
            product_ = static_cast<std::uint16_t>(multiplicand_ * value);
            remainder_ = 0;
            break;
        case 0x4204: dividend_ = static_cast<std::uint16_t>((dividend_ & 0xFF00) | value); break;
        case 0x4205: dividend_ = static_cast<std::uint16_t>((dividend_ & 0x00FF) | value << 8); break;
        case 0x4206:
            quotient_ = value == 0 ? 0xFFFF : static_cast<std::uint16_t>(dividend_ / value);
            remainder_ = value == 0 ? dividend_ : static_cast<std::uint16_t>(dividend_ % value);
            if (remainder_ == 0) product_ = 0;
            break;
        case 0x420B: dma(value); break;
        default:
            if (address >= 0x4300 && address < 0x4380) dma_regs_[address - 0x4300] = value;
            break;
    }
}

// The channels named in `mask`, one after another: a transfer of DAS bytes (0 is 65536) between the A bus ($43x2-$43x4) and the B-bus register $21xx.
void Machine::dma(std::uint8_t mask) {
    static constexpr std::array<std::array<std::uint8_t, 4>, 8> kPattern = {{{0, 0, 0, 0}, {0, 1, 0, 1}, {0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 2, 3}, {0, 1, 0, 1}, {0, 0, 0, 0}, {0, 0, 1, 1}}};
    static constexpr std::array<std::uint8_t, 8> kLength = {1, 2, 2, 4, 4, 4, 2, 4};
    for (unsigned ch = 0; ch < 8; ++ch) {
        if (!(mask >> ch & 1)) continue;
        std::uint8_t* r = &dma_regs_[ch * 0x10];
        const std::uint8_t control = r[0], b_register = r[1];
        std::uint16_t a_address = static_cast<std::uint16_t>(r[2] | r[3] << 8);
        const std::uint32_t a_bank = static_cast<std::uint32_t>(r[4]) << 16;
        std::uint32_t count = static_cast<std::uint32_t>(r[5] | r[6] << 8);
        if (count == 0) count = 0x10000;
        const unsigned mode = control & 7;
        const bool to_a = (control & 0x80) != 0, fixed = (control & 0x08) != 0, down = (control & 0x10) != 0;
        for (std::uint32_t done = 0; done < count; ++done) {
            const std::uint16_t b = static_cast<std::uint16_t>(0x2100 + static_cast<std::uint8_t>(b_register + kPattern[mode][done % kLength[mode]]));
            if (to_a) write(a_bank | a_address, read(b));
            else write(b, read(a_bank | a_address));
            if (!fixed) a_address = static_cast<std::uint16_t>(a_address + (down ? -1 : 1));
        }
        r[2] = static_cast<std::uint8_t>(a_address);
        r[3] = static_cast<std::uint8_t>(a_address >> 8);
        r[5] = r[6] = 0;
    }
}

// ---- Cpu -----------------------------------------------------------------------------------------------------------

namespace {

class Cpu {
public:
    explicit Cpu(Machine& machine) : m_(machine) {}
    Registers regs;
    std::uint64_t steps = 0;

    // nullopt: it ran. A Fault: WAI or STP.
    std::optional<Fault> step();

private:
    bool m8() const { return regs.e || (regs.p & status::M); }
    bool x8() const { return regs.e || (regs.p & status::X); }
    std::uint16_t xr() const { return x8() ? static_cast<std::uint16_t>(regs.x & 0xFF) : regs.x; }
    std::uint16_t yr() const { return x8() ? static_cast<std::uint16_t>(regs.y & 0xFF) : regs.y; }
    void flag(std::uint8_t f, bool on) { regs.p = on ? static_cast<std::uint8_t>(regs.p | f) : static_cast<std::uint8_t>(regs.p & ~f); }
    bool has(std::uint8_t f) const { return (regs.p & f) != 0; }

    std::uint8_t rd8(std::uint32_t a) { return m_.read(a); }
    std::uint16_t rd16(std::uint32_t a) {   // the high byte is the next address, across a bank
        const std::uint16_t lo = rd8(a);
        return static_cast<std::uint16_t>(lo | rd8((a + 1) & 0xFFFFFF) << 8);
    }
    std::uint32_t rd24(std::uint32_t a) {
        const std::uint32_t lo = rd16(a);
        return lo | static_cast<std::uint32_t>(rd8((a + 2) & 0xFFFFFF)) << 16;
    }
    // direct page and the stack: a word at the end of bank 0 wraps within it
    std::uint16_t rd16_bank0(std::uint16_t a) {
        const std::uint16_t lo = rd8(a);
        return static_cast<std::uint16_t>(lo | rd8(static_cast<std::uint16_t>(a + 1)) << 8);
    }
    std::uint32_t rd24_bank0(std::uint16_t a) {
        const std::uint32_t lo = rd16_bank0(a);
        return lo | static_cast<std::uint32_t>(rd8(static_cast<std::uint16_t>(a + 2))) << 16;
    }
    void wr8(std::uint32_t a, std::uint8_t v) { m_.write(a, v); }
    void wr16(std::uint32_t a, std::uint16_t v) {
        wr8(a, static_cast<std::uint8_t>(v));
        wr8((a + 1) & 0xFFFFFF, static_cast<std::uint8_t>(v >> 8));
    }

    void push8(std::uint8_t v) {
        wr8(regs.s, v);
        regs.s = regs.e ? static_cast<std::uint16_t>(0x0100 | ((regs.s - 1) & 0xFF)) : static_cast<std::uint16_t>(regs.s - 1);
    }
    std::uint8_t pull8() {
        regs.s = regs.e ? static_cast<std::uint16_t>(0x0100 | ((regs.s + 1) & 0xFF)) : static_cast<std::uint16_t>(regs.s + 1);
        return rd8(regs.s);
    }
    void push16(std::uint16_t v) {
        push8(static_cast<std::uint8_t>(v >> 8));
        push8(static_cast<std::uint8_t>(v));
    }
    std::uint16_t pull16() {
        const std::uint16_t lo = pull8();
        return static_cast<std::uint16_t>(lo | pull8() << 8);
    }

    // N and Z from a value of the given width; returns the value.
    std::uint16_t nz(std::uint32_t v, bool w8) {
        const std::uint16_t r = static_cast<std::uint16_t>(w8 ? v & 0xFF : v & 0xFFFF);
        flag(status::Z, r == 0);
        flag(status::N, (r & (w8 ? 0x80 : 0x8000)) != 0);
        return r;
    }
    void set_a(std::uint16_t v, bool w8) { regs.a = w8 ? static_cast<std::uint16_t>((regs.a & 0xFF00) | (v & 0xFF)) : v; }
    std::uint16_t get_a(bool w8) const { return w8 ? static_cast<std::uint16_t>(regs.a & 0xFF) : regs.a; }

    void adc(std::uint16_t v, bool w8);
    void sbc(std::uint16_t v, bool w8);
    void compare(std::uint16_t reg, std::uint16_t v, bool w8) {
        flag(status::C, reg >= v);
        nz(static_cast<std::uint32_t>(reg) - v, w8);
    }
    std::uint16_t shift(Op op, std::uint16_t v, bool w8);
    void set_flags_after_mode_change() {
        if (x8()) {
            regs.x &= 0xFF;
            regs.y &= 0xFF;
        }
    }
    void interrupt(bool brk, std::uint16_t pc_after);

    Machine& m_;
};

void Cpu::adc(std::uint16_t v, bool w8) {
    const unsigned a = get_a(w8);
    const unsigned carry = regs.p & status::C;
    const unsigned sign = w8 ? 0x80 : 0x8000;
    unsigned r = 0;
    if (has(status::D)) {
        unsigned c = carry;
        for (unsigned i = 0; i < (w8 ? 2u : 4u); ++i) {
            unsigned s = ((a >> (4 * i)) & 0xF) + ((v >> (4 * i)) & 0xF) + c;
            c = s > 9;
            if (c) s += 6;
            r |= (s & 0xF) << (4 * i);
        }
        flag(status::C, c != 0);
    } else {
        const unsigned total = a + v + carry;
        r = total & (w8 ? 0xFF : 0xFFFF);
        flag(status::C, total > (w8 ? 0xFFu : 0xFFFFu));
    }
    flag(status::V, (~(a ^ v) & (a ^ r) & sign) != 0);
    set_a(nz(r, w8), w8);
}

void Cpu::sbc(std::uint16_t v, bool w8) {
    const unsigned a = get_a(w8);
    const unsigned borrow = 1 - (regs.p & status::C);
    const unsigned sign = w8 ? 0x80 : 0x8000;
    unsigned r = 0;
    if (has(status::D)) {
        int c = static_cast<int>(1 - borrow);
        for (unsigned i = 0; i < (w8 ? 2u : 4u); ++i) {
            int s = static_cast<int>((a >> (4 * i)) & 0xF) - static_cast<int>((v >> (4 * i)) & 0xF) - (1 - c);
            c = s >= 0;
            if (!c) s -= 6;
            r |= (static_cast<unsigned>(s) & 0xF) << (4 * i);
        }
        flag(status::C, c != 0);
    } else {
        const int total = static_cast<int>(a) - static_cast<int>(v) - static_cast<int>(borrow);
        r = static_cast<unsigned>(total) & (w8 ? 0xFFu : 0xFFFFu);
        flag(status::C, total >= 0);
    }
    flag(status::V, ((a ^ v) & (a ^ r) & sign) != 0);
    set_a(nz(r, w8), w8);
}

std::uint16_t Cpu::shift(Op op, std::uint16_t v, bool w8) {
    const unsigned sign = w8 ? 0x80 : 0x8000;
    const unsigned carry = regs.p & status::C;
    unsigned out = 0, r = 0;
    switch (op) {
        case Op::Asl: out = v & sign; r = static_cast<unsigned>(v) << 1; break;
        case Op::Lsr: out = v & 1; r = v >> 1; break;
        case Op::Rol: out = v & sign; r = (static_cast<unsigned>(v) << 1) | carry; break;
        default: out = v & 1; r = (v >> 1) | (carry ? sign : 0); break;
    }
    flag(status::C, out != 0);
    return nz(r, w8);
}

// BRK and COP: push the return address (and the program bank in native mode) and the status, and go through the vector.
void Cpu::interrupt(bool brk, std::uint16_t pc_after) {
    if (!regs.e) push8(regs.pbr);
    push16(pc_after);
    push8(regs.e ? static_cast<std::uint8_t>(regs.p | status::M | status::X) : regs.p);   // (in emulation mode bit 4 is the break flag)
    flag(status::I, true);
    flag(status::D, false);
    regs.pbr = 0;
    const std::uint32_t vector = regs.e ? (brk ? 0xFFFE : 0xFFF4) : (brk ? 0xFFE6 : 0xFFE4);
    regs.pc = rd16(vector);
}

std::optional<Fault> Cpu::step() {
    const std::uint32_t pc24 = static_cast<std::uint32_t>(regs.pbr) << 16 | regs.pc;
    const std::uint8_t opcode = rd8(pc24);
    const Entry& e = table()[opcode];
    const bool w8 = m8(), i8 = x8();
    const std::uint8_t size = dec::instruction_size(e.mode, !w8, !i8);
    ++steps;
    // the operand bytes as the instruction holds them (a Block's are the destination and the source bank)
    std::uint32_t operand = 0;
    for (unsigned i = size; i-- > 1;) operand = operand << 8 | rd8((pc24 & 0xFF0000) | ((regs.pc + i) & 0xFFFF));
    std::uint16_t next = static_cast<std::uint16_t>(regs.pc + size);
    const std::uint32_t bank = static_cast<std::uint32_t>(regs.dbr) << 16;
    const auto dp = [&](std::uint32_t off) { return static_cast<std::uint16_t>(regs.d + off); };

    // the effective address of a memory operand
    const auto ea = [&]() -> std::uint32_t {
        switch (e.mode) {
            case Mode::Dp: return dp(operand);
            case Mode::DpX: return dp(operand + xr());
            case Mode::DpY: return dp(operand + yr());
            case Mode::DpInd: return bank | rd16_bank0(dp(operand));
            case Mode::DpIndX: return bank | rd16_bank0(dp(operand + xr()));
            case Mode::DpIndY: return (bank | rd16_bank0(dp(operand))) + yr();
            case Mode::DpIndL: return rd24_bank0(dp(operand));
            case Mode::DpIndLY: return rd24_bank0(dp(operand)) + yr();
            case Mode::Abs: return bank | operand;
            case Mode::AbsX: return (bank | operand) + xr();
            case Mode::AbsY: return (bank | operand) + yr();
            case Mode::Long: return operand;
            case Mode::LongX: return operand + xr();
            case Mode::Sr: return static_cast<std::uint16_t>(regs.s + operand);
            case Mode::SrIndY: return (bank | rd16_bank0(static_cast<std::uint16_t>(regs.s + operand))) + yr();
            default: break;
        }
        CB_ASSERT(false, "an addressing mode with no effective address");
        return 0;
    };
    const auto mask24 = [](std::uint32_t a) { return a & 0xFFFFFF; };
    // a value of the accumulator's (m) or the index registers' (x) width: the immediate, or memory
    const auto load = [&](bool eight, Mode imm) -> std::uint16_t {
        if (e.mode == imm) return static_cast<std::uint16_t>(operand);
        const std::uint32_t a = mask24(ea());
        return eight ? static_cast<std::uint16_t>(rd8(a)) : rd16(a);
    };
    const auto store = [&](std::uint16_t v, bool eight) {
        const std::uint32_t a = mask24(ea());
        if (eight) wr8(a, static_cast<std::uint8_t>(v));
        else wr16(a, v);
    };
    const auto branch = [&](bool take) {
        if (take) next = static_cast<std::uint16_t>(next + static_cast<std::int8_t>(operand));
    };
    const auto set_x = [&](std::uint16_t v) { regs.x = nz(v, i8); };
    const auto set_y = [&](std::uint16_t v) { regs.y = nz(v, i8); };

    switch (e.op) {
        case Op::Lda: set_a(nz(load(w8, Mode::ImmM), w8), w8); break;
        case Op::Ldx: set_x(load(i8, Mode::ImmX)); break;
        case Op::Ldy: set_y(load(i8, Mode::ImmX)); break;
        case Op::Sta: store(get_a(w8), w8); break;
        case Op::Stx: store(xr(), i8); break;
        case Op::Sty: store(yr(), i8); break;
        case Op::Stz: store(0, w8); break;
        case Op::Adc: adc(load(w8, Mode::ImmM), w8); break;
        case Op::Sbc: sbc(load(w8, Mode::ImmM), w8); break;
        case Op::And: set_a(nz(get_a(w8) & load(w8, Mode::ImmM), w8), w8); break;
        case Op::Ora: set_a(nz(get_a(w8) | load(w8, Mode::ImmM), w8), w8); break;
        case Op::Eor: set_a(nz(get_a(w8) ^ load(w8, Mode::ImmM), w8), w8); break;
        case Op::Cmp: compare(get_a(w8), load(w8, Mode::ImmM), w8); break;
        case Op::Cpx: compare(xr(), load(i8, Mode::ImmX), i8); break;
        case Op::Cpy: compare(yr(), load(i8, Mode::ImmX), i8); break;
        case Op::Bit: {
            const std::uint16_t v = load(w8, Mode::ImmM);
            flag(status::Z, (get_a(w8) & v) == 0);
            if (e.mode != Mode::ImmM) {
                flag(status::N, (v & (w8 ? 0x80 : 0x8000)) != 0);
                flag(status::V, (v & (w8 ? 0x40 : 0x4000)) != 0);
            }
            break;
        }
        case Op::Asl:
        case Op::Lsr:
        case Op::Rol:
        case Op::Ror:
            if (e.mode == Mode::Acc) {
                set_a(shift(e.op, get_a(w8), w8), w8);
            } else {
                const std::uint32_t a = mask24(ea());
                const std::uint16_t v = w8 ? rd8(a) : rd16(a);
                const std::uint16_t r = shift(e.op, v, w8);
                if (w8) wr8(a, static_cast<std::uint8_t>(r));
                else wr16(a, r);
            }
            break;
        case Op::Inc:
        case Op::Dec: {
            const int delta = e.op == Op::Inc ? 1 : -1;
            if (e.mode == Mode::Acc) {
                set_a(nz(static_cast<std::uint32_t>(static_cast<int>(get_a(w8)) + delta), w8), w8);
            } else {
                const std::uint32_t a = mask24(ea());
                const std::uint16_t v = w8 ? rd8(a) : rd16(a);
                const std::uint16_t r = nz(static_cast<std::uint32_t>(static_cast<int>(v) + delta), w8);
                if (w8) wr8(a, static_cast<std::uint8_t>(r));
                else wr16(a, r);
            }
            break;
        }
        case Op::Tsb:
        case Op::Trb: {
            const std::uint32_t a = mask24(ea());
            const std::uint16_t v = w8 ? rd8(a) : rd16(a);
            flag(status::Z, (get_a(w8) & v) == 0);
            const std::uint16_t r = e.op == Op::Tsb ? static_cast<std::uint16_t>(v | get_a(w8)) : static_cast<std::uint16_t>(v & ~get_a(w8));
            if (w8) wr8(a, static_cast<std::uint8_t>(r));
            else wr16(a, r);
            break;
        }
        case Op::Inx: set_x(static_cast<std::uint16_t>(xr() + 1)); break;
        case Op::Iny: set_y(static_cast<std::uint16_t>(yr() + 1)); break;
        case Op::Dex: set_x(static_cast<std::uint16_t>(xr() - 1)); break;
        case Op::Dey: set_y(static_cast<std::uint16_t>(yr() - 1)); break;
        case Op::Tax: set_x(i8 ? static_cast<std::uint16_t>(regs.a & 0xFF) : regs.a); break;
        case Op::Tay: set_y(i8 ? static_cast<std::uint16_t>(regs.a & 0xFF) : regs.a); break;
        case Op::Txa: set_a(nz(xr(), w8), w8); break;
        case Op::Tya: set_a(nz(yr(), w8), w8); break;
        case Op::Txy: set_y(xr()); break;
        case Op::Tyx: set_x(yr()); break;
        case Op::Tsx: set_x(i8 ? static_cast<std::uint16_t>(regs.s & 0xFF) : regs.s); break;
        case Op::Txs: regs.s = regs.e ? static_cast<std::uint16_t>(0x0100 | (regs.x & 0xFF)) : regs.x; break;
        case Op::Tcs: regs.s = regs.e ? static_cast<std::uint16_t>(0x0100 | (regs.a & 0xFF)) : regs.a; break;
        case Op::Tsc: regs.a = nz(regs.s, false); break;
        case Op::Tcd: regs.d = nz(regs.a, false); break;
        case Op::Tdc: regs.a = nz(regs.d, false); break;
        case Op::Xba:
            regs.a = static_cast<std::uint16_t>(regs.a << 8 | regs.a >> 8);
            nz(regs.a & 0xFF, true);
            break;
        case Op::Pha: if (w8) push8(static_cast<std::uint8_t>(regs.a)); else push16(regs.a); break;
        case Op::Phx: if (i8) push8(static_cast<std::uint8_t>(regs.x)); else push16(regs.x); break;
        case Op::Phy: if (i8) push8(static_cast<std::uint8_t>(regs.y)); else push16(regs.y); break;
        case Op::Pla: if (w8) set_a(nz(pull8(), true), true); else regs.a = nz(pull16(), false); break;
        case Op::Plx: if (i8) regs.x = nz(pull8(), true); else regs.x = nz(pull16(), false); break;
        case Op::Ply: if (i8) regs.y = nz(pull8(), true); else regs.y = nz(pull16(), false); break;
        case Op::Php: push8(regs.e ? static_cast<std::uint8_t>(regs.p | status::M | status::X) : regs.p); break;
        case Op::Plp:
            regs.p = pull8();
            if (regs.e) regs.p |= status::M | status::X;
            set_flags_after_mode_change();
            break;
        case Op::Phb: push8(regs.dbr); break;
        case Op::Plb: regs.dbr = static_cast<std::uint8_t>(nz(pull8(), true)); break;
        case Op::Phd: push16(regs.d); break;
        case Op::Pld: regs.d = nz(pull16(), false); break;
        case Op::Phk: push8(regs.pbr); break;
        case Op::Pea: push16(static_cast<std::uint16_t>(operand)); break;
        case Op::Pei: push16(rd16_bank0(dp(operand))); break;
        case Op::Per: push16(static_cast<std::uint16_t>(next + static_cast<std::int16_t>(operand))); break;
        case Op::Clc: flag(status::C, false); break;
        case Op::Sec: flag(status::C, true); break;
        case Op::Cli: flag(status::I, false); break;
        case Op::Sei: flag(status::I, true); break;
        case Op::Cld: flag(status::D, false); break;
        case Op::Sed: flag(status::D, true); break;
        case Op::Clv: flag(status::V, false); break;
        case Op::Rep:
            regs.p = static_cast<std::uint8_t>(regs.p & ~operand);
            if (regs.e) regs.p |= status::M | status::X;
            break;
        case Op::Sep:
            regs.p = static_cast<std::uint8_t>(regs.p | operand);
            set_flags_after_mode_change();
            break;
        case Op::Xce: {
            const bool carry = has(status::C);
            flag(status::C, regs.e);
            regs.e = carry;
            if (regs.e) {
                regs.p |= status::M | status::X;
                regs.s = static_cast<std::uint16_t>(0x0100 | (regs.s & 0xFF));
                regs.x &= 0xFF;
                regs.y &= 0xFF;
            }
            break;
        }
        case Op::Bcc: branch(!has(status::C)); break;
        case Op::Bcs: branch(has(status::C)); break;
        case Op::Beq: branch(has(status::Z)); break;
        case Op::Bne: branch(!has(status::Z)); break;
        case Op::Bmi: branch(has(status::N)); break;
        case Op::Bpl: branch(!has(status::N)); break;
        case Op::Bvc: branch(!has(status::V)); break;
        case Op::Bvs: branch(has(status::V)); break;
        case Op::Bra: branch(true); break;
        case Op::Brl: next = static_cast<std::uint16_t>(next + static_cast<std::int16_t>(operand)); break;
        case Op::Jmp:
            if (e.mode == Mode::Abs) next = static_cast<std::uint16_t>(operand);
            else if (e.mode == Mode::AbsInd) next = rd16_bank0(static_cast<std::uint16_t>(operand));
            else next = rd16((pc24 & 0xFF0000) | ((operand + xr()) & 0xFFFF));
            break;
        case Op::Jml:
            if (e.mode == Mode::Long) {
                regs.pbr = static_cast<std::uint8_t>(operand >> 16);
                next = static_cast<std::uint16_t>(operand);
            } else {
                const std::uint32_t to = rd24_bank0(static_cast<std::uint16_t>(operand));
                regs.pbr = static_cast<std::uint8_t>(to >> 16);
                next = static_cast<std::uint16_t>(to);
            }
            break;
        case Op::Jsr:
            push16(static_cast<std::uint16_t>(regs.pc + 2));
            if (e.mode == Mode::Abs) next = static_cast<std::uint16_t>(operand);
            else next = rd16((pc24 & 0xFF0000) | ((operand + xr()) & 0xFFFF));
            break;
        case Op::Jsl:
            push8(regs.pbr);
            push16(static_cast<std::uint16_t>(regs.pc + 3));
            regs.pbr = static_cast<std::uint8_t>(operand >> 16);
            next = static_cast<std::uint16_t>(operand);
            break;
        case Op::Rts: next = static_cast<std::uint16_t>(pull16() + 1); break;
        case Op::Rtl:
            next = static_cast<std::uint16_t>(pull16() + 1);
            regs.pbr = pull8();
            break;
        case Op::Rti:
            regs.p = pull8();
            next = pull16();
            if (regs.e) regs.p |= status::M | status::X;
            else regs.pbr = pull8();
            set_flags_after_mode_change();
            break;
        case Op::Brk: interrupt(true, static_cast<std::uint16_t>(regs.pc + 2)); next = regs.pc; break;
        case Op::Cop: interrupt(false, static_cast<std::uint16_t>(regs.pc + 2)); next = regs.pc; break;
        case Op::Mvn:
        case Op::Mvp: {
            const std::uint32_t destination = (operand & 0xFF) << 16, source = (operand >> 8 & 0xFF) << 16;
            regs.dbr = static_cast<std::uint8_t>(operand & 0xFF);
            const std::int32_t step = e.op == Op::Mvn ? 1 : -1;
            for (unsigned moved = 0; moved < 0x10000; ++moved) {
                wr8(destination | yr(), rd8(source | xr()));
                regs.x = i8 ? static_cast<std::uint16_t>((regs.x + step) & 0xFF) : static_cast<std::uint16_t>(regs.x + step);
                regs.y = i8 ? static_cast<std::uint16_t>((regs.y + step) & 0xFF) : static_cast<std::uint16_t>(regs.y + step);
                const bool last = regs.a == 0;
                regs.a = static_cast<std::uint16_t>(regs.a - 1);
                if (last) break;
            }
            break;
        }
        case Op::Nop:
        case Op::Wdm: break;
        case Op::Wai: return Fault{"WAI", pc24};
        case Op::Stp: return Fault{"STP", pc24};
    }
    regs.pc = next;
    return std::nullopt;
}

}  // namespace

// ---- running a function --------------------------------------------------------------------------------------------

std::string Fault::text() const { return std::format("{} at {:06x}", what, pc); }

std::string Run::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string());
        case End::Limit: break;
    }
    return "limit";
}

Run run(Machine& machine, std::uint32_t entry, const Options& options) {
    const std::size_t log_start = machine.log().size();
    Cpu cpu(machine);
    Registers& r = cpu.regs;
    r.e = !options.native;
    r.p = status::I;
    if (r.e || !options.m16) r.p |= status::M;
    if (r.e || !options.x16) r.p |= status::X;
    r.a = options.a;
    r.x = (r.p & status::X) ? static_cast<std::uint16_t>(options.x & 0xFF) : options.x;
    r.y = (r.p & status::X) ? static_cast<std::uint16_t>(options.y & 0xFF) : options.y;
    r.s = 0x01FF;
    r.d = 0;
    r.pbr = r.dbr = static_cast<std::uint8_t>(entry >> 16);
    r.pc = static_cast<std::uint16_t>(entry);
    // the frame a JSL leaves, so that RTS and RTL both come back to kReturnAddress
    const std::uint16_t ret = kReturnAddress - 1;
    machine.write(r.s, r.pbr);
    --r.s;
    machine.write(r.s, static_cast<std::uint8_t>(ret >> 8));
    --r.s;
    machine.write(r.s, static_cast<std::uint8_t>(ret));
    --r.s;
    Run out;
    while (cpu.steps < options.max_steps) {
        if (r.pc == kReturnAddress) {
            out.end = End::Return;
            break;
        }
        if (const std::optional<Fault> fault = cpu.step()) {
            out.end = End::Stopped;
            out.fault = fault;
            break;
        }
    }
    out.regs = r;
    out.steps = cpu.steps;
    out.io.assign(machine.log().begin() + static_cast<std::ptrdiff_t>(log_start), machine.log().end());
    return out;
}

}  // namespace cb::emulate::w65816
