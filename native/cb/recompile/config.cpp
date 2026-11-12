// cb/recompile/config.cpp -- see config.hpp.

#include "cb/recompile/config.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::recompile::config {

namespace {
namespace cfg {

constexpr std::uint32_t kBlr = 0x4E800020;
constexpr std::uint32_t kBctr = 0x4E800420;
constexpr std::uint32_t kLiR11Minus0x120 = 0x3960FEE0;   // li r11,-0x120: the start of __savevmx_14 and __restvmx_14
constexpr std::uint32_t kLiR11Minus0x400 = 0x3960FC00;   // li r11,-0x400: _64

std::uint32_t word_at(ByteView code, std::size_t offset) {
    return (static_cast<std::uint32_t>(code[offset]) << 24) | (static_cast<std::uint32_t>(code[offset + 1]) << 16) |
           (static_cast<std::uint32_t>(code[offset + 2]) << 8) | code[offset + 3];
}

bool zero_word_at(ByteView code, std::int64_t offset) {
    return offset >= 0 && static_cast<std::size_t>(offset) + 4 <= code.size() && word_at(code, static_cast<std::size_t>(offset)) == 0;
}

// An unconditional or conditional relative branch that does not link: `b` and `bc`. -> its displacement.
std::optional<std::int64_t> plain_branch_displacement(std::uint32_t w) {
    if ((w & 3) != 0) return std::nullopt;   // AA or LK set
    const std::uint32_t op = w >> 26;
    if (op == 18) {
        const std::int64_t d = w & 0x03FFFFFC;
        return (d & 0x02000000) ? d - 0x04000000 : d;
    }
    if (op == 16) {
        const std::int64_t d = w & 0xFFFC;
        return (d & 0x8000) ? d - 0x10000 : d;
    }
    return std::nullopt;
}

struct Range {
    std::uint32_t begin = 0;
    std::uint32_t end = 0;
};

bool in_any(const std::vector<Range>& ranges, std::uint64_t a) {
    return std::ranges::any_of(ranges, [a](const Range& r) { return r.begin <= a && a < r.end; });
}

// ---- a TOML reader for the switch files ------------------------------------------------------------------------------------

// Reads the text once, left to right. A value is an integer, a string, a boolean, a float or an array of values; only the
// integers and the arrays of them are kept.
class Toml {
public:
    Toml(std::string_view text, std::string_view name) : text_(text), name_(name) {}

    struct Value {
        std::optional<std::int64_t> integer;
        std::vector<std::int64_t> integers;   // the integers of an array
        bool is_array = false;
        bool other = false;                   // anything else
    };
    struct Table {                            // one [[switch]]
        std::size_t line = 0;
        std::optional<Value> base, r, default_target, labels;
    };

    std::vector<Table> switches() {
        std::vector<Table> out;
        Table* current = nullptr;
        for (;;) {
            skip_blank();
            if (at_end()) break;
            if (peek() == '[') {
                const bool array = text_.substr(pos_).starts_with("[[");
                pos_ += array ? 2 : 1;
                const std::string name = key_path();
                expect(array ? "]]" : "]", "a table header is closed with ]");
                end_of_line();
                if (array && name == "switch") {
                    out.push_back({line_});
                    current = &out.back();
                } else {
                    current = nullptr;
                }
                continue;
            }
            const std::string key = key_path();
            skip_spaces();
            expect("=", "a key is followed by =");
            skip_spaces();
            Value value = parse_value();
            end_of_line();
            if (!current) continue;
            if (key == "base") current->base = std::move(value);
            else if (key == "r") current->r = std::move(value);
            else if (key == "default") current->default_target = std::move(value);
            else if (key == "labels") current->labels = std::move(value);
        }
        return out;
    }

