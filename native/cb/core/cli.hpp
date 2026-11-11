// cb/core/cli.hpp -- the declarative command-line parser, with our own help text.
//
// A command is declared, not parsed by hand:
//
//     cli::Command{"decompile", "decompile one function or a whole module",
//         {cli::Positional{"module", "corpus name or path"},
//          cli::Option{"--function", "-f", "name or hex address", cli::Arg::Value, "NAME"},
//          cli::Option{"--judge", "", "run the equivalence judge", cli::Arg::Flag}},
//         [](const cli::Args& a) { ...; return kExitOk; }}
//
// Grammar: `--opt value`, `--opt=value`, `-o value`, `-abc` clusters of flags (the last may take a value),
// `--` ends options (everything after it is positional), a repeated option collects a list, `-` alone is a positional (stdin), `-5` is a positional
// when no option claims it. Commands may nest (`codebreaker <command> <sub> ...`). `--help`/`-h` is built in.
// Misuse is a UsageError (message and usage on stderr, exit code 2). argparse's abbreviation matching,
// %(default)s expansion and error wording are not reproduced. See docs/CPP_DESIGN.md 3.12.
#pragma once

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/text.hpp"

namespace cb::cli {

enum class Arg { Flag, Value };

// A named argument without dashes, taken in order. `variadic` takes all the rest (must be last).
struct Positional {
    std::string name;
    std::string help;
    bool required = true;
    bool variadic = false;
};

// `--long`, an optional `-s` alias (empty for none), the help line, whether it takes a value. `value_name` is
// the placeholder in help ("NAME"); `default_value` is returned when the option is absent (and shown in help).
struct Option {
    std::string name;
    std::string alias;
    std::string help;
    Arg arg = Arg::Flag;
    std::string value_name;
    std::string default_value;
};

using Param = std::variant<Positional, Option>;

// The parsed command line a handler reads. Names are the long option names with or without the dashes
// ("function" or "--function") and the positionals' names.
class Args {
public:
    // Flags.
    bool flag(std::string_view name) const { return count(name) > 0; }
    std::size_t count(std::string_view name) const;   // how many times the flag/option was given
    // Value options and positionals.
    std::optional<std::string> value(std::string_view name) const;   // the last occurrence, else the default
    std::string value_or(std::string_view name, std::string fallback) const;
    std::string required(std::string_view name) const;               // the value; the parser guaranteed it
    std::vector<std::string> values(std::string_view name) const;    // every occurrence in order (or variadic rest)
    // The value as an integer (base 10, or 0x-prefixed hex); UsageError if it is present but not a number.
    template <class T>
    std::optional<T> integer(std::string_view name) const {
        auto v = value(name);
        if (!v) return std::nullopt;
        if (auto n = text::parse_int<T>(*v, 0)) return n;
        throw UsageError("--" + std::string(strip(name)) + " expects an integer, got '" + *v + "'");
    }

private:
    friend class Cli;
    static std::string_view strip(std::string_view n);
    std::map<std::string, std::vector<std::string>, std::less<>> values_;
    std::map<std::string, std::size_t, std::less<>> counts_;
    std::map<std::string, std::string, std::less<>> defaults_;
};

using Handler = std::function<int(const Args&)>;

struct Command {
    std::string name;
    std::string summary;                  // one line, shown in lists
    std::vector<Param> params;
    Handler handler;                      // empty for a command that only groups subcommands
    std::vector<Command> subcommands;
    std::string area;                     // heading in `help` ("Decompile"); empty = "Commands"
    std::string description;              // longer text after the usage line in this command's help
};

class Cli {
public:
    // `program` is the name shown in usage lines ("codebreaker").
    Cli(std::string program, std::vector<Command> commands);

    // Parse and run. Returns the handler's exit code, 0 after printing help, or 2 after a usage error (message
    // and usage on stderr). Exceptions thrown by a handler propagate.
    int run(const std::vector<std::string>& args) const;
    int run(int argc, char** argv) const;   // argv[0] is skipped

    // The pieces, for tests and for tools that render help themselves.
    struct Parsed {
        const Command* command = nullptr;
        std::string path;   // "xbox-disc list"
        Args args;
        bool help = false;
    };
    Parsed parse(const std::vector<std::string>& args) const;   // throws UsageError
    std::string overview() const;                                // the grouped command list
    std::string help(const Command& command, std::string_view path) const;
    const std::vector<Command>& commands() const { return commands_; }
    // Find a command by its path words ("xbox-disc", "list").
    const Command* find(const std::vector<std::string>& path) const;

private:
    std::string program_;
    std::vector<Command> commands_;
};

}  // namespace cb::cli
