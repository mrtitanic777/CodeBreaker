#include "cb/source/ne_asm/run.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace {
// (std::string::contains is C++23.)
bool has(std::string_view haystack, std::string_view needle) { return haystack.find(needle) != std::string_view::npos; }
}  // namespace


using namespace cb;
namespace run = cb::source::ne_asm::run;

CB_TEST("source.ne_asm.run.a_tool_gets_the_rest_of_the_command_line", [] {
    std::vector<std::string> seen;
    base::commands::ToolTable tools = {
        {"segdiff", [&](std::span<const std::string> args) { seen.assign(args.begin(), args.end()); return 7; }},
        {"recon", [](std::span<const std::string>) { return 0; }},
    };
    const std::vector<std::string> line = {"segdiff", "x.c", "--seg", "1"};
    CB_CHECK_EQ(run::main(tools, line), 7);
    CB_CHECK((seen) == ((std::vector<std::string>{"x.c", "--seg", "1"})));
});

CB_TEST("source.ne_asm.run.no_tool_or_an_unknown_one_prints_the_help", [] {
    base::commands::ToolTable tools = {{"segdiff", [](std::span<const std::string>) { return 0; }}, {"idiom", [](std::span<const std::string>) { return 0; }}};
    {
        testing::CapturedStd captured;
        CB_CHECK((run::main(tools, std::vector<std::string>{})) == (kExitUsage));
        const std::string out = captured.out();
        CB_CHECK(has(out, "codebreaker ne-asm <tool> [arguments]"));
        CB_CHECK(out.ends_with("\ntools: idiom, segdiff\n"));   // the names the table has, sorted
    }
    testing::CapturedStd captured;
    CB_CHECK((run::main(tools, std::vector<std::string>{"nonsense", "x"})) == (kExitUsage));
    CB_CHECK(has(captured.out(), "tools: idiom, segdiff"));
});

CB_TEST("source.ne_asm.run.the_help_describes_every_tool", [] {
    const std::string_view help = run::usage();
    CB_CHECK_EQ(run::tool_names().size(), 11u);
    for (const std::string_view tool : run::tool_names()) CB_CHECK(has(help, tool));
    CB_CHECK(has(help, R"(WIN32SDK\SCT\DEBUG16)"));
    CB_CHECK(has(help, "NE_SAVE=FILE keeps the source"));
    CB_CHECK(help.ends_with("compared."));
});
