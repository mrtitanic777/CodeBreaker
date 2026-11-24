// engine/pcode_cpu_arm.cpp -- the ARM lanes' CPU on the one p-code emulator: emulate::arm::run's call, on languages/ARM4t_le.sla (the GBA, the DS's ARM7), ARM5t_le.sla (the DS's
// ARM9), ARM6_le.sla (the 3DS), ARM7_le.sla (the Vita) and ARM4t_be.sla (the 3DO: big-endian words).
//
// Carried here: the state (ARM or Thumb: the description's TMode context follows BX, loads into the pc and BLX), the software interrupts (logged with their
// number and r0-r3, then answered as the BIOS does by the own core's own answer), the imported functions of the Vita (logged by name, answered 0), bus errors (a fetch,
// load or store of an address nothing answers), IT blocks (kept here: an instruction in one is skipped or run under a context that stops the 16-bit data-processing
// instructions from setting the flags), and the registers and flags as the own core hands them back. What the own core refuses stays refused (a 32-bit Thumb instruction
// it only decodes). A few instructions run on the own core, counted in engine/pcode_cpu_arm.hpp: coprocessor 15, instructions fetched from hardware, and the
// instructions the description gives another result (the manual leaves them open) or does not read.
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/fs.hpp"
#include "decompile/arm.hpp"
#include "decompile/arm_ext.hpp"
#include "decompile/engine/pcode_emulator.hh"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_arm.hpp"
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
    bool whole = true;   // (read as one: no hardware over any of it)
};

struct ArmMirror {
    std::unordered_map<std::uint32_t, ArmCodePage> pages;
    std::uint32_t run = 0;
    bool configured = false;
};

// One mirror for each emulator (each description holds its own memory).
ArmMirror& arm_mirror(const void* emulator) {
    thread_local std::unordered_map<const void*, ArmMirror> mirrors;
    return mirrors[emulator];
}

