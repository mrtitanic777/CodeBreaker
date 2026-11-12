// Tests of the PowerPC call analysis on listings written by hand: operand reading, what an instruction writes, the
// arity of a call site and the consensus of several, the clobber fixed point over the call graph, variadic functions.
#include "cb/analyze/ppc_calls.hpp"

#include "cb/core/testing.hpp"

namespace {

using namespace cb::analyze::ppc_calls;

// A body from lines "mnemonic operands", addresses counting up from `start` in fours.
Body body(std::uint64_t start, std::initializer_list<const char*> lines) {
    Body out;
    std::uint64_t at = start;
    for (const char* line : lines) {
        std::string_view l = line;
        const std::size_t space = l.find(' ');
        const std::string mnemonic(l.substr(0, space));
        out.push_back({at, mnemonic, split_operands(space == std::string_view::npos ? "" : l.substr(space + 1))});
        at += 4;
    }
    return out;
}

}  // namespace

CB_TEST("analyze.ppc_calls.operands_split_at_commas_and_are_trimmed", [] {
    CB_CHECK((split_operands("r3, r4 ,  0x10(r1)") == std::vector<std::string>{"r3", "r4", "0x10(r1)"}));
    CB_CHECK(split_operands("").empty());
    CB_CHECK(split_operands("   ").empty());
    CB_CHECK((split_operands("0x82001000") == std::vector<std::string>{"0x82001000"}));
});

CB_TEST("analyze.ppc_calls.register_tokens", [] {
    CB_CHECK(register_number("r3") == 3);
    CB_CHECK(register_number("r31") == 31);
    CB_CHECK(!register_number("r"));
    CB_CHECK(!register_number("x3"));
    CB_CHECK(!register_number("r3a"));
    CB_CHECK(!register_number("f3"));
    CB_CHECK(!register_number("-0x10(r1)"));
    CB_CHECK(register_number("f13", true) == 13);
    CB_CHECK(register_number("fr7", true) == 7);
    CB_CHECK(!register_number("r7", true));
    CB_CHECK(!register_number("", true));
});

CB_TEST("analyze.ppc_calls.memory_operands", [] {
    const auto a = memory_operand("-0x10(r1)");
    CB_CHECK(a && a->displacement == -16 && a->base == 1);
    const auto b = memory_operand("0x8(r31)");
    CB_CHECK(b && b->displacement == 8 && b->base == 31);
    const auto c = memory_operand("0(r3)");
    CB_CHECK(c && c->displacement == 0 && c->base == 3);
    CB_CHECK(!memory_operand("r3"));
    CB_CHECK(!memory_operand("0x8(x1)"));
    CB_CHECK(!memory_operand("zz(r1)"));
    CB_CHECK(!memory_operand("0x8(r1"));
    CB_CHECK(!memory_operand("(r1)"));
});

CB_TEST("analyze.ppc_calls.a_branch_target_is_an_address_written_in_hex", [] {
    CB_CHECK(branch_target({"0x82001234"}) == std::uint64_t{0x82001234});
    CB_CHECK(branch_target({"0X82001234"}) == std::uint64_t{0x82001234});
    CB_CHECK(!branch_target({"r3"}));
    CB_CHECK(!branch_target({}));
    CB_CHECK(!branch_target({"0x"}));
});

CB_TEST("analyze.ppc_calls.what_an_instruction_writes", [] {
    auto gw = [](const char* m, const char* ops = "") { return gpr_writes(m, split_operands(ops)); };
    CB_CHECK(gw("li", "r3, 5") == std::set<int>{3});
    CB_CHECK(gw("addi.", "r4, r4, 1") == std::set<int>{4});             // (record form)
    CB_CHECK(gw("lwz", "r5, 0x10(r1)") == std::set<int>{5});
    CB_CHECK(gw("stw", "r5, 0x10(r1)").empty());
    CB_CHECK(gw("stwu", "r1, -0x40(r1)") == std::set<int>{1});          // the update form changes its base
    CB_CHECK(gw("lwzu", "r6, 4(r7)") == (std::set<int>{6, 7}));
    CB_CHECK(gw("lmw", "r28, 0x10(r1)") == (std::set<int>{28, 29, 30, 31}));
    CB_CHECK(gw("mtlr", "r12").empty());
    CB_CHECK(gw("cmpwi", "r3, 0").empty());
    CB_CHECK(gw("bl", "0x82001000").empty());
    CB_CHECK(gw("blr").empty());
    CB_CHECK(gw("nop").empty());
    CB_CHECK(gw("mflr", "r12") == std::set<int>{12});
    CB_CHECK(gw("stfsu", "f1, 4(r9)") == std::set<int>{9});
    auto fw = [](const char* m, const char* ops = "") { return fpr_writes(m, split_operands(ops)); };
    CB_CHECK(fw("lfs", "f1, 0x10(r3)") == std::set<int>{1});
    CB_CHECK(fw("fadds", "f2, f0, f1") == std::set<int>{2});
    CB_CHECK(fw("stfs", "f1, 0x10(r3)").empty());
    CB_CHECK(fw("fcmpu", "cr6, f1, f2").empty());
    CB_CHECK(fw("bl", "0x1000").empty());
    CB_CHECK(fw("li", "r3, 1").empty());
});

