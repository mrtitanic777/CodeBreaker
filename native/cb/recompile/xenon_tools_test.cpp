// Tests of the Xbox 360 preparation commands (cb/recompile/xenon_tools.hpp) on a real Xbox 360 program (LoganMinecraftTest.xex of C:/Nexia360/Games:
// each test is skipped when it or the xex program is not on this machine), against the recorded output for the same program
// (testdata/xenon_tools_goldens.txt): the size and SHA-1 of every file the three commands write.
//
// `switch-tables` also runs XenonAnalyse from the built XenonRecomp tree (native/cb/recompile/xenonrecomp/build under the checkout the configuration names; the main
// checkout's when the tests run in a worktree); it is skipped when that is not built, so no test builds XenonRecomp.
//
// A missing .xex is an error.
#include "cb/recompile/xenon_tools.hpp"

#include <string>
#include <vector>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/command_test_kit.hpp"

namespace xenon_tools_test {

using namespace cb;
namespace stdfs = std::filesystem;
namespace xt = cb::recompile::xenon_tools;

// The configuration of a test: a state folder of its own, XenonRecomp from the main checkout. ok is false when the title is not on this machine.
struct Setup {
    fs::ScopedTempDir dir{"cb-xenon-tools"};
    std::optional<fs::ScopedEnv> state;
    xt::Environment environment;
    std::optional<stdfs::path> xex;
    bool analyse_built = false;
    bool ok = false;

