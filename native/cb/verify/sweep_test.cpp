#include "cb/verify/sweep.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

// Design notes:
//   * the command's pieces are `params`, `options_of` and `run` over `Services` (the corpus listing, the build tools check and the
//     two output streams are given), not an argv walked by hand; `--only` takes the same two forms;
//   * a module that fails outright is reported with the error's text, not "<TypeName>: text";
//   * a file nothing reconstructs is reported as FAILED and counted.

using namespace cb;
using namespace cb::verify;

namespace {

// A lane whose module "bad.fk" cannot be prepared.
class SweepLane final : public lanes::Kind {
public:
    const std::string& name() const override {
        static const std::string kName = "fake";
        return kName;
    }
    lanes::Profile profile(const std::filesystem::path&) const override { return {"d1", "fake settings"}; }
    std::unique_ptr<lanes::Prepared> prepare(const std::filesystem::path& module, const std::filesystem::path&) const override {
        if (module.filename() == "bad.fk")
            throw FormatError("fake", 0, "not a module");
        auto p = std::make_unique<lanes::Prepared>();
        p->header = "h";
        p->order = {"f"};
        p->key["f"] = "key:" + module.filename().generic_string();
        p->all_keys = {p->key["f"]};
        p->texts["f"] = "int f(void) { return 1; }";
        p->sig = "whole";
        return p;
    }
    lanes::Built build(const lanes::Prepared& p, const std::filesystem::path&, const std::filesystem::path&) const override {
        lanes::Built b;
        b.items = p.order;
        b.judge = [](const std::string&) {
            Judgement j;
            j.verdict = Verdict::Equivalent;
            return j;
        };
        return b;
    }
};

struct Sweeping {
    fs::ScopedTempDir tmp{"cb-sweep"};
    engine::FunctionStore store{tmp.path() / "functions.sqlite", tmp.path() / "db"};
    reconstruct::Kinds kinds;
    std::optional<reconstruct::Reconstructor> rec;
    std::vector<std::string> out, err;
    std::optional<sweep::Services> services;

    Sweeping() {
        kinds.add(std::make_shared<SweepLane>());
        reconstruct::Services s{.kinds = kinds, .store = store};
        s.kind_of_file = [](const std::filesystem::path& p) -> std::optional<std::string> {
            return p.extension() == ".fk" ? std::optional<std::string>("fake") : std::nullopt;
        };
        s.content_sha = [](const std::filesystem::path& p) { return std::optional<std::string>("sha-" + p.filename().generic_string()); };
        s.scratch = tmp.path() / "work";
        s.decompiler_rev = "1";
        rec.emplace(std::move(s));
        services.emplace(sweep::Services{.reconstruct = *rec, .store = store});
        services->modules = [](std::string_view kind) {
            return kind == "pe" ? std::vector<std::filesystem::path>{"a.fk", "bad.fk", "b.fk", "c.txt"} : std::vector<std::filesystem::path>{};
        };
        services->out = [this](std::string_view line) { out.emplace_back(line); };
        services->err = [this](std::string_view line) { err.emplace_back(line); };
    }
};

}  // namespace

CB_TEST("verify.sweep.names_come_from_a_list_or_a_file", [] {
    CB_CHECK(sweep::wanted_names("A.dll, b.DLL ,,c.exe") == (std::set<std::string>{"a.dll", "b.dll", "c.exe"}));
    fs::ScopedTempDir tmp("cb-sweep");
    fs::write_text(tmp.path() / "list.txt", "one.dll\r\n\r\n  Two.DLL  \n");
    CB_CHECK(sweep::wanted_names("@" + fs::to_text(tmp.path() / "list.txt")) == (std::set<std::string>{"one.dll", "two.dll"}));
    CB_CHECK_THROWS_AS(IoError, (void)sweep::wanted_names("@" + fs::to_text(tmp.path() / "missing.txt")));
});

