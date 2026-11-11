// codebreaker/java/run.cpp -- typed C++ from codebreaker.java.run (see run.hpp).
#include "codebreaker/java/run.hpp"

// (transpile.hpp first: translate/ir.hpp names a constant VOID, which <windows.h> (py_subprocess.hpp) defines as a macro)
#include "codebreaker/java/transpile.hpp"

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "codebreaker/decompile/jar.hpp"
#include "codebreaker/java/analyze.hpp"
#include "codebreaker/java/apimap.hpp"
#include "codebreaker/java/bugfix_ledger.hpp"
#include "codebreaker/java/console/assets.hpp"
#include "codebreaker/java/console/platform_config.hpp"
#include "codebreaker/java/console/project_layout.hpp"
#include "codebreaker/java/console/project_scan.hpp"
#include "codebreaker/java/console/sdk.hpp"
#include "codebreaker/java/console/vcxproj.hpp"
#include "codebreaker/java/console/worklist.hpp"
#include "codebreaker/java/console/xact.hpp"
#include "codebreaker/java/debug.hpp"
#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/profiles/package.hpp"
#include "codebreaker/java/projects/minecraft/achievements.hpp"
#include "codebreaker/java/projects/minecraft/applet_ping.hpp"
#include "codebreaker/java/projects/minecraft/controller_nav.hpp"
#include "codebreaker/java/projects/minecraft/edition.hpp"
#include "codebreaker/java/projects/minecraft/keybind_mark.hpp"
#include "codebreaker/java/projects/minecraft/render_fix.hpp"
#include "codebreaker/java/projects/minecraft/sound_fix.hpp"
#include "codebreaker/java/projects/minecraft/ui_mark.hpp"
#include "codebreaker/java/reflection_factory.hpp"
#include "codebreaker/java/report.hpp"
#include "codebreaker/java/scaffold.hpp"
#include "codebreaker/java/signatures.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::java::run {

namespace console = codebreaker::java::console;
namespace mc = codebreaker::java::projects::minecraft;

namespace {

// ============================================================================================ argparse
enum class Kind { HELP, STORE, STORE_TRUE, APPEND, PARSER };

struct ActionSpec {
    Kind kind;
    std::string dest;
    std::vector<py::str> opts;
    char nargs;              // 'N' one argument, '?' at most one, '0' none, 'P' a parser name and the rest
    char type;               // 's' text, 'f' float
    std::vector<py::str> choices;
    std::optional<py::str> dflt;
    bool required;
    std::optional<py::str> metavar;
};

struct ParserSpec {
    std::string name, prog, usage, help;
    std::vector<ActionSpec> actions;
};

#include "codebreaker/java/run_parser.inc"

// what the arguments leave in the Namespace
struct Value {
    std::optional<py::str> s;
    std::optional<double> f;
    bool b = false;
    std::vector<py::str> list;
    bool has_list = false;
};

class Namespace {
  public:
    std::map<std::string, Value> v;
    std::optional<py::str> str(const std::string& k) const {
        auto it = v.find(k);
        return it == v.end() ? std::nullopt : it->second.s;
    }
    bool flag(const std::string& k) const {
        auto it = v.find(k);
        return it != v.end() && it->second.b;
    }
    std::optional<double> num(const std::string& k) const {
        auto it = v.find(k);
        return it == v.end() ? std::nullopt : it->second.f;
    }
    std::vector<py::str> list(const std::string& k) const {
        auto it = v.find(k);
        return it == v.end() ? std::vector<py::str>() : it->second.list;
    }
    bool truthy(const std::string& k) const {
        auto s = str(k);
        return s && !s->empty();
    }
};

struct ArgError {
    std::optional<std::string> argument;
    py::str message;
    py::str text() const { return argument ? py::str("argument " + *argument + ": ") + message : message; }
};

std::string action_name(const ActionSpec& a) {
    if (!a.opts.empty()) {
        std::string s;
        for (size_t i = 0; i < a.opts.size(); ++i) s += (i ? "/" : "") + a.opts[i].utf8();
        return s;
    }
    if (a.metavar) return a.metavar->utf8();
    return a.dest;
}

void write_err_(const std::string& s) {
    std::fflush(stdout);
    std::fwrite(s.data(), 1, s.size(), stderr);
    std::fflush(stderr);
}

[[noreturn]] void parser_error(const ParserSpec& ps, const py::str& message) {
    write_err_(ps.usage);
    write_err_(ps.prog + ": error: " + message.utf8() + "\n");
    throw py::SystemExit(2);
}

std::string nargs_pattern(const ActionSpec& a) {
    bool option = !a.opts.empty();
    switch (a.nargs) {
        case 'N': return option ? "([A])" : "(-*A-*)";
        case '?': return option ? "(A?)" : "(-*A?-*)";
        case '0': return option ? "([AO]{0})" : "((?:-*A){0}-*)";
        case 'P': return option ? "(A[AO]*)" : "(-*A[-AO]*)";
    }
    return "()";
}

struct OptTuple {
    const ActionSpec* action;      // null: not an option of this parser
    std::string option_string;
    std::optional<std::string> sep, explicit_arg;
};

class Parser {
  public:
    const ParserSpec& spec;
    std::vector<const ActionSpec*> option_actions;   // (in the order argparse registers the option strings)
    explicit Parser(const ParserSpec& s) : spec(s) {
        for (const ActionSpec& a : spec.actions)
            for (const py::str& o : a.opts) option_strings.emplace_back(o.utf8(), &a);
    }

    // parse_known_args: -> the extras
    std::vector<std::string> parse_known(const std::vector<std::string>& args, Namespace& ns) const {
        // defaults of the actions
        for (const ActionSpec& a : spec.actions) {
            if (a.kind == Kind::HELP) continue;
            Value& val = ns.v[a.dest];
            if (a.kind == Kind::STORE_TRUE) val.b = false;
            else if (a.kind == Kind::APPEND) { val.has_list = false; }
            else if (a.dflt) val.s = a.dflt;
        }
        try {
            return parse_known_inner(args, ns);
        } catch (const ArgError& e) {
            parser_error(spec, e.text());
        }
    }

    // parse_args
    void parse_args(const std::vector<std::string>& args, Namespace& ns) const {
        std::vector<std::string> extras = parse_known(args, ns);
        if (!extras.empty()) {
            std::string m = "unrecognized arguments: ";
            for (size_t i = 0; i < extras.size(); ++i) m += (i ? " " : "") + extras[i];
            parser_error(spec, py::str(m));
        }
    }

  private:
    std::vector<std::pair<std::string, const ActionSpec*>> option_strings;

    const ActionSpec* find_option(const std::string& s) const {
        for (auto& p : option_strings)
            if (p.first == s) return p.second;
        return nullptr;
    }

    // None (a positional) or the option tuples
    std::optional<std::vector<OptTuple>> parse_optional(const std::string& arg) const {
        if (arg.empty()) return std::nullopt;
        if (arg[0] != '-') return std::nullopt;
        if (const ActionSpec* a = find_option(arg)) return std::vector<OptTuple>{{a, arg, std::nullopt, std::nullopt}};
        if (py::len(py::str(arg)) == 1) return std::nullopt;
        size_t eq = arg.find('=');
        if (eq != std::string::npos) {
            std::string os = arg.substr(0, eq);
            if (const ActionSpec* a = find_option(os)) return std::vector<OptTuple>{{a, os, std::string("="), arg.substr(eq + 1)}};
        }
        std::vector<OptTuple> tuples = option_tuples(arg);
        if (!tuples.empty()) return tuples;
        // a negative number is a positional (no option here looks like one)
        py::re::Pattern neg = py::re::compile("^-\\d+$|^-\\d*\\.\\d+$");
        if (neg.match(py::str(arg))) return std::nullopt;
        if (arg.find(' ') != std::string::npos) return std::nullopt;
        return std::vector<OptTuple>{{nullptr, arg, std::nullopt, std::nullopt}};
    }

