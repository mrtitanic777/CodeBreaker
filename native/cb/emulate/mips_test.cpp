// cb/emulate/mips_test.cpp -- the R3000A interpreter: the bus (the RAM's views, the devices), the arithmetic and logic groups, branches
// and their delay slots, the load delay slot, unaligned loads and stores, multiply and divide, the coprocessor registers, the
// exceptions that end a run, BIOS calls, running a function, and the run record.
//
// Every expected value is worked out by hand from the MIPS I architecture (the R3000A's load and branch delay slots, LWL/LWR/SWL/SWR's
// byte merges, the HI/LO results of the multiply and divide group), by writing the program in GNU as syntax and assembling it with
// cb::decompile::mips.
#include "cb/emulate/mips.hpp"

#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/decompile/mips.hpp"

namespace m = cb::emulate::mips;
namespace d = cb::decompile::mips;
using cb::Bytes;

#define CHECK_U(actual, expected) CB_CHECK_EQ(static_cast<std::uint64_t>(actual), static_cast<std::uint64_t>(expected))

namespace {

constexpr std::uint32_t kCode = 0x80010000;

// A program written line by line, assembled where it will run; `name:` lines are labels.
struct Asm {
    std::uint32_t base;
    std::vector<std::string> lines;
    explicit Asm(std::uint32_t at = kCode) : base(at) {}

    Asm& line(const std::string& text) {
        lines.push_back(text);
        return *this;
    }
    Bytes assemble() const {
        // pass 1 sizes the lines with every label standing for the start (a branch is in reach); pass 2 has the real addresses
        cb::decompile::NameLabels labels;
        for (const std::string& l : lines)
            if (l.back() == ':') labels[l.substr(0, l.size() - 1)] = base;
        std::uint32_t at = base;
        cb::decompile::NameLabels real;
        for (const std::string& l : lines) {
            if (l.back() == ':') real[l.substr(0, l.size() - 1)] = at;
            else at += static_cast<std::uint32_t>(d::assemble(l, at, labels).size());
        }
        Bytes out;
        at = base;
        for (const std::string& l : lines) {
            if (l.back() == ':') continue;
            const Bytes b = d::assemble(l, at, real);
            out.insert(out.end(), b.begin(), b.end());
            at += static_cast<std::uint32_t>(b.size());
        }
        return out;
    }
    void load(m::Bus& bus) const {
        const Bytes b = assemble();
        std::copy(b.begin(), b.end(), bus.memory().begin() + (base & 0x1FFFFF));
    }
};

// Run `program` (ending with `jr $ra; nop`) with the options.
m::RunResult run_program(Asm& program, m::Bus& bus, const m::RunOptions& options = {}) {
    program.load(bus);
    return m::run(bus, program.base, options);
}

}  // namespace

CB_TEST("emulate.mips.bus_views_of_the_ram", [] {
    m::Bus bus;
    bus.write(0x80010000, 4, 0x11223344);
    CHECK_U(bus.read(0x80010000, 4), 0x11223344);
    CHECK_U(bus.read(0xA0010000, 4), 0x11223344);   // KSEG1
    CHECK_U(bus.read(0x00010000, 4), 0x11223344);   // KUSEG
    CHECK_U(bus.read(0x00210000, 4), 0x11223344);   // the 2 MB seen again
    CHECK_U(bus.read(0x80010000, 1), 0x44);         // little-endian
    CHECK_U(bus.read(0x80010001, 2), 0x2233);
    bus.write(0x80010002, 2, 0xAABB);
    CHECK_U(bus.read(0x80010000, 4), 0xAABB3344);
    bus.write(0x1F800010, 4, 0x5555AAAA);   // the scratchpad
    CHECK_U(bus.read(0x9F800010, 4), 0x5555AAAA);
    CB_CHECK(bus.mapped(0x80000000));
    CB_CHECK(bus.mapped(0x1F801810));
    CB_CHECK(bus.mapped(0x1FC00000));
    CB_CHECK(!bus.mapped(0x30000000));
    CB_CHECK(!bus.mapped(0x1F900000));
    // memory (RAM) accesses are not logged; devices are
    CB_CHECK(bus.log().empty());
    bus.write(0x1F801810, 4, 0xE1000000);
    CHECK_U(bus.read(0x1F801814, 4), 0);
    CB_CHECK_EQ(bus.log().size(), std::size_t{2});
    CB_CHECK_EQ(bus.log()[0].text(), std::string("w4"));
    CHECK_U(bus.log()[0].address, 0x1F801810);
    CHECK_U(bus.log()[0].value, 0xE1000000);
    CB_CHECK_EQ(bus.log()[1].text(), std::string("r4"));
    // a device range answers; the last range mapped wins
    bus.map(0x1F801814, 0x1F801817, [](std::uint32_t, unsigned) -> std::uint32_t { return 0x1C000000; });
    CHECK_U(bus.read(0x1F801814, 4), 0x1C000000);
    CHECK_U(bus.read(0x1F801810, 4), 0);
    // an image
    const Bytes image = {1, 2, 3, 4};
    m::Bus with_image{cb::ByteView(image)};
    CHECK_U(with_image.read(0x80000000, 4), 0x04030201);
    CB_CHECK_THROWS_AS(cb::UsageError, m::Bus(Bytes(0x300000)));
});

