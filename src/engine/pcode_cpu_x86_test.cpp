// cb-unity: alone (file-local names that other files in a batch also use)
// The x86 engine on the one emulator (engine/pcode_cpu_x86.cpp) against Unicorn, which the WonderSwan lane and the PlayStation 4, PlayStation 5 and Xbox One lanes run on: the
// same single instructions and short programs on both, every register, the memory, the fault and the port accesses compared; and the lanes' homebrew through compare_homebrew.
#include <algorithm>
#include <cstdio>
#include <format>
#include <map>
#include <random>
#include <string>

#include "base/config.hpp"
#include "core/cpu_time.hpp"
#include "core/fs.hpp"
#include "decompile/engine/pcode_emulator.hh"
#include "core/testing.hpp"
#include "emulate/unicorn_uc.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/machine_wonderswan.hpp"
#include "engine/pcode_cpu_compare.hpp"
#include "engine/pcode_cpu_x86.hpp"
#include "formats/orbis.hpp"

namespace {

using namespace cb;
namespace uu = cb::emulate::unicorn_uc;
namespace pc = cb::engine::pcode_cpu;
using u64 = std::uint64_t;

int reg(const char* name) { return *uu::x86_register(name); }

constexpr u64 kCode64 = 0x400000, kData64 = 0x500000, kStack64 = 0x600000, kSize64 = 0x10000;

// What a run left behind, as the lanes compare it.
struct Outcome {
    int fault = 0;   // Unicorn's error number, 0: the run ended
    std::string message;
    std::vector<u64> regs;
    u64 flags = 0;
    u64 ip = 0;
    Bytes memory;
    Bytes vectors;   // XMM0..15 (64-bit)
    std::vector<std::string> io;
    bool operator==(const Outcome& o) const = default;
};

const char* const kRegs64[] = {"RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15"};
const char* const kRegs16[] = {"AX", "BX", "CX", "DX", "SI", "DI", "BP", "SP", "CS", "DS", "ES", "SS"};

struct Scenario {
    int bits = 64;
    Bytes code;                  // at the code address
    Bytes data;                  // at the data address (64-bit) or from the start of memory (16-bit)
    std::vector<u64> regs;       // in kRegs64 / kRegs16 order
    Bytes xmm;                   // XMM0..15 at the start (64-bit)
    u64 count = 1;
    u64 flags = 0x2;
};

Outcome run(const Scenario& s, bool emulator) {
    const bool wide = s.bits == 64;
    std::unique_ptr<core16::Uc> uc = emulator ? pc::open_x86_emulator(s.bits) : uu::open_x86(s.bits);
    Outcome out;
    u64 code_at = 0, until = 0;
    std::vector<std::pair<u64, u64>> watched;
    if (wide) {
        uc->mem_map(kCode64, kSize64);
        uc->mem_map(kData64, kSize64);
        uc->mem_map(kStack64, kSize64);
        uc->mem_write(kCode64, s.code);
        uc->mem_write(kData64, s.data);
        code_at = kCode64;
        until = 0x7FFFFFF0;
        watched = {{kData64, kSize64}, {kStack64, kSize64}};
        for (std::size_t i = 0; i < 16; ++i) uc->reg_write(reg(kRegs64[i]), s.regs[i]);
        uc->reg_write(reg("FS_BASE"), kData64 + 0x800);
        uc->reg_write(reg("GS_BASE"), kData64 + 0x1000);
        for (unsigned i = 0; i < 16 && s.xmm.size() >= 256; ++i) pc::write_xmm(*uc, i, ByteView(s.xmm).subspan(i * 16, 16));
    } else {
        uc->mem_map(0, 0x110000);
        uc->mem_write(0, s.data);
        const u64 cs = s.regs[8];
        uc->mem_write((cs << 4) + 0x100, s.code);
        code_at = (cs << 4) + 0x100;
        until = (cs << 4) + 0xFFFE;
        watched = {{0, 0x10000}};
        for (std::size_t i = 0; i < 12; ++i) uc->reg_write(reg(kRegs16[i]), s.regs[i]);
    }
    uc->reg_write(reg("EFLAGS"), s.flags);
    uc->hook_add_insn_in([&](core16::Uc&, std::uint32_t port, std::uint32_t size) -> std::uint32_t {
        const std::uint32_t value = (port * 37 + size) & 0xFF;
        out.io.push_back(std::format("in {:x} {} {:x}", port, size, value));
        return value;
    });
    uc->hook_add_insn_out([&](core16::Uc&, std::uint32_t port, std::uint32_t size, std::uint32_t value) { out.io.push_back(std::format("out {:x} {} {:x}", port, size, value)); });
    try {
        uc->emu_start(code_at, until, 0, s.count);
    } catch (const uu::UcFault& f) {
        out.fault = f.code();
        out.message = f.what();
    }
    for (std::size_t i = 0; i < (wide ? 16u : 12u); ++i) out.regs.push_back(uc->reg_read(reg(wide ? kRegs64[i] : kRegs16[i])));
    out.flags = uc->reg_read(reg("EFLAGS")) & (wide ? 0xFFFFFFFFull : 0x8D5ull);
    if (wide) {
        out.ip = uc->reg_read(reg("RIP"));
    } else {
        out.ip = (uc->reg_read(reg("CS")) << 4) + uc->reg_read(reg("IP"));
    }
    if (wide)
        for (unsigned i = 0; i < 16; ++i) {
            const Bytes v = pc::read_xmm(*uc, i);
            out.vectors.insert(out.vectors.end(), v.begin(), v.end());
        }
    for (const auto& [a, n] : watched) {
        const Bytes b = uc->mem_read(a, n);
        out.memory.insert(out.memory.end(), b.begin(), b.end());
    }
    return out;
}

// What differs between two outcomes, as text (empty: nothing). Flags are told apart: `flags_only` is set when nothing else differs.
std::string describe(const Outcome& a, const Outcome& b, bool wide, bool& flags_only) {
    std::string out;
    flags_only = false;
    if (a.fault != b.fault) out += std::format("fault(unicorn {} / emulator {}) ", a.fault, b.fault);
    else if (a.message != b.message) out += "message ";
    for (std::size_t i = 0; i < a.regs.size(); ++i)
        if (a.regs[i] != b.regs[i]) out += std::format("{}[uc {:X} emu {:X}] ", wide ? kRegs64[i] : kRegs16[i], a.regs[i], b.regs[i]);
    if (a.ip != b.ip) out += std::format("ip[{:X}/{:X}] ", a.ip, b.ip);
    if (a.vectors != b.vectors)
        for (std::size_t i = 0; i < a.vectors.size(); ++i)
            if (a.vectors[i] != b.vectors[i]) {
                out += std::format("xmm{}.{}[uc {:02X} emu {:02X}] ", i / 16, i % 16, a.vectors[i], b.vectors[i]);
                break;
            }
    if (a.io != b.io) out += "io ";
    if (a.memory != b.memory) {
        for (std::size_t i = 0; i < a.memory.size(); ++i)
            if (a.memory[i] != b.memory[i]) {
                out += std::format("mem@{:X}[uc {:02X} emu {:02X}] ", i, a.memory[i], b.memory[i]);
                break;
            }
    }
    if (out.empty() && a.flags != b.flags) {
        out += std::format("flags[uc {:X} emu {:X} xor {:X}]", a.flags, b.flags, a.flags ^ b.flags);
        flags_only = true;
    }
    return out;
}

// The instruction's key: its opcode after the prefixes (and the ModRM register field for the groups).
std::string key_of(const Bytes& code, bool wide) {
    std::size_t i = 0;
    while (i < 6) {
        const unsigned b = code[i];
        if (b == 0x66 || b == 0x67 || b == 0xF2 || b == 0xF3 || b == 0x26 || b == 0x2E || b == 0x36 || b == 0x3E || b == 0x64 || b == 0x65 || (wide && b >= 0x40 && b <= 0x4F)) ++i;
        else break;
    }
    const unsigned op = code[i];
    if (op == 0x0F) return std::format("0f {:02x}", code[i + 1]);
    if (op == 0xC4 || op == 0xC5 || op == 0x62) return std::format("{:02x}(vex)", op);
    if (op == 0x80 || op == 0x81 || op == 0x83 || op == 0xC0 || op == 0xC1 || (op >= 0xD0 && op <= 0xD3) || op == 0xF6 || op == 0xF7 || op == 0xFE || op == 0xFF || (op >= 0xD8 && op <= 0xDF) || op == 0x8F || op == 0xC6 || op == 0xC7)
        return std::format("{:02x} /{}", op, (code[i + 1] >> 3) & 7);
    return std::format("{:02x}", op);
}

std::string hex_of(const Bytes& b, std::size_t n) {
    std::string s;
    for (std::size_t i = 0; i < n && i < b.size(); ++i) s += std::format("{:02X}", b[i]);
    return s;
}

// The flags the instruction at the start of `code` leaves undefined (or, for AF, Unicorn computes and the description does not): a difference in them is not a difference in the
// instruction. AF is that for every instruction.
u64 undefined_flags(const Bytes& code, bool wide) {
    std::size_t i = 0;
    while (i < 6) {
        const unsigned b = code[i];
        if (b == 0x66 || b == 0x67 || b == 0xF2 || b == 0xF3 || b == 0x26 || b == 0x2E || b == 0x36 || b == 0x3E || b == 0x64 || b == 0x65 || b == 0xF0 || (wide && b >= 0x40 && b <= 0x4F)) ++i;
        else break;
    }
    const unsigned op = code[i], reg = (code[i + 1] >> 3) & 7, next = code[i + 1];
    u64 mask = 0x10;
    if (op == 0xC0 || op == 0xC1 || (op >= 0xD0 && op <= 0xD3)) mask |= 0x800;
    if (op == 0xF6 || op == 0xF7) mask |= reg == 4 || reg == 5 ? 0xC4 : reg >= 6 ? 0x8C5 : 0;
    if (op == 0x69 || op == 0x6B) mask |= 0xC4;
    if (op == 0xD4 || op == 0xD5) mask |= 0x8C5;
    if (op == 0x0F) {
        if (next == 0xAF) mask |= 0xC4;
        if (next == 0xA3 || next == 0xAB || next == 0xB3 || next == 0xBB || next == 0xBA) mask |= 0x8C4;
        if (next == 0xBC || next == 0xBD) mask |= 0x8C5;
        if (next == 0xA4 || next == 0xA5 || next == 0xAC || next == 0xAD) mask |= 0x800;
    }
    return mask;
}

// The start of an instruction of the V30MZ (the 8086's and the 80186's: no 386 prefix, no 0F page, no coprocessor), which is what a WonderSwan program holds: an optional segment
// override or REP prefix and an opcode of that set, the rest of `code` as it is (random).
void v30_instruction(std::mt19937& rng, Bytes& code) {
    std::size_t at = 0;
    const unsigned prefixes = rng() % 5 == 0 ? 1 : 0;
    const std::uint8_t allowed[] = {0x26, 0x2E, 0x36, 0x3E, 0xF2, 0xF3};
    for (unsigned i = 0; i < prefixes; ++i) code[at++] = allowed[rng() % 6];
    unsigned op;
    do op = static_cast<unsigned>(rng() & 0xFF);
    while (op == 0x0F || (op >= 0x63 && op <= 0x67) || op == 0xF1 || op == 0xF0 || op == 0xD6 || (op >= 0xD8 && op <= 0xDF) || op == 0x26 || op == 0x2E || op == 0x36 || op == 0x3E || op == 0xF2 || op == 0xF3);
    code[at] = static_cast<std::uint8_t>(op);
}

// A straight-line program of valid instructions: each is random bytes that Unicorn runs without a fault on registers that point into the data, and that leave the program counter after
// themselves; the program is those instructions' bytes one after the other (so a run of it goes on to the end instead of stopping at the first undefined byte).
Bytes valid_program(std::mt19937& rng, int bits, std::size_t instructions) {
    const bool wide = bits == 64;
    Bytes program;
    for (unsigned tries = 0; program.size() < instructions * 4 && tries < instructions * 40; ++tries) {
        Scenario t;
        t.bits = bits;
        t.code.resize(16);
        for (std::uint8_t& b : t.code) b = static_cast<std::uint8_t>(rng());
        if (!wide) v30_instruction(rng, t.code);
        t.count = 1;
        t.data.assign(wide ? kSize64 : 0x10000, 0x11);
        if (wide) {
            for (std::size_t i = 0; i < 16; ++i) t.regs.push_back(kData64 + 0x2000 + 0x100 * i);
            t.regs[7] = kStack64 + 0x8000;
            t.regs[2] = 3;
        } else {
            for (std::size_t i = 0; i < 8; ++i) t.regs.push_back(0x2000 + 0x100 * i);
            t.regs[7] = 0x3F00;
            t.regs[2] = 3;
            t.regs.insert(t.regs.end(), {0x1000, 0, 0, 0});
        }
        bool clock = false;   // (the time stamp and the random number differ from one Unicorn run to the next: not for a comparison)
        for (std::size_t k = 0; k + 1 < 7; ++k)
            if (t.code[k] == 0x0F && (t.code[k + 1] == 0x31 || t.code[k + 1] == 0xC7 || t.code[k + 1] == 0x01)) clock = true;
        // Unicorn's decoder skips a VEX prefix (C5 xx, C4 xx xx) and runs what follows as the one-byte-escape 0F page: so VEX opcode 31 is its RDTSC too, and as unrepeatable.
        if (wide)
            for (std::size_t k = 0; k + 3 < 7; ++k)
                if ((t.code[k] == 0xC5 && t.code[k + 2] == 0x31) || (t.code[k] == 0xC4 && t.code[k + 3] == 0x31)) clock = true;
        if (clock) continue;
        const Outcome o = run(t, false);
        const u64 base = wide ? kCode64 : (t.regs[8] << 4) + 0x100;
        if (o.fault != 0 || !o.io.empty() || o.ip <= base || o.ip - base > 15) continue;
        program.insert(program.end(), t.code.begin(), t.code.begin() + static_cast<std::ptrdiff_t>(o.ip - base));
    }
    program.resize(std::max<std::size_t>(program.size(), 96), 0x90);
    return program;
}

Scenario random_scenario(std::mt19937& rng, int bits, bool single) {
    Scenario s;
    s.bits = bits;
    const bool wide = bits == 64;
    s.count = single ? 1 : 24;
    s.code.resize(single ? 16 : 96);
    for (std::uint8_t& b : s.code) b = static_cast<std::uint8_t>(rng());
    if (!single) s.code = valid_program(rng, bits, 24);
    if (!wide && single) v30_instruction(rng, s.code);
    if (wide) {
        s.data.resize(kSize64);
        for (std::size_t i = 0; i < s.data.size(); ++i) s.data[i] = static_cast<std::uint8_t>(rng());
        const auto pointer = [&] { return kData64 + 0x2000 + (rng() & 0x3FF8); };
        for (std::size_t i = 0; i < 16; ++i) {
            const unsigned kind = rng() % 6;
            s.regs.push_back(kind < 3 ? pointer() : kind == 3 ? (static_cast<u64>(rng()) << 32 | rng()) : kind == 4 ? rng() % 40 : static_cast<u64>(-static_cast<std::int64_t>(rng() % 40)));
        }
        s.regs[7] = kStack64 + 0x8000 + (rng() & 0xFF8);
        s.xmm.resize(256);
        for (std::uint8_t& b : s.xmm) b = static_cast<std::uint8_t>(rng());
        if ((rng() & 7) == 0) s.regs[2] = rng() % 5;
        s.flags = 0x2 | (rng() & 0xCD5);
    } else {
        s.data.resize(0x10000);
        for (std::size_t i = 0x400; i < s.data.size(); ++i) s.data[i] = static_cast<std::uint8_t>(rng());
        const auto pointer = [&] { return 0x1000 + (rng() & 0x2FFF); };
        for (std::size_t i = 0; i < 8; ++i) {
            const unsigned kind = rng() % 6;
            s.regs.push_back(kind < 3 ? pointer() : kind == 3 ? (rng() & 0xFFFF) : kind == 4 ? rng() % 40 : static_cast<u64>(0x10000 - rng() % 40) & 0xFFFF);
        }
        s.regs[7] = 0x3F00 + (rng() & 0xE) ;
        if ((rng() & 7) == 0) s.regs[2] = rng() % 5;
        const u64 segs[] = {0, 0x1000, 0x2000, 0xF000};
        s.regs.push_back(segs[rng() % 4 == 0 ? 2 : 1]);   // CS (the code's own 64 KB)
        s.regs.push_back(0);                               // DS
        s.regs.push_back(rng() % 3 == 0 ? 0x1000 : 0);     // ES
        s.regs.push_back(0);                               // SS
        s.flags = 0x2 | (rng() & 0xCD5);
    }
    return s;
}

void compare_random(const char* title, int bits, bool single) {
    std::mt19937 rng(20261122 + bits);
    const bool wide = bits == 64;
    unsigned total = single ? 20000u : 2000u;
    if (const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE"); given && !given->empty() && *given != "0") total = static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10));
    std::map<std::string, unsigned> by_kind, flag_kind, flag_other;   // (flag_other: the differences that are not AF alone)
    std::map<std::string, u64> flag_bits;
    std::map<std::string, std::string> example, flag_example;
    pc::x86_stats().clear();
    unsigned runs = 0, different = 0, flags_differ = 0, excused_runs = 0, garbage_runs = 0;
    for (unsigned n = 0; n < total; ++n) {
        const Scenario s = random_scenario(rng, bits, single);
        const Outcome a = run(s, false), b = run(s, true);
        ++runs;
        if (a == b) continue;
        bool flags_only = false;
        const std::string fields = describe(a, b, wide, flags_only);
        std::string key = (single ? "1: " : "N: ") + key_of(s.code, wide);
        Bytes shown = s.code;
        bool excused = false;
        if (!single) {   // the first instruction after which the two differ: the one to blame (a difference in flags the processor leaves undefined, or AF, is Unicorn's to choose, and excused)
            Scenario t = s;
            for (u64 upto = 1; upto <= s.count; ++upto) {
                t.count = upto;
                const Outcome x = run(t, false), y = run(t, true);
                if (x == y) continue;
                bool only_flags = false;
                const std::string first = describe(x, y, wide, only_flags);
                if (only_flags) {
                    Scenario u = s;
                    u.count = upto - 1;
                    const Outcome before = upto == 1 ? Outcome{} : run(u, false);
                    const u64 where = upto == 1 ? (wide ? kCode64 : (s.regs[8] << 4) + 0x100) : before.ip;
                    const u64 base0 = wide ? kCode64 : (s.regs[8] << 4) + 0x100;
                    if (where < base0 || where >= base0 + s.code.size()) {   // (the program ran out of its own bytes into memory: random data, with the 386 instructions Unicorn knows and a V30MZ does not)
                        excused = true;
                        ++garbage_runs;
                        break;
                    }
                    if (where >= base0 && where - base0 + 16 <= s.code.size()) {
                        const Bytes at_insn(s.code.begin() + static_cast<std::ptrdiff_t>(where - base0), s.code.end());
                        if (((x.flags ^ y.flags) & ~undefined_flags(at_insn, wide)) == 0) {
                            excused = true;
                            break;
                        }
                    }
                    break;
                }
                t.count = upto - 1;
                const Outcome before = upto == 1 ? Outcome{} : run(t, false);
                const u64 base = wide ? kCode64 : (s.regs[8] << 4) + 0x100;
                const u64 at = upto == 1 ? base : before.ip;
                if (!wide && (at < base || at >= base + s.code.size())) {   // (as above: not the program's instruction)
                    excused = true;
                    ++garbage_runs;
                    break;
                }
                if (at >= base && at - base + 16 <= s.code.size()) {
                    shown.assign(s.code.begin() + static_cast<std::ptrdiff_t>(at - base), s.code.end());
                    key = std::format("N@{}: ", upto) + key_of(shown, wide);
                }
                break;
            }
        }
        if (excused) {
            ++excused_runs;
            continue;
        }
        if (const auto dir = fs::get_env("CB_X86_SAVE_DIR"); dir && !dir->empty()) {   // (the scenario kept, for CB_X86_REPLAY)
            Bytes blob;
            const auto put64 = [&](u64 v) {
                for (int k = 0; k < 8; ++k) blob.push_back(static_cast<std::uint8_t>(v >> (8 * k)));
            };
            put64(static_cast<u64>(s.bits));
            put64(s.count);
            put64(s.flags);
            put64(s.regs.size());
            for (const u64 r : s.regs) put64(r);
            put64(s.code.size());
            blob.insert(blob.end(), s.code.begin(), s.code.end());
            put64(s.data.size());
            blob.insert(blob.end(), s.data.begin(), s.data.end());
            put64(s.xmm.size());
            blob.insert(blob.end(), s.xmm.begin(), s.xmm.end());
            fs::write_file(fs::from_text(*dir) / std::format("scenario_{}.bin", n), blob);
        }
        {   // (is the difference the same when it is run again?)
            const Outcome a2 = run(s, false), b2 = run(s, true);
            if (!(a2 == a) || !(b2 == b))
                std::printf("  NOT REPEATABLE %s: unicorn again %s, emulator again %s: code %s\n", key.c_str(), a2 == a ? "same" : "different", b2 == b ? "same" : "different", hex_of(s.code, 40).c_str());
            else if (!single)
                std::printf("  repeatable %s: code %s\n", key.c_str(), hex_of(s.code, 40).c_str());
        }
        if (const auto show = fs::get_env("CB_X86_SHOW"); show && !show->empty() && key.find(*show) != std::string::npos && !flags_only) {
            std::printf("SHOW %s code %s\n  start:", key.c_str(), hex_of(s.code, 96).c_str());
            for (std::size_t i = 0; i < s.regs.size(); ++i) std::printf(" %s=%llX", wide ? kRegs64[i] : kRegs16[i], static_cast<unsigned long long>(s.regs[i]));
            std::printf(" flags=%llX\n  unicorn: fault %d ip %llX", static_cast<unsigned long long>(s.flags), a.fault, static_cast<unsigned long long>(a.ip));
            for (std::size_t i = 0; i < a.regs.size(); ++i) std::printf(" %s=%llX", wide ? kRegs64[i] : kRegs16[i], static_cast<unsigned long long>(a.regs[i]));
            std::printf(" flags=%llX\n  emulator: fault %d ip %llX", static_cast<unsigned long long>(a.flags), b.fault, static_cast<unsigned long long>(b.ip));
            for (std::size_t i = 0; i < b.regs.size(); ++i) std::printf(" %s=%llX", wide ? kRegs64[i] : kRegs16[i], static_cast<unsigned long long>(b.regs[i]));
            std::printf(" flags=%llX\n", static_cast<unsigned long long>(b.flags));
            for (const std::string& line : a.io) std::printf("  unicorn io: %s\n", line.c_str());
            for (const std::string& line : b.io) std::printf("  emulator io: %s\n", line.c_str());
        }
        if (flags_only) {
            ++flags_differ;
            ++flag_kind[key];
            flag_bits[key] |= a.flags ^ b.flags;
            if (((a.flags ^ b.flags) & ~u64{0x10}) != 0) ++flag_other[key];
            if (!flag_example.count(key)) flag_example[key] = hex_of(shown, 10) + ": " + fields;
        } else {
            ++different;
            ++by_kind[key];
            if (!example.count(key)) example[key] = hex_of(shown, 10) + ": " + fields;
        }
    }
    std::printf("pcode_cpu_x86 %s: %u runs, %u differ, %u differ in flags only, %u excused (a flag the processor leaves undefined, or AF, differed first; %u ran out of the program into random memory)\n", title, runs, different, flags_differ, excused_runs, garbage_runs);
    unsigned shown = 0;
    for (const auto& [k, c] : by_kind)
        if (++shown <= 60) std::printf("    %u x %s   e.g. %s\n", c, k.c_str(), example[k].c_str());
    shown = 0;
    for (const auto& [k, c] : flag_kind)
        if (flag_other[k] != 0 && ++shown <= 80) std::printf("    flags: %u x %s (%u not AF alone, bits %llX)   e.g. %s\n", c, k.c_str(), flag_other[k], static_cast<unsigned long long>(flag_bits[k]), flag_example[k].c_str());
    std::printf("    on the description: %llu instructions in %llu runs\n", static_cast<unsigned long long>(pc::x86_stats().steps), static_cast<unsigned long long>(pc::x86_stats().runs));
    for (const auto& [k, c] : pc::x86_stats().handoffs) std::printf("    to Unicorn: %s x%llu\n", k.c_str(), static_cast<unsigned long long>(c));
    for (const auto& [k, c] : pc::x86_stats().opcodes) std::printf("    no instruction: %s x%llu\n", k.c_str(), static_cast<unsigned long long>(c));
    for (const auto& [k, c] : pc::x86_stats().unserved) std::printf("    unserved user op: %s x%llu\n", k.c_str(), static_cast<unsigned long long>(c));
    CB_CHECK_EQ(different, 0u);
}

}  // namespace

CB_TEST("engine.pcode_cpu.x86_64_instructions_run_as_unicorn_does", [] { compare_random("64-bit instructions", 64, true); });
CB_TEST("engine.pcode_cpu.x86_64_programs_run_as_unicorn_does", [] { compare_random("64-bit programs", 64, false); });
CB_TEST("engine.pcode_cpu.x86_16_instructions_run_as_unicorn_does", [] { compare_random("16-bit instructions", 16, true); });
CB_TEST("engine.pcode_cpu.x86_16_programs_run_as_unicorn_does", [] { compare_random("16-bit programs", 16, false); });


// Every two-byte opcode with each legacy prefix and a register form and a memory form of the ModRM byte, on random registers and vector registers: the vector instructions one
// by one (a random byte string reaches each of the thousand only now and then).
CB_TEST("engine.pcode_cpu.x86_64_two_byte_opcodes_run_as_unicorn_does", [] {
    std::mt19937 rng(77);
    std::map<std::string, unsigned> bad;
    std::map<std::string, std::string> example;
    pc::x86_stats().clear();
    unsigned runs = 0;
    const unsigned rounds = [] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 4u;
    }();
    for (const unsigned prefix : {0x00u, 0x66u, 0xF3u, 0xF2u})
        for (unsigned op = 0x10; op <= 0xFF; ++op)
            for (const unsigned modrm : {0xC1u, 0xD3u, 0x4Au, 0x02u})
                if (op != 0x31 && op != 0xC7)   // (the time stamp and the random number: not the same twice)
                for (unsigned round = 0; round < rounds; ++round) {
                    Scenario s = random_scenario(rng, 64, true);
                    Bytes code;
                    if (prefix != 0) code.push_back(static_cast<std::uint8_t>(prefix));
                    code.push_back(0x0F);
                    code.push_back(static_cast<std::uint8_t>(op));
                    code.push_back(static_cast<std::uint8_t>(modrm));
                    code.push_back(static_cast<std::uint8_t>(rng() % 8));   // (a displacement or an immediate)
                    for (std::size_t i = 0; i < code.size(); ++i) s.code[i] = code[i];
                    if (modrm == 0x02 || modrm == 0x4A) {
                        s.regs[2] = kData64 + 0x2000 + (rng() & 0x3FF0);    // RCX, RDX the memory operands (aligned)
                        s.regs[3] = kData64 + 0x2000 + (rng() & 0x3FF0);
                    }
                    const Outcome a = run(s, false), b = run(s, true);
                    ++runs;
                    if (a == b) continue;
                    bool flags_only = false;
                    const std::string fields = describe(a, b, true, flags_only);
                    if (flags_only) continue;
                    const std::string key = std::format("{} 0f {:02x}", prefix == 0 ? "--" : std::format("{:02x}", prefix), op);
                    ++bad[key];
                    if (!example.count(key)) example[key] = hex_of(s.code, 6) + ": " + fields;
                }
    std::printf("pcode_cpu_x86 two-byte opcodes: %u runs, %zu opcodes differ\n", runs, bad.size());
    for (const auto& [k, c] : bad) std::printf("    %u x %s   e.g. %s\n", c, k.c_str(), example[k].c_str());
    for (const auto& [k, c] : pc::x86_stats().unserved) std::printf("    unserved user op: %s x%llu\n", k.c_str(), static_cast<unsigned long long>(c));
    CB_CHECK_EQ(bad.size(), 0u);
});

