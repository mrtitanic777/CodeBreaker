#include "source/pipeline.hpp"

#include <algorithm>
#include <set>

#include "build/module.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "decompile/decompiler.hpp"
#include "formats/dos.hpp"
#include "jobs/workers.hpp"
#include "source/pipeline_world.hpp"
#include "source/placement.hpp"
#include "verify/measure.hpp"
#include "verify/sweep.hpp"

namespace cb::source::pipeline {

namespace stdfs = std::filesystem;

World::World(const Environment& environment) : config_(environment.config), kit_(environment.config, environment.corpus) {
    if (environment.workers || engine::hosting_workers()) kit_.set_worker_launcher(jobs::WorkerLauncher::for_this_program());
}

World::~World() = default;

engine::FunctionStore& World::store() {
    if (!store_) store_ = std::make_unique<engine::FunctionStore>(config_.state("functions.sqlite"), config_.state("db"));
    return *store_;
}

const verify::reconstruct::Kinds& World::kinds() {
    if (!kinds_) {
        kinds_.emplace();
        kit_.add_lanes(*kinds_);
    }
    return *kinds_;
}

const verify::reconstruct::Reconstructor& World::reconstructor() {
    if (!reconstructor_) {
        verify::reconstruct::Services services = kit_.reconstruct_services(kinds(), store());
        if (verbose) services.log = [](std::string_view line) { cb::out("{}\n", line); };
        reconstructor_ = std::make_unique<verify::reconstruct::Reconstructor>(std::move(services));
    }
    return *reconstructor_;
}

base::converted::Store World::converted() const {
    return base::converted::Store(config_.path("converted").value_or(config_.checkout_root() / "Converted"));
}

std::optional<stdfs::path> World::find(std::string_view module) {
    std::error_code ec;
    const stdfs::path as_given = fs::from_text(module);
    if (stdfs::is_directory(as_given, ec)) return as_given;   // a folder: a Java program
    return corpus().retail(module);
}

// ---- decompile <module> [--out FILE] ---------------------------------------------------------------------------------------

cli::Command decompile(const Environment& environment) {
    cli::Command declared{"decompile",
                          "decompile any module the corpus or a path names, and write its functions as C",
                          {cli::Positional{"args", "<module> [--out FILE]", false, true}},
                          {}, {}, "Reconstruct", "Nothing is built or judged; `module` does that."};
    declared.passthrough = true;
    return in_world(std::move(declared), environment, [](World& world, const cli::Args& args) {
        const std::vector<std::string> words = args.values("args");
        return decompile::decompiler::command(world.kit().decompiler(), world.corpus(), world.converted(), words);
    });
}

// ---- place <module>... | --all [--list] ---------------------------------------------------------------------------------------

cli::Command place(const Environment& environment) {
    cli::Command declared{"place", "say where each module's source goes in the source tree, and how that was decided",
                          {cli::Positional{"args", "<module>... | --all [--list]", false, true}},
                          {}, {}, "Reconstruct", "Writes nothing but the report."};
    declared.passthrough = true;
    return in_world(std::move(declared), environment, [](World& world, const cli::Args& args) {
        base::Corpus& corpus = world.corpus();
        const stdfs::path table_file = world.config().checkout_root() / "config" / "source" / "placement.json";
        const placement::Placement where(corpus.design_dir().parent_path(), placement::load_table(table_file, corpus.current()));
        const std::vector<std::string> words = args.values("args");
        return placement::place_command(where, corpus, words);
    });
}

// ---- measure <module> ---------------------------------------------------------------------------------------------------------

cli::Command measure(const Environment& environment) {
    cli::Command declared = verify::measure::command(verify::measure::Services{});
    declared.area = "Reconstruct";
    return in_world(std::move(declared), environment, [](World& world, const cli::Args& args) {
        world.verbose = true;
        verify::measure::Services services;
        services.find_module = [&world](std::string_view module) { return world.find(module); };
        // The engine is the only way there is: --no-engine and --engine are read and make no difference.
        services.judge = [&world](const stdfs::path& module, verify::measure::Engine) {
            const verify::reconstruct::Outcome done = world.reconstructor().run(module);
            std::vector<verify::measure::Row> rows;
            for (const engine::Candidate& c : world.store().candidates(done.sha, done.profile)) {
                verify::measure::Row row;
                if (c.verdict) row.verdict = verify::parse_verdict(*c.verdict);
                row.reason = c.reason;
                rows.push_back(std::move(row));
            }
            return rows;
        };
        return verify::measure::command(std::move(services)).handler(args);
    });
}

// ---- sweep [--16 | --drivers | --dos | --all] [--only LIST] [--status] [--again] -------------------------------------------------

namespace {

// The real-mode programs of the corpus, smallest first: one per name, the best copy, of a name with a DOS extension that reads as a program.
std::vector<stdfs::path> dos_programs_of(base::Corpus& corpus) {
    static constexpr std::string_view kExtensions[] = {".exe", ".com", ".sys", ".dos", ".mod", ".new", ".vxd", ".386", ".bin", ".ovl"};
    const auto index = corpus.index();
    std::vector<std::string> names;
    for (const auto& [name, copies] : *index) {
        (void)copies;
        if (std::any_of(std::begin(kExtensions), std::end(kExtensions), [&](std::string_view e) { return name.ends_with(e); })) names.push_back(name);
    }
    std::sort(names.begin(), names.end());
    std::vector<std::pair<std::uintmax_t, stdfs::path>> found;
    for (const std::string& name : names) {
        const stdfs::path& best = index->at(name).front();
        try {
            const Bytes data = fs::read_file(best);
            if (formats::dos::kind_of(data, name)) {
                std::error_code ec;
                found.emplace_back(stdfs::file_size(best, ec), best);
            }
        } catch (const Error&) {   // a file that cannot be read is not a program
        }
    }
    std::stable_sort(found.begin(), found.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    std::vector<stdfs::path> paths;
    for (auto& entry : found) paths.push_back(std::move(entry.second));
    return paths;
}

}  // namespace

cli::Command sweep(const Environment& environment) {
    cli::Command declared{"sweep", "reconstruct and judge every module of the corpus, smallest first, resumably", verify::sweep::params(), {}, {}, "Reconstruct",
                          "A module counts as measured when every one of its functions has a result under the current verifier settings."};
    return in_world(std::move(declared), environment, [environment](World& world, const cli::Args& args) {
        const verify::sweep::Options options = verify::sweep::options_of(args);
        verify::sweep::Services services{.reconstruct = world.reconstructor(), .store = world.store(),
                                         .modules = [&world](std::string_view kind) -> std::vector<stdfs::path> {
                                             base::Corpus& corpus = world.corpus();
                                             if (kind == "le") return corpus.modules(base::Lane::Le, {".vxd", ".386"});
                                             if (kind == "dos") return dos_programs_of(corpus);
                                             if (const auto lane = base::lane_from_name(kind)) return corpus.modules(*lane);
                                             return {};
                                         }};
        services.tools_missing = [&world]() -> std::optional<std::string> {
            if (build::watcom::OpenWatcom::from_settings(world.config().text("openwatcom")).tool("wcc386").empty())
                return "OpenWatcom is not set up (settings.json: openwatcom): nothing could be built";
            return std::nullopt;
        };
        services.out = [](std::string_view line) { cb::out("{}\n", line); };
        services.err = [](std::string_view line) { cb::err("{}\n", line); };
        return verify::sweep::run(options, services);
    });
}

// ---- build <module>... [--out DIR] [--no-run] -------------------------------------------------------------------------------------

cli::Command build(const Environment& environment) {
    build::module::ModuleBuilder declaring{build::module::Services{}};
    cli::Command declared = declaring.command();
    declared.area = "Reconstruct";
    return in_world(std::move(declared), environment, [](World& world, const cli::Args& args) {
        const build::module::ModuleBuilder builder(world.kit().module_services());
        return builder.command().handler(args);
    });
}

std::vector<cli::Command> commands(const Environment& environment) {
    return {decompile(environment), module(environment), scoreboard(environment), build(environment), measure(environment), teeth(environment),
            sweep(environment),     match(environment),  score16(environment),    repair(environment), place(environment)};
}

}  // namespace cb::source::pipeline