CB_TEST("emulate.mips.arithmetic_and_logic", [] {
    m::Bus bus;
    Asm p;
    p.line("addiu $v0, $zero, 7").line("addiu $v1, $zero, -3").line("addu $t0, $v0, $v1").line("subu $t1, $v0, $v1").line("and $t2, $v0, $v1")
        .line("or $t3, $v0, $v1").line("xor $t4, $v0, $v1").line("nor $t5, $v0, $v1").line("slt $t6, $v1, $v0").line("sltu $t7, $v1, $v0")
        .line("sll $s0, $v0, 4").line("srl $s1, $v1, 28").line("sra $s2, $v1, 1").line("lui $s3, 0x8001").line("ori $s3, $s3, 0x1234")
        .line("andi $s4, $s3, 0xFF").line("xori $s5, $v0, 0xFF").line("slti $s6, $v1, -2").line("sltiu $s7, $v1, -2")
        .line("li $a0, 3").line("sllv $a1, $v0, $a0").line("srlv $a2, $v1, $a0").line("srav $a3, $v1, $a0").line("jr $ra").line("nop");
    const m::RunResult r = run_program(p, bus);
    CB_CHECK(r.end == m::End::Return);
    CHECK_U(r.r[8], 4);                  // 7 + -3
    CHECK_U(r.r[9], 10);                 // 7 - -3
    CHECK_U(r.r[10], 7 & 0xFFFFFFFD);    // 7 & ...FFFD = 5
    CHECK_U(r.r[11], 0xFFFFFFFF);        // 7 | -3
    CHECK_U(r.r[12], 0xFFFFFFFA);        // 7 ^ -3
    CHECK_U(r.r[13], 0x00000000);        // ~(7 | -3)
    CHECK_U(r.r[14], 1);                 // -3 < 7 signed
    CHECK_U(r.r[15], 0);                 // 0xFFFFFFFD < 7 unsigned: no
    CHECK_U(r.r[16], 0x70);
    CHECK_U(r.r[17], 0xF);               // 0xFFFFFFFD >> 28
    CHECK_U(r.r[18], 0xFFFFFFFE);        // -3 >> 1 (arithmetic) = -2
    CHECK_U(r.r[19], 0x80011234);
    CHECK_U(r.r[20], 0x34);
    CHECK_U(r.r[21], 7 ^ 0xFF);
    CHECK_U(r.r[22], 1);                 // -3 < -2
    CHECK_U(r.r[23], 1);                 // 0xFFFFFFFD < 0xFFFFFFFE unsigned
    CHECK_U(r.r[5], 56);                 // 7 << 3
    CHECK_U(r.r[6], 0x1FFFFFFF);         // 0xFFFFFFFD >> 3
    CHECK_U(r.r[7], 0xFFFFFFFF);         // -3 >> 3 = -1
    CHECK_U(r.r[0], 0);
    CB_CHECK(r.fault == std::nullopt);
});

