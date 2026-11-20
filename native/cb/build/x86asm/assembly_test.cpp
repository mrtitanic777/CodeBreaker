// Tests of `Assembly`, the run state of x86asm: NASM's error and warning handling (error.c) with its exact message text,
// the pass logic, the location and offsets, the CPU level, the limits, and the source position stack.
#include "cb/build/x86asm/assembly.hpp"

#include <string>
#include <vector>

#include "cb/core/testing.hpp"

namespace {
using namespace cb::build;
using namespace cb::build::x86asm;

// An assembly that is reading line `line` of "f.asm".
void at(Assembly& a, std::int32_t line = 1, const char* file = "f.asm") { a.src.set(line, file); }
}  // namespace

CB_TEST("x86asm.assembly.error_text", [] {
    Assembly a;
    at(a, 12, "foo.asm");
    a.nonfatal("bad {}", 3);
    CB_CHECK_EQ(a.diagnostics().size(), std::size_t{1});
    CB_CHECK_EQ(a.diagnostics()[0].text, "foo.asm:12: error: bad 3");
    CB_CHECK_EQ(a.error_output(), "foo.asm:12: error: bad 3\n");
    CB_CHECK_EQ(a.erropt.worst, ERR_NONFATAL);
    CB_CHECK(a.terminate_after_phase());
    CB_CHECK((a.erropt.never & ERR_UNDEAD) != 0);

    // A literal brace is doubled in a format.
    Assembly b;
    at(b);
    b.nonfatal("bad {{}}");
    CB_CHECK_EQ(b.diagnostics()[0].text, "f.asm:1: error: bad {}");

    // The MSVC format.
    Assembly c;
    at(c, 12, "foo.asm");
    CB_CHECK_EQ(c.set_error_format("vc"), 0);
    c.nonfatal("bad");
    CB_CHECK_EQ(c.diagnostics()[0].text, "foo.asm(12) : error: bad");
    CB_CHECK_EQ(c.set_error_format("gcc"), 0);
    CB_CHECK_EQ(c.set_error_format("nonsense"), -1);

    // No file: the input name, else the output name, else "nasm".
    Assembly d;
    d.nonfatal("x");
    CB_CHECK_EQ(d.diagnostics()[0].text, "nasm: error: x");
    Assembly e;
    e.inname = "in.asm";
    e.nonfatal("x");
    CB_CHECK_EQ(e.diagnostics()[0].text, "in.asm: error: x");
    Assembly f;
    f.outname = "out.bin";
    f.nonfatal("x");
    CB_CHECK_EQ(f.diagnostics()[0].text, "out.bin: error: x");

    // ERR_NOFILE and ERR_HERE.
    Assembly g;
    at(g, 4);
    g.nonfatalf(ERR_NOFILE, "x");
    CB_CHECK_EQ(g.diagnostics()[0].text, "nasm: error: x");
    g.nonfatalf(ERR_HERE, "dup");
    CB_CHECK_EQ(g.diagnostics()[1].text, "f.asm:4: error: dup here");

    // Two assemblies do not touch each other.
    CB_CHECK(a.diagnostics().size() == 1 && c.diagnostics().size() == 1 && b.diagnostics().size() == 1);
});

CB_TEST("x86asm.assembly.severity_prefixes", [] {
    CB_CHECK_EQ(std::string(error_prefix(ERR_LISTMSG)), ";;; ");
    CB_CHECK_EQ(std::string(error_prefix(ERR_NOTE)), "note: ");
    CB_CHECK_EQ(std::string(error_prefix(ERR_DEBUG)), "debug: ");
    CB_CHECK_EQ(std::string(error_prefix(ERR_INFO)), "info: ");
    CB_CHECK_EQ(std::string(error_prefix(ERR_WARNING)), "warning: ");
    CB_CHECK_EQ(std::string(error_prefix(ERR_NONFATAL)), "error: ");
    CB_CHECK_EQ(std::string(error_prefix(ERR_FATAL)), "fatal: ");
    CB_CHECK_EQ(std::string(error_prefix(ERR_CRITICAL)), "critical: ");
    CB_CHECK_EQ(std::string(error_prefix(ERR_PANIC)), "panic: ");
    CB_CHECK_EQ(std::string(error_prefix(0x7)), "internal error: ");
    CB_CHECK_EQ(warn_idx(WARN_DB_EMPTY), 1u);
    CB_CHECK_EQ(WARN_OTHER, 68u << 16);
    CB_CHECK_EQ(warning(3), 3u << 16);
});

