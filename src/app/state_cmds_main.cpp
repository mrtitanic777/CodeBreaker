// cb_state_cmds: the proof harness of the commands that read and write the state folder -- names, knowledge, coverage, progress, facts, origin,
// driver-services. Each gets the Environment the command table will give it (the settings, the corpus), so what
// it reads of a state folder and writes to it can be checked on a copy of a real folder. Not part of codebreaker.exe; not built by default (`cmake --build <dir> --target cb_state_cmds`).
//
//     CB_STATE=<copy of state> cb_state_cmds <command> [arguments]
//
// The table lines the integrator adds to native_commands.cpp are the registrations below (see `commands()`).
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "app/state_commands.hpp"
#include "analyze/origin.hpp"
#include "base/config.hpp"
#include "base/corpus.hpp"
#include "core/cli.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "coverage/ledger.hpp"
#include "engine/engine.hpp"
#include "engine/facts.hpp"
#include "engine/lane_kit.hpp"
#include "engine/show.hpp"
#include "formats/detect.hpp"
#include "formats/inspect.hpp"
#include "knowledge/store.hpp"
#include "store/functions.hpp"
#include "symbols/driver_services.hpp"
#include "symbols/environment.hpp"
#include "naming/names.hpp"
#include "ui/mcp_server.hpp"
#include "ui/mcp_wiring.hpp"
#include "ui/progress_store.hpp"
#include "verify/reconstruct.hpp"


using namespace cb;

int main(int argc, char** argv) {
    const config::Config& config = config::current();
    const std::shared_ptr<base::Corpus> corpus = ui::mcp::standard_corpus(config);
    std::vector<std::string> args(argv + 1, argv + argc);
    if (args.empty()) {
        err("usage: cb_state_cmds <names|knowledge|coverage|progress|facts|origin|driver-services> [arguments]\n");
        return kExitUsage;
    }
    int code = kExitUsage;
    if (args.size() == 2 && args[0] == "code-bytes") {   // what the ledger takes a file's code size to be (the inspector's figure)
        const formats::inspect::Description d = formats::inspect::describe(fs::from_text(args[1]));
        out("{}\n", d.summary ? static_cast<long long>(d.summary->code_bytes) : -1LL);
        flush_out();
        return kExitOk;
    }
    try {
        cli::Cli program("codebreaker", cb::app::state_commands(config, corpus));
        code = program.run(args);
    } catch (const Error& e) {
        err("{}\n", e.what());
        code = e.exit_code();
    }
    flush_out();
    return code;
}
