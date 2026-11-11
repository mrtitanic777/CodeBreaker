#include "cb/verify/authored/search16.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <set>

#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::verify::authored::search16 {

namespace {

// std::string::contains is C++23; this answers for strings and for sets and maps alike
template <class Haystack, class Needle>
bool contains(const Haystack& haystack, const Needle& needle) {
    if constexpr (requires { haystack.find(needle) != Haystack::npos; })
        return haystack.find(needle) != Haystack::npos;
    else
        return haystack.contains(needle);
}

// ---- the patterns of a line of the decompiler's C -------------------------------------------------------
// (each is compiled once; a line that cannot hold the pattern's punctuation is not offered to it)

// `    int4 x;`: indent, type, the gap (and stars) before the name, name, the rest up to the `;`
const Regex& decl_pattern() {
    static const Regex re(R"re(^(\s+)((?:unsigned |signed )?\w+)(\s+\**\s*)(\w+)(\s*(?:\[[^\]]*\])?\s*;)\s*$)re");
    return re;
}
// a comparison with a constant, as the decompiler normalizes it (`x <= 0x19`)
const Regex& compare_pattern() {
    static const Regex re(R"re((?<![<>=!])(<=|>=|<|>)\s*(-?)(0x[0-9a-fA-F]+|\d+)\b)re");
    return re;
}
// the constant first (`(0x19 < iVar1)`)
const Regex& left_pattern() {
    static const Regex re(R"re(\(\s*(-?)(0x[0-9a-fA-F]+|\d+)\s*(<=|>=|<|>)\s*([\w.\[\]]+)\s*\))re");
    return re;
}
const Regex& if_pattern() {
    static const Regex re(R"re(^(\s*)if \((.*)\) \{\s*$)re");
    return re;
}
const Regex& condition_pattern() {
    static const Regex re(R"re(\s*([^()&|!<>=]+?)\s*(==|!=|<=|>=|<|>)\s*([^()&|!<>=]+?)\s*)re");
    return re;
}
// a simple statement: not a declaration of a control line
const Regex& statement_pattern() {
    static const Regex re(
        R"re(^(\s+)(?!return\b|if\b|else\b|while\b|for\b|do\b|switch\b|case\b|default\b|goto\b|break\b|continue\b)[^{};]*;\s*$)re");
    return re;
}
const Regex& self_pattern() {   // x = x + y
    static const Regex re(R"re(^(\s*)([\w.\[\]]+) = \2 (\+|-|\*|&|\||\^|<<|>>) (.+);\s*$)re");
    return re;
}
const Regex& compound_pattern() {   // x += y
    static const Regex re(R"re(^(\s*)([\w.\[\]]+) (\+|-|\*|&|\||\^|<<|>>)= (.+);\s*$)re");
    return re;
}
const Regex& commute_pattern() {   // (a == b)
    static const Regex re(R"re(\(([\w.]+) (==|!=|\+|\*|&|\|) ([\w.]+|0x[0-9a-fA-F]+|\d+)\))re");
    return re;
}

// Necessary conditions of the patterns above: a line without them cannot match, so the regex engine need not be asked.
bool may_end_statement(std::string_view s) { return contains(s, ';'); }
bool may_compare(std::string_view s) { return contains(s, '<') || contains(s, '>'); }
bool may_left(std::string_view s) { return contains(s, '(') && may_compare(s); }
bool may_if(std::string_view s) { return contains(s, '{') && contains(s, "if ("); }
bool may_assign(std::string_view s) { return contains(s, '=') && contains(s, ';'); }

std::optional<RegexMatch> decl_of(const std::string& line) {
    return may_end_statement(line) ? decl_pattern().match(line) : std::nullopt;
}
bool is_statement(const std::string& line) { return may_end_statement(line) && statement_pattern().match(line); }
std::optional<RegexMatch> self_of(const std::string& line) { return may_assign(line) ? self_pattern().match(line) : std::nullopt; }
std::optional<RegexMatch> compound_of(const std::string& line) { return may_assign(line) ? compound_pattern().match(line) : std::nullopt; }
std::optional<RegexMatch> if_of(const std::string& line) { return may_if(line) ? if_pattern().match(line) : std::nullopt; }

// ---- constants and signs ----------------------------------------------------------------------------
// int1 <-> uint1 ..., and what an unknown size becomes
std::optional<std::string_view> flipped_sign(std::string_view type) {
    static const std::array<std::pair<std::string_view, std::string_view>, 9> table = {{
        {"int1", "uint1"}, {"uint1", "int1"}, {"int2", "uint2"}, {"uint2", "int2"}, {"int4", "uint4"},
        {"uint4", "int4"}, {"xunknown2", "int2"}, {"xunknown4", "int4"}, {"xunknown1", "int1"}}};
    for (const auto& [from, to] : table) {
        if (type == from) return to;
    }
    return std::nullopt;
}

// A constant of C source (decimal or 0x hex, with the sign it was written with). nullopt for an octal one (leading 0)
// or one past 64 bits.
std::optional<std::int64_t> constant_value(std::string_view digits, bool negative) {
    if (digits.size() > 1 && digits[0] == '0' && digits[1] != 'x' && digits.find_first_not_of('0') != std::string_view::npos) return std::nullopt;   // octal (zeros alone are zero)
    const auto v = text::parse_int<std::int64_t>(digits, 0);
    if (!v || *v == INT64_MIN) return std::nullopt;
    return negative ? -*v : *v;
}

// "-0x1a" or "0x1a"
std::string hex_constant(std::int64_t v) {
    return v < 0 ? std::format("-0x{:x}", static_cast<std::uint64_t>(-v)) : std::format("0x{:x}", v);
}

struct Flip {
    std::string_view op;
    std::int64_t offset;   // what the constant moves by
};
// `x <= N` is `x < N+1`, and so on
std::optional<Flip> flip_of(std::string_view op) {
    if (op == "<=") return Flip{"<", 1};
    if (op == "<") return Flip{"<=", -1};
    if (op == ">=") return Flip{">", -1};
    if (op == ">") return Flip{">=", 1};
    return std::nullopt;
}

// One thing a rewrite can do to a comparison: the text to put where the match was.
struct Edit {
    std::size_t line = 0, begin = 0, end = 0;
    std::string replacement;
};

// A comparison with a constant written the other way: `x <= 0x19` as `x < 0x1a`.
std::optional<std::string> flipped_compare(const RegexMatch& m) {
    const auto value = constant_value(m.group(3), !m.group(2).empty());
    const auto flip = flip_of(m.group(1));
    if (!value || !flip) return std::nullopt;
    return std::format("{} {}", flip->op, hex_constant(*value + flip->offset));
}

// `(0x19 < x)`, the constant first, as `(x >= 0x1a)`.
std::optional<std::string> flipped_left(const RegexMatch& m) {
    const auto value = constant_value(m.group(2), !m.group(1).empty());
    if (!value) return std::nullopt;
    // 0x19 < x  means  x > 0x19, which is x >= 0x1a
    std::string_view op;
    std::int64_t offset = 0;
    const std::string_view written = m.group(3);
    if (written == "<") { op = ">="; offset = 1; }
    else if (written == "<=") { op = ">"; offset = -1; }
    else if (written == ">") { op = "<="; offset = -1; }
    else { op = "<"; offset = 1; }
    return std::format("({} {} {})", m.group(4), op, hex_constant(*value + offset));
}

std::string_view opposite_operator(std::string_view op) {
    if (op == "==") return "!=";
    if (op == "!=") return "==";
    if (op == "<") return ">=";
    if (op == ">=") return "<";
    if (op == ">") return "<=";
    return ">";   // "<="
}

std::size_t indent_of(std::string_view line) { return line.size() - text::ltrim(line).size(); }

// the index of the first line that is `{` (the start of the body), else nullopt
std::optional<std::size_t> body_start(const std::vector<std::string>& lines) {
    for (std::size_t k = 0; k < lines.size(); ++k) {
        if (text::trim(lines[k]) == "{") return k;
    }
    return std::nullopt;
}

std::size_t below(Rng& rng, std::size_t n) { return static_cast<std::size_t>(rng() % n); }
template <class T>
const T& choice(Rng& rng, const std::vector<T>& items) { return items[below(rng, items.size())]; }
double unit_random(Rng& rng) { return static_cast<double>(rng() >> 11) * (1.0 / 9007199254740992.0); }

enum class Kind { Order, Sign, Compare, Branch, Swap, Assign, Commute };

}  // namespace