CB_TEST("emulate.mips.register_zero_is_never_written", [] {
    m::Bus bus;
    Asm p;
    p.line("addiu $zero, $zero, 5").line("addu $v0, $zero, $zero").line("jr $ra").line("nop");
    const m::RunResult r = run_program(p, bus);
    CHECK_U(r.r[0], 0);
    CHECK_U(r.r[2], 0);
});

CB_TEST("emulate.mips.the_delay_slot_runs_before_a_branch_takes_effect", [] {
    m::Bus bus;
    Asm p;
    // a taken branch: its delay slot (v0 += 1) still runs; the instruction it skips (v0 += 100) does not
    p.line("addiu $v0, $zero, 0").line("beq $zero, $zero, skip").line("addiu $v0, $v0, 1").line("addiu $v0, $v0, 100").line("skip:")
        .line("bne $zero, $zero, out").line("addiu $v0, $v0, 10").line("addiu $v0, $v0, 1000").line("out:").line("jr $ra").line("addiu $v0, $v0, 5000");
    const m::RunResult r = run_program(p, bus);
    // 0; +1 (delay of taken beq); [skipped +100]; bne not taken: +10 (delay), +1000 (falls through); jr's delay: +5000
    CHECK_U(r.r[2], 1 + 10 + 1000 + 5000);
    CB_CHECK(r.end == m::End::Return);
});

CB_TEST("emulate.mips.jal_links_past_the_delay_slot_and_a_loop_counts", [] {
    m::Bus bus;
    Asm p;
    // main: v0 = f(3) + f(4), where f(x) = x * 2; ra saved in s0
    p.line("move $s0, $ra").line("addiu $a0, $zero, 3").line("jal f").line("nop").line("move $s1, $v0").line("addiu $a0, $zero, 4")
        .line("jal f").line("nop").line("addu $v0, $v0, $s1").line("jr $s0").line("nop")
        .line("f:").line("jr $ra").line("addu $v0, $a0, $a0");
    const m::RunResult r = run_program(p, bus);
    CHECK_U(r.r[2], 14);
    // the link: $ra after the first call was the address of the instruction after the delay slot (jal at +8, so +16)
    CHECK_U(r.r[31], kCode + 0x20);   // the second jal is at +0x18: $ra = +0x20
});

CB_TEST("emulate.mips.loops", [] {
    m::Bus bus;
    Asm p;
    // v0 = sum of 1..a0
    p.line("move $v0, $zero").line("loop:").line("addu $v0, $v0, $a0").line("addiu $a0, $a0, -1").line("bgtz $a0, loop").line("nop")
        .line("jr $ra").line("nop");
    m::RunOptions options;
    options.a = {100, 0, 0, 0};
    const m::RunResult r = run_program(p, bus, options);
    CHECK_U(r.r[2], 5050);
    CHECK_U(r.steps, 403);   // move; 100 x (addu addiu bgtz nop); jr and its delay slot
});

CB_TEST("emulate.mips.the_load_delay_slot", [] {
    m::Bus bus;
    bus.write(0x80020000, 4, 0x1234);
    Asm p;
    // $t0 holds 5; lw $t0; the next instruction still sees 5; the one after sees 0x1234
    p.line("lui $a0, 0x8002").line("addiu $t0, $zero, 5").line("lw $t0, 0($a0)").line("addu $t1, $t0, $zero").line("addu $t2, $t0, $zero")
        // a register the instruction in the delay slot writes keeps what it wrote
        .line("lw $t3, 0($a0)").line("addiu $t3, $zero, 9").line("addu $t4, $t3, $zero")
        .line("jr $ra").line("nop");
    const m::RunResult r = run_program(p, bus);
    CHECK_U(r.r[9], 5);
    CHECK_U(r.r[10], 0x1234);
    CHECK_U(r.r[11], 9);
    CHECK_U(r.r[12], 9);
});

