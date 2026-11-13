#include "cb/java/transpile.hpp"

#include <nlohmann/json.hpp>

#include <map>
#include <string>
#include <vector>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/java/emit/source_test_support.hpp"

namespace {

namespace trn = cb::java::transpile;
namespace stdfs = std::filesystem;
using cb::Json;

// The translator of these tests: the deterministic fake back end, with the tally the Python golden run was given.
class TrFakeTranslator : public trn::BodyTranslator {
public:
    trn::TranslatedBodies translate(const trn::TranslateRequest& request) override {
        asked = request.classes.size();
        platform = request.platform ? std::string(*request.platform) : "";
        year = request.cpp_year;
        trn::TranslatedBodies out;
        out.bodies = std::make_unique<cb::java::emit::source::FakeBodies>();
        out.classes_lowered = 5;
        out.methods_written = 7;
        out.declined = {{"alpha", 3}, {"beta", 3}, {"gamma", 1}};
        return out;
    }
    std::size_t asked = 0;
    std::string platform;
    int year = 0;
};

struct TrWorld {
    cb::java::JavaData data{cb::testing::data_path("../../data/java")};
    cb::java::runtime::Runtime runtime{data};
    cb::java::profiles::ProfileSet profiles;
    cb::java::projects::ProjectSet projects{data.path({"projects"})};
    TrFakeTranslator translator;
    trn::Services services{data, runtime, profiles, projects, translator};
};

stdfs::path trt_source() { return cb::testing::data_path("java/testdata/source"); }

// testdata/convert.golden.json: the original Python convert() on testdata/source with the same fake translator, three configurations:
// the file set (a sha256 of each file's text; the report folder by name only), the progress lines, and the summary.
CB_TEST("java.transpile.convert_matches_python", [] {
    const Json golden = cb::json::load(cb::testing::data_path("java/testdata/convert.golden.json"));
    TrWorld world;
    for (const Json& config : golden) {
        const std::string name = config["name"].get<std::string>();
        cb::fs::ScopedTempDir tmp("cbconvert");
        std::vector<std::string> messages;
        trn::Options options;
        options.cpp_std = config["args"]["cpp_std"].get<std::string>();
        options.platform = config["args"]["platform"].is_null() ? "" : config["args"]["platform"].get<std::string>();
        options.emit_debug = config["args"]["emit_debug"].get<bool>();
        options.write_report = config["args"]["write_report"].get<bool>();
        options.overlays_root = tmp.path() / "overlays";
        options.progress = [&messages](std::string_view line) { messages.emplace_back(line); };
        const stdfs::path out = tmp.path() / "out";
        const trn::Summary summary = trn::convert(trt_source(), out, options, world.services);

        std::map<std::string, std::string> files;
        for (const auto& entry : stdfs::recursive_directory_iterator(out)) {
            if (!entry.is_regular_file()) continue;
            const std::string rel = stdfs::relative(entry.path(), out).generic_string();
            if (rel.starts_with("report/")) { files[rel] = ""; continue; }
            const std::string text = cb::fs::read_text(entry.path());
            files[rel] = cb::crypto::to_hex(cb::crypto::sha256(cb::Bytes(text.begin(), text.end())));
        }
        const auto& want = config["files"];
        CB_CHECK_EQ(files.size(), want.size());
        for (const auto& [rel, sha] : want.items()) {
            const auto it = files.find(rel);
            CB_CHECK(it != files.end());
            if (it != files.end()) CB_CHECK_EQ(it->second, sha.get<std::string>());
        }
        // the progress lines, but for the two that name the output folder
        std::vector<std::string> got, expected = config["messages"].get<std::vector<std::string>>();
        for (const std::string& m : messages) {
            if (!m.starts_with("[report] written to")) got.push_back(m);
        }
        // (the golden run was made with the bug-fix ledger stubbed out; a conversion regenerates it only when asked to, see below)
        std::erase_if(expected, [](const std::string& m) { return m.starts_with("[report] written to") || m.starts_with("[bugfix-ledger]"); });
        CB_CHECK_EQ(got.size(), expected.size());
        for (std::size_t i = 0; i < std::min(got.size(), expected.size()); ++i) CB_CHECK_EQ(got[i], expected[i]);
        const auto& s = config["summary"];
        CB_CHECK_EQ(summary.parsed, s["parsed"].get<std::size_t>());
        CB_CHECK_EQ(summary.headers, s["headers"].get<std::size_t>());
        CB_CHECK_EQ(summary.sources, s["sources"].get<std::size_t>());
        CB_CHECK(summary.markers == s["markers"].get<std::vector<std::string>>());
        CB_CHECK_EQ(summary.rule_findings, s["rule_findings"].get<std::size_t>());
        CB_CHECK_EQ(summary.events, s["events"].get<std::size_t>());
        CB_CHECK_EQ(summary.high, s["high"].get<std::size_t>());
        CB_CHECK_EQ(summary.cpp_std, s["cpp_std"].get<std::string>());
        CB_CHECK_EQ(summary.platform, s["platform"].get<std::string>());
        CB_CHECK_EQ(summary.profile, s["profile"].get<std::string>());
        CB_CHECK_EQ(summary.methods_written, s["tree_bodies"][0].get<std::size_t>());
        CB_CHECK_EQ(world.translator.year, config["args"]["cpp_std"] == "C++17" ? 2017 : (config["args"]["cpp_std"] == "C++11" ? 2011 : 2003));
        CB_CHECK_EQ(summary.report.has_value(), config["args"]["write_report"].get<bool>());
        (void)name;
    }
});

CB_TEST("java.transpile.standards", [] {
    CB_CHECK_EQ(trn::parse_standard("C++98").year, 1998);
    CB_CHECK_EQ(trn::parse_standard("C++17").label, "C++17");
    CB_CHECK_EQ(trn::parse_standard("C++23").year, 2023);
    // DIVERGES: the Python read an unknown label as C++03; a typo is now an error
    CB_CHECK_THROWS_AS(cb::UsageError, (void)trn::parse_standard("C++4"));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)trn::parse_standard("c++11"));
});

