// app/board_c_compare.cpp -- see board_c_compare.hpp.
#include "app/board_c_compare.hpp"

#include <algorithm>
#include <charconv>
#include <filesystem>
#include <format>
#include <map>
#include <nlohmann/json.hpp>
#include <system_error>

#include "app/board_c_functions.hpp"
#include "core/capstone_api.hpp"
#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/text.hpp"
#include "formats/ne.hpp"
#include "formats/pe.hpp"

namespace cb::app::board {

namespace {

bool c_compare_word_char(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

std::vector<std::string_view> c_compare_lines(std::string_view text) {
    std::vector<std::string_view> out;
    while (!text.empty()) {
        const std::size_t end = text.find('\n');
        std::string_view line = text.substr(0, end);
        text.remove_prefix(end == std::string_view::npos ? text.size() : end + 1);
        if (!line.empty() && line.back() == '\r') line.remove_suffix(1);
        out.push_back(line);
    }
    return out;
}

std::string c_compare_shown(std::string_view line) {
    std::string out;
    out.reserve(line.size());
    for (const char c : line) {
        if (c == '\t') out += "    ";
        else out += c;
    }
    return out;
}

// The line index of the "{" under a definition whose signature starts at `at` (it may run over several lines), or npos.
std::size_t c_compare_body(const std::vector<std::string_view>& lines, std::size_t at) {
    constexpr std::size_t kMostSignatureLines = 24;
    if (lines[at].empty() || lines[at].front() == ' ' || lines[at].front() == '\t' || lines[at].find('(') == std::string_view::npos) return std::string_view::npos;
    int depth = 0;
    std::size_t closed = lines.size();
    for (std::size_t i = at; i < lines.size() && i < at + kMostSignatureLines && closed == lines.size(); ++i) {
        for (const char c : lines[i]) depth += c == '(' ? 1 : c == ')' ? -1 : 0;
        if (depth <= 0) closed = i;
    }
    if (closed == lines.size() || lines[closed].ends_with(";")) return std::string_view::npos;
    for (std::size_t i = closed + 1; i < lines.size() && i <= closed + 2; ++i) {
        if (lines[i].empty()) continue;
        return lines[i].starts_with("{") ? i : std::string_view::npos;
    }
    return std::string_view::npos;
}

// A long signature puts its return type on the line above.
bool c_compare_return_type(std::string_view line) {
    return !line.empty() && line.front() != ' ' && line.front() != '\t' && line.front() != '#' && line.front() != '}' && !line.starts_with("//") && !line.ends_with(";") && line.find('(') == std::string_view::npos;
}

std::uint64_t c_compare_count(const Json& entry, const char* key) {
    const auto it = entry.find(key);
    return it != entry.end() && it->is_number_unsigned() ? it->get<std::uint64_t>() : 0;
}

bool c_compare_hex(std::string_view s, std::uint64_t& out);   // (below)

std::optional<CCodeSlice> c_original_pe(ByteView file, std::string_view function, const Json& entry) {
    const std::optional<formats::pe::Image> image = formats::pe::Image::try_parse(file);
    if (!image || image->header().machine != 0x14C || image->header().is_plus) return std::nullopt;
    const std::uint64_t length = c_compare_count(entry, "original");
    if (length == 0) return std::nullopt;
    std::optional<std::uint64_t> rva;
    std::uint64_t numbered = 0;
    if (function.starts_with("sub_") && c_compare_hex(function.substr(4), numbered)) rva = numbered;
    else
        for (const formats::pe::Export& e : image->exports())
            if (e.name && *e.name == function) rva = e.rva;
    if (!rva) return std::nullopt;
    const std::optional<ByteView> code = image->read_rva(*rva, length);
    if (!code) return std::nullopt;
    return c_disassemble(*code, image->header().image_base + *rva, 32);
}

std::optional<CCodeSlice> c_original_ne(ByteView file, std::string_view function, const Json& all) {
    const std::optional<formats::ne::Module> module = formats::ne::Module::try_open(file);
    if (!module) return std::nullopt;
    std::map<std::string, std::uint32_t, std::less<>> ordinals;
    for (const auto& table : {module->resident_names(), module->nonresident_names()})
        for (const formats::ne::Name& n : table)
            if (n.ordinal != 0) ordinals.emplace(text::lower(n.name), n.ordinal);
    std::map<std::uint32_t, std::pair<std::uint32_t, std::uint64_t>> entries;
    for (const formats::ne::Entry& e : module->entries())
        if (e.kind != formats::ne::Entry::Kind::Unused) entries[e.ordinal] = {e.segment, e.offset};
    const auto place = [&](std::string_view name) -> std::optional<std::pair<std::uint32_t, std::uint64_t>> {
        if (name.starts_with("sub_")) {
            const std::string_view rest = name.substr(4);
            const std::size_t bar = rest.find('_');
            std::uint64_t segment = 0, offset = 0;
            if (bar != std::string_view::npos && c_compare_hex(rest.substr(bar + 1), offset)) {
                const std::string_view digits = rest.substr(0, bar);
                const auto [end, ec] = std::from_chars(digits.data(), digits.data() + digits.size(), segment, 10);
                if (ec == std::errc{} && end == digits.data() + digits.size()) return std::pair{static_cast<std::uint32_t>(segment), offset};
            }
        }
        const auto ordinal = ordinals.find(text::lower(name));
        if (ordinal == ordinals.end()) return std::nullopt;
        const auto entry = entries.find(ordinal->second);
        return entry == entries.end() ? std::nullopt : std::optional{entry->second};
    };
    const auto where = place(function);
    if (!where) return std::nullopt;
    std::vector<std::uint64_t> starts;
    for (auto it = all.begin(); it != all.end(); ++it) {
        const auto other = place(it.key());
        if (other && other->first == where->first) starts.push_back(other->second);
    }
    for (const formats::ne::Segment& s : module->segments()) {
        if (s.index != where->first || s.file_offset == 0 || where->second >= s.length || s.file_offset + s.length > module->data().size()) continue;
        const std::uint64_t end = c_function_end(starts, where->second, s.length);
        return c_disassemble(module->data().subspan(s.file_offset + where->second, end - where->second), where->second, 16);
    }
    return std::nullopt;
}

// Where `name(` stands as a whole word in the line.
bool c_compare_names(std::string_view line, std::string_view name) {
    for (std::size_t at = line.find(name); at != std::string_view::npos; at = line.find(name, at + 1)) {
        const std::size_t after = at + name.size();
        if ((at == 0 || !c_compare_word_char(line[at - 1])) && after < line.size() && line[after] == '(') return true;
    }
    return false;
}

bool c_compare_hex(std::string_view s, std::uint64_t& out) {
    if (s.empty()) return false;
    const auto [end, ec] = std::from_chars(s.data(), s.data() + s.size(), out, 16);
    return ec == std::errc{} && end == s.data() + s.size();
}

// In `folder`, the file named `wanted`, else the first with this ending. Empty: none.
std::filesystem::path c_compare_pick(const std::filesystem::path& folder, const std::string& wanted, std::string_view ending) {
    std::vector<std::filesystem::path> found;
    std::error_code ec;
    for (std::filesystem::directory_iterator it(folder, ec), end; !ec && it != end; it.increment(ec)) {
        if (!it->is_regular_file(ec)) continue;
        const std::string name = text::lower(fs::to_text(it->path().filename()));
        if (name == wanted) return it->path();
        if (name.ends_with(ending)) found.push_back(it->path());
    }
    std::sort(found.begin(), found.end());
    return found.empty() ? std::filesystem::path() : found.front();
}

}  // namespace

CCodeSlice c_function_source(std::string_view c_text, std::string_view name) {
    CCodeSlice out;
    const std::vector<std::string_view> lines = c_compare_lines(c_text);
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (!c_compare_names(lines[i], name) || c_compare_body(lines, i) == std::string_view::npos) continue;
        for (std::size_t k = i > 0 && c_compare_return_type(lines[i - 1]) ? i - 1 : i; k < lines.size(); ++k) {
            if (out.lines.size() == kCCompareMostLines) {
                out.cut = true;
                break;
            }
            out.lines.push_back(c_compare_shown(lines[k]));
            if (lines[k].starts_with("}")) break;
        }
        break;
    }
    return out;
}

std::optional<std::string> c_function_label(std::string_view name, std::uint64_t image_base) {
    if (!name.starts_with("sub_")) return std::nullopt;
    const std::string_view rest = name.substr(4);
    std::uint64_t offset = 0;
    if (const std::size_t bar = rest.find('_'); bar != std::string_view::npos) {
        const std::string_view segment = rest.substr(0, bar);
        if (segment.empty() || !std::all_of(segment.begin(), segment.end(), [](char c) { return c >= '0' && c <= '9'; })) return std::nullopt;
        if (!c_compare_hex(rest.substr(bar + 1), offset)) return std::nullopt;
        return std::format("s{}_L{:x}", segment, offset);
    }
    if (image_base == 0 || !c_compare_hex(rest, offset)) return std::nullopt;
    return std::format("L{:x}", image_base + offset);
}

std::uint64_t c_listing_base(std::string_view asm_text) {
    constexpr std::string_view kImage = "IMAGE equ 0x";
    std::size_t at = asm_text.starts_with(kImage) ? 0 : asm_text.find("\nIMAGE equ 0x");
    if (at == std::string_view::npos) return 0;
    at += asm_text[at] == '\n' ? 1 + kImage.size() : kImage.size();
    std::size_t end = at;
    while (end < asm_text.size() && ((asm_text[end] >= '0' && asm_text[end] <= '9') || (asm_text[end] >= 'a' && asm_text[end] <= 'f') || (asm_text[end] >= 'A' && asm_text[end] <= 'F'))) ++end;
    std::uint64_t base = 0;
    return c_compare_hex(asm_text.substr(at, end - at), base) ? base : 0;
}

std::set<std::string, std::less<>> c_function_labels(std::string_view c_text, std::uint64_t image_base) {
    std::set<std::string, std::less<>> out;
    const std::vector<std::string_view> lines = c_compare_lines(c_text);
    for (std::size_t i = 0; i < lines.size(); ++i) {
        const std::string_view line = lines[i];
        const std::size_t paren = line.find('(');
        if (paren == std::string_view::npos || c_compare_body(lines, i) == std::string_view::npos) continue;
        std::size_t start = paren;
        while (start > 0 && c_compare_word_char(line[start - 1])) --start;
        if (const auto label = c_function_label(line.substr(start, paren - start), image_base)) out.insert(*label);
    }
    return out;
}

CCodeSlice c_function_assembly(std::string_view asm_text, std::string_view label, const std::set<std::string, std::less<>>& ends) {
    CCodeSlice out;
    const std::vector<std::string_view> lines = c_compare_lines(asm_text);
    std::size_t first = lines.size();
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].size() > label.size() && lines[i].starts_with(label) && lines[i][label.size()] == ':') {
            first = i;
            break;
        }
    }
    for (std::size_t i = first; i < lines.size(); ++i) {
        const std::string_view line = lines[i];
        if (i > first) {
            if (line.starts_with("section ") || line.starts_with("segment ")) break;
            if (const std::size_t colon = line.find(':'); colon != std::string_view::npos && ends.find(line.substr(0, colon)) != ends.end()) break;
        }
        if (out.lines.size() == kCCompareMostLines) {
            out.cut = true;
            break;
        }
        out.lines.push_back(c_compare_shown(line));
    }
    while (!out.lines.empty() && out.lines.back().find_first_not_of(' ') == std::string::npos) out.lines.pop_back();
    return out;
}

