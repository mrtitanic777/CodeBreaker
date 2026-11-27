// source/dialect.cpp -- see dialect.hpp: the one printer and the one parser over a dialect's data.

#include "source/dialect.hpp"

#include <algorithm>
#include <cctype>
#include <deque>
#include <format>
#include <map>
#include <mutex>
#include <nlohmann/json.hpp>
#include <optional>
#include <set>

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/text.hpp"

namespace cb::source::dialect {
namespace {

bool is_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }
bool is_hex(char c) { return std::isxdigit(static_cast<unsigned char>(c)) != 0; }
bool is_blank(char c) { return c == ' ' || c == '\t'; }

// Whether `before` ends at a shift by thirty-two (`lsr #`, `asr #`), which the description writes in decimal.
bool shift_by_32(std::string_view before, std::string_view digits, std::string_view whole) {
    if (before.size() >= 5 && before.back() == '#' && text::iequals(before.substr(before.size() - 5, 3), "ror") && (text::istarts_with(whole, "uxt") || text::istarts_with(whole, "sxt"))) return true;   // (the rotation of an extend: decimal)
    if (digits != "32" || before.size() < 5 || before.back() != '#') return false;
    const std::string_view op = before.substr(before.size() - 5, 3);
    return text::iequals(op, "lsr") || text::iequals(op, "asr");
}

std::string_view trim_zeros(std::string_view digits) {
    while (digits.size() > 1 && digits.front() == '0') digits.remove_prefix(1);
    return digits;
}

// ---- templates -----------------------------------------------------------------------------------------------

// `.org {v}` or `.DEFINE {name:<16} {v}`: literal text and {placeholders}, an optional `:<N` pad to the right.
struct Part {
    bool placeholder = false;
    std::string text;   // the literal, or the placeholder's name
    int width = 0;
};
using Template = std::vector<Part>;

Template compile(std::string_view s) {
    Template t;
    std::string literal;
    for (std::size_t i = 0; i < s.size();) {
        if (s[i] != '{') {
            literal += s[i++];
            continue;
        }
        const std::size_t close = s.find('}', i);
        if (close == std::string_view::npos) throw UsageError(std::format("dialect template `{}` has an unclosed {{", s));
        if (!literal.empty()) t.push_back({false, std::move(literal), 0}), literal.clear();
        std::string_view inner = s.substr(i + 1, close - i - 1);
        Part p;
        p.placeholder = true;
        if (const std::size_t colon = inner.find(':'); colon != std::string_view::npos) {
            if (inner.substr(colon + 1, 1) == "<") p.width = std::atoi(std::string(inner.substr(colon + 2)).c_str());
            inner = inner.substr(0, colon);
        }
        p.text = std::string(inner);
        t.push_back(std::move(p));
        i = close + 1;
    }
    if (!literal.empty()) t.push_back({false, std::move(literal), 0});
    return t;
}

// Where `lit` stands in `s` from `from`, outside quotes (a word, when `word`: not run into letters either side); npos when nowhere.
std::size_t find_outside(std::string_view s, std::size_t from, std::string_view lit, bool word) {
    bool quoted = false;
    char quote = 0;
    for (std::size_t i = from; i + lit.size() <= s.size(); ++i) {
        const char c = s[i];
        if (quoted) {
            if (c == quote) quoted = false;
            continue;
        }
        if (c == '"' || c == '\'') {
            quoted = true;
            quote = c;
            continue;
        }
        if (!text::iequals(s.substr(i, lit.size()), lit)) continue;
        if (word && ((i > 0 && is_word(s[i - 1])) || (i + lit.size() < s.size() && is_word(s[i + lit.size()])))) continue;
        return i;
    }
    return std::string_view::npos;
}

bool all_blank(std::string_view s) { return std::ranges::all_of(s, [](char c) { return is_blank(c); }); }
bool alphabetic(std::string_view s) { return !s.empty() && std::ranges::all_of(s, [](char c) { return std::isalpha(static_cast<unsigned char>(c)) != 0; }); }

using Captures = std::vector<std::pair<std::string_view, std::string_view>>;

// Does `s` read as the template? The captures are views into `s` (and the template).
bool match(const Template& t, std::string_view s, Captures& out) {
    out.clear();
    std::size_t pos = 0;
    const auto skip_blanks = [&] {
        while (pos < s.size() && is_blank(s[pos])) ++pos;
    };
    skip_blanks();
    for (std::size_t i = 0; i < t.size(); ++i) {
        const Part& p = t[i];
        if (!p.placeholder) {
            const std::string_view lit = text::trim(p.text);
            if (lit.empty()) {
                skip_blanks();
                continue;
            }
            skip_blanks();
            if (pos + lit.size() > s.size() || !text::iequals(s.substr(pos, lit.size()), lit)) return false;
            // (a register's name with its number after it -- `float fpul,dr{w}` -- is one word: a literal that ends a longer text with no blank before a placeholder attaches to it)
            const bool attached = i + 1 < t.size() && t[i + 1].placeholder && !is_blank(p.text.back()) && !std::ranges::all_of(lit, is_word);
            if (!attached && is_word(lit.back()) && lit.back() != '_' && !lit.ends_with("0x") && pos + lit.size() < s.size() && is_word(s[pos + lit.size()])) return false;   // (a literal ending in _ is a prefix:  then the name)
            pos += lit.size();
            skip_blanks();
            continue;
        }
        std::size_t end = s.size();
        if (i + 1 < t.size()) {
            const std::string_view lit = text::trim(t[i + 1].text);
            if (lit.empty()) {   // whitespace between two placeholders: the capture runs to the next blank
                end = pos;
                while (end < s.size() && !is_blank(s[end])) ++end;
            } else {
                end = find_outside(s, pos, lit, alphabetic(lit));
                if (end == std::string_view::npos) return false;
            }
        }
        const std::string_view value = text::trim(s.substr(pos, end - pos));
        if (value.empty()) return false;
        if ((p.text[0] == 'w' || p.text == "m") && !std::ranges::all_of(value.starts_with('-') && value.size() > 1 && p.text[0] == 'w' ? value.substr(1) : value, [](char c) { return is_word(c) || c == '.' || c == '$'; })) return false;   // ({w}: a name, a number or a word: no operand syntax)
        out.emplace_back(p.text, value);
        pos = end;
    }
    skip_blanks();
    return pos == s.size();
}

std::string_view capture(const Captures& c, std::string_view name) {
    for (const auto& [n, v] : c)
        if (n == name) return v;
    return {};
}

// ---- numbers -----------------------------------------------------------------------------------------------

struct NumberFormat {
    std::string prefix = "0x";
    std::string suffix;
    bool upper = false;
    bool even = false;           // an odd count of digits is padded to an even one
    bool letter_lead = false;    // a number whose first digit is a letter gets a 0 ahead (`0C000h`)
};

// ---- the dialect ----------------------------------------------------------------------------------------------

struct Rule {
    Template neutral, dialect;
    bool print = true, read = true, bytes = false;
    std::string separator;   // between a list's items, when not the dialect's
    bool exact = false;      // numbers keep the digits the neutral text gave (`org $200000`, `org $0`)
    bool incbin = false;   // the line brings a file in (`.incbin "x.chr"`): its bytes stand in the neutral text
    bool decimal = false;    // an instruction row: its numbers are decimal (`MOVEQ #1,D0`)
    int pad = 0;             // an instruction row: the number {w} is padded to this many hex digits (`($00200000).L`)
    unsigned long decimal_limit = 0xFFFF;   // an instruction row: only numbers up to this are written in decimal (SuperH: an unsigned immediate over 9 is hex)
    std::string neutral_head;   // the first word of the neutral template (when it starts with a literal), for a quick test
};

}  // namespace

