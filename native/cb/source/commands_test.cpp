// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/commands: that every tool of the `vxd` and `ne-asm` commands is wired, that a command made from
// the settings does nothing until it runs, and the commands that need no tool of the machine. The commands run on real inputs in the
// report of the change (cb_source_commands).
#include "cb/source/commands.hpp"

#include <algorithm>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/source/ne_asm/run.hpp"
#include "cb/source/vxd/run.hpp"

namespace cm = cb::source::commands;

namespace {

std::vector<std::string> cm_names(const cb::base::commands::ToolTable& tools) {
    std::vector<std::string> names;
    for (const auto& [name, run] : tools) names.push_back(name);
    return names;
}

struct CmWorld {
    cb::fs::ScopedTempDir dir{"commandstest"};
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    cb::fs::ScopedEnv root{"CB_CORPUS_ROOT", cb::fs::to_text(dir.path() / "corpus")};
    cb::fs::ScopedEnv no_data{"WT_DATA_ROOT", std::nullopt};
    cb::fs::ScopedEnv state{"CB_STATE", cb::fs::to_text(dir.path() / "state")};
    std::unique_ptr<cm::Production> production;
    CmWorld() {
        cb::fs::make_dirs(dir.path() / "corpus");
        cb::config::Config config = cb::config::Config::load(dir.path());
        production = std::make_unique<cm::Production>(config, std::make_shared<cb::base::Corpus>(config.products()));
    }
};

}  // namespace

CB_TEST("source.commands.every_tool_of_the_vxd_command_is_wired", [] {
    const CmWorld world;
    const std::vector<std::string> wired = cm_names(world.production->vxd_tools());
    std::vector<std::string> listed;
    for (const std::string_view tool : cb::source::vxd::kVxdTools) listed.emplace_back(tool);
    std::sort(listed.begin(), listed.end());
    CB_CHECK(wired == listed);
    CB_CHECK(std::find(wired.begin(), wired.end(), "boot") != wired.end());
    CB_CHECK(std::find(wired.begin(), wired.end(), "loadable-corpus") != wired.end());
    CB_CHECK_EQ(wired.size(), std::size_t{11});
});

CB_TEST("source.commands.every_tool_of_the_ne_asm_command_is_wired", [] {
    const CmWorld world;
    const std::vector<std::string> wired = cm_names(world.production->ne_asm_tools());
    std::vector<std::string> listed;
    for (const std::string_view tool : cb::source::ne_asm::run::tool_names()) listed.emplace_back(tool);
    std::sort(listed.begin(), listed.end());
    CB_CHECK(wired == listed);
    for (const char* tool : {"idiom", "sweep", "recon", "segdiff", "reachability", "datacode", "segment", "fixups", "debug-names", "debug-names-estimate"})
        CB_CHECK(std::find(wired.begin(), wired.end(), tool) != wired.end());
});

CB_TEST("source.commands.a_command_names_its_tools_when_none_is_given", [] {
    const CmWorld world;
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(world.production->vxd()(std::vector<std::string>{}), cb::kExitUsage);
        const std::string said = cap.out();
        CB_CHECK(said.starts_with("vxd -- a virtual device driver as the source its authors wrote"));
        CB_CHECK(said.find("tools: boot, corpus, disassembly, finalize, from-source, idiom, link, loadable-corpus, masm, owbuild, tree\n") != std::string::npos);
    }
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(world.production->ne_asm()(std::vector<std::string>{"nonesuch"}), cb::kExitUsage);
        CB_CHECK(cap.out().find("tools: datacode, debug-names, debug-names-estimate, fixups, idiom, msvc152, reachability, recon, segdiff, segment, sweep\n") != std::string::npos);
    }
});

CB_TEST("source.commands.the_commands_that_take_what_they_need_when_they_run", [] {
    const CmWorld world;
    // none of these makes a tool, the DDK's tables or the lane kit until it runs
    const cb::cli::Command build = world.production->dos_build();
    CB_CHECK_EQ(build.name, "dos-build");
    CB_CHECK(!build.params.empty());
    const cb::source::commands::Runner source = world.production->driver_source();
    const cb::source::commands::Runner program = world.production->dos_source();
    CB_CHECK(static_cast<bool>(source));
    CB_CHECK(static_cast<bool>(program));
    // a tool that needs nothing of the machine: the finalizer's usage, with no arguments
    cb::testing::CapturedStd cap;
    CB_CHECK_EQ(world.production->vxd()(std::vector<std::string>{"finalize"}), cb::kExitUsage);
});
