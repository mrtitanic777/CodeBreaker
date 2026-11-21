// engine/pcode_cpu_mips.cpp -- the PlayStation lane's R3000A on the one p-code emulator: emulate::mips::run's call, on languages/mips32le.sla.
//
// What the description leaves to the machine is carried here, one piece at a time: the BIOS calls (a jump to $A0, $B0 or $C0 is logged with its arguments and
// answered with 0; syscall with $a0 of 1 or 2 is Enter/ExitCriticalSection), the load delay slot (a loaded register gets its value after the next instruction),
// the faults (an unaligned or unmapped fetch, load or store, ADD/ADDI/SUB overflow, break, syscall, and any word the R3000A has no instruction for), and the
// memory: the RAM's three views (KUSEG, KSEG0, KSEG1) are mirrored for instruction fetch, every data access goes through the Bus. The GTE and coprocessor 0
// are NOT carried yet: those instructions end the run as "not run", which the own core does only for the GTE commands it has no code for.
#include <algorithm>
#include <array>
#include <cstring>
#include <string>
#include <vector>

#include "decompile/engine/pcode_emulator.hh"
#include "decompile/mips.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_internal.hpp"

namespace cb::engine::pcode_cpu {

namespace mp = cb::emulate::mips;
namespace sl = cb::decompile::sleigh;

namespace {

constexpr std::uint32_t kBlock = 0x1000;
constexpr std::uint32_t kViews[3] = {0x00000000u, 0x80000000u, 0xA0000000u};
constexpr const char* kNames[32] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
                                    "s0",   "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "s8", "ra"};

struct Mirror {
    std::vector<std::uint8_t> seen;   // the RAM as the emulator's pages hold it
};

Mirror& mirror() {
    thread_local Mirror m;
    return m;
}

std::int32_t sext16(std::uint32_t w) { return static_cast<std::int16_t>(w & 0xFFFF); }

// The register an instruction writes (-1: none), for the load delay slot.
int destination(std::uint32_t w) {
    const unsigned op = w >> 26, rt = (w >> 16) & 31, rd = (w >> 11) & 31;
    if (op == 0) {
        const unsigned f = w & 0x3F;
        if (f == 0x08 || f == 0x0C || f == 0x0D || f == 0x11 || f == 0x13 || (f >= 0x18 && f <= 0x1B)) return -1;   // jr syscall break mthi mtlo mult multu div divu
        return static_cast<int>(rd);
    }
    if (op == 0x03) return 31;
    if (op == 0x01) return (rt >= 16) ? 31 : -1;
    if (op >= 0x08 && op <= 0x0F) return static_cast<int>(rt);   // addi .. lui
    if (op >= 0x20 && op <= 0x26) return static_cast<int>(rt);   // loads
    if (op == 0x10 || op == 0x12) return ((w >> 21) & 31) == 0 || ((w >> 21) & 31) == 2 ? static_cast<int>(rt) : -1;
    return -1;
}

bool is_branch(std::uint32_t w) {
    const unsigned op = w >> 26;
    if (op == 0) return (w & 0x3F) == 0x08 || (w & 0x3F) == 0x09;
    return op == 0x01 || (op >= 0x02 && op <= 0x07);
}

}  // namespace

mp::RunResult run(mp::Bus& bus, std::uint32_t entry, const mp::RunOptions& options) {
    sl::Emulator& emu = emulator_named("mips32le.sla");
    const std::size_t log_start = bus.log().size();
    const std::span<std::uint8_t> memory = bus.memory();
    Mirror& mir = mirror();
    if (mir.seen.size() != memory.size()) {
        emu.clearMemory();
        mir.seen.assign(memory.size(), 0);
    }
    for (std::size_t at = 0; at < memory.size(); at += kBlock) {
        const std::size_t n = std::min<std::size_t>(kBlock, memory.size() - at);
        if (std::memcmp(mir.seen.data() + at, memory.data() + at, n) == 0) continue;
        std::memcpy(mir.seen.data() + at, memory.data() + at, n);
        for (const std::uint32_t base : kViews) emu.write(base + at, memory.data() + at, n);
    }
    emu.clearStops();

    std::optional<mp::Fault::Kind> raised;
    std::string raised_text;
    std::vector<mp::BiosCall> bios;
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        value = bus.read(static_cast<std::uint32_t>(addr), size);
        return true;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!is_ram(space)) return false;
        const std::uint32_t a = static_cast<std::uint32_t>(addr);
        bus.write(a, size, static_cast<std::uint32_t>(value));
        const std::uint32_t phys = a & 0x1FFFFFFFu;
        if (phys + size <= memory.size()) {   // (the program may run what it wrote: the instruction fetch sees it, as the own core's does)
            std::memcpy(mir.seen.data() + phys, memory.data() + phys, size);
            for (const std::uint32_t base : kViews) emu.write(base + phys, memory.data() + phys, size);
        }
        return true;
    });
    // The coprocessors the description leaves to the machine: the GTE's registers and commands are the own core's (emulate/mips.hpp's gte_*), coprocessor 0's are
    // the 32 words a program moves to and from.
    thread_local mp::Bus gte_bus;
    thread_local mp::Cpu gte(gte_bus);
    gte.gte_data = {};
    gte.gte_control = {};
    std::array<std::uint32_t, 32> cop0{};
    emu.onUserOp([&](sl::Emulator&, const std::string& name, const std::vector<std::uint64_t>& args, std::uint64_t& result) {
        std::string lower = name;
        std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        // (MFC2, MTC2, CFC2 and CTC2 give the register as the instruction's 16-bit field, rd << 11; LWC2 and SWC2 as its number)
        const auto copreg = [&] { return args.size() < 2 ? 0u : static_cast<unsigned>(args[1] >= 32 ? (args[1] >> 11) & 31 : args[1]); };
        if (lower == "setisamode") return true;   // (the description marks each jump's ISA mode: there is one)
        if (lower.find("syscall") != std::string::npos) raised = mp::Fault::Kind::Syscall;
        else if (lower.find("break") != std::string::npos || lower == "trap") raised = mp::Fault::Kind::Break;
        else if (lower == "getcopreg") result = mp::gte_register(gte, copreg());
        else if (lower == "setcopreg" && args.size() >= 3) {
            mp::gte_set_register(gte, copreg(), static_cast<std::uint32_t>(args[2]));
        } else if (lower == "getcopcontrolword") result = mp::gte_control_register(gte, copreg());
        else if (lower == "setcopcontrolword" && args.size() >= 3) gte.gte_control[copreg()] = static_cast<std::uint32_t>(args[2]);
        else if (lower == "copfunction" && args.size() >= 2) {
            const std::uint32_t command_word = (0x12u << 26) | (1u << 25) | static_cast<std::uint32_t>(args[1] & 0x1FFFFFFu);
            const std::optional<decompile::mips::Insn> cmd = decompile::mips::decode_word(command_word, 0);
            if (!cmd || cmd->form != decompile::mips::Form::Command || !mp::gte_run_command(gte, static_cast<std::uint32_t>(cmd->imm), cmd->mnemonic)) {
                raised = mp::Fault::Kind::NotRun;
                raised_text = cmd ? cmd->mnemonic : "cop2";
            }
        } else {
            raised = mp::Fault::Kind::NotRun;
            raised_text = name;
        }
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

    const auto reg = [&](unsigned i) { return static_cast<std::uint32_t>(emu.reg(kNames[i])); };
    const auto set_reg = [&](unsigned i, std::uint32_t v) {
        if (i != 0) emu.setReg(kNames[i], v);
    };
    for (unsigned i = 1; i < 32; ++i) emu.setReg(kNames[i], 0);
    for (unsigned i = 0; i < 4; ++i) set_reg(4 + i, options.a[i]);
    for (unsigned i = 0; i < 8; ++i) set_reg(16 + i, options.s[i]);
    for (unsigned i = 0; i < 8; ++i) set_reg(8 + i, options.t[i]);
    set_reg(28, options.gp);
    set_reg(29, options.sp);
    set_reg(31, mp::kReturnAddress);
    emu.setReg("ac0", 0);
    emu.setReg("zero", 0);
    emu.setPc(entry);

    mp::RunResult out;
    out.end = mp::End::Limit;
    std::uint64_t steps = 0;
    struct Pending {
        int reg = -1;
        std::uint32_t value = 0;
    } pending;   // the load whose value arrives after the next instruction
    const auto stop = [&](mp::Fault::Kind kind, std::uint32_t pc, std::uint32_t address) {
        out.end = mp::End::Stopped;
        out.fault = mp::Fault{kind, pc, address, {}, 0};
    };
    while (steps < options.max_steps) {
        const std::uint32_t pc = static_cast<std::uint32_t>(emu.pc());
        if (pc == mp::kReturnAddress) {
            out.end = mp::End::Return;
            break;
        }
        const std::uint32_t phys = pc & 0x1FFFFFFFu;
        if (phys == 0xA0 || phys == 0xB0 || phys == 0xC0) {   // a BIOS call: logged, answered with 0, and returned from
            mp::BiosCall call;
            call.table = static_cast<std::uint8_t>(phys);
            call.function = reg(9);
            for (unsigned i = 0; i < 4; ++i) call.args[i] = reg(4 + i);
            bios.push_back(call);
            set_reg(2, 0);
            if (pending.reg >= 0) {
                set_reg(static_cast<unsigned>(pending.reg), pending.value);
                pending.reg = -1;
            }
            emu.setPc(reg(31));
            ++steps;
            continue;
        }
        if ((pc & 3) != 0) {
            stop(mp::Fault::Kind::AddressError, pc, pc);
            break;
        }
        if (!bus.mapped(pc)) {
            stop(mp::Fault::Kind::BusError, pc, pc);
            break;
        }
        if (phys >= memory.size() || (pc >> 29) == 7) {   // (no code of ours: the bus reads zeros there, which is a NOP)
            emu.setPc(pc + 4);
            ++steps;
            continue;
        }
        const std::uint32_t word = static_cast<std::uint32_t>(memory[phys]) | (static_cast<std::uint32_t>(memory[phys + 1]) << 8) |
                                   (static_cast<std::uint32_t>(memory[phys + 2]) << 16) | (static_cast<std::uint32_t>(memory[phys + 3]) << 24);
        const std::uint32_t slot_word = is_branch(word) && phys + 7 < memory.size()
                                            ? static_cast<std::uint32_t>(memory[phys + 4]) | (static_cast<std::uint32_t>(memory[phys + 5]) << 8) |
                                                  (static_cast<std::uint32_t>(memory[phys + 6]) << 16) | (static_cast<std::uint32_t>(memory[phys + 7]) << 24)
                                            : 0;
        // What the own core refuses or faults on, for the instruction and for the one in its delay slot (checked before either runs).
        const auto check = [&](std::uint32_t w, std::uint32_t at, bool in_slot) -> std::optional<mp::Fault> {
            const std::optional<decompile::mips::Insn> insn = decompile::mips::decode_word(w, at);
            if (!insn) return mp::Fault{mp::Fault::Kind::NoInstruction, at, 0, {}, 0};
            const unsigned op = w >> 26, rs = (w >> 21) & 31, rt = (w >> 16) & 31;
            const std::uint32_t simm = static_cast<std::uint32_t>(sext16(w));
            if ((op >= 0x20 && op <= 0x26) || (op >= 0x28 && op <= 0x2B) || op == 0x2E || op == 0x32 || op == 0x3A) {   // a load or a store
                const std::uint32_t address = reg(rs) + simm;
                const bool word_access = op == 0x23 || op == 0x2B || op == 0x32 || op == 0x3A;
                const bool half_access = op == 0x21 || op == 0x25 || op == 0x29;
                if ((word_access && (address & 3) != 0) || (half_access && (address & 1) != 0)) return mp::Fault{mp::Fault::Kind::AddressError, at, address, {}, 0};
                if (!bus.mapped(address)) return mp::Fault{mp::Fault::Kind::BusError, at, address, {}, 0};
            }
            // (coprocessor 0 is run here, not by the description, and not in a delay slot; the FPU and coprocessor 3 are not on this machine; the GTE's are the hooks')
            if ((op == 0x10 && in_slot) || op == 0x30 || op == 0x31 || op == 0x38 || op == 0x39 || op == 0x11 || op == 0x13)
                return mp::Fault{mp::Fault::Kind::NotRun, at, 0, insn->mnemonic, 0};
            if (op == 0x08 || (op == 0 && ((w & 0x3F) == 0x20 || (w & 0x3F) == 0x22))) {   // ADDI, ADD, SUB trap on overflow
                const std::int64_t a = static_cast<std::int32_t>(reg(rs));
                const std::int64_t b = op == 0x08 ? static_cast<std::int64_t>(static_cast<std::int32_t>(simm)) : static_cast<std::int64_t>(static_cast<std::int32_t>(reg(rt)));
                const std::int64_t s = (op == 0 && (w & 0x3F) == 0x22) ? a - b : a + b;
                if (s != static_cast<std::int32_t>(s)) return mp::Fault{mp::Fault::Kind::Overflow, at, 0, {}, 0};
            }
            return std::nullopt;
        };
        if (const std::optional<mp::Fault> fault = check(word, pc, false)) {
            out.end = mp::End::Stopped;
            out.fault = *fault;
            break;
        }
        const bool half = is_branch(word) && steps + 1 >= options.max_steps;   // (the step limit falls between the branch and its slot)
        int link_reg = -1;   // the register the branch links
        bool slot_done = false;   // the delay slot was an LWC2, already made
        if (is_branch(word)) {
            const unsigned bop = word >> 26;
            if (bop == 0x03) link_reg = 31;
            else if (bop == 0 && (word & 0x3F) == 0x09) link_reg = static_cast<int>((word >> 11) & 31);
            else if (bop == 0x01 && ((word >> 16) & 31) >= 16) link_reg = 31;
            if (link_reg == 0) link_reg = -1;
        }
        if (is_branch(word) && !half) {
            // the slot is checked as the own core meets it: after the branch has run (its link written)
            const std::uint32_t before = link_reg > 0 ? reg(static_cast<unsigned>(link_reg)) : 0;
            if (link_reg > 0) set_reg(static_cast<unsigned>(link_reg), pc + 8);
            const std::optional<mp::Fault> fault = check(slot_word, pc + 4, true);
            if (fault) {
                if (pending.reg >= 0 && pending.reg != link_reg) set_reg(static_cast<unsigned>(pending.reg), pending.value);   // (the branch finished: what was in flight arrived)
                pending.reg = -1;
                emu.setPc(pc + 4);
                out.end = mp::End::Stopped;
                out.fault = *fault;
                ++steps;
                break;
            }
            if ((slot_word >> 26) == 0x32) {   // LWC2 in the slot: made here (with the branch's link in place), the slot then runs as a NOP
                mp::gte_set_register(gte, (slot_word >> 16) & 31, bus.read(reg((slot_word >> 21) & 31) + static_cast<std::uint32_t>(sext16(slot_word)), 4));
                slot_done = true;
            }
            if (link_reg > 0) set_reg(static_cast<unsigned>(link_reg), before);
        } else if (is_branch(word)) {
            // (half: the slot is not run, so it is not checked)
        } else if ((word >> 26) == 0x10) {   // coprocessor 0: the 32 words a program moves to and from (its RFE shifts the status register's mode bits back)
            const unsigned rs = (word >> 21) & 31, rt = (word >> 16) & 31, rd = (word >> 11) & 31;
            if (rs == 0) {   // MFC0: a load
                if (pending.reg >= 0 && !(rt == static_cast<unsigned>(pending.reg))) set_reg(static_cast<unsigned>(pending.reg), pending.value);
                pending.reg = rt == 0 ? -1 : static_cast<int>(rt);
                pending.value = cop0[rd];
            } else if (rs == 4) {   // MTC0
                if (pending.reg >= 0) set_reg(static_cast<unsigned>(pending.reg), pending.value);
                pending.reg = -1;
                cop0[rd] = reg(rt);
            } else {   // RFE
                if (pending.reg >= 0) set_reg(static_cast<unsigned>(pending.reg), pending.value);
                pending.reg = -1;
                cop0[12] = (cop0[12] & ~0xFu) | ((cop0[12] >> 2) & 0xFu);
            }
            emu.setPc(pc + 4);
            ++steps;
            continue;
        }
        if ((cop0[12] & 0x10000u) != 0) {   // the cache is isolated: a load reads nothing and a store writes nothing
            const unsigned o = word >> 26;
            if ((o >= 0x20 && o <= 0x26) || (o >= 0x28 && o <= 0x2B) || o == 0x2E || o == 0x32 || o == 0x3A) {
                emu.setPc(pc + 4);
                ++steps;
                continue;
            }
        }
        if ((word >> 26) == 0x32) {   // LWC2: the description gives the coprocessor the register's contents, not its number, so the load is made here
            const std::uint32_t address = reg((word >> 21) & 31) + static_cast<std::uint32_t>(sext16(word));
            mp::gte_set_register(gte, (word >> 16) & 31, bus.read(address, 4));
            if (pending.reg >= 0) set_reg(static_cast<unsigned>(pending.reg), pending.value);
            pending.reg = -1;
            emu.setPc(pc + 4);
            ++steps;
            continue;
        }
        // LWL and LWR merge with a load still in flight to the same register
        if (((word >> 26) == 0x22 || (word >> 26) == 0x26) && pending.reg == static_cast<int>((word >> 16) & 31)) {
            set_reg((word >> 16) & 31, pending.value);
            pending.reg = -1;
        }
        // A load in flight arrives after the branch, before its delay slot: when the branch does not read that register it arrives before the pair runs
        if (is_branch(word) && !half && pending.reg >= 0 && static_cast<unsigned>(pending.reg) != ((word >> 21) & 31) && static_cast<unsigned>(pending.reg) != ((word >> 16) & 31)) {
            set_reg(static_cast<unsigned>(pending.reg), pending.value);
            pending.reg = -1;
        }
        const bool pair = is_branch(word) && !half;
        const std::uint32_t last = pair && !slot_done ? slot_word : word;
        const int last_destination = destination(last);
        // (a load in the slot of a linking branch: the register the branch linked is what it holds until the load arrives)
        std::uint32_t old_value = last_destination > 0 ? reg(static_cast<unsigned>(last_destination)) : 0;
        if (pair && last_destination > 0 && last_destination == link_reg) old_value = pc + 8;
        std::uint8_t nop_saved[4] = {};
        if (half || slot_done) {   // the step limit falls between the branch and its slot, or the slot is done: the branch alone runs (the slot is made a NOP for the step)
            const std::uint8_t zero4[4] = {};
            for (unsigned i = 0; i < 4; ++i) nop_saved[i] = memory[phys + 4 + i];
            for (const std::uint32_t base : kViews) emu.write(base + phys + 4, zero4, 4);
        }
        raised.reset();
        const sl::Emulator::Status status = emu.step();
        if (half || slot_done)
            for (const std::uint32_t base : kViews) emu.write(base + phys + 4, nop_saved, 4);
        if (status == sl::Emulator::Status::BadInstruction) {
            stop(mp::Fault::Kind::NoInstruction, pc, 0);
            break;
        }
        if (status == sl::Emulator::Status::Error) {
            stop(mp::Fault::Kind::NoInstruction, pc, 0);
            break;
        }
        if (raised) {
            if (*raised == mp::Fault::Kind::Syscall && (reg(4) == 1 || reg(4) == 2)) {   // EnterCriticalSection, ExitCriticalSection
                mp::BiosCall call;
                call.table = 0;
                call.function = reg(4);
                bios.push_back(call);
                set_reg(2, reg(4) == 1 ? 1 : 0);
            } else {
                out.end = mp::End::Stopped;
                const std::uint32_t faulting = pair ? slot_word : word;   // (a fault in a delay slot: the branch counts as run, the slot is where it stopped)
                out.fault = mp::Fault{*raised, pair ? pc + 4 : pc, 0, *raised == mp::Fault::Kind::NotRun ? raised_text : std::string{}, (faulting >> 6) & 0xFFFFFu};
                if (pair) ++steps;
                if (*raised == mp::Fault::Kind::Syscall || *raised == mp::Fault::Kind::Break) {   // (these finish: a load in flight arrives)
                    ++steps;
                    if (pending.reg >= 0) set_reg(static_cast<unsigned>(pending.reg), pending.value);
                }
                break;
            }
        }
        // the load delay: what the instruction before delivered arrives now (unless this instruction wrote that register); what this one loads arrives after the next
        if (pending.reg >= 0 && !(last_destination == pending.reg || destination(word) == pending.reg)) set_reg(static_cast<unsigned>(pending.reg), pending.value);
        pending.reg = -1;
        const unsigned last_op = last >> 26;
        if (last_destination > 0 && ((last_op >= 0x20 && last_op <= 0x26) || (last_op == 0x12 && (((last >> 21) & 31) == 0 || ((last >> 21) & 31) == 2)))) {
            pending.reg = last_destination;
            pending.value = reg(static_cast<unsigned>(last_destination));
            set_reg(static_cast<unsigned>(last_destination), old_value);
        }
        emu.setReg("zero", 0);   // (a write to register 0 goes nowhere)
        steps += pair ? 2 : 1;   // (the own core counts a branch and its delay slot as two instructions)
    }
    // (a load still in flight when the run ends is not delivered: the own core's registers are as that instruction left them)
    for (unsigned i = 0; i < 32; ++i) out.r[i] = reg(i);
    const std::uint64_t ac = emu.reg("ac0");
    out.hi = static_cast<std::uint32_t>(ac >> 32);
    out.lo = static_cast<std::uint32_t>(ac);
    out.gte_data = gte.gte_data;
    out.gte_control = gte.gte_control;
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    out.bios = std::move(bios);
    return out;
}

mp::RunResult call(mp::Bus& bus, std::uint32_t entry, const mp::RunOptions& options) { return enabled(Core::Mips) ? pcode_cpu::run(bus, entry, options) : mp::run(bus, entry, options); }

}  // namespace cb::engine::pcode_cpu
