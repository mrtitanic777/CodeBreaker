// cb-unity: alone (file-local names that other files in a batch also use)
// The x86 engine's named operations (engine/pcode_ops_x86.cpp) against Unicorn, the instruction classes the three-byte opcode maps (0F 38, 0F 3A) hold and the classes the engine
// carries out itself (HLT, INT, CPUID, invalid opcodes, faults of every kind): the same instruction on both engines, every register, the vector registers, the memory, the fault.
#include <algorithm>
#include <cstdio>
#include <format>
#include <map>
#include <random>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/unicorn_uc.hpp"
#include "emulate/cpu/pcode_cpu_x86.hpp"

namespace cb {
namespace {

namespace uu = cb::emulate::unicorn_uc;
namespace pc = cb::emulate::pcode_cpu;
using u64 = std::uint64_t;

int reg(const char* name) { return *uu::x86_register(name); }
constexpr u64 kCode = 0x400000, kData = 0x500000, kStack = 0x600000, kSize = 0x10000;
const char* const kGpr[] = {"RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15"};

struct Ran {
    int fault = 0;
    std::string message;
    std::vector<u64> regs;
    u64 flags = 0, ip = 0;
    Bytes memory, vectors;
    bool operator==(const Ran&) const = default;
};

struct Input {
    Bytes code, data, xmm;
    std::vector<u64> regs;
    u64 count = 1;
    u64 flags = 0x2;
};

Ran run(const Input& in, bool emulator) {
    std::unique_ptr<core16::Uc> uc = emulator ? pc::open_x86_emulator(64) : uu::open_x86(64);
    uc->mem_map(kCode, kSize);
    uc->mem_map(kData, kSize);
    uc->mem_map(kStack, kSize);
    uc->mem_write(kCode, in.code);
    uc->mem_write(kData, in.data);
    for (std::size_t i = 0; i < 16; ++i) uc->reg_write(reg(kGpr[i]), in.regs[i]);
    uc->reg_write(reg("FS_BASE"), kData + 0x800);
    for (unsigned i = 0; i < 16; ++i) pc::write_xmm(*uc, i, ByteView(in.xmm).subspan(i * 16, 16));
    uc->reg_write(reg("EFLAGS"), in.flags);
    Ran out;
    try {
        uc->emu_start(kCode, 0x7FFFFFF0, 0, in.count);
    } catch (const uu::UcFault& f) {
        out.fault = f.code();
        out.message = f.what();
    }
    for (std::size_t i = 0; i < 16; ++i) out.regs.push_back(uc->reg_read(reg(kGpr[i])));
    out.flags = uc->reg_read(reg("EFLAGS")) & 0x8C5;   // (AF is the one flag the description never computes)
    out.ip = uc->reg_read(reg("RIP"));
    for (unsigned i = 0; i < 16; ++i) {
        const Bytes v = pc::read_xmm(*uc, i);
        out.vectors.insert(out.vectors.end(), v.begin(), v.end());
    }
    for (const auto& [a, n] : {std::pair<u64, u64>{kData, kSize}, {kStack, kSize}}) {
        const Bytes b = uc->mem_read(a, n);
        out.memory.insert(out.memory.end(), b.begin(), b.end());
    }
    return out;
}

Input random_input(std::mt19937& rng) {
    Input in;
    in.code.resize(32, 0x90);
    in.data.resize(kSize);
    for (auto& b : in.data) b = static_cast<std::uint8_t>(rng());
    const auto pointer = [&] { return kData + 0x2000 + (rng() & 0x3FF0); };
    for (std::size_t i = 0; i < 16; ++i) {
        const unsigned kind = rng() % 6;
        in.regs.push_back(kind < 3 ? pointer() : kind == 3 ? (static_cast<u64>(rng()) << 32 | rng()) : kind == 4 ? rng() % 40 : static_cast<u64>(-static_cast<std::int64_t>(rng() % 40)));
    }
    in.regs[7] = kStack + 0x8000;
    in.xmm.resize(256);
    for (auto& b : in.xmm) b = static_cast<std::uint8_t>(rng());
    in.flags = 0x2 | (rng() & 0x8D5);
    return in;
}

std::string hex(const Bytes& b, std::size_t n);
std::string diff(const Ran& a, const Ran& b) {
    std::string out;
    if (a.fault != b.fault) out += std::format("fault(unicorn {} / emulator {}) ", a.fault, b.fault);
    for (std::size_t i = 0; i < a.regs.size(); ++i)
        if (a.regs[i] != b.regs[i]) out += std::format("{}[uc {:X} emu {:X}] ", kGpr[i], a.regs[i], b.regs[i]);
    if (a.ip != b.ip) out += std::format("ip[{:X}/{:X}] ", a.ip, b.ip);
    if (a.flags != b.flags) out += std::format("flags[uc {:X} emu {:X}] ", a.flags, b.flags);
    for (std::size_t i = 0; i < a.vectors.size(); ++i)
        if (a.vectors[i] != b.vectors[i]) {
            out += std::format("xmm{}.{}[uc {:02X} emu {:02X}] ", i / 16, i % 16, a.vectors[i], b.vectors[i]);
            out += "uc " + hex(Bytes(a.vectors.begin() + static_cast<std::ptrdiff_t>(i / 16 * 16), a.vectors.begin() + static_cast<std::ptrdiff_t>(i / 16 * 16 + 16)), 16) + " emu " +
                   hex(Bytes(b.vectors.begin() + static_cast<std::ptrdiff_t>(i / 16 * 16), b.vectors.begin() + static_cast<std::ptrdiff_t>(i / 16 * 16 + 16)), 16) + " ";
            break;
        }
    for (std::size_t i = 0; i < a.memory.size(); ++i)
        if (a.memory[i] != b.memory[i]) {
            out += std::format("mem@{:X}[uc {:02X} emu {:02X}] ", i, a.memory[i], b.memory[i]);
            break;
        }
    return out;
}

std::string hex(const Bytes& b, std::size_t n) {
    std::string s;
    for (std::size_t i = 0; i < n && i < b.size(); ++i) s += std::format("{:02X}", b[i]);
    return s;
}

unsigned rounds_from_env(unsigned fallback) {
    const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
    return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : fallback;
}

}  // namespace

// Every opcode of the three-byte maps with each mandatory prefix and a register form and a memory form, on random registers and vector registers.
CB_TEST("emulate.pcode_cpu.x86_64_three_byte_opcodes_run_as_unicorn_does", [] {
    std::mt19937 rng(38);
    std::map<std::string, unsigned> bad;
    std::map<std::string, std::string> example;
    pc::x86_stats().clear();
    unsigned runs = 0;
    const unsigned rounds = rounds_from_env(3);
    for (const unsigned map : {0x38u, 0x3Au})
        for (const unsigned prefix : {0x00u, 0x66u, 0xF3u, 0xF2u})
            for (unsigned op = 0; op <= 0xFF; ++op)
                for (const unsigned modrm : {0xC1u, 0xD3u, 0x4Au, 0x02u})
                    for (unsigned round = 0; round < rounds; ++round) {
                        Input in = random_input(rng);
                        Bytes code;
                        if (prefix != 0) code.push_back(static_cast<std::uint8_t>(prefix));
                        code.insert(code.end(), {0x0F, static_cast<std::uint8_t>(map), static_cast<std::uint8_t>(op), static_cast<std::uint8_t>(modrm), static_cast<std::uint8_t>(map == 0x3A && op >= 0x60 && op <= 0x63 ? rng() % 128 : rng() % 8), static_cast<std::uint8_t>(rng() % 8)});
                        std::copy(code.begin(), code.end(), in.code.begin());
                        if (modrm == 0x02 || modrm == 0x4A) {
                            in.regs[2] = kData + 0x2000 + (rng() & 0x3FF0);
                            in.regs[3] = kData + 0x2000 + (rng() & 0x3FF0);
                        }
                        if (map == 0x38 && op == 0xF0 && prefix == 0xF2) continue;   // (CRC32 and MOVBE: not the point here)
                        // (the string compares with the ranges aggregation and an odd number of bounds: Unicorn reads the word before the register, a defect of QEMU's helper, which the
                        // processor does not have; the manual's behaviour is checked by pcode_ops_x86_test.cpp)
                        if (map == 0x3A && op >= 0x60 && op <= 0x63 && ((in.code[code.size() - 2] >> 2) & 3) == 1) continue;
                        const Ran a = run(in, false), b = run(in, true);
                        ++runs;
                        if (a == b) continue;
                        const std::string key = std::format("{} 0f {:02x} {:02x}", prefix == 0 ? "--" : std::format("{:02x}", prefix), map, op);
                        ++bad[key];
                        if (!example.count(key)) example[key] = hex(in.code, 8) + " xmm0 " + hex(Bytes(in.xmm.begin(), in.xmm.begin() + 16), 16) + " xmm1 " + hex(Bytes(in.xmm.begin() + 16, in.xmm.begin() + 32), 16) + " rax " + std::format("{:X} rdx {:X}", in.regs[0], in.regs[3]) + ": " + diff(a, b);
                    }
    std::printf("pcode_cpu_x86 three-byte opcodes: %u runs, %zu opcodes differ\n", runs, bad.size());
    for (const auto& [k, c] : bad) std::printf("    %u x %s   e.g. %s\n", c, k.c_str(), example[k].c_str());
    for (const auto& [k, c] : pc::x86_stats().unserved) std::printf("    unserved user op: %s x%llu\n", k.c_str(), static_cast<unsigned long long>(c));
    for (const auto& [k, c] : pc::x86_stats().handoffs) std::printf("    to Unicorn: %s x%llu\n", k.c_str(), static_cast<unsigned long long>(c));
    CB_CHECK_EQ(bad.size(), 0u);
});

// The x87 float: every opcode D8 to DF with every ModRM byte, after three loads onto the stack, observed through the status word and two stores of the stack's top (CB_X86_X87=1: the
// instructions on the description, which keeps no top of stack, tag word or exception flags).
CB_TEST("emulate.pcode_cpu.x86_64_x87_instructions_run_as_unicorn_does", [] {
    const auto on = fs::get_env("CB_X86_X87");
    if (!on || on->empty()) return;
    pc::x86_options().x87_on_description = true;
    std::mt19937 rng(87);
    std::map<std::string, unsigned> bad;
    std::map<std::string, std::string> example;
    const unsigned rounds = rounds_from_env(2);
    unsigned runs = 0;
    for (unsigned op = 0xD8; op <= 0xDF; ++op)
        for (unsigned modrm = 0; modrm < 256; ++modrm)
            for (unsigned round = 0; round < rounds; ++round) {
                Input in = random_input(rng);
                // fld qword [rbx] ; fld qword [rbx+8] ; fld qword [rbx+16] ; <instruction> ; fnstsw ax ; fstp tword [rsi] ; fstp tword [rsi+16]
                Bytes code = {0xDD, 0x03, 0xDD, 0x43, 0x08, 0xDD, 0x43, 0x10, static_cast<std::uint8_t>(op), static_cast<std::uint8_t>(modrm), static_cast<std::uint8_t>(rng() % 8), 0x90, 0x90};
                const std::size_t at = code.size();
                code.insert(code.end(), {0xDF, 0xE0, 0xDB, 0x3E, 0xDB, 0x7E, 0x10});
                std::copy(code.begin(), code.end(), in.code.begin());
                in.code.resize(64, 0x90);
                in.regs[1] = kData + 0x2000;
                in.regs[4] = kData + 0x3000;
                for (const int r : {0, 2, 3, 5, 6}) in.regs[static_cast<std::size_t>(r)] = kData + 0x2100;
                for (int k = 0; k < 24; ++k) {   // (ordinary doubles at the loaded addresses)
                    const double d = static_cast<double>(static_cast<int>(rng() % 2000) - 1000) / 16.0;
                    std::memcpy(in.data.data() + 0x2000 + 8 * k, &d, 8);
                }
                in.count = 3 + 1 + 3;
                (void)at;
                const Ran a = run(in, false), b = run(in, true);
                ++runs;
                if (a == b) continue;
                const std::string key = std::format("{:02x} {:02x}", op, (modrm >> 6) == 3 ? modrm : (modrm & 0x38) | 0x40);
                ++bad[key];
                if (!example.count(key)) example[key] = hex(in.code, 12) + ": " + diff(a, b);
            }
    std::printf("pcode_cpu_x86 x87: %u runs, %zu opcodes differ\n", runs, bad.size());
    for (const auto& [k, c] : bad) std::printf("    %u x %s   e.g. %s\n", c, k.c_str(), example[k].c_str());
    for (const auto& [k, c] : pc::x86_stats().unserved) std::printf("    unserved user op: %s x%llu\n", k.c_str(), static_cast<unsigned long long>(c));
    pc::x86_options().x87_on_description = false;
});

// One instruction's bytes (CB_X86_RUN, hex) on both engines with random registers: a tool for a difference the sweeps found.
CB_TEST("emulate.pcode_cpu.x86_64_run_the_given_bytes_on_both_engines", [] {
    const auto given = fs::get_env("CB_X86_RUN");
    if (!given || given->empty()) return;
    pc::x86_options().vex_as_unicorn = true;
    std::mt19937 rng(5);
    for (unsigned round = 0; round < 8; ++round) {
        Input in = random_input(rng);
        Bytes code;
        for (std::size_t i = 0; i + 1 < given->size(); i += 2) code.push_back(static_cast<std::uint8_t>(std::stoul(given->substr(i, 2), nullptr, 16)));
        std::copy(code.begin(), code.end(), in.code.begin());
        const Ran a = run(in, false), b = run(in, true);
        std::printf("  %s: %s\n", a == b ? "same" : "DIFFERENT", diff(a, b).c_str());
    }
});

// CPUID for every leaf and sub-leaf Unicorn answers differently from zero, and some it does not.
CB_TEST("emulate.pcode_cpu.x86_64_cpuid_answers_as_unicorn_does", [] {
    std::mt19937 rng(11);
    unsigned different = 0, runs = 0;
    std::vector<u64> leaves;
    for (u64 l = 0; l < 0x30; ++l) leaves.push_back(l), leaves.push_back(0x80000000ull + l);
    for (int i = 0; i < 40; ++i) leaves.push_back(static_cast<std::uint32_t>(rng()));
    for (const u64 leaf : leaves)
        for (const u64 sub : {0ull, 1ull, 2ull, 3ull, 5ull, 7ull, 8ull, 40ull, 0xFFFFFFFFull}) {
            Input in = random_input(rng);
            in.code = {0x0F, 0xA2};
            in.code.resize(32, 0x90);
            in.regs[0] = leaf | (static_cast<u64>(rng()) << 32);
            in.regs[2] = sub | (static_cast<u64>(rng()) << 32);
            const Ran a = run(in, false), b = run(in, true);
            ++runs;
            if (!(a == b)) {
                if (++different <= 10) std::printf("  cpuid %llX %llX: %s\n", static_cast<unsigned long long>(leaf), static_cast<unsigned long long>(sub), diff(a, b).c_str());
            }
        }
    std::printf("pcode_cpu_x86 cpuid: %u runs, %u differ\n", runs, different);
    CB_CHECK_EQ(different, 0u);
});

}  // namespace cb