CB_TEST("x86asm.assembly.warnings", [] {
    Assembly a;
    at(a, 12, "foo.asm");
    // The defaults come from warnings.dat: db-empty is on, float-denorm off, directive-garbage-eol an error.
    CB_CHECK(a.warn_active(WARN_DB_EMPTY));
    CB_CHECK(!a.warn_active(WARN_FLOAT_DENORM));
    CB_CHECK(a.warning_state == kWarningDefault);

    a.warn(WARN_DB_EMPTY, "no operand");
    CB_CHECK_EQ(a.diagnostics().size(), std::size_t{1});
    CB_CHECK_EQ(a.diagnostics()[0].text, "foo.asm:12: warning: no operand [-w+db-empty]");
    CB_CHECK_EQ(a.erropt.worst, ERR_WARNING);
    CB_CHECK(!a.terminate_after_phase());

    a.warn(WARN_FLOAT_DENORM, "denormal");   // off: nothing
    CB_CHECK_EQ(a.diagnostics().size(), std::size_t{1});

    CB_CHECK(a.set_warning_status("+float-denorm"));
    a.warn(WARN_FLOAT_DENORM, "denormal");
    CB_CHECK_EQ(a.diagnostics().size(), std::size_t{2});
    CB_CHECK(a.set_warning_status("-float-denorm"));
    CB_CHECK(!a.warn_active(WARN_FLOAT_DENORM));
    CB_CHECK(a.set_warning_status("no-db-empty"));
    CB_CHECK(!a.warn_active(WARN_DB_EMPTY));

    // A warning treated as an error.
    Assembly b;
    at(b, 12, "foo.asm");
    CB_CHECK(b.set_warning_status("error=db-empty"));
    b.warn(WARN_DB_EMPTY, "no operand");
    CB_CHECK_EQ(b.diagnostics()[0].text, "foo.asm:12: error: no operand [-w+error=db-empty]");
    CB_CHECK_EQ(b.diagnostics()[0].severity & ERR_MASK, ERR_WARNING);
    CB_CHECK_EQ(b.diagnostics()[0].true_type, ERR_NONFATAL);
    CB_CHECK(b.terminate_after_phase());

    // Prefixes of a name stand for its family; "all" and "none" for everything.
    Assembly c;
    c.error_init();   // the state "*name" goes back to
    CB_CHECK(c.set_warning_status("-label"));
    CB_CHECK(!c.warn_active(WARN_LABEL_ORPHAN));
    CB_CHECK(!c.warn_active(WARN_LABEL_REDEF_LATE));
    CB_CHECK(c.warn_active(WARN_DB_EMPTY));
    CB_CHECK(c.set_warning_status("none"));
    CB_CHECK(!c.warn_active(WARN_DB_EMPTY));
    CB_CHECK(c.set_warning_status("all"));
    CB_CHECK(c.warn_active(WARN_DB_EMPTY));
    CB_CHECK(c.warn_active(WARN_LABEL_ORPHAN));
    CB_CHECK(c.set_warning_status("orphan-labels"));   // an alias
    CB_CHECK(!c.set_warning_status("no-such-class"));   // unknown
    CB_CHECK(c.set_warning_status("   -db-empty"));   // leading spaces
    CB_CHECK(!c.warn_active(WARN_DB_EMPTY));
    CB_CHECK(c.set_warning_status("*db-empty"));   // reset to the state at error_init
    CB_CHECK(c.warn_active(WARN_DB_EMPTY));
});