std::vector<std::size_t> leading_declarations(const std::vector<std::string>& lines) {
    std::vector<std::size_t> out;
    const auto start = body_start(lines);
    if (!start) return out;
    for (std::size_t k = *start + 1; k < lines.size(); ++k) {
        if (decl_of(lines[k])) out.push_back(k);
        else if (!text::trim(lines[k]).empty()) break;
    }
    return out;
}

std::string negate_condition(std::string_view cond) {
    if (const auto m = condition_pattern().full_match(cond))
        return std::format("{} {} {}", m->group(1), opposite_operator(m->group(2)), m->group(3));
    return std::format("!({})", cond);
}

std::vector<IfElse> if_else_blocks(const std::vector<std::string>& lines) {
    std::vector<IfElse> out;
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = if_of(lines[k]);
        if (!m) continue;
        const std::string indent(m->group(1));
        std::optional<std::size_t> close;
        for (std::size_t j = k + 1; j < lines.size(); ++j) {
            if (lines[j] == indent + "}") { close = j; break; }
        }
        if (!close || *close + 1 >= lines.size() || lines[*close + 1] != indent + "else {") continue;
        for (std::size_t j = *close + 2; j < lines.size(); ++j) {
            if (lines[j] == indent + "}") {
                out.push_back({k, *close + 1, j});
                break;
            }
        }
    }
    return out;
}

