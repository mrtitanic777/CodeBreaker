// Tests of linking a program from its sources: the objects the linker is given and the files it is run with, and the comparison of what it wrote with the
// original -- on the real projects of the research (C 3.00's P0 and P1, LIB 3.00) against the recorded answers
// (testdata/w5b3.golden.json): where the library part begins, the linker's switches, the kit's objects linked first, the objects of the variables
// without a value, the library's modules the rest needs (CRTLIB) and the names left open, an EXE taken apart, and the diagnosis of an EXE against the
// original -- the same, one with bytes damaged in the code, the library part and the data, one with its data moved by two bytes -- and where a data
// segment runs ahead of or behind another. The linker itself is run by a stand-in DOS host here (the real one is the period tools': a run of it is
// the gate's, not a unit test's), and the project's modules by a compiler that is none: what is tested of them is that a module that does not compile
// stops the link, and a kit's object goes in as it is.
//
// Behaviour pinned here:
//   * The linker's environment (PATH=C:\ and TMP=C:\) is the DOS host's to give: DosHost::run has no environment.
//   * A relocation of an EXE is held as (offset << 16) | segment; the comparisons are the same ones.
#include "cb/exact/link.hpp"

#include "cb/core/crypto.hpp"
#include "cb/exact/real_projects.hpp"

using namespace cb;
using namespace cb::exact;
using test::canon;
using test::check;
using test::golden;

namespace {

std::string sha1_of(ByteView b) { return crypto::to_hex(crypto::sha1(b)); }

Json objects_json(const std::vector<link::NamedObject>& v) {
    Json j = Json::array();
    for (const link::NamedObject& o : v) j.push_back(Json::array({o.name, sha1_of(o.object)}));
    return j;
}

Json strings_json(const std::vector<std::string>& v) { return Json(v); }

// A DOS host that writes a program and says what it was asked.
struct FakeDos final : linkwhole::DosHost {
    mutable std::vector<std::string> commands;
    mutable std::vector<std::filesystem::path> exes;
    mutable std::string exe_bytes = "MZ fake program";
    std::string run(const std::filesystem::path& exe, const std::filesystem::path& workdir, std::string_view command_line, std::optional<std::uint32_t> psp) const override {
        commands.push_back(std::string(command_line));
        exes.push_back(exe);
        CB_CHECK(psp.has_value());
        if (exe.filename() == "LINK.EXE") {
            fs::write_text(workdir / "OUT.EXE", exe_bytes);
            fs::write_text(workdir / "OUT.MAP", " Start  Stop   Length Name\r\n 00000H 00010H 00011H _TEXT\r\n");
            return "Microsoft (R) Overlay Linker  Version 3.65\r\nCopyright (C) Microsoft Corp 1983-1988.  All rights reserved.\r\n\r\nLINK : warning L4021: no stack segment\r\n";
        }
        return "EXEMOD said this\r\n";
    }
};

}  // namespace

CB_TEST("exact.link.the_inputs_of_the_link_of_a_real_program_match_the_recorded_output", [] {
    test::Real real;
    if (test::skipped(real, "link inputs")) return;
    for (auto it = golden().at("link").begin(); it != golden().at("link").end(); ++it) {
        const std::string name = it.key();
        const Json& want = it.value();
        const auto p = real.open_named(name);
        check(link::library_start(*p) == want.at("library_start").get<std::uint32_t>(), name + ": where the library part begins");
        check(link::options_of(*p) == want.at("opts").get<std::string>(), name + ": the linker's switches");
        check(canon(objects_json(link::first_objects(*p))) == canon(want.at("first_objects")), name + ": the kit's objects linked first");
        const link::BssObjects bss = link::bss_objects(*p);
        check(canon(objects_json(bss.objects)) == canon(want.at("bss_objects")), name + ": the objects of the variables without a value");
        check(bss.after == want.at("bss_after").get<bool>(), name + ": after the library");
        // the library's modules the rest needs, and the names left open
        std::vector<link::NamedObject> known = link::first_objects(*p);
        known.insert(known.end(), bss.objects.begin(), bss.objects.end());
        const link::CrtResult crt = link::crt(*p, known);
        check(canon(objects_json(crt.modules)) == canon(want.at("crt").at(0)), name + ": the library's modules");
        check(canon(strings_json(crt.missing)) == canon(want.at("crt").at(1)), name + ": the names left open");
        check(canon(strings_json(crt.conflicts)) == canon(want.at("crt").at(2)), name + ": the routines the program has twice");
        // DOS names
        for (const Json& pair : want.at("dosnames")) check(link::dos_name(pair.at(0).get<std::string>()) == pair.at(1).get<std::string>(), "dos_name of " + pair.at(0).get<std::string>());
    }
});

