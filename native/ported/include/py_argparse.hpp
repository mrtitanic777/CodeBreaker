// py_argparse.hpp -- a small argparse.ArgumentParser (Python 3.14) for command-line scripts (the Xbox 360 crypto tools
// sd_signer, xekeys_signer, exp_signer, shadowboot use it).
//
// It reads a command line the way argparse does for parsers made of: one-argument options (-o X, --opt X,
// --opt=X, -oX, abbreviated long options), flags (store_true), positionals with nargs=None, and -h/--help.
// The usage line, the help text and the errors are formatted as argparse formats them at its default width
// (80 columns less 2; COLUMNS is not read). A parser error prints the usage and "prog: error: message" to
// the error stream and raises SystemExit(2); -h prints the help to the output stream and raises SystemExit(0).
//
// What it does not do: nargs other than one, subparsers, choices in the usage ({a,b}), count / append / store_const.
#pragma once

#include <functional>
#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include "py.hpp"

namespace py {
namespace argparse {

// argparse.ArgumentError: the message of "argument -b/--bytes: expected one argument"
struct ArgumentError : py::Exception {
    ArgumentError(const py::str& argument, const py::str& message)
        : py::Exception(message.utf8()), argument_(argument), msg_(message) {}
    explicit ArgumentError(const py::str& message) : py::Exception(message.utf8()), msg_(message) {}
    const char* type() const override { return "ArgumentError"; }
    py::str text() const { return argument_.empty() ? msg_ : py::str("argument ") + argument_ + py::str(": ") + msg_; }

private:
    py::str argument_;
    py::str msg_;
};

// What the arguments leave in the Namespace: a str (or None) for a positional / one-argument option, a bool for a flag.
class Namespace {
public:
    std::optional<py::str> get(const std::string& dest) const {
        auto it = values_.find(dest);
        return it == values_.end() ? std::nullopt : it->second;
    }
    bool flag(const std::string& dest) const {
        auto it = flags_.find(dest);
        return it != flags_.end() && it->second;
    }
    void set(const std::string& dest, const py::str& v) { values_[dest] = v; }
    void set_default(const std::string& dest, const std::optional<py::str>& v) { values_[dest] = v; }
    void set_flag(const std::string& dest, bool v) { flags_[dest] = v; }

private:
    std::map<std::string, std::optional<py::str>> values_;
    std::map<std::string, bool> flags_;
};

class ArgumentParser {
public:
    enum class Kind { HELP, STORE, STORE_TRUE, POSITIONAL };
    // the conversion of a string (argparse's `type=`); may throw ArgumentError (or exit through error())
    using Type = std::function<py::str(const py::str&)>;

    ArgumentParser(const py::str& prog, const py::str& description, std::ostream& out, std::ostream& err)
        : prog_(prog), description_(description), out_(&out), err_(&err) {
        Action h;
        h.kind = Kind::HELP;
        h.dest = "help";
        h.option_strings = {py::str("-h"), py::str("--help")};
        h.help = py::str("show this help message and exit");
        actions_.push_back(h);
    }

    // add_argument("input", help=...)
    void add_positional(const std::string& dest, const py::str& help, const Type& type = Type()) {
        Action a;
        a.kind = Kind::POSITIONAL;
        a.dest = dest;
        a.metavar = py::str(dest);
        a.help = help;
        a.type = type;
        actions_.push_back(a);
    }
    // add_argument("-o", "--ofile", help=..., default=...)   (a one-argument option)
    void add_option(const std::vector<py::str>& flags, const py::str& help, const std::optional<py::str>& default_value = std::nullopt,
                    const Type& type = Type()) {
        Action a = option_action(flags, help);
        a.kind = Kind::STORE;
        a.default_value = default_value;
        a.type = type;
        actions_.push_back(a);
    }
    // add_argument("--encrypt", action="store_true", help=...)
    void add_flag(const std::vector<py::str>& flags, const py::str& help) {
        Action a = option_action(flags, help);
        a.kind = Kind::STORE_TRUE;
        actions_.push_back(a);
    }

    // parser.error(message)
    [[noreturn]] void error(const py::str& message) const {
        *err_ << format_usage().utf8();
        *err_ << prog_.utf8() << ": error: " << message.utf8() << "\n";
        throw py::SystemExit(2);
    }

    // parse_args(args): the Namespace, or the SystemExit of an error / -h
    Namespace parse_args(const py::list<py::str>& arg_strings) const;

