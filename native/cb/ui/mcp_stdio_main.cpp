// cb/ui/mcp_stdio_main.cpp -- the MCP server as a program of its own, over the standard services (cb::ui::mcp::standard_services):
//
//     cb_mcp_stdio                                     serve over stdin and stdout
//     cb_mcp_stdio --http PORT [--host H] [--allow-host NAME ...] [--public]
//
// What `codebreaker mcp` serves too; this one starts without the whole command table, so the tools can be tried (and compared with the Python
// server's answers) on their own. CB_MCP_LAUNCHER names the program the tools' jobs run as (default: this program).
#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/core/fs.hpp"
#include "cb/ui/mcp_server.hpp"

int main(int argc, char** argv) {
    using namespace cb;
    std::vector<std::string> args(argv + 1, argv + argc);
    auto after = [&](const char* flag) -> std::optional<std::string> {
        for (std::size_t i = 0; i + 1 < args.size(); ++i)
            if (args[i] == flag) return args[i + 1];
        return std::nullopt;
    };
    auto has = [&](const char* flag) { return std::find(args.begin(), args.end(), std::string(flag)) != args.end(); };
    try {
        const config::Config& config = config::current();
        const std::shared_ptr<base::Corpus> corpus = ui::mcp::standard_corpus(config);
        ui::service::Environment env = ui::mcp::standard_environment(config, *corpus);
        if (const std::optional<std::string> launcher = fs::get_env("CB_MCP_LAUNCHER"); launcher && !launcher->empty()) env.launcher = {*launcher};
        ui::service::Service service(std::move(env));
        ui::service::ServiceLink link(service, {.mode = ui::service::mode_of_environment(), .start = true});
        ui::mcp::Server server(service, link, ui::mcp::standard_services(config, *corpus), ui::mcp::Options{.writes = !has("--public")});
        if (has("--http")) {
            const int port = std::stoi(after("--http").value_or("0"));
            const std::string host = after("--host").value_or("127.0.0.1");
            std::vector<std::string> hosts;
            for (std::size_t i = 0; i + 1 < args.size(); ++i)
                if (args[i] == "--allow-host") hosts.push_back(args[i + 1]);
            (void)ui::mcp::token(config.state());
            return server.serve_http(static_cast<std::uint16_t>(port), host, hosts, has("--public"));
        }
        return server.serve_stdio();
    } catch (const std::exception& e) {
        std::fprintf(stderr, "cb_mcp_stdio: %s\n", e.what());
        return 1;
    }
}
