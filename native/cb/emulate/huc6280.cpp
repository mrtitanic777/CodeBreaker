#include "cb/emulate/huc6280.hpp"

#include <bit>
#include <format>
#include <utility>

#include "cb/formats/pce.hpp"

namespace cb::emulate::huc6280 {

namespace {

constexpr std::uint16_t kZeroPage = formats::pce::kZeroPage;
constexpr std::uint16_t kStack = formats::pce::kStack;
constexpr std::uint16_t kBrkVector = 0xFFF6;

bool is_ram_bank(std::uint8_t bank) { return bank >= 0xF8 && bank <= 0xFB; }

// The opcodes SET changes: ORA, AND, EOR and ADC in every addressing mode.
bool is_t_operation(std::uint8_t op) {
    switch (op) {
        case 0x01: case 0x05: case 0x09: case 0x0D: case 0x11: case 0x12: case 0x15: case 0x19: case 0x1D:   // ORA
        case 0x21: case 0x25: case 0x29: case 0x2D: case 0x31: case 0x32: case 0x35: case 0x39: case 0x3D:   // AND
        case 0x41: case 0x45: case 0x49: case 0x4D: case 0x51: case 0x52: case 0x55: case 0x59: case 0x5D:   // EOR
        case 0x61: case 0x65: case 0x69: case 0x6D: case 0x71: case 0x72: case 0x75: case 0x79: case 0x7D: return true;   // ADC
        default: return false;
    }
}

// The 65C02 core with the HuC6280's additions, over a Machine.
class Core {
public:
    explicit Core(Machine& machine) : machine_(machine), cpu_(bus_, true, m6502::Variant::Cmos) {
        bus_.map_quiet(
            0x0000, 0xFFFF, [this](std::uint16_t a) { return machine_.read(a); }, [this](std::uint16_t a, std::uint8_t v) { machine_.write(a, v); });
        cpu_.set_pages(kZeroPage, kStack);
    }

    m6502::Registers& regs() { return cpu_.regs; }
    std::uint64_t steps() const { return cpu_.steps; }

    std::optional<Fault> step() {
        m6502::Registers& r = cpu_.regs;
        const std::uint16_t pc = r.pc;
        const std::uint8_t op = machine_.read(pc);
        const bool t = std::exchange(set_pending_, false);
        if (t && is_t_operation(op)) {   // SET: the instruction's accumulator is the byte at zero page,X
            const std::uint16_t cell = static_cast<std::uint16_t>(kZeroPage + r.x);
            const std::uint8_t kept = r.a;
            r.a = machine_.read(cell);
            const std::optional<m6502::Fault> fault = cpu_.step();
            if (fault) {
                r.a = kept;
                return Fault{fault->opcode, fault->pc};
            }
            machine_.write(cell, r.a);
            r.a = kept;
            return std::nullopt;
        }
        if (handle(op, pc)) return std::nullopt;
        if (const std::optional<m6502::Fault> fault = cpu_.step()) return Fault{fault->opcode, fault->pc};
        return std::nullopt;
    }

private:
    std::uint8_t rd(std::uint16_t a) { return machine_.read(a); }
    std::uint16_t rd16(std::uint16_t a) {
        const std::uint16_t lo = rd(a);
        return static_cast<std::uint16_t>(lo | rd(static_cast<std::uint16_t>(a + 1)) << 8);
    }
    void push(std::uint8_t v) {
        machine_.write(static_cast<std::uint16_t>(kStack | cpu_.regs.s), v);
        cpu_.regs.s = static_cast<std::uint8_t>(cpu_.regs.s - 1);
    }
    void flag(std::uint8_t f, bool on) { cpu_.regs.p = on ? static_cast<std::uint8_t>(cpu_.regs.p | f) : static_cast<std::uint8_t>(cpu_.regs.p & ~f); }
    void count() { ++cpu_.steps; }

