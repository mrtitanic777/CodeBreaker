// Tests of the exact command: its arguments and what it answers to each -- no arguments, -h, --toolchains (against what the live Python's exact/run.py
// listed of the profiles of the checkout: testdata/w5b3.golden.json), a project and a stage, no stage (the ledger), an unknown stage, a project that is
// not there -- and the standard table of stages driven from a real project (the carve stage against the Python's text of it).
//
// DIVERGES from the Python: the stages are a table the caller gives (`Stages`), not a list of modules imported by name; --toolchains lists the profiles
// without loading their tools, so a plugin's profile is listed whether its plugin is registered or not.
#include "cb/exact/run.hpp"

#include "cb/exact/real_projects.hpp"

using namespace cb;
using namespace cb::exact;
using test::check;
using test::golden;

CB_TEST("exact.run.the_arguments_that_are_the_commands_own", [] {
    test::Real real;
    run::Stages stages;
    toolchain::Locations locations;
    locations.profiles = testing::data_path("../../data/exact/toolchains");
    toolchain::Toolchains toolchains(locations, nullptr, nullptr);
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run::main({}, stages, toolchains, nullptr), 2);   // none: the usage, and a code that says it was not a command
        CB_CHECK(cap.out().find("codebreaker exact PROJECT STAGE [options]") != std::string::npos);
    }
    for (const char* flag : {"-h", "--help"}) {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run::main({flag, "x"}, stages, toolchains, nullptr), 0);
        CB_CHECK_EQ(cap.out(), run::usage() + "\n");
    }
    CB_CHECK(run::usage().find("link        the program linked from its sources") != std::string::npos);
    // --toolchains: every profile there is, with its title and its variants
    testing::CapturedStd cap;
    CB_CHECK_EQ(run::main({"--toolchains"}, stages, toolchains, nullptr), golden().at("run_toolchains").at(1).get<int>());
    check(cap.out() == golden().at("run_toolchains").at(0).get<std::string>(), "--toolchains");
    CB_CHECK(cap.out().find("msc300") != std::string::npos);
    // --compile wants a profile and a file
    CB_CHECK_THROWS_AS(UsageError, (void)run::main({"--compile", "msc300"}, stages, toolchains, nullptr));
    CB_CHECK_THROWS_AS(NotFoundError, (void)run::main({"--compile", "no-such-profile", "x.c"}, stages, toolchains, nullptr));
});

CB_TEST("exact.run.a_project_and_a_stage", [] {
    test::Real real;
    if (test::skipped(real, "run stages")) return;
    run::Stages stages;
    std::vector<std::string> seen;
    std::string project_dir;
    stages.add("echo", [&](project::Project& p, const std::vector<std::string>& args) {
        seen = args;
        project_dir = fs::to_text(p.dir());
        return 7;
    });
    stages.add("ledger", [&](project::Project&, const std::vector<std::string>& args) {
        seen = args;
        return 3;
    });
    const std::string p0 = fs::to_text(test::research_root() / "msc-3.0/recon/p0");
    CB_CHECK_EQ(run::main({p0, "echo", "a", "-b", "--c"}, stages, *real.toolchains, real.services), 7);
    CB_CHECK(seen == (std::vector<std::string>{"a", "-b", "--c"}));
    CB_CHECK(project_dir.ends_with("msc-3.0/recon/p0"));
    CB_CHECK_EQ(run::main({p0}, stages, *real.toolchains, real.services), 3);   // no stage: the ledger
    CB_CHECK(seen.empty());
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run::main({p0, "nope"}, stages, *real.toolchains, real.services), 2);
        CB_CHECK_EQ(cap.out(), "no stage nope (stages: echo, ledger)\n");
    }
    CB_CHECK_THROWS_AS(NotFoundError, (void)run::main({"no/such/project", "echo"}, stages, *real.toolchains, real.services));
    // a stage added again replaces the one of its name
    stages.add("echo", [](project::Project&, const std::vector<std::string>&) { return 9; });
    CB_CHECK_EQ(run::main({p0, "echo"}, stages, *real.toolchains, real.services), 9);
    CB_CHECK(stages.names() == (std::vector<std::string>{"echo", "ledger"}));
    CB_CHECK(stages.find("none") == nullptr);
});

CB_TEST("exact.run.the_standard_stages_are_driven_from_the_project", [] {
    test::Real real;
    const run::Stages standard = run::standard_stages();
    CB_CHECK(standard.names() == (std::vector<std::string>{"ledger", "library", "carve", "modules", "bss", "data", "repair", "names", "identify", "bench"}));
    struct Dos final : linkwhole::DosHost {
        std::string run(const std::filesystem::path&, const std::filesystem::path&, std::string_view, std::optional<std::uint32_t>) const override { return ""; }
    } dos;
    CB_CHECK(run::standard_stages(&dos).find("link") != nullptr);
    CB_CHECK(standard.find("link") == nullptr);
    if (test::skipped(real, "run standard")) return;
    const Json& want = golden().at("carve").at("msc-3.0/recon/p0");
    testing::CapturedStd cap;
    CB_CHECK_EQ(run::main({fs::to_text(test::research_root() / "msc-3.0/recon/p0"), "carve", "-v"}, standard, *real.toolchains, real.services), 0);
    check(cap.out() == want.at("main").at(0).get<std::string>(), "the carve stage through the command");
});
