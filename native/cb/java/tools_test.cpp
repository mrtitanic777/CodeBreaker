// Tests of `codebreaker java <tool>` complete (cb/java/tools.hpp) on the transpiler's own test fixtures (native/cb/java/testdata/tools_fixtures: nine
// classes), against the recorded output of the same commands (testdata/tools/): the text of each tool, with the
// folders it names replaced by <fixtures> and <out>, and for the trees a tool writes a manifest ("path size sha1") of its files.
//
// The conversions need a JDK (javac's typed tree is the front end's input): skipped where there is none. What a conversion writes is
// compared for the nine classes of the fixture (their headers and sources); the runtime and platform files beside them are the lane's data.
//
// A line that reports a problem goes to stderr; a text a file holds for a person writes yes/no (PLATFORM_CHOICES.md); a path an emitted file names uses `/` (the SDK folder in
// PLATFORM_CHOICES.md and PLATFORM_LAYER.md); platform.config.json ends with a newline.
#include "cb/java/tools.hpp"

#include <algorithm>
#include <functional>
#include <tuple>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/jar.hpp"

using namespace cb;
namespace jt = cb::java::tools;

namespace {

namespace stdfs = std::filesystem;

stdfs::path jtt_fixtures() { return testing::data_path("java/testdata/tools_fixtures").lexically_normal(); }
std::string jtt_golden(std::string_view name) { return fs::read_text(testing::data_path("java/testdata/tools/" + std::string(name))); }

// A tool's environment: the checkout (the lane's data), a state folder and the JDK of the machine.
struct JttSetup {
    fs::ScopedTempDir dir{"cb-java-tools"};
    std::optional<fs::ScopedEnv> state, converted;
    jt::Environment environment;
    JttSetup() {
        state.emplace("CB_STATE", fs::to_text(dir.path() / "state"));
        converted.emplace("CB_CONVERTED", fs::to_text(dir.path() / "Converted"));
        environment.config = config::Config::load(testing::data_path("../.."));
    }
};

struct JttRun {
    std::string out, err;
    int code = 0;
};

JttRun jtt_run(const jt::Environment& env, const std::vector<std::string>& args) {
    testing::CapturedStd cap;
    const int code = jt::main(args, env);
    return {cap.out(), cap.err(), code};
}

// A tool's text as the goldens have it: LF, the folders named by <fixtures> and <out>.
std::string jtt_norm(std::string text, const stdfs::path& out) {
    text = text::replace_all(text, "\r\n", "\n");
    std::replace(text.begin(), text.end(), '\\', '/');
    text = text::replace_all(text, fs::to_text(jtt_fixtures()), "<fixtures>");
    std::string root = fs::to_text(out);
    std::replace(root.begin(), root.end(), '\\', '/');
    return text::replace_all(text, root, "<out>");
}

// "path size sha1" lines of the files of `folder` (LF-normalised) that `keep` takes, sorted.
std::string jtt_manifest(const stdfs::path& folder, const std::function<bool(const std::string&)>& keep) {
    std::vector<std::string> lines;
    for (const auto& entry : stdfs::recursive_directory_iterator(folder)) {
        if (!entry.is_regular_file()) continue;
        const std::string rel = stdfs::relative(entry.path(), folder).generic_string();
        if (!keep(rel)) continue;
        Bytes data = fs::read_file(entry.path());
        Bytes lf;
        for (std::size_t i = 0; i < data.size(); ++i)
            if (!(data[i] == '\r' && i + 1 < data.size() && data[i + 1] == '\n')) lf.push_back(data[i]);
        lines.push_back(std::format("{} {} {}\n", rel, lf.size(), crypto::to_hex(crypto::sha1(lf))));
    }
    std::sort(lines.begin(), lines.end());
    std::string out;
    for (const std::string& l : lines) out += l;
    return out;
}

bool jtt_have_jdk() {
    const std::optional<stdfs::path> java = decompile::jar::find_java({});
    std::error_code ec;
    return java && stdfs::is_regular_file(java->parent_path() / "javac.exe", ec);
}

bool jtt_fixture_class(const std::string& rel) {
    static const std::vector<std::string> kClasses = {"AxisAlignedBB", "Block", "Entity", "EntityMob", "EnumOS1", "ICamera", "IInventory", "MathHelper", "Vec3D"};
    return rel.find('/') == std::string::npos && std::find(kClasses.begin(), kClasses.end(), rel.substr(0, rel.rfind('.'))) != kClasses.end();
}

void jtt_convert_against_golden(const char* std_label, const char* golden_text, const char* golden_manifest) {
    if (!jtt_have_jdk()) return;
    JttSetup setup;
    const stdfs::path out = setup.dir.path() / (std::string("out") + (std::string(std_label) == "C++03" ? "03" : "17"));
    const JttRun r = jtt_run(setup.environment, {"convert", fs::to_text(jtt_fixtures()), fs::to_text(out), "--cpp-std", std_label});
    CB_CHECK_EQ(r.code, 0);
    // (the golden names the folder converted into: out03 / out17 under <out>)
    CB_CHECK_EQ(jtt_norm(r.out, setup.dir.path()), text::replace_all(jtt_golden(golden_text), "<out>/", "<out>/"));
    CB_CHECK_EQ(jtt_manifest(out, jtt_fixture_class), jtt_golden(golden_manifest));
}

}  // namespace

