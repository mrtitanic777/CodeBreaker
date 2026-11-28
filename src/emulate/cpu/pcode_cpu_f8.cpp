#include "emulate/cpu/pcode_cpu_f8.hpp"

#include "emulate/cpu/pcode_cpu_internal.hpp"

#include <cstring>
#include <format>
#include <string>
#include <vector>

#include "decompile/engine/pcode_emulator.hh"

namespace cb::emulate::pcode_cpu {

namespace f8cpu = cb::emulate::f8;
namespace sl = cb::decompile::sleigh;

namespace {

constexpr const char* kF8Names[] = {"A", "ISAR", "SF", "CF", "ZF", "OF", "ICB", "PC1", "DC0", "DC1"};
namespace f8_reg {
enum Name : std::size_t { A, ISAR, SF, CF, ZF, OF, ICB, PC1, DC0, DC1 };
}   // namespace f8_reg

// The own core's decimal add (the description leaves it to the host): the sum with both digits biased by 6, the adjustments where a digit did not carry, and the carry out.
struct F8Decimal {
    std::uint8_t result;
    bool carry;
};
F8Decimal f8_add_decimal(std::uint8_t a, std::uint8_t b) {
    const std::uint8_t biased = static_cast<std::uint8_t>(a + 0x66);
    const unsigned sum = biased + b;
    const bool carry = a + 0x66 + b > 0xFF || a + 0x66 > 0xFF;
    const bool low_carry = ((biased & 0xF) + (b & 0xF)) > 0xF;
    std::uint8_t res = static_cast<std::uint8_t>(sum);
    if (!low_carry) res = static_cast<std::uint8_t>((res & 0xF0) | ((res - 6) & 0x0F));
    if (!carry) res = static_cast<std::uint8_t>(((res - 0x60) & 0xF0) | (res & 0x0F));
    return {res, carry};
}

}  // namespace

f8cpu::RunResult run(f8cpu::Bus& bus, std::uint16_t entry, const f8cpu::RunOptions& options) {
    sl::Emulator& emu = emulator_named("f8.sla");
    const auto& reg = reg_refs(emu, kF8Names);
    const std::size_t log_start = bus.log().size();
    constexpr std::uint32_t kSize = 0x10000;

    // The page map the emulator decodes from: the bus's memory as it was last looked at (a store into writable memory is compared with it).
    std::vector<std::uint8_t> shadow(kSize);
    {
        const ByteView memory = bus.memory();
        std::memcpy(shadow.data(), memory.data(), kSize);
    }
    emu.write(0, shadow.data(), kSize);
    emu.flushDecoded();
    emu.clearStops();

    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (is_ram(space)) {
            value = 0;
            for (unsigned i = 0; i < size; ++i) value |= static_cast<std::uint64_t>(bus.read(static_cast<std::uint16_t>(addr + i))) << (8 * i);
            return true;
        }
        if (space == "PORT") {
            value = bus.port_in(static_cast<std::uint8_t>(addr));
            return true;
        }
        return false;   // (the scratchpad is the emulator's)
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (is_ram(space)) {
            for (unsigned i = 0; i < size; ++i) {
                const std::uint16_t at = static_cast<std::uint16_t>(addr + i);
                bus.write(at, static_cast<std::uint8_t>(value >> (8 * i)));
                if (const std::uint8_t now = bus.read(at); now != shadow[at]) {   // (a store into RAM that code runs from)
                    shadow[at] = now;
                    emu.write(at, &now, 1);
                }
            }
            return true;
        }
        if (space == "PORT") {
            bus.port_out(static_cast<std::uint8_t>(addr), static_cast<std::uint8_t>(value));
            return true;
        }
        return false;
    });
    emu.onUserOp([&](sl::Emulator&, const std::string& name, const std::vector<std::uint64_t>& args, std::uint64_t& result) {
        if (name == "bcdAdd" && args.size() >= 2) result = f8_add_decimal(static_cast<std::uint8_t>(args[0]), static_cast<std::uint8_t>(args[1])).result;
        else if (name == "bcdCarry" && args.size() >= 2) result = f8_add_decimal(static_cast<std::uint8_t>(args[0]), static_cast<std::uint8_t>(args[1])).carry ? 1 : 0;
        else if (name != "nop" && name != "disableInt" && name != "enableInt") return false;   // (interrupts are not modelled)
        return true;
    });
    struct Unhook {
        sl::Emulator& e;
        ~Unhook() {
            e.onRead({});
            e.onWrite({});
            e.onUserOp({});
        }
    } unhook{emu};

    // the registers: the scratchpad starts as R0-R11 given and zeros, the flags clear, a run is made as PI makes it
    std::uint8_t scratch[64] = {};
    for (std::size_t k = 0; k < options.r.size(); ++k) scratch[k] = options.r[k];
    emu.write(0, scratch, sizeof scratch, "SPAD");
    emu.setReg(reg[f8_reg::A], options.a);
    for (const f8_reg::Name flag : {f8_reg::ISAR, f8_reg::SF, f8_reg::CF, f8_reg::ZF, f8_reg::OF, f8_reg::ICB}) emu.setReg(reg[flag], 0);
    emu.setReg(reg[f8_reg::PC1], f8cpu::kReturnAddress);
    emu.setReg(reg[f8_reg::DC0], 0);
    emu.setReg(reg[f8_reg::DC1], 0);
    emu.setPc(entry);

    const auto snapshot = [&] {
        f8cpu::Registers g;
        g.a = static_cast<std::uint8_t>(emu.reg(reg[f8_reg::A]));
        g.status = static_cast<std::uint8_t>((emu.reg(reg[f8_reg::SF]) ? f8cpu::w::S : 0) | (emu.reg(reg[f8_reg::CF]) ? f8cpu::w::C : 0) | (emu.reg(reg[f8_reg::ZF]) ? f8cpu::w::Z : 0) |
                                             (emu.reg(reg[f8_reg::OF]) ? f8cpu::w::O : 0) | (emu.reg(reg[f8_reg::ICB]) ? f8cpu::w::ICB : 0));
        g.isar = static_cast<std::uint8_t>(emu.reg(reg[f8_reg::ISAR]));
        g.pc0 = static_cast<std::uint16_t>(emu.pc());
        g.pc1 = static_cast<std::uint16_t>(emu.reg(reg[f8_reg::PC1]));
        g.dc0 = static_cast<std::uint16_t>(emu.reg(reg[f8_reg::DC0]));
        g.dc1 = static_cast<std::uint16_t>(emu.reg(reg[f8_reg::DC1]));
        emu.read(0, g.scratch.data(), g.scratch.size(), "SPAD");
        return g;
    };

    f8cpu::RunResult out;
    out.end = f8cpu::End::Limit;
    std::uint64_t steps = 0;
    while (steps < options.max_steps) {
        const std::uint16_t pc = static_cast<std::uint16_t>(emu.pc());
        if (pc == f8cpu::kReturnAddress) {
            out.end = f8cpu::End::Return;
            break;
        }
        if (options.on_instruction) options.on_instruction(snapshot());
        const std::uint8_t op = shadow[pc];
        // PI and JMP load the accumulator with the high byte of the address (neither reads the accumulator: set before the instruction)
        if (op == 0x28 || op == 0x29) emu.setReg(reg[f8_reg::A], shadow[static_cast<std::uint16_t>(pc + 1)]);
        // CI and CM compare x with the accumulator as x + ~A + 1; their overflow is that of the whole sum
        std::uint8_t compared = 0, accumulator = 0;
        const bool compare = op == 0x25 || op == 0x8D;
        if (compare) {
            compared = op == 0x25 ? shadow[static_cast<std::uint16_t>(pc + 1)] : bus.read(static_cast<std::uint16_t>(emu.reg(reg[f8_reg::DC0])));
            accumulator = static_cast<std::uint8_t>(emu.reg(reg[f8_reg::A]));
        }
        const sl::Emulator::Status status = emu.step();
        if (status == sl::Emulator::Status::BadInstruction || status == sl::Emulator::Status::Error) {
            out.end = f8cpu::End::Stopped;
            out.fault = f8cpu::Fault{pc, std::format("undefined opcode {:02X} at {:04X}", op, pc)};
            break;
        }
        if (compare) {
            const std::uint8_t negated = static_cast<std::uint8_t>(~accumulator), res = static_cast<std::uint8_t>(compared + negated + 1);
            emu.setReg(reg[f8_reg::OF], (((compared ^ res) & (negated ^ res)) & 0x80) != 0 ? 1 : 0);
        }
        ++steps;
    }
    if (out.end == f8cpu::End::Limit && emu.pc() == f8cpu::kReturnAddress) out.end = f8cpu::End::Return;
    out.regs = snapshot();
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

}  // namespace cb::emulate::pcode_cpu
