// Tests of cb/base/tool_catalog: the shipped catalog reads, a tool is found by its rules over folders made here, and the attached tools are
// what a setting's value comes from once settings.json says nothing (the scan of this computer itself is not tested).
#include "base/tool_catalog.hpp"

#include <algorithm>
#include <set>

#include "base/config.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace tc = cb::config::tools;
namespace stdfs = std::filesystem;

namespace {

void tct_file(const stdfs::path& p, const std::string& body = "") {
    cb::fs::make_dirs(p.parent_path());
    cb::fs::write_text(p, body);
}

}  // namespace

CB_TEST("base.tool_catalog.the_shipped_catalog_reads_and_names_path_settings", [] {
    const std::vector<tc::Tool> catalog = tc::catalog_of(cb::config::find_checkout());
    CB_CHECK(catalog.size() >= 20);
    std::set<std::string> ids;
    for (const tc::Tool& t : catalog) {
        CB_CHECK(ids.insert(t.id).second);   // (one file per tool)
        CB_CHECK(!t.name.empty() && !t.role.empty() && !t.description.empty());
        CB_CHECK(t.role == "compiler" || t.role == "assembler" || t.role == "decompiler" || t.role == "build" || t.role == "sdk");
        if (!t.setting.empty()) CB_CHECK(cb::config::Config::is_path_setting(t.setting));
        if (t.built_in) CB_CHECK(t.setting.empty() && t.find.empty());
    }
    const auto java = std::find_if(catalog.begin(), catalog.end(), [](const tc::Tool& t) { return t.id == "java"; });
    CB_CHECK(java != catalog.end() && java->least == 11 && java->pick == "newest" && java->setting == "java");
});

CB_TEST("base.tool_catalog.a_tool_is_found_by_its_rules_and_the_newest_wins", [] {
    cb::fs::ScopedTempDir dir{"cbtoolcat"};
    const stdfs::path root = dir.path();
    for (const auto& [name, version] : {std::pair{"jdk-17", "17.0.2"}, {"jdk-21", "21.0.1"}, {"jdk1.8", "1.8.0_381"}}) {
        tct_file(root / "jdks" / name / "bin" / "java.exe");
        tct_file(root / "jdks" / name / "bin" / "javac.exe");
        tct_file(root / "jdks" / name / "release", std::string("JAVA_VERSION=\"") + version + "\"\n");
    }
    tct_file(root / "watcom" / "binnt" / "wcc386.exe");
    tct_file(root / "catalog" / "jdk.json",
             R"({"id": "jdk", "name": "JDK", "role": "compiler", "description": "x", "setting": "java", "value": "bin/java.exe", "order": 2,
                 "requires": ["bin/java.exe", "bin/javac.exe"], "version": {"file": "release", "key": "JAVA_VERSION", "least": 11}, "pick": "newest",
                 "find": [{"dir": ")" + (root / "jdks").generic_string() + R"(", "children": true}]})");
    tct_file(root / "catalog" / "ow.json",
             R"({"id": "ow", "name": "OpenWatcom", "role": "compiler", "description": "x", "setting": "openwatcom", "order": 1,
                 "requires_any": ["binnt/wcc386.exe", "binnt64/wcc386.exe"], "find": [{"dir": ")" + (root / "nowhere").generic_string() + R"("},
                 {"dir": ")" + (root / "watcom").generic_string() + R"("}]})");
    tct_file(root / "catalog" / "mine.json", R"({"id": "mine", "name": "Mine", "role": "decompiler", "description": "x", "built_in": true})");
    const std::vector<tc::Tool> catalog = tc::load_catalog(root / "catalog");
    CB_CHECK_EQ(catalog.size(), 3u);
    CB_CHECK_EQ(catalog.front().id, std::string("ow"));   // (by order)
    const std::vector<tc::Detected> found = tc::scan(catalog);
    CB_CHECK_EQ(found.size(), 2u);   // (a built-in tool is not looked for)
    CB_CHECK(found[0].id == "ow" && found[0].home == (root / "watcom").lexically_normal());
    CB_CHECK(found[1].id == "jdk" && found[1].version.value_or(0) == 21);
    CB_CHECK(stdfs::path(found[1].value) == (root / "jdks" / "jdk-21" / "bin" / "java.exe").lexically_normal());
    // a value names the tool's folder back, and an old JDK is not the right one
    CB_CHECK(tc::home_of(catalog[1], found[1].value) == (root / "jdks" / "jdk-21").lexically_normal());
    const tc::Check old = tc::check(catalog[1], root / "jdks" / "jdk1.8");
    CB_CHECK(!old.ok && old.problem.find("older than 11") != std::string::npos);
});

CB_TEST("base.tool_catalog.an_attached_tool_is_the_value_settings_json_does_not_give", [] {
    cb::fs::ScopedTempDir dir{"cbtoolcat"};
    const stdfs::path root = dir.path();
    tct_file(root / "config" / "tools" / "java.json", R"({"id": "java", "name": "Java", "role": "compiler", "description": "x", "setting": "java"})");
    tct_file(root / "config" / "tools" / "cmake.json", R"({"id": "cmake", "name": "CMake", "role": "build", "description": "x", "setting": "cmake"})");
    tct_file(root / "settings.json", R"({"cmake": "C:/mine/cmake.exe"})");
    const stdfs::path file = root / "attached.json";
    tc::write_attached(file, {{"java", {"C:/jdk/bin/java.exe", "found", "21"}}, {"cmake", {"C:/found/cmake.exe", "found", ""}}});
    const auto back = tc::read_attached(file);
    CB_CHECK(back.size() == 2 && back.at("java").how == "found" && back.at("java").version == "21");
    const cb::fs::ScopedEnv named("CB_TOOLS_FILE", cb::fs::to_text(file));
    const cb::config::Config c = cb::config::Config::load(root);
    CB_CHECK_EQ(c.text("java"), std::string("C:/jdk/bin/java.exe"));
    CB_CHECK(c.source("java") == cb::config::Config::Source::Attached);
    CB_CHECK_EQ(c.text("cmake"), std::string("C:/mine/cmake.exe"));   // (settings.json comes first)
    CB_CHECK(c.source("cmake") == cb::config::Config::Source::SettingsFile);
    CB_CHECK(c.source("ilspycmd") == cb::config::Config::Source::None);
});
