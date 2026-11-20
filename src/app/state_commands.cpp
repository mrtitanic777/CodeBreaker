// app/state_commands.cpp -- see state_commands.hpp.
#include "app/state_commands.hpp"

#include <algorithm>
#include <cctype>
#include <map>
#include <memory>
#include <span>
#include <string>
#include <vector>

#include "analyze/function_evidence.hpp"
#include "analyze/origin.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "decompile/decompiler.hpp"
#include "coverage/ledger.hpp"
#include "engine/engine.hpp"
#include "engine/facts.hpp"
#include "engine/lane_kit.hpp"
#include "engine/show.hpp"
#include "formats/detect.hpp"
#include "formats/inspect.hpp"
#include "formats/pe.hpp"
#include "knowledge/store.hpp"
#include "store/functions.hpp"
#include "store/migrate_state.hpp"
#include "symbols/driver_services.hpp"
#include "symbols/environment.hpp"
#include "symbols/names.hpp"
#include "ui/mcp_server.hpp"
#include "ui/mcp_wiring.hpp"
#include "ui/progress_store.hpp"
#include "verify/reconstruct.hpp"

namespace cb::app {

namespace {

namespace stdfs = std::filesystem;

// `migrate-state`'s verifier settings: what Reconstructor::profiles() says of the lanes this machine has.
struct MigrateProfiles {
    std::unique_ptr<engine::LaneKit> kit;
    verify::reconstruct::Kinds kinds;
};

// What proposing names reads of a module: the facts that could name each function (the analyses), and the decompiler's C for some. The kit that
// holds the decompiler is made when the first of them is asked.
class NameFacts {
public:
    NameFacts(const config::Config& config, std::shared_ptr<base::Corpus> corpus) : config_(config), corpus_(std::move(corpus)) {}

    std::vector<symbols::FunctionEvidence> evidence(const stdfs::path& module) {
        analyze::report::Environment e;
        base::Corpus& corpus = *corpus_;
        e.retail = [&corpus](std::string_view name) -> std::optional<stdfs::path> { return corpus.retail(name); };
        e.sha = [&corpus](const stdfs::path& path) { return corpus.sha(path); };
        e.state = config_.state();
        e.knowledge = &kit().decompiler().context().environment->knowledge();
        return analyze::function_evidence::of_module(module, e);
    }

    std::map<std::uint64_t, std::string> decompile(const stdfs::path& module, std::span<const std::uint64_t> starts) {
        decompile::decompiler::Decompiler& decompiler = kit().decompiler();
        std::map<std::uint64_t, std::string> out;
        const bool x86 = corpus_->lane(module) == base::Lane::Pe && formats::pe::Image::open(module).header().machine == formats::pe::kMachineI386;
        if (x86) {
            const std::vector<decompile::decompiler::Va> vas(starts.begin(), starts.end());
            const decompile::c_text::Compilable32 made = decompiler.compilable32(module, vas);
            for (const std::uint64_t start : starts) {
                const auto name = made.by_va.find(static_cast<decompile::decompiler::Va>(start));
                if (name != made.by_va.end() && made.texts.contains(name->second)) out[start] = made.texts.at(name->second);
            }
            return out;
        }
        const std::string whole = decompiler.as_c(module).text;   // (the whole module's C, a function cut out of it by its header)
        for (const std::uint64_t start : starts)
            if (std::string cut = function_in(whole, symbols::names::function_key(base::Lane::Ne, start)); !cut.empty()) out[start] = std::move(cut);
        return out;
    }

private:
    engine::LaneKit& kit() {
        if (!kit_) kit_ = std::make_unique<engine::LaneKit>(config_, corpus_);
        return *kit_;
    }

    // The function whose header names `key` ("1:0116" is sub_1_0116): its lines up to the closing brace at the margin.
    static std::string function_in(const std::string& text, const std::string& key) {
        // (the decompiler writes the offset in capitals: sub_1_00B2; a name in lower case is looked for too)
        std::string token = "sub_", upper = "sub_";
        for (const char c : key) {
            token += c == ':' ? '_' : c;
            upper += c == ':' ? '_' : static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }
        token += '(';
        upper += '(';
        std::size_t at = 0;
        while (at < text.size()) {
            std::size_t end = text.find('\n', at);
            if (end == std::string::npos) end = text.size();
            const std::string_view line(text.data() + at, end - at);
            const bool header = !line.empty() && line.front() != ' ' && line.front() != '\t' && line.front() != '}' && line.front() != '/' && line.front() != '#' &&
                                (line.find(token) != std::string_view::npos || line.find(upper) != std::string_view::npos);
            if (header) {
                const std::size_t close = text.find("\n}", at);
                return text.substr(at, close == std::string::npos ? std::string::npos : close + 2 - at);
            }
            at = end + 1;
        }
        return {};
    }

    config::Config config_;
    std::shared_ptr<base::Corpus> corpus_;
    std::unique_ptr<engine::LaneKit> kit_;
};

}  // namespace

std::vector<cli::Command> state_commands(const config::Config& config, const std::shared_ptr<base::Corpus>& corpus) {
    std::vector<cli::Command> out;

    // names <module> ...
    {
        const auto facts = std::make_shared<NameFacts>(config, corpus);
        symbols::Environment names_environment{corpus.get(), &config};
        names_environment.evidence = [facts](const stdfs::path& module) { return facts->evidence(module); };
        names_environment.decompile = [facts](const stdfs::path& module, std::span<const std::uint64_t> starts) { return facts->decompile(module, starts); };
        out.push_back(symbols::names::command(names_environment));
    }

    // knowledge [platform [kind [key]]]
    out.push_back(knowledge::show::command(knowledge::environment(config.state(), knowledge::DataFolders::under(config.checkout_root()))));

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
        // (made when `facts` first asks: the databases folder is not made by the other commands)
        const auto facts = std::make_shared<std::unique_ptr<Facts>>();
        const stdfs::path root = config.state() / "db";
        out.push_back(engine::show::command(engine::show::Environment{
            .engine = [facts, root]() -> engine::Engine& {
                if (!*facts) *facts = std::make_unique<Facts>(root);
                return (*facts)->engine;
            },
            .retail = [corpus](std::string_view name) { return corpus->retail(name); }}));
    }

    // origin <module> [--list]
    out.push_back(analyze::origin::command(analyze::origin::Environment{
        .retail = [corpus](std::string_view name) { return corpus->retail(name); }, .scratch = config.state() / "origin", .knowledge = nullptr}));

    // driver-services [--generate] [FILE ...]
    {
        const knowledge::DataFolders folders = knowledge::DataFolders::under(config.checkout_root());
        const auto kb = std::make_shared<knowledge::Store>(knowledge::Store::default_path(folders), folders);
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
        env.table_file = config.config_root() / "symbols" / "driver_services.json";
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
            if (!mine->kit) {
                mine->kit = std::make_unique<engine::LaneKit>(*settings);
                mine->kit->add_lanes(mine->kinds);
            }
            for (const auto& [kind, profile] : verify::reconstruct::profiles_of(mine->kinds)) result[kind] = {profile.digest, profile.settings};
            return result;
        };
        out.push_back(store::migrate::command(services));
    }
    return out;
}

}  // namespace cb::app
