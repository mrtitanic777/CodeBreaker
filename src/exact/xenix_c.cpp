// cb-unity: alone
#include "exact/xenix_c.hpp"

#include <algorithm>
#include <array>
#include <format>

#include <nlohmann/json.hpp>

#include "base/cache.hpp"
#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"
#include "exact/plugin.hpp"

namespace cb::exact::xenix_c {

namespace stdfs = std::filesystem;

namespace {

// ---- reading a line of c1's assembly -----------------------------------------------------------------------------------------

bool xc_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; }

// The line's leading labels (`L12:` ...), and what follows them.
struct XcLabelled {
    std::string_view labels, rest;
};
XcLabelled xc_split_labels(std::string_view line) {
    std::size_t at = 0;
    while (at < line.size() && line[at] == 'L') {
        std::size_t d = at + 1;
        while (d < line.size() && std::isdigit(static_cast<unsigned char>(line[d]))) ++d;
        if (d == at + 1 || d >= line.size() || line[d] != ':') break;
        at = d + 1;
    }
    return {line.substr(0, at), line.substr(at)};
}

// `sub\tXX,XX` with two-letter registers: the pair it names.
std::optional<std::pair<std::string_view, std::string_view>> xc_sub_pair(std::string_view line) {
    if (line.size() != 9 || !line.starts_with("sub\t") || line[6] != ',') return std::nullopt;
    for (const std::size_t i : {4u, 5u, 7u, 8u})
        if (!xc_word(line[i])) return std::nullopt;
    return std::make_pair(line.substr(4, 2), line.substr(7, 2));
}

// `OP\tREG` or `OP\tREG,...` where OP is mov or one of the operations that change a register in place.
bool xc_changes_register(std::string_view line, std::string_view reg) {
    static constexpr std::array<std::string_view, 14> ops = {"mov", "add", "sub", "and", "or", "xor", "sal", "shl", "shr", "sar", "neg", "not", "inc", "dec"};
    const std::size_t tab = line.find('\t');
    if (tab == std::string_view::npos || std::find(ops.begin(), ops.end(), line.substr(0, tab)) == ops.end()) return false;
    const std::string_view rest = line.substr(tab + 1);
    return rest == reg || (rest.starts_with(reg) && rest.size() > reg.size() && rest[reg.size()] == ',');
}

// `mov\tX,REG` where X holds a parenthesis or an underscore (a memory operand) and no blank.
bool xc_stores_register(std::string_view line, std::string_view reg) {
    if (!line.starts_with("mov\t") || !line.ends_with(reg) || line.size() < 4 + 1 + 1 + reg.size()) return false;
    if (line[line.size() - reg.size() - 1] != ',') return false;
    const std::string_view x = line.substr(4, line.size() - 4 - reg.size() - 1);
    return !x.empty() && std::none_of(x.begin(), x.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }) &&
           x.find_first_of("(_") != std::string_view::npos;
}

// The lines with [from, to) replaced.
void xc_replace(std::vector<std::string>& lines, std::size_t from, std::size_t to, const std::vector<std::string>& with) {
    from = std::min(from, lines.size());
    to = std::min(std::max(to, from), lines.size());
    lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(from), lines.begin() + static_cast<std::ptrdiff_t>(to));
    lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(from), with.begin(), with.end());
}

bool xc_lines_are(const std::vector<std::string>& lines, std::size_t at, std::string_view a, std::string_view b) {
    return at + 2 <= lines.size() && lines[at] == a && lines[at + 1] == b;
}

// ---- c2's list, from outside -----------------------------------------------------------------------------------------------------

constexpr int kJbr = 1, kCbr = 2, kJmp = 3, kLabel = 4;