namespace {

// Where in an Orbis ELF a function's first bytes are: its address in the segment that holds it (the lane's keys are the program's own addresses).
std::size_t orbis_offset(const std::filesystem::path& path, std::uint32_t address) {
    try {
        const formats::orbis::Program program = formats::orbis::read(path);
        for (const formats::orbis::Segment& segment : program.segments)
            if (segment.loaded() && address >= segment.address && address - segment.address < segment.file_size) return static_cast<std::size_t>(segment.offset + (address - segment.address));
    } catch (const Error&) {
    }
    return SIZE_MAX;
}

void report_stats(const char* lane) {
    const pc::X86Stats& st = pc::x86_stats();
    std::printf("pcode_cpu_x86 %s: %llu runs started, %llu instructions on the description, %llu on Unicorn after a hand-off (%.1f%% of the instructions)\n", lane, static_cast<unsigned long long>(st.runs),
                static_cast<unsigned long long>(st.steps), static_cast<unsigned long long>(st.tail_steps), 100.0 * static_cast<double>(st.tail_steps) / static_cast<double>(std::max<u64>(1, st.steps + st.tail_steps)));
    for (const auto& [k, c] : st.handoffs) std::printf("    to Unicorn: %s x%llu\n", k.c_str(), static_cast<unsigned long long>(c));
    for (const auto& [k, c] : st.opcodes) std::printf("    no instruction: %s x%llu\n", k.c_str(), static_cast<unsigned long long>(c));
    for (const auto& [k, c] : st.unserved) std::printf("    unserved user op: %s x%llu\n", k.c_str(), static_cast<unsigned long long>(c));
}

}  // namespace