    [[noreturn]] void fail(const std::string& what) const { throw ParseError(name_, static_cast<std::uint32_t>(line_), static_cast<std::uint32_t>(column()), what); }

private:
    bool at_end() const { return pos_ >= text_.size(); }
    char peek() const { return text_[pos_]; }
    std::size_t column() const {
        const std::size_t start = text_.rfind('\n', pos_ == 0 ? 0 : pos_ - 1);
        return pos_ - (start == std::string_view::npos || pos_ == 0 ? 0 : start + 1) + 1;
    }

    void skip_spaces() {
        while (!at_end() && (peek() == ' ' || peek() == '\t')) ++pos_;
    }
    void skip_comment() {
        if (!at_end() && peek() == '#') {
            while (!at_end() && peek() != '\n') ++pos_;
        }
    }
    // blanks, newlines and comments between statements (and inside arrays)
    void skip_blank() {
        for (;;) {
            skip_spaces();
            skip_comment();
            if (at_end()) return;
            if (peek() == '\n') {
                ++line_;
                ++pos_;
            } else if (peek() == '\r') {
                ++pos_;
            } else {
                return;
            }
        }
    }
    void end_of_line() {
        skip_spaces();
        skip_comment();
        if (at_end()) return;
        if (peek() == '\r') ++pos_;
        if (at_end()) return;
        if (peek() != '\n') fail("expected the end of the line");
    }
    void expect(std::string_view what, const char* why) {
        if (!text_.substr(pos_).starts_with(what)) fail(why);
        pos_ += what.size();
    }

