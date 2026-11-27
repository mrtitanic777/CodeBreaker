// Tests of the wiring of lanes: a lane described once reaches reconstruct's kinds and the engine's steps, and the builders say which
// platform's steps make a file's module build. The lane here is a made-up one (the words of a file are its functions).
#include "engine/lanes_wiring.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"

using namespace cb;
using namespace cb::engine;
using namespace cb::engine::lanes_wiring;

namespace {

using verify::lanes::Built;
using verify::lanes::Prepared;

class WireKind : public verify::lanes::Kind {
public:
    explicit WireKind(std::string name) : name_(std::move(name)) {}
    const std::string& name() const override { return name_; }
    verify::lanes::Profile profile(const std::filesystem::path&) const override { return {"p", "{}"}; }
    std::unique_ptr<Prepared> prepare(const std::filesystem::path& module, const std::filesystem::path&) const override {
        auto p = std::make_unique<Prepared>();
        const std::string word = fs::read_text(module);
        p->order = {"f0"};
        p->texts["f0"] = "int f0(void) { return " + word + "; }";
        p->key["f0"] = "0000";
        p->all_keys = {"0000"};
        p->sig = "whole";
        return p;
    }
    Built build(const Prepared& p, const std::filesystem::path&, const std::filesystem::path&) const override {
        Built b;
        b.items = p.order;
        b.judge = [](const std::string&) { return store::Judgement{store::Verdict::Equivalent, "", {}, ""}; };
        return b;
    }

private:
    std::string name_;
};

struct Wired {
    fs::ScopedTempDir dir{"wiring"};
    facts::Databases databases;
    Steps steps;
    knowledge::ReadDigests digests;
    Engine engine;
    lanes::Lanes lanes_;
    verify::reconstruct::Kinds kinds;
    Builders builders;
    Wiring wiring;

    Wired()
        : databases(facts::Config{dir.path() / "db", 1 * facts::kGiB}),
          digests([](const knowledge::Read&) { return std::string(knowledge::kAbsent); }),
          engine(steps, databases, digests, 2),
          lanes_(steps, engine, [](const std::filesystem::path& p) { return std::optional<std::string>(p.extension() == ".pe" ? "pe" : "ne"); }),
          wiring{kinds, lanes_, builders, [](const std::filesystem::path& p) { return p.extension() == ".pe"; }} {}
};

}  // namespace

CB_TEST("engine.lanes_wiring.a_lane_reaches_the_kinds_and_the_steps", [] {
    Wired w;
    CB_CHECK_EQ(register_lane(w.wiring, {.kind = std::make_shared<WireKind>("ne"), .traits = {.per_function = true}, .platform = "win16"}), "ne");
    CB_CHECK(w.kinds.find("ne") != nullptr);
    CB_CHECK(w.kinds.traits("ne").per_function);
    CB_CHECK(w.lanes_.kinds() == (std::vector<std::string>{"ne"}));
    CB_CHECK(w.builders.platforms().empty());   // (no link: no builder)
    const auto steps = w.steps.list("win16");
    CB_CHECK(std::any_of(steps.begin(), steps.end(), [](const auto& s) { return s->kind == "lane.ne.rows"; }));
    // registered again: replaced
    register_lane(w.wiring, {.kind = std::make_shared<WireKind>("ne"), .traits = {.per_function = false}, .platform = "win16"});
    CB_CHECK(!w.kinds.traits("ne").per_function);
    CB_CHECK_EQ(w.lanes_.kinds().size(), std::size_t{1});
});

CB_TEST("engine.lanes_wiring.the_lane_pe_is_linked_whole_and_its_files_are_built_by_the_engine", [] {
    Wired w;
    int links = 0;
    register_lane(w.wiring, {.kind = std::make_shared<WireKind>("pe"),
                             .traits = {.per_function = true},
                             .platform = "win32",
                             .link = [&](const Prepared& m, const std::filesystem::path&, const std::filesystem::path&) {
                                 ++links;
                                 lanes::Linked l;
                                 l.name = "thing.pe";
                                 l.image = Bytes(m.texts.at("f0").begin(), m.texts.at("f0").end());
                                 l.report = "linked";
                                 return l;
                             }});
    CB_CHECK(w.builders.platforms() == (std::vector<std::string>{"win32"}));
    const std::filesystem::path file = w.dir.path() / "THING.pe";
    fs::write_text(file, "7");
    const std::optional<BuildResult> b = w.builders.delegate(file, w.dir.path() / "out");
    CB_CHECK(b.has_value());
    CB_CHECK_EQ(b->platform, "win32");
    CB_CHECK_EQ(b->name, "thing.pe");
    CB_CHECK_EQ(b->report, "linked");
    CB_CHECK_EQ(fs::read_text(*b->image), "int f0(void) { return 7; }");
    CB_CHECK_EQ(links, 1);
    // a file no platform's steps take
    const std::filesystem::path other = w.dir.path() / "THING.ne";
    fs::write_text(other, "1");
    CB_CHECK(!w.builders.delegate(other).has_value());
});

CB_TEST("engine.lanes_wiring.the_first_platform_that_takes_the_file_builds_it_and_a_platform_added_again_replaces_its_builder", [] {
    Builders builders;
    int first = 0, second = 0;
    auto make = [&](const std::string& platform, const std::string& ext, int& count) {
        return Builder{platform, [ext](const std::filesystem::path& p) { return p.extension() == ext; },
                       [&count, platform](const std::filesystem::path&, const std::optional<std::filesystem::path>&, const Run::Log&) {
                           ++count;
                           return BuildResult{platform, "n", "r", {}, {}, std::nullopt};
                       }};
    };
    builders.add(make("a", ".x", first));
    builders.add(make("b", ".x", second));   // (also takes .x, but is after a)
    builders.add(make("b", ".y", second));   // (replaces b)
    CB_CHECK(builders.platforms() == (std::vector<std::string>{"a", "b"}));
    CB_CHECK_EQ(builders.delegate("m.x")->platform, "a");
    CB_CHECK_EQ(builders.delegate("m.y")->platform, "b");
    CB_CHECK_EQ(first, 1);
    CB_CHECK_EQ(second, 1);
    CB_CHECK(!builders.delegate("m.z").has_value());
});

CB_TEST("engine.lanes_wiring.the_errors_of_registering", [] {
    Wired w;
    CB_CHECK_THROWS_AS(UsageError, (void)register_lane(w.wiring, {}));
    CB_CHECK_THROWS_AS(UsageError, w.builders.add({}));
    // a lane other than "pe" cannot link whole
    CB_CHECK_THROWS_AS(UsageError, (void)register_lane(w.wiring, {.kind = std::make_shared<WireKind>("ne"), .platform = "win16",
                                                                    .link = [](const Prepared&, const std::filesystem::path&, const std::filesystem::path&) { return lanes::Linked{}; }}));
    // the lane "pe" with no test for its files
    Wired v;
    v.wiring.is_pe = nullptr;
    CB_CHECK_THROWS_AS(UsageError, (void)register_lane(v.wiring, {.kind = std::make_shared<WireKind>("pe"), .platform = "win32",
                                                                    .link = [](const Prepared&, const std::filesystem::path&, const std::filesystem::path&) { return lanes::Linked{}; }}));
});