struct Dialect::Impl {
    std::string name;
    std::string comment = ";";
    NumberFormat number;
    std::vector<Rule> rules;
    std::vector<Rule> insn_rules;   // rows for an instruction's own wording
    std::string reg_prefix;                          // `$` for `$sp` (GNU as's MIPS)
    std::set<std::string, std::less<>> regs;         // the register names that take it
    bool decimal = false;                            // small numbers of an instruction are decimal ...
    std::set<std::string, std::less<>> decimal_unless;   // ... except in these mnemonics (lui, ori: hex)
    struct Wrap {
        std::string from_open, to_open, to_close;
    };
    std::vector<Wrap> wraps;
    std::string forced_suffix;                       // DASM's `STA.w x`: the absolute form of a zero-page address
    std::string forced_prefix;                       // ca65's `STA a:x`
    std::map<std::string, int> forced_opcodes;       // mnemonic (upper case) -> its absolute opcode
    std::string long_prefix;                         // ca65's `LDA f:x`: the 24-bit form of an address below $10000
    std::map<std::string, std::pair<int, int>> long_opcodes;   // mnemonic (upper case) -> its long opcode, and its long indexed (,x) one
    std::string wrap_out(std::string s) const;
    std::string wrap_in(std::string s) const;
    std::string decimals(std::string_view s, unsigned long limit = 0xFFFF) const;
    std::string with_registers(std::string_view s) const;
    std::string without_registers(std::string_view s) const;
    bool tight_commas = false;       // an instruction read back has no blank after its commas (the rows say `a,b`)
    bool decimal_keep_zero = false;  // a zero stays 0x0 (the description writes some fields as plain 0)
    int mnemonic_pad = 0;           // the mnemonic is padded to this many columns when it has operands (GNU as for SuperH)
    bool mnemonic_upper = false;    // the dialect writes the mnemonic (the first word) in capitals; the description's is lower case
    struct Replace {   // how the dialect spells an operand the description spells otherwise (`( rLY )` as `(rLY)`, `,X` as `,x`)
        std::string from, to;
        bool both = false;   // the parser undoes it (`,x` read as `,X`) and not only the printer does it
    };
    std::vector<Replace> replace;
    std::vector<std::pair<std::string, std::string>> after_parse;   // applied to an instruction the parser has read back (`,lsl` as `, lsl`)
    std::string list_separator = ",";   // between the items of a list of numbers and strings
    bool label_at_column_zero = false;  // a label is the name alone at the head of a line (DASM)
    Template label = compile("{name}:");
    std::map<std::string, std::vector<std::string>, std::less<>> blocks;
    // the data lines: how a row is written
    std::size_t row = 16, run = 16, string_min = 4;
    // WLA-DX's slots: `.MEMORYMAP` ... `.ENDME` lists `SLOT n address`, `.BANK b SLOT n` makes a following `.ORG` relative to that slot
    struct Layout {
        bool present = false;
        std::string begin, end;
        Template slot, bank;
        std::vector<std::pair<std::string, std::string>> skip;
    } layout;
    // ca65's segments: `.segment "BANK0"` loads where the linker configuration (kept beside the source) says: `memory` lines name an area and its start,
    // `segment` lines say which area a segment loads into
    struct Segments {
        bool present = false;
        Template directive, memory, segment;
    } segments;
    bool identity = false;

    // -- numbers --
    // `text` with each number of the neutral spelling (0x<hex>) written the dialect's way.
    std::string numbers_out(std::string_view text, bool exact = false) const;   // exact: the digits as given (no padding to pairs)
    // `text` with each number of the dialect's spelling written 0x<hex>.
    std::string numbers_in(std::string_view text) const;
    std::string convert_list_out(std::string_view list) const { return numbers_out(list); }
    std::string render(const Template& t, const Captures& caps, bool to_dialect, std::string_view separator = {}, bool convert = true, bool exact = false, int name_width = 0) const;
    std::string number_text(std::uint64_t value, unsigned digits) const;   // a number the dialect's way, directly
    const Rule* rule_for(std::string_view head) const;
    std::string instruction(std::string_view text) const;
    std::string instruction_in(std::string_view text) const;   // the parser's: an instruction as the description reads it
};

namespace {
std::vector<std::string_view> items_of(std::string_view list);   // (below)
}

std::string Dialect::Impl::numbers_out(std::string_view s, bool exact) const {
    if (identity || (s.find("0x") == std::string_view::npos && s.find("0X") == std::string_view::npos)) return std::string(s);
    std::string out;
    out.reserve(s.size() + 8);
    char quote = 0;
    for (std::size_t i = 0; i < s.size();) {
        const char c = s[i];
        if (quote != 0) {
            out += c;
            if (c == quote) quote = 0;
            ++i;
            continue;
        }
        if (c == '"' || c == '\'') {
            quote = c;
            out += c;
            ++i;
            continue;
        }
        if (c == '0' && i + 2 < s.size() + 0 && (s[i + 1] == 'x' || s[i + 1] == 'X') && (i == 0 || !is_word(s[i - 1]))) {
            std::size_t e = i + 2;
            while (e < s.size() && is_hex(s[e])) ++e;
            if (e > i + 2 && !(e < s.size() && is_word(s[e]))) {
                std::string digits(s.substr(i + 2, e - i - 2));
                if (number.even && !exact && digits.size() % 2 != 0) digits.insert(digits.begin(), '0');
                for (char& d : digits) d = number.upper ? static_cast<char>(std::toupper(static_cast<unsigned char>(d))) : static_cast<char>(std::tolower(static_cast<unsigned char>(d)));
                if (number.letter_lead && std::isalpha(static_cast<unsigned char>(digits[0])) != 0) digits.insert(digits.begin(), '0');
                out += number.prefix;
                out += digits;
                out += number.suffix;
                i = e;
                continue;
            }
        }
        out += c;
        ++i;
    }
    return out;
}