CB_TEST("x86asm.assembly.warning_stack", [] {
    Assembly a;
    a.error_init();
    CB_CHECK(a.warn_active(WARN_DB_EMPTY));
    a.push_warnings();
    CB_CHECK(a.set_warning_status("-db-empty"));
    CB_CHECK(!a.warn_active(WARN_DB_EMPTY));
    a.push_warnings();
    CB_CHECK(a.set_warning_status("+float-denorm"));
    a.pop_warnings();
    CB_CHECK(!a.warn_active(WARN_DB_EMPTY));
    CB_CHECK(!a.warn_active(WARN_FLOAT_DENORM));
    a.pop_warnings();
    CB_CHECK(a.warn_active(WARN_DB_EMPTY));

    // The end of a pass unwinds the stack to its first entry.
    a.push_warnings();
    a.push_warnings();
    CB_CHECK(a.set_warning_status("-db-empty"));
    a.error_pass_end();
    CB_CHECK(a.warn_active(WARN_DB_EMPTY));
});

CB_TEST("x86asm.assembly.buffered_warnings", [] {
    // On a pass before the last, warnings are held until an error shows up; at the end of the pass they are dropped.
    Assembly a;
    at(a);
    a.begin_pass();
    a.warn(WARN_DB_EMPTY, "w1");
    CB_CHECK(a.diagnostics().empty());
    a.nonfatal("e1");
    CB_CHECK_EQ(a.diagnostics().size(), std::size_t{2});
    CB_CHECK_EQ(a.diagnostics()[0].text, "f.asm:1: warning: w1 [-w+db-empty]");
    CB_CHECK_EQ(a.diagnostics()[1].text, "f.asm:1: error: e1");

    Assembly b;
    at(b);
    b.begin_pass();
    b.warn(WARN_DB_EMPTY, "w1");
    b.error_pass_end();
    CB_CHECK(b.diagnostics().empty());

    // ERR_PASS2 messages wait for the final pass.
    Assembly c;
    at(c);
    c.begin_pass();
    c.warn(WARN_DB_EMPTY | ERR_PASS2, "late");
    c.error_pass_end();
    c.begin_pass();   // optimize
    c.error_pass_end();
    c.begin_pass();   // stabilize
    c.error_pass_end();
    c.begin_pass();   // final
    CB_CHECK(c.pass_final());
    c.warn(WARN_DB_EMPTY | ERR_PASS2, "late");
    CB_CHECK_EQ(c.diagnostics().size(), std::size_t{1});   // not buffered in the last pass
    CB_CHECK_EQ(c.diagnostics()[0].text, "f.asm:1: warning: late [-w+db-empty]");
});

CB_TEST("x86asm.assembly.fatal_errors_throw", [] {
    Assembly a;
    at(a);
    std::vector<std::string> events;
    a.hooks.final_report = [&](bool failure) { events.push_back(failure ? "final_report" : "?"); };
    a.hooks.close_output = [&](bool failure) { events.push_back(failure ? "close_output" : "?"); };
    CB_CHECK_THROWS_AS(AsmError, a.fatal("boom {}", 1));
    CB_CHECK_EQ(a.exit_code(), 1);
    CB_CHECK_EQ(a.diagnostics().back().text, "f.asm:1: fatal: boom 1");
    CB_CHECK_EQ(events.size(), std::size_t{2});
    CB_CHECK_EQ(events[0], "final_report");
    CB_CHECK_EQ(events[1], "close_output");

    Assembly b;
    at(b);
    CB_CHECK_THROWS_AS(AsmError, b.critical("c"));
    CB_CHECK_EQ(b.exit_code(), 3);
    CB_CHECK_EQ(b.diagnostics().back().text, "critical: f.asm:1: c");   // critical errors write the severity first

    Assembly c;
    at(c);
    CB_CHECK_THROWS_AS(AsmError, c.panic("p"));
    CB_CHECK_EQ(c.exit_code(), 4);
    CB_CHECK_EQ(c.diagnostics().back().text, "panic: f.asm:1: p");

    Assembly d;
    at(d);
    CB_CHECK_THROWS_AS(AsmError, d.panic_here());
    CB_CHECK(d.error_output().find("internal error in ") != std::string::npos);
    Assembly e;
    CB_CHECK_THROWS_AS(AsmError, e.assert_failed("x == y"));
    CB_CHECK(e.error_output().find("assertion x == y failed in ") != std::string::npos);

    // A usage message follows an error flagged for it.
    Assembly f;
    CB_CHECK_THROWS_AS(AsmError, f.fatalf(ERR_USAGE, "bad option"));
    CB_CHECK(f.error_output().find("Usage: nasm [-@ response_file] [options...] [--] filename\n") != std::string::npos);

    // A warning promoted to an error does not end the run; a FATAL one is final even for a warning class.
    Assembly g;
    CB_CHECK(g.set_warning_status("error=db-empty"));
    CB_CHECK_NOTHROW(g.warn(WARN_DB_EMPTY, "x"));
});

