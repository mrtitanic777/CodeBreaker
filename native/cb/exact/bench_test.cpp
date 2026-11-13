// Tests of the inverse model held to a project whose sources are known, on a real project of the research (C 3.00's P0) and a model and a prover that are
// stand-ins (the real ones are the period compiler's, a few seconds a function: their own tests and the exact gate hold them). What bench is held to
// here: which functions it takes (those carve finds that have a one-function source, or the addresses asked for, in address order; one carve does not
// find is left out), what a model that declines and a prover that falls over come to (a row, not a crash), what --show prints beside the source, what
// --out writes, the tally of the verdicts (most common first), and the arguments (an address in hex; the file --out names is not one, whatever its name
// is made of).
//
// DIVERGES from the Python: the source of a function whose proof fell over is "<kind>: <message>" (the Python's was a traceback); the toolchain whose
// profile names no inverse model is a UsageError (the Python's was a KeyError).
#include "cb/exact/bench.hpp"

#include "cb/exact/real_projects.hpp"

using namespace cb;
using namespace cb::exact;

namespace {

// A model that declines the function it is asked for at the place it is told to (0: the first), and otherwise reads it as a function of no statements.
class StandInModel final : public draft::InverseModel {
public:
    StandInModel(std::shared_ptr<int> asked, int refuses) : asked_(std::move(asked)), refuses_(refuses) {}
    draft::ModelCapabilities capabilities() const override { return {}; }
    draft::Recovery recover(ByteView, const draft::Reading&) override {
        if ((*asked_)++ == refuses_) throw UsageError("the model does not read this: a very long reason " + std::string(120, 'x'));
        return {};
    }
    draft::Recovery recover_structured(ByteView, const draft::Reading&, const draft::Structuring&) override { return {}; }
    std::string to_c(std::string_view name, const draft::Recovery&) override { return std::format("{}()\n{{\n}}\n", name); }
    std::vector<std::string> fold_ladders(const std::vector<std::string>& s) override { return s; }
    draft::ModelLast last() const override { return {}; }

private:
    std::shared_ptr<int> asked_;
    int refuses_;
};

struct World {
    test::Real real;
    std::shared_ptr<project::Services> services = std::make_shared<project::Services>();
    std::vector<std::uint32_t> proved;
    std::uint32_t falls_over = 0xFFFFFFFF, differs = 0xFFFFFFFF;
    int refuses = -1;                    // the n'th function the model is asked for (from 0) is the one it declines
    std::shared_ptr<int> asked = std::make_shared<int>(0);
    std::shared_ptr<project::Project> open() {
        *services = *real.services;
        services->inverse_model = [this](std::string_view) -> std::shared_ptr<draft::InverseModel> { return std::make_shared<StandInModel>(asked, refuses); };
        services->prove_texts = [this](draft::InverseModel&, const draft::ModelTexts&, std::uint32_t a, std::uint32_t, const std::map<std::uint32_t, std::string>&) -> draft::ProveOutcome {
            proved.push_back(a);
            if (a == falls_over) throw ToolError("compiler", 1, "the compiler fell over in a way that goes on for a good while yet " + std::string(100, 'y'));
            return {std::format("draft of {:05x}", a), a == differs ? "differs at 3" : "identical"};
        };
        return project::Project::open(test::research_root() / "msc-3.0/recon/p0", services);
    }
};

}  // namespace