// The lanes' homebrew on both engines: every program's functions in every state of the lane on Unicorn and on the emulator, and each function judged against copies with one of its first
// bytes turned over, the verdicts compared. (An ELF is a plain file: a function's first bytes are where its segment puts them.)
CB_TEST("engine.pcode_cpu.the_playstation_4_lane_runs_homebrew_the_same_on_both_engines", [] {
    pc::x86_stats().clear();
    pc::x86_stats().count_tail = fs::get_env("CB_X86_COUNT_TAIL").value_or("") == "1";   // (a hook on every instruction Unicorn runs: not for the timing)
    engine::pcode_compare::compare_homebrew("ps4", "ps4", ".elf", orbis_offset);
    report_stats("ps4");
});

CB_TEST("engine.pcode_cpu.the_playstation_5_lane_runs_homebrew_the_same_on_both_engines", [] {
    pc::x86_stats().clear();
    pc::x86_stats().count_tail = fs::get_env("CB_X86_COUNT_TAIL").value_or("") == "1";   // (a hook on every instruction Unicorn runs: not for the timing)
    engine::pcode_compare::compare_homebrew("ps5", "ps5", ".elf", orbis_offset);
    report_stats("ps5");
});

// (The Xbox One's programs are in a .msixbundle, a zip whose members are compressed: a function's first bytes are nowhere in the file, so no edited copies are judged; the functions'
// effects are compared.)
CB_TEST("engine.pcode_cpu.the_xbox_one_lane_runs_homebrew_the_same_on_both_engines", [] {
    pc::x86_stats().clear();
    pc::x86_stats().count_tail = fs::get_env("CB_X86_COUNT_TAIL").value_or("") == "1";   // (a hook on every instruction Unicorn runs: not for the timing)
    engine::pcode_compare::compare_homebrew("xboxone", "xboxone", ".msixbundle", [](const std::filesystem::path&, std::uint32_t) -> std::size_t { return SIZE_MAX; });
    report_stats("xboxone");
});

