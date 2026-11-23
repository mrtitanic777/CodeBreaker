// engine/pcode_cpu_w65816.cpp -- the Super Nintendo lane's 65C816 on the one p-code emulator: emulate::w65816::run's call, on languages/65816.sla (see the header).
//
// What the description decides is the instruction set at both widths: the widths are its context variables m16 and x16, which the adapter sets from the M and X flags
// before each instruction (the decoded instructions are kept per context, so a REP or SEP costs nothing). What the adapter carries, as the own core makes it:
//   - the bytes of an instruction are read through the Machine in the own core's order (the opcode, then the operand from its last byte to its first) and put where
//     the emulator decodes from, so a function that runs from a chip's register logs the same reads;
//   - the data accesses are the Machine's (the hook splits a word into its bytes, low first, across a bank as the own core does);
//   - emulation mode: M and X set, the high bytes of X and Y clear, S in page 1 (the description has E as a flag only), and an 8-bit index clears the high bytes;
//   - the program bank stays where it is when the pc runs past the end of the bank (the description carries into the next bank);
//   - BRK, COP and RTI (the vectors and the frame differ with E), the block moves (a whole move is one instruction, X and Y wrap at 8 bits with an 8-bit index) and
//     WAI and STP (the run ends there, with the pc on the instruction).
#include "engine/pcode_cpu_w65816.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>

#include "decompile/engine/pcode_emulator.hh"
#include "decompile/w65816.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_internal.hpp"