// c2's linked list of instructions in the data segment `ds`: a node's op and sub at its start, then forward, back and the reference.
class XcList {
public:
    XcList(emu::xenix::Machine& m, std::uint16_t ds) : m_(m), base_(static_cast<std::uint32_t>(ds) * 16) {}
    std::uint32_t w(std::int64_t a) const {
        const Bytes b = m_.read(static_cast<std::uint32_t>(base_ + a), 2);
        return static_cast<std::uint32_t>(b[0] | (b[1] << 8));
    }
    void setw(std::int64_t a, std::int64_t v) {
        const std::uint16_t value = static_cast<std::uint16_t>(v & 0xFFFF);
        const Bytes b = {static_cast<std::uint8_t>(value & 0xFF), static_cast<std::uint8_t>(value >> 8)};
        m_.write(static_cast<std::uint32_t>(base_ + a), b);
    }
    std::uint32_t op(std::uint32_t p) const { return m_.read(base_ + p, 1)[0]; }
    std::uint32_t sub(std::uint32_t p) const { return m_.read(base_ + p + 1, 1)[0]; }
    std::uint32_t forw(std::uint32_t p) const { return w(p + 2); }
    std::uint32_t back(std::uint32_t p) const { return w(p + 4); }
    std::uint32_t ref(std::uint32_t p) const { return w(p + 6); }
    void link(std::uint32_t a, std::uint32_t b) {
        setw(a + 2, b);
        setw(static_cast<std::int64_t>(b) + 4, a);
    }
    void unlink(std::uint32_t p) { link(back(p), forw(p)); }
    std::uint8_t byte_at(std::int64_t a) const { return m_.read(static_cast<std::uint32_t>(base_ + a), 1)[0]; }
    void set_byte(std::int64_t a, std::uint8_t v) { m_.write(static_cast<std::uint32_t>(base_ + a), Bytes{v}); }

private:
    emu::xenix::Machine& m_;
    std::uint32_t base_;
};

struct XcBlock {
    std::uint32_t label, end, target;
};

// The block a conditional jump at `p1` leads over: a label right after it, and the one block, with no jump or label in it, that ends in a
// jump just before the label the jump reaches.
std::optional<XcBlock> xc_one_block(const XcList& l, std::uint32_t p1) {
    const std::uint32_t label = l.forw(p1);
    if (l.op(label) != kLabel) return std::nullopt;
    std::uint32_t t = l.ref(p1);
    while (l.op(l.back(t)) == kLabel) t = l.back(t);
    const std::uint32_t bend = l.back(t);
    if (l.op(bend) != kJbr && l.op(bend) != kJmp) return std::nullopt;
    std::uint32_t p = l.forw(label);
    while (p != bend) {
        if (p == 0 || l.op(p) == kJbr || l.op(p) == kJmp || l.op(p) == kLabel) return std::nullopt;
        p = l.forw(p);
    }
    return XcBlock{label, bend, t};
}

// The block changes places with the one before the jump that leads to it: the jump's sense is reversed. The jump's place, or 0 when it
// does not apply.
std::uint32_t xc_swap(XcList& l, std::uint32_t p1) {
    const std::optional<XcBlock> block = xc_one_block(l, p1);
    if (!block) return 0;
    const auto [label, bend, t] = *block;
    const std::uint32_t target = l.ref(p1);
    if (l.w(label + 0xC) != 1) return 0;
    std::uint32_t q = l.back(p1);
    int n = 0;
    while (!(l.op(q) == kCbr && l.ref(q) == label)) {
        if (q == 0 || l.op(q) == kJbr || l.op(q) == kJmp || l.op(q) == kLabel || n > 64) return 0;
        q = l.back(q);
        ++n;
    }
    const std::uint32_t x = l.forw(q);
    if (x == p1) return 0;
    const std::uint32_t b0 = l.forw(label);
    l.link(l.back(p1), t);
    l.link(q, b0);
    l.link(bend, label);
    l.link(label, x);
    const std::uint8_t reversed = l.byte_at(0x106 + static_cast<std::int64_t>(l.sub(q)));
    l.set_byte(static_cast<std::int64_t>(q) + 1, reversed);
    const std::int64_t count = static_cast<std::int64_t>(l.w(target + 0xC)) - 1;
    l.setw(target + 0xC, count);
    if (count <= 0) l.unlink(target);
    l.setw(0x800, static_cast<std::int64_t>(l.w(0x800)) + 1);
    return q;
}