    std::vector<OptTuple> option_tuples(const std::string& option_string) const {
        std::vector<OptTuple> result;
        py::str os(option_string);
        py::str c0 = os.slice(0, 1), c1 = os.slice(1, 2);
        if (c0 == py::str("-") && c1 == py::str("-")) {
            std::string prefix = option_string;
            std::optional<std::string> sep, explicit_arg;
            size_t eq = option_string.find('=');
            if (eq != std::string::npos) {
                prefix = option_string.substr(0, eq);
                sep = "=";
                explicit_arg = option_string.substr(eq + 1);
            }
            for (auto& p : option_strings)
                if (p.first.compare(0, prefix.size(), prefix) == 0) result.push_back({p.second, p.first, sep, explicit_arg});
        } else if (c0 == py::str("-") && c1 != py::str("-")) {
            std::string prefix = option_string;
            std::optional<std::string> sep, explicit_arg;
            size_t eq = option_string.find('=');
            if (eq != std::string::npos) {
                prefix = option_string.substr(0, eq);
                sep = "=";
                explicit_arg = option_string.substr(eq + 1);
            }
            std::string short_prefix = os.slice(0, 2).utf8(), short_explicit = os.slice_from(2).utf8();
            for (auto& p : option_strings) {
                if (p.first == short_prefix) result.push_back({p.second, p.first, std::string(""), short_explicit});
                else if (p.first.compare(0, prefix.size(), prefix) == 0) result.push_back({p.second, p.first, sep, explicit_arg});
            }
        } else {
            throw ArgError{std::nullopt, py::str("unexpected option string: " + option_string)};
        }
        return result;
    }

    // the number of argument strings the action takes from the pattern
    int64_t match_argument(const ActionSpec& a, const std::string& pattern) const {
        py::re::Pattern p = py::re::compile(nargs_pattern(a));
        auto m = p.match(py::str(pattern));
        if (!m) {
            const char* msg = a.nargs == 'N' ? "expected one argument" : a.nargs == '?' ? "expected at most one argument" : nullptr;
            if (!msg) msg = "expected 0 arguments";   // (nargs 0 always matches)
            throw ArgError{action_name(a), py::str(msg)};
        }
        return py::len(m->group(1));
    }

    std::vector<int64_t> match_partial(const std::vector<const ActionSpec*>& actions, const std::string& pattern) const {
        for (int64_t i = static_cast<int64_t>(actions.size()); i > 0; --i) {
            std::string joined;
            for (int64_t k = 0; k < i; ++k) joined += nargs_pattern(*actions[k]);
            auto m = py::re::compile(joined).match(py::str(pattern));
            if (m) {
                std::vector<int64_t> result;
                for (int64_t g = 1; g <= i; ++g) result.push_back(py::len(m->group(g)));
                if (m->end() < static_cast<int64_t>(pattern.size()) && pattern[static_cast<size_t>(m->end())] == 'O') {
                    while (!result.empty() && !result.back()) result.pop_back();
                }
                return result;
            }
        }
        return {};
    }

    // the converted value of one argument string (a float, or the text), checked against the choices
    void convert(const ActionSpec& a, const std::string& s, Value& out, bool check = true) const {
        if (a.type == 'f') {
            try {
                out.f = py::float_from_str(py::str(s));
            } catch (const py::ValueError&) {
                throw ArgError{action_name(a), py::str("invalid float value: " + py::repr(py::str(s)).utf8())};
            }
            out.s = py::str(s);
        } else {
            out.s = py::str(s);
        }
        if (check && !a.choices.empty()) {
            bool ok = false;
            for (const py::str& c : a.choices)
                if (c == py::str(s)) ok = true;
            if (!ok) {
                std::string list;
                for (size_t i = 0; i < a.choices.size(); ++i) list += (i ? ", " : "") + a.choices[i].utf8();
                throw ArgError{action_name(a), py::str("invalid choice: " + py::repr(py::str(s)).utf8() + " (choose from " + list + ")")};
            }
        }
    }

    void take_action(const ActionSpec& a, const std::vector<std::string>& strings, Namespace& ns, std::set<const ActionSpec*>& seen) const {
        seen.insert(&a);
        if (a.kind == Kind::HELP) {
            std::fwrite(spec.help.data(), 1, spec.help.size(), stdout);
            std::fflush(stdout);
            throw py::SystemExit(0);
        }
        Value& val = ns.v[a.dest];
        if (a.kind == Kind::STORE_TRUE) {
            val.b = true;
            return;
        }
        if (a.kind == Kind::PARSER) {
            // values[0] is checked against the choices; the rest goes to the subparser
            Value tmp;
            convert(a, strings.at(0), tmp);
            val.s = py::str(strings[0]);
            std::vector<std::string> rest(strings.begin() + 1, strings.end());
            const ParserSpec* sub = nullptr;
            for (const ParserSpec& p : PARSERS())
                if (p.name == strings[0]) sub = &p;
            Parser sp(*sub);
            Namespace subns;
            std::vector<std::string> extras = sp.parse_known(rest, subns);
            for (auto& kv : subns.v) ns.v[kv.first] = kv.second;
            unrecognized.insert(unrecognized.end(), extras.begin(), extras.end());
            return;
        }
        // STORE / APPEND (one string, or none for nargs '?': the default)
        if (strings.empty()) {
            // (a '?' positional with no string: its default, converted when it is a text)
            val.s = a.dflt;
            return;
        }
        Value conv;
        convert(a, strings[0], conv);
        if (a.kind == Kind::APPEND) {
            val.has_list = true;
            val.list.push_back(*conv.s);
        } else {
            val.s = conv.s;
            val.f = conv.f;
        }
    }