CB_TEST("java.tools.convert_cpp03_matches_the_golden", [] { jtt_convert_against_golden("C++03", "convert03.txt", "convert03_classes.manifest"); });
CB_TEST("java.tools.convert_cpp17_matches_the_golden", [] { jtt_convert_against_golden("C++17", "convert17.txt", "convert17_classes.manifest"); });

CB_TEST("java.tools.scaffold_in_each_language_matches_the_golden", [] {
    JttSetup setup;
    for (const auto& [language, golden, label] : {std::tuple{"c", "scaffold_c.manifest", "C"}, std::tuple{"csharp", "scaffold_cs.manifest", "C#"},
                                                  std::tuple{"rust", "scaffold_rust.manifest", "Rust"}}) {
        const stdfs::path out = setup.dir.path() / language;
        const JttRun r = jtt_run(setup.environment, {"scaffold", fs::to_text(jtt_fixtures()), fs::to_text(out), "--language", language});
        CB_CHECK_EQ(r.code, 0);
        CB_CHECK_EQ(jtt_norm(r.out, setup.dir.path()), std::format("scaffold: 9 {} skeleton file(s) for 9 class(es) -> <out>/{}\n", label, language));
        CB_CHECK_EQ(jtt_manifest(out, [](const std::string&) { return true; }), jtt_golden(golden));
    }
});

CB_TEST("java.tools.signatures_match_the_golden", [] {
    JttSetup setup;
    const stdfs::path out = setup.dir.path() / "signatures.md";
    const JttRun r = jtt_run(setup.environment, {"signatures", fs::to_text(jtt_fixtures()), "--out", fs::to_text(out)});
    CB_CHECK_EQ(r.code, 0);
    CB_CHECK_EQ(jtt_norm(r.out, setup.dir.path()), jtt_golden("signatures.txt"));
    // (the text names the command as `codebreaker java ...`, with no interpreter prefix)
    CB_CHECK_EQ(fs::read_text(out), text::replace_all(jtt_golden("signatures.md"), "`python -m codebreaker java", "`codebreaker java"));
});

CB_TEST("java.tools.apimap_lists_the_legacy_map_too", [] {
    JttSetup setup;
    const JttRun r = jtt_run(setup.environment, {"apimap", "subsystems"});
    CB_CHECK_EQ(r.code, 0);
    CB_CHECK_EQ(r.out, jtt_golden("apimap_subsystems.txt"));
});

CB_TEST("java.tools.debug_support_is_emitted_as_recorded", [] {
    JttSetup setup;
    for (const char* name : {"debug", "debug_off"}) {
        const stdfs::path out = setup.dir.path() / name;
        const bool off = std::string(name) == "debug_off";
        std::vector<std::string> args = {"debug", fs::to_text(out)};
        if (off) args.push_back("--off");
        const JttRun r = jtt_run(setup.environment, args);
        CB_CHECK_EQ(r.code, 0);
        CB_CHECK_EQ(r.out, jtt_golden(std::string(name) + ".txt"));
    }
});