CB_TEST("emulate.mips.loads_and_stores_of_every_size", [] {
    m::Bus bus;
    bus.write(0x80020000, 4, 0x8182FF7F);
    Asm p;
    p.line("lui $a0, 0x8002").line("lb $t0, 0($a0)").line("nop").line("lb $t1, 1($a0)").line("nop").line("lbu $t2, 1($a0)").line("nop")
        .line("lh $t3, 2($a0)").line("nop").line("lhu $t4, 2($a0)").line("nop").line("lw $t5, 0($a0)").line("nop")
        .line("addiu $t6, $zero, 0x1234").line("sb $t6, 8($a0)").line("sh $t6, 10($a0)").line("sw $t6, 12($a0)").line("jr $ra").line("nop");
    const m::RunResult r = run_program(p, bus);
    CHECK_U(r.r[8], 0x7F);
    CHECK_U(r.r[9], 0xFFFFFFFF);
    CHECK_U(r.r[10], 0xFF);
    CHECK_U(r.r[11], 0xFFFF8182);
    CHECK_U(r.r[12], 0x8182);
    CHECK_U(r.r[13], 0x8182FF7F);
    CHECK_U(bus.read(0x80020008, 1), 0x34);
    CHECK_U(bus.read(0x8002000A, 2), 0x1234);
    CHECK_U(bus.read(0x8002000C, 4), 0x1234);
});

CB_TEST("emulate.mips.unaligned_loads_and_stores", [] {
    m::Bus bus;
    // bytes 00 11 22 33 44 55 66 77 at $80020000
    bus.write(0x80020000, 4, 0x33221100);
    bus.write(0x80020004, 4, 0x77665544);
    Asm p;
    // the usual unaligned word load of the bytes at +1..+4, and the store of a word at +9..+12
    p.line("lui $a0, 0x8002").line("lwr $t0, 1($a0)").line("lwl $t0, 4($a0)").line("nop").line("lui $t1, 0xAABB").line("ori $t1, $t1, 0xCCDD")
        .line("swr $t1, 9($a0)").line("swl $t1, 12($a0)")
        // the other unaligned offsets: lwl alone at +2 keeps the register's low bytes
        .line("lui $t2, 0xFFFF").line("ori $t2, $t2, 0xFFFF").line("lwl $t2, 2($a0)").line("nop").line("jr $ra").line("nop");
    const m::RunResult r = run_program(p, bus);
    CHECK_U(r.r[8], 0x44332211);
    CHECK_U(bus.read(0x80020009, 1), 0xDD);   // swr/swl: the register's bytes in little-endian order at +9..+12
    CHECK_U(bus.read(0x8002000A, 1), 0xCC);
    CHECK_U(bus.read(0x8002000B, 1), 0xBB);
    CHECK_U(bus.read(0x8002000C, 1), 0xAA);
    CHECK_U(bus.read(0x80020008, 1), 0);      // the byte before is untouched
    CHECK_U(bus.read(0x8002000D, 1), 0);
    // lwl at +2: the aligned word's low three bytes (00 11 22) go to the register's high three bytes; the low byte stays 0xFF
    CHECK_U(r.r[10], 0x221100FF);
});

CB_TEST("emulate.mips.multiply_and_divide", [] {
    m::Bus bus;
    Asm p;
    p.line("lui $a0, 0x7FFF").line("ori $a0, $a0, 0xFFFF").line("addiu $a1, $zero, 4").line("mult $a0, $a1").line("mflo $t0").line("mfhi $t1")
        .line("multu $a0, $a1").line("mflo $t2").line("mfhi $t3")
        .line("addiu $a2, $zero, -7").line("addiu $a3, $zero, 2").line("div $a2, $a3").line("mflo $t4").line("mfhi $t5")
        .line("divu $a2, $a3").line("mflo $t6").line("mfhi $t7")
        .line("div $a3, $zero").line("mflo $s0").line("mfhi $s1")
        .line("div $a2, $zero").line("mflo $s2").line("mfhi $s3")
        .line("divu $a3, $zero").line("mflo $s4").line("mfhi $s5").line("jr $ra").line("nop");
    const m::RunResult r = run_program(p, bus);
    CHECK_U(r.r[8], 0xFFFFFFFC);   // 0x7FFFFFFF * 4 = 0x1_FFFFFFFC
    CHECK_U(r.r[9], 1);
    CHECK_U(r.r[10], 0xFFFFFFFC);
    CHECK_U(r.r[11], 1);
    CHECK_U(r.r[12], 0xFFFFFFFD);  // -7 / 2 = -3 (truncated), remainder -1
    CHECK_U(r.r[13], 0xFFFFFFFF);
    CHECK_U(r.r[14], 0x7FFFFFFC);  // 0xFFFFFFF9 / 2
    CHECK_U(r.r[15], 1);
    CHECK_U(r.r[16], 0xFFFFFFFF);  // 2 / 0: LO = -1 (the dividend is positive), HI = the dividend
    CHECK_U(r.r[17], 2);
    CHECK_U(r.r[18], 1);           // -7 / 0: LO = 1 (the dividend is negative)
    CHECK_U(r.r[19], 0xFFFFFFF9);
    CHECK_U(r.r[20], 0xFFFFFFFF);  // 2 / 0 unsigned
    CHECK_U(r.r[21], 2);
});

