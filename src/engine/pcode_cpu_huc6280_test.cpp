// cb-unity: alone
// The PC Engine lane's HuC6280 on the one emulator (engine/pcode_cpu_huc6280.cpp). The own core (emulate/huc6280's interpreter) was retired once the emulator matched it on
// random instructions and programs (40,000 runs) and on the lane's whole homebrew corpus (every function in every state, and the verdicts on edited copies). What the random
// runs gave on both is kept as a digest (CRC-32 of every run's exit, registers, steps, accesses to the hardware page, RAM and mapping registers), so a change in what the
// emulator does shows. The own machine's unit tests follow, on the emulator.
#include <algorithm>
#include <array>
#include <cstdio>
#include <format>
#include <random>
#include <string>

#include "core/crypto.hpp"
#include "core/testing.hpp"
#include "emulate/huc6280.hpp"
#include "engine/pcode_cpu_huc6280.hpp"

using namespace cb;
namespace h = cb::emulate::huc6280;

namespace {

std::string describe_huc(const h::Run& a, const h::Run& b) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    const auto eight = [&](const char* name, unsigned x, unsigned y) {
        if (x != y) out += std::format("{}[own {:X} emu {:X}] ", name, x, y);
    };
    eight("a", a.regs.a, b.regs.a);
    eight("x", a.regs.x, b.regs.x);
    eight("y", a.regs.y, b.regs.y);
    eight("s", a.regs.s, b.regs.s);
    eight("p", a.regs.p, b.regs.p);
    eight("pc", a.regs.pc, b.regs.pc);
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    if (a.io.size() != b.io.size()) out += std::format("io[{}/{}] ", a.io.size(), b.io.size());
    else
        for (std::size_t i = 0; i < a.io.size(); ++i)
            if (a.io[i].write != b.io[i].write || a.io[i].offset != b.io[i].offset || a.io[i].value != b.io[i].value || a.io[i].vdc != b.io[i].vdc) {
                out += std::format("io@{}[own {} emu {}] ", i, a.io[i].text(), b.io[i].text());
                break;
            }
    return out;
}


// A run folded into the digest: everything describe_huc compares (against an empty run), each access to the hardware page, the RAM and the mapping registers.
std::uint32_t huc_fold(std::uint32_t digest, const h::Run& r, const h::Machine& machine) {
    std::string text = describe_huc(h::Run{}, r);
    for (const h::Access& x : r.io) text += "|" + x.text();
    digest = crypto::crc32(ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()), digest);
    digest = crypto::crc32(ByteView(machine.ram.data(), machine.ram.size()), digest);
    return crypto::crc32(ByteView(machine.mpr.data(), machine.mpr.size()), digest);
}

}  // namespace

