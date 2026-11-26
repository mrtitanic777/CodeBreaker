// Tests of cb/base/host_tools: how a JDK is recognised and which one wins (over folders made here; the scan of this computer is not tested).
#include "base/host_tools.hpp"

#include "base/config.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace ht = cb::config::host_tools;
namespace stdfs = std::filesystem;

namespace {

stdfs::path fake_jdk(const stdfs::path& root, const std::string& name, const std::string& version, bool javac = true) {
    const stdfs::path home = root / name;
    cb::fs::make_dirs(home / "bin");
    cb::fs::write_text(home / "bin" / "java.exe", "");
    if (javac) cb::fs::write_text(home / "bin" / "javac.exe", "");
    cb::fs::write_text(home / "release", "IMPLEMENTOR=\"Eclipse Adoptium\"\nJAVA_VERSION=\"" + version + "\"\n");
    return home;
}

}  // namespace

CB_TEST("base.host_tools.a_java_version_reads_as_its_major_number", [] {
    CB_CHECK_EQ(ht::java_major("17.0.2").value_or(0), 17);
    CB_CHECK_EQ(ht::java_major("\"1.8.0_381\"").value_or(0), 8);
    CB_CHECK_EQ(ht::java_major("21").value_or(0), 21);
    CB_CHECK(!ht::java_major("").has_value());
});

CB_TEST("base.host_tools.the_newest_jdk_with_a_compiler_of_at_least_java_11_wins", [] {
    cb::fs::ScopedTempDir dir{"cbhosttools"};
    const stdfs::path old = fake_jdk(dir.path(), "jdk1.8.0_381", "1.8.0_381");
    const stdfs::path jdk17 = fake_jdk(dir.path(), "jdk-17", "17.0.2");
    const stdfs::path jre21 = fake_jdk(dir.path(), "jre-21", "21.0.1", false);   // (a runtime: no javac)
    const stdfs::path jdk21 = fake_jdk(dir.path(), "jdk-21", "21.0.1");
    CB_CHECK_EQ(ht::jdk_version(old).value_or(0), 8);
    CB_CHECK(!ht::jdk_version(jre21).has_value());
    CB_CHECK(ht::newest_jdk({old, jdk17, jre21, jdk21}) == jdk21);
    CB_CHECK(ht::newest_jdk({old, jdk17}) == jdk17);
    CB_CHECK(!ht::newest_jdk({old, jre21}).has_value());
    CB_CHECK(!ht::newest_jdk({dir.path() / "nothing"}).has_value());
});

CB_TEST("base.host_tools.a_loaded_configuration_does_not_look_on_this_computer", [] {
    cb::fs::ScopedTempDir dir{"cbhosttools"};
    const cb::config::Config c = cb::config::Config::load(dir.path());
    CB_CHECK(c.found_on_host().empty());
    CB_CHECK(cb::config::Config::is_path_setting("cmake"));
});
