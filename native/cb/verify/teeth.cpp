// cb/verify/teeth.cpp -- see teeth.hpp.
//
// The five breaks read the function's C text with small scanners instead of regular expressions (docs/CPP_DESIGN.md
// 3.7: an idiom is code). Each looks for the FIRST place its pattern fits, in the text with strings, character
// constants and comments blanked, and edits the same place in the real text (blanking keeps every offset).

#include "cb/verify/teeth.hpp"

#include <algorithm>
#include <cstdint>
#include <format>
#include <map>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::verify::teeth {

namespace {

constexpr std::size_t kChangeWidth = 200;   // a "change" line is cut to this many bytes
constexpr std::size_t kSurvivorsShown = 12; // survivors of the strong verdict the command lists

bool is_word(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
bool is_digit(char c) {
    return c >= '0' && c <= '9';
}
bool is_ident_start(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
bool is_hex(char c) {
    return is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

// The text with every comment, string and character constant that is closed made spaces, quotes and newlines too
// (a quote with no partner on its line is left as it is: an apostrophe). Every offset is the same.
// TODO(cb/text/lex): the shared C lexer replaces this when it exists.
std::string blank_literals(std::string_view s) {
    std::string out(s);
    const std::size_t n = s.size();
    std::size_t i = 0;
    while (i < n) {
        if (s[i] == '/' && i + 1 < n && s[i + 1] == '*') {
            const std::size_t close = s.find("*/", i + 2);
            if (close != std::string_view::npos) {
                std::fill(out.begin() + static_cast<std::ptrdiff_t>(i),
                          out.begin() + static_cast<std::ptrdiff_t>(close + 2), ' ');
                i = close + 2;
                continue;
            }
        } else if (s[i] == '"' || s[i] == '\'') {
            const char quote = s[i];
            std::size_t j = i + 1;
            bool closed = false;
            while (j < n) {
                if (s[j] == '\\') {
                    if (j + 1 >= n)
                        break;
                    j += 2; // an escape takes the next character, even a newline
                } else if (s[j] == quote) {
                    closed = true;
                    break;
                } else if (s[j] == '\n') {
                    break;
                } else {
                    ++j;
                }
            }
            if (closed) {
                std::fill(out.begin() + static_cast<std::ptrdiff_t>(i),
                          out.begin() + static_cast<std::ptrdiff_t>(j + 1), ' ');
                i = j + 1;
                continue;
            }
        }
        ++i;
    }
    return out;
}

// A number written as digits is bumped by one in its own radix as TEXT, so a constant of any size is broken exactly
// (a 64-bit limit would make the largest constants unbreakable). Leading zeros do not count.
std::string_view strip_zeros(std::string_view digits) {
    std::size_t i = 0;
    while (i + 1 < digits.size() && digits[i] == '0')
        ++i;
    return digits.substr(i);
}
std::string increment_decimal(std::string_view digits) {
    std::string d(strip_zeros(digits));
    for (std::size_t i = d.size(); i-- > 0;) {
        if (d[i] != '9') {
            ++d[i];
            return d;
        }
        d[i] = '0';
    }
    return "1" + d;
}
std::string increment_hex(std::string_view digits) {
    std::string d = text::lower(strip_zeros(digits));
    for (std::size_t i = d.size(); i-- > 0;) {
        if (d[i] != 'f') {
            d[i] = d[i] == '9' ? 'a' : static_cast<char>(d[i] + 1);
            return d;
        }
        d[i] = '0';
    }
    return "1" + d;
}

std::string splice(std::string_view body, std::size_t from, std::size_t to, std::string_view with) {
    std::string out(body.substr(0, from));
    out += with;
    out += body.substr(to);
    return out;
}

// ---- the breaks: (the statements, the same with strings and comments blanked) -> the statements broken, or nothing

// The first number that is not part of a name, a member or an index: one more than it was.
std::optional<std::string> break_constant(std::string_view body, std::string_view shadow) {
    const std::size_t n = shadow.size();
    for (std::size_t i = 0; i < n; ++i) {
        if (!is_digit(shadow[i]) || (i > 0 && (is_word(shadow[i - 1]) || shadow[i - 1] == '.' || shadow[i - 1] == '[')))
            continue;
        std::size_t j = i;
        while (j < n && is_digit(shadow[j]))
            ++j;
        if (j >= n || !(is_word(shadow[j]) || shadow[j] == '.' || shadow[j] == ']')) { // a decimal ("0x.." is not one)
            return splice(body, i, j, increment_decimal(shadow.substr(i, j - i)));
        }
        if (shadow[i] == '0' && i + 1 < n && shadow[i + 1] == 'x') { // a hexadecimal, 0x lowercase
            std::size_t k = i + 2;
            while (k < n && is_hex(shadow[k]))
                ++k;
            if (k > i + 2 && (k >= n || !(is_word(shadow[k]) || shadow[k] == '.' || shadow[k] == ']')))
                return splice(body, i, k, "0x" + increment_hex(shadow.substr(i + 2, k - i - 2)));
        }
    }
    return std::nullopt;
}

// The first == or != turned round; failing that, the first < or > (spaced) made <= / >=.
std::optional<std::string> break_comparison(std::string_view body, std::string_view shadow) {
    const std::size_t n = shadow.size();
    auto in = [](char c, std::string_view set) { return set.find(c) != std::string_view::npos; };
    for (std::size_t i = 0; i + 1 < n; ++i) {
        if ((shadow[i] != '=' && shadow[i] != '!') || shadow[i + 1] != '=')
            continue;
        if (i > 0 && in(shadow[i - 1], "=!<>"))
            continue;
        if (i + 2 < n && shadow[i + 2] == '=')
            continue;
        return splice(body, i, i + 2, shadow[i] == '=' ? "!=" : "==");
    }
    for (std::size_t i = 0; i + 3 <= n; ++i) {
        if (!text::is_space(shadow[i]) || (shadow[i + 1] != '<' && shadow[i + 1] != '>') || i + 2 >= n ||
            !text::is_space(shadow[i + 2]))
            continue;
        if (i > 0 && in(shadow[i - 1], "<>=-"))
            continue;
        if (i + 3 < n && in(shadow[i + 3], "<>="))
            continue;
        return splice(body, i + 1, i + 2, shadow[i + 1] == '<' ? ">=" : "<=");
    }
    return std::nullopt;
}

// The first spaced + between an operand end and an operand start made a minus.
std::optional<std::string> break_operator(std::string_view body, std::string_view shadow) {
    const std::size_t n = shadow.size();
    for (std::size_t i = 1; i + 3 < n; ++i) {
        if (!text::is_space(shadow[i]) || shadow[i + 1] != '+' || !text::is_space(shadow[i + 2]))
            continue;
        const char before = shadow[i - 1], after = shadow[i + 3];
        if (!(is_word(before) || before == ')' || before == ']'))
            continue;
        if (!(is_word(after) || after == '(' || after == '*'))
            continue;
        return splice(body, i, i + 3, " - ");
    }
    return std::nullopt;
}

// The first line that is a store through a pointer (`*p = v;`) taken out.
std::optional<std::string> break_store(std::string_view body, std::string_view shadow) {
    const std::size_t n = shadow.size();
    std::size_t line = 0;
    while (line < n) {
        const std::size_t nl = shadow.find('\n', line);
        if (nl == std::string_view::npos)
            break; // the statement must end the line with a newline
        std::string_view s = shadow.substr(line, nl - line);
        std::size_t i = 0;
        while (i < s.size() && (s[i] == ' ' || s[i] == '\t'))
            ++i;
        if (i < s.size() && s[i] == '*') {
            const std::size_t eq = s.find_first_of(";=", i + 1);
            if (eq != std::string_view::npos && s[eq] == '=') {
                const std::size_t end = s.find_first_of(";=", eq + 1);
                if (end != std::string_view::npos && s[end] == ';' &&
                    s.find_first_not_of(" \t", end + 1) == std::string_view::npos)
                    return splice(body, line, nl + 1, "");
            }
        }
        line = nl + 1;
    }
    return std::nullopt;
}

// The end of an operand (a name, a decimal or a 0x number) that starts at `i` and is followed by one of
// `terminators`; npos if there is none.
std::size_t operand_end(std::string_view s, std::size_t i, std::string_view terminators) {
    const std::size_t n = s.size();
    if (i >= n)
        return std::string_view::npos;
    std::size_t e = i;
    if (is_ident_start(s[i])) {
        while (e < n && is_word(s[e]))
            ++e;
    } else if (is_digit(s[i])) {
        while (e < n && is_digit(s[e]))
            ++e;
        if (!(e < n && terminators.find(s[e]) != std::string_view::npos) && s[i] == '0' && i + 1 < n &&
            s[i + 1] == 'x') {
            e = i + 2;
            while (e < n && is_hex(s[e]))
                ++e;
            if (e == i + 2)
                return std::string_view::npos;
        }
    } else {
        return std::string_view::npos;
    }
    return e < n && terminators.find(s[e]) != std::string_view::npos ? e : std::string_view::npos;
}

// The first call whose first two arguments are plain operands: the two swapped. Not for if/while/for/switch/
// return/sizeof, and not for two alike.
std::optional<std::string> break_arguments(std::string_view body, std::string_view shadow) {
    const std::size_t n = shadow.size();
    for (std::size_t i = 0; i < n; ++i) {
        if (!is_ident_start(shadow[i]) || (i > 0 && is_word(shadow[i - 1])))
            continue;
        std::size_t j = i;
        while (j < n && is_word(shadow[j]))
            ++j;
        if (j >= n || shadow[j] != '(')
            continue;
        const std::size_t first = j + 1;
        const std::size_t comma = operand_end(shadow, first, ",");
        if (comma == std::string_view::npos)
            continue;
        const std::size_t second = comma + 1;
        const std::size_t end = operand_end(shadow, second, ",)");
        if (end == std::string_view::npos)
            continue;
        const std::string_view fn = shadow.substr(i, j - i);
        const std::string_view a = shadow.substr(first, comma - first), b = shadow.substr(second, end - second);
        if (a == b || fn == "if" || fn == "while" || fn == "for" || fn == "switch" || fn == "return" || fn == "sizeof")
            return std::nullopt;
        return splice(body, first, end, std::string(b) + "," + std::string(a));
    }
    return std::nullopt;
}

// A function's text as (its head: everything up to the opening brace, and the declarations after it), and the
// statements: breaks are made in the statements.
std::pair<std::string, std::string> split_body(std::string_view text) {
    static const Regex declaration(R"(\s+[\w ]+[ *]\w+(\s*\[[^\]]*\])?;\s*$)"); // `    int x;`, `    char buf[8];`
    static const Regex struct_declaration(R"(\s+struct \{.*\} \w+;)");
    const std::size_t brace = text.find('{');
    std::string head(text.substr(0, brace == std::string_view::npos ? text.size() : brace + 1));
    const std::string_view rest = brace == std::string_view::npos ? std::string_view{} : text.substr(brace + 1);
    const std::vector<std::string_view> lines = text::split(rest, '\n');
    std::size_t i = 0;
    while (i < lines.size() && (text::trim(lines[i]).empty() || declaration.match(lines[i]).has_value() ||
                                struct_declaration.match(lines[i]).has_value()))
        ++i;
    std::string statements;
    for (std::size_t k = i; k < lines.size(); ++k) {
        if (k > i)
            statements += '\n';
        statements += lines[k];
    }
    for (std::size_t k = 0; k < i; ++k) {
        if (k > 0)
            head += '\n';
        head += lines[k];
    }
    if (i > 0)
        head += '\n';
    return {std::move(head), std::move(statements)};
}

std::string_view prefix_bytes(std::string_view s, std::size_t n) {
    if (s.size() <= n)
        return s;
    while (n > 0 && (static_cast<unsigned char>(s[n]) & 0xC0) == 0x80)
        --n; // not inside a UTF-8 sequence
    return s.substr(0, n);
}

} // namespace

std::string_view word(BreakKind k) {
    constexpr std::string_view words[kBreakKindCount] = {"constant", "comparison", "operator", "store", "arguments"};
    return words[static_cast<std::size_t>(k)];
}

std::string_view word(Fate f) {
    switch (f) {
    case Fate::Caught:
        return "caught";
    case Fate::Survived:
        return "survived";
    case Fate::NotJudged:
        return "not judged";
    case Fate::DoesNotCompile:
        return "does not compile";
    }
    return "";
}

std::vector<TextBreak> breaks(std::string_view text) {
    auto [head, body] = split_body(text);
    const std::string shadow = blank_literals(body);
    using Make = std::optional<std::string> (*)(std::string_view, std::string_view);
    constexpr Make makes[kBreakKindCount] = {break_constant, break_comparison, break_operator, break_store,
                                             break_arguments};
    std::vector<TextBreak> out;
    for (std::size_t k = 0; k < kBreakKindCount; ++k) {
        std::optional<std::string> made = makes[k](body, shadow);
        if (made && *made != body)
            out.push_back({static_cast<BreakKind>(k), head + *made});
    }
    return out;
}

std::string changed(std::string_view before, std::string_view after) {
    const std::vector<std::string_view> a = text::lines(before), b = text::lines(after);
    std::size_t i = 0;
    while (i < std::min(a.size(), b.size()) && a[i] == b[i])
        ++i;
    const std::string_view now =
        (b.size() == a.size() && i < b.size()) ? text::trim(b[i]) : std::string_view("(removed)");
    const std::string_view was = i < a.size() ? text::trim(a[i]) : std::string_view();
    return std::string(prefix_bytes(std::format("{}  ->  {}", was, now), kChangeWidth));
}

std::vector<std::vector<std::uint32_t>> reach(std::span<const NamedText> functions) {
    const std::size_t n = functions.size();
    std::unordered_map<std::string_view, std::uint32_t> index; // lookup only
    for (std::size_t i = 0; i < n; ++i)
        index.emplace(functions[i].name, static_cast<std::uint32_t>(i));
    // what each function's text mentions that is a function of the module (not itself)
    std::vector<std::vector<std::uint32_t>> calls(n);
    for (std::size_t f = 0; f < n; ++f) {
        const std::string_view t = functions[f].text;
        for (std::size_t i = 0; i < t.size();) {
            if (!is_ident_start(t[i])) {
                ++i;
                continue;
            }
            std::size_t j = i;
            while (j < t.size() && is_word(t[j]))
                ++j;
            if (auto it = index.find(t.substr(i, j - i)); it != index.end() && it->second != f)
                calls[f].push_back(it->second);
            i = j;
        }
        std::sort(calls[f].begin(), calls[f].end());
        calls[f].erase(std::unique(calls[f].begin(), calls[f].end()), calls[f].end());
    }
    std::vector<std::vector<std::uint32_t>> out(n);
    std::vector<std::uint32_t> mark(n, UINT32_MAX); // the function whose search last visited each
    std::vector<std::uint32_t> todo;
    for (std::size_t f = 0; f < n; ++f) {
        todo.assign(1, static_cast<std::uint32_t>(f));
        mark[f] = static_cast<std::uint32_t>(f);
        while (!todo.empty()) {
            const std::uint32_t at = todo.back();
            todo.pop_back();
            out[f].push_back(at);
            for (std::uint32_t c : calls[at])
                if (mark[c] != f) {
                    mark[c] = static_cast<std::uint32_t>(f);
                    todo.push_back(c);
                }
        }
        std::sort(out[f].begin(), out[f].end());
    }
    return out;
}

Report run(Lane& lane, int rounds, const Log& log) {
    const std::vector<Function>& functions = lane.functions();
    std::vector<NamedText> texts;
    texts.reserve(functions.size());
    for (const Function& f : functions)
        texts.push_back({f.name, f.text});
    const std::vector<std::vector<std::uint32_t>> reaches = reach(texts);

    // the functions the judge passed that a break applies to, in build order
    std::vector<std::uint32_t> candidates;
    for (std::uint32_t i = 0; i < functions.size(); ++i)
        if (functions[i].passed && !breaks(functions[i].text).empty())
            candidates.push_back(i);

    // Each round takes the candidates not yet broken, in order, skipping any that reaches or is reached by one
    // already chosen: the breaks of a round never reach one another.
    std::vector<char> used(functions.size(), 0);
    int round_number = 0;
    auto next_round = [&]() -> std::optional<Round> {
        if (round_number >= rounds)
            return std::nullopt;
        std::vector<std::uint32_t> chosen;
        std::vector<char> taken(functions.size(), 0);
        for (std::uint32_t n : candidates) {
            if (used[n])
                continue;
            bool clash = false;
            for (std::uint32_t x : reaches[n])
                clash |= taken[x] != 0;
            for (std::uint32_t c : chosen)
                clash |= std::binary_search(reaches[c].begin(), reaches[c].end(), n);
            if (clash)
                continue;
            chosen.push_back(n);
            for (std::uint32_t x : reaches[n])
                taken[x] = 1;
        }
        if (chosen.empty())
            return std::nullopt;
        Round batch;
        for (std::size_t i = 0; i < chosen.size(); ++i) {
            used[chosen[i]] = 1;
            const std::vector<TextBreak> options = breaks(functions[chosen[i]].text);
            const TextBreak& pick =
                options[(i + static_cast<std::size_t>(round_number)) % options.size()]; // the kinds in turn
            batch.push_back({functions[chosen[i]].name, std::string(word(pick.kind)), pick.text});
        }
        ++round_number;
        return batch;
    };

    std::unordered_map<std::string_view, std::uint32_t> by_name; // lookup only
    for (std::uint32_t i = 0; i < functions.size(); ++i)
        by_name.emplace(functions[i].name, i);

    auto judge = [&](const Round& batch) {
        std::vector<Replacement> broken;
        for (const Attempt& a : batch)
            broken.push_back({by_name.at(a.name), a.made});
        return lane.judge(broken);
    };
    auto outcome = [](const Said& said) {
        if (is_good(said.verdict))
            return Outcome::Missed;
        return said.verdict == Verdict::Divergent ? Outcome::Caught : Outcome::Other;
    };

    Report report;
    int shown_round = 0;
    auto each = [&](std::span<const Planted<Said>> planted) {
        for (const Planted<Said>& p : planted) {
            const Function& original = functions[by_name.at(p.attempt->name)];
            Result x;
            x.name = p.attempt->name;
            x.was = *original.passed;
            x.made = p.attempt->what;
            x.reason = p.said.reason;
            if (!p.said.verdict) {
                x.now = Fate::DoesNotCompile;
            } else {
                x.change = changed(original.text, p.attempt->made);
                x.now = p.outcome == Outcome::Missed   ? Fate::Survived
                        : p.outcome == Outcome::Caught ? Fate::Caught
                                                       : Fate::NotJudged;
                x.limits = text::join(p.said.limits, ",");
            }
            report.results.push_back(std::move(x));
        }
        ++shown_round;
        if (log) {
            std::size_t caught = 0, done = 0;
            for (const Result& x : report.results)
                if (x.now == Fate::Caught || x.now == Fate::Survived) {
                    ++done;
                    caught += x.now == Fate::Caught;
                }
            log(std::format("  round {}: {} functions broken; so far {} of {} caught", shown_round, planted.size(),
                            caught, done));
        }
    };
    plant<Said>(next_round, judge, outcome, each);

    std::map<std::string, Group> tally; // sorted by name: the report lists the groups in that order
    for (const Result& x : report.results) {
        if (x.now != Fate::Caught && x.now != Fate::Survived)
            continue;
        for (const std::string& group : {std::string(word(x.was)), "by break: " + x.made}) {
            Group& g = tally[group];
            g.name = group;
            g.caught += x.now == Fate::Caught;
            g.of += 1;
        }
    }
    for (auto& [name, g] : tally)
        report.caught.push_back(std::move(g));
    report.module = lane.module_name();
    report.kind = lane.kind();
    report.rounds = rounds;
    return report;
}

std::string report_json(const Report& report) {
    Json results = Json::array();
    for (const Result& x : report.results) {
        Json r = Json::object();
        r["name"] = x.name;
        r["was"] = std::string(word(x.was));
        if (x.now == Fate::DoesNotCompile) { // it never ran: nothing of what it was changed to, no limits
            r["now"] = std::string(word(x.now));
            r["reason"] = x.reason;
            r["made"] = x.made;
        } else {
            r["made"] = x.made;
            r["reason"] = x.reason;
            r["change"] = x.change;
            r["now"] = std::string(word(x.now));
            r["limits"] = x.limits;
        }
        results.push_back(std::move(r));
    }
    Json caught = Json::object();
    for (const Group& g : report.caught)
        caught[g.name] = Json{{"caught", g.caught}, {"of", g.of}};
    Json j = Json::object();
    j["module"] = report.module;
    j["kind"] = report.kind;
    j["rounds"] = report.rounds;
    j["results"] = std::move(results);
    j["caught"] = std::move(caught);
    return json::dump(j, json::Style::Pretty);
}

namespace {

std::string say_group(const Group& g) {
    const std::string_view title = g.name == "verified" ? "it had called VERIFIED"
                                   : g.name == "weak"   ? "it had called VERIFIED (weak)"
                                                        : std::string_view(g.name);
    return std::format("  {:<34} caught {} of {}  ({}%)\n", title, g.caught, g.of,
                       100 * g.caught / std::max<std::size_t>(g.of, 1));
}

int run_command(const Services& services, std::string_view module, int rounds,
                const std::optional<std::string>& json_file) {
    std::optional<std::filesystem::path> path = services.find_module(module);
    if (!path)
        throw NotFoundError(std::filesystem::path(module), 2, "no such module");
    std::unique_ptr<Lane> lane = services.open(*path);
    const Report report = run(*lane, rounds, [](std::string_view line) { out_text(std::string(line) + "\n"); });

    const std::filesystem::path file =
        json_file ? fs::from_text(*json_file) : services.report_dir / (report.module + ".json");
    if (file.has_parent_path())
        fs::make_dirs(file.parent_path());
    fs::write_text(file, report_json(report));

    for (const Group& g : report.caught)
        out_text(say_group(g));
    const std::size_t counted = std::count_if(report.results.begin(), report.results.end(), [](const Result& x) {
        return x.now == Fate::Caught || x.now == Fate::Survived;
    });
    if (counted < report.results.size())
        out_text(std::format("  not counted: {} (the break did not compile, or could not be judged)\n",
                             report.results.size() - counted));
    std::size_t shown = 0;
    for (const Result& x : report.results) {
        if (x.now != Fate::Survived || x.was != Class::Verified)
            continue;
        if (shown++ >= kSurvivorsShown)
            break;
        out_text(std::format("    survived: {:<24} {:<10} {}\n", x.name, x.made, x.limits));
    }
    out_text(std::format("written to {}\n", fs::to_text(file)));
    return kExitOk;
}

} // namespace

cli::Command command(Services services) {
    return cli::Command{
        "teeth",
        "break functions the judge passed, on purpose: does it notice?",
        {cli::Positional{"module", "a corpus name or a file"},
         cli::Option{"--rounds", "", "rounds of breaks (each function is broken once)", cli::Arg::Value, "N", "3"},
         cli::Option{"--json", "", "where the report is written (default: the report folder, named after the module)",
                     cli::Arg::Value, "FILE"}},
        [services = std::move(services)](const cli::Args& args) {
            const std::optional<int> rounds = args.integer<int>("rounds");
            if (!rounds || *rounds < 0)
                throw UsageError("--rounds expects a number of rounds, 0 or more");
            return run_command(services, args.required("module"), *rounds, args.value("json"));
        },
        {},
        "",
        "Break functions the judge has passed, one small way each, and see whether it notices."};
}

} // namespace cb::verify::teeth