void xc_put16(Bytes& b, std::size_t at, std::uint32_t v) {
    b[at] = static_cast<std::uint8_t>(v & 0xFF);
    b[at + 1] = static_cast<std::uint8_t>((v >> 8) & 0xFF);
}
std::uint32_t xc_get16(const Bytes& b, std::size_t at) { return b[at] | (b[at + 1] << 8); }

Bytes xc_hex(std::string_view hex) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) out.push_back(static_cast<std::uint8_t>(text::parse_int<unsigned>(hex.substr(i, 2), 16).value()));
    return out;
}

// A near jump or call (opcode `op`) at `at` to `to`.
Bytes xc_jump(std::uint8_t op, std::int64_t to, std::int64_t at) {
    const std::int64_t rel = to - (at + 3);
    return {op, static_cast<std::uint8_t>(rel & 0xFF), static_cast<std::uint8_t>((rel >> 8) & 0xFF)};
}

void xc_append(Bytes& out, const Bytes& more) { out.insert(out.end(), more.begin(), more.end()); }

[[noreturn]] void xc_not_this(std::string_view pass) { throw FormatError("xenixc", 0, std::format("not the {} this was written for", pass)); }

// bytes `now` in place of the ones `was` at `at` in the c2 image (16 bytes of header before the text)
void xc_change(Bytes& c2, std::size_t at, std::string_view was, std::string_view now) {
    const Bytes old = xc_hex(was), fresh = xc_hex(now);
    if (c2.size() < 16 + at + old.size() || !std::equal(old.begin(), old.end(), c2.begin() + static_cast<std::ptrdiff_t>(16 + at))) xc_not_this("c2");
    std::copy(fresh.begin(), fresh.end(), c2.begin() + static_cast<std::ptrdiff_t>(16 + at));
}

}  // namespace

// ---- c1's assembly, revised ------------------------------------------------------------------------------------------------

