// `teeth <module>`: the judge asked whether it notices a broken function, over the lanes the kit makes (verify/teeth is the breaking and the report;
// this is the lane it breaks: a kind's prepared module, built again with the broken texts and judged).
#include <memory>

#include "cb/core/parallel.hpp"
#include "cb/source/pipeline_world.hpp"
#include "cb/verify/teeth.hpp"

namespace cb::source::pipeline {

namespace stdfs = std::filesystem;

namespace {

// One kind of module as teeth breaks it: the C of each function as the kind prepared it, built with some replaced and judged.
class TeethLane final : public verify::teeth::Lane {
public:
    TeethLane(const verify::lanes::Kind& kind, stdfs::path module, stdfs::path work, const std::map<std::string, verify::Class>& passed)
        : kind_(kind), module_(std::move(module)), work_(std::move(work)), prepared_(kind.prepare(module_, work_)) {
        for (const std::string& name : prepared_->order) {
            verify::teeth::Function f;
            f.name = name;
            f.text = prepared_->texts.at(name);
            const auto key = prepared_->key.find(name);
            if (key != prepared_->key.end()) {
                const auto verdict = passed.find(key->second);
                if (verdict != passed.end()) f.passed = verdict->second;
            }
            functions_.push_back(std::move(f));
        }
    }

    std::string kind() const override { return kind_.name(); }
    std::string module_name() const override { return text::lower(fs::to_text(module_.filename())); }
    const std::vector<verify::teeth::Function>& functions() const override { return functions_; }

    std::vector<verify::teeth::Said> judge(std::span<const verify::teeth::Replacement> broken) override {
        // The module as it was, with the broken functions' texts in place of their own for the length of the build (the prepared module is the
        // kind's own type, which a copy would cut down to the common part).
        for (const verify::teeth::Replacement& r : broken) prepared_->set_text(functions_[r.function].name, r.text);
        const verify::lanes::Built built = kind_.build(*prepared_, module_, work_ / ("round" + std::to_string(round_number_++)));
        for (const verify::teeth::Replacement& r : broken) prepared_->set_text(functions_[r.function].name, functions_[r.function].text);
        std::vector<verify::teeth::Said> said(broken.size());
        parallel::parallel_for(broken.size(), [&](std::size_t i) {
            const std::string& name = functions_[broken[i].function].name;
            if (std::find(built.items.begin(), built.items.end(), name) == built.items.end()) {   // dropped: not built, and why
                const auto why = built.missing.find(name);
                said[i].reason = why == built.missing.end() ? "" : why->second;
                return;
            }
            const verify::Judgement j = built.judge(name);
            said[i].verdict = j.verdict;
            said[i].reason = j.reason;
            said[i].limits = j.limits;
        });
        return said;
    }

private:
    const verify::lanes::Kind& kind_;
    stdfs::path module_, work_;
    std::unique_ptr<verify::lanes::Prepared> prepared_;
    std::vector<verify::teeth::Function> functions_;
    int round_number_ = 0;
};

}  // namespace

cli::Command teeth(const Environment& environment) {
    cli::Command declared = verify::teeth::command(verify::teeth::Services{});
    declared.area = "Reconstruct";
    return in_world(std::move(declared), environment, [](World& world, const cli::Args& args) {
        verify::teeth::Services services;
        services.report_dir = world.config().state("teeth");
        // a file as given or a corpus name; a .xex as the file the lanes read (its basefile)
        services.find_module = [&world](std::string_view module) { return world.corpus().retail(module); };
        services.open = [&world](const stdfs::path& module) -> std::unique_ptr<verify::teeth::Lane> {
            const verify::reconstruct::Reconstructor& rec = world.reconstructor();
            const verify::lanes::Kind& kind = rec.kind_of(module);
            if (!world.kinds().traits(kind.name()).per_function)
                throw UsageError(fs::to_text(module.filename()) + ": this kind of module is rebuilt whole and compared byte for byte; there is no judge "
                                                                   "of behaviour to test");
            const verify::reconstruct::Outcome done = rec.run(module);   // make sure it has been judged
            std::map<std::string, verify::Class> passed;
            for (const auto& [address, best] : world.store().best(done.sha, done.profile))
                if (best.cls == verify::Class::Verified || best.cls == verify::Class::Weak) passed.emplace(address, best.cls);
            const stdfs::path work = world.kit().scratch() / "teeth" / kind.name() / text::lower(fs::to_text(module.filename()));
            return std::make_unique<TeethLane>(kind, module, work, passed);
        };
        return verify::teeth::command(std::move(services)).handler(args);
    });
}

}  // namespace cb::source::pipeline