CB_TEST("x86asm.assembly.suppression_and_hooks", [] {
    Assembly a;
    at(a);
    // After an error, ERR_UNDEAD messages are dropped.
    a.nonfatal("first");
    a.nonfatalf(ERR_UNDEAD, "second");
    CB_CHECK_EQ(a.diagnostics().size(), std::size_t{1});

    // The preprocessor can suppress, and is asked to list the macro stack after a printed message.
    Assembly b;
    at(b);
    int listed = 0;
    b.hooks.pp_error_list_macros = [&](ErrFlags) { ++listed; };
    b.nonfatal("x");
    CB_CHECK_EQ(listed, 1);
    b.hooks.pp_suppress_error = [](ErrFlags) { return true; };
    b.nonfatal("y");
    CB_CHECK_EQ(b.diagnostics().size(), std::size_t{1});

    // The listing's copy of a message, with and without a place.
    Assembly c;
    at(c, 4);
    std::vector<std::string> listing;
    c.hooks.list_error = [&](ErrFlags, const std::string& text) { listing.push_back(text); };
    c.nonfatal("plain");
    c.nonfatalf(ERR_HERE, "dup");
    c.listmsg("remark");
    CB_CHECK_EQ(listing.size(), std::size_t{3});
    CB_CHECK_EQ(listing[0], "error: plain");
    CB_CHECK_EQ(listing[1], "error: dup at f.asm:4");
    CB_CHECK_EQ(listing[2], ";;; remark");
    CB_CHECK_EQ(c.diagnostics().size(), std::size_t{2});   // the list-only message is not printed

    // Info and debug messages by level.
    Assembly d;
    at(d);
    d.erropt.verbose_info = 1;
    d.info(1, "hello");
    d.info(2, "quiet");
    d.debug(1, "no debug");
    CB_CHECK_EQ(d.diagnostics().size(), std::size_t{1});
    CB_CHECK_EQ(d.diagnostics()[0].text, "f.asm:1: info: hello [--info=1]");
});

CB_TEST("x86asm.assembly.held_errors", [] {
    Assembly a;
    at(a);
    ErrHold h = a.error_hold_push();
    a.nonfatal("held");
    CB_CHECK(a.diagnostics().empty());
    CB_CHECK_EQ(a.error_hold_pop(h, false), ERR_NONFATAL);   // dropped, and its severity reported
    CB_CHECK(a.diagnostics().empty());
    CB_CHECK(!a.terminate_after_phase());

    h = a.error_hold_push();
    a.nonfatal("kept");
    CB_CHECK_EQ(a.error_hold_pop(h, true), ERR_NONFATAL);
    CB_CHECK_EQ(a.diagnostics().size(), std::size_t{1});
    CB_CHECK_EQ(a.diagnostics()[0].text, "f.asm:1: error: kept");

    // A nested hold hands its messages to the one around it.
    Assembly b;
    at(b);
    const ErrHold outer = b.error_hold_push();
    const ErrHold inner = b.error_hold_push();
    b.nonfatal("inner");
    CB_CHECK_EQ(b.error_hold_pop(inner, true), 0u);
    CB_CHECK(b.diagnostics().empty());
    CB_CHECK_EQ(b.error_hold_pop(outer, true), ERR_NONFATAL);
    CB_CHECK_EQ(b.diagnostics().size(), std::size_t{1});
    CB_CHECK_EQ(b.error_hold_pop(0, true), 0u);   // no hold: nothing

    // A hold must be popped in order.
    Assembly c;
    const ErrHold first = c.error_hold_push();
    c.error_hold_push();
    CB_CHECK_THROWS_AS(AsmError, c.error_hold_pop(first, false));
});

