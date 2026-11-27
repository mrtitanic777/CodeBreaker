// Tests of Convert's judging stage (convert_judge.hpp) over stand-in judges: where the stage comes, what it shows while it works, the classes each
// function of the C takes, the counts it notes, a program no judge can run, a judge that fails, and a cancel.
#include "app/convert_judge.hpp"

#include <algorithm>
#include <atomic>

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::app::convert {
namespace {

namespace stdfs = std::filesystem;
using store::Class;
using verify::reconstruct::Step;

// A route that decompiles: writes prog.c with three functions, as a decompiler names them.
Route cj_route() {
    Route route;
    route.name = "test";
    route.stages = {State::Opening, State::Decompiling};
    route.run = [](Reporter& r, const stdfs::path&, const stdfs::path& work) {
        r.enter(State::Opening);
        r.enter(State::Decompiling);
        fs::write_text(work / "prog.c",
                       "int sub_140001000(int a)\n{\n    return a + 1;\n}\n\n"
                       "void WinMainCRTStartup(void)\n{\n}\n\n"
                       "int helper(int a) {\n    return a;\n}\n");
    };
    return route;
}

Planner cj_planner(Route route) {
    return [route](const stdfs::path&) {
        Plan p;
        p.what = "test program";
        p.route = route;
        return p;
    };
}

// A judge that tells the steps and answers with what it is given.
Judge cj_judge(source::pipeline::Judging answer, std::size_t judged = 4) {
    return [answer, judged](const stdfs::path&, const source::pipeline::JudgeSteps& steps) {
        steps.progress(Step::Preparing, 0, 0);
        steps.progress(Step::Building, 0, judged);
        steps.progress(Step::Judging, 0, judged);
        for (std::size_t i = 1; i <= judged; ++i) {
            if (steps.stop && steps.stop()) throw verify::reconstruct::Stopped();
            steps.progress(Step::Judging, i, judged);
        }
        return answer;
    };
}

source::pipeline::JudgedFunction cj_fn(std::string key, std::string name, Class cls) {
    source::pipeline::JudgedFunction f;
    f.key = std::move(key);
    f.name = std::move(name);
    f.classification.cls = cls;
    return f;
}

source::pipeline::Judging cj_answer() {
    source::pipeline::Judging j;
    j.judged = true;
    j.lane = "win64";
    j.outcome.functions = 4;
    j.outcome.scores = {{Class::Verified, 1}, {Class::Weak, 1}, {Class::Unverified, 1}, {Class::NoCompile, 1}, {Class::NoSource, 0}};
    j.functions = {cj_fn("140001000", "sub_140001000", Class::Verified),       // by name
                   cj_fn("140001020", "", Class::Weak),                        // a function the C does not name: added after them
                   cj_fn("140001040", "WinMainCRTStartup", Class::Unverified),
                   cj_fn("140001060", "helper", Class::NoCompile)};
    return j;
}

struct CjFixture {
    fs::ScopedTempDir dir{"cb_convert_judge"};
    stdfs::path input = dir.path() / "prog.exe";
    std::vector<Progress> seen;
    std::atomic<bool> cancel{false};
    CjFixture() { fs::write_file(input, Bytes{'M', 'Z', 0, 0}); }
    Request request() const { return Request{.input = input, .out_dir = std::nullopt, .converted_root = dir.path() / "Converted"}; }
    ProgressFn listener() {
        return [this](const Progress& p) { seen.push_back(p); };
    }
};

const FunctionRecord* cj_find(const std::vector<FunctionRecord>& list, std::string_view name) {
    const auto it = std::find_if(list.begin(), list.end(), [&](const FunctionRecord& r) { return r.name == name; });
    return it == list.end() ? nullptr : &*it;
}

CB_TEST("app.convert_judge.checking_comes_after_decompiling_and_before_writing", [] {
    CjFixture f;
    const Outcome done = convert(f.request(), cj_planner(judged(cj_route(), cj_judge(cj_answer()))), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Done);
    const std::vector<State> expected = {State::Identifying, State::Opening, State::Decompiling, State::Checking, State::Writing, State::Done};
    CB_CHECK(done.stages == expected);
    int last = 0;
    for (const Progress& p : f.seen) {
        CB_CHECK(p.percent >= last);
        last = p.percent;
    }
});

CB_TEST("app.convert_judge.the_stage_says_each_step_and_counts_only_the_functions_judged", [] {
    CjFixture f;
    (void)convert(f.request(), cj_planner(judged(cj_route(), cj_judge(cj_answer()))), f.listener(), f.cancel);
    bool preparing = false, building = false, counted = false;
    int at_start = -1, at_end = -1;
    for (const Progress& p : f.seen) {
        if (p.state != State::Checking) continue;
        if (at_start < 0) at_start = p.percent;
        at_end = p.percent;
        preparing = preparing || (p.detail == "Making the C ready to build" && p.total == 0);
        building = building || (p.detail == "Building 4 functions" && p.total == 0);
        counted = counted || (p.detail == "Running each beside the original: 3 of 4 functions" && p.done == 3 && p.total == 4);
        CB_CHECK_EQ(p.label, "Checking");
    }
    CB_CHECK(preparing && building && counted);
    CB_CHECK(at_end > at_start);   // (the count moves the bar inside the stage)
});

CB_TEST("app.convert_judge.every_function_of_the_c_takes_its_class_and_the_counts_are_noted", [] {
    CjFixture f;
    const Outcome done = convert(f.request(), cj_planner(judged(cj_route(), cj_judge(cj_answer()))), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Done);
    CB_CHECK_EQ(done.checked, "4 functions judged in the win64 lane: 1 verified, 1 weak, 1 unverified, 1 does not compile, 0 no source");
    const FunctionRecord* first = cj_find(done.functions, "sub_140001000");
    CB_CHECK(first != nullptr && first->judged == Class::Verified && first->verdict == FunctionVerdict::Matches);
    CB_CHECK(first != nullptr && first->source_file == "prog.c" && first->source_line == 1);
    const FunctionRecord* start = cj_find(done.functions, "WinMainCRTStartup");
    CB_CHECK(start != nullptr && start->judged == Class::Unverified && start->verdict == FunctionVerdict::Differs && start->address == "0x140001040");
    const FunctionRecord* helper = cj_find(done.functions, "helper");
    CB_CHECK(helper != nullptr && helper->judged == Class::NoCompile && helper->verdict == FunctionVerdict::NotChecked);
    const FunctionRecord* unnamed = cj_find(done.functions, "sub_140001020");   // judged, not named in the C: listed after its functions
    CB_CHECK(unnamed != nullptr && unnamed->judged == Class::Weak && unnamed->verdict == FunctionVerdict::Matches && unnamed->source_file.empty());
    CB_CHECK_EQ(done.functions.size(), 4u);
});

CB_TEST("app.convert_judge.a_program_no_judge_can_run_is_done_and_says_why", [] {
    CjFixture f;
    source::pipeline::Judging none;
    none.why_not = "The tools of the win64 judge are not set up on this computer (Settings, Advanced).";
    const Outcome done = convert(f.request(), cj_planner(judged(cj_route(), cj_judge(none, 0))), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Done);
    CB_CHECK_EQ(done.checked, "Not judged. " + none.why_not);
    CB_CHECK(done.functions.empty());
    CB_CHECK(std::find(done.stages.begin(), done.stages.end(), State::Checking) != done.stages.end());
});

CB_TEST("app.convert_judge.a_judge_that_fails_leaves_the_conversion_done_and_unjudged", [] {
    CjFixture f;
    const Judge failing = [](const stdfs::path&, const source::pipeline::JudgeSteps&) -> source::pipeline::Judging {
        throw std::runtime_error("the linker gave up");
    };
    const Outcome done = convert(f.request(), cj_planner(judged(cj_route(), failing)), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Done);
    CB_CHECK_EQ(done.checked, "Not judged. The judge stopped: the linker gave up");
    CB_CHECK(stdfs::exists(done.output / "prog.c"));
});

CB_TEST("app.convert_judge.a_cancel_while_judging_stops_the_judge_and_leaves_nothing", [] {
    CjFixture f;
    ProgressFn cancelling = [&f](const Progress& p) {
        if (p.state == State::Checking && p.done == 2) f.cancel = true;
    };
    std::size_t asked_after = 0;
    const Judge judge = [&asked_after](const stdfs::path&, const source::pipeline::JudgeSteps& steps) -> source::pipeline::Judging {
        for (std::size_t i = 1; i <= 10; ++i) {
            if (steps.stop()) throw verify::reconstruct::Stopped();
            steps.progress(Step::Judging, i, 10);
            if (i >= 2) ++asked_after;
        }
        return cj_answer();
    };
    const Outcome done = convert(f.request(), cj_planner(judged(cj_route(), judge)), cancelling, f.cancel);
    CB_CHECK(done.state == State::Cancelled);
    CB_CHECK_EQ(asked_after, 1u);   // (the next function was not judged)
    CB_CHECK(done.output.empty());
    CB_CHECK(!stdfs::exists(f.dir.path() / "Converted" / "prog"));
});

CB_TEST("app.convert_judge.the_words_for_each_step", [] {
    CB_CHECK_EQ(judge_words(Step::Preparing, 0, 0), "Making the C ready to build");
    CB_CHECK_EQ(judge_words(Step::Building, 0, 1), "Building 1 function");
    CB_CHECK_EQ(judge_words(Step::Judging, 12, 300), "Running each beside the original: 12 of 300 functions");
});

}  // namespace
}  // namespace cb::app::convert
