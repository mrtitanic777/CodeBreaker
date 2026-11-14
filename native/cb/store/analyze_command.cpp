#include "cb/store/analyze_command.hpp"

#include <memory>
#include <optional>

#include "cb/analyze/facts.hpp"
#include "cb/analyze/observe.hpp"
#include "cb/analyze/report.hpp"
#include "cb/decompile/decompiler.hpp"
#include "cb/decompile/environment.hpp"
#include "cb/engine/function_store.hpp"
#include "cb/engine/lane_kit.hpp"
#include "cb/engine/lane_kit_internal.hpp"
#include "cb/symbols/driver_services.hpp"

namespace cb::store::analyze_command {

namespace {

// The kit's knowledge with a driver's service calls named by the driver kit's tables (the kit's own knows the other kinds of name): a call of
// service 0x0123 of device 1 is `VMM_Get_Cur_VM_Handle`, not `vxd_1_123`.
class DriverNames final : public decompile::describe::Knowledge {
public:
    DriverNames(decompile::describe::Knowledge& base, symbols::driver_services::Table table) : base_(base), table_(std::move(table)) {}
    std::optional<std::int64_t> win16_argument_bytes(std::string_view api) override { return base_.win16_argument_bytes(api); }
    std::optional<std::int64_t> win32_argument_bytes(std::string_view module_stem, std::string_view api) override { return base_.win32_argument_bytes(module_stem, api); }
    std::optional<std::int64_t> declared_argument_bytes(std::string_view name) override { return base_.declared_argument_bytes(name); }
    std::optional<std::string> import_name(std::string_view module, std::uint32_t ordinal) override { return base_.import_name(module, ordinal); }
    std::string driver_service_label(std::uint32_t device, std::uint32_t service) override { return table_.label(device, service); }
    std::set<std::string> imported_names(const std::filesystem::path& module) override { return base_.imported_names(module); }
    Conventions conventions(const std::filesystem::path& module) override { return base_.conventions(module); }
    std::optional<std::int64_t> pe_function_pop(const std::filesystem::path& module, std::uint32_t rva) override { return base_.pe_function_pop(module, rva); }

private:
    decompile::describe::Knowledge& base_;
    symbols::driver_services::Table table_;
};

// What the report is given, made from the kit: the function store is opened for writing when the first module is noted in it.
class Surroundings {
public:
    explicit Surroundings(const Environment& environment)
        : config_(environment.config), kit_(environment.config, environment.corpus), modules_(kit_.corpus()),
          store_(config_.state("functions.sqlite"), config_.state("db")), sizes_(kit_.corpus(), kit_.knowledge()), workers_(environment.workers) {}

    analyze::report::Environment report_environment() {
        analyze::report::Environment e;
        base::Corpus& corpus = kit_.corpus();
        e.retail = [&corpus](std::string_view name) -> std::optional<std::filesystem::path> { return corpus.retail(name); };
        e.sha = [&corpus](const std::filesystem::path& path) { return corpus.sha(path); };
        e.add_module = [this](std::string_view sha, std::string_view name, const std::filesystem::path& path, std::string_view kind) {
            store_.add_module(sha, name, fs::to_text(path), kind);
        };
        e.facts_file = analyze::facts::default_path(config_);
        e.state = config_.state();
        names_ = std::make_unique<DriverNames>(kit_.decompiler().context().environment->knowledge(), symbols::driver_services::table_of(kit_.knowledge().store()));
        e.knowledge = names_.get();
        e.export_name = [this](std::string_view module, std::uint32_t ordinal) -> std::optional<std::string> {
            const auto table = modules_.table(module);
            if (!table) return std::nullopt;
            const auto found = table->names.find(ordinal);
            return found == table->names.end() ? std::nullopt : std::optional<std::string>(found->second);
        };
        // observe: the emulator finds the corpus's modules and the code says how many arguments an import pops (the headers' prototypes are not read:
        // the runs do not use them, and reading the SDK's headers costs more than the runs)
        e.emulator.resolve = [&corpus](std::string_view name) { return corpus.resolve(name); };
        e.import_argc = [this](const emulate::native_trace::ImportName& import) { return sizes_.argc(import.dll, import.name); };
        e.workers = workers_;
        return e;
    }

private:
    std::optional<jobs::WorkerLauncher> workers_;
    config::Config config_;
    engine::LaneKit kit_;
    decompile::environment::CorpusModules modules_;
    engine::FunctionStore store_;
    engine::kit_detail::KitSizes sizes_;
    std::unique_ptr<DriverNames> names_;
};

}  // namespace

void register_worker_kinds(jobs::WorkerKinds& kinds, const Environment& environment) {
    // A worker makes the surroundings once and keeps them for the life of its process: observe's Setting holds functions of them.
    analyze::observe::register_worker_kinds(kinds, [environment](analyze::observe::Setting& setting) {
        static const std::unique_ptr<Surroundings> world = std::make_unique<Surroundings>(environment);
        analyze::report::Environment e = world->report_environment();
        setting.environment = e.emulator;
        setting.argc = e.import_argc;
    });
}

cli::Command command(const Environment& environment) {
    cli::Command declared = analyze::report::command(analyze::report::Environment{});
    declared.handler = [environment](const cli::Args& args) {
        Surroundings world(environment);
        return analyze::report::command(world.report_environment()).handler(args);
    };
    return declared;
}

}  // namespace cb::store::analyze_command