std::string Dialect::Impl::numbers_in(std::string_view s) const {
    if (identity || (number.prefix == "0x" && number.suffix.empty()) || (number.suffix.empty() && !number.prefix.empty() && s.find(number.prefix) == std::string_view::npos)) return std::string(s);
    std::string out;
    out.reserve(s.size() + 8);
    char quote = 0;
    for (std::size_t i = 0; i < s.size();) {
        const char c = s[i];
        if (quote != 0) {
            out += c;
            if (c == quote) quote = 0;
            ++i;
            continue;
        }
        if (c == '"' || c == '\'') {
            quote = c;
            out += c;
            ++i;
            continue;
        }
        const bool word_start = i == 0 || !is_word(s[i - 1]);
        if (!number.prefix.empty() && word_start && s.substr(i, number.prefix.size()) == number.prefix) {
            std::size_t e = i + number.prefix.size();
            const std::size_t first = e;
            while (e < s.size() && is_hex(s[e])) ++e;
            if (e > first && !(e < s.size() && is_word(s[e]))) {
                if (number.suffix.empty() || text::iequals(s.substr(e, number.suffix.size()), number.suffix)) {
                    out += "0x";   // (the description's assembler reads the number's value, as its text writes it: no leading zeros)
                    out += trim_zeros(s.substr(first, e - first));
                    i = e + number.suffix.size();
                    continue;
                }
            }
        } else if (number.prefix.empty() && !number.suffix.empty() && word_start && std::isdigit(static_cast<unsigned char>(c)) != 0) {   // 0C000h
            std::size_t e = i;
            while (e < s.size() && is_hex(s[e])) ++e;
            if (e < s.size() && text::iequals(s.substr(e, number.suffix.size()), number.suffix) &&
                !(e + number.suffix.size() < s.size() && is_word(s[e + number.suffix.size()]))) {
                out += "0x";
                out += trim_zeros(s.substr(i, e - i));
                i = e + number.suffix.size();
                continue;
            }
        }
        out += c;
        ++i;
    }
    return out;
}

std::string Dialect::Impl::number_text(std::uint64_t value, unsigned digits) const {
    std::string d = number.upper ? std::format("{:0{}X}", value, digits) : std::format("{:0{}x}", value, digits);
    if (number.even && d.size() % 2 != 0) d.insert(d.begin(), '0');
    if (number.letter_lead && std::isalpha(static_cast<unsigned char>(d[0])) != 0) d.insert(d.begin(), '0');
    return number.prefix + d + number.suffix;
}

const Rule* Dialect::Impl::rule_for(std::string_view head) const {
    for (const Rule& r : rules)
        if (r.print && r.neutral_head == head) return &r;
    return nullptr;
}

std::string Dialect::Impl::render(const Template& t, const Captures& caps, bool to_dialect, std::string_view separator, bool convert, bool exact, int name_width) const {
    const std::string_view sep = separator.empty() ? std::string_view(list_separator) : separator;
    std::string out;
    for (const Part& p : t) {
        if (!p.placeholder) {
            out += p.text;
            continue;
        }
        std::string value(capture(caps, p.text));
        if (convert && (p.text == "v" || p.text == "list")) value = to_dialect ? numbers_out(value, exact) : numbers_in(value);
        if (p.text == "list" && sep != ",") {
            std::string joined;
            for (const std::string_view item : items_of(value)) {
                if (!joined.empty()) joined += to_dialect ? sep : std::string_view(",");
                joined += item;
            }
            value = std::move(joined);
        }
        const int width = name_width > 0 && p.text == "name" ? name_width : p.width;
        if (width > 0 && value.size() < static_cast<std::size_t>(width)) value.append(static_cast<std::size_t>(width) - value.size(), ' ');
        out += value;
    }
    return out;
}

// `%up(x)` as `(x >> 16)` and the like: an operand wrapper the dialect spells with other brackets.
std::string Dialect::Impl::wrap_out(std::string s) const {
    for (const Wrap& w : wraps) {
        for (std::size_t at = s.find(w.from_open); at != std::string::npos; at = s.find(w.from_open, at + 1)) {
            int depth = 1;
            std::size_t j = at + w.from_open.size();
            for (; j < s.size() && depth > 0; ++j) depth += s[j] == '(' ? 1 : s[j] == ')' ? -1 : 0;
            if (depth != 0) break;
            const std::string inner = s.substr(at + w.from_open.size(), j - 1 - (at + w.from_open.size()));
            const std::string made = w.to_open + inner + w.to_close;
            s.replace(at, j - at, made);
        }
    }
    return s;
}

std::string Dialect::Impl::wrap_in(std::string s) const {
    for (const Wrap& w : wraps) {
        for (std::size_t at = s.find(w.to_close); at != std::string::npos; at = s.find(w.to_close, at + 1)) {
            int depth = 1;
            std::size_t k = at;
            while (k > 0 && depth > 0) {
                --k;
                depth += s[k] == ')' ? 1 : s[k] == '(' ? -1 : 0;
            }
            if (depth != 0 || s.compare(k, w.to_open.size(), w.to_open) != 0) continue;
            const std::string inner = s.substr(k + w.to_open.size(), at - (k + w.to_open.size()));
            const std::string made = w.from_open + inner + ")";
            s.replace(k, at + w.to_close.size() - k, made);
            at = k;
        }
    }
    return s;
}

std::string Dialect::Impl::instruction(std::string_view code) const {
    std::string out(code);
    Captures caps;
    bool rule_decimal = false;
    unsigned long rule_limit = 0xFFFF;
    for (const Rule& r : insn_rules) {   // an instruction the dialect words otherwise (`jsr x.l` as `JSR (x).L`)
        if (r.print && match(r.neutral, code, caps)) {
            if (r.pad > 0) {   // the number {w} in this many hex digits
                std::deque<std::string> padded_values;   // (the padded numbers the captures now view)
                for (auto& [capture_name, value] : caps) {
                    if (capture_name[0] == 'w' && value.starts_with("0x") && value.size() - 2 < static_cast<std::size_t>(r.pad)) {
                        padded_values.push_back("0x" + std::string(static_cast<std::size_t>(r.pad) - (value.size() - 2), '0') + std::string(value.substr(2)));
                        value = padded_values.back();
                    }
                }
                out = render(r.dialect, caps, true, {}, false);
                rule_decimal = r.decimal;
                rule_limit = r.decimal_limit;
                break;
            }
            out = render(r.dialect, caps, true, {}, false);
            rule_decimal = r.decimal;
            rule_limit = r.decimal_limit;
            break;
        }
    }
    const std::size_t word_end = out.find_first_of(" \t");
    if (rule_decimal || (decimal && !decimal_unless.contains(text::lower(out.substr(0, word_end))))) out = decimals(out, rule_limit);
    else out = numbers_out(out);
    if (!wraps.empty()) out = wrap_out(out);
    for (const Replace& r : replace) out = text::replace_all(out, r.from, r.to);
    if (!replace.empty()) out = text::replace_all(out, ",  ", ", ");   // (a comma the text already spaced is not spaced twice)
    if (!regs.empty()) out = with_registers(out);
    if (mnemonic_upper) {
        const std::size_t end = out.find_first_of(" \t");
        for (std::size_t i = 0; i < std::min(end, out.size()); ++i) out[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(out[i])));
    }
    if (mnemonic_pad > 0) {   // the mnemonic padded to its column
        const std::size_t blank = out.find_first_of(" 	");
        if (blank != std::string::npos) {
            const std::string rest(text::ltrim(std::string_view(out).substr(blank)));
            out = out.substr(0, blank) + std::string(std::max<std::size_t>(1, static_cast<std::size_t>(mnemonic_pad) > blank ? static_cast<std::size_t>(mnemonic_pad) - blank : 1), ' ') + rest;
        }
    }
    return out;
}

