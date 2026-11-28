// cb-unity: alone (file-local names that other files in a batch also use)
// emulate/cpu/pcode_cpu_v810.cpp -- the Virtual Boy lane's NEC V810 on the one p-code emulator, on languages/v810exact.sla.
//
// The description runs every instruction (v810exact.sla: v810.slaspec with EXACT, the floating point's flags, the bit strings, the system registers as the processor
// keeps them). The adapter carries what it leaves to the machine: the instruction is fetched through the bus (a range the bus does not log is peeked, a fetch from a
// device or a logged range is made through the bus, which logs it as the processor's own would), r0 stays zero, and what ends a run: trap, reti, halt, a division by
// zero and a bit string longer than a megabit (the description hands them over as user operations, with nothing changed), and a pattern that is no instruction.
#include <array>
#include <cstring>
#include <format>
#include <optional>
#include <string>

#include "decompile/engine/pcode_emulator.hh"
#include "emulate/cpu/pcode_cpu_internal.hpp"
#include "emulate/cpu/pcode_cpu_v810.hpp"

namespace cb::emulate::pcode_cpu {

namespace v8 = cb::emulate::v810;
namespace sl = cb::decompile::sleigh;

namespace {

constexpr const char* kRegisters[32] = {"r0",  "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",  "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15",
                                        "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23", "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31"};
// The program status word's flags in the description, each a register of its own, and the bit of the word it is.
constexpr const char* kFlagNames[14] = {"Z", "S", "OV", "CY", "FPR", "FUD", "FOV", "FZD", "FIV", "FRO", "ID", "AE", "EP", "NP"};
constexpr unsigned kFlagBits[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14, 15};
constexpr const char* kSystem[8] = {"eipc", "eipsw", "fepc", "fepsw", "ecr", "chcw", "adtre", "PSWI"};

}  // namespace

v8::RunResult run(v8::Bus& bus, std::uint32_t entry, const v8::RunOptions& options) {
    sl::Emulator& emu = emulator_named("v810exact.sla");
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
    // what ends a run, as the description hands it over (the instruction changed nothing)
    std::optional<std::string> stop;
    emu.onUserOp([&](sl::Emulator&, const std::string& name, const std::vector<std::uint64_t>& args, std::uint64_t&) {
        if (name == "trap") stop = std::format("trap {}", args.empty() ? 0 : args[0]);
        else if (name == "halt") stop = "halt";
        else if (name == "reti") stop = "return from interrupt";
        else if (name == "divideByZero") stop = "divide by zero";
        else if (name == "bitStringTooLong") stop = "bit string over 1 Mbit";
        else return false;
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
    v8::RunResult out;
    out.end = v8::End::Limit;
    std::uint64_t steps = 0;
    while (steps < options.max_steps) {
        const std::uint32_t pc = static_cast<std::uint32_t>(emu.pc());
        if (pc == v8::kReturnAddress) {
            out.end = v8::End::Return;
            break;
        }
        if (options.on_instruction) options.on_instruction(registers());
        // the instruction: a word, and a second when the first says it has one. A range the bus does not log is peeked; a fetch from a device or a logged range is made through the
        // bus, which logs it as it logs the processor's own (the first halfword, then the second)
        std::array<std::uint8_t, 4> code{};
        std::size_t length = 2;   // (the instruction's bytes)
        const auto fetch = [&](std::uint32_t address) -> std::uint32_t {
            const std::optional<std::uint32_t> seen = bus.peek(address, 2);
            return seen ? *seen : bus.read(address, 2);
        };
        const std::uint32_t first = fetch(pc);
        code[0] = static_cast<std::uint8_t>(first);
        code[1] = static_cast<std::uint8_t>(first >> 8);
        if ((first >> 13) != 4 && (first >> 10) >= 0x28) {
            const std::uint32_t second = fetch(pc + 2);
            code[2] = static_cast<std::uint8_t>(second);
            code[3] = static_cast<std::uint8_t>(second >> 8);
            length = 4;
        }
        emu.write(pc, code.data(), length);
        emu.setPc(pc);
        stop.reset();
        const sl::Emulator::Status status = emu.step();
        if (status != sl::Emulator::Status::Ok || stop) {
            emu.setPc(pc);
            out.end = v8::End::Stopped;
            out.fault = v8::Fault{pc, std::format("{} at {:08X}", stop ? *stop : status == sl::Emulator::Status::BadInstruction ? std::string("undefined instruction") : emu.lastError(), pc)};
            break;
        }
        emu.setReg(r[0], 0);
        ++steps;
    }
    if (out.end == v8::End::Limit && static_cast<std::uint32_t>(emu.pc()) == v8::kReturnAddress) out.end = v8::End::Return;
    out.regs = registers();
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

}  // namespace cb::emulate::pcode_cpu
