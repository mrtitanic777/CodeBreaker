// Tests of cb/formats/xbox/system_ui.hpp. There is no console system update on this machine: the packages (testdata/system_ui) are STFS
// packages made with `stfs.build` that hold the files a system update has, as junk (the module files are not XEX files, so
// the xex program finds no resources in them) and fonts of random bytes. The command's output and the files it wrote are what `codebreaker
// system-ui ...` printed and wrote for the same packages (system_ui.txt: each case's output with `exit=N`; system_ui_files.txt: "case:
// file size sha1"). The XUIZ filter is run with a stand-in for the xex program (a batch file), and the retail Minecraft module with the
// real one when it is here (Minecraft's retail module carries one XUI package).
#include "cb/formats/xbox/system_ui.hpp"

#include <map>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/command_test_kit.hpp"
#include "cb/formats/xbox/stfs.hpp"

using namespace cb;
namespace system_ui = cb::formats::xbox::system_ui;

namespace {

std::filesystem::path sysui_data(const char* name) { return testing::data_path(std::string("formats/testdata/system_ui/") + name); }

// The golden's section for a case: its output lines.
std::string sysui_case(const std::string& name) {
    const std::string log = fs::read_text(sysui_data("system_ui.txt"));
    std::string out;
    bool inside = false;
    for (const std::string_view line : text::lines(log)) {
        if (line.starts_with("== ")) inside = line.substr(3) == name;
        else if (inside) out += std::string(line) + "\n";
    }
    return out;
}

// The golden's files of a case, as a manifest ("name size sha1", sorted).
std::string sysui_files(const std::string& name) {
    const std::string log = fs::read_text(sysui_data("system_ui_files.txt"));
    std::vector<std::string> lines;
    for (const std::string_view line : text::lines(log))
        if (line.starts_with(name + ": ")) lines.push_back(std::string(line.substr(name.size() + 2)));
    std::sort(lines.begin(), lines.end());
    std::string out;
    for (const std::string& line : lines) out += line + "\n";
    return out;
}

// A stand-in for the xex engine's resource dump: every module carries two XUI packages (the third resource is not one, so it is not asked).
system_ui::Environment sysui_fake() {
    const auto blob = [](std::string_view text) { return Bytes(text.begin(), text.end()); };
    return {[blob](ByteView) { return std::map<std::string, Bytes>{{"skin", blob("XUIZ one\r\n")}, {"scene", blob("XUIZ two\r\n")}}; }};
}

}  // namespace

CB_TEST("formats.xbox.system_ui.a_system_update_gives_its_fonts_and_the_xui_packages_of_its_modules", [] {
    const fs::ScopedTempDir dir{"cb-sysui"};
    const system_ui::Environment env = sysui_fake();   // (the modules of the update are junk: the stand-in answers for all of them)
    const std::filesystem::path package = sysui_data("su_full.bin");
    std::vector<std::string> lines;
    const std::size_t count = system_ui::extract(env, package, dir.path() / "out", std::nullopt, [&](const std::string& l) { lines.push_back(l); });
    // eight modules, two XUI packages each (the third resource is not one), then three fonts
    CB_CHECK_EQ(count, std::size_t{8 * 2 + 3});
    CB_CHECK_EQ(lines.front(), std::format("{:<40} {:>8}", "xam.scene.xzp", 10));
    CB_CHECK_EQ(lines[1], std::format("{:<40} {:>8}", "xam.skin.xzp", 10));
    CB_CHECK_EQ(lines.size(), std::size_t{19});
    CB_CHECK(std::filesystem::exists(dir.path() / "out" / "deviceselector.skin.xzp"));
    CB_CHECK(!std::filesystem::exists(dir.path() / "out" / "xam.media.xzp"));
    // the online modules: those a folder has
    fs::make_dirs(dir.path() / "online" / "17559");
    fs::write_file(dir.path() / "online" / "17559" / "GUIDE.friends.XEX", Bytes{1});
    lines.clear();
    system_ui::extract(env, package, dir.path() / "out2", dir.path() / "online", [&](const std::string& l) { lines.push_back(l); });
    CB_CHECK(std::filesystem::exists(dir.path() / "out2" / "guide.friends.scene.xzp"));
    CB_CHECK_EQ(lines[lines.size() - 3], std::format("{:<40} {:>8}", "guide.friends.skin.xzp", 10));
    CB_CHECK(lines.back().starts_with(std::format("{:<40} (not in ", "Guide.Voicemail.xex")));
});

