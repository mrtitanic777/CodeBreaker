// cb-unity: alone
// engine/pcode_cpu_m6809.cpp -- the Vectrex lane's 6809 on the one p-code emulator: emulate::m6809::run's call, on the description languages/6809.sla.
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
enum Kind : std::uint8_t { kUnknown = 0, kRun, kPush, kUndefined, kWaits };

bool waits_for_interrupt(std::string_view mnemonic) { return mnemonic == "SYNC" || mnemonic == "CWAI" || mnemonic == "SWI" || mnemonic == "SWI2" || mnemonic == "SWI3"; }

// The instructions that push: the own core writes the low byte of a word first (the stack grows down), the description writes the word as one access.
bool pushes(std::string_view mnemonic) { return mnemonic == "BSR" || mnemonic == "LBSR" || mnemonic == "JSR" || mnemonic == "PSHS" || mnemonic == "PSHU"; }

}  // namespace

m::RunResult run(m::Bus& bus, std::uint16_t entry, const m::RunOptions& options) {
    sl::Emulator& emu = emulator_named("6809.sla");
    const std::span<std::uint8_t> memory = bus.memory();
    constexpr std::uint32_t kSize = m::kMemorySize;

    // The page map the emulator decodes from (`shadow`): the bus's memory as the own core reads instructions (not through the device ranges). It is kept per thread with the
    // emulator, and so is what was found about the instruction at each address: only the bytes of the instruction about to run are read from it, so they are compared with the
    // memory just before each step, and a byte that differs is handed over then (a store, a device, or another run's memory cost nothing until code runs from the changed bytes).
    thread_local std::vector<std::uint8_t> held;
    thread_local std::vector<std::uint8_t> kinds;
    if (held.size() != kSize) {
        held.assign(kSize, 0);
        kinds.assign(kSize, kUnknown);
        emu.flushDecoded();
    }
    std::vector<std::uint8_t>& shadow = held;
    emu.clearStops();

    const auto update = [&](std::uint32_t at) {   // a byte of memory the emulator decodes from that is not what it holds: it decodes the new one, and what was found about instructions near it is forgotten
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
        for (std::uint32_t i = 0; i < 5; ++i) update((pc + i) & (kSize - 1));   // (the longest instruction has five bytes)
        std::uint8_t& kind = kinds[pc];
        if (kind == kUnknown) {
            const std::optional<dm::Insn> found = dm::decode(memory, pc, pc);
            kind = !found ? kUndefined : waits_for_interrupt(found->mnemonic) ? kWaits : pushes(found->mnemonic) ? kPush : kRun;
        }
        if (kind == kUndefined || kind == kWaits) {
            out.end = m::End::Stopped;
            const std::optional<dm::Insn> found = dm::decode(memory, pc, pc);
            out.fault = m::Fault{pc, kind == kUndefined ? std::format("undefined opcode {:02X} at {:04X}", memory[pc], pc)
                                                        : std::format("waits for an interrupt ({}) at {:04X}", found->mnemonic, pc)};
            break;
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
    }
    if (out.end == m::End::Limit && emu.pc() == m::kReturnAddress) out.end = m::End::Return;
    out.regs = registers();
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

m::RunResult call(m::Bus& bus, std::uint16_t entry, const m::RunOptions& options) { return enabled() ? pcode_cpu::run(bus, entry, options) : m::run(bus, entry, options); }

}  // namespace cb::engine::pcode_cpu
