#include "exact/identify.hpp"

#include <algorithm>
#include <set>

#include <nlohmann/json.hpp>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::exact;
using namespace cb::exact::identify;
namespace stdfs = std::filesystem;

class FakeToolchain final : public modules::Toolchain {
public:
    explicit FakeToolchain(std::string name) : name_(std::move(name)) {}
    std::optional<std::string> profile(std::string_view) const override { return std::nullopt; }
    modules::CompileResult compile(ByteView, const modules::CompileOptions&) const override { return {}; }
    std::string name_;
};

// A project whose sources are given; what identify changes (the switches) is recorded.
class FakeProject final : public ledger::Project {
public:
    stdfs::path dir_path;
    std::vector<ledger::SourceFile> files;
    std::optional<std::string> current;
    std::vector<std::optional<std::string>> assigned;

    stdfs::path dir() const override { return dir_path; }
    stdfs::path src() const override { return dir_path / "src"; }
    std::string exe() const override { return "PROG.EXE"; }
    stdfs::path path(std::string_view name) const override { return dir_path / std::string(name); }
    std::optional<std::string> config_text(std::string_view) const override { return std::nullopt; }
    const pascal_join::Project* compilands() const override { return nullptr; }
    std::map<std::string, Bytes> headers() const override { return {}; }
    std::vector<std::string> include() const override { return {}; }
    std::optional<std::string> switches() const override { return current; }
    std::vector<std::uint32_t> starts() const override { return {}; }
    bool whole(const stdfs::path&) const override { return true; }
    ByteView image() const override { return {}; }
    std::uint32_t code_end() const override { return 0; }
    std::vector<ledger::LabelledToolchain> toolchains() const override { return {}; }
    SymbolTable symbols() const override { return {}; }
    void save_symbols(const SymbolTable&) const override {}
    std::vector<ledger::SourceFile> sources() const override { return files; }
    std::vector<std::shared_ptr<const ledger::Project>> sets() const override { return {}; }
    compare::Program program() const override { return {}; }
    void set_switches(const std::optional<std::string>& switches) {
        current = switches;
        assigned.push_back(switches);
    }
};

struct Fixture {
    fs::ScopedTempDir dir{"identify"};
    toolchain::Locations locations;
    FakeProject project;
    std::vector<std::string> loaded;

    Fixture() {
        locations.profiles = dir.path() / "profiles";
        fs::make_dirs(locations.profiles);
        fs::write_text(locations.profiles / "cc.json", R"({"name": "cc"})");
        fs::write_text(locations.profiles / "cc2.json", R"({"name": "cc2", "try_switches": ["-Gs"]})");
        fs::write_text(locations.profiles / "mspascal.json", R"({"name": "mspascal"})");
        fs::write_text(locations.profiles / "plm86.json", R"({"name": "plm86"})");
        fs::write_text(locations.profiles / "masm.json", R"({"name": "masm", "source_ext": ".ASM", "try_switches": ["/ML", "/MX"]})");
        fs::write_text(locations.profiles / "upper.json", R"({"name": "upper", "source_ext": ".C"})");
        project.dir_path = dir.path() / "proj";
        fs::make_dirs(project.dir_path);
        fs::write_text(project.dir_path / "project.json", R"({"name": "prog", "toolchain": "old", "variant": "v0", "keep": [1, 2]})");
        for (int i = 0; i < 20; ++i) project.files.push_back({0x1000 + i * 16, project.dir_path / ("F" + std::to_string(i) + ".C")});
    }

    Services services(const std::function<std::int64_t(const Candidate&, const std::optional<std::string>&)>& identical_of) {
        Services s;
        s.locations = locations;
        s.set_switches = [this](const std::optional<std::string>& sw) { project.set_switches(sw); };
        s.load = [this](const std::string& name, const std::optional<std::string>&) -> std::shared_ptr<const ledger::Toolchain> {
            loaded.push_back(name);
            if (name == "nokit") throw UsageError("the kit 'nokit' is not set up");
            return std::make_shared<FakeToolchain>(name);
        };
        s.check = [identical_of](const ledger::Project& p, const ledger::Toolchain& t, const ledger::SourceFile&, const SymbolTable&, SymbolTable&) {
            // one function per file: identical when the identify's `identical_of` says so for the toolchain and the switches now
            const auto& tc = static_cast<const FakeToolchain&>(t);
            ledger::Row r;
            r.verdict = identical_of({tc.name_, std::nullopt, std::nullopt}, p.switches()) ? ledger::Verdict::Identical : ledger::Verdict::Differs;
            return std::vector<ledger::Row>{r};
        };
        return s;
    }
};

}  // namespace