std::string rule_u(std::string_view assembly) {
    std::vector<std::string> L;
    for (const std::string_view l : text::split(assembly, '\n')) L.emplace_back(l);
    std::size_t k = 0;
    while (k < L.size()) {
        const auto pair = xc_sub_pair(L[k]);
        if (pair && pair->first == pair->second) {
            const std::string hi(pair->first);
            // the low register of the pair the high one is: si:di and dx:si
            const std::string lo = hi == "di" ? "si" : hi == "si" ? "dx" : "";
            std::size_t j = k;
            while (!lo.empty() && j > 0 && (xc_changes_register(L[j - 1], lo) || (j < k && L[j - 1].starts_with("mov\tcx,")))) --j;
            const auto labelled_move_to_lo = [&](std::size_t at) {
                const XcLabelled lab = xc_split_labels(L[at]);
                return !lab.labels.empty() && lab.rest.starts_with("mov\t" + lo + ",");
            };
            if ((!lo.empty() && j > 0 && j == k) || (!lo.empty() && j < k && !L[j].starts_with("mov\t")))
                j = (!lo.empty() && j > 0 && labelled_move_to_lo(j - 1)) ? j - 1 : k;
            // a move of something into the low register, at line j (its labels, the register, what is moved)
            std::optional<std::tuple<std::string, std::string, std::string>> m2;
            if (!lo.empty() && j < k) {
                const XcLabelled lab = xc_split_labels(L[j]);
                if (lab.rest.starts_with("mov\t") && lab.rest.size() >= 7 && xc_word(lab.rest[4]) && xc_word(lab.rest[5]) && lab.rest[6] == ',')
                    m2 = std::make_tuple(std::string(lab.labels), std::string(lab.rest.substr(4, 2)), std::string(lab.rest.substr(7)));
            }
            const bool direct = m2 && std::get<2>(*m2) == "ax" && j + 1 == k && k + 3 < L.size() && xc_stores_register(L[k + 1], lo) &&
                                xc_stores_register(L[k + 2], hi) && L[k + 3].starts_with("jmp\t");
            if (m2 && std::get<1>(*m2) == lo && std::get<2>(*m2).find("(" + lo + ")") == std::string::npos && !direct) {
                const auto& [labels, ignored, source] = *m2;
                const bool pushed = xc_lines_are(L, k + 1, "push\t" + lo, "push\t" + hi);
                if (j + 1 == k && xc_lines_are(L, k + 1, "mov\tax," + lo, "mov\tdx," + hi)) {
                    std::vector<std::string> made;
                    if (!(source == "ax" && labels.empty())) made.push_back(labels + "mov\tax," + source);
                    made.push_back("sub\tdx,dx");
                    xc_replace(L, j, k + 3, made);
                    k = j + made.size() - 1;
                } else {
                    std::vector<std::string> made;
                    if (j + 1 == k) {
                        made.push_back(labels + "mov\tax," + source);
                    } else {
                        made.push_back(labels + "mov\t" + hi + "," + source);
                        for (std::size_t i = j + 1; i < k; ++i) made.push_back(text::replace_all(L[i], lo, hi));
                        if (!pushed) made.push_back("mov\tax," + hi);
                    }
                    made.push_back("sub\tdx,dx");
                    std::size_t end;
                    if (pushed) {
                        made.push_back("push\t" + std::string(j + 1 == k ? "ax" : hi));
                        made.push_back("push\tdx");
                        end = k + 3;
                    } else {
                        made.push_back("mov\t" + hi + ",dx");
                        made.push_back("mov\t" + lo + ",ax");
                        end = k + 1;
                    }
                    xc_replace(L, j, end, made);
                    k = j + made.size() - 1;
                }
            }
        }
        ++k;
    }
    return text::join(L, "\n");
}

std::string rule_d(std::string_view assembly) {
    // the increment goes after the byte is fetched: movb dx,... / inc|dec X / and dx,#255. | mov si|di,dx
    static const Regex put_off(R"((movb\tdx,[^\n]+\n)((?:inc|dec)\t(?:\w\w|[*#]?-?\d+\.\(bp\)|_\w+)\n)(and\tdx,#255\.\n|mov\t(?:si|di),dx\n))");
    static const Regex through(R"(mov\tax,(\*-?\d+\.\(bp\))\nmov\tbx,ax\nmovb\tax,\(bx\)\ncbw\n(inc|dec)\t\1\nadd\tax,)");
    static const Regex register_count(R"(\|NREG (\d+))");
    const std::string t = put_off.replace_all(assembly, "$1$3$2");
    // the register count in force where a match is: the last `|NREG n` before it
    std::vector<std::pair<std::size_t, std::string>> counts;   // (where it ends, n)
    register_count.for_each_match(t, [&](const RegexMatch& m) {
        counts.emplace_back(m.end(), std::string(m.group(1)));
        return true;
    });
    return through.replace_all(t, [&](const RegexMatch& m) {
        const auto before = std::upper_bound(counts.begin(), counts.end(), m.start(), [](std::size_t at, const auto& c) { return at < c.first; });
        if (before == counts.begin() || std::prev(before)->second != "3") return std::string(m.whole());
        return "mov\tdi," + std::string(m.group(1)) + "\nmovb\tax,(di)\ncbw\n" + std::string(m.group(2)) + "\t" + std::string(m.group(1)) + "\nadd\tax,";
    });
}

std::string rule_m(std::string_view assembly) {
    // M: a memory operand (a local, an indexed or a named place)
    static const Regex memory_operand(
        R"((^|\n)((?:L\d+:)*)(add|sub|and|or|xor)\t(\((?:bx|si|di)\)),((?:(?:[*#]?-?\d+\.)?(?:\+?_\w+)?\((?:bp|bx|si|di)\)|(?:\d+\.\+)?_\w+))(?=\n))");
    return memory_operand.replace_all(assembly, "$1$2mov\tax,$5\n$3\t$4,ax");
}

