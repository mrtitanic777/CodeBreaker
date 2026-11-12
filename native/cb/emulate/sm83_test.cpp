// cb/emulate/sm83_test.cpp -- the SM83 interpreter: flags of the ALU, INC/DEC, 16-bit and SP arithmetic, rotates and
// bit operations, DAA, the loads (HL+ and HL-, the $FF00 page, SP stores), the stack, jumps, calls and conditions,
// HALT / STOP / undecodable bytes, devices on the bus, running a function, and the run record.
//
// Every expected value is worked out by hand from the Game Boy CPU manual (Pan Docs) and the SM83 opcode map, by writing
// the program in RGBDS syntax and assembling it with cb::decompile::sm83 (not recorded from this code). The old harness
// (state/work/typed-port/emulate.sm83: 7,952 cases made by the Python module) agrees on every one of its step and run
// cases that the CPU defines.
//
// DIVERGES from the Python module:
//  - Registers are 8 and 16 bits wide types (Python's ints could hold more), and F keeps only its high nibble when run()
//    is given it (the Python stored whatever it was handed).
//  - A run's `io` is what that run logged (the Python gave the bus's whole log, carried over a reused bus).
//  - A CPU that was told HALT, hit STOP, or cannot decode, returns a Fault from step() instead of raising.
#include "cb/emulate/sm83.hpp"

#include <format>
#include <string>
#include <vector>

#include "cb/core/testing.hpp"
#include "cb/decompile/sm83.hpp"

namespace s = cb::emulate::sm83;
namespace d = cb::decompile::sm83;
using cb::Bytes;
namespace fl = s::flag;

#define CHECK_U(actual, expected) CB_CHECK_EQ(static_cast<std::uint64_t>(actual), static_cast<std::uint64_t>(expected))

namespace {

constexpr std::uint16_t kCode = 0x8000;

// Lines of source assembled in place at kCode, then a CPU about to run them.
struct Rig {
    s::Bus bus;
    s::Cpu cpu;
    std::uint16_t end = kCode;
    explicit Rig(const std::vector<std::string>& lines) : cpu(bus) {
        for (const std::string& l : lines) {
            const Bytes b = d::assemble(l, end);
            std::copy(b.begin(), b.end(), bus.memory().begin() + end);
            end = static_cast<std::uint16_t>(end + b.size());
        }
        cpu.regs.pc = kCode;
    }
    // Run every instruction of the program.
    void go() {
        while (cpu.regs.pc < end) CB_CHECK(!cpu.step());
    }
    std::uint8_t f() const { return cpu.regs.f; }
};

}  // namespace

// ---- the ALU ----------------------------------------------------------------------------------------------------------

CB_TEST("emulate.sm83.add_adc_flags", [] {
    struct Case {
        std::vector<std::string> lines;
        std::uint8_t a, f;
    };
    const std::vector<Case> cases = {
        {{"LD A,$0F", "ADD A,$01"}, 0x10, fl::H},
        {{"LD A,$FF", "ADD A,$01"}, 0x00, fl::Z | fl::H | fl::C},
        {{"LD A,$80", "ADD A,$80"}, 0x00, fl::Z | fl::C},
        {{"SCF", "LD A,$0E", "ADC A,$01"}, 0x10, fl::H},    // 0E + 01 + carry
        {{"LD A,$01", "LD B,$02", "ADD A,B"}, 0x03, 0},
    };
    for (const Case& c : cases) {
        Rig r(c.lines);
        r.go();
        CHECK_U(r.cpu.regs.a, c.a);
        CHECK_U(r.f(), c.f);
    }
});

CB_TEST("emulate.sm83.sub_sbc_cp_flags", [] {
    struct Case {
        std::vector<std::string> lines;
        std::uint8_t a, f;
    };
    const std::vector<Case> cases = {
        {{"LD A,$10", "SUB $01"}, 0x0F, fl::N | fl::H},
        {{"LD A,$00", "SUB $01"}, 0xFF, fl::N | fl::H | fl::C},
        {{"LD A,$05", "CP $05"}, 0x05, fl::Z | fl::N},             // CP leaves A
        {{"SCF", "LD A,$05", "SBC A,$05"}, 0xFF, fl::N | fl::H | fl::C},
        {{"LD A,$05", "SUB A"}, 0x00, fl::Z | fl::N},
    };
    for (const Case& c : cases) {
        Rig r(c.lines);
        r.go();
        CHECK_U(r.cpu.regs.a, c.a);
        CHECK_U(r.f(), c.f);
    }
});

