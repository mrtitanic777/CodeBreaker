// ui/mcp_command.hpp -- `codebreaker mcp`: the MCP server as a command (what .mcp.json starts), over the standard services of the settings.
//
//     codebreaker mcp                                              stdio (what Claude Code starts)
//     codebreaker mcp --http PORT [--host H] [--allow-host NAME ...] [--public]
//         streamable HTTP at http://H:PORT/mcp (H: 127.0.0.1), every request needing the bearer token (CB_MCP_TOKEN, else
//         <state>/mcp/http_token.txt); --allow-host: a Host name besides 127.0.0.1 and localhost to answer to, such as a tunnel's;
//         --public: the reading tools only, and the token also accepted as the URL's first segment (/<token>/mcp), for clients that cannot send a header
//
//     cli::Cli cli("codebreaker", {ui::mcp_command::command({.config = config, .corpus = corpus})});
//
// The tools are answered by the service of the state folder (serve.hpp, started on demand) so the terminal interface, every MCP server and the
// command line share one process; with no service (CB_SERVICE=0, or it cannot start) they are answered here. Stdout is the protocol.
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "base/config.hpp"
#include "base/corpus.hpp"
#include "core/cli.hpp"

namespace cb::ui::mcp_command {

struct Environment {
    config::Config config;
    std::shared_ptr<base::Corpus> corpus;   // the products' files; the configuration's own when null
    std::vector<std::string> launcher;      // what the tools' jobs, and the service, are started as; default: CB_MCP_LAUNCHER, else this program
};

cli::Command command(const Environment& environment);

}  // namespace cb::ui::mcp_command