std::string rule_b(std::string_view assembly) {
    static const Regex or_byte(R"((^|\n)((?:L\d+:)*)orb\t(ax|dx|bx|cx),(#\d+\.)(?=\n))");
    return or_byte.replace_all(assembly, "$1$2or\t$3,$4");
}

std::string apply_rules(std::string_view letters, std::string_view assembly) {
    std::string t(assembly);
    for (const char n : letters) {
        switch (n) {
            case 'U': t = rule_u(t); break;
            case 'D': t = rule_d(t); break;
            case 'M': t = rule_m(t); break;
            case 'B': t = rule_b(t); break;
            default: throw UsageError(std::format("xenixc: no rule {}", n));
        }
    }
    return t;
}

// ---- the passes, changed as Microsoft's --------------------------------------------------------------------------------------

Bytes patched_c1(Bytes c1) {
    if (c1.size() < 4) xc_not_this("c1");
    const std::size_t q = 16 + xc_get16(c1, 2) + 0x472;
    if (q + 2 > c1.size() || xc_get16(c1, q) != 100) xc_not_this("c1");
    xc_put16(c1, q, 0x7FFF);
    return c1;
}

Bytes patched_c2(Bytes c2) {
    if (c2.size() < 16 + 0x2C54) xc_not_this("c2");
    xc_change(c2, 0x2F82, "7417", "74e7");
    xc_change(c2, 0x332A, "803c017509837ef6ff7418e9ccfe", "803c017405803c3f7504e9cdfe");
    const std::size_t tsz = xc_get16(c2, 2);
    if (!(xc_get16(c2, 16 + 0x1885) == 0x17BD && xc_get16(c2, 16 + 0x18E1) == 0x17BD)) xc_not_this("c2");
    // a jump or call through a pointer counts 6 more (the figure for jumps and calls, 17BD, through a stub)
    Bytes stub = xc_hex("8b5c0a" "803f40" "7504" "8346fa06");
    xc_append(stub, xc_jump(0xE9, 0x17BD, static_cast<std::int64_t>(tsz) + static_cast<std::int64_t>(stub.size())));
    c2.insert(c2.begin() + static_cast<std::ptrdiff_t>(16 + tsz), stub.begin(), stub.end());
    xc_put16(c2, 2, static_cast<std::uint32_t>(tsz + stub.size()));
    xc_put16(c2, 16 + 0x1885, static_cast<std::uint32_t>(tsz));
    xc_put16(c2, 16 + 0x18E1, static_cast<std::uint32_t>(tsz));
    // a constant is not taken from SI or DI while a variable lives there (1DDE)
    const std::int64_t at4 = static_cast<std::int64_t>(tsz + stub.size());
    Bytes s4 = xc_hex("807f0164" "7503");
    xc_append(s4, xc_jump(0xE9, 0x1E3B, at4 + static_cast<std::int64_t>(s4.size())));
    xc_append(s4, xc_hex("803e8c092a" "752b" "83ff02" "7c26" "83ff03" "7f21" "8bc7" "48" "39060208" "7f18" "b82800" "50" "b88c09" "50"));
    xc_append(s4, xc_jump(0xE8, 0x410E, at4 + static_cast<std::int64_t>(s4.size())));
    xc_append(s4, xc_hex("83c404" "85c0" "7506" "bfffff"));
    xc_append(s4, xc_jump(0xE9, 0x1E24, at4 + static_cast<std::int64_t>(s4.size())));
    xc_append(s4, xc_jump(0xE9, 0x1E64, at4 + static_cast<std::int64_t>(s4.size())));
    {
        const Bytes expected = xc_hex("807f01647529");
        if (!std::equal(expected.begin(), expected.end(), c2.begin() + static_cast<std::ptrdiff_t>(16 + 0x1E35))) xc_not_this("c2");
    }
    c2.insert(c2.begin() + static_cast<std::ptrdiff_t>(16 + at4), s4.begin(), s4.end());
    stub.insert(stub.end(), s4.begin(), s4.end());
    xc_put16(c2, 2, static_cast<std::uint32_t>(at4 + static_cast<std::int64_t>(s4.size())));
    {
        const std::int64_t rel = at4 - 0x1E38;   // (relative to the byte after the three that make the jump)
        const Bytes jump = {0xE9, static_cast<std::uint8_t>(rel & 0xFF), static_cast<std::uint8_t>((rel >> 8) & 0xFF), 0x90, 0x90, 0x90};
        std::copy(jump.begin(), jump.end(), c2.begin() + static_cast<std::ptrdiff_t>(16 + 0x1E35));
    }
    // a call through a pointer uses every register (2788, and the fold of a load into its push, 2436)
    const std::int64_t at5 = static_cast<std::int64_t>(tsz + stub.size());
    Bytes s5 = xc_hex("c6440fff" "8b5c0a" "803f40" "7403");
    xc_append(s5, xc_jump(0xE9, 0x2A6E, at5 + static_cast<std::int64_t>(s5.size())));
    xc_append(s5, xc_hex("c6440eff"));
    xc_append(s5, xc_jump(0xE9, 0x2C74, at5 + static_cast<std::int64_t>(s5.size())));
    const std::int64_t at6 = at5 + static_cast<std::int64_t>(s5.size());
    Bytes s6 = xc_hex("803c2f" "7403");
    xc_append(s6, xc_jump(0xE9, 0x243B, at6 + static_cast<std::int64_t>(s6.size())));
    xc_append(s6, xc_hex("8b5c0a" "803f40" "7503"));
    xc_append(s6, xc_jump(0xE9, 0x2487, at6 + static_cast<std::int64_t>(s6.size())));
    xc_append(s6, xc_jump(0xE9, 0x2453, at6 + static_cast<std::int64_t>(s6.size())));
    {
        const Bytes expected = xc_hex("803c2f7418");
        if (!std::equal(expected.begin(), expected.end(), c2.begin() + static_cast<std::ptrdiff_t>(16 + 0x2436)) || xc_get16(c2, 16 + 0x2C52) != 0x2A6A) xc_not_this("c2");
    }
    {
        const std::int64_t rel = at6 - 0x2439;
        const Bytes jump = {0xE9, static_cast<std::uint8_t>(rel & 0xFF), static_cast<std::uint8_t>((rel >> 8) & 0xFF), 0x90, 0x90};
        std::copy(jump.begin(), jump.end(), c2.begin() + static_cast<std::ptrdiff_t>(16 + 0x2436));
    }
    xc_append(s5, s6);
    c2.insert(c2.begin() + static_cast<std::ptrdiff_t>(16 + at5), s5.begin(), s5.end());
    xc_put16(c2, 2, static_cast<std::uint32_t>(at5 + static_cast<std::int64_t>(s5.size())));
    xc_put16(c2, 16 + 0x2C52, static_cast<std::uint32_t>(at5));
    return c2;
}

