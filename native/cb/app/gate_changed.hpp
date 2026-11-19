// cb/app/gate_changed.hpp -- what a change can affect: `tools\gate.cmd --changed` runs only the command cases and the unit-test areas that depend
// on what changed since the last passing gate.
//
// THE RECORD. A passing gate writes the tree it passed (state/work/gate-last.txt): for every file the gate depends on, its size, write time and
// SHA-1 (the hash is made again only when size or time differ), plus one entry per command case (a hash of its line and of the variables it
// is read with). "Changed since" is the exact difference of that record and the tree now -- not a git diff, so a dirty tree, a reset and a
// merge all mean what they say.
//
// THE RULES (select()). Err toward running:
//   - core/, base/, the build system (CMakeLists, *.cmake, third_party, tools/), the gate's own sources, the command table and its wiring in app/,
//     test inputs (testdata, tests/exe_gate/in, tests/fixtures), a data/ folder that no area owns, an unknown path: EVERYTHING.
//   - native/cb/<area>/...: that area. Its unit tests run, and those of every area that includes it (directly or through another area): the include
//     graph of the sources, read from the `#include "cb/<area>/..."` lines. A command case runs when the area is one the command's code reaches:
//     the areas listed for its command in tests/exe_gate/areas.txt (plus `@areas=` on the case), and every area their sources include, transitively.
//     A command with no line there is always run.
//   - app/window_*, tools*, settings_edit*: the window's own files ("app:window"): the unit tests of app and of what includes it, and no command case.
//     app/convert*: "app:convert". A *_test.cpp: its area's unit tests only.
//   - data/<area>/...: that area, as its code.
//   - tests/exe_gate/golden/NAME.txt or the line of case NAME: that case.
//
// Every function here is pure over text, so the rules are tested without a tree (gate_changed_test.cpp).
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace cb::app::gate_changed {

struct FileStamp {
    std::uint64_t size = 0;
    std::int64_t mtime = 0;
    std::string sha1;
    bool operator==(const FileStamp&) const = default;
};

// path (forward slashes, relative to the repository) -> stamp. The entries "case:NAME" and "case-vars" stand for the command cases.
struct Record {
    std::map<std::string, FileStamp> files;
    std::string commit;   // informational: HEAD when it was written
};

std::string record_text(const Record& record);
Record read_record(std::string_view text);

// The files the gate depends on, stamped. `previous` saves hashing the files whose size and time did not change; `extra` are the case entries.
Record scan_tree(const std::filesystem::path& repo, const Record* previous, const std::map<std::string, std::string>& extra);

// Paths whose stamp differs (changed, added or removed).
std::vector<std::string> changed_paths(const Record& was, const Record& now);

// ---- the include graph ---------------------------------------------------------------------------------------------------

struct Graph {
    std::map<std::string, std::set<std::string>> code;   // area -> areas its non-test sources include
    std::map<std::string, std::set<std::string>> all;    // ... and its tests too
};
// Reads the include lines of every .cpp and .hpp under <repo>/native/cb.
Graph read_graph(const std::filesystem::path& repo);
// Builds one from (path, text) pairs, for tests.
Graph graph_of(const std::vector<std::pair<std::string, std::string>>& sources);

// The areas reached from `seeds` through `edges` (the seeds included).
std::set<std::string> reach_down(const std::map<std::string, std::set<std::string>>& edges, const std::set<std::string>& seeds);
// Every area from which one of `targets` is reached (the targets included).
std::set<std::string> reach_up(const std::map<std::string, std::set<std::string>>& edges, const std::set<std::string>& targets);

// ---- the command map -----------------------------------------------------------------------------------------------------

// tests/exe_gate/areas.txt: `command: area area ...` lines; `#` comments.
struct CommandAreas {
    std::map<std::string, std::vector<std::string>> by_command;
};
CommandAreas parse_areas(std::string_view text);

// ---- selection -----------------------------------------------------------------------------------------------------------

struct Selection {
    bool everything = false;
    std::string why;                     // the first path that made it everything
    std::set<std::string> areas;         // areas changed (code or data), "app:window" and "app:convert" among them
    std::set<std::string> unit_only;     // areas whose test files alone changed
    std::set<std::string> cases;         // cases named by their line or golden
};

Selection select(const std::vector<std::string>& changed);

// The areas whose unit tests run: the changed areas and everything that includes them, and the areas whose tests changed.
std::set<std::string> unit_areas(const Selection& selection, const Graph& graph);

// Whether a case runs. `command` is its first word; `extra` its @areas.
bool case_selected(const Selection& selection, const Graph& graph, const CommandAreas& map, const std::string& name, const std::string& command,
                   const std::vector<std::string>& extra);

}  // namespace cb::app::gate_changed
