// The proof harness of the program-reading and recompiling commands (not part of codebreaker.exe: the command table wires these):
//
//     cb_cmds <command> [arguments]       classes | xdk-match | recompile | recompile-project | xenon-recompile | java | xna
//
// It builds each command's Environment from the configuration of the checkout (CB_CHECKOUT_ROOT, settings.json, CB_<KEY>) exactly as the
// command table will, runs it, and prints its output, so what a command writes can be compared with what the translated Python wrote.
#include <algorithm>
#include <string>
#include <vector>

#include "cb/base/config.hpp"
#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/log.hpp"
#include "cb/java/tools.hpp"
#include "cb/recompile/command.hpp"
#include "cb/symbols/program_commands.hpp"
#include "cb/xna/command.hpp"

int main(int argc, char** argv) {
    using namespace cb;
    std::vector<std::string> args(argv + 1, argv + argc);
    if (args.empty()) {
        err("usage: cb_cmds <command> [arguments]\n");
        return kExitUsage;
    }
    const std::string name = args.front();
    const std::vector<std::string> rest(args.begin() + 1, args.end());
    const config::Config& config = config::current();
    int code = kExitUsage;
    try {
        if (name == "classes") {
            cli::Cli program("codebreaker", {symbols::program_commands::classes_command({config})});
            code = program.run(args);
        } else if (name == "xdk-match") {
            code = symbols::program_commands::xdk_match(rest, {config});
        } else if (name == "recompile") {
            code = recompile::command::recompile(rest, {config});
        } else if (name == "recompile-project") {
            code = recompile::command::recompile_project(rest, {config});
        } else if (name == "xenon-recompile") {
            code = recompile::command::xenon_recompile(rest, {config});
        } else if (name == "java") {
            code = java::tools::main(rest, {config});
        } else if (name == "xna") {
            code = xna::command::run(rest, {config});
        } else {
            err("unknown command {}\n", name);
        }
    } catch (const Error& e) {
        err("{}\n", e.what());
        code = e.exit_code();
    }
    flush_out();
    return code;
}