    Setup() {
        xex = testing::game_file("LoganMinecraftTest/LoganMinecraftTest.xex");
        if (!xex) return;
        const stdfs::path checkout = testing::data_path("../..");
        const stdfs::path main_checkout = config::Config::load(checkout).main_checkout();
        state.emplace("CB_STATE", fs::to_text(dir.path() / "state"));
        environment.config = config::Config::load(main_checkout);
        std::error_code ec;
        analyse_built = stdfs::is_regular_file(main_checkout / "native" / "cb" / "recompile" / "xenonrecomp" / "build" / "XenonAnalyse" / "XenonAnalyse.exe", ec);
        ok = true;
    }
};

std::string golden(const std::string& name) {
    const std::string log = fs::read_text(testing::data_path("recompile/testdata/xenon_tools_goldens.txt"));
    std::string out;
    bool inside = false;
    for (const std::string_view line : text::lines(log)) {
        if (line.starts_with("== ")) inside = line.substr(3) == name;
        else if (inside) out += std::string(line) + "\n";
    }
    CB_CHECK(!out.empty());
    return out;
}

// "file <rel> <size> <sha1>" lines of the golden, as {rel: "size sha1"}.
std::string golden_file(const std::string& name, const std::string& rel) {
    for (const std::string_view line : text::lines(golden(name)))
        if (line.starts_with("file " + rel + " ")) return std::string(line.substr(5 + rel.size() + 1));
    testing::record_failure(__FILE__, __LINE__, "no file " + rel + " in the golden " + name);
    return {};
}

// The size and SHA-1 of a file. A configuration names the module that wrote it in its first line; the golden's name for it (its source file) is put back (as the other
// recompile tests do).
std::string described(const stdfs::path& file) {
    Bytes data = fs::read_file(file);
    const std::string whole(as_text(data));
    if (whole.starts_with("# XenonRecomp configuration")) {
        const std::string fixed = text::replace_first(whole, "(recompile/config)", "(codebreaker/recompile/config.py)");
        data.assign(fixed.begin(), fixed.end());
    }
    return std::format("{} {}", data.size(), crypto::to_hex(crypto::sha1(data)));
}

std::string forward(std::string s) { return text::replace_all(s, "\\", "/"); }

// What the command prints (nothing, for these) and its exit code.
std::string run_one(const cli::Command& command, std::vector<std::string> args) { return testing::run_command(command, std::move(args)); }

CB_TEST("recompile.xenon_tools.xex_basic_writes_the_recorded_output", [] {
    Setup s;
    if (!s.ok) return;
    const std::string w = forward(fs::to_text(s.dir.path()));
    CB_CHECK_EQ(run_one(xt::xex_basic_command(s.environment), {fs::to_text(*s.xex), w + "/basic/default.xex"}), "exit=0\n");
    CB_CHECK_EQ(described(s.dir.path() / "basic" / "default.xex"), golden_file("xex_basic", "basic/default.xex"));
    CB_CHECK(!stdfs::exists(s.dir.path() / "basic" / "default.xex.part"));
});

CB_TEST("recompile.xenon_tools.xenon_config_writes_the_recorded_output", [] {
    Setup s;
    if (!s.ok) return;
    const std::string w = forward(fs::to_text(s.dir.path()));
    CB_CHECK_EQ(run_one(xt::xex_basic_command(s.environment), {fs::to_text(*s.xex), w + "/basic/default.xex"}), "exit=0\n");
    CB_CHECK_EQ(run_one(xt::xenon_config_command(s.environment),
                        {w + "/basic/default.xex", w + "/cfg.toml", "--out-dir", "ppc", "--file-path", "default.xex"}),
                "exit=0\n");
    CB_CHECK_EQ(described(s.dir.path() / "cfg.toml"), golden_file("xenon_config", "cfg.toml"));
    // the default for --file-path is the .xex as given
    CB_CHECK_EQ(run_one(xt::xenon_config_command(s.environment), {w + "/basic/default.xex", w + "/cfg_default.toml"}), "exit=0\n");
    const std::string text_written = fs::read_text(s.dir.path() / "cfg_default.toml");
    CB_CHECK(text_written.find("file_path = \"" + w + "/basic/default.xex\"") != std::string::npos);
    CB_CHECK(text_written.find("out_directory_path = \"ppc\"") != std::string::npos);
    CB_CHECK(text_written.find("switch_table_file_path = \"switch_tables.toml\"") != std::string::npos);
});

CB_TEST("recompile.xenon_tools.switch_tables_write_the_finders_and_xenon_analyses_and_the_config_reads_them", [] {
    Setup s;
    if (!s.ok || !s.analyse_built) return;
    const std::string w = forward(fs::to_text(s.dir.path()));
    CB_CHECK_EQ(run_one(xt::xex_basic_command(s.environment), {fs::to_text(*s.xex), w + "/basic/default.xex"}), "exit=0\n");
    CB_CHECK_EQ(run_one(xt::switch_tables_command(s.environment), {w + "/basic/default.xex", w + "/basic/switch_tables.toml"}), "exit=0\n");
    CB_CHECK_EQ(described(s.dir.path() / "basic" / "switch_tables.toml"), golden_file("switch_tables", "basic/switch_tables.toml"));
    CB_CHECK_EQ(described(s.dir.path() / "basic" / "switch_tables.analysed.toml"), golden_file("switch_tables", "basic/switch_tables.analysed.toml"));
    // a configuration made next to those tables is another (its functions come from them)
    CB_CHECK_EQ(run_one(xt::xenon_config_command(s.environment),
                        {w + "/basic/default.xex", w + "/cfg2.toml", "--out-dir", "ppc.new", "--switch-tables", "switch_tables.toml", "--file-path", "default.xex"}),
                "exit=0\n");
    CB_CHECK_EQ(described(s.dir.path() / "cfg2.toml"), golden_file("xenon_config_with_tables", "cfg2.toml"));
});

CB_TEST("recompile.xenon_tools.bad_arguments_and_missing_files_are_errors", [] {
    Setup s;
    if (!s.ok) return;
    const std::string w = forward(fs::to_text(s.dir.path()));
    CB_CHECK_THROWS_AS(Error, (void)run_one(xt::xenon_config_command(s.environment), {w + "/nothing.xex", w + "/o.toml"}));
    CB_CHECK_THROWS_AS(Error, (void)run_one(xt::switch_tables_command(s.environment), {w + "/nothing.xex", w + "/o.toml"}));
    CB_CHECK_THROWS_AS(Error, (void)run_one(xt::xex_basic_command(s.environment), {w + "/nothing.xex", w + "/o.xex"}));
    CB_CHECK(!stdfs::exists(s.dir.path() / "o.toml"));
    CB_CHECK_EQ(xt::commands(s.environment).size(), std::size_t{3});
});

}  // namespace xenon_tools_test