    static bool bare_key_char(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'; }

    // a key or a dotted key: the text of it, the dots kept
    std::string key_path() {
        std::string out;
        for (;;) {
            skip_spaces();
            if (!at_end() && (peek() == '"' || peek() == '\'')) {
                out += parse_string();
            } else {
                const std::size_t begin = pos_;
                while (!at_end() && bare_key_char(peek())) ++pos_;
                if (pos_ == begin) fail("a key was expected");
                out += text_.substr(begin, pos_ - begin);
            }
            skip_spaces();
            if (!at_end() && peek() == '.') {
                out += '.';
                ++pos_;
                continue;
            }
            return out;
        }
    }

    std::string parse_string() {
        const char quote = peek();
        const bool basic = quote == '"';
        ++pos_;
        std::string out;
        while (!at_end() && peek() != quote) {
            if (peek() == '\n') fail("a string is not closed on its line");
            if (basic && peek() == '\\' && pos_ + 1 < text_.size()) ++pos_;   // (an escape: the next character is part of it)
            out += peek();
            ++pos_;
        }
        if (at_end()) fail("a string is not closed");
        ++pos_;
        return out;
    }

    Value parse_value() {
        if (at_end()) fail("a value was expected");
        Value v;
        const char c = peek();
        if (c == '"' || c == '\'') {
            parse_string();
            v.other = true;
        } else if (c == '[') {
            ++pos_;
            v.is_array = true;
            for (;;) {
                skip_blank();
                if (at_end()) fail("an array is not closed");
                if (peek() == ']') {
                    ++pos_;
                    break;
                }
                Value item = parse_value();
                if (item.integer) v.integers.push_back(*item.integer);
                else v.other = true;
                skip_blank();
                if (!at_end() && peek() == ',') ++pos_;
                else if (!at_end() && peek() != ']') fail("an array item was expected, or the end of the array");
            }
        } else if (text_.substr(pos_).starts_with("true")) {
            pos_ += 4;
            v.other = true;
        } else if (text_.substr(pos_).starts_with("false")) {
            pos_ += 5;
            v.other = true;
        } else {
            const std::size_t begin = pos_;
            while (!at_end() && (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_' || peek() == '+' || peek() == '-' || peek() == '.')) ++pos_;
            const std::string_view token = text_.substr(begin, pos_ - begin);
            if (token.empty()) fail("a value was expected");
            if (const std::optional<std::int64_t> n = integer_of(token)) v.integer = n;
            else if (is_float(token)) v.other = true;
            else fail(std::format("`{}` is not a value", token));
        }
        return v;
    }

    // 42, -7, 0x2A, 0o52, 0b101010, 1_000
    static std::optional<std::int64_t> integer_of(std::string_view token) {
        bool negative = false;
        std::string_view t = token;
        if (t.starts_with('+') || t.starts_with('-')) {
            negative = t.front() == '-';
            t.remove_prefix(1);
        }
        int base = 10;
        if (t.starts_with("0x")) base = 16;
        else if (t.starts_with("0o")) base = 8;
        else if (t.starts_with("0b")) base = 2;
        if (base != 10) t.remove_prefix(2);
        std::string digits;
        for (std::size_t i = 0; i < t.size(); ++i) {
            if (t[i] == '_') {
                if (i == 0 || i + 1 == t.size() || t[i - 1] == '_') return std::nullopt;
                continue;
            }
            digits += t[i];
        }
        if (digits.empty()) return std::nullopt;
        const std::optional<std::int64_t> n = text::parse_int<std::int64_t>(digits, base);
        if (!n) return std::nullopt;
        return negative ? -*n : *n;
    }

    static bool is_float(std::string_view t) {
        if (t == "inf" || t == "+inf" || t == "-inf" || t == "nan" || t == "+nan" || t == "-nan") return true;
        bool digit = false;
        for (const char c : t) {
            if (c >= '0' && c <= '9') digit = true;
            else if (c != '.' && c != 'e' && c != 'E' && c != '+' && c != '-' && c != '_') return false;
        }
        return digit;
    }

    std::string_view text_;
    std::string_view name_;
    std::size_t pos_ = 0;
    std::size_t line_ = 1;
};

std::uint32_t address_of(const Toml& toml, std::int64_t value, const char* what) {
    if (value < 0 || value > 0xFFFFFFFFll) toml.fail(std::format("{} {} is not a 32-bit address", what, value));
    return static_cast<std::uint32_t>(value);
}

}  // namespace cfg
}  // namespace

// ---- helpers and the CRT's jump buffer ----------------------------------------------------------------------------------

std::uint32_t start_before(ByteView code, std::uint32_t begin, std::uint32_t address) {
    std::size_t o = address >= begin ? std::min<std::size_t>(address - begin, code.size()) : 0;
    while (o >= 4) {
        const std::uint32_t w = cfg::word_at(code, o - 4);
        if (w == 0 || w == cfg::kBlr) break;
        o -= 4;
    }
    return begin + static_cast<std::uint32_t>(o);
}

std::uint32_t end_of_run(ByteView code, std::uint32_t begin, std::uint32_t address) {
    for (std::size_t o = address >= begin ? address - begin : 0; o + 4 <= code.size(); o += 4) {
        const std::uint32_t w = cfg::word_at(code, o);
        if (w == cfg::kBlr || w == cfg::kBctr || (w >> 26 == 18 && (w & 3) == 0)) return begin + static_cast<std::uint32_t>(o) + 4;
    }
    return begin + static_cast<std::uint32_t>(code.size());
}

Addresses jump_buffers(const Title& title) {
    const ByteView code = title.image.code;
    const std::uint32_t lo = title.image.code_begin;
    Addresses out;
    const std::size_t count = code.size() / 4;
    for (std::size_t i = 0; i < count; ++i) {
        const std::uint32_t w = cfg::word_at(code, 4 * i);
        // setjmp stores the caller's r1 at 0x90 of the jmp_buf. The buffer pointer is its first argument (r3) by the ABI,
        // but some system modules (XAM, Guide.TFA) reach it through another register, so match `std r1,0x90(rA)` for any
        // rA -- keying on r3 alone misses those and would leave longjmp below without a setjmp.
        if (w >> 26 == 62 && ((w >> 21) & 31) == 1 && (w & 0xFFFF) == 0x90 && !out.contains("setjmp_address"))
            out["setjmp_address"] = start_before(code, lo, lo + static_cast<std::uint32_t>(4 * i));
        // lwz rD,0x134(rA) then lwz rE,0x90(rA) (D-form, opcode 32) within a few words
        if (w >> 26 == 32 && (w & 0xFFFF) == 0x134 && !out.contains("longjmp_address")) {
            const std::uint32_t ra = (w >> 16) & 31;
            for (std::size_t k = i + 1; k < std::min(i + 4, count); ++k) {
                const std::uint32_t v = cfg::word_at(code, 4 * k);
                if (v >> 26 != 32 || (v & 0xFFFF) != 0x90 || ((v >> 16) & 31) != ra) continue;
                const std::uint32_t a = lo + static_cast<std::uint32_t>(4 * i);
                std::optional<std::uint32_t> holder;   // the first .pdata function that holds it
                for (const auto& [start, size] : title.image.extents) {
                    if (start <= a && a < static_cast<std::uint64_t>(start) + size) {
                        holder = start;
                        break;
                    }
                }
                out["longjmp_address"] = holder ? *holder : start_before(code, lo, a);
                break;
            }
        }
    }
    // A longjmp_address with no setjmp_address is the silent crash: XenonRecomp reads setJmpAddress as 0, the guest's setjmp
    // never fills the host jump buffer, and the first longjmp jumps through zeros (rip=0) -- uncatchable. Never emit one
    // without the other.
    if (out.contains("longjmp_address") && !out.contains("setjmp_address")) out.erase("longjmp_address");
    return out;
}

Addresses helpers(const Title& title, disassemble::Disassembler& dis) {
    Addresses out;
    std::map<std::string, std::uint32_t> runs;   // name -> address (a name met twice keeps the later address)
    for (const auto& [address, name] : title.run_names) runs[name] = address;
    static constexpr std::array<std::pair<std::string_view, std::string_view>, 4> kRuns = {{
        {"savegprlr_14_address", "__savegprlr_14"},
        {"restgprlr_14_address", "__restgprlr_14"},
        {"savefpr_14_address", "__savefpr_14"},
        {"restfpr_14_address", "__restfpr_14"},
    }};
    for (const auto& [key, name] : kRuns) {
        if (const auto it = runs.find(std::string(name)); it != runs.end()) out[std::string(key)] = it->second;
    }
    // the vector runs: li r11,-0x120 (v14) / li r11,-0x400 (v64), then stvx or lvx through r12
    const ByteView code = title.image.code;
    const std::uint32_t lo = title.image.code_begin;
    for (std::size_t o = 0; o + 8 <= code.size(); o += 4) {
        const std::uint32_t w = cfg::word_at(code, o);
        if (w != cfg::kLiR11Minus0x120 && w != cfg::kLiR11Minus0x400) continue;
        const std::vector<disassemble::Row> next = dis.listing(code, lo, o + 4, 1);
        if (next.empty()) continue;
        std::string operands = next[0].operands;
        std::erase(operands, ' ');
        const bool v14 = w == cfg::kLiR11Minus0x120;
        if (operands != (v14 ? "v14,r11,r12" : "v64,r11,r12")) continue;
        const std::string n = v14 ? "14" : "64";
        if (next[0].mnemonic.starts_with("stvx")) out.try_emplace("savevmx_" + n + "_address", lo + static_cast<std::uint32_t>(o));
        else if (next[0].mnemonic.starts_with("lvx")) out.try_emplace("restvmx_" + n + "_address", lo + static_cast<std::uint32_t>(o));
    }
    for (const auto& [key, address] : jump_buffers(title)) out[key] = address;
    return out;
}

// ---- the switch-table file ------------------------------------------------------------------------------------------------

std::vector<SwitchEntry> parse_switch_entries(std::string_view toml, std::string_view name) {
    cfg::Toml reader(toml, name);
    std::vector<SwitchEntry> out;
    for (const cfg::Toml::Table& t : reader.switches()) {
        if (!t.base || !t.base->integer) throw ParseError(name, static_cast<std::uint32_t>(t.line), 1, "a [[switch]] needs a `base`");
        if (!t.labels || !t.labels->is_array) throw ParseError(name, static_cast<std::uint32_t>(t.line), 1, "a [[switch]] needs `labels`");
        SwitchEntry e;
        e.base = cfg::address_of(reader, *t.base->integer, "base");
        if (t.r && t.r->integer) e.reg = static_cast<std::uint32_t>(std::clamp<std::int64_t>(*t.r->integer, 0, 0xFFFFFFFFll));
        if (t.default_target && t.default_target->integer) e.default_target = cfg::address_of(reader, *t.default_target->integer, "default");
        for (const std::int64_t l : t.labels->integers) e.labels.push_back(cfg::address_of(reader, l, "label"));
        out.push_back(std::move(e));
    }
    return out;
}

SwitchTables switch_tables_of(std::string_view toml, std::string_view name) {
    SwitchTables out;
    for (SwitchEntry& e : parse_switch_entries(toml, name)) out.insert_or_assign(e.base, std::move(e.labels));
    return out;
}

SwitchTables read_switch_tables(const std::filesystem::path& path) {
    return switch_tables_of(fs::read_text(path), fs::to_text(path));
}

Merged merge_xenon_analyse(std::vector<switches::SwitchTable> found, const switches::CodeImage& image, std::string_view analysed,
                           std::string_view name) {
    Merged out;
    out.found = found.size();
    std::set<std::uint32_t> have;
    for (const switches::SwitchTable& t : found) have.insert(t.bctr);
    const std::uint32_t lo = image.code_begin;
    const std::uint32_t hi = image.code_end;
    const ByteView code = lo >= image.base && image.data.size() >= lo - image.base ? ByteView(image.data).subspan(lo - image.base) : ByteView();
    for (const SwitchEntry& s : parse_switch_entries(analysed, name)) {
        if (!s.reg) throw ParseError(name, 0, 0, std::format("the switch at 0x{:X} has no `r`", s.base));
        if (s.base < lo) continue;   // (not in the code)
        std::uint32_t bctr = s.base;   // XenonAnalyse's base is the sequence's start: find its bctr
        while (bctr < hi && static_cast<std::size_t>(bctr - lo) + 4 <= code.size() && cfg::word_at(code, bctr - lo) != cfg::kBctr) bctr += 4;
        if (have.contains(bctr) || bctr >= hi) continue;
        have.insert(bctr);
        switches::SwitchTable t;
        t.bctr = s.base;
        t.index_reg = *s.reg;
        t.default_target = s.default_target;
        t.kind = switches::TableKind::XenonAnalyse;
        t.labels = s.labels;
        t.guard = 0;
        found.push_back(std::move(t));
        ++out.added;
    }
    out.tables = std::move(found);
    return out;
}

WrittenTables write_switch_tables(const std::filesystem::path& path, const switches::CodeImage& image, const std::optional<std::string>& analysed) {
    switches::Found found = switches::find(image);
    Merged merged = merge_xenon_analyse(std::move(found.tables), image, analysed.value_or(std::string()));
    switches::write(path, merged.tables);
    return {merged.found, merged.added};
}

// ---- the functions .pdata leaves out ----------------------------------------------------------------------------------------

std::vector<ExtraFunction> extra_functions(const Title& title, const discover::Functions& discovered, disassemble::Disassembler& dis,
                                           const SwitchTables& tables) {
    const discover::Image& img = title.image;
    const ByteView code = img.code;
    const std::uint32_t lo = img.code_begin;
    const std::uint32_t hi = img.code_end;
    std::set<std::uint32_t> labels;
    for (const auto& [base, cases] : tables) labels.insert(cases.begin(), cases.end());
    // XenonRecomp emits each save/restore run itself, an entry per register: nothing inside one is extra
    std::vector<cfg::Range> runs;
    for (const auto& [key, address] : helpers(title, dis)) runs.push_back({address, end_of_run(code, lo, address)});
    std::vector<std::uint32_t> starts;
    for (const auto& [a, size] : discovered)
        if (!labels.contains(a) && !cfg::in_any(runs, a)) starts.push_back(a);
    std::set<std::uint32_t> bound_set(starts.begin(), starts.end());
    bound_set.insert(img.imports.begin(), img.imports.end());
    for (const auto& [a, name] : title.run_names) bound_set.insert(a);
    for (const auto& [a, size] : img.extents) bound_set.insert(a);
    for (const cfg::Range& r : runs) bound_set.insert(r.begin);
    const std::vector<std::uint32_t> bounds(bound_set.begin(), bound_set.end());
    std::vector<cfg::Range> pdata;   // [start, start + size) of each .pdata function, in address order
    std::vector<std::uint32_t> pdata_starts;
    for (const auto& [s, n] : img.extents) {
        pdata.push_back({s, s + n});
        pdata_starts.push_back(s);
    }
    // the last of `starts` (sorted) that is at or before `a`: index, or -1
    const auto last_at_or_before = [](const std::vector<std::uint32_t>& sorted, std::uint32_t a) {
        return static_cast<std::ptrdiff_t>(std::upper_bound(sorted.begin(), sorted.end(), a) - sorted.begin()) - 1;
    };
    const auto size_of = [&](std::uint32_t a) -> std::uint32_t {
        const std::ptrdiff_t i = last_at_or_before(pdata_starts, a);
        if (i >= 0 && pdata[static_cast<std::size_t>(i)].begin < a && a < pdata[static_cast<std::size_t>(i)].end)   // a second entry of a .pdata function: runs to its end
            return pdata[static_cast<std::size_t>(i)].end - a;
        const auto next = std::upper_bound(bounds.begin(), bounds.end(), a);
        std::uint64_t end = std::min<std::uint64_t>({next != bounds.end() ? *next : hi, static_cast<std::uint64_t>(a) + 0x4000, hi});
        for (const auto& [base, cases] : tables) {   // its switches' cases are inside it
            if (a <= base && base < end && !cases.empty())
                end = std::max<std::uint64_t>(end, std::min<std::uint64_t>(end_of_run(code, lo, *std::ranges::max_element(cases)), hi));
        }
        while (end - 4 > a && cfg::zero_word_at(code, static_cast<std::int64_t>(end) - 4 - lo)) end -= 4;   // trailing padding
        return static_cast<std::uint32_t>(end - a);
    };

    std::map<std::uint32_t, std::uint32_t> found;
    for (const std::uint32_t a : starts) {   // (in address order)
        if (img.extents.contains(a) || title.run_names.contains(a) || a < lo || a >= hi) continue;
        found[a] = size_of(a);
    }
    // A switch outside every known function (between .pdata functions): its function runs from the start of that gap (after
    // the previous function's end and any padding) to the end of its last case; left to XenonRecomp, the gap's analysis stops
    // at the bctr and the cases fall outside it.
    std::vector<cfg::Range> covered = pdata;
    for (const auto& [a, n] : found) covered.push_back({a, a + n});
    std::ranges::sort(covered, [](const cfg::Range& x, const cfg::Range& y) { return x.begin != y.begin ? x.begin < y.begin : x.end < y.end; });
    std::vector<std::uint32_t> covered_starts;
    for (const cfg::Range& r : covered) covered_starts.push_back(r.begin);
    for (const auto& [bctr, cases] : tables) {
        if (cases.empty()) continue;   // (a switch with no case: nothing to run to)
        const std::ptrdiff_t i = last_at_or_before(covered_starts, bctr);
        if (i >= 0 && covered[static_cast<std::size_t>(i)].begin <= bctr && bctr < covered[static_cast<std::size_t>(i)].end) continue;
        std::uint32_t start = i >= 0 ? covered[static_cast<std::size_t>(i)].end : lo;
        while (start < bctr && cfg::zero_word_at(code, static_cast<std::int64_t>(start) - lo)) start += 4;
        if (img.extents.contains(start) || start < lo || start >= hi) continue;
        std::uint32_t end = std::max(end_of_run(code, lo, *std::ranges::max_element(cases)), bctr + 4);
        const auto j = std::upper_bound(pdata_starts.begin(), pdata_starts.end(), start);
        if (j != pdata_starts.end()) end = std::min(end, *j);
        found[start] = end - start;
    }
    // Every branch (b, bc; no link) out of a function is a tail call: its target must be a function too, or XenonRecomp has
    // nothing to call there. New functions have branches of their own: to a fixed point.
    std::set<std::uint32_t> known;
    for (const auto& [a, n] : img.extents) known.insert(a);
    for (const auto& [a, n] : found) known.insert(a);
    for (const auto& [a, name] : title.run_names) known.insert(a);
    known.insert(img.imports.begin(), img.imports.end());
    for (const cfg::Range& r : runs) known.insert(r.begin);
    std::vector<cfg::Range> todo = pdata;
    for (const auto& [a, n] : found) todo.push_back({a, a + n});
    while (!todo.empty()) {
        std::vector<cfg::Range> added;
        for (const cfg::Range& fn : todo) {
            const std::int64_t stop = static_cast<std::int64_t>(std::min(fn.end, hi)) - lo;
            for (std::int64_t o = static_cast<std::int64_t>(fn.begin) - lo; o < stop; o += 4) {
                if (o < 0 || static_cast<std::size_t>(o) + 4 > code.size()) continue;
                const std::optional<std::int64_t> d = cfg::plain_branch_displacement(cfg::word_at(code, static_cast<std::size_t>(o)));
                if (!d) continue;
                const std::int64_t t = lo + o + *d;
                if ((fn.begin <= t && t < fn.end) || t < lo || t >= hi) continue;
                const std::uint32_t target = static_cast<std::uint32_t>(t);
                if (known.contains(target) || labels.contains(target) || cfg::in_any(runs, target)) continue;
                known.insert(target);
                found[target] = size_of(target);
                added.push_back({target, target + found[target]});
            }
        }
        todo = std::move(added);
    }
    std::vector<ExtraFunction> out;
    out.reserve(found.size());
    for (const auto& [a, n] : found) out.push_back({a, n});
    return out;
}

// ---- the configuration ------------------------------------------------------------------------------------------------

namespace {

std::string slashes(std::string_view s) {
    std::string out(s);
    std::ranges::replace(out, '\\', '/');
    return out;
}

}  // namespace

std::string make(const Title& title, disassemble::Disassembler& dis, const std::filesystem::path& xex_path, const MakeOptions& options) {
    std::string out = "# XenonRecomp configuration, written by CodeBreaker (recompile/config)\n[main]\n";
    out += std::format("file_path = \"{}\"\n", slashes(options.file_path ? *options.file_path : fs::to_text(xex_path)));
    out += std::format("out_directory_path = \"{}\"\n", slashes(options.out_dir));
    out += std::format("switch_table_file_path = \"{}\"\n\n", slashes(options.switch_tables));
    for (const RegisterSetting& s : kRegisterSettings) out += std::format("{} = {}\n", s.key, s.value ? "true" : "false");
    out += '\n';
    for (const auto& [key, address] : helpers(title, dis)) out += std::format("{} = 0x{:08X}\n", key, address);
    SwitchTables tables;
    if (options.switches) {
        tables = *options.switches;
    } else {
        const std::filesystem::path beside = std::filesystem::absolute(xex_path).parent_path() / fs::from_text(options.switch_tables);
        std::error_code ec;
        if (std::filesystem::is_regular_file(beside, ec)) tables = read_switch_tables(beside);
    }
    const discover::Discovery discovery = discover::discover(title.image, dis);
    const std::vector<ExtraFunction> extra = extra_functions(title, discovery.functions, dis, tables);
    out += std::format("\n# functions .pdata leaves out ({})\nfunctions = [\n", extra.size());
    for (const ExtraFunction& f : extra) out += std::format("    {{ address = 0x{:08X}, size = 0x{:X} }},\n", f.address, f.size);
    out += "]\n";
    if (options.hooks) {
        out += '\n';
        out += text::trim(*options.hooks);
        out += '\n';
    }
    return out;
}

}  // namespace cb::recompile::config