namespace {

// `bytes` as the lines of one side; `differs[i]`: byte i is a difference.
CByteSide c_byte_side(ByteView bytes, ByteView mask, const std::vector<char>& differs) {
    constexpr std::size_t kPerLine = 16, kHexAt = 6;
    CByteSide side;
    for (std::size_t at = 0; at < bytes.size(); at += kPerLine) {
        std::string line = std::format("{:04X}  ", at);
        std::vector<preview::Span> spans;
        std::vector<std::uint8_t> columns;
        for (std::size_t c = 0; c < kPerLine && at + c < bytes.size(); ++c) {
            line += std::format("{:02X} ", bytes[at + c]);
            if (at + c < mask.size() && mask[at + c] != 0) spans.push_back({static_cast<std::uint32_t>(kHexAt + c * 3), 2, preview::Tok::Comment});
            if (differs[at + c]) columns.push_back(static_cast<std::uint8_t>(c));
        }
        line.pop_back();
        side.lines.push_back(std::move(line));
        side.spans.push_back(std::move(spans));
        side.differ.push_back(std::move(columns));
    }
    return side;
}

}  // namespace

CByteDiff c_byte_diff(ByteView built, ByteView built_mask, ByteView original, ByteView original_mask) {
    CByteDiff out;
    out.same_length = built.size() == original.size();
    const auto masked = [](ByteView mask, std::size_t i) { return i < mask.size() && mask[i] != 0; };
    const auto equal = [&](std::size_t b, std::size_t o) { return masked(built_mask, b) || masked(original_mask, o) || built[b] == original[o]; };
    std::vector<char> built_differs(built.size(), 0), original_differs(original.size(), 0);
    if (out.same_length) {
        for (std::size_t i = 0; i < built.size(); ++i) {
            if (masked(built_mask, i) || masked(original_mask, i)) continue;
            ++out.compared;
            if (built[i] != original[i]) {
                built_differs[i] = original_differs[i] = 1;
                ++out.differing;
            }
        }
    } else {
        const std::size_t shorter = std::min(built.size(), original.size());
        std::size_t start = 0, end = 0;
        while (start < shorter && equal(start, start)) ++start;
        while (end < shorter - start && equal(built.size() - 1 - end, original.size() - 1 - end)) ++end;
        for (std::size_t i = start; i + end < built.size(); ++i) built_differs[i] = masked(built_mask, i) ? 0 : 1;
        for (std::size_t i = start; i + end < original.size(); ++i) original_differs[i] = masked(original_mask, i) ? 0 : 1;
        const auto count = [](const std::vector<char>& v) { return static_cast<std::uint64_t>(std::count(v.begin(), v.end(), char{1})); };
        out.differing = std::max(count(built_differs), count(original_differs));
        std::uint64_t free_built = 0, free_original = 0;
        for (std::size_t i = 0; i < built.size(); ++i) free_built += masked(built_mask, i) ? 0 : 1;
        for (std::size_t i = 0; i < original.size(); ++i) free_original += masked(original_mask, i) ? 0 : 1;
        out.compared = std::max(free_built, free_original);
    }
    out.built = c_byte_side(built, built_mask, built_differs);
    out.original = c_byte_side(original, original_mask, original_differs);
    return out;
}