CB_TEST("emulate.mips.overflow_alignment_and_bus_errors_end_the_run", [] {
    {
        m::Bus bus;
        Asm p;
        p.line("lui $a0, 0x7FFF").line("ori $a0, $a0, 0xFFFF").line("addiu $v0, $zero, 1").line("add $v1, $a0, $v0").line("jr $ra").line("nop");
        const m::RunResult r = run_program(p, bus);
        CB_CHECK(r.end == m::End::Stopped);
        CB_CHECK_EQ(r.exit(), std::string("stopped: arithmetic overflow"));
        CHECK_U(r.r[3], 0);   // the destination is not written
        // ADDU does not trap
        m::Bus bus2;
        Asm q;
        q.line("lui $a0, 0x7FFF").line("ori $a0, $a0, 0xFFFF").line("addu $v1, $a0, $a0").line("jr $ra").line("nop");
        CHECK_U(run_program(q, bus2).r[3], 0xFFFFFFFE);
    }
    {
        m::Bus bus;
        Asm p;
        p.line("lui $a0, 0x8002").line("lw $v0, 2($a0)").line("nop").line("jr $ra").line("nop");
        const m::RunResult r = run_program(p, bus);
        CB_CHECK_EQ(r.exit(), std::string("stopped: address error at 80020002"));
    }
    {
        m::Bus bus;
        Asm p;
        p.line("lui $a0, 0x3000").line("sw $zero, 0($a0)").line("jr $ra").line("nop");
        CB_CHECK_EQ(run_program(p, bus).exit(), std::string("stopped: bus error at 30000000"));
    }
    {
        m::Bus bus;
        bus.write(0x80010000, 4, 0xFFFFFFFF);
        const m::RunResult r = m::run(bus, 0x80010000);
        CB_CHECK_EQ(r.exit(), std::string("stopped: no instruction at 80010000"));
    }
    {
        m::Bus bus;
        Asm p;
        p.line("break 7").line("jr $ra").line("nop");
        CB_CHECK_EQ(run_program(p, bus).exit(), std::string("stopped: break 7"));
    }
    {
        m::Bus bus;   // a GTE command is not run
        bus.write(0x80010000, 4, 0x4A180001);
        const m::RunResult r = m::run(bus, 0x80010000);
        CB_CHECK_EQ(r.exit(), std::string("stopped: not run: rtps"));
    }
});

CB_TEST("emulate.mips.an_endless_loop_hits_the_step_limit", [] {
    m::Bus bus;
    Asm p;
    p.line("again:").line("b again").line("nop");
    m::RunOptions options;
    options.max_steps = 1000;
    const m::RunResult r = run_program(p, bus, options);
    CB_CHECK(r.end == m::End::Limit);
    CB_CHECK_EQ(r.exit(), std::string("limit"));
    CHECK_U(r.steps, 1000);
    CB_CHECK(m::record(r).truncated);
});

