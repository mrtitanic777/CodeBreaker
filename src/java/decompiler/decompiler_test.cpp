// Tests of the whole decompiler: a jar decompiled with RetroMCP's options gives, class for class, the text the original Fernflower gives. The golden
// (testdata/java/gradle-wrapper) is the original's own output: fernflower-ordered.jar (testdata/oracle/README.txt) run as
//     java -cp fernflower-ordered.jar de.fernflower.main.decompiler.ConsoleDecompiler -nco=1 -rbr=0 -asc=1 -ovr=0 -dgs=1 -ind=<tab> <jar> <out>
// and its .java entries unpacked. RetroMCP's own jar (585 classes; the build the golden was made of, work/fernflower-src/retromcp.jar) is checked the
// same way when its golden is in work/fernflower-src/golden.
#include "java/decompiler/decompiler.hpp"

#include <cstdio>
#include <filesystem>

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::java::decompiler {
namespace {

DecompileRequest retromcp_request(const std::filesystem::path& jar) {
    DecompileRequest req;
    req.sources.push_back(jar);
    req.options = {{"nco", "1"}, {"rbr", "0"}, {"asc", "1"}, {"ovr", "0"}, {"dgs", "1"}, {"ind", "\t"}};
    return req;
}

// Every .java under `golden` against the run's entry of the same name; the differing ones named on stderr. The number that match.
int check_against(const DecompileResult& res, const std::filesystem::path& golden, int& total) {
    const auto texts = res.java_texts();
    std::map<std::string, std::string> by_entry;
    for (const auto& [name, text] : texts) {
        const auto bang = name.find("!/");
        by_entry[bang == std::string::npos ? name : name.substr(bang + 2)] = text;
    }
    int same = 0;
    total = 0;
    for (const auto& e : std::filesystem::recursive_directory_iterator(golden)) {
        if (!e.is_regular_file() || e.path().extension() != ".java") continue;
        ++total;
        const std::string rel = fs::to_text(std::filesystem::relative(e.path(), golden).generic_string());
        const auto it = by_entry.find(rel);
        if (it != by_entry.end() && it->second == fs::read_text(e.path())) ++same;
        else std::fprintf(stderr, "  differs from the original: %s\n", rel.c_str());
    }
    return same;
}

CB_TEST("java.decompiler.decompile.a_jar_decompiles_as_the_original_decompiles_it", [] {
    const DecompileResult res = decompile(retromcp_request(testing::data_path("java/retromcp/gradle/wrapper/gradle-wrapper.jar")));
    int total = 0;
    const int same = check_against(res, testing::data_path("java/decompiler/testdata/java/gradle-wrapper"), total);
    CB_CHECK_EQ(total, 31);
    CB_CHECK_EQ(same, total);
    // the original's failures are reproduced too: a method it could not write is a comment in its body
    CB_CHECK(res.java_texts().size() == 31u);
});

CB_TEST("java.decompiler.decompile.retromcp_jar_as_the_original", [] {
    const std::filesystem::path golden = testing::data_path("../work/fernflower-src/golden/retromcp");
    std::error_code ec;
    if (!std::filesystem::is_directory(golden, ec)) return;   // (the big golden lives in work/, made as the small one)
    const DecompileResult res = decompile(retromcp_request(testing::data_path("../work/fernflower-src/retromcp.jar")));   // (the jar the golden was made of)
    int total = 0;
    const int same = check_against(res, golden, total);
    CB_CHECK_EQ(total, 585);
    CB_CHECK_EQ(same, total);
});

CB_TEST("java.decompiler.decompile.options", [] {
    std::map<std::string, std::string> o;
    CB_CHECK(parse_option_argument("-dgs=true", o) && o["dgs"] == "1");
    CB_CHECK(parse_option_argument("-rbr=0", o) && o["rbr"] == "0");
    CB_CHECK(!parse_option_argument("game.jar", o));
});

}  // namespace
}  // namespace cb::java::decompiler
