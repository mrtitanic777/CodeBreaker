// cb_state_cmds: the proof harness of the commands that read and write the state folder -- names, knowledge, coverage, progress, facts, origin,
// driver-services -- and of `migrate-state`. Each gets the Environment the command table will give it (the settings, the corpus, the verifier
// profiles in force), so what it reads of a state folder and writes to it can be compared with what the translated Python did on the same
// folder. Not part of codebreaker.exe; not built by default (`cmake --build <dir> --target cb_state_cmds`).
//
//     CB_STATE=<copy of state> cb_state_cmds <command> [arguments]
//
// The table lines the integrator adds to native_commands.cpp are the registrations below (see `commands()`).
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

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

namespace {

using namespace cb;
namespace stdfs = std::filesystem;

// `migrate-state`'s verifier settings: what Reconstructor::profiles() says of the lanes this machine has.
struct MigrateProfiles {
    std::unique_ptr<engine::LaneKit> kit;
    verify::reconstruct::Kinds kinds;
    std::unique_ptr<engine::FunctionStore> store;
    std::unique_ptr<verify::reconstruct::Reconstructor> reconstructor;
};

std::vector<cli::Command> commands(const config::Config& config, const std::shared_ptr<base::Corpus>& corpus) {
    std::vector<cli::Command> out;

    // names <module> ...
    out.push_back(symbols::names::command(symbols::Environment{corpus.get(), &config}));

    // knowledge [platform [kind [key]]]
    out.push_back(knowledge::show::command(knowledge::environment(config.state())));

    // coverage, progress: the ledger over the function store of the state folder
    const auto profiles = ui::mcp::wiring::verifier_profiles(config);
    const coverage::ledger::Services ledger = coverage::ledger::services_over(*corpus, config.state(), profiles);
    out.push_back(coverage::ledger::command(ledger));
    out.push_back(ui::progress::command(ui::progress::store_reader(ledger)));

    // facts <binary> [kind [key]]
    {
        struct Facts {
            engine::facts::Databases databases;
            engine::Steps steps;
            knowledge::ReadDigests digests{[](const knowledge::Read&) { return std::string("absent"); }};
            engine::Engine engine;
            explicit Facts(const stdfs::path& root)
                : databases(engine::facts::Config{.root = root, .max_bytes = 4 * engine::facts::kGiB}), engine(steps, databases, digests, 2) {}
        };
        const auto facts = std::make_shared<Facts>(config.state() / "db");
        out.push_back(engine::show::command(engine::show::Environment{
            .engine = [facts]() -> engine::Engine& { return facts->engine; },
            .retail = [corpus](std::string_view name) { return corpus->retail(name); }}));
    }

    // origin <module> [--list]
    out.push_back(analyze::origin::command(analyze::origin::Environment{
        .retail = [corpus](std::string_view name) { return corpus->retail(name); }, .scratch = config.state() / "work" / "state_cmds", .knowledge = nullptr}));

    // driver-services [--generate] [FILE ...]
    {
        const auto kb = std::make_shared<knowledge::Store>(knowledge::Store::default_path(config.state()), knowledge::DataFolders::from_environment());
        stdfs::path reference;
        try {
            reference = corpus->design_dir() / "reference";
        } catch (const Error&) {
        }
        symbols::driver_services::Environment env;
        env.table = [kb] { return symbols::driver_services::table_of(*kb); };
        env.retail = [corpus](std::string_view name) { return corpus->retail(name); };
        env.corpus_drivers = [corpus] {
            std::vector<stdfs::path> drivers;
            for (const auto& [name, copies] : *corpus->index())
                if (!copies.empty() && formats::detect::holds_drivers(copies.front())) drivers.push_back(copies.front());
            return drivers;
        };
        env.table_file = config.checkout_root() / "codebreaker" / "symbols" / "driver_services.json";
        env.kit_folder = config.text("driver_kit");
        env.reference_folder = reference;
        out.push_back(symbols::driver_services::command(env));
    }

    // migrate-state [--dry] [--state DIR]
    {
        const auto mine = std::make_shared<MigrateProfiles>();
        store::migrate::Services services;
        services.state_dir = config.state();
        const config::Config* settings = &config;
        services.verifier_profiles = [mine, settings]() -> std::map<std::string, store::migrate::Verifier> {
            std::map<std::string, store::migrate::Verifier> result;
            mine->store = store::functions::open(settings->state(), std::nullopt, true);
            if (!mine->store) return result;
            mine->kit = std::make_unique<engine::LaneKit>(*settings);
            mine->kit->add_lanes(mine->kinds);
            mine->reconstructor = std::make_unique<verify::reconstruct::Reconstructor>(mine->kit->reconstruct_services(mine->kinds, *mine->store));
            for (const auto& [kind, profile] : mine->reconstructor->profiles()) result[kind] = {profile.digest, profile.settings};
            return result;
        };
        out.push_back(store::migrate::command(services));
    }
    return out;
}

}  // namespace

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
        cli::Cli program("codebreaker", commands(config, corpus));
        code = program.run(args);
    } catch (const Error& e) {
        err("{}\n", e.what());
        code = e.exit_code();
    }
    flush_out();
    return code;
}