// `text` with each small number (under 0x10000) written in decimal, the larger ones the dialect's way.
std::string Dialect::Impl::decimals(std::string_view s, unsigned long limit) const {
    std::string out;
    for (std::size_t i = 0; i < s.size();) {
        if (s[i] == '0' && i + 2 < s.size() && s[i + 1] == 'x' && (i == 0 || !is_word(s[i - 1]))) {
            std::size_t e = i + 2;
            while (e < s.size() && is_hex(s[e])) ++e;
            if (e > i + 2 && e - i - 2 <= 4 && !(e < s.size() && is_word(s[e])) && (!decimal_keep_zero || std::strtoul(std::string(s.substr(i + 2, e - i - 2)).c_str(), nullptr, 16) != 0) && std::strtoul(std::string(s.substr(i + 2, e - i - 2)).c_str(), nullptr, 16) <= limit) {   // (zero stays 0x0: a field the description writes as plain 0 is another)
                out += std::to_string(std::strtoul(std::string(s.substr(i + 2, e - i - 2)).c_str(), nullptr, 16));
                i = e;
                continue;
            }
        }
        out += s[i++];
    }
    return numbers_out(out);
}

// `s` with the register names of its operands (not its mnemonic) written with the dialect's prefix: `sp` as `$sp`.
std::string Dialect::Impl::with_registers(std::string_view s) const {
    std::string out;
    const std::size_t first = s.find_first_of(" \t");
    out.append(s.substr(0, first));
    for (std::size_t i = first == std::string_view::npos ? s.size() : first; i < s.size();) {
        if (is_word(s[i]) && (i == 0 || (!is_word(s[i - 1]) && s[i - 1] != '%' && s[i - 1] != '.' && s[i - 1] != '$'))) {
            std::size_t e = i;
            while (e < s.size() && is_word(s[e])) ++e;
            if (regs.contains(s.substr(i, e - i)) && !(e < s.size() && s[e] == '(' && false)) out += reg_prefix;
            out.append(s.substr(i, e - i));
            i = e;
            continue;
        }
        out += s[i++];
    }
    return out;
}

std::string Dialect::Impl::without_registers(std::string_view s) const {
    std::string out;
    for (std::size_t i = 0; i < s.size();) {
        if (s.substr(i, reg_prefix.size()) == reg_prefix && (i == 0 || !is_word(s[i - 1]))) {
            std::size_t e = i + reg_prefix.size();
            while (e < s.size() && is_word(s[e])) ++e;
            if (regs.contains(s.substr(i + reg_prefix.size(), e - i - reg_prefix.size()))) {
                out.append(s.substr(i + reg_prefix.size(), e - i - reg_prefix.size()));
                i = e;
                continue;
            }
        }
        out += s[i++];
    }
    return out;
}

std::string Dialect::Impl::instruction_in(std::string_view written) const {
    std::string collapsed;   // (a padded mnemonic: one blank between it and its operands, for the rows below)
    std::string_view code = written;
    if (mnemonic_pad > 0) {
        const std::size_t blank = written.find_first_of(" 	");
        if (blank != std::string_view::npos) {
            collapsed = std::string(written.substr(0, blank)) + " " + std::string(text::ltrim(written.substr(blank)));
            code = collapsed;
        }
    }
    std::string tight;   // (a blank after a comma is no part of an operand list: the rows read `a,b`)
    if (tight_commas && code.find(", ") != std::string_view::npos) {
        for (std::size_t k = 0; k < code.size(); ++k) {
            tight += code[k];
            if (code[k] == ',')
                while (k + 1 < code.size() && is_blank(code[k + 1])) ++k;
        }
        code = tight;
    }
    std::string out;
    Captures caps;
    bool matched = false, rule_decimal = false, rule_literal = false;
    for (const Rule& r : insn_rules) {
        if (r.read && match(r.dialect, code, caps)) {
            out = render(r.neutral, caps, false, {}, false);
            matched = true;
            rule_decimal = r.decimal;
            rule_literal = r.exact;   // (an instruction row marked exact: its numbers are the description's own spelling)
            break;
        }
    }
    if (!matched) out = std::string(code);
    if (!regs.empty()) out = without_registers(out);
    if (!wraps.empty()) out = wrap_in(out);   // (before the numbers: `(x >> 16)` is read as `%up(x)`)
    if ((decimal && !rule_literal) || rule_decimal) {   // (the description's assembler reads a number as 0x<hex>)
        std::string hex;
        const std::string_view s = out;
        for (std::size_t i = 0; i < s.size();) {
            if (std::isdigit(static_cast<unsigned char>(s[i])) != 0 && (i == 0 || !(is_word(s[i - 1]) || s[i - 1] == '$' || s[i - 1] == '.'))) {
                std::size_t e = i;
                while (e < s.size() && std::isdigit(static_cast<unsigned char>(s[e])) != 0) ++e;
                if (!(e < s.size() && (is_word(s[e]) || s[e] == '*')) && !shift_by_32(s.substr(0, i), s.substr(i, e - i), s) && !(decimal_keep_zero && e - i == 1 && s[i] == '0')) {
                    hex += std::format("0x{:x}", std::strtoull(std::string(s.substr(i, e - i)).c_str(), nullptr, 10));
                    i = e;
                    continue;
                }
            }
            hex += s[i++];
        }
        out = std::move(hex);
    }
    out = numbers_in(out);
    for (auto it = replace.rbegin(); it != replace.rend(); ++it)   // (undone in the order opposite to the printer's)
        if (it->both) out = text::replace_all(out, it->to, it->from);
    for (const auto& [from, to] : after_parse) out = text::replace_all(out, from, to);   // (what the description spells otherwise than the reversal leaves it)
    if (mnemonic_upper) {
        const std::size_t end = out.find_first_of(" \t");
        for (std::size_t i = 0; i < std::min(end, out.size()); ++i) out[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(out[i])));
    }
    if (mnemonic_pad > 0) {   // (the description reads the mnemonic and its operands with one blank between)
        const std::size_t blank = out.find_first_of(" 	");
        if (blank != std::string::npos) out = out.substr(0, blank) + " " + std::string(text::ltrim(std::string_view(out).substr(blank)));
    }
    return out;
}

Dialect::Dialect(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}
Dialect::~Dialect() = default;

const std::string& Dialect::name() const { return impl_->name; }
bool Dialect::neutral() const { return impl_->identity; }

