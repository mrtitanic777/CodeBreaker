// Tests of the first reading of a program (program_prep): which files the port keeps and the mending that is the same for
// every program (the structure passes of views.hpp among it). The expected values are what the Python original made of
// the same sources (testdata/views/make_views_cases.py): small hand-written programs, one for each mending, and the real
// trees of MS C 3.00 (compared by CRC).
#include "cb/core/testing.hpp"
#include "cb/port/program_prep.hpp"
#include "cb/port/views_case.hpp"

using namespace cb;
using namespace cb::port::pre_ansi;
using namespace cb::port::pre_ansi::vcase;

namespace {

// The program's own library: what the profile says is the old C library's.
Profile library_profile() {
    Profile p;
    p.runtime_headers = {"stdio.h"};
    p.runtime_defines = {"fopen"};
    return p;
}

void check_case(const std::string& name) {
    const Case c = load(name);
    const Profile profile = profile_of(c);
    OrderedMap<std::string, std::string> members;
    for (const auto& [k, v] : c.expected["members"].items()) members.insert_or_assign(k, v.get<std::string>());
    const PreparedProgram p = prepare_program(c.originals, profile, members);
    const J& want = c.expected["ctor"];

    std::vector<std::string> kept;
    for (const auto& kv : p.files) kept.push_back(kv.first);
    CHECK_J(J(kept), want["kept"]);
    J left = J::array();
    for (const auto& kv : p.left_out) left.push_back(J::array({kv.first, kv.second}));
    CHECK_J(left, want["left_out"]);
    CHECK_J(notes_json(p.notes), want["notes"]);
    CHECK_J(notes_json(p.union_notes), want["union_notes"]);
    CHECK_J(J(p.member_fixes), want["member_fixes"]);
    J dropped = J::array();
    for (const auto& kv : p.far_near_dropped) dropped.push_back(J::array({kv.first, kv.second}));
    CHECK_J(dropped, want["far_near"]);
    J aliases = J::array();
    for (const auto& kv : p.aliases) aliases.push_back(J::array({kv.first, kv.second.where, kv.second.declaration}));
    CHECK_J(aliases, want["aliases"]);
    CHECK_J(J(p.view_vars), want["view_vars"]);
    CHECK_J(J(p.view_headers), want["view_headers"]);
    J renames = J::array();
    for (const auto& kv : p.renames) renames.push_back(J::array({kv.first, kv.second}));
    CHECK_J(renames, want["renames"]);
    CB_CHECK_EQ(p.int16, want["int16"].get<bool>());
    for (const auto& kv : p.originals) CHECK_J(crc(kv.second), want["originals"][kv.first]);
    for (const auto& kv : p.files) {
        CHECK_J(crc(kv.second), want["files"][kv.first]);
        if (!c.real) CB_CHECK_EQ(kv.second, fs::read_text(c.dir / "ctor" / kv.first));   // (small: the whole text, to read a failure by)
    }
}

}  // namespace

CB_TEST("port.program_prep.synthetic_programs_are_mended_as_the_original_mended_them", [] {
    for (const std::string& name : synthetic_names()) {
        // (the cases of the passes that need more than the constructor are views_test's)
        check_case(name);
    }
});

CB_TEST("port.program_prep.ms_c_3_00_trees_are_mended_as_the_original_mended_them", [] {
    for (const std::string& name : real_names()) check_case(name);
});

CB_TEST("port.program_prep.the_old_librarys_files_are_left_out", [] {
    const Profile profile = library_profile();
    CB_CHECK_EQ(runtime_left_out("stdio.h", "int x;", profile).value_or(""),
                "the old C library's header: the C library of today has its own");
    CB_CHECK(!runtime_left_out("app.h", "int x;", profile));
    CB_CHECK_EQ(runtime_left_out("io.c", "FILE *fopen(n, m)\nchar *n, *m;\n{\n\treturn 0;\n}\n", profile).value_or(""),
                "the old C library (it defines fopen): the C library of today stands in");
    CB_CHECK(!runtime_left_out("io.c", "main()\n{\n\treturn 0;\n}\n", profile));
    CB_CHECK(!runtime_left_out("stdio.c", "/* fopen( in a comment */\nint x;\n", profile));   // (a name in a comment is no definition)
});

CB_TEST("port.program_prep.a_member_without_its_structures_closing_is_left_alone", [] {
    // DIVERGES: the Python looked for `};` with find(), got -1 for a structure closed any other way, and cut the file at its
    // last character; the structure is skipped now (the member keeps the type it has).
    Files files;
    files.insert_or_assign("a.h", "typedef struct rec { int n; } REC;\n");
    OrderedMap<std::string, std::string> members;
    members.insert_or_assign("rec.n", "long");
    const PreparedProgram p = prepare_program(files, library_profile(), members);
    CB_CHECK_EQ(p.files.at("a.h"), "typedef struct rec { int n; } REC;\n");
    CB_CHECK(p.member_fixes.empty());
});

CB_TEST("port.program_prep.a_member_is_declared_the_type_the_program_uses_it_as", [] {
    Files files;
    files.insert_or_assign("a.h", "struct rec {\n\tint n;\n\tint m;\n};\n");
    OrderedMap<std::string, std::string> members;
    members.insert_or_assign("rec.m", "char *");
    const PreparedProgram p = prepare_program(files, library_profile(), members);
    CB_CHECK_EQ(p.files.at("a.h"), "struct rec {\n\tint n;\n\tchar *m;\n};\n");
    CB_CHECK_EQ(p.member_fixes.size(), 1u);
    CB_CHECK_EQ(p.member_fixes[0], "a.h: struct rec's m is char * (as the program uses it)");
    CB_CHECK_EQ(p.originals.at("a.h"), p.files.at("a.h"));   // (the originals are as corrected)
});