CB_TEST("java.transpile.registrars_of_a_project", [] {
    cb::java::projects::Project generic;
    const auto r = trn::registrars_of(generic);
    CB_CHECK(r.names.empty());
    CB_CHECK(r.prefixes == std::vector<std::string>{"register"});
    CB_CHECK(r.matches_line("EntityList.registerEntity(Pig.class);"));
    CB_CHECK(!r.matches_line("EntityList.addMapping(Pig.class);"));
    cb::java::projects::Project minecraft;
    minecraft.registrars = {"addMapping", "addIdClassMapping", R"(register\w*)"};
    const auto m = trn::registrars_of(minecraft);
    CB_CHECK(m.names == (std::vector<std::string>{"addMapping", "addIdClassMapping"}));
    CB_CHECK(m.matches_line("addMapping(Pig.class, \"Pig\", 90);"));
    CB_CHECK(m.matches_line("registerAll(x);"));
    CB_CHECK(!m.matches_line("addMappings(x);"));
    minecraft.registrars = {"a|b"};
    CB_CHECK_THROWS_AS(cb::UsageError, (void)trn::registrars_of(minecraft));
});

CB_TEST("java.transpile.the_translator_is_asked_once_with_the_program", [] {
    TrWorld world;
    cb::fs::ScopedTempDir tmp("cbconvert");
    trn::Options options;
    options.platform = "pc_windows";
    options.cpp_std = "C++17";
    options.overlays_root = tmp.path();
    options.write_report = false;
    options.emit_debug = false;
    (void)trn::convert(trt_source(), tmp.path() / "out", options, world.services);
    CB_CHECK_EQ(world.translator.asked, std::size_t{18});
    CB_CHECK_EQ(world.translator.platform, "pc_windows");
    CB_CHECK_EQ(world.translator.year, 2017);
}
);

CB_TEST("java.transpile.the_bugfix_ledger_is_regenerated_on_request", [] {
    TrWorld world;
    cb::fs::ScopedTempDir tmp("cbconvert");
    trn::Options options;
    options.overlays_root = tmp.path();
    options.write_report = false;
    options.emit_debug = false;
    options.bugfix_ledger = tmp.path() / "BUGFIX_LEDGER.md";
    std::vector<std::string> messages;
    options.progress = [&messages](std::string_view line) { messages.emplace_back(line); };
    (void)trn::convert(trt_source(), tmp.path() / "out", options, world.services);
    CB_CHECK(stdfs::exists(tmp.path() / "BUGFIX_LEDGER.md"));
    CB_CHECK(std::ranges::any_of(messages, [](const std::string& m) { return m.starts_with("[bugfix-ledger] ") && m.ends_with("-> BUGFIX_LEDGER.md"); }));
});

