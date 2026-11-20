// engine/show.hpp -- `codebreaker facts`: what a binary's database holds, and why.
//
//     codebreaker facts <binary>                  every kind: how many facts, their size, who made them
//     codebreaker facts <binary> <kind>           each fact of the kind: its key, hash, size, when made, whether its step's version is
//                                                 the version of now
//     codebreaker facts <binary> <kind> <key>     one fact: its value (in short) and what it was made from
//     codebreaker facts <binary> <kind> [key] --make      made current first (the engine runs)
//     codebreaker facts --databases               every database, its size, and the cap (db_max_gb)
//
// <binary> is a path or a corpus name. Nothing is made unless --make is given. The command answers itself, the engine being the program's own.
//
// What it needs of the rest of the program is the engine (with its steps declared) and how a corpus name finds its binary.
#pragma once

#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>

#include "core/cli.hpp"
#include "engine/engine.hpp"

namespace cb::engine::show {

struct Environment {
    // The engine, with its steps; asked for when the command runs.
    std::function<Engine&()> engine;
    // A corpus name or a path -> the binary (corpus.retail); nullopt when there is none.
    std::function<std::optional<std::filesystem::path>(std::string_view)> retail;
};

// A fact's value in a few lines (what the command shows of it): bytes as a count, text as its lines (the first twelve, each cut at `width`
// columns), a document as its first entries, anything else as its text cut at `width`.
std::string short_value(const Value& value, std::size_t width = 100);

// "12.3 MB" from a megabyte, else "4.5 KB".
std::string size_text(std::uint64_t bytes);

// "2026-11-12 03:04:05 PM" in local time; "-" for no time (0).
std::string when_text(double seconds_since_1970);

cli::Command command(const Environment& environment);

}  // namespace cb::engine::show
