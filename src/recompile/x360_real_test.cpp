// recompile/x360_real_test.cpp -- the two slow checks that build and run real programs (set CB_SLOW_TESTS=1; each prints SKIP
// otherwise, or when a tool or the game is not on this machine):
//
//  - the runtime's xex::ReadImage (src/recompile/x360/runtime/src/xex.cpp, the path XAM's data reads take), built with clang-cl by src/recompile/x360/runtime/test,
//    gives the xex engine's basefile of a real XEX at the base its header states, with every resource inside it and the resources the engine lists;
//  - Xenia's PowerPC instruction tests through XenonRecomp (assembled with the PowerPC binutils, recompiled, built, run): no mismatch.
//
// Names here are file-local to this unity unit: every one starts with `x3r_`.
#include <algorithm>
#include <format>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "formats/command_test_kit.hpp"
#include "formats/xex.hpp"
#include "recompile/xenon.hpp"
#include "formats/xex/src/xex_file.h"   // formats/xex/src: the xex engine's parser

namespace stdfs = std::filesystem;
namespace xenon = cb::recompile::xenon;

namespace {

bool x3r_slow() {
    const auto env = cb::fs::get_env("CB_SLOW_TESTS");
    return env && !env->empty();
}

std::string x3r_text(const cb::Bytes& bytes) { return std::string(bytes.begin(), bytes.end()); }

xenon::Config x3r_config(const cb::config::Config& settings, const stdfs::path& state) {
    xenon::Config c;
    c.checkout_root = stdfs::path(CB_SOURCE_DIR).parent_path();
    c.state_dir = state;
    c.visual_studio = settings.path("visual_studio");
    c.ppc_binutils = settings.path("ppc_binutils");
    return c;
}

// How the runtime's ReadImage of `path` differs from the xex engine's basefile of it; the problems, none when it agrees.
std::vector<std::string> x3r_read_image_problems(const stdfs::path& program, const stdfs::path& path, const stdfs::path& work,
                                                 const cb::formats::xex::Tools& tools) {
    std::vector<std::string> problems;
    const stdfs::path out = work / "image.bin";
    cb::process::ProcessSpec read;
    read.argv = {cb::fs::to_text(program), cb::fs::to_text(path), cb::fs::to_text(out)};
    const cb::process::ProcessResult r = cb::process::run(read);
    if (!r.ok()) return {"ReadImage failed: " + x3r_text(r.err)};
    const std::string printed = x3r_text(r.out);   // (the pieces below are views into it)
    const std::vector<std::string_view> lines = cb::text::split(printed, '\n');
    // "base 82000000 size 12345", then one "resource <name> <address> <size> inside|outside" each
    const auto head = cb::text::split(cb::text::trim(lines.at(0)), ' ');
    const std::uint32_t base = *cb::text::parse_int<std::uint32_t>(head.at(1), 16);
    const std::size_t size = *cb::text::parse_int<std::size_t>(head.at(3), 10);
    std::vector<std::tuple<std::string, std::uint32_t, std::uint32_t>> read_resources;
    for (std::size_t i = 1; i < lines.size(); ++i) {
        const auto parts = cb::text::split(cb::text::trim(lines[i]), ' ');
        if (parts.size() < 5 || parts[0] != "resource") continue;
        read_resources.emplace_back(std::string(parts[1]), *cb::text::parse_int<std::uint32_t>(parts[2], 16),
                                    *cb::text::parse_int<std::uint32_t>(parts[3], 16));
        if (parts[4] != "inside") problems.push_back("resource " + std::string(parts[1]) + " outside the image");
    }
    const cb::Bytes want = cb::fs::read_file(cb::formats::xex::basefile(tools, path));
    const cb::Bytes got = cb::fs::read_file(out);
    if (got != want || size != want.size())
        problems.push_back(std::format("image differs from the basefile ({} bytes, basefile {})", got.size(), want.size()));
    const cb::formats::xex::Header header = cb::formats::xex::read_header(path);
    if (header.base && *header.base != base) problems.push_back(std::format("base {:08X}, header {:08X}", base, *header.base));
    // the resources as the xex engine reads them from the header (what its `list` prints under "Resources")
    std::vector<std::tuple<std::string, std::uint32_t, std::uint32_t>> listed;
    const cb::Bytes xex_bytes = cb::fs::read_file(path);
    for (const ::xex::Resource& resource : ::xex::XexFile::parse(std::vector<std::uint8_t>(xex_bytes.begin(), xex_bytes.end())).resources())
        listed.emplace_back(resource.name, resource.address, resource.size);
    std::sort(listed.begin(), listed.end());
    std::sort(read_resources.begin(), read_resources.end());
    if (read_resources != listed) problems.push_back(std::format("resources read: {}, listed: {}", read_resources.size(), listed.size()));
    return problems;
}

}  // namespace

