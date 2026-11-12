// cb/exact/repair.hpp -- a function that differs from the program by what the comparison names, mended in its source and
// kept only when the compiler proves it.
//
//     codebreaker exact PROJECT repair [--write] [-v]
//
//     std::vector<repair::Job> todo = repair::jobs(project);              // each differing function the note can mend
//     int code = repair::command(project, {"--write"});                   // try them, keep what is proved
//
// The ledger's comparison says where a function parts from the program; some of what it says is a fix it can try. For each
// such function the source file is changed, compiled again, and held to the program: a change is kept only if the function
// comes out identical and no function of the file that was identical stops being so. Without --write the changes are only
// reported.
//
//   values each +N   bytes that differ alone, each by the same amount (an immediate): the source names a constant or an
//                    enumeration's member with another value. Tried, in order: the names that end in a number equal to such
//                    a value (an enumeration named by its members' ordinals: DK46) with the number moved by N -- those
//                    alone, then every name of that family; a literal of that value made the program's.
//
// A Pascal program's change is written to the project's own overlay (its first source folder), so the lower layers stay what
// they are for the other programs built from them. Source text is bytes (the Pascal and C sources of the period are Latin-1
// or CP437): it is read and written as they are.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/exact/ledger.hpp"
#include "cb/exact/pascal_join.hpp"

namespace cb::exact::repair {

// What a note names: the shift of the values, and the places in the program where they differ.
struct Shift {
    std::int64_t by = 0;
    std::vector<std::int64_t> offsets;
};
// The first note of the form "? N values each +D in the program (offsets)", or none.
std::optional<Shift> shifts(const std::vector<std::string>& notes);

// One change of the source and what it is.
struct Candidate {
    std::string what, text;
};
// The source changed so that the values `mine` become mine + d, in the order they are tried.
std::vector<Candidate> candidates(std::string_view text, std::span<const std::int64_t> mine, std::int64_t d);

// One function the ledger finds differing in a way a change may mend.
struct Job {
    std::optional<std::string> function;
    std::optional<std::int64_t> at;                    // where the program has it
    std::filesystem::path source;                      // the file that holds it
    std::filesystem::path dest;                        // where a change is written (a Pascal piece: the project's own overlay)
    std::vector<std::int64_t> mine;                    // the bytes the compiler gives at the places that differ
    std::int64_t d = 0;                                // by what the program's differ
    std::optional<std::string> unit;                   // a Pascal compiland
    std::function<std::vector<ledger::Row>()> check;   // the ledger's rows of the file (or compiland) as it is on disk now
};

// The project with only the compiland `name` (what pascal_join::run is given to check one).
pascal_join::Project only(const pascal_join::Project& project, const std::string& name);

// The file whose first line declares P<h> (a piece, or one nested in a piece's folders), the top layer's.
std::optional<std::filesystem::path> piece_of(const pascal_join::Compiland& compiland, std::string_view h);

std::vector<Job> jobs(const ledger::Project& project);

// Where the lines of the report go (each without its line end). Default: standard output.
using Sink = std::function<void(std::string_view line)>;

// Try each job's candidates, in order, against its check; one that makes the function identical without spoiling an
// identical one is kept when `write`, only reported otherwise. -> 0. Reports a line per function and the count mended.
int mend(const std::vector<Job>& todo, bool write, const Sink& say = {});

// `repair [--write] [-v]`: mend(jobs(project), --write given).
int command(const ledger::Project& project, const std::vector<std::string>& args, const Sink& say = {});

}  // namespace cb::exact::repair
