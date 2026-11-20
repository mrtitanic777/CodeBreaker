// cb-unity: alone (test helpers named for this module)
// Tests of `judge-c-java`: the command's shape and, where the machine has a JDK and clang, the round trip with a Host made from the production
// pieces (the Java lane's converter, clang reading its C++ back, j2x, the Java back end, javac and the JVM) on the Java lane's fixture classes.
// The round trip gives `equivalent`, tier `idiomatic`, no notes, and the Java in testdata/j2x_RoundTripped.java.txt for them.
#include "translate/judge_c_java_command.hpp"

#include <algorithm>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "java/production.hpp"

namespace {

namespace jc = cb::translate::judge_c_java;
namespace stdfs = std::filesystem;

jc::Environment jct_environment() { return {cb::config::Config::load(cb::testing::data_path(".."))}; }

bool jct_machine_has_the_tools(const jc::Environment& e) {
    const auto java = cb::java::production::find_java(e.config);
    if (!java) return false;
    std::error_code ec;
    const bool javac = stdfs::is_regular_file(java->parent_path() / "javac.exe", ec);
    const stdfs::path vs = R"(C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin\clang.exe)";
    return javac && (stdfs::is_regular_file(vs, ec) || e.config.path("clang").has_value());
}

}  // namespace

CB_TEST("translate.judge_c_java.the_command_has_a_roundtrip_subcommand", [] {
    const cb::cli::Command c = jc::command(jct_environment());
    CB_CHECK_EQ(c.name, std::string("judge-c-java"));
    CB_CHECK_EQ(c.subcommands.size(), std::size_t(1));
    CB_CHECK_EQ(c.subcommands[0].name, std::string("roundtrip"));
    CB_CHECK(static_cast<bool>(c.subcommands[0].handler));
    CB_CHECK(!c.summary.empty());
});

CB_TEST("translate.judge_c_java.default_inputs_are_the_fixture_classes", [] {
    const jc::Environment e = jct_environment();
    cb::fs::ScopedTempDir staging{"jcj", cb::testing::data_path("../state/work/judge-c-java-tests")};
    cb::fs::make_dirs(staging.path());
    const jc::RoundTripInputs inputs = jc::default_inputs(e, staging.path());
    CB_CHECK_EQ(inputs.main_class, std::string("sample.RoundTrip"));
    CB_CHECK_EQ(inputs.files.size(), std::size_t(6));
    for (const auto& f : inputs.files) CB_CHECK(stdfs::is_regular_file(f));
    const std::string base = cb::fs::read_text(inputs.files[4]);
    CB_CHECK(base.starts_with("package sample;\n\npublic class Base {"));
    CB_CHECK(base.find("this.scale += var2;") != std::string::npos);
});

CB_TEST("translate.judge_c_java.the_roundtrip_command_gives_the_expected_verdict", [] {
    const jc::Environment e = jct_environment();
    if (!jct_machine_has_the_tools(e)) return;
    cb::fs::make_dirs(cb::testing::data_path("../state/work/judge-c-java-tests"));
    const stdfs::path work = cb::testing::data_path("../state/work/judge-c-java-tests/rt");
    // the verdict and the Java it wrote
    cb::fs::ScopedTempDir staging{"jcj", cb::testing::data_path("../state/work/judge-c-java-tests")};
    const jc::RoundTripInputs inputs = jc::default_inputs(e, staging.path());
    const jc::ProductionHost made = jc::make_host(e);
    const auto v = cb::translate::judge_native_jvm::roundtrip(made.host, inputs.files, inputs.main_class, work);
    CB_CHECK_EQ(v.reason, std::string());
    CB_CHECK(v.verdict == cb::translate::judge_native_jvm::Kind::Equivalent);
    CB_CHECK_EQ(v.tier.value_or(""), std::string("idiomatic"));
    CB_CHECK(v.notes.empty());
    std::string golden = cb::fs::read_text(cb::testing::data_path("translate/frontends/clang/testdata/j2x_RoundTripped.java.txt"));
    std::string wrote = v.java.value_or("");
    golden.erase(std::remove(golden.begin(), golden.end(), '\r'), golden.end());
    wrote.erase(std::remove(wrote.begin(), wrote.end(), '\r'), wrote.end());
    CB_CHECK_EQ(wrote, golden);
    // and the command says so
    cb::testing::CapturedStd captured;
    const int code = jc::main({"roundtrip", "--work", cb::fs::to_text(work)}, e);
    CB_CHECK_EQ(code, 0);
    const std::string out = captured.out();
    CB_CHECK(out.find("Round trip: equivalent") != std::string::npos);
    CB_CHECK(out.find("| Java -> C++ -> Java: Vec3D, MathHelper, AxisAlignedBB, Base, Sub, RoundTrip | equivalent | idiomatic |") != std::string::npos);
    std::error_code ec;
    stdfs::remove_all(work, ec);
});
