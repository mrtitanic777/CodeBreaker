// cb/exact/identify.hpp -- which toolchain made the program: the one that makes the most of its sources' functions its bytes.
//
//     codebreaker exact PROJECT identify [--try NAME[:VARIANT][/SWITCHES] ..] [--sample N] [--write]
//
// A program's sources -- seeded from another version (exact/seed), drafted, or written -- are compiled by each candidate
// toolchain and held to the program as the ledger holds them. The candidates are every toolchain profile whose sources are of
// the project's kind (the files' extension), each with its default switches and the alternatives its profile lists
// ("try_switches": a compiler's switches that change the code -- C 3.00's -Gs), or those --try names. --write sets the project's
// toolchain, variant and switches to the best.
//
//     identify::Services services{.locations = locations, .load = loader};
//     identify::Score s = identify::score(project, candidate, sample, services);     // "7 of 12"
//     int code = identify::command(project, {"--sample", "20", "--write"}, services);
//
// The sample is the first N (default 12) of the project's source files after a shuffle with a fixed seed, so that the same
// project is always asked the same question; every candidate is scored on the same sample.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/exact/ledger.hpp"
#include "cb/exact/toolchain.hpp"

namespace cb::exact::identify {

// A toolchain, a variant of it, and switches.
struct Candidate {
    std::string toolchain;
    std::optional<std::string> variant, switches;
    friend bool operator==(const Candidate&, const Candidate&) = default;
};

using Loader = std::function<std::shared_ptr<const ledger::Toolchain>(const std::string& name, const std::optional<std::string>& variant)>;
// Held to the program, one source file: the ledger's rows (check_file).
using Checker = std::function<std::vector<ledger::Row>(const ledger::Project&, const ledger::Toolchain&, const ledger::SourceFile&,
                                                       const SymbolTable& symbols, SymbolTable& learnt)>;

struct Services {
    toolchain::Locations locations;   // where the profiles are
    // The one thing identify does to a project: assigning its switches (the compile switches of every source of it). The
    // ledger's Project reads them (switches()); this writes them.
    std::function<void(const std::optional<std::string>&)> set_switches;
    std::function<void(std::string_view line)> say;   // where the lines of the report go, each without its line end (default: standard output)
    Loader load;                      // a toolchain by name (toolchain::Toolchains::load)
    Checker check;                    // default: ledger::check_file
};

// The candidates: those `tries` name, else every profile whose source extension is one of the project's sources'.
std::vector<Candidate> candidates(const ledger::Project& project, const toolchain::Locations& locations, std::span<const std::string> tries = {});

// How many of the files' functions come out the program's under a candidate; -1 and the reason when its toolchain cannot be
// loaded. The project's switches are the candidate's for the length of the scoring, and are put back.
struct Score {
    std::int64_t same = 0;
    std::string text;   // "N of M", or the error
};
Score score(const ledger::Project& project, const Candidate& candidate, std::span<const ledger::SourceFile> files, const Services& services);

// The sample of `n` source files: by address then path, shuffled with a fixed seed, the first n.
std::vector<ledger::SourceFile> sample(std::vector<ledger::SourceFile> files, std::size_t n);

// `identify [--try ..] [--sample N] [--write]`. -> 0; 1 when no toolchain makes any of the sample's functions the program's.
int command(const ledger::Project& project, const std::vector<std::string>& args, const Services& services);

}  // namespace cb::exact::identify