CB_TEST("analyze.ppc_calls.arity_of_a_call_site_counts_argument_registers_set_without_a_gap", [] {
    Body two = body(0x1000, {"li r3, 1", "li r4, 2", "bl 0x2000"});
    CB_CHECK_EQ(site_arity(two, 2), 2);
    Body gap = body(0x1000, {"li r3, 1", "li r5, 2", "bl 0x2000"});          // r4 not set: only r3 counts
    CB_CHECK_EQ(site_arity(gap, 2), 1);
    Body none = body(0x1000, {"nop", "bl 0x2000"});
    CB_CHECK_EQ(site_arity(none, 1), 0);
    Body stops_at_a_call = body(0x1000, {"li r3, 1", "bl 0x3000", "li r4, 2", "bl 0x2000"});   // r3 was set before the earlier call
    CB_CHECK_EQ(site_arity(stops_at_a_call, 3), 0);
    Body out_of_order = body(0x1000, {"mr r4, r30", "mr r3, r31", "lwz r5, 4(r1)", "bl 0x2000"});
    CB_CHECK_EQ(site_arity(out_of_order, 3), 3);
    Body wide = body(0x1000, {"li r3,0", "li r4,0", "li r5,0", "li r6,0", "li r7,0", "li r8,0", "li r9,0", "li r10,0", "bl 0x2000"});
    CB_CHECK_EQ(site_arity(wide, 8), 8);
    Body stores_do_not_set = body(0x1000, {"stw r3, 0(r1)", "bl 0x2000"});
    CB_CHECK_EQ(site_arity(stores_do_not_set, 1), 0);
    // only the 13 instructions before the call are looked at
    Body far = body(0x1000, {"li r3, 1", "nop", "nop", "nop", "nop", "nop", "nop", "nop", "nop", "nop", "nop", "nop", "nop", "nop", "bl 0x2000"});
    CB_CHECK_EQ(site_arity(far, 14), 0);
    Body near = body(0x1000, {"li r3, 1", "nop", "nop", "nop", "nop", "nop", "nop", "nop", "nop", "nop", "nop", "nop", "nop", "bl 0x2000"});
    CB_CHECK_EQ(site_arity(near, 13), 1);
});

CB_TEST("analyze.ppc_calls.consensus_of_call_sites", [] {
    auto c = [](std::vector<int> v, bool stub) { return consensus(v, stub); };
    CB_CHECK(!c({2, 2}, false));                       // too few sites
    CB_CHECK(c({2, 2, 2}, false) == 2);
    CB_CHECK(c({2, 2, 1}, false) == 2);                // 67 % agree
    CB_CHECK(!c({2, 1, 3}, false));                    // no agreement
    CB_CHECK(!c({0, 0, 0}, false));                    // 0 is a pass-through as often as no arguments
    CB_CHECK(c({0, 0, 0}, true) == 0);                 // ... but an import has no body to pass through
    CB_CHECK(!c({8, 8, 8, 2, 8, 8, 8, 2}, false));     // 75 %: not enough for 8
    CB_CHECK(c({8, 8, 8, 8, 8, 8, 2}, false) == 8);    // 86 %
    CB_CHECK(!c({2, 2, 2, 1}, true));                  // an import needs 85 %
    CB_CHECK(c({2, 2, 2, 2, 2, 2, 2, 1}, true) == 2);
    CB_CHECK(!c({9, 9, 9}, false));                    // out of the range
    CB_CHECK(c({1, 2, 2, 1, 1}, false) == 1);          // (60 %; the tie-breaker is not needed)
    CB_CHECK(!c({1, 1, 2, 2, 3, 3}, false));           // a tie of a third each: under 60 %
});