CB_TEST("emulate.sm83.logic_flags", [] {
    Rig a({"LD A,$F0", "AND $0F"});
    a.go();
    CHECK_U(a.cpu.regs.a, 0);
    CHECK_U(a.f(), fl::Z | fl::H);   // AND always sets H
    Rig x({"LD A,$5A", "XOR A"});
    x.go();
    CHECK_U(x.cpu.regs.a, 0);
    CHECK_U(x.f(), fl::Z);
    Rig o({"XOR A", "OR $01"});
    o.go();
    CHECK_U(o.cpu.regs.a, 1);
    CHECK_U(o.f(), 0);
});

CB_TEST("emulate.sm83.inc_dec_keep_carry", [] {
    struct Case {
        std::vector<std::string> lines;
        std::uint8_t b, f;
    };
    const std::vector<Case> cases = {
        {{"SCF", "LD B,$0F", "INC B"}, 0x10, fl::C | fl::H},
        {{"LD B,$FF", "INC B"}, 0x00, fl::Z | fl::H},
        {{"LD B,$10", "DEC B"}, 0x0F, fl::N | fl::H},
        {{"LD B,$01", "DEC B"}, 0x00, fl::Z | fl::N},
        {{"SCF", "LD B,$80", "DEC B"}, 0x7F, fl::C | fl::N | fl::H},   // the low nibble borrows
    };
    for (const Case& c : cases) {
        Rig r(c.lines);
        r.go();
        CHECK_U(r.cpu.regs.b, c.b);
        CHECK_U(r.f(), c.f);
    }
});

CB_TEST("emulate.sm83.inc_dec_through_hl_and_sixteen_bit_pairs_touch_no_flags", [] {
    Rig r({"LD HL,$C000", "LD (HL),$0F", "INC (HL)", "LD BC,$FFFF", "INC BC", "LD DE,$0000", "DEC DE", "LD SP,$0000", "DEC SP"});
    r.go();
    CHECK_U(r.bus.memory()[0xC000], 0x10);
    CHECK_U(r.cpu.regs.bc(), 0x0000);
    CHECK_U(r.cpu.regs.de(), 0xFFFF);
    CHECK_U(r.cpu.regs.sp, 0xFFFF);
    CHECK_U(r.f(), fl::H);   // only INC (HL) set flags
});

CB_TEST("emulate.sm83.add_hl_and_sp_arithmetic", [] {
    {   // ADD HL,rr: Z is kept, H from bit 11, C from bit 15
        Rig r({"XOR A", "LD HL,$0FFF", "LD BC,$0001", "ADD HL,BC"});
        r.go();
        CHECK_U(r.cpu.regs.hl(), 0x1000);
        CHECK_U(r.f(), fl::Z | fl::H);
    }
    {
        Rig r({"XOR A", "LD HL,$FFFF", "LD DE,$0001", "ADD HL,DE"});
        r.go();
        CHECK_U(r.cpu.regs.hl(), 0x0000);
        CHECK_U(r.f(), fl::Z | fl::H | fl::C);
    }
    {   // ADD SP,e: H and C from the low byte's unsigned add, Z and N cleared
        Rig r({"XOR A", "ADD SP,$08"});
        r.cpu.regs.sp = 0xFFF8;
        r.go();
        CHECK_U(r.cpu.regs.sp, 0x0000);
        CHECK_U(r.f(), fl::H | fl::C);
    }
    {
        Rig r({"ADD SP,-1"});
        r.cpu.regs.sp = 0x0000;
        r.go();
        CHECK_U(r.cpu.regs.sp, 0xFFFF);
        CHECK_U(r.f(), 0);
    }
    {   // LD HL,SP+e: the same flags, SP unchanged
        Rig r({"LD HL,SP+$05"});
        r.cpu.regs.sp = 0x00FF;
        r.go();
        CHECK_U(r.cpu.regs.hl(), 0x0104);
        CHECK_U(r.cpu.regs.sp, 0x00FF);
        CHECK_U(r.f(), fl::H | fl::C);
    }
    {
        Rig r({"LD SP,HL", "LD HL,SP-$01"});
        r.cpu.regs.h = 0x12;
        r.cpu.regs.l = 0x34;
        r.go();
        CHECK_U(r.cpu.regs.sp, 0x1234);
        CHECK_U(r.cpu.regs.hl(), 0x1233);
    }
});