// How fast it goes: a loop of 786,000 instructions on Unicorn, on the adapter, and on the emulator with nothing around it (one step at a time, and in blocks): where the adapter's
// time goes.
CB_TEST("engine.pcode_cpu.x86_64_speed_of_a_loop", [] {
    // xor eax,eax ; L: add rax,1 ; cmp rax,0x40000 ; jb L
    const Bytes code = {0x31, 0xC0, 0x48, 0x83, 0xC0, 0x01, 0x48, 0x3D, 0x00, 0x00, 0x04, 0x00, 0x72, 0xF4};
    double seconds[4] = {0, 0, 0, 0};
    u64 iterations = 0;
    for (int engine = 0; engine < 2; ++engine) {
        std::unique_ptr<core16::Uc> uc = engine != 0 ? pc::open_x86_emulator(64) : uu::open_x86(64);
        uc->mem_map(kCode64, kSize64);
        uc->mem_map(kStack64, kSize64);
        uc->mem_write(kCode64, code);
        uc->reg_write(reg("RSP"), kStack64 + 0x8000);
        const double start = cb::thread_cpu_seconds();
        uc->emu_start(kCode64, kCode64 + code.size(), 0, 2000000);
        seconds[engine] = cb::thread_cpu_seconds() - start;
        iterations = uc->reg_read(reg("RAX"));
    }
    for (int blocks = 0; blocks < 2; ++blocks) {   // the emulator alone
        decompile::sleigh::Emulator emu(config::current().checkout_root() / "src" / "decompile" / "engine" / "languages" / "x86-64.sla",
                                        std::vector<std::pair<std::string, int>>{{"longMode", 1}, {"addrsize", 2}, {"bit64", 1}, {"opsize", 1}});
        emu.write(kCode64, code.data(), code.size());
        emu.setPc(kCode64);
        const double start = cb::thread_cpu_seconds();
        if (blocks == 0) {
            while (emu.pc() != kCode64 + code.size() && emu.step() == decompile::sleigh::Emulator::Status::Ok) {}
        } else {
            while (emu.pc() != kCode64 + code.size() && emu.runBlocks(2000000, true, kCode64 + code.size()).executed != 0) {}
        }
        seconds[2 + blocks] = cb::thread_cpu_seconds() - start;
    }
    std::printf("pcode_cpu_x86 loop of %llu instructions: Unicorn %.3fs, adapter %.3fs, emulator step() %.3fs, emulator runBlocks() %.3fs\n", static_cast<unsigned long long>(iterations * 3), seconds[0], seconds[1],
                seconds[2], seconds[3]);
});

