// cb/emulate/m6502.cpp -- see m6502.hpp.
#include "cb/emulate/m6502.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <string_view>

#include "cb/core/error.hpp"
#include "cb/decompile/m6502.hpp"

namespace cb::emulate::m6502 {

namespace dec = cb::decompile::m6502;

namespace {

// The mnemonics, as the interpreter's switch knows them.
enum class Op : std::uint8_t {
    Lda, Ldx, Ldy, Sta, Stx, Sty, Adc, Sbc, And, Ora, Eor, Cmp, Cpx, Cpy, Bit, Asl, Lsr, Rol, Ror, Inc, Dec,
    Inx, Dex, Iny, Dey, Tax, Tay, Txa, Tya, Tsx, Txs, Clc, Sec, Cli, Sei, Cld, Sed, Clv, Pha, Php, Pla, Plp,
    Bcc, Bcs, Beq, Bmi, Bne, Bpl, Bvc, Bvs, Jmp, Jsr, Rts, Rti, Brk, Nop,
};

struct Named {
    std::string_view name;
    Op op;
};
constexpr Named kNames[] = {
    {"LDA", Op::Lda}, {"LDX", Op::Ldx}, {"LDY", Op::Ldy}, {"STA", Op::Sta}, {"STX", Op::Stx}, {"STY", Op::Sty},
    {"ADC", Op::Adc}, {"SBC", Op::Sbc}, {"AND", Op::And}, {"ORA", Op::Ora}, {"EOR", Op::Eor}, {"CMP", Op::Cmp},
    {"CPX", Op::Cpx}, {"CPY", Op::Cpy}, {"BIT", Op::Bit}, {"ASL", Op::Asl}, {"LSR", Op::Lsr}, {"ROL", Op::Rol},
    {"ROR", Op::Ror}, {"INC", Op::Inc}, {"DEC", Op::Dec}, {"INX", Op::Inx}, {"DEX", Op::Dex}, {"INY", Op::Iny},
    {"DEY", Op::Dey}, {"TAX", Op::Tax}, {"TAY", Op::Tay}, {"TXA", Op::Txa}, {"TYA", Op::Tya}, {"TSX", Op::Tsx},
    {"TXS", Op::Txs}, {"CLC", Op::Clc}, {"SEC", Op::Sec}, {"CLI", Op::Cli}, {"SEI", Op::Sei}, {"CLD", Op::Cld},
    {"SED", Op::Sed}, {"CLV", Op::Clv}, {"PHA", Op::Pha}, {"PHP", Op::Php}, {"PLA", Op::Pla}, {"PLP", Op::Plp},
    {"BCC", Op::Bcc}, {"BCS", Op::Bcs}, {"BEQ", Op::Beq}, {"BMI", Op::Bmi}, {"BNE", Op::Bne}, {"BPL", Op::Bpl},
    {"BVC", Op::Bvc}, {"BVS", Op::Bvs}, {"JMP", Op::Jmp}, {"JSR", Op::Jsr}, {"RTS", Op::Rts}, {"RTI", Op::Rti},
    {"BRK", Op::Brk}, {"NOP", Op::Nop},
};

struct Entry {
    bool documented = false;
    Op op = Op::Nop;
    dec::Mode mode = dec::Mode::Imp;
    std::uint8_t size = 1;
};

// The 256 opcodes, resolved once from the decoder's table (the one description the disassembler and the assembler
// read too).
const std::array<Entry, 256>& table() {
    static const std::array<Entry, 256> t = [] {
        std::array<Entry, 256> out{};
        const auto& source = dec::opcode_table();
        for (std::size_t code = 0; code < 256; ++code) {
            if (!source[code]) continue;
            const auto it = std::find_if(std::begin(kNames), std::end(kNames),
                                         [&](const Named& n) { return n.name == source[code]->mnemonic; });
            CB_ASSERT(it != std::end(kNames), "an opcode of the table the interpreter does not know");
            out[code] = {true, it->op, source[code]->mode, dec::instruction_size(source[code]->mode)};
        }
        return out;
    }();
    return t;
}

}  // namespace

// ---- Bus ---------------------------------------------------------------------------------------------------------

Bus::Bus() : owned_(kMemorySize, 0), ram_(owned_) {}

Bus::Bus(ByteView image) : owned_(kMemorySize, 0), ram_(owned_) {
    if (image.size() > kMemorySize) throw UsageError(std::format("a 6502 bus holds 64 KB, not {} bytes", image.size()));
    std::copy(image.begin(), image.end(), owned_.begin());
}

Bus Bus::over(std::span<std::uint8_t> ram) {
    if (ram.size() != kMemorySize) throw UsageError(std::format("a 6502 bus is 64 KB, not {} bytes", ram.size()));
    Bus bus;
    bus.owned_.clear();
    bus.owned_.shrink_to_fit();
    bus.ram_ = ram;
    return bus;
}

void Bus::map(std::uint16_t lo, std::uint16_t hi, ReadFn read, WriteFn write) {
    ranges_.push_back({lo, hi, std::move(read), std::move(write)});
}

std::uint8_t Bus::read(std::uint16_t address) {
    for (const Range& r : ranges_) {
        if (r.lo <= address && address <= r.hi) {
            const std::uint8_t v = r.read ? r.read(address) : 0;
            log_.push_back({DeviceAccess::Kind::Read, address, v});
            return v;
        }
    }
    return ram_[address];
}

void Bus::write(std::uint16_t address, std::uint8_t value) {
    for (const Range& r : ranges_) {
        if (r.lo <= address && address <= r.hi) {
            log_.push_back({DeviceAccess::Kind::Write, address, value});
            if (r.write) r.write(address, value);
            return;
        }
    }
    ram_[address] = value;
}

// ---- Cpu ---------------------------------------------------------------------------------------------------------

std::string Fault::text() const { return std::format("undocumented opcode {:02X} at {:04X}", opcode, pc); }

std::uint16_t Cpu::rd16(std::uint16_t a) {
    const std::uint16_t lo = rd(a);   // (the low byte is fetched first: a device range sees the accesses in this order)
    const std::uint16_t hi = rd(static_cast<std::uint16_t>(a + 1));
    return static_cast<std::uint16_t>(lo | hi << 8);
}

std::uint16_t Cpu::rd16_zp(std::uint8_t a) {
    const std::uint16_t lo = rd(a);
    const std::uint16_t hi = rd(static_cast<std::uint8_t>(a + 1));
    return static_cast<std::uint16_t>(lo | hi << 8);
}

void Cpu::push(std::uint8_t v) {
    wr(static_cast<std::uint16_t>(0x100 | regs.s), v);
    regs.s = static_cast<std::uint8_t>(regs.s - 1);
}

std::uint8_t Cpu::pull() {
    regs.s = static_cast<std::uint8_t>(regs.s + 1);
    return rd(static_cast<std::uint16_t>(0x100 | regs.s));
}

void Cpu::set_flag(std::uint8_t f, bool on) {
    regs.p = on ? static_cast<std::uint8_t>(regs.p | f) : static_cast<std::uint8_t>(regs.p & ~f);
}

// N and Z from the low byte of `v`; returns that byte.
std::uint8_t Cpu::set_nz(unsigned v) {
    const std::uint8_t b = static_cast<std::uint8_t>(v);
    set_flag(status::Z, b == 0);
    set_flag(status::N, (b & 0x80) != 0);
    return b;
}

void Cpu::adc(std::uint8_t v) {
    const int a = regs.a;
    const int c = regs.p & status::C;
    if (decimal_ && (regs.p & status::D)) {
        int lo = (a & 0x0F) + (v & 0x0F) + c;
        int hi = (a >> 4) + (v >> 4) + (lo > 9 ? 1 : 0);
        const int binary = a + v + c;
        set_flag(status::Z, (binary & 0xFF) == 0);
        set_flag(status::N, (hi & 0x08) != 0);
        set_flag(status::V, (~(a ^ v) & (a ^ (hi << 4)) & 0x80) != 0);
        if (lo > 9) lo += 6;
        if (hi > 9) hi += 6;
        set_flag(status::C, hi > 15);
        regs.a = static_cast<std::uint8_t>((hi << 4) | (lo & 0x0F));
        return;
    }
    const int r = a + v + c;
    set_flag(status::C, r > 0xFF);
    set_flag(status::V, (~(a ^ v) & (a ^ r) & 0x80) != 0);
    regs.a = set_nz(static_cast<unsigned>(r));
}

void Cpu::sbc(std::uint8_t v) {
    if (decimal_ && (regs.p & status::D)) {
        const int a = regs.a;
        const int borrow = 1 - (regs.p & status::C);
        const int r = a - v - borrow;
        int lo = (a & 0x0F) - (v & 0x0F) - borrow;
        int hi = (a >> 4) - (v >> 4) - (lo < 0 ? 1 : 0);
        set_flag(status::C, r >= 0);
        set_flag(status::V, ((a ^ v) & (a ^ r) & 0x80) != 0);
        set_nz(static_cast<unsigned>(r));
        if (lo < 0) lo -= 6;
        if (hi < 0) hi -= 6;
        regs.a = static_cast<std::uint8_t>(hi * 16 | (lo & 0x0F));
        return;
    }
    adc(static_cast<std::uint8_t>(v ^ 0xFF));   // the carry is "no borrow": a - v - b is a + ~v + c
}

void Cpu::compare(std::uint8_t reg, std::uint8_t v) {
    set_flag(status::C, reg >= v);
    set_nz(static_cast<unsigned>(reg) - v);
}

Cpu::Flags Cpu::flags() const {
    const std::uint8_t p = regs.p;
    return {static_cast<std::uint8_t>(p >> 7 & 1), static_cast<std::uint8_t>(p >> 6 & 1), static_cast<std::uint8_t>(p >> 4 & 1),
            static_cast<std::uint8_t>(p >> 3 & 1), static_cast<std::uint8_t>(p >> 2 & 1), static_cast<std::uint8_t>(p >> 1 & 1),
            static_cast<std::uint8_t>(p & 1)};
}

std::optional<Fault> Cpu::step() {
    using Mode = dec::Mode;
    const std::uint16_t pc = regs.pc;
    const std::uint8_t opcode = rd(pc);
    const Entry& e = table()[opcode];
    if (!e.documented) return Fault{opcode, pc};
    std::uint16_t next = static_cast<std::uint16_t>(pc + e.size);
    ++steps;

    // The effective address of the operand (reads the operand bytes through the bus, in the order the chip does).
    auto address = [&](Mode mode) -> std::uint16_t {
        const std::uint16_t operand = static_cast<std::uint16_t>(pc + 1);
        switch (mode) {
            case Mode::Zp: return rd(operand);
            case Mode::Zpx: return static_cast<std::uint8_t>(rd(operand) + regs.x);
            case Mode::Zpy: return static_cast<std::uint8_t>(rd(operand) + regs.y);
            case Mode::Abs: return rd16(operand);
            case Mode::Abx: return static_cast<std::uint16_t>(rd16(operand) + regs.x);
            case Mode::Aby: return static_cast<std::uint16_t>(rd16(operand) + regs.y);
            case Mode::Izx: return rd16_zp(static_cast<std::uint8_t>(rd(operand) + regs.x));
            case Mode::Izy: return static_cast<std::uint16_t>(rd16_zp(rd(operand)) + regs.y);
            case Mode::Ind: {   // the page-wrap of the original chip
                const std::uint16_t p = rd16(operand);
                const std::uint16_t lo = rd(p);
                const std::uint16_t hi = rd(static_cast<std::uint16_t>((p & 0xFF00) | ((p + 1) & 0xFF)));
                return static_cast<std::uint16_t>(lo | hi << 8);
            }
            default: break;
        }
        CB_ASSERT(false, "an addressing mode with no effective address");
        return 0;
    };
    auto load = [&](Mode mode) -> std::uint8_t {
        if (mode == Mode::Imm) return rd(static_cast<std::uint16_t>(pc + 1));
        if (mode == Mode::Acc) return regs.a;
        return rd(address(mode));
    };

    switch (e.op) {
        case Op::Lda: regs.a = set_nz(load(e.mode)); break;
        case Op::Ldx: regs.x = set_nz(load(e.mode)); break;
        case Op::Ldy: regs.y = set_nz(load(e.mode)); break;
        case Op::Sta: wr(address(e.mode), regs.a); break;
        case Op::Stx: wr(address(e.mode), regs.x); break;
        case Op::Sty: wr(address(e.mode), regs.y); break;
        case Op::Adc: adc(load(e.mode)); break;
        case Op::Sbc: sbc(load(e.mode)); break;
        case Op::And: regs.a = set_nz(regs.a & load(e.mode)); break;
        case Op::Ora: regs.a = set_nz(regs.a | load(e.mode)); break;
        case Op::Eor: regs.a = set_nz(regs.a ^ load(e.mode)); break;
        case Op::Cmp: compare(regs.a, load(e.mode)); break;
        case Op::Cpx: compare(regs.x, load(e.mode)); break;
        case Op::Cpy: compare(regs.y, load(e.mode)); break;
        case Op::Bit: {
            const std::uint8_t v = load(e.mode);
            set_flag(status::Z, (regs.a & v) == 0);
            set_flag(status::N, (v & 0x80) != 0);
            set_flag(status::V, (v & 0x40) != 0);
            break;
        }
        case Op::Asl:
        case Op::Lsr:
        case Op::Rol:
        case Op::Ror: {
            const bool acc = e.mode == Mode::Acc;
            std::uint16_t addr = 0;
            if (!acc) addr = address(e.mode);
            const unsigned v = acc ? regs.a : rd(addr);
            const unsigned carry = regs.p & status::C;
            unsigned out = 0, r = 0;
            switch (e.op) {
                case Op::Asl: out = v & 0x80; r = v << 1; break;
                case Op::Lsr: out = v & 1; r = v >> 1; break;
                case Op::Rol: out = v & 0x80; r = (v << 1) | carry; break;
                default: out = v & 1; r = (v >> 1) | (carry << 7); break;
            }
            set_flag(status::C, out != 0);
            const std::uint8_t result = set_nz(r);
            if (acc) {
                regs.a = result;
            } else {
                wr(addr, result);
            }
            break;
        }
        case Op::Inc:
        case Op::Dec: {
            const std::uint16_t addr = address(e.mode);
            const unsigned v = rd(addr);
            wr(addr, set_nz(e.op == Op::Inc ? v + 1 : v - 1));
            break;
        }
        case Op::Inx: regs.x = set_nz(regs.x + 1u); break;
        case Op::Dex: regs.x = set_nz(regs.x - 1u); break;
        case Op::Iny: regs.y = set_nz(regs.y + 1u); break;
        case Op::Dey: regs.y = set_nz(regs.y - 1u); break;
        case Op::Tax: regs.x = set_nz(regs.a); break;
        case Op::Tay: regs.y = set_nz(regs.a); break;
        case Op::Txa: regs.a = set_nz(regs.x); break;
        case Op::Tya: regs.a = set_nz(regs.y); break;
        case Op::Tsx: regs.x = set_nz(regs.s); break;
        case Op::Txs: regs.s = regs.x; break;   // (no flags)
        case Op::Clc: set_flag(status::C, false); break;
        case Op::Sec: set_flag(status::C, true); break;
        case Op::Cli: set_flag(status::I, false); break;
        case Op::Sei: set_flag(status::I, true); break;
        case Op::Cld: set_flag(status::D, false); break;
        case Op::Sed: set_flag(status::D, true); break;
        case Op::Clv: set_flag(status::V, false); break;
        case Op::Pha: push(regs.a); break;
        case Op::Php: push(regs.p | status::B | status::U); break;
        case Op::Pla: regs.a = set_nz(pull()); break;
        case Op::Plp: regs.p = static_cast<std::uint8_t>((pull() & ~status::B) | status::U); break;
        case Op::Bcc:
        case Op::Bcs:
        case Op::Beq:
        case Op::Bmi:
        case Op::Bne:
        case Op::Bpl:
        case Op::Bvc:
        case Op::Bvs: {
            std::uint8_t bit = status::C;
            bool want = false;
            switch (e.op) {
                case Op::Bcc: bit = status::C; want = false; break;
                case Op::Bcs: bit = status::C; want = true; break;
                case Op::Beq: bit = status::Z; want = true; break;
                case Op::Bne: bit = status::Z; want = false; break;
                case Op::Bmi: bit = status::N; want = true; break;
                case Op::Bpl: bit = status::N; want = false; break;
                case Op::Bvc: bit = status::V; want = false; break;
                default: bit = status::V; want = true; break;
            }
            if (((regs.p & bit) != 0) == want) {
                const std::int8_t d = static_cast<std::int8_t>(rd(static_cast<std::uint16_t>(pc + 1)));
                next = static_cast<std::uint16_t>(next + d);
            }
            break;
        }
        case Op::Jmp: next = address(e.mode); break;
        case Op::Jsr: {
            const std::uint16_t ret = static_cast<std::uint16_t>(pc + 2);   // the address of its own last byte
            push(static_cast<std::uint8_t>(ret >> 8));
            push(static_cast<std::uint8_t>(ret));
            next = address(Mode::Abs);
            break;
        }
        case Op::Rts: {
            const std::uint16_t lo = pull();
            const std::uint16_t hi = pull();
            next = static_cast<std::uint16_t>((hi << 8 | lo) + 1);
            break;
        }
        case Op::Rti: {
            regs.p = static_cast<std::uint8_t>((pull() & ~status::B) | status::U);
            const std::uint16_t lo = pull();
            const std::uint16_t hi = pull();
            next = static_cast<std::uint16_t>(hi << 8 | lo);
            break;
        }
        case Op::Brk: {
            const std::uint16_t ret = static_cast<std::uint16_t>(pc + 2);
            push(static_cast<std::uint8_t>(ret >> 8));
            push(static_cast<std::uint8_t>(ret));
            push(regs.p | status::B | status::U);
            set_flag(status::I, true);
            next = rd16(0xFFFE);
            break;
        }
        case Op::Nop: break;
    }
    regs.pc = next;
    return std::nullopt;
}

// ---- running a function --------------------------------------------------------------------------------------------

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string());
        case End::Limit: break;
    }
    return "limit";
}