std::optional<CByteDiff> parse_c_byte_diff(std::string_view text, std::string_view function) {
    Json all;
    if (!json::try_parse(text, all) || !all.is_object()) return std::nullopt;
    const auto entry = all.find(std::string(function));
    if (entry == all.end() || !entry->is_object()) return std::nullopt;
    const auto field = [&](const char* key) -> std::optional<Bytes> {
        const auto it = entry->find(key);
        if (it == entry->end() || !it->is_string()) return std::nullopt;
        try {
            return crypto::from_hex(it->get<std::string>());
        } catch (const std::exception&) {
            return std::nullopt;
        }
    };
    const auto built = field("built"), built_mask = field("built_mask"), original = field("original"), original_mask = field("original_mask");
    if (!built || !built_mask || !original || !original_mask || built_mask->size() != built->size() || original_mask->size() != original->size()) return std::nullopt;
    return c_byte_diff(*built, *built_mask, *original, *original_mask);
}

std::string c_bytes_file_name(std::string_view rel) {
    std::string name = c_functions_file_name(rel);
    return name.substr(0, name.size() - 5) + ".bytes.json";
}

CCodeSlice c_disassemble(ByteView code, std::uint64_t address, int bits) {
    const auto& cs = disasm::capstone::api();
    csh handle = 0;
    disasm::capstone::check(cs.cs_open(CS_ARCH_X86, bits == 16 ? CS_MODE_16 : CS_MODE_32, &handle), "cs_open");
    cs_insn* insn = cs.cs_malloc(handle);
    CCodeSlice out;
    const std::uint8_t* at = code.data();
    std::size_t left = code.size();
    std::uint64_t pc = address;
    const int width = bits == 16 ? 4 : 8;
    while (left > 0) {
        if (out.lines.size() == kCCompareMostLines) {
            out.cut = true;
            break;
        }
        if (insn != nullptr && cs.cs_disasm_iter(handle, &at, &left, &pc, insn)) {
            std::string line = std::format("{:0{}X}  {}", insn->address, width, insn->mnemonic);
            if (insn->op_str[0] != '\0') line += std::format(" {}", insn->op_str);
            out.lines.push_back(std::move(line));
        } else {
            out.lines.push_back(std::format("{:0{}X}  db 0x{:02X}", pc, width, *at));
            ++at;
            --left;
            ++pc;
        }
    }
    if (insn != nullptr) cs.cs_free(insn, 1);
    cs.cs_close(&handle);
    return out;
}

