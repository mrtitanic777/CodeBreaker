// cb/app/commands.hpp -- the command table of codebreaker.exe and its dispatch (docs/CPP_DESIGN.md 3.12).
//
//     codebreaker <command> [arguments]
//     codebreaker                 with no command, in a terminal: the terminal interface (`ui`)
//
// Every command is listed in the catalog, by name, with how it is written and what it does; nothing is discovered
// by scanning folders. The catalog is what `help` prints and what the MCP `commands`/`command_help` tools read.
// A command RUNS when something registers a runner for it: a `cli::Command` (declared: parsed, validated and
// documented by cb::cli) or a raw runner (hands the words after the command name to code that parses them itself --
// the transition's way for a command whose module is not yet declared). A command with a catalog row and no runner
// goes to the fallback runner if there is one (while the program still hands unported commands to the translated
// Python `__main__`), else it is reported as not available in this build.
//
// `--corpus NAME` before or after any command selects another product's files (settings.json: corpora); the
// option is taken off here and acted on through the Environment, so the command never sees it.
//
// The exit codes are the ones of docs/CPP_DESIGN.md 3.6: an exception reaching `main` maps through the Error's own
// exit code, and anything else is an internal error.
#pragma once

#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/cli.hpp"

namespace cb::app {

// One catalog row.
struct CommandDoc {
    std::string_view name;     // "decompile"
    std::string_view synopsis; // "decompile <name> [--out FILE]"
    std::string_view summary;  // "decompile any module (32-bit, 16-bit, PowerPC) to C; nothing is built"
};

// Every command, in the order `help` lists them.
std::span<const CommandDoc> command_docs();
// A command's row, or null.
const CommandDoc* find_command_doc(std::string_view name);

// Runs a command from the words after its name (any fixed arguments the registrant needs are bound into it).
using RawRunner = std::function<int(std::span<const std::string> args)>;

// What dispatch needs of the rest of the program, none of which lives here.
struct Environment {
    std::function<void(const std::string& name)> use_corpus; // `--corpus NAME`: select that product's files
    std::function<std::vector<std::string>()> corpus_names;  // listed when `--corpus` has no name
    std::function<bool()> terminal_interface_available;      // stdin and stdout are terminals and `ui` is installed
};

// The default `terminal_interface_available` check: stdin and stdout are terminals (the caller adds "and the
// interface is installed").
bool stdio_are_terminals();

// A command line with `--corpus NAME` taken off. `corpus` is the name given (the first `--corpus` only).
struct CorpusOption {
    std::vector<std::string> rest;
    std::optional<std::string> corpus;
};
// UsageError if `--corpus` is the last word (it needs a name; `known` is listed in the message).
CorpusOption take_corpus_option(std::span<const std::string> args, std::span<const std::string> known = {});

class CommandTable {
public:
    explicit CommandTable(Environment env = {});

    // Register how a command runs. The name must be in the catalog (a name that is not is a bug: InternalError).
    // A later registration of the same name replaces the earlier.
    void set(std::string_view name, RawRunner runner);
    void set(cli::Command command);
    // What a command with a catalog row and no runner is handed to: the whole command line, the command's name first
    // (the translated Python `__main__` finds the command again by name).
    void set_fallback(RawRunner runner);
    bool has_runner(std::string_view name) const;

    // The command line: `--corpus` taken off, the terminal interface for no command in a terminal, help for a
    // help word, otherwise the command run. Returns its exit code (2 for no command or an unknown one). Errors a
    // command throws propagate.
    int run(const std::vector<std::string>& args) const;
    // `run`, with errors mapped to exit codes: a cb::Error says its own, its message goes to stderr.
    int main(const std::vector<std::string>& args) const;

    // The help text: usage lines, then every command in the catalog with its synopsis and summary.
    std::string usage() const;

private:
    struct Entry {
        std::optional<cli::Command> declared;
        RawRunner raw;
    };
    Environment env_;
    std::unordered_map<std::string, Entry> entries_; // lookup only; the catalog gives the order
    RawRunner fallback_;
};

} // namespace cb::app
