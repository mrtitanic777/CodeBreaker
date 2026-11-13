// cb/ui/mcp_symbols.cpp -- the rest of what the MCP tools read of the program: the names of a module's functions and imports, the runtime libraries and
// the compiler of a module, the numbered exports' names, naming a function, the function store's stored reconstructions, the coverage ledger and what a
// running sweep is doing, the command table, the source tree and what a file is.
//
// Each is a function of `Services`; the tools page, cut and combine the documents these make (mcp_tools.cpp). The documents are the ones the Python
// modules made, so the same tool answers the same.
//
// DIVERGES from ui/mcp_server.py:
//   * the command table is the catalog of cb/app (a command's synopsis and summary), where the Python read each command's module docstring;
//   * the version of the program is a fixed "2.0.0", where the Python asked its package metadata.
#include <algorithm>
#include <chrono>
#include <format>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

#include "cb/app/commands.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/coverage/ledger.hpp"
#include "cb/coverage/reach.hpp"
#include "cb/engine/function_store.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/gb.hpp"
#include "cb/formats/genesis.hpp"
#include "cb/formats/msx.hpp"
#include "cb/formats/nes.hpp"
#include "cb/formats/pe.hpp"
#include "cb/formats/sms.hpp"
#include "cb/formats/xex.hpp"
#include "cb/formats/zx.hpp"
#include "cb/java/corpus.hpp"
#include "cb/platforms/dos.hpp"
#include "cb/platforms/gb.hpp"
#include "cb/platforms/genesis.hpp"
#include "cb/platforms/java.hpp"
#include "cb/platforms/msx.hpp"
#include "cb/platforms/nes.hpp"
#include "cb/platforms/package.hpp"
#include "cb/platforms/sms.hpp"
#include "cb/platforms/vxd.hpp"
#include "cb/platforms/win16.hpp"
#include "cb/source/placement.hpp"
#include "cb/store/functions.hpp"
#include "cb/ui/mcp_wiring.hpp"
#include "cb/ui/progress.hpp"
#include "cb/verify/lanes/java.hpp"