// ---- rotates, shifts, bits --------------------------------------------------------------------------------------------

CB_TEST("emulate.sm83.accumulator_rotates_clear_z_but_the_cb_forms_set_it", [] {
    {
        Rig r({"LD A,$85", "RLCA"});
        r.go();
        CHECK_U(r.cpu.regs.a, 0x0B);
        CHECK_U(r.f(), fl::C);
    }
    {
        Rig r({"LD A,$85", "RLA"});
        r.go();
        CHECK_U(r.cpu.regs.a, 0x0A);
        CHECK_U(r.f(), fl::C);
    }
    {
        Rig r({"LD A,$01", "RRCA"});
        r.go();
        CHECK_U(r.cpu.regs.a, 0x80);
        CHECK_U(r.f(), fl::C);
    }
    {   // RLA of 0 with no carry is zero, and Z stays clear
        Rig r({"XOR A", "LD A,$00", "RLA"});
        r.go();
        CHECK_U(r.f(), 0);
    }
    {   // RL B of 0x80: zero, with Z
        Rig r({"LD B,$80", "RL B"});
        r.go();
        CHECK_U(r.cpu.regs.b, 0);
        CHECK_U(r.f(), fl::Z | fl::C);
    }
});

CB_TEST("emulate.sm83.cb_rotates_and_shifts", [] {
    struct Case {
        std::vector<std::string> lines;
        std::uint8_t b, f;
    };
    const std::vector<Case> cases = {
        {{"LD B,$81", "RLC B"}, 0x03, fl::C},
        {{"LD B,$81", "RRC B"}, 0xC0, fl::C},
        {{"SCF", "LD B,$80", "RR B"}, 0xC0, 0},          // carry in at the top, bit 0 out
        {{"LD B,$80", "SLA B"}, 0x00, fl::Z | fl::C},
        {{"LD B,$81", "SRA B"}, 0xC0, fl::C},            // the sign is kept
        {{"LD B,$81", "SRL B"}, 0x40, fl::C},
        {{"LD B,$F1", "SWAP B"}, 0x1F, 0},
        {{"LD B,$00", "SWAP B"}, 0x00, fl::Z},
    };
    for (const Case& c : cases) {
        Rig r(c.lines);
        r.go();
        CHECK_U(r.cpu.regs.b, c.b);
        CHECK_U(r.f(), c.f);
    }
    // through memory
    Rig m({"LD HL,$C000", "LD (HL),$01", "SLA (HL)"});
    m.go();
    CHECK_U(m.bus.memory()[0xC000], 0x02);
});

CB_TEST("emulate.sm83.bit_set_res", [] {
    {
        Rig r({"LD A,$80", "BIT 7,A"});
        r.go();
        CHECK_U(r.f(), fl::H);   // bit set: Z clear, H always set
    }
    {
        Rig r({"SCF", "LD A,$80", "BIT 0,A"});
        r.go();
        CHECK_U(r.f(), fl::Z | fl::H | fl::C);   // carry kept
    }
    {
        Rig r({"LD B,$00", "SET 3,B", "SET 7,B", "RES 3,B"});
        r.go();
        CHECK_U(r.cpu.regs.b, 0x80);
    }
    {   // through (HL): read then write
        Rig r({"LD HL,$C000", "LD (HL),$FF", "RES 0,(HL)", "BIT 0,(HL)"});
        r.go();
        CHECK_U(r.bus.memory()[0xC000], 0xFE);
        CHECK_U(r.f(), fl::Z | fl::H);
    }
});

