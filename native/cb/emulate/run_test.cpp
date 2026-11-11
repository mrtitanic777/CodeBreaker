// cb/emulate/run_test.cpp -- the run record: how a run ended, whether registers were kept, the summary line.
#include "cb/emulate/run.hpp"

#include "cb/core/testing.hpp"

using namespace cb::emulate;

CB_TEST("emulate.run.ended_words", [] {
    CB_CHECK_EQ(ended(std::nullopt), "none");
    CB_CHECK_EQ(ended(std::string("ret")), "ret");
    CB_CHECK_EQ(ended(std::string("exception: UC_ERR_READ_UNMAPPED")), "fault");
    CB_CHECK_EQ(ended(std::string("exception")), "fault");
    CB_CHECK_EQ(ended(std::string("truncated")), "truncated");
    CB_CHECK_EQ(ended(std::string("wild")), "wild");
    CB_CHECK_EQ(ended(std::string("int:21")), "int");               // the word before the colon
    CB_CHECK_EQ(ended(std::string("return")), "return");
    CB_CHECK_EQ(ended(std::string("")), "");
    CB_CHECK_EQ(ended(std::string(":x")), "");
    CB_CHECK_EQ(ended(std::string("retx")), "retx");                // only "ret" itself is a ret
    Run r;
    CB_CHECK_EQ(r.ended(), "none");
    r.exit = "ret";
    CB_CHECK_EQ(r.ended(), "ret");
});

CB_TEST("emulate.run.kept_compares_entry_and_end", [] {
    Run r;
    CB_CHECK(!r.kept());                                            // not read: not kept
    CB_CHECK(!r.kept(std::vector<std::string>{"EBX"}));
    r.preserved = Preserved{};
    CB_CHECK(r.kept());                                             // nothing to hand back: kept
    (*r.preserved)["EBX"] = {0x10, 0x10};
    (*r.preserved)["ESI"] = {0x20, 0x99};
    (*r.preserved)["EDI"] = {std::nullopt, std::nullopt};
    CB_CHECK(!r.kept());
    const std::vector<std::string> good = {"EBX", "EDI"};
    CB_CHECK(r.kept(good));
    const std::vector<std::string> bad = {"EBX", "ESI"};
    CB_CHECK(!r.kept(bad));
    const std::vector<std::string> unknown = {"EBP"};               // never recorded: nothing to compare
    CB_CHECK(r.kept(unknown));
    CB_CHECK(r.kept(std::span<const std::string>{}));
    (*r.preserved)["ESI"] = {0x20, std::nullopt};                   // an end that was not read differs from one that was
    CB_CHECK(!r.kept());
});

CB_TEST("emulate.run.results_keep_the_order_they_were_named_in", [] {
    Run r;
    r.results["EAX"] = std::uint64_t{7};
    r.results["ST0"] = 1.5;
    r.results["flags"] = std::monostate{};
    r.results["EAX"] = std::uint64_t{8};                            // set again: same place
    CB_CHECK_EQ(r.results.size(), 3u);
    CB_CHECK_EQ(r.results.begin()->first, "EAX");
    CB_CHECK_EQ(std::get<std::uint64_t>(r.results.at("EAX")), 8u);
    CB_CHECK_EQ(std::get<double>(r.results.at("ST0")), 1.5);
    CB_CHECK(std::holds_alternative<std::monostate>(r.results.at("flags")));
});

CB_TEST("emulate.run.summary_line", [] {
    Run r;
    r.exit = "ret";
    r.steps = 42;
    r.calls.resize(2);
    r.writes.resize(3);
    CB_CHECK_EQ(r.summary(), "<Run ret steps=42 calls=2 writes=3>");
    r.truncated = true;
    CB_CHECK_EQ(r.summary(), "<Run ret steps=42 calls=2 writes=3 truncated>");
    Run empty;
    CB_CHECK_EQ(empty.summary(), "<Run none steps=none calls=0 writes=0>");
});

CB_TEST("emulate.run.call_details_are_typed", [] {
    Call c;
    c.name = "KERNEL.GlobalAlloc";
    c.args = std::vector<std::int64_t>{2, 0x100};
    ImportHit hit;
    hit.api = "KERNEL.GlobalAlloc";
    hit.n = 17;
    hit.args_stack = {2, 0x100};
    hit.pointed.emplace_back(0, cb::Bytes{'A', 'B'});
    c.detail = hit;
    CB_CHECK(std::holds_alternative<ImportHit>(c.detail));
    CB_CHECK_EQ(std::get<ImportHit>(c.detail).n, 17);
    Call d;
    d.detail = ServiceCall{0x0001, 0x8002};                         // device 1, service 2 with the jump bit
    CB_CHECK(std::get<ServiceCall>(d.detail).device.has_value());
    d.detail = ServiceCall{std::nullopt, 0xC0001000};               // a call out through a pointer: an address
    CB_CHECK(!std::get<ServiceCall>(d.detail).device.has_value());
    CB_CHECK(!d.name.has_value() && !d.args.has_value());
});

CB_TEST("emulate.run.raw_holds_the_emulators_own_record", [] {
    struct Record { int insns = 0; };
    Run r;
    r.raw = Record{99};
    CB_CHECK_EQ(std::any_cast<Record>(&r.raw)->insns, 99);
    Run copy = r;                                                   // a Run copies (the record with it)
    CB_CHECK_EQ(std::any_cast<Record>(&copy.raw)->insns, 99);
});