// A far return, a return with a count and an interrupt return to a segment not on a 64 KB boundary (the description leaves the segment out of the last two).
CB_TEST("engine.pcode_cpu.x86_16_far_returns_load_the_segment", [] {
    for (const unsigned opcode : {0xCBu, 0xCAu, 0xCFu}) {
        for (const unsigned cs_new : {0x9521u, 0xFCB9u, 0x2000u, 0x1000u}) {
            Scenario s;
            s.bits = 16;
            s.code = {static_cast<std::uint8_t>(opcode), 0x04, 0x00};   // retf ; retf 4 ; iret
            s.data.assign(0x10000, 0);
            s.data[0x3F00] = 0x8D;
            s.data[0x3F01] = 0x74;   // the new IP, 0x748D
            s.data[0x3F02] = static_cast<std::uint8_t>(cs_new);
            s.data[0x3F03] = static_cast<std::uint8_t>(cs_new >> 8);
            s.regs = {0, 0, 0, 0, 0, 0, 0, 0x3F00, 0x1000, 0, 0, 0};
            const Outcome a = run(s, false), b = run(s, true);
            bool flags_only = false;
            CB_CHECK(describe(a, b, false, flags_only).empty() || flags_only);
            CB_CHECK_EQ(a.ip, b.ip);
        }
    }
});