std::string Dialect::number(std::uint64_t value, unsigned digits) const {
    return impl_->numbers_out(std::format("0x{:0{}x}", value, digits));
}

std::string Dialect::instruction(std::string_view t) const { return impl_->instruction(t); }

std::string Dialect::label_line(std::string_view name) const {
    Captures caps{{"name", name}};
    return impl_->render(impl_->label, caps, true);
}

namespace {

// Where a line's comment starts (outside quotes), or npos.
std::size_t comment_at(std::string_view s, char comment) {
    char quote = 0;
    for (std::size_t i = 0; i < s.size(); ++i) {
        const char c = s[i];
        if (quote != 0) {
            if (c == '\\') ++i;
            else if (c == quote) quote = 0;
        } else if (c == '"') {
            quote = c;
        } else if (c == comment) {
            return i;
        }
    }
    return std::string_view::npos;
}

// A byte item that is the low or high byte of a value (`<x`, `>x`) as the expression the directive layer reads.
std::string low_high(std::string_view list) {
    std::string out;
    for (std::size_t i = 0; i < list.size();) {
        if ((list[i] == '<' || list[i] == '>') && (i == 0 || list[i - 1] == ',')) {
            std::size_t e = i + 1;
            while (e < list.size() && list[e] != ',') ++e;
            const std::string_view operand = list.substr(i + 1, e - i - 1);
            out += list[i] == '<' ? std::format("({})&0xff", operand) : std::format("({})>>8", operand);
            i = e;
            continue;
        }
        out += list[i++];
    }
    return out;
}

bool label_name(std::string_view s) {
    if (s.empty() || !(std::isalpha(static_cast<unsigned char>(s[0])) != 0 || s[0] == '_' || s[0] == '.')) return false;
    return std::ranges::all_of(s, [](char c) { return is_word(c) || c == '.'; });
}

}  // namespace

std::string Dialect::line(std::string_view neutral_line, int name_width) const {
    const Impl& d = *impl_;
    if (d.identity) return std::string(neutral_line);
    std::size_t indent_end = 0;
    while (indent_end < neutral_line.size() && is_blank(neutral_line[indent_end])) ++indent_end;
    const std::string_view indent = neutral_line.substr(0, indent_end);
    const std::string_view rest = neutral_line.substr(indent_end);
    if (rest.empty()) return std::string(neutral_line);
    if (!rest.empty() && d.comment.size() == 1 && rest.front() == d.comment[0] && d.comment != ";") return std::string(neutral_line);   // (a comment already written in the dialect's)
    const std::size_t semi = comment_at(rest, ';');
    std::string_view code = semi == std::string_view::npos ? rest : rest.substr(0, semi);
    std::string tail;
    if (semi != std::string_view::npos) {
        const std::string_view trimmed = text::rtrim(code);
        tail = std::string(rest.substr(trimmed.size()));
        if (const std::size_t at = tail.find(';'); at != std::string::npos && d.comment != ";") tail.replace(at, 1, d.comment);
        code = trimmed;
    }
    if (code.empty()) return std::string(indent) + tail;
    // a label line
    if (code.back() == ':' && label_name(code.substr(0, code.size() - 1))) {
        Captures caps{{"name", code.substr(0, code.size() - 1)}};
        return std::string(indent) + d.render(d.label, caps, true) + tail;
    }
    if (code.front() == '.') {
        const std::size_t head_end = code.find_first_of(" \t");
        const std::string_view head = code.substr(0, head_end);
        Captures caps;
        for (const Rule& r : d.rules) {
            if (!r.print || !text::iequals(r.neutral_head, head) || !match(r.neutral, code, caps)) continue;
            return std::string(indent) + d.render(r.dialect, caps, true, r.separator, true, r.exact, name_width) + tail;
        }
        return std::string(indent) + d.numbers_out(code) + tail;   // (a directive no rule names: its numbers still written the dialect's way)
    }
    return std::string(indent) + d.instruction(code) + tail;
}

namespace {

// Items of a list at its top-level commas (quotes kept).
std::vector<std::string_view> items_of(std::string_view list) {
    std::vector<std::string_view> out;
    char quote = 0;
    std::size_t start = 0;
    for (std::size_t i = 0; i < list.size(); ++i) {
        const char c = list[i];
        if (quote != 0) {
            if (c == '\\') ++i;
            else if (c == quote) quote = 0;
        } else if (c == '"') {
            quote = c;
        } else if (c == ',') {
            out.push_back(text::trim(list.substr(start, i - start)));
            start = i + 1;
        }
    }
    out.push_back(text::trim(list.substr(start)));
    return out;
}

bool printable(std::uint8_t b) { return b >= 32 && b < 127 && b != 0x22 && b != 0x5C; }

// A row's bytes as items: runs of string_min or more printable characters as one string, the rest as 0x<xx>.
std::string row_items(std::span<const std::uint8_t> row, std::size_t string_min) {
    std::string out;
    std::size_t i = 0;
    while (i < row.size()) {
        std::size_t j = i;
        while (j < row.size() && printable(row[j])) ++j;
        if (!out.empty()) out += ',';
        if (j - i >= string_min) {
            out += '"';
            out += as_text(row.subspan(i, j - i));
            out += '"';
            i = j;
        } else {
            out += std::format("0x{:02x}", row[i]);
            ++i;
        }
    }
    return out;
}

}  // namespace

std::vector<std::string> Dialect::data_lines(ByteView data) const {
    const Impl& d = *impl_;
    std::vector<std::string> out;
    // (the dialect's own rows, written directly: the byte row's and the fill's templates with the numbers already in its spelling)
    const Rule* byte_rule = d.identity ? nullptr : d.rule_for(".byte");
    const Rule* fill_rule = d.identity ? nullptr : d.rule_for(".space");
    const bool direct = byte_rule != nullptr && fill_rule != nullptr;
    std::size_t i = 0;
    while (i < data.size()) {
        std::size_t run = 1;
        while (i + run < data.size() && data[i + run] == data[i]) ++run;
        if (run >= d.run) {
            if (direct) {
                const std::string n = std::to_string(run), v = d.number_text(data[i], 2);
                out.push_back("        " + d.render(fill_rule->dialect, Captures{{"n", n}, {"v", v}}, true, fill_rule->separator, false));
                i += run;
                continue;
            }
            out.push_back(line(std::format("        .space {}, 0x{:02x}", run, data[i])));
            i += run;
            continue;
        }
        std::size_t row_end = i;
        while (row_end < data.size() && row_end - i < d.row) {
            std::size_t same = 1;
            while (row_end + same < data.size() && data[row_end + same] == data[row_end]) ++same;
            if (same >= d.run) break;   // (the run after the row is a fill line of its own)
            ++row_end;
        }
        if (direct) {
            std::string items;
            const std::span<const std::uint8_t> row = data.subspan(i, row_end - i);
            for (std::size_t k = 0; k < row.size();) {
                std::size_t j = k;
                while (j < row.size() && printable(row[j])) ++j;
                if (!items.empty()) items += ',';
                if (j - k >= d.string_min) {
                    items += '"';
                    items += as_text(row.subspan(k, j - k));
                    items += '"';
                    k = j;
                } else {
                    items += d.number_text(row[k], 2);
                    ++k;
                }
            }
            out.push_back("        " + d.render(byte_rule->dialect, Captures{{std::string_view("list"), std::string_view(items)}}, true, byte_rule->separator, false));
        } else {
            out.push_back(line("        .byte " + row_items(data.subspan(i, row_end - i), d.string_min)));
        }
        i = row_end;
    }
    return out;
}