    // The instruction if it is one of the HuC6280's (or a Rockwell bit instruction), done; false for an instruction the shared core runs.
    bool handle(std::uint8_t op, std::uint16_t pc) {
        m6502::Registers& r = cpu_.regs;
        const auto next = [&](unsigned size) {
            r.pc = static_cast<std::uint16_t>(pc + size);
            count();
            return true;
        };
        const auto zp_cell = [&](unsigned at) { return static_cast<std::uint16_t>(kZeroPage + rd(static_cast<std::uint16_t>(pc + at))); };
        if ((op & 0x0F) == 0x07 || (op & 0x0F) == 0x0F) {   // RMBn SMBn (n in the high nibble) / BBRn BBSn
            const unsigned bit = (op >> 4) & 7;
            const bool high = (op & 0x80) != 0;   // SMB / BBS
            const std::uint16_t cell = zp_cell(1);
            if ((op & 0x0F) == 0x07) {
                const std::uint8_t v = rd(cell);
                machine_.write(cell, high ? static_cast<std::uint8_t>(v | (1u << bit)) : static_cast<std::uint8_t>(v & ~(1u << bit)));
                return next(2);
            }
            const bool set = (rd(cell) >> bit & 1) != 0;
            const std::int8_t d = static_cast<std::int8_t>(rd(static_cast<std::uint16_t>(pc + 2)));
            r.pc = static_cast<std::uint16_t>(pc + 3 + (set == high ? d : 0));
            count();
            return true;
        }
        switch (op) {
            case 0x00: {   // BRK
                const std::uint16_t ret = static_cast<std::uint16_t>(pc + 2);
                push(static_cast<std::uint8_t>(ret >> 8));
                push(static_cast<std::uint8_t>(ret));
                push(r.p | m6502::status::B | m6502::status::U);
                flag(m6502::status::I, true);
                flag(m6502::status::D, false);
                count();
                r.pc = rd16(kBrkVector);
                return true;
            }
            case 0x02: std::swap(r.x, r.y); return next(1);                                   // SXY
            case 0x22: std::swap(r.a, r.x); return next(1);                                   // SAX
            case 0x42: std::swap(r.a, r.y); return next(1);                                   // SAY
            case 0x03: machine_.hardware_write(0x0000, rd(static_cast<std::uint16_t>(pc + 1))); return next(2);   // ST0
            case 0x13: machine_.hardware_write(0x0002, rd(static_cast<std::uint16_t>(pc + 1))); return next(2);   // ST1
            case 0x23: machine_.hardware_write(0x0003, rd(static_cast<std::uint16_t>(pc + 1))); return next(2);   // ST2
            case 0x43: {   // TMA
                const std::uint8_t mask = rd(static_cast<std::uint16_t>(pc + 1));
                if (mask != 0) r.a = machine_.mpr[static_cast<std::size_t>(std::countr_zero(mask))];
                return next(2);
            }
            case 0x53: {   // TAM
                const std::uint8_t mask = rd(static_cast<std::uint16_t>(pc + 1));
                for (unsigned i = 0; i < 8; ++i) {
                    if (mask >> i & 1) machine_.mpr[i] = r.a;
                }
                return next(2);
            }
            case 0x44: {   // BSR
                const std::uint16_t ret = static_cast<std::uint16_t>(pc + 1);   // the address of its own last byte, as JSR pushes
                push(static_cast<std::uint8_t>(ret >> 8));
                push(static_cast<std::uint8_t>(ret));
                const std::int8_t d = static_cast<std::int8_t>(rd(static_cast<std::uint16_t>(pc + 1)));
                r.pc = static_cast<std::uint16_t>(pc + 2 + d);
                count();
                return true;
            }
            case 0x54: case 0xD4: return next(1);   // CSL CSH: the clock speed is not modelled
            case 0x62: r.a = 0; return next(1);     // CLA
            case 0x82: r.x = 0; return next(1);     // CLX
            case 0xC2: r.y = 0; return next(1);     // CLY
            case 0xF4: set_pending_ = true; return next(1);   // SET
            case 0x83: case 0x93: case 0xA3: case 0xB3: {     // TST
                const std::uint8_t mask = rd(static_cast<std::uint16_t>(pc + 1));
                std::uint16_t cell = 0;
                switch (op) {
                    case 0x83: cell = static_cast<std::uint16_t>(kZeroPage + rd(static_cast<std::uint16_t>(pc + 2))); break;
                    case 0x93: cell = rd16(static_cast<std::uint16_t>(pc + 2)); break;
                    case 0xA3: cell = static_cast<std::uint16_t>(kZeroPage + static_cast<std::uint8_t>(rd(static_cast<std::uint16_t>(pc + 2)) + r.x)); break;
                    default: cell = static_cast<std::uint16_t>(rd16(static_cast<std::uint16_t>(pc + 2)) + r.x); break;
                }
                const std::uint8_t v = rd(cell);
                flag(m6502::status::Z, (mask & v) == 0);
                flag(m6502::status::N, (v & 0x80) != 0);
                flag(m6502::status::V, (v & 0x40) != 0);
                return next(op == 0x83 || op == 0xA3 ? 3 : 4);
            }
            case 0x73: case 0xC3: case 0xD3: case 0xE3: case 0xF3: block(op, pc); return next(7);
            default: return false;
        }
    }