std::vector<std::size_t> swappable_statements(const std::vector<std::string>& lines, std::size_t from) {
    std::vector<std::size_t> out;
    for (std::size_t k = from; k + 1 < lines.size(); ++k) {
        const std::string& a = lines[k];
        const std::string& b = lines[k + 1];
        if (is_statement(a) && is_statement(b) && indent_of(a) == indent_of(b) && !decl_of(a) && !decl_of(b) &&
            (contains(a, " = ") || contains(b, " = ")))
            out.push_back(k);
    }
    return out;
}

std::optional<std::string> rewrite(std::string_view text_in, Rng& rng) {
    std::vector<std::string> lines;
    for (const std::string_view l : text::split(text_in, '\n')) lines.emplace_back(l);
    const auto decls = leading_declarations(lines);
    const std::size_t body = body_start(lines).value_or(0);

    std::vector<Kind> kinds;
    if (decls.size() > 1) kinds.push_back(Kind::Order);
    std::vector<std::size_t> signed_decls;   // the declarations whose type has another sign
    for (const std::size_t k : decls) {
        if (flipped_sign(decl_of(lines[k])->group(2))) signed_decls.push_back(k);
    }
    if (!signed_decls.empty()) kinds.push_back(Kind::Sign);

    std::vector<Edit> compares;
    for (std::size_t k = body; k < lines.size(); ++k) {
        if (may_compare(lines[k])) {
            compare_pattern().for_each_match(lines[k], [&](const RegexMatch& m) {
                if (auto r = flipped_compare(m)) compares.push_back({k, m.start(), m.end(), std::move(*r)});
                return true;
            });
        }
    }
    for (std::size_t k = body; k < lines.size(); ++k) {
        if (may_left(lines[k])) {
            left_pattern().for_each_match(lines[k], [&](const RegexMatch& m) {
                if (auto r = flipped_left(m)) compares.push_back({k, m.start(), m.end(), std::move(*r)});
                return true;
            });
        }
    }
    if (!compares.empty()) kinds.insert(kinds.end(), 2, Kind::Compare);

    const auto branches = if_else_blocks(lines);
    if (!branches.empty()) kinds.insert(kinds.end(), 2, Kind::Branch);
    const auto swaps = swappable_statements(lines, body + 1);
    if (!swaps.empty()) kinds.push_back(Kind::Swap);
    std::vector<std::size_t> assigns;
    for (std::size_t k = body; k < lines.size(); ++k) {
        if (self_of(lines[k]) || compound_of(lines[k])) assigns.push_back(k);
    }
    if (!assigns.empty()) kinds.push_back(Kind::Assign);
    std::vector<Edit> commutes;
    for (std::size_t k = body; k < lines.size(); ++k) {
        if (!contains(lines[k], '(')) continue;
        commute_pattern().for_each_match(lines[k], [&](const RegexMatch& m) {   // (a == b) -> (b == a)
            commutes.push_back({k, m.start(), m.end(), std::format("({} {} {})", m.group(3), m.group(2), m.group(1))});
            return true;
        });
    }
    if (!commutes.empty()) kinds.push_back(Kind::Commute);
    if (kinds.empty()) return std::nullopt;

    switch (choice(rng, kinds)) {
        case Kind::Swap: {   // two statements in the other order
            const std::size_t k = choice(rng, swaps);
            std::swap(lines[k], lines[k + 1]);
            break;
        }
        case Kind::Assign: {   // `x = x + y` <-> `x += y`, and `x += 1` <-> `x++`
            const std::size_t k = choice(rng, assigns);
            if (const auto m = self_of(lines[k])) {
                lines[k] = std::format("{}{} {}= {};", m->group(1), m->group(2), m->group(3), m->group(4));
            } else {
                const auto c = compound_of(lines[k]);
                const std::string indent(c->group(1)), var(c->group(2)), op(c->group(3)), rhs(c->group(4));
                const std::string_view one = text::trim(rhs);
                if ((op == "+" || op == "-") && (one == "1" || one == "0x1") && unit_random(rng) < 0.5)
                    lines[k] = std::format("{}{}{}{};", indent, var, op, op);
                else
                    lines[k] = std::format("{}{} = {} {} {};", indent, var, var, op, rhs);
            }
            break;
        }
        case Kind::Commute: {
            const Edit& e = choice(rng, commutes);
            lines[e.line].replace(e.begin, e.end - e.begin, e.replacement);
            break;
        }
        case Kind::Branch: {   // `if (c) {A} else {B}` -> `if (!c) {B} else {A}`
            const IfElse& b = choice(rng, branches);
            const auto m = if_of(lines[b.if_line]);
            const std::string indent(m->group(1));
            std::vector<std::string> a(lines.begin() + static_cast<std::ptrdiff_t>(b.if_line) + 1, lines.begin() + static_cast<std::ptrdiff_t>(b.else_line) - 1);
            std::vector<std::string> other(lines.begin() + static_cast<std::ptrdiff_t>(b.else_line) + 1, lines.begin() + static_cast<std::ptrdiff_t>(b.end_line));
            std::vector<std::string> block{indent + "if (" + negate_condition(m->group(2)) + ") {"};
            block.insert(block.end(), other.begin(), other.end());
            block.push_back(indent + "}");
            block.push_back(indent + "else {");
            block.insert(block.end(), a.begin(), a.end());
            block.push_back(indent + "}");
            lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(b.if_line), lines.begin() + static_cast<std::ptrdiff_t>(b.end_line) + 1);
            lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(b.if_line), block.begin(), block.end());
            break;
        }
        case Kind::Compare: {
            const Edit& e = choice(rng, compares);
            lines[e.line].replace(e.begin, e.end - e.begin, e.replacement);
            break;
        }
        case Kind::Order: {   // two locals in the other order
            const std::size_t a = below(rng, decls.size());
            std::size_t b = below(rng, decls.size() - 1);
            if (b >= a) ++b;   // two different ones
            std::swap(lines[decls[a]], lines[decls[b]]);
            break;
        }
        case Kind::Sign: {
            const std::size_t k = choice(rng, signed_decls);
            const auto m = decl_of(lines[k]);
            lines[k] = std::format("{}{}{}{}{}", m->group(1), *flipped_sign(m->group(2)), m->group(3), m->group(4), m->group(5));
            break;
        }
    }
    return text::join(lines, "\n");
}

