// cb/port/asm16_source.cpp -- reading an assembler module: lines, operands, segments and the old assembler's listing.
// (See asm16.hpp.)
// cb-unity: alone

#include <algorithm>
#include <cctype>
#include <charconv>
#include <filesystem>

#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/port/asm16.hpp"

namespace cb::port::asm16 {

namespace {

bool is_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; }
bool is_name_start(char c) { return std::isalpha(static_cast<unsigned char>(c)) || c == '_' || c == '$' || c == '@' || c == '?'; }
bool is_name_char(char c) { return is_word(c) || c == '$' || c == '@' || c == '?'; }
bool is_digit(char c) { return c >= '0' && c <= '9'; }
bool is_hex(char c) { return is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

std::size_t skip_space(std::string_view s, std::size_t i) {
    while (i < s.size() && text::is_space(s[i])) ++i;
    return i;
}

// The first word and what follows it after the spaces (`s.split(None, 1)`); the rest is empty when there is none.
std::pair<std::string_view, std::string_view> first_word(std::string_view s) {
    std::size_t i = skip_space(s, 0);
    std::size_t j = i;
    while (j < s.size() && !text::is_space(s[j])) ++j;
    return {s.substr(i, j - i), s.substr(skip_space(s, j))};
}

// Whether `s` begins with `word` (ASCII case-insensitive) and then, after any spaces, `:`.
bool begins_with_colon(std::string_view s, std::string_view word) {
    if (s.size() < word.size() || !text::iequals(s.substr(0, word.size()), word)) return false;
    const std::size_t i = skip_space(s, word.size());
    return i < s.size() && s[i] == ':';
}

bool starts_with_segment_colon(std::string_view s) {
    return begins_with_colon(s, "es") || begins_with_colon(s, "ds") || begins_with_colon(s, "ss") || begins_with_colon(s, "cs");
}

// `label:` at the start of a statement (a `::` is not a label); the label and what follows.
std::optional<std::pair<std::string_view, std::string_view>> split_label(std::string_view s) {
    if (s.empty() || !is_name_start(s[0])) return std::nullopt;
    std::size_t j = 1;
    while (j < s.size() && is_name_char(s[j])) ++j;
    const std::size_t k = skip_space(s, j);
    if (k >= s.size() || s[k] != ':' || (k + 1 < s.size() && s[k + 1] == ':')) return std::nullopt;
    return std::pair{s.substr(0, j), text::trim(s.substr(k + 1))};
}

bool one_of(std::string_view s, std::initializer_list<std::string_view> set) {
    for (std::string_view x : set)
        if (s == x) return true;
    return false;
}

std::string upper_of(std::string_view s) { return text::upper(s); }

// An equate's value, when it is a number.
std::optional<std::int64_t> equate_number(const Equates* equ, const std::string& name) {
    if (!equ) return std::nullopt;
    const auto e = equ->values.find(name);
    return e == equ->values.end() ? std::nullopt : number(e->second);
}

std::optional<int> size_word(std::string_view w) {
    const std::string l = text::lower(w);
    if (l == "byte") return 1;
    if (l == "word") return 2;
    if (l == "dword") return 4;
    if (l == "qword") return 8;
    if (l == "tbyte") return 10;
    return std::nullopt;
}

// `offset [seg:]sym[+-disp]` as a whole.
struct OffsetOperand {
    std::string symbol;
    std::optional<std::string> displacement;   // the sign and the word after it (`-4`), as written
    bool negative = false;
    std::string_view word;
};
std::optional<OffsetOperand> read_offset_operand(std::string_view s) {
    if (s.size() < 7 || !text::iequals(s.substr(0, 6), "offset") || !text::is_space(s[6])) return std::nullopt;
    std::size_t i = skip_space(s, 6);
    // an optional `seg:` (words then a colon), which the symbol after it replaces
    std::size_t w = i;
    while (w < s.size() && is_word(s[w])) ++w;
    if (w > i && w < s.size() && s[w] == ':') i = w + 1;
    std::size_t j = i;
    while (j < s.size() && is_name_char(s[j])) ++j;
    if (j == i) return std::nullopt;
    OffsetOperand o;
    o.symbol = std::string(s.substr(i, j - i));
    std::size_t k = skip_space(s, j);
    if (k == s.size()) return o;
    if (s[k] != '+' && s[k] != '-') return std::nullopt;
    o.negative = s[k] == '-';
    k = skip_space(s, k + 1);
    std::size_t e = k;
    while (e < s.size() && is_word(s[e])) ++e;
    if (e == k || e != s.size()) return std::nullopt;
    o.word = s.substr(k, e - k);
    o.displacement = std::string(o.word);
    return o;
}

}  // namespace

// ---- registers and mnemonics ----------------------------------------------------------------------------------------

std::string_view reg64(std::string_view r) {
    if (r == "ax") return "rax";
    if (r == "bx") return "rbx";
    if (r == "cx") return "rcx";
    if (r == "dx") return "rdx";
    if (r == "si") return "rsi";
    if (r == "di") return "rdi";
    if (r == "bp") return "rbp";
    if (r == "sp") return "rsp";
    return {};
}

std::string_view reg8_parent(std::string_view r) {
    if (r == "al" || r == "ah") return "ax";
    if (r == "bl" || r == "bh") return "bx";
    if (r == "cl" || r == "ch") return "cx";
    if (r == "dl" || r == "dh") return "dx";
    return {};
}

bool is_reg16(std::string_view n) { return !reg64(n).empty(); }
bool is_reg8(std::string_view n) { return !reg8_parent(n).empty(); }
bool is_segment_register(std::string_view n) { return one_of(n, {"ds", "es", "ss", "cs"}); }

bool is_jump(std::string_view op) {
    return one_of(op, {"jmp", "je", "jne", "jz", "jnz", "jb", "jnb", "jae", "jnae", "ja", "jna", "jbe", "jnbe", "jc", "jnc",
                       "jl", "jnl", "jge", "jnge", "jg", "jng", "jle", "jnle", "js", "jns", "jo", "jno", "jp", "jnp", "jpe",
                       "jpo", "loop", "loope", "loopne", "loopz", "loopnz", "jcxz"});
}

bool is_string_op(std::string_view op) {
    return one_of(op, {"lodsb", "lodsw", "stosb", "stosw", "movsb", "movsw", "cmpsb", "cmpsw", "scasb", "scasw"});
}

bool is_calling_word(std::string_view w) {
    return one_of(w, {"near", "far", "huge", "pascal", "cdecl", "fortran", "_near", "_far", "_pascal", "_cdecl"});
}

// ---- the source -----------------------------------------------------------------------------------------------------

std::string strip_comment(std::string_view line) {
    char quote = 0;
    for (std::size_t i = 0; i < line.size(); ++i) {
        const char c = line[i];
        if (quote) {
            if (c == quote) quote = 0;
        } else if (c == '\'' || c == '"') {
            quote = c;
        } else if (c == ';') {
            return std::string(line.substr(0, i));
        }
    }
    return std::string(line);
}

std::vector<std::string> split_ops(std::string_view s) {
    std::vector<std::string> out;
    int depth = 0;
    std::string cur;
    char quote = 0;
    for (const char c : s) {
        if (quote) {
            cur.push_back(c);
            if (c == quote) quote = 0;
            continue;
        }
        if (c == '\'' || c == '"') quote = c;
        else if (c == '[' || c == '(') ++depth;
        else if (c == ']' || c == ')') --depth;
        if (c == ',' && depth == 0) {
            out.emplace_back(text::trim(cur));
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    if (!text::trim(cur).empty()) out.emplace_back(text::trim(cur));
    return out;
}

std::optional<std::int64_t> number(std::string_view text_in) {
    const std::string_view s = text::trim(text_in);
    if (s.empty()) return std::nullopt;
    auto parse = [](std::string_view digits, int base) -> std::optional<std::int64_t> {
        std::uint64_t v = 0;
        const auto [end, ec] = std::from_chars(digits.data(), digits.data() + digits.size(), v, base);
        if (ec != std::errc() || end != digits.data() + digits.size() || v > static_cast<std::uint64_t>(INT64_MAX)) return std::nullopt;
        return static_cast<std::int64_t>(v);
    };
    // 0ffh: a digit, hex digits, h
    if ((s.back() == 'h' || s.back() == 'H') && s.size() >= 2 && is_digit(s[0]) &&
        std::all_of(s.begin(), s.end() - 1, [](char c) { return is_hex(c); }))
        return parse(s.substr(0, s.size() - 1), 16);
    if (std::all_of(s.begin(), s.end(), is_digit)) return parse(s, 10);
    if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X') &&
        std::all_of(s.begin() + 2, s.end(), [](char c) { return is_hex(c); }))
        return parse(s.substr(2), 16);
    return std::nullopt;
}

std::vector<Line> parse(std::string_view source) {
    std::vector<Line> out;
    int no = 0;
    for (const std::string_view raw : text::split(source, '\n')) {
        Line l;
        l.no = ++no;
        l.text = std::string(raw);
        const std::string uncommented = strip_comment(raw);
        std::string_view s = text::trim(uncommented);
        if (const auto lab = split_label(s); lab && !starts_with_segment_colon(s)) {
            l.label = std::string(lab->first);
            s = lab->second;
        }
        if (s.empty()) {
            out.push_back(std::move(l));
            continue;
        }
        const auto [word, rest_text] = first_word(s);
        if (!rest_text.empty()) {
            const auto [second, after_second] = first_word(rest_text);
            const std::string kind = upper_of(second);
            if (one_of(kind, {"SEGMENT", "ENDS", "GROUP", "PROC", "ENDP", "LABEL", "EQU", "DB", "DW", "DD", "DQ", "DT", "="})) {
                l.directive = Directive{kind, std::string(word), std::string(after_second)};
                out.push_back(std::move(l));
                continue;
            }
        }
        const std::string head = upper_of(word);
        if (one_of(head, {"DB", "DW", "DD", "DQ", "DT", "PUBLIC", "EXTRN", "ASSUME", "TITLE", "END", "ORG", "EVEN", "ALIGN",
                          "INCLUDE", "NAME", "PAGE", "SUBTTL", ".8086", ".186", ".286", ".8087"})) {
            l.directive = Directive{head, std::nullopt, std::string(rest_text)};
            out.push_back(std::move(l));
            continue;
        }
        std::string op = text::lower(word);
        std::string_view rest = rest_text;
        if (one_of(op, {"rep", "repe", "repz", "repne", "repnz"})) {
            l.prefix = op;
            const auto [next, after] = first_word(rest);
            if (next.empty()) throw Asm16Error("line " + std::to_string(l.no) + ": a prefix with no instruction after it");
            op = text::lower(next);
            rest = after;
        }
        l.op = op;
        l.args = split_ops(rest);
        out.push_back(std::move(l));
    }
    return out;
}

std::set<std::string> code_segments(std::span<const Line> lines) {
    std::set<std::string> out;
    std::map<std::string, std::vector<std::string>> groups;   // upper-case name -> members
    for (const Line& l : lines) {
        const auto& d = l.directive;
        if (!d) continue;
        if (d->kind == "SEGMENT") {
            std::optional<std::string> cls;
            if (const std::size_t q = d->rest.find('\''); q != std::string::npos)
                if (const std::size_t e = d->rest.find('\'', q + 1); e != std::string::npos) cls = d->rest.substr(q + 1, e - q - 1);
            const bool code_class = cls && (upper_of(*cls) == "CODE" || upper_of(*cls) == "PROG");
            if (code_class || upper_of(*d->name).ends_with("TEXT")) out.insert(*d->name);
        } else if (d->kind == "GROUP") {
            std::vector<std::string> members;
            const std::string uncommented = strip_comment(d->rest);
            for (const std::string_view x : text::split(uncommented, ','))
                if (!text::trim(x).empty()) members.emplace_back(text::trim(x));
            groups[upper_of(*d->name)] = std::move(members);
        }
    }
    for (const Line& l : lines) {
        const auto& d = l.directive;
        if (!d || d->kind != "ASSUME") continue;
        // \bcs\s*:\s*([\w$@?]+)
        const std::string_view r = d->rest;
        for (std::size_t at = 0; at + 2 <= r.size(); ++at) {
            if ((at > 0 && is_word(r[at - 1])) || !text::iequals(r.substr(at, 2), "cs")) continue;
            std::size_t i = skip_space(r, at + 2);
            if (i >= r.size() || r[i] != ':') continue;
            i = skip_space(r, i + 1);
            std::size_t j = i;
            while (j < r.size() && is_name_char(r[j])) ++j;
            if (j == i) continue;
            const std::string name(r.substr(i, j - i));
            const auto g = groups.find(upper_of(name));
            if (g == groups.end()) {
                if (name != "NOTHING" && name != "nothing") out.insert(name);
            } else {
                for (const std::string& m : g->second)
                    if (m != "NOTHING" && m != "nothing") out.insert(m);
            }
            break;
        }
    }
    return out;
}

std::optional<DirectMove> decode_direct(cb::ByteView b) {
    static constexpr std::array<std::string_view, 8> kReg16Code = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
    static constexpr std::array<std::string_view, 8> kReg8Code = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
    auto word_at = [](std::string_view size, unsigned address) {
        char buf[32];
        std::snprintf(buf, sizeof buf, "%s ptr [0%04xh]", std::string(size).c_str(), address);
        return std::string(buf);
    };
    if (b.size() >= 4 && b[0] >= 0x88 && b[0] <= 0x8B && (b[1] & 0xC7) == 0x06) {
        const std::string_view reg = (b[0] & 1 ? kReg16Code : kReg8Code)[(b[1] >> 3) & 7];
        const std::string mem = word_at(b[0] & 1 ? "word" : "byte", b[2] | (b[3] << 8));
        return DirectMove{4, "mov", (b[0] & 2) ? std::vector<std::string>{std::string(reg), mem}
                                               : std::vector<std::string>{mem, std::string(reg)}};
    }
    if (b.size() >= 3 && b[0] >= 0xA0 && b[0] <= 0xA3) {
        const std::string reg = (b[0] & 1) ? "ax" : "al";
        const std::string mem = word_at(b[0] & 1 ? "word" : "byte", b[1] | (b[2] << 8));
        return DirectMove{3, "mov", b[0] < 0xA2 ? std::vector<std::string>{reg, mem} : std::vector<std::string>{mem, reg}};
    }
    return std::nullopt;
}

// ---- operands -------------------------------------------------------------------------------------------------------

Operand operand(std::string_view a, const Equates* equ) {
    const std::string_view trimmed = text::trim(a);
    std::string_view s = trimmed;
    const std::string low = text::lower(s);
    Operand o;
    if (is_reg16(low) || is_reg8(low) || is_segment_register(low)) {
        o.kind = Operand::Kind::Reg;
        o.text = low;
        return o;
    }
    if (const auto off = read_offset_operand(s)) {
        o.kind = Operand::Kind::Off;
        o.text = off->symbol;
        if (off->displacement) {
            // (a displacement that is no number is an equate's, or an error here)
            std::optional<std::int64_t> d = number(off->word);
            if (!d) d = equate_number(equ, std::string(off->word));
            if (!d) throw Asm16Error("the displacement of `" + std::string(a) + "` is not a number");
            o.disp = off->negative ? -*d : *d;
        }
        return o;
    }
    std::optional<int> size;
    {   // byte|word|dword|qword|tbyte ptr REST
        std::size_t w = 0;
        while (w < s.size() && std::isalpha(static_cast<unsigned char>(s[w]))) ++w;
        if (w > 0 && w < s.size() && text::is_space(s[w]) && size_word(s.substr(0, w))) {
            std::size_t i = skip_space(s, w);
            if (i + 3 <= s.size() && text::iequals(s.substr(i, 3), "ptr") && i + 3 < s.size() && text::is_space(s[i + 3])) {
                size = *size_word(s.substr(0, w));
                s = text::trim(s.substr(skip_space(s, i + 3)));
            }
        }
    }
    std::optional<std::string> seg;
    if (s.size() >= 2) {
        const std::string head = text::lower(s.substr(0, 2));
        if (is_segment_register(head) && begins_with_colon(s, head)) {
            seg = head;
            s = text::trim(s.substr(skip_space(s, 2) + 1));
        }
    }
    if (s.find('[') != std::string_view::npos || size || seg) {
        Mem m;
        m.size = size;
        m.seg = seg;
        m.text = std::string(a);
        // the bracketed parts and what is outside them, as one sum
        std::string inside, outside;
        for (std::size_t i = 0; i < s.size();) {
            const std::size_t close = s[i] == '[' ? s.find(']', i + 1) : std::string_view::npos;
            if (s[i] == '[' && close != std::string_view::npos) {
                if (!inside.empty()) inside += " + ";
                inside.append(s.substr(i + 1, close - i - 1));
                outside.push_back(' ');
                i = close + 1;
            } else {
                outside.push_back(s[i++]);
            }
        }
        std::string sum = outside + " + " + inside;
        sum.erase(std::remove(sum.begin(), sum.end(), ' '), sum.end());
        for (std::size_t p = 0; p < sum.size();) {
            std::size_t q = p;
            const bool negative = sum[q] == '-';
            if (sum[q] == '+' || sum[q] == '-') ++q;
            q = skip_space(sum, q);
            std::size_t e = q;
            while (e < sum.size() && sum[e] != '+' && sum[e] != '-' && !text::is_space(sum[e])) ++e;
            if (e == q) {
                p += 1;
                continue;
            }
            p = e;
            const std::string t = sum.substr(q, e - q);
            if (is_reg16(text::lower(t))) {
                m.regs.push_back(text::lower(t));
            } else if (const auto n = number(t)) {
                m.disp += negative ? -*n : *n;
            } else if (const auto eq = equate_number(equ, t)) {
                m.disp += negative ? -*eq : *eq;
            } else {
                if (m.sym) throw Asm16Error("two symbols in `" + std::string(a) + "`");
                m.sym = t;
            }
        }
        o.kind = Operand::Kind::Mem;
        o.mem = std::move(m);
        return o;
    }
    const bool quoted_char = s.size() == 3 && s[0] == '\'' && s[2] == '\'';
    bool negated_word = false;
    if (!s.empty() && s[0] == '-') {
        const std::size_t i = skip_space(s, 1);
        negated_word = i < s.size() && std::all_of(s.begin() + static_cast<std::ptrdiff_t>(i), s.end(), is_word);
    }
    if (number(s) || quoted_char || negated_word) {
        o.kind = Operand::Kind::Imm;
        o.text = std::string(s);
        return o;
    }
    const std::string name(s);
    if (equ && equ->values.contains(name)) {
        o.kind = Operand::Kind::Imm;
        o.text = name;
        return o;
    }
    if (equ) {
        if (const auto d = equ->sizes.find(name); d != equ->sizes.end()) {   // (a variable named bare: the memory it is)
            o.kind = Operand::Kind::Mem;
            o.mem = Mem{.size = d->second, .sym = name, .text = std::string(a)};
            return o;
        }
    }
    o.kind = Operand::Kind::Sym;
    o.text = name;
    return o;
}

// ---- the old assembler's listing ------------------------------------------------------------------------------------

namespace {

std::string expand_tabs(std::string_view s) {
    std::string out;
    std::size_t col = 0;
    for (const char c : s) {
        if (c == '\t') {
            const std::size_t n = 8 - col % 8;
            out.append(n, ' ');
            col += n;
        } else {
            out.push_back(c);
            col = (c == '\n' || c == '\r') ? 0 : col + 1;
        }
    }
    return out;
}

}  // namespace

Listing parse_listing(std::string_view lst_in) {
    // MASM 5's listing lines (docs: a line is ` OFFSET  BYTES   source`; a segment's head and its ENDS name the segment)
    static const Regex code_line(R"(^ ([0-9A-F]{4})  ((?:[0-9A-F]{2,4}R? ?)*)\s*(.*)$)");
    static const Regex seg_head(R"(^ ([0-9A-F]{4}) {2,}.*?\b([A-Za-z_$@?][\w$@?]*)\s+SEGMENT\b)", RegexFlags::IgnoreCase);
    static const Regex seg_end(R"(^ [0-9A-F]{4}\s+[A-Za-z_$@?][\w$@?]*\s+ENDS\b)", RegexFlags::IgnoreCase);
    static const Regex label_line(R"(^ ([0-9A-F]{4})\s+(?:[0-9A-F ]+R?\s+)?\s*([A-Za-z_$@?][\w$@?]*)(?::|\s+LABEL\b))",
                                  RegexFlags::IgnoreCase);
    Listing out;
    std::optional<std::string> seg;
    const std::string lst = text::replace_all(lst_in, "\r", "");
    for (const std::string_view raw : text::split(lst, '\n')) {
        const std::string l = expand_tabs(raw);
        if (const auto m = code_line.match(l); m && seg && l.size() > 32) {
            std::uint32_t nbytes = 0;
            std::string hex;
            bool whole = true;
            for (const std::string_view t : text::split_ws(m->group(2))) {
                const std::string_view digits = text::trim(t, "R");
                if (digits.size() % 2) whole = false;
                nbytes += static_cast<std::uint32_t>(digits.size() / 2);
                hex += digits;
            }
            if (whole) {   // (a token of an odd number of digits is no bytes' column)
                ListedLine line;
                line.seg = *seg;
                line.offset = *text::parse_int<std::uint32_t>(m->group(1), 16);
                line.length = nbytes;
                std::string collapsed;
                bool in_space = false;
                for (const char c : l.substr(32)) {
                    if (text::is_space(c)) {
                        in_space = true;
                    } else {
                        if (in_space && !collapsed.empty()) collapsed.push_back(' ');
                        in_space = false;
                        collapsed.push_back(c);
                    }
                }
                line.text = collapsed.substr(0, 24);
                for (std::size_t i = 0; i + 1 < hex.size(); i += 2)
                    line.bytes.push_back(static_cast<std::uint8_t>(*text::parse_int<unsigned>(hex.substr(i, 2), 16)));
                out.lines.push_back(std::move(line));
            }
        }
        if (const auto m = seg_head.match(l)) {
            seg = std::string(m->group(2));
            continue;
        }
        if (seg_end.match(l)) {
            seg.reset();
            continue;
        }
        if (const auto m = label_line.match(l); m && seg)
            out.labels.insert_or_assign(std::string(m->group(2)),
                                        Place{*seg, *text::parse_int<std::uint32_t>(m->group(1), 16)});
    }
    return out;
}

Listing listing_offsets(const std::filesystem::path& source, const std::filesystem::path& assembler,
                        const AssemblerRunner& run_assembler, std::string_view args) {
    fs::ScopedTempDir work("asm16");
    fs::write_file(work.path() / "ASM.EXE", fs::read_file(assembler));
    std::string src = fs::read_text(source);
    src = text::replace_all(text::replace_all(src, "\r\n", "\n"), "\n", "\r\n");   // (the assembler wants CRLF)
    fs::write_text(work.path() / "T.ASM", src);
    run_assembler(work.path(), args);
    return parse_listing(fs::read_text(work.path() / "T.LST"));
}

translate::backends::assembly::Document module(std::string_view name, std::string_view text_in) {
    translate::backends::assembly::Document doc;
    doc.units.push_back(translate::backends::assembly::MachineFragment{std::string(name), std::string(text_in), false});
    return doc;
}

}  // namespace cb::port::asm16
