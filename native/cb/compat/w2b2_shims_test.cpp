// The old verify/lane_common and analyze/dos_functions API (codebreaker::*, over py::) as the compat shims at
// native/typed/cpp/codebreaker/... give it: conversion and forwarding to cb/verify/lane_common and cb/analyze/dos_functions.
// Builds at /W3. (The dispatch and structures shims take an old analyze::image::Image, which cannot be made here: they are
// compiled by shim_check_w2b2.cpp, and run through the exe.)
#include "codebreaker/analyze/dos_functions.hpp"
#include "codebreaker/verify/lane_common.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

namespace lc = codebreaker::verify::lane_common;
namespace df = codebreaker::analyze::dos_functions;
namespace dos = codebreaker::formats::dos;
namespace run = codebreaker::emulate::run;

std::vector<std::string> strs(const py::list<py::str>& l) {
    std::vector<std::string> out;
    for (const py::str& s : l) out.push_back(s.utf8());
    return out;
}

}  // namespace

CB_TEST("compat.w2b2.lane_common_text_of_calls_and_arguments", [] {
    CB_CHECK(lc::call(py::str("f")) == py::str("f"));
    py::list<py::str> args;
    args.append(py::str("a0"));
    args.append(py::str("S"));
    CB_CHECK(lc::call(py::str("f"), args) == py::str("f(a0,S)"));
    py::list<lc::Called> seq;
    seq.append(lc::Called{py::str("A"), py::list<int64_t>{1, 2}});
    seq.append(lc::Called{std::nullopt, py::list<int64_t>{3}});           // a call with no name is "None", as the port had it
    seq.append(lc::Called{py::str("skipped"), std::nullopt});
    seq.append(lc::Called{py::str("B"), std::nullopt});
    const py::list<py::str> shown = lc::calls(seq, [](int64_t v) { return py::percent("%d", v); }, nullptr,
                                              [](const std::optional<py::str>& n) { return n && *n == py::str("skipped"); });
    CB_CHECK((strs(shown) == std::vector<std::string>{"A(1,2)", "None(3)", "B"}));
    const py::list<py::str> capped = lc::calls(seq, [](int64_t v) { return py::percent("%d", v); }, nullptr, nullptr, std::optional<int64_t>(1));
    CB_CHECK((strs(capped) == std::vector<std::string>{"A(1,2)", "..."}));
    py::list<int64_t> inputs{7, 0x12345};
    CB_CHECK(lc::argument(0x12345, inputs, 0x100) == py::str("a1"));
    CB_CHECK(lc::argument(7, inputs, 0x100) == py::str("7"));
    CB_CHECK(lc::argument(0xFFFFFFFF, inputs, std::nullopt, true) == py::str("-1"));
    py::list<lc::Span> frame;
    frame.append(std::nullopt);
    frame.append(lc::Span(std::make_tuple(int64_t(0x7ff00000), int64_t(0x80000000))));
    CB_CHECK(lc::argument(0x7ff01000, py::list<int64_t>(), std::nullopt, false, frame) == py::str("S"));
    CB_CHECK(lc::argument(0x401000, py::list<int64_t>(), std::nullopt, false, py::list<lc::Span>(),
                          lc::Image(std::make_tuple(int64_t(0x400000), int64_t(0x500000)))) == py::str("P"));
    CB_CHECK(lc::argument(0x1f3a0000) == py::str("1f3a0000"));
});

CB_TEST("compat.w2b2.lane_common_memory_digest_and_inputs", [] {
    lc::State state;
    state.set(lc::Place(int64_t(4096)), lc::Held(int64_t(5)));
    state.set(lc::Place(int64_t(4097)), lc::Held(py::str("S")));
    CB_CHECK(lc::writes(state) == py::str("2:a337cdbf5e9315c6"));        // the digest the fingerprints already stored were made with
    CB_CHECK(lc::writes(lc::State()) == py::str(""));
    const py::list<int64_t> counted = lc::counting(3, 10, 5), down = lc::descending(2, 0xFFFFFFF0);
    const py::list<int64_t> mixed = lc::alternating(4, lc::Pattern(int64_t(1)), lc::Pattern(std::function<int64_t(int64_t)>([](int64_t i) { return i * 10; })));
    CB_CHECK((counted.vec() == std::vector<int64_t>{10, 15, 20}));
    CB_CHECK((down.vec() == std::vector<int64_t>{0xFFFFFFF0, 0xFFFFFFEF}));
    CB_CHECK((mixed.vec() == std::vector<int64_t>{1, 10, 1, 30}));
    CB_CHECK(lc::MAX_GUIDED == 16 && lc::GUIDED_BUDGET == 96 && lc::PATH_BUDGET == 32 && lc::MAX_PATHS == 24 && lc::GUIDED_REST == 32);
});