std::vector<std::string> Dialect::block(std::string_view name, const Vars& vars) const {
    const auto it = impl_->blocks.find(name);
    if (it == impl_->blocks.end()) return {};
    std::vector<std::string> out;
    for (std::string text : it->second) {
        for (const auto& [key, value] : vars) text = text::replace_all(text, "{" + key + "}", value);
        out.push_back(std::move(text));
    }
    return out;
}

bool Dialect::has_block(std::string_view name) const { return impl_->blocks.contains(name); }

namespace {

// ca65's `.loword(x)` inside the bytes of a forced form (`STA a:.loword(label)`), said as a mask the directive layer's expressions read.
std::string without_loword(std::string s) {
    for (std::size_t at = s.find(".loword("); at != std::string::npos; at = s.find(".loword(", at)) s.replace(at, 8, "0xffff&(");
    return s;
}

}  // namespace

std::string Dialect::parse(std::string_view source, const Aux& aux) const {
    const Impl& d = *impl_;
    if (d.identity) return std::string(source);
    std::string out;
    out.reserve(source.size() + source.size() / 8);
    std::map<std::string, std::int64_t> segment_base;   // (the linker configuration's: segment -> where it loads)
    if (d.segments.present && !aux.linker_config.empty()) {
        std::map<std::string, std::int64_t> area_start;
        std::vector<std::pair<std::string, std::string>> loads;
        Captures caps;
        for (const std::string_view l : text::split(aux.linker_config, '\n')) {
            const std::string_view t = text::trim(l);
            if (match(d.segments.memory, t, caps)) {
                const std::string n = d.numbers_in(capture(caps, "v"));
                if (const auto v = n.starts_with("0x") ? text::parse_int<std::int64_t>(std::string_view(n).substr(2), 16) : text::parse_int<std::int64_t>(n, 10))
                    area_start[std::string(capture(caps, "m"))] = *v;
            } else if (match(d.segments.segment, t, caps)) {
                loads.emplace_back(std::string(capture(caps, "s")), std::string(capture(caps, "m")));
            }
        }
        for (const auto& [segment, area] : loads)
            if (const auto it = area_start.find(area); it != area_start.end()) segment_base[segment] = it->second;
    }
    std::map<std::int64_t, std::int64_t> slot_base;
    std::optional<std::int64_t> current_slot;
    bool in_map = false;
    std::string skip_until;
    std::uint32_t number = 0;
    const char comment = d.comment.empty() ? ';' : d.comment[0];
    const auto fail = [&](const std::string& what) { throw ParseError(d.name, number, 0, what); };
    const auto value_of = [&](std::string_view v) -> std::optional<std::int64_t> {
        const std::string n = d.numbers_in(v);
        if (n.empty()) return std::nullopt;
        return n.starts_with("0x") ? text::parse_int<std::int64_t>(std::string_view(n).substr(2), 16) : text::parse_int<std::int64_t>(n, 10);
    };

    std::size_t at = 0;
    while (at <= source.size()) {
        const std::size_t nl = source.find('\n', at);
        std::string_view raw = source.substr(at, nl == std::string_view::npos ? std::string_view::npos : nl - at);
        at = nl == std::string_view::npos ? source.size() + 1 : nl + 1;
        ++number;
        if (!raw.empty() && raw.back() == '\r') raw.remove_suffix(1);
        std::string_view code = raw;
        if (const std::size_t c = comment_at(raw, comment); c != std::string_view::npos) code = raw.substr(0, c);
        code = text::trim(code);
        if (code.empty()) {
            out += '\n';
            continue;
        }
        if (!skip_until.empty()) {
            if (text::iequals(code, skip_until)) skip_until.clear();
            out += '\n';
            continue;
        }
        if (d.layout.present) {
            if (in_map) {
                Captures caps;
                if (text::iequals(code, d.layout.end)) {
                    in_map = false;
                } else if (match(d.layout.slot, code, caps)) {
                    const auto n = value_of(capture(caps, "n")), base = value_of(capture(caps, "v"));
                    if (!n || !base) fail("a slot is `SLOT number address`");
                    slot_base[*n] = *base;
                }
                out += '\n';
                continue;
            }
            if (text::iequals(code, d.layout.begin)) {
                in_map = true;
                out += '\n';
                continue;
            }
            bool skipped = false;
            for (const auto& [begin, end] : d.layout.skip) {
                if (text::iequals(code, begin)) {
                    skip_until = end;
                    skipped = true;
                    break;
                }
            }
            if (skipped) {
                out += '\n';
                continue;
            }
            Captures caps;
            if (match(d.layout.bank, code, caps)) {
                const auto n = value_of(capture(caps, "n"));
                if (!n || !slot_base.contains(*n)) fail("a bank in a slot the memory map does not define");
                current_slot = *n;
                out += '\n';
                continue;
            }
        }
        if (d.segments.present) {
            Captures caps;
            if (match(d.segments.directive, code, caps)) {
                const std::string name(text::trim(capture(caps, "s"), "\""));
                const auto it = segment_base.find(name);
                if (it == segment_base.end()) fail("a segment the linker configuration does not place: " + name);
                out += std::format("        .org 0x{:x}\n", it->second);
                continue;
            }
        }
        // labels at the head of the line
        std::string prefix;
        for (;;) {
            const std::size_t colon = code.find(':');
            if (colon == std::string_view::npos || !label_name(code.substr(0, colon))) break;
            prefix += std::string(code.substr(0, colon + 1)) + " ";
            code = text::ltrim(code.substr(colon + 1));
            if (code.empty()) break;
        }
        if (code.empty()) {
            out += prefix;
            out += '\n';
            continue;
        }
        bool done = false;
        Captures caps;
        for (const Rule& r : d.rules) {
            if (!r.read || !match(r.dialect, code, caps)) continue;
            if (r.bytes && capture(caps, "list").find('"') == std::string_view::npos) {   // (numbers only: the commonest row, one pass)
                out += prefix + "        .byte " + low_high(d.numbers_in(capture(caps, "list"))) + '\n';
                done = true;
                break;
            }
            if (r.bytes) {
                std::string run;
                const auto flush = [&] {
                    if (run.empty()) return;
                    out += "        .byte " + run + '\n';
                    run.clear();
                };
                bool first = true;
                for (const std::string_view item : items_of(capture(caps, "list"))) {
                    if (!item.empty() && item.front() == '"') {
                        flush();
                        out += (first ? prefix : std::string()) + "        .ascii " + std::string(item) + '\n';
                    } else {
                        if (!run.empty()) run += ',';
                        run += low_high(d.numbers_in(item));
                        if (first && !prefix.empty()) {
                            out += prefix + '\n';
                            prefix.clear();
                        }
                    }
                    first = false;
                }
                flush();
                done = true;
                break;
            }
            if (r.incbin) {
                const std::string file(text::trim(capture(caps, "s"), "\""));
                const Bytes data = fs::read_file(aux.folder / file);
                for (std::size_t i = 0; i < data.size(); i += 16) {
                    out += "        .byte ";
                    for (std::size_t k = i; k < std::min(data.size(), i + 16); ++k) out += std::format("{}0x{:02x}", k == i ? "" : ",", data[k]);
                    out += '\n';
                }
                out += prefix + '\n';
                done = true;
                break;
            }
            if (r.neutral.empty()) {   // a line the engine does not need (DASM's `ORG`, `processor 6502`)
                out += prefix + '\n';
                done = true;
                break;
            }
            std::string neutral = d.render(r.neutral, caps, false);
            if (d.layout.present && current_slot && text::iequals(r.neutral_head, ".org")) {   // an origin inside a slot's bank
                if (const auto v = value_of(capture(caps, "v"))) neutral = std::format(".org 0x{:x}", slot_base.at(*current_slot) + *v);
            }
            out += prefix + "        " + neutral + '\n';
            done = true;
            break;
        }
        if (done) continue;
        if (!d.long_prefix.empty()) {   // `LDA f:x`, `LDA f:x,x`: the long form, as its four bytes
            const std::size_t blank = code.find_first_of(" \t");
            if (blank != std::string_view::npos) {
                const auto op = d.long_opcodes.find(text::upper(code.substr(0, blank)));
                std::string_view rest = text::trim(code.substr(blank));
                if (op != d.long_opcodes.end() && text::istarts_with(rest, d.long_prefix)) {
                    rest = text::trim(rest.substr(d.long_prefix.size()));
                    bool indexed = false;
                    if (rest.size() > 2 && text::iequals(rest.substr(rest.size() - 2), ",x")) {
                        indexed = true;
                        rest = text::trim(rest.substr(0, rest.size() - 2));
                    }
                    const std::string operand = without_loword(d.numbers_in(rest));
                    out += std::format("{}        .byte {},({})&0xff,(({})>>8)&0xff,({})>>16\n", prefix, indexed ? op->second.second : op->second.first, operand, operand, operand);
                    continue;
                }
            }
        }
        if (!d.forced_prefix.empty()) {   // `STA a:x`
            const std::size_t blank = code.find_first_of(" \t");
            if (blank != std::string_view::npos) {
                const auto op = d.forced_opcodes.find(text::upper(code.substr(0, blank)));
                const std::string_view rest = text::trim(code.substr(blank));
                if (op != d.forced_opcodes.end() && text::istarts_with(rest, d.forced_prefix)) {
                    const std::string operand = without_loword(d.numbers_in(text::trim(rest.substr(d.forced_prefix.size()))));
                    out += std::format("{}        .byte {},({})&0xff,({})>>8\n", prefix, op->second, operand, operand);
                    continue;
                }
            }
        }
        if (!d.forced_suffix.empty()) {   // `STA.w x`: the absolute form, as its three bytes
            const std::size_t blank = code.find_first_of(" \t");
            const std::string_view word = code.substr(0, blank);
            if (blank != std::string_view::npos && word.size() > d.forced_suffix.size() && text::iequals(word.substr(word.size() - d.forced_suffix.size()), d.forced_suffix)) {
                const auto op = d.forced_opcodes.find(text::upper(word.substr(0, word.size() - d.forced_suffix.size())));
                if (op != d.forced_opcodes.end()) {
                    const std::string operand = without_loword(d.numbers_in(text::trim(code.substr(blank))));
                    out += std::format("{}        .byte {},({})&0xff,({})>>8\n", prefix, op->second, operand, operand);
                    continue;
                }
            }
        }
        if (d.label_at_column_zero && !raw.empty() && !is_blank(raw.front()) && code.find_first_of(" \t") == std::string_view::npos) {
            out += prefix + std::string(code) + ":\n";   // (DASM: a label is the name alone at the head of its line)
            continue;
        }
        if (code.front() == '.' && d.layout.present) fail("unknown directive " + std::string(code.substr(0, code.find_first_of(" \t"))));
        out += prefix + "        " + (code.front() == '.' ? d.numbers_in(code) : d.instruction_in(code)) + '\n';
    }
    return out;
}

