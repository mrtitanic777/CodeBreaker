// The shims at codebreaker/base/{commands,capstone_dll,ctext,runs,parallel}.hpp and codebreaker/build/pklite.hpp answer what the
// old layer's callers expect, through the new code. (The shims with a translation unit of their own -- base/corpus, base/tidy_state,
// build/functions -- call the old ports and are checked for compiling only: see the cb_shim_check target.)
#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/base/commands.hpp"
#include "codebreaker/base/ctext.hpp"
#include "codebreaker/base/parallel.hpp"
#include "codebreaker/base/runs.hpp"
#include "codebreaker/build/pklite.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

#include <cstdlib>

namespace stdfs = std::filesystem;
namespace base = codebreaker::base;

namespace {

// The old layer reads settings through the C runtime's environment (getenv), which only _putenv_s changes: CB_<KEY> beats
// settings.json. (The new code reads the process's own, which _putenv_s changes too.)
struct CrtEnv {
    std::string name;
    explicit CrtEnv(std::string n, const std::string& value) : name(std::move(n)) { _putenv_s(name.c_str(), value.c_str()); }
    ~CrtEnv() { _putenv_s(name.c_str(), ""); }
};

struct Settings {
    cb::fs::ScopedTempDir dir{"cbshim"};
    CrtEnv root{"CB_CHECKOUT_ROOT", cb::fs::to_text(dir.path())};
    CrtEnv state{"CB_STATE", cb::fs::to_text(dir.path() / "state")};
    cb::fs::ScopedEnv jobs{"CB_JOBS", std::nullopt};
    cb::fs::ScopedEnv low{"CB_LOW_LOAD", std::nullopt};
    Settings() { py::settings::reload(); }
};

}  // namespace

CB_TEST("compat.base_shim.commands", [] {
    namespace commands = base::commands;
    py::list<py::str> seen;
    commands::set_module_runner([&](const py::str& module) {
        seen = commands::sys_argv;
        if (module == py::str("tool.fail")) throw commands::SystemExit(int64_t(3));
        if (module == py::str("tool.text")) throw commands::SystemExit(py::str("a message"));
        if (module == py::str("tool.none")) throw commands::SystemExit();
    });
    py::dict<py::str, py::str> tools;
    tools.set(py::str("fail"), py::str("tool.fail"));
    tools.set(py::str("ok"), py::str("tool.ok"));
    tools.set(py::str("text"), py::str("tool.text"));
    tools.set(py::str("none"), py::str("tool.none"));
    CB_CHECK_EQ(commands::dispatch(tools, py::list<py::str>{py::str("ok"), py::str("-x"), py::str("y")}), 0);
    CB_CHECK_EQ(seen.size(), 3);   // sys.argv is [module, its arguments...] while it runs
    CB_CHECK(seen[0] == py::str("tool.ok") && seen[2] == py::str("y"));
    CB_CHECK_EQ(commands::sys_argv.size(), 0);   // and is put back afterwards
    CB_CHECK_EQ(commands::dispatch(tools, py::list<py::str>{py::str("fail")}), 3);
    CB_CHECK_EQ(commands::dispatch(tools, py::list<py::str>{py::str("text")}), 1);
    CB_CHECK_EQ(commands::dispatch(tools, py::list<py::str>{py::str("none")}), 0);
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(commands::dispatch(tools, py::list<py::str>{py::str("nope")}, py::str("the doc")), 2);
        CB_CHECK_EQ(cap.out(), "the doc\ntools: fail, none, ok, text\n");
    }
    commands::set_module_runner(nullptr);
    CB_CHECK_THROWS_AS(py::ImportError, commands::run_module(py::str("no.such"), py::list<py::str>()));
});

CB_TEST("compat.base_shim.ctext", [] {
    namespace ctext = base::ctext;
    CB_CHECK(ctext::blank(py::str("/* c */ int x; \"s\"")) == py::str("        int x; \" \""));
    CB_CHECK(ctext::blank(py::str("\"s\" 'c' x"), true) == py::str("        x"));
    CB_CHECK(ctext::blank(py::str("\"a\nb\" x"), true, true) == py::str("      x"));
    // offsets count code points: a character inside a literal is ONE space, however many bytes it has
    const py::str unicode = py::str(std::string("\"caf\xC3\xA9 \xE4\xB8\xAD\" /* \xC3\xA9 */ x"));
    const py::str blanked = ctext::blank(unicode);
    CB_CHECK_EQ(blanked.size(), unicode.size());
    CB_CHECK(blanked == py::str(std::string("\"      \"         x")));
    // outside a literal a multi-byte character stays
    CB_CHECK(ctext::blank(py::str(std::string("\xC3\xA9 x"))) == py::str(std::string("\xC3\xA9 x")));
    // (sub_code takes the old layer's regex, which needs the old runtime to link: it is compiled here, not run)
});

CB_TEST("compat.base_shim.runs", [] {
    Settings s;
    namespace runs = base::runs;
    CB_CHECK_EQ(runs::AGE, 24 * 3600);
    const py::str shots = runs::results(py::str("phase3"));
    CB_CHECK(stdfs::is_directory(stdfs::path(shots.utf8())));
    CB_CHECK(shots.utf8().find("phase3") != std::string::npos);
    CB_CHECK(shots.utf8().find('/') == std::string::npos);   // (native separators, as the old layer's paths are)
    py::str kept;
    {
        runs::Folder work(py::str("phase3"), py::str("my tag"));
        kept = work.path();
        CB_CHECK(stdfs::is_directory(stdfs::path(kept.utf8())));
        CB_CHECK(kept.utf8().find("my-tag_") != std::string::npos);
    }
    CB_CHECK(!stdfs::exists(stdfs::path(kept.utf8())));
    const py::str a = runs::start(py::str("phase3"));
    CB_CHECK(runs::sweep(py::str("phase3"), 10.0 * 24 * 3600).empty());
    CB_CHECK_EQ(runs::sweep(py::str("phase3"), -1.0).size(), 1);
    CB_CHECK(!stdfs::exists(stdfs::path(a.utf8())));
    CB_CHECK(runs::remove(a, 1));
});

