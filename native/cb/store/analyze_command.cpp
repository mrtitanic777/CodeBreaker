#include "cb/store/analyze_command.hpp"

#include <memory>
#include <optional>

#include "cb/analyze/facts.hpp"
#include "cb/analyze/report.hpp"
#include "cb/decompile/decompiler.hpp"
#include "cb/decompile/environment.hpp"
#include "cb/engine/function_store.hpp"
#include "cb/engine/lane_kit.hpp"

namespace cb::store::analyze_command {

namespace {

// What the report is given, made from the kit: the function store is opened for writing when the first module is noted in it.
class Surroundings {
public:
    explicit Surroundings(const Environment& environment)
        : config_(environment.config), kit_(environment.config, environment.corpus), modules_(kit_.corpus()),
          store_(config_.state("functions.sqlite"), config_.state("db")) {}

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
        e.knowledge = &kit_.decompiler().context().environment->knowledge();
        e.export_name = [this](std::string_view module, std::uint32_t ordinal) -> std::optional<std::string> {
            const auto table = modules_.table(module);
            if (!table) return std::nullopt;
            const auto found = table->names.find(ordinal);
            return found == table->names.end() ? std::nullopt : std::optional<std::string>(found->second);
        };
        return e;
    }

private:
    config::Config config_;
    engine::LaneKit kit_;
    decompile::environment::CorpusModules modules_;
    engine::FunctionStore store_;
};

}  // namespace

cli::Command command(const Environment& environment) {
    cli::Command declared = analyze::report::command(analyze::report::Environment{});
    declared.handler = [environment](const cli::Args& args) {
        Surroundings world(environment);
        return analyze::report::command(world.report_environment()).handler(args);
    };
    return declared;
}

}  // namespace cb::store::analyze_command