CB_TEST("java.transpile.parse_failures_are_ledgered_not_fatal", [] {
    TrWorld world;
    cb::fs::ScopedTempDir tmp("cbconvert");
    cb::fs::write_text(tmp.path() / "src" / "Good.java", "package t;\npublic class Good {\n    int a;\n}\n");
    cb::fs::write_text(tmp.path() / "src" / "Bad.java", "this is not java at all {{{");
    trn::Options options;
    options.overlays_root = tmp.path();
    options.write_report = false;
    options.emit_debug = false;
    std::vector<std::string> messages;
    options.progress = [&messages](std::string_view line) { messages.emplace_back(line); };
    const trn::Summary s = trn::convert(tmp.path() / "src", tmp.path() / "out", options, world.services);
    CB_CHECK_EQ(s.parsed, std::size_t{1});
    CB_CHECK(stdfs::exists(tmp.path() / "out" / "Good.h"));
    CB_CHECK(std::ranges::any_of(messages, [](const std::string& m) { return m.starts_with("[parse] FAIL Bad.java"); }));
    CB_CHECK(std::ranges::any_of(s.ledger.events(), [](const cb::java::ledger::Event& e) { return e.rule == "parse-error" && e.file == "Bad.java"; }));
    CB_CHECK(s.high >= 1);
}
);

// ---- tests/java/test_edition_convert_integration.py and test_review_fixes.py (the profile of a conversion). The Python switched a process-wide active
// profile for the run and restored it after; here the profile of a conversion is handed down, so "restored" is "the shared profile set was never
// switched" (checked on the active name). The cases that pick a Minecraft edition need the Minecraft project, which this world does not have: they
// are in translate/java_ports_test.cpp, on the application's own assembly.

namespace {

struct TrEditionRun {
    std::vector<std::string> logs;
    trn::Summary summary;
};

TrEditionRun tr_convert_files(TrWorld& world, const std::map<std::string, std::string>& files, const std::string& edition, const std::string& version) {
    cb::fs::ScopedTempDir tmp("cbedition");
    for (const auto& [name, text] : files) cb::fs::write_text(tmp.path() / "java" / name, text);
    trn::Options options;
    options.overlays_root = tmp.path();
    options.write_report = false;
    options.emit_debug = false;
    options.edition = edition;
    options.mc_version = version;
    TrEditionRun run;
    options.progress = [&run](std::string_view line) { run.logs.emplace_back(line); };
    run.summary = trn::convert(tmp.path() / "java", tmp.path() / "out", options, world.services);
    return run;
}

bool tr_logged(const TrEditionRun& run, std::string_view part) {
    return std::ranges::any_of(run.logs, [&](const std::string& m) { return m.find(part) != std::string::npos; });
}

}  // namespace

CB_TEST("java.transpile.an_inconclusive_corpus_keeps_the_default_profile", [] {
    TrWorld world;
    const std::string before = world.profiles.active_name();
    const TrEditionRun run = tr_convert_files(world, {{"Foo.java", "public class Foo { public int x; }"}}, "", "");
    CB_CHECK(!tr_logged(run, "active target profile"));   // nothing conclusive: no switch
    CB_CHECK_EQ(world.profiles.active_name(), before);
});

// A conversion that fails midway leaves the shared profile set as it found it.
CB_TEST("java.transpile.a_conversion_that_fails_midway_leaves_the_profiles_alone", [] {
    struct Failing : trn::BodyTranslator {
        trn::TranslatedBodies translate(const trn::TranslateRequest&) override { throw cb::UsageError("midway failure"); }
    };
    TrWorld world;
    Failing failing;
    trn::Services services{world.data, world.runtime, world.profiles, world.projects, failing};
    const std::string before = world.profiles.active_name();
    cb::fs::ScopedTempDir tmp("cbfail");
    cb::fs::write_text(tmp.path() / "java" / "Level.java", "public class Level { public int x; }");
    trn::Options options;
    options.overlays_root = tmp.path();
    options.write_report = false;
    CB_CHECK_THROWS_AS(cb::UsageError, (void)trn::convert(tmp.path() / "java", tmp.path() / "out", options, services));
    CB_CHECK_EQ(world.profiles.active_name(), before);
});

}  // namespace