namespace cb::ui::mcp::wiring {
namespace {

namespace sym = cb::symbols;

// ---- what a file is ---------------------------------------------------------------------------------------------------------

std::optional<platforms::ExeKind> sy_exe_kind(std::optional<formats::detect::Lane> lane) {
    if (!lane) return std::nullopt;
    switch (*lane) {
    case formats::detect::Lane::Pe: return platforms::ExeKind::Pe;
    case formats::detect::Lane::Ne: return platforms::ExeKind::Ne;
    case formats::detect::Lane::Le: return platforms::ExeKind::Le;
    case formats::detect::Lane::Dos: return platforms::ExeKind::Dos;
    }
    return std::nullopt;
}

std::unique_ptr<platforms::Registry> sy_registry() {
    platforms::Probes probes;
    probes.executable_class = [](const std::filesystem::path& p) { return sy_exe_kind(formats::detect::lane(p)); };
    probes.kind = [](const std::filesystem::path& p) { return sy_exe_kind(formats::detect::kind(p)); };
    probes.pe_machine = [](const std::filesystem::path& p) -> std::optional<std::uint16_t> {
        try {
            return static_cast<std::uint16_t>(formats::pe::Image::open(p).header().machine);
        } catch (const Error&) {
            return std::nullopt;
        }
    };
    probes.holds_drivers = [](const std::filesystem::path& p) { return formats::detect::holds_drivers(p); };
    probes.is_xex = [](const std::filesystem::path& p) { return formats::xex::probe(p); };
    probes.is_nes = [](const std::filesystem::path& p) { return formats::nes::probe(p); };
    probes.is_sms = [](const std::filesystem::path& p) { return formats::sms::probe(p); };
    probes.is_gb = [](const std::filesystem::path& p) { return formats::gb::probe(p); };
    probes.is_genesis = [](const std::filesystem::path& p) { return formats::genesis::probe(p); };
    probes.is_msx = [](const std::filesystem::path& p) { return formats::msx::probe(p); };
    probes.is_zx = [](const std::filesystem::path& p) { return formats::zx::probe(p); };
    probes.is_java_module = [](const std::filesystem::path& p) { return verify::lanes::JavaLane::is_module(p); };
    probes.xex_basefile = [](const std::filesystem::path& p) { return p; };
    probes.xbe_to_pe = [](const std::filesystem::path& p) { return p; };
    auto registry = std::make_unique<platforms::Registry>(std::move(probes));
    registry->add(platforms::dos());
    registry->add(platforms::vxd());
    registry->add(platforms::win16());
    registry->add(platforms::win32());
    registry->add(platforms::msx());
    registry->add(platforms::zx());
    registry->add(platforms::genesis());
    registry->add(platforms::gb());
    registry->add(platforms::sms());
    registry->add(platforms::nes());
    registry->add(platforms::xbe());
    registry->add(platforms::xbox360());
    registry->add(platforms::java());
    return registry;
}

// ---- the store and the ledger -------------------------------------------------------------------------------------------------

std::unique_ptr<engine::FunctionStore> sy_open_store(const config::Config& config) { return store::functions::open(config.state(), std::nullopt, true); }

Json sy_ledger_json(const config::Config& config, const base::Corpus& corpus, const std::function<std::map<std::string, std::optional<std::string>>()>& profiles, bool refresh) {
    const std::unique_ptr<engine::FunctionStore> store = sy_open_store(config);
    coverage::ledger::Ledger ledger(coverage::ledger::services_over(corpus, config.state(), profiles), store.get());
    return coverage::ledger::to_json(ledger.measure(std::nullopt, refresh));
}

// What a running sweep is doing: the store and the ledger as the progress reading wants them.
progress::StoreReading sy_read_store(const config::Config& config, const base::Corpus& corpus, const std::function<std::map<std::string, std::optional<std::string>>()>& profiles, progress::Clock::time_point now) {
    progress::StoreReading out;
    const std::unique_ptr<engine::FunctionStore> store = sy_open_store(config);
    coverage::ledger::Ledger ledger(coverage::ledger::services_over(corpus, config.state(), profiles), store.get());
    const coverage::ledger::Report report = ledger.measure();
    out.store_path = store::functions::default_path(config.state()).make_preferred().string();   // (as the system writes it)
    out.opened = store != nullptr;
    for (const coverage::ledger::Summary& k : report.kinds) {
        const auto profile = report.profiles.find(k.kind);
        if (profile == report.profiles.end() || !profile->second) continue;
        progress::KindReading r;
        r.kind = k.kind;
        r.name = k.name;
        r.modules = static_cast<std::uint64_t>(k.modules);
        r.measured = static_cast<std::uint64_t>(k.measured);
        r.functions = static_cast<std::uint64_t>(k.functions);
        for (const verify::Class c : verify::kClassOrder) {
            const auto n = k.classes.find(c);
            r.classes.push_back({std::string(verify::word(c)), std::string(verify::title(c)), n == k.classes.end() ? 0 : static_cast<std::uint64_t>(n->second)});
        }
        out.kinds.push_back(std::move(r));
    }
    if (!store) return out;
    const double seconds = std::chrono::duration<double>(now.time_since_epoch()).count();
    for (const engine::RecentModule& m : store->recent(8))
        out.recent.push_back({m.name, std::string(coverage::ledger::kind_title(m.kind).empty() ? std::string_view(m.kind) : coverage::ledger::kind_title(m.kind)), static_cast<std::uint64_t>(m.judged),
                              progress::Clock::time_point(std::chrono::duration_cast<progress::Clock::duration>(std::chrono::duration<double>(m.last)))});
    out.fresh = static_cast<std::uint64_t>(store->verdicts_since(seconds - static_cast<double>(progress::kWindow.count())));
    return out;
}

Json sy_progress_json(const progress::Snapshot& s) {
    Json out = Json::object();
    out["time"] = s.time;
    Json kinds = Json::array();
    for (const progress::KindReading& k : s.kinds) {
        Json row = Json::object();
        row["kind"] = k.kind;
        row["name"] = k.name;
        row["modules"] = k.modules;
        row["measured"] = k.measured;
        row["functions"] = k.functions;
        for (const progress::ClassCount& c : k.classes) row[c.id] = c.count;
        kinds.push_back(std::move(row));
    }
    out["kinds"] = std::move(kinds);
    Json recent = Json::array();
    for (const progress::Recent& r : s.recent) recent.push_back(Json{{"module", r.module}, {"kind", r.kind}, {"results", r.results}, {"age", r.age.count()}});
    out["recent"] = std::move(recent);
    out["rate"] = s.rate;
    out["idle"] = s.idle ? Json(s.idle->count()) : Json(nullptr);
    out["workers"] = s.workers;
    out["eta"] = s.eta ? Json(s.eta->count()) : Json(nullptr);
    out["active"] = s.active;
    out["log"] = s.log;
    out["store"] = s.store;
    return out;
}

}  // namespace

void fill_symbols(Services& services, const std::shared_ptr<Context>& context) {
    // the names of a module's functions and imports
    services.names = [context](const std::filesystem::path& p) {
        std::lock_guard hold(context->lock());
        const sym::names::ModuleNames got = context->bench().namer().of_module(p);
        Json functions = Json::array(), imports = Json::array();
        for (const sym::names::FunctionName& f : got.functions) functions.push_back(Json::array({f.address, f.key, f.name, f.source}));
        for (const sym::names::ImportName& i : got.imports)
            imports.push_back(Json::array({i.module, i.ordinal ? Json(*i.ordinal) : Json(nullptr), i.name, i.source}));
        return Json{{"functions", std::move(functions)}, {"imports", std::move(imports)}};
    };
    // the functions a runtime library made, and what built the module
    services.identified = [context](const std::filesystem::path& p) {
        std::lock_guard hold(context->lock());
        sym::identify::Identifier& identifier = context->bench().identifier();
        const sym::identify::Found found = identifier.identify(p);
        Json rows = Json::array();
        for (const auto& [address, what] : found) rows.push_back(Json::array({address, what.name, what.library}));
        Json out = Json::object();
        out["identified"] = std::move(rows);
        try {
            const sym::runtime_library::CompilerInfo info = identifier.compiler(p, found);
            out["compiler"] = Json{{"linker", info.linker}, {"compiler", info.compiler}, {"evidence", info.evidence}};
        } catch (const Error& e) {
            out["compiler"] = std::string("unknown (") + e.what() + ")";
        }
        return out;
    };
    // "KERNEL32!#17" as its name when the number has one
    services.ordinal_name = [context](std::string_view module, std::int64_t ordinal) -> Json {
        std::lock_guard hold(context->lock());
        const sym::ordinals::Ordinals& ordinals = context->bench().ordinals();
        const std::uint32_t number = static_cast<std::uint32_t>(ordinal);
        if (const std::optional<std::string> real = ordinals.name(module, number)) return Json{{"name", *real}, {"source", ordinals.source(module, number)}};
        if (const auto doc = sym::ordinals::documented(module, number)) return Json{{"name", doc->name}, {"source", doc->where}};
        return Json(nullptr);
    };
    // naming a function: the name goes on its body, so every module holding the same code has it
    services.set_name = [context](const std::filesystem::path& p, std::int64_t address, std::string_view name) {
        std::lock_guard hold(context->lock());
        const std::optional<std::string> digest = context->bench().namer().set_name(p, static_cast<sym::names::Address>(address), name);
        return digest ? *digest : std::string();
    };
    services.modules_with_body = [context](std::string_view digest) -> Json {
        std::lock_guard hold(context->lock());
        return Json(static_cast<std::int64_t>(context->bench().store().modules_with_body(digest)));
    };
    // the stored reconstructions of a function
    services.stored_candidates = [context](std::string_view module_sha, std::string_view addr) -> Json {
        const std::unique_ptr<engine::FunctionStore> store = sy_open_store(context->config());
        Json out = Json::array();
        if (!store) return out;
        for (const engine::Stored& c : store->stored(module_sha, addr)) {
            Json verdicts = Json::array();
            for (const engine::StoredVerdict& v : c.verdicts)
                verdicts.push_back(Json{{"profile", v.profile}, {"verdict", v.verdict ? Json(*v.verdict) : Json(nullptr)}, {"reason", v.reason}, {"limits", v.limits}, {"carried", v.carried}});
            out.push_back(Json{{"engine", c.engine}, {"rev", c.rev ? Json(*c.rev) : Json(nullptr)}, {"name", c.name ? Json(*c.name) : Json(nullptr)}, {"source", c.source}, {"problem", c.problem},
                               {"verdicts", std::move(verdicts)}});
        }
        return out;
    };
    // the ledger of the corpus, and what a running sweep is doing
    services.ledger = [context](bool refresh) { return sy_ledger_json(context->config(), context->corpus(), context->profiles(), refresh); };
    services.progress = [context]() {
        const progress::Snapshot s = progress::snapshot([context](progress::Clock::time_point now) { return sy_read_store(context->config(), context->corpus(), context->profiles(), now); });
        return sy_progress_json(s);
    };
    // the command table
    services.usage = [] { return app::CommandTable().usage(); };
    services.command_names = [] {
        std::vector<std::string> names;
        for (const app::CommandDoc& d : app::command_docs()) names.emplace_back(d.name);
        return names;
    };
    services.command_help = [](std::string_view command) -> std::optional<std::pair<std::string, std::string>> {
        const app::CommandDoc* d = app::find_command_doc(command);
        if (d == nullptr) return std::nullopt;
        return std::pair<std::string, std::string>{"codebreaker " + std::string(d->name), std::string(d->synopsis) + "\n\n" + std::string(d->summary) + "\n"};
    };
    // the source tree
    services.tree_root = [context]() -> std::optional<std::filesystem::path> { return context->corpus().design_dir().parent_path(); };
    services.placement = [context](std::string_view name, const std::filesystem::path& file) -> std::pair<std::string, std::string> {
        const source::placement::Placement placement(
            context->corpus().design_dir().parent_path(),
            source::placement::load_table(context->config().checkout_root() / "codebreaker" / "source" / "placement.json", context->corpus().current()));
        const source::placement::Decision d = placement.resolve(name, file, context->corpus().lane(file));
        return {d.folder, std::string(source::placement::how_name(d.how))};
    };
    // what a file is
    auto registry = std::shared_ptr<platforms::Registry>(sy_registry());
    services.reach_describe = [registry](const std::filesystem::path& p) -> Json {
        coverage::reach::Services reach;
        reach.identify_platform = coverage::reach::identify_with(*registry);
        const coverage::reach::Described d = coverage::reach::describe(p, reach);
        return Json{{"found", d.found}, {"platform", d.platform}, {"lane", d.lane ? Json(*d.lane) : Json(nullptr)}, {"family", d.family}};
    };
    services.java_classes = [](const std::filesystem::path& p) { return static_cast<std::int64_t>(java::corpus::discover(p).size()); };
    services.version = [] { return std::string("2.0.0"); };
}

}  // namespace cb::ui::mcp::wiring
