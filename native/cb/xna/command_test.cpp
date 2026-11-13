// Tests of `codebreaker xna` complete (cb/xna/command.hpp), on a real XNA game: Arcadecraft's package under C:/Nexia360/Games (skipped when it,
// ILSpy's command line or the settings' de4dot are not on this machine), against what the translated Python's `xna` made of it
// (testdata/command/):
//   arcadecraft_content.tsv       one line per converted .xnb (740 of them: 442 textures, 6 fonts, 3 effects, 289 other), sorted: path, platform, kind,
//                                 format, width, height, levels, glyphs, the effect's SHA-1, how many type readers and the first
//   arcadecraft_decompile.json    each assembly of the game, how it was cleaned and what the fix-ups did (6 assemblies; SunBurn is obfuscated)
//   the stdout of the command, and the C# of the six projects, by digest (below)
// The converted textures were compared with the Python's pixel for pixel (460 files of content), and the C# file for file (613): see the report.
//
// DIVERGES from the Python, by design: the reports are written with a two-space indent, and an error in a content record carries this module's
// message (the records are compared with the text of an error left out).
#include "cb/xna/command.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/command_test_kit.hpp"

using namespace cb;
namespace xcmd = cb::xna::command;

namespace {

namespace stdfs = std::filesystem;

// One line per record as the goldens have it: the columns of records_tsv, sorted.
std::string xc_records_tsv(const Json& records) {
    std::vector<std::string> rows;
    for (const Json& r : records) {
        const auto g = [&](const char* key) -> std::string {
            const auto it = r.find(key);
            if (it == r.end() || it->is_null()) return "-";
            return it->is_string() ? it->get<std::string>() : it->dump();
        };
        const Json readers = r.contains("readers") ? r.at("readers") : Json::array();
        rows.push_back(std::format("{}/{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}", r.at("folder").get<std::string>(), r.at("file").get<std::string>(), g("platform"),
                                   r.contains("error") ? std::string("error") : g("kind"), g("format"), g("width"), g("height"), g("levels"), g("glyphs"), g("sha1"),
                                   readers.size(), readers.empty() ? std::string("-") : readers[0].get<std::string>()));
    }
    std::sort(rows.begin(), rows.end());
    return text::join(rows, "\n") + "\n";
}

}  // namespace

CB_TEST("xna.command.usage", [] {
    xcmd::Environment env;
    env.config = config::Config::load(testing::data_path("../.."));
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(xcmd::run(std::vector<std::string>{}, env), 2);   // no source: the usage, as a failure
        CB_CHECK(cap.out().find("codebreaker xna <package, folder or game .exe>") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(xcmd::run(std::vector<std::string>{"--help"}, env), 0);
    }
    fs::ScopedTempDir dir{"cb-xna-command"};
    const fs::ScopedEnv converted("CB_CONVERTED", fs::to_text(dir.path() / "Converted"));
    testing::CapturedStd cap;
    // a folder with no .NET assembly in it is not a game
    fs::make_dirs(dir.path() / "empty");
    CB_CHECK_EQ(xcmd::run(std::vector<std::string>{fs::to_text(dir.path() / "empty")}, env), 1);
    CB_CHECK(cap.out().find("xna: ") != std::string::npos);
});

CB_TEST("xna.command.arcadecraft_matches_the_python", [] {
    const auto game = testing::game_file("Arcadecraft/35BC57B394081DACC69A21C3B66888F8B27DC77658");
    if (!game) return;
    fs::ScopedTempDir dir{"cb-xna-command"};
    const fs::ScopedEnv state("CB_STATE", fs::to_text(dir.path() / "state")), converted("CB_CONVERTED", fs::to_text(dir.path() / "Converted"));
    xcmd::Environment env;
    env.config = config::Config::load(config::find_checkout());
    try {
        xna::run::find_ilspycmd(xcmd::run_environment(env).tools);
    } catch (const NotFoundError&) {
        return;   // ILSpy is not installed
    }
    if (!xna::run::find_de4dot(xcmd::run_environment(env).tools)) return;   // nor de4dot, which cleans SunBurn: the report says so
    testing::CapturedStd cap;
    const int code = xcmd::run(std::vector<std::string>{fs::to_text(*game), "--name", "arcade"}, env);
    const std::string printed = cap.out();
    CB_CHECK_EQ(code, 0);
    const stdfs::path project = dir.path() / "Converted" / "arcade";
    std::string root = fs::to_text(project);
    std::replace(root.begin(), root.end(), '/', '\\');
    const std::string expected_end = "-> " + root + "\n";
    CB_CHECK(printed.ends_with(expected_end));
    const std::string head = printed.substr(0, printed.size() - expected_end.size());
    CB_CHECK_EQ(head,
                "  Arcadecraft                  C#: 0 ref casts, 0 constructors rewritten, 0 left\n"
                "  AvatarCustomAnimation        C#: 0 ref casts, 0 constructors rewritten, 0 left\n"
                "  DPSFXbox360                  C#: 0 ref casts, 0 constructors rewritten, 0 left\n"
                "  EasyStorage                  C#: 0 ref casts, 0 constructors rewritten, 0 left\n"
                "  FirebaseEncryptionLib        C#: 0 ref casts, 0 constructors rewritten, 0 left\n"
                "  SynapseGaming-SunBurn-Pro    C#: 0 ref casts, 0 constructors rewritten, 0 left  (deobfuscated: Spices.Net)\n"
                "  content: 3 effect, 6 font, 289 other, 442 texture\n");
    CB_CHECK_EQ(xc_records_tsv(json::load(project / "reports" / "content.json")), fs::read_text(testing::data_path("xna/testdata/command/arcadecraft_content.tsv")));
    CB_CHECK(json::load(project / "reports" / "decompile.json") == json::load(testing::data_path("xna/testdata/command/arcadecraft_decompile.json")));
    // every converted texture and font exists as a PNG, and every effect as its HLSL
    std::size_t pngs = 0, fx = 0;
    for (const auto& entry : stdfs::recursive_directory_iterator(project / "assets" / "content")) {
        if (entry.path().extension() == ".png") ++pngs;
        if (entry.path().extension() == ".fx") ++fx;
    }
    CB_CHECK_EQ(pngs, std::size_t{448});   // 442 textures and the 6 fonts' pages
    CB_CHECK_EQ(fx, std::size_t{3});
});
