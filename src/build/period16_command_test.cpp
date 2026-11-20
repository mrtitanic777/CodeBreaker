// The `build16` command: the LZEXPAND reconstruction (tests/fixtures/lzexpand) built in DOSBox-X is the
// golden (build/testdata/build16): the command writes the same file, with and without the original's DOS stub in front. Skipped when the
// period tools are not set up (a machine without DOSBox-X, MS C 6 and VC++ 1.52).

#include <iostream>

#include "build/period16.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace {

using namespace cb;
namespace stdfs = std::filesystem;

struct P16Setup {
    config::Config config = config::current();
    stdfs::path fixture, original;
    bool ready = false;
};

P16Setup p16_setup() {
    P16Setup s;
    const base::Corpus corpus(s.config.products());
    const build::period16::Tools tools = build::period16::tools_from(s.config, corpus);
    const auto exists = [](const stdfs::path& p) {
        std::error_code ec;
        return !p.empty() && stdfs::exists(p, ec);
    };
    for (const stdfs::path& root : {s.config.checkout_root(), s.config.main_checkout()}) {
        if (s.fixture.empty() && exists(root / "tests" / "fixtures" / "lzexpand")) s.fixture = root / "tests" / "fixtures" / "lzexpand";
        if (s.original.empty() && exists(root / "state" / "mini" / "original" / "LZEXPAND.DLL")) s.original = root / "state" / "mini" / "original" / "LZEXPAND.DLL";
    }
    s.ready = exists(tools.dosbox_x) && exists(tools.msc6) && exists(tools.msvc152) && tools.win31_kit && !s.fixture.empty() && !s.original.empty();
    if (!s.ready) std::cerr << "build16: the period tools or the fixture are not here; skipped\n";
    return s;
}

int p16_run(const P16Setup& s, std::vector<std::string> args) {
    const cli::Cli cli("codebreaker", {build::period16::command({s.config, nullptr})});
    args.insert(args.begin(), "build16");
    return cli.run(args);
}

}  // namespace

CB_TEST("build.period16.command_writes_the_dll_as_recorded", [] {
    const P16Setup s = p16_setup();
    if (!s.ready) return;
    const fs::ScopedTempDir work("build16-test");
    CB_CHECK_EQ(p16_run(s, {fs::to_text(s.fixture), "--work", fs::to_text(work.path()), "--original", fs::to_text(s.original)}), 0);
    CB_CHECK(fs::read_file(work.path() / "lzexpand.dll") == fs::read_file(testing::data_path("build/testdata/build16/lzexpand_python.dll")));
});

CB_TEST("build.period16.command_puts_the_donors_stub_in_front", [] {
    const P16Setup s = p16_setup();
    if (!s.ready) return;
    const fs::ScopedTempDir work("build16-test");
    CB_CHECK_EQ(p16_run(s, {fs::to_text(s.fixture), "--work", fs::to_text(work.path()), "--original", fs::to_text(s.original), "--stub", fs::to_text(s.original)}), 0);
    CB_CHECK(fs::read_file(work.path() / "lzexpand.dll") == fs::read_file(testing::data_path("build/testdata/build16/lzexpand_python_stubbed.dll")));
});

CB_TEST("build.period16.command_fails_when_no_dll_is_made_and_on_a_missing_donor", [] {
    const P16Setup s = p16_setup();
    if (!s.ready) return;
    const fs::ScopedTempDir empty("build16-test");
    fs::write_text(empty.path() / "x.lnk", "nothing.obj\nnothing.dll\n");   // names a DLL no source makes
    const fs::ScopedTempDir work("build16-test");
    CB_CHECK_EQ(p16_run(s, {fs::to_text(empty.path()), "--work", fs::to_text(work.path())}), kExitFailed);
    CB_CHECK_THROWS_AS(NotFoundError, (void)p16_run(s, {fs::to_text(s.fixture), "--work", fs::to_text(work.path()), "--stub", "no-such-module.dll"}));
});