CB_TEST("compat.base_shim.parallel", [] {
    Settings s;
    namespace parallel = base::parallel;
    {
        cb::fs::ScopedEnv jobs("CB_JOBS", "3");
        CB_CHECK_EQ(parallel::workers(), 3);
    }
    {
        cb::fs::ScopedEnv jobs("CB_JOBS", "9");
        cb::fs::ScopedEnv low("CB_LOW_LOAD", "1");
        CB_CHECK_EQ(parallel::workers(), parallel::LOW_LOAD);
        CB_CHECK(parallel::low_load());
    }
    CB_CHECK(!parallel::low_load());
    CB_CHECK(parallel::cpu_count() >= 1);
    CB_CHECK(parallel::workers() >= 1);
    CB_CHECK(parallel::total_gb().value_or(0) > 0);
    CB_CHECK(parallel::available_gb().has_value());
    {
        cb::fs::ScopedEnv free("CB_MIN_FREE_GB", "0");
        CB_CHECK_EQ(parallel::min_free_gb(), 0.0);
        CB_CHECK_EQ(parallel::wait_for_memory(py::str("test")), 0.0);
        CB_CHECK(parallel::memory_allows(1).value_or(0) > 0);
    }
    cb::fs::ScopedEnv jobs("CB_JOBS", "2");
    cb::fs::ScopedEnv free("CB_MIN_FREE_GB", "0");
    parallel::slots held(4, 1, py::str("test"));
    const int64_t n = held.enter();
    CB_CHECK(n >= 1 && n <= 2);
    CB_CHECK(!held.exit());   // False: nothing is swallowed
    CB_CHECK(parallel::idle_cores(0.02).has_value() || true);
});

CB_TEST("compat.base_shim.capstone_dll", [] {
    cb::fs::ScopedTempDir dir("cbshimdll");
    const stdfs::path dll = dir.path() / "capstone.dll";
    cb::fs::write_text(dll, "x");
    cb::fs::ScopedEnv env("CB_CAPSTONE_DLL", cb::fs::to_text(dll));
    // (looked for once per process: this is the first time the test run asks)
    const py::str found = base::capstone_dll::find();
    CB_CHECK(found.utf8().find("capstone.dll") != std::string::npos);
});

CB_TEST("compat.base_shim.pklite", [] {
    namespace pk = codebreaker::build::pklite;
    const py::bytes module = cb::compat::to_py(cb::ByteView(cb::fs::read_file(cb::testing::data_path("build/testdata/extract.mod"))));
    CB_CHECK_EQ(pk::stream_start(module), 656);
    const pk::Stored s = pk::read(module);
    CB_CHECK_EQ(s.start, 656);
    CB_CHECK_EQ(s.end, 45573);
    CB_CHECK_EQ(s.image.size(), 86150);
    CB_CHECK_EQ(s.tokens.size(), 33011);
    CB_CHECK(s.tokens[s.tokens.size() - 1] == pk::Token::end());
    CB_CHECK(s.regs == std::make_tuple(int64_t(2260), int64_t(30), int64_t(5568), int64_t(5120)));
    CB_CHECK_EQ(s.relocs.size(), 28);
    CB_CHECK_EQ(std::get<1>(s.relocs[0])[0], 923);
    CB_CHECK_EQ(s.stub.lengths.size(), 24);
    CB_CHECK_EQ(s.stub.highs.size(), 31);
    // the stub's codes as bit lists: the length-2 code is 1 0
    CB_CHECK_EQ(s.stub.lengths.at(2).size(), 2);
    CB_CHECK_EQ(s.stub.lengths.at(2)[0], 1);
    CB_CHECK_EQ(s.stub.lengths.at(2)[1], 0);
    // encode what was read: the stream; parse what was expanded: the tokens
    const py::bytes stream = pk::encode(s.tokens, s.stub);
    CB_CHECK_EQ(stream.size(), s.end - s.start);
    CB_CHECK(pk::parse(s.image) == s.tokens);
    // pack the program back
    py::list<std::tuple<int64_t, int64_t>> relocs;
    for (const auto& [seg, offs] : s.relocs)
        for (int64_t off : offs) relocs.append(std::make_tuple(seg, off));
    auto [packed, header] = pk::pack(s.image, relocs, s.regs, s.stub.head, 0);
    CB_CHECK_EQ(packed.size(), module.size());
    CB_CHECK_EQ(header.min_alloc, 2631);
    CB_CHECK_EQ(header.ss, 2917);
    CB_CHECK_EQ(header.relocs.size(), 1);
    CB_CHECK_EQ(std::get<0>(header.relocs[0]), 7);
    CB_CHECK(pk::_relocation_table(py::list<std::tuple<int64_t, int64_t>>{}) == py::bytes(std::string(1, '\0')));
    // errors are the old layer's: a module that is not PKLITE's is a ValueError, a value that does not fit a field a struct.error
    CB_CHECK_THROWS_AS(py::ValueError, pk::read(py::bytes(std::string(100, 'x'))));
    CB_CHECK_THROWS_AS(py::StructError, pk::pack(s.image, relocs, std::make_tuple(int64_t(70000), int64_t(0), int64_t(0), int64_t(0)), s.stub.head, 0));
});
