// engine/pcode_cpu_arm.cpp -- the Game Boy Advance lane's ARM7TDMI on the one p-code emulator: emulate::arm::run's call, on languages/ARM4t_le.sla.
//
// Carried here: the state (ARM or Thumb: the description's TMode context follows BX, loads into the pc and BLX), the software interrupts (logged with their
// number and r0-r3, then answered as the BIOS does by the own core's own answer), bus errors (a fetch, load or store of an address nothing answers), and the
// registers and flags as the own core hands them back. Other machines (the DS, the 3DS, the Vita) and the instructions the description gives no meaning
// to run the own core.
#include <algorithm>
#include <array>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

#include "decompile/arm.hpp"
#include "decompile/engine/pcode_emulator.hh"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_internal.hpp"

namespace cb::engine::pcode_cpu {

namespace ar = cb::emulate::arm;
namespace sl = cb::decompile::sleigh;
namespace da = cb::decompile::arm;

namespace {

constexpr std::uint32_t kArmPage = 0x1000;
constexpr const char* kArmNames[15] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "sp", "lr"};

struct ArmCodePage {
    std::vector<std::uint8_t> bytes = std::vector<std::uint8_t>(kArmPage, 0);
    std::uint32_t run = 0;
};

struct ArmMirror {
    std::unordered_map<std::uint32_t, ArmCodePage> pages;
    std::uint32_t run = 0;
};

ArmMirror& arm_mirror() {
    thread_local ArmMirror m;
    return m;
}

bool passes(unsigned cond, bool n, bool z, bool c, bool v) {
    switch (cond) {
        case 0: return z;
        case 1: return !z;
        case 2: return c;
        case 3: return !c;
        case 4: return n;
        case 5: return !n;
        case 6: return v;
        case 7: return !v;
        case 8: return c && !z;
        case 9: return !c || z;
        case 10: return n == v;
        case 11: return n != v;
        case 12: return !z && n == v;
        case 13: return z || n != v;
        default: return true;
    }
}

}  // namespace