CB_TEST("x86asm.assembly.passes", [] {
    Assembly a;
    CB_CHECK(a.not_started());
    CB_CHECK_EQ(a.pass_count(), std::int64_t{0});
    CB_CHECK_EQ(std::string(a.pass_type_name()), "init");

    a.begin_pass();
    CB_CHECK_EQ(static_cast<int>(a.pass_type()), static_cast<int>(PASS_FIRST));
    CB_CHECK_EQ(a.pass_count(), std::int64_t{1});
    CB_CHECK(a.pass_first());
    CB_CHECK(!a.pass_stable());
    CB_CHECK(a.location.known);
    CB_CHECK_EQ(a.location.segment, NO_SEG);
    CB_CHECK((a.erropt.never & ERR_PASS2) != 0);
    a.error_pass_end();

    a.begin_pass();
    CB_CHECK_EQ(static_cast<int>(a.pass_type()), static_cast<int>(PASS_OPT));
    CB_CHECK_EQ(std::string(a.pass_type_name()), "optimize");
    CB_CHECK(!a.pass_first());
    a.error_pass_end();

    // The optimization pass repeats while a label moved.
    a.global_offset_changed = 1;
    a.begin_pass();
    CB_CHECK_EQ(static_cast<int>(a.pass_type()), static_cast<int>(PASS_OPT));
    CB_CHECK_EQ(a.global_offset_changed, std::uint64_t{0});
    CB_CHECK_EQ(a.pass_count(), std::int64_t{3});
    a.error_pass_end();

    a.begin_pass();
    CB_CHECK_EQ(static_cast<int>(a.pass_type()), static_cast<int>(PASS_STAB));
    CB_CHECK(a.pass_stable());
    CB_CHECK(!a.pass_final());
    a.error_pass_end();

    a.begin_pass();
    CB_CHECK_EQ(static_cast<int>(a.pass_type()), static_cast<int>(PASS_FINAL));
    CB_CHECK(a.pass_final());
    CB_CHECK(a.pass_final_or_preproc());
    CB_CHECK_EQ(a.erropt.never & ERR_PASS2, 0u);
    CB_CHECK_EQ(std::string(a.pass_type_name()), "final");

    a.erropt.verbose_info = 1;
    a.print_final_report(false);
    CB_CHECK_EQ(a.diagnostics().back().text, "nasm: info: assembly completed after 1+2+2 passes [--info=1]");

    Assembly p;
    p.begin_preproc_pass();
    CB_CHECK_EQ(static_cast<int>(p.pass_type()), static_cast<int>(PASS_PREPROC));
    CB_CHECK(p.pass_final_or_preproc());
    CB_CHECK(!p.location.known);
});

CB_TEST("x86asm.assembly.location_and_offsets", [] {
    Assembly a;
    a.begin_pass();
    CB_CHECK_EQ(a.switch_segment(1), std::int64_t{0});
    CB_CHECK_EQ(a.location.segment, 1);
    CB_CHECK_EQ(a.increment_offset(4), std::int64_t{4});
    CB_CHECK_EQ(a.location.offset, std::int64_t{4});
    CB_CHECK_EQ(a.increment_offset(0), std::int64_t{4});
    CB_CHECK_EQ(a.switch_segment(3), std::int64_t{0});
    a.increment_offset(7);
    CB_CHECK_EQ(a.switch_segment(1), std::int64_t{4});   // each segment keeps its own offset

    // ABSOLUTE has an offset of its own.
    CB_CHECK_EQ(a.switch_segment(NO_SEG), std::int64_t{0});
    CB_CHECK(a.in_absolute);
    a.increment_offset(10);
    CB_CHECK_EQ(a.absolute.offset, std::int64_t{10});
    CB_CHECK_EQ(a.switch_segment(1), std::int64_t{4});
    CB_CHECK(!a.in_absolute);
    CB_CHECK_EQ(a.switch_segment(NO_SEG), std::int64_t{10});

    // The next pass starts with every offset at 0 and the location at the start.
    a.error_pass_end();
    a.begin_pass();
    CB_CHECK_EQ(a.switch_segment(1), std::int64_t{0});
    CB_CHECK(!a.in_absolute);

    // Per-pass defaults.
    a.cmd_sb = 32;
    a.globl.bits = 16;
    a.globl.rel = EAF_REL;
    a.error_pass_end();
    a.begin_pass();
    CB_CHECK_EQ(a.globl.bits, 32);
    CB_CHECK_EQ(a.globl.rel, 0);
    CB_CHECK_EQ(a.globl.reldef, static_cast<int>(EAF_FS | EAF_GS));
    CB_CHECK(a.globl.dollarhex);
    CB_CHECK(!a.globl.bnd);
});