namespace {

// What the lanes do with the engine: stubs for imported functions (a code hook that sets RAX and may stop the run), a syscall hook, a thread block through FS_BASE.
struct HookedResult {
    std::vector<u64> regs;
    std::vector<std::string> log;
    int fault = 0;
    bool operator==(const HookedResult&) const = default;
};

HookedResult hooked_run(bool emulator, bool stop_at_stub, const Bytes& code) {
    std::unique_ptr<core16::Uc> uc = emulator ? pc::open_x86_emulator(64) : uu::open_x86(64);
    constexpr u64 kStub = 0x70000000;
    uc->mem_map(kCode64, kSize64);
    uc->mem_map(kStack64, kSize64);
    uc->mem_map(kStub, 0x1000);
    uc->mem_map(0x71000000, 0x1000);
    uc->mem_write(kCode64, code);
    uc->mem_write(kStub, Bytes(0x1000, 0xC3));
    uc->reg_write(reg("RSP"), kStack64 + 0x8000);
    uc->reg_write(reg("FS_BASE"), 0x71000800);
    uc->mem_write(0x71000800, Bytes{1, 2, 3, 4, 5, 6, 7, 8});
    HookedResult out;
    uc->hook_add_code(
        [&](core16::Uc& u, u64 address, std::uint32_t) {
            out.log.push_back(std::format("stub {:x} rdi {:x}", address, u.reg_read(reg("RDI"))));
            u.reg_write(reg("RAX"), 0x1234);
            if (stop_at_stub) u.emu_stop();
        },
        kStub, kStub + 0xFF);
    const std::shared_ptr<void> hook = pc::hook_syscall(*uc, [&](core16::Uc& u) {
        out.log.push_back(std::format("syscall {:x}", u.reg_read(reg("RAX"))));
        u.reg_write(reg("RAX"), 0x77);
    });
    try {
        uc->emu_start(kCode64, 0x6F000000, 0, 1000);
    } catch (const uu::UcFault& f) {
        out.fault = f.code();
    }
    for (const char* name : {"RAX", "RBX", "RCX", "RDX", "RSP", "RIP", "R11", "RDI"}) out.regs.push_back(uc->reg_read(reg(name)));
    return out;
}

}  // namespace

CB_TEST("engine.pcode_cpu.x86_64_stubs_syscalls_and_the_thread_block_run_as_on_unicorn", [] {
    // mov rdi,9 ; mov rax,0x70000010 ; call rax ; mov rbx,[fs:0] ; syscall ; mov rcx,5 ; ret (to nowhere)
    const Bytes code = {0x48, 0xC7, 0xC7, 0x09, 0x00, 0x00, 0x00, 0x48, 0xB8, 0x10, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xD0, 0x64, 0x48, 0x8B, 0x1C, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x0F, 0x05, 0x48, 0xC7, 0xC1, 0x05, 0x00, 0x00, 0x00, 0xC3};
    for (const bool stop : {false, true}) {
        const HookedResult a = hooked_run(false, stop, code), b = hooked_run(true, stop, code);
        if (!(a == b)) {
            std::printf("stop=%d unicorn:", stop);
            for (u64 v : a.regs) std::printf(" %llX", static_cast<unsigned long long>(v));
            std::printf(" fault %d log %zu\n  emulator:", a.fault, a.log.size());
            for (u64 v : b.regs) std::printf(" %llX", static_cast<unsigned long long>(v));
            std::printf(" fault %d log %zu\n", b.fault, b.log.size());
        }
        CB_CHECK(a == b);
        CB_CHECK(!a.log.empty());
    }
});

// A saved scenario (CB_X86_SAVE_DIR makes the random tests keep each one that differs) run step by step on both engines, the state after each shown until they differ.
CB_TEST("engine.pcode_cpu.x86_replay_a_saved_scenario", [] {
    const auto path = fs::get_env("CB_X86_REPLAY");
    if (!path || path->empty()) return;
    const Bytes blob = fs::read_file(fs::from_text(*path));
    std::size_t at = 0;
    const auto get64 = [&] {
        u64 v = 0;
        for (int k = 0; k < 8; ++k) v |= static_cast<u64>(blob[at + static_cast<std::size_t>(k)]) << (8 * k);
        at += 8;
        return v;
    };
    Scenario s;
    s.bits = static_cast<int>(get64());
    s.count = get64();
    s.flags = get64();
    for (u64 n = get64(); n > 0; --n) s.regs.push_back(get64());
    const u64 code_size = get64();
    s.code.assign(blob.begin() + static_cast<std::ptrdiff_t>(at), blob.begin() + static_cast<std::ptrdiff_t>(at + code_size));
    at += code_size;
    const u64 data_size = get64();
    s.data.assign(blob.begin() + static_cast<std::ptrdiff_t>(at), blob.begin() + static_cast<std::ptrdiff_t>(at + data_size));
    at += data_size;
    const u64 xmm_size = get64();
    s.xmm.assign(blob.begin() + static_cast<std::ptrdiff_t>(at), blob.begin() + static_cast<std::ptrdiff_t>(at + xmm_size));
    const bool wide = s.bits == 64;
    const u64 total = s.count;
    for (u64 upto = 1; upto <= total; ++upto) {
        s.count = upto;
        const Outcome a = run(s, false), b = run(s, true);
        std::printf("after %llu: unicorn ip %llX flags %llX", static_cast<unsigned long long>(upto), static_cast<unsigned long long>(a.ip), static_cast<unsigned long long>(a.flags));
        for (std::size_t i = 0; i < a.regs.size(); ++i) std::printf(" %s=%llX", wide ? kRegs64[i] : kRegs16[i], static_cast<unsigned long long>(a.regs[i]));
        std::printf("\n          emulator ip %llX flags %llX", static_cast<unsigned long long>(b.ip), static_cast<unsigned long long>(b.flags));
        for (std::size_t i = 0; i < b.regs.size(); ++i) std::printf(" %s=%llX", wide ? kRegs64[i] : kRegs16[i], static_cast<unsigned long long>(b.regs[i]));
        std::printf("\n");
        bool flags_only = false;
        if (!(a == b)) {
            std::printf("  DIFFERENT: %s\n", describe(a, b, wide, flags_only).c_str());
            if (!flags_only) break;
        }
    }
});

// The cause of the one 64-bit program difference that never repeated: C5 5A 31 (a VEX prefix, then opcode 31) is not an AVX instruction on Unicorn, whose decoder reads it as RDTSC,
// so EAX and EDX are the host's clock. Two runs of Unicorn alone on it differ, as they do on the plain RDTSC (0F 31): the random programs leave such instructions out.
CB_TEST("engine.pcode_cpu.x86_64_vex_opcode_31_is_unicorns_clock_and_not_repeatable", [] {
    const auto clock_of = [](Bytes code) {
        Scenario s;
        s.code = std::move(code);
        s.data.assign(kSize64, 0);
        for (std::size_t i = 0; i < 16; ++i) s.regs.push_back(kData64 + 0x2000);
        s.regs[7] = kStack64 + 0x8000;
        const Outcome o = run(s, false);
        CB_CHECK_EQ(o.fault, 0);
        return o.regs[0] | o.regs[3] << 32;   // RAX, RDX
    };
    for (const Bytes& code : {Bytes{0xC5, 0x5A, 0x31, 0x90}, Bytes{0x0F, 0x31, 0x90, 0x90}}) {
        bool differs = false;
        const u64 first = clock_of(code);
        for (unsigned n = 0; n < 200 && !differs; ++n) differs = clock_of(code) != first;
        CB_CHECK(differs);
    }
});

