#include "exact/repair.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::exact::repair;
using cb::exact::pascal_join::Verdict;

std::vector<std::string> whats(const std::vector<Candidate>& c) {
    std::vector<std::string> out;
    for (const Candidate& x : c) out.push_back(x.what);
    return out;
}

}  // namespace

CB_TEST("exact.repair.a_note_names_the_shift_and_the_places", [] {
    const auto s = shifts({"unrelated", "? 2 values each +3 in the program (+1a, 20)"});
    CB_CHECK(s.has_value());
    CB_CHECK_EQ(s->by, 3);
    CB_CHECK_EQ(s->offsets.size(), 2u);
    CB_CHECK_EQ(s->offsets[0], 0x1a);
    CB_CHECK_EQ(s->offsets[1], 0x20);
    const auto negative = shifts({"? 1 value -2 in the program (4)"});
    CB_CHECK_EQ(negative->by, -2);
    CB_CHECK_EQ(negative->offsets.size(), 1u);
    CB_CHECK(!shifts({"? 1 value in the program (4)"}).has_value());
    CB_CHECK(!shifts({}).has_value());
    // a blank place is skipped
    CB_CHECK_EQ(shifts({"? 3 values each +1 in the program (1, , 2)"})->offsets.size(), 2u);
});

CB_TEST("exact.repair.the_names_of_a_family_are_moved_first_alone_then_together", [] {
    const std::string text = "x := DK46;\ny := DK10;\nz := DK46 + TK46;\n";
    const std::int64_t mine[] = {46};
    const auto c = candidates(text, mine, 1);
    const auto w = whats(c);
    // DK46 and TK46 are named as 46 is (two families); the families are tried in the order they appear
    CB_CHECK_EQ(w.size(), 3u);
    CB_CHECK_EQ(w[0], "DK46 moved by +1");
    CB_CHECK_EQ(c[0].text, "x := DK47;\ny := DK10;\nz := DK47 + TK46;\n");
    CB_CHECK_EQ(w[1], "every DK<n> moved by +1");
    CB_CHECK_EQ(c[1].text, "x := DK47;\ny := DK11;\nz := DK47 + TK46;\n");
    CB_CHECK_EQ(w[2], "TK46 moved by +1");
    CB_CHECK_EQ(c[2].text, "x := DK46;\ny := DK10;\nz := DK46 + TK47;\n");
});

CB_TEST("exact.repair.a_family_whose_every_member_is_wanted_is_one_candidate", [] {
    const std::int64_t mine[] = {3};
    const auto c = candidates("A := PT3; B := PT3;", mine, 2);
    // moving the named members alone is moving every member: the second candidate would be the same text
    CB_CHECK_EQ(c.size(), 1u);
    CB_CHECK_EQ(c[0].what, "PT3 moved by +2");
    CB_CHECK_EQ(c[0].text, "A := PT5; B := PT5;");
});

CB_TEST("exact.repair.a_literal_is_changed_where_it_stands_alone", [] {
    const std::string text = "a := 12;\nb := x12;\nc := 12.5;\nd := 112;\ne := 12 + 1;\n";
    const std::int64_t mine[] = {12};
    const auto c = candidates(text, mine, -2);
    const auto w = whats(c);
    CB_CHECK_EQ(w.size(), 3u);   // line 1, line 5, and every one
    CB_CHECK_EQ(w[0], "the literal 12 made 10 (line 1)");
    CB_CHECK_EQ(c[0].text, "a := 10;\nb := x12;\nc := 12.5;\nd := 112;\ne := 12 + 1;\n");
    CB_CHECK_EQ(w[1], "the literal 12 made 10 (line 5)");
    CB_CHECK_EQ(c[1].text, "a := 12;\nb := x12;\nc := 12.5;\nd := 112;\ne := 10 + 1;\n");
    CB_CHECK_EQ(w[2], "every literal 12 made 10");
    CB_CHECK_EQ(c[2].text, "a := 10;\nb := x12;\nc := 12.5;\nd := 112;\ne := 10 + 1;\n");
});

CB_TEST("exact.repair.the_values_are_taken_as_bytes", [] {
    const std::int64_t mine[] = {0x12C};   // 300: the byte that differs is 0x2C = 44
    const auto c = candidates("n := 44;", mine, 1);
    CB_CHECK_EQ(c.size(), 1u);
    CB_CHECK_EQ(c[0].what, "the literal 44 made 45 (line 1)");
    CB_CHECK(candidates("nothing here", mine, 1).empty());
    CB_CHECK(candidates("", mine, 1).empty());
});

namespace {

// A job whose "program" is the number 7 in the source: the check says identical when the file has it. The file is
// what the check reads, as the ledger reads the project's source.
struct MendFixture {
    fs::ScopedTempDir dir{"repair"};
    std::filesystem::path source = dir.path() / "src" / "A.PAS";
    std::filesystem::path dest = dir.path() / "overlay" / "A.PAS";
    int checks = 0;
    // `other_identical`: the file's other function, identical before a change and only so if the text keeps its "ok"
    Job job(bool other_identical = false) {
        fs::make_dirs(source.parent_path());
        fs::write_text(source, "limit := 6;\nok;\n");
        Job j;
        j.function = "F";
        j.at = 0x1234;
        j.source = source;
        j.dest = dest;
        j.mine = {6};
        j.d = 1;
        j.check = [this, other_identical] {
            ++checks;
            // what the compiler would be given: the destination's text if it has been written, else the source's
            const std::string text = fs::read_text(std::filesystem::exists(dest) ? dest : source);
            std::vector<cb::exact::ledger::Row> rows;
            cb::exact::ledger::Row f;
            f.function = "F";
            f.verdict = text.find("limit := 7") != std::string::npos ? Verdict::Identical : Verdict::Differs;
            rows.push_back(f);
            if (other_identical) {
                cb::exact::ledger::Row g;
                g.function = "G";
                g.verdict = text.find("ok") != std::string::npos ? Verdict::Identical : Verdict::Differs;
                rows.push_back(g);
            }
            return rows;
        };
        return j;
    }
};

}  // namespace

