// Tests of the milestone regression's logic (verify/regress): its rules -- the machines, the disk, the verdict -- with a builder and a
// machine that are made up. The real run (QEMU, the Setup CD, the builders) is `codebreaker regress all15`, which this machine's
// tests do not start.
#include "cb/verify/regress.hpp"

#include <atomic>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::verify;
using namespace cb::verify::regress;

namespace {

namespace rsfs = std::filesystem;

struct RegressFake {
    fs::ScopedTempDir dir{"cb-regress"};
    std::atomic<int> boots{0};
    std::atomic<int> at_once{0}, most_at_once{0};
    std::vector<std::string> built_names;
    std::vector<bool> all_assembly_seen;
    setup16::Verdict boot_verdict = setup16::Verdict::Pass;
    std::string fail_to_link;
    double free = 100;

    Services services() {
        Services s;
        s.work = dir.path() / "work";
        s.mini_original = dir.path() / "mini";
        s.setup_original = dir.path() / "setup";
        s.free_gb = [this] { return free; };
        s.build = [this](const std::vector<std::string>& names, const rsfs::path&, const rsfs::path& outdir, bool all_assembly) {
            all_assembly_seen.push_back(all_assembly);
            std::vector<Built> out;
            for (const std::string& n : names) {
                built_names.push_back(n);
                Built b;
                b.name = n;
                b.linked = n != fail_to_link;
                if (b.linked) {
                    b.image = outdir / n;
                    fs::write_text(b.image, "image of " + n);
                } else {
                    b.error = "did not link";
                }
                b.functions = 10;
                b.with_text = 8;
                out.push_back(b);
            }
            return out;
        };
        s.boot = [this](const setup16::Replace& replace, const std::string& tag) {
            const int now = ++at_once;
            int seen = most_at_once.load();
            while (now > seen && !most_at_once.compare_exchange_weak(seen, now)) {
            }
            CB_CHECK(!replace.empty());
            for (const auto& [name, image] : replace) CB_CHECK(rsfs::exists(image));   // (the built images are there while a machine runs)
            ++boots;
            --at_once;
            return setup16::Outcome{boot_verdict, dir.path() / (tag + ".png")};
        };
        return s;
    }
};

bool has(const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; }

}  // namespace

CB_TEST("verify.regress.all15_builds_the_fifteen_and_boots_twice", [] {
    RegressFake f;
    const Report r = run(Tier::All15, f.services(), {});
    CB_CHECK(r.pass);
    CB_CHECK_EQ(f.built_names.size(), std::size_t{15});
    CB_CHECK_EQ(f.built_names.front(), "KRNL386.EXE");
    CB_CHECK_EQ(f.boots.load(), 2);
    CB_CHECK_EQ(r.boots.size(), std::size_t{2});
    CB_CHECK_EQ(f.most_at_once.load(), 1);   // (one machine at a time unless --vms says more)
    CB_CHECK(has(r.text(), "PASS"));
    CB_CHECK(has(r.text(), "regress-all15-2"));
    // the run's copies are gone: the built images do not outlive it
    CB_CHECK(rsfs::is_empty(f.dir.path() / "work"));
});

CB_TEST("verify.regress.one_failed_boot_fails_the_run", [] {
    RegressFake f;
    f.boot_verdict = setup16::Verdict::FailHang;
    const Report r = run(Tier::All15, f.services(), {});
    CB_CHECK(!r.pass);
    CB_CHECK_EQ(r.boots.size(), std::size_t{2});
    CB_CHECK(has(r.text(), "FAIL(hang)"));
});

CB_TEST("verify.regress.a_module_that_does_not_link_is_not_booted", [] {
    RegressFake f;
    f.fail_to_link = "USER.EXE";
    const Report r = run(Tier::All15, f.services(), {});
    CB_CHECK(!r.pass);
    CB_CHECK_EQ(f.boots.load(), 0);
    CB_CHECK(has(r.text(), "NOT LINKED"));
    CB_CHECK(has(r.text(), "nothing was booted"));
});

CB_TEST("verify.regress.setup_adds_the_setup_programs_built_as_assembly", [] {
    RegressFake f;
    const Report r = run(Tier::Setup, f.services(), {.runs = 1});
    CB_CHECK(r.pass);
    CB_CHECK_EQ(f.built_names.size(), std::size_t{22});
    CB_CHECK_EQ(f.all_assembly_seen.size(), std::size_t{2});
    CB_CHECK(!f.all_assembly_seen[0]);
    CB_CHECK(f.all_assembly_seen[1]);
    CB_CHECK_EQ(f.built_names.back(), "SETUP.EXE");
});

