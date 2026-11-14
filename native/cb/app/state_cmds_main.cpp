// cb_state_cmds: the proof harness of the commands that read and write the state folder -- names, knowledge, coverage, progress, facts, origin,
// driver-services -- and of `migrate-state`. Each gets the Environment the command table will give it (the settings, the corpus, the verifier
// profiles in force), so what it reads of a state folder and writes to it can be checked on a copy of a real folder. Not part of codebreaker.exe; not built by default (`cmake --build <dir> --target cb_state_cmds`).
//
//     CB_STATE=<copy of state> cb_state_cmds <command> [arguments]
//
// The table lines the integrator adds to native_commands.cpp are the registrations below (see `commands()`).
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "cb/app/state_commands.hpp"
#include "cb/analyze/origin.hpp"
#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/coverage/ledger.hpp"
#include "cb/engine/engine.hpp"
#include "cb/engine/facts.hpp"
#include "cb/engine/lane_kit.hpp"
#include "cb/engine/show.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/inspect.hpp"
#include "cb/knowledge/store.hpp"
#include "cb/store/functions.hpp"
#include "cb/store/migrate_state.hpp"
#include "cb/symbols/driver_services.hpp"
#include "cb/symbols/environment.hpp"
#include "cb/symbols/names.hpp"
#include "cb/ui/mcp_server.hpp"
#include "cb/ui/mcp_wiring.hpp"
#include "cb/ui/progress_store.hpp"
#include "cb/verify/reconstruct.hpp"


using namespace cb;

int main(int argc, char** argv) {
    const config::Config& config = config::current();
    const std::shared_ptr<base::Corpus> corpus = ui::mcp::standard_corpus(config);
    std::vector<std::string> args(argv + 1, argv + argc);
    if (args.empty()) {
        err("usage: cb_state_cmds <names|knowledge|coverage|progress|facts|origin|driver-services|migrate-state> [arguments]\n");
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
