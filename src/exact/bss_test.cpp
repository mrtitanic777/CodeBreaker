// Tests of the variables that begin without a value (src/BSS), on the real projects of the research (C 3.00's P0 and P2, LIB 3.00, LINK 2.44 and MASM 5.10,
// whose compiler puts its communal names in a segment of its own) against the recorded answers (testdata/w5b3.golden.json): the range,
// the names with their places, the library modules' own _BSS holes, the text of src/BSS and the command's output.
//
// Behaviour pinned here: a word of a source is a run of ASCII letters, digits and underscores (not any letter of the text read as
// Latin-1); none of the projects has a source with a letter beyond ASCII in a name.
#include "exact/bss.hpp"

#include "exact/real_projects.hpp"

using namespace cb;
using namespace cb::exact;
using test::canon;
using test::check;
using test::golden;

CB_TEST("exact.bss.the_proposals_for_real_programs_match_the_recorded_output", [] {
    test::Real real;
    if (test::skipped(real, "bss")) return;
    int with_holes = 0, with_communal = 0;
    for (auto it = golden().at("bss").begin(); it != golden().at("bss").end(); ++it) {
        const std::string name = it.key();
        const auto p = real.open_named(name);
        const bss::Proposal got = bss::propose(*p);
        const Json& want = it.value().at("propose");
        check(got.edata == want.at(0).get<std::int64_t>() && got.stack == want.at(1).get<std::int64_t>(), name + ": the range");
        Json names = Json::array();
        for (const auto& [n, a] : got.names) names.push_back(Json::array({n, a}));
        check(canon(names) == canon(want.at(2)), name + ": the names");
        Json holes = Json::array();
        for (const bss::Hole& h : got.holes) holes.push_back(Json::array({h.address, h.length, h.modules}));
        check(canon(holes) == canon(want.at(3)), name + ": the holes");
        with_holes += !got.holes.empty();
        with_communal += it.value().at("communal").is_object();
        // the text of src/BSS
        const std::vector<std::string> lines = bss::lines(p->profile_parts()->communal, got);
        check(canon(Json(lines)) == canon(it.value().at("lines")), name + ": the lines");
        // the command
        testing::CapturedStd cap;
        check(bss::run(*p, {}) == it.value().at("main").at(1).get<int>(), name + ": exit code");
        check(cap.out() == it.value().at("main").at(0).get<std::string>(), name + ": the command's text");
    }
    CB_CHECK(with_holes > 0);       // some library module has its own _BSS among them
    CB_CHECK_EQ(with_communal, 1);  // and one compiler's communal names are in a segment of their own
});

CB_TEST("exact.bss.the_text_of_src_bss_and_the_file_the_command_writes", [] {
    test::Real real;
    if (test::skipped(real, "bss write")) return;
    fs::ScopedTempDir dir("w5b3-bss");
    const auto p = real.open(test::copy_project("msc-3.0/recon/p0", dir.path() / "p"));
    {
        testing::CapturedStd cap;
        bss::run(*p, {"--write"});
    }
    const std::string written = fs::read_text(p->path("BSS"));
    const bss::Proposal proposal = bss::propose(*p);
    CB_CHECK_EQ(written, text::join(bss::lines(std::nullopt, proposal), "\n") + "\n");
    CB_CHECK(written.starts_with("# the variables that begin without a value (codebreaker exact bss)"));
    CB_CHECK(written.find(std::format("range {:04x} {:04x}", proposal.edata, proposal.stack)) != std::string::npos);
    // the lines of a hand-made proposal: the range, a segment of communal names, a hole, the names by place then name
    bss::Proposal made;
    made.edata = 0x10;
    made.stack = 0x20;
    made.holes = {{0x12, 4, {"A", "B"}}};
    made.names = {{"_b", 0x14}, {"_a", 0x14}, {"_c", 0x11}};
    const std::vector<std::string> lines = bss::lines(project::Communal{"c_common", true}, made);
    CB_CHECK_EQ(lines.size(), std::size_t{8});
    CB_CHECK_EQ(lines[2], "range 0010 0020");
    CB_CHECK_EQ(lines[3], "segment c_common para     # (the compiler's segment of communal names: after the library)");
    CB_CHECK_EQ(lines[4], "hole 0012 4     # the _BSS of the library's A, B");
    CB_CHECK_EQ(lines[5], "_c 0011");
    CB_CHECK_EQ(lines[6], "_a 0014");
    CB_CHECK_EQ(lines[7], "_b 0014");
    CB_CHECK_EQ(bss::lines(project::Communal{"c_common", false}, made)[3], "segment c_common     # (the compiler's segment of communal names: after the library)");
});
