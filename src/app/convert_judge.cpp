// app/convert_judge.cpp -- see convert_judge.hpp.
#include "app/convert_judge.hpp"

#include <format>
#include <mutex>

#include "app/function_source.hpp"
#include "base/config.hpp"
#include "engine/lane_kit.hpp"

namespace cb::app::convert {

namespace stdfs = std::filesystem;

Judge standard_judge() {
    return [](const stdfs::path& input, const source::pipeline::JudgeSteps& steps) {
        return source::pipeline::judge(source::pipeline::Environment{.config = config::current(), .corpus = nullptr, .workers = engine::hosting_workers()}, input,
                                       steps);
    };
}

std::string judge_words(verify::reconstruct::Step step, std::size_t done, std::size_t total) {
    switch (step) {
        case verify::reconstruct::Step::Preparing: return "Making the C ready to build";
        case verify::reconstruct::Step::Building: return std::format("Building {} {}", total, total == 1 ? "function" : "functions");
        case verify::reconstruct::Step::Judging: break;
    }
    return std::format("Running each beside the original: {} of {} functions", done, total);
}

void judge_stage(Reporter& r, const stdfs::path& input, const stdfs::path& work, const Judge& judge) {
    r.enter(State::Checking);
    // The judge tells from its threads: the reporter is told one thing at a time, and a cancel it reports is kept (the judge must not be thrown through).
    std::mutex telling;
    bool cancelled = false;
    source::pipeline::JudgeSteps steps;
    steps.progress = [&](verify::reconstruct::Step step, std::size_t done, std::size_t total) {
        const std::lock_guard hold(telling);
        if (cancelled) return;
        const bool counting = step == verify::reconstruct::Step::Judging;   // (preparing and building have no count: the bar stays)
        try {
            r.count(counting ? done : 0, counting ? total : 0, judge_words(step, done, total));
        } catch (const Cancelled&) {
            cancelled = true;
        }
    };
    steps.stop = [&] {
        const std::lock_guard hold(telling);
        if (!cancelled) {
            try {
                r.check();
            } catch (const Cancelled&) {
                cancelled = true;
            }
        }
        return cancelled;
    };
    source::pipeline::Judging judging;
    try {
        judging = judge(input, steps);
    } catch (const verify::reconstruct::Stopped&) {
        throw Cancelled();
    } catch (const Cancelled&) {
        throw;
    } catch (const std::exception& e) {   // the judge failing whole: the conversion stands, unjudged
        judging = {};
        judging.why_not = std::string("The judge stopped: ") + e.what();
    }
    if (cancelled) throw Cancelled();
    r.check();
    if (!judging.judged) {
        r.note("Not judged. " + judging.why_not);
        return;
    }
    std::vector<fnsrc::Judged> classes;
    classes.reserve(judging.functions.size());
    for (const source::pipeline::JudgedFunction& f : judging.functions) classes.push_back({f.key, f.name, f.classification.cls});
    r.functions(fnsrc::with_judged(fnsrc::scan_folder(work), classes));
    const std::string counts = fnsrc::judged_counts(judging.outcome.scores, static_cast<std::int64_t>(judging.outcome.functions), judging.lane);
    r.note(counts);
    r.count(1, 1, counts);
}

Route judged(Route route, Judge judge) {
    route.stages.push_back(State::Checking);
    if (!route.weights.contains(State::Checking)) route.weights[State::Checking] = 60;
    route.run = [run = std::move(route.run), judge = std::move(judge)](Reporter& r, const stdfs::path& input, const stdfs::path& work) {
        run(r, input, work);
        judge_stage(r, input, work, judge);
    };
    return route;
}

}  // namespace cb::app::convert
