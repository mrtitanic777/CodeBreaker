// Helpers for the tests of command layers (`cb::<area>::<module>::command(environment)`): run one command and capture what it prints
// the way the golden files made from the Python have it, and list a folder of written files as "path size sha1" lines.
#pragma once

#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/cli.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

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

}  // namespace cb::testing