CB_TEST("exact.identify.the_candidates_are_the_profiles_of_the_sources_kind", [] {
    Fixture f;
    auto c = candidates(f.project, f.locations);
    // C sources: cc, cc2 (with its alternative switches), upper (".C" in capitals is the same); not Pascal, PL/M or assembler
    std::vector<std::string> names;
    for (const Candidate& x : c) names.push_back(x.toolchain + (x.switches ? "/" + *x.switches : ""));
    CB_CHECK((names == std::vector<std::string>{"cc", "cc2", "cc2/-Gs", "upper"}));

    f.project.files = {{std::nullopt, f.project.dir_path / "A.PAS"}, {std::nullopt, f.project.dir_path / "b.pas"}};
    c = candidates(f.project, f.locations);
    CB_CHECK_EQ(c.size(), 1u);
    CB_CHECK_EQ(c[0].toolchain, "mspascal");   // no "source_ext": its name says Pascal

    f.project.files = {{std::nullopt, f.project.dir_path / "x.plm"}, {std::nullopt, f.project.dir_path / "m.asm"}};
    c = candidates(f.project, f.locations);
    names.clear();
    for (const Candidate& x : c) names.push_back(x.toolchain + (x.switches ? "/" + *x.switches : ""));
    CB_CHECK((names == std::vector<std::string>{"masm", "masm//ML", "masm//MX", "plm86"}));
});

CB_TEST("exact.identify.try_names_the_candidates_itself", [] {
    Fixture f;
    const std::vector<std::string> tries = {"msc300", "msc300:own", "msc300:own/-Gs", "msc400/-Ox -Gs", "asm86/"};
    const auto c = candidates(f.project, f.locations, tries);
    CB_CHECK_EQ(c.size(), 5u);
    CB_CHECK((c[0] == Candidate{"msc300", std::nullopt, std::nullopt}));
    CB_CHECK((c[1] == Candidate{"msc300", "own", std::nullopt}));
    CB_CHECK((c[2] == Candidate{"msc300", "own", "-Gs"}));
    CB_CHECK((c[3] == Candidate{"msc400", std::nullopt, "-Ox -Gs"}));
    CB_CHECK((c[4] == Candidate{"asm86", std::nullopt, std::nullopt}));
});

CB_TEST("exact.identify.a_score_counts_the_functions_that_come_out_the_programs", [] {
    Fixture f;
    f.project.current = "-Oat";
    // identical when the compile switches are the candidate's -Gs; a plain compile gets none
    const Services s = f.services([](const Candidate&, const std::optional<std::string>& sw) { return sw == std::optional<std::string>("-Gs"); });
    const std::vector<ledger::SourceFile> files(f.project.files.begin(), f.project.files.begin() + 4);
    const Score plain = score(f.project, {"cc", std::nullopt, std::nullopt}, files, s);
    CB_CHECK_EQ(plain.same, 0);
    CB_CHECK_EQ(plain.text, "0 of 4");
    const Score gs = score(f.project, {"cc", std::nullopt, "-Gs"}, files, s);
    CB_CHECK_EQ(gs.same, 4);
    CB_CHECK_EQ(gs.text, "4 of 4");
    // the project's own switches were what the plain candidate was scored under, and they are back
    CB_CHECK(f.project.current == std::optional<std::string>("-Oat"));
    CB_CHECK(f.project.assigned.back() == std::optional<std::string>("-Oat"));
});

CB_TEST("exact.identify.a_toolchain_that_cannot_be_loaded_scores_minus_one_with_the_reason", [] {
    Fixture f;
    const Services s = f.services([](const Candidate&, const std::optional<std::string>&) { return true; });
    const Score r = score(f.project, {"nokit", std::nullopt, std::nullopt}, f.project.files, s);
    CB_CHECK_EQ(r.same, -1);
    CB_CHECK_EQ(r.text, "the kit 'nokit' is not set up");
    CB_CHECK(f.project.assigned.empty());   // nothing was changed in the project
});

CB_TEST("exact.identify.the_switches_come_back_when_a_check_throws", [] {
    Fixture f;
    f.project.current = "-Oat";
    Services s = f.services([](const Candidate&, const std::optional<std::string>&) { return true; });
    s.check = [](const ledger::Project&, const ledger::Toolchain&, const ledger::SourceFile&, const SymbolTable&, SymbolTable&) -> std::vector<ledger::Row> {
        throw EmulationError("the guest faulted");
    };
    CB_CHECK_THROWS_AS(EmulationError, (void)score(f.project, {"cc", std::nullopt, "-Gs"}, f.project.files, s));
    CB_CHECK(f.project.current == std::optional<std::string>("-Oat"));
});