CB_TEST("exact.repair.a_change_that_makes_the_function_the_programs_is_kept_with_write", [] {
    MendFixture f;
    testing::CapturedStd cap;
    CB_CHECK_EQ(mend({f.job()}, /*write*/ true), 0);
    CB_CHECK(cap.out().find("F 01234: the literal 6 made 7 (line 1)") != std::string::npos);
    CB_CHECK(cap.out().find("1 of 1 mended") != std::string::npos);
    CB_CHECK_EQ(fs::read_text(f.dest), "limit := 7;\nok;\n");
    CB_CHECK_EQ(fs::read_text(f.source), "limit := 6;\nok;\n");   // the source file stays: the change is in the overlay
});

CB_TEST("exact.repair.without_write_a_change_is_only_reported_and_the_file_is_as_it_was", [] {
    MendFixture f;
    testing::CapturedStd cap;
    (void)mend({f.job()}, /*write*/ false);
    CB_CHECK(cap.out().find("(not written: --write)") != std::string::npos);
    CB_CHECK(!std::filesystem::exists(f.dest));
    CB_CHECK(cap.out().find("1 of 1 mended") != std::string::npos);
});

CB_TEST("exact.repair.a_change_that_spoils_an_identical_function_is_not_kept", [] {
    MendFixture f;
    Job j = f.job(/*other_identical*/ true);
    // the candidate drops the "ok" the other function needs: the text the check reads has no "ok" ... so make the only
    // candidate do so by changing the job's source
    fs::write_text(f.source, "limit := 6; ok\n");
    j.check = [&f] {
        const std::string text = fs::read_text(std::filesystem::exists(f.dest) ? f.dest : f.source);
        std::vector<cb::exact::ledger::Row> rows;
        cb::exact::ledger::Row a;
        a.function = "F";
        a.verdict = text.find("limit := 7") != std::string::npos ? Verdict::Identical : Verdict::Differs;
        cb::exact::ledger::Row b;
        b.function = "G";
        // G is identical only while the text reads "6" (a change of the literal costs it)
        b.verdict = text.find("limit := 6") != std::string::npos ? Verdict::Identical : Verdict::Differs;
        rows = {a, b};
        return rows;
    };
    testing::CapturedStd cap;
    (void)mend({j}, /*write*/ true);
    CB_CHECK(cap.out().find("no change of those tried makes it the program's") != std::string::npos);
    CB_CHECK(cap.out().find("0 of 1 mended") != std::string::npos);
    CB_CHECK(!std::filesystem::exists(f.dest));
});

CB_TEST("exact.repair.an_existing_destination_is_put_back_after_a_try_that_fails", [] {
    MendFixture f;
    Job j = f.job();
    fs::make_dirs(f.dest.parent_path());
    fs::write_text(f.dest, "previous overlay text\n");
    j.mine = {99};   // no candidate will make F identical: the literal 99 is nowhere ... but the family names are not there either
    testing::CapturedStd cap;
    (void)mend({j}, true);
    CB_CHECK_EQ(fs::read_text(f.dest), "previous overlay text\n");
    CB_CHECK(cap.out().find("no change of those tried") != std::string::npos);
});

CB_TEST("exact.repair.the_piece_of_a_pascal_compiland_is_found_by_its_first_line", [] {
    // piece_of needs a Compiland; its directories are the overlay and the compiland's own folder
    fs::ScopedTempDir dir("repair-piece");
    const auto src = dir.path() / "FIRST";
    fs::make_dirs(src / "FIRST.d");
    fs::write_text(src / "pA1B.pas", "  procedure PA1B;\nbegin end;\n");
    fs::write_text(src / "FIRST.d" / "pC2D.pas", "FUNCTION pc2d(x: integer): integer;\n");
    fs::write_text(src / "pEEEE.pas", "{ a comment }\nPROCEDURE PEEEE;\n");   // the declaration is not on the first line
    cb::exact::pascal_join::Project project;
    project.dir = dir.path();
    cb::exact::pascal_join::CompilandConfig cc;
    cc.dir = "FIRST";
    const cb::exact::pascal_join::Compiland c(project, "FIRST", cc);
    const auto a = piece_of(c, "A1B");
    CB_CHECK(a.has_value());
    CB_CHECK(a->filename() == "pA1B.pas");
    const auto b = piece_of(c, "C2D");   // nested one folder down
    CB_CHECK(b.has_value());
    CB_CHECK(b->filename() == "pC2D.pas");
    CB_CHECK(!piece_of(c, "EEEE").has_value());
    CB_CHECK(!piece_of(c, "ZZZZ").has_value());
});

CB_TEST("exact.repair.only_cuts_a_project_to_one_compiland", [] {
    cb::exact::pascal_join::Project project;
    cb::exact::pascal_join::CompilandConfig a, b;
    a.dir = "A";
    b.dir = "B";
    project.config.compilands.insert_or_assign("FIRST", a);
    project.config.compilands.insert_or_assign("SECOND", b);
    const auto one = only(project, "SECOND");
    CB_CHECK_EQ(one.config.compilands.size(), 1u);
    CB_CHECK(one.config.compilands.contains("SECOND"));
    CB_CHECK_EQ(project.config.compilands.size(), 2u);   // the project itself is not changed
});