ar::RunResult run(ar::Bus& bus, std::uint32_t entry, const ar::RunOptions& options) {
    sl::Emulator& emu = emulator_named("ARM4t_le.sla");
    const std::size_t log_start = bus.log().size();
    ArmMirror& mir = arm_mirror();
    ++mir.run;

    // A page of code is read from the bus the first time an instruction is fetched from it in a run (and handed to the emulator when it differs from what the
    // emulator holds); what the program writes to a page already read is written to the emulator too.
    std::uint32_t last_loaded = 1;   // (the page last looked at: an address that is no page's)
    const auto load_page = [&](std::uint32_t address) {
        const std::uint32_t base = address & ~(kArmPage - 1);
        if (base == last_loaded) return;
        ArmCodePage& page = mir.pages[base];
        last_loaded = base;
        if (page.run == mir.run) return;
        page.run = mir.run;
        if (const std::uint8_t* plain = bus.plain_block(base, kArmPage)) {   // (memory read straight, not a word at a time)
            if (std::memcmp(page.bytes.data(), plain, kArmPage) != 0) {
                std::memcpy(page.bytes.data(), plain, kArmPage);
                emu.write(base, plain, kArmPage);
            }
            return;
        }
        std::uint8_t now[kArmPage];
        for (std::uint32_t i = 0; i < kArmPage; i += 4) {
            const std::uint32_t at = base + i;
            const std::uint32_t w = bus.mapped(at) ? bus.read(at, 4) : 0;
            for (unsigned b = 0; b < 4; ++b) now[i + b] = static_cast<std::uint8_t>(w >> (8 * b));
        }
        if (std::memcmp(page.bytes.data(), now, kArmPage) != 0) {
            std::memcpy(page.bytes.data(), now, kArmPage);
            emu.write(base, now, kArmPage);
        }
    };

    std::optional<std::uint32_t> bus_error, odd_half, odd_byte;
    bool signed_half = false;   // the instruction about to run is an LDRSH
    bool block_transfer = false;   // ... an LDM or STM
    std::optional<std::uint32_t> fault_pc;
    std::optional<std::string> not_run;
    std::optional<bool> mode_switch;
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        const std::uint32_t raw = static_cast<std::uint32_t>(addr);
        const bool byte_load = size == 2 && (raw & 1) != 0 && signed_half;   // (a signed halfword load from an odd address is a signed byte load)
        const std::uint32_t a = byte_load ? raw : raw & ~(size - 1);   // (the own core's accesses are aligned)
        if (!bus.mapped(a)) {
            if (!bus_error) bus_error = block_transfer ? a : raw;   // (a single transfer reports the address as the program gave it, a block transfer the word's)
            value = 0;
            return true;
        }
        value = bus.read(a, byte_load ? 1 : size);
        if (size == 4 && (raw & 3) != 0) {   // (a word load from an unaligned address rotates the aligned word: the description does not)
            const unsigned rotate = 8 * (raw & 3);
            value = ((value >> rotate) | (value << (32 - rotate))) & 0xFFFFFFFFu;
        }
        if (size == 2 && (raw & 1) != 0) {   // (a halfword load from an odd address gives the halfword rotated by 8, a signed one the byte sign-extended: 32 bits the
                                              // description's 16-bit read cannot hold, so the destination register is set after the instruction)
            const std::uint32_t half = static_cast<std::uint32_t>(value);
            odd_half = (half >> 8) | (half << 24);
            odd_byte = bus.read(raw, 1);
        }
        return true;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!is_ram(space)) return false;
        const std::uint32_t a = static_cast<std::uint32_t>(addr) & ~(size - 1);   // (a store goes to the aligned address)
        if (!bus.mapped(a)) {
            if (!bus_error) bus_error = block_transfer ? a : static_cast<std::uint32_t>(addr);
            return true;
        }
        bus.write(a, size, static_cast<std::uint32_t>(value));
        const auto page = mir.pages.find(a & ~(kArmPage - 1));
        if (page != mir.pages.end() && page->second.run == mir.run) {   // (the program may run what it wrote)
            for (std::uint32_t w = a & ~3u; w < a + size; w += 4) {
                const std::uint32_t word = bus.mapped(w) ? bus.read(w, 4) : 0;
                std::uint8_t bytes[4];
                for (unsigned b = 0; b < 4; ++b) bytes[b] = static_cast<std::uint8_t>(word >> (8 * b));
                std::memcpy(page->second.bytes.data() + (w & (kArmPage - 1)), bytes, 4);
                emu.write(w, bytes, 4);
            }
        }
        return true;
    });
    emu.onUserOp([&](sl::Emulator&, const std::string& name, const std::vector<std::uint64_t>& args, std::uint64_t&) {
        if (name == "SetThumbMode") {
            if (!mode_switch) mode_switch = !args.empty() && (args[0] & 1) != 0;   // (the first: the description sets it back to ARM after a call for the decompiler's sake)
        }
        else if (name == "software_interrupt") return true;
        else if (name.rfind("coprocessor", 0) == 0 || name.rfind("software", 0) == 0) not_run = name;
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

    bool thumb = (entry & 1) != 0;
    emu.setContext("TMode", thumb ? 1 : 0);
    for (unsigned i = 0; i < 15; ++i) emu.setReg(kArmNames[i], 0);
    for (unsigned i = 0; i < 4; ++i) emu.setReg(kArmNames[i], options.a[i]);
    for (unsigned i = 0; i < 8; ++i) emu.setReg(kArmNames[4 + i], options.s[i]);
    emu.setReg("sp", options.sp);
    emu.setReg("lr", options.return_address);
    for (const char* flag : {"NG", "ZR", "CY", "OV", "Q"}) emu.setReg(flag, 0);
    emu.setReg("TB", thumb ? 1 : 0);
    emu.setReg("cpsr", 0x1F);
    emu.setReg("spsr", 0);
    // The banked registers as the own core keeps them: a mode change (an MSR to the control bits) swaps r13, r14 and the saved status register of the mode left and
    // the mode entered, and r8-r12 for FIQ.
    struct Bank {
        std::uint32_t sp = 0, lr = 0, spsr = 0;
    };
    std::array<Bank, 6> banks{};
    std::array<std::uint32_t, 5> fiq_high{}, user_high{};
    unsigned mode = 0x1F;
    const auto bank_of = [](unsigned m) -> unsigned {
        switch (m & 0x1F) {
            case 0x11: return 1;
            case 0x12: return 2;
            case 0x13: return 3;
            case 0x17: return 4;
            case 0x1B: return 5;
            default: return 0;
        }
    };
    const auto change_mode = [&](unsigned to_mode) {
        const unsigned from = bank_of(mode), to = bank_of(to_mode);
        if (from != to) {
            banks[from].sp = static_cast<std::uint32_t>(emu.reg("sp"));
            banks[from].lr = static_cast<std::uint32_t>(emu.reg("lr"));
            banks[from].spsr = static_cast<std::uint32_t>(emu.reg("spsr"));
            const auto high = [&](std::array<std::uint32_t, 5>& into) {
                for (unsigned i = 0; i < 5; ++i) into[i] = static_cast<std::uint32_t>(emu.reg(kArmNames[8 + i]));
            };
            const auto load_high = [&](const std::array<std::uint32_t, 5>& from_regs) {
                for (unsigned i = 0; i < 5; ++i) emu.setReg(kArmNames[8 + i], from_regs[i]);
            };
            if (from == 1) {
                high(fiq_high);
                load_high(user_high);
            }
            if (to == 1) {
                high(user_high);
                load_high(fiq_high);
            }
            emu.setReg("sp", banks[to].sp);
            emu.setReg("lr", banks[to].lr);
            emu.setReg("spsr", banks[to].spsr);
        }
        mode = to_mode & 0x1F;
    };
    emu.setPc(entry & ~1u);

    ar::Cpu scratch(bus);   // (the BIOS's answers read and write a Cpu's registers)
    ar::RunResult out;
    out.end = ar::End::Limit;
    std::uint64_t steps = 0;
    const auto flags = [&](bool& n, bool& z, bool& c, bool& v) {
        n = emu.reg("NG") != 0;
        z = emu.reg("ZR") != 0;
        c = emu.reg("CY") != 0;
        v = emu.reg("OV") != 0;
    };
    const auto stop = [&](ar::Fault::Kind kind, std::uint32_t pc, std::uint32_t address) {
        out.end = ar::End::Stopped;
        out.fault = ar::Fault{kind, pc, address, {}, 0};
    };
    while (steps < options.max_steps) {
        const std::uint32_t pc = static_cast<std::uint32_t>(emu.pc());
        if (pc == options.return_address) {
            out.end = ar::End::Return;
            break;
        }
        if (!bus.mapped(pc)) {
            stop(ar::Fault::Kind::BusError, pc, pc);
            break;
        }
        load_page(pc);
        load_page(pc + 3);
        std::optional<da::Insn> insn;
        if (thumb) {
            const std::uint32_t a = bus.read(pc & ~1u, 2);
            const std::uint32_t b = (a >> 11) == 0x1E ? bus.read((pc & ~1u) + 2, 2) : 0;
            insn = da::decode_thumb(static_cast<std::uint16_t>(a), static_cast<std::uint16_t>(b), pc | 1, bus.arch());
        } else {
            insn = da::decode_arm(bus.read(pc & ~3u, 4), pc, bus.arch());
        }
        if (!insn) {
            stop(ar::Fault::Kind::NoInstruction, pc, 0);
            break;
        }
        bool n = false, z = false, c = false, v = false;
        if (insn->op == da::Op::Swi || insn->op == da::Op::Bl) flags(n, z, c, v);
        if (insn->op == da::Op::Swi && passes(insn->cond, n, z, c, v)) {   // a BIOS call: logged and answered as the own core does
            ar::BiosCall call;
            call.function = thumb ? insn->imm & 0xFF : (insn->imm >> 16) & 0xFF;
            for (unsigned i = 0; i < 4; ++i) call.args[i] = static_cast<std::uint32_t>(emu.reg(kArmNames[i]));
            out.bios.push_back(call);
            if (std::find(options.ending.begin(), options.ending.end(), call.function) != options.ending.end()) {
                out.end = ar::End::Stopped;
                out.fault = ar::Fault{ar::Fault::Kind::NotRun, pc, 0, "svc " + std::to_string(call.function), call.function};
                break;
            }
            for (unsigned i = 0; i < 4; ++i) scratch.r[i] = call.args[i];
            ar::bios_answer(scratch, call.function);
            for (unsigned i = 0; i < 4; ++i) emu.setReg(kArmNames[i], scratch.r[i]);
            emu.setPc(pc + insn->length);
            ++steps;
            continue;
        }
        bus_error.reset();
        odd_half.reset();
        odd_byte.reset();
        signed_half = insn->size == 2 && insn->sign;
        const bool memory_op = insn->op == da::Op::Ldr || insn->op == da::Op::Str || insn->op == da::Op::Ldm || insn->op == da::Op::Stm || insn->op == da::Op::Swp;
        std::array<std::uint32_t, 15> before_regs{};
        unsigned touched = 0;   // the registers the transfer can change
        if (memory_op) {   // (an access that fails leaves the registers as they were: the description has written back and loaded zeros by then)
            if (insn->op == da::Op::Ldm || insn->op == da::Op::Stm) touched = static_cast<unsigned>(insn->list) & 0x7FFF;
            else if (insn->rd < 15) touched = 3u << insn->rd;   // (LDRD: the pair)
            if (insn->rn < 15) touched |= 1u << insn->rn;
            touched &= 0x7FFF;
            for (unsigned i = 0; i < 15; ++i)
                if ((touched >> i) & 1) before_regs[i] = static_cast<std::uint32_t>(emu.reg(kArmNames[i]));
        }
        block_transfer = insn->op == da::Op::Ldm || insn->op == da::Op::Stm;
        not_run.reset();
        mode_switch.reset();
        const sl::Emulator::Status status = emu.step();
        if (status != sl::Emulator::Status::Ok) {
            if (status == sl::Emulator::Status::Error) {   // (the description's p-code failed: not an instruction the own core refuses)
                out.end = ar::End::Stopped;
                out.fault = ar::Fault{ar::Fault::Kind::NotRun, pc, 0, emu.lastError(), 0};
            } else {
                stop(ar::Fault::Kind::NoInstruction, pc, 0);
            }
            break;
        }
        if (bus_error) {
            for (unsigned i = 0; i < 15; ++i)
                if ((touched >> i) & 1) emu.setReg(kArmNames[i], before_regs[i]);
            out.end = ar::End::Stopped;
            out.fault = ar::Fault{ar::Fault::Kind::BusError, pc, *bus_error, {}, 0};
            fault_pc = pc;   // (the pc stays at the instruction that failed)
            break;
        }
        if (not_run) {
            out.end = ar::End::Stopped;
            out.fault = ar::Fault{ar::Fault::Kind::NotRun, pc, 0, *not_run, 0};
            break;
        }
        if (odd_half && insn->size == 2 && insn->rd < 15) {   // LDRH / LDRSH from an odd address
            const std::uint32_t result = insn->sign ? static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(*odd_byte))) : *odd_half;
            emu.setReg(kArmNames[insn->rd], result);
        }
        // the state after the instruction: BX and loads into the pc say it; a BLX by an immediate changes it
        bool next_thumb = thumb;
        if (insn->op == da::Op::Bx) next_thumb = emu.reg("TB") != 0;   // (on the ARMv4T only BX changes the state, and the description records it in TB: MOV and LDR to the pc ignore bit 0)
        else if (insn->op == da::Op::Bl && insn->target && passes(insn->cond, n, z, c, v) && insn->length == 4 && !thumb && (insn->word >> 28) == 0xF) next_thumb = true;
        else if (thumb && insn->word >> 11 == 0x1D) next_thumb = false;   // BLX (immediate) in Thumb
        if (next_thumb != thumb) {
            thumb = next_thumb;
            emu.setContext("TMode", thumb ? 1 : 0);
        }
        if ((static_cast<unsigned>(emu.reg("cpsr")) & 0x1F) != mode) change_mode(static_cast<unsigned>(emu.reg("cpsr")));
        if (const std::uint32_t next_pc = static_cast<std::uint32_t>(emu.pc()); next_pc != (next_pc & (thumb ? ~1u : ~3u))) emu.setPc(next_pc & (thumb ? ~1u : ~3u));   // (a pc written by a load keeps no state bit)
        ++steps;
    }
    for (unsigned i = 0; i < 15; ++i) out.r[i] = static_cast<std::uint32_t>(emu.reg(kArmNames[i]));
    out.r[15] = fault_pc ? *fault_pc : static_cast<std::uint32_t>(emu.pc());
    bool n, z, c, v;
    flags(n, z, c, v);
    out.cpsr = (static_cast<unsigned>(emu.reg("cpsr")) & 0xDF) | (thumb ? 0x20u : 0u) | (n ? 0x80000000u : 0u) | (z ? 0x40000000u : 0u) | (c ? 0x20000000u : 0u) | (v ? 0x10000000u : 0u) | (emu.reg("Q") != 0 ? 0x08000000u : 0u);
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

ar::RunResult call(ar::Bus& bus, std::uint32_t entry, const ar::RunOptions& options) {
    return enabled(Core::Arm) && bus.system() == ar::System::Gba && options.imports.empty() ? pcode_cpu::run(bus, entry, options) : ar::run(bus, entry, options);
}

}  // namespace cb::engine::pcode_cpu
