// codebreaker/ui/mcp_server.hpp -- COMPAT SHIM over cb/ui/mcp_server.hpp (docs/CPP_DESIGN.md 6.3), for the one consumer that still names it: the
// old command table (__main__.hpp registers `main`). Forwarding only; deleted with the old command table.
//
// The module (the wire, the 32 tools, the transports) is native/cb/ui/mcp_server. `main` builds what it needs from the checkout's configuration:
// the corpus (standard_corpus), a service over the state folder (standard_environment: the function store at state/functions.sqlite and state/db, as
// the Python's `functions.open()` finds it), a link to the running service (started on demand), and the services every tool reads, filled from
// the program's own modules (cb::ui::mcp::standard_services: the folders, the corpus, the inspector, the code, the decompiler, the version
// comparison, the names, the ledger, the command table).
//
// Not carried over: the old Hooks, the tool functions as C++ functions and `INTERACTIVE()` (no caller used them; the tools are
// `cb::ui::mcp::Server::call_tool` and the wire's own), and the old service module's process-wide state (a ServiceLink is an object).
#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <optional>
#include <string>
#include <vector>

#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/ui/mcp_server.hpp"
#include "py.hpp"

namespace codebreaker::ui::mcp_server {

namespace mcp = ::cb::ui::mcp;

constexpr int64_t MOST = static_cast<int64_t>(mcp::kMost);   // characters in one answer, at most
inline const std::string INSTRUCTIONS{mcp::kInstructions};

// The secret an HTTP client must send (Authorization: Bearer <it>): CB_MCP_TOKEN, else one made once and kept in
// state/mcp/http_token.txt.
inline py::str token() { return ::cb::compat::to_py(mcp::token(::cb::config::current().state())); }

// python -m codebreaker mcp [--http PORT [--host H] [--allow-host NAME ...] [--public]]
inline int64_t main(const py::list<py::str>& argv) {
    std::vector<std::string> args;
    for (const py::str& a : argv) args.push_back(a.utf8());
    auto after = [&](const char* flag) -> std::optional<std::string> {
        for (size_t i = 0; i + 1 < args.size(); ++i)
            if (args[i] == flag) return args[i + 1];
        return std::nullopt;
    };
    auto has = [&](const char* flag) { return std::find(args.begin(), args.end(), std::string(flag)) != args.end(); };

    const ::cb::config::Config& config = ::cb::config::current();
    static const std::shared_ptr<::cb::base::Corpus> corpus = mcp::standard_corpus(config);
    static ::cb::ui::service::Service service(mcp::standard_environment(config, *corpus));
    static ::cb::ui::service::ServiceLink link(service, {.mode = ::cb::ui::service::mode_of_environment(), .start = true});
    mcp::Server server(service, link, mcp::standard_services(config, *corpus), mcp::Options{.writes = !has("--public")});   // (public: the reading tools only)

    if (has("--http")) {
        const int port = std::stoi(after("--http").value_or("0"));
        const std::string host = after("--host").value_or("127.0.0.1");
        std::vector<std::string> hosts;
        for (size_t i = 0; i + 1 < args.size(); ++i)
            if (args[i] == "--allow-host") hosts.push_back(args[i + 1]);
        const bool is_public = has("--public");
        (void)mcp::token(config.state());
        std::fprintf(stderr, "CodeBreaker MCP on http://%s:%d/mcp%s -- token in %s\n", host.c_str(), port, is_public ? " (public: reading tools only, also at /<token>/mcp)" : "",
                     ::cb::fs::get_env("CB_MCP_TOKEN") ? "CB_MCP_TOKEN" : (config.state() / "mcp" / "http_token.txt").string().c_str());
        std::fflush(stderr);
        return server.serve_http(static_cast<std::uint16_t>(port), host, hosts, is_public);
    }
    return server.serve_stdio();
}

}  // namespace codebreaker::ui::mcp_server