std::vector<Best> search(const std::vector<NearMiss>& near, const CompileGroup& compile_group, const Score& score,
                         const Options& options) {
    Rng rng(options.seed);
    std::vector<Best> best;
    for (const NearMiss& n : near) best.push_back({n.function, n.text, n.verdict});
    for (int round = 0; round < options.rounds; ++round) {
        std::map<std::string, std::string> tries;   // function -> its rewritten text
        for (const Best& b : best) {
            if (b.verdict.match == ByteMatch::Identical) continue;
            if (auto changed = rewrite(b.text, rng); changed && *changed != b.text) tries[b.function] = std::move(*changed);
        }
        if (tries.empty()) break;
        std::size_t gained = 0;
        std::set<Group> groups;
        for (const NearMiss& n : near) {
            if (contains(tries, n.function)) groups.insert(n.group);
        }
        for (const Group& g : groups) {
            std::map<std::string, std::string> mine;
            for (const NearMiss& n : near) {
                if (n.group == g && contains(tries, n.function)) mine[n.function] = tries[n.function];
            }
            for (const auto& [function, verdict] : compile_group(g, mine)) {
                const auto it = std::ranges::find(best, function, &Best::function);
                if (it == best.end() || !contains(mine, function)) continue;   // (a function that was not asked about)
                if (score(verdict) > score(it->verdict)) {
                    *it = {function, mine[function], verdict};
                    ++gained;
                }
            }
        }
        if (options.log) {
            const auto identical = std::ranges::count_if(best, [](const Best& b) { return b.verdict.match == ByteMatch::Identical; });
            options.log(std::format("  round {}: {} rewrites, {} nearer, {} identical", round + 1, tries.size(), gained, identical));
        }
    }
    return best;
}

}  // namespace cb::verify::authored::search16
