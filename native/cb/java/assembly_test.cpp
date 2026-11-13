// Tests of the transpiler's assembly (assembly.hpp): the services made from the lane's data, the front end handed to the translator
// and the project's fixes applied after it, and the conversion a lane asks for (a dialect, no platform, no report). The units the
// translator writes bodies from are the front end's; here it serves none, which the back end declines body by body -- what is
// checked is what the assembly does: who is asked, with what, and what a conversion leaves. The whole path with the units of real
// Java is javajudge_lane_test's (translate).

#include "cb/java/assembly.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::java::assembly {
namespace {

namespace stdfs = std::filesystem;

JavaData asm_data() { return JavaData(testing::data_path("../../codebreaker/java")); }
stdfs::path asm_source() { return testing::data_path("java/testdata/source"); }

CB_TEST("java.assembly.a_conversion_asks_the_front_end_for_the_programs_units", [] {
    std::vector<stdfs::path> asked;
    Assembly assembly(asm_data(), [&](Arena&, const stdfs::path& dir) {
        asked.push_back(dir);
        return std::vector<translate::ir::Node*>{};
    });
    fs::ScopedTempDir tmp("cb_assembly");
    const transpile::Summary done = assembly.converter("C++03")(asm_source(), tmp.path() / "out");
    CB_CHECK_EQ(asked.size(), std::size_t{1});
    if (!asked.empty()) CB_CHECK(asked.front() == asm_source());
    CB_CHECK(done.parsed > 0);
    CB_CHECK(done.headers > 0);
    CB_CHECK_EQ(done.cpp_std, "C++03");
    CB_CHECK_EQ(done.platform, "none");
    CB_CHECK(!done.report.has_value());                                    // the lane asks for no report
    CB_CHECK(stdfs::is_regular_file(tmp.path() / "out" / "JavaRT.h"));      // the runtime is written beside the classes
});

CB_TEST("java.assembly.the_dialect_is_the_one_asked_for_and_one_assembly_converts_again", [] {
    Assembly assembly(asm_data(), [](Arena&, const stdfs::path&) { return std::vector<translate::ir::Node*>{}; });
    fs::ScopedTempDir tmp("cb_assembly");
    const Converter c17 = assembly.converter("C++17");
    const Converter c03 = assembly.converter("C++03");
    const transpile::Summary a = c17(asm_source(), tmp.path() / "a");
    const transpile::Summary b = c03(asm_source(), tmp.path() / "b");
    const transpile::Summary again = c17(asm_source(), tmp.path() / "c");
    CB_CHECK_EQ(a.cpp_std, "C++17");
    CB_CHECK_EQ(b.cpp_std, "C++03");
    CB_CHECK_EQ(again.cpp_std, "C++17");
    CB_CHECK_EQ(again.parsed, a.parsed);                                   // the same program, the same classes
    CB_CHECK_THROWS_AS(UsageError, assembly.converter("C++99")(asm_source(), tmp.path() / "d"));   // not a dialect
});

CB_TEST("java.assembly.the_services_are_the_assemblys_own", [] {
    Assembly assembly(asm_data(), [](Arena&, const stdfs::path&) { return std::vector<translate::ir::Node*>{}; });
    const transpile::Services services = assembly.services();
    CB_CHECK(&services.runtime == &assembly.services().runtime);
    CB_CHECK(!services.profiles.names().empty());
    CB_CHECK(services.profiles.find("minecraft_xbox360") != nullptr);       // the project's era profiles are registered
    CB_CHECK_EQ(services.projects.project("minecraft").name, "minecraft");
});

}  // namespace
}  // namespace cb::java::assembly
