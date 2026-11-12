// cb-unity: alone
#include "cb/verify/authored/repair.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <functional>
#include <set>
#include <thread>

#include "cb/base/ctext.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/verify/authored/shape.hpp"

namespace cb::verify::authored::repair {

namespace {

// ---- the patterns the rewrites look for their places with ------------------------------------------------------------

bool has(std::string_view hay, std::string_view needle) { return hay.find(needle) != std::string_view::npos; }

constexpr std::string_view kId = R"((?:[A-Za-z_]\w*|0[xX][0-9a-fA-F]+|\d+))";
constexpr std::string_view kName = R"([A-Za-z_]\w*)";

std::string join_parts(std::initializer_list<std::string_view> parts) {
    std::string out;
    for (const std::string_view p : parts) out += p;
    return out;
}

// An expression that is `a op b` and nothing more: it begins after ( = , ; { or `return`, and ends at ) ; or , -- so no
// neighbour's precedence can take an operand away.
const Regex& pair_pattern() {
    static const Regex re(join_parts({R"((?<pre>(?:[(,;{]|(?<![=!<>])=)\s*|\breturn\s+)(?<a>)", kId,
                                      R"()\s*(?<op>==|!=|<=|>=|[-+*&|^<>])\s*(?<b>)", kId, R"()(?<post>\s*[);,]))"}));
    return re;
}
// A statement that only steps a variable: x++;  ++x;  x += 1;  x = x + 1;
const Regex& step_pattern() {
    static const Regex re(join_parts({R"((?<pre>[;{}]\s*|\belse\s+)(?:(?<x1>)", kName, R"()\s*(?<o1>\+\+|--)|(?<o2>\+\+|--)\s*(?<x2>)", kName,
                                      R"()|(?<x3>)", kName, R"()\s*(?<o3>[-+])=\s*1)(?<post>\s*[;)]))"}));
    return re;
}
// x = x op y;
const Regex& long_pattern() {
    static const Regex re(join_parts({R"((?<pre>[;{}]\s*|\belse\s+)(?<x>)", kName, R"()\s*=\s*\2\s*(?<op>[-+*&|^])\s*(?<y>)", kId,
                                      R"()(?<post>\s*[;)]))"}));
    return re;
}
// x op= y;
const Regex& short_pattern() {
    static const Regex re(join_parts({R"((?<pre>[;{}]\s*|\belse\s+)(?<x>)", kName, R"()\s*(?<op>[-+*&|^])=\s*(?<y>)", kId,
                                      R"()(?<post>\s*[;)]))"}));
    return re;
}
// a declaration line: a type, pointer stars, a name, perhaps one array dimension
const Regex& decl_pattern() {
    static const Regex re(R"(^[A-Za-z_][\w ]*\**\s*[A-Za-z_]\w*(\s*\[[^\]]*\])?\s*;$)");
    return re;
}
const Regex& spare_pattern() {
    static const Regex re(R"(uint1 _rest\[(\d+)\]; \} _f;)");
    return re;
}
const Regex& call_pattern() {
    static const Regex re(R"([A-Za-z_]\w*\s*\()");
    return re;
}
const Regex& if_while_pattern() {
    static const Regex re(R"(\b(?:if|while)\s*\()");
    return re;
}

bool is_decl(std::string_view line) { return decl_pattern().test(line); }

// The operand of a commutative operator the other way round: a < b is b > a.
std::string_view commuted(std::string_view op) {
    if (op == "+" || op == "*" || op == "&" || op == "|" || op == "^" || op == "==" || op == "!=") return op;
    if (op == "<") return ">";
    if (op == ">") return "<";
    if (op == "<=") return ">=";
    return "<=";   // ">="
}

std::string blank_code(std::string_view text) { return base::ctext::blank(text, base::ctext::Mode::WholeClosed); }

std::ptrdiff_t paren_depth(std::string_view shadow, std::size_t at) {
    std::ptrdiff_t d = 0;
    for (std::size_t i = 0; i < at && i < shadow.size(); ++i) d += (shadow[i] == '(') - (shadow[i] == ')');
    return d;
}

std::string joined_lines(const std::vector<std::string>& lines) { return text::join(lines, "\n"); }

// "\n" + the declarations, or nothing where there are none.
std::string decls_block(const std::vector<std::string>& decls) { return decls.empty() ? std::string() : "\n" + joined_lines(decls); }

// ---- ordering by instruction -------------------------------------------------------------------------------------------

// The total size of the matching blocks of difflib's longest-matching-block algorithm (no junk): the earliest longest
// block first, then what lies either side of it.
std::size_t matching_total(const std::vector<std::string>& a, const std::vector<std::string>& b) {
    struct Region {
        std::size_t alo, ahi, blo, bhi;
    };
    std::size_t total = 0;
    std::vector<Region> queue = {{0, a.size(), 0, b.size()}};
    std::vector<std::size_t> prev, cur;
    while (!queue.empty()) {
        const Region q = queue.back();
        queue.pop_back();
        // the longest common run in a[alo, ahi) x b[blo, bhi); the earliest in a, then in b, among the longest
        std::size_t best_i = q.alo, best_j = q.blo, best_k = 0;
        prev.assign(q.bhi - q.blo + 1, 0);
        for (std::size_t i = q.alo; i < q.ahi; ++i) {
            cur.assign(q.bhi - q.blo + 1, 0);
            for (std::size_t j = q.blo; j < q.bhi; ++j) {
                if (a[i] != b[j]) continue;
                const std::size_t k = prev[j - q.blo] + 1;
                cur[j - q.blo + 1] = k;
                if (k > best_k) {
                    best_i = i + 1 - k;
                    best_j = j + 1 - k;
                    best_k = k;
                }
            }
            std::swap(prev, cur);
        }
        if (best_k == 0) continue;
        total += best_k;
        if (q.alo < best_i && q.blo < best_j) queue.push_back({q.alo, best_i, q.blo, best_j});
        if (best_i + best_k < q.ahi && best_j + best_k < q.bhi) queue.push_back({best_i + best_k, q.ahi, best_j + best_k, q.bhi});
    }
    return total;
}

std::string word_bytes(std::uint32_t w) {
    return std::string{static_cast<char>(w >> 24), static_cast<char>(w >> 16), static_cast<char>(w >> 8), static_cast<char>(w)};
}

}  // namespace

// ---- the rewrites ------------------------------------------------------------------------------------------------------

Split split(std::string_view source) {
    const std::size_t brace = source.find('{');
    if (brace == std::string_view::npos) return {std::string(source), {}, {}};
    const std::vector<std::string_view> lines = text::split(source.substr(brace + 1), '\n');
    Split out;
    out.head = std::string(source.substr(0, brace + 1));
    std::size_t i = 0;
    for (; i < lines.size(); ++i) {
        const std::string_view s = text::trim(lines[i]);
        if (!s.empty() && !is_decl(s)) break;
        if (!s.empty()) out.decls.emplace_back(lines[i]);
    }
    std::string rest;
    for (std::size_t k = i; k < lines.size(); ++k) {
        if (k > i) rest += '\n';
        rest += lines[k];
    }
    out.body = std::move(rest);
    return out;
}

std::vector<TestSwap> test_swaps(std::string_view shadow, std::string_view body) {
    std::vector<TestSwap> out;
    std::size_t from = 0;
    while (const auto m = if_while_pattern().search(shadow, from)) {
        from = m->end();
        const std::size_t i = m->end();
        std::size_t depth = 1, j = i;
        while (j < shadow.size() && depth) {
            depth += shadow[j] == '(';
            depth -= shadow[j] == ')';
            ++j;
        }
        if (depth) continue;
        const std::string_view inner = shadow.substr(i, j - 1 - i);
        std::vector<std::pair<std::size_t, std::string>> ops;
        int d = 0;
        bool plain = true;
        for (std::size_t k = 0; k < inner.size(); ++k) {
            const char c = inner[k];
            const std::string_view two = inner.substr(k, 2);
            if (c == '(' || c == '[') {
                ++d;
            } else if (c == ')' || c == ']') {
                --d;
            } else if (two == "++" || two == "--" || two == "+=" || two == "-=" || two == "*=" || two == "/=" || two == "|=" || two == "&=" ||
                       two == "^=" || two == "%=") {
                plain = false;
            } else if (d == 0 && (two == "&&" || two == "||" || c == '?' || c == ',')) {
                plain = false;
            } else if (two == "<<" || two == ">>" || two == "->") {
                ++k;
            } else if (two == "==" || two == "!=" || two == "<=" || two == ">=") {
                if (d == 0) ops.emplace_back(k, std::string(two));
                ++k;
            } else if (c == '<' || c == '>') {
                if (d == 0) ops.emplace_back(k, std::string(1, c));
            } else if (c == '=') {
                plain = false;
            } else if (d == 0 && (c == '&' || c == '|' || c == '^')) {
                plain = false;   // binds looser than a comparison
            }
        }
        if (!plain || ops.size() != 1 || call_pattern().search(inner, 0)) continue;
        const auto& [at, op] = ops[0];
        const std::string_view left = text::trim(body.substr(i, at));
        const std::string_view right = text::trim(body.substr(i + at + op.size(), j - 1 - (i + at + op.size())));
        if (!left.empty() && !right.empty() && left != right)
            out.push_back({i, j - 1, std::format("{} {} {}", right, commuted(op), left)});
    }
    return out;
}

std::optional<std::string> toggle_register(std::string_view line) {
    const std::string_view s = text::trim(line);
    if (has(s, "[") || s.starts_with("static ") || s.starts_with("extern ") || !is_decl(s)) return std::nullopt;
    const std::string indent(line.substr(0, line.size() - text::ltrim(line).size()));
    const std::string other = s.starts_with("register ") ? std::string(s.substr(9)) : "register " + std::string(s);
    const auto words = text::split_ws(other);
    if (words.size() == 1 && (words[0] == "register" || words[0] == "int;")) return std::nullopt;
    return indent + other;
}

std::vector<Knob> knobs(std::string_view source, std::size_t target_frame, std::optional<std::size_t> draft_frame) {
    const Split parts = split(source);
    const std::string& head = parts.head;
    const std::vector<std::string>& decls = parts.decls;
    const std::string& body = parts.body;
    const std::string top = head + decls_block(decls) + "\n";
    std::vector<Knob> out;
    auto add = [&](std::string name, std::string text) { out.push_back({std::move(name), std::move(text)}); };

    // frame: the draft keeps the original's locals in one block with spare room at its end; the room is made what the
    // original's frame says it was
    if (const auto spare = spare_pattern().search(source, 0); spare && target_frame && draft_frame && *draft_frame != target_frame) {
        const auto have = text::parse_int<std::int64_t>(spare->group(1));
        const std::int64_t room = (have ? *have : 0) - (static_cast<std::int64_t>(*draft_frame) - static_cast<std::int64_t>(target_frame));
        if (room >= 0) {
            const std::string made = room ? std::format("uint1 _rest[{}]; }} _f;", room) : "} _f;";
            add(std::format("frame{}", room), std::string(source.substr(0, spare->start())) + made + std::string(source.substr(spare->end())));
        }
    }
    // pad: the original reserves more stack than the draft -- an unused local of the difference
    if (target_frame && draft_frame && target_frame > *draft_frame) {
        const std::size_t gap = target_frame - *draft_frame;
        add(std::format("pad{}", gap), head + "\n" + std::format("  char codebreaker_unused[{}];", gap) + decls_block(decls) + "\n" + body);
    }
    // order: two of the leading locals change places (the frame is laid out in declaration order; any order is some
    // changes of two, and the search goes on from each)
    const std::size_t lead = std::min(decls.size(), kMaxLocals);
    std::size_t n = 0;
    for (std::size_t i = 0; i < lead; ++i)
        for (std::size_t j = i + 1; j < lead; ++j, ++n) {
            std::vector<std::string> moved = decls;
            std::swap(moved[i], moved[j]);
            if (moved != decls) add(std::format("order{}", n), head + "\n" + joined_lines(moved) + "\n" + body);
        }
    // register: a local kept in a register, or not (the compilers of the time did as they were told)
    for (std::size_t i = 0; i < std::min(decls.size(), kMaxLocals); ++i)
        if (const auto other = toggle_register(decls[i])) {
            std::vector<std::string> changed = decls;
            changed[i] = *other;
            add(std::format("register{}", i), head + "\n" + joined_lines(changed) + "\n" + body);
        }
    // regparam: the same for an argument, where each is declared on a line of its own (f(a) int a; {)
    if (const std::size_t close = head.rfind(')'); close != std::string::npos) {
        std::vector<std::string> lines;
        for (const std::string_view l : text::split(std::string_view(head).substr(close + 1), '\n')) lines.emplace_back(l);
        for (std::size_t i = 0; i < lines.size(); ++i) {
            const auto other = text::trim(lines[i]).empty() ? std::nullopt : toggle_register(lines[i]);
            if (!other) continue;
            std::vector<std::string> changed = lines;
            changed[i] = *other;
            add(std::format("regparam{}", i), head.substr(0, close + 1) + joined_lines(changed) + decls_block(decls) + "\n" + body);
        }
    }
    const std::string shadow = blank_code(body);
    auto splice = [&](std::size_t from, std::size_t to, std::string_view replacement) {
        return top + body.substr(0, from) + std::string(replacement) + body.substr(to);
    };
    // swap: the operands of a commutative operator the other way round; a < b as b > a
    {
        std::size_t site = 0, from = 0;
        while (site < kMaxSites) {
            const auto m = pair_pattern().search(shadow, from);
            if (!m) break;
            from = m->end() > m->start() ? m->end() : m->end() + 1;
            if (m->named("op").value() == "-" || m->named("a") == m->named("b")) continue;   // a - b is not commutative
            const std::string made = std::format("{}{} {} {}{}", m->named("pre").value(), m->named("b").value(),
                                                 commuted(m->named("op").value()), m->named("a").value(), m->named("post").value());
            add(std::format("swap{}", site++), splice(m->start(), m->end(), made));
        }
    }
    // switch: a chain of `if (v == A) .. else if (v == B) ..` as a switch on v
    {
        std::size_t k = 0;
        for (const shape::Rewrite& r : shape::switches(body, shadow)) {
            if (k >= kMaxSites) break;
            add(std::format("switch{}", k++), splice(r.begin, r.end, r.text));
        }
    }
    // fold: `t = f(..);` and at once `if (t == N)`, as `if (f(..) == N)`
    {
        std::size_t k = 0;
        for (const shape::Rewrite& r : shape::folds(decls, body, shadow)) {
            if (k >= kMaxSites) break;
            add(std::format("fold{}", k++), splice(r.begin, r.end, r.text));
        }
    }
    // return: `r = x;` that only the final `return r;` follows, as `return x;`
    if (const auto early = shape::returns(head, decls, body, shadow)) add("return", top + *early);
    // test: the two sides of the one comparison an `if` or `while` makes, the other way round
    {
        std::size_t k = 0;
        for (const TestSwap& t : test_swaps(shadow, body)) {
            if (k >= kMaxSites) break;
            add(std::format("test{}", k++), splice(t.begin, t.end, t.text));
        }
    }
    // step: the ways of writing a statement that adds or takes one
    {
        std::size_t site = 0, from = 0;
        while (site < kMaxSites) {
            const auto m = step_pattern().search(shadow, from);
            if (!m) break;
            from = m->end() > m->start() ? m->end() : m->end() + 1;
            const std::ptrdiff_t d = paren_depth(shadow, m->start());
            const std::string_view post = text::trim(m->named("post").value());
            if ((post == ";" && d != 0) || (post == ")" && d != 1)) continue;   // the condition of a for: its value is used
            const bool prefix = m->named("x2").has_value(), long_form = m->named("x3").has_value();
            const std::string_view x = prefix ? *m->named("x2") : long_form ? *m->named("x3") : *m->named("x1");
            const std::string_view o = prefix ? *m->named("o2") : long_form ? *m->named("o3") : *m->named("o1");
            const char sign = o[0];
            const std::array<std::string, 3> forms = {std::format("{}{}{}", x, sign, sign), std::format("{}{}{}", sign, sign, x),
                                                      std::format("{} {}= 1", x, sign)};
            const std::size_t mine = prefix ? 1 : long_form ? 2 : 0;
            for (std::size_t k = 0; k < forms.size(); ++k)
                if (k != mine)
                    add(std::format("step{}.{}", site, k),
                        splice(m->start(), m->end(), std::string(m->named("pre").value()) + forms[k] + std::string(m->named("post").value())));
            ++site;
        }
    }
    // assign: x = x + y  and  x += y
    {
        std::size_t site = 0;
        for (const bool long_first : {true, false}) {
            const Regex& re = long_first ? long_pattern() : short_pattern();
            std::size_t from = 0;
            while (site < kMaxSites) {
                const auto m = re.search(shadow, from);
                if (!m) break;
                from = m->end() > m->start() ? m->end() : m->end() + 1;
                const std::ptrdiff_t d = paren_depth(shadow, m->start());
                const std::string_view post = text::trim(m->named("post").value());
                if ((post == ";" && d != 0) || (post == ")" && d != 1)) continue;
                const std::string_view x = *m->named("x"), op = *m->named("op"), y = *m->named("y");
                const std::string form = long_first ? std::format("{} {}= {}", x, op, y) : std::format("{} = {} {} {}", x, x, op, y);
                add(std::format("assign{}", site++),
                    splice(m->start(), m->end(), std::string(m->named("pre").value()) + form + std::string(m->named("post").value())));
            }
        }
    }
    return out;
}

// ---- the search --------------------------------------------------------------------------------------------------------

Searcher::Searcher(Isa isa, Distance distance) : isa_(isa), distance_(std::move(distance)) {}

std::size_t Searcher::frame(ByteView code) {
    if (isa_ == Isa::PowerPC) {
        const std::size_t limit = std::min<std::size_t>(code.size(), 48);
        for (std::size_t k = 0; k + 3 < limit; k += 4) {
            const std::uint32_t w = (std::uint32_t{code[k]} << 24) | (std::uint32_t{code[k + 1]} << 16) | (std::uint32_t{code[k + 2]} << 8) | code[k + 3];
            if (w >> 16 == 0x9421) return (0u - (w & 0xFFFF)) & 0xFFFF;   // stwu r1,-N(r1)
        }
        return 0;
    }
    if (isa_ == Isa::Unknown) return 0;
    const unsigned bits = isa_ == Isa::X86_32 ? 32 : 16;
    const std::string_view sp = bits == 32 ? "esp," : "sp,";
    const platforms::Decoder& cs = decoders_.get(platforms::Arch::X86, bits);
    std::size_t k = 0;
    for (const platforms::Insn& insn : cs.disasm(code.subspan(0, std::min<std::size_t>(code.size(), 32)), 0)) {
        if (insn.mnemonic == "sub" && std::string_view(insn.op_str).starts_with(sp)) {
            const std::size_t comma = insn.op_str.find(',');
            const auto n = text::parse_int<std::int64_t>(text::trim(std::string_view(insn.op_str).substr(comma + 1)), 0);
            return n && *n > 0 ? static_cast<std::size_t>(*n) : 0;
        }
        if (k >= 6 || (insn.mnemonic != "push" && insn.mnemonic != "mov")) break;
        ++k;
    }
    return 0;
}

std::vector<std::string> Searcher::instructions(const Code& built) {
    std::vector<std::string> out;
    const Bytes& code = built.bytes;
    if (isa_ == Isa::PowerPC) {
        for (std::size_t i = 0; i + 3 < code.size(); i += 4) {
            const std::uint32_t w = (std::uint32_t{code[i]} << 24) | (std::uint32_t{code[i + 1]} << 16) | (std::uint32_t{code[i + 2]} << 8) | code[i + 3];
            const std::size_t at = i / 4;
            out.push_back(word_bytes(w & ~(at < built.word_mask.size() ? built.word_mask[at] : 0u)));
        }
        return out;
    }
    const platforms::Decoder& cs = decoders_.get(platforms::Arch::X86, isa_ == Isa::X86_32 ? 32 : 16);
    std::size_t at = 0;
    while (at < code.size()) {
        std::size_t next = at;
        for (const platforms::Insn& insn : cs.disasm(ByteView(code).subspan(at), at)) {
            std::string one;
            for (std::size_t i = insn.address; i < insn.address + insn.size && i < code.size(); ++i)
                one += static_cast<char>(i < built.mask.size() && built.mask[i] ? 0 : code[i]);
            out.push_back(std::move(one));
            next = insn.address + insn.size;
        }
        if (next <= at) {   // not an instruction: one byte, and on
            out.emplace_back(1, static_cast<char>(code[at]));
            next = at + 1;
        }
        at = next;
    }
    return out;
}

Searcher::Score Searcher::score(const Code& built, const Code& original, const std::vector<std::string>* target) {
    const ByteVerdict v = distance_(built, original);
    if (!v.differing || isa_ == Isa::Unknown) return {{v.differing, v.differing}, v};
    const std::vector<std::string> mine = instructions(built);
    const std::vector<std::string> own = target ? std::vector<std::string>() : instructions(original);
    const std::vector<std::string>& theirs = target ? *target : own;
    const std::size_t same = matching_total(mine, theirs);
    return {{std::max(mine.size(), theirs.size()) - same + 1, v.differing}, v};
}

std::optional<Repaired> Searcher::repair(const Compile& compile, std::string_view source, const Code& original) {
    std::optional<Code> built = compile(source);
    if (!built) return std::nullopt;
    std::vector<std::string> target;
    std::size_t target_frame = 0;
    if (isa_ != Isa::Unknown) {
        target = instructions(original);
        target_frame = frame(original.bytes);
    }
    const std::vector<std::string>* target_ptr = isa_ != Isa::Unknown ? &target : nullptr;
    Score first = score(*built, original, target_ptr);
    std::pair<std::size_t, std::size_t> best_rank = first.rank;
    Repaired best{std::string(source), first.verdict, "", 0};

    struct Candidate {
        std::pair<std::size_t, std::size_t> rank;
        std::string text;
        Code code;
        std::string path;
    };
    std::vector<Candidate> frontier;
    frontier.push_back({first.rank, std::string(source), std::move(*built), ""});
    std::set<std::string> seen = {std::string(source)};   // lookup only
    std::size_t tried = 1;
    int stale = 0;
    while (best_rank.second && tried < kMaxWork && stale < static_cast<int>(kPatience) && !frontier.empty()) {
        std::vector<Candidate> found;
        for (const Candidate& at : frontier) {
            std::optional<std::size_t> draft_frame;
            if (isa_ != Isa::Unknown) draft_frame = frame(at.code.bytes);
            for (Knob& k : knobs(at.text, target_frame, draft_frame)) {
                if (tried >= kMaxWork || !seen.insert(k.source).second) continue;
                ++tried;
                std::optional<Code> now = compile(k.source);
                if (!now) continue;
                const Score s = score(*now, original, target_ptr);
                std::string path = at.path.empty() ? k.name : at.path + "," + k.name;
                if (s.rank < best_rank) {
                    best_rank = s.rank;
                    best = {k.source, s.verdict, path, 0};
                    stale = -1;
                }
                const bool exact = s.verdict.differing == 0;
                found.push_back({s.rank, std::move(k.source), std::move(*now), std::move(path)});
                if (exact) break;
            }
            if (!best_rank.second) break;
        }
        ++stale;
        std::stable_sort(found.begin(), found.end(), [](const Candidate& a, const Candidate& b) {
            return a.rank != b.rank ? a.rank < b.rank : a.path.size() < b.path.size();
        });
        if (found.size() > kBeam) found.resize(kBeam);
        frontier = std::move(found);
    }
    best.tried = tried;
    return best;
}

// ---- a module ----------------------------------------------------------------------------------------------------------

std::optional<Repaired> repair_function(const Compiler& cc, std::string_view header, const Target& target, std::string_view flags,
                                        const std::filesystem::path& work) {
    struct Setting {
        std::string flags, header, label;
    };
    std::vector<Setting> settings = {{std::string(flags), std::string(header), ""}};
    if (!has(flags, "/Oy-")) settings.push_back({std::string(flags) + " /Oy-", std::string(header), "/Oy-"});
    const std::string near_header = cc.needed_header(header, target.text);
    const std::vector<Setting> plain = settings;
    for (const Setting& s : plain) settings.push_back({s.flags, near_header, std::string(text::trim(s.label + " own-declarations"))});

    Searcher searcher(Isa::X86_32, [&](const Code& built, const Code& original) { return cc.distance(built, original); });
    std::optional<std::pair<std::pair<std::size_t, std::size_t>, const Setting*>> best;
    for (const Setting& s : settings) {
        const auto built = cc.compile(s.header, target.text, target.name, s.flags, work);
        if (!built) continue;
        const auto rank = searcher.score(*built, target.original).rank;
        if (!best || rank < best->first) best = {rank, &s};
        if (!rank.second) break;
    }
    if (!best) return std::nullopt;
    const Setting& chosen = *best->second;
    std::optional<Repaired> r = searcher.repair(
        [&](std::string_view text) { return cc.compile(chosen.header, text, target.name, chosen.flags, work); }, target.text, target.original);
    if (r && !chosen.label.empty()) r->knobs = std::string(text::trim("[" + chosen.label + "]," + r->knobs, ","));
    return r;
}

std::string best_flags(const Compiler& cc, const Module& module, const std::filesystem::path& work, std::size_t sample) {
    const std::vector<std::string> sets = cc.flag_sets();
    CB_ASSERT(!sets.empty(), "a compiler with no switch sets");
    const std::size_t step = module.isa == Isa::PowerPC ? 10 : std::max<std::size_t>(1, module.targets.size() / sample);
    std::vector<const Target*> chosen;
    for (std::size_t i = 0; i < module.targets.size() && chosen.size() < sample; i += step) chosen.push_back(&module.targets[i]);
    std::size_t best_score = 0;
    std::string best;
    bool any = false;
    for (const std::string& flags : sets) {
        std::size_t got = 0;
        for (const Target* t : chosen) {
            const auto built = cc.compile(module.header, t->text, t->name, flags, work);
            if (!built) continue;
            const ByteVerdict v = cc.distance(*built, t->original);
            got += v.match == verify::ByteMatch::Identical ? 3 : v.match == verify::ByteMatch::SameSize ? 1 : 0;
        }
        if (!any || got > best_score) {
            best_score = got;
            best = flags;
            any = true;
        }
    }
    return best;
}

Counts run(const Module& module, const Compiler& cc, const RunOptions& options) {
    std::string flags = options.flags.value_or("");
    if (flags.empty()) {
        flags = best_flags(cc, module, options.work / "flags");
        if (options.log) options.log("  compiler switches that fit best: " + flags);
    }
    const std::size_t count = options.limit ? std::min(options.limit, module.targets.size()) : module.targets.size();
    const std::vector<const Target*> todo = [&] {
        std::vector<const Target*> v;
        for (std::size_t i = 0; i < count; ++i) v.push_back(&module.targets[i]);
        return v;
    }();
    auto one = [&](const Target* t) -> std::optional<Repaired> {
        // a folder per worker
        const std::filesystem::path own = options.work / std::format("t{}", std::hash<std::thread::id>()(std::this_thread::get_id()) & 0x7FFFFFFF);
        if (module.isa == Isa::X86_32) return repair_function(cc, module.header, *t, flags, own);
        Searcher searcher(module.isa, [&](const Code& built, const Code& original) { return cc.distance(built, original); });
        return searcher.repair([&](std::string_view text) { return cc.compile(module.header, text, t->name, flags, own); }, t->text, t->original);
    };
    parallel::ThreadPool pool(static_cast<std::size_t>(parallel::workers()));
    const std::vector<std::optional<Repaired>> results = parallel::parallel_map(pool, todo, one);
    Counts counts;
    for (std::size_t i = 0; i < todo.size(); ++i) {
        const std::optional<Repaired>& r = results[i];
        if (!r) continue;
        if (options.record) options.record(*todo[i], *r, module.compiler_name + " " + flags);
        ++counts.functions;
        (r->verdict.match == verify::ByteMatch::Identical ? counts.identical : r->verdict.match == verify::ByteMatch::SameSize ? counts.same_size : counts.different) += 1;
        if (options.log && !r->knobs.empty())
            options.log(std::format("  {:<28} {:<9} distance {}/{}  via {}", todo[i]->name, verify::word(r->verdict.match), r->verdict.differing,
                                    r->verdict.total, r->knobs));
    }
    return counts;
}

// ---- testing the search on known sources -------------------------------------------------------------------------------

TruthCounts truth(const Profile& profile, const std::filesystem::path& work, std::size_t limit, const Log& log) {
    const std::vector<Unit> units = profile.units();
    Searcher searcher(profile.isa, profile.distance);
    const Compile compile = [&](std::string_view text) { return profile.compile(text, work); };
    TruthCounts counts;
    for (std::size_t ui = 0; ui < units.size() && (!limit || ui < limit); ++ui) {
        const Unit& u = units[ui];
        ++counts.units;
        const auto built = compile(u.source);
        if (!built || profile.distance(*built, u.original).match != verify::ByteMatch::Identical) continue;
        ++counts.identical;
        const std::vector<Knob> one = knobs(u.source, 0, std::nullopt);
        std::vector<Knob> two;
        for (std::size_t a = 0; a < std::min<std::size_t>(one.size(), 2); ++a) {
            const std::vector<Knob> inner = knobs(one[a].source, 0, std::nullopt);
            for (std::size_t b = inner.size() > 2 ? inner.size() - 2 : 0; b < inner.size(); ++b)
                two.push_back({one[a].name + "," + inner[b].name, inner[b].source});
        }
        // the first two of each kind of knob (its name without the number), then the first two of the pairs
        std::vector<std::pair<std::string, std::vector<const Knob*>>> kinds;
        for (const Knob& k : one) {
            const std::string kind = std::string(text::trim(k.name, "0123456789"));
            auto it = std::find_if(kinds.begin(), kinds.end(), [&](const auto& p) { return p.first == kind; });
            if (it == kinds.end()) {
                kinds.emplace_back(kind, std::vector<const Knob*>());
                it = kinds.end() - 1;
            }
            it->second.push_back(&k);
        }
        std::vector<const Knob*> picks;
        for (const auto& g : kinds)
            for (std::size_t k = 0; k < std::min<std::size_t>(2, g.second.size()); ++k) picks.push_back(g.second[k]);
        for (std::size_t k = 0; k < std::min<std::size_t>(2, two.size()); ++k) picks.push_back(&two[k]);
        for (const Knob* pick : picks) {
            const auto moved = compile(pick->source);
            if (!moved || profile.distance(*moved, u.original).differing == 0) continue;   // the compiler gives the same bytes either way
            ++counts.knocked;
            const std::optional<Repaired> r = searcher.repair(compile, pick->source, u.original);
            const bool ok = r && r->verdict.differing == 0;
            counts.brought_back += ok;
            if (log && !ok)
                log(std::format("  {:<12} {:<16} not brought back: distance {} after {}", u.name, pick->name,
                                r ? std::to_string(r->verdict.differing) : "none", r ? r->knobs : "none"));
        }
    }
    return counts;
}

// ---- the command line --------------------------------------------------------------------------------------------------

std::optional<Arguments> parse_arguments(const std::vector<std::string>& args) {
    Arguments out;
    for (std::size_t i = 0; i < args.size(); ++i) {
        const std::string& a = args[i];
        if (a == "--truth") {
            out.truth = true;
            out.truth_args.assign(args.begin() + static_cast<std::ptrdiff_t>(i) + 1, args.end());
            return out.truth_args.empty() ? std::nullopt : std::optional<Arguments>(out);
        }
        if (a == "--flags" || a == "--first" || a == "--json") {
            if (i + 1 >= args.size()) return std::nullopt;
            const std::string& value = args[++i];
            if (a == "--flags") {
                out.flags = value;
            } else if (a == "--first") {
                const auto n = text::parse_int<std::size_t>(value);
                if (!n) return std::nullopt;
                out.first = *n;
            }
        } else if (a.starts_with("--")) {
            continue;   // an option the command does not know is ignored
        } else if (out.module.empty()) {
            out.module = a;
        }
    }
    return out.module.empty() ? std::nullopt : std::optional<Arguments>(out);
}

std::string summary_line(std::string_view module_name, const Counts& counts) {
    return std::format("{}: {} functions repaired toward the original's bytes: {} identical, {} same size, {} different", module_name,
                       counts.functions, counts.identical, counts.same_size, counts.different);
}

}  // namespace cb::verify::authored::repair