    // TII TDD TIN TIA TAI: copy `length` bytes (0 is 65536) from source to destination; the registers are unchanged.
    void block(std::uint8_t op, std::uint16_t pc) {
        std::uint16_t src = rd16(static_cast<std::uint16_t>(pc + 1));
        std::uint16_t dst = rd16(static_cast<std::uint16_t>(pc + 3));
        std::uint32_t length = rd16(static_cast<std::uint16_t>(pc + 5));
        if (length == 0) length = 0x10000;
        const std::uint16_t dst0 = dst, src0 = src;
        for (std::uint32_t i = 0; i < length; ++i) {
            machine_.write(dst, rd(src));
            switch (op) {
                case 0x73: ++src; ++dst; break;                                                                  // TII
                case 0xC3: --src; --dst; break;                                                                  // TDD
                case 0xD3: ++src; break;                                                                         // TIN
                case 0xE3: ++src; dst = static_cast<std::uint16_t>(dst0 + ((i + 1) & 1)); break;                 // TIA
                default: src = static_cast<std::uint16_t>(src0 + ((i + 1) & 1)); ++dst; break;                   // TAI
            }
        }
    }

    Machine& machine_;
    m6502::Bus bus_;
    m6502::Cpu cpu_;
    bool set_pending_ = false;
};

}  // namespace

std::string Access::text() const {
    std::string name = formats::pce::register_name(offset, write);
    if (vdc != 0xFF) name += "(" + formats::pce::vdc_register_name(vdc) + ")";
    return std::format("{} {} {:02x}", write ? "w" : "r", name, value);
}

Machine::Machine(ByteView rom) : rom_(rom) { mpr = {0xFF, 0xF8, 2, 3, 4, 5, 6, 0}; }

std::uint8_t Machine::read(std::uint16_t logical) {
    const std::uint8_t bank = mpr[logical >> 13];
    const std::uint16_t offset = logical & 0x1FFF;
    if (bank == formats::pce::kHardwareBank) return hardware_read(offset);
    if (is_ram_bank(bank)) return ram[offset];
    if (bank > 0x7F) return 0xFF;
    const std::size_t count = rom_.size() / kBankSize;
    if (count == 0) return 0xFF;
    std::size_t index = bank;
    if (std::has_single_bit(count)) index &= count - 1;
    else if (index >= count) return 0xFF;
    return rom_[index * kBankSize + offset];
}

void Machine::write(std::uint16_t logical, std::uint8_t value) {
    const std::uint8_t bank = mpr[logical >> 13];
    const std::uint16_t offset = logical & 0x1FFF;
    if (bank == formats::pce::kHardwareBank) hardware_write(offset, value);
    else if (is_ram_bank(bank)) ram[offset] = value;
}

std::uint8_t Machine::hardware_read(std::uint16_t offset) {
    std::uint8_t value = 0;
    std::uint8_t vdc = 0xFF;
    if (offset < 0x0400) {
        const unsigned reg = offset & 3;
        if (reg == 0) value = (vdc_status_reads_++ & 1) ? 0x20 : 0x00;   // the vertical-blank bit, on every other read
        else if (reg >= 2) vdc = vdc_select_;
    } else if (offset >= 0x1000 && offset < 0x1400) {
        value = 0xFF;   // no button pressed
    }
    log_.push_back({false, offset, value, vdc});
    return value;
}

void Machine::hardware_write(std::uint16_t offset, std::uint8_t value) {
    std::uint8_t vdc = 0xFF;
    if (offset < 0x0400) {
        const unsigned reg = offset & 3;
        if (reg == 0) vdc_select_ = value & 0x1F;
        else if (reg >= 2) vdc = vdc_select_;
    }
    log_.push_back({true, offset, value, vdc});
}

std::string Fault::text() const { return std::format("no instruction for opcode {:02x} at {:04x}", opcode, pc); }

std::string Run::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string());
        case End::Limit: break;
    }
    return "limit";
}

Run run(Machine& machine, std::uint16_t entry, const Options& options) {
    const std::size_t log_start = machine.log().size();
    Core core(machine);
    m6502::Registers& r = core.regs();
    r.a = options.a;
    r.x = options.x;
    r.y = options.y;
    r.s = 0xFD;
    const std::uint16_t ret = kReturnAddress - 1;   // RTS adds one
    machine.write(static_cast<std::uint16_t>(kStack | r.s), static_cast<std::uint8_t>(ret >> 8));
    r.s = static_cast<std::uint8_t>(r.s - 1);
    machine.write(static_cast<std::uint16_t>(kStack | r.s), static_cast<std::uint8_t>(ret));
    r.s = static_cast<std::uint8_t>(r.s - 1);
    r.pc = entry;
    Run out;
    while (core.steps() < options.max_steps) {
        if (r.pc == kReturnAddress) {
            out.end = End::Return;
            break;
        }
        if (const std::optional<Fault> fault = core.step()) {
            out.end = End::Stopped;
            out.fault = fault;
            break;
        }
    }
    out.regs = r;
    out.steps = core.steps();
    out.io.assign(machine.log().begin() + static_cast<std::ptrdiff_t>(log_start), machine.log().end());
    return out;
}

}  // namespace cb::emulate::huc6280
