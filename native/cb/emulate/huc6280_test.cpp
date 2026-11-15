// The HuC6280 machine: the instructions Hudson added, the mapping registers and the chips' logged registers.
#include "cb/emulate/huc6280.hpp"

#include <algorithm>

#include "cb/core/testing.hpp"

namespace hc = cb::emulate::huc6280;
using cb::Bytes;

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

CB_TEST("emulate.huc6280.mapping_registers_move_between_the_accumulator_and_the_map", [] {
    const Bytes rom = rom_with({0xA9, 0x01, 0x53, 0x04, 0xAD, 0x00, 0x40, 0xA2, 0x00, 0x43, 0x04, 0x60});   // LDA #1; TAM #4; LDA $4000; LDX #0; TMA #4; RTS
    hc::Machine m(rom);
    const hc::Run r = hc::run(m, 0xE000);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(m.mpr[2], 1);
    CB_CHECK_EQ(r.regs.a, 1);   // TMA left the mapping register's number
    // after the TAM, $4000 read bank 1's $A5: run the load alone
    const Bytes rom2 = rom_with({0xA9, 0x01, 0x53, 0x04, 0xAD, 0x00, 0x40, 0x60});
    hc::Machine m2(rom2);
    CB_CHECK_EQ(hc::run(m2, 0xE000).regs.a, 0xA5);
});

CB_TEST("emulate.huc6280.block_transfers", [] {
    // TII $E100 -> $2000, 4 bytes; TIA $E100 -> $0002 (alternating $0002, $0003), 4 bytes; TDD $E103 -> $2013 down, 2 bytes; TIN $E100 -> $2020, 3; TAI $E100 -> $2030, 4
    const Bytes rom = rom_with({0x73, 0x00, 0xE1, 0x00, 0x20, 0x04, 0x00,                                             // TII
                                0xE3, 0x00, 0xE1, 0x02, 0x00, 0x04, 0x00,                                             // TIA
                                0xC3, 0x03, 0xE1, 0x13, 0x20, 0x02, 0x00,                                             // TDD
                                0xD3, 0x00, 0xE1, 0x20, 0x20, 0x03, 0x00,                                             // TIN
                                0xF3, 0x00, 0xE1, 0x30, 0x20, 0x04, 0x00, 0x60});                                      // TAI; RTS
    hc::Machine m(rom);
    const hc::Run r = hc::run(m, 0xE000);
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

CB_TEST("emulate.huc6280.set_makes_the_next_alu_instruction_work_on_zero_page_x", [] {
    // LDX #2; SET; ORA #$05; RTS with $2002 = $30 and A = $07: the byte becomes $35, A stays
    const Bytes rom = rom_with({0xA2, 0x02, 0xF4, 0x09, 0x05, 0x60});
    hc::Machine m(rom);
    m.ram[2] = 0x30;
    const hc::Run r = hc::run(m, 0xE000, {.a = 7});
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(m.ram[2], 0x35);
    CB_CHECK_EQ(r.regs.a, 7);
});

CB_TEST("emulate.huc6280.the_vdc_stores_are_logged_under_their_registers", [] {
    // ST0 #$05; ST1 #$80; ST2 #$00; LDA $0000 (status twice); RTS
    const Bytes rom = rom_with({0x03, 0x05, 0x13, 0x80, 0x23, 0x00, 0xAD, 0x00, 0x00, 0xAD, 0x00, 0x00, 0x60});
    hc::Machine m(rom);
    const hc::Run r = hc::run(m, 0xE000);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(r.io.size(), 5u);
    CB_CHECK_EQ(r.io[0].text(), "w VDC_SELECT 05");
    CB_CHECK_EQ(r.io[1].text(), "w VDC_DATA_L(CR) 80");
    CB_CHECK_EQ(r.io[2].text(), "w VDC_DATA_H(CR) 00");
    CB_CHECK_EQ(r.io[3].text(), "r VDC_STATUS 00");
    CB_CHECK_EQ(r.io[4].text(), "r VDC_STATUS 20");   // the vertical-blank bit shows on every other read
});

CB_TEST("emulate.huc6280.subroutines_bits_and_swaps", [] {
    // BSR sub; SXY; RTS ... sub: SMB3 $21; BBS3 $21, +1; CLA; TST #$08,$21; RTS  (BSR at E000 -> sub at E010)
    Bytes code = {0x44, 0x0E, 0x02, 0x60};
    code.resize(0x10, 0xEA);
    const Bytes sub = {0x87 + 0x30, 0x21, 0x3F + 0x80, 0x21, 0x01, 0x62, 0x83, 0x08, 0x21, 0x60};   // SMB3, BBS3 skipping the CLA, TST
    code.insert(code.end(), sub.begin(), sub.end());
    const Bytes rom = rom_with(code);
    hc::Machine m(rom);
    const hc::Run r = hc::run(m, 0xE000, {.a = 9, .x = 1, .y = 2});
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(m.ram[0x21], 0x08);
    CB_CHECK_EQ(r.regs.a, 9);   // CLA was skipped by the branch
    CB_CHECK_EQ(r.regs.x, 2);   // SXY swapped X and Y
    CB_CHECK_EQ(r.regs.y, 1);
    CB_CHECK_EQ(r.regs.p & 0x02, 0);   // TST: the mask met the bit, Z is clear
});

CB_TEST("emulate.huc6280.an_opcode_without_an_instruction_stops_the_run", [] {
    const Bytes rom = rom_with({0xEA, 0x5C});   // NOP; $5C is nothing
    hc::Machine m(rom);
    const hc::Run r = hc::run(m, 0xE000);
    CB_CHECK(r.end == hc::End::Stopped);
    CB_CHECK_EQ(r.exit(), "stopped: no instruction for opcode 5c at e001");
});