CB_TEST("verify.regress.the_machines_are_at_most_three", [] {
    RegressFake f;
    CB_CHECK_THROWS_AS(UsageError, run(Tier::All15, f.services(), {.vms = 4}));
    CB_CHECK_THROWS_AS(UsageError, run(Tier::All15, f.services(), {.vms = 0}));
    const Report r = run(Tier::All15, f.services(), {.runs = 6, .vms = 3});
    CB_CHECK(r.pass);
    CB_CHECK_EQ(f.boots.load(), 6);
    CB_CHECK(f.most_at_once.load() <= 3);
});

CB_TEST("verify.regress.nothing_starts_on_a_full_disk_or_without_the_tools", [] {
    RegressFake f;
    f.free = 12;
    const Report r = run(Tier::All15, f.services(), {});
    CB_CHECK(!r.pass);
    CB_CHECK(has(r.refused, "12.0 GB free"));
    CB_CHECK_EQ(f.boots.load(), 0);
    CB_CHECK(f.built_names.empty());
    f.free = 100;
    Services s = f.services();
    s.missing = [](Tier) { return std::vector<std::string>{"qemu (setting qemu) (C:/nowhere/qemu.exe)"}; };
    const Report r2 = run(Tier::All15, s, {});
    CB_CHECK(!r2.pass);
    CB_CHECK(has(r2.text(), "NOT RUN: missing: qemu"));
    CB_CHECK(f.built_names.empty());
});

CB_TEST("verify.regress.only_rebuilds_the_named_modules_and_says_it_is_not_the_regression", [] {
    RegressFake f;
    const Report r = run(Tier::All15, f.services(), {.runs = 1, .only = {"user.exe"}});
    CB_CHECK_EQ(f.built_names.size(), std::size_t{1});
    CB_CHECK(has(r.text(), "not the regression"));
    CB_CHECK_THROWS_AS(UsageError, run(Tier::All15, f.services(), {.only = {"NOTEPAD.EXE"}}));
    CB_CHECK_THROWS_AS(UsageError, run(Tier::All15, f.services(), {.only = {"SUWIN.EXE"}}));   // (a Setup program is not of all15)
});

CB_TEST("verify.regress.keep_built_keeps_the_images", [] {
    RegressFake f;
    const rsfs::path keep = f.dir.path() / "kept";
    const Report r = run(Tier::All15, f.services(), {.runs = 1, .keep_built = keep});
    CB_CHECK(r.pass);
    CB_CHECK(rsfs::exists(keep / "GDI.EXE"));
    CB_CHECK(rsfs::exists(keep / "WIN87EM.DLL"));
});

CB_TEST("verify.regress.slow_cases_pass_on_exit_and_output_and_skip_what_is_not_there", [] {
    RegressFake f;
    const rsfs::path cases = f.dir.path() / "slow.json";
    fs::write_text(cases, R"({"cases":[
        {"name":"works","argv":["a","{package}"],"package_setting":"pkg","expect_contains":["checked"]},
        {"name":"no-package","argv":["b"],"package_setting":"missing_one"},
        {"name":"wrong-exit","argv":["c"],"expect_exit":3}]})");
    const rsfs::path exe = f.dir.path() / "codebreaker.exe";
    fs::write_text(exe, "x");
    Services s = f.services();
    std::vector<std::vector<std::string>> ran;
    s.setting_path = [&](const std::string& key) -> std::optional<rsfs::path> { return key == "pkg" ? std::optional<rsfs::path>(f.dir.path()) : std::nullopt; };
    s.run_exe = [&](const std::vector<std::string>& argv, std::chrono::minutes) {
        ran.push_back(argv);
        return std::pair<int, std::string>{0, "all checked\n"};
    };
    const Report r = run(Tier::Slow, s, {.cases = cases, .exe = exe});
    CB_CHECK_EQ(r.cases.size(), std::size_t{3});
    CB_CHECK_EQ(r.cases[0].status, "PASS");
    CB_CHECK_EQ(r.cases[1].status, "SKIPPED");
    CB_CHECK_EQ(r.cases[2].status, "FAIL");   // exit 0 where 3 was expected
    CB_CHECK(has(r.cases[2].why, "exit 0 (wanted 3)"));
    CB_CHECK(!r.pass);
    CB_CHECK_EQ(ran.size(), std::size_t{2});
    CB_CHECK_EQ(ran[0][2], fs::to_text(f.dir.path()));   // {package} filled from the setting
    // without a program or a file the run says so and runs nothing
    const Report none = run(Tier::Slow, s, {.cases = cases});
    CB_CHECK(has(none.refused, "--exe"));
    CB_CHECK(!none.pass);
});

CB_TEST("verify.regress.tier_names", [] {
    CB_CHECK(tier_named("ALL15") == Tier::All15);
    CB_CHECK(tier_named("setup") == Tier::Setup);
    CB_CHECK(!tier_named("all"));
    CB_CHECK_EQ(all15_names().size(), std::size_t{15});
    CB_CHECK_EQ(setup_names().size(), std::size_t{7});
});