CB_TEST("emulate.sm83.daa_cpl_scf_ccf", [] {
    {   // 15 + 27 = 42 in BCD
        Rig r({"LD A,$15", "ADD A,$27", "DAA"});
        r.go();
        CHECK_U(r.cpu.regs.a, 0x42);
        CHECK_U(r.f(), 0);
    }
    {   // 99 + 01 = 100: A = 00 with carry
        Rig r({"LD A,$99", "ADD A,$01", "DAA"});
        r.go();
        CHECK_U(r.cpu.regs.a, 0x00);
        CHECK_U(r.f(), fl::Z | fl::C);
    }
    {   // 42 - 15 = 27
        Rig r({"LD A,$42", "SUB $15", "DAA"});
        r.go();
        CHECK_U(r.cpu.regs.a, 0x27);
        CHECK_U(r.f(), fl::N);
    }
    {
        Rig r({"LD A,$0F", "CPL"});
        r.go();
        CHECK_U(r.cpu.regs.a, 0xF0);
        CHECK_U(r.f(), fl::N | fl::H);
    }
    {   // SCF keeps Z; CCF flips C; both clear N and H
        Rig r({"XOR A", "SCF", "CCF", "CCF"});
        r.go();
        CHECK_U(r.f(), fl::Z | fl::C);
    }
});

// ---- loads, stack -----------------------------------------------------------------------------------------------------

CB_TEST("emulate.sm83.loads_through_hl_with_increment_and_decrement", [] {
    Rig r({"LD HL,$C000", "LD A,$7F", "LD (HL+),A", "LD (HL+),A", "LD A,$11", "LD (HL-),A", "LD A,(HL+)", "LD B,(HL)"});
    r.go();
    CHECK_U(r.bus.memory()[0xC000], 0x7F);
    CHECK_U(r.bus.memory()[0xC001], 0x7F);
    CHECK_U(r.bus.memory()[0xC002], 0x11);
    // HL ran 0xC000 -> C001 -> C002 -> (store at C002) C001 -> (read at C001) C002
    CHECK_U(r.cpu.regs.a, 0x7F);
    CHECK_U(r.cpu.regs.hl(), 0xC002);
    CHECK_U(r.cpu.regs.b, 0x11);
});

CB_TEST("emulate.sm83.high_page_and_absolute_loads", [] {
    Rig r({"LD A,$5A", "LDH ($FF80),A", "LD ($C100),A", "LD A,$00", "LDH A,($FF80)", "LD C,$81", "LD (C),A", "LD A,$00", "LD A,(C)",
           "LD B,A", "LD A,($C100)"});
    r.go();
    CHECK_U(r.bus.memory()[0xFF80], 0x5A);
    CHECK_U(r.bus.memory()[0xFF81], 0x5A);
    CHECK_U(r.bus.memory()[0xC100], 0x5A);
    CHECK_U(r.cpu.regs.b, 0x5A);
    CHECK_U(r.cpu.regs.a, 0x5A);
});

CB_TEST("emulate.sm83.ld_nn_sp_stores_both_bytes", [] {
    Rig r({"LD SP,$BEEF", "LD ($C200),SP"});
    r.go();
    CHECK_U(r.bus.memory()[0xC200], 0xEF);
    CHECK_U(r.bus.memory()[0xC201], 0xBE);
});

CB_TEST("emulate.sm83.push_pop_and_the_low_nibble_of_f", [] {
    Rig r({"LD SP,$D000", "LD BC,$12FF", "PUSH BC", "POP AF", "PUSH AF", "POP DE"});
    r.go();
    CHECK_U(r.cpu.regs.a, 0x12);
    CHECK_U(r.cpu.regs.f, 0xF0);   // bits 3-0 of F are always zero
    CHECK_U(r.cpu.regs.de(), 0x12F0);
    CHECK_U(r.cpu.regs.sp, 0xD000);
    CHECK_U(r.bus.memory()[0xCFFF], 0x12);   // the high byte at the higher address
    CHECK_U(r.bus.memory()[0xCFFE], 0xF0);
});

// ---- control flow -----------------------------------------------------------------------------------------------------