    // format_usage(): "usage: ...\n"
    py::str format_usage() const { return usage_line() + py::str("\n"); }
    py::str format_help() const;

private:
    struct Action {
        Kind kind = Kind::STORE;
        std::string dest;
        std::vector<py::str> option_strings;   // none for a positional
        py::str metavar;
        py::str help;
        std::optional<py::str> default_value;
        Type type;
    };

    py::str prog_;
    py::str description_;
    std::ostream* out_;
    std::ostream* err_;
    std::vector<Action> actions_;

    static constexpr int64_t kWidth = 80 - 2;   // HelpFormatter's width: the columns (80) less 2

    static Action option_action(const std::vector<py::str>& flags, const py::str& help) {
        Action a;
        a.option_strings = flags;
        a.help = help;
        // dest: the first long option (else the first option) without its dashes, '-' as '_'
        py::str chosen = flags[0];
        for (const py::str& f : flags) {
            if (py::len(f) > 1 && f.at_(1) == U'-') {
                chosen = f;
                break;
            }
        }
        std::string d = chosen.utf8();
        size_t k = d.find_first_not_of('-');
        d = d.substr(k == std::string::npos ? d.size() : k);
        for (char& c : d)
            if (c == '-') c = '_';
        a.dest = d;
        std::string up = d;
        for (char& c : up)
            if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
        a.metavar = py::str(up);
        return a;
    }

    // "-o, --ofile OFILE" as the help lists an action
    static py::str invocation(const Action& a) {
        if (a.kind == Kind::POSITIONAL) return a.metavar;
        py::list<py::str> flags;
        for (const py::str& f : a.option_strings) flags.append(f);
        py::str s = py::join(py::str(", "), flags);
        if (a.kind == Kind::STORE) s += py::str(" ") + a.metavar;
        return s;
    }

    py::str usage_line() const;