CB_TEST("x86asm.assembly.cpu", [] {
    Assembly a;
    // The default level allows everything but "any".
    CB_CHECK(iflag_cpu_level_ok(a.cpu, IF_386));
    CB_CHECK(iflag_cpu_level_ok(a.cpu, IF_X86_64));
    CB_CHECK(!iflag_cpu_level_ok(a.cpu, IF_ANY));
    CB_CHECK(a.cpu == a.cmd_cpu);

    a.set_cpu("8086");
    CB_CHECK(iflag_cpu_level_ok(a.cpu, IF_8086));
    CB_CHECK(!iflag_cpu_level_ok(a.cpu, IF_186));
    a.set_cpu("386");
    CB_CHECK(iflag_cpu_level_ok(a.cpu, IF_386));
    CB_CHECK(!iflag_cpu_level_ok(a.cpu, IF_486));
    a.set_cpu("P6");   // case does not matter
    CB_CHECK(iflag_cpu_level_ok(a.cpu, IF_P6));
    a.set_cpu("pentium, x64");
    CB_CHECK(iflag_cpu_level_ok(a.cpu, IF_X86_64));
    a.set_cpu("any");
    CB_CHECK(iflag_cpu_level_ok(a.cpu, IF_ANY));
    CB_CHECK(a.cpu.test(IF_LATEVEX));

    a.set_cpu("default");
    CB_CHECK(!iflag_cpu_level_ok(a.cpu, IF_ANY));
    a.set_cpu("latevex");
    CB_CHECK(a.cpu.test(IF_LATEVEX));
    a.set_cpu("nolatevex");
    CB_CHECK(!a.cpu.test(IF_LATEVEX));
    a.set_cpu("+latevex");
    a.set_cpu("-latevex");
    CB_CHECK(!a.cpu.test(IF_LATEVEX));

    a.set_cpu("8086");
    a.set_cpu("*");   // back to the command line's
    CB_CHECK(a.cpu == a.cmd_cpu);
    a.set_cpu(std::nullopt);
    CB_CHECK(!iflag_cpu_level_ok(a.cpu, IF_ANY));

    Assembly b;
    at(b);
    b.set_cpu("bogus");
    CB_CHECK_EQ(b.diagnostics().size(), std::size_t{1});
    CB_CHECK_EQ(b.diagnostics()[0].text, "f.asm:1: error: unknown CPU type or flag 'bogus'");

    // The cpu is the command line's again at the start of each pass.
    Assembly c;
    c.set_cpu("8086");
    c.begin_pass();
    CB_CHECK(c.cpu == c.cmd_cpu);
});

CB_TEST("x86asm.assembly.bits", [] {
    Assembly a;
    at(a);
    CB_CHECK_EQ(a.get_bits("16"), 16);
    CB_CHECK_EQ(a.get_bits("32"), 32);
    CB_CHECK_EQ(a.get_bits("64"), 64);
    CB_CHECK(a.diagnostics().empty());
    CB_CHECK_EQ(a.get_bits("17"), 16);
    CB_CHECK_EQ(a.diagnostics().back().text, "f.asm:1: error: `17' is not a valid segment size; must be 16, 32 or 64");
    a.set_cpu("8086");
    CB_CHECK_EQ(a.get_bits("32"), 16);
    CB_CHECK_EQ(a.diagnostics().back().text, "f.asm:1: error: cannot specify 32-bit segment on processor below a 386");
    a.set_cpu("386");
    CB_CHECK_EQ(a.get_bits("64"), 16);
    CB_CHECK_EQ(a.diagnostics().back().text, "f.asm:1: error: cannot specify 64-bit segment on processor below an x86-64");
});