// ---- reading a dialect's file -------------------------------------------------------------------------------------

namespace {

std::string str(const nlohmann::ordered_json& j, const char* key, std::string fallback = {}) {
    const auto it = j.find(key);
    return it != j.end() && it->is_string() ? it->get<std::string>() : fallback;
}

std::unique_ptr<Dialect::Impl> read(std::string_view name) {
    const std::filesystem::path file = config::current().config_root() / "dialects" / (std::string(name) + ".json");
    if (!std::filesystem::exists(file)) throw UsageError(std::format("no dialect {} (config/dialects/{}.json)", name, name));
    const nlohmann::ordered_json j = nlohmann::ordered_json::parse(fs::read_text(file));
    auto d = std::make_unique<Dialect::Impl>();
    d->name = std::string(name);
    d->comment = str(j, "comment", ";");
    if (const auto it = j.find("number"); it != j.end()) {
        d->number.prefix = str(*it, "prefix", "0x");
        d->number.suffix = str(*it, "suffix");
        d->number.upper = str(*it, "case", "lower") == "upper";
        d->number.even = str(*it, "pad", "none") == "even";
        d->number.letter_lead = it->value("letter_lead", false);
    }
    if (const auto it = j.find("label"); it != j.end() && it->is_string()) d->label = compile(it->get<std::string>());
    if (const auto it = j.find("data"); it != j.end()) {
        d->row = static_cast<std::size_t>(it->value("row", 16));
        d->run = static_cast<std::size_t>(it->value("run", 16));
        d->string_min = static_cast<std::size_t>(it->value("string_min", 4));
        if (d->run == 0) d->run = static_cast<std::size_t>(-1);               // (0: no fill lines)
        if (d->string_min == 0) d->string_min = static_cast<std::size_t>(-1);   // (0: no strings)
    }
    if (const auto it = j.find("directives"); it != j.end()) {
        for (const auto& row : *it) {
            Rule r;
            r.neutral = compile(str(row, "neutral"));
            r.dialect = compile(str(row, "dialect"));
            r.print = row.value("print", true);
            r.read = row.value("read", true);
            r.bytes = row.value("bytes", false);
            r.incbin = row.value("incbin", false);
            r.separator = str(row, "separator");
            r.exact = row.value("exact", false);
            r.decimal = row.value("decimal", false);
            r.pad = row.value("pad", 0);
            if (!r.neutral.empty() && !r.neutral[0].placeholder) {
                const std::string_view lit = text::trim(r.neutral[0].text);
                r.neutral_head = std::string(lit.substr(0, lit.find_first_of(" \t")));
            }
            d->rules.push_back(std::move(r));
        }
    }
    if (const auto it = j.find("instructions"); it != j.end()) {
        for (const auto& row : *it) {
            Rule r;
            r.neutral = compile(str(row, "neutral"));
            r.dialect = compile(str(row, "dialect"));
            r.print = row.value("print", true);
            r.read = row.value("read", true);
            r.decimal = row.value("decimal", false);
            r.pad = row.value("pad", 0);
            r.exact = row.value("exact", false);
            r.decimal_limit = row.value("decimal_limit", 0xFFFFul);
            d->insn_rules.push_back(std::move(r));
        }
    }
    d->mnemonic_upper = str(j, "mnemonic_case") == "upper";
    d->mnemonic_pad = j.value("mnemonic_pad", 0);
    if (const auto it = j.find("after_parse"); it != j.end())
        for (const auto& row : *it) d->after_parse.emplace_back(row.at(0).get<std::string>(), row.at(1).get<std::string>());
    if (const auto it = j.find("forced_absolute"); it != j.end()) {
        d->forced_suffix = str(*it, "suffix");
        d->forced_prefix = str(*it, "prefix");
        for (auto o = it->at("opcodes").begin(); o != it->at("opcodes").end(); ++o) d->forced_opcodes[o.key()] = o.value().get<int>();
    }
    if (const auto it = j.find("forced_long"); it != j.end()) {
        d->long_prefix = str(*it, "prefix");
        for (auto o = it->at("opcodes").begin(); o != it->at("opcodes").end(); ++o) d->long_opcodes[o.key()] = {o.value().at(0).get<int>(), o.value().at(1).get<int>()};
    }
    if (const auto it = j.find("wraps"); it != j.end())   // [{"from": "%up(", "to_open": "(", "to_close": " >> 16)"}]
        for (const auto& row : *it) d->wraps.push_back({str(row, "from"), str(row, "to_open"), str(row, "to_close")});
    if (const auto it = j.find("registers"); it != j.end()) {
        d->reg_prefix = str(*it, "prefix");
        for (const auto& n : it->at("names")) d->regs.insert(n.get<std::string>());
    }
    if (const auto it = j.find("decimal"); it != j.end()) {
        d->decimal = true;
        d->decimal_keep_zero = it->value("keep_zero", false);
        for (const auto& n : it->at("unless")) d->decimal_unless.insert(n.get<std::string>());
    }
    d->tight_commas = j.value("tight_commas", false);
    if (const auto it = j.find("replace"); it != j.end())   // [from, to] or [from, to, true] (the parser undoes it too)
        for (const auto& row : *it) d->replace.push_back({row.at(0).get<std::string>(), row.at(1).get<std::string>(), row.size() > 2 && row.at(2).get<bool>()});
    d->list_separator = str(j, "list_separator", ",");
    d->label_at_column_zero = j.value("label_at_column_zero", false);
    if (const auto it = j.find("blocks"); it != j.end()) {
        for (auto b = it->begin(); b != it->end(); ++b) d->blocks[b.key()] = b.value().get<std::vector<std::string>>();
    }
    if (const auto it = j.find("layout"); it != j.end()) {
        d->layout.present = true;
        d->layout.begin = str(*it, "begin");
        d->layout.end = str(*it, "end");
        d->layout.slot = compile(str(*it, "slot"));
        d->layout.bank = compile(str(*it, "bank"));
        if (const auto s = it->find("skip"); s != it->end())
            for (const auto& pair : *s) d->layout.skip.emplace_back(pair.at(0).get<std::string>(), pair.at(1).get<std::string>());
    }
    if (const auto it = j.find("segments"); it != j.end()) {
        d->segments.present = true;
        d->segments.directive = compile(str(*it, "directive"));
        d->segments.memory = compile(str(*it, "memory"));
        d->segments.segment = compile(str(*it, "segment"));
    }
    d->identity = d->number.prefix == "0x" && d->number.suffix.empty() && !d->number.upper && !d->number.even && d->rules.empty() && d->wraps.empty() && d->insn_rules.empty() && d->regs.empty() && !d->decimal && d->mnemonic_pad == 0 && !d->mnemonic_upper && d->replace.empty() && d->comment == ";" &&
                  !d->layout.present && !d->segments.present && !d->label_at_column_zero && d->list_separator == ",";
    return d;
}

}  // namespace

