// dotnet/decompiler/ilspycmd/command_line.hpp -- the command line as McMaster.Extensions.CommandLineUtils reads it (the library ilspycmd
// is built on): how options and arguments are told apart, what each kind of option takes, which mistakes are what errors, and the help
// text it lays out.
//
//     std::vector<CommandOption> options = {
//         {"o", "outputdir", "-o|--outputdir <directory>", "The output directory.", OptionType::SingleValue},
//         {"p", "project", "-p|--project", "Decompile as a project.", OptionType::NoValue}};
//     ParsedCommandLine parsed = parse_command_line(args, options);       // CommandParsingError for a mistake
//     parsed.options[0].values;   parsed.arguments;
//     std::string help = generate_help_text(info, options);
//
// Not a port of McMaster's source (it is not part of ILSpy): written from what ilspycmd 11.0 does with every kind of command line,
// found by running it (the cases are in ilspycmd_test.cpp).
//
// The rules (McMaster's defaults as ilspycmd uses them):
// - "-x" is the option whose short name is x, "--name" the option whose long name is name (names are case sensitive; a short name can
//   be longer than one character: -il, -lv, -ds). "-" alone and every argument that does not start with "-" is an argument. "--" is an
//   unrecognized option; options cannot be clustered (-lc) or abbreviated.
// - A value follows the name after ":" or "=" ("-o:dir", "--outputdir=dir"), or is the next argument, whatever it looks like
//   ("-o -p" takes "-p"). A SingleOrNoValue option takes a value only after ":" or "=". A NoValue option given a value, a
//   SingleValue option given twice, and a value-taking option with nothing after it are errors; a MultipleValue option may repeat.
// - The help option and the version option end the parse where they stand (what comes after them is not looked at); a mistake before
//   them is still a mistake.
//
// Differences: an unhandled .NET exception's stack trace is not reproduced (CommandParsingError::unhandled_exception_text gives the
// first line, and the inner exception's line where there is one).
#pragma once

#include <span>
#include <string>
#include <vector>

#include "core/error.hpp"

namespace cb::dotnet::decompiler::ilspycmd {

enum class OptionType { NoValue, SingleValue, MultipleValue, SingleOrNoValue };
enum class OptionRole { Normal, Help, Version };

struct CommandOption {
    std::string short_name;   // "o" (typed -o); "" for none
    std::string long_name;    // "outputdir" (typed --outputdir); "" for none
    std::string help_label;   // as the help lists it: "-o|--outputdir <directory>"
    std::string description;  // '\n' ends a line of the help text
    OptionType type = OptionType::NoValue;
    OptionRole role = OptionRole::Normal;
    bool show_in_help = true;
};

struct ParsedOption {
    bool is_set = false;
    std::vector<std::string> values;   // each value given (SingleValue: at most one); empty for an option given without one
};

struct ParsedCommandLine {
    OptionRole stopped_by = OptionRole::Normal;   // Help or Version: that option was met, nothing after it was read
    std::vector<ParsedOption> options;            // as many as the options given, in their order
    std::vector<std::string> arguments;
};

// A mistake on the command line (McMaster's CommandParsingException and UnrecognizedCommandParsingException), or a value that does not
// convert (CommandParsingException with the FormatException it wraps).
class CommandParsingError : public Error {
public:
    CommandParsingError(std::string message, bool unrecognized = false, std::string inner_exception_line = "")
        : Error(message), unrecognized_(unrecognized), inner_line_(std::move(inner_exception_line)) {}
    // The text .NET prints for an exception nothing caught (without the stack trace): "Unhandled exception. <type>: <message>".
    std::string unhandled_exception_text() const;
    CB_ERROR_BOILERPLATE(CommandParsingError)

private:
    bool unrecognized_;
    std::string inner_line_;
};

// Reads the arguments (not the program's name). CommandParsingError for a mistake.
ParsedCommandLine parse_command_line(std::span<const std::string> args, const std::vector<CommandOption>& options);

struct HelpInfo {
    std::string version_text;           // the first lines of the help ("ilspycmd: 11.0.0.9375\r\nICSharpCode.Decompiler: 11.0.0.9375")
    std::string description;
    std::string usage;                  // "Usage: ilspycmd [options] <Assembly file name(s)>"
    std::string argument_name;          // "Assembly file name(s)"
    std::string argument_description;
    std::string extended_help_text;     // written after the options, as it is ("\r\nRemarks:..."); line ends are CR LF
};

// The text of --help: the version, the description, the usage, the arguments and options (the options that show in the help, the
// description wrapped at 80 columns), and the extended help text. Lines end with CR LF.
std::string generate_help_text(const HelpInfo& info, const std::vector<CommandOption>& options);

}  // namespace cb::dotnet::decompiler::ilspycmd
