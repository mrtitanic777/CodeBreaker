// cb/app/commands.cpp -- see commands.hpp.

#include "cb/app/commands.hpp"

#include <io.h>

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::app {

namespace {

constexpr CommandDoc kCatalog[] = {
#include "commands_catalog.inc"
};

constexpr std::size_t kSummaryColumn = 36; // where a command's summary starts in `help`
constexpr std::size_t kLineWidth = 118;

// Greedy word wrap of `text` into lines of at most `width` columns (a word longer than that stands alone).
std::vector<std::string> wrap(std::string_view text, std::size_t width) {
    std::vector<std::string> lines;
    std::string line;
    for (std::string_view word : text::split_ws(text)) {
        if (!line.empty() && line.size() + 1 + word.size() > width) {
            lines.push_back(std::move(line));
            line.clear();
        }
        if (!line.empty())
            line += ' ';
        line += word;
    }
    if (!line.empty())
        lines.push_back(std::move(line));
    return lines;
}

bool is_help_word(std::string_view s) {
    return s == "-h" || s == "--help" || s == "help";
}

} // namespace

std::span<const CommandDoc> command_docs() {
    return kCatalog;
}

const CommandDoc* find_command_doc(std::string_view name) {
    for (const CommandDoc& d : kCatalog)
        if (d.name == name)
            return &d;
    return nullptr;
}

bool stdio_are_terminals() {
    return _isatty(0) != 0 && _isatty(1) != 0;
}

CorpusOption take_corpus_option(std::span<const std::string> args, std::span<const std::string> known) {
    CorpusOption out;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (!out.corpus && args[i] == "--corpus") {
            if (i + 1 >= args.size())
                throw UsageError("--corpus needs a name: " + text::join(known, ", "));
            out.corpus = args[i + 1];
            ++i;
            continue;
        }
        out.rest.push_back(args[i]);
    }
    return out;
}

CommandTable::CommandTable(Environment env) : env_(std::move(env)) {}

void CommandTable::set(std::string_view name, RawRunner runner) {
    CB_ASSERT(find_command_doc(name) != nullptr, "a runner for a command that is not in the catalog");
    entries_[std::string(name)] = Entry{std::nullopt, std::move(runner)};
}

void CommandTable::set(cli::Command command) {
    CB_ASSERT(find_command_doc(command.name) != nullptr, "a command that is not in the catalog");
    const std::string name = command.name;
    entries_[name] = Entry{std::move(command), nullptr};
}

void CommandTable::set_fallback(RawRunner runner) {
    fallback_ = std::move(runner);
}

bool CommandTable::has_runner(std::string_view name) const {
    auto it = entries_.find(std::string(name));
    return it != entries_.end() || (fallback_ && find_command_doc(name) != nullptr);
}

std::string CommandTable::usage() const {
    std::string out = "usage: codebreaker <command> [arguments]\n";
    out += "       codebreaker               with no command, in a terminal: the terminal interface (ui)\n\n";
    for (const CommandDoc& d : kCatalog) {
        std::string line = "    ";
        line += d.synopsis;
        std::vector<std::string> summary = wrap(d.summary, kLineWidth - kSummaryColumn);
        std::size_t first = 0;
        if (line.size() + 2 <= kSummaryColumn && !summary.empty()) { // the summary starts on the synopsis's line
            line.append(kSummaryColumn - line.size(), ' ');
            line += summary[0];
            first = 1;
        }
        out += line + "\n";
        for (std::size_t i = first; i < summary.size(); ++i)
            out += std::string(kSummaryColumn, ' ') + summary[i] + "\n";
    }
    out += "\n`--corpus NAME` before or after any command selects another product's files (settings.json: corpora).\n";
    return out;
}

int CommandTable::run(const std::vector<std::string>& line) const {
    std::vector<std::string> known;
    if (env_.corpus_names && std::find(line.begin(), line.end(), "--corpus") != line.end())
        known = env_.corpus_names();
    CorpusOption taken = take_corpus_option(line, known);
    if (taken.corpus && env_.use_corpus)
        env_.use_corpus(*taken.corpus);
    std::vector<std::string> args = std::move(taken.rest);

    // no command, in a terminal with the interface installed: the interface
    if (args.empty() && env_.terminal_interface_available && env_.terminal_interface_available())
        args = {"ui"};

    if (!args.empty() && is_help_word(args[0])) {
        out_text(usage());
        return kExitOk;
    }
    if (args.empty() || find_command_doc(args[0]) == nullptr) {
        if (!args.empty())
            err_text(std::format("codebreaker: unknown command '{}'\n\n", args[0]));
        err_text(usage());
        return kExitUsage;
    }

    const std::string& name = args[0];
    auto it = entries_.find(name);
    if (it != entries_.end()) {
        const Entry& e = it->second;
        if (e.declared)
            return cli::Cli("codebreaker", {*e.declared}).run(args);
        return e.raw(std::span<const std::string>(args).subspan(1));
    }
    if (fallback_)
        return fallback_(args); // the whole command line, name first: the translated `__main__` dispatches again
    throw UsageError(std::format("the command '{}' is not available in this build", name));
}

int CommandTable::main(const std::vector<std::string>& args) const {
    try {
        return run(args);
    } catch (const std::exception& e) { // the dispatcher is an isolation boundary: one command's failure ends the run
        err_text(std::format("codebreaker: {}\n", e.what()));
        return exit_code_of(e);
    }
}

} // namespace cb::app
