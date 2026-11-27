// ui/mcp_command.cpp -- the `mcp` command (mcp_command.hpp).
#include "ui/mcp_command.hpp"

#include <cstdio>

#include "core/fs.hpp"
#include "ui/mcp_server.hpp"

namespace cb::ui::mcp_command {

// `mcp --setup`: how to connect an assistant to this copy of CodeBreaker, with its own path filled in, ready to paste.
std::string mcp_setup_text(const std::string& exe) {
    std::string json_exe;   // (the path as a JSON string's content: backslashes doubled)
    for (const char c : exe) json_exe += c == '\\' ? std::string("\\\\") : std::string(1, c);
    return "Connect an AI assistant to CodeBreaker (docs/MCP.md has the tools). It runs on this computer: no account, key or internet.\n"
           "\n"
           "Claude Code (once, for every folder):\n"
           "    claude mcp add --scope user codebreaker -- \"" + exe + "\" mcp\n"
           "\n"
           "Claude Desktop: Settings, Developer, Edit Config, and add to claude_desktop_config.json:\n"
           "    {\"mcpServers\": {\"codebreaker\": {\"command\": \"" + json_exe + "\", \"args\": [\"mcp\"]}}}\n"
           "\n"
           "Any other MCP client (VS Code, Cursor, ...): a stdio server with\n"
           "    command: " + exe + "\n"
           "    args:    mcp\n"
           "\n"
           "Then ask it, for example: \"Use codebreaker to triage C:\\Windows\\notepad.exe\". A module is a file path, or a name in the\n"
           "program collection when Settings names one.\n";
}

namespace {

int mcp_run(const Environment& environment, const cli::Args& args) {
    if (args.flag("setup")) {
        std::fputs(mcp_setup_text(fs::to_text(fs::executable_path())).c_str(), stdout);
        return 0;
    }
    const config::Config& config = environment.config;
    const std::shared_ptr<base::Corpus> corpus = environment.corpus ? environment.corpus : mcp::standard_corpus(config);
    service::Environment service_environment = mcp::standard_environment(config, *corpus);
    if (!environment.launcher.empty()) service_environment.launcher = environment.launcher;
    else if (const std::optional<std::string> launcher = fs::get_env("CB_MCP_LAUNCHER"); launcher && !launcher->empty()) service_environment.launcher = {*launcher};
    service::Service service(std::move(service_environment));
    service::ServiceLink link(service, {.mode = service::mode_of_environment(), .start = true});   // (the service, started on demand)
    const bool is_public = args.flag("public");
    mcp::Server server(service, link, mcp::standard_services(config, *corpus), mcp::Options{.writes = !is_public});
    if (const std::optional<std::int64_t> port = args.integer<std::int64_t>("http")) {
        const std::string host = args.value_or("host", "127.0.0.1");
        (void)mcp::token(config.state());   // (made once, so the line below can say where it is)
        std::fprintf(stderr, "CodeBreaker MCP on http://%s:%lld/mcp%s -- token in %s\n", host.c_str(), static_cast<long long>(*port),
                     is_public ? " (public: reading tools only, also at /<token>/mcp)" : "",
                     fs::get_env("CB_MCP_TOKEN") ? "CB_MCP_TOKEN" : fs::to_text(config.state() / "mcp" / "http_token.txt").c_str());
        std::fflush(stderr);
        return server.serve_http(static_cast<std::uint16_t>(*port), host, args.values("allow-host"), is_public);
    }
    return server.serve_stdio();
}

}  // namespace

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "mcp";
    c.summary = "CodeBreaker as an MCP server: what the command line shows, for an assistant to use";
    c.params = {cli::Option{.name = "http", .help = "serve streamable HTTP at http://HOST:PORT/mcp behind a bearer token (default: stdio)", .arg = cli::Arg::Value, .value_name = "PORT"},
                cli::Option{.name = "host", .help = "the address HTTP listens on (default 127.0.0.1)", .arg = cli::Arg::Value, .value_name = "H"},
                cli::Option{.name = "allow-host", .help = "a Host name besides 127.0.0.1 and localhost to answer to, such as a tunnel's (repeatable)", .arg = cli::Arg::Value, .value_name = "NAME"},
                cli::Option{.name = "public", .help = "the reading tools only, and the token also accepted as the URL's first segment (/<token>/mcp)"},
                cli::Option{.name = "setup", .help = "print how to connect Claude Code, Claude Desktop or another MCP client to this copy, its path filled in"}};
    c.description = "Stdout is the protocol. The token for HTTP is CB_MCP_TOKEN, else <state>/mcp/http_token.txt.";
    c.handler = [environment](const cli::Args& args) { return mcp_run(environment, args); };
    return c;
}

}  // namespace cb::ui::mcp_command
