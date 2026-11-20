// coverage/reach.hpp -- throw anything at CodeBreaker and see how far each program gets.
//
//     codebreaker reach <file or folder> ... [--per-platform N] [--judge] [--timeout S] [--containers N]
//                                            [--name RUN] [--again] [--max-size KB]
//     codebreaker reach --report [RUN]       the last results again, without running anything
//
// Every file given -- and every file in every folder given, and the programs inside every disc image, Xbox 360 package or
// Games-on-Demand folder found -- is taken through these steps, as far as it goes:
//
//     found        what the file is (formats/detect), or "unknown"
//     platform     the platform it belongs to, or the platform CodeBreaker has no lane for yet, named ("Game Boy Advance
//                  (ARM7TDMI)": a gap, counted)
//     judged       with --judge: the platform's lane reconstructs it and runs its functions beside the original's
//                  (`measure`, a process of its own, stopped after --timeout seconds); the function store's tally of
//                  what came of it
//
// Load stays low on purpose: one program at a time, at most --per-platform programs of each platform (default 5: spread
// evenly across their sizes, from the smallest to the largest under --max-size KB, default 4096), at most --containers
// programs taken out of each container (default 20). Results are kept in state/reach/<run>.json by each file's content
// hash and the tools' revision, so a program already measured with the same tools is not measured again (--again
// measures it anyway). The report is written beside them, <run>.md.
//
// What reach needs of the rest of the program (the platform registry, the function store's tally, the way a container is
// opened, the command that measures) comes in through `Services`, so the module owns no global and a test can stand in
// for each part.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "core/cli.hpp"
#include "formats/media.hpp"

namespace cb::platforms {
class Registry;
}

namespace cb::coverage::reach {

// What the platform registry says of a file.
struct PlatformInfo {
    std::string title, name, family, container;
    bool has_lane = false;   // a verify lane reconstructs and judges its programs
};

// The function store's class -> number of functions, for one program.
using Tally = std::map<std::string, std::int64_t>;

struct Services {
    // The platform a file belongs to, nullopt when no platform recognises it. Required by describe().
    std::function<std::optional<PlatformInfo>(const std::filesystem::path&)> identify_platform;
    // The volumes of a container (disc, package, Games-on-Demand folder); empty when the path is not one. May throw a
    // cb::Error for one that is damaged. Default: formats::media::volumes (a file it does not know is empty).
    std::function<std::vector<std::unique_ptr<formats::media::Volume>>(const std::filesystem::path&)> open_volumes;
    // The argv that measures one program (default: this executable, `measure PATH`).
    std::function<std::vector<std::string>(const std::filesystem::path&)> measure_command;
    // The function store's tally for a program; throws when there is none. Default: no store, so an error.
    std::function<Tally(const std::filesystem::path&)> tally;
    // The tools' revision: results measured by other tools are measured again. Default: git's short HEAD of
    // `checkout_root`, "?" when git does not say.
    std::function<std::string()> tools_revision;
    std::function<double()> clock;                     // seconds since 1970 (default: the system clock)
    std::function<void(std::string_view)> log;         // a line of progress (default: standard output)
    std::filesystem::path state_dir;                   // results and unpacked programs go under <state_dir>/reach
    std::filesystem::path checkout_root;               // where git is asked the revision
    // The file extensions of the platform lanes' programs (".gba": lower case with the dot), which reach takes for programs when it looks
    // inside a container, beside the executable ones it knows itself. The platform registry's lanes give them (engine/lane_registry.hpp).
    std::vector<std::string> program_extensions;
};

// The platform of a program from a registry: the Services::identify_platform for it. The registry must outlive it.
std::function<std::optional<PlatformInfo>(const std::filesystem::path&)> identify_with(const platforms::Registry& registry);

// ---- what a file is
struct Described {
    std::string found, platform;
    std::optional<std::string> lane;   // the platform's name when it has a lane
    std::string family;                // "gap" for a platform CodeBreaker has no lane for
};

// (title, cpu) of a program of a platform CodeBreaker has no lane for, told by its first 0x200 bytes and its extension.
struct Gap {
    std::string title, cpu;
};
std::optional<Gap> gap_of(const std::filesystem::path& path, ByteView head);

Described describe(const std::filesystem::path& path, const Services& services);

// ---- the rows
struct Judged {
    Tally tally;
    double seconds = 0.0;   // to a tenth
    std::string stopped;    // why it did not finish, empty when it did
};

struct Row {
    std::string found, platform;
    std::optional<std::string> lane;
    std::string family, path, origin;
    std::uint64_t size = 0;
    std::string sha;                 // the content hash of a program; "path:<path>" for anything else
    bool picked = false;
    std::optional<Judged> judged;
    std::optional<std::string> tools;
};

// A file found, and what it came out of ("" for a file given directly; the container or folder otherwise).
struct Found {
    std::filesystem::path path;
    std::string origin;
};

// The programs inside a container, written under <state_dir>/reach/unpacked, smallest first, at most `limit`.
std::vector<std::filesystem::path> unpack(const std::filesystem::path& container, std::size_t limit, const Services& services);

// Every file under `inputs`, containers opened.
std::vector<Found> programs(const std::vector<std::filesystem::path>& inputs, std::size_t containers, const Services& services);

// Run the platform's lane on one program in its own process held in a job; stopped after `timeout_seconds`.
Judged judge(const std::filesystem::path& program, int timeout_seconds, const Services& services);

// `n` of `rows` evenly across their sizes (the smallest, the largest under `max_size`, and between): a sample of a
// platform, not its easiest programs. Smallest first.
std::vector<Row*> spread(std::vector<Row*> rows, std::size_t n, std::uint64_t max_size);

struct Options {
    std::size_t per_platform = 5;
    bool judge = false;
    int timeout_seconds = 300;
    std::size_t containers = 20;
    std::string name = "reach";   // the run: <state_dir>/reach/<name>.json and .md
    bool again = false;           // measure what was measured with these tools before
    std::uint64_t max_size = std::uint64_t{4} << 20;
};

struct Result {
    std::vector<Row> rows;   // this run's
    std::string report;
};
Result run(const std::vector<std::filesystem::path>& inputs, const Options& options, const Services& services);

// Markdown: by platform, how many programs, how far they got.
std::string report(const std::vector<Row>& rows);

// The rows of a results file (<state_dir>/reach/<run>.json); empty when there is none or it cannot be read.
std::vector<Row> read_results(const std::filesystem::path& file);

// The `reach` command, declared; the program's command table registers it.
cli::Command command(const Services& services);

}  // namespace cb::coverage::reach