CB_TEST("x86asm.assembly.limits", [] {
    Assembly a;
    CB_CHECK_EQ(a.nasm_limit[LIMIT_EVAL], std::int64_t{8192});
    CB_CHECK_EQ(a.nasm_limit[LIMIT_REP], std::int64_t{1000000});
    CB_CHECK_EQ(a.nasm_limit[LIMIT_PARAMS], std::int64_t{16383});
    CB_CHECK_EQ(a.nasm_limit[LIMIT_PASSES], LIMIT_MAX_VAL);   // no default: the maximum
    CB_CHECK_EQ(a.nasm_limit[LIMIT_LINES], std::int64_t{2000000000});
    CB_CHECK_EQ(std::string(Assembly::limit_name(LIMIT_MACRO_LEVELS)), "macro-levels");
    CB_CHECK(Assembly::limit_name(LIMIT_MAX) == nullptr);
    CB_CHECK_EQ(Assembly::limit_str(LIMIT_MAX_VAL), "unlimited");
    CB_CHECK_EQ(Assembly::limit_str(42), "42");

    CB_CHECK_EQ(static_cast<int>(a.set_limit("rep", "50")), static_cast<int>(DIRR_OK));
    CB_CHECK_EQ(a.nasm_limit[LIMIT_REP], std::int64_t{50});
    CB_CHECK_EQ(static_cast<int>(a.set_limit("REP", "default")), static_cast<int>(DIRR_OK));
    CB_CHECK_EQ(a.nasm_limit[LIMIT_REP], std::int64_t{1000000});
    a.set_limit("rep", "unlimited");
    CB_CHECK_EQ(a.nasm_limit[LIMIT_REP], LIMIT_MAX_VAL);
    a.set_limit("rep", "0x10");
    CB_CHECK_EQ(a.nasm_limit[LIMIT_REP], std::int64_t{16});
    a.set_limit("rep", "0");   // zero means "the initial value"
    CB_CHECK_EQ(a.nasm_limit[LIMIT_REP], std::int64_t{1000000});
    a.nasm_limit[LIMIT_REP] = 7;
    a.set_limit("rep", "reset");
    CB_CHECK_EQ(a.nasm_limit[LIMIT_REP], std::int64_t{1000000});

    CB_CHECK_EQ(static_cast<int>(a.set_limit("bogus", "1")), static_cast<int>(DIRR_ERROR));
    CB_CHECK_EQ(static_cast<int>(a.set_limit("rep", "junk")), static_cast<int>(DIRR_ERROR));
    CB_CHECK_EQ(static_cast<int>(a.set_limit("rep", "-1")), static_cast<int>(DIRR_ERROR));

    // A value past the maximum is capped.
    CB_CHECK_EQ(static_cast<int>(a.set_limit("params", "99999999999")), static_cast<int>(DIRR_OK));
    CB_CHECK_EQ(a.nasm_limit[LIMIT_PARAMS], std::int64_t{1073741823});

    CB_CHECK_EQ(a.get_limit("rep", GET_LIMIT_DEFAULT), std::int64_t{1000000});
    CB_CHECK_EQ(a.get_limit("params", GET_LIMIT_MAX), std::int64_t{1073741823});
    CB_CHECK_EQ(a.get_limit("params", GET_LIMIT_CURRENT), std::int64_t{1073741823});
    CB_CHECK_EQ(a.get_limit("nope", GET_LIMIT_CURRENT), std::int64_t{0});

    // The command line's limits are what a pass starts from.
    a.set_limit("rep", "25");
    a.keep_limits_from_cmdline();
    a.set_limit("rep", "26");
    a.reset_limits_before_pass();
    CB_CHECK_EQ(a.nasm_limit[LIMIT_REP], std::int64_t{25});
    CB_CHECK_EQ(a.get_limit("rep", GET_LIMIT_INIT), std::int64_t{25});
    a.set_limit("rep", "reset");
    CB_CHECK_EQ(a.nasm_limit[LIMIT_REP], std::int64_t{25});

    // The readnum hook decides how a number is written.
    a.hooks.readnum = [](std::string_view, bool& error) {
        error = false;
        return std::int64_t{99};
    };
    a.set_limit("rep", "ninety-nine");
    CB_CHECK_EQ(a.nasm_limit[LIMIT_REP], std::int64_t{99});
});