std::map<std::uint32_t, emu::xenix::Hook> c2_hooks(std::string_view mode) {
    using emu::xenix::Reg;
    const auto pending = std::make_shared<std::vector<std::uint32_t>>();   // jumps whose block was left where it is
    const std::string kind(mode);
    std::map<std::uint32_t, emu::xenix::Hook> hooks;
    hooks[0xCE3] = [pending, kind](emu::xenix::Machine& m, std::uint16_t, std::uint16_t ds) {
        XcList l(m, ds);
        const std::uint32_t p1 = m.reg(Reg::Di) & 0xFFFF;
        const std::uint32_t bp = m.reg(Reg::Bp) & 0xFFFF;
        const std::uint32_t p3 = l.w(static_cast<std::int64_t>(bp) - 6);
        if (p3 == p1 || l.ref(p3) != l.ref(p1) || !xc_one_block(l, p1)) return;
        if (kind.find('S') != std::string::npos) {
            if (const std::uint32_t q = xc_swap(l, p1)) {
                m.set_reg(Reg::Dx, q);
                m.set_reg(Reg::Ip, 0xE5A);
                return;
            }
        }
        pending->push_back(p1);
        l.setw(static_cast<std::int64_t>(bp) + 4, p3);
        m.set_reg(Reg::Di, p3);
        m.set_reg(Reg::Si, l.back(m.reg(Reg::Si) & 0xFFFF));
        m.set_reg(Reg::Ip, 0xD4F);
    };
    hooks[0xE5A] = [pending](emu::xenix::Machine& m, std::uint16_t, std::uint16_t) {
        if (pending->empty()) return;
        m.set_reg(Reg::Dx, pending->back());
        pending->pop_back();
    };
    return hooks;
}