CB_TEST("emulate.mips.devices_are_logged_and_answer", [] {
    m::Bus bus;
    bus.map(0x1F801814, 0x1F801817, [](std::uint32_t, unsigned) -> std::uint32_t { return 0x14000000; });   // GPUSTAT: ready
    Asm p;
    // wait for GPUSTAT bit 26, then write a GP0 command
    p.line("lui $a0, 0x1F80").line("again:").line("lw $t0, 0x1814($a0)").line("nop").line("lui $t1, 0x0400").line("and $t0, $t0, $t1")
        .line("beq $t0, $zero, again").line("nop").line("lui $t2, 0xE100").line("sw $t2, 0x1810($a0)").line("jr $ra").line("nop");
    const m::RunResult r = run_program(p, bus);
    CB_CHECK(r.end == m::End::Return);
    CB_CHECK_EQ(r.io.size(), std::size_t{2});
    CB_CHECK_EQ(r.io[0].text(), std::string("r4"));
    CHECK_U(r.io[0].address, 0x1F801814);
    CHECK_U(r.io[0].value, 0x14000000);
    CB_CHECK_EQ(r.io[1].text(), std::string("w4"));
    CHECK_U(r.io[1].address, 0x1F801810);
    CHECK_U(r.io[1].value, 0xE1000000);
    // the run record carries them
    const cb::emulate::Run rec = m::record(r);
    CB_CHECK_EQ(rec.io.size(), std::size_t{2});
    CB_CHECK_EQ(rec.io[1].kind, std::string("w4"));
    CHECK_U(rec.io[1].port, 0x1F801810);
});

CB_TEST("emulate.mips.bios_calls_are_logged_and_return", [] {
    m::Bus bus;
    Asm p;
    // putchar('A') through B0 function $3C, then A0 function $3F (printf) with a pointer
    p.line("move $s0, $ra").line("addiu $a0, $zero, 65").line("addiu $t2, $zero, 0xB0").line("jalr $t2").line("addiu $t1, $zero, 0x3C")
        .line("lui $a0, 0x8001").line("addiu $t2, $zero, 0xA0").line("jalr $t2").line("addiu $t1, $zero, 0x3F").line("addiu $v1, $v0, 77")
        .line("jr $s0").line("nop");
    const m::RunResult r = run_program(p, bus);
    CB_CHECK(r.end == m::End::Return);
    CB_CHECK_EQ(r.bios.size(), std::size_t{2});
    CHECK_U(r.bios[0].table, 0xB0);
    CHECK_U(r.bios[0].function, 0x3C);
    CHECK_U(r.bios[0].args[0], 65);
    CHECK_U(r.bios[1].table, 0xA0);
    CHECK_U(r.bios[1].function, 0x3F);
    CHECK_U(r.bios[1].args[0], 0x80010000);
    CB_CHECK_EQ(r.bios[0].text(), std::string("B0:3C(41,0,0,0)"));
    CHECK_U(r.r[3], 77);   // the BIOS answered 0
    const cb::emulate::Run rec = m::record(r);
    CB_CHECK_EQ(rec.calls.size(), std::size_t{2});
    CB_CHECK_EQ(rec.calls[0].name.value(), std::string("B0:3C"));
});

CB_TEST("emulate.mips.critical_sections_return_through_syscall", [] {
    m::Bus bus;
    Asm p;
    p.line("addiu $a0, $zero, 1").line("syscall").line("move $s0, $v0").line("addiu $a0, $zero, 2").line("syscall").line("move $s1, $v0")
        .line("jr $ra").line("nop");
    const m::RunResult r = run_program(p, bus);
    CB_CHECK(r.end == m::End::Return);
    CHECK_U(r.r[16], 1);
    CHECK_U(r.r[17], 0);
    CB_CHECK_EQ(r.bios.size(), std::size_t{2});
    // any other syscall ends the run
    m::Bus bus2;
    Asm q;
    q.line("addiu $a0, $zero, 5").line("syscall").line("jr $ra").line("nop");
    CB_CHECK_EQ(run_program(q, bus2).exit(), std::string("stopped: syscall"));
});

