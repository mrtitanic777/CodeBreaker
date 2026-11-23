// cb-unity: alone
// engine/pcode_cpu_huc6280.cpp -- the PC Engine lane's HuC6280 on the one p-code emulator: emulate::huc6280::run's call, on the description languages/huc6280.sla.
#include <algorithm>
#include <array>
#include <bit>
#include <cstring>
#include <utility>
#include <vector>

#include "decompile/engine/pcode_emulator.hh"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_huc6280.hpp"
#include "engine/pcode_cpu_internal.hpp"
#include "formats/pce.hpp"

namespace cb::engine::pcode_cpu {

namespace h = cb::emulate::huc6280;
namespace m = cb::emulate::m6502;
namespace sl = cb::decompile::sleigh;

namespace {

constexpr std::uint16_t kZeroPage = formats::pce::kZeroPage;
constexpr std::uint16_t kStack = formats::pce::kStack;
constexpr std::uint32_t kLogical = 0x10000;

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

// The byte at a logical address as the program would find it, without a hardware access (the hardware page and a bank nothing answers read $FF).
std::uint8_t peek(const h::Machine& machine, std::uint16_t logical) {
    const std::uint8_t bank = machine.mpr[logical >> 13];
    const std::uint16_t offset = logical & 0x1FFF;
    if (is_ram_bank(bank)) return machine.ram[offset];
    if (bank > 0x7F) return 0xFF;
    const ByteView rom = machine.rom();
    const std::size_t count = rom.size() / h::kBankSize;
    if (count == 0) return 0xFF;
    std::size_t index = bank;
    if (std::has_single_bit(count)) index &= count - 1;
    else if (index >= count) return 0xFF;
    return rom[index * h::kBankSize + offset];
}

// The opcodes the own core's wrapper runs itself (the HuC6280's additions and the Rockwell bit instructions); every other opcode it hands to the 65C02 core, which reads it again.
bool is_handled(std::uint8_t op) {
    if ((op & 0x0F) == 0x07 || (op & 0x0F) == 0x0F) return true;
    switch (op) {
        case 0x00: case 0x02: case 0x22: case 0x42: case 0x03: case 0x13: case 0x23: case 0x43: case 0x53: case 0x44: case 0x54: case 0xD4: case 0x62: case 0x82: case 0xC2: case 0xF4:
        case 0x83: case 0x93: case 0xA3: case 0xB3: case 0x73: case 0xC3: case 0xD3: case 0xE3: case 0xF3: return true;
        default: return false;
    }
}

// ADC and SBC (the opcodes of every addressing mode).
bool is_adc(std::uint8_t op) { return (op & 0xE0) == 0x60 && ((op & 0x1F) == 0x01 || (op & 0x1F) == 0x05 || (op & 0x1F) == 0x09 || (op & 0x1F) == 0x0D || (op & 0x1F) == 0x11 || (op & 0x1F) == 0x12 || (op & 0x1F) == 0x15 || (op & 0x1F) == 0x19 || (op & 0x1F) == 0x1D); }
bool is_sbc(std::uint8_t op) { return (op & 0xE0) == 0xE0 && ((op & 0x1F) == 0x01 || (op & 0x1F) == 0x05 || (op & 0x1F) == 0x09 || (op & 0x1F) == 0x0D || (op & 0x1F) == 0x11 || (op & 0x1F) == 0x12 || (op & 0x1F) == 0x15 || (op & 0x1F) == 0x19 || (op & 0x1F) == 0x1D); }

}  // namespace

h::Run run(h::Machine& machine, std::uint16_t entry, const h::Options& options) {
    sl::Emulator& emu = emulator_named("huc6280.sla");
    const std::size_t log_start = machine.log().size();

    // The page map the emulator decodes from: the logical address space as the mapping registers show it. It is kept per thread, as the emulator is: the same card is run again and
    // again, and what the emulator decoded from a window that has not changed need not be decoded again (a window that differs is handed over, and what was decoded from it dropped).
    thread_local std::vector<std::uint8_t> held;
    const bool fresh = held.size() != kLogical;
    if (fresh) held.assign(kLogical, 0);
    std::vector<std::uint8_t>& shadow = held;
    const auto view_window = [&](unsigned window) {
        std::array<std::uint8_t, h::kBankSize> want;
        for (std::uint32_t i = 0; i < h::kBankSize; ++i) want[i] = peek(machine, static_cast<std::uint16_t>((window << 13) | i));
        if (!fresh && std::memcmp(want.data(), shadow.data() + (window << 13), h::kBankSize) == 0) return;
        std::memcpy(shadow.data() + (window << 13), want.data(), h::kBankSize);
        emu.write(window << 13, want.data(), h::kBankSize);
    };
    for (unsigned w = 0; w < 8; ++w) view_window(w);
    if (fresh) emu.flushDecoded();
    emu.clearStops();

    const auto after_ram_write = [&](std::uint16_t offset) {   // a store into RAM: every window that shows it
        for (unsigned w = 0; w < 8; ++w) {
            if (!is_ram_bank(machine.mpr[w])) continue;
            const std::size_t at = (w << 13) | offset;
            if (shadow[at] == machine.ram[offset]) continue;
            shadow[at] = machine.ram[offset];
            emu.write(at, &shadow[at], 1);
        }
    };
    // A byte the instruction at hand was read as, handed over when the emulator does not hold it already (the bytes past $FFFF, where an instruction that crosses the end of the
    // address space has its operands, are kept apart).
    std::array<std::uint8_t, 8> beyond{};
    const auto put_code = [&](std::uint32_t at, std::uint8_t value) {
        std::uint8_t& slot = at < kLogical ? shadow[at] : beyond[at - kLogical];
        if (slot == value) return;
        slot = value;
        emu.write(at, &value, 1);
    };
    const auto store = [&](std::uint16_t logical, std::uint8_t value) {
        machine.write(logical, value);
        if (is_ram_bank(machine.mpr[logical >> 13])) after_ram_write(logical & 0x1FFF);
    };
    // The description reads an operand more than once in some instructions (an access of a hardware register is an event): what an instruction has read or written is kept
    // until it ends (not in a block transfer, which reads one place again and again on purpose).
    struct Seen {
        std::uint64_t addr, value;
        unsigned size;
    };
    std::array<Seen, 8> seen_reads{};
    unsigned seen_count = 0;
    bool keep_reads = true;       // the instruction is not a block transfer
    bool wrap_pointer = false;    // a zero page pointer at $FF: the chip's second byte is $00 of the page, the description's is $2100
    std::uint8_t current = 0;     // the opcode being run
    bool pushes = false, pulls = false;   // it is one that pushes or pulls: the description's SP is 16 bits wide, the chip's S wraps within its page
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        if (keep_reads)
            for (unsigned k = 0; k < seen_count; ++k)
                if (seen_reads[k].addr == addr && seen_reads[k].size == size) {
                    value = seen_reads[k].value;
                    return true;
                }
        value = 0;
        for (unsigned i = 0; i < size; ++i) {
            std::uint16_t at = static_cast<std::uint16_t>(addr + i);
            if (pulls && (at >> 8) == (kStack >> 8) + 1) at = static_cast<std::uint16_t>(at - 0x100);
            if (wrap_pointer && size == 2 && i == 1 && addr == kZeroPage + 0xFF) at = kZeroPage;
            value |= static_cast<std::uint64_t>(machine.read(at)) << (8 * i);
        }
        if (keep_reads && seen_count < seen_reads.size()) seen_reads[seen_count++] = {addr, value, size};
        return true;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!is_ram(space)) return false;
        if ((current == 0x08 || current == 0x00) && size == 1) value |= 0x20;   // (PHP and BRK push P with bit 5 set, as the own core's P has it; the description pushes T there)
        if (keep_reads) {
            seen_count = 0;
            seen_reads[seen_count++] = {addr, value, size};   // (what the instruction reads of this place from now on is what it wrote)
        }
        for (unsigned i = 0; i < size; ++i) {
            std::uint16_t at = static_cast<std::uint16_t>(addr + i);
            if (pushes && (at >> 8) == (kStack >> 8) - 1) at = static_cast<std::uint16_t>(at + 0x100);
            store(at, static_cast<std::uint8_t>(value >> (8 * i)));
        }
        return true;
    });
    const auto prepare = [&](std::uint8_t op, std::uint8_t operand) {
        current = op;
        seen_count = 0;
        pushes = op == 0x48 || op == 0xDA || op == 0x5A || op == 0x08 || op == 0x00 || op == 0x20 || op == 0x44;
        pulls = op == 0x68 || op == 0xFA || op == 0x7A || op == 0x28 || op == 0x60 || op == 0x40;
        keep_reads = !(op == 0x73 || op == 0xC3 || op == 0xD3 || op == 0xE3 || op == 0xF3);
        wrap_pointer = (op & 0x1F) == 0x12 && operand == 0xFF;
    };
    emu.onUserOp([&](sl::Emulator&, const std::string&, const std::vector<std::uint64_t>&, std::uint64_t&) { return false; });
    struct Unhook {
        sl::Emulator& e;
        ~Unhook() {
            e.onRead({});
            e.onWrite({});
            e.onUserOp({});
        }
    } unhook{emu};

    static constexpr const char* kNames[] = {"A", "X", "Y", "N", "V", "B", "D", "I", "Z", "C", "T", "S", "SH", "MPR0", "MPR1", "MPR2", "MPR3", "MPR4", "MPR5", "MPR6", "MPR7"};
    enum : unsigned { kA, kX, kY, kN, kV, kB, kD, kI, kZ, kC, kT, kS, kSH, kMpr };
    const auto& refs = reg_refs(emu, kNames);
    emu.setReg(refs[kA], options.a);
    emu.setReg(refs[kX], options.x);
    emu.setReg(refs[kY], options.y);
    for (unsigned f : {kN, kV, kB, kD, kZ, kC, kT}) emu.setReg(refs[f], 0);
    emu.setReg(refs[kI], 1);   // (the own core's P starts as U and I)
    for (unsigned i = 0; i < 8; ++i) emu.setReg(refs[kMpr + i], machine.mpr[i]);
    emu.setReg(refs[kSH], kStack >> 8);
    // the call as JSR makes it: the return address less one pushed (through the machine like any write), RTS adds one
    const std::uint16_t ret = h::kReturnAddress - 1;
    std::uint8_t s = 0xFD;
    store(static_cast<std::uint16_t>(kStack | s), static_cast<std::uint8_t>(ret >> 8));
    --s;
    store(static_cast<std::uint16_t>(kStack | s), static_cast<std::uint8_t>(ret));
    --s;
    emu.setReg(refs[kS], s);
    emu.setPc(entry);

    // With D set ADC and SBC work in decimal (the own core's 65C02 does; the description's are binary): the operand is what the binary result and the registers before it leave
    // (ADC: A + v + C, SBC: A + ~v + C), the result and flags are the own core's.
    struct Before {
        std::uint8_t a;
        bool carry, decimal;
    };
    const auto before = [&]() { return Before{static_cast<std::uint8_t>(emu.reg(refs[kA])), emu.reg(refs[kC]) != 0, emu.reg(refs[kD]) != 0}; };
    const auto decimal_fix = [&](std::uint8_t op, const Before& b) {
        const bool adc = is_adc(op), sbc = is_sbc(op);
        if (!b.decimal || (!adc && !sbc)) return;
        const int a = b.a, c = b.carry ? 1 : 0;
        const int binary = static_cast<int>(emu.reg(refs[kA]));
        int v = (binary - a - c) & 0xFF;
        if (sbc) v ^= 0xFF;
        int result = 0;
        bool carry = false, overflow = false;
        if (adc) {
            int lo = (a & 0x0F) + (v & 0x0F) + c;
            int hi = (a >> 4) + (v >> 4) + (lo > 9 ? 1 : 0);
            overflow = (~(a ^ v) & (a ^ (hi << 4)) & 0x80) != 0;
            if (lo > 9) lo += 6;
            if (hi > 9) hi += 6;
            carry = hi > 15;
            result = (hi << 4) | (lo & 0x0F);
        } else {
            const int borrow = 1 - c;
            const int r = a - v - borrow;
            int lo = (a & 0x0F) - (v & 0x0F) - borrow;
            int hi = (a >> 4) - (v >> 4) - (lo < 0 ? 1 : 0);
            carry = r >= 0;
            overflow = ((a ^ v) & (a ^ r) & 0x80) != 0;
            if (lo < 0) lo -= 6;
            if (hi < 0) hi -= 6;
            result = hi * 16 | (lo & 0x0F);
        }
        const std::uint8_t byte = static_cast<std::uint8_t>(result);
        emu.setReg(refs[kA], byte);
        emu.setReg(refs[kC], carry ? 1 : 0);
        emu.setReg(refs[kV], overflow ? 1 : 0);
        emu.setReg(refs[kZ], byte == 0 ? 1 : 0);
        emu.setReg(refs[kN], (byte & 0x80) != 0 ? 1 : 0);
    };

    h::Run out;
    out.end = h::End::Limit;
    std::uint64_t steps = 0;
    bool set_pending = false;
    while (steps < options.max_steps) {
        const std::uint16_t pc = static_cast<std::uint16_t>(emu.pc());
        if (pc == h::kReturnAddress) {
            out.end = h::End::Return;
            break;
        }
        // The instruction's bytes. A program does not run from the hardware page, but a function the lane judges may be data, or a vector may point there: the own core then reads the
        // opcode through the chips' registers (a read the log shows, which changes the VDC status's next answer), reads it again for the 65C02 core when it is not one of the
        // HuC6280's own, and the operand bytes after it; the bytes it finds are what runs. An instruction that runs over the end of the address space finds its operands at $0000.
        std::array<std::uint8_t, 8> code;
        for (unsigned i = 0; i < code.size(); ++i) code[i] = shadow[static_cast<std::uint16_t>(pc + i)];
        bool brk_by_65c02 = false;
        const auto hardware = [&](std::uint32_t at) { return machine.mpr[(at & 0xFFFF) >> 13] == formats::pce::kHardwareBank; };
        if (hardware(pc) || hardware(pc + 6) || pc > 0xFFF9) {
            std::uint8_t first = code[0];
            if (hardware(pc)) {
                first = machine.read(pc);
                if (!is_handled(first)) {
                    first = machine.read(pc);   // (the 65C02 core's read of the opcode)
                    brk_by_65c02 = first == 0x00;
                }
            }
            code[0] = first;
            put_code(pc, first);
            thread_local std::array<std::int8_t, 256> lengths = [] { std::array<std::int8_t, 256> a; a.fill(-1); return a; }();   // (an opcode's length does not depend on its operands)
            if (lengths[first] < 0) lengths[first] = static_cast<std::int8_t>(std::min<std::size_t>(emu.length(pc), 7));
            const unsigned length = static_cast<unsigned>(lengths[first]);
            for (unsigned i = 1; i < 8; ++i) {
                const std::uint32_t at = static_cast<std::uint32_t>(pc) + i;
                if (i < length && hardware(at)) code[i] = machine.read(static_cast<std::uint16_t>(at));
                put_code(at, code[i]);
            }
        }
        const std::uint8_t op = code[0];
        const bool t = std::exchange(set_pending, false);
        if (t && is_t_operation(op)) {   // SET: the instruction's accumulator is the byte at zero page,X
            const std::uint16_t cell = static_cast<std::uint16_t>(kZeroPage + static_cast<std::uint8_t>(emu.reg(refs[kX])));
            const std::uint64_t kept = emu.reg(refs[kA]);
            emu.setReg(refs[kA], machine.read(cell));
            prepare(op, code[1]);
            const Before was = before();
            if (emu.step() != sl::Emulator::Status::Ok) {
                emu.setReg(refs[kA], kept);
                out.end = h::End::Stopped;
                out.fault = h::Fault{op, pc};
                break;
            }
            decimal_fix(op, was);
            store(cell, static_cast<std::uint8_t>(emu.reg(refs[kA])));
            emu.setReg(refs[kA], kept);
            emu.setReg(refs[kSH], kStack >> 8);
            ++steps;
            continue;
        }
        if (op == 0x03 || op == 0x13 || op == 0x23) {   // ST0 ST1 ST2: the VDC's ports, whatever the mapping
            machine.hardware_write(op == 0x03 ? 0x0000 : op == 0x13 ? 0x0002 : 0x0003, code[1]);
            emu.setPc(static_cast<std::uint16_t>(pc + 2));
        } else if (op == 0xF4 || op == 0x54 || op == 0xD4) {   // SET; CSL CSH: the clock speed is not modelled
            set_pending = op == 0xF4;
            emu.setPc(static_cast<std::uint16_t>(pc + 1));
        } else if (op == 0x43) {   // TMA: with several bits of the mask set the own core takes the lowest register
            const std::uint8_t mask = code[1];
            if (mask != 0) emu.setReg(refs[kA], emu.reg(refs[kMpr + static_cast<unsigned>(std::countr_zero(mask))]));
            emu.setPc(static_cast<std::uint16_t>(pc + 2));
        } else {
            prepare(op, code[1]);
            const std::uint64_t n = emu.reg(refs[kN]), v = emu.reg(refs[kV]);
            const Before was = before();
            const sl::Emulator::Status status = emu.step();
            if (op == 0x89) {   // BIT #: the own core's (the 65C02's) leaves N and V alone, the description sets them from the byte
                emu.setReg(refs[kN], n);
                emu.setReg(refs[kV], v);
            }
            if (status == sl::Emulator::Status::BadInstruction || status == sl::Emulator::Status::Error) {
                out.end = h::End::Stopped;
                out.fault = h::Fault{op, pc};
                break;
            }
            emu.setReg(refs[kSH], kStack >> 8);   // (S wraps within its page: the high byte is put back after every instruction)
            decimal_fix(op, was);
            if (op == 0x00) {   // (BRK clears D after pushing P, as the own core's 65C02 does; the description leaves it)
                emu.setReg(refs[kD], 0);
                if (brk_by_65c02) emu.setPc(static_cast<std::uint16_t>(machine.read(0xFFFE) | machine.read(0xFFFF) << 8));   // (and takes its vector from $FFFE, not the HuC6280's $FFF6)
            }
            if (op == 0x53) {   // TAM: the mapping changed
                for (unsigned i = 0; i < 8; ++i) {
                    const std::uint8_t now = static_cast<std::uint8_t>(emu.reg(refs[kMpr + i]));
                    if (now == machine.mpr[i]) continue;
                    machine.mpr[i] = now;
                    view_window(i);
                }
            }
        }
        if (emu.pc() > 0xFFFF) emu.setPc(emu.pc() & 0xFFFF);   // (the program counter is 16 bits: an instruction that ends past $FFFF goes on at $0000)
        ++steps;
    }
    m::Registers& r = out.regs;
    r.a = static_cast<std::uint8_t>(emu.reg(refs[kA]));
    r.x = static_cast<std::uint8_t>(emu.reg(refs[kX]));
    r.y = static_cast<std::uint8_t>(emu.reg(refs[kY]));
    r.s = static_cast<std::uint8_t>(emu.reg(refs[kS]));
    r.pc = static_cast<std::uint16_t>(emu.pc());
    // (B is no flag of the chip: the description sets it as PHP and BRK push it; the own core's P keeps the bit it began with, none)
    std::uint8_t p = m::status::U;
    for (const auto& [flag, bit] : {std::pair{kN, m::status::N}, {kV, m::status::V}, {kD, m::status::D}, {kI, m::status::I}, {kZ, m::status::Z}, {kC, m::status::C}})
        if (emu.reg(refs[flag]) != 0) p |= bit;
    r.p = p;
    out.steps = steps;
    out.io.assign(machine.log().begin() + static_cast<std::ptrdiff_t>(log_start), machine.log().end());
    return out;
}

h::Run call(h::Machine& machine, std::uint16_t entry, const h::Options& options) { return enabled_when_asked() ? pcode_cpu::run(machine, entry, options) : h::run(machine, entry, options); }

}  // namespace cb::engine::pcode_cpu
