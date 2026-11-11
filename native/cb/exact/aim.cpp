#include "cb/exact/aim.hpp"

#include <algorithm>
#include <cstddef>
#include <format>
#include <map>
#include <tuple>
#include <unordered_map>

#include "cb/core/error.hpp"
#include "cb/core/text.hpp"

namespace cb::exact::aim {

namespace {

bool is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
bool is_digit(char c) { return c >= '0' && c <= '9'; }
bool is_hex(char c) { return is_digit(c) || (c >= 'a' && c <= 'f'); }   // (lowercase: the listings and Capstone)

// The jumps and operations whose signed form differs from the unsigned one the other compiler used.
const std::map<std::string, std::string>& signed_pairs() {
    static const std::map<std::string, std::string> pairs = {
        {"jl", "jb"},   {"jle", "jbe"}, {"jg", "ja"},   {"jge", "jae"}, {"sar", "shr"},
        {"imul", "mul"}, {"idiv", "div"}, {"cwd", "xor"}, {"cbw", "sub"}};
    return pairs;
}

// ---- the listing -------------------------------------------------------------------------------------------------

// `   _name   PROC` -> the name; `   _name   ENDP` -> the name (the keyword must end at a word boundary).
std::optional<std::string_view> procedure_line(std::string_view line, std::string_view keyword) {
    std::size_t i = 0;
    while (i < line.size() && text::is_space(line[i])) ++i;
    const std::size_t start = i;
    while (i < line.size() && is_word(line[i])) ++i;
    if (i == start) return std::nullopt;
    const std::string_view word = line.substr(start, i - start);
    const std::size_t gap = i;
    while (i < line.size() && text::is_space(line[i])) ++i;
    if (i == gap || !line.substr(i).starts_with(keyword)) return std::nullopt;
    i += keyword.size();
    if (i < line.size() && is_word(line[i])) return std::nullopt;
    return word;
}

std::string_view without_underscore(std::string_view s) {
    while (!s.empty() && s.front() == '_') s.remove_prefix(1);
    return s;
}

std::optional<std::uint32_t> source_line(std::string_view line) {
    constexpr std::string_view kPrefix = "; Line ";
    if (!line.starts_with(kPrefix)) return std::nullopt;
    std::size_t i = kPrefix.size();
    const std::size_t start = i;
    while (i < line.size() && is_digit(line[i])) ++i;
    if (i == start) return std::nullopt;
    return text::parse_int<std::uint32_t>(line.substr(start, i - start));
}

// `*** 00001a  55 8b ec ` -> (offset, bytes): "***", spaces, six hex digits, spaces, then one or more "hh " pairs.
std::optional<std::pair<std::uint32_t, Bytes>> code_line(std::string_view line) {
    std::size_t i = 0;
    while (i < line.size() && text::is_space(line[i])) ++i;
    if (!line.substr(i).starts_with("***")) return std::nullopt;
    i += 3;
    std::size_t gap = i;
    while (i < line.size() && text::is_space(line[i])) ++i;
    if (i == gap) return std::nullopt;
    if (i + 6 > line.size()) return std::nullopt;
    for (std::size_t k = 0; k < 6; ++k)
        if (!is_hex(line[i + k])) return std::nullopt;
    const auto offset = text::parse_int<std::uint32_t>(line.substr(i, 6), 16);
    i += 6;
    gap = i;
    while (i < line.size() && text::is_space(line[i])) ++i;
    if (i == gap) return std::nullopt;
    Bytes bytes;
    while (i + 2 < line.size() && is_hex(line[i]) && is_hex(line[i + 1]) && line[i + 2] == ' ') {
        bytes.push_back(static_cast<std::uint8_t>(*text::parse_int<unsigned>(line.substr(i, 2), 16)));
        i += 3;
    }
    if (bytes.empty()) return std::nullopt;
    return std::make_pair(*offset, std::move(bytes));
}

// ---- normalising one instruction ---------------------------------------------------------------------------------

// The number a token is: "0x1f" or decimal digits; larger than anything that matters saturates.
std::uint64_t number_value(std::string_view token) {
    const bool hex = token.starts_with("0x");
    if (hex) token.remove_prefix(2);
    std::uint64_t v = 0;
    for (const char c : token) {
        const std::uint64_t d = is_digit(c) ? static_cast<std::uint64_t>(c - '0') : static_cast<std::uint64_t>(c - 'a' + 10);
        if (v > (UINT64_MAX - d) / (hex ? 16 : 10)) return UINT64_MAX;
        v = v * (hex ? 16 : 10) + d;
    }
    return v;
}

// The text of a memory operand's inside, e.g. "bp - 6" or "bx + si + 0x20", as its compared form: a frame's place
// is kept; a displacement of the data (large, or the 0 of an unlinked object) is masked.
std::string memory_operand(std::string_view inner) {
    std::vector<std::string> regs;
    std::vector<std::uint64_t> nums;
    for (std::size_t i = 0; i < inner.size();) {
        while (i < inner.size() && (text::is_space(inner[i]) || inner[i] == '+' || inner[i] == '-')) ++i;
        const std::size_t start = i;
        while (i < inner.size() && !text::is_space(inner[i]) && inner[i] != '+' && inner[i] != '-') ++i;
        const std::string_view part = inner.substr(start, i - start);
        if (part.empty()) continue;
        if (part.size() == 2 && part[0] >= 'a' && part[0] <= 'z' && part[1] >= 'a' && part[1] <= 'z') {
            regs.emplace_back(part);
        } else if (part.starts_with("0x") && part.size() > 2 &&
                   std::all_of(part.begin() + 2, part.end(), is_hex)) {
            nums.push_back(number_value(part));
        } else if (std::all_of(part.begin(), part.end(), is_digit)) {
            nums.push_back(number_value(part));
        }
    }
    const std::string reg_text = text::join(regs, "+");
    const bool has_bp = std::find(regs.begin(), regs.end(), "bp") != regs.end();
    if (has_bp && !nums.empty())
        return std::format("[{}{}{}]", reg_text, inner.find(" - ") != std::string_view::npos ? "-" : "+", nums.front());
    std::string out = "[" + (reg_text.empty() ? std::string("M") : reg_text);
    if (!regs.empty()) {
        const auto small = std::find_if(nums.begin(), nums.end(), [](std::uint64_t n) { return n > 0 && n < 0x80; });
        if (small != nums.end()) out += std::format("+{}", *small);
    }
    return out + "]";
}

// A number outside brackets: kept when small (1..0x7f), else "N". Only a number that stands alone is one (not a
// part of a name, and not after '[', '+' or '-': those were the memory operands' business).
std::string mask_numbers(std::string_view ops) {
    std::string out;
    for (std::size_t i = 0; i < ops.size();) {
        const char prev = i == 0 ? ' ' : ops[i - 1];
        if (is_digit(ops[i]) && !is_word(prev) && prev != '[' && prev != '+' && prev != '-') {
            std::size_t end = 0;
            if (ops.substr(i).starts_with("0x") && i + 2 < ops.size() && is_hex(ops[i + 2])) {
                std::size_t k = i + 2;
                while (k < ops.size() && is_hex(ops[k])) ++k;
                if (k == ops.size() || !is_word(ops[k])) end = k;
            }
            if (end == 0) {
                std::size_t k = i;
                while (k < ops.size() && is_digit(ops[k])) ++k;
                if (k == ops.size() || !is_word(ops[k])) end = k;
            }
            if (end != 0) {
                const std::string_view token = ops.substr(i, end - i);
                const std::uint64_t v = number_value(token);
                if (v > 0 && v < 0x80) out.append(token);
                else out += 'N';
                i = end;
                continue;
            }
        }
        out += ops[i++];
    }
    return out;
}

// ---- telling the kind of a difference ------------------------------------------------------------------------------

// "[bp-6]" and "[bp+4]" -> "[bp]", so two instructions that differ only in a frame place compare equal.
std::string without_frame_places(std::string_view s) {
    std::string out;
    for (std::size_t i = 0; i < s.size();) {
        if (s.substr(i).starts_with("[bp") && i + 4 < s.size() && (s[i + 3] == '-' || s[i + 3] == '+')) {
            std::size_t k = i + 4;
            while (k < s.size() && is_digit(s[k])) ++k;
            if (k > i + 4 && k < s.size() && s[k] == ']') {
                out += "[bp]";
                i = k + 1;
                continue;
            }
        }
        out += s[i++];
    }
    return out;
}

std::string_view mnemonic_of(std::string_view normalized) { return normalized.substr(0, normalized.find(' ')); }

bool signed_pair(std::string_view x, std::string_view y) {
    const auto& pairs = signed_pairs();
    const auto xy = pairs.find(std::string(x));
    const auto yx = pairs.find(std::string(y));
    return (xy != pairs.end() && xy->second == y) || (yx != pairs.end() && yx->second == x);
}

bool has_byte_register(std::string_view s) {
    static constexpr std::string_view kNames[] = {"al", "ah", "bl", "cl", "dl"};
    for (const auto name : kNames) {
        for (std::size_t at = s.find(name); at != std::string_view::npos; at = s.find(name, at + 1)) {
            const bool before = at == 0 || !is_word(s[at - 1]);
            const bool after = at + 2 >= s.size() || !is_word(s[at + 2]);
            if (before && after) return true;
        }
    }
    return false;
}

bool is_width_marker(const std::string& x) { return (x.find("byte ptr") != std::string::npos) || x == "cbw" || x == "cwde"; }

}  // namespace

std::string_view kind_name(Kind k) {
    switch (k) {
        case Kind::Frame: return "frame";
        case Kind::Width: return "width";
        case Kind::Sign: return "sign";
        case Kind::Order: return "order";
        case Kind::Shape: break;
    }
    return "shape";
}

std::vector<ListingEntry> listing_code(std::string_view listing, std::optional<std::string_view> name) {
    std::vector<ListingEntry> out;
    std::optional<std::uint32_t> line;
    bool inside = !name.has_value();
    for (const std::string_view l : text::split(listing, '\n')) {
        if (const auto proc = procedure_line(l, "PROC")) {
            inside = !name || without_underscore(*proc) == without_underscore(*name);
            continue;
        }
        if (procedure_line(l, "ENDP")) {
            if (inside && !out.empty()) break;
            continue;
        }
        if (const auto n = source_line(l)) {
            line = n;
            continue;
        }
        if (inside) {
            if (auto code = code_line(l)) out.push_back({code->first, std::move(code->second), line});
        }
    }
    return out;
}

Bytes code_of(const std::vector<ListingEntry>& listing) {
    Bytes out;
    if (listing.empty()) return out;
    const std::uint32_t base = listing.front().offset;
    for (const ListingEntry& e : listing) {
        if (e.offset < base) throw FormatError("listing", e.offset, "an instruction before the function's first");
        // An instruction lies where it says, or at the end when the listing skipped bytes (the gap is closed).
        const std::size_t at = std::min<std::size_t>(e.offset - base, out.size());
        const std::size_t overwritten = std::min(e.bytes.size(), out.size() - at);
        out.erase(out.begin() + static_cast<std::ptrdiff_t>(at), out.begin() + static_cast<std::ptrdiff_t>(at + overwritten));
        out.insert(out.begin() + static_cast<std::ptrdiff_t>(at), e.bytes.begin(), e.bytes.end());
    }
    return out;
}

std::string normalize(const x86::Insn& insn) {
    const std::string& mn = insn.mnemonic;
    if (mn.starts_with("j") || mn == "call" || mn == "loop") return mn + " T";
    // memory operands first, then the plain numbers
    std::string with_memory;
    const std::string& ops = insn.op_str;
    for (std::size_t i = 0; i < ops.size();) {
        if (ops[i] == '[') {
            const std::size_t close = ops.find(']', i);
            if (close != std::string::npos) {
                with_memory += memory_operand(std::string_view(ops).substr(i + 1, close - i - 1));
                i = close + 1;
                continue;
            }
        }
        with_memory += ops[i++];
    }
    return std::string(text::trim(mn + " " + mask_numbers(with_memory)));
}

Kind classify(const std::vector<std::string>& a, const std::vector<std::string>& b) {
    if (a.size() == b.size()) {
        const auto same_frame = [&] {
            for (std::size_t i = 0; i < a.size(); ++i)
                if (without_frame_places(a[i]) != without_frame_places(b[i])) return false;
            return true;
        };
        if (same_frame()) return Kind::Frame;
        bool all_related = true, any_signed = false;
        for (std::size_t i = 0; i < a.size(); ++i) {
            const auto x = mnemonic_of(a[i]), y = mnemonic_of(b[i]);
            const bool sp = signed_pair(x, y);
            any_signed = any_signed || sp;
            if (x != y && !sp) all_related = false;
        }
        if (all_related && any_signed) return Kind::Sign;
        for (std::size_t i = 0; i < a.size(); ++i)
            if ((a[i].find("byte ptr") != std::string::npos) != (b[i].find("byte ptr") != std::string::npos) ||
                (has_byte_register(a[i]) && !has_byte_register(b[i])))
                return Kind::Width;
    }
    {
        auto sa = a, sb = b;
        std::sort(sa.begin(), sa.end());
        std::sort(sb.begin(), sb.end());
        if (sa == sb) return Kind::Order;
    }
    const auto mark = [](const std::string& x) { return is_width_marker(x) || x.starts_with("sub ah"); };
    const bool any_mark = std::any_of(a.begin(), a.end(), mark) || std::any_of(b.begin(), b.end(), mark);
    const auto markers = [](const std::vector<std::string>& v) {
        std::vector<std::string> m;
        for (const auto& x : v)
            if (is_width_marker(x)) m.push_back(x);
        return m;
    };
    const std::size_t diff = a.size() > b.size() ? a.size() - b.size() : b.size() - a.size();
    if (any_mark && diff <= 2 && markers(a) != markers(b)) return Kind::Width;
    return Kind::Shape;
}

Diagnosis diagnose(ByteView orig, ByteView code, const std::vector<ListingEntry>& listing, const x86::Decoder16& decoder) {
    const auto normalized = [&](ByteView bytes, std::vector<x86::Insn>& insns) {
        insns = x86::disasm(bytes, 0, static_cast<std::uint32_t>(bytes.size()), true, decoder);
        std::vector<std::string> out;
        out.reserve(insns.size());
        for (const auto& i : insns) out.push_back(normalize(i));
        return out;
    };
    std::vector<x86::Insn> orig_insns, code_insns;
    const std::vector<std::string> on = normalized(orig, orig_insns), cn = normalized(code, code_insns);

    std::unordered_map<std::uint32_t, std::optional<std::uint32_t>> line_at;   // (an entry with no line says "none")
    for (const auto& e : listing) line_at[e.offset] = e.line;
    std::vector<std::optional<std::uint32_t>> lines_of;   // each candidate instruction's line: the last one given
    std::optional<std::uint32_t> last;
    for (const auto& i : code_insns) {
        if (const auto it = line_at.find(i.address); it != line_at.end()) last = it->second;   // (the last given)
        lines_of.push_back(last);
    }

    Diagnosis d;
    const auto count = [&](Kind k, std::size_t n) {
        for (auto& kv : d.kinds)
            if (kv.first == k) {
                kv.second += n;
                return;
            }
        d.kinds.emplace_back(k, n);
    };
    for (const Opcode& op : opcodes(on, cn)) {
        if (op.tag == Opcode::Tag::Equal) continue;
        const Kind k = classify({on.begin() + static_cast<std::ptrdiff_t>(op.a1), on.begin() + static_cast<std::ptrdiff_t>(op.a2)},
                                {cn.begin() + static_cast<std::ptrdiff_t>(op.b1), cn.begin() + static_cast<std::ptrdiff_t>(op.b2)});
        count(k, std::max(op.a2 - op.a1, op.b2 - op.b1));
        for (std::size_t j = op.b1; j < std::max(op.b2, op.b1 + 1); ++j) {
            if (lines_of.empty()) break;
            const auto ln = lines_of[std::min(j, lines_of.size() - 1)];
            if (ln && std::find(d.lines.begin(), d.lines.end(), *ln) == d.lines.end()) d.lines.push_back(*ln);
        }
    }
    // (the same instructions elsewhere: a block the layout moved - order, not shape)
    const auto shape = std::find_if(d.kinds.begin(), d.kinds.end(), [](const auto& kv) { return kv.first == Kind::Shape; });
    if (shape != d.kinds.end() && shape->second != 0) {
        auto sa = on, sb = cn;
        std::sort(sa.begin(), sa.end());
        std::sort(sb.begin(), sb.end());
        if (sa == sb) {
            const std::size_t moved = shape->second;
            d.kinds.erase(shape);
            count(Kind::Order, moved);
        }
    }
    return d;
}

// ---- difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes() -------------------------------------------------

namespace {

struct Block {
    std::size_t i, j, k;   // a[i, i+k) == b[j, j+k)
};

class Matcher {
public:
    Matcher(const std::vector<std::string>& a, const std::vector<std::string>& b) : a_(a), b_(b) {
        for (std::size_t j = 0; j < b_.size(); ++j) b2j_[b_[j]].push_back(j);
    }

