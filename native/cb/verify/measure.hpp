// cb/verify/measure.hpp -- reconstruct and judge one module and print the tally. Nothing is written to the source
// tree.
//
//     codebreaker measure <module> [--no-engine]
//
// The decompile, build and judge are steps of the engine by default: only what changed since the last measure is
// made again. --no-engine runs the old direct path (--engine, the default, is kept for scripts). This is the number
// to watch while changing the decompiler or the verifier.
//
// What it prints: why the functions the judge called unverified were (by the reason it gave, commonest first), and
// the commonest compiler messages of the functions that did not compile. The reconstruct-and-judge itself is the
// lanes' business, so it is a service the command is given (`Services`), as the function store and the corpus are.
#pragma once

#include <cstddef>
#include <filesystem>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/cli.hpp"
#include "cb/verify/verdicts.hpp"

namespace cb::verify::measure {

// One function's row of the store, as measure reads it.
struct Row {
    std::optional<Verdict> verdict;
    std::string reason; // why the judge said so, or the compiler's message for NoCompile; empty if none
};

struct Cause {
    std::string reason;
    std::size_t count = 0;
};

struct Summary {
    std::vector<Cause> unverified; // reasons of the Divergent rows, commonest first (ties: first seen first)
    std::vector<Cause> no_compile; // the first 60 bytes of the NoCompile rows' reasons, the four commonest
};

// How many characters of a compiler message tell two failures apart.
inline constexpr std::size_t kCauseKeyLength = 60;
inline constexpr std::size_t kCompileCausesShown = 4;

Summary summarise(std::span<const Row> rows);

// The lines measure prints: "unverified by cause: a 3, b 1" and "does not compile: x (2); y (1)", each only if there
// is something to say.
std::string render(const Summary& summary);

// Which way the pipeline runs: the engine (incremental), or the old direct path.
enum class Engine { Default, On, Off };

struct Services {
    // A corpus name or a path (a folder: a Java program) as the module to measure; nullopt if there is none.
    std::function<std::optional<std::filesystem::path>(std::string_view)> find_module;
    // Reconstruct and judge the module (made again only where it changed) and give its function rows.
    std::function<std::vector<Row>(const std::filesystem::path&, Engine)> judge;
};

// Measure a module and print the tally. NotFoundError if the module is not found. Returns the exit code.
int run(const Services& services, std::string_view module, Engine engine);

// The declared command (`measure <module> [--no-engine]`).
cli::Command command(Services services);

} // namespace cb::verify::measure
