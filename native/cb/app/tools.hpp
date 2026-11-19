// cb/app/tools.hpp -- every command of the command table as a tool a person can use from the window: a plain-word title, a group, one sentence of what
// it does, and a small argument spec the Tools page builds its form from. Nothing here knows about windows (so the tests run everywhere).
//
//     const tools::Tool* t = tools::find("pe-source");   // "Windows program", group "Rebuild source", args: Program (file, required), Stub (file, --stub)
//     tools::Values v = tools::empty_values(*t);  v.text[0] = "C:/Games/PINBALL.EXE";
//     if (!tools::missing(*t, v)) run(tools::build_argv(*t, v));   // {"pe-source", "C:/Games/PINBALL.EXE"}
//
// The titles, groups, descriptions and specs are tools_catalog.inc, one line per command of commands_catalog.inc (a test checks that none is missing; the
// commands left out of the list are named there with the reason). The CLI's command names and `help` do not come from here and do not change.
//
// An argument spec is a small text, kept short on purpose:  [?][*]K Label[=a|b|c] [--flag]   separated by ';'
//     K       F file (Browse...), D folder (Browse...), T text, W free words (split on spaces), C choice (a drop-down of the =a|b|c values), B toggle (needs --flag)
//     ?       optional (otherwise the form must have it before Run is enabled)
//     *       a list: entries separated by ';' in the field, each one argument
//     --flag  an option: written before its value (a toggle: alone); without it the argument is positional, in the order of the spec
//     _       in the label is a space
// "=" instead of a spec means: derive it from the usage string of the command table (parse_usage): <x> required positional, [--flag] a toggle, [--opt V]
// an option, [x] optional, a|b a choice, `...` a list.
#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::app::tools {

enum class ArgKind { File, Folder, Text, Words, Choice, Toggle };

struct Arg {
    std::string label;                  // "Program"
    ArgKind kind = ArgKind::Text;
    std::string flag;                   // "--stub"; empty: positional
    std::vector<std::string> choices;   // Choice
    bool required = false;
    bool list = false;                  // entries separated by ';'
};

struct Tool {
    std::string name;                   // the command: "pe-source"
    std::string title;                  // "Windows program"
    std::string group;                  // "Rebuild source"
    std::string description;            // one plain sentence
    std::string usage;                  // the command table's usage line
    std::vector<Arg> args;
};

// The groups in the order the list shows them, and every tool, in the order of its group (then the table's).
const std::vector<std::string>& groups();
const std::vector<Tool>& all();
const Tool* find(std::string_view name);

// The commands of the table that are not tools, each with the reason (names only: "app", "mcp").
struct LeftOut {
    std::string name, reason;
};
const std::vector<LeftOut>& left_out();

// The argument spec text -> args; the usage line -> args. Both are lenient: what they cannot read is left out, never an exception.
std::vector<Arg> parse_spec(std::string_view spec);
std::vector<Arg> parse_usage(std::string_view usage);

// Tools whose title, command name or description holds every word of `query` (any case); all of them when it is empty. In list order.
std::vector<const Tool*> search(std::string_view query);
bool matches(const Tool& tool, std::string_view query);

// What a person has entered: text per argument (a Choice: the chosen value, a Toggle: "1" or "").
struct Values {
    std::vector<std::string> text;
};
Values empty_values(const Tool& tool);
// The label of the first required argument not filled in; nullopt when the form is complete (Run may be enabled).
std::optional<std::string> missing(const Tool& tool, const Values& values);
// The command line after the program name: the command, then each filled argument.
std::vector<std::string> build_argv(const Tool& tool, const Values& values);
// The text split the way a Words field or a list is: on spaces, or on ';'.
std::vector<std::string> split_entries(std::string_view text, bool on_semicolon);

}  // namespace cb::app::tools