CB_TEST("java.tools.the_rewrites_of_an_empty_tree_say_so", [] {
    JttSetup setup;
    const stdfs::path out = setup.dir.path() / "tree";
    fs::make_dirs(out);
    const std::string dir = fs::to_text(out);
    for (const auto& [tool, golden, code] : {std::tuple{"sound-fix", "sound_fix.txt", 0}, std::tuple{"render-fix", "render_fix.txt", 0}, std::tuple{"applet-ping", "applet_ping.txt", 0},
                                             std::tuple{"reflect-factories", "reflect.txt", 0}, std::tuple{"controller-nav", "controller_nav.txt", 1}}) {
        const JttRun r = jtt_run(setup.environment, {tool, dir});
        CB_CHECK_EQ(r.code, code);
        CB_CHECK_EQ(r.out, jtt_golden(golden));
    }
    CB_CHECK_EQ(jtt_run(setup.environment, {"ui-mark", dir, "--dry-run"}).out, jtt_golden("ui_mark.txt"));
    CB_CHECK_EQ(jtt_run(setup.environment, {"keybind-mark", dir, "--dry-run"}).out, jtt_golden("keybind_mark.txt"));
    CB_CHECK_EQ(jtt_norm(jtt_run(setup.environment, {"scan-project", fs::to_text(jtt_fixtures())}).out, setup.dir.path()), jtt_golden("scan_project.txt"));
});

CB_TEST("java.tools.platform_config_lists_and_writes", [] {
    JttSetup setup;
    const stdfs::path out = setup.dir.path() / "cfg";
    const JttRun list = jtt_run(setup.environment, {"platform-config", fs::to_text(out), "--list"});
    CB_CHECK_EQ(list.code, 0);
    CB_CHECK(list.out.find("Sensitivity: **1.00**   Stick deadzone: **0.24**   Trigger: **0.30**") != std::string::npos);
    CB_CHECK(!stdfs::exists(out / "platform.config.json"));   // --list writes nothing
    // a medium that needs no XDK symbol is written without the SDK: the memory card
    const JttRun w = jtt_run(setup.environment, {"platform-config", fs::to_text(out), "--storage", "memory", "--keyboard", "stub", "--layout", "southpaw", "--invert-y",
                                                 "--bind", "jump=B", "--platform", "pc_windows"});
    CB_CHECK_EQ(w.code, 0);
    const std::string config = fs::read_text(out / "platform.config.json");
    CB_CHECK(config.find("southpaw") != std::string::npos && config.find("\"jump\": \"B\"") != std::string::npos);
    // the settings it wrote are kept by the next run
    const JttRun again = jtt_run(setup.environment, {"platform-config", fs::to_text(out), "--platform", "pc_windows"});
    CB_CHECK(again.out.find("layout=southpaw invertY=True | keyboard=stub | storage=memory") != std::string::npos);
});

CB_TEST("java.tools.misuse", [] {
    JttSetup setup;
    CB_CHECK_EQ(jtt_run(setup.environment, {}).code, 2);                                          // no tool: the list, as a failure
    CB_CHECK_EQ(jtt_run(setup.environment, {"platform-config", "x", "--layout", "sideways"}).code, 2);   // a choice that is not one
    CB_CHECK_EQ(jtt_run(setup.environment, {"debug"}).code, 2);                                   // a missing argument
    const JttRun missing = jtt_run(setup.environment, {"scaffold", fs::to_text(setup.dir.path() / "no-such-folder"), fs::to_text(setup.dir.path() / "o")});
    CB_CHECK(missing.code != 0 || missing.out.find("0 C skeleton") != std::string::npos);
    const JttRun bad_sdk = jtt_run(setup.environment, {"sdk", "wii", "--sdk", fs::to_text(setup.dir.path())});
    CB_CHECK_EQ(bad_sdk.code, 1);   // no SDK there: said, and the exit code says so
    const JttRun no_dir = jtt_run(setup.environment, {"worklist", fs::to_text(setup.dir.path() / "none")});
    CB_CHECK_EQ(no_dir.code, 2);
    CB_CHECK(no_dir.err.find("ERROR: not a directory") != std::string::npos);
});