CB_TEST("exact.bench.every_function_with_a_source_is_drafted_and_proved_in_address_order", [] {
    World w;
    if (test::skipped(w.real, "bench")) return;
    const auto p = w.open();
    std::vector<std::uint32_t> have;
    {
        std::set<std::uint32_t> carved;
        for (const carve::Function& f : carve::functions(*p)) carved.insert(f.address);
        for (const project::Source& s : p->sources())
            if (s.at >= 0 && carved.contains(static_cast<std::uint32_t>(s.at))) have.push_back(static_cast<std::uint32_t>(s.at));
        std::sort(have.begin(), have.end());
    }
    CB_CHECK(have.size() > 3);
    const std::vector<bench::Row> rows = bench::run(*p);
    CB_CHECK_EQ(rows.size(), have.size());
    for (std::size_t i = 0; i < rows.size(); ++i) {
        CB_CHECK_EQ(rows[i].address, have[i]);
        CB_CHECK_EQ(rows[i].verdict, "identical");
        CB_CHECK_EQ(rows[i].source, std::format("draft of {:05x}", have[i]));
    }
    // the addresses asked for: in address order; one carve does not find is left out
    w.proved.clear();
    const std::vector<bench::Row> some = bench::run(*p, {have[2], have[0], 0xFFFF0});
    CB_CHECK_EQ(some.size(), std::size_t{2});
    CB_CHECK_EQ(some[0].address, have[0]);
    CB_CHECK_EQ(some[1].address, have[2]);
    CB_CHECK(w.proved == (std::vector<std::uint32_t>{have[0], have[2]}));
});

CB_TEST("exact.bench.a_model_that_declines_and_a_prover_that_falls_over_are_rows_not_a_crash", [] {
    World w;
    if (test::skipped(w.real, "bench errors")) return;
    const auto p = w.open();
    const std::vector<bench::Row> all = bench::run(*p);
    w.falls_over = all[1].address;
    w.differs = all[2].address;
    w.refuses = *w.asked;   // (the model declines the first function it is asked for now)
    const std::vector<bench::Row> rows = bench::run(*p);
    CB_CHECK_EQ(rows.size(), all.size());
    CB_CHECK(rows[0].verdict.starts_with("declined: the model does not read this"));   // the reason is cut at 80 bytes
    CB_CHECK_EQ(rows[0].verdict.size(), std::string("declined: ").size() + 80);
    CB_CHECK(rows[0].source.empty());
    CB_CHECK(rows[1].verdict.starts_with("failed: ToolError: "));
    CB_CHECK(rows[1].source.starts_with("ToolError: "));
    CB_CHECK_EQ(rows[2].verdict, "differs at 3");
    // a profile with no inverse model
    World none;
    none.services->toolchains = none.real.services->toolchains;
    CB_CHECK_THROWS_AS(UsageError, (void)bench::run(*project::Project::open(test::research_root() / "lib-2.20", none.real.services)));
});

CB_TEST("exact.bench.the_command_tallies_the_verdicts_and_writes_what_it_is_asked", [] {
    World w;
    if (test::skipped(w.real, "bench command")) return;
    const auto p = w.open();
    const std::vector<bench::Row> all = bench::run(*p);
    w.differs = all[0].address;
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(bench::main(*p, {}), 0);
        CB_CHECK_EQ(cap.out(), std::format("{} functions: {} identical, 1 differs at 3\n", all.size(), all.size() - 1));   // (the most common first)
    }
    // --show: each draft, then the source beside it
    {
        testing::CapturedStd cap;
        bench::main(*p, {std::format("{:x}", all[1].address), "--show"});
        const std::string text = cap.out();
        CB_CHECK(text.starts_with(std::format("== {:05x} ", all[1].address)));
        CB_CHECK(text.find("-- the source:\n") != std::string::npos);
        CB_CHECK(text.find("1 functions: 1 identical\n") != std::string::npos);
    }
    // --out: one line a function; the file's name is not an address, whatever it is made of
    fs::ScopedTempDir dir("w5b3-bench");
    for (const char* name : {"out.txt", "beef", "1234", "a"}) {
        const std::filesystem::path file = dir.path() / name;
        testing::CapturedStd cap;
        bench::main(*p, {"--out", fs::to_text(file), std::format("{:x}", all[0].address)});
        CB_CHECK_EQ(fs::read_text(file), std::format("{:05x} differs at 3\n", all[0].address));
        CB_CHECK_EQ(cap.out(), "1 functions: 1 differs at 3\n");
    }
});