CB_TEST("x86asm.assembly.source_position", [] {
    SourcePosition s;
    s.set(5, "a.asm");
    CB_CHECK_EQ(std::string(s.get_fname()), "a.asm");
    CB_CHECK_EQ(s.get_linnum(), 5);
    CB_CHECK_EQ(std::string(s.set_fname("b.asm")), "a.asm");   // the old name comes back
    CB_CHECK_EQ(s.set_linnum(9), 5);

    // src_get: -2 when the file changed, else the line difference.
    std::int32_t line = 0;
    std::string_view name;
    CB_CHECK_EQ(s.get(line, name), -2);
    CB_CHECK_EQ(s.get(line, name), 0);
    s.set_linnum(12);
    CB_CHECK_EQ(s.get(line, name), 3);
    s.set_fname("c.asm");
    CB_CHECK_EQ(s.get(line, name), -2);

    // Inside a macro: the top level stays what error messages use until the hierarchy is walked down.
    int macro_a = 0;
    s.set(30, "m.asm");
    s.macro_push(&macro_a, SrcLocation{"m.asm", 3});
    CB_CHECK_EQ(s.where().lineno, 3);
    CB_CHECK_EQ(s.where_top().lineno, 30);
    CB_CHECK_EQ(s.where_error().lineno, 30);
    CB_CHECK(s.macro_current() == &macro_a);
    CB_CHECK(s.error_down() == &macro_a);
    CB_CHECK_EQ(s.where_error().lineno, 3);
    CB_CHECK(s.error_down() == nullptr);   // already at the bottom
    s.error_reset();
    CB_CHECK_EQ(s.where_error().lineno, 30);
    CB_CHECK_EQ(s.update(SrcLocation{"m.asm", 4}).lineno, 3);
    s.macro_pop();
    CB_CHECK_EQ(s.where().lineno, 30);
    CB_CHECK(s.macro_current() == nullptr);
});

CB_TEST("x86asm.assembly.misc_state", [] {
    // MASM data types are remembered without regard to case; size 0 is ignored.
    Assembly a;
    a.masm_type_note("CurTDB", 4);
    CB_CHECK_EQ(a.masm_type_query("curtdb"), 4);
    CB_CHECK_EQ(a.masm_type_query("CURTDB"), 4);
    a.masm_type_note("flag", 0);
    CB_CHECK_EQ(a.masm_type_query("flag"), 0);
    CB_CHECK_EQ(a.masm_type_query("unknown"), 0);

    // The compile time (a date in 1971: 365 days after the epoch), and the reproducible-build rule.
    a.set_compile_time(std::time_t{86400} * 365);
    CB_CHECK(a.official_compile_time.have_gm);
    CB_CHECK(a.official_compile_time.have_posix);
    CB_CHECK_EQ(a.official_compile_time.gm.tm_year, 71);
    CB_CHECK_EQ(a.official_compile_time.posix, std::int64_t{86400} * 365);
    CB_CHECK_EQ(a.posix_timestamp(), std::int64_t{86400} * 365);
    a.reproducible = true;
    CB_CHECK_EQ(a.posix_timestamp(), std::int64_t{0});

    // The default optimization level, and the forward-reference marker of an instruction (none are recorded yet).
    CB_CHECK_EQ(static_cast<int>(a.optimizing), static_cast<int>(OPTIM_DEFAULT));
    Insn insn;
    insn.operands = 1;
    insn.oprs[0].opflags = OPFLAG_FORWARD;
    a.forward_refs(insn);
    CB_CHECK_EQ(insn.oprs[0].opflags, OPFLAG_FORWARD);

    a.warn_dollar_hex();
    a.warn_dollar_hex();
    a.src.set(2, "h.asm");
    a.warn_dollar_hex();
    CB_CHECK_EQ(a.diagnostics().back().text, "h.asm:2: warning: $ prefix for hexadecimal is deprecated [-w+number-deprecated-hex]");
});