CB_TEST("verify.sweep.the_options_say_which_kinds_and_which_modules", [] {
    cli::Command command{"sweep", "reconstruct and judge the corpus", sweep::params(), [](const cli::Args&) { return 0; }};
    const cli::Cli cli("codebreaker", {command});
    const auto options = [&](std::vector<std::string> args) {
        args.insert(args.begin(), "sweep");
        return sweep::options_of(cli.parse(args).args);
    };
    CB_CHECK(options({}).kinds == std::vector<std::string>{"pe"});
    CB_CHECK(options({"--16"}).kinds == std::vector<std::string>{"ne"});
    CB_CHECK(options({"--drivers"}).kinds == std::vector<std::string>{"le"});
    CB_CHECK(options({"--dos"}).kinds == std::vector<std::string>{"dos"});
    CB_CHECK(options({"--all"}).kinds == (std::vector<std::string>{"pe", "ne", "le", "dos"}));
    CB_CHECK(options({"--all", "--16"}).kinds.size() == 4u);   // --all wins
    CB_CHECK(!options({}).only);
    CB_CHECK(options({"--only", "a.dll,B.dll"}).only == (std::set<std::string>{"a.dll", "b.dll"}));
    CB_CHECK(options({"--status"}).status_only);
    CB_CHECK(options({"--again"}).again);
    CB_CHECK(!options({}).again);
});

CB_TEST("verify.sweep.a_sweep_measures_what_is_not_measured_and_goes_past_a_failure", [] {
    Sweeping s;
    const int code = sweep::run({}, *s.services);
    CB_CHECK_EQ(code, 1);   // bad.fk failed outright
    CB_CHECK_EQ(s.out.front(), "0 of 4 modules measured under the current verifier settings; 0 functions: ");
    CB_CHECK(s.out[1].starts_with("[1/4] a.fk: "));
    CB_CHECK(s.out[2].starts_with("[2/4] bad.fk FAILED: "));
    CB_CHECK(s.out[3].starts_with("[3/4] b.fk: "));
    CB_CHECK(s.out[4].starts_with("[4/4] c.txt FAILED: "));   // nothing reconstructs it
    CB_CHECK(s.out.back().starts_with("2 of 4 modules measured under the current verifier settings; 2 functions: "));
    CB_CHECK_EQ(s.store.best("sha-a.fk", "d1").at("key:a.fk").cls, verify::Class::Verified);
});

CB_TEST("verify.sweep.a_second_sweep_picks_up_where_the_first_stopped", [] {
    Sweeping s;
    (void)sweep::run({}, *s.services);
    s.out.clear();
    (void)sweep::run({}, *s.services);
    // only the failures are tried again
    CB_CHECK(s.out[0].starts_with("2 of 4 modules measured"));
    CB_CHECK(s.out[1].starts_with("[2/4] bad.fk FAILED"));
    CB_CHECK(s.out[2].starts_with("[4/4] c.txt FAILED"));
    s.out.clear();
    sweep::Options again;
    again.again = true;
    again.only = std::set<std::string>{"a.fk"};
    CB_CHECK_EQ(sweep::run(again, *s.services), 0);
    CB_CHECK(s.out[0].starts_with("1 of 1 modules measured"));
    CB_CHECK(s.out[1].starts_with("[1/1] a.fk: "));
});

CB_TEST("verify.sweep.status_does_no_work", [] {
    Sweeping s;
    sweep::Options options;
    options.status_only = true;
    CB_CHECK_EQ(sweep::run(options, *s.services), 0);
    CB_CHECK_EQ(s.out.size(), 1u);
    CB_CHECK(s.out[0].starts_with("0 of 4 modules measured"));
    CB_CHECK(s.store.modules().empty());
});

CB_TEST("verify.sweep.nothing_is_swept_without_the_build_tools", [] {
    Sweeping s;
    s.services->tools_missing = [] { return std::optional<std::string>("OpenWatcom is not set up (settings.json: openwatcom): nothing could be built"); };
    CB_CHECK_EQ(sweep::run({}, *s.services), 1);
    CB_CHECK(s.out.empty());
    CB_CHECK_EQ(s.err.size(), 1u);
    CB_CHECK(s.err[0].starts_with("OpenWatcom is not set up"));
});
