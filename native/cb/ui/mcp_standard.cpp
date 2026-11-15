// cb/ui/mcp_standard.cpp -- the services of the MCP server, every one filled from the program's own modules: the folders, the corpus, the inspector
// here; the code (disassembly, cross-references: mcp_code.cpp), the decompiler and the version comparison (mcp_work.cpp), the names, the runtime
// libraries, the ledger, the command table and the source tree (mcp_symbols.cpp). The parts they read (the symbols' workbench, the decompiler's
// surroundings) are made when a tool first asks.
#include <nlohmann/json.hpp>

#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/coverage/ledger.hpp"
#include "cb/engine/lane_kit.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/formats/detect.hpp"
#include "cb/store/functions.hpp"
#include "cb/verify/reconstruct.hpp"
#include "cb/formats/inspect.hpp"
#include "cb/source/module_writer.hpp"
#include "cb/ui/mcp_server.hpp"
#include "cb/ui/mcp_wiring.hpp"

namespace cb::ui::mcp {

namespace wiring {

Context::Context(const config::Config& config, const base::Corpus& corpus) : config_(&config), corpus_(&corpus), profiles_(verifier_profiles(config)) {}

symbols::Workbench& Context::bench() {
    std::lock_guard hold(make_);
    if (!bench_) {
        symbols::Environment environment;
        environment.corpus = corpus_;
        environment.config = config_;
        bench_ = std::make_unique<symbols::Workbench>(environment);
    }
    return *bench_;
}

decompile::environment::CorpusModules& Context::modules() {
    std::lock_guard hold(make_);
    if (!modules_) modules_ = std::make_unique<decompile::environment::CorpusModules>(*corpus_);
    return *modules_;
}

engine::LaneKit& Context::kit() {
    std::lock_guard hold(make_);
    if (!kit_) kit_ = std::make_unique<engine::LaneKit>(*config_);
    return *kit_;
}

}  // namespace wiring

std::shared_ptr<base::Corpus> standard_corpus(const config::Config& config) {
    base::FormatProbes probes;
    probes.lane = [](const std::filesystem::path& p) -> std::optional<base::Lane> {
        const std::optional<formats::detect::Lane> l = formats::detect::lane(p);
        if (!l) return std::nullopt;
        switch (*l) {
        case formats::detect::Lane::Pe: return base::Lane::Pe;
        case formats::detect::Lane::Ne: return base::Lane::Ne;
        case formats::detect::Lane::Le: return base::Lane::Le;
        case formats::detect::Lane::Dos: return base::Lane::Dos;
        }
        return std::nullopt;
    };
    return std::make_shared<base::Corpus>(config.products(), std::move(probes));
}

namespace wiring {

namespace {

// The verifier settings in force now, per kind of module: the lanes of a kit over the settings say them (their tools' digests).
struct StdProfiles {
    std::mutex lock;
    std::unique_ptr<engine::LaneKit> kit;
    verify::reconstruct::Kinds kinds;
    bool made = false;
};

}  // namespace

// Made when the ledger first asks, from the lanes alone (a machine with no function store yet has the same settings in force).
// CB_MCP_STORED_PROFILES=1 asks for none always: the ledger then reads the results as the store last filed them, whichever verifier made them
// (the proof against the recorded answers uses it: the two verifiers' settings digests differ, so the recorded results are not
// "under the settings in force" of this one until it judges them again).
std::function<std::map<std::string, std::optional<std::string>>()> verifier_profiles(const config::Config& config) {
    const auto profiles = std::make_shared<StdProfiles>();
    const config::Config* settings = &config;
    return [profiles, settings]() -> std::map<std::string, std::optional<std::string>> {
        // every kind the store files results under: one whose settings cannot be worked out here has none, and the ones it last used stand in
        std::map<std::string, std::optional<std::string>> out;
        for (const char* kind : {"pe", "ne", "le", "dos", "java", "powerpc", "nes", "sms", "gb", "genesis", "ps1", "gba", "zx", "msx", "coleco", "cpc"}) out[kind] = std::nullopt;
        for (const std::string& kind : engine::machine_lanes().names()) out[kind] = std::nullopt;   // (platforms registered in engine/machine_lanes.hpp)
        if (const std::optional<std::string> stored = fs::get_env("CB_MCP_STORED_PROFILES"); stored && !stored->empty() && *stored != "0") return out;
        std::lock_guard hold(profiles->lock);
        if (!profiles->made) {
            profiles->kit = std::make_unique<engine::LaneKit>(*settings);
            profiles->kit->add_lanes(profiles->kinds);
            profiles->made = true;
        }
        for (auto& [kind, digest] : verify::reconstruct::digests_of(profiles->kinds)) out[kind] = std::move(digest);
        return out;
    };
}

}  // namespace wiring

service::Environment standard_environment(const config::Config& config, const base::Corpus& corpus) {
    service::Environment env;
    env.state = config.state();
    env.root = config.checkout_root();
    env.ledger = coverage::ledger::services_over(corpus, env.state, wiring::verifier_profiles(config));
    env.retail = [&corpus](std::string_view name) { return corpus.retail(name); };
    env.describe_best = [](const engine::Best& best) { return source::module_writer::describe(best); };   // (the words the module writer's status gives)
    return env;
}

Services standard_services(const config::Config& config, const base::Corpus& corpus) {
    Services s;
    s.state = config.state();
    s.converted = config.path("converted").value_or(config.checkout_root() / "Converted");
    s.root = config.checkout_root();
    s.corpus_root = corpus.root();
    s.retail = [&corpus](std::string_view module) { return corpus.retail(module); };
    s.lane = [&corpus](const std::filesystem::path& file) {
        const std::optional<base::Lane> lane = corpus.lane(file);
        return lane ? std::string(base::lane_name(*lane)) : std::string();
    };
    s.inspect = [](const std::filesystem::path& file) {
        Json out = formats::inspect::to_json(formats::inspect::describe(file));
        if (out.is_object() && out.contains("path") && out["path"].is_string()) {   // (the path as the system writes it: the tools show it so)
            std::filesystem::path shown(out["path"].get<std::string>());
            out["path"] = shown.make_preferred().string();
        }
        return out;
    };
    s.strings = [](const std::filesystem::path& file, std::int64_t min_len) {
        formats::inspect::StringOptions options;
        options.min_length = static_cast<std::size_t>(min_len);
        return formats::inspect::to_json(formats::inspect::strings(file, options));
    };
    s.hexdump = [](const std::filesystem::path& file, std::int64_t offset, std::int64_t n) {
        return formats::inspect::hexdump(file, static_cast<std::uint64_t>(offset), static_cast<std::uint64_t>(n));
    };
    s.resource_body = [](const std::filesystem::path& file, std::int64_t offset, std::int64_t size, std::int64_t type_id, bool wide) {
        const formats::inspect::ResourceBody body =
            formats::inspect::resource_body(file, static_cast<std::uint64_t>(offset), static_cast<std::uint64_t>(size), static_cast<std::int32_t>(type_id), wide);
        ResourceBody out;
        if (body.kind == formats::inspect::ResourceBody::Kind::Image) {
            out.image = true;
            out.bytes = body.bytes;
            out.format = body.content_type.substr(body.content_type.rfind('/') + 1);
            return out;
        }
        Json doc = Json::object();
        doc["kind"] = body.kind == formats::inspect::ResourceBody::Kind::Text ? "text" : "hex";
        doc["text"] = body.text;
        out.document = std::make_shared<Json>(std::move(doc));
        return out;
    };
    const auto context = std::make_shared<wiring::Context>(config, corpus);
    wiring::fill_symbols(s, context);
    wiring::fill_code(s, context);
    wiring::fill_work(s, context);
    return s;
}

}  // namespace cb::ui::mcp