// The description each architecture runs on: the ARM7TDMI (the GBA, the DS's ARM7) ARMv4T, the ARM946E-S (the DS's ARM9) ARMv5TE.
const char* arm_description(da::Arch arch, bool big_endian) {
    if (big_endian) return "ARM4t_be.sla";
    switch (arch) {
        case da::Arch::V5TE: return "ARM5t_le.sla";
        case da::Arch::V6K: return "ARM6_le.sla";
        case da::Arch::V7A: return "ARM7_le.sla";
        default: return "ARM4t_le.sla";
    }
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

ArmHandover& arm_handover() {
    thread_local ArmHandover counts;
    return counts;
}

ar::RunResult run(ar::Bus& bus, std::uint32_t entry, const ar::RunOptions& options) {
    const bool big_endian = bus.big_endian();   // (the 3DO: the ARM60 as big-endian words, on the ARMv4T description of that order)
    sl::Emulator& emu = emulator_named(arm_description(bus.arch(), big_endian));
    const bool v5 = bus.arch() >= da::Arch::V5TE;
    const std::size_t log_start = bus.log().size();
    ArmMirror& mir = arm_mirror(&emu);
    if (!mir.configured) {   // (the state is the run's own: nothing a decode set for another address is kept)
        emu.keepAddressContext(false);
        mir.configured = true;
    }
    ++mir.run;

    // A page of code is read from the bus the first time an instruction is fetched from it in a run (and handed to the emulator when it differs from what the
    // emulator holds); what the program writes to a page already read is written to the emulator too. Memory with hardware over it is not read (a read is an access: it is
    // logged and the device answers it): those words are left zero, and an instruction fetched from them is the own core's to run. The result says whether the page was
    // read whole (no hardware in it).
    std::uint32_t last_loaded = 1;   // (the page last looked at: an address that is no page's)
    bool last_whole = true;
    const auto load_page = [&](std::uint32_t address) {
        const std::uint32_t base = address & ~(kArmPage - 1);
        if (base == last_loaded) return last_whole;
        ArmCodePage& page = mir.pages[base];
        last_loaded = base;
        if (page.run == mir.run) return last_whole = page.whole;
        page.run = mir.run;
        std::uint8_t now[kArmPage];
        page.whole = true;
        if (const std::uint8_t* plain = bus.plain_block(base, kArmPage)) {   // (memory read straight, not a word at a time)
            if (big_endian) {   // (the bus holds each word reversed)
                for (std::uint32_t i = 0; i < kArmPage; i += 4)
                    for (unsigned b = 0; b < 4; ++b) now[i + b] = plain[i + 3 - b];
                plain = now;
            }
            if (std::memcmp(page.bytes.data(), plain, kArmPage) != 0) {
                std::memcpy(page.bytes.data(), plain, kArmPage);
                emu.write(base, plain, kArmPage);
            }
            return last_whole = true;
        }
        if (bus.copy_block(base, now, kArmPage)) {
            if (std::memcmp(page.bytes.data(), now, kArmPage) != 0) {
                std::memcpy(page.bytes.data(), now, kArmPage);
                emu.write(base, now, kArmPage);
            }
            return last_whole = true;
        }
        page.whole = false;
        for (std::uint32_t i = 0; i < kArmPage; i += 4) {
            std::uint8_t word[4];
            const std::uint8_t* p = bus.plain_block(base + i, 4);
            if (!p && bus.copy_block(base + i, word, 4)) p = word;
            if (!p && !big_endian) {   // (the handhelds: a word that is not plain memory is read as the machine answers it)
                const std::uint32_t w = bus.mapped(base + i) ? bus.read(base + i, 4) : 0;
                for (unsigned b = 0; b < 4; ++b) now[i + b] = static_cast<std::uint8_t>(w >> (8 * b));
                continue;
            }
            for (unsigned b = 0; b < 4; ++b) now[i + b] = p ? p[big_endian ? 3 - b : b] : 0;
        }
        if (std::memcmp(page.bytes.data(), now, kArmPage) != 0) {
            std::memcpy(page.bytes.data(), now, kArmPage);
            emu.write(base, now, kArmPage);
        }
        return last_whole = false;
    };
    // Is the word at an aligned address plain memory (not hardware)?
    const auto plain_word = [&](std::uint32_t at) {
        std::uint8_t word[4];
        return bus.plain_block(at, 4) != nullptr || bus.copy_block(at, word, 4);
    };

    std::optional<std::uint32_t> bus_error, odd_half, odd_byte;
    std::uint32_t current_word = 0;   // the instruction about to run
    bool signed_half = false;   // the instruction about to run is an LDRSH
    bool block_transfer = false;   // ... an LDM or STM
    bool dword_transfer = false;   // ... an LDRD or STRD
    bool vfp_transfer = false;     // ... a VFP load or store
    std::uint32_t block_start = 0; // an LDM or STM: the lowest word it transfers (the own core goes up from it, so its first failing access is the one nearest above)
    std::optional<std::uint32_t> fault_pc;
    std::optional<std::string> not_run;
    std::optional<bool> mode_switch;
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        const std::uint32_t raw = static_cast<std::uint32_t>(addr);
        if (size == 8) {   // (a VFP double: two words)
            const std::uint32_t first = vfp_transfer ? raw : raw & ~3u;   // (the own core's VFP transfers do not align)
            for (unsigned k = 0; k < 2; ++k)
                if (!bus.mapped(first + 4 * k)) {
                    if (!bus_error) bus_error = raw + 4 * k;   // (as the program gave it)
                    value = 0;
                    return true;
                }
            value = static_cast<std::uint64_t>(bus.read(first, 4)) | static_cast<std::uint64_t>(bus.read(first + 4, 4)) << 32;
            return true;
        }
        const bool byte_load = size == 2 && (raw & 1) != 0 && signed_half;   // (a signed halfword load from an odd address is a signed byte load)
        const std::uint32_t a = byte_load || vfp_transfer ? raw : raw & ~(size - 1);   // (the own core's accesses are aligned, the VFP's not)
        if (!bus.mapped(a)) {
            if (!bus_error || (block_transfer && a - block_start < *bus_error - block_start)) bus_error = block_transfer ? a : raw;   // (a single transfer reports the address as the program gave it, a block transfer the first word's that fails going up)
            value = 0;
            return true;
        }
        value = bus.read(a, byte_load ? 1 : size);
        if (size == 4 && (raw & 3) != 0 && !block_transfer && !dword_transfer && !vfp_transfer) {   // (a word load from an unaligned address rotates the aligned word: the description does not; LDM just aligns)
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
        if (size == 8) {   // (a VFP double: two words)
            const std::uint32_t first = vfp_transfer ? static_cast<std::uint32_t>(addr) : static_cast<std::uint32_t>(addr) & ~3u;
            for (unsigned k = 0; k < 2; ++k)
                if (!bus.mapped(first + 4 * k)) {
                    if (!bus_error) bus_error = static_cast<std::uint32_t>(addr) + 4 * k;
                    return true;
                }
            for (unsigned k = 0; k < 2; ++k) {
                const std::uint32_t at = first + 4 * k;
                const std::uint32_t word = static_cast<std::uint32_t>(value >> (32 * k));
                bus.write(at, 4, word);
                const auto page = mir.pages.find(at & ~(kArmPage - 1));
                if (page != mir.pages.end() && page->second.run == mir.run) {   // (the program may run what it wrote)
                    std::uint8_t bytes[4];
                    for (unsigned b = 0; b < 4; ++b) bytes[b] = static_cast<std::uint8_t>(bus.read(at, 4) >> (big_endian ? 24 - 8 * b : 8 * b));
                    std::memcpy(page->second.bytes.data() + (at & (kArmPage - 1)), bytes, 4);
                    emu.write(at, bytes, 4);
                }
            }
            return true;
        }
        const std::uint32_t a = vfp_transfer ? static_cast<std::uint32_t>(addr) : static_cast<std::uint32_t>(addr) & ~(size - 1);   // (a store goes to the aligned address, the VFP's to the one given)
        if (!bus.mapped(a)) {
            if (!bus_error || (block_transfer && a - block_start < *bus_error - block_start)) bus_error = block_transfer ? a : static_cast<std::uint32_t>(addr);
            return true;
        }
        bus.write(a, size, static_cast<std::uint32_t>(value));
        const auto page = mir.pages.find(a & ~(kArmPage - 1));
        if (page != mir.pages.end() && page->second.run == mir.run) {   // (the program may run what it wrote)
            for (std::uint32_t w = a & ~3u; w < a + size; w += 4) {
                const std::uint32_t word = bus.mapped(w) ? bus.read(w, 4) : 0;
                std::uint8_t bytes[4];
                for (unsigned b = 0; b < 4; ++b) bytes[b] = static_cast<std::uint8_t>(word >> (big_endian ? 24 - 8 * b : 8 * b));
                std::memcpy(page->second.bytes.data() + (w & (kArmPage - 1)), bytes, 4);
                emu.write(w, bytes, 4);
            }
        }
        return true;
    });
    emu.onUserOp([&](sl::Emulator&, const std::string& name, const std::vector<std::uint64_t>& args, std::uint64_t& result) {
        if (name == "hasExclusiveAccess") {   // (one processor runs: the exclusive monitor always holds, as the own core has it)
            result = 1;
            return true;
        }
        if (name == "VectorSignedToFloat" || name == "VectorUnsignedToFloat") {   // the VFP's integer to float (the description leaves it a name): a double or a single by the word's size field
            const std::uint32_t source = static_cast<std::uint32_t>(args.at(0));
            const double v = name == "VectorSignedToFloat" ? static_cast<double>(static_cast<std::int32_t>(source)) : static_cast<double>(source);
            result = (current_word >> 8 & 3) == 3 ? std::bit_cast<std::uint64_t>(v) : std::bit_cast<std::uint32_t>(static_cast<float>(v));
            return true;
        }
        if (name == "VectorFloatToSigned" || name == "VectorFloatToUnsigned") {   // ... and float to integer: toward zero, saturating, a NaN 0
            const double v = (current_word >> 8 & 3) == 3 ? std::bit_cast<double>(args.at(0)) : static_cast<double>(std::bit_cast<float>(static_cast<std::uint32_t>(args.at(0))));
            if (name == "VectorFloatToSigned")
                result = std::isnan(v) ? 0u : static_cast<std::uint32_t>(v >= 2147483647.0 ? INT32_MAX : v <= -2147483648.0 ? INT32_MIN : static_cast<std::int32_t>(v));
            else
                result = std::isnan(v) || v <= 0 ? 0u : v >= 4294967295.0 ? 0xFFFFFFFFu : static_cast<std::uint32_t>(v);
            return true;
        }
        if (name == "setISAMode") {   // (SetThumbMode's: TB, then this)
            if (!mode_switch) mode_switch = !args.empty() && (args[0] & 1) != 0;   // (the first: the description sets it back to ARM after a call for the decompiler's sake)
        }
        else if (name == "software_interrupt") return true;
        else if (name.rfind("coprocessor", 0) == 0 || name.rfind("software", 0) == 0) not_run = name;
        return true;
    });
    // An IT block as the own core runs it: the block's state is kept here, and an instruction in it either is skipped (its condition fails) or runs under a context that
    // says "in an IT block" (the description then leaves the flags of the 16-bit data-processing instructions alone) with the condition already settled. The
    // description's own way (the IT instruction's context for the instructions after it) is dropped after each decode, as the run keeps the state itself.
    const bool thumb2 = bus.arch() >= da::Arch::V7A;
    std::uint8_t it_first = 0, it_mask = 0, it_index = 0, it_left = 0;
    bool it_context = false;   // (the emulator is told it is in an IT block)
    const auto leave_it_context = [&] {
        if (!it_context) return;
        emu.setContext("cond_full", 0);
        emu.setContext("cond_mask", 0);
        it_context = false;
    };
    struct Unhook {
        sl::Emulator& e;
        const decltype(leave_it_context)& context;
        ~Unhook() {
            e.onRead({});
            e.onWrite({});
            e.onUserOp({});
            context();
        }
    } unhook{emu, leave_it_context};

    bool thumb = (entry & 1) != 0;
    emu.setContext("TMode", thumb ? 1 : 0);
    static constexpr const char* kArmStatus[6] = {"NG", "ZR", "CY", "OV", "cpsr", "TB"};
    const auto& arm_r = reg_refs(emu, kArmNames);
    const auto& arm_status = reg_refs(emu, kArmStatus);
    for (unsigned i = 0; i < 15; ++i) emu.setReg(arm_r[i], 0);
    for (unsigned i = 0; i < 4; ++i) emu.setReg(arm_r[i], options.a[i]);
    for (unsigned i = 0; i < 8; ++i) emu.setReg(arm_r[4 + i], options.s[i]);
    emu.setReg("sp", options.sp);
    emu.setReg("lr", options.return_address);
    for (const char* flag : {"NG", "ZR", "CY", "OV", "Q"}) emu.setReg(flag, 0);
    if (bus.arch() >= da::Arch::V6K) {   // the VFP's registers and status, as a new run has them (the emulator keeps the last run's)
        for (unsigned i = 0; i < 32; ++i) {
            const std::string name = "d" + std::to_string(i);
            if (emu.hasReg(name)) emu.setReg(name, 0);
        }
        if (emu.hasReg("fpscr")) emu.setReg("fpscr", 0);
    }
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
                for (unsigned i = 0; i < 5; ++i) into[i] = static_cast<std::uint32_t>(emu.reg(arm_r[8 + i]));
            };
            const auto load_high = [&](const std::array<std::uint32_t, 5>& from_regs) {
                for (unsigned i = 0; i < 5; ++i) emu.setReg(arm_r[8 + i], from_regs[i]);
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
        n = emu.reg(arm_status[0]) != 0;
        z = emu.reg(arm_status[1]) != 0;
        c = emu.reg(arm_status[2]) != 0;
        v = emu.reg(arm_status[3]) != 0;
    };
    const auto stop = [&](ar::Fault::Kind kind, std::uint32_t pc, std::uint32_t address) {
        out.end = ar::End::Stopped;
        out.fault = ar::Fault{kind, pc, address, {}, 0};
    };
    // One instruction by the own core, on the registers as they stand (the coprocessor 15 accesses of the DS: the hardware the bus answers).
    const auto delegate = [&](std::uint32_t pc, bool in_thumb, const char* why) -> std::optional<ar::Fault> {
        ++arm_handover().by_class[why];
        for (unsigned i = 0; i < 15; ++i) scratch.r[i] = static_cast<std::uint32_t>(emu.reg(arm_r[i]));
        scratch.r[15] = pc;
        bool n, z, c, v;
        flags(n, z, c, v);
        scratch.cpsr = (static_cast<unsigned>(emu.reg("cpsr")) & 0xDF) | (in_thumb ? 0x20u : 0u) | (n ? 0x80000000u : 0u) | (z ? 0x40000000u : 0u) | (c ? 0x20000000u : 0u) |
                       (v ? 0x10000000u : 0u) | (emu.reg("Q") != 0 ? 0x08000000u : 0u);
        const std::optional<ar::Fault> fault = scratch.step();
        for (unsigned i = 0; i < 15; ++i) emu.setReg(arm_r[i], scratch.r[i]);
        emu.setReg("NG", (scratch.cpsr >> 31) & 1);
        emu.setReg("ZR", (scratch.cpsr >> 30) & 1);
        emu.setReg("CY", (scratch.cpsr >> 29) & 1);
        emu.setReg("OV", (scratch.cpsr >> 28) & 1);
        emu.setReg("Q", (scratch.cpsr >> 27) & 1);
        emu.setPc(scratch.r[15]);
        if (const bool now_thumb = (scratch.cpsr & 0x20) != 0; now_thumb != thumb) {   // (a load into the pc can change the state)
            thumb = now_thumb;
            emu.setContext("TMode", thumb ? 1 : 0);
            emu.setReg("TB", thumb ? 1 : 0);
        }
        return fault;
    };
    while (steps < options.max_steps) {
        const std::uint32_t pc = static_cast<std::uint32_t>(emu.pc());
        if (pc == options.return_address) {
            out.end = ar::End::Return;
            break;
        }
        if (!options.imports.empty()) {
            if (const auto it = options.imports.find(pc); it != options.imports.end()) {   // a call into an imported function: logged by name, answered with 0, returned from
                ar::BiosCall call;
                call.name = it->second;
                for (unsigned i = 0; i < 4; ++i) call.args[i] = static_cast<std::uint32_t>(emu.reg(arm_r[i]));
                out.bios.push_back(call);
                if (std::find(options.ending_imports.begin(), options.ending_imports.end(), it->second) != options.ending_imports.end()) {
                    out.end = ar::End::Stopped;
                    out.fault = ar::Fault{ar::Fault::Kind::NotRun, pc, 0, "import " + it->second, 0};
                    break;
                }
                emu.setReg(arm_r[0], 0);
                const std::uint32_t back = static_cast<std::uint32_t>(emu.reg(arm_r[14]));
                if (const bool to_thumb = (back & 1) != 0; to_thumb != thumb) {
                    thumb = to_thumb;
                    emu.setContext("TMode", thumb ? 1 : 0);
                    emu.setReg("TB", thumb ? 1 : 0);
                }
                emu.setPc(back & (thumb ? ~1u : ~3u));
                ++steps;
                continue;
            }
        }
        if (!bus.mapped(pc)) {
            stop(ar::Fault::Kind::BusError, pc, pc);
            break;
        }
        const bool whole_here = load_page(pc), whole_next = load_page(pc + 3);
        if (big_endian && ((!whole_here && !plain_word(pc & ~3u)) || (!whole_next && (pc + 3) / kArmPage != pc / kArmPage && !plain_word((pc + 3) & ~3u)))) {   // fetched from hardware (the 3DO's Clio): the own core reads it as the machine does
            if (const std::optional<ar::Fault> fault = delegate(pc, thumb, "an instruction fetched from hardware")) {
                out.end = ar::End::Stopped;
                out.fault = *fault;
                fault_pc = pc;
                break;
            }
            ++steps;
            continue;
        }
        std::optional<da::Insn> insn;
        if (thumb) {
            const std::uint32_t a = bus.read(pc & ~1u, 2);
            const std::uint32_t b = (a >> 11) == 0x1E || (bus.arch() >= da::Arch::V7A && a >= 0xE800) ? bus.read((pc & ~1u) + 2, 2) : 0;
            insn = da::decode_thumb(static_cast<std::uint16_t>(a), static_cast<std::uint16_t>(b), pc | 1, bus.arch());
        } else {
            insn = da::decode_arm(bus.read(pc & ~3u, 4), pc, bus.arch());
        }
        if (!insn) {
            stop(ar::Fault::Kind::NoInstruction, pc, 0);
            break;
        }
        bool n = false, z = false, c = false, v = false;
        const bool is_thumb_insn = insn->thumb();
        if (thumb2) {   // IT: the instruction sets the block's state and does nothing else; each instruction after it takes the block's condition
            if (insn->op == da::Op::Ext && is_thumb_insn && insn->ext == da::t2::kIt) {
                it_first = static_cast<std::uint8_t>(insn->rs);
                it_mask = static_cast<std::uint8_t>(insn->list);
                it_index = 0;
                it_left = static_cast<std::uint8_t>(4 - std::countr_zero(static_cast<unsigned>(insn->list)));
                emu.setPc(pc + insn->length);
                ++steps;
                continue;
            }
            if (it_left > 0) {
                const unsigned cond = da::t2::it_condition(it_first, it_mask, it_index);
                ++it_index;
                --it_left;
                flags(n, z, c, v);
                if (!passes(cond, n, z, c, v)) {
                    emu.setPc(pc + insn->length);
                    ++steps;
                    continue;
                }
                if (insn->op == da::Op::B && insn->cond < 14 && insn->target) {   // a conditional branch in the block: the block's condition has been met, its own is not asked again
                    emu.setPc(*insn->target & ~1u);
                    ++steps;
                    continue;
                }
                if (!it_context) {
                    emu.setContext("cond_full", 14);
                    emu.setContext("cond_mask", 8);
                    it_context = true;
                }
            } else {
                leave_it_context();
            }
            if (insn->op == da::Op::Ext && is_thumb_insn && insn->ext == da::t2::kOpaque) {   // a 32-bit instruction the own core only decodes: it stops there as not run
                const std::string text = da::text(*insn);
                out.end = ar::End::Stopped;
                out.fault = ar::Fault{ar::Fault::Kind::NotRun, pc, 0, "thumb2 " + text.substr(0, text.find(' ')), 0};
                ++arm_handover().refused[text.substr(0, text.find(' '))];
                fault_pc = pc;
                break;
            }
        }
        const bool user_block = (insn->op == da::Op::Ldm || insn->op == da::Op::Stm) && insn->hat && !(insn->op == da::Op::Ldm && (insn->list & 0x8000));
        if (insn->op == da::Op::Swi || insn->op == da::Op::Bl || user_block) flags(n, z, c, v);
        if (user_block && passes(insn->cond, n, z, c, v)) {   // LDM and STM of the user registers: the own core does not run them, and the lane says so alike
            out.end = ar::End::Stopped;
            out.fault = ar::Fault{ar::Fault::Kind::NotRun, pc, 0, insn->op == da::Op::Ldm ? "ldm^" : "stm^", 0};
            fault_pc = pc;
            break;
        }
        if (insn->op == da::Op::Swi && passes(insn->cond, n, z, c, v)) {   // a BIOS call: logged and answered as the own core does
            ar::BiosCall call;
            call.function = bus.paged() ? insn->imm & 0xFFFF : thumb ? insn->imm & 0xFF : (insn->imm >> 16) & 0xFF;   // (a 3DS kernel call: the low 16 bits)
            for (unsigned i = 0; i < 4; ++i) call.args[i] = static_cast<std::uint32_t>(emu.reg(arm_r[i]));
            out.bios.push_back(call);
            if (std::find(options.ending.begin(), options.ending.end(), call.function) != options.ending.end()) {
                out.end = ar::End::Stopped;
                out.fault = ar::Fault{ar::Fault::Kind::NotRun, pc, 0, std::format("svc {:02X}", call.function), call.function};
                break;
            }
            if (bus.paged()) {   // (a kernel call: logged, answered with success)
                emu.setReg("r0", 0);
            } else {
                for (unsigned i = 0; i < 4; ++i) scratch.r[i] = call.args[i];
                ar::bios_answer(scratch, call.function);
                ++mir.run;   // (a BIOS call that moves memory may have written code: every page is read again)
                last_loaded = 1;
                for (unsigned i = 0; i < 4; ++i) emu.setReg(arm_r[i], scratch.r[i]);
            }
            emu.setPc(pc + insn->length);
            ++steps;
            continue;
        }
        if (insn->op == da::Op::Mcr || insn->op == da::Op::Mrc || insn->op == da::Op::Pld) {   // coprocessor 15: the bus's hardware, answered by the own core
            if (const std::optional<ar::Fault> fault = delegate(pc, thumb, "coprocessor 15 and PLD")) {
                out.end = ar::End::Stopped;
                out.fault = *fault;
                fault_pc = pc;
                break;
            }
            ++steps;
            continue;
        }
        const bool arm_row = insn->op == da::Op::Ext && insn->ext >= 0 && insn->ext < da::t2::kIt;   // (a row of the ARM table: in Thumb, the VFP's encodings; the Thumb-only ones are above 1000)
        // What the description does otherwise than the own core, in cases the manual leaves open: the own core's result is kept by handing the one instruction to it.
        const char* handed = nullptr;
        if (insn->op == da::Op::Stm && insn->wb && insn->rn < 15 && ((insn->list >> insn->rn) & 1)) handed = "stm with the base in the list";   // (the base is stored as written back, unless it is the first register)
        if (insn->op == da::Op::Ldm && insn->wb && insn->rn < 15 && ((insn->list >> insn->rn) & 1)) handed = "ldm with the base in the list";   // (the base is loaded, not written back)
        if (insn->op == da::Op::Str && (insn->wb || !insn->pre) && insn->rn < 15 && insn->rn == insn->rd) handed = "store of the base register with write-back";   // (the manual leaves the value stored open)
        if (!is_thumb_insn && (insn->op == da::Op::Ldr || insn->op == da::Op::Str) && insn->rn == 15 && (insn->size == 2 || insn->size == 8 || insn->sign)) handed = "ARM-state halfword, signed or doubleword load or store with the pc as base";   // (the description reads another address)
        if (bus.arch() >= da::Arch::V7A && !is_thumb_insn && arm_row && !da::ext::mnemonic(insn->ext).starts_with('v'))
            handed = "ARMv6 media instruction in ARM state (the description leaves the saturation helpers to the machine, and the own core refuses some of the family)";
        if (handed) {
            if (const std::optional<ar::Fault> fault = delegate(pc, thumb, handed)) {
                out.end = ar::End::Stopped;
                out.fault = *fault;
                fault_pc = pc;
                break;
            }
            ++steps;
            continue;
        }
        bus_error.reset();
        odd_half.reset();
        odd_byte.reset();
        signed_half = insn->size == 2 && insn->sign;
        const std::uint32_t ext_word = is_thumb_insn && insn->length == 4 ? insn->word << 16 | insn->word >> 16 : insn->word;   // (a Thumb word is held with its halves swapped)
        const bool memory_op = insn->op == da::Op::Ldr || insn->op == da::Op::Str || insn->op == da::Op::Ldm || insn->op == da::Op::Stm || insn->op == da::Op::Swp ||
                               arm_row;   // (the exclusive loads and the VFP's transfers)
        std::array<std::uint32_t, 15> before_regs{};
        unsigned touched = 0;   // the registers the transfer can change
        if (memory_op) {   // (an access that fails leaves the registers as they were: the description has written back and loaded zeros by then)
            if (insn->op == da::Op::Ext) {
                touched = 3u << (ext_word >> 12 & 15) | 1u << (ext_word >> 16 & 15);   // (Rt and the pair of LDREXD, the base)
            } else {
                if (insn->op == da::Op::Ldm || insn->op == da::Op::Stm) touched = static_cast<unsigned>(insn->list) & 0x7FFF;
                else if (insn->rd < 15) touched = 3u << insn->rd;   // (LDRD: the pair)
                if (insn->rn < 15) touched |= 1u << insn->rn;
            }
            touched &= 0x7FFF;
            for (unsigned i = 0; i < 15; ++i)
                if ((touched >> i) & 1) before_regs[i] = static_cast<std::uint32_t>(emu.reg(arm_r[i]));
        }
        block_transfer = insn->op == da::Op::Ldm || insn->op == da::Op::Stm;
        if (block_transfer) {
            const std::uint32_t count = static_cast<std::uint32_t>(std::popcount(static_cast<unsigned>(insn->list) & 0xFFFFu));
            const std::uint32_t base = insn->rn < 15 ? before_regs[insn->rn] : pc + (thumb ? 4u : 8u);
            const std::uint32_t low = insn->up ? (insn->pre ? base + 4 : base) : (insn->pre ? base - 4 * count : base - 4 * count + 4);
            block_start = low & ~3u;
        }
        dword_transfer = (insn->op == da::Op::Ldr || insn->op == da::Op::Str) && insn->size == 8;
        vfp_transfer = arm_row && da::ext::mnemonic(insn->ext).starts_with('v');
        current_word = ext_word;
        not_run.reset();
        mode_switch.reset();
        // The carry flag of a Thumb-2 logical instruction with the S bit as the own core has it: the shifter's carry out of a register operand, the flag as it was for an
        // immediate (the manual has the carry of an expanded immediate that is rotated; the description sets the carry of only some of these instructions, and the
        // own core's is the one this lane keeps).
        std::optional<bool> carry_after;
        if (is_thumb_insn && insn->length == 4 && insn->s && (insn->op == da::Op::And || insn->op == da::Op::Eor || insn->op == da::Op::Orr || insn->op == da::Op::Mov ||
                                                               insn->op == da::Op::Bic || insn->op == da::Op::Mvn || insn->op == da::Op::Tst || insn->op == da::Op::Teq)) {
            const bool carry_in = emu.reg(arm_status[2]) != 0;
            if (insn->imm_form) {
                carry_after = carry_in;
            } else if (insn->rm < 15 && (!insn->shift_by_reg || insn->rs < 15)) {
                const std::uint32_t value = static_cast<std::uint32_t>(emu.reg(arm_r[insn->rm]));
                const std::uint32_t amount = insn->shift_by_reg ? static_cast<std::uint32_t>(emu.reg(arm_r[insn->rs])) & 0xFF : insn->amount;
                if (insn->rrx) carry_after = (value & 1) != 0;
                else if (amount == 0) carry_after = carry_in;
                else switch (insn->shift) {
                    case da::Shift::Lsl: carry_after = amount < 32 ? ((value >> (32 - amount)) & 1) != 0 : amount == 32 && (value & 1); break;
                    case da::Shift::Lsr: carry_after = amount < 32 ? ((value >> (amount - 1)) & 1) != 0 : amount == 32 && (value >> 31) != 0; break;
                    case da::Shift::Asr: carry_after = amount < 32 ? ((value >> (amount - 1)) & 1) != 0 : (value >> 31) != 0; break;
                    case da::Shift::Ror: carry_after = (amount & 31) == 0 ? (value >> 31) != 0 : (da::rotate_right(value, amount & 31) >> 31) != 0; break;
                }
            }
        }
        const sl::Emulator::Status status = emu.step();
        if (status != sl::Emulator::Status::Ok) {
            if (status == sl::Emulator::Status::Error) {   // (the description's p-code failed: not an instruction the own core refuses)
                out.end = ar::End::Stopped;
                out.fault = ar::Fault{ar::Fault::Kind::NotRun, pc, 0, emu.lastError(), 0};
            } else if (status == sl::Emulator::Status::BadInstruction) {   // an encoding the own core's decoder reads and the description does not: the own core runs it
                const std::string text = da::text(*insn);
                if (const std::optional<ar::Fault> fault = delegate(pc, thumb, ("not in the description: " + text.substr(0, text.find(' '))).c_str())) {
                    out.end = ar::End::Stopped;
                    out.fault = *fault;
                    fault_pc = pc;
                    break;
                }
                ++steps;
                continue;
            } else {
                stop(ar::Fault::Kind::NoInstruction, pc, 0);
            }
            break;
        }
        if (carry_after) emu.setReg(arm_status[2], *carry_after ? 1 : 0);
        if (bus_error) {
            // (an LDM that fails part way has loaded the registers whose words came before the one that failed, as the own core goes up the block)
            const std::uint32_t loaded = insn->op == da::Op::Ldm ? (*bus_error - block_start) / 4 : 0;
            for (unsigned i = 0; i < 15; ++i)
                if (((touched >> i) & 1) && !(insn->op == da::Op::Ldm && ((insn->list >> i) & 1) && static_cast<std::uint32_t>(std::popcount(static_cast<unsigned>(insn->list) & ((1u << i) - 1))) < loaded))
                    emu.setReg(arm_r[i], before_regs[i]);
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
            emu.setReg(arm_r[insn->rd], result);
        }
        // the state after the instruction: BX and loads into the pc say it; a BLX by an immediate changes it
        bool next_thumb = thumb;
        if (insn->op == da::Op::Bx) next_thumb = emu.reg(arm_status[5]) != 0;   // (on the ARMv4T only BX changes the state, and the description records it in TB: MOV and LDR to the pc ignore bit 0)
        else if (v5 && mode_switch && ((insn->op == da::Op::Ldr && insn->rd == 15) || (insn->op == da::Op::Ldm && (insn->list & 0x8000)) || (insn->op == da::Op::Blx && !insn->target)))
            next_thumb = *mode_switch;   // (on the ARMv5 a load into the pc, POP and BLX by a register take the state from bit 0)
        else if (insn->op == da::Op::Blx && insn->target) next_thumb = (*insn->target & 1) != 0;   // (BLX by an immediate: to the other state, the decoder's target says which)
        else if (insn->op == da::Op::Bl && insn->target && passes(insn->cond, n, z, c, v) && insn->length == 4 && !thumb && (insn->word >> 28) == 0xF) next_thumb = true;
        else if (thumb && insn->word >> 11 == 0x1D) next_thumb = false;   // BLX (immediate) in Thumb
        if (next_thumb != thumb) {
            thumb = next_thumb;
            emu.setContext("TMode", thumb ? 1 : 0);
        }
        if ((static_cast<unsigned>(emu.reg(arm_status[4])) & 0x1F) != mode) change_mode(static_cast<unsigned>(emu.reg(arm_status[4])));
        if (const std::uint32_t next_pc = static_cast<std::uint32_t>(emu.pc()); next_pc != (next_pc & (thumb ? ~1u : ~3u))) emu.setPc(next_pc & (thumb ? ~1u : ~3u));   // (a pc written by a load keeps no state bit)
        ++steps;
    }
    for (unsigned i = 0; i < 15; ++i) out.r[i] = static_cast<std::uint32_t>(emu.reg(arm_r[i]));
    out.r[15] = fault_pc ? *fault_pc : static_cast<std::uint32_t>(emu.pc());
    bool n, z, c, v;
    flags(n, z, c, v);
    out.cpsr = (static_cast<unsigned>(emu.reg("cpsr")) & 0xDF) | (thumb ? 0x20u : 0u) | (n ? 0x80000000u : 0u) | (z ? 0x40000000u : 0u) | (c ? 0x20000000u : 0u) | (v ? 0x10000000u : 0u) | (emu.reg("Q") != 0 ? 0x08000000u : 0u);
    out.steps = steps;
    arm_handover().steps += steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

ar::RunResult call(ar::Bus& bus, std::uint32_t entry, const ar::RunOptions& options) {
    return enabled() ? pcode_cpu::run(bus, entry, options) : ar::run(bus, entry, options);
}

}  // namespace cb::engine::pcode_cpu