// ---- the toolchain ----------------------------------------------------------------------------------------------------------

XenixC::XenixC(toolchain::Profile profile, std::optional<std::string> variant, const toolchain::Context& context)
    : toolchain::Toolchain(profile, variant.has_value() || profile.default_variant ? variant : std::optional<std::string>("microsoft"), context),
      ctx_(context), raw_(std::make_unique<Json>(plugin::profile_json(*context.locations, profile.name))) {
    const Json& variants = (*raw_)["variants"];
    const Json empty = Json::object();
    const Json& mine = variants.is_object() && variants.contains(this->variant()) ? variants[this->variant()] : empty;
    rules_ = plugin::text_or(mine, "rules", "");
    patch_ = mine.is_object() && mine.contains("patch") && mine["patch"].is_boolean() && mine["patch"].get<bool>();
    c2move_ = plugin::text_or(mine, "c2move", "");
    // the key names a toolchain's compiles: the whole profile, the variant and this module's version
    plugin_key_ = crypto::to_hex(crypto::sha1(to_bytes(raw_->dump() + this->variant() + std::string(kCacheVersion)))).substr(0, 12);
}

XenixC::~XenixC() = default;

namespace {

// The source with LF line ends (the XENIX tools read no CR), the headers by name, then the include folders' .h files.
struct XcInput {
    Bytes source;
    std::vector<std::pair<std::string, Bytes>> files;   // headers, then include files
    std::vector<std::pair<std::string, Bytes>> keyed;   // the same with a mark between the two, for the cache key
};

XcInput xc_input(ByteView source, const modules::CompileOptions& options) {
    XcInput in;
    for (std::size_t i = 0; i < source.size(); ++i)
        if (!(source[i] == '\r' && i + 1 < source.size() && source[i + 1] == '\n')) in.source.push_back(source[i]);
    for (const auto& h : options.headers) in.files.push_back(h);   // (a map: sorted by name already)
    in.keyed = in.files;
    in.keyed.emplace_back("\x01includes", Bytes{});
    for (const std::string& dir : options.include) {
        std::vector<stdfs::path> names;
        std::error_code ec;
        for (stdfs::directory_iterator it(fs::from_text(dir), ec), end; !ec && it != end; it.increment(ec))
            if (it->is_regular_file(ec) && text::iends_with(it->path().filename().string(), ".h")) names.push_back(it->path());
        std::sort(names.begin(), names.end());
        for (const stdfs::path& f : names) {
            in.files.emplace_back(fs::to_text(f.filename()), fs::read_file(f));
            in.keyed.push_back(in.files.back());
        }
    }
    return in;
}

}  // namespace

std::string XenixC::cache_key(ByteView source, const modules::CompileOptions& options) const {
    const XcInput in = xc_input(source, options);
    return plugin::compile_key(in.source, in.keyed, "", plugin_key_);
}

