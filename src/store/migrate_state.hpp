// store/migrate_state.hpp -- `codebreaker migrate-state`: what the Python-driven program kept, carried to where this program reads it.
//
//     codebreaker migrate-state [--dry] [--state DIR]
//
// codebreaker.exe is being switched command by command from the Python (translated) to this program, and the two keep some of their
// state in forms of their own. A state folder the Python filled is read by this program as if it were empty in these ways; this is the
// one-time move that makes it see what the Python saw. It leaves every old file in place, never deletes or overwrites anything the
// native side already holds, and can be run again (a second run finds nothing to do). `--dry` reports what it would do and writes nothing.
//
//   (a) Judged results. A verdict is filed under the digest of the verifier settings it was reached under (the lane's profile). The Python
//       makes that digest from a repr or JSON of its own settings, this program from its own text, so the same settings have two digests
//       and a native reader finds no verdicts under its own. For each profile the store lists, the settings it stood for are read back
//       (the store keeps them beside the digest) and compared with the settings in force now for that kind of module; where they are the
//       same -- the same kind, the same judge and comparer revision, the same inputs and tools where both record them -- the verdicts
//       (and the build signatures) filed under the old digest are filed under the new one too, with their own times. A profile whose
//       settings are not the settings in force (an older judge) is left alone: its verdicts mean something the judge of now does not.
//   (b) The knowledge base. The Python keeps it in state/db/knowledge.sqlite, this program in state/db/knowledge.v2.sqlite. What a person
//       or a past run made and the headers cannot make again moves over: the bodies of each module and the names given to bodies (the
//       name store) and the rows of learned kinds whose keys mean the same on both sides. Authored rows are made again from their files and
//       derived rows from the headers, so they are not moved.
//   (c) Everything else was checked and needs nothing: see docs/history/STATE_MIGRATION.md.
//
// What the verifier settings in force are is not this module's to know (the lanes are the verifier's): the program says (Services).
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "core/cli.hpp"

namespace cb::store::migrate {

// The verifier settings in force now for one kind of module: the digest results are filed under and the text it was made from (the lane's
// profile).
struct Verifier {
    std::string digest;
    std::string settings;
};

struct Services {
    std::filesystem::path state_dir;
    // {kind of module: its verifier settings in force now}; a kind whose tools are not set up here is left out (verify's
    // Reconstructor::profiles()). Without it no profile is carried.
    std::function<std::map<std::string, Verifier>()> verifier_profiles;
};

struct Options {
    bool dry = false;   // find out and say, write nothing
};

// One profile of the store (a digest the Python made) and what became of it.
struct ProfileMove {
    std::string kind;                    // what kind of module the settings were for
    std::string from;                    // the digest in the store
    std::string to;                      // the digest of the same settings in force now; empty: none
    std::string why_not;                 // when `to` is empty: how the settings differ
    std::int64_t modules = 0;            // modules that hold results under `from`
    std::int64_t found = 0;              // verdicts under `from` (counted where it is carried)
    std::int64_t carried = 0;            // of them: filed under `to` now (dry: that would be)
    std::int64_t present = 0;            // of them: already filed under `to`
    std::int64_t built = 0;              // build signatures carried
};

struct VerdictReport {
    bool store_found = false;            // the function store of the state folder exists
    std::vector<ProfileMove> profiles;   // every Python profile that holds results, newest first
    std::int64_t modules_changed = 0;
};

// A group of rows the migration did not move, and why.
struct Left {
    std::string what;
    std::int64_t rows = 0;
    std::string why;
};

struct KnowledgeReport {
    bool found = false;                  // the Python's knowledge base exists
    bool same_file = false;              // ... but is the file this program reads (CB_KNOWLEDGE_DB names one for both)
    std::int64_t modules = 0, bodies = 0;               // carried: modules whose bodies came over, and the bodies
    std::int64_t modules_present = 0;                   // modules this program already indexed itself: left as it has them
    std::int64_t names = 0, names_present = 0;          // names given to bodies: carried, already given
    std::int64_t learned = 0, learned_present = 0;      // learned rows: carried, already held
    std::vector<Left> left;
};

struct Report {
    bool dry = false;
    VerdictReport verdicts;
    KnowledgeReport knowledge;
};

// Carry the state; `options.dry` writes nothing.
Report migrate(const Services& services, const Options& options);

// The report as the lines the command prints.
std::vector<std::string> render(const Report& report);

// `migrate-state [--dry] [--state DIR]`.
cli::Command command(const Services& services);

}  // namespace cb::store::migrate
