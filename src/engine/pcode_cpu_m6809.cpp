// cb-unity: alone
// engine/pcode_cpu_m6809.cpp -- the Vectrex lane's 6809 on the one p-code emulator: emulate::m6809::run's call, on the description languages/6809.sla.
#include <algorithm>
#include <array>
#include <iterator>
#include <cstring>
#include <format>
#include <string>
#include <string_view>
#include <vector>

#include "decompile/engine/pcode_emulator.hh"
#include "decompile/m6809.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_internal.hpp"
#include "engine/pcode_cpu_m6809.hpp"

namespace cb::engine::pcode_cpu {

namespace m = cb::emulate::m6809;
namespace sl = cb::decompile::sleigh;
namespace dm = cb::decompile::m6809;

namespace {

// What an instruction address holds, found once and kept until a byte near it changes.
enum Kind : std::uint8_t { kUnknown = 0, kRun, kPush, kCarried, kUndefined, kWaits };

bool waits_for_interrupt(std::string_view mnemonic) { return mnemonic == "SYNC" || mnemonic == "CWAI" || mnemonic == "SWI" || mnemonic == "SWI2" || mnemonic == "SWI3"; }

// The instructions that push: the own core writes the low byte of a word first (the stack grows down), the description writes the word as one access.
bool pushes(std::string_view mnemonic) { return mnemonic == "BSR" || mnemonic == "LBSR" || mnemonic == "JSR" || mnemonic == "PSHS" || mnemonic == "PSHU"; }

// The instructions the description does not do as the CPU does (its branch conditions, rotates and flags: the stock description's), which run as the own core runs them.
bool carried(std::string_view mnemonic) {
    static constexpr std::string_view names[] = {"BGT", "LBGT", "BLT", "LBLT", "ROR", "RORA", "RORB", "SEX", "DAA"};
    return std::find(std::begin(names), std::end(names), mnemonic) != std::end(names);
}

}  // namespace

m::RunResult run(m::Bus& bus, std::uint16_t entry, const m::RunOptions& options) {
    sl::Emulator& emu = emulator_named("6809.sla");
    const std::span<std::uint8_t> memory = bus.memory();
    constexpr std::uint32_t kSize = m::kMemorySize;

    // The page map the emulator decodes from: the bus's memory as the own core reads instructions (not through the device ranges).
    std::vector<std::uint8_t> shadow(memory.begin(), memory.begin() + kSize);
    emu.write(0, shadow.data(), kSize);
    emu.flushDecoded();
    emu.clearStops();
    std::vector<std::uint8_t> kinds(kSize, kUnknown);

    const auto update = [&](std::uint32_t at) {   // a byte of memory the bus changed: the emulator decodes the new one, and what was found about instructions near it is forgotten
        if (memory[at] == shadow[at]) return;
        shadow[at] = memory[at];
        emu.write(at, &shadow[at], 1);
        for (std::uint32_t k = at >= 4 ? at - 4 : 0; k <= at; ++k) kinds[k] = kUnknown;
    };
    // The description reads an operand more than once in some instructions, and reads it again after storing the result (an access of a device is an event): what an instruction has read or written is kept until it ends.
    struct Seen {
        std::uint64_t addr, value;
        unsigned size;
    };
    std::array<Seen, 8> seen_reads{};
    unsigned seen_count = 0;
    bool descending = false;   // the instruction pushes
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        for (unsigned k = 0; k < seen_count; ++k)
            if (seen_reads[k].addr == addr && seen_reads[k].size == size) {
                value = seen_reads[k].value;
                return true;
            }
        value = 0;
        for (unsigned i = 0; i < size; ++i) value = value << 8 | bus.read(static_cast<std::uint16_t>(addr + i));
        if (seen_count < seen_reads.size()) seen_reads[seen_count++] = {addr, value, size};
        return true;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!is_ram(space)) return false;
        seen_count = 0;
        seen_reads[seen_count++] = {addr, value, size};   // (what the instruction reads of this place from now on is what it wrote)
        for (unsigned k = 0; k < size; ++k) {
            const unsigned i = descending ? size - 1 - k : k;
            const std::uint16_t at = static_cast<std::uint16_t>(addr + i);
            bus.write(at, static_cast<std::uint8_t>(value >> (8 * (size - 1 - i))));
            update(at);
        }
        return true;
    });
    struct Unhook {
        sl::Emulator& e;
        ~Unhook() {
            e.onRead({});
            e.onWrite({});
        }
    } unhook{emu};

    static constexpr const char* kNames[] = {"A", "B", "DP", "CC", "X", "Y", "U", "S"};
    enum : unsigned { kA, kB, kDP, kCC, kX, kY, kU, kS };
    const auto& refs = reg_refs(emu, kNames);
    emu.setReg(refs[kA], options.a);
    emu.setReg(refs[kB], options.b);
    emu.setReg(refs[kDP], options.dp);
    emu.setReg(refs[kCC], m::cc::I | m::cc::F);
    emu.setReg(refs[kX], options.x);
    emu.setReg(refs[kY], options.y);
    emu.setReg(refs[kU], options.u);
    // the call as JSR makes it: the return address pushed through the bus like any write
    const std::uint16_t sp = static_cast<std::uint16_t>(options.s - 2);
    emu.setReg(refs[kS], sp);
    bus.write(sp, static_cast<std::uint8_t>(m::kReturnAddress >> 8));
    bus.write(static_cast<std::uint16_t>(sp + 1), static_cast<std::uint8_t>(m::kReturnAddress));
    update(sp);
    update(static_cast<std::uint16_t>(sp + 1));
    emu.setPc(entry);
    const std::size_t log_start = bus.log().size();   // (the return address is no effect of the function)

    const auto registers = [&] {
        m::Registers r;
        r.a = static_cast<std::uint8_t>(emu.reg(refs[kA]));
        r.b = static_cast<std::uint8_t>(emu.reg(refs[kB]));
        r.dp = static_cast<std::uint8_t>(emu.reg(refs[kDP]));
        r.cc = static_cast<std::uint8_t>(emu.reg(refs[kCC]));
        r.x = static_cast<std::uint16_t>(emu.reg(refs[kX]));
        r.y = static_cast<std::uint16_t>(emu.reg(refs[kY]));
        r.u = static_cast<std::uint16_t>(emu.reg(refs[kU]));
        r.s = static_cast<std::uint16_t>(emu.reg(refs[kS]));
        r.pc = static_cast<std::uint16_t>(emu.pc());
        return r;
    };
    const auto sync = [&] {   // what a device did to the bus's memory: the bytes that differ
        if (std::memcmp(shadow.data(), memory.data(), kSize) == 0) return;
        for (std::uint32_t i = 0; i < kSize; ++i) update(i);
    };
    std::size_t seen = bus.log().size();

    m::RunResult out;
    out.end = m::End::Limit;
    std::uint64_t steps = 0;
    while (steps < options.max_steps) {
        const std::uint16_t pc = static_cast<std::uint16_t>(emu.pc());
        if (pc == m::kReturnAddress) {
            out.end = m::End::Return;
            break;
        }
        if (options.on_instruction) options.on_instruction(registers());
        std::uint8_t& kind = kinds[pc];
        if (kind == kUnknown) {
            const std::optional<dm::Insn> found = dm::decode(memory, pc, pc);
            kind = !found ? kUndefined : waits_for_interrupt(found->mnemonic) ? kWaits : carried(found->mnemonic) ? kCarried : pushes(found->mnemonic) ? kPush : kRun;
        }
        if (kind == kUndefined || kind == kWaits) {
            out.end = m::End::Stopped;
            const std::optional<dm::Insn> found = dm::decode(memory, pc, pc);
            out.fault = m::Fault{pc, kind == kUndefined ? std::format("undefined opcode {:02X} at {:04X}", memory[pc], pc)
                                                        : std::format("waits for an interrupt ({}) at {:04X}", found->mnemonic, pc)};
            break;
        }
        if (kind == kCarried) {   // as the own core runs it, on the registers as they stand
            m::Cpu cpu(bus);
            cpu.regs = registers();
            cpu.step();
            emu.setReg(refs[kA], cpu.regs.a);
            emu.setReg(refs[kB], cpu.regs.b);
            emu.setReg(refs[kDP], cpu.regs.dp);
            emu.setReg(refs[kCC], cpu.regs.cc);
            emu.setReg(refs[kX], cpu.regs.x);
            emu.setReg(refs[kY], cpu.regs.y);
            emu.setReg(refs[kU], cpu.regs.u);
            emu.setReg(refs[kS], cpu.regs.s);
            emu.setPc(cpu.regs.pc);
            sync();
            seen = bus.log().size();
            ++steps;
            continue;
        }
        descending = kind == kPush;
        seen_count = 0;
        const sl::Emulator::Status status = emu.step();
        if (status == sl::Emulator::Status::BadInstruction || status == sl::Emulator::Status::Error) {   // (an instruction the table has and the description lacks)
            out.end = m::End::Stopped;
            out.fault = m::Fault{pc, std::format("undefined opcode {:02X} at {:04X}", memory[pc], pc)};
            break;
        }
        ++steps;
        if (bus.log().size() != seen) {   // a device was touched: it may have changed the memory
            sync();
            seen = bus.log().size();
        }
    }
    if (out.end == m::End::Limit && emu.pc() == m::kReturnAddress) out.end = m::End::Return;
    out.regs = registers();
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

m::RunResult call(m::Bus& bus, std::uint16_t entry, const m::RunOptions& options) { return enabled_when_asked() ? pcode_cpu::run(bus, entry, options) : m::run(bus, entry, options); }

}  // namespace cb::engine::pcode_cpu
