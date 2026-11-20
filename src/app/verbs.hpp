// app/verbs.hpp -- the condensed command line: nine verbs that pick their steps from the input.
//
//     codebreaker convert FILE      any program to source (routes itself)       inspect FILE   what it is and holds
//     codebreaker extract FILE      files out of any container                  build FOLDER   make it again from its source
//     codebreaker check FILE        prove it behaves                            compare A B    two builds, function by function
//     codebreaker corpus            the whole collection                        lookup NAME    a name, service or signature
//     codebreaker xbox              Xbox 360 specialist tools
//
// A verb is a router: it works out from the input (the file's kind, a word, a flag) which of the older commands does the work, and runs it with
// the words that are left. Every older command keeps its name, hidden: `decompile x.dll` is `convert x.dll --decompile` and prints the same.
// The table of modes below is the one source of that: the aliases, the help of each verb ("Old names") and the routing all read it.
//
//     verbs::route("extract", {"a.cab", "out"}, verbs::file_probe())   // -> {"cab", {"extract", "a.cab", "out"}}
#pragma once

#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace cb::app {
class CommandTable;
}

namespace cb::app::verbs {

// What an input is, as far as routing needs to know.
enum class Kind {
    Missing,     // not a file or folder (a module name, a word)
    Folder,      // a folder with nothing recognisable
    DriverSource, // a folder of assembly that is a driver's source
    DosSource,   // a folder of assembly
    CSource,     // a folder of C (a 16-bit DLL's source, .def, .mak)
    Pe, Ne, Le, Dos,   // executables, by the lane of formats::detect
    Xex,         // an Xbox 360 executable
    Cab,
    Disc,        // a floppy, hard-disk or CD image, or an Xbox 360 package
    XboxDisc,
    HeavyIron,
    Gfx,
    Font,
    JavaSource, CsSource, PySource,
    Config,      // a .toml
    Other,       // a file nothing above names
};

// How an input is classified: the real one reads the file (formats::detect), a test gives its own.
using Probe = std::function<Kind(const std::string& path)>;
Probe file_probe();

// One way a verb can go: a flag (or, for `corpus` and `xbox`, a word) that picks an older command; when two rows share a flag, the kind of the input picks.
struct Mode {
    std::string_view verb;
    std::string_view flag;      // "--decompile"; "run"; "" for the verb's default
    std::string_view old_name;  // "decompile": the command that does the work, and the hidden alias
    std::optional<Kind> kind;   // chosen when the input is this kind (several rows share one flag)
};
std::span<const Mode> modes();

// The nine verbs, in the order `help` lists them, and the commands that stay as they are.
std::span<const std::string_view> verb_names();
std::span<const std::string_view> kept_names();
bool is_verb(std::string_view name);

// The verb an older command now belongs to ("" for a verb itself, a kept command, or a name that is none).
std::string_view verb_of(std::string_view old_name);
// The mode (the verb and flag) behind an older name, or null.
const Mode* alias_of(std::string_view old_name);
// Hidden: an older command that a verb has taken over.
inline bool is_alias(std::string_view name) { return !verb_of(name).empty(); }

// One older command to run and its words. A route can have two steps (`convert FILE --check` converts, then checks).
struct Step {
    std::string command;
    std::vector<std::string> args;
    bool operator==(const Step&) const = default;
};
using Route = std::vector<Step>;

// What `verb args...` runs; nullopt when there is nothing to run (no input: show the verb's help). `legacy_default`: the verb's own name also
// being an older command (`convert`, `build`, `cache`) is why its default step names it.
std::optional<Route> route(std::string_view verb, std::span<const std::string> args, const Probe& probe);

// `codebreaker VERB --help`: what it does, what it picks, its options, and the old names at the end.
std::string help(std::string_view verb);

// Make the verbs run: each of the nine (and `cache`, which takes in tidy-state and migrate-state) is set on `table`, in front of any command of
// the same name, which stays reachable as the verb's default. The older commands must be registered first.
void register_verbs(CommandTable& table, Probe probe = file_probe());

}  // namespace cb::app::verbs