// Instructions with a segment-override prefix, whose AF the adapter works out from the operands.
CB_TEST("engine.pcode_cpu.x86_16_flags_of_arithmetic_with_a_prefix_are_unicorns", [] {
    for (const Bytes& code : {Bytes{0x26, 0x04, 0xC0}, Bytes{0x04, 0xC0}, Bytes{0x2E, 0x26, 0x04, 0xC0}, Bytes{0x40}, Bytes{0x26, 0x40}}) {
        Scenario s;
        s.bits = 16;
        s.code = code;
        s.code.resize(16, 0x90);
        s.data.assign(0x10000, 0);
        s.regs = {0x200B, 0, 0, 0, 0, 0, 0, 0x3F00, 0x2000, 0, 0, 0};
        s.flags = 0x2;
        const Outcome a = run(s, false), b = run(s, true);
        std::printf("code %s: unicorn flags %llX, emulator %llX\n", hex_of(code, 4).c_str(), static_cast<unsigned long long>(a.flags), static_cast<unsigned long long>(b.flags));
        CB_CHECK(a == b);
    }
});

namespace {

// The programs again, in lockstep: both engines run one instruction at a time (emu_start with a count of one, the machine as it stood), everything but the flags compared after each,
// and the flags too (those the instruction does not leave undefined, and AF where the 64-bit machine does not carry it); then the emulator is given Unicorn's flags, so that a
// difference in an undefined flag is not carried into what comes after, and each instruction is judged on its own. The first difference is the instruction's.
struct Lockstep {
    std::unique_ptr<core16::Uc> uc;
    std::shared_ptr<std::vector<std::string>> io = std::make_shared<std::vector<std::string>>();
    u64 begin = 0, until = 0;
    int bits = 64;
};

Lockstep open_lockstep(const Scenario& s, bool emulator) {
    Lockstep l;
    l.bits = s.bits;
    const bool wide = s.bits == 64;
    l.uc = emulator ? pc::open_x86_emulator(s.bits) : uu::open_x86(s.bits);
    core16::Uc& uc = *l.uc;
    if (wide) {
        uc.mem_map(kCode64, kSize64);
        uc.mem_map(kData64, kSize64);
        uc.mem_map(kStack64, kSize64);
        uc.mem_write(kCode64, s.code);
        uc.mem_write(kData64, s.data);
        l.begin = kCode64;
        l.until = 0x7FFFFFF0;
        for (std::size_t i = 0; i < 16; ++i) uc.reg_write(reg(kRegs64[i]), s.regs[i]);
        uc.reg_write(reg("FS_BASE"), kData64 + 0x800);
        uc.reg_write(reg("GS_BASE"), kData64 + 0x1000);
        for (unsigned i = 0; i < 16 && s.xmm.size() >= 256; ++i) pc::write_xmm(uc, i, ByteView(s.xmm).subspan(i * 16, 16));
    } else {
        uc.mem_map(0, 0x110000);
        uc.mem_write(0, s.data);
        const u64 cs = s.regs[8];
        uc.mem_write((cs << 4) + 0x100, s.code);
        l.begin = (cs << 4) + 0x100;
        l.until = (cs << 4) + 0xFFFE;
        for (std::size_t i = 0; i < 12; ++i) uc.reg_write(reg(kRegs16[i]), s.regs[i]);
    }
    uc.reg_write(reg("EFLAGS"), s.flags);
    const std::shared_ptr<std::vector<std::string>> log = l.io;
    uc.hook_add_insn_in([log](core16::Uc&, std::uint32_t port, std::uint32_t size) -> std::uint32_t {
        const std::uint32_t value = (port * 37 + size) & 0xFF;
        log->push_back(std::format("in {:x} {} {:x}", port, size, value));
        return value;
    });
    uc.hook_add_insn_out([log](core16::Uc&, std::uint32_t port, std::uint32_t size, std::uint32_t value) { log->push_back(std::format("out {:x} {} {:x}", port, size, value)); });
    return l;
}

// The registers, the instruction pointer and the vector registers of an engine, as text to compare.
std::string state_of(Lockstep& l, u64& ip) {
    const bool wide = l.bits == 64;
    std::string out;
    for (std::size_t i = 0; i < (wide ? 16u : 12u); ++i) out += std::format("{}={:X} ", wide ? kRegs64[i] : kRegs16[i], l.uc->reg_read(reg(wide ? kRegs64[i] : kRegs16[i])));
    ip = wide ? l.uc->reg_read(reg("RIP")) : (l.uc->reg_read(reg("CS")) << 4) + l.uc->reg_read(reg("IP"));
    out += std::format("ip={:X} ", ip);
    if (wide)
        for (unsigned i = 0; i < 16; ++i) {
            const Bytes v = pc::read_xmm(*l.uc, i);
            for (const std::uint8_t b : v) out += std::format("{:02X}", b);
            out += ' ';
        }
    return out;
}

// An empty string when the program runs alike on both, else what differs and at which instruction.
std::string lockstep_difference(const Scenario& s) {
    const bool wide = s.bits == 64;
    Lockstep u = open_lockstep(s, false), e = open_lockstep(s, true);
    const u64 base = u.begin;
    u64 next = base;   // (the instruction pointer: the engines are given it by emu_start)
    for (u64 step = 1; step <= s.count; ++step) {
        u64 ip_u = 0, ip_e = 0;
        const std::string before_u = state_of(u, ip_u), before_e = state_of(e, ip_e);
        if (before_u != before_e) return std::format("before step {}: states differ", step);
        ip_u = ip_e = next;
        if (ip_u < base || ip_u >= base + s.code.size()) return {};   // (out of the program's own bytes: random memory, not compared)
        Bytes at_insn(s.code.begin() + static_cast<std::ptrdiff_t>(ip_u - base), s.code.end());
        at_insn.resize(16, 0x90);
        int fault_u = 0, fault_e = 0;
        try {
            u.uc->emu_start(ip_u, u.until, 0, 1);
        } catch (const uu::UcFault& f) {
            fault_u = f.code();
        }
        try {
            e.uc->emu_start(ip_e, e.until, 0, 1);
        } catch (const uu::UcFault& f) {
            fault_e = f.code();
        }
        if (fault_u != fault_e) return std::format("step {} ({}): fault {} on Unicorn, {} on the emulator", step, hex_of(at_insn, 8), fault_u, fault_e);
        if (*u.io != *e.io) return std::format("step {} ({}): port accesses differ", step, hex_of(at_insn, 8));
        if (fault_u != 0) return {};
        const std::string after_u = state_of(u, ip_u), after_e = state_of(e, ip_e);
        if (after_u != after_e) return std::format("step {} ({}):\n  unicorn  {}\n  emulator {}", step, hex_of(at_insn, 8), after_u, after_e);
        next = ip_u;
        const u64 flags_u = u.uc->reg_read(reg("EFLAGS")) & (wide ? 0xFFFFFFFFull : 0x8D5ull), flags_e = e.uc->reg_read(reg("EFLAGS")) & (wide ? 0xFFFFFFFFull : 0x8D5ull);
        const u64 excused = wide ? undefined_flags(at_insn, true) : 0;   // (the 16-bit machine matches every flag)
        if (((flags_u ^ flags_e) & ~excused) != 0) return std::format("step {} ({}): flags {:X} on Unicorn, {:X} on the emulator", step, hex_of(at_insn, 8), flags_u, flags_e);
        e.uc->reg_write(reg("EFLAGS"), u.uc->reg_read(reg("EFLAGS")));
    }
    const std::vector<std::pair<u64, u64>> watched = wide ? std::vector<std::pair<u64, u64>>{{kData64, kSize64}, {kStack64, kSize64}} : std::vector<std::pair<u64, u64>>{{0, 0x10000}};
    for (const auto& [a, n] : watched)
        if (u.uc->mem_read(a, n) != e.uc->mem_read(a, n)) return "memory differs at the end";
    return {};
}

void lockstep_programs(const char* title, int bits) {
    std::mt19937 rng(424242 + bits);
    unsigned total = 1500;
    if (const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE"); given && !given->empty() && *given != "0") total = static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10));
    pc::x86_stats().clear();
    unsigned different = 0, shown = 0;
    for (unsigned n = 0; n < total; ++n) {
        const Scenario s = random_scenario(rng, bits, false);
        const std::string d = lockstep_difference(s);
        if (d.empty()) continue;
        ++different;
        if (++shown <= 12) std::printf("  program %u: %s\n", n, d.c_str());
    }
    std::printf("pcode_cpu_x86 %s: %u programs in lockstep, %u differ; %llu instructions on the description\n", title, total, different, static_cast<unsigned long long>(pc::x86_stats().steps));
    for (const auto& [k, c] : pc::x86_stats().handoffs) std::printf("    to Unicorn: %s x%llu\n", k.c_str(), static_cast<unsigned long long>(c));
    CB_CHECK_EQ(different, 0u);
}

}  // namespace

CB_TEST("engine.pcode_cpu.x86_64_programs_in_lockstep_run_as_unicorn_does", [] { lockstep_programs("64-bit", 64); });
CB_TEST("engine.pcode_cpu.x86_16_programs_in_lockstep_run_as_unicorn_does", [] { lockstep_programs("16-bit", 16); });

// The one-byte and two-byte opcodes with REX.W (64-bit operands: where the description's arithmetic may be wider than the emulator's word), a register and a memory form of ModRM.
CB_TEST("engine.pcode_cpu.x86_64_rex_w_opcodes_run_as_unicorn_does", [] {
    std::mt19937 rng(88);
    std::map<std::string, unsigned> bad;
    std::map<std::string, std::string> example;
    pc::x86_stats().clear();
    unsigned runs = 0;
    const unsigned rounds = [] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 4u;
    }();
    for (const bool two_byte : {false, true})
        for (unsigned op = two_byte ? 0x10 : 0x00; op <= 0xFF; ++op)
            for (const unsigned modrm : {0xC1u, 0xD3u, 0x4Au, 0x02u})
                for (unsigned round = 0; round < rounds; ++round) {
                    if (op == 0x0F || op == 0x31 || op == 0xC7 || (op >= 0x40 && op <= 0x4F && !two_byte)) continue;
                    Scenario s = random_scenario(rng, 64, true);
                    Bytes code = {0x48};
                    if (two_byte) code.push_back(0x0F);
                    code.push_back(static_cast<std::uint8_t>(op));
                    code.push_back(static_cast<std::uint8_t>(modrm));
                    code.push_back(static_cast<std::uint8_t>(rng() % 8));
                    for (std::size_t i = 0; i < code.size(); ++i) s.code[i] = code[i];
                    if (modrm == 0x02 || modrm == 0x4A) {
                        s.regs[2] = kData64 + 0x2000 + (rng() & 0x3FF0);
                        s.regs[3] = kData64 + 0x2000 + (rng() & 0x3FF0);
                    }
                    const Outcome a = run(s, false), b = run(s, true);
                    ++runs;
                    if (a == b) continue;
                    bool flags_only = false;
                    const std::string fields = describe(a, b, true, flags_only);
                    if (flags_only && ((a.flags ^ b.flags) & ~undefined_flags(s.code, true)) == 0) continue;   // (the flags the instruction leaves undefined, and AF)
                    const std::string key = std::format("48 {}{:02x}", two_byte ? "0f " : "", op);
                    ++bad[key];
                    if (!example.count(key)) example[key] = hex_of(s.code, 6) + ": " + fields;
                }
    std::printf("pcode_cpu_x86 REX.W opcodes: %u runs, %zu opcodes differ\n", runs, bad.size());
    for (const auto& [k, c] : bad) std::printf("    %u x %s   e.g. %s\n", c, k.c_str(), example[k].c_str());
    CB_CHECK_EQ(bad.size(), 0u);
});

