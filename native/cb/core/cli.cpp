#include "cb/core/cli.hpp"

#include <algorithm>
#include <cctype>
#include <format>

#include "cb/core/log.hpp"

namespace cb::cli {

// ---- Args ---------------------------------------------------------------------------------------------

std::string_view Args::strip(std::string_view n) {
    while (!n.empty() && n.front() == '-') n.remove_prefix(1);
    return n;
}

std::size_t Args::count(std::string_view name) const {
    auto it = counts_.find(strip(name));
    return it == counts_.end() ? 0 : it->second;
}

std::optional<std::string> Args::value(std::string_view name) const {
    const std::string_view key = strip(name);
    if (auto it = values_.find(key); it != values_.end() && !it->second.empty()) return it->second.back();
    if (auto it = defaults_.find(key); it != defaults_.end()) return it->second;
    return std::nullopt;
}

std::string Args::value_or(std::string_view name, std::string fallback) const {
    auto v = value(name);
    return v ? *v : std::move(fallback);
}

std::string Args::required(std::string_view name) const {
    auto v = value(name);
    CB_ASSERT(v.has_value(), "Args::required on an argument the command did not declare required");
    return *v;
}

std::vector<std::string> Args::values(std::string_view name) const {
    auto it = values_.find(strip(name));
    return it == values_.end() ? std::vector<std::string>{} : it->second;
}

// ---- parsing ------------------------------------------------------------------------------------------

namespace {

const Positional* as_positional(const Param& p) { return std::get_if<Positional>(&p); }
const Option* as_option(const Param& p) { return std::get_if<Option>(&p); }

std::string bare(const std::string& long_name) {
    std::size_t i = 0;
    while (i < long_name.size() && long_name[i] == '-') ++i;
    return long_name.substr(i);
}

// Edit distance for "did you mean".
std::size_t distance(std::string_view a, std::string_view b) {
    std::vector<std::size_t> prev(b.size() + 1), cur(b.size() + 1);
    for (std::size_t j = 0; j <= b.size(); ++j) prev[j] = j;
    for (std::size_t i = 1; i <= a.size(); ++i) {
        cur[0] = i;
        for (std::size_t j = 1; j <= b.size(); ++j) {
            cur[j] = std::min({prev[j] + 1, cur[j - 1] + 1, prev[j - 1] + (a[i - 1] == b[j - 1] ? 0 : 1)});
        }
        std::swap(prev, cur);
    }
    return prev[b.size()];
}

std::string suggestion(std::string_view word, const std::vector<std::string>& candidates) {
    std::string best;
    std::size_t best_d = 3;   // only suggest within two edits
    for (const auto& c : candidates) {
        const std::size_t d = distance(word, c);
        if (d < best_d) {
            best_d = d;
            best = c;
        }
    }
    return best.empty() ? std::string() : std::format(" (did you mean '{}'?)", best);
}

bool looks_like_option(const std::string& tok) { return tok.size() > 1 && tok[0] == '-'; }

}  // namespace

Cli::Cli(std::string program, std::vector<Command> commands) : program_(std::move(program)), commands_(std::move(commands)) {}

const Command* Cli::find(const std::vector<std::string>& path) const {
    const std::vector<Command>* level = &commands_;
    const Command* found = nullptr;
    for (const auto& word : path) {
        found = nullptr;
        for (const auto& c : *level) {
            if (c.name == word) {
                found = &c;
                break;
            }
        }
        if (found == nullptr) return nullptr;
        level = &found->subcommands;
    }
    return found;
}

Cli::Parsed Cli::parse(const std::vector<std::string>& args) const {
    Parsed result;
    if (args.empty()) throw UsageError("no command given");

    // 1. The command words.
    const std::vector<Command>* level = &commands_;
    const Command* cmd = nullptr;
    std::size_t i = 0;
    while (i < args.size()) {
        const std::string& word = args[i];
        if (word == "-h" || word == "--help") {
            result.help = true;
            result.command = cmd;
            return result;
        }
        const Command* found = nullptr;
        for (const auto& c : *level) {
            if (c.name == word) {
                found = &c;
                break;
            }
        }
        if (found == nullptr) {
            std::vector<std::string> names;
            for (const auto& c : *level) names.push_back(c.name);
            throw UsageError(std::format("unknown command '{}'{}", word, suggestion(word, names)));
        }
        cmd = found;
        result.path += (result.path.empty() ? "" : " ") + cmd->name;
        ++i;
        if (cmd->subcommands.empty()) break;
        level = &cmd->subcommands;
        if (i >= args.size()) {   // a group with no subcommand named: show its help
            result.help = true;
            result.command = cmd;
            return result;
        }
    }
    result.command = cmd;

    // 2. The command's own arguments.
    std::map<std::string, const Option*, std::less<>> long_opts, short_opts;
    std::vector<const Positional*> positionals;
    for (const Param& p : cmd->params) {
        if (const Option* o = as_option(p)) {
            long_opts[bare(o->name)] = o;
            if (!o->alias.empty()) short_opts[bare(o->alias)] = o;
            if (!o->default_value.empty()) result.args.defaults_[bare(o->name)] = o->default_value;
        } else if (const Positional* pos = as_positional(p)) {
            positionals.push_back(pos);
        }
    }

    std::size_t next_positional = 0;
    auto add_positional = [&](const std::string& tok) {
        if (next_positional >= positionals.size())
            throw UsageError(std::format("unexpected argument '{}'", tok));
        const Positional* pos = positionals[next_positional];
        result.args.values_[pos->name].push_back(tok);
        result.args.counts_[pos->name]++;
        if (!pos->variadic) ++next_positional;
    };
    auto take_value = [&](std::string_view as_written, std::size_t& at) -> std::string {
        if (at + 1 >= args.size()) throw UsageError(std::format("option {} needs a value", as_written));
        return args[++at];
    };
    auto record = [&](const Option& o, std::string value) {
        result.args.values_[bare(o.name)].push_back(std::move(value));
        result.args.counts_[bare(o.name)]++;
    };

    bool options_ended = false;
    for (std::size_t at = i; at < args.size(); ++at) {
        const std::string& tok = args[at];
        if (options_ended) {
            add_positional(tok);
        } else if (tok == "--") {
            options_ended = true;
        } else if (tok == "-h" || tok == "--help") {
            result.help = true;
            return result;
        } else if (tok.starts_with("--")) {
            const std::size_t eq = tok.find('=');
            const std::string name = tok.substr(2, eq == std::string::npos ? std::string::npos : eq - 2);
            auto it = long_opts.find(name);
            if (it == long_opts.end()) {
                std::vector<std::string> names;
                for (const auto& [n, o] : long_opts) names.push_back("--" + n);
                throw UsageError(std::format("unknown option '--{}'{}", name, suggestion("--" + name, names)));
            }
            const Option& o = *it->second;
            if (o.arg == Arg::Flag) {
                if (eq != std::string::npos) throw UsageError(std::format("option --{} does not take a value", name));
                result.args.counts_[bare(o.name)]++;
            } else {
                record(o, eq != std::string::npos ? tok.substr(eq + 1) : take_value("--" + name, at));
            }
        } else if (looks_like_option(tok) && !(std::isdigit(static_cast<unsigned char>(tok[1])) && !short_opts.contains(tok.substr(1, 1)))) {
            for (std::size_t j = 1; j < tok.size(); ++j) {
                const std::string c(1, tok[j]);
                auto it = short_opts.find(c);
                if (it == short_opts.end()) throw UsageError(std::format("unknown option '-{}'", c));
                const Option& o = *it->second;
                if (o.arg == Arg::Flag) {
                    result.args.counts_[bare(o.name)]++;
                    continue;
                }
                if (j + 1 < tok.size()) {   // -n5 / -f=name: the rest of the cluster is the value
                    std::string rest = tok.substr(j + 1);
                    if (rest.starts_with('=')) rest.erase(0, 1);
                    record(o, std::move(rest));
                } else {
                    record(o, take_value("-" + c, at));
                }
                break;
            }
        } else {
            add_positional(tok);
        }
    }

    // 3. Required positionals.
    for (std::size_t p = 0; p < positionals.size(); ++p) {
        const Positional& pos = *positionals[p];
        if (pos.required && result.args.count(pos.name) == 0)
            throw UsageError(std::format("missing argument <{}>", pos.name));
    }
    return result;
}

// ---- help ---------------------------------------------------------------------------------------------

namespace {

std::string usage_line(std::string_view program, std::string_view path, const Command& c) {
    std::string line = std::format("usage: {} {}", program, path);
    if (!c.subcommands.empty()) return line + " <command> [options]";
    bool has_options = false;
    for (const Param& p : c.params) {
        if (as_option(p) != nullptr) has_options = true;
    }
    if (has_options) line += " [options]";
    for (const Param& p : c.params) {
        if (const Positional* pos = as_positional(p)) {
            std::string name = "<" + pos->name + ">";
            if (pos->variadic) name += "...";
            line += pos->required ? " " + name : " [" + name + "]";
        }
    }
    return line;
}

// Two-column rows with the right column aligned.
void append_rows(std::string& out, const std::vector<std::pair<std::string, std::string>>& rows) {
    std::size_t width = 0;
    for (const auto& [left, right] : rows) width = std::max(width, std::min<std::size_t>(left.size(), 30));
    for (const auto& [left, right] : rows) {
        out += "  " + left;
        if (left.size() > 30) {
            out += "\n  " + std::string(width, ' ') + "  " + right + "\n";
        } else {
            out += std::string(width - left.size(), ' ') + "  " + right + "\n";
        }
    }
}

}  // namespace

std::string Cli::help(const Command& c, std::string_view path) const {
    std::string out = usage_line(program_, path, c) + "\n\n" + c.summary + "\n";
    if (!c.description.empty()) out += "\n" + c.description + "\n";
    if (!c.subcommands.empty()) {
        out += "\ncommands:\n";
        std::vector<std::pair<std::string, std::string>> rows;
        for (const auto& s : c.subcommands) rows.emplace_back(s.name, s.summary);
        append_rows(out, rows);
        return out;
    }
    std::vector<std::pair<std::string, std::string>> args_rows, opt_rows;
    for (const Param& p : c.params) {
        if (const Positional* pos = as_positional(p)) {
            args_rows.emplace_back(pos->name + (pos->variadic ? "..." : ""), pos->help);
        } else if (const Option* o = as_option(p)) {
            std::string left = o->alias.empty() ? "    " : o->alias + ", ";
            left += o->name;
            if (o->arg == Arg::Value) left += " " + (o->value_name.empty() ? text::upper(bare(o->name)) : o->value_name);
            std::string right = o->help;
            if (!o->default_value.empty()) right += std::format(" (default: {})", o->default_value);
            opt_rows.emplace_back(std::move(left), std::move(right));
        }
    }
    opt_rows.emplace_back("-h, --help", "show this help");
    if (!args_rows.empty()) {
        out += "\narguments:\n";
        append_rows(out, args_rows);
    }
    out += "\noptions:\n";
    append_rows(out, opt_rows);
    return out;
}

std::string Cli::overview() const {
    std::string out = std::format("usage: {} <command> [arguments]\n", program_);
    std::vector<std::string> areas;
    for (const auto& c : commands_) {
        const std::string area = c.area.empty() ? "Commands" : c.area;
        if (std::find(areas.begin(), areas.end(), area) == areas.end()) areas.push_back(area);
    }
    for (const auto& area : areas) {
        out += "\n" + area + ":\n";
        std::vector<std::pair<std::string, std::string>> rows;
        for (const auto& c : commands_) {
            if ((c.area.empty() ? std::string("Commands") : c.area) == area) rows.emplace_back(c.name, c.summary);
        }
        append_rows(out, rows);
    }
    out += std::format("\nRun '{} <command> --help' for a command's arguments and options.\n", program_);
    return out;
}

int Cli::run(const std::vector<std::string>& args) const {
    // `help` and `help <command...>` are built in.
    if (!args.empty() && args[0] == "help") {
        const std::vector<std::string> path(args.begin() + 1, args.end());
        if (path.empty()) {
            out_text(overview());
            return kExitOk;
        }
        if (const Command* c = find(path)) {
            std::string joined;
            for (const auto& w : path) joined += (joined.empty() ? "" : " ") + w;
            out_text(help(*c, joined));
            return kExitOk;
        }
        err("{}: unknown command '{}'\n", program_, path.front());
        return kExitUsage;
    }
    Parsed parsed;
    try {
        parsed = parse(args);
    } catch (const UsageError& e) {
        err("{}: error: {}\n", program_, e.what());
        // show the usage line of the command the words reached, if any
        std::vector<std::string> words;
        for (const auto& a : args) {
            if (looks_like_option(a)) break;
            words.push_back(a);
            if (find(words) == nullptr) {
                words.pop_back();
                break;
            }
        }
        if (const Command* c = find(words)) {
            std::string joined;
            for (const auto& w : words) joined += (joined.empty() ? "" : " ") + w;
            err("{}\n", usage_line(program_, joined, *c));
        } else {
            err("run '{} help' for the commands\n", program_);
        }
        return kExitUsage;
    }
    if (parsed.help) {
        if (parsed.command != nullptr) out_text(help(*parsed.command, parsed.path));
        else out_text(overview());
        return kExitOk;
    }
    CB_ASSERT(parsed.command != nullptr, "parse returned no command");
    if (!parsed.command->handler) throw InternalError("command '" + parsed.path + "' has no handler");
    return parsed.command->handler(parsed.args);
}

int Cli::run(int argc, char** argv) const {
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) args.emplace_back(argv[i]);
    if (args.empty()) {
        out_text(overview());
        return kExitUsage;
    }
    return run(args);
}

}  // namespace cb::cli