RunResult run(Bus& bus, std::uint16_t entry, const RunOptions& options) {
    const std::size_t log_start = bus.log().size();
    Cpu cpu(bus, options.decimal);
    cpu.regs.a = options.a;
    cpu.regs.x = options.x;
    cpu.regs.y = options.y;
    cpu.regs.p = options.p;
    const std::uint16_t ret = kReturnAddress - 1;   // RTS adds one
    // (the two pushes go through the bus like any write)
    cpu.regs.s = 0xFD;
    {
        bus.write(static_cast<std::uint16_t>(0x100 | cpu.regs.s), static_cast<std::uint8_t>(ret >> 8));
        cpu.regs.s = static_cast<std::uint8_t>(cpu.regs.s - 1);
        bus.write(static_cast<std::uint16_t>(0x100 | cpu.regs.s), static_cast<std::uint8_t>(ret));
        cpu.regs.s = static_cast<std::uint8_t>(cpu.regs.s - 1);
    }
    cpu.regs.pc = entry;
    RunResult out;
    out.end = End::Limit;
    while (cpu.steps < options.max_steps) {
        if (cpu.regs.pc == kReturnAddress) {
            out.end = End::Return;
            break;
        }
        if (auto fault = cpu.step()) {
            out.end = End::Stopped;
            out.fault = fault;
            break;
        }
    }
    out.regs = cpu.regs;
    out.steps = cpu.steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

cb::emulate::Run record(const RunResult& result) {
    cb::emulate::Run run;
    run.exit = result.exit();
    run.results["A"] = static_cast<std::uint64_t>(result.regs.a);
    run.results["X"] = static_cast<std::uint64_t>(result.regs.x);
    run.results["Y"] = static_cast<std::uint64_t>(result.regs.y);
    run.results["P"] = static_cast<std::uint64_t>(result.regs.p);
    run.results["S"] = static_cast<std::uint64_t>(result.regs.s);
    for (const DeviceAccess& a : result.io) {
        cb::emulate::IoAccess io;
        io.kind = a.kind == DeviceAccess::Kind::Read ? "r" : "w";
        io.port = a.address;
        io.value = a.value;
        run.io.push_back(std::move(io));
    }
    run.steps = result.steps;
    run.truncated = result.end == End::Limit;
    run.endian = cb::emulate::Endian::Little;
    run.raw = result;
    return run;
}

cb::emulate::Run recorded(Bus& bus, std::uint16_t entry, const RunOptions& options) {
    return record(run(bus, entry, options));
}

}  // namespace cb::emulate::m6502
