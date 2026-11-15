// cb/ui/mcp_stdio_main.cpp -- the MCP server as a program of its own (cb_mcp_stdio): the `mcp` command (mcp_command.hpp) without the rest of the
// command table, so the tools can be tried -- on their own.
//
//     cb_mcp_stdio                                     serve over stdin and stdout
//     cb_mcp_stdio --http PORT [--host H] [--allow-host NAME ...] [--public]
//
// CB_MCP_LAUNCHER names the program the tools' jobs, and the service, run as (default: this program).
#include <cstdio>
#include <string>
#include <vector>

#include "cb/base/config.hpp"
#include "cb/engine/lane_registry.hpp"
#include "cb/ui/mcp_command.hpp"

int main(int argc, char** argv) {
    using namespace cb;
    engine::register_all_lanes();   // (the platform lanes `info` and `reach` ask: engine/lane_registry.hpp)
    try {
        const cli::Cli cli("cb_mcp_stdio", {ui::mcp_command::command({.config = config::current()})});
        std::vector<std::string> args{"mcp"};
        args.insert(args.end(), argv + 1, argv + argc);
        return cli.run(args);
    } catch (const std::exception& e) {
        std::fprintf(stderr, "cb_mcp_stdio: %s\n", e.what());
        return 1;
    }
}