const Dialect& find(std::string_view name) {
    static std::mutex lock;
    static std::map<std::string, std::unique_ptr<Dialect>, std::less<>> cache;
    std::lock_guard hold(lock);
    const auto it = cache.find(name);
    if (it != cache.end()) return *it->second;
    auto made = std::make_unique<Dialect>(read(name));
    const Dialect& ref = *made;
    cache.emplace(std::string(name), std::move(made));
    return ref;
}

}  // namespace cb::source::dialect

namespace cb::source::dialect {

std::string Dialect::print(std::string_view neutral_source) const {
    if (impl_->identity) return std::string(neutral_source);
    std::string out;
    out.reserve(neutral_source.size() + neutral_source.size() / 8);
    std::size_t at = 0;
    while (at <= neutral_source.size()) {
        const std::size_t nl = neutral_source.find('\n', at);
        const std::string_view l = neutral_source.substr(at, nl == std::string_view::npos ? std::string_view::npos : nl - at);
        out += line(l);
        if (nl == std::string_view::npos) break;
        out += '\n';
        at = nl + 1;
    }
    return out;
}

}  // namespace cb::source::dialect

namespace cb::source::dialect {

std::string Dialect::fallback(std::string_view code) const {
    const Impl& d = *impl_;
    if (d.identity || !code.starts_with(".byte")) return std::string(code);
    const std::size_t at = code.rfind("  ; ");
    if (at == std::string_view::npos) return std::string(code);
    return std::string(code.substr(0, at)) + "  ; " + d.instruction(code.substr(at + 4));
}

std::string Dialect::forced_absolute(std::string_view mnemonic, std::string_view operand) const {
    const Impl& d = *impl_;
    if (d.forced_suffix.empty() && d.forced_prefix.empty()) return {};
    const std::string upper = text::upper(mnemonic);
    if (!d.forced_opcodes.contains(upper)) return {};
    return upper + d.forced_suffix + " " + d.forced_prefix + std::string(operand);
}

}  // namespace cb::source::dialect

namespace cb::source::dialect {

std::string Dialect::forced_long(std::string_view mnemonic, std::string_view operand, bool indexed) const {
    const Impl& d = *impl_;
    if (d.long_prefix.empty()) return {};
    const std::string upper = text::upper(mnemonic);
    const auto it = d.long_opcodes.find(upper);
    if (it == d.long_opcodes.end()) return {};
    return upper + " " + d.long_prefix + std::string(operand) + (indexed ? ",x" : "");
}

}  // namespace cb::source::dialect