CB_TEST("exact.link.an_exe_taken_apart_and_compared_with_the_original", [] {
    test::Real real;
    if (test::skipped(real, "link diagnose")) return;
    for (const char* name : {"msc-3.0/recon/p0", "msc-3.0/recon/p1"}) {
        const Json& want = golden().at("link").at(name);
        const auto p = real.open_named(name);
        const ByteView exe = p->file();
        const link::Parts parts = link::parts(exe);
        const Json& wp = want.at("parts");
        check(canon(Json(std::vector<std::uint16_t>(parts.header.begin(), parts.header.end()))) == canon(wp.at(0)), std::string(name) + ": the header");
        check(sha1_of(parts.image) == wp.at(1).get<std::string>() && parts.image.size() == wp.at(2).get<std::size_t>(), std::string(name) + ": the image");
        check(parts.relocations.size() == wp.at(4).get<std::size_t>(), std::string(name) + ": the relocations");
        for (std::size_t i = 0; i < 5 && i < parts.relocations.size(); ++i)
            check(Json::array({parts.relocations[i] >> 16, parts.relocations[i] & 0xFFFF}) == wp.at(3).at(i), std::string(name) + ": a relocation");
        // the same as itself: nothing to say
        check(link::diagnose(*p, exe, exe) == want.at("diagnose_same").get<std::vector<std::string>>(), std::string(name) + ": diagnose of the same");
        // bytes damaged in the code, the library part and the data
        Bytes bad(exe.begin(), exe.end());
        const std::size_t base = static_cast<std::size_t>(parts.header[4]) * 16;
        for (std::size_t off : {std::size_t{0x40}, std::size_t{0x41}, std::size_t{p->own_end()} + 5, static_cast<std::size_t>(p->dgroup()) + 7, static_cast<std::size_t>(p->dgroup()) + 8, static_cast<std::size_t>(p->dgroup()) + 300})
            bad[base + off] ^= 0x55;
        check(sha1_of(bad) == want.at("damaged_sha").get<std::string>(), std::string(name) + ": the damaged copy is the recorded one");
        check(link::diagnose(*p, exe, bad) == want.at("diagnose_damaged").get<std::vector<std::string>>(), std::string(name) + ": diagnose of the damaged");
        // the data moved by two bytes
        Bytes shifted(exe.begin(), exe.begin() + static_cast<std::ptrdiff_t>(base + p->dgroup() + 20));
        shifted.push_back(1);
        shifted.push_back(2);
        shifted.insert(shifted.end(), exe.begin() + static_cast<std::ptrdiff_t>(base + p->dgroup() + 20), exe.end());
        check(link::diagnose(*p, exe, shifted) == want.at("diagnose_shifted").get<std::vector<std::string>>(), std::string(name) + ": diagnose of the shifted");
        // where a data segment runs ahead of another
        const ByteView data = p->image().subspan(static_cast<std::size_t>(p->dgroup()));
        Bytes moved(data.begin(), data.begin() + 100);
        moved.push_back(7);
        moved.push_back(7);
        moved.insert(moved.end(), data.begin() + 100, data.end());
        check(link::drift(data, moved) == want.at("drift").get<std::vector<std::string>>(), std::string(name) + ": drift");
    }
});

CB_TEST("exact.link.drift_names_where_one_runs_ahead_of_the_other", [] {
    Bytes a;
    for (int i = 0; i < 600; ++i) a.push_back(static_cast<std::uint8_t>((i * 7 + (i >> 3)) & 0xFF));
    const Json& cases = golden().at("drift_synthetic").at("cases");
    check(sha1_of(a) == golden().at("drift_synthetic").at("a").get<std::string>(), "the synthetic bytes are the recorded ones");
    for (const Json& c : cases) {
        const std::size_t cut = c.at(0).get<std::size_t>();
        Bytes b(a.begin(), a.begin() + static_cast<std::ptrdiff_t>(cut));
        b.push_back(0xAA);
        b.push_back(0xBB);
        b.insert(b.end(), a.begin() + static_cast<std::ptrdiff_t>(cut), a.end());
        check(link::drift(a, b) == c.at(1).get<std::vector<std::string>>(), "drift at " + std::to_string(cut));
        check(link::drift(a, b, 4, 8, 64) == c.at(2).get<std::vector<std::string>>(), "drift with its own step at " + std::to_string(cut));
    }
    // typed: the shifts are the lines' numbers
    const std::vector<link::Shift> s = link::shifts(a, a);
    CB_CHECK_EQ(s.size(), std::size_t{1});   // one change from the start: no move
    CB_CHECK_EQ(s[0].by, 0);
    // a run of few distinct bytes says nothing
    const Bytes zeros(300, 0);
    CB_CHECK(link::shifts(zeros, zeros).empty());
});

