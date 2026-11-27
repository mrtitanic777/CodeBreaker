// cb-unity: alone
// The Super Nintendo lane's 65C816 on the one emulator (engine/pcode_cpu_w65816.cpp). The own core (emulate/w65816's interpreter) was retired once the emulator matched it
// on random instructions and programs (12,000 runs) and on the lane's whole homebrew corpus (every function in every state, and the verdicts on edited copies). What the
// random runs gave on both is kept as a digest (CRC-32 of every run's exit, registers, steps, accesses to the chips and work RAM), so a change in what the emulator does
// shows. The own machine's unit tests follow, on the emulator.
#include <cstdio>
#include <format>
#include <memory>
#include <random>
#include <string>

#include "core/crypto.hpp"
#include "core/testing.hpp"
#include "emulate/w65816.hpp"
#include "engine/pcode_cpu_w65816.hpp"
#include "formats/snes_test_image.hpp"

namespace w = cb::emulate::w65816;

namespace {

std::string describe(const w::Run& a, const w::Run& b, const w::Machine& ma, const w::Machine& mb) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    const auto reg = [&](const char* name, unsigned x, unsigned y) {
        if (x != y) out += std::format("{}[own {:X} emu {:X}] ", name, x, y);
    };
    reg("A", a.regs.a, b.regs.a);
    reg("X", a.regs.x, b.regs.x);
    reg("Y", a.regs.y, b.regs.y);
    reg("S", a.regs.s, b.regs.s);
    reg("D", a.regs.d, b.regs.d);
    reg("PC", a.regs.pc, b.regs.pc);
    reg("DBR", a.regs.dbr, b.regs.dbr);
    reg("PBR", a.regs.pbr, b.regs.pbr);
    reg("P", a.regs.p, b.regs.p);
    reg("E", a.regs.e, b.regs.e);
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    if (a.io.size() != b.io.size()) out += std::format("io[{}/{}] ", a.io.size(), b.io.size());
    else
        for (std::size_t i = 0; i < a.io.size(); ++i)
            if (a.io[i].write != b.io[i].write || a.io[i].address != b.io[i].address || a.io[i].value != b.io[i].value) {
                out += std::format("io@{}[own '{}' emu '{}'] ", i, a.io[i].text(), b.io[i].text());
                break;
            }
    if (ma.wram != mb.wram)
        for (std::size_t i = 0; i < ma.wram.size(); ++i)
            if (ma.wram[i] != mb.wram[i]) {
                out += std::format("wram@{:X}[own {:02X} emu {:02X}] ", i, ma.wram[i], mb.wram[i]);
                break;
            }
    return out;
}

struct Case {
    cb::Bytes file;
    w::Options options;
    std::uint64_t wram_seed = 0;
};

// One run: a machine of its own on the case's cartridge, its RAM filled the same way.
w::Run play(const Case& c, w::Machine** keep, std::unique_ptr<w::Machine>& holder, const cb::formats::snes::Rom& rom, std::uint64_t max_steps) {
    holder = std::make_unique<w::Machine>(rom);
    std::mt19937 fill(static_cast<std::uint32_t>(c.wram_seed));
    for (std::uint8_t& b : holder->wram) b = static_cast<std::uint8_t>(fill());
    w::Options o = c.options;
    o.max_steps = max_steps;
    *keep = holder.get();
    return cb::engine::pcode_cpu::run(*holder, 0x008100, o);
}

std::uint32_t random_digest(bool single) {
    std::mt19937 rng(single ? 20261122 : 20261123);
    std::uint32_t digest = 0;
    for (unsigned n = 0; n < 6000; ++n) {
        Case c;
        c.file = cb::formats::snes_test_image::cartridge();
        for (std::size_t i = 0x100; i < 0x7000; ++i) c.file[i] = static_cast<std::uint8_t>(rng());
        if (rng() % 3 == 0)   // (data in the ROM that points at the RAM, the chips and the ROM itself)
            for (std::size_t i = 0x100; i < 0x180; i += 2) c.file[i + 1] = static_cast<std::uint8_t>((rng() % 4) == 0 ? 0x21 + rng() % 2 * 0x20 : rng() % 0x20);
        c.file[0x100] = static_cast<std::uint8_t>(single ? n % 256 : rng());
        c.wram_seed = rng();
        auto word = [&] { return static_cast<std::uint16_t>((rng() & 1) ? rng() & 0xFF : (rng() & 3) == 0 ? 0x2100 + (rng() & 0xFF) : rng() & 0xFFFF); };
        c.options.a = word();
        c.options.x = word();
        c.options.y = word();
        c.options.native = (rng() % 5) != 0;
        c.options.m16 = (rng() & 1) != 0;
        c.options.x16 = (rng() & 1) != 0;
        const cb::formats::snes::Rom rom = cb::formats::snes::parse(c.file);
        std::unique_ptr<w::Machine> holder;
        w::Machine* machine = nullptr;
        const w::Run run = play(c, &machine, holder, rom, single ? 1 : 40);
        // everything describe compares (against an empty run on an empty machine), each access, and the work RAM
        const w::Machine empty(rom);
        std::string text = describe(w::Run{}, run, empty, empty);
        for (const auto& x : run.io) text += "|" + x.text();
        digest = cb::crypto::crc32(cb::ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()), digest);
        digest = cb::crypto::crc32(cb::ByteView(machine->wram.data(), machine->wram.size()), digest);
    }
    return digest;
}

}  // namespace