CB_TEST("emulate.sm83.conditional_jumps_follow_z_and_c", [] {
    struct Case {
        std::vector<std::string> lines;
        std::uint8_t b;
    };
    // each case sets B=1 then skips "LD B,2" (4 bytes of JR plus 2 of LD) if the condition holds
    const std::vector<Case> cases = {
        {{"XOR A", "LD B,$01", "JR Z,$8007", "LD B,$02"}, 1},     // Z set: taken
        {{"LD A,$01", "AND A", "LD B,$01", "JR Z,$8009", "LD B,$02"}, 2},   // Z clear: not taken
        {{"XOR A", "LD B,$01", "JR NZ,$8007", "LD B,$02"}, 2},
        {{"SCF", "LD B,$01", "JR C,$8007", "LD B,$02"}, 1},
        {{"SCF", "LD B,$01", "JR NC,$8007", "LD B,$02"}, 2},
        {{"SCF", "LD B,$01", "JP C,$8008", "LD B,$02"}, 1},
        {{"XOR A", "LD B,$01", "JP NZ,$8008", "LD B,$02"}, 2},
    };
    for (const Case& c : cases) {
        Rig r(c.lines);
        // run to the end of the program or past it
        for (int i = 0; i < 20 && r.cpu.regs.pc < r.end; ++i) CB_CHECK(!r.cpu.step());
        CHECK_U(r.cpu.regs.b, c.b);
    }
});

CB_TEST("emulate.sm83.loop_call_ret_and_rst", [] {
    s::Bus bus;
    // main: LD B,3 ; loop: DEC B ; JR NZ,loop ; CALL $8100 ; RST $08 ; RET
    const std::vector<std::string> main_lines = {"LD B,$03", "DEC B", "JR NZ,$8002", "CALL $8100", "RST $08", "RET"};
    std::uint16_t at = kCode;
    for (const std::string& l : main_lines) {
        const Bytes b = d::assemble(l, at);
        std::copy(b.begin(), b.end(), bus.memory().begin() + at);
        at = static_cast<std::uint16_t>(at + b.size());
    }
    const Bytes sub = d::assemble("LD C,$07", 0x8100);
    std::copy(sub.begin(), sub.end(), bus.memory().begin() + 0x8100);
    const Bytes sub2 = d::assemble("RET", 0x8102);
    std::copy(sub2.begin(), sub2.end(), bus.memory().begin() + 0x8102);
    const Bytes rst = d::assemble("LD D,$09", 0x0008);
    std::copy(rst.begin(), rst.end(), bus.memory().begin() + 0x0008);
    const Bytes rst2 = d::assemble("RET", 0x000A);
    std::copy(rst2.begin(), rst2.end(), bus.memory().begin() + 0x000A);
    s::RunOptions o;
    o.sp = 0xDFF0;
    const s::RunResult r = s::run(bus, kCode, o);
    CB_CHECK(r.end == s::End::Return);
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CHECK_U(r.regs.b, 0);
    CHECK_U(r.regs.c, 7);
    CHECK_U(r.regs.d, 9);
    CHECK_U(r.regs.sp, 0xDFF0);
    // LD B (1) + 3 x (DEC, JR) + CALL + LD C + RET + RST + LD D + RET + final RET
    CHECK_U(r.steps, 1 + 6 + 1 + 2 + 1 + 2 + 1);
});

CB_TEST("emulate.sm83.jp_hl_reti_di_ei", [] {
    Rig r({"EI", "LD HL,$8010", "JP HL"});
    const Bytes tail = d::assemble("DI", 0x8010);
    std::copy(tail.begin(), tail.end(), r.bus.memory().begin() + 0x8010);
    for (int i = 0; i < 3; ++i) CB_CHECK(!r.cpu.step());
    CB_CHECK(r.cpu.regs.ime);
    CHECK_U(r.cpu.regs.pc, 0x8010);
    CB_CHECK(!r.cpu.step());
    CB_CHECK(!r.cpu.regs.ime);
    // RETI pops and enables interrupts
    Rig k({"RETI"});
    k.cpu.regs.sp = 0xD000;
    k.bus.memory()[0xD000] = 0x34;
    k.bus.memory()[0xD001] = 0x12;
    CB_CHECK(!k.cpu.step());
    CHECK_U(k.cpu.regs.pc, 0x1234);
    CB_CHECK(k.cpu.regs.ime);
    CHECK_U(k.cpu.regs.sp, 0xD002);
});

