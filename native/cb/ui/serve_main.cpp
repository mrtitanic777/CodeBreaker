// cb/ui/serve_main.cpp -- the service as a program of its own (cb_serve): the `serve` command (serve.hpp) without the rest of the command table.
//
//     cb_serve serve [--status | --stop] [--idle MINUTES] [--port N]
//
// What a client starts when it is given this program as the launcher (CB_MCP_LAUNCHER): `<launcher> serve`.
#include <cstdio>

#include "cb/base/config.hpp"
#include "cb/ui/serve.hpp"

int main(int argc, char** argv) {
    using namespace cb;
    try {
        const cli::Cli cli("cb_serve", {ui::serve::command({.config = config::current()})});
        return cli.run(argc, argv);
    } catch (const std::exception& e) {
        std::fprintf(stderr, "cb_serve: %s
", e.what());
        return 1;
    }
}