namespace {

// 0x1000 f_main: calls leaf (0x2000) with two arguments three times, an import stub (0x3000) with one, a pointer once.
Module sample() {
    Module m;
    m.functions[0x1000] = body(0x1000, {"mflr r12", "stw r12, -8(r1)", "stwu r1, -0x40(r1)",
                                        "li r3, 1", "li r4, 2", "bl 0x2000",
                                        "li r3, 1", "li r4, 2", "bl 0x2000",
                                        "li r3, 5", "li r4, 6", "bl 0x2000",
                                        "li r3, 0", "bl 0x3000", "li r3, 0", "bl 0x3000", "li r3, 0", "bl 0x3000",
                                        "addi r1, r1, 0x40", "blr"});
    // the leaf: changes r3 and r4 only; reloads r30 from where it saved it
    m.functions[0x2000] = body(0x2000, {"stw r30, -0x10(r1)", "mr r30, r3", "add r3, r30, r4", "lwz r30, -0x10(r1)", "blr"});
    // a function calling the leaf, and one calling through a pointer
    m.functions[0x2100] = body(0x2100, {"li r11, 1", "bl 0x2000", "blr"});
    m.functions[0x2200] = body(0x2200, {"mtctr r12", "bctrl", "blr"});
    m.functions[0x2300] = body(0x2300, {"li r0, 1", "blr"});
    // a function homing a variable argument list after two fixed arguments
    m.functions[0x2400] = body(0x2400, {"std r5, 0x10(r1)", "std r6, 0x18(r1)", "std r7, 0x20(r1)", "stw r8, 0x28(r1)", "stw r9, 0x2c(r1)", "stw r10, 0x30(r1)", "stwu r1, -0x60(r1)", "blr"});
    m.functions[0x2500] = body(0x2500, {"stw r8, 0x28(r1)", "stw r9, 0x2c(r1)", "stw r10, 0x30(r1)", "stwu r1, -0x60(r1)", "blr"});   // all three: 5 fixed (r3.. up to r8)
    m.functions[0x2600] = body(0x2600, {"stw r8, 0x28(r1)", "stw r9, 0x2c(r1)", "stwu r1, -0x60(r1)", "stw r10, 0x30(r1)", "blr"});    // r10 is after the stwu: not homed
    m.import_names[0x3000] = "XamGetSystemVersion";
    m.import_names[0x3010] = "DbgPrint";
    m.functions[0x2700] = body(0x2700, {"li r3,0", "bl 0x3010", "li r3,0", "bl 0x3010", "li r3,0", "bl 0x3010", "blr"});
    m.functions[0x2800] = body(0x2800, {"li r30, 1", "bl 0x5000", "blr"});          // a call out of the known functions
    m.transparent = {0x4000};                                                         // (a save/restore run: not a call out)
    m.functions[0x2900] = body(0x2900, {"li r30, 1", "bl 0x4000", "blr"});
    return m;
}

}  // namespace

CB_TEST("analyze.ppc_calls.arity_of_internal_functions_and_imports", [] {
    const Result r = analyse(sample());
    CB_CHECK_EQ(r.functions, std::size_t{11});
    CB_CHECK_EQ(r.arity.at(0x2000), 2);               // two of three sites set r3 and r4 ... all three: 2, 2, 2 -> 2
    CB_CHECK(!r.arity.contains(0x2100));              // never called
    CB_CHECK_EQ(r.imports.at("XamGetSystemVersion"), 1);
    CB_CHECK(!r.imports.contains("DbgPrint"));        // the printf family is left out
});

CB_TEST("analyze.ppc_calls.clobber_is_what_a_function_provably_keeps", [] {
    const Result r = analyse(sample());
    // the leaf writes r3 (always changed, not tracked), r4 (not written: `add r3, r30, r4` reads it) and r30 (not volatile)
    CB_CHECK_EQ(r.clobber.at(0x2000), 0u);            // keeps every volatile register
    CB_CHECK_EQ(r.clobber.at(0x2100), 1u << 11);      // writes r11, and the leaf changes nothing more
    CB_CHECK(!r.clobber.contains(0x2200));            // a call through a pointer may change anything
    CB_CHECK(!r.clobber.contains(0x1000));            // it calls an import
    CB_CHECK_EQ(r.clobber.at(0x2300), 1u << 0);
    CB_CHECK(!r.clobber.contains(0x2800));            // a call out of the known functions
    CB_CHECK_EQ(r.clobber.at(0x2900), 0u);            // ... but a register save run is no call out
});

