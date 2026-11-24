// cb-unity: alone (file-local names that other files in a batch also use)
// engine/pcode_cpu_v810.cpp -- the Virtual Boy lane's NEC V810 on the one p-code emulator: emulate::v810::run's call, on languages/v810.sla.
//
// The description runs the integer instructions, the branches, jr / jal / jmp, and the loads and stores (every data access is the Bus's). The adapter carries what it leaves
// to the machine: the instruction is fetched from the bus's memory and handed to the emulator a word at a time (a fetch the bus would log, from a device or a logged range,
// is made by the own core), jmp clears the low bit of its target, r0 stays zero, and the own core runs, on the registers as they stand, what the description does not carry:
// the floating-point instructions (their flags), the bit strings, ldsr, stsr, caxi, a division the description cannot do (by zero, or the one that overflows), trap, reti and
// halt, and an undefined pattern (the faults come from it).
#include <array>
#include <cstring>
#include <optional>
#include <string>

#include "decompile/engine/pcode_emulator.hh"
#include "decompile/v810.hpp"
#include "engine/pcode_cpu_internal.hpp"
#include "engine/pcode_cpu_v810.hpp"

namespace cb::engine::pcode_cpu {

namespace v8 = cb::emulate::v810;
namespace dv = cb::decompile::v810;
namespace sl = cb::decompile::sleigh;

namespace {

constexpr const char* kRegisters[32] = {"r0",  "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",  "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15",
                                        "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23", "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31"};
// The program status word's flags in the description, each a register of its own, and the bit of the word it is.
constexpr const char* kFlagNames[14] = {"Z", "S", "OV", "CY", "FPR", "FUD", "FOV", "FZD", "FIV", "FRO", "ID", "AE", "EP", "NP"};
constexpr unsigned kFlagBits[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14, 15};
constexpr const char* kSystem[8] = {"eipc", "eipsw", "fepc", "fepsw", "ecr", "chcw", "adtre", "PSWI"};

// An instruction the own core runs, because the description does not carry what it needs.
bool own_core_runs(const dv::Insn& insn, const std::array<std::uint32_t, 32>& r) {
    using namespace dv::id;
    switch (insn.id) {
        case kTrap: case kReti: case kHalt: case kLdsr: case kStsr: case kCaxi: return true;
        case kDiv: return r[insn.r1] == 0 || (r[insn.r1] == 0xFFFFFFFFu && r[insn.r2] == 0x80000000u);
        case kDivu: return r[insn.r1] == 0;
        default: break;
    }
    return insn.id >= kBitStringBase && insn.id != kXb && insn.id != kXh && insn.id != kMpyhw;   // (the bit strings and the floating point; rev)
}

}  // namespace

v8::RunResult run(v8::Bus& bus, std::uint32_t entry, const v8::RunOptions& options) {
    sl::Emulator& emu = emulator_named("v810.sla");
    const std::size_t log_start = bus.log().size();
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        value = bus.read(static_cast<std::uint32_t>(addr), size);
        return true;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!is_ram(space)) return false;
        bus.write(static_cast<std::uint32_t>(addr), size, static_cast<std::uint32_t>(value));
        return true;
    });
    emu.onUserOp([](sl::Emulator&, const std::string&, const std::vector<std::uint64_t>&, std::uint64_t&) { return true; });
    struct Unhook {
        sl::Emulator& e;
        ~Unhook() {
            e.onRead({});
            e.onWrite({});
            e.onUserOp({});
        }
    } unhook{emu};

    const auto& r = reg_refs(emu, kRegisters);
    const auto& flags = reg_refs(emu, kFlagNames);
    const auto& system = reg_refs(emu, kSystem);
    const auto status_word = [&]() {
        std::uint32_t w = static_cast<std::uint32_t>(emu.reg(system[7])) << 16;
        for (unsigned i = 0; i < 14; ++i) w |= static_cast<std::uint32_t>(emu.reg(flags[i]) & 1) << kFlagBits[i];
        return w;
    };
    const auto set_status_word = [&](std::uint32_t w) {
        for (unsigned i = 0; i < 14; ++i) emu.setReg(flags[i], (w >> kFlagBits[i]) & 1);
        emu.setReg(system[7], (w >> 16) & 15);
    };

    for (const sl::Emulator::RegRef& ref : r) emu.setReg(ref, 0);
    for (unsigned i = 1; i < 32; ++i) emu.setReg(r[i], options.r[i]);
    emu.setReg(r[3], options.sp);
    emu.setReg(r[31], v8::kReturnAddress);
    set_status_word(0);
    for (unsigned i = 0; i < 7; ++i) emu.setReg(system[i], 0);
    emu.setPc(entry);

    const auto registers = [&]() {
        v8::Registers g;
        for (unsigned i = 0; i < 32; ++i) g.r[i] = static_cast<std::uint32_t>(emu.reg(r[i]));
        g.pc = static_cast<std::uint32_t>(emu.pc());
        g.psw = status_word();
        g.eipc = static_cast<std::uint32_t>(emu.reg(system[0]));
        g.eipsw = static_cast<std::uint32_t>(emu.reg(system[1]));
        g.fepc = static_cast<std::uint32_t>(emu.reg(system[2]));
        g.fepsw = static_cast<std::uint32_t>(emu.reg(system[3]));
        g.ecr = static_cast<std::uint32_t>(emu.reg(system[4]));
        g.chcw = static_cast<std::uint32_t>(emu.reg(system[5]));
        g.adtre = static_cast<std::uint32_t>(emu.reg(system[6]));
        return g;
    };
    // The own core, for what it runs: the registers go to it as they stand and come back.
    v8::Cpu scratch(bus);
    std::uint64_t steps_made = 0;   // what the last own_step counted
    const auto own_step = [&]() -> std::optional<v8::Fault> {
        scratch.regs = registers();
        const std::uint64_t before = scratch.steps;
        const std::optional<v8::Fault> fault = scratch.step();
        const v8::Registers& g = scratch.regs;
        for (unsigned i = 0; i < 32; ++i) emu.setReg(r[i], g.r[i]);
        emu.setPc(g.pc);
        set_status_word(g.psw);
        emu.setReg(system[0], g.eipc);
        emu.setReg(system[1], g.eipsw);
        emu.setReg(system[2], g.fepc);
        emu.setReg(system[3], g.fepsw);
        emu.setReg(system[4], g.ecr);
        emu.setReg(system[5], g.chcw);
        emu.setReg(system[6], g.adtre);
        steps_made = scratch.steps - before;
        return fault;
    };

    v8::RunResult out;
    out.end = v8::End::Limit;
    std::uint64_t steps = 0;
    std::array<std::uint32_t, 32> values{};
    while (steps < options.max_steps) {
        const std::uint32_t pc = static_cast<std::uint32_t>(emu.pc());
        if (pc == v8::kReturnAddress) {
            out.end = v8::End::Return;
            break;
        }
        if (options.on_instruction) options.on_instruction(registers());
        // the instruction, read without the bus's log: a word, and a second when the first says it has one
        std::array<std::uint8_t, 4> code{};
        std::size_t length = 2;   // (the instruction's bytes)
        bool quiet = (pc & 1) == 0;
        std::optional<dv::Insn> insn;
        if (quiet) {
            const std::optional<std::uint32_t> first = bus.peek(pc, 2);
            if (first) {
                code[0] = static_cast<std::uint8_t>(*first);
                code[1] = static_cast<std::uint8_t>(*first >> 8);
                if ((*first >> 13) != 4 && (*first >> 10) >= 0x28) {
                    const std::optional<std::uint32_t> second = bus.peek(pc + 2, 2);
                    if (second) {
                        code[2] = static_cast<std::uint8_t>(*second);
                        code[3] = static_cast<std::uint8_t>(*second >> 8);
                        length = 4;
                    } else {
                        quiet = false;
                    }
                }
            } else {
                quiet = false;
            }
            if (quiet) insn = dv::decode(ByteView(code.data(), length), 0, pc);
        }
        bool own = !quiet || !insn;
        if (!own) {
            for (unsigned i = 0; i < 32; ++i) values[i] = static_cast<std::uint32_t>(emu.reg(r[i]));
            own = own_core_runs(*insn, values);
        }
        if (own) {
            const std::optional<v8::Fault> fault = own_step();
            steps += steps_made;
            if (fault) {
                out.end = v8::End::Stopped;
                out.fault = fault;
                break;
            }
            continue;
        }
        emu.write(pc, code.data(), length);
        const sl::Emulator::Status status = emu.step();
        if (status != sl::Emulator::Status::Ok) {   // (the description has no such instruction: the own core says what it is)
            const std::optional<v8::Fault> fault = own_step();
            steps += steps_made;
            if (fault) {
                out.end = v8::End::Stopped;
                out.fault = fault;
                break;
            }
            continue;
        }
        emu.setReg(r[0], 0);
        if (insn->id == dv::id::kJmp) emu.setPc(emu.pc() & ~std::uint64_t{1});
        ++steps;
    }
    if (out.end == v8::End::Limit && static_cast<std::uint32_t>(emu.pc()) == v8::kReturnAddress) out.end = v8::End::Return;
    out.regs = registers();
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

v8::RunResult call(v8::Bus& bus, std::uint32_t entry, const v8::RunOptions& options) { return enabled() ? pcode_cpu::run(bus, entry, options) : v8::run(bus, entry, options); }

}  // namespace cb::engine::pcode_cpu