// The WonderSwan lane's own small cartridge (the one machine_wonderswan_test.cpp writes: ports in and out, a call, a store to RAM) through the lane's effects on both engines.
CB_TEST("engine.pcode_cpu.the_wonderswan_lane_runs_its_cartridge_the_same_on_both_engines", [] {
    Bytes rom(0x10000, 0xFF);
    const Bytes code = {0xB8, 0x34, 0x12, 0xE6, 0x00, 0xE8, 0x06, 0x00, 0xEB, 0xF6, 0x90, 0x90, 0x90, 0x90, 0xE4, 0xB5, 0xA2, 0x00, 0x08, 0xC3};
    std::copy(code.begin(), code.end(), rom.begin());
    const Bytes footer = {0xEA, 0x00, 0x00, 0x00, 0xF0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    std::copy(footer.begin(), footer.end(), rom.end() - 16);
    fs::ScopedTempDir dir("cb_pcode_ws");
    const std::filesystem::path path = dir.path() / "game.ws";
    fs::write_file(path, rom);
    const verify::lanes::ImageLaneSpec spec = engine::wonderswan_lane_spec();
    unsigned runs = 0, differ = 0;
    for (const std::uint32_t entry : {0xF0000u, 0xF000Eu})
        for (const verify::lanes::State& state : spec.states) {
            verify::lanes::Effects e[2];
            for (int core = 0; core < 2; ++core) {
                engine::pcode_compare::ForcedCore forced(core == 1);
                e[core] = spec.effects(path, entry, state);
            }
            ++runs;
            if (!(e[0] == e[1])) ++differ;
        }
    std::printf("pcode_cpu_x86 wonderswan lane cartridge: %u runs, %u differ\n", runs, differ);
    CB_CHECK_EQ(differ, 0u);
});

// RCR of a 64-bit register: the description rotates a 128-bit value (carry flag and register), which the emulator's 64-bit values cannot hold, so the adapter hands it to Unicorn. Every
// register and count, with the carry set and clear, runs as Unicorn does.
CB_TEST("engine.pcode_cpu.x86_64_rcr_of_a_64_bit_register_runs_as_unicorn_does", [] {
    unsigned different = 0;
    for (const Bytes& code : {Bytes{0x48, 0xD1, 0xDD}, Bytes{0x4A, 0xD1, 0xDD}, Bytes{0x48, 0xD3, 0xDD}, Bytes{0x48, 0xC1, 0xDD, 0x05}, Bytes{0x49, 0xD1, 0x1B}, Bytes{0x4C, 0xC1, 0xDB, 0x3F}})
        for (const u64 flags : {u64{0x486}, u64{0x487}})
            for (const u64 value : {u64{0x1C}, u64{0x8000000000000001ull}, u64{0xFFFFFFFFFFFFFFFFull}}) {
                Scenario s;
                s.code = code;
                s.code.resize(16, 0x90);
                s.data.assign(kSize64, 0x5A);
                for (std::size_t i = 0; i < 16; ++i) s.regs.push_back(kData64 + 0x2000);
                s.regs[6] = value;
                s.regs[1] = value ^ 0x33;
                s.regs[2] = 0x45;
                s.regs[7] = kStack64 + 0x8000;
                s.flags = flags;
                if (!(run(s, false) == run(s, true))) ++different;
            }
    CB_CHECK_EQ(different, 0u);
});