    std::vector<std::string> parse_known_inner(const std::vector<std::string>& arg_strings, Namespace& ns) const {
        std::map<int64_t, std::vector<OptTuple>> option_string_indices;
        std::string pattern;
        for (size_t i = 0; i < arg_strings.size(); ++i) {
            if (arg_strings[i] == "--") {
                pattern += '-';
                for (++i; i < arg_strings.size(); ++i) pattern += 'A';
                break;
            }
            auto ot = parse_optional(arg_strings[i]);
            if (!ot) pattern += 'A';
            else {
                option_string_indices[static_cast<int64_t>(i)] = *ot;
                pattern += 'O';
            }
        }
        std::set<const ActionSpec*> seen;
        std::vector<std::string> extras;
        std::vector<const ActionSpec*> positionals;
        for (const ActionSpec& a : spec.actions)
            if (a.opts.empty()) positionals.push_back(&a);

        auto slice = [&](int64_t from, int64_t to) { return std::vector<std::string>(arg_strings.begin() + from, arg_strings.begin() + to); };

        auto consume_optional = [&](int64_t start_index) -> int64_t {
            const std::vector<OptTuple>& tuples = option_string_indices.at(start_index);
            if (tuples.size() > 1) {
                std::string options;
                for (size_t i = 0; i < tuples.size(); ++i) options += (i ? ", " : "") + tuples[i].option_string;
                throw ArgError{std::nullopt, py::str("ambiguous option: " + arg_strings[static_cast<size_t>(start_index)] + " could match " + options)};
            }
            const OptTuple& t = tuples[0];
            if (!t.action) {
                extras.push_back(arg_strings[static_cast<size_t>(start_index)]);
                return start_index + 1;
            }
            const ActionSpec& a = *t.action;
            int64_t stop;
            std::vector<std::string> args;
            if (t.explicit_arg) {
                int64_t arg_count = match_argument(a, "A");
                if (arg_count == 0 && t.option_string.size() > 1 && t.option_string[1] != '-' && !t.explicit_arg->empty()) {
                    // (a single-dash flag followed by more characters: no flag here has one)
                    throw ArgError{action_name(a), py::str("ignored explicit argument " + py::repr(py::str(*t.explicit_arg)).utf8())};
                } else if (arg_count == 1) {
                    stop = start_index + 1;
                    args = {*t.explicit_arg};
                } else {
                    throw ArgError{action_name(a), py::str("ignored explicit argument " + py::repr(py::str(*t.explicit_arg)).utf8())};
                }
            } else {
                int64_t start = start_index + 1;
                int64_t arg_count = match_argument(a, pattern.substr(static_cast<size_t>(start)));
                stop = start + arg_count;
                args = slice(start, stop);
            }
            take_action(a, args, ns, seen);
            return stop;
        };

        auto consume_positionals = [&](int64_t start_index) -> int64_t {
            std::vector<int64_t> arg_counts = match_partial(positionals, pattern.substr(static_cast<size_t>(start_index)));
            for (size_t i = 0; i < arg_counts.size(); ++i) {
                const ActionSpec& a = *positionals[i];
                int64_t n = arg_counts[i];
                std::vector<std::string> args = slice(start_index, start_index + n);
                if (a.nargs == 'P') {
                    if (pattern[static_cast<size_t>(start_index)] == '-') args.erase(args.begin());
                } else if (pattern.find('-', static_cast<size_t>(start_index)) < static_cast<size_t>(start_index + n)) {
                    args.erase(std::find(args.begin(), args.end(), std::string("--")));
                }
                start_index += n;
                take_action(a, args, ns, seen);
            }
            positionals.erase(positionals.begin(), positionals.begin() + static_cast<int64_t>(arg_counts.size()));
            return start_index;
        };

        int64_t start_index = 0;
        int64_t max_option_string_index = option_string_indices.empty() ? -1 : option_string_indices.rbegin()->first;
        while (start_index <= max_option_string_index) {
            int64_t next_option_string_index = start_index;
            while (next_option_string_index <= max_option_string_index) {
                if (option_string_indices.count(next_option_string_index)) break;
                ++next_option_string_index;
            }
            if (start_index != next_option_string_index) {
                int64_t positionals_end_index = consume_positionals(start_index);
                if (positionals_end_index > start_index) {
                    start_index = positionals_end_index;
                    continue;
                }
                start_index = positionals_end_index;
            }
            if (!option_string_indices.count(start_index)) {
                std::vector<std::string> strings = slice(start_index, next_option_string_index);
                extras.insert(extras.end(), strings.begin(), strings.end());
                start_index = next_option_string_index;
            }
            start_index = consume_optional(start_index);
        }
        int64_t stop_index = consume_positionals(start_index);
        for (int64_t i = stop_index; i < static_cast<int64_t>(arg_strings.size()); ++i) extras.push_back(arg_strings[static_cast<size_t>(i)]);

        std::string required;
        for (const ActionSpec& a : spec.actions) {
            if (!seen.count(&a) && a.required) required += (required.empty() ? "" : ", ") + action_name(a);
        }
        if (!required.empty()) throw ArgError{std::nullopt, py::str("the following arguments are required: " + required)};
        // what the subparser did not recognise
        extras.insert(extras.end(), unrecognized.begin(), unrecognized.end());
        unrecognized.clear();
        return extras;
    }

