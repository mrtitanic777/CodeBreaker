#include "exact/command.hpp"

#include <algorithm>
#include <memory>
#include <optional>

#include "base/cache.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "exact/carve.hpp"
#include "exact/draft/bind.hpp"
#include "exact/draft/early_c_model.hpp"
#include "exact/emu/native_dosexec.hpp"
#include "exact/habits/newcomp.hpp"
#include "exact/habits/oldcomp.hpp"
#include "exact/hooks_lattice.hpp"
#include "exact/linkwhole.hpp"
#include "exact/mspascal.hpp"
#include "exact/plm86.hpp"
#include "exact/project.hpp"
#include "exact/readable.hpp"
#include "exact/run.hpp"
#include "exact/seed.hpp"
#include "exact/toolchain.hpp"
#include "exact/xenix_c.hpp"
#include "formats/zip.hpp"

namespace cb::exact::command {

namespace {

namespace stdfs = std::filesystem;

// The DOS executive as the linker's host: the program runs in `workdir` on the disk, its console text comes back.
class ExecutiveHost final : public linkwhole::DosHost {
public:
    explicit ExecutiveHost(const emu::dos::Executive& executive) : executive_(executive) {}
    std::string run(const stdfs::path& exe, const stdfs::path& workdir, std::string_view command_line, std::optional<std::uint32_t> psp) const override {
        emu::dos::RunSpec spec;
        spec.exe = exe;
        spec.workdir = workdir;
        spec.command_tail = " " + std::string(command_line);
        if (psp) spec.psp = static_cast<std::uint16_t>(*psp);
        const emu::dos::Result result = executive_.run(spec);
        return std::string(result.console.begin(), result.console.end());
    }

private:
    const emu::dos::Executive& executive_;
};

// What a stage that needs the DOS executive says when there is none.
class MissingHost final : public linkwhole::DosHost {
public:
    explicit MissingHost(std::string why) : why_(std::move(why)) {}
    std::string run(const stdfs::path&, const stdfs::path&, std::string_view, std::optional<std::uint32_t>) const override {
        throw UnsupportedError("exact", 0, "the DOS executive is not available: " + why_);
    }

private:
    std::string why_;
};

// The pass of a plain toolchain when there is no executive: the first thing a compile asks.
class MissingRunner final : public toolchain::PassRunner {
public:
    explicit MissingRunner(std::string why) : why_(std::move(why)) {}
    emu::dos::Result run(const toolchain::PassRun&) const override {
        throw UnsupportedError("exact", 0, "the DOS executive is not available: " + why_);
    }

private:
    std::string why_;
};

habits::cmpp::Options entry_options(std::uint32_t helper_data) {
    habits::cmpp::Options o;
    o.entry_data = helper_data == 2 ? habits::cmpp::EntryData::Word : habits::cmpp::EntryData::ByteAndWord;
    return o;
}

// Everything the stages stand on, made in the order it is needed and let go in reverse.
class Surroundings {
public:
    explicit Surroundings(const Environment& environment) {
        const config::Config& config = environment.config;
        locations_ = toolchain::Locations::of_checkout(config.checkout_root());
        locations_.state = config.state();

        emu::native_dosexec::Opened opened = emu::native_dosexec::open(emu::native_dosexec::locate(config), emu::native_dosexec::limits_from_environment());
        if (opened.usable()) {
            executive_ = std::move(opened.executive);
            runner_ = std::make_unique<toolchain::ExecutiveRunner>(*executive_);
            host_ = std::make_unique<ExecutiveHost>(*executive_);
        } else {
            const std::string why = opened.why.empty() ? "it is not set up" : opened.why.front();
            runner_ = std::make_unique<MissingRunner>(why);
            host_ = std::make_unique<MissingHost>(why);
        }
        base::cache::Config cache_config;
        cache_config.state_dir = config.state("cache");
        cache_config.shared_dir = config.path("build_cache").value_or(config.state("build_cache"));
        cache_config.disabled = base::cache::Config::disabled_by_environment();
        cache_ = std::make_unique<base::cache::Cache>(std::move(cache_config));

        toolchains_ = std::make_unique<toolchain::Toolchains>(locations_, runner_.get(), cache_.get());
        toolchains_->register_hook("codebreaker.exact.hooks_lattice:cmp_test",
                                   [](const toolchain::WordReader& read_word, std::int64_t bp, std::int64_t ss, std::int64_t ds) -> std::optional<std::int64_t> {
                                       return hooks_lattice::cmp_test(
                                           [&read_word](std::uint32_t segment, std::uint32_t offset) { return static_cast<std::uint16_t>(read_word(segment, offset)); },
                                           static_cast<std::uint32_t>(bp), static_cast<std::uint32_t>(ss), static_cast<std::uint32_t>(ds));
                                   });
        plm86::register_plugin(*toolchains_);
        xenix_c::register_plugin(*toolchains_);
        mspascal::Hooks pascal_hooks;
        pascal_hooks.extract_zip = [](const stdfs::path& zip, const stdfs::path& dest) {
            const formats::ZipArchive archive = formats::ZipArchive::open(zip);
            for (const formats::ZipArchive::Entry& entry : archive.entries()) {
                if (entry.is_directory()) continue;
                fs::write_file(dest / fs::from_text(entry.name), archive.read(entry));
            }
        };
        mspascal::register_plugin(*toolchains_, std::move(pascal_hooks));

        services_ = std::make_shared<project::Services>();
        services_->toolchains = toolchains_.get();
        services_->carve = carve::service();
        // the draft stage: the inverse model the profile names, and the prover bound to each project (its compiles, judge, aim and rewrites)
        services_->inverse_model = draft::inverse_model_named;
        services_->prover_for = draft::bound_prover;
        // the comparers a project's "equivalent" names: the habits of another state of the compiler
        services_->equivalents["codebreaker.exact.habits.newcomp:equivalent"] = [](ByteView want, ByteView got, const std::set<std::uint32_t>& holes, std::uint32_t helper_data) {
            const habits::newcomp::Finding f = habits::newcomp::equivalent(want, got, holes, entry_options(helper_data));
            return pascal_join::Equivalence{f.same, f.notes};
        };
        services_->equivalents["codebreaker.exact.habits.newcomp:equivalent3"] = [](ByteView want, ByteView got, const std::set<std::uint32_t>& holes, std::uint32_t helper_data) {
            const habits::newcomp::Finding f = habits::newcomp::equivalent3(want, got, holes, entry_options(helper_data));
            return pascal_join::Equivalence{f.same, f.notes};
        };
        services_->equivalents["codebreaker.exact.habits.oldcomp:equivalent"] = [](ByteView want, ByteView got, const std::set<std::uint32_t>& holes, std::uint32_t) {
            const habits::oldcomp::Verdict v = habits::oldcomp::equivalent(want, got, std::set<std::size_t>(holes.begin(), holes.end()));
            return pascal_join::Equivalence{v.equivalent, v.equivalent ? v.found : std::vector<std::string>{v.stopped}};
        };

        stages_ = run::standard_stages(host_.get());
        stages_.add("seed", [](project::Project& p, const std::vector<std::string>& a) { return seed::run(p, a); });
        stages_.add("readable", [](project::Project& p, const std::vector<std::string>& a) {
            readable::Options options;
            options.write = std::ranges::find(a, "--write") != a.end();
            return readable::run(*p.as_readable(), options);
        });
        stages_.add("draft", [](project::Project& p, const std::vector<std::string>& a) {
            draft::Options options;
            options.every = std::ranges::find(a, "--all") != a.end();
            options.verbose = std::ranges::find(a, "-v") != a.end();
            const auto limit = std::ranges::find(a, "--limit");
            if (limit != a.end() && limit + 1 != a.end()) {
                const auto n = text::parse_int<std::size_t>(*(limit + 1), 10);
                if (!n) throw UsageError("--limit expects a number, got '" + *(limit + 1) + "'");
                options.limit = n;
            }
            return draft::run_command(*p.as_draft(), options);
        });
    }

    int run(std::span<const std::string> args) {
        return run::main(std::vector<std::string>(args.begin(), args.end()), stages_, *toolchains_, services_);
    }

private:
    toolchain::Locations locations_;
    std::unique_ptr<emu::dos::Executive> executive_;
    std::unique_ptr<toolchain::PassRunner> runner_;
    std::unique_ptr<linkwhole::DosHost> host_;
    std::unique_ptr<base::cache::Cache> cache_;
    std::unique_ptr<toolchain::Toolchains> toolchains_;
    std::shared_ptr<project::Services> services_;
    run::Stages stages_;
};

}  // namespace

int run(std::span<const std::string> args, const Environment& environment) {
    Surroundings world(environment);
    return world.run(args);
}

std::function<int(std::span<const std::string>)> runner(const Environment& environment) {
    return [environment](std::span<const std::string> args) { return run(args, environment); };
}

}  // namespace cb::exact::command
