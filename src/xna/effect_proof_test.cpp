// cb-unity: alone
// The proof that a recovered effect behaves as the source it was compiled from.
//
// The effects in tests/data/xbox_effects are our own HLSL, each with the Xbox 360 SDK's fxc output beside it (<name>.fxo). The .fxo is
// recovered as HLSL (decompile/xbox_effect); the original .fx and the recovered one are both compiled by MonoGame's mgfxc and drawn by
// config/xna/shadercheck with the same vertices, parameters and texture; every technique draws something and every pixel agrees within one
// step. A machine without mgfxc or dotnet skips the test (neither is ours to install: NOT_IN_REPO.md); a shadercheck that is built is
// kept in state/xna/shadercheck and rebuilt only when its sources are newer.
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/process.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "decompile/xbox_effect.hpp"

namespace {

namespace stdfs = std::filesystem;
using namespace cb;

// Is this program on the PATH? (a program that cannot be started is a ToolError; one that runs, whatever it says, is there)
bool on_path(const char* program) {
    try {
        process::run({.argv = {program, "--help"}, .timeout = std::chrono::milliseconds(60000)});
        return true;
    } catch (const ToolError&) {
        return false;
    }
}

std::map<std::string, std::string> roll_forward() { return {{"DOTNET_ROLL_FORWARD", "LatestMajor"}, {"DOTNET_CLI_TELEMETRY_OPTOUT", "1"}}; }

stdfs::path newest_source(const stdfs::path& dir) {
    stdfs::path newest;
    stdfs::file_time_type when = stdfs::file_time_type::min();
    for (const char* name : {"Program.cs", "shadercheck.csproj"}) {
        const stdfs::path p = dir / name;
        if (cb::fs::last_write_time(p) > when) {
            when = cb::fs::last_write_time(p);
            newest = p;
        }
    }
    return newest;
}

}  // namespace

CB_TEST("xna.proof.recovered_hlsl_behaves_as_the_source", [] {
    if (!on_path("mgfxc") || !on_path("dotnet")) return;   // MonoGame's effect compiler and the .NET SDK are the machine's own

    const stdfs::path data = testing::data_path("../tests/data/xbox_effects");
    const stdfs::path source = testing::data_path("../config/xna/shadercheck");
    const stdfs::path built = testing::data_path("../state/xna/shadercheck");
    const stdfs::path exe = built / "shadercheck.exe";
    if (!stdfs::exists(exe) || cb::fs::last_write_time(exe) < cb::fs::last_write_time(newest_source(source))) {
        stdfs::create_directories(built);
        const process::ProcessResult r = process::run({.argv = {"dotnet", "build", (source / "shadercheck.csproj").string(), "-c", "Release", "-o",
                                                                built.string(), "--nologo", "-v", "q"},
                                                       .env = roll_forward()});
        CB_CHECK(r.exit_code == 0 && stdfs::exists(exe));
        if (!stdfs::exists(exe)) return;
    }

    const stdfs::path work = stdfs::temp_directory_path() / "cb_effect_proof";
    stdfs::remove_all(work);
    stdfs::create_directories(work);
    std::vector<std::string> names;
    for (const auto& entry : stdfs::directory_iterator(data))
        if (entry.path().extension() == ".fx") names.push_back(entry.path().stem().string());
    std::sort(names.begin(), names.end());
    CB_CHECK_EQ(names.size(), 8u);

    const auto compile = [&](const stdfs::path& src, const stdfs::path& out) {
        const process::ProcessResult r = process::run({.argv = {"mgfxc", src.string(), out.string(), "/Profile:OpenGL"}, .env = roll_forward()});
        CB_CHECK(r.exit_code == 0);
        return r.exit_code == 0;
    };
    for (const std::string& name : names) {
        // MonoGame takes no row_major parameter: the original with the word taken out is the same function for a matrix set by its elements
        const std::string original = text::replace_all(fs::read_text(data / (name + ".fx")), "row_major ", "");
        fs::write_text(work / (name + "_pc.fx"), original);
        fs::write_text(work / (name + "_recovered.fx"), decompile::xbox_effect::to_fx(fs::read_file(data / (name + ".fxo")), name));
        const stdfs::path a = work / (name + "_a.mgfxo"), b = work / (name + "_b.mgfxo"), result = work / (name + ".json");
        if (!compile(work / (name + "_pc.fx"), a) || !compile(work / (name + "_recovered.fx"), b)) continue;
        process::run({.argv = {exe.string(), a.string(), b.string(), result.string()}, .env = roll_forward(), .timeout = std::chrono::milliseconds(180000)});
        CB_CHECK(stdfs::exists(result));
        if (!stdfs::exists(result)) continue;
        const Json compared = json::parse(fs::read_text(result));
        CB_CHECK(!compared.contains("error"));
        if (compared.contains("error")) continue;
        for (const auto& [technique, r] : compared.items()) {
            // a technique that drew nothing would prove nothing
            CB_CHECK(r.at("drawn").get<double>() > 0);
            CB_CHECK(r.at("worst").get<double>() <= 1);
        }
    }
    stdfs::remove_all(work);
});
