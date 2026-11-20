// Tests of the Xbox 360 recompiler's commands (cb/recompile/command.hpp) on a real Xbox 360 program (LoganMinecraftTest.xex under
// C:/Nexia360/Games: each test is skipped when it, the xex program, or -- for the Xbox 360 runtime -- the built XenonRecomp is not on this
// machine), against the recorded output on the same program (testdata/command/):
//
//   recompile --at 0x82060088 --size 0x100        at_82060088.cpp
//   recompile --all --limit 20                    all20.cpp (a5c4cd03...) and all20_init.cpp (52460ece...), by SHA-1
//   recompile-project --runtime rexglue           rex_manifest.txt: every file of the project less the copied rexglue sources
//   recompile-project (the Xbox 360 runtime)      x360_manifest.txt: every file of the project less the machine's paths (the Build script and
//                                                 the CMakeLists name folders of this machine) and the XenonRecomp log
//   xenon-recompile <config.toml>                 "38 files generated ... 12 warning lines" of the same configuration
//
// Whole-program output was also compared file by file with the reference run (the 8561 functions of `recompile --all`, the 1163 files of the rexglue
// project, the 47 of the Xbox 360 one): identical but for the line ends (LF here) and the provenance comment line
// of three files (CMakeLists.txt, src/logan.toml, src/module_info.cpp name the module that wrote them).
//
// `--at` with an address outside the code is a usage error.
#include "cb/recompile/command.hpp"

#include <algorithm>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/command_test_kit.hpp"

using namespace cb;
namespace rcmd = cb::recompile::command;

namespace {

// A manifest line per file ("path size sha1"), the files of `folder` less those whose path holds one of `skip`.
std::string rc_manifest(const std::filesystem::path& folder, const std::vector<std::string>& skip) {
    std::vector<std::string> lines;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(folder)) {
        if (!entry.is_regular_file()) continue;
        const std::string rel = std::filesystem::relative(entry.path(), folder).generic_string();
        if (std::any_of(skip.begin(), skip.end(), [&](const std::string& s) { return rel.find(s) != std::string::npos; })) continue;
        const Bytes data = fs::read_file(entry.path());
        lines.push_back(std::format("{} {} {}\n", rel, data.size(), crypto::to_hex(crypto::sha1(data))));
    }
    std::sort(lines.begin(), lines.end());
    std::string out;
    for (const std::string& line : lines) out += line;
    return out;
}

std::string rc_sha1(const std::filesystem::path& file) { return crypto::to_hex(crypto::sha1(fs::read_file(file))); }

// The environment of a test: a state folder and a Converted folder of its own.
struct RcSetup {
    fs::ScopedTempDir dir{"cb-recompile-command"};
    std::optional<fs::ScopedEnv> state, converted;
    rcmd::Environment environment;
    std::optional<std::filesystem::path> program;
    bool ok = false;

    explicit RcSetup(const std::filesystem::path& checkout = testing::data_path("../..")) {
        program = testing::game_file("LoganMinecraftTest/LoganMinecraftTest.xex");
        if (!program) return;
        state.emplace("CB_STATE", fs::to_text(dir.path() / "state"));
        converted.emplace("CB_CONVERTED", fs::to_text(dir.path() / "Converted"));
        environment.config = config::Config::load(checkout);
        ok = true;
    }
};

// What a command prints, and its exit code.
struct RcRun {
    std::string out;
    int code = 0;
};
RcRun rc_run(int (*command)(std::span<const std::string>, const rcmd::Environment&), const rcmd::Environment& env, const std::vector<std::string>& args) {
    testing::CapturedStd cap;
    const int code = command(args, env);
    flush_out();
    return {cap.out(), code};
}

std::string rc_golden(const char* name) { return fs::read_text(testing::data_path(std::string("recompile/testdata/command/") + name)); }

}  // namespace

CB_TEST("recompile.command.one_function_matches_the_golden", [] {
    RcSetup setup;
    if (!setup.ok) return;
    const std::string out = fs::to_text(setup.dir.path()) + "/at.cpp";
    const RcRun r = rc_run(rcmd::recompile, setup.environment, {fs::to_text(*setup.program), "--at", "0x82060088", "--size", "0x100", "-o", out});
    CB_CHECK_EQ(r.code, 0);
    CB_CHECK_EQ(r.out, "sub_82060088: 63/64 instructions translated, 1 not, 1 labels, 3 calls\n");
    CB_CHECK_EQ(fs::read_text(fs::from_text(out)), rc_golden("at_82060088.cpp"));
});

CB_TEST("recompile.command.the_first_functions_match_the_golden", [] {
    RcSetup setup;
    if (!setup.ok) return;
    const std::string out = fs::to_text(setup.dir.path()) + "/all20.cpp";
    const RcRun r = rc_run(rcmd::recompile, setup.environment, {fs::to_text(*setup.program), "--all", "--limit", "20", "-o", out});
    CB_CHECK_EQ(r.code, 0);
    CB_CHECK_EQ(r.out, "20 functions (5492 found, 0 tail-jump entries), 1747/1749 instructions translated (99.9%), 2 not\nwrote " + out + " and its mapping table\n");
    CB_CHECK_EQ(rc_sha1(fs::from_text(out)), "a5c4cd034b4b7e67a476fdaae266c00d657d4584");
    CB_CHECK_EQ(rc_sha1(fs::from_text(fs::to_text(setup.dir.path()) + "/all20_init.cpp")), "52460ece25194143a103da130eadf9db81492c43");
});

