// cb-unity: alone
// Tests of the translator's registry (registry.hpp): routes are computed from the registered front ends and back ends, and the table capability.hpp
// publishes is exactly the table that was written by hand before.
#include "translate/registry.hpp"

#include <algorithm>

#include "core/testing.hpp"
#include "translate/capability.hpp"

namespace {

namespace reg = cb::translate::registry;
namespace cap = cb::translate::capability;

reg::RuleSet regt_rule(const char* from, bool file_level) { return reg::RuleSet{from, "judge", "proof", {"tool"}, file_level}; }

}  // namespace

CB_TEST("translate.registry.a_route_needs_both_a_front_end_and_a_back_end", [] {
    reg::Registry r;
    r.add(reg::BackEnd{"cpp17", "emitter", {regt_rule("csharp", true), regt_rule("python", true)}});
    CB_CHECK(r.routes().empty());   // no front end yet
    r.add(reg::FrontEnd{"csharp", "Roslyn", {".cs"}});
    CB_CHECK_EQ(r.routes().size(), std::size_t{1});
    CB_CHECK_EQ(r.routes()[0].from, "csharp");
    CB_CHECK_EQ(r.routes()[0].to, "cpp17");
    CB_CHECK_EQ(r.routes()[0].front_end, "Roslyn");
    r.add(reg::FrontEnd{"python", "ast", {".py"}});
    CB_CHECK_EQ(r.routes().size(), std::size_t{2});
    CB_CHECK_EQ(r.file_routes().size(), std::size_t{2});
});

CB_TEST("translate.registry.file_routes_are_the_file_level_rule_sets_in_language_order", [] {
    reg::Registry r;
    r.add(reg::FrontEnd{"python", "ast", {".py"}});
    r.add(reg::FrontEnd{"csharp", "Roslyn", {".cs"}});
    r.add(reg::BackEnd{"csharp", "b1", {regt_rule("csharp", true), regt_rule("python", false)}});
    r.add(reg::BackEnd{"cpp17", "b2", {regt_rule("python", true), regt_rule("csharp", true)}});
    const std::vector<reg::Route> all = r.routes();
    CB_CHECK_EQ(all.size(), std::size_t{4});
    const std::vector<reg::Route> files = r.file_routes();
    CB_CHECK_EQ(files.size(), std::size_t{3});
    CB_CHECK(files[0].from == "csharp" && files[0].to == "cpp17");
    CB_CHECK(files[1].from == "csharp" && files[1].to == "csharp");
    CB_CHECK(files[2].from == "python" && files[2].to == "cpp17");
});

CB_TEST("translate.registry.the_standard_registry_holds_every_front_end_and_back_end", [] {
    const reg::Registry& r = reg::standard();
    for (const char* lang : {"c", "csharp", "java", "python"}) CB_CHECK(r.front_end_for(lang) != nullptr);
    for (const char* lang : {"c", "cpp03", "cpp17", "csharp", "java"}) CB_CHECK(r.has_back_end(lang));
    CB_CHECK(!r.has_back_end("python"));
    CB_CHECK(r.front_end_for("cpp17") == nullptr);
    // every route the survey lists is computed
    auto has = [&](const char* from, const char* to, const char* emitter) {
        const auto all = r.routes();
        return std::any_of(all.begin(), all.end(), [&](const reg::Route& x) { return x.from == from && x.to == to && x.emitter == emitter; });
    };
    CB_CHECK(has("csharp", "cpp17", "backends/csharp_cpp17"));
    CB_CHECK(has("csharp", "csharp", "backends/csharp"));
    CB_CHECK(has("csharp", "cpp03", "dotnet/cs2cpp with backends/cpp03"));
    CB_CHECK(has("java", "cpp03", "backends/cpp03"));
    CB_CHECK(has("java", "cpp17", "backends/cpp03_cpp17 (Bodies17)"));
    CB_CHECK(has("java", "cpp17", "l2 core cpp17"));
    CB_CHECK(has("java", "c", "backends/c_java"));
    CB_CHECK(has("java", "csharp", "backends/csharp"));
    CB_CHECK(has("c", "java", "backends/java"));
    CB_CHECK(has("python", "cpp17", "l2 core cpp17"));
});

CB_TEST("translate.registry.the_published_paths_are_the_table_written_by_hand_before", [] {
    const std::vector<cap::Path>& paths = cap::paths();
    CB_CHECK_EQ(paths.size(), std::size_t{3});
    CB_CHECK(paths[0].from == "csharp" && paths[0].to == "cpp17" && paths[0].judge == "judge-cs-cpp");
    CB_CHECK_EQ(paths[0].proof,
                "builds the C++17 translation, runs it beside the C# program (csc and the installed .NET), and compares what they print and their exit code");
    CB_CHECK((paths[0].needs == std::vector<std::string>{"dotnet", "cxx"}));
    CB_CHECK(paths[1].from == "csharp" && paths[1].to == "csharp" && paths[1].judge == "judge-cs-cpp");
    CB_CHECK_EQ(paths[1].proof, "writes the program as C# again, runs both (csc and the installed .NET), and compares what they print and their exit code");
    CB_CHECK((paths[1].needs == std::vector<std::string>{"dotnet"}));
    CB_CHECK(paths[2].from == "python" && paths[2].to == "cpp17" && paths[2].judge == "judge-py-cpp");
    CB_CHECK_EQ(paths[2].proof, "builds the C++17 translation, runs it beside the Python module (CPython), and compares the answer of every run_* function");
    CB_CHECK((paths[2].needs == std::vector<std::string>{"python", "cxx", "l2"}));
});

CB_TEST("translate.registry.the_published_languages_are_the_table_written_by_hand_before", [] {
    const std::vector<cap::Language>& langs = cap::languages();
    CB_CHECK_EQ(langs.size(), std::size_t{6});
    const char* ids[] = {"c", "cpp03", "cpp17", "csharp", "java", "python"};
    const char* names[] = {"C", "C++03", "C++17", "C#", "Java", "Python"};
    const bool back[] = {true, true, true, true, true, false};
    const std::vector<std::string> ext[] = {{".c", ".h"}, {}, {}, {".cs"}, {".java"}, {".py"}};
    for (std::size_t i = 0; i < 6; ++i) {
        CB_CHECK_EQ(langs[i].id, ids[i]);
        CB_CHECK_EQ(langs[i].name, names[i]);
        CB_CHECK_EQ(langs[i].back_end, back[i]);
        CB_CHECK((langs[i].extensions == ext[i]));
    }
});
