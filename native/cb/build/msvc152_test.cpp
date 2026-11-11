#include "cb/build/msvc152.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::build;

class FakeRunner : public DosRunner {
public:
    DosRunResult run(const DosRunSpec& spec) const override {
        seen = spec;
        ++calls;
        return result;
    }
    mutable DosRunSpec seen;
    mutable int calls = 0;
    DosRunResult result;
};

struct Fixture {
    fs::ScopedTempDir dir{"msvc152"};
    msvc152::Tools tools;
    Fixture() {
        fs::make_dirs(dir.path() / "BIN");
        fs::write_file(dir.path() / "BIN" / "CL.EXE", Bytes{1});
        fs::write_file(dir.path() / "dosbox-x.exe", Bytes{1});
        tools.dosbox_x = dir.path() / "dosbox-x.exe";
        tools.msvc_root = dir.path();
    }
};

}  // namespace

CB_TEST("build.msvc152.compile_asks_the_emulator_for_the_right_run", [] {
    Fixture f;
    FakeRunner runner;
    runner.result.ok = true;
    runner.result.work = f.dir.path() / "work";
    runner.result.made["t.OBJ"] = Bytes{0x80, 1, 2};
    runner.result.made["t.COD"] = to_bytes("; listing \xE9\n");
    runner.result.made["t.LOG"] = to_bytes("compiled\n");
    const msvc152::Compiled c = msvc152::compile_c(runner, f.tools, "int f(void){return 1;}", "/AM /Ox");
    CB_CHECK_EQ(runner.calls, 1);
    const DosRunSpec& s = runner.seen;
    CB_CHECK_EQ(s.commands.size(), 1u);
    CB_CHECK_EQ(s.commands[0], "CL /AM /Ox /Fct.COD /Fot.OBJ t.C > t.LOG 2>&1");
    CB_CHECK_EQ(s.mounts.size(), 1u);
    CB_CHECK_EQ(s.mounts[0].drive, "c");
    CB_CHECK(fs::same_path(s.mounts[0].folder, f.dir.path()));
    CB_CHECK_EQ(s.artifacts.size(), 3u);
    CB_CHECK_EQ(std::get<std::string>(s.files.at("t.C")), "int f(void){return 1;}");
    CB_CHECK_EQ(s.timeout.count(), 180);
    CB_CHECK_EQ(s.cpu.at("cputype"), "386");
    CB_CHECK_EQ(s.cpu.at("cycles"), "max");
    CB_CHECK_EQ(s.dos.at("ver"), "6.22");
    CB_CHECK_EQ(s.variables.at("PATH"), "C:\\BIN");
    CB_CHECK_EQ(s.variables.at("INCLUDE"), "C:\\INCLUDE");
    CB_CHECK_EQ(s.variables.at("TMP"), "D:\\");
    // the variables keep the order they were given in
    std::vector<std::string> names;
    for (const auto& [k, v] : s.variables) names.push_back(k);
    CB_CHECK((names == std::vector<std::string>{"PATH", "INCLUDE", "LIB", "TMP"}));

    CB_CHECK(c.obj.has_value());
    CB_CHECK_EQ(c.obj->size(), 3u);
    CB_CHECK_EQ(c.cod.value(), "; listing \xC3\xA9\n");   // Latin-1 listing, UTF-8 text
    CB_CHECK_EQ(c.log.value(), "compiled\n");
    CB_CHECK(c.work.has_value());
});

CB_TEST("build.msvc152.parts_that_were_not_made_are_absent", [] {
    Fixture f;
    FakeRunner runner;   // the emulator ran and made nothing
    runner.result.work = f.dir.path() / "work";
    const msvc152::Compiled c = msvc152::compile_c(runner, f.tools, "x", "/c", "prog", std::chrono::seconds(5));
    CB_CHECK(!c.obj && !c.cod && !c.log);
    CB_CHECK_EQ(runner.seen.commands[0], "CL /c /Fcprog.COD /Foprog.OBJ prog.C > prog.LOG 2>&1");
    CB_CHECK_EQ(runner.seen.timeout.count(), 5);
    CB_CHECK(runner.seen.files.contains("prog.C"));
    const std::string report = msvc152::report(c);
    CB_CHECK(report.find("NO OBJ produced") != std::string::npos);
    CB_CHECK(report.find("(no log)") != std::string::npos);
});

CB_TEST("build.msvc152.refuses_what_would_become_a_command", [] {
    Fixture f;
    FakeRunner runner;
    for (const char* flags : {"/c & del *.*", "/c > x", "/c | y", "/c %PATH%", "/c\nformat"}) {
        CB_CHECK_THROWS_AS(UsageError, (void)msvc152::compile_c(runner, f.tools, "x", flags));
    }
    for (const char* stem : {"", "toolongname", "a b", "a&b", "..", "a.c"}) {
        CB_CHECK_THROWS_AS(UsageError, (void)msvc152::compile_c(runner, f.tools, "x", "/c", stem));
    }
    CB_CHECK_EQ(runner.calls, 0);   // nothing reached the emulator
    // tools that are not there
    msvc152::Tools missing = f.tools;
    missing.dosbox_x = f.dir.path() / "nope.exe";
    CB_CHECK_THROWS_AS(NotFoundError, (void)msvc152::compile_c(runner, missing, "x"));
    missing = f.tools;
    missing.msvc_root = f.dir.path() / "elsewhere";
    CB_CHECK_THROWS_AS(NotFoundError, (void)msvc152::compile_c(runner, missing, "x"));
});

CB_TEST("build.msvc152.report_shows_listing_size_and_work_folder", [] {
    msvc152::Compiled c;
    c.cod = std::string(5000, 'x');
    c.obj = Bytes(77);
    c.work = std::filesystem::path("C:/w");
    const std::string r = msvc152::report(c);
    CB_CHECK(r.starts_with("=== /Fc listing (codegen) ===\n"));
    CB_CHECK(r.find(std::string(3000, 'x') + "\n=== OBJ: 77 bytes ===\n") != std::string::npos);   // cut at 3000 bytes
    CB_CHECK(r.find(std::string(3001, 'x')) == std::string::npos);
    CB_CHECK(r.ends_with("# work dir: C:/w\n"));
});
