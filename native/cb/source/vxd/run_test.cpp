// Tests of cb/source/vxd/run: the usage text, the tool list, and the dispatch to a registered tool.
#include "cb/source/vxd/run.hpp"

#include <algorithm>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace vxd = cb::source::vxd;

CB_TEST("source.vxd.run.usage_lists_every_tool", [] {
    for (const std::string_view tool : vxd::kVxdTools) {
        // each tool is described in the text under the name the command line uses
        CB_CHECK(vxd::kVxdUsage.find("    " + std::string(tool) + " ") != std::string_view::npos);
    }
    CB_CHECK(vxd::kVxdUsage.starts_with("vxd -- "));
    CB_CHECK(std::is_sorted(vxd::kVxdTools.begin(), vxd::kVxdTools.end()) == false);   // in the order the usage lists them
    CB_CHECK_EQ(vxd::kVxdTools.size(), std::size_t{11});
});

CB_TEST("source.vxd.run.dispatches_to_a_registered_tool", [] {
    std::vector<std::string> seen;
    cb::base::commands::ToolTable tools;
    tools["masm"] = [&](std::span<const std::string> args) {
        seen.assign(args.begin(), args.end());
        return 7;
    };
    const std::vector<std::string> line = {"masm", "driver.vxd", "out"};
    CB_CHECK_EQ(vxd::run(line, tools), 7);
    CB_CHECK(seen == (std::vector<std::string>{"driver.vxd", "out"}));
});

CB_TEST("source.vxd.run.no_tool_or_an_unknown_one_prints_the_usage", [] {
    cb::base::commands::ToolTable tools;
    tools["tree"] = [](std::span<const std::string>) { return 0; };
    tools["idiom"] = [](std::span<const std::string>) { return 0; };
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(vxd::run({}, tools), cb::kExitUsage);
        const std::string out = cap.out();
        CB_CHECK(out.starts_with(std::string(vxd::kVxdUsage) + "\ntools: idiom, tree\n"));   // sorted by name
    }
    {
        cb::testing::CapturedStd cap;
        const std::vector<std::string> line = {"nonsense"};
        CB_CHECK_EQ(vxd::run(line, tools), cb::kExitUsage);
        CB_CHECK(cap.out().find("tools: idiom, tree") != std::string::npos);
    }
});
