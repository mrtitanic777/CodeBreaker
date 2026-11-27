// dotnet/decompiler/ilspycmd/command_line.cpp -- see command_line.hpp.
#include "dotnet/decompiler/ilspycmd/command_line.hpp"

#include <algorithm>
#include <optional>

namespace cb::dotnet::decompiler::ilspycmd {
namespace {

constexpr const char* kNewLine = "\r\n";
constexpr int kMaxLineLength = 80;

// The option a name (typed after "-" or "--") is, or nullptr.
const CommandOption* find_option(const std::vector<CommandOption>& options, bool long_form, const std::string& name, std::size_t* index) {
    for (std::size_t i = 0; i < options.size(); ++i) {
        const std::string& candidate = long_form ? options[i].long_name : options[i].short_name;
        if (!candidate.empty() && candidate == name) {
            *index = i;
            return &options[i];
        }
    }
    return nullptr;
}

// The length of a text in UTF-16 units (what .NET pads and measures by).
std::size_t utf16_length(const std::string& text) {
    std::size_t n = 0;
    for (std::size_t i = 0; i < text.size(); ++i) {
        const unsigned char c = static_cast<unsigned char>(text[i]);
        if ((c & 0xC0) == 0x80) continue;
        n += c >= 0xF0 ? 2 : 1;
    }
    return n;
}

// The text in lines of at most `width` characters: words (split at spaces) are kept whole, a word longer than a line gets a line of its
// own; a '\n' always ends a line.
std::vector<std::string> wrap(const std::string& text, int width) {
    std::vector<std::string> lines;
    std::size_t paragraph_start = 0;
    while (true) {
        const std::size_t paragraph_end = text.find('\n', paragraph_start);
        const std::string paragraph = text.substr(paragraph_start, paragraph_end == std::string::npos ? std::string::npos : paragraph_end - paragraph_start);
        std::string line;
        std::size_t pos = 0;
        while (pos <= paragraph.size()) {
            std::size_t space = paragraph.find(' ', pos);
            if (space == std::string::npos) space = paragraph.size();
            const std::string word = paragraph.substr(pos, space - pos);
            pos = space + 1;
            if (word.empty()) continue;
            if (line.empty())
                line = word;
            else if (static_cast<int>(utf16_length(line) + 1 + utf16_length(word)) <= width)
                line += " " + word;
            else {
                lines.push_back(line);
                line = word;
            }
        }
        lines.push_back(line);
        if (paragraph_end == std::string::npos) break;
        paragraph_start = paragraph_end + 1;
    }
    return lines;
}

std::string pad_right(const std::string& text, std::size_t width) {
    const std::size_t n = utf16_length(text);
    return n >= width ? text : text + std::string(width - n, ' ');
}

// "  <label padded><description's first line>" and the other lines under it.
void write_entry(std::string& out, const std::string& label, const std::string& description, std::size_t first_column_width) {
    const int description_width = kMaxLineLength - static_cast<int>(2 + first_column_width);
    const std::vector<std::string> lines = wrap(description, description_width);
    out += "  " + pad_right(label, first_column_width) + (lines.empty() ? "" : lines[0]) + kNewLine;
    for (std::size_t i = 1; i < lines.size(); ++i) out += std::string(2 + first_column_width, ' ') + lines[i] + kNewLine;
}

}  // namespace

std::string CommandParsingError::unhandled_exception_text() const {
    const char* type = unrecognized_ ? "McMaster.Extensions.CommandLineUtils.UnrecognizedCommandParsingException"
                                     : "McMaster.Extensions.CommandLineUtils.CommandParsingException";
    std::string text = std::string("Unhandled exception. ") + type + ": " + what() + kNewLine;
    if (!inner_line_.empty()) text += " ---> " + inner_line_ + kNewLine;
    return text;
}

ParsedCommandLine parse_command_line(std::span<const std::string> args, const std::vector<CommandOption>& options) {
    ParsedCommandLine result;
    result.options.resize(options.size());
    for (std::size_t i = 0; i < args.size(); ++i) {
        const std::string& arg = args[i];
        if (arg == "--") throw CommandParsingError("Unrecognized option '--'", true);
        if (arg.size() < 2 || arg[0] != '-') {
            result.arguments.push_back(arg);
            continue;
        }

        const bool long_form = arg[1] == '-';
        const std::string body = arg.substr(long_form ? 2 : 1);
        const std::size_t separator = body.find_first_of(":=");
        const std::string name = body.substr(0, separator);
        std::optional<std::string> inline_value;
        if (separator != std::string::npos) inline_value = body.substr(separator + 1);

        std::size_t index = 0;
        const CommandOption* option = find_option(options, long_form, name, &index);
        if (option == nullptr) throw CommandParsingError("Unrecognized option '" + arg + "'", true);

        ParsedOption& parsed = result.options[index];
        switch (option->type) {
            case OptionType::NoValue:
                if (inline_value) throw CommandParsingError("Unexpected value '" + *inline_value + "' for option '" + name + "'");
                parsed.is_set = true;
                break;
            case OptionType::SingleOrNoValue:
                parsed.is_set = true;
                if (inline_value) {
                    parsed.values.clear();
                    parsed.values.push_back(*inline_value);
                }
                break;
            case OptionType::SingleValue:
            case OptionType::MultipleValue: {
                std::string value;
                if (inline_value) {
                    value = *inline_value;
                } else if (i + 1 < args.size()) {
                    value = args[++i];
                } else {
                    throw CommandParsingError("Missing value for option '" + name + "'");
                }
                if (option->type == OptionType::SingleValue && parsed.is_set) throw CommandParsingError("Unexpected value '" + value + "' for option '" + name + "'");
                parsed.is_set = true;
                parsed.values.push_back(std::move(value));
                break;
            }
        }
        if (option->role != OptionRole::Normal) {
            result.stopped_by = option->role;
            return result;
        }
    }
    return result;
}

std::string generate_help_text(const HelpInfo& info, const std::vector<CommandOption>& options) {
    std::size_t first_column_width = info.argument_name.size();
    for (const CommandOption& option : options)
        if (option.show_in_help) first_column_width = std::max(first_column_width, utf16_length(option.help_label));
    first_column_width += 2;

    std::string out;
    out += info.version_text + kNewLine + kNewLine;
    if (!info.description.empty()) out += info.description + std::string(kNewLine) + kNewLine;
    out += info.usage + kNewLine + kNewLine;
    out += std::string("Arguments:") + kNewLine;
    write_entry(out, info.argument_name, info.argument_description, first_column_width);
    out += kNewLine;
    out += std::string("Options:") + kNewLine;
    for (const CommandOption& option : options)
        if (option.show_in_help) write_entry(out, option.help_label, option.description, first_column_width);
    out += info.extended_help_text + kNewLine;
    return out;
}

}  // namespace cb::dotnet::decompiler::ilspycmd