// ---- HALT, STOP, undecodable bytes ------------------------------------------------------------------------------------------

CB_TEST("emulate.sm83.halt_ends_a_run_on_the_next_step", [] {
    Rig r({"LD A,$01", "HALT", "LD A,$02"});
    CB_CHECK(!r.cpu.step());
    CB_CHECK(!r.cpu.step());   // HALT itself runs
    CB_CHECK(r.cpu.regs.halted);
    const auto fault = r.cpu.step();
    CB_CHECK(fault.has_value());
    CB_CHECK(fault->kind == s::Fault::Kind::Halt);
    CB_CHECK_EQ(fault->text(), std::string("HALT"));
    CHECK_U(r.cpu.regs.a, 1);
    CHECK_U(r.cpu.steps, 2);
    Rig m({"LD A,$01", "HALT"});
    const s::RunResult run = s::run(m.bus, kCode);
    CB_CHECK_EQ(run.exit(), std::string("stopped: HALT"));
    CHECK_U(run.steps, 2);
});

CB_TEST("emulate.sm83.stop_and_undecodable_bytes_stop_a_run", [] {
    {
        Rig r({"STOP"});
        const s::RunResult run = s::run(r.bus, kCode);
        CB_CHECK_EQ(run.exit(), std::string("stopped: STOP"));
        CHECK_U(run.steps, 0);   // the stopping instruction is not counted
        CHECK_U(run.regs.pc, 0x8002);
    }
    for (std::uint8_t opcode : cb::decompile::sm83::kIllegalOpcodes) {
        s::Bus bus;
        bus.memory()[kCode] = opcode;
        s::Cpu cpu(bus);
        cpu.regs.pc = kCode;
        const auto fault = cpu.step();
        CB_CHECK(fault.has_value());
        CB_CHECK(fault->kind == s::Fault::Kind::Undecodable);
        CB_CHECK_EQ(fault->text(), std::string("no instruction at 8000"));
        CHECK_U(cpu.regs.pc, kCode);
    }
});

// ---- the bus: devices, code that changes ------------------------------------------------------------------------------------

CB_TEST("emulate.sm83.device_ranges_are_called_and_logged", [] {
    Rig r2({"LDH A,($FF44)", "LD B,A", "LD A,$01", "LDH ($FFFF),A"});
    r2.bus.map(0xFF00, 0xFF7F, [](std::uint16_t a) -> std::uint8_t { return a == 0xFF44 ? 0x90 : 0; });
    r2.bus.map(0xFFFF, 0xFFFF);
    r2.go();
    CHECK_U(r2.cpu.regs.b, 0x90);
    const auto& log = r2.bus.log();
    CB_CHECK_EQ(log.size(), std::size_t{2});
    CB_CHECK(log[0].kind == s::BusAccess::Kind::Read);
    CHECK_U(log[0].where, 0xFF44);
    CHECK_U(log[0].value, 0x90);
    CB_CHECK(log[1].kind == s::BusAccess::Kind::Write);
    CHECK_U(log[1].where, 0xFFFF);
    CHECK_U(log[1].value, 1);
});

CB_TEST("emulate.sm83.code_is_decoded_again_when_the_bytes_under_it_change", [] {
    s::Bus bus;
    const Bytes a = d::assemble("LD A,$01", kCode);
    std::copy(a.begin(), a.end(), bus.memory().begin() + kCode);
    s::Cpu cpu(bus);
    cpu.regs.pc = kCode;
    CB_CHECK(!cpu.step());
    CHECK_U(cpu.regs.a, 1);
    bus.memory()[kCode + 1] = 0x02;   // the operand changes
    cpu.regs.pc = kCode;
    CB_CHECK(!cpu.step());
    CHECK_U(cpu.regs.a, 2);
    const Bytes b = d::assemble("LD B,$09", kCode);
    std::copy(b.begin(), b.end(), bus.memory().begin() + kCode);   // another instruction altogether
    cpu.regs.pc = kCode;
    CB_CHECK(!cpu.step());
    CHECK_U(cpu.regs.b, 9);
});

// ---- running -----------------------------------------------------------------------------------------------------------