CB_TEST("emulate.mips.coprocessor_registers", [] {
    m::Bus bus;
    Asm p;
    // GTE data registers move: a 16-bit register reads sign-extended, LZCS sets LZCR, the SXY FIFO pushes
    p.line("lui $t0, 0xFFFF").line("ori $t0, $t0, 0xFFF0").line("mtc2 $t0, $9").line("mfc2 $t1, $9").line("nop")
        .line("lui $t2, 0x0001").line("mtc2 $t2, $30").line("mfc2 $t3, $31").line("nop")
        .line("addiu $t4, $zero, 1").line("mtc2 $t4, $15").line("addiu $t4, $zero, 2").line("mtc2 $t4, $15").line("mfc2 $t5, $14").line("mfc2 $t6, $13").line("nop")
        .line("addiu $t7, $zero, 0x1234").line("ctc2 $t7, $4").line("cfc2 $s0, $4").line("nop")
        .line("mtc2 $zero, $30").line("mfc2 $s1, $31").line("nop")
        .line("addiu $s2, $zero, 0x7C1F").line("mtc2 $s2, $28").line("mfc2 $s3, $9").line("mfc2 $s4, $10").line("mfc2 $s5, $11").line("nop")
        // coprocessor 0: the status register, and RFE moving the mode bits
        .line("addiu $s6, $zero, 0x14").line("mtc0 $s6, $12").line("rfe").line("mfc0 $s7, $12").line("nop")
        .line("jr $ra").line("nop");
    const m::RunResult r = run_program(p, bus);
    CB_CHECK(r.end == m::End::Return);
    CHECK_U(r.r[9], 0xFFFFFFF0);    // IR1 sign-extended
    CHECK_U(r.r[11], 15);           // 0x10000 has 15 leading zeros
    CHECK_U(r.r[13], 2);            // SXY2 = last pushed
    CHECK_U(r.r[14], 1);            // SXY1 = the one before
    CHECK_U(r.r[16], 0x1234);
    CHECK_U(r.r[17], 32);           // LZCS = 0: 32 leading zeros
    CHECK_U(r.r[19], 0x1F * 0x80);  // IRGB 0x7C1F: red 0x1F, green 0, blue 0x1F
    CHECK_U(r.r[20], 0);
    CHECK_U(r.r[21], 0x1F * 0x80);
    CHECK_U(r.r[23], (0x14u & ~0xFu) | ((0x14u >> 2) & 0xFu));   // RFE: the low four bits take the previous mode's
});

CB_TEST("emulate.mips.arguments_and_registers_at_the_start", [] {
    m::Bus bus;
    Asm p;
    p.line("addu $v0, $a0, $a1").line("addu $v0, $v0, $a2").line("addu $v0, $v0, $a3").line("addu $v0, $v0, $s0").line("addu $v0, $v0, $t0")
        .line("move $v1, $sp").line("jr $ra").line("nop");
    m::RunOptions options;
    options.a = {1, 2, 4, 8};
    options.s = {16, 0, 0, 0, 0, 0, 0, 0};
    options.t = {32, 0, 0, 0, 0, 0, 0, 0};
    options.sp = 0x801FF000;
    const m::RunResult r = run_program(p, bus, options);
    CHECK_U(r.r[2], 63);
    CHECK_U(r.r[3], 0x801FF000);
    CHECK_U(r.r[31], m::kReturnAddress);
});

CB_TEST("emulate.mips.the_run_record", [] {
    m::Bus bus;
    Asm p;
    p.line("addiu $v0, $zero, 42").line("addiu $s3, $zero, 9").line("mult $v0, $v0").line("jr $ra").line("nop");
    const cb::emulate::Run run = m::record(run_program(p, bus));
    CB_CHECK_EQ(run.exit.value(), std::string("return"));
    CHECK_U(std::get<std::uint64_t>(run.results.at("V0")), 42);
    CHECK_U(std::get<std::uint64_t>(run.results.at("S3")), 9);
    CHECK_U(std::get<std::uint64_t>(run.results.at("LO")), 1764);
    CHECK_U(std::get<std::uint64_t>(run.results.at("HI")), 0);
    CB_CHECK(run.endian == cb::emulate::Endian::Little);
    CB_CHECK(!run.truncated);
    CB_CHECK(run.steps.value() > 0);
    // run + record
    m::Bus again;
    Asm q;
    q.line("jr $ra").line("addiu $v0, $zero, 1");
    q.load(again);
    CHECK_U(std::get<std::uint64_t>(m::recorded(again, kCode).results.at("V0")), 1);
});

