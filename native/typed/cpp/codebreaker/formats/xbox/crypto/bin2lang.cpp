// codebreaker/formats/xbox/crypto/bin2lang.cpp -- C++ port of codebreaker.formats.xbox.crypto.bin2lang.

#include "codebreaker/formats/xbox/crypto/bin2lang.hpp"

#include <cstdio>
#include <functional>
#include <iostream>
#include <vector>

namespace codebreaker::formats::xbox::crypto::bin2lang {

namespace {

std::ostream* out_stream_ = &std::cout;
std::ostream* err_stream_ = &std::cerr;

// print(text)
void print_to(std::ostream& os, const py::str& text) { os << text.utf8() << "\n"; }

// one chunk of input: read(byte_count), the empty bytes at the end
using Reader = std::function<py::bytes(int64_t)>;

// "\t" + ", ".join(f"0x{x:02X}" for x in data) + ","  for each byte_count bytes, the last without its comma
py::list<py::str> byte_lines(const Reader& read, int64_t byte_count) {
    py::list<py::str> lines;
    while (true) {
        py::bytes data = read(byte_count);
        if (data.empty()) {
            break;
        }
        py::list<py::str> hexes;
        for (int64_t i = 0; i < data.size(); ++i) {
            char buf[8];
            std::snprintf(buf, sizeof buf, "0x%02X", static_cast<unsigned>(data[i]));
            hexes.append(py::str(buf));
        }
        lines.append(py::str("\t") + py::join(py::str(", "), hexes) + py::str(","));
    }
    lines[-1] = lines[-1].rstrip(py::str(","));
    return lines;
}

// the text between `head` and `tail`: the lines of bytes, one per line
py::str block(const py::str& head, const Reader& read, int64_t byte_count, const py::str& tail) {
    py::str out = head;
    for (const py::str& x : byte_lines(read, byte_count)) {
        out += x + py::str("\n");
    }
    return out + tail;
}

py::str format_(const Reader& read, const std::function<void()>& close_input, const std::optional<py::str>& dout,
                Language language, const py::str& var_name, int64_t byte_count) {
    py::str sio;
    const int64_t lang = static_cast<int64_t>(language);
    if (lang == static_cast<int64_t>(Language::PYTHON)) {
        sio = block(var_name + py::str(" = bytearray([\n"), read, byte_count, py::str("])\n"));
    } else if (lang == static_cast<int64_t>(Language::C) || lang == static_cast<int64_t>(Language::CPLUSPLUS)) {
        sio = block(py::str("#ifndef BYTE\ntypedef unsigned char BYTE\n#endif\n\n#ifndef __") + var_name +
                        py::str("__\n#define __") + var_name + py::str("__\nBYTE ") + var_name + py::str("[] = {\n"),
                    read, byte_count, py::str("};\n#endif\n"));
    } else if (lang == static_cast<int64_t>(Language::CSHARP)) {
        sio = block(py::str("#region ") + var_name + py::str("\nbyte[] ") + var_name + py::str(" = {\n"), read, byte_count,
                    py::str("};\n#endregion\n"));
    } else if (lang == static_cast<int64_t>(Language::PHP) || lang == static_cast<int64_t>(Language::PHP_NEW)) {  // using fast arrays
        sio = block(py::str("$") + var_name + py::str(" = [\n"), read, byte_count, py::str("];\n"));
    } else if (lang == static_cast<int64_t>(Language::PHP_OLD)) {  // using slow arrays
        sio = block(py::str("$") + var_name + py::str(" = array(\n"), read, byte_count, py::str(");\n"));
    }
    const py::str data = sio;

    // close input stream
    close_input();

    if (dout) {
        // Path(dout).write_text(data): the locale's encoding, "\n" as the platform's newline
        py::file f = py::open(dout->empty() ? py::str(".") : *dout, py::str("w"));
        f.write(data);
        f.close();
    } else {
        print_to(*out_stream_, data);
    }
    return data;
}

// io.BytesIO(data).read(size): size < 0 reads what is left
Reader memory_reader(const std::string& bytes) {
    auto pos = std::make_shared<size_t>(0);
    auto buf = std::make_shared<std::string>(bytes);
    return [pos, buf](int64_t size) -> py::bytes {
        const size_t left = buf->size() - *pos;
        const size_t n = size < 0 ? left : std::min<size_t>(static_cast<size_t>(size), left);
        py::bytes b(buf->substr(*pos, n));
        *pos += n;
        return b;
    };
}

}  // namespace

py::str lowercase(const py::str& s) { return s.lower(); }

py::str lang_format(const py::str& din, const std::optional<py::str>& dout, Language language, const py::str& var_name,
                    int64_t byte_count) {
    py::file sin = py::open_rb(din);
    auto left = std::make_shared<int64_t>(-1);   // what is left of the file, once known
    Reader read = [&sin, left](int64_t size) -> py::bytes {
        if (size < -1) throw py::ValueError("read length must be non-negative or -1");
        if (size > INT64_MAX - 33) throw py::OverflowError("byte string is too large");   // (the buffer would not be)
        if (size >= (int64_t(1) << 36)) throw py::MemoryError();                          // (nor would the memory be)
        if (size > 0) {   // (no more than what is left of the file is asked for)
            if (*left < 0) {
                *left = sin.seek(0, 2);
                sin.seek(0, 0);
            }
            size = std::min(size, *left);
        }
        py::bytes got = sin.read(size);
        if (*left >= 0) *left = std::max<int64_t>(*left - got.size(), 0);
        return got;
    };
    return format_(read, [&sin]() { sin.close(); }, dout, language, var_name, byte_count);
}

py::str lang_format(const py::bytes& din, const std::optional<py::str>& dout, Language language, const py::str& var_name,
                    int64_t byte_count) {
    return format_(memory_reader(din.raw()), []() {}, dout, language, var_name, byte_count);
}

py::str lang_format(const py::bytearray& din, const std::optional<py::str>& dout, Language language,
                    const py::str& var_name, int64_t byte_count) {
    return format_(memory_reader(py::bytes(din.view()).raw()), []() {}, dout, language, var_name, byte_count);
}

void set_streams(std::ostream& out, std::ostream& err) {
    out_stream_ = &out;
    err_stream_ = &err;
}

// ---- the command line: argparse's reading of
//   bin2lang.py [-h] [-l LANGUAGE] [-b BYTES] [-v VARIABLE] input output

namespace {

enum Dest { HELP = 0, LANGUAGE, BYTES, VARIABLE, INPUT, OUTPUT, NONE = -1 };

struct Action {
    Dest dest;
    std::vector<py::str> option_strings;   // none: a positional
    bool takes_value;                      // nargs None (one argument) or 0
    const char* metavar;
};

const std::vector<Action>& actions() {
    static const std::vector<Action> a = {
        {HELP, {py::str("-h"), py::str("--help")}, false, "HELP"},
        {LANGUAGE, {py::str("-l"), py::str("--language")}, true, "LANGUAGE"},
        {BYTES, {py::str("-b"), py::str("--bytes")}, true, "BYTES"},
        {VARIABLE, {py::str("-v"), py::str("--variable")}, true, "VARIABLE"},
        {INPUT, {}, true, "input"},
        {OUTPUT, {}, true, "output"},
    };
    return a;
}

// the parser's option strings in the order argparse keeps them (_option_string_actions)
const std::vector<std::pair<py::str, Dest>>& option_map() {
    static const std::vector<std::pair<py::str, Dest>> m = [] {
        std::vector<std::pair<py::str, Dest>> out;
        for (const Action& a : actions())
            for (const py::str& s : a.option_strings) out.emplace_back(s, a.dest);
        return out;
    }();
    return m;
}

const Action& action_of(Dest d) { return actions()[static_cast<size_t>(d)]; }

std::optional<Dest> lookup(const py::str& s) {
    for (const auto& kv : option_map())
        if (kv.first == s) return kv.second;
    return std::nullopt;
}

struct ArgumentError : py::Exception {
    ArgumentError(Dest action, const py::str& message)
        : py::Exception(message.utf8()), dest(action), has_action(true), msg(message) {}
    explicit ArgumentError(const py::str& message) : py::Exception(message.utf8()), dest(NONE), has_action(false), msg(message) {}
    Dest dest;
    bool has_action;
    py::str msg;
    const char* type() const override { return "ArgumentError"; }
    // str(err): "argument -b/--bytes: expected one argument"
    py::str text() const {
        if (!has_action) return msg;
        const Action& a = action_of(dest);
        py::str name;
        if (!a.option_strings.empty()) {
            for (size_t i = 0; i < a.option_strings.size(); ++i) name += (i ? py::str("/") : py::str("")) + a.option_strings[i];
        } else {
            name = py::str(a.metavar);
        }
        return py::str("argument ") + name + py::str(": ") + msg;
    }
};

const int kWidth = 80 - 2;   // HelpFormatter's width: the terminal's columns (80 when there is none) less 2

// format_usage(), without its closing newline
py::str usage_line(const py::str& prog) {
    const py::str prefix("usage: ");
    py::list<py::str> opt_parts = {py::str("[-h]"), py::str("[-l LANGUAGE]"), py::str("[-b BYTES]"), py::str("[-v VARIABLE]")};
    py::list<py::str> pos_parts = {py::str("input"), py::str("output")};
    py::list<py::str> all;
    all.extend(opt_parts);
    all.extend(pos_parts);
    py::str usage = (prog.empty() ? py::str("") : prog + py::str(" ")) + py::join(py::str(" "), all);   // (the empty parts are left out)
    const int64_t text_width = kWidth;
    if (py::len(prefix) + py::len(usage) > text_width) {
        auto get_lines = [&](const py::list<py::str>& parts, const py::str& indent, const std::optional<py::str>& pfx) {
            py::list<py::str> lines;
            py::list<py::str> line;
            const int64_t indent_length = py::len(indent);
            int64_t line_len = pfx ? py::len(*pfx) - 1 : indent_length - 1;
            for (const py::str& part : parts) {
                const int64_t part_len = py::len(part);
                if (line_len + 1 + part_len > text_width && !line.empty()) {
                    lines.append(indent + py::join(py::str(" "), line));
                    line = py::list<py::str>();
                    line_len = indent_length - 1;
                }
                line.append(part);
                line_len += part_len + 1;
            }
            if (!line.empty()) lines.append(indent + py::join(py::str(" "), line));
            if (pfx) lines[0] = lines[0].slice(indent_length, py::len(lines[0]));
            return lines;
        };
        py::list<py::str> lines;
        const int64_t prog_len = py::len(prog);
        if (static_cast<double>(py::len(prefix) + prog_len) <= 0.75 * static_cast<double>(text_width)) {
            // if prog is short, follow it with optionals or positionals
            const py::str indent(std::u32string(static_cast<size_t>(py::len(prefix) + prog_len + 1), U' '));
            py::list<py::str> first = {prog};
            first.extend(opt_parts);
            lines = get_lines(first, indent, prefix);
            lines.extend(get_lines(pos_parts, indent, std::nullopt));
        } else {
            // if prog is long, put it on its own line
            const py::str indent(std::u32string(static_cast<size_t>(py::len(prefix)), U' '));
            lines = get_lines(all, indent, std::nullopt);
            if (py::len(lines) > 1) {
                lines = py::list<py::str>();
                lines.extend(get_lines(opt_parts, indent, std::nullopt));
                lines.extend(get_lines(pos_parts, indent, std::nullopt));
            }
            lines = py::list<py::str>{prog} + lines;
        }
        usage = py::join(py::str("\n"), lines);
    }
    return prefix + usage;
}

py::str help_text(const py::str& prog) {
    return usage_line(prog) + py::str("\n\n") +
           py::str("A script to make embedding binaries in code a breeze\n"
                   "\n"
                   "positional arguments:\n"
                   "  input                 The binary to embed\n"
                   "  output                A file to write the output to\n"
                   "\n"
                   "options:\n"
                   "  -h, --help            show this help message and exit\n"
                   "  -l, --language LANGUAGE\n"
                   "                        The programming language to use\n"
                   "  -b, --bytes BYTES     The number of bytes per line\n"
                   "  -v, --variable VARIABLE\n"
                   "                        The name of the variable\n");
}

// what the options leave in the Namespace
struct Namespace {
    py::str input;
    py::str output;
    py::str language = py::str("python");
    py::str variable = py::str("output");
    int64_t bytes = 16;
    bool bytes_huge = false;   // the number given for -b/--bytes does not fit in 64 bits
};

struct OptionTuple {
    std::optional<Dest> action;
    py::str option_string;
    std::optional<py::str> sep;
    std::optional<py::str> explicit_arg;
};

bool starts_with_prefix(const py::str& s) { return !s.empty() && s.at_(0) == U'-'; }

std::vector<OptionTuple> get_option_tuples(const py::str& arg) {
    std::vector<OptionTuple> result;
    // option strings starting with two prefix characters are only split at the '='
    if (arg.at_(0) == U'-' && arg.at_(1) == U'-') {
        auto [option_prefix, sep, explicit_arg] = arg.partition(py::str("="));
        const bool has_sep = !sep.empty();
        for (const auto& kv : option_map()) {
            if (kv.first.startswith(option_prefix)) {
                result.push_back({kv.second, kv.first, has_sep ? std::optional<py::str>(sep) : std::nullopt,
                                  has_sep ? std::optional<py::str>(explicit_arg) : std::nullopt});
            }
        }
    } else {
        // single character options can be concatenated with their arguments
        auto [option_prefix, sep, explicit_arg] = arg.partition(py::str("="));
        const bool has_sep = !sep.empty();
        const py::str short_option_prefix = arg.slice(0, 2);
        const py::str short_explicit_arg = arg.slice(2, py::len(arg));
        for (const auto& kv : option_map()) {
            if (kv.first == short_option_prefix) {
                result.push_back({kv.second, kv.first, py::str(""), short_explicit_arg});
            } else if (kv.first.startswith(option_prefix)) {
                result.push_back({kv.second, kv.first, has_sep ? std::optional<py::str>(sep) : std::nullopt,
                                  has_sep ? std::optional<py::str>(explicit_arg) : std::nullopt});
            }
        }
    }
    return result;
}

// None: a positional; else the interpretations of the option string
std::optional<std::vector<OptionTuple>> parse_optional(const py::str& arg) {
    if (arg.empty()) return std::nullopt;
    if (!starts_with_prefix(arg)) return std::nullopt;
    if (auto a = lookup(arg)) return std::vector<OptionTuple>{{a, arg, std::nullopt, std::nullopt}};
    if (py::len(arg) == 1) return std::nullopt;
    {
        auto [option_string, sep, explicit_arg] = arg.partition(py::str("="));
        if (!sep.empty()) {
            if (auto a = lookup(option_string)) return std::vector<OptionTuple>{{a, option_string, sep, explicit_arg}};
        }
    }
    std::vector<OptionTuple> option_tuples = get_option_tuples(arg);
    if (!option_tuples.empty()) return option_tuples;
    // if it looks like a negative number, it was meant to be positional
    if (py::re::match(py::str("-\\.?\\d"), arg)) return std::nullopt;
    // if it contains a space, it was meant to be a positional
    if (arg.contains(py::str(" "))) return std::nullopt;
    // it was meant to be an optional but there is no such option in this parser
    return std::vector<OptionTuple>{{std::nullopt, arg, std::nullopt, std::nullopt}};
}

// the number given for an int option: int(text), kept apart when it is beyond 64 bits
void set_bytes(Namespace& ns, const py::str& text) {
    try {
        ns.bytes = py::int_from_str(text);
        ns.bytes_huge = false;
    } catch (const py::OverflowError&) {
        // int() takes any size of number: huge only when the text is one
        if (!py::re::fullmatch(py::str("\\s*[+-]?\\d+(?:_\\d+)*\\s*"), text)) {
            throw ArgumentError(BYTES, py::str("invalid int value: ") + py::repr(text));
        }
        ns.bytes_huge = true;
    } catch (const py::ValueError&) {
        throw ArgumentError(BYTES, py::str("invalid int value: ") + py::repr(text));
    }
}

void take_action(Namespace& ns, Dest dest, const py::list<py::str>& argument_strings, const py::str& prog) {
    (void)prog;
    if (dest == LANGUAGE) ns.language = lowercase(argument_strings[0]);
    else if (dest == BYTES) set_bytes(ns, argument_strings[0]);
    else if (dest == VARIABLE) ns.variable = argument_strings[0];
    else if (dest == INPUT) ns.input = argument_strings[0];
    else if (dest == OUTPUT) ns.output = argument_strings[0];
}

}  // namespace

namespace {

[[noreturn]] void parser_error(const py::str& prog, const py::str& message) {
    *err_stream_ << usage_line(prog).utf8() << "\n";   // print_usage
    *err_stream_ << prog.utf8() << ": error: " << message.utf8() << "\n";
    throw SystemExit(2);
}

// parse_args(sys.argv[1:])
Namespace parse_args(const py::list<py::str>& arg_strings, const py::str& prog) {
    Namespace ns;
    py::list<py::str> extras;
    try {
        // find all option indices, and determine the arg_string_pattern: an 'O' for an option, an 'A' for
        // an argument, a '-' for a '--'
        std::vector<std::pair<int64_t, std::vector<OptionTuple>>> option_string_indices;
        auto option_at = [&](int64_t i) -> const std::vector<OptionTuple>* {
            for (const auto& kv : option_string_indices)
                if (kv.first == i) return &kv.second;
            return nullptr;
        };
        std::string pattern;
        for (int64_t i = 0; i < py::len(arg_strings); ++i) {
            if (arg_strings[i] == "--") {
                pattern.push_back('-');
                for (++i; i < py::len(arg_strings); ++i) pattern.push_back('A');
            } else {
                auto option_tuples = parse_optional(arg_strings[i]);
                if (!option_tuples) {
                    pattern.push_back('A');
                } else {
                    option_string_indices.emplace_back(i, *option_tuples);
                    pattern.push_back('O');
                }
            }
        }
        const py::str arg_strings_pattern(pattern);
        auto pattern_from = [&](int64_t start) { return arg_strings_pattern.slice(start, py::len(arg_strings_pattern)); };
        std::vector<Dest> seen;
        auto mark_seen = [&](Dest d) { seen.push_back(d); };

        // the positionals left to be parsed
        std::vector<Dest> positionals = {INPUT, OUTPUT};

        auto match_argument = [&](Dest dest, const py::str& patt) -> int64_t {
            const Action& a = action_of(dest);
            if (!a.takes_value) return 0;   // nargs=0: '([AO]{0})'
            if (patt.empty() || patt.at_(0) != U'A') throw ArgumentError(dest, py::str("expected one argument"));
            return 1;
        };

        // consume_optional
        auto consume_optional = [&](int64_t start_index) -> int64_t {
            const std::vector<OptionTuple>& option_tuples = *option_at(start_index);
            if (option_tuples.size() > 1) {
                py::list<py::str> names;
                for (const auto& t : option_tuples) names.append(t.option_string);
                throw ArgumentError(py::str("ambiguous option: ") + arg_strings[start_index] + py::str(" could match ") +
                                    py::join(py::str(", "), names));
            }
            std::optional<Dest> action = option_tuples[0].action;
            py::str option_string = option_tuples[0].option_string;
            std::optional<py::str> sep = option_tuples[0].sep;
            std::optional<py::str> explicit_arg = option_tuples[0].explicit_arg;

            // identify additional optionals in the same arg string (e.g. -xyz is -x -y -z if no args are required)
            struct Taken { Dest action; py::list<py::str> args; };
            std::vector<Taken> action_tuples;
            int64_t stop = 0;
            while (true) {
                // if we found no optional action, skip it
                if (!action) {
                    extras.append(arg_strings[start_index]);
                    return start_index + 1;
                }
                if (explicit_arg) {
                    // if there is an explicit argument, try to match the optional's string arguments to only this
                    const int64_t arg_count = match_argument(*action, py::str("A"));
                    // if the action is a single-dash option and takes no arguments, try to parse more
                    // single-dash options out of the tail of the option string
                    if (arg_count == 0 && option_string.at_(1) != U'-' && !explicit_arg->empty()) {
                        if ((sep && !sep->empty()) || explicit_arg->at_(0) == U'-') {
                            throw ArgumentError(*action, py::str("ignored explicit argument ") + py::repr(*explicit_arg));
                        }
                        action_tuples.push_back({*action, py::list<py::str>()});
                        const py::str ch = option_string.slice(0, 1);
                        option_string = ch + explicit_arg->slice(0, 1);
                        if (auto next = lookup(option_string)) {
                            action = next;
                            explicit_arg = explicit_arg->slice(1, py::len(*explicit_arg));
                            if (explicit_arg->empty()) {
                                sep = std::nullopt;
                                explicit_arg = std::nullopt;
                            } else if (explicit_arg->at_(0) == U'=') {
                                sep = py::str("=");
                                explicit_arg = explicit_arg->slice(1, py::len(*explicit_arg));
                            } else {
                                sep = py::str("");
                            }
                        } else {
                            extras.append(ch + *explicit_arg);
                            stop = start_index + 1;
                            break;
                        }
                    } else if (arg_count == 1) {
                        // if the action expect exactly one argument, we've successfully matched the optional
                        stop = start_index + 1;
                        action_tuples.push_back({*action, py::list<py::str>{*explicit_arg}});
                        break;
                    } else {
                        // error if a double-dash option did not use the explicit argument
                        throw ArgumentError(*action, py::str("ignored explicit argument ") + py::repr(*explicit_arg));
                    }
                } else {
                    // if there is no explicit argument, try to match the optional's string arguments with
                    // the following strings
                    const int64_t start = start_index + 1;
                    const int64_t arg_count = match_argument(*action, pattern_from(start));
                    stop = start + arg_count;
                    action_tuples.push_back({*action, arg_strings.slice(start, stop)});
                    break;
                }
            }
            for (const Taken& t : action_tuples) {
                mark_seen(t.action);
                if (t.action == HELP) {
                    *out_stream_ << help_text(prog).utf8();
                    throw SystemExit(0);
                }
                take_action(ns, t.action, t.args, prog);
            }
            return stop;
        };

        // consume_positionals
        auto consume_positionals = [&](int64_t start_index) -> int64_t {
            // match as many Positionals as possible
            const py::str selected = pattern_from(start_index);
            std::vector<int64_t> arg_counts;
            for (size_t n = positionals.size(); n > 0; --n) {
                std::string rx;
                for (size_t k = 0; k < n; ++k) rx += "(-*A-*)";
                auto m = py::re::match(py::str(rx), selected);
                if (m) {
                    for (size_t k = 1; k <= n; ++k) arg_counts.push_back(py::len(m->group(static_cast<int64_t>(k))));
                    if (m->end() < py::len(selected) && selected.at_(m->end()) == U'O') {
                        while (!arg_counts.empty() && arg_counts.back() == 0) arg_counts.pop_back();
                    }
                    break;
                }
            }
            // slice off the appropriate arg strings for each Positional and add the Positional and its args
            size_t used = 0;
            for (size_t k = 0; k < arg_counts.size(); ++k, ++used) {
                const int64_t arg_count = arg_counts[k];
                py::list<py::str> args = arg_strings.slice(start_index, start_index + arg_count);
                // strip out the first '--'
                if (arg_strings_pattern.find(py::str("-"), start_index, start_index + arg_count) >= 0) args.remove(py::str("--"));
                start_index += arg_count;
                mark_seen(positionals[k]);
                take_action(ns, positionals[k], args, prog);
            }
            positionals.erase(positionals.begin(), positionals.begin() + static_cast<std::ptrdiff_t>(used));
            return start_index;
        };

        // consume Positionals and Optionals alternately, until we have passed the last option string
        int64_t start_index = 0;
        const int64_t max_option_string_index = option_string_indices.empty() ? -1 : option_string_indices.back().first;
        while (start_index <= max_option_string_index) {
            // consume any Positionals preceding the next option
            int64_t next_option_string_index = start_index;
            while (next_option_string_index <= max_option_string_index) {
                if (option_at(next_option_string_index)) break;
                next_option_string_index += 1;
            }
            if (start_index != next_option_string_index) {
                const int64_t positionals_end_index = consume_positionals(start_index);
                // only try to parse the next optional if we didn't consume the option string during the positionals parsing
                if (positionals_end_index > start_index) {
                    start_index = positionals_end_index;
                    continue;
                }
                start_index = positionals_end_index;
            }
            // if we consumed all the positionals we could and we're not at the index of an option string,
            // there were extra arguments
            if (!option_at(start_index)) {
                extras.extend(arg_strings.slice(start_index, next_option_string_index));
                start_index = next_option_string_index;
            }
            // consume the next optional and any arguments for it
            start_index = consume_optional(start_index);
        }
        // consume any positionals following the last Optional
        const int64_t stop_index = consume_positionals(start_index);
        // if we didn't consume all the argument strings, there were extras
        extras.extend(arg_strings.slice(stop_index, py::len(arg_strings)));

        // make sure all required actions were present
        py::list<py::str> required;
        for (Dest d : {INPUT, OUTPUT}) {
            bool got = false;
            for (Dest s : seen) got = got || s == d;
            if (!got) required.append(py::str(action_of(d).metavar));
        }
        if (!required.empty()) throw ArgumentError(py::str("the following arguments are required: ") + py::join(py::str(", "), required));
    } catch (const ArgumentError& err) {
        parser_error(prog, err.text());
    }
    if (!extras.empty()) parser_error(prog, py::str("unrecognized arguments: ") + py::join(py::str(" "), extras));
    return ns;
}

}  // namespace

void main(const py::list<py::str>& argv) {
    const py::str prog = py::os::path::basename(argv[0]);
    // ArgumentParser(description="A script to make embedding binaries in code a breeze")
    Namespace args = parse_args(argv.slice(1, py::len(argv)), prog);

    if (!py::os::path::isfile(args.input)) {
        throw py::AssertionError("The specified input file doesn't exist");
    }

    Language lang;
    if (args.language == "python" || args.language == "py") {
        lang = Language::PYTHON;
    } else if (args.language == "c") {
        lang = Language::C;
    } else if (args.language == "c++" || args.language == "cpp" || args.language == "cplusplus") {
        lang = Language::CPLUSPLUS;
    } else if (args.language == "csharp" || args.language == "cs" || args.language == "c#") {
        lang = Language::CSHARP;
    } else if (args.language == "php" || args.language == "php-new") {
        lang = Language::PHP;
    } else if (args.language == "php-old") {
        lang = Language::PHP_OLD;
    } else {
        throw py::Exception("Invalid language specified!");
    }

    if (args.bytes_huge) {
        // read(n) of an n beyond ssize_t
        py::open_rb(args.input).close();
        throw py::OverflowError("Python int too large to convert to C ssize_t");
    }
    print_to(*out_stream_, lang_format(args.input, args.output, lang, args.variable, args.bytes));
}

}  // namespace codebreaker::formats::xbox::crypto::bin2lang
