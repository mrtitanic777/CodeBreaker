#include "cb/base/commands.hpp"

#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::base;

namespace {

commands::ToolTable make_tools(std::vector<std::string>& seen) {
    return {
        {"zeta", [&seen](std::span<const std::string> a) {
             seen.assign(a.begin(), a.end());
             return 7;
         }},
        {"alpha", [](std::span<const std::string>) { return 0; }},
    };
}

}  // namespace

CB_TEST("base.commands.dispatch_passes_the_rest_and_the_exit_code", [] {
    std::vector<std::string> seen;
    const auto tools = make_tools(seen);
    const std::vector<std::string> argv = {"zeta", "-x", "file with space"};
    CB_CHECK_EQ(commands::dispatch(tools, argv, "usage"), 7);
    CB_CHECK_EQ(seen.size(), 2u);
    CB_CHECK_EQ(seen[0], "-x");
    CB_CHECK_EQ(seen[1], "file with space");
    const std::vector<std::string> bare = {"alpha"};
    CB_CHECK_EQ(commands::dispatch(tools, bare), 0);
});

CB_TEST("base.commands.unknown_tool_or_none_prints_usage_and_sorted_names", [] {
    std::vector<std::string> seen;
    const auto tools = make_tools(seen);
    {
        testing::CapturedStd cap;
        const std::vector<std::string> argv = {"nope"};
        CB_CHECK_EQ(commands::dispatch(tools, argv, "the usage"), kExitUsage);
        CB_CHECK_EQ(cap.out(), "the usage\ntools: alpha, zeta\n");
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(commands::dispatch(tools, {}, "the usage"), kExitUsage);
        CB_CHECK_EQ(cap.out(), "the usage\ntools: alpha, zeta\n");
    }
    CB_CHECK(seen.empty());
});
