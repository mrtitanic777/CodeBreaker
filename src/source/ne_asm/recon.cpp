// source/ne_asm/recon.cpp -- hand-written assembly reconstructed byte for byte (see recon.hpp).
// cb-unity: alone
#include "source/ne_asm/recon.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <format>
#include <iterator>
#include <map>
#include <tuple>

#include "core/cli.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "formats/omf.hpp"
#include "source/ne_asm/reachability.hpp"

namespace cb::source::ne_asm::recon {

namespace stdfs = std::filesystem;
using segment::Row;
using segment::Seg;

namespace {

bool recon_is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
bool recon_is_lower_hex(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }
bool recon_is_hex(char c) { return recon_is_lower_hex(c) || (c >= 'A' && c <= 'F'); }
bool recon_is_space(char c) { return c == ' ' || c == '\t'; }

std::string recon_squeezed(std::string_view s) {
    std::string out;
    for (const std::string_view word : text::split_ws(s)) {
        if (!out.empty()) out += ' ';
        out += word;
    }
    return out;
}

// The prefixes of a string or locked instruction.
bool recon_is_prefix(std::string_view word) {
    static constexpr std::array<std::string_view, 6> prefixes = {"rep", "repz", "repe", "repnz", "repne", "lock"};
    return std::find(prefixes.begin(), prefixes.end(), word) != prefixes.end();
}

bool recon_is_string_op(std::string_view op) {
    static constexpr std::array<std::string_view, 5> ops = {"lods", "stos", "movs", "scas", "cmps"};
    return std::find(ops.begin(), ops.end(), op) != ops.end();
}

std::string recon_lower_case_words(std::string s) {
    for (const auto& [from, to] : {std::pair<std::string_view, std::string_view>{"DWORD PTR", "dword ptr"}, {"WORD PTR", "word ptr"},
                                   {"BYTE PTR", "byte ptr"}, {"FWORD PTR", "fword ptr"}})
        s = text::replace_all(s, from, to);
    return s;
}

// Every `0x1f` of the text as MASM writes numbers.
std::string recon_masm_numbers(std::string_view s) {
    std::string out;
    for (std::size_t i = 0; i < s.size();) {
        if (s[i] == '0' && i + 2 < s.size() + 0 && s[i + 1] == 'x' && recon_is_lower_hex(s[i + 2])) {
            std::size_t j = i + 2;
            while (j < s.size() && recon_is_lower_hex(s[j])) ++j;
            out += masm_hex(*text::parse_int<std::uint32_t>(s.substr(i + 2, j - i - 2), 16));
            i = j;
        } else {
            out += s[i++];
        }
    }
    return out;
}

// The first leading-offset field of a MASM listing line: up to four blanks, four hex digits, then a non-word character. The end of the match.
std::optional<std::pair<std::uint32_t, std::size_t>> recon_listing_offset(std::string_view line) {
    std::size_t i = 0;
    while (i < line.size() && recon_is_space(line[i])) ++i;
    if (i > 4 || i + 4 > line.size()) return std::nullopt;
    for (std::size_t k = i; k < i + 4; ++k)
        if (!recon_is_hex(line[k])) return std::nullopt;
    if (i + 4 < line.size() && recon_is_word(line[i + 4])) return std::nullopt;
    return std::pair{*text::parse_int<std::uint32_t>(line.substr(i, 4), 16), i + 4};
}

// Blanks then two hex digits at a word's end: the byte field of a listing line that makes bytes.
bool recon_has_byte_field(std::string_view rest) {
    std::size_t i = 0;
    while (i < rest.size() && recon_is_space(rest[i])) ++i;
    if (i == 0 || i + 2 > rest.size() || !recon_is_hex(rest[i]) || !recon_is_hex(rest[i + 1])) return false;
    return i + 2 == rest.size() || !recon_is_word(rest[i + 2]);
}

// `; 04a4` (a lower-case hex offset of `lo` to `hi` digits) anywhere in the line.
std::optional<std::uint32_t> recon_comment_offset(std::string_view line, std::size_t lo, std::size_t hi) {
    for (std::size_t semi = line.find(';'); semi != std::string_view::npos; semi = line.find(';', semi + 1)) {
        std::size_t i = semi + 1;
        while (i < line.size() && recon_is_space(line[i])) ++i;
        std::size_t j = i;
        while (j < line.size() && recon_is_lower_hex(line[j])) ++j;
        const std::size_t n = j - i;
        if (n >= lo && n <= hi && (j == line.size() || !recon_is_word(line[j]))) return text::parse_int<std::uint32_t>(line.substr(i, n), 16);
    }
    return std::nullopt;
}

// The numbers of the source lines the assembler's messages name: `(12): error ...` or `(12) : fatal ...`.
std::vector<std::size_t> recon_error_lines(std::string_view messages) {
    std::vector<std::size_t> lines;
    for (std::size_t open = messages.find('('); open != std::string_view::npos; open = messages.find('(', open + 1)) {
        std::size_t i = open + 1;
        const std::size_t digits = i;
        while (i < messages.size() && messages[i] >= '0' && messages[i] <= '9') ++i;
        const std::size_t digits_end = i;
        if (digits_end == digits || i >= messages.size() || messages[i] != ')') continue;
        ++i;
        while (i < messages.size() && recon_is_space(messages[i])) ++i;
        if (i >= messages.size() || messages[i] != ':') continue;
        ++i;
        while (i < messages.size() && recon_is_space(messages[i])) ++i;
        const std::string_view rest = messages.substr(i);
        if (text::istarts_with(rest, "error") || text::istarts_with(rest, "fatal"))
            if (const auto number = text::parse_int<std::size_t>(messages.substr(digits, digits_end - digits), 10)) lines.push_back(*number);
    }
    return lines;
}

const char* kRetfMacro[] = {"retf    macro   n", "        ifb     <n>", "        db      0cbh", "        else", "        db      0cah",
                            "        dw      n", "        endif", "        endm", ""};

}  // namespace

std::string masm_hex(std::uint32_t value) {
    const std::string s = std::format("{:x}", value);
    return !s.empty() && s[0] >= 'a' && s[0] <= 'f' ? "0" + s + "h" : s + "h";
}

std::string to_masm(std::string_view instruction, const LabelOf& label_of) {
    std::string asm_text = recon_squeezed(instruction);
    std::string prefix;
    if (const std::size_t space = asm_text.find(' '); space != std::string::npos && recon_is_prefix(std::string_view(asm_text).substr(0, space))) {
        prefix = asm_text.substr(0, space + 1);
        asm_text = asm_text.substr(space + 1);
    }
    const std::size_t space = asm_text.find(' ');
    const std::string op = asm_text.substr(0, space);
    std::string rest = space == std::string::npos ? "" : asm_text.substr(space + 1);
    if (recon_is_string_op(op)) {
        const std::string lower = text::lower(asm_text);
        return prefix + op + (lower.find("word ptr") != std::string::npos ? "w" : "b");
    }
    if (const auto branch = reachability::direct_branch(op + " " + rest); branch && branch->op == op) {
        if (const auto label = label_of(branch->target)) return std::format("{} {}", op, *label);
    }
    rest = recon_masm_numbers(recon_lower_case_words(rest));
    return rest.empty() ? op : std::string(text::trim(op + " " + rest));
}

void emit_db(std::vector<std::string>& lines, ByteView raw, std::uint32_t off, std::string_view note) {
    for (std::size_t c = 0; c < raw.size(); c += 16) {
        std::vector<std::string> bytes;
        for (const std::uint8_t b : raw.subspan(c, std::min<std::size_t>(16, raw.size() - c))) {
            const std::string s = std::format("{:x}", b);
            bytes.push_back(s[0] >= 'a' && s[0] <= 'f' ? std::format("0{:02x}h", b) : std::format("{:02x}h", b));
        }
        lines.push_back(std::format("        db      {:<28} ; {:04x} {}", text::join(bytes, ", "), off + c, c == 0 ? note : std::string_view()));
    }
}

void emit_data(std::vector<std::string>& lines, ByteView code, std::uint32_t start, std::uint32_t end, datacode::Kind kind) {
    if (kind != datacode::Kind::String) {
        emit_db(lines, code.subspan(start, end - start), start, "data");
        return;
    }
    const auto printable = [&](std::uint32_t at) { return code[at] >= 0x20 && code[at] < 0x7F && code[at] != 0x27; };
    std::uint32_t i = start;
    while (i < end) {
        if (printable(i)) {
            std::uint32_t j = i;
            while (j < end && printable(j) && j - i < 40) ++j;
            lines.push_back(std::format("        db      {:<24} ; {:04x} string", "'" + text::from_latin1(code.subspan(i, j - i)) + "'", i));
            i = j;
        } else {
            std::uint32_t j = i;
            while (j < end && !printable(j)) ++j;
            emit_db(lines, code.subspan(i, j - i), i, "data");
            i = j;
        }
    }
}

std::vector<std::string> emit_function(const Seg& seg, std::uint32_t start, std::uint32_t end, const std::set<std::uint32_t>& force_db,
                                       std::span<const DataSpan> data_spans) {
    std::vector<const Row*> rows;
    for (const Row& row : seg.rows())
        if (row.offset >= start && row.offset < end) rows.push_back(&row);
    std::set<std::uint32_t> starts;
    for (const Row* row : rows) starts.insert(row->offset);
    std::map<std::uint32_t, std::string> labels;
    for (const Row* row : rows)
        if (const auto branch = reachability::direct_branch(row->text); branch && branch->target >= start && branch->target < end && starts.contains(branch->target))
            labels[branch->target] = std::format("L_{:04x}", branch->target);
    const LabelOf label_of = [&](std::uint32_t at) -> std::optional<std::string> {
        const auto found = labels.find(at);
        return found == labels.end() ? std::nullopt : std::optional<std::string>(found->second);
    };
    std::vector<DataSpan> spans(data_spans.begin(), data_spans.end());
    std::sort(spans.begin(), spans.end(), [](const DataSpan& a, const DataSpan& b) { return std::tie(a.start, a.end) < std::tie(b.start, b.end); });
    const auto span_at = [&](std::uint32_t p) -> const DataSpan* {
        for (const DataSpan& s : spans)
            if (s.start <= p && p < s.end) return &s;
        return nullptr;
    };
    const auto next_data = [&](std::uint32_t p) {
        std::uint32_t found = end;
        for (const DataSpan& s : spans)
            if (s.start > p) found = std::min(found, s.start);
        return found;
    };
    const ByteView code = seg.code();
    std::map<std::uint32_t, std::size_t> row_index;
    for (std::size_t i = 0; i < rows.size(); ++i) row_index[rows[i]->offset] = i;
    std::vector<std::string> lines;
    std::uint32_t pos = start;
    while (pos < end) {
        if (const DataSpan* data = span_at(pos)) {   // a readable DATA region
            const std::uint32_t stop = std::min(data->end, end);
            if (labels.contains(pos)) lines.push_back(labels[pos] + ":");
            emit_data(lines, code, pos, stop, data->kind);
            pos = stop;
            continue;
        }
        const std::uint32_t clamp = next_data(pos);
        const auto here = row_index.find(pos);
        if (here != row_index.end()) {
            const std::size_t i = here->second;
            const std::uint32_t off = rows[i]->offset;
            const std::string& instruction = rows[i]->text;
            const std::uint32_t next = std::min({i + 1 < rows.size() ? rows[i + 1]->offset : end, end, clamp});
            const ByteView raw = code.subspan(off, next - off);
            if (labels.contains(off)) lines.push_back(labels[off] + ":");
            const std::string masm = to_masm(instruction, label_of);
            const auto branch = reachability::direct_branch(instruction);
            const bool external_branch = branch && !labels.contains(branch->target);
            // far call and jump (9A, EA) are an absolute seg:off pointer: inherently outside, not a live MASM target here; always db
            const bool far = !raw.empty() && (raw[0] == 0x9A || raw[0] == 0xEA);
            // raw longer than any real 16-bit instruction (more than 8 bytes) means objdump folded a run of equal data bytes into this row (its `...`);
            // writing the mnemonic would DROP those bytes, so the whole raw is db (invisible to the per-instruction check where the instruction is a
            // macro like retf, so it is caught here)
            if (force_db.contains(off) || external_branch || far || instruction.find("(bad)") != std::string::npos || raw.size() > 8) {
                emit_db(lines, raw, off, masm);
            } else {
                std::vector<std::string> shown;
                for (const std::uint8_t b : raw.subspan(0, std::min<std::size_t>(10, raw.size()))) shown.push_back(std::format("{:02x}", b));
                lines.push_back(std::format("        {:<30} ; {:04x} {}", masm, off, text::join(shown, " ") + (raw.size() > 10 ? " ..." : "")));
            }
            pos = next;
        } else {
            // bytes no instruction row covers (the carve begins out of step with objdump, or data is embedded) are db, so coverage is complete and the
            // all-db fallback is always byte-exact
            std::uint32_t next_row = end;
            for (const Row* row : rows)
                if (row->offset > pos) {
                    next_row = row->offset;
                    break;
                }
            const std::uint32_t stop = std::min(next_row, clamp);
            if (labels.contains(pos)) lines.push_back(labels[pos] + ":");
            emit_db(lines, code.subspan(pos, stop - pos), pos, "data/uncovered");
            pos = stop;
        }
    }
    return lines;
}

std::string wrap(std::span<const std::string> body) {
    std::vector<std::string> all = {"        title   asmrecon"};
    all.insert(all.end(), std::begin(kRetfMacro), std::end(kRetfMacro));
    all.push_back("_TEXT   segment byte public 'CODE'");
    all.push_back("        assume  cs:_TEXT");
    all.insert(all.end(), body.begin(), body.end());
    all.push_back("_TEXT   ends");
    all.push_back("        end");
    return text::join(all, "\n") + "\n";
}

std::vector<ListedPair> masm_listing_pairs(std::string_view listing) {
    // (MASM's offset, the original's) of every listing line that makes bytes. A line that makes bytes has a leading offset AND a byte field;
    // labels and segment directives have an offset and no bytes. MASM wraps a long `db` line so that its `; NNNN` comment lands on the
    // CONTINUATION line: it is taken from there, or the db's bytes are credited to the previous instruction's length (the defect that
    // snowballed length differences).
    const std::vector<std::string_view> lines = text::lines(listing);
    std::vector<ListedPair> pairs;
    for (std::size_t idx = 0; idx < lines.size(); ++idx) {
        const auto leading = recon_listing_offset(lines[idx]);
        if (!leading || !recon_has_byte_field(lines[idx].substr(leading->second))) continue;
        std::optional<std::uint32_t> original = recon_comment_offset(lines[idx], 4, 6);
        if (!original && idx + 1 < lines.size() && !recon_listing_offset(lines[idx + 1])) original = recon_comment_offset(lines[idx + 1], 4, 6);
        pairs.emplace_back(leading->first, original);
    }
    return pairs;
}

std::vector<ListedPair> nasm_listing_pairs(std::string_view listing) {
    std::vector<ListedPair> pairs;
    for (const std::string_view line : text::lines(listing)) {
        // `<source line> <offset, 8 hex> <first byte, 2 hex> ...`
        std::size_t i = 0;
        while (i < line.size() && std::isspace(static_cast<unsigned char>(line[i]))) ++i;
        const std::size_t digits = i;
        while (i < line.size() && line[i] >= '0' && line[i] <= '9') ++i;
        if (i == digits) continue;
        const std::size_t after_number = i;
        while (i < line.size() && std::isspace(static_cast<unsigned char>(line[i]))) ++i;
        if (i == after_number || i + 8 > line.size()) continue;
        const std::string_view offset = line.substr(i, 8);
        if (!std::all_of(offset.begin(), offset.end(), recon_is_hex)) continue;
        i += 8;
        const std::size_t after_offset = i;
        while (i < line.size() && std::isspace(static_cast<unsigned char>(line[i]))) ++i;
        if (i == after_offset || i + 2 > line.size() || !recon_is_hex(line[i]) || !recon_is_hex(line[i + 1])) continue;
        pairs.emplace_back(*text::parse_int<std::uint32_t>(offset, 16), recon_comment_offset(line, 4, 4));
    }
    return pairs;
}

Divergers divergers_of(std::span<const ListedPair> pairs, ByteView our, const segdiff::Mask& our_mask, ByteView shipped, const segdiff::Mask& shipped_mask,
                       const std::map<std::uint32_t, std::uint32_t>& spans, std::uint32_t start) {
    Divergers found;
    for (std::size_t i = 0; i < pairs.size(); ++i) {
        const auto& [moff, ooff] = pairs[i];
        if (!ooff || !spans.contains(*ooff)) continue;   // an uncovered-data db line: exact already
        const std::int64_t mlen = i + 1 < pairs.size() ? static_cast<std::int64_t>(pairs[i + 1].first) - moff : static_cast<std::int64_t>(our.size()) - moff;
        const std::int64_t olen = static_cast<std::int64_t>(spans.at(*ooff)) - *ooff;
        const std::int64_t oi = static_cast<std::int64_t>(*ooff) - start;
        if (mlen != olen) {
            found.length.insert(*ooff);
            continue;
        }
        for (std::int64_t k = 0; k < olen; ++k) {
            if (our_mask.contains(static_cast<std::uint64_t>(moff + k)) || shipped_mask.contains(static_cast<std::uint64_t>(oi + k))) continue;
            if (static_cast<std::size_t>(moff + k) >= our.size() || static_cast<std::size_t>(oi + k) >= shipped.size()) break;
            if (our[static_cast<std::size_t>(moff + k)] != shipped[static_cast<std::size_t>(oi + k)]) {
                found.re_encode.insert(*ooff);
                break;
            }
        }
    }
    return found;
}

Divergers divergers(std::string_view listing, ByteView our, const segdiff::Mask& our_mask, ByteView shipped, const segdiff::Mask& shipped_mask,
                    const std::map<std::uint32_t, std::uint32_t>& spans, std::uint32_t start) {
    return divergers_of(masm_listing_pairs(listing), our, our_mask, shipped, shipped_mask, spans, start);
}

std::vector<std::size_t> masm_error_lines(std::string_view messages) { return recon_error_lines(messages); }

// `:12: error` / `:12: fatal`
std::vector<std::size_t> nasm_error_lines(std::string_view messages) {
    std::vector<std::size_t> lines;
    for (std::size_t colon = messages.find(':'); colon != std::string_view::npos; colon = messages.find(':', colon + 1)) {
        std::size_t i = colon + 1;
        const std::size_t digits = i;
        while (i < messages.size() && messages[i] >= '0' && messages[i] <= '9') ++i;
        const std::size_t digits_end = i;
        if (digits_end == digits || i >= messages.size() || messages[i] != ':') continue;
        ++i;
        while (i < messages.size() && recon_is_space(messages[i])) ++i;
        const std::string_view rest = messages.substr(i);
        if (text::istarts_with(rest, "error") || text::istarts_with(rest, "fatal"))
            if (const auto number = text::parse_int<std::size_t>(messages.substr(digits, digits_end - digits), 10)) lines.push_back(*number);
    }
    return lines;
}

std::optional<std::uint32_t> comment_offset(std::string_view line, std::size_t digits) { return recon_comment_offset(line, digits, digits); }
std::vector<DataSpan> safe_data_spans(const Seg& seg, std::uint32_t start, std::uint32_t end) {
    std::vector<datacode::Region> regions;
    std::set<std::uint32_t> targets;
    try {
        regions = reachability::safe_data_regions(seg);
        targets = reachability::branch_targets(seg);
    } catch (const Error&) {   // no regions: everything is written as instructions
        return {};
    }
    std::vector<std::uint32_t> row_starts;
    for (const Row& row : seg.rows()) row_starts.push_back(row.offset);
    std::sort(row_starts.begin(), row_starts.end());
    std::vector<DataSpan> spans;
    for (const datacode::Region& region : regions) {
        if (region.end <= start || region.start >= end) continue;
        std::uint32_t lo = region.start;
        if (!row_starts.empty() && row_starts.front() <= region.start)
            lo = *(std::upper_bound(row_starts.begin(), row_starts.end(), region.start) - 1);
        const auto high = std::lower_bound(row_starts.begin(), row_starts.end(), region.end);
        std::uint32_t hi = high != row_starts.end() ? *high : end;
        lo = std::max(lo, start);
        hi = std::min(hi, end);
        const auto entered = targets.lower_bound(lo);
        if (hi <= lo || (entered != targets.end() && *entered < hi)) continue;   // snapping reached a target: not safe
        spans.push_back({lo, hi, region.kind});
    }
    return spans;
}

Reconciled reconcile(const Seg& seg, std::uint32_t start, std::uint32_t end, const segdiff::Mask& seg_mask, const Assemble& assemble, std::size_t max_passes) {
    const ByteView shipped = seg.code().subspan(start, end - start);
    segdiff::Mask shipped_mask;
    for (const std::uint64_t m : seg_mask)
        if (m >= start && m < end) shipped_mask.insert(m - start);
    const std::vector<DataSpan> data_spans = safe_data_spans(seg, start, end);
    std::set<std::uint32_t> data_rows;
    for (const DataSpan& span : data_spans)
        for (const Row& row : seg.rows())
            if (row.offset >= span.start && row.offset < span.end) data_rows.insert(row.offset);
    std::vector<std::uint32_t> rows;
    for (const Row& row : seg.rows())
        if (row.offset >= start && row.offset < end && !data_rows.contains(row.offset)) rows.push_back(row.offset);
    std::sort(rows.begin(), rows.end());
    std::map<std::uint32_t, std::uint32_t> spans;
    for (std::size_t i = 0; i < rows.size(); ++i) spans[rows[i]] = i + 1 < rows.size() ? rows[i + 1] : end;

    Reconciled result;
    std::set<std::uint32_t> errored;   // what MASM cannot assemble is definitively data
    for (std::size_t pass = 0; pass < max_passes; ++pass) {
        const std::vector<std::string> body = emit_function(seg, start, end, result.force_db, data_spans);
        result.source = wrap(body);
        const std::vector<std::string_view> source_lines = text::lines(result.source);
        const Assembled made = assemble(result.source);
        // MASM's errors are localised to the instruction by the `; NNNN` offset each source line carries, and just those are written as db -- not
        // the whole function -- so the live instructions stay
        std::set<std::uint32_t> errs;
        for (const std::size_t line : recon_error_lines(made.log + "\n" + made.listing))
            if (line >= 1 && line <= source_lines.size())
                if (const auto at = recon_comment_offset(source_lines[line - 1], 4, 4)) errs.insert(*at);
        if (!made.object || !errs.empty()) {
            std::set<std::uint32_t> fresh;
            std::set_difference(errs.begin(), errs.end(), result.force_db.begin(), result.force_db.end(), std::inserter(fresh, fresh.end()));
            if (!fresh.empty()) {
                result.force_db.insert(fresh.begin(), fresh.end());
                errored.insert(fresh.begin(), fresh.end());   // MASM errored on these: data, not code
                continue;
            }
            if (result.force_db.size() >= rows.size()) break;   // cannot localise: the last resort is db for everything
            result.force_db = std::set<std::uint32_t>(rows.begin(), rows.end());
            continue;
        }
        const formats::omf::CodeSegment ours = formats::omf::code_segment(*made.object);
        result.verdict = segdiff::diff(ours.code, ours.fixup_mask, shipped, shipped_mask);
        if (result.verdict->exact) break;
        // Two phases per instruction, by the listing. First every instruction of another LENGTH, then assemble again: that restores the original
        // layout, so that a neighbouring near jump MASM would otherwise re-encode comes out right and stays live. Only once no length differs, the
        // true same-length re-encodings.
        const Divergers diverged = divergers(made.listing, ours.code, ours.fixup_mask, shipped, shipped_mask, spans, start);
        std::set<std::uint32_t> fresh;
        std::set_difference(diverged.length.begin(), diverged.length.end(), result.force_db.begin(), result.force_db.end(), std::inserter(fresh, fresh.end()));
        if (!fresh.empty()) {
            result.force_db.insert(fresh.begin(), fresh.end());
            continue;
        }
        std::set_difference(diverged.re_encode.begin(), diverged.re_encode.end(), result.force_db.begin(), result.force_db.end(), std::inserter(fresh, fresh.end()));
        if (fresh.empty()) break;
        result.force_db.insert(fresh.begin(), fresh.end());
    }
    // What MASM could not assemble is data (objdump's nonsense): out of the code count and the db count, so that the live share counts real
    // instructions only. It stays db in the source, which is byte-exact all the same.
    std::set<std::uint32_t> kept;
    std::set_difference(result.force_db.begin(), result.force_db.end(), errored.begin(), errored.end(), std::inserter(kept, kept.end()));
    result.force_db = std::move(kept);
    result.instructions = static_cast<std::size_t>(std::count_if(rows.begin(), rows.end(), [&](std::uint32_t o) { return !errored.contains(o); }));
    return result;
}

namespace {

struct Pick {
    std::string module;
    std::size_t segment = 0;
    std::uint32_t function = 0;
};

stdfs::path recon_resolve(const base::Corpus& corpus, const std::string& module) { return corpus.retail(module).value_or(fs::from_text(module)); }

// A real, substantive (16 to 120 bytes) carved function to reconstruct.
std::optional<Pick> recon_pick(const Environment& env, const segment::SharedOrdinals& ordinals) {
    for (const char* name : {"MOUSE", "KEYBOARD", "SYSTEM", "GDI", "USER"}) {
        try {
            const formats::ne::Module module = fixups::open_module(recon_resolve(env.corpus, name));
            for (const formats::ne::Segment& s : module.segments()) {
                if (s.is_data()) continue;
                const Seg seg(module, s.index, ordinals, {}, env.objdump);
                std::optional<std::pair<std::uint32_t, std::uint32_t>> best;   // (size, start)
                for (const auto& [start, function] : seg.functions()) {
                    const std::uint32_t size = function.end - function.start;
                    if (size >= 16 && size <= 120 && (!best || std::pair{size, start} < *best)) best = std::pair{size, start};
                }
                if (best) return Pick{name, s.index, best->second};
            }
        } catch (const Error&) {   // this module is not there: the next
        }
    }
    return std::nullopt;
}

// One function reconstructed and reported; nullopt (a line on stderr) when it is not found.
std::optional<Reconciled> recon_run(const Environment& env, const segment::SharedOrdinals& ordinals, const std::string& module, std::size_t index,
                                    std::optional<std::uint32_t> function) {
    const stdfs::path path = recon_resolve(env.corpus, module);
    const formats::ne::Module ne = fixups::open_module(path);
    const Seg seg(ne, index, ordinals, {}, env.objdump);
    const segdiff::Shipped shipped = segdiff::shipped_segment(ne, index);
    if (!function) {   // the smallest carved function of the segment
        std::optional<std::pair<std::uint32_t, std::uint32_t>> best;
        for (const auto& [start, f] : seg.functions())
            if (f.end > f.start && (!best || std::pair{f.end - f.start, start} < *best)) best = std::pair{f.end - f.start, start};
        if (!best) {
            err("no carved functions in {} seg{}\n", module, index);
            return std::nullopt;
        }
        function = best->second;
    }
    const auto found = seg.functions().find(*function);
    if (found == seg.functions().end()) {
        err("no function carved at {} seg{} 0x{:04x}\n", module, index, *function);
        return std::nullopt;
    }
    const std::uint32_t start = found->second.start, end = found->second.end;
    Reconciled r = reconcile(seg, start, end, shipped.mask, env.assemble);
    const std::size_t live = r.instructions - r.force_db.size();
    out("{} seg{} func 0x{:04x}  [{} bytes, {} instrs]\n", module, index, start, end - start, r.instructions);
    out("  verdict: {}\n", r.exact() ? "BYTE-EXACT" : "not exact");
    out("  live: {}/{} instructions ({:.0f}%), {} db\n", live, r.instructions, 100.0 * static_cast<double>(live) / static_cast<double>(std::max<std::size_t>(1, r.instructions)),
        r.force_db.size());
    return r;
}

}  // namespace

int main(const Environment& env, std::span<const std::string> args) {
    const cli::Command command{
        "recon",
        "a function as assembly that re-assembles to the same bytes",
        {cli::Positional{"module", "a corpus name or a path", false},
         cli::Option{"--seg", "", "the segment (from 1)", cli::Arg::Value, "N", "1"},
         cli::Option{"--func", "", "the function's offset (default: the smallest carved)", cli::Arg::Value, "OFFSET"},
         cli::Option{"--selftest", "", "reconstruct a real function and prove it exact", cli::Arg::Flag},
         cli::Option{"--out", "", "write the reconstructed .asm here", cli::Arg::Value, "FILE"},
         cli::Option{"--ords", "", "the ORDINALS.md to name ordinals from", cli::Arg::Value, "PATH", "ORDINALS.md"}},
        [&](const cli::Args& a) -> int {
            const segment::SharedOrdinals ordinals = std::make_shared<const segment::Ordinals>(segment::load_ordinals(fs::from_text(a.required("--ords"))));
            if (a.flag("--selftest")) {
                const auto pick = recon_pick(env, ordinals);
                if (!pick) {
                    err("no suitable NE function found for selftest\n");
                    return kExitFailed;
                }
                out("asmrecon selftest on {}\n\n", pick->module);
                const auto r = recon_run(env, ordinals, pick->module, pick->segment, pick->function);
                const bool ok = r && r->exact();
                out("\nSELFTEST: {}\n", ok ? "PASS - real function reconstructed byte-exact" : "FAIL");
                return ok ? kExitOk : kExitFailed;
            }
            const auto module = a.value("module");
            if (!module) throw UsageError("give a module or --selftest");
            const auto r = recon_run(env, ordinals, *module, a.integer<std::size_t>("--seg").value_or(1), a.integer<std::uint32_t>("--func"));
            if (!r) return kExitFailed;
            if (const auto target = a.value("out")) {
                fs::write_text(fs::from_text(*target), text::replace_all(r->source, "\n", "\r\n"));
                out("  wrote {}\n", *target);
            }
            return r->exact() ? kExitOk : kExitFailed;
        },
        {}, {}, {}};
    const cli::Cli program("recon", {command});
    std::vector<std::string> line = {"recon"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::ne_asm::recon