CB_TEST("emulate.mips.gte_commands_that_need_no_division", [] {
    m::Bus bus;
    Asm p;
    // NCLIP: the screen triangle (0,0) (10,0) (0,10) has twice its area, 100, in MAC0 (the three points are SXY0..SXY2: y in the high half)
    p.line("li $t0, 0").line("mtc2 $t0, $12").line("li $t0, 10").line("mtc2 $t0, $13").line("lui $t0, 10").line("mtc2 $t0, $14").line("nclip")
        .line("mfc2 $s0, $24")
        // AVSZ3: ZSF3 = 0x555, SZ1..SZ3 = 100 200 300: MAC0 = 0x555 * 600, OTZ = MAC0 >> 12
        .line("li $t0, 0x555").line("ctc2 $t0, $29").line("li $t0, 100").line("mtc2 $t0, $17").line("li $t0, 200").line("mtc2 $t0, $18")
        .line("li $t0, 300").line("mtc2 $t0, $19").line("avsz3").line("mfc2 $s1, $24").line("mfc2 $s2, $7")
        // SQR with sf = 0: MAC = IR squared; IR3 = 0x100 squared does not fit 16 bits and saturates (flag bit 22)
        .line("li $t0, -3").line("mtc2 $t0, $9").line("li $t0, 4").line("mtc2 $t0, $10").line("li $t0, 0x100").line("mtc2 $t0, $11").line("sqr 0, 0")
        .line("mfc2 $s3, $9").line("mfc2 $s4, $10").line("mfc2 $s5, $11").line("cfc2 $s6, $31")
        .line("jr $ra").line("nop");
    const m::RunResult r = run_program(p, bus);
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CHECK_U(r.r[16], 100);
    CHECK_U(r.r[17], 0x555 * 600);
    CHECK_U(r.r[18], (0x555 * 600) >> 12);
    CHECK_U(r.r[19], 9);
    CHECK_U(r.r[20], 16);
    CHECK_U(r.r[21], 0x7FFF);
    CHECK_U(r.r[22], 1u << 22);
});

CB_TEST("emulate.mips.gte_matrix_multiply_and_the_colour_fifo", [] {
    m::Bus bus;
    Asm p;
    // MVMVA with the identity rotation (0x1000 = 1.0), V0 = (1, 2, 3) and TR = (10, 20, 30), sf = 1: IR = TR + V0
    p.line("li $t0, 0x1000").line("ctc2 $t0, $0").line("ctc2 $t0, $2").line("ctc2 $t0, $4")
        .line("li $t0, 10").line("ctc2 $t0, $5").line("li $t0, 20").line("ctc2 $t0, $6").line("li $t0, 30").line("ctc2 $t0, $7")
        .line("lui $t0, 2").line("ori $t0, $t0, 1").line("mtc2 $t0, $0").line("li $t0, 3").line("mtc2 $t0, $1")
        .line("mvmva 1, 0, 0, 0, 0").line("mfc2 $s0, $9").line("mfc2 $s1, $10").line("mfc2 $s2, $11").line("mfc2 $s3, $25")
        // GPF with sf = 1: MAC = IR0 * IR >> 12, and the colour FIFO takes MAC >> 4 with the code of RGBC
        .line("li $t0, 0x1000").line("mtc2 $t0, $8").line("li $t0, 0x10").line("mtc2 $t0, $9").line("li $t0, 0x20").line("mtc2 $t0, $10")
        .line("li $t0, 0x30").line("mtc2 $t0, $11").line("lui $t0, 0x2C00").line("mtc2 $t0, $6").line("gpf 1, 0").line("mfc2 $s4, $22").line("mfc2 $s5, $25")
        .line("jr $ra").line("nop");
    const m::RunResult r = run_program(p, bus);
    CB_CHECK_EQ(r.exit(), std::string("return"));
    CHECK_U(r.r[16], 11);
    CHECK_U(r.r[17], 22);
    CHECK_U(r.r[18], 33);
    CHECK_U(r.r[19], 11);   // MAC1
    CHECK_U(r.r[20], 0x2C030201);
    CHECK_U(r.r[21], 0x10);
});