// A random program in RAM at $2400 (and in ROM at $E000), the ROM and the rest of RAM random: single instructions and short programs, the mapping registers as the machine starts and as
// TAM leaves them.
CB_TEST("engine.pcode_cpu.the_huc6280_runs_random_programs_as_the_own_core_did", [] {
    std::mt19937 rng(20261122);
    constexpr unsigned total = 20000;
    std::uint32_t digest = 0;
    for (unsigned n = 0; n < 2 * total; ++n) {
        const bool single = n < total;
        Bytes rom(0x20000);
        for (auto& b : rom) b = static_cast<std::uint8_t>(rng());
        std::array<std::uint8_t, h::kBankSize> ram_init{};
        for (auto& b : ram_init) b = static_cast<std::uint8_t>(rng());
        for (std::size_t i = 0x400; i < 0x440; ++i) ram_init[i] = static_cast<std::uint8_t>(rng());
        if (single) ram_init[0x400] = static_cast<std::uint8_t>(n * 7 + n / 256);
        else {
            // A program that stays in its RAM: the old core executed bytes it read from the hardware page (the chips' registers, read twice for the opcode, the status bit changing on every
            // read), which no program does, and the page is where a jump to a random address lands. The branches take small forward offsets; the jumps, calls, returns, BRK, the
            // bit-test branches, TAM (a register that shows the hardware page where the program runs) and the block transfers (which write over the program) become NOPs (each of them is run as the one instruction of the single-instruction runs).
            for (std::size_t i = 0x400; i < 0x440; ++i) {
                const std::uint8_t op = ram_init[i];
                if (op == 0x00 || op == 0x20 || op == 0x40 || op == 0x60 || op == 0x4C || op == 0x6C || op == 0x7C || op == 0x44 || (op & 0x0F) == 0x0F || op == 0x53 || op == 0x73 || op == 0xC3 || op == 0xD3 || op == 0xE3 || op == 0xF3) ram_init[i] = 0xEA;
                else if (op == 0x80 || (op & 0x1F) == 0x10) ram_init[i + 1] = static_cast<std::uint8_t>(ram_init[i + 1] & 7);
            }
            for (std::size_t i = 0x440; i < 0x4C0; ++i) ram_init[i] = 0xEA;   // (what a program that runs off its end finds)
        }
        h::Options options;
        options.max_steps = single ? 1 : 40;
        options.a = static_cast<std::uint8_t>(rng());
        options.x = static_cast<std::uint8_t>(rng());
        options.y = static_cast<std::uint8_t>(rng());
        if ((rng() & 7) == 0) options.a = static_cast<std::uint8_t>(0xF8 + (rng() & 3));
        h::Machine machine(rom);
        machine.ram = ram_init;
        if (!single) machine.mpr[0] = 0xF8;
        const h::Run rr = engine::pcode_cpu::run(machine, 0x2400, options);
        digest = huc_fold(digest, rr, machine);
    }
    std::printf("pcode_cpu huc6280 instructions: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0x118CA267u);
});

namespace hc = cb::emulate::huc6280;

namespace {

// Two banks: `code` at $E000 (bank 0), the bytes 1 2 3 4 at $E100, bank 1 all $A5.
Bytes rom_with(const Bytes& code) {
    Bytes rom(0x4000, 0xFF);
    std::copy(code.begin(), code.end(), rom.begin());
    const Bytes data = {1, 2, 3, 4};
    std::copy(data.begin(), data.end(), rom.begin() + 0x100);
    std::fill(rom.begin() + 0x2000, rom.end(), std::uint8_t{0xA5});
    return rom;
}

struct Result {
    hc::Run run;
    hc::Machine* machine;
};

}  // namespace

CB_TEST("engine.pcode_cpu.huc6280_mapping_registers_move_between_the_accumulator_and_the_map", [] {
    const Bytes rom = rom_with({0xA9, 0x01, 0x53, 0x04, 0xAD, 0x00, 0x40, 0xA2, 0x00, 0x43, 0x04, 0x60});   // LDA #1; TAM #4; LDA $4000; LDX #0; TMA #4; RTS
    hc::Machine m(rom);
    const hc::Run r = cb::engine::pcode_cpu::run(m, 0xE000);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(m.mpr[2], 1);
    CB_CHECK_EQ(r.regs.a, 1);   // TMA left the mapping register's number
    // after the TAM, $4000 read bank 1's $A5: run the load alone
    const Bytes rom2 = rom_with({0xA9, 0x01, 0x53, 0x04, 0xAD, 0x00, 0x40, 0x60});
    hc::Machine m2(rom2);
    CB_CHECK_EQ(cb::engine::pcode_cpu::run(m2, 0xE000).regs.a, 0xA5);
});

CB_TEST("engine.pcode_cpu.huc6280_block_transfers", [] {
    // TII $E100 -> $2000, 4 bytes; TIA $E100 -> $0002 (alternating $0002, $0003), 4 bytes; TDD $E103 -> $2013 down, 2 bytes; TIN $E100 -> $2020, 3; TAI $E100 -> $2030, 4
    const Bytes rom = rom_with({0x73, 0x00, 0xE1, 0x00, 0x20, 0x04, 0x00,                                             // TII
                                0xE3, 0x00, 0xE1, 0x02, 0x00, 0x04, 0x00,                                             // TIA
                                0xC3, 0x03, 0xE1, 0x13, 0x20, 0x02, 0x00,                                             // TDD
                                0xD3, 0x00, 0xE1, 0x20, 0x20, 0x03, 0x00,                                             // TIN
                                0xF3, 0x00, 0xE1, 0x30, 0x20, 0x04, 0x00, 0x60});                                      // TAI; RTS
    hc::Machine m(rom);
    const hc::Run r = cb::engine::pcode_cpu::run(m, 0xE000);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK(std::equal(m.ram.begin(), m.ram.begin() + 4, rom.begin() + 0x100));                  // TII
    CB_CHECK_EQ(m.ram[0x13], 4);                                                                    // TDD: $E103 down
    CB_CHECK_EQ(m.ram[0x12], 3);
    CB_CHECK_EQ(m.ram[0x20], 3);                                                                    // TIN: the destination stays, so the last byte is what is there
    CB_CHECK_EQ(m.ram[0x30], 1);                                                                    // TAI: the source alternates 1 2 1 2, the destination climbs
    CB_CHECK_EQ(m.ram[0x31], 2);
    CB_CHECK_EQ(m.ram[0x32], 1);
    CB_CHECK_EQ(m.ram[0x33], 2);
    CB_CHECK_EQ(r.io.size(), 4u);                                                                   // TIA: four writes alternating the two data ports
    CB_CHECK_EQ(r.io[0].text(), "w VDC_DATA_L(MAWR) 01");
    CB_CHECK_EQ(r.io[1].text(), "w VDC_DATA_H(MAWR) 02");
    CB_CHECK_EQ(r.io[2].text(), "w VDC_DATA_L(MAWR) 03");
    CB_CHECK_EQ(r.io[3].text(), "w VDC_DATA_H(MAWR) 04");
});

CB_TEST("engine.pcode_cpu.huc6280_set_makes_the_next_alu_instruction_work_on_zero_page_x", [] {
    // LDX #2; SET; ORA #$05; RTS with $2002 = $30 and A = $07: the byte becomes $35, A stays
    const Bytes rom = rom_with({0xA2, 0x02, 0xF4, 0x09, 0x05, 0x60});
    hc::Machine m(rom);
    m.ram[2] = 0x30;
    const hc::Run r = cb::engine::pcode_cpu::run(m, 0xE000, {.a = 7});
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(m.ram[2], 0x35);
    CB_CHECK_EQ(r.regs.a, 7);
});

CB_TEST("engine.pcode_cpu.huc6280_the_vdc_stores_are_logged_under_their_registers", [] {
    // ST0 #$05; ST1 #$80; ST2 #$00; LDA $0000 (status twice); RTS
    const Bytes rom = rom_with({0x03, 0x05, 0x13, 0x80, 0x23, 0x00, 0xAD, 0x00, 0x00, 0xAD, 0x00, 0x00, 0x60});
    hc::Machine m(rom);
    const hc::Run r = cb::engine::pcode_cpu::run(m, 0xE000);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(r.io.size(), 5u);
    CB_CHECK_EQ(r.io[0].text(), "w VDC_SELECT 05");
    CB_CHECK_EQ(r.io[1].text(), "w VDC_DATA_L(CR) 80");
    CB_CHECK_EQ(r.io[2].text(), "w VDC_DATA_H(CR) 00");
    CB_CHECK_EQ(r.io[3].text(), "r VDC_STATUS 00");
    CB_CHECK_EQ(r.io[4].text(), "r VDC_STATUS 20");   // the vertical-blank bit shows on every other read
});

CB_TEST("engine.pcode_cpu.huc6280_subroutines_bits_and_swaps", [] {
    // BSR sub; SXY; RTS ... sub: SMB3 $21; BBS3 $21, +1; CLA; TST #$08,$21; RTS  (BSR at E000 -> sub at E010)
    Bytes code = {0x44, 0x0E, 0x02, 0x60};
    code.resize(0x10, 0xEA);
    const Bytes sub = {0x87 + 0x30, 0x21, 0x3F + 0x80, 0x21, 0x01, 0x62, 0x83, 0x08, 0x21, 0x60};   // SMB3, BBS3 skipping the CLA, TST
    code.insert(code.end(), sub.begin(), sub.end());
    const Bytes rom = rom_with(code);
    hc::Machine m(rom);
    const hc::Run r = cb::engine::pcode_cpu::run(m, 0xE000, {.a = 9, .x = 1, .y = 2});
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(m.ram[0x21], 0x08);
    CB_CHECK_EQ(r.regs.a, 9);   // CLA was skipped by the branch
    CB_CHECK_EQ(r.regs.x, 2);   // SXY swapped X and Y
    CB_CHECK_EQ(r.regs.y, 1);
    CB_CHECK_EQ(r.regs.p & 0x02, 0);   // TST: the mask met the bit, Z is clear
});

CB_TEST("engine.pcode_cpu.huc6280_an_opcode_without_an_instruction_stops_the_run", [] {
    const Bytes rom = rom_with({0xEA, 0x5C});   // NOP; $5C is nothing
    hc::Machine m(rom);
    const hc::Run r = cb::engine::pcode_cpu::run(m, 0xE000);
    CB_CHECK(r.end == hc::End::Stopped);
    CB_CHECK_EQ(r.exit(), "stopped: no instruction for opcode 5c at e001");
});