modules::CompileResult XenixC::build(ByteView source, const modules::CompileOptions& options) const {
    const XcInput in = xc_input(source, options);
    return run_passes(in.source, in.files);
}

modules::CompileResult XenixC::compile(ByteView source, const modules::CompileOptions& options) const {
    const std::string key = cache_key(source, options);
    if (ctx_.cache) {
        if (auto got = ctx_.cache->value<plugin::Built>(toolchain::kCacheKind, key)) return {std::move(got->object), std::move(got->console)};
    }
    const modules::CompileResult built = build(source, options);
    if (ctx_.cache) ctx_.cache->keep(toolchain::kCacheKind, key, plugin::Built{built.object, built.console});
    return built;
}

modules::CompileResult XenixC::run_passes(const Bytes& source, const std::vector<std::pair<std::string, Bytes>>& headers) const {
    CB_ASSERT(ctx_.locations != nullptr, "a toolchain needs the locations of its kit");
    const auto kit_name = profile_data().kit;
    if (!kit_name) throw UsageError(name() + " names no kit to run its tools from");
    const stdfs::path root = ctx_.locations->kit(*kit_name);
    const fs::ScopedTempDir work("xenixc");   // gone when this returns, whatever happens
    stdfs::create_directories(work.path() / "tmp");
    stdfs::create_directories(work.path() / "lib");
    fs::write_file(work.path() / "x.c", source);
    for (const auto& [file, bytes] : headers) {
        Bytes lf;
        for (std::size_t i = 0; i < bytes.size(); ++i)
            if (!(bytes[i] == '\r' && i + 1 < bytes.size() && bytes[i + 1] == '\n')) lf.push_back(bytes[i]);
        fs::write_file(work.path() / file, lf);
    }
    Bytes c1 = fs::read_file(root / "lib" / "c1");
    Bytes c2 = fs::read_file(root / "lib" / "c2");
    if (patch_) {
        c1 = patched_c1(std::move(c1));
        c2 = patched_c2(std::move(c2));
    }
    fs::write_file(work.path() / "lib" / "c1", c1);
    fs::write_file(work.path() / "lib" / "c2", c2);
    struct Pass {
        std::string program;
        std::vector<std::string> args;
    };
    const std::vector<Pass> steps = {{"lib/cpp", {"x.c", "x.i"}},
                                     {"lib/c0", {"x.i", "t1", "t2"}},
                                     {"lib/c1", {"t1", "t2", "x.s"}},
                                     {"lib/c2", {"x.s", "x.o.s"}},
                                     {"bin/as", {"-o", "x.o", "x.o.s"}}};
    std::string log;
    for (const Pass& step : steps) {
        emu::xenix::Spec spec;
        spec.root = root;
        spec.work = work.path();
        spec.program = step.program;
        spec.args = step.args;
        if (step.program == "lib/c2" && !c2move_.empty()) spec.hooks = c2_hooks(c2move_);
        const emu::xenix::Result r = emu::xenix::run(spec);
        log += text::from_latin1(r.console);
        if (!r.status || *r.status != 0 || r.error) {
            log += std::format("{}: exit {} {}\n(the pass that stopped: {})\n", step.program, r.status ? std::to_string(*r.status) : std::string("None"),
                               r.error.value_or(""), step.program);
            return {std::nullopt, log};
        }
        if (step.program == "lib/c1" && !rules_.empty()) {
            const stdfs::path file = work.path() / "x.s";
            const std::string revised = apply_rules(rules_, text::from_latin1(fs::read_file(file)));
            fs::write_file(file, text::to_latin1(revised));
        }
    }
    return {fs::read_file(work.path() / "x.o"), log};
}

void register_plugin(toolchain::Toolchains& toolchains) {
    toolchains.register_plugin(std::string(kPluginName), [](const toolchain::Profile& p, const std::optional<std::string>& variant, const toolchain::Context& c) {
        return std::make_shared<XenixC>(p, variant, c);
    });
}

}  // namespace cb::exact::xenix_c