// test_runtime_read_image
CB_TEST("recompile.x360.the_runtimes_read_image_is_the_xex_engines_basefile", [] {
    if (!x3r_slow()) {
        std::cout << "SKIP slow: set CB_SLOW_TESTS=1\n";
        return;
    }
    const auto game = cb::testing::game_file("LoganMinecraftTest/LoganMinecraftTest.xex");
    cb::fs::ScopedTempDir dir("cb-x360-read-image");
    const cb::formats::xex::Tools tools{dir.path() / "unpacked"};
    if (!game) {
        std::cout << "SKIP no game image or no xex program here\n";
        return;
    }
    const cb::config::Config settings = cb::config::Config::load(cb::config::current().main_checkout());
    xenon::Xenon x(x3r_config(settings, dir.path() / "state"));
    std::optional<stdfs::path> clang;
    try {
        clang = x.clang_cl();
    } catch (const cb::Error& e) {
        std::cout << "SKIP " << e.what() << "\n";
        return;
    }
    const stdfs::path build = dir.path() / "build";
    const stdfs::path source = stdfs::path(CB_SOURCE_DIR) / "recompile" / "x360" / "runtime" / "test";
    for (const std::vector<std::string>& argv : {
             std::vector<std::string>{"cmake", "-S", cb::fs::to_text(source), "-B", cb::fs::to_text(build), "-G", "Ninja", "-DCMAKE_BUILD_TYPE=Release",
                                      "-DCMAKE_CXX_COMPILER=" + cb::fs::to_text(*clang)},
             std::vector<std::string>{"cmake", "--build", cb::fs::to_text(build), "-j", "4"}}) {
        cb::process::ProcessSpec spec;
        spec.argv = argv;
        spec.env = x.msvc_env();
        spec.timeout = std::chrono::minutes(30);
        const cb::process::ProcessResult r = cb::process::run(spec);
        if (!r.ok()) std::cout << x3r_text(r.out) << x3r_text(r.err) << "\n";
        CB_CHECK(r.ok());
        if (!r.ok()) return;
    }
    std::vector<stdfs::path> modules = {*game};
    if (const auto folder = cb::fs::get_env("CB_X360_CONVERTED"); folder && !folder->empty())   // XAM's own modules (the dashboard's SystemUpdate/converted)
        for (const char* name : {"xam.xex", "huduiskin.xex"}) modules.push_back(cb::fs::from_text(*folder) / name);
    for (const stdfs::path& module : modules) {
        const std::vector<std::string> problems = x3r_read_image_problems(build / "xex_image_test.exe", module, dir.path(), tools);
        for (const std::string& p : problems) std::cout << cb::fs::to_text(module) << ": " << p << "\n";
        CB_CHECK(problems.empty());
    }
});

// test_instruction_tests
CB_TEST("recompile.x360.xenias_instruction_tests_through_xenonrecomp", [] {
    if (!x3r_slow()) {
        std::cout << "SKIP slow: set CB_SLOW_TESTS=1\n";
        return;
    }
    cb::fs::ScopedTempDir dir("cb-x360-instruction-tests");
    const cb::config::Config settings = cb::config::Config::load(cb::config::current().main_checkout());
    if (!settings.path("ppc_binutils")) {
        std::cout << "SKIP no PowerPC binutils (setting ppc_binutils)\n";
        return;
    }
    xenon::Xenon x(x3r_config(settings, dir.path() / "state"));
    xenon::InstructionTestResult result;
    try {
        result = x.instruction_tests();
    } catch (const cb::Error& e) {
        std::cout << "SKIP " << e.what() << "\n";
        return;
    }
    for (const std::string& m : result.mismatches) std::cout << "  mismatch: " << m << "\n";
    CB_CHECK(result.tested >= 150);
    CB_CHECK(result.mismatches.empty());
});