    // textwrap.wrap(text, width): the words separated by blanks, a line filled as far as it goes
    static py::list<py::str> wrap(const py::str& text, int64_t width) {
        py::list<py::str> lines;
        std::string cur;
        int64_t cur_len = 0;
        std::string word;
        std::vector<std::string> words;
        for (char c : text.utf8()) {
            if (c == ' ' || c == '\n' || c == '\t') {
                if (!word.empty()) words.push_back(word);
                word.clear();
            } else {
                word.push_back(c);
            }
        }
        if (!word.empty()) words.push_back(word);
        for (const std::string& w : words) {
            int64_t wl = py::len(py::str(w));
            if (cur_len > 0 && cur_len + 1 + wl > width) {
                lines.append(py::str(cur));
                cur.clear();
                cur_len = 0;
            }
            if (cur_len > 0) {
                cur += " ";
                cur_len += 1;
            }
            cur += w;
            cur_len += wl;
        }
        if (cur_len > 0) lines.append(py::str(cur));
        return lines;
    }
};

// ---- usage -----------------------------------------------------------------------------------------------
inline py::str ArgumentParser::usage_line() const {
    const py::str prefix("usage: ");
    py::list<py::str> opt_parts;
    py::list<py::str> pos_parts;
    for (const Action& a : actions_) {
        if (a.kind == Kind::POSITIONAL) pos_parts.append(a.metavar);
        else if (a.kind == Kind::STORE) opt_parts.append(py::str("[") + a.option_strings[0] + py::str(" ") + a.metavar + py::str("]"));
        else opt_parts.append(py::str("[") + a.option_strings[0] + py::str("]"));
    }
    py::list<py::str> all;
    all.extend(opt_parts);
    all.extend(pos_parts);
    py::str usage = (prog_.empty() ? py::str("") : prog_ + py::str(" ")) + py::join(py::str(" "), all);
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
        const int64_t prog_len = py::len(prog_);
        if (static_cast<double>(py::len(prefix) + prog_len) <= 0.75 * static_cast<double>(text_width)) {
            // if prog is short, follow it with optionals or positionals
            const py::str indent(std::u32string(static_cast<size_t>(py::len(prefix) + prog_len + 1), U' '));
            py::list<py::str> first = {prog_};
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
            lines = py::list<py::str>{prog_} + lines;
        }
        usage = py::join(py::str("\n"), lines);
    }
    return prefix + usage;
}

// ---- help ------------------------------------------------------------------------------------------------
inline py::str ArgumentParser::format_help() const {
    int64_t action_max_length = 0;
    for (const Action& a : actions_) action_max_length = std::max(action_max_length, py::len(invocation(a)) + 2);
    const int64_t max_help_position = std::min<int64_t>(24, std::max<int64_t>(kWidth - 20, 4));
    const int64_t help_position = std::min(action_max_length + 2, max_help_position);
    const int64_t help_width = std::max<int64_t>(kWidth - help_position, 11);
    const int64_t action_width = help_position - 2 - 2;

    auto section = [&](bool positional) {
        std::string out;
        for (const Action& a : actions_) {
            if ((a.kind == Kind::POSITIONAL) != positional) continue;
            const py::str inv = invocation(a);
            py::list<py::str> help_lines = wrap(a.help, help_width);
            std::string line = "  ";
            if (help_lines.empty()) {
                out += line + inv.utf8() + "\n";
                continue;
            }
            if (py::len(inv) <= action_width) {
                line += inv.utf8() + std::string(static_cast<size_t>(action_width - py::len(inv)), ' ') + "  " + help_lines[0].utf8() + "\n";
            } else {
                line += inv.utf8() + "\n" + std::string(static_cast<size_t>(help_position), ' ') + help_lines[0].utf8() + "\n";
            }
            out += line;
            for (int64_t i = 1; i < py::len(help_lines); ++i)
                out += std::string(static_cast<size_t>(help_position), ' ') + help_lines[i].utf8() + "\n";
        }
        return out;
    };

    std::string text = usage_line().utf8() + "\n\n";
    if (!description_.empty()) {
        for (const py::str& l : wrap(description_, kWidth)) text += l.utf8() + "\n";
        text += "\n";
    }
    bool has_pos = false;
    for (const Action& a : actions_) has_pos = has_pos || a.kind == Kind::POSITIONAL;
    if (has_pos) text += "positional arguments:\n" + section(true) + "\n";
    text += "options:\n" + section(false);
    return py::str(text);
}

// ---- parse_args ------------------------------------------------------------------------------------------
inline Namespace ArgumentParser::parse_args(const py::list<py::str>& arg_strings) const {
    struct OptionTuple {
        const Action* action;   // null: no such option
        py::str option_string;
        std::optional<py::str> sep;
        std::optional<py::str> explicit_arg;
    };
    // the parser's option strings in the order argparse keeps them (_option_string_actions)
    std::vector<std::pair<py::str, const Action*>> option_map;
    for (const Action& a : actions_)
        for (const py::str& s : a.option_strings) option_map.emplace_back(s, &a);
    auto lookup = [&](const py::str& s) -> const Action* {
        for (const auto& kv : option_map)
            if (kv.first == s) return kv.second;
        return nullptr;
    };
    auto action_name = [&](const Action& a) {
        if (a.kind == Kind::POSITIONAL) return a.metavar;
        py::str name;
        for (size_t i = 0; i < a.option_strings.size(); ++i) name += (i ? py::str("/") : py::str("")) + a.option_strings[i];
        return name;
    };
    auto fail = [&](const Action& a, const py::str& msg) { return ArgumentError(action_name(a), msg); };

    auto get_option_tuples = [&](const py::str& arg) {
        std::vector<OptionTuple> result;
        if (arg.at_(0) == U'-' && arg.at_(1) == U'-') {
            // option strings starting with two prefix characters are only split at the '='
            auto [option_prefix, sep, explicit_arg] = arg.partition(py::str("="));
            const bool has_sep = !sep.empty();
            for (const auto& kv : option_map) {
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
            for (const auto& kv : option_map) {
                if (kv.first == short_option_prefix) {
                    result.push_back({kv.second, kv.first, py::str(""), short_explicit_arg});
                } else if (kv.first.startswith(option_prefix)) {
                    result.push_back({kv.second, kv.first, has_sep ? std::optional<py::str>(sep) : std::nullopt,
                                      has_sep ? std::optional<py::str>(explicit_arg) : std::nullopt});
                }
            }
        }
        return result;
    };

    // None: a positional; else the interpretations of the option string
    auto parse_optional = [&](const py::str& arg) -> std::optional<std::vector<OptionTuple>> {
        if (arg.empty()) return std::nullopt;
        if (arg.at_(0) != U'-') return std::nullopt;
        if (const Action* a = lookup(arg)) return std::vector<OptionTuple>{{a, arg, std::nullopt, std::nullopt}};
        if (py::len(arg) == 1) return std::nullopt;
        {
            auto [option_string, sep, explicit_arg] = arg.partition(py::str("="));
            if (!sep.empty()) {
                if (const Action* a = lookup(option_string)) return std::vector<OptionTuple>{{a, option_string, sep, explicit_arg}};
            }
        }
        std::vector<OptionTuple> option_tuples = get_option_tuples(arg);
        if (!option_tuples.empty()) return option_tuples;
        // if it looks like a negative number, it was meant to be positional
        if (py::re::match(py::str("-\\.?\\d"), arg)) return std::nullopt;
        // if it contains a space, it was meant to be a positional
        if (arg.contains(py::str(" "))) return std::nullopt;
        // it was meant to be an optional but there is no such option in this parser
        return std::vector<OptionTuple>{{nullptr, arg, std::nullopt, std::nullopt}};
    };

    Namespace ns;
    for (const Action& a : actions_) {
        if (a.kind == Kind::STORE) ns.set_default(a.dest, a.default_value);
        else if (a.kind == Kind::STORE_TRUE) ns.set_flag(a.dest, false);
        else if (a.kind == Kind::POSITIONAL) ns.set_default(a.dest, std::nullopt);
    }
    py::list<py::str> extras;

    auto take_action = [&](const Action& a, const py::list<py::str>& args) {
        if (a.kind == Kind::HELP) {
            *out_ << format_help().utf8();
            throw py::SystemExit(0);
        }
        if (a.kind == Kind::STORE_TRUE) {
            ns.set_flag(a.dest, true);
            return;
        }
        ns.set(a.dest, a.type ? a.type(args[0]) : args[0]);
    };

    try {
        // find all option indices, and determine the arg_string_pattern: an 'O' for an option, an 'A' for an
        // argument, a '-' for a '--'
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
        std::vector<const Action*> seen;

        // the positionals left to be parsed
        std::vector<const Action*> positionals;
        for (const Action& a : actions_)
            if (a.kind == Kind::POSITIONAL) positionals.push_back(&a);

        auto match_argument = [&](const Action& a, const py::str& patt) -> int64_t {
            if (a.kind == Kind::HELP || a.kind == Kind::STORE_TRUE) return 0;   // nargs=0: '([AO]{0})'
            if (patt.empty() || patt.at_(0) != U'A') throw fail(a, py::str("expected one argument"));
            return 1;
        };

        auto consume_optional = [&](int64_t start_index) -> int64_t {
            const std::vector<OptionTuple>& option_tuples = *option_at(start_index);
            if (option_tuples.size() > 1) {
                py::list<py::str> names;
                for (const auto& t : option_tuples) names.append(t.option_string);
                throw ArgumentError(py::str("ambiguous option: ") + arg_strings[start_index] + py::str(" could match ") +
                                    py::join(py::str(", "), names));
            }
            const Action* action = option_tuples[0].action;
            py::str option_string = option_tuples[0].option_string;
            std::optional<py::str> sep = option_tuples[0].sep;
            std::optional<py::str> explicit_arg = option_tuples[0].explicit_arg;

            // identify additional optionals in the same arg string (e.g. -xyz is -x -y -z if no args are required)
            struct Taken {
                const Action* action;
                py::list<py::str> args;
            };
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
                            throw fail(*action, py::str("ignored explicit argument ") + py::repr(*explicit_arg));
                        }
                        action_tuples.push_back({action, py::list<py::str>()});
                        const py::str ch = option_string.slice(0, 1);
                        option_string = ch + explicit_arg->slice(0, 1);
                        if (const Action* next = lookup(option_string)) {
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
                        action_tuples.push_back({action, py::list<py::str>{*explicit_arg}});
                        break;
                    } else {
                        // error if a double-dash option did not use the explicit argument
                        throw fail(*action, py::str("ignored explicit argument ") + py::repr(*explicit_arg));
                    }
                } else {
                    // if there is no explicit argument, try to match the optional's string arguments with
                    // the following strings
                    const int64_t start = start_index + 1;
                    const int64_t arg_count = match_argument(*action, pattern_from(start));
                    stop = start + arg_count;
                    action_tuples.push_back({action, arg_strings.slice(start, stop)});
                    break;
                }
            }
            for (const Taken& t : action_tuples) {
                seen.push_back(t.action);
                take_action(*t.action, t.args);
            }
            return stop;
        };

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
                seen.push_back(positionals[k]);
                take_action(*positionals[k], args);
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
                // only try to parse the next optional if we didn't consume the option string during the
                // positionals parsing
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
        for (const Action& a : actions_) {
            if (a.kind != Kind::POSITIONAL) continue;
            bool got = false;
            for (const Action* s : seen) got = got || s == &a;
            if (!got) required.append(a.metavar);
        }
        if (!required.empty()) throw ArgumentError(py::str("the following arguments are required: ") + py::join(py::str(", "), required));
    } catch (const ArgumentError& err) {
        error(err.text());
    }
    if (!extras.empty()) error(py::str("unrecognized arguments: ") + py::join(py::str(" "), extras));
    return ns;
}

}  // namespace argparse
}  // namespace py