CB_TEST("formats.xbox.system_ui.the_command_prints_the_recorded_output", [] {
    const fs::ScopedTempDir dir{"cb-sysui"};
    system_ui::Environment env;
    // the junk modules carry no resources the xex engine can read: only the three fonts are written, and the count says so
    const std::string folder = fs::to_text(dir.path());
    const std::string printed = testing::run_command(system_ui::command(env), {fs::to_text(sysui_data("su_full.bin")), folder + "/o1"});
    CB_CHECK(printed.ends_with(std::format("3 files -> {}/o1\nexit=0\n", folder)));
    CB_CHECK(!std::filesystem::exists(dir.path() / "o1" / "xam.skin.xzp"));
});

CB_TEST("formats.xbox.system_ui.the_junk_modules_have_no_resources_for_the_real_xex_program", [] {
    const fs::ScopedTempDir dir{"cb-sysui"};
    system_ui::Environment env;
    const std::string folder = fs::to_text(dir.path());
    const std::string package = fs::to_text(sysui_data("su_full.bin"));
    std::string printed = testing::run_command(system_ui::command(env), {package, folder + "/o1"});
    printed = text::replace_all(printed, folder, "<w>");
    CB_CHECK_EQ(printed, sysui_case("full"));
    CB_CHECK_EQ(testing::tree_manifest(dir.path() / "o1"), sysui_files("full"));
    // with an online folder holding a module that is not an XEX
    fs::make_dirs(dir.path() / "online" / "17559");
    fs::write_file(dir.path() / "online" / "17559" / "Guide.Friends.xex", as_bytes("not an xex either"));
    printed = testing::run_command(system_ui::command(env), {package, folder + "/o2", folder + "/online"});
    printed = text::replace_all(text::replace_all(printed, folder + "/online", "<online>"), folder, "<w>");
    CB_CHECK_EQ(printed, sysui_case("online"));
    CB_CHECK_EQ(testing::tree_manifest(dir.path() / "o2"), sysui_files("online"));
});

CB_TEST("formats.xbox.system_ui.a_module_carries_its_xui_package_as_a_resource", [] {
    const fs::ScopedTempDir dir{"cb-sysui"};
    const auto retail = testing::game_file("Minecraft Xbox 360 Edition/default.xex");
    if (!retail) return;
    // Minecraft's retail module has two resources: its title data (XDBF) and its media (a XUI package)
    const auto packages = system_ui::xui_packages(fs::read_file(*retail));
    CB_CHECK_EQ(packages.size(), std::size_t{1});
    CB_CHECK(packages.contains("media"));
    if (packages.contains("media")) {
        CB_CHECK_EQ(packages.at("media").size(), std::size_t{34520841});
        CB_CHECK_EQ(as_text(ByteView(packages.at("media")).first(4)), "XUIZ");
    }
});

CB_TEST("formats.xbox.system_ui.a_package_without_a_module_or_a_font_is_refused", [] {
    const fs::ScopedTempDir dir{"cb-sysui"};
    const auto report = [](const std::string&) {};
    CB_CHECK_THROWS_AS(FormatError, system_ui::extract(system_ui::Environment{}, sysui_data("su_other.bin"), dir.path() / "a", std::nullopt, report));
    CB_CHECK_THROWS_AS(FormatError, system_ui::extract(system_ui::Environment{}, sysui_data("su_nofont.bin"), dir.path() / "b", std::nullopt, report));
    CB_CHECK(std::filesystem::exists(dir.path() / "b" / "SegoeXbox-Light.xtt"));   // (written before the missing font was met)
    CB_CHECK_THROWS_AS(FormatError, system_ui::extract(system_ui::Environment{}, testing::data_path("formats/testdata/lz32.dll"), dir.path() / "c", std::nullopt, report));
});