namespace cb::engine::pcode_cpu {

namespace w = cb::emulate::w65816;
namespace sl = cb::decompile::sleigh;
namespace dec = cb::decompile::w65816;

namespace {

constexpr const char* kNames[] = {"A", "X", "Y", "SP", "DP", "DB", "N", "V", "MF", "XF", "D", "I", "Z", "C", "E"};
enum Reg { kA, kX, kY, kSP, kDP, kDB, kN, kV, kMF, kXF, kD, kI, kZ, kC, kE };

// The status register as one byte, from the description's one-byte flags.
std::uint8_t status_of(const sl::Emulator& emu, const std::array<sl::Emulator::RegRef, 15>& r) {
    std::uint8_t p = 0;
    p |= emu.reg(r[kN]) ? w::status::N : 0;
    p |= emu.reg(r[kV]) ? w::status::V : 0;
    p |= emu.reg(r[kMF]) ? w::status::M : 0;
    p |= emu.reg(r[kXF]) ? w::status::X : 0;
    p |= emu.reg(r[kD]) ? w::status::D : 0;
    p |= emu.reg(r[kI]) ? w::status::I : 0;
    p |= emu.reg(r[kZ]) ? w::status::Z : 0;
    p |= emu.reg(r[kC]) ? w::status::C : 0;
    return p;
}

void set_status(sl::Emulator& emu, const std::array<sl::Emulator::RegRef, 15>& r, std::uint8_t p) {
    emu.setReg(r[kN], (p & w::status::N) ? 1 : 0);
    emu.setReg(r[kV], (p & w::status::V) ? 1 : 0);
    emu.setReg(r[kMF], (p & w::status::M) ? 1 : 0);
    emu.setReg(r[kXF], (p & w::status::X) ? 1 : 0);
    emu.setReg(r[kD], (p & w::status::D) ? 1 : 0);
    emu.setReg(r[kI], (p & w::status::I) ? 1 : 0);
    emu.setReg(r[kZ], (p & w::status::Z) ? 1 : 0);
    emu.setReg(r[kC], (p & w::status::C) ? 1 : 0);
}

// The instructions that push to the stack and the ones that pull from it (the description's stack macros write a word low byte first, at the address below the pointer).
constexpr bool pushes(std::uint8_t op) {
    return op == 0x48 || op == 0xDA || op == 0x5A || op == 0x0B || op == 0x8B || op == 0x4B || op == 0x08 || op == 0xF4 || op == 0xD4 || op == 0x62 || op == 0x20 || op == 0xFC || op == 0x22;
}
constexpr bool pulls(std::uint8_t op) { return op == 0x68 || op == 0xFA || op == 0x7A || op == 0x2B || op == 0xAB || op == 0x28 || op == 0x60 || op == 0x6B; }

// A stack byte's address: in bank 0, and in page 1 in emulation mode.
constexpr std::uint32_t stack_address(std::uint64_t addr, bool emulation) { return emulation ? 0x100u | (static_cast<std::uint32_t>(addr) & 0xFF) : static_cast<std::uint32_t>(addr) & 0xFFFF; }

// ADC and SBC in decimal mode, as the own core makes them (the description adds in binary: the operand is found again from its result).
struct DecimalResult {
    std::uint16_t value = 0;
    bool c = false, v = false, n = false, z = false;
};

DecimalResult decimal_add(unsigned a, unsigned v, unsigned carry, bool eight) {
    DecimalResult out;
    const unsigned sign = eight ? 0x80 : 0x8000;
    unsigned r = 0, c = carry;
    for (unsigned i = 0; i < (eight ? 2u : 4u); ++i) {
        unsigned s = ((a >> (4 * i)) & 0xF) + ((v >> (4 * i)) & 0xF) + c;
        c = s > 9;
        if (c) s += 6;
        r |= (s & 0xF) << (4 * i);
    }
    out.c = c != 0;
    out.v = (~(a ^ v) & (a ^ r) & sign) != 0;
    out.value = static_cast<std::uint16_t>(r & (eight ? 0xFF : 0xFFFF));
    out.z = out.value == 0;
    out.n = (out.value & sign) != 0;
    return out;
}

DecimalResult decimal_subtract(unsigned a, unsigned v, unsigned carry, bool eight) {
    DecimalResult out;
    const unsigned sign = eight ? 0x80 : 0x8000;
    unsigned r = 0;
    int c = static_cast<int>(carry);
    for (unsigned i = 0; i < (eight ? 2u : 4u); ++i) {
        int s = static_cast<int>((a >> (4 * i)) & 0xF) - static_cast<int>((v >> (4 * i)) & 0xF) - (1 - c);
        c = s >= 0;
        if (!c) s -= 6;
        r |= (static_cast<unsigned>(s) & 0xF) << (4 * i);
    }
    out.c = c != 0;
    out.v = ((a ^ v) & (a ^ r) & sign) != 0;
    out.value = static_cast<std::uint16_t>(r & (eight ? 0xFF : 0xFFFF));
    out.z = out.value == 0;
    out.n = (out.value & sign) != 0;
    return out;
}

// ADC (true) or SBC (false) by its opcode, else nothing.
std::optional<bool> adds(std::uint8_t op) {
    switch (op) {
        case 0x69: case 0x65: case 0x75: case 0x6D: case 0x7D: case 0x79: case 0x61: case 0x71: case 0x72: case 0x67: case 0x77: case 0x6F: case 0x7F: case 0x63: case 0x73: return true;
        case 0xE9: case 0xE5: case 0xF5: case 0xED: case 0xFD: case 0xF9: case 0xE1: case 0xF1: case 0xF2: case 0xE7: case 0xF7: case 0xEF: case 0xFF: case 0xE3: case 0xF3: return false;
        default: return std::nullopt;
    }
}

}  // namespace

w::Run run(w::Machine& machine, std::uint32_t entry, const w::Options& options) {
    sl::Emulator& emu = emulator_named("65816.sla");
    const auto& r = reg_refs(emu, kNames);
    const std::size_t log_start = machine.log().size();
    // the bytes the emulator holds for the code run so far (a run's pc lands on the same instructions again and again: only a byte that differs is handed over). Reset with each
    // run, as other users of this emulator may have written its memory since.
    thread_local std::array<std::uint32_t, 4096> held_at;
    thread_local std::array<std::array<std::uint8_t, 4>, 4096> held_bytes;
    thread_local std::array<std::uint8_t, 4096> held_size;   // (the bytes written at the pc: a longer instruction there later is written again)
    held_at.fill(0xFFFFFFFFu);
    thread_local const sl::Emulator* configured = nullptr;
    if (configured != &emu) {   // (once for the emulator: the widths are set here, from the flags, so no REP or SEP of an earlier run's flow decides how code is read; what is decoded stays for the next run)
        emu.keepAddressContext(false);
        configured = &emu;
    }
    emu.clearStops();

    // every data access is the machine's: a word is its bytes, low first, each at the next address of the 24-bit space; the stack is in bank 0 and, in emulation
    // mode, in page 1 (the addresses wrap there), and a push writes the high byte of a word first
    std::uint8_t current_opcode = 0;
    bool in_emulation = false;
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        value = 0;
        if (pulls(current_opcode)) {
            for (unsigned i = 0; i < size; ++i) value |= static_cast<std::uint64_t>(machine.read(stack_address(addr + i, in_emulation))) << (8 * i);
            return true;
        }
        for (unsigned i = 0; i < size; ++i) value |= static_cast<std::uint64_t>(machine.read(static_cast<std::uint32_t>(addr + i) & 0xFFFFFF)) << (8 * i);
        return true;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!is_ram(space)) return false;
        if (pushes(current_opcode)) {
            for (unsigned i = size; i-- > 0;) machine.write(stack_address(addr + i, in_emulation), static_cast<std::uint8_t>(value >> (8 * i)));
            return true;
        }
        for (unsigned i = 0; i < size; ++i) machine.write(static_cast<std::uint32_t>(addr + i) & 0xFFFFFF, static_cast<std::uint8_t>(value >> (8 * i)));
        return true;
    });
    struct Unhook {
        sl::Emulator& e;
        ~Unhook() {
            e.onRead({});
            e.onWrite({});
        }
    } unhook{emu};

    // the registers as the own core starts a function: the flags, the widths, the frame a JSL leaves
    const bool emulation = !options.native;
    const bool m8 = emulation || !options.m16, x8 = emulation || !options.x16;
    entry &= 0xFFFFFF;
    emu.setReg(r[kA], options.a);
    emu.setReg(r[kX], x8 ? (options.x & 0xFF) : options.x);
    emu.setReg(r[kY], x8 ? (options.y & 0xFF) : options.y);
    emu.setReg(r[kDP], 0);
    emu.setReg(r[kDB], entry >> 16);
    set_status(emu, r, static_cast<std::uint8_t>(w::status::I | (m8 ? w::status::M : 0) | (x8 ? w::status::X : 0)));
    emu.setReg(r[kE], emulation ? 1 : 0);
    {
        std::uint16_t s = 0x01FF;
        machine.write(s, static_cast<std::uint8_t>(entry >> 16));
        --s;
        const std::uint16_t ret = w::kReturnAddress - 1;
        machine.write(s, static_cast<std::uint8_t>(ret >> 8));
        --s;
        machine.write(s, static_cast<std::uint8_t>(ret));
        --s;
        emu.setReg(r[kSP], s);
    }
    emu.setPc(entry);
    bool flag_e = emulation, flag_m = m8, flag_x = x8;   // E, M and X as the last instruction that could change them left them (read there, not at the start of every instruction)
    bool wide_a = !m8, wide_x = !x8;   // the widths the emulator decodes in now
    emu.setContext("m16", wide_a ? 1 : 0);
    emu.setContext("x16", wide_x ? 1 : 0);

    w::Run out;
    out.end = w::End::Limit;
    std::uint64_t steps = 0;
    while (steps < options.max_steps) {
        const std::uint32_t pc24 = static_cast<std::uint32_t>(emu.pc()) & 0xFFFFFF;
        const std::uint16_t pc16 = static_cast<std::uint16_t>(pc24);
        if (pc16 == w::kReturnAddress) {
            out.end = w::End::Return;
            break;
        }
        const bool emulation_now = flag_e;
        const bool a8 = emulation_now || flag_m, i8 = emulation_now || flag_x;
        if (a8 == wide_a || i8 == wide_x) {   // (the flags and the widths the emulator decodes in differ)
            if (a8 == wide_a) emu.setContext("m16", (wide_a = !a8) ? 1 : 0);
            if (i8 == wide_x) emu.setContext("x16", (wide_x = !i8) ? 1 : 0);
        }
        // the instruction's bytes as the own core reads them, to where the emulator decodes from
        const std::uint32_t bank = pc24 & 0xFF0000;
        std::uint8_t code[4] = {static_cast<std::uint8_t>(machine.read(pc24)), 0, 0, 0};
        const dec::OpcodeEntry& entry_of = dec::opcode_table()[code[0]];
        const std::uint8_t size = dec::instruction_size(entry_of.mode, !a8, !i8);
        for (unsigned i = size; i-- > 1;) code[i] = machine.read(bank | ((pc16 + i) & 0xFFFF));
        {
            const std::size_t slot = (pc24 ^ pc24 >> 12) & 4095;
            if (held_at[slot] != pc24 || held_size[slot] != size || held_bytes[slot][1] != code[1] || held_bytes[slot][2] != code[2] || held_bytes[slot][3] != code[3] || held_bytes[slot][0] != code[0]) {
                for (std::uint32_t neighbor = pc24 - 3; neighbor != pc24 + 4; ++neighbor) {   // (the instructions held at addresses this write overlaps are no longer what the emulator holds)
                    const std::size_t other = ((neighbor & 0xFFFFFF) ^ (neighbor & 0xFFFFFF) >> 12) & 4095;
                    if (held_at[other] == (neighbor & 0xFFFFFF)) held_at[other] = 0xFFFFFFFFu;
                }
                held_at[slot] = pc24;
                held_size[slot] = size;
                std::memcpy(held_bytes[slot].data(), code, 4);
                emu.write(pc24, code, size);
            }
        }
        ++steps;
        const std::uint8_t opcode = code[0];
        std::uint32_t operand = 0;
        for (unsigned i = size; i-- > 1;) operand = operand << 8 | code[i];

        if (opcode == 0xCB || opcode == 0xDB) {   // WAI, STP
            out.end = w::End::Stopped;
            out.fault = w::Fault{opcode == 0xCB ? "WAI" : "STP", pc24};
            break;
        }
        if (opcode == 0x00 || opcode == 0x02 || opcode == 0x40) {   // BRK, COP, RTI
            std::uint16_t s = static_cast<std::uint16_t>(emu.reg(r[kSP]));
            std::uint8_t p = status_of(emu, r);
            std::uint32_t next_bank = bank >> 16;
            std::uint16_t next_pc = 0;
            const auto push = [&](std::uint8_t v) {
                machine.write(s, v);
                s = emulation_now ? static_cast<std::uint16_t>(0x0100 | ((s - 1) & 0xFF)) : static_cast<std::uint16_t>(s - 1);
            };
            const auto pull = [&]() -> std::uint8_t {
                s = emulation_now ? static_cast<std::uint16_t>(0x0100 | ((s + 1) & 0xFF)) : static_cast<std::uint16_t>(s + 1);
                return machine.read(s);
            };
            if (opcode == 0x40) {
                p = pull();
                const std::uint8_t lo = pull();
                next_pc = static_cast<std::uint16_t>(lo | pull() << 8);
                if (emulation_now) p |= w::status::M | w::status::X;
                else next_bank = pull();
            } else {
                const std::uint16_t after = static_cast<std::uint16_t>(pc16 + 2);
                if (!emulation_now) push(static_cast<std::uint8_t>(next_bank));
                push(static_cast<std::uint8_t>(after >> 8));
                push(static_cast<std::uint8_t>(after));
                push(emulation_now ? static_cast<std::uint8_t>(p | w::status::M | w::status::X) : p);
                p = static_cast<std::uint8_t>((p | w::status::I) & ~w::status::D);
                next_bank = 0;
                const bool brk = opcode == 0x00;
                const std::uint32_t vector = emulation_now ? (brk ? 0xFFFE : 0xFFF4) : (brk ? 0xFFE6 : 0xFFE4);
                const std::uint8_t lo = machine.read(vector);
                next_pc = static_cast<std::uint16_t>(lo | machine.read(vector + 1) << 8);
            }
            set_status(emu, r, p);
            emu.setReg(r[kSP], s);
            emu.setPc(next_bank << 16 | next_pc);
        } else if (opcode == 0x44 || opcode == 0x54) {   // MVP, MVN: the whole move
            const std::uint32_t destination = (operand & 0xFF) << 16, source = (operand >> 8 & 0xFF) << 16;
            std::uint16_t x = static_cast<std::uint16_t>(emu.reg(r[kX])), y = static_cast<std::uint16_t>(emu.reg(r[kY])), a = static_cast<std::uint16_t>(emu.reg(r[kA]));
            emu.setReg(r[kDB], operand & 0xFF);
            const std::int32_t delta = opcode == 0x54 ? 1 : -1;
            for (unsigned moved = 0; moved < 0x10000; ++moved) {
                machine.write(destination | (i8 ? y & 0xFF : y), machine.read(source | (i8 ? x & 0xFF : x)));
                x = i8 ? static_cast<std::uint16_t>((x + delta) & 0xFF) : static_cast<std::uint16_t>(x + delta);
                y = i8 ? static_cast<std::uint16_t>((y + delta) & 0xFF) : static_cast<std::uint16_t>(y + delta);
                const bool last = a == 0;
                a = static_cast<std::uint16_t>(a - 1);
                if (last) break;
            }
            emu.setReg(r[kX], x);
            emu.setReg(r[kY], y);
            emu.setReg(r[kA], a);
            emu.setPc(bank | static_cast<std::uint16_t>(pc16 + size));
        } else {
            const std::optional<bool> add = adds(opcode);   // (in decimal mode: carried here, the description adds in binary)
            const bool decimal = add && emu.reg(r[kD]) != 0;
            const unsigned a_before = decimal ? static_cast<unsigned>(emu.reg(r[kA])) : 0, carry = decimal ? static_cast<unsigned>(emu.reg(r[kC])) : 0;
            current_opcode = opcode;
            in_emulation = emulation_now;
            const sl::Emulator::Status status = emu.step();
            current_opcode = 0;
            if (status == sl::Emulator::Status::BadInstruction || status == sl::Emulator::Status::Error) {
                out.end = w::End::Stopped;
                out.fault = w::Fault{"undecodable", pc24};
                break;
            }
            if (decimal) {
                const unsigned mask = a8 ? 0xFF : 0xFFFF;
                const unsigned a = a_before & mask, binary = static_cast<unsigned>(emu.reg(r[kA])) & mask;
                const unsigned v = (*add ? binary - a - carry : a - binary - (1 - carry)) & mask;   // (the operand, from the binary result)
                const DecimalResult d = *add ? decimal_add(a, v, carry, a8) : decimal_subtract(a, v, carry, a8);
                emu.setReg(r[kA], a8 ? ((a_before & 0xFF00) | d.value) : d.value);
                emu.setReg(r[kC], d.c ? 1 : 0);
                emu.setReg(r[kV], d.v ? 1 : 0);
                emu.setReg(r[kN], d.n ? 1 : 0);
                emu.setReg(r[kZ], d.z ? 1 : 0);
            }
            // the program bank stays across the end of a bank, but for the instructions that name another
            if (opcode != 0x5C && opcode != 0xDC && opcode != 0x22 && opcode != 0x6B) {
                const std::uint32_t now = static_cast<std::uint32_t>(emu.pc()) & 0xFFFFFF;
                if ((now & 0xFF0000) != bank) emu.setPc(bank | (now & 0xFFFF));
            }
        }
        // Only an instruction that changes the mode (REP, SEP, PLP, RTI, XCE, BRK, COP) can leave the flags and the index registers as the description has them and not as the chip does,
        // and one that moves the stack pointer (a push or a pull, TCS, TXS) the stack outside page 1 in emulation mode: the others leave all of them as they were.
        const bool mode_change = opcode == 0xC2 || opcode == 0xE2 || opcode == 0x28 || opcode == 0x40 || opcode == 0xFB || opcode == 0x00 || opcode == 0x02;
        if (mode_change) {
            flag_e = emu.reg(r[kE]) != 0;
            if (flag_e) {
                emu.setReg(r[kMF], 1);
                emu.setReg(r[kXF], 1);
                emu.setReg(r[kSP], 0x0100 | (emu.reg(r[kSP]) & 0xFF));
                flag_m = flag_x = true;
            } else {
                flag_m = emu.reg(r[kMF]) != 0;
                flag_x = emu.reg(r[kXF]) != 0;
            }
            if (flag_x) {
                if (const std::uint64_t x = emu.reg(r[kX]); x > 0xFF) emu.setReg(r[kX], x & 0xFF);
                if (const std::uint64_t y = emu.reg(r[kY]); y > 0xFF) emu.setReg(r[kY], y & 0xFF);
            }
        } else if (flag_e && (pushes(opcode) || pulls(opcode) || opcode == 0x1B || opcode == 0x9A)) {
            emu.setReg(r[kSP], 0x0100 | (emu.reg(r[kSP]) & 0xFF));
        }
    }
    w::Registers& g = out.regs;
    g.a = static_cast<std::uint16_t>(emu.reg(r[kA]));
    g.x = static_cast<std::uint16_t>(emu.reg(r[kX]));
    g.y = static_cast<std::uint16_t>(emu.reg(r[kY]));
    g.s = static_cast<std::uint16_t>(emu.reg(r[kSP]));
    g.d = static_cast<std::uint16_t>(emu.reg(r[kDP]));
    g.dbr = static_cast<std::uint8_t>(emu.reg(r[kDB]));
    g.pc = static_cast<std::uint16_t>(emu.pc());
    g.pbr = static_cast<std::uint8_t>(emu.pc() >> 16);
    g.p = status_of(emu, r);
    g.e = emu.reg(r[kE]) != 0;
    out.steps = steps;
    out.io.assign(machine.log().begin() + static_cast<std::ptrdiff_t>(log_start), machine.log().end());
    return out;
}

w::Run call(w::Machine& machine, std::uint32_t entry, const w::Options& options) { return enabled() ? pcode_cpu::run(machine, entry, options) : w::run(machine, entry, options); }

}  // namespace cb::engine::pcode_cpu