CB_TEST("recompile.command.without_an_output_the_project_folder_is_used", [] {
    RcSetup setup;
    if (!setup.ok) return;
    const RcRun r = rc_run(rcmd::recompile, setup.environment, {fs::to_text(*setup.program), "--all", "--limit", "2"});
    CB_CHECK_EQ(r.code, 0);
    const std::filesystem::path root = setup.dir.path() / "Converted";
    std::size_t written = 0;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(root))
        if (entry.is_regular_file() && entry.path().extension() == ".cpp") ++written;
    CB_CHECK_EQ(written, std::size_t{2});   // the code and its mapping table, in the project's src/
    bool recorded = false;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(root))
        if (entry.path().filename() == "project.json") recorded = fs::read_text(entry.path()).find("\"recompile\"") != std::string::npos;
    CB_CHECK(recorded);
});

CB_TEST("recompile.command.misuse", [] {
    RcSetup setup;
    if (!setup.ok) return;
    CB_CHECK_EQ(rc_run(rcmd::recompile, setup.environment, {}).code, 2);
    CB_CHECK_EQ(rc_run(rcmd::recompile, setup.environment, {fs::to_text(*setup.program)}).code, 2);   // nothing says what to recompile
    const RcRun outside = rc_run(rcmd::recompile, setup.environment, {fs::to_text(*setup.program), "--at", "0x1000"});
    CB_CHECK_EQ(outside.code, 2);
    const std::filesystem::path junk = setup.dir.path() / "junk.bin";
    fs::write_file(junk, Bytes(64, 0x41));
    CB_CHECK_THROWS_AS(FormatError, rc_run(rcmd::recompile, setup.environment, {fs::to_text(junk), "--all"}));
    CB_CHECK_THROWS_AS(NotFoundError, rc_run(rcmd::recompile, setup.environment, {fs::to_text(setup.dir.path() / "missing.xex"), "--all"}));
});

CB_TEST("recompile.command.the_rexglue_project_matches_the_golden", [] {
    RcSetup setup;
    if (!setup.ok) return;
    const std::filesystem::path out = setup.dir.path() / "project";
    const RcRun r = rc_run(rcmd::recompile_project, setup.environment, {fs::to_text(*setup.program), "-o", fs::to_text(out), "--runtime", "rexglue", "-j", "4"});
    CB_CHECK_EQ(r.code, 0);
    std::filesystem::path preferred = out;
    const std::string dir = preferred.make_preferred().string();   // (the command writes paths as the platform does)
    CB_CHECK_EQ(r.out, "LoganMinecraftTest: 8561 functions in 33 source files, 99.9% of 749758 instructions translated, 110 imports\n  -> " + dir +
                           std::format("\n  build: cmake -S \"{0}\" -B \"{0}/build\" && cmake --build \"{0}/build\"\n", dir));
    // every file of the project, less the rexglue sources it carries a copy of
    CB_CHECK_EQ(rc_manifest(out, {"kernel/rex_kernel", "graphics/rex_dxbc"}), rc_golden("rex_manifest.txt"));
});

CB_TEST("recompile.command.the_xbox_360_project_matches_the_golden", [] {
    // the program is XenonRecomp's: built from native/xenonrecomp (`xenon-recompile` builds it when it is not)
    const std::filesystem::path checkout = config::find_checkout();
    std::error_code ec;
    if (!std::filesystem::is_regular_file(checkout / "native" / "xenonrecomp" / "build" / "XenonRecomp" / "XenonRecomp.exe", ec)) return;
    RcSetup setup(checkout);
    if (!setup.ok) return;
    const std::filesystem::path out = setup.dir.path() / "project";
    const RcRun r = rc_run(rcmd::recompile_project, setup.environment, {fs::to_text(*setup.program), "-o", fs::to_text(out), "--name", "logan"});
    CB_CHECK_EQ(r.code, 0);
    std::filesystem::path preferred = out;
    const std::string dir = preferred.make_preferred().string();   // (the command writes paths as the platform does)
    CB_CHECK(r.out.starts_with("logan: 5861 functions, 91 switch tables, 0 unimplemented instructions\n  -> " + dir));
    CB_CHECK_EQ(rc_manifest(out, {"ppc.new", "reports/", "Build-", "CMakeLists.txt"}), rc_golden("x360_manifest.txt"));
    // XenonRecomp again on the title's configuration: its warnings, and the files it generated
    const RcRun again = rc_run(rcmd::xenon_recompile, setup.environment, {fs::to_text(out / "src" / "logan.toml")});
    CB_CHECK_EQ(again.code, 0);
    CB_CHECK(again.out.ends_with("27 files generated, 27 changed, 0 stale removed; 12 warning lines\n"));
    // and the same synced into a folder that has them: nothing changed
    const RcRun third = rc_run(rcmd::xenon_recompile, setup.environment, {fs::to_text(out / "src" / "logan.toml")});
    CB_CHECK(third.out.ends_with("27 files generated, 0 changed, 0 stale removed; 12 warning lines\n"));
});

CB_TEST("recompile.command.xenon_recompile_misuse", [] {
    rcmd::Environment env;
    env.config = config::Config::load(testing::data_path("../.."));
    fs::ScopedTempDir dir{"cb-recompile-command"};
    CB_CHECK_EQ(rc_run(rcmd::xenon_recompile, env, {}).code, 2);
    CB_CHECK_EQ(rc_run(rcmd::xenon_recompile, env, {"a.toml", "b.toml"}).code, 2);
});