CB_TEST("exact.link.the_linker_is_run_over_objects_in_a_folder_of_its_own", [] {
    test::Real real;
    if (test::skipped(real, "link run")) return;
    // LIB 3.00's linker: the kit's own LINK.EXE is copied in, the objects named for DOS and listed six to a line in a response file
    const auto p = real.open_named("msc-3.0/recon/p0");
    FakeDos dos;
    std::vector<link::NamedObject> objs;
    for (const char* n : {"_exit", "exit", "BINMODE", "A", "B", "C", "D"}) objs.push_back({n, Bytes{1, 2, 3}});
    fs::ScopedTempDir keep("w5b3-link");
    const linkwhole::LinkOutput out = link::run_link(*p, objs, dos, keep.path(), "/NOI /STACK:4096");
    CB_CHECK(out.exe.has_value());
    CB_CHECK_EQ(std::string(out.exe->begin(), out.exe->end()), "MZ fake program");
    CB_CHECK(out.console.find("LINK : warning L4021") != std::string::npos);
    CB_CHECK(out.console.find("L4021: no stack segment\n") != std::string::npos);   // (the linker's words have no carriage returns)
    CB_CHECK(out.map.find("_TEXT") != std::string::npos);
    CB_CHECK_EQ(dos.commands.at(0), "@L.LNK");
    const std::string rsp = fs::read_text(keep.path() / "L.LNK");
    CB_CHECK(rsp.starts_with("EXIT+EXIT01+BINMODE+A+B+C+\r\nD\r\nOUT.EXE\r\nOUT.MAP\r\n"));   // (DOS names: _exit and exit are both EXIT; six a line)
    CB_CHECK(rsp.ends_with("/NOI /STACK:4096;\r\n"));
    CB_CHECK(std::filesystem::is_regular_file(keep.path() / "EXIT.OBJ") && std::filesystem::is_regular_file(keep.path() / "EXIT01.OBJ"));
    CB_CHECK(std::filesystem::is_regular_file(keep.path() / "LINK.EXE"));
    // a linker that wrote nothing: no program
    FakeDos silent;
    std::filesystem::remove(keep.path() / "OUT.EXE");
    struct Nothing final : linkwhole::DosHost {
        std::string run(const std::filesystem::path&, const std::filesystem::path&, std::string_view, std::optional<std::uint32_t>) const override { return "Fatal error L1051\r\n"; }
    } none;
    const linkwhole::LinkOutput failed = link::run_link(*p, objs, none);
    CB_CHECK(!failed.exe.has_value());
    CB_CHECK_EQ(failed.console, "Fatal error L1051\n");
    CB_CHECK(failed.map.empty());
});

CB_TEST("exact.link.the_kits_objects_are_linked_first", [] {
    test::Real real;
    if (test::skipped(real, "link objects")) return;
    // FIRSTOBJ and the kit: a name a line, a comment, blank lines
    fs::ScopedTempDir dir("w5b3-link");
    const auto folder = test::copy_project("msc-3.0/recon/p0", dir.path() / "p");
    fs::write_text(folder / "src" / "FIRSTOBJ", "# the kit's objects\n\nBINMODE.OBJ   # one\n");
    const auto q = real.open(folder);
    const std::vector<link::NamedObject> first = link::first_objects(*q);
    CB_CHECK_EQ(first.size(), std::size_t{1});
    CB_CHECK_EQ(first[0].name, "BINMODE");
    CB_CHECK(!first[0].object.empty());
});

CB_TEST("exact.link.a_toolchain_with_no_linker_is_a_usage_error", [] {
    test::Real real;
    if (test::skipped(real, "link profile")) return;
    const auto p = real.open_named("masm-1.00/recon/exact/c100");
    CB_CHECK_THROWS_AS(UsageError, (void)link::first_objects(*p));
    FakeDos dos;
    CB_CHECK_THROWS_AS(UsageError, (void)link::run_link(*p, {}, dos));
});