CB_TEST("engine.pcode_cpu.w65816_instructions_run_as_the_own_core_did", [] {
    const std::uint32_t digest = random_digest(true);
    std::printf("pcode_cpu w65816 instructions: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0x97FD8DC5u);
});

CB_TEST("engine.pcode_cpu.w65816_programs_run_as_the_own_core_did", [] {
    const std::uint32_t digest = random_digest(false);
    std::printf("pcode_cpu w65816 programs: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0xFD19225Cu);
});

namespace sn = cb::emulate::w65816;
namespace img = cb::formats::snes_test_image;
using cb::Bytes;

namespace {

// The test cartridge with `code` at $00:8100.
cb::formats::snes::Rom rom_with(const Bytes& code) {
    Bytes file = img::cartridge();
    img::put(file, 0x100, code);
    return cb::formats::snes::parse(file);
}

}  // namespace

CB_TEST("engine.pcode_cpu.w65816_sixteen_bit_arithmetic_and_a_store_to_the_ram", [] {
    // REP #$30; LDA #$1234; CLC; ADC #$1111; STA $0300; RTS
    const auto rom = rom_with({0xC2, 0x30, 0xA9, 0x34, 0x12, 0x18, 0x69, 0x11, 0x11, 0x8D, 0x00, 0x03, 0x60});
    sn::Machine m(rom);
    const sn::Run r = cb::engine::pcode_cpu::run(m, 0x008100);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(r.regs.a, 0x2345);
    CB_CHECK_EQ(m.wram[0x300], 0x45);
    CB_CHECK_EQ(m.wram[0x301], 0x23);
    CB_CHECK_EQ(r.regs.p & sn::status::M, 0);   // REP cleared M
});

CB_TEST("engine.pcode_cpu.w65816_a_function_starts_in_the_widths_it_is_given", [] {
    // LDA #$12 (a one-byte immediate while M is set) then RTS: run with 8-bit A, then the same bytes read as 16-bit would eat the RTS
    const auto rom = rom_with({0xA9, 0x12, 0x60});
    sn::Machine m(rom);
    const sn::Run r = cb::engine::pcode_cpu::run(m, 0x008100, {.a = 0xAB00});
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(r.regs.a, 0xAB12);   // the high byte is kept
    sn::Machine m2(rom);
    const sn::Run wide = cb::engine::pcode_cpu::run(m2, 0x008100, {.m16 = true});
    CB_CHECK(wide.end != sn::End::Return);   // LDA #$6012, then whatever follows
});

CB_TEST("engine.pcode_cpu.w65816_decimal_mode_adds_in_bcd", [] {
    // SED; CLC; LDA #$19; ADC #$01; RTS
    const auto rom = rom_with({0xF8, 0x18, 0xA9, 0x19, 0x69, 0x01, 0x60});
    sn::Machine m(rom);
    CB_CHECK_EQ(cb::engine::pcode_cpu::run(m, 0x008100).regs.a & 0xFF, 0x20);
});

CB_TEST("engine.pcode_cpu.w65816_long_calls_block_moves_and_the_stack", [] {
    // REP #$30; LDX #$9000; LDY #$0200; LDA #$0003; MVN $7E,$00; PHA; PLA; RTL   (copies four bytes of ROM at $00:9000 to $7E:0200)
    const auto rom = rom_with({0xC2, 0x30, 0xA2, 0x00, 0x90, 0xA0, 0x00, 0x02, 0xA9, 0x03, 0x00, 0x54, 0x7E, 0x00, 0x48, 0x68, 0x6B});
    sn::Machine m(rom);
    const sn::Run r = cb::engine::pcode_cpu::run(m, 0x008100);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(m.wram[0x200], 0);
    CB_CHECK_EQ(m.wram[0x201], 1);
    CB_CHECK_EQ(m.wram[0x202], 2);
    CB_CHECK_EQ(m.wram[0x203], 3);
    CB_CHECK_EQ(r.regs.a, 0xFFFF);   // the count ran out
    CB_CHECK_EQ(r.regs.dbr, 0x7E);
    CB_CHECK_EQ(r.regs.x, 0x9004);
    CB_CHECK_EQ(r.regs.y, 0x0204);
});

CB_TEST("engine.pcode_cpu.w65816_a_dma_to_vram_is_a_run_of_register_writes", [] {
    // SEP #$20; LDA #$01; STA $4300; LDA #$18; STA $4301; LDA #$00; STA $4302; LDA #$90; STA $4303; STZ $4304; LDA #$04; STA $4305; STZ $4306; LDA #$01; STA $420B; RTS
    const auto rom = rom_with({0xE2, 0x20, 0xA9, 0x01, 0x8D, 0x00, 0x43, 0xA9, 0x18, 0x8D, 0x01, 0x43, 0xA9, 0x00, 0x8D, 0x02, 0x43, 0xA9, 0x90, 0x8D, 0x03, 0x43,
                               0x9C, 0x04, 0x43, 0xA9, 0x04, 0x8D, 0x05, 0x43, 0x9C, 0x06, 0x43, 0xA9, 0x01, 0x8D, 0x0B, 0x42, 0x60});
    sn::Machine m(rom);
    const sn::Run r = cb::engine::pcode_cpu::run(m, 0x008100);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(r.io.size(), 12u);
    CB_CHECK_EQ(r.io[0].text(), "w DMAP0 01");
    CB_CHECK_EQ(r.io[1].text(), "w BBAD0 18");
    CB_CHECK_EQ(r.io[3].text(), "w A1T0H 90");
    CB_CHECK_EQ(r.io[7].text(), "w MDMAEN 01");
    CB_CHECK_EQ(r.io[8].text(), "w VMDATAL 00");   // the DMA: four bytes alternating the two VRAM data registers ($9000 holds 0 1 2 3)
    CB_CHECK_EQ(r.io[9].text(), "w VMDATAH 01");
    CB_CHECK_EQ(r.io[10].text(), "w VMDATAL 02");
    CB_CHECK_EQ(r.io[11].text(), "w VMDATAH 03");
});

CB_TEST("engine.pcode_cpu.w65816_the_chips_answer_as_an_idle_console", [] {
    // LDA $4210; LDA $4210; LDA $2140; LDX $2141 (as a byte); RTS  -- all in the register space
    const auto rom = rom_with({0xAD, 0x10, 0x42, 0xAD, 0x10, 0x42, 0xAD, 0x40, 0x21, 0xAE, 0x41, 0x21, 0x60});
    sn::Machine m(rom);
    const sn::Run r = cb::engine::pcode_cpu::run(m, 0x008100);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(r.io.size(), 4u);
    CB_CHECK_EQ(r.io[0].text(), "r RDNMI 82");
    CB_CHECK_EQ(r.io[1].text(), "r RDNMI 02");   // vertical blank shows on every other read
    CB_CHECK_EQ(r.io[2].text(), "r APUIO0 aa");   // the sound CPU's ready signature
    CB_CHECK_EQ(r.regs.x, 0xBB);
});

CB_TEST("engine.pcode_cpu.w65816_the_multiplier_and_divider", [] {
    // SEP #$20; LDA #$07; STA $4202; LDA #$09; STA $4203; LDA $4216; RTS  (7 * 9 = 63)
    const auto rom = rom_with({0xE2, 0x20, 0xA9, 0x07, 0x8D, 0x02, 0x42, 0xA9, 0x09, 0x8D, 0x03, 0x42, 0xAD, 0x16, 0x42, 0x60});
    sn::Machine m(rom);
    CB_CHECK_EQ(cb::engine::pcode_cpu::run(m, 0x008100).regs.a & 0xFF, 63);
});

CB_TEST("engine.pcode_cpu.w65816_software_interrupts_and_a_halt", [] {
    // BRK #$00 goes through the native BRK vector ($8063: RTI) and comes back to the next instruction: RTS
    const auto brk = rom_with({0x00, 0x00, 0x60});
    sn::Machine m(brk);
    CB_CHECK_EQ(cb::engine::pcode_cpu::run(m, 0x008100).exit(), "return");
    const auto wai = rom_with({0xCB, 0x60});
    sn::Machine m2(wai);
    const sn::Run r = cb::engine::pcode_cpu::run(m2, 0x008100);
    CB_CHECK(r.end == sn::End::Stopped);
    CB_CHECK_EQ(r.exit(), "stopped: WAI at 008100");
});

CB_TEST("engine.pcode_cpu.w65816_emulation_mode_has_a_page_one_stack_and_eight_bit_registers", [] {
    // REP #$30 does nothing in emulation mode: LDA #$12 stays one byte; PHA; PLA; RTS
    const auto rom = rom_with({0xC2, 0x30, 0xA9, 0x12, 0x48, 0x68, 0x60});
    sn::Machine m(rom);
    const sn::Run r = cb::engine::pcode_cpu::run(m, 0x008100, {.native = false});
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK(r.regs.e);
    CB_CHECK_EQ(r.regs.a & 0xFF, 0x12);
    CB_CHECK_EQ(r.regs.s >> 8, 1);
});