    // The longest matching block in a[alo, ahi) x b[blo, bhi): the earliest in a, then in b, among the longest.
    Block longest(std::size_t alo, std::size_t ahi, std::size_t blo, std::size_t bhi) const {
        std::size_t besti = alo, bestj = blo, bestsize = 0;
        std::unordered_map<std::size_t, std::size_t> j2len;
        for (std::size_t i = alo; i < ahi; ++i) {
            std::unordered_map<std::size_t, std::size_t> next;
            const auto it = b2j_.find(a_[i]);
            if (it != b2j_.end()) {
                for (const std::size_t j : it->second) {
                    if (j < blo) continue;
                    if (j >= bhi) break;
                    const auto prev = j > 0 ? j2len.find(j - 1) : j2len.end();
                    const std::size_t k = (prev == j2len.end() ? 0 : prev->second) + 1;
                    next[j] = k;
                    if (k > bestsize) {
                        besti = i + 1 - k;
                        bestj = j + 1 - k;
                        bestsize = k;
                    }
                }
            }
            j2len = std::move(next);
        }
        return {besti, bestj, bestsize};
    }

    std::vector<Block> matching_blocks() const {
        std::vector<std::tuple<std::size_t, std::size_t, std::size_t, std::size_t>> queue = {{0, a_.size(), 0, b_.size()}};
        std::vector<Block> found;
        while (!queue.empty()) {
            const auto [alo, ahi, blo, bhi] = queue.back();
            queue.pop_back();
            const Block x = longest(alo, ahi, blo, bhi);
            if (x.k == 0) continue;
            found.push_back(x);
            if (alo < x.i && blo < x.j) queue.emplace_back(alo, x.i, blo, x.j);
            if (x.i + x.k < ahi && x.j + x.k < bhi) queue.emplace_back(x.i + x.k, ahi, x.j + x.k, bhi);
        }
        std::sort(found.begin(), found.end(), [](const Block& p, const Block& q) { return std::tie(p.i, p.j, p.k) < std::tie(q.i, q.j, q.k); });
        // adjacent blocks are one block
        std::vector<Block> merged;
        for (const Block& b : found) {
            if (!merged.empty() && merged.back().i + merged.back().k == b.i && merged.back().j + merged.back().k == b.j)
                merged.back().k += b.k;
            else
                merged.push_back(b);
        }
        merged.push_back({a_.size(), b_.size(), 0});   // the sentinel that closes the script
        return merged;
    }

private:
    const std::vector<std::string>& a_;
    const std::vector<std::string>& b_;
    std::unordered_map<std::string, std::vector<std::size_t>> b2j_;   // value -> its places in b, ascending: lookup only
};

}  // namespace

std::vector<Opcode> opcodes(const std::vector<std::string>& a, const std::vector<std::string>& b) {
    std::vector<Opcode> script;
    std::size_t i = 0, j = 0;
    for (const Block& m : Matcher(a, b).matching_blocks()) {
        std::optional<Opcode::Tag> tag;
        if (i < m.i && j < m.j) tag = Opcode::Tag::Replace;
        else if (i < m.i) tag = Opcode::Tag::Delete;
        else if (j < m.j) tag = Opcode::Tag::Insert;
        if (tag) script.push_back({*tag, i, m.i, j, m.j});
        i = m.i + m.k;
        j = m.j + m.k;
        if (m.k) script.push_back({Opcode::Tag::Equal, m.i, i, m.j, j});
    }
    return script;
}

}  // namespace cb::exact::aim