CB_TEST("analyze.ppc_calls.a_reload_of_a_saved_register_is_not_a_change", [] {
    Module m;
    // r11 stored at -8(r1) and reloaded from it: not a change; r12 loaded from elsewhere: a change
    m.functions[0x1000] = body(0x1000, {"stw r11, -8(r1)", "li r11, 7", "lwz r11, -8(r1)", "lwz r12, -0x20(r1)", "blr"});
    const Result r = analyse(m);
    CB_CHECK_EQ(r.clobber.at(0x1000), (1u << 11) | (1u << 12));   // r11 written by li, r12 by the load; the reload is skipped but li counted
    Module n;
    n.functions[0x1000] = body(0x1000, {"stw r11, -8(r1)", "lwz r11, -8(r1)", "blr"});
    CB_CHECK_EQ(analyse(n).clobber.at(0x1000), 0u);
});

CB_TEST("analyze.ppc_calls.the_fixed_point_follows_the_call_graph", [] {
    Module m;
    m.functions[0x1000] = body(0x1000, {"bl 0x2000", "blr"});          // calls 0x2000
    m.functions[0x2000] = body(0x2000, {"bl 0x3000", "li r5, 1", "blr"});
    m.functions[0x3000] = body(0x3000, {"li r6, 1", "bl 0x2000", "blr"});   // recursion: 0x2000 <-> 0x3000
    const Result r = analyse(m);
    const std::uint32_t both = (1u << 5) | (1u << 6);
    CB_CHECK_EQ(r.clobber.at(0x2000), both);
    CB_CHECK_EQ(r.clobber.at(0x3000), both);
    CB_CHECK_EQ(r.clobber.at(0x1000), both);
});

CB_TEST("analyze.ppc_calls.floating_point_registers_are_tracked_apart", [] {
    Module m;
    m.functions[0x1000] = body(0x1000, {"lfs f1, 0(r3)", "fadds f20, f1, f1", "blr"});   // f20 is not volatile
    m.functions[0x2000] = body(0x2000, {"bl 0x1000", "fmr f0, f1", "blr"});
    const Result r = analyse(m);
    CB_CHECK_EQ(r.fpclobber.at(0x1000), 1u << 1);
    CB_CHECK_EQ(r.fpclobber.at(0x2000), (1u << 1) | (1u << 0));
    CB_CHECK_EQ(r.clobber.at(0x1000), 0u);
});

CB_TEST("analyze.ppc_calls.variadic_functions_home_r8_to_r10_before_their_stwu", [] {
    const Result r = analyse(sample());
    CB_CHECK_EQ(r.variadic.at(0x2400), 2);            // r5 is the lowest homed: two fixed arguments (r3, r4)
    CB_CHECK_EQ(r.variadic.at(0x2500), 5);            // r8: five fixed
    CB_CHECK(!r.variadic.contains(0x2600));
    CB_CHECK(!r.variadic.contains(0x1000));
});

CB_TEST("analyze.ppc_calls.the_summary_and_the_tsv", [] {
    const Result r = analyse(sample());
    CB_CHECK(summary(r).starts_with("11 functions: arity for 1, import arity for 1, 2 variadic, "));
    const std::string tsv = to_tsv(r);
    CB_CHECK(tsv.starts_with("kind\taddress or name\tvalue\n"));
    CB_CHECK(tsv.find("arity\t00002000\t2\n") != std::string::npos);
    CB_CHECK(tsv.find("import arity\tXamGetSystemVersion\t1\n") != std::string::npos);
    CB_CHECK(tsv.find("variadic\t00002400\t2\n") != std::string::npos);
    CB_CHECK(tsv.find("clobber\t00002000\t-\n") != std::string::npos);
    CB_CHECK(tsv.find("clobber\t00002100\tr11\n") != std::string::npos);
    CB_CHECK(tsv.find("clobber\t00002300\tr0\n") != std::string::npos);
    // the kinds come in order, each sorted by address
    CB_CHECK(tsv.find("arity\t") < tsv.find("import arity\t"));
    CB_CHECK(tsv.find("import arity\t") < tsv.find("variadic\t"));
    CB_CHECK(tsv.find("variadic\t") < tsv.find("\nclobber\t"));
});

CB_TEST("analyze.ppc_calls.an_empty_module_has_nothing_to_say", [] {
    const Result r = analyse(Module{});
    CB_CHECK_EQ(r.functions, std::size_t{0});
    CB_CHECK(r.arity.empty() && r.imports.empty() && r.clobber.empty() && r.fpclobber.empty() && r.variadic.empty());
    CB_CHECK_EQ(to_tsv(r), std::string("kind\taddress or name\tvalue\n"));
});
