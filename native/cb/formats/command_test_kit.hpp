// Helpers for the tests of command layers (`cb::<area>::<module>::command(environment)`): run one command and capture what it prints
// the way the golden files made from the Python have it, and list a folder of written files as "path size sha1" lines.
#pragma once

#include <algorithm>
#include <filesystem>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/cli.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/xex.hpp"
#include "cb/formats/xex_imports.hpp"

namespace cb::testing {

// What `command` prints to stdout for `args` (its name first, then the arguments), then `exit=N` and a newline: a golden file's text.
inline std::string run_command(const cli::Command& command, std::vector<std::string> args) {
    cli::Cli cli("codebreaker", {command});
    CapturedStd cap;
    args.insert(args.begin(), command.name);
    const int code = cli.run(args);
    return cap.out() + "exit=" + std::to_string(code) + "\n";
}

// The files under `folder`, one "relative/path size sha1hex" line each, sorted by path (as the goldens' manifests are).
inline std::string tree_manifest(const std::filesystem::path& folder) {
    std::vector<std::string> lines;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(folder)) {
        if (!entry.is_regular_file()) continue;
        const Bytes data = fs::read_file(entry.path());
        lines.push_back(std::format("{} {} {}\n", std::filesystem::relative(entry.path(), folder).generic_string(), data.size(),
                                    crypto::to_hex(crypto::sha1(data))));
    }
    std::sort(lines.begin(), lines.end());
    std::string out;
    for (const std::string& line : lines) out += line;
    return out;
}

// A real Xbox 360 title the tests may read when it is on this machine (Minecraft under C:/Nexia360/Games), else nullopt.
inline std::optional<std::filesystem::path> game_file(const char* relative) {
    const std::filesystem::path path = std::filesystem::path("C:/Nexia360/Games") / relative;
    std::error_code ec;
    if (!std::filesystem::is_regular_file(path, ec)) return std::nullopt;
    return path;
}

// The xex program, for the tests that read a real .xex: the folder in CB_XEX_BIN, else the one built in the source tree. nullopt when
// there is none (the test is then skipped). Unpacked images go under `unpacked`.
inline std::optional<formats::xex::Tools> real_xex_tools(const std::filesystem::path& unpacked) {
    std::vector<std::filesystem::path> folders;
    if (const auto env = fs::get_env("CB_XEX_BIN")) folders.push_back(fs::from_text(*env));
    folders.push_back(data_path("../xex/build/bin"));
    for (const auto& folder : folders) {
        std::error_code ec;
        if (std::filesystem::is_regular_file(folder / "xex.exe", ec)) return formats::xex::Tools{folder / "xex.exe", unpacked};
    }
    return std::nullopt;
}

// The console ordinals the knowledge base keeps (codebreaker/symbols/console_ordinals.json): {library: {ordinal: name}}, as the function
// the xex commands' environments take.
inline formats::xex_imports::OrdinalNames console_ordinal_names() {
    auto table = std::make_shared<Json>(json::load(data_path("../../data/symbols/console_ordinals.json")));
    return [table](std::string_view library, std::uint32_t ordinal) -> std::optional<std::string> {
        const auto lib = table->find(std::string(library));
        if (lib == table->end() || !lib->is_object()) return std::nullopt;
        const auto name = lib->find(std::to_string(ordinal));
        if (name == lib->end() || !name->is_string()) return std::nullopt;
        return name->get<std::string>();
    };
}

}  // namespace cb::testing