CB_TEST("emulate.sm83.run_starts_from_the_given_registers", [] {
    Rig r({"ADD A,B", "RET"});
    s::RunOptions o;
    o.regs.a = std::uint8_t{3};
    o.regs.b = std::uint8_t{4};
    o.regs.f = std::uint8_t{0xFF};   // only the high nibble is F
    o.regs.ime = true;
    o.sp = 0xDFF0;
    const s::RunResult run = s::run(r.bus, kCode, o);
    CB_CHECK(run.end == s::End::Return);
    CHECK_U(run.regs.a, 7);
    CHECK_U(run.regs.f, 0);
    CHECK_U(run.steps, 2);
    CB_CHECK(run.regs.ime);
    CHECK_U(run.regs.sp, 0xDFF0);
});

CB_TEST("emulate.sm83.run_hits_the_step_limit", [] {
    Rig r({"JR $8000"});
    s::RunOptions o;
    o.max_steps = 25;
    const s::RunResult run = s::run(r.bus, kCode, o);
    CB_CHECK(run.end == s::End::Limit);
    CB_CHECK_EQ(run.exit(), std::string("limit"));
    CHECK_U(run.steps, 25);
    CB_CHECK(s::record(run).truncated);
});

CB_TEST("emulate.sm83.record_is_the_run_record", [] {
    Rig r({"LDH A,($FF44)", "LD B,A", "RET"});
    r.bus.map(0xFF00, 0xFF7F, [](std::uint16_t) -> std::uint8_t { return 0x90; });
    s::RunOptions o;
    o.regs.c = std::uint8_t{0x55};
    const cb::emulate::Run rec = s::record(s::run(r.bus, kCode, o));
    CB_CHECK(rec.exit && *rec.exit == "return");
    CB_CHECK(rec.endian == cb::emulate::Endian::Little);
    CHECK_U(rec.steps.value_or(0), 3);
    CB_CHECK(!rec.truncated);
    CHECK_U(std::get<std::uint64_t>(rec.results.at("A")), 0x90);
    CHECK_U(std::get<std::uint64_t>(rec.results.at("F")), 0);
    CHECK_U(std::get<std::uint64_t>(rec.results.at("BC")), 0x9055);
    CHECK_U(std::get<std::uint64_t>(rec.results.at("SP")), 0xDFF0);
    CB_CHECK_EQ(rec.io.size(), std::size_t{1});
    CB_CHECK_EQ(rec.io[0].kind, std::string("r"));
    CHECK_U(rec.io[0].port, 0xFF44);
    CHECK_U(rec.io[0].value, 0x90);
    std::vector<std::string> order;
    for (const auto& [k, v] : rec.results) order.push_back(k);
    CB_CHECK_EQ(order.size(), std::size_t{6});
    CB_CHECK_EQ(order.front(), std::string("A"));
    CB_CHECK_EQ(order.back(), std::string("SP"));
});

// ---- every opcode of both pages has a program ----------------------------------------------------------------------------

CB_TEST("emulate.sm83.every_opcode_steps_without_crashing", [] {
    // Each opcode of the main page and of the CB page (with operand bytes that point into memory), on a recognisable
    // register state: the step ends (ran, or a Fault) and never throws -- every pattern the decoder gives has a program.
    for (unsigned page = 0; page < 2; ++page) {
        for (unsigned opcode = 0; opcode < 256; ++opcode) {
            s::Bus bus;
            std::uint16_t at = kCode;
            if (page == 1) bus.memory()[at++] = 0xCB;
            bus.memory()[at++] = static_cast<std::uint8_t>(opcode);
            bus.memory()[at++] = 0x34;
            bus.memory()[at++] = 0xC1;
            s::Cpu cpu(bus);
            cpu.regs.pc = kCode;
            cpu.regs.a = 0x42;
            cpu.regs.f = 0x50;
            cpu.regs.b = 0xC0;
            cpu.regs.c = 0x10;
            cpu.regs.d = 0xC1;
            cpu.regs.e = 0x20;
            cpu.regs.h = 0xC2;
            cpu.regs.l = 0x30;
            cpu.regs.sp = 0xD000;
            (void)cpu.step();
        }
    }
});