CB_TEST("exact.identify.the_sample_is_the_same_every_time_and_has_no_file_twice", [] {
    Fixture f;
    const auto a = sample(f.project.files, 12);
    const auto b = sample(f.project.files, 12);
    CB_CHECK_EQ(a.size(), 12u);
    for (std::size_t i = 0; i < a.size(); ++i) CB_CHECK(a[i].path == b[i].path);
    std::set<std::string> seen;
    for (const auto& file : a) seen.insert(fs::to_text(file.path));
    CB_CHECK_EQ(seen.size(), 12u);
    // the order the project listed them in does not matter
    auto reversed = f.project.files;
    std::reverse(reversed.begin(), reversed.end());
    const auto c = sample(reversed, 12);
    for (std::size_t i = 0; i < a.size(); ++i) CB_CHECK(a[i].path == c[i].path);
    // more asked for than there are: every file once
    CB_CHECK_EQ(sample(f.project.files, 100).size(), 20u);
    CB_CHECK_EQ(sample(f.project.files, 0).size(), 0u);
    CB_CHECK(sample({}, 5).empty());
    // a file without an address sorts before the others and does not throw
    auto mixed = f.project.files;
    mixed.push_back({std::nullopt, f.project.dir_path / "NOADDR.C"});
    CB_CHECK_EQ(sample(mixed, 100).size(), 21u);
});

CB_TEST("exact.identify.the_best_toolchain_is_reported_and_written_to_the_project", [] {
    Fixture f;
    // cc2 with -Gs makes every function identical; cc2 plain makes half
    int calls = 0;
    const Services s = f.services([&](const Candidate& c, const std::optional<std::string>& sw) {
        ++calls;
        if (c.toolchain == "cc2" && sw == std::optional<std::string>("-Gs")) return true;
        return false;
    });
    testing::CapturedStd cap;
    CB_CHECK_EQ(command(f.project, {"--sample", "5", "--write"}, s), 0);
    const std::string out = cap.out();
    CB_CHECK(out.find("5 of 5") != std::string::npos);
    CB_CHECK(out.find("0 of 5") != std::string::npos);
    CB_CHECK(out.find("the best: cc2 -Gs") != std::string::npos);
    CB_CHECK(out.find("written to") != std::string::npos);
    CB_CHECK_EQ(calls, 5 * 4);   // four candidates, the same five files
    const Json j = json::load(f.project.dir_path / "project.json");
    CB_CHECK_EQ(j["toolchain"].get<std::string>(), "cc2");
    CB_CHECK_EQ(j["switches"].get<std::string>(), "-Gs");
    CB_CHECK(!j.contains("variant"));            // the old variant is gone: the candidate has none
    CB_CHECK_EQ(j["keep"].size(), 2u);           // the rest of the file is as it was
    CB_CHECK_EQ(j["name"].get<std::string>(), "prog");
});

CB_TEST("exact.identify.without_write_the_project_is_not_touched", [] {
    Fixture f;
    const Services s = f.services([](const Candidate&, const std::optional<std::string>&) { return true; });
    const std::string before = fs::read_text(f.project.dir_path / "project.json");
    testing::CapturedStd cap;
    CB_CHECK_EQ(command(f.project, {"--try", "cc"}, s), 0);
    CB_CHECK(cap.out().find("the best: cc\n") != std::string::npos);
    CB_CHECK_EQ(fs::read_text(f.project.dir_path / "project.json"), before);
});

CB_TEST("exact.identify.when_nothing_makes_a_function_the_programs_it_says_so", [] {
    Fixture f;
    const Services s = f.services([](const Candidate&, const std::optional<std::string>&) { return false; });
    testing::CapturedStd cap;
    CB_CHECK_EQ(command(f.project, {}, s), 1);
    CB_CHECK(cap.out().find("no toolchain makes any of the sample's functions the program's") != std::string::npos);
    f.project.files.clear();   // no sources: no candidate either
    testing::CapturedStd again;
    CB_CHECK_EQ(command(f.project, {}, s), 1);
});

CB_TEST("exact.identify.a_bad_sample_is_a_usage_error", [] {
    Fixture f;
    const Services s = f.services([](const Candidate&, const std::optional<std::string>&) { return true; });
    CB_CHECK_THROWS_AS(UsageError, (void)command(f.project, {"--sample"}, s));
    CB_CHECK_THROWS_AS(UsageError, (void)command(f.project, {"--sample", "many"}, s));
});