    mutable std::vector<std::string> unrecognized;
};

// ============================================================================================ helpers
std::string fmt(const char* f, ...) {
    va_list ap;
    va_start(ap, f);
    char buf[4096];
    int n = std::vsnprintf(buf, sizeof buf, f, ap);
    va_end(ap);
    if (n >= static_cast<int>(sizeof buf)) {
        std::string big(static_cast<size_t>(n) + 1, '\0');
        va_start(ap, f);
        std::vsnprintf(&big[0], big.size(), f, ap);
        va_end(ap);
        big.resize(static_cast<size_t>(n));
        return big;
    }
    return std::string(buf, static_cast<size_t>(n));
}

void say(const std::string& s) { py::print(py::str(s)); }

// '%-Ns' of a text (padded to N code points)
std::string pad(const py::str& s, int64_t width) {
    int64_t n = py::len(s);
    return s.utf8() + std::string(static_cast<size_t>(std::max<int64_t>(0, width - n)), ' ');
}

std::string join(const py::list<py::str>& xs, const char* sep = ", ") {
    std::string o;
    bool first = true;
    for (const py::str& x : xs) {
        if (!first) o += sep;
        o += x.utf8();
        first = false;
    }
    return o;
}

py::str need(const std::optional<py::str>& v, const char* what) {
    if (!v) throw py::TypeError(std::string("expected str, bytes or os.PathLike object, not NoneType (") + what + ")");
    return *v;
}

py::str basename(const py::str& s) { return py::os::path::basename(s); }

// settings.state(*parts): a path inside the state folder (its folder made)
py::str state(const std::vector<std::string>& parts) {
    py::str base = py::settings::get_str(py::str("state"));
    py::str path = base;
    for (const std::string& p : parts) path = py::os::path::join(path, py::str(p));
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

std::string truth(bool b) { return b ? "True" : "False"; }

// ============================================================================================ commands
int64_t cmd_xact(const Namespace& A) {
    py::str act = need(A.str("action"), "action");
    py::str a = need(A.str("a"), "a");
    std::optional<py::str> b = A.str("b"), c = A.str("c");
    bool run = A.flag("run");
    if (act == py::str("dump")) {
        if (!py::os::path::isfile(a)) {
            say("ERROR: not a file: " + a.utf8());
            return 2;
        }
        say(console::xact::dump(a).utf8());
        return 0;
    }
    if (act == py::str("build")) {
        std::optional<py::str> sdk_root;
        if (run) {
            auto info = console::sdk::detect(py::str("xbox360"));
            if (info) sdk_root = info->root;
        }
        auto [xap, cues] = console::xact::build(a, need(b, "out_dir"), std::nullopt, run, sdk_root, [](const py::str& s) { say(s.utf8()); });
        say(fmt("Wrote %s (%d cues) + build_banks.bat + convert_audio.bat to %s", xap.utf8().c_str(), static_cast<int>(cues.size()), b->utf8().c_str()));
        if (!run)
            say("Next: run convert_audio.bat (ffmpeg ogg->wav) then build_banks.bat (XDK XACTBLD) -- or pass --run to autorun both now.");
        return 0;
    }
    if (act == py::str("coverage")) {
        auto cov = console::xact::cue_coverage(a, need(b, "xsb"));
        say(fmt("needed=%d  authored=%d  MISSING=%d  unused=%d", static_cast<int>(cov.needed.size()), static_cast<int>(cov.authored.size()),
                static_cast<int>(cov.missing.size()), static_cast<int>(cov.unused.size())));
        for (const py::str& cue : cov.missing) {
            py::list<py::str> who = cov.needed.at(cue);
            say("  MISSING (silent): " + cue.utf8() + "  <- " + join(who.slice(0, 3)));
        }
        return 0;
    }
    if (act == py::str("match")) {
        py::str out = c && !c->empty() ? *c : py::str(".");
        auto m = console::xact::match_sources(a, need(b, "resources_dir"));
        py::str bat = console::xact::write_ffmpeg_script(m, out, *b);
        say(fmt("matched=%d  MISSING=%d  (index keys=%d)", static_cast<int>(m.matched.size()), static_cast<int>(m.missing.size()), static_cast<int>(m.index_keys)));
        py::list<py::str> keys;
        for (const auto& kv : m.missing.items()) keys.append(std::get<0>(kv));
        for (const py::str& cue : py::sorted(keys)) say("  no source ogg: " + cue.utf8() + "  <- " + join(m.missing.at(cue).slice(0, 3)));
        say("Wrote " + bat.utf8() + " (ffmpeg .ogg->.wav per matched cue).");
        return 0;
    }
    say("ERROR: unknown xact action " + py::repr(act).utf8() + " (dump/build/coverage/match)");
    return 2;
}

int64_t cmd_achievements(const Namespace& A) {
    py::str code_dir = need(A.str("code_dir"), "code_dir");
    if (!py::os::path::isdir(code_dir)) {
        say("ERROR: not a directory: " + code_dir.utf8());
        return 2;
    }
    auto res = mc::achievements::run(code_dir, need(A.str("out_dir"), "out_dir"), A.str("lang"), *A.str("profile"), *A.str("delivery"), A.str("content_dir"),
                                     A.str("title_id"), A.str("title_name"));
    const auto& m = res.mapping;
    say(fmt("Found %d achievement(s) -> %s profile, %d gamerscore (limit %d)", static_cast<int>(res.count), m.profile.utf8().c_str(),
            static_cast<int>(m.total_score), static_cast<int>(m.limit)));
    for (const py::str& w : m.warnings) say("  WARNING: " + w.utf8());
    const auto& ic = res.icons;
    say(fmt("  icons: %d auto-wired, %d need manual art%s", static_cast<int>(ic.wired.size()), static_cast<int>(ic.unresolved.size()),
            (res.content_dir && !res.content_dir->empty() ? "   (atlases: " + res.content_dir->utf8() + ")" : std::string()).c_str()));
    int64_t shown = 0;
    for (const auto& u : ic.unresolved) {
        if (shown++ >= 6) break;
        say("    - " + std::get<0>(u).utf8() + ": " + std::get<1>(u).utf8());
    }
    if (ic.unresolved.size() > 6) say(fmt("    - ... and %d more (see ACHIEVEMENTS.md)", static_cast<int>(ic.unresolved.size()) - 6));
    say("  title id: 0x" + res.title_id.utf8() + (res.title_suggested ? " (suggested \xE2\x80\x94 verify no collision)" : ""));
    say("  report: " + res.report.utf8());
    std::string codes;
    for (int64_t i = 0; i < res.code.size(); ++i) codes += (i ? ", " : "") + basename(res.code[i]).utf8();
    say("  xlast:  " + res.xlast.utf8() + "   code: " + codes);
    return 0;
}

int64_t cmd_ui_mark(const Namespace& A) {
    py::str dir = need(A.str("dir"), "dir");
    if (!py::os::path::isdir(dir)) {
        say("ERROR: not a directory: " + dir.utf8());
        return 2;
    }
    py::list<py::str> extra;
    for (const py::str& piece : py::split(A.str("extra") ? *A.str("extra") : py::str(""), py::str(","))) {
        py::str t = piece.strip();
        if (!t.empty()) extra.append(t);
    }
    bool dry = A.flag("dry_run");
    auto res = mc::ui_mark::run(dir, dry, extra);
    int64_t total = 0;
    for (const auto& kv : res.marked.items()) total += std::get<1>(kv).size();
    say(fmt("UI classes: %d   methods %s: %d   (already-marked skipped: %d)", static_cast<int>(res.classes.size()),
            dry ? "that would be marked" : "marked", static_cast<int>(total), static_cast<int>(res.skipped)));
    if (dry) {
        py::list<py::str> keys;
        for (const auto& kv : res.marked.items()) keys.append(std::get<0>(kv));
        int64_t shown = 0;
        for (const py::str& cls : py::sorted(keys)) {
            if (shown++ >= 20) break;
            say(fmt("  %s (%d)", cls.utf8().c_str(), static_cast<int>(res.marked.at(cls).size())));
        }
    } else {
        say("  index: " + (res.index ? res.index->utf8() : std::string("None")));
    }
    return 0;
}

int64_t cmd_keybind_mark(const Namespace& A) {
    py::str dir = need(A.str("dir"), "dir");
    if (!py::os::path::isdir(dir)) {
        say("ERROR: not a directory: " + dir.utf8());
        return 2;
    }
    bool dry = A.flag("dry_run");
    auto res = mc::keybind_mark::run(dir, dry);
    say(fmt("keyboard key-press sites %s: %d   (already-marked skipped: %d)", dry ? "that would be tagged" : "tagged", static_cast<int>(res.sites.size()),
            static_cast<int>(res.skipped)));
    if (!dry) say("  index: " + (res.index ? res.index->utf8() : std::string("None")));
    return 0;
}

int64_t cmd_bugfix_ledger(const Namespace& A) {
    auto entries = bugfix_ledger::run(A.str("repo"), A.str("out"));
    py::str out = A.truthy("out") ? *A.str("out") : state({"java", "BUGFIX_LEDGER.md"});
    std::set<std::string> slugs;
    for (const auto& e : entries) slugs.insert(e.slug.utf8());
    say(fmt("bugfix-ledger: %d markers / %d distinct fixes -> %s", static_cast<int>(entries.size()), static_cast<int>(slugs.size()), out.utf8().c_str()));
    return 0;
}

int64_t cmd_debug(const Namespace& A) {
    py::str out_dir = need(A.str("out_dir"), "out_dir");
    py::os::makedirs(out_dir, 0777, true);
    if (A.flag("off")) {
        auto paths = debug::write_debug(out_dir, false);
        std::string names;
        for (int64_t i = 0; i < paths.size(); ++i) names += (i ? ", " : "") + basename(paths[i]).utf8();
        say("Emitted debug files with DEBUG_ENABLED=0 (opt-in off): " + names);
    } else {
        auto paths = debug::write_debug(out_dir, true);
        std::string names;
        for (int64_t i = 0; i < paths.size(); ++i) names += (i ? ", " : "") + basename(paths[i]).utf8();
        say("Emitted " + names + " (master toggle DEBUG_ENABLED=1; J2X-DEBUG markers).");
    }
    auto markers = debug::scan_debug_markers(out_dir);
    py::list<py::str> keys;
    for (const auto& kv : markers.items()) keys.append(std::get<0>(kv));
    py::str joined = py::str(join(py::sorted(keys)));
    say(fmt("  J2X-DEBUG marker tags present: %d (%s)", static_cast<int>(markers.size()), joined.slice(0, 100).utf8().c_str()));
    return 0;
}

int64_t cmd_reflect_factories(const Namespace& A) {
    auto r = reflection_factory::run(need(A.str("out_dir"), "out_dir"));
    if (!r.total_sites) {
        say("No reflection sites to rewrite (already applied, or none present).");
        return 0;
    }
    for (const auto& reg : r.registries) {
        say(fmt("  %s -> %s_createByReflection  (%d classes, %d site%s)", pad(reg.file, 22).c_str(), reg.registry.utf8().c_str(),
                static_cast<int>(reg.classes.size()), static_cast<int>(reg.sites), reg.sites == 1 ? "" : "s"));
    }
    for (const auto& ru : r.reused) say(fmt("  %s -> reuses a shared factory  (%d site%s)", pad(ru.file, 22).c_str(), static_cast<int>(ru.sites), ru.sites == 1 ? "" : "s"));
    say(fmt("Generated ReflectionFactory.h; %d sites rewritten across %d registries (%d classes).", static_cast<int>(r.total_sites),
            static_cast<int>(r.registries.size()), static_cast<int>(r.total_classes)));
    return 0;
}

int64_t cmd_sound_fix(const Namespace& A) {
    auto r = mc::sound_fix::run(need(A.str("out_dir"), "out_dir"));
    if (!r.sites) say("No sound guard sites to rewrite (already applied, or none present).");
    for (const auto& fi : r.files)
        say(fmt("  %s -> %d SFX + %d music site(s) routed to the XACT bridge", pad(fi.file, 22).c_str(), static_cast<int>(fi.sfx), static_cast<int>(fi.music)));
    if (r.sites) say(fmt("Sound bypass applied: %d site(s) across %d file(s).", static_cast<int>(r.sites), static_cast<int>(r.files.size())));
    if (!r.stranded.empty()) {
        say("WARNING: soundPoolSounds null-guard survived in: " + join(r.stranded) + " (guard shape changed; update sound_fix.py).");
        return 1;
    }
    return 0;
}

int64_t cmd_controller_nav(const Namespace& A) {
    auto r = mc::controller_nav::run(need(A.str("out_dir"), "out_dir"));
    say(fmt("controller menu nav: GuiScreen.h=%s GuiScreen.cpp=%s call-site=%s", truth(r.guiscreen_h).c_str(), truth(r.guiscreen_cpp).c_str(), truth(r.callsite).c_str()));
    if (!r.missing.empty()) {
        say("WARNING: anchor(s) not found -> menus may be un-navigable: " + join(r.missing, "; "));
        return 1;
    }
    return 0;
}

int64_t cmd_render_fix(const Namespace& A) {
    auto r = mc::render_fix::run(need(A.str("out_dir"), "out_dir"));
    say(fmt("render-fix: updateRenderers budget=%d site(s), hurt-camera NaN guard=%d site(s)", static_cast<int>(r.update), static_cast<int>(r.hurt)));
    if (!r.stranded.empty()) {
        say("WARNING: a render-fix trigger survived in: " + join(r.stranded, "; ") +
            " (shape changed; update render_fix.py). The first in-game frame may hang on \"Spawning\" or render invisible.");
        return 1;
    }
    return 0;
}

int64_t cmd_applet_ping(const Namespace& A) {
    auto r = mc::applet_ping::run(need(A.str("out_dir"), "out_dir"));
    if (!r.sites) say("No applet ping sites to rewrite (already applied, or none present).");
    for (const auto& fi : r.files) say(fmt("  %s -> %d applet ping site(s) neutralized", pad(fi.file, 22).c_str(), static_cast<int>(fi.sites)));
    if (r.sites) say(fmt("Applet ping removed: %d site(s) across %d file(s).", static_cast<int>(r.sites), static_cast<int>(r.files.size())));
    if (!r.stranded.empty()) {
        say("WARNING: a getDocumentBase()->openStream() try survived in: " + join(r.stranded) + " (shape changed; update applet_ping.py).");
        return 1;
    }
    return 0;
}

int64_t cmd_scaffold(const Namespace& A) {
    py::str language = need(A.str("language"), "language");
    auto r = scaffold::run(need(A.str("java_dir"), "java_dir"), need(A.str("out_dir"), "out_dir"), language);
    say(fmt("scaffold: %d %s skeleton file(s) for %d class(es) -> %s", static_cast<int>(r.written.size()), scaffold::LANG_LABEL.at(language).utf8().c_str(),
            static_cast<int>(r.classes), A.str("out_dir")->utf8().c_str()));
    return 0;
}

int64_t cmd_scan_project(const Namespace& A) {
    auto r = console::project_scan::resolve(need(A.str("folder"), "folder"));
    // (sorted keys: base, jar, java_src, lang, resources)
    auto line = [](const char* k, const std::optional<py::str>& v) { say("  " + pad(py::str(k), 10) + " " + (v && !v->empty() ? v->utf8() : std::string("(not found)"))); };
    line("base", r.base);
    line("jar", r.jar);
    line("java_src", r.java_src);
    line("lang", r.lang);
    line("resources", r.resources);
    return 0;
}

int64_t cmd_build(const Namespace& A) {
    py::str msbuild = py::settings::get_str(py::str("msbuild"));
    if (msbuild.empty() || !py::os::path::isfile(msbuild)) {
        say("MSBuild is not set up (settings.json: msbuild)");
        return 2;
    }
    py::str log = A.truthy("log") ? *A.str("log") : state({"java", "build.log"});
    py::list<py::str> cmd{msbuild, py::str("/m"), py::str("/nologo"), py::str("/p:Configuration=") + *A.str("config"),
                          py::str("/p:Platform=") + *A.str("platform"), py::str("/flp:LogFile=") + log + py::str(";Verbosity=normal")};
    cmd.insert(1, need(A.str("sln"), "sln"));
    std::string shown;
    for (int64_t i = 0; i < cmd.size(); ++i) {
        const py::str& c = cmd[i];
        shown += (i ? " " : "") + (c.contains(py::str(" ")) ? "\"" + c.utf8() + "\"" : c.utf8());
    }
    say(shown);
    std::fflush(stdout);
    py::subprocess::RunOptions ro;
    ro.stdout_ = py::subprocess::Stream::Inherit;
    ro.stderr_ = py::subprocess::Stream::Inherit;
    int64_t code = py::subprocess::run(cmd, ro).returncode;
    say(std::string("build ") + (code == 0 ? "OK" : "FAILED (exit " + std::to_string(code) + ")") + " -- log: " + log.utf8());
    say("crash-class report with the build log: codebreaker java report OUT --build-log " + log.utf8());
    return code;
}

int64_t cmd_project_layout(const Namespace& A) {
    auto r = console::project_layout::layout(need(A.str("root"), "root"), A.truthy("name") ? A.str("name") : std::nullopt);
    if (r.moved) say(fmt("Moved %d flat entr%s down into %s\\", static_cast<int>(r.moved), r.moved == 1 ? "y" : "ies", r.name.utf8().c_str()));
    say("Solution layout ready:");
    say("  " + r.sln.utf8());
    say("  " + r.proj_dir.utf8() + "\\  (" + r.name.utf8() + ".vcxproj)");
    return 0;
}

int64_t cmd_platform_config(const Namespace& A) {
    py::str out_dir = need(A.str("out_dir"), "out_dir");
    py::os::makedirs(out_dir, 0777, true);
    py::dict<py::str, console::platform_config::Update> updates;
    if (A.truthy("layout")) updates.set(py::str("layout"), *A.str("layout"));
    if (A.flag("invert_y")) updates.set(py::str("invert_y"), true);
    if (A.flag("no_invert_y")) updates.set(py::str("invert_y"), false);
    if (A.num("sensitivity")) updates.set(py::str("look_sensitivity"), *A.num("sensitivity"));
    if (A.truthy("keyboard")) updates.set(py::str("keyboard"), *A.str("keyboard"));
    if (A.truthy("storage")) updates.set(py::str("storage"), *A.str("storage"));
    for (const py::str& b : A.list("bind")) {
        if (b.contains(py::str("="))) {
            py::list<py::str> kv = py::split(b, py::str("="), 1);
            updates.set(py::str("bind.") + kv[0].strip(), console::platform_config::Update(kv[1].strip()));
        }
    }
    if (A.flag("list")) {
        auto res = console::platform_config::run(out_dir, std::nullopt, updates, true);
        say(console::platform_config::_choices_md(std::get<0>(res)).utf8());
        return 0;
    }
    std::tuple<console::platform_config::Config, py::list<py::str>, std::optional<console::sdk::SdkInfo>, py::list<py::str>> res;
    try {
        res = console::platform_config::run(out_dir, std::nullopt, updates, false, A.str("platform"), A.str("sdk"));
    } catch (const console::sdk::SdkNotFound& e) {
        say(std::string("ERROR (SDK gate): ") + e.what());
        return 3;
    }
    const auto& cfg = std::get<0>(res);
    const auto& written = std::get<1>(res);
    const auto& info = std::get<2>(res);
    const auto& unverified = std::get<3>(res);
    say(fmt("Wrote %d file(s) to %s", static_cast<int>(written.size()), py::os::path::abspath(out_dir).utf8().c_str()));
    say("  controller layout=" + cfg.controller.layout.utf8() + " invertY=" + truth(cfg.controller.invert_y) + " | keyboard=" + cfg.keyboard.utf8() +
        " | storage=" + cfg.storage.utf8());
    if (info && !info->root.empty()) say("  SDK: " + info->name.utf8() + " @ " + info->root.utf8() + " (via " + info->source.utf8() + ")");
    if (!unverified.empty()) say("  WARNING: symbols not found in the located SDK: " + join(unverified));
    say("  see " + py::os::path::join(out_dir, py::str("PLATFORM_CHOICES.md")).utf8());
    return 0;
}

int64_t cmd_sdk(const Namespace& A) {
    std::optional<py::str> platform = A.str("platform"), over = A.truthy("sdk") ? A.str("sdk") : std::nullopt;
    say(console::sdk::describe(platform, over).utf8());
    auto info = console::sdk::detect(platform, over);
    if (info && !info->include_dirs.empty()) say("  include dirs: " + join(info->include_dirs));
    return info ? 0 : 1;
}

int64_t cmd_assets(const Namespace& A) {
    py::str jar = need(A.str("jar"), "jar");
    if (!py::os::path::isfile(jar)) {
        say("ERROR: not a file: " + jar.utf8());
        return 2;
    }
    py::str content_dir = need(A.str("content_dir"), "content_dir");
    auto res = console::assets::run(jar, content_dir, A.truthy("code") ? A.str("code") : std::nullopt, A.flag("only_needed"), !A.flag("no_rgba"));
    say(fmt("Extracted %d asset(s) -> %s", static_cast<int>(res.written.size()), py::os::path::abspath(content_dir).utf8().c_str()));
    if (!res.console.empty()) say(fmt("  + %d console-only asset(s) from the converter bundle: %s", static_cast<int>(res.console.size()), join(res.console).c_str()));
    if (!res.converted.empty())
        say(fmt("  normalized %d non-RGBA PNG(s) to RGBA (360 render fix): %s", static_cast<int>(res.converted.size()),
                (join(res.converted.slice(0, 6)) + (res.converted.size() > 6 ? " ..." : "")).c_str()));
    if (!res.unconverted.empty())
        say(fmt("  WARNING: %d non-RGBA PNG(s) could NOT be converted (Pillow missing) -> will misrender", static_cast<int>(res.unconverted.size())));
    if (res.coverage) {
        const auto& cov = *res.coverage;
        say(fmt("  coverage: code needs %d, jar provides %d, present %d, MISSING %d, jar extras %d", static_cast<int>(cov.needed.size()),
                static_cast<int>(cov.jar_count), static_cast<int>(cov.present.size()), static_cast<int>(cov.missing.size()), static_cast<int>(cov.extra.size())));
        for (const py::str& p : cov.missing) say("    MISSING (runtime NULL image): " + p.utf8());
        if (res.report && !res.report->empty()) say("  report: " + res.report->utf8());
    }
    return 0;
}

int64_t cmd_convert(const Namespace& A) {
    std::optional<py::str> java_dir = A.str("java_dir"), out_dir = A.str("out_dir");
    if (A.truthy("mc_version")) {
        // `convert --mc-version <id> <out_dir>`: the single positional is the output dir.
        if (!out_dir) out_dir = java_dir;
        if (!out_dir || out_dir->empty()) {
            say("ERROR: output directory required, e.g. `convert --mc-version a1.2.6 out/`");
            return 2;
        }
        py::str ws = A.truthy("mc_workspace") ? *A.str("mc_workspace") : py::os::path::join(*out_dir, py::str("_decompiled"));
        say("Decompiling Minecraft " + A.str("mc_version")->utf8() + " via the bundled RetroMCP/Fernflower backend...");
        try {
            java_dir = decompile::jar::decompile(*A.str("mc_version"), ws, A.truthy("java") ? A.str("java") : std::nullopt,
                                                 [](const py::str& ln) { say("  " + ln.utf8()); });
        } catch (const decompile::jar::DecompileError& e) {
            say(std::string("ERROR (decompile): ") + e.what());
            return 4;
        }
    }
    if (!java_dir || java_dir->empty() || !out_dir || out_dir->empty()) {
        say("ERROR: usage: convert <java_dir> <out_dir>   (or: convert --mc-version <id> <out_dir>)");
        return 2;
    }
    if (!py::os::path::isdir(*java_dir)) {
        say("ERROR: not a directory: " + java_dir->utf8());
        return 2;
    }
    bool project = A.flag("project");
    std::optional<py::str> platform = A.str("platform");
    std::optional<py::str> sdk_override = A.str("sdk");
    transpile::Summary s;
    try {
        s = transpile::convert(*java_dir, *out_dir, std::nullopt, nullptr, true, *A.str("cpp_std"), platform, nullptr, A.flag("platform_refs") || project, sdk_override,
                               true, std::nullopt, A.str("edition"), A.str("mc_version"));
    } catch (const console::sdk::SdkNotFound& e) {
        say(std::string("ERROR (SDK gate): ") + e.what());
        return 3;
    }
    say("Converted " + py::os::path::abspath(*java_dir).utf8() + " -> " + py::os::path::abspath(*out_dir).utf8());
    if (project) {
        // One-step buildable project: preserve/re-emit config, then (re)generate the VS project. Each
        // step is best-effort -- a missing SDK is reported, never fatal to the conversion.
        try {
            auto res = console::platform_config::run(*out_dir, std::nullopt, std::nullopt, false, platform && !platform->empty() ? platform : std::optional<py::str>(py::str("xbox360")), sdk_override);
            say(fmt("  project: config emitted (%d file(s), controller binds preserved)", static_cast<int>(std::get<1>(res).size())));
        } catch (const console::sdk::SdkNotFound& e) {
            say(std::string("  project: platform-config skipped (SDK gate): ") + e.what());
        } catch (const py::Exception& e) {
            say(std::string("  project: platform-config FAILED: ") + e.what());
        }
        py::str proj_abs = py::os::path::abspath(*out_dir);
        bool has_vcxproj = false;
        if (py::os::path::isdir(proj_abs))
            for (const py::str& f : py::os::listdir(proj_abs))
                if (f.endswith(py::str(".vcxproj"))) has_vcxproj = true;
        try {
            if (has_vcxproj) {
                console::vcxproj::run(*out_dir);   // prints its own byte/file-count status
                say("  project: VS project + filters refreshed");
            } else {
                // out_dir is the project subfolder (the reference layout is <root>/<Name>/<Name>.vcxproj),
                // so the solution root is its parent and the project name is its basename.
                auto info = console::project_layout::layout(py::os::path::dirname(proj_abs), py::os::path::basename(py::os::path::normpath(proj_abs)));
                say("  project: VS solution bootstrapped -> " + basename(info.sln).utf8() + " (+ " + basename(info.vcxproj).utf8() + ")");
            }
        } catch (const py::Exception& e) {
            say(std::string("  project: VS project step FAILED: ") + e.what());
        }
    }
    say("  target: " + s.platform.utf8() + " / " + s.cpp_std.utf8() + " / profile " + s.profile.utf8());
    say(fmt("  parsed=%d  headers=%d  sources=%d", static_cast<int>(s.parsed), static_cast<int>(s.headers), static_cast<int>(s.sources)));
    say(fmt("  verify (crash-class rule findings on output): %d", static_cast<int>(s.rule_findings)));
    say(fmt("  ledger events=%d  (HIGH=%d)", static_cast<int>(s.events), static_cast<int>(s.high)));
    if (s.report && !s.report->empty()) say("  report: " + py::os::path::join(*s.report, py::str("index.md")).utf8());
    return 0;
}

int64_t cmd_decompile(const Namespace& A) {
    if (!decompile::jar::find_java()) {
        say("ERROR: no Java runtime found. Install a JRE/JDK (8+) and put `java` on PATH or set JAVA_HOME. Only the decompile backend needs Java; the transpiler does not.");
        return 4;
    }
    py::str src;
    try {
        src = decompile::jar::decompile(need(A.str("version"), "version"), need(A.str("workspace"), "workspace"), A.truthy("java") ? A.str("java") : std::nullopt,
                                        [](const py::str& ln) { say("  " + ln.utf8()); });
    } catch (const decompile::jar::DecompileError& e) {
        say(std::string("ERROR: ") + e.what());
        return 4;
    }
    say("Decompiled Minecraft " + A.str("version")->utf8() + " -> " + src.utf8());
    say("  feed it to: codebreaker java convert \"" + src.utf8() + "\" <out_dir>");
    return 0;
}

int64_t cmd_report(const Namespace& A) {
    py::str output_dir = need(A.str("output_dir"), "output_dir");
    if (!py::os::path::isdir(output_dir)) {
        say("ERROR: not a directory: " + output_dir.utf8());
        return 2;
    }
    py::str root = py::os::getcwd();
    ledger::Ledger led;
    auto counts = analyze::analyze_all(led, output_dir, A.truthy("build_log") ? A.str("build_log") : std::nullopt, root, A.truthy("platform") ? A.str("platform") : std::nullopt);
    py::dict<py::str, py::str> meta;
    meta.set(py::str("output_dir"), py::os::path::abspath(output_dir));
    meta.set(py::str("build_log"), counts.build_log_path && !counts.build_log_path->empty() ? *counts.build_log_path : py::str("(none)"));
    meta.set(py::str("profile"), A.truthy("profile") ? *A.str("profile") : py::str("(unspecified)"));
    py::str out_dir = report::write_reports(led, output_dir, A.truthy("out") ? A.str("out") : std::nullopt, meta);
    auto sevs = led.totals_by_severity();
    say("Analyzed " + py::os::path::abspath(output_dir).utf8());
    say(fmt("  markers=%d  rules=%d  lint=%d  build_log=%d", static_cast<int>(counts.markers), static_cast<int>(counts.rules), static_cast<int>(counts.lint),
            static_cast<int>(counts.build_log)));
    say(fmt("  events: %d  (HIGH=%d MED=%d LOW=%d)", static_cast<int>(led.size()), static_cast<int>(sevs.at(ledger::Severity::HIGH)),
            static_cast<int>(sevs.at(ledger::Severity::MED)), static_cast<int>(sevs.at(ledger::Severity::LOW))));
    std::vector<ledger::FileRisk> top;
    for (const auto& r : led.file_risk_table())
        if (std::get<1>(r) > 0) top.push_back(r);
    if (top.size() > 8) top.resize(8);
    if (!top.empty()) {
        say("  highest-risk files:");
        for (const auto& r : top) say(fmt("    %4d  %s", static_cast<int>(std::get<1>(r)), std::get<0>(r).utf8().c_str()));
    }
    say("Report written to " + py::os::path::abspath(out_dir).utf8());
    say("  open " + py::os::path::join(out_dir, py::str("index.md")).utf8());
    return 0;
}

// Activate the target profile for a standalone post-hoc command (signatures/vcxproj) that runs outside `convert`. Only an
// explicit --edition is honored here. -> the previous profile name (or None) so the caller can restore it.
std::optional<py::str> apply_edition(const Namespace& A) {
    if (!A.truthy("edition")) return std::nullopt;
    py::str prev = profiles::active()->name;
    std::optional<py::str> name = mc::edition::resolve(A.str("edition"));
    if (name && !name->empty() && *name != prev) {
        profiles::use(*name);
        say("  edition: active target profile: " + name->utf8());
    }
    return prev;
}

void restore_edition(const std::optional<py::str>& prev) {
    if (prev && profiles::active()->name != *prev) profiles::use(*prev);
}

int64_t cmd_signatures(const Namespace& A) {
    py::str java_dir = need(A.str("java_dir"), "java_dir");
    if (!py::os::path::isdir(java_dir)) {
        say("ERROR: not a directory: " + java_dir.utf8());
        return 2;
    }
    std::optional<py::str> prev = apply_edition(A);
    try {
        signatures::run(java_dir, A.truthy("out") ? *A.str("out") : state({"java", "signatures.md"}));
    } catch (...) {
        restore_edition(prev);
        throw;
    }
    restore_edition(prev);
    return 0;
}

int64_t cmd_worklist(const Namespace& A) {
    py::str out_dir = need(A.str("out_dir"), "out_dir");
    if (!py::os::path::isdir(out_dir)) {
        say("ERROR: not a directory: " + out_dir.utf8());
        return 2;
    }
    return console::worklist::run(out_dir, A.truthy("cc") ? A.str("cc") : std::nullopt, *A.str("std"), A.truthy("out") ? A.str("out") : std::nullopt);
}

int64_t cmd_vcxproj(const Namespace& A) {
    std::optional<py::str> prev = apply_edition(A);
    try {
        console::vcxproj::run(A.truthy("proj") ? A.str("proj") : std::nullopt);
    } catch (...) {
        restore_edition(prev);
        throw;
    }
    restore_edition(prev);
    return 0;
}

int64_t cmd_apimap(const Namespace& A) {
    auto reg = apimap::load();
    py::str act = need(A.str("action"), "action");
    std::optional<py::str> a = A.truthy("a") ? A.str("a") : std::nullopt, src = A.truthy("src") ? A.str("src") : std::nullopt,
                           dst = A.truthy("dst") ? A.str("dst") : std::nullopt, dialect = A.truthy("dialect") ? A.str("dialect") : std::nullopt;
    if (act == py::str("languages")) {
        py::list<py::str> ids;
        for (const auto& kv : reg->languages.items()) ids.append(std::get<0>(kv));
        for (const py::str& lid : py::sorted(ids)) {
            const auto& info = reg->languages.at(lid);
            std::string dl = info.dialects && !info.dialects->empty() ? "  dialects: " + join(*info.dialects) : "";
            say("  " + pad(lid, 12) + " [" + (info.kind ? info.kind->utf8() : std::string("?")) + "] " + (info.desc ? info.desc->utf8() : std::string()) + dl);
        }
        return 0;
    }
    if (act == py::str("subsystems")) {
        py::list<py::str> ids;
        for (const auto& kv : reg->subsystems.items()) ids.append(std::get<0>(kv));
        for (const py::str& sid : py::sorted(ids)) {
            const auto& s = reg->subsystems.at(sid);
            say("  " + pad(sid, 24) + " (" + s.language.utf8() + ")  " + s.description.slice(0, 70).utf8());
        }
        return 0;
    }
    if (act == py::str("list")) {
        if (!a) {
            say("ERROR: apimap list <subsystem>");
            return 2;
        }
        auto rows = reg->symbols_in(*a);
        if (rows.empty()) {
            say("No symbols in subsystem " + py::repr(*a).utf8() + ". Try: apimap subsystems");
            return 2;
        }
        for (const auto& row : rows) {
            std::string tgts;
            bool first = true;
            for (const auto& kv : std::get<1>(row)->maps.items()) {
                tgts += (first ? "" : ", ") + std::get<0>(kv).utf8() + "=" + std::get<1>(kv).target.utf8();
                first = false;
            }
            say("  " + pad(std::get<0>(row), 28) + " -> " + tgts);
        }
        return 0;
    }
    if (act == py::str("resolve")) {
        if (!(a && src && dst)) {
            say("ERROR: apimap resolve <symbol> --from <lang> --to <lang> [--dialect D]");
            return 2;
        }
        auto m = reg->resolve(*a, *src, *dst, dialect);
        if (!m) {
            say("UNMAPPED: " + a->utf8() + " (" + src->utf8() + " -> " + dst->utf8() + ")");
            return 1;
        }
        say(a->utf8() + "  (" + src->utf8() + " -> " + dst->utf8() + (m->dialect && !m->dialect->empty() ? "@" + m->dialect->utf8() : std::string()) + ")");
        say("  target : " + m->target.utf8());
        say("  kind   : " + m->kind.utf8());
        if (!m->note.empty()) say("  note   : " + m->note.utf8());
        return 0;
    }
    if (act == py::str("coverage")) {
        if (!(a && src && dst)) {
            say("ERROR: apimap coverage <java_dir> --from <lang> --to <lang> [--dialect D] [--out FILE]");
            return 2;
        }
        if (!py::os::path::isdir(*a)) {
            say("ERROR: not a directory: " + a->utf8());
            return 2;
        }
        auto syms = apimap::extract_symbols_from_dir(*a);
        py::str md = apimap::render_coverage_md(syms, *src, *dst, dialect, reg);
        py::str out = A.truthy("out") ? *A.str("out") : py::os::path::join(*a, py::str("APIMAP.md"));
        py::file f = py::open(out, py::str("w"), py::str("utf-8"));
        f.write(md);
        f.close();
        auto cov = reg->coverage(syms, *src, *dst, dialect);
        say(fmt("symbols=%d  mapped=%d  unsupported=%d  unmapped=%d  ->  %s", static_cast<int>(syms.size()), static_cast<int>(cov.mapped.size()),
                static_cast<int>(cov.unsupported.size()), static_cast<int>(cov.unmapped.size()), out.utf8().c_str()));
        return 0;
    }
    say("ERROR: unknown apimap action " + py::repr(act).utf8());
    return 2;
}

int64_t dispatch(const std::string& cmd, const Namespace& A) {
    if (cmd == "convert") return cmd_convert(A);
    if (cmd == "decompile") return cmd_decompile(A);
    if (cmd == "report") return cmd_report(A);
    if (cmd == "signatures") return cmd_signatures(A);
    if (cmd == "worklist") return cmd_worklist(A);
    if (cmd == "vcxproj") return cmd_vcxproj(A);
    if (cmd == "xact") return cmd_xact(A);
    if (cmd == "assets") return cmd_assets(A);
    if (cmd == "platform-config") return cmd_platform_config(A);
    if (cmd == "achievements") return cmd_achievements(A);
    if (cmd == "ui-mark") return cmd_ui_mark(A);
    if (cmd == "keybind-mark") return cmd_keybind_mark(A);
    if (cmd == "bugfix-ledger") return cmd_bugfix_ledger(A);
    if (cmd == "debug") return cmd_debug(A);
    if (cmd == "sdk") return cmd_sdk(A);
    if (cmd == "project-layout") return cmd_project_layout(A);
    if (cmd == "reflect-factories") return cmd_reflect_factories(A);
    if (cmd == "sound-fix") return cmd_sound_fix(A);
    if (cmd == "controller-nav") return cmd_controller_nav(A);
    if (cmd == "render-fix") return cmd_render_fix(A);
    if (cmd == "applet-ping") return cmd_applet_ping(A);
    if (cmd == "apimap") return cmd_apimap(A);
    if (cmd == "build") return cmd_build(A);
    if (cmd == "scaffold") return cmd_scaffold(A);
    if (cmd == "scan-project") return cmd_scan_project(A);
    throw py::KeyError(cmd);
}

}  // namespace

int64_t main(const py::list<py::str>& argv) {
    // the top-level parser: -h, and the subcommand (a name and the rest of the line)
    static const ParserSpec main_spec = [] {
        ParserSpec m{MAIN_PROG, MAIN_PROG, MAIN_USAGE, MAIN_HELP, {}};
        m.actions.push_back({Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt});
        ActionSpec cmd{Kind::PARSER, "cmd", {}, 'P', 's', {}, std::nullopt, false, std::nullopt};
        for (const ParserSpec& p : PARSERS()) cmd.choices.push_back(py::str(p.name));
        m.actions.push_back(cmd);
        return m;
    }();
    std::vector<std::string> args;
    for (const py::str& a : argv) args.push_back(a.utf8());
    Namespace ns;
    Parser(main_spec).parse_args(args, ns);
    auto cmd = ns.str("cmd");
    if (!cmd || cmd->empty()) {
        say(MODULE_DOC);
        return 2;
    }
    return dispatch(cmd->utf8(), ns);
}

}  // namespace codebreaker::java::run
