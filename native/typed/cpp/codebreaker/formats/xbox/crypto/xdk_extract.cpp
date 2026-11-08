// codebreaker/formats/xbox/crypto/xdk_extract.cpp -- C++ from codebreaker.formats.xbox.crypto.xdk_extract.

#include "codebreaker/formats/xbox/crypto/xdk_extract.hpp"

#include <map>
#include <optional>
#include <string>
#include <vector>

namespace codebreaker::formats::xbox::crypto::xdk_extract {

const py::str author = py::str("Visual Studio");
const py::str description = py::str("A script to extract XDKRecoveryXenonXXXXX.exe and XDKSetupXenonXXXXX.exe files");
const py::list<py::str> platforms{py::str("Windows"), py::str("Linux"), py::str("macOS")};

const py::bytes PEMagic = py::bytes(std::string("PE\0\0", 4));
const py::bytes CABMagic = py::bytes("MSCF");

namespace {

// ctypes' Structure.from_buffer_copy refuses a buffer shorter than the structure
void check_size(const py::bytes& buffer, int64_t size) {
    if (buffer.size() < size) {
        throw py::ValueError("Buffer size too small (" + std::to_string(buffer.size()) + " instead of at least " + std::to_string(size) + " bytes)");
    }
}

// an unsigned little-endian integer of `size` bytes at `at`
int64_t field(const py::bytes& b, int64_t at, int64_t size) {
    int64_t v = 0;
    for (int64_t k = size - 1; k >= 0; k--) v = (v << 8) | b.byte_(at + k);
    return v;
}

py::list<int64_t> array(const py::bytes& b, int64_t at, int64_t count, int64_t size) {
    py::list<int64_t> out;
    for (int64_t k = 0; k < count; k++) out.append(field(b, at + k * size, size));
    return out;
}

}  // namespace

CabHeader CabHeader::from_buffer_copy(const py::bytes& buffer) {
    check_size(buffer, SIZE);
    CabHeader h;
    h.Magic = array(buffer, 0, 4, 1);
    h.Reserved = field(buffer, 4, 4);
    h.PackedSize = field(buffer, 8, 4);
    return h;
}

DOSHeader DOSHeader::from_buffer_copy(const py::bytes& buffer) {
    check_size(buffer, SIZE);
    DOSHeader h;
    h.Magic = field(buffer, 0, 2);
    h.UsedBytesInTheLastPage = field(buffer, 2, 2);
    h.FileSizeInPages = field(buffer, 4, 2);
    h.NumberOfRelocationItems = field(buffer, 6, 2);
    h.HeaderSizeInParagraphs = field(buffer, 8, 2);
    h.MinimumExtraParagraphs = field(buffer, 10, 2);
    h.MaximumExtraParagraphs = field(buffer, 12, 2);
    h.InitialRelativeSS = field(buffer, 14, 2);
    h.InitialSP_first = field(buffer, 16, 2);
    h.InitialSP = field(buffer, 18, 2);
    h.InitialIP = field(buffer, 20, 2);
    h.InitialRelativeCS = field(buffer, 22, 2);
    h.AddressOfRelocationTable = field(buffer, 24, 2);
    h.OverlayNumber = field(buffer, 26, 2);
    h.Reserved = array(buffer, 28, 4, 2);
    h.OEMid = field(buffer, 36, 2);
    h.OEMinfo = field(buffer, 38, 2);
    h.Reserved2 = array(buffer, 40, 10, 2);
    h.AddressOfNewExeHeader = field(buffer, 60, 4);
    return h;
}

FileHeader FileHeader::from_buffer_copy(const py::bytes& buffer) {
    check_size(buffer, SIZE);
    FileHeader h;
    h.Machine = field(buffer, 0, 2);
    h.NumberOfSections = field(buffer, 2, 2);
    h.TimeDateStamp = field(buffer, 4, 4);
    h.PointerToSymbolTable = field(buffer, 8, 4);
    h.NumberOfSymbols = field(buffer, 12, 4);
    h.SizeOfOptionalHeader = field(buffer, 16, 2);
    h.Characteristics = field(buffer, 18, 2);
    return h;
}

DataDirectory DataDirectory::from_buffer_copy(const py::bytes& buffer) {
    check_size(buffer, SIZE);
    DataDirectory h;
    h.RelativeVirtualAddress = field(buffer, 0, 4);
    h.Size = field(buffer, 4, 4);
    return h;
}

PE32Header PE32Header::from_buffer_copy(const py::bytes& buffer) {
    check_size(buffer, SIZE);
    PE32Header h;
    h.Magic = field(buffer, 0, 2);
    h.MajorLinkerVersion = field(buffer, 2, 1);
    h.MinorLinkerVersion = field(buffer, 3, 1);
    h.SizeOfCode = field(buffer, 4, 4);
    h.SizeOfInitializedData = field(buffer, 8, 4);
    h.SizeOfUninitializedData = field(buffer, 12, 4);
    h.AddressOfEntryPoint = field(buffer, 16, 4);
    h.BaseOfCode = field(buffer, 20, 4);
    h.BaseOfData = field(buffer, 24, 4);
    h.ImageBase = field(buffer, 28, 4);
    h.SectionAlignment = field(buffer, 32, 4);
    h.FileAlignment = field(buffer, 36, 4);
    h.MajorOperatingSystemVersion = field(buffer, 40, 2);
    h.MinorOperatingSystemVersion = field(buffer, 42, 2);
    h.MajorImageVersion = field(buffer, 44, 2);
    h.MinorImageVersion = field(buffer, 46, 2);
    h.MajorSubsystemVersion = field(buffer, 48, 2);
    h.MinorSubsystemVersion = field(buffer, 50, 2);
    h.Win32VersionValue = field(buffer, 52, 4);
    h.SizeOfImage = field(buffer, 56, 4);
    h.SizeOfHeaders = field(buffer, 60, 4);
    h.CheckSum = field(buffer, 64, 4);
    h.Subsystem = field(buffer, 68, 2);
    h.DLLCharacteristics = field(buffer, 70, 2);
    h.SizeOfStackReserve = field(buffer, 72, 4);
    h.SizeOfStackCommit = field(buffer, 76, 4);
    h.SizeOfHeapReserve = field(buffer, 80, 4);
    h.SizeOfHeapCommit = field(buffer, 84, 4);
    h.LoaderFlags = field(buffer, 88, 4);
    h.NumberOfRvaAndSize = field(buffer, 92, 4);
    for (int64_t k = 0; k < NumberOfDataDirectories; k++) {
        h.DataDirectories.append(DataDirectory::from_buffer_copy(buffer.slice(96 + k * DataDirectory::SIZE, 96 + (k + 1) * DataDirectory::SIZE)));
    }
    return h;
}

SectionHeader SectionHeader::from_buffer_copy(const py::bytes& buffer) {
    check_size(buffer, SIZE);
    SectionHeader h;
    h.Name = array(buffer, 0, NameSize, 1);
    h.VirtualSize = field(buffer, 8, 4);
    h.VirtualAddress = field(buffer, 12, 4);
    h.SizeOfRawData = field(buffer, 16, 4);
    h.PointerToRawData = field(buffer, 20, 4);
    h.PointerToRelocations = field(buffer, 24, 4);
    h.PointerToLineNumbers = field(buffer, 28, 4);
    h.NumberOfRelocations = field(buffer, 32, 2);
    h.NumberOfLineNumbers = field(buffer, 34, 2);
    h.Characteristics = field(buffer, 36, 4);
    return h;
}

int64_t get_size_of_image(const py::file& stream) {
    DOSHeader dos_header = DOSHeader::from_buffer_copy(stream.read(DOSHeader::SIZE));
    if (!(dos_header.Magic == DOSMagic)) throw py::AssertionError("Invalid DOS header");
    stream.seek(dos_header.AddressOfNewExeHeader);
    py::bytes magic = stream.read(4);
    if (!(magic == PEMagic)) throw py::AssertionError("Invalid or corrupt executable");
    FileHeader file_header = FileHeader::from_buffer_copy(stream.read(FileHeader::SIZE));
    if (!(file_header.Machine == MachineI386)) throw py::AssertionError("Input file isn't an i386 executable");
    // pe32_header = PE32Header.from_buffer_copy(stream.read(sizeof(PE32Header)))
    stream.seek(PE32Header::SIZE, 1);
    int64_t top_offset = 0;
    for (int64_t i = 0; i < file_header.NumberOfSections; i++) {
        SectionHeader section_header = SectionHeader::from_buffer_copy(stream.read(SectionHeader::SIZE));
        top_offset = py::max(top_offset, section_header.PointerToRawData + section_header.SizeOfRawData);
    }
    return top_offset;
}

namespace {

// ---- argparse, for this one parser: positionals input and output, options -h/--help and -p/--prefix PREFIX ----
// (the algorithm of Python 3.14's argparse.ArgumentParser._parse_known_args, for these four actions)

enum class Kind { Help, Prefix, Input, Output };

struct OptionString {
    const char* text;
    Kind kind;
};
// the parser's _option_string_actions, in the order they were added
const OptionString OPTION_STRINGS[] = {{"-h", Kind::Help}, {"--help", Kind::Help}, {"-p", Kind::Prefix}, {"--prefix", Kind::Prefix}};

// argparse prints this for -h when it cannot ask the terminal its width (80 columns)
const char* const HELP_TEXT =
    "\n"
    "A script to extract XDKRecoveryXenonXXXXX.X(X).exe and\n"
    "XDKSetupXenonXXXXX.X(X).exe files\n"
    "\n"
    "positional arguments:\n"
    "  input                The input executable to extract from\n"
    "  output               The directory to write cabinet files out to\n"
    "\n"
    "options:\n"
    "  -h, --help           show this help message and exit\n"
    "  -p, --prefix PREFIX  The prefix to use on the cabinet files\n";

// argparse.ArgumentError, as str(err) reads: "argument -p/--prefix: expected one argument"
struct ArgumentError {
    py::str message;
};

const char* action_name(Kind kind) {
    switch (kind) {
        case Kind::Help: return "-h/--help";
        case Kind::Prefix: return "-p/--prefix";
        case Kind::Input: return "input";
        default: return "output";
    }
}

ArgumentError argument_error(std::optional<Kind> action, const py::str& message) {
    if (!action) return ArgumentError{message};
    return ArgumentError{py::str("argument ") + py::str(action_name(*action)) + py::str(": ") + message};
}

void write_to(std::FILE* stream, const py::str& text) {
    std::string u = text.utf8();
    std::fwrite(u.data(), 1, u.size(), stream);
    std::fflush(stream);
}

py::str usage(const py::str& prog) { return py::str("usage: ") + prog + py::str(" [-h] [-p PREFIX] input output\n"); }

// parser.error(message): the usage and the message on stderr, exit code 2
[[noreturn]] void fail(const py::str& prog, const py::str& message) {
    write_to(stderr, usage(prog));
    write_to(stderr, prog + py::str(": error: ") + message + py::str("\n"));
    throw SystemExit(2);
}

struct OptionTuple {
    std::optional<Kind> action;   // None: an option the parser does not have
    py::str option_string;
    std::optional<py::str> sep;
    std::optional<py::str> explicit_arg;
};

std::optional<Kind> kind_of(const py::str& option_string) {
    for (const OptionString& o : OPTION_STRINGS)
        if (option_string == py::str(o.text)) return o.kind;
    return std::nullopt;
}

// str.partition("="): (before, sep, after) with sep None when there is no "="
struct Partition {
    py::str before;
    std::optional<py::str> sep;
    py::str after;
};
Partition partition(const py::str& s) {
    const std::u32string& u = s.u32();
    size_t eq = u.find(U'=');
    if (eq == std::u32string::npos) return {s, std::nullopt, py::str()};
    return {py::str(u.substr(0, eq)), py::str("="), py::str(u.substr(eq + 1))};
}

// ArgumentParser._get_option_tuples (allow_abbrev is on)
std::vector<OptionTuple> get_option_tuples(const py::str& option_string) {
    std::vector<OptionTuple> result;
    const std::u32string& u = option_string.u32();
    if (u[0] == U'-' && u[1] == U'-') {
        // option strings starting with two prefix characters are only split at the '='
        Partition p = partition(option_string);
        for (const OptionString& o : OPTION_STRINGS) {
            if (py::str(o.text).startswith(p.before)) {
                result.push_back({o.kind, py::str(o.text), p.sep, p.sep ? std::optional<py::str>(p.after) : std::nullopt});
            }
        }
    } else {
        // single character options can be concatenated with their arguments
        Partition p = partition(option_string);
        py::str short_option_prefix(u.substr(0, 2));
        py::str short_explicit_arg(u.substr(2));
        for (const OptionString& o : OPTION_STRINGS) {
            if (py::str(o.text) == short_option_prefix) {
                result.push_back({o.kind, py::str(o.text), py::str(""), short_explicit_arg});
            } else if (py::str(o.text).startswith(p.before)) {
                result.push_back({o.kind, py::str(o.text), p.sep, p.sep ? std::optional<py::str>(p.after) : std::nullopt});
            }
        }
    }
    return result;
}

// ArgumentParser._parse_optional: None (a positional) or the option tuples
std::optional<std::vector<OptionTuple>> parse_optional(const py::str& arg_string) {
    if (arg_string.empty()) return std::nullopt;
    const std::u32string& u = arg_string.u32();
    if (u[0] != U'-') return std::nullopt;
    if (auto kind = kind_of(arg_string)) return std::vector<OptionTuple>{{kind, arg_string, std::nullopt, std::nullopt}};
    if (u.size() == 1) return std::nullopt;
    Partition p = partition(arg_string);
    if (p.sep) {
        if (auto kind = kind_of(p.before)) return std::vector<OptionTuple>{{kind, p.before, p.sep, p.after}};
    }
    std::vector<OptionTuple> option_tuples = get_option_tuples(arg_string);
    if (!option_tuples.empty()) return option_tuples;
    // a negative number is a positional (the parser has no options that look like one)
    if (py::re::match(py::str("-\\.?\\d"), arg_string)) return std::nullopt;
    if (arg_string.contains(py::str(" "))) return std::nullopt;
    return std::vector<OptionTuple>{{std::nullopt, arg_string, std::nullopt, std::nullopt}};
}

// ArgumentParser._get_nargs_pattern: an option's value is one 'A'; a positional's is -*A-*
const char* nargs_pattern(Kind kind) {
    switch (kind) {
        case Kind::Help: return "([AO]{0})";
        case Kind::Prefix: return "([A])";
        default: return "(-*A-*)";
    }
}

// ArgumentParser._match_argument: how many pattern characters the option takes (ArgumentError if it takes none it needs)
int64_t match_argument(Kind action, const std::string& arg_strings_pattern) {
    auto m = py::re::match(py::str(nargs_pattern(action)), py::str(arg_strings_pattern));
    if (!m) throw argument_error(action, py::str("expected one argument"));
    return m->end(1) - m->start(1);
}

// ArgumentParser._match_arguments_partial: how many arg strings each leading positional takes
std::vector<int64_t> match_arguments_partial(const std::vector<Kind>& actions, const std::string& arg_strings_pattern) {
    for (int64_t i = static_cast<int64_t>(actions.size()); i > 0; i--) {
        std::string pattern;
        for (int64_t k = 0; k < i; k++) pattern += nargs_pattern(actions[static_cast<size_t>(k)]);
        auto m = py::re::match(py::str(pattern), py::str(arg_strings_pattern));
        if (m) {
            std::vector<int64_t> result;
            for (int64_t g = 1; g <= i; g++) result.push_back(m->end(g) - m->start(g));
            if (m->end(0) < static_cast<int64_t>(arg_strings_pattern.size()) && arg_strings_pattern[static_cast<size_t>(m->end(0))] == 'O') {
                while (!result.empty() && result.back() == 0) result.pop_back();
            }
            return result;
        }
    }
    return {};
}

struct Namespace {
    py::str input;
    py::str output;
    py::str prefix = py::str("data");
};

// ArgumentParser._parse_known_args (intermixed off), then parse_args' check of the extras
Namespace parse_args(const py::list<py::str>& argv, const py::str& prog) {
    std::vector<py::str> arg_strings(argv.vec().begin(), argv.vec().end());
    Namespace ns;

    // find all option indices, and determine the arg_string_pattern: an 'O' for an option, an 'A' for an
    // argument, a '-' for the first '--'
    std::map<int64_t, std::vector<OptionTuple>> option_string_indices;
    std::string arg_strings_pattern;
    for (size_t i = 0; i < arg_strings.size(); i++) {
        if (arg_strings[i] == py::str("--")) {   // all args after -- are non-options
            arg_strings_pattern += '-';
            for (size_t k = i + 1; k < arg_strings.size(); k++) arg_strings_pattern += 'A';
            break;
        }
        std::optional<std::vector<OptionTuple>> option_tuples = parse_optional(arg_strings[i]);
        if (!option_tuples) {
            arg_strings_pattern += 'A';
        } else {
            option_string_indices[static_cast<int64_t>(i)] = *option_tuples;
            arg_strings_pattern += 'O';
        }
    }

    std::vector<Kind> seen_actions;
    std::vector<py::str> extras;
    std::vector<Kind> positionals{Kind::Input, Kind::Output};

    auto take_action = [&](Kind action, const std::vector<py::str>& argument_strings) {
        seen_actions.push_back(action);
        switch (action) {
            case Kind::Help:
                write_to(stdout, usage(prog) + py::str(HELP_TEXT));
                throw SystemExit(0);
            case Kind::Prefix: ns.prefix = argument_strings[0]; break;
            case Kind::Input: ns.input = argument_strings[0]; break;
            case Kind::Output: ns.output = argument_strings[0]; break;
        }
    };

    // function to convert arg_strings into an optional action
    auto consume_optional = [&](int64_t start_index) -> int64_t {
        const std::vector<OptionTuple>& option_tuples = option_string_indices[start_index];
        // if multiple actions match, the option string was ambiguous
        if (option_tuples.size() > 1) {
            py::list<py::str> names;
            for (const OptionTuple& t : option_tuples) names.append(t.option_string);
            throw ArgumentError{py::str("ambiguous option: ") + arg_strings[static_cast<size_t>(start_index)] + py::str(" could match ") +
                                py::join(py::str(", "), names)};
        }
        std::optional<Kind> action = option_tuples[0].action;
        py::str option_string = option_tuples[0].option_string;
        std::optional<py::str> sep = option_tuples[0].sep;
        std::optional<py::str> explicit_arg = option_tuples[0].explicit_arg;

        // identify additional optionals in the same arg string (e.g. -xyz is the same as -x -y -z if no args are required)
        std::vector<std::pair<Kind, std::vector<py::str>>> action_tuples;
        int64_t stop = 0;
        for (;;) {
            // if we found no optional action, skip it
            if (!action) {
                extras.push_back(arg_strings[static_cast<size_t>(start_index)]);
                return start_index + 1;
            }
            // if there is an explicit argument, try to match the optional's string arguments to only this
            if (explicit_arg) {
                int64_t arg_count = match_argument(*action, "A");
                // if the action is a single-dash option and takes no arguments, try to parse more single-dash
                // options out of the tail of the option string
                if (arg_count == 0 && option_string.u32()[1] != U'-' && !explicit_arg->empty()) {
                    if ((sep && !sep->empty()) || explicit_arg->u32()[0] == U'-') {
                        throw argument_error(action, py::str("ignored explicit argument ") + py::repr(*explicit_arg));
                    }
                    action_tuples.push_back({*action, {}});
                    std::u32string chosen;
                    chosen += option_string.u32()[0];
                    chosen += explicit_arg->u32()[0];
                    option_string = py::str(chosen);
                    if (auto next = kind_of(option_string)) {
                        action = next;
                        py::str rest(explicit_arg->u32().substr(1));
                        if (rest.empty()) {
                            sep = std::nullopt;
                            explicit_arg = std::nullopt;
                        } else if (rest.u32()[0] == U'=') {
                            sep = py::str("=");
                            explicit_arg = py::str(rest.u32().substr(1));
                        } else {
                            sep = py::str("");
                            explicit_arg = rest;
                        }
                    } else {
                        std::u32string tail;
                        tail += chosen[0];
                        tail += explicit_arg->u32();
                        extras.push_back(py::str(tail));
                        stop = start_index + 1;
                        break;
                    }
                } else if (arg_count == 1) {
                    // if the action expect exactly one argument, we've successfully matched the option; exit the loop
                    stop = start_index + 1;
                    action_tuples.push_back({*action, {*explicit_arg}});
                    break;
                } else {
                    // error if a double-dash option did not use the explicit argument
                    throw argument_error(action, py::str("ignored explicit argument ") + py::repr(*explicit_arg));
                }
            } else {
                // if there is no explicit argument, try to match the optional's string arguments with the following strings
                int64_t start = start_index + 1;
                int64_t arg_count = match_argument(*action, arg_strings_pattern.substr(static_cast<size_t>(start)));
                stop = start + arg_count;
                action_tuples.push_back({*action, std::vector<py::str>(arg_strings.begin() + start, arg_strings.begin() + stop)});
                break;
            }
        }
        for (const auto& t : action_tuples) take_action(t.first, t.second);
        return stop;
    };

    // function to convert arg_strings into positional actions
    auto consume_positionals = [&](int64_t start_index) -> int64_t {
        // match as many Positionals as possible
        std::vector<int64_t> arg_counts = match_arguments_partial(positionals, arg_strings_pattern.substr(static_cast<size_t>(start_index)));
        // slice off the appropriate arg strings for each Positional and add the Positional and its args to the list
        for (size_t k = 0; k < arg_counts.size(); k++) {
            int64_t arg_count = arg_counts[k];
            std::vector<py::str> args(arg_strings.begin() + start_index, arg_strings.begin() + start_index + arg_count);
            // strip out the first '--'
            size_t dash = arg_strings_pattern.find('-', static_cast<size_t>(start_index));
            if (dash != std::string::npos && static_cast<int64_t>(dash) < start_index + arg_count) {
                for (size_t a = 0; a < args.size(); a++) {
                    if (args[a] == py::str("--")) {
                        args.erase(args.begin() + static_cast<std::ptrdiff_t>(a));
                        break;
                    }
                }
            }
            start_index += arg_count;
            take_action(positionals[k], args);
        }
        // slice off the Positionals that we just parsed
        positionals.erase(positionals.begin(), positionals.begin() + static_cast<std::ptrdiff_t>(arg_counts.size()));
        return start_index;
    };

    try {
        // consume Positionals and Optionals alternately, until we have passed the last option string
        int64_t start_index = 0;
        int64_t max_option_string_index = option_string_indices.empty() ? -1 : option_string_indices.rbegin()->first;
        while (start_index <= max_option_string_index) {
            // consume any Positionals preceding the next option
            int64_t next_option_string_index = start_index;
            while (next_option_string_index <= max_option_string_index) {
                if (option_string_indices.count(next_option_string_index)) break;
                next_option_string_index++;
            }
            if (start_index != next_option_string_index) {
                int64_t positionals_end_index = consume_positionals(start_index);
                // only try to parse the next optional if we didn't consume the option string during the positionals parsing
                if (positionals_end_index > start_index) {
                    start_index = positionals_end_index;
                    continue;
                } else {
                    start_index = positionals_end_index;
                }
            }
            // if we consumed all the positionals we could and we're not at the index of an option string, there were extra arguments
            if (!option_string_indices.count(start_index)) {
                for (int64_t k = start_index; k < next_option_string_index; k++) extras.push_back(arg_strings[static_cast<size_t>(k)]);
                start_index = next_option_string_index;
            }
            // consume the next optional and any arguments for it
            start_index = consume_optional(start_index);
        }
        // consume any positionals following the last Optional
        int64_t stop_index = consume_positionals(start_index);
        // if we didn't consume all the argument strings, there were extras
        for (size_t k = static_cast<size_t>(stop_index); k < arg_strings.size(); k++) extras.push_back(arg_strings[k]);

        // make sure all required actions were present
        py::list<py::str> required_actions;
        for (Kind action : {Kind::Input, Kind::Output}) {
            bool seen = false;
            for (Kind s : seen_actions) seen = seen || s == action;
            if (!seen) required_actions.append(py::str(action_name(action)));
        }
        if (!required_actions.empty()) {
            throw ArgumentError{py::str("the following arguments are required: ") + py::join(py::str(", "), required_actions)};
        }
    } catch (const ArgumentError& err) {
        fail(prog, err.message);
    }
    if (!extras.empty()) {
        py::list<py::str> rest;
        for (const py::str& e : extras) rest.append(e);
        fail(prog, py::str("unrecognized arguments: ") + py::join(py::str(" "), rest));
    }
    return ns;
}

// os.mkdir: one folder, its parent must exist (FileExistsError if the name is taken, FileNotFoundError if the parent is missing)
void mkdir(const py::str& path) {
    if (path.empty()) throw py::FileNotFoundError("[WinError 3] The system cannot find the path specified: ''");
    std::error_code ec;
    std::filesystem::path p = std::filesystem::u8path(path.utf8());
    if (std::filesystem::exists(p, ec)) {
        throw py::FileExistsError("[WinError 183] Cannot create a file when that file already exists: " + py::repr(path).utf8());
    }
    std::filesystem::create_directory(p, ec);
    if (ec) py::os::raise_os_(ec, path);
}

}  // namespace

void main(const py::list<py::str>& argv, const py::str& prog) {
    Namespace args = parse_args(argv, prog);

    if (!py::os::path::isfile(args.input)) throw py::AssertionError("Input file missing");

    if (!py::os::path::isdir(args.output)) mkdir(args.output);

    py::file recovery_exe = py::open(args.input, py::str("rb"));
    int64_t pe_size = get_size_of_image(recovery_exe);
    py::print(py::str("Start of data: 0x") + py::format(pe_size, "04X"));
    int64_t index = 0;
    for (;;) {
        recovery_exe.seek(pe_size);
        CabHeader cab_header = CabHeader::from_buffer_copy(recovery_exe.read(CabHeader::SIZE));
        if (!(py::bytes::from_list(cab_header.Magic) == CABMagic)) break;
        recovery_exe.seek(pe_size);
        py::bytes cab_data = recovery_exe.read(cab_header.PackedSize);
        py::str cab_file_name = args.prefix + py::str("_") + py::to_str(index) + py::str(".cab");
        index += 1;
        py::print(py::str("Extracting \"") + cab_file_name + py::str("\"..."));
        {
            py::file f = py::open(py::os::path::join(args.output, cab_file_name), py::str("wb"));
            f.write(cab_data);
        }
        pe_size += cab_header.PackedSize;
    }
}

}  // namespace codebreaker::formats::xbox::crypto::xdk_extract
