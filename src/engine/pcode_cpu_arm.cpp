// engine/pcode_cpu_arm.cpp -- the ARM lanes' CPU on the one p-code emulator: emulate::arm::run's call, on languages/ARM4t_le.sla (the GBA, the DS's ARM7), ARM5t_le.sla (the DS's
// ARM9), ARM6_le.sla (the 3DS), ARM7_le.sla (the Vita) and ARM4t_be.sla (the 3DO: big-endian words).
//
// Carried here: the state (ARM or Thumb: the description's TMode context follows BX, loads into the pc, BLX and, from the ARMv7, an ARM data-processing write of the pc), the
// software interrupts (logged with their number and r0-r3, then answered as the BIOS does by the own core's own answer), the imported functions of the Vita (logged by name,
// answered 0), bus errors (a fetch, load or store of an address nothing answers; a block transfer or an Advanced SIMD transfer that has a word nothing answers does nothing),
// IT blocks (kept here: an instruction in one is skipped or run under a context that stops the 16-bit data-processing instructions from setting the flags), coprocessor 15
// and the preload hint (the machine's registers are the bus's), and the registers and flags as the own core hands them back. What the own core refuses stays refused (a
// 32-bit Thumb instruction it only decodes, an encoding the manual leaves UNPREDICTABLE). Where the description is wrong the adapter does the work itself, and says so at
// the place: the saturating and absolute-difference instructions, LDRSB (literal) and the pc-based halfword and doubleword transfers, LDRD (literal)'s register order and
// negative offset, a block transfer that lists its base. Nothing runs on the own core.
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
#include "engine/pcode_neon_arm32.hpp"
#include "engine/pcode_ops_arm32.hpp"

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
    pcode_ops::Arm32Context ops;   // what the bound operations (engine/pcode_ops_arm32) read of the run
    std::array<sl::Emulator::RegRef, 32> d_regs{};   // d0-d31 and the floating point status register, found once (the ARMv7 description)
    sl::Emulator::RegRef fpscr{};
    bool vfp_found = false;
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
        pcode_ops::bind_arm32(emu, &mir.ops);   // (the description's named operations the table serves, by number)
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
    bool signed_half = false;   // the instruction about to run is an LDRSH
    bool block_transfer = false;   // ... an LDM or STM
    bool dword_transfer = false;   // ... an LDRD or STRD (or an exclusive pair)
    bool neon_access = false;          // ... an Advanced SIMD load or store (the bytes are moved one at a time, from any address)
    bool exclusive_transfer = false;   // ... an exclusive load or store, or a table branch (a word read from an address that is not aligned is not rotated)
    std::uint32_t base_slot = 0, base_loaded = 0;   // ... an LDM or STM with write-back that lists its base: the slot the base's word is at, and (an LDM) the word it loaded
    bool base_listed_load = false;
    std::optional<std::uint32_t> last_write_address;   // ... the address of the last store it made
    std::optional<std::uint32_t> dword_raw;   // ... the address the program gave it (a failing access reports this one, whichever of its words fails)
    bool negative_literal = false;   // ... a Thumb LDRD of the pc-relative form with the offset subtracted (the description always adds it)
    std::uint32_t literal_base = 0;
    bool vfp_transfer = false;     // ... a VFP load or store
    std::uint32_t block_start = 0; // an LDM or STM: the lowest word it transfers (the own core goes up from it, so its first failing access is the one nearest above)
    std::optional<std::uint32_t> fault_pc;
    std::optional<std::string> not_run;
    std::optional<bool> mode_switch;
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        std::uint32_t raw = static_cast<std::uint32_t>(addr);
        if (neon_access) {   // (every byte is there: checked before the step)
            std::uint64_t v = 0;
            for (unsigned k = 0; k < size && k < 8; ++k) v |= static_cast<std::uint64_t>(bus.read(raw + k, 1)) << (8 * k);
            value = v;
            return true;
        }
        if (negative_literal && size == 8) raw = 2 * literal_base - raw;
        if (dword_transfer && !dword_raw) dword_raw = raw;
        if (size == 8) {   // (a VFP double: two words)
            const std::uint32_t first = vfp_transfer ? raw : raw & ~3u;   // (the own core's VFP transfers do not align)
            for (unsigned k = 0; k < 2; ++k)
                if (!bus.mapped(first + 4 * k)) {
                    if (!bus_error) bus_error = dword_transfer ? raw : raw + 4 * k;   // (as the program gave it)
                    value = 0;
                    return true;
                }
            value = static_cast<std::uint64_t>(bus.read(first, 4)) | static_cast<std::uint64_t>(bus.read(first + 4, 4)) << 32;
            return true;
        }
        const bool byte_load = size == 2 && (raw & 1) != 0 && signed_half;   // (a signed halfword load from an odd address is a signed byte load)
        const std::uint32_t a = byte_load || vfp_transfer ? raw : raw & ~(size - 1);   // (the own core's accesses are aligned, the VFP's not)
        if (!bus.mapped(a)) {
            if (!bus_error || (block_transfer && a - block_start < *bus_error - block_start)) bus_error = block_transfer ? a : dword_transfer ? *dword_raw : raw;   // (a single transfer reports the address as the program gave it, a block transfer the first word's that fails going up)
            value = 0;
            return true;
        }
        value = bus.read(a, byte_load ? 1 : size);
        if (base_listed_load && a == base_slot) base_loaded = static_cast<std::uint32_t>(value);
        if (size == 4 && (raw & 3) != 0 && !block_transfer && !dword_transfer && !vfp_transfer && !exclusive_transfer) {   // (a word load from an unaligned address rotates the aligned word: the description does not; LDM just aligns)
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
        if (neon_access) {
            for (unsigned k = 0; k < size && k < 8; ++k) {
                const std::uint32_t at = static_cast<std::uint32_t>(addr) + k;
                bus.write(at, 1, static_cast<std::uint32_t>(value >> (8 * k)) & 0xFF);
                const auto page = mir.pages.find(at & ~(kArmPage - 1));
                if (page != mir.pages.end() && page->second.run == mir.run) {   // (the program may run what it wrote)
                    const std::uint32_t word_at = at & ~3u, word = bus.mapped(word_at) ? bus.read(word_at, 4) : 0;
                    std::uint8_t bytes[4];
                    for (unsigned b = 0; b < 4; ++b) bytes[b] = static_cast<std::uint8_t>(word >> (big_endian ? 24 - 8 * b : 8 * b));
                    std::memcpy(page->second.bytes.data() + (word_at & (kArmPage - 1)), bytes, 4);
                    emu.write(word_at, bytes, 4);
                }
            }
            return true;
        }
        if (dword_transfer && !dword_raw) {   // (a pair of words: both must be there, or neither is written)
            dword_raw = static_cast<std::uint32_t>(addr);
            const std::uint32_t first = static_cast<std::uint32_t>(addr) & ~3u;
            if (size == 4 && (!bus.mapped(first) || !bus.mapped(first + 4))) bus_error = dword_raw;
        }
        if (dword_transfer && bus_error) return true;
        if (size == 8) {   // (a VFP double: two words)
            const std::uint32_t first = vfp_transfer ? static_cast<std::uint32_t>(addr) : static_cast<std::uint32_t>(addr) & ~3u;
            for (unsigned k = 0; k < 2; ++k)
                if (!bus.mapped(first + 4 * k)) {
                    if (!bus_error) bus_error = dword_transfer ? static_cast<std::uint32_t>(addr) : static_cast<std::uint32_t>(addr) + 4 * k;
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
        last_write_address = a;
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
    emu.onUserOp([&](sl::Emulator&, const std::string& name, const std::vector<std::uint64_t>& args, std::uint64_t&) {
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
    // The saturating instructions run by the adapter itself (ARM ARM A8.8: QADD QSUB QDADD QDSUB, SSAT USAT SSAT16 USAT16): the description computes the sum or the shifted value
    // in the width of the register and saturates what is left of it (the overflow is already gone), and its Thumb SSAT passes the field without the 1 it stands for. A
    // register-only instruction: it reads and writes the registers and Q. True: it was one of them and has run.
    static const std::vector<bool> host_rows = [] {   // (the ARM rows the adapter runs itself)
        std::vector<bool> rows(static_cast<std::size_t>(da::ext::row_count()), false);
        for (int row = 0; row < da::ext::row_count(); ++row) {
            const std::string_view name = da::ext::mnemonic(row);
            rows[static_cast<std::size_t>(row)] = name == "ssat" || name == "usat" || name == "ssat16" || name == "usat16" || name == "usad8" || name == "usada8";
        }
        return rows;
    }();
    const auto host_candidate = [&](const da::Insn& in, bool in_thumb) {
        switch (in.op) {
            case da::Op::Qadd: case da::Op::Qsub: case da::Op::Qdadd: case da::Op::Qdsub: return true;
            case da::Op::Ext:
                if (in.ext >= da::t2::kSsatT && in.ext <= da::t2::kUsat16T) return in_thumb;
                return in.ext >= 0 && in.ext < da::t2::kIt && host_rows[static_cast<std::size_t>(in.ext)] && (!in_thumb || in.imm != 0);
            default: return false;
        }
    };
    const auto host_saturating = [&](const da::Insn& in, bool in_thumb) -> bool {
        const auto reg = [&](unsigned k) -> std::uint32_t { return k < 15 ? static_cast<std::uint32_t>(emu.reg(arm_r[k])) : 0; };
        const auto put = [&](unsigned k, std::uint32_t v) {
            if (k < 15) emu.setReg(arm_r[k], v);
        };
        const auto clamp = [&](std::int64_t v, std::int64_t low, std::int64_t high) {
            if (v < low || v > high) emu.setReg("Q", 1);
            return static_cast<std::uint32_t>(std::clamp(v, low, high));
        };
        const auto shifted = [&](unsigned rm, bool asr, unsigned amount) -> std::int64_t {   // (the operand of SSAT and USAT: an arithmetic shift by 0 is by 32)
            const std::int32_t v = static_cast<std::int32_t>(reg(rm));
            return asr ? v >> (amount == 0 ? 31 : amount) : static_cast<std::int32_t>(static_cast<std::uint32_t>(v) << amount);
        };
        const auto halves = [&](unsigned rm, unsigned rd, std::int64_t low, std::int64_t high) {
            const std::uint32_t v = reg(rm);
            const std::uint32_t lo = clamp(static_cast<std::int16_t>(v), low, high), hi = clamp(static_cast<std::int16_t>(v >> 16), low, high);
            put(rd, (lo & 0xFFFFu) | hi << 16);
        };
        switch (in.op) {
            case da::Op::Qadd: case da::Op::Qsub: case da::Op::Qdadd: case da::Op::Qdsub: {
                const std::int64_t m = static_cast<std::int32_t>(reg(in.rm));
                std::int64_t n = static_cast<std::int32_t>(reg(in.rn));
                if (in.op == da::Op::Qdadd || in.op == da::Op::Qdsub) n = static_cast<std::int32_t>(clamp(n * 2, INT32_MIN, INT32_MAX));
                put(in.rd, clamp(in.op == da::Op::Qadd || in.op == da::Op::Qdadd ? m + n : m - n, INT32_MIN, INT32_MAX));
                return true;
            }
            case da::Op::Ext: break;
            default: return false;
        }
        if (in_thumb && in.ext >= da::t2::kSsatT && in.ext <= da::t2::kUsat16T) {
            const bool is_signed = in.ext == da::t2::kSsatT || in.ext == da::t2::kSsat16T;
            const std::int64_t high = is_signed ? (std::int64_t{1} << (in.imm - 1)) - 1 : (std::int64_t{1} << in.imm) - 1, low = is_signed ? -(std::int64_t{1} << (in.imm - 1)) : 0;
            if (in.ext == da::t2::kSsat16T || in.ext == da::t2::kUsat16T) halves(in.rn, in.rd, low, high);
            else put(in.rd, clamp(shifted(in.rn, in.shift == da::Shift::Asr, in.amount), low, high));
            return true;
        }
        const std::uint32_t w = in_thumb ? in.imm : in.word;   // (the ARM word a Thumb ARMv6 instruction means is in imm)
        if (in.ext < 0 || in.ext >= da::t2::kIt || (in_thumb && w == 0)) return false;
        const std::string_view name = da::ext::mnemonic(in.ext);
        if (name == "ssat" || name == "usat") {
            const bool is_signed = name == "ssat";
            const unsigned bits = ((w >> 16) & 31) + (is_signed ? 1 : 0);
            const std::int64_t high = is_signed ? (std::int64_t{1} << (bits - 1)) - 1 : (std::int64_t{1} << bits) - 1, low = is_signed ? -(std::int64_t{1} << (bits - 1)) : 0;
            put((w >> 12) & 15, clamp(shifted(w & 15, (w >> 6) & 1, (w >> 7) & 31), low, high));
            return true;
        }
        if (name == "usad8" || name == "usada8") {   // the sum of the absolute differences of the four bytes (the description subtracts bytes and takes the absolute value of the wrapped byte)
            std::uint32_t sum = name == "usada8" ? reg((w >> 12) & 15) : 0;
            const std::uint32_t x = reg(w & 15), y = reg((w >> 8) & 15);
            for (unsigned k = 0; k < 32; k += 8) {
                const std::int32_t a = static_cast<std::int32_t>(x >> k & 0xFF), b = static_cast<std::int32_t>(y >> k & 0xFF);
                sum += static_cast<std::uint32_t>(a > b ? a - b : b - a);
            }
            put((w >> 16) & 15, sum);
            return true;
        }
        if (name == "ssat16" || name == "usat16") {
            const bool is_signed = name == "ssat16";
            const unsigned bits = ((w >> 16) & 15) + (is_signed ? 1 : 0);
            halves(w & 15, (w >> 12) & 15, is_signed ? -(std::int64_t{1} << (bits - 1)) : 0, is_signed ? (std::int64_t{1} << (bits - 1)) - 1 : (std::int64_t{1} << bits) - 1);
            return true;
        }
        return false;
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
        std::optional<std::uint32_t> fetched;   // the word of an instruction fetched from hardware
        if (big_endian && ((!whole_here && !plain_word(pc & ~3u)) || (!whole_next && (pc + 3) / kArmPage != pc / kArmPage && !plain_word((pc + 3) & ~3u)))) {   // fetched from hardware (the 3DO's Clio): the word is read as the machine answers it (one logged read) and given to the emulator for this step
            const std::uint32_t word = bus.read(pc & ~3u, 4);
            const std::uint8_t bytes[4] = {static_cast<std::uint8_t>(word >> 24), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word)};
            emu.write(pc & ~3u, bytes, 4);
            fetched = word;
        }
        std::optional<da::Insn> insn;
        if (thumb) {
            const std::uint32_t a = bus.read(pc & ~1u, 2);
            const std::uint32_t b = (a >> 11) == 0x1E || (bus.arch() >= da::Arch::V7A && a >= 0xE800) ? bus.read((pc & ~1u) + 2, 2) : 0;
            insn = da::decode_thumb(static_cast<std::uint16_t>(a), static_cast<std::uint16_t>(b), pc | 1, bus.arch());
        } else {
            insn = da::decode_arm(fetched ? *fetched : bus.read(pc & ~3u, 4), pc, bus.arch());
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
        if (host_candidate(*insn, is_thumb_insn)) {
            flags(n, z, c, v);
            if (is_thumb_insn || passes(insn->cond, n, z, c, v)) {   // (an ARM word's condition; a Thumb one's is the IT block's, settled above)
                if (host_saturating(*insn, is_thumb_insn)) {
                    emu.setPc(pc + insn->length);
                    ++steps;
                    continue;
                }
            } else {
                emu.setPc(pc + insn->length);
                ++steps;
                continue;
            }
        }
        if (insn->op == da::Op::Ext && insn->ext == da::t2::kNeon && bus.arch() >= da::Arch::V7A) {   // an Advanced SIMD data-processing instruction or a transfer to or from the scalars: the lane library's (the description names these operations and leaves out the destination of an accumulating one and the saturation of a result)
            if (const std::optional<da::t2::NeonDp> simd = da::t2::neon_dp(insn->imm, is_thumb_insn)) {
                flags(n, z, c, v);
                if (is_thumb_insn || passes(insn->cond, n, z, c, v)) {
                    if (!mir.vfp_found) {
                        for (unsigned i = 0; i < 32; ++i) mir.d_regs[i] = emu.regRef("d" + std::to_string(i));
                        mir.fpscr = emu.regRef("fpscr");
                        mir.vfp_found = true;
                    }
                    pcode_neon_arm32::Regs regs;
                    const std::uint32_t used = pcode_neon_arm32::reads(*simd);
                    std::uint64_t before[32];
                    for (unsigned i = 0; i < 32; ++i)
                        if ((used >> i) & 1) before[i] = regs.d[i] = emu.reg(mir.d_regs[i]);
                    const bool pair = simd->op == da::t2::NeonOp::VMovPair;
                    if (simd->op == da::t2::NeonOp::MovToScalar || simd->op == da::t2::NeonOp::DupCore || (pair && !simd->u)) {
                        regs.r[simd->rt] = static_cast<std::uint32_t>(emu.reg(arm_r[simd->rt]));
                        if (pair) regs.r[simd->idx] = static_cast<std::uint32_t>(emu.reg(arm_r[simd->idx]));
                    }
                    const std::uint32_t raised = pcode_neon_arm32::run(*simd, regs);
                    for (unsigned i = 0; i < 32; ++i)
                        if (((used >> i) & 1) && regs.d[i] != before[i]) emu.setReg(mir.d_regs[i], regs.d[i]);
                    if (simd->op == da::t2::NeonOp::MovFromScalar) emu.setReg(arm_r[simd->rt], regs.r[simd->rt]);
                    if (pair && simd->u) {
                        emu.setReg(arm_r[simd->rt], regs.r[simd->rt]);
                        emu.setReg(arm_r[simd->idx], regs.r[simd->idx]);
                    }
                    if (raised != 0) emu.setReg(mir.fpscr, emu.reg(mir.fpscr) | raised);
                }
                emu.setPc(pc + insn->length);
                ++steps;
                continue;
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
        if (!is_thumb_insn && ((static_cast<unsigned>(insn->op) < 16 && insn->s && insn->rd == 15 && insn->op != da::Op::Tst && insn->op != da::Op::Teq && insn->op != da::Op::Cmp && insn->op != da::Op::Cmn) ||
                               (insn->op == da::Op::Ldm && insn->hat && (insn->list & 0x8000))) && bank_of(mode) == 0) {   // (a return from an exception in a mode that has none: UNPREDICTABLE)
            flags(n, z, c, v);
            if (passes(insn->cond, n, z, c, v)) {
                out.end = ar::End::Stopped;
                out.fault = ar::Fault{ar::Fault::Kind::NotRun, pc, 0, "exception return outside an exception mode", 0};
                fault_pc = pc;
                break;
            }
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
        if (insn->op == da::Op::Mcr || insn->op == da::Op::Mrc || insn->op == da::Op::Pld) {   // coprocessor 15 and the preload hint: the adapter's own semantics (the machine's registers are the bus's)
            flags(n, z, c, v);
            if (is_thumb_insn || passes(insn->cond, n, z, c, v)) {   // (a Thumb instruction's condition is the IT block's, settled above)
                if (insn->op != da::Op::Pld) {
                    if (insn->cp != 15) {   // (the one coprocessor there is)
                        out.end = ar::End::Stopped;
                        out.fault = ar::Fault{ar::Fault::Kind::NotRun, pc, 0, std::format("{} p{}", insn->op == da::Op::Mcr ? "mcr" : "mrc", insn->cp), 0};
                        break;
                    }
                    const std::uint32_t reg = static_cast<std::uint32_t>(insn->opc1) << 12 | static_cast<std::uint32_t>(insn->crn) << 8 | static_cast<std::uint32_t>(insn->crm) << 4 | insn->opc2;
                    if (insn->op == da::Op::Mcr) {
                        const std::uint32_t value = insn->rd == 15 ? pc + 8 : static_cast<std::uint32_t>(emu.reg(arm_r[insn->rd]));
                        bus.log_access(ar::BusAccess::Kind::Write, 4, ar::kNdsCp15Base | reg, value);
                        bus.cp15_write(reg, value);
                    } else {
                        const std::uint32_t value = bus.cp15_read(reg);
                        bus.log_access(ar::BusAccess::Kind::Read, 4, ar::kNdsCp15Base | reg, value);
                        if (insn->rd == 15) {   // (the flags take the top four bits)
                            emu.setReg("NG", (value >> 31) & 1);
                            emu.setReg("ZR", (value >> 30) & 1);
                            emu.setReg("CY", (value >> 29) & 1);
                            emu.setReg("OV", (value >> 28) & 1);
                        } else {
                            emu.setReg(arm_r[insn->rd], value);
                        }
                    }
                }
            }
            emu.setPc(pc + insn->length);
            ++steps;
            continue;
        }
        if (is_thumb_insn && insn->length == 4 && insn->op == da::Op::Ldr && insn->rn == 15 && insn->size == 1 && insn->sign && insn->rd < 15) {   // LDRSB (literal)
            // by the adapter: the description reads the word at the address and takes the byte it holds as the address of the byte it loads (a second indirection)
            const std::uint32_t base = (pc + 4) & ~3u, at = insn->up ? base + insn->imm : base - insn->imm;
            if (!bus.mapped(at)) {
                out.end = ar::End::Stopped;
                out.fault = ar::Fault{ar::Fault::Kind::BusError, pc, at, {}, 0};
                fault_pc = pc;
                break;
            }
            emu.setReg(arm_r[insn->rd], static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(bus.read(at, 1)))));
            emu.setPc(pc + 4);
            ++steps;
            continue;
        }
        if (insn->op == da::Op::Bkpt) {   // a breakpoint is not run: the run stops at it
            out.end = ar::End::Stopped;
            out.fault = ar::Fault{ar::Fault::Kind::NotRun, pc, 0, "bkpt", 0};
            fault_pc = pc;
            break;
        }
        if (!is_thumb_insn && (insn->op == da::Op::Ldr || insn->op == da::Op::Str) && insn->rn == 15 && (insn->size == 2 || insn->size == 8 || insn->sign) && insn->imm_form) {
            // ARM state, a halfword, signed byte or doubleword transfer on the pc (the literal pool): by the adapter. The description reads the address it computes twice over a register
            // it has just changed, and a store of the pair goes to address 0. The address is the pc (this instruction plus 8) plus or minus the offset.
            flags(n, z, c, v);
            if (passes(insn->cond, n, z, c, v)) {
                ++arm_handover().by_class["ARM-state halfword, signed or doubleword transfer on the pc (the adapter's own)"];
                const std::uint32_t base = pc + 8, at = insn->up ? base + insn->imm : base - insn->imm, address = insn->pre ? at : base;
                const unsigned size = insn->size == 8 ? 4 : insn->size;
                const std::uint32_t first = insn->size == 8 ? address & ~3u : address & ~(size - 1);
                if (!bus.mapped(first) || (insn->size == 8 && !bus.mapped(first + 4))) {
                    out.end = ar::End::Stopped;
                    out.fault = ar::Fault{ar::Fault::Kind::BusError, pc, address, {}, 0};
                    fault_pc = pc;
                    break;
                }
                if (insn->op == da::Op::Str) {
                    const auto value = [&](unsigned k) { return k == 15 ? pc + 12 : static_cast<std::uint32_t>(emu.reg(arm_r[k])); };
                    if (insn->size == 8) {
                        emu.busWrite(first, 4, value(insn->rd), "ram");
                        emu.busWrite(first + 4, 4, value(insn->rd + 1u), "ram");
                    } else {
                        const std::uint32_t word = value(insn->rd);
                        emu.busWrite(first, size, size == 4 ? word : word & ((1u << (8 * size)) - 1), "ram");
                    }
                } else if (insn->size == 8) {
                    const std::uint32_t low = bus.read(first, 4), high = bus.read(first + 4, 4);
                    emu.setReg(arm_r[insn->rd], low);
                    emu.setReg(arm_r[insn->rd + 1u], high);
                } else {
                    std::uint32_t loaded;
                    if (insn->size == 1) loaded = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(bus.read(address, 1))));
                    else if (insn->sign) loaded = (address & 1) ? static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(bus.read(address, 1)))) : static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(bus.read(address, 2))));
                    else loaded = da::rotate_right(bus.read(first, 2), (address & 1) ? 8 : 0);
                    if (insn->rd < 15) emu.setReg(arm_r[insn->rd], loaded);
                }
            }
            emu.setPc(pc + 4);
            ++steps;
            continue;
        }
        const bool arm_row = insn->op == da::Op::Ext && insn->ext >= 0 && insn->ext < da::t2::kIt;   // (a row of the ARM table: in Thumb, the VFP's encodings; the Thumb-only ones are above 1000)
        bus_error.reset();
        odd_half.reset();
        odd_byte.reset();
        signed_half = insn->size == 2 && insn->sign;
        const std::uint32_t ext_word = is_thumb_insn && insn->length == 4 ? insn->word << 16 | insn->word >> 16 : insn->word;   // (a Thumb word is held with its halves swapped)
        const bool t32_memory = insn->op == da::Op::Ext && is_thumb_insn && insn->ext >= da::t2::kLdrexT && insn->ext <= da::t2::kTb;   // (the exclusive ones, the table branches)
        const bool memory_op = insn->op == da::Op::Ldr || insn->op == da::Op::Str || insn->op == da::Op::Ldm || insn->op == da::Op::Stm || insn->op == da::Op::Swp ||
                               arm_row || t32_memory;   // (the exclusive loads and the VFP's transfers)
        std::array<std::uint32_t, 15> before_regs{};
        unsigned touched = 0;   // the registers the transfer can change
        if (memory_op) {   // (an access that fails leaves the registers as they were: the description has written back and loaded zeros by then)
            if (t32_memory) {
                touched = 0x7FFF;
            } else if (insn->op == da::Op::Ext) {
                touched = 3u << (ext_word >> 12 & 15) | 1u << (ext_word >> 16 & 15);   // (Rt and the pair of LDREXD, the base)
            } else {
                if (insn->op == da::Op::Ldm || insn->op == da::Op::Stm) touched = static_cast<unsigned>(insn->list) & 0x7FFF;
                else if (is_thumb_insn && insn->size == 8) touched = 1u << insn->rd | 1u << insn->rs;   // (Thumb LDRD: the two registers it names)
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
            if (insn->wb && insn->rn < 15 && ((insn->list >> insn->rn) & 1)) {   // a base that is listed: where its word is
                base_slot = block_start + 4 * static_cast<std::uint32_t>(std::popcount(static_cast<unsigned>(insn->list) & ((1u << insn->rn) - 1)));
                base_listed_load = insn->op == da::Op::Ldm;
            } else {
                base_listed_load = false;
            }
        } else {
            base_listed_load = false;
        }
        bool cond_ok = true;   // (the condition of an ARM word, asked where a fix-up after the step needs it)
        if (!is_thumb_insn && insn->cond != 14 && (block_transfer || (insn->op == da::Op::Str && (insn->wb || !insn->pre) && insn->rn == insn->rd))) {
            flags(n, z, c, v);
            cond_ok = passes(insn->cond, n, z, c, v);
        }
        if (block_transfer && cond_ok) {   // (a block transfer that has a word nothing answers does nothing: it can be restarted, and the manual leaves the words that came before UNKNOWN)
            const std::uint32_t count = static_cast<std::uint32_t>(std::popcount(static_cast<unsigned>(insn->list) & 0xFFFFu));
            for (std::uint32_t k = 0; k < count && !bus_error; ++k)
                if (!bus.mapped(block_start + 4 * k)) bus_error = block_start + 4 * k;
            if (bus_error) {
                out.end = ar::End::Stopped;
                out.fault = ar::Fault{ar::Fault::Kind::BusError, pc, *bus_error, {}, 0};
                fault_pc = pc;
                break;
            }
        }
        neon_access = false;
        if (insn->op == da::Op::Ext && insn->ext == da::t2::kNeon) {
            if (const std::optional<da::t2::NeonTransfer> t = da::t2::neon_transfer(insn->imm)) {   // (nothing is moved unless every byte is there)
                neon_access = true;
                const std::uint32_t base_address = static_cast<std::uint32_t>(emu.reg(arm_r[t->rn]));
                for (unsigned k = 0; k < t->bytes; ++k)
                    if (!bus.mapped(base_address + k)) {
                        out.end = ar::End::Stopped;
                        out.fault = ar::Fault{ar::Fault::Kind::BusError, pc, base_address + k, {}, 0};
                        fault_pc = pc;
                        break;
                    }
                if (out.end == ar::End::Stopped) break;
            }
        }
        last_write_address.reset();
        dword_raw.reset();
        negative_literal = is_thumb_insn && insn->op == da::Op::Ldr && insn->size == 8 && insn->rn == 15 && !insn->up;
        literal_base = (pc + 4) & ~3u;
        dword_transfer = ((insn->op == da::Op::Ldr || insn->op == da::Op::Str) && insn->size == 8) || (t32_memory && (insn->ext == da::t2::kLdrexdT || insn->ext == da::t2::kStrexdT));
        const std::string_view row_name = arm_row ? da::ext::mnemonic(insn->ext) : std::string_view();
        exclusive_transfer = t32_memory || row_name.starts_with("ldrex") || row_name.starts_with("strex");
        vfp_transfer = arm_row && row_name.starts_with('v');
        mir.ops.word = ext_word;
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
                carry_after = insn->rot != 0 ? (insn->imm >> 31) != 0 : carry_in;   // (ThumbExpandImm_C: the rotated ones' carry out is bit 31)
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
        const bool alu_pc_write = static_cast<unsigned>(insn->op) < 16 && !thumb && bus.arch() >= da::Arch::V7A && insn->rd == 15 && !insn->s;
        std::uint64_t cpsr_before = 0;
        if (alu_pc_write) cpsr_before = emu.reg(arm_status[4]);
        const sl::Emulator::Status status = emu.step();
        if (status != sl::Emulator::Status::Ok) {
            if (status == sl::Emulator::Status::Error) {   // (the description's p-code failed: not an instruction the own core refuses)
                out.end = ar::End::Stopped;
                out.fault = ar::Fault{ar::Fault::Kind::NotRun, pc, 0, emu.lastError(), 0};
            } else if (status == sl::Emulator::Status::BadInstruction) {   // an encoding the own core's decoder reads and the description does not (the 3DS: the VFP registers d16-d31, which VFPv2 has not): no instruction on this machine
                stop(ar::Fault::Kind::NoInstruction, pc, 0);
            } else {
                stop(ar::Fault::Kind::NoInstruction, pc, 0);
            }
            break;
        }
        if (carry_after) emu.setReg(arm_status[2], *carry_after ? 1 : 0);
        if (bus_error) {
            // (an LDM that fails part way has loaded nothing: the instruction can be restarted, and the manual leaves the registers it would have loaded UNKNOWN)
            for (unsigned i = 0; i < 15; ++i)
                if ((touched >> i) & 1) emu.setReg(arm_r[i], before_regs[i]);
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
        if (cond_ok && block_transfer && insn->wb && insn->rn < 15 && ((insn->list >> insn->rn) & 1)) {   // the base of a block transfer that is also in its list (ARM ARM leaves the result open; the own core's: a load keeps the word it loaded, a store keeps the written-back base unless the base is the first register listed)
            if (insn->op == da::Op::Ldm) {
                emu.setReg(arm_r[insn->rn], base_loaded);
            } else {
                const std::uint32_t count = static_cast<std::uint32_t>(std::popcount(static_cast<unsigned>(insn->list) & 0xFFFFu));
                const std::uint32_t written_back = insn->up ? before_regs[insn->rn] + 4 * count : before_regs[insn->rn] - 4 * count;
                emu.busWrite(base_slot, 4, insn->rn != std::countr_zero(static_cast<unsigned>(insn->list)) ? written_back : before_regs[insn->rn], "ram");
                emu.setReg(arm_r[insn->rn], written_back);
            }
        }
        if (cond_ok && insn->op == da::Op::Str && (insn->wb || !insn->pre) && insn->rn < 15 && insn->rn == insn->rd && insn->size != 8 && last_write_address) {   // a store of the base register with write-back: the value read before the base changed
            const std::uint32_t mask = insn->size >= 4 ? 0xFFFFFFFFu : (1u << (8 * insn->size)) - 1;
            emu.busWrite(*last_write_address, insn->size, before_regs[insn->rn] & mask, "ram");
        }
        if (is_thumb_insn && insn->op == da::Op::Ldr && insn->size == 8 && insn->rn == 15) {   // LDRD (literal): the description puts the high word in the first register and the low in the second
            const std::uint64_t first = emu.reg(arm_r[insn->rd]);
            emu.setReg(arm_r[insn->rd], emu.reg(arm_r[insn->rs]));
            emu.setReg(arm_r[insn->rs], first);
        }
        if (odd_half && insn->op == da::Op::Ldr && insn->size == 2 && insn->rd < 15) {   // LDRH / LDRSH from an odd address
            const std::uint32_t result = insn->sign ? static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(*odd_byte))) : *odd_half;
            emu.setReg(arm_r[insn->rd], result);
        }
        // the state after the instruction: BX and loads into the pc say it; a BLX by an immediate changes it
        bool next_thumb = thumb;
        if (alu_pc_write) {   // (ARMv7: ALUWritePC takes the state from bit 0 of the value; the description's SUB to the pc copies the saved status register as the form that returns from an exception does)
            if (emu.reg(arm_status[4]) != cpsr_before) {
                emu.setReg(arm_status[4], cpsr_before);
                mode_switch = (static_cast<std::uint32_t>(emu.pc()) & 1) != 0;
            }
        }
        if (insn->op == da::Op::Bx) next_thumb = emu.reg(arm_status[5]) != 0;   // (on the ARMv4T only BX changes the state, and the description records it in TB: MOV and LDR to the pc ignore bit 0)
        else if (v5 && mode_switch && ((insn->op == da::Op::Ldr && insn->rd == 15) || (insn->op == da::Op::Ldm && (insn->list & 0x8000)) || (insn->op == da::Op::Blx && !insn->target) ||
                                                       (static_cast<unsigned>(insn->op) < 16 && !thumb && bus.arch() >= da::Arch::V7A && insn->rd == 15 && !insn->s)))
            next_thumb = *mode_switch;   // (on the ARMv5 a load into the pc, POP and BLX by a register take the state from bit 0, and from the ARMv7 an ARM data-processing write of the pc)
        else if (insn->op == da::Op::Blx && insn->target) next_thumb = (*insn->target & 1) != 0;   // (BLX by an immediate: to the other state, the decoder's target says which)
        else if (insn->op == da::Op::Bl && insn->target && passes(insn->cond, n, z, c, v) && insn->length == 4 && !thumb && (insn->word >> 28) == 0xF) next_thumb = true;
        else if (thumb && insn->length == 2 && insn->word >> 11 == 0x1D) next_thumb = false;   // BLX (immediate) in Thumb
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