CB_TEST("compat.w2b2.lane_common_run_to_fingerprint", [] {
    py::list<run::Write> writes;
    run::Write w;
    w.address = 0x1000;
    w.size = 4;
    w.value = 0x11223344;
    w.old = 0x11223300;
    writes.append(w);
    run::Write w2;
    w2.address = 0x3000;
    w2.size = 4;
    w2.value = 0x402000;
    writes.append(w2);
    run::Run r(py::str("ret"), std::nullopt, writes, false, py::list<run::Call>(), py::list<run::IO>(), 8);
    const lc::State changed = lc::changed(r);
    CB_CHECK(changed.size() == 5);                                         // the one byte that differs, and the four of a write with no old value
    const auto tokens = lc::changed(r, nullptr, [](int64_t v) -> std::optional<py::str> { return v == 0x402000 ? std::optional<py::str>(py::str("S")) : std::nullopt; });
    CB_CHECK(tokens.at(lc::Place(int64_t(0x3000))) == lc::Held(py::str("S")));
    CB_CHECK(lc::left(r).has_value());
    const auto fp = lc::fingerprint(r, py::list<py::str>(), std::optional<int64_t>(42), py::str(""), true);
    CB_CHECK(fp.ret == 42 && fp.exit == py::str("ret") && fp.pop == 8 && fp.saved == true && fp.writes == std::optional<py::str>(py::str("")));
    run::Run truncated(py::str("truncated"), std::nullopt, py::list<run::Write>(), true, py::list<run::Call>(), py::list<run::IO>(), 8, std::nullopt,
                       py::list<int64_t>(), std::nullopt, true);
    CB_CHECK(!lc::left(truncated).has_value());                            // writes capped: none
    const auto fp2 = lc::fingerprint(truncated, py::list<py::str>(), std::nullopt, std::nullopt, true);
    CB_CHECK(fp2.truncated && !fp2.pop && !fp2.saved && !fp2.writes);
});

CB_TEST("compat.w2b2.lane_common_judged_keeps_the_old_exceptions_apart", [] {
    auto [verdict, reason, limits, detail] = lc::judged([](codebreaker::verify::compare::Info& info) {
        info.detail = py::str("ret 1 != 2");
        return std::make_tuple(py::str("divergent"), py::str("return value"), py::list<py::str>{py::str("argc-unknown")});
    });
    CB_CHECK(verdict == py::str("divergent") && reason == py::str("return value") && detail == py::str("ret 1 != 2") && limits.size() == 1);
    auto [v2, r2, l2, d2] = lc::judged([](codebreaker::verify::compare::Info&) -> std::tuple<py::str, py::str, py::list<py::str>> {
        throw py::ValueError("bad");
    });
    CB_CHECK(v2 == py::str("inconclusive") && r2 == py::str("judge: ValueError") && l2.empty() && d2 == py::str(""));
    auto [v3, r3, l3, d3] = lc::judged([](codebreaker::verify::compare::Info&) -> std::tuple<py::str, py::str, py::list<py::str>> {
        throw std::runtime_error("boom");
    });
    CB_CHECK(r3 == py::str("judge: Exception"));
    CB_CHECK_THROWS_AS(py::ImportError, (void)lc::judged([](codebreaker::verify::compare::Info&) -> std::tuple<py::str, py::str, py::list<py::str>> {
                           throw py::ImportError("no module");
                       }));
});

CB_TEST("compat.w2b2.dos_functions_over_an_old_program", [] {
    // 0: call 10h / mov ah,4Ch / int 21h ; 10h: push bp / mov bp,sp / mov ah,9 / int 21h / pop bp / ret
    std::string image(31, '\0');
    const unsigned char main_code[] = {0xE8, 0x0D, 0x00, 0xB4, 0x4C, 0xCD, 0x21};
    const unsigned char callee[] = {0x55, 0x8B, 0xEC, 0xB4, 0x09, 0xCD, 0x21, 0x5D, 0xC3};
    std::copy(main_code, main_code + sizeof main_code, image.begin());
    std::copy(callee, callee + sizeof callee, image.begin() + 0x10);
    dos::Program p(py::str("C:/x/demo.exe"), py::str("mz"));
    p.image = py::bytes(image);
    p.spans.append(dos::Span(p.base, 31, py::str("image")));
    p.entries.append(dos::EntryPoint(p.base, py::str("start"), py::str("near")));
    const df::Analysis a = df::discover(p);
    CB_CHECK(a.functions.size() == 2);
    const auto start = a.functions.at(p.base);
    CB_CHECK(start->name == py::str("start") && start->evidence == py::str("entry") && start->distance == py::str("near") && start->removes == -1 &&
             start->size == 7 && start->sure);
    const auto fn = a.functions.at(p.base + 0x10);
    CB_CHECK(fn->name == py::str("sub_00010") && fn->evidence == py::str("called") && fn->removes == 0 && fn->size == 9 && !fn->interrupt);
    CB_CHECK(a.interrupts.size() == 2);
    CB_CHECK(a.interrupts[0].number == 0x21 && a.interrupts[0].ah == 0x4C && !a.interrupts[0].al && a.interrupts[0].function == start);
    CB_CHECK(a.interrupts[1].function == fn && a.interrupts[1].address == p.base + 0x15);
    const auto census = a.census();
    CB_CHECK(census.get(py::str("code"), -1) == 16 && census.get(py::str("not classified"), -1) == 15 && census.get(py::str("total"), -1) == 31);
    const py::bytes marks = a._mark_bytes();
    CB_CHECK(marks.size() == 31 && marks.byte_(0) == 1 && marks.byte_(1) == 2 && marks.byte_(3) == 1 && marks.byte_(0x20 - 1) == 0);
    CB_CHECK(a._guess_bytes().size() == 31);
    CB_CHECK(fn->repr() == py::str("Function(sub_00010 at 0x10010, near, called)"));
});

CB_TEST("compat.w2b2.dos_functions_with_a_program_that_has_no_data_segment", [] {
    // a table jump through DS with no DS known crashed the port with a TypeError; the table is just not followed
    std::string image(0x20, '\0');
    const unsigned char code[] = {0xFF, 0xA7, 0x10, 0x00};
    std::copy(code, code + sizeof code, image.begin());
    dos::Program p(py::str("x.exe"), py::str("mz"));
    p.image = py::bytes(image);
    p.ds = std::nullopt;
    p.spans.append(dos::Span(p.base, 0x20, py::str("image")));
    p.entries.append(dos::EntryPoint(p.base, py::str("start"), py::str("near")));
    const df::Analysis a = df::discover(p);
    CB_CHECK(a.functions.size() == 1 && a.tables.empty());
});