std::uint64_t c_function_end(const std::vector<std::uint64_t>& starts, std::uint64_t start, std::uint64_t limit) {
    std::uint64_t end = limit;
    for (const std::uint64_t s : starts)
        if (s > start && s < end) end = s;
    return end;
}

std::optional<CCodeSlice> c_original_code(ByteView file, std::string_view format, std::string_view function, std::string_view results) {
    Json all;
    if (!json::try_parse(results, all) || !all.is_object()) return std::nullopt;
    const auto entry = all.find(std::string(function));
    if (entry == all.end() || !entry->is_object()) return std::nullopt;
    if (format == "PE") return c_original_pe(file, function, *entry);
    if (format == "NE") return c_original_ne(file, function, all);
    return std::nullopt;
}

CFunctionCompare load_c_compare(const Board& board, const Index& index, std::uint32_t card, const CScores* scores, std::string_view function, std::string_view measure) {
    CFunctionCompare out;
    out.function = std::string(function);
    out.measure = std::string(measure);
    if (card >= board.cards.size() || index.nodes.size() != board.cards.size()) return out;
    out.program = board.cards[card].name;
    const std::string scored = scores != nullptr ? c_scored_rel(board, index, card, *scores) : std::string();
    const Card* converted = nullptr;
    const auto consider = [&](const Card& c) {
        if (c.conv != Conv::Done || c.output.empty()) return;
        if (converted == nullptr || (c.rel == scored && converted->rel != scored)) converted = &c;
    };
    consider(board.cards[card]);
    if (!index.nodes[card].sha256.empty())
        for (std::uint32_t i = 0; i < index.nodes.size(); ++i)
            if (index.nodes[i].group == index.nodes[card].group) consider(board.cards[i]);
    if (converted == nullptr) {
        out.source_note = out.assembly_note = "This program's conversion is not on disk.";
        return out;
    }
    const std::filesystem::path folder = fs::from_text(converted->output);
    const std::string lower = text::lower(converted->name);
    const std::filesystem::path c_file = c_compare_pick(folder, lower.substr(0, lower.rfind('.')) + ".c", ".c");
    const std::filesystem::path asm_file = c_compare_pick(folder, lower + ".asm", ".asm");
    std::string c_text, asm_text;
    try {
        if (!c_file.empty()) c_text = fs::read_text(c_file);
        if (!asm_file.empty()) asm_text = fs::read_text(asm_file);
    } catch (const std::exception&) {
        out.source_note = out.assembly_note = "The conversion's files could not be read just now.";
        return out;
    }
    CCodeSlice source = c_function_source(c_text, function);
    if (source.lines.empty()) out.source_note = "This function is not in the converted C.";
    else if (source.cut) out.source_note = std::format("Showing the first {} lines.", kCCompareMostLines);
    const std::uint64_t base = c_listing_base(asm_text);
    std::optional<CCodeSlice> exact;
    try {
        const std::string rel = scored.empty() ? converted->rel : scored;
        const std::string results = fs::read_text(board.root / "c_scores" / c_functions_file_name(rel));
        const Bytes file = fs::read_file(board.root / "files" / fs::from_text(rel));
        exact = c_original_code(file, converted->format, function, results);
    } catch (const std::exception&) {}   // (the listing is still there)
    if (exact && !exact->lines.empty()) {
        if (exact->cut) out.assembly_note = std::format("Showing the first {} lines.", kCCompareMostLines);
        else if (converted->format == "NE") out.assembly_note = "Calls the loader fixes up show placeholder addresses.";
        out.assembly = std::move(exact->lines);
    } else if (const std::optional<std::string> label = c_function_label(function, base)) {
        CCodeSlice assembly = c_function_assembly(asm_text, *label, c_function_labels(c_text, base));
        if (assembly.lines.empty()) out.assembly_note = "The listing has no label at this function's address.";
        else if (assembly.cut) out.assembly_note = std::format("Showing the first {} lines.", kCCompareMostLines);
        out.assembly = std::move(assembly.lines);
    } else {
        out.assembly_note = "The listing labels code by address, and this function is known only by name.";
    }
    try {
        const std::string rel = scored.empty() ? converted->rel : scored;
        const std::filesystem::path bytes_file = board.root / "c_scores" / c_bytes_file_name(rel);
        std::error_code ec;
        if (std::filesystem::is_regular_file(bytes_file, ec)) out.bytes = parse_c_byte_diff(fs::read_text(bytes_file), function);
    } catch (const std::exception&) {}   // (the scorer has it open: the panel says the bytes are not there)
    out.source = std::move(source.lines);
    out.source_spans = preview::highlight(preview::lang_for(".c"), out.source);
    out.assembly_spans = preview::highlight(preview::lang_for(".asm"), out.assembly);
    return out;
}

}  // namespace cb::app::board
