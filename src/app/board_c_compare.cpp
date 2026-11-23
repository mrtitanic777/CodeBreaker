// app/board_c_compare.cpp -- see board_c_compare.hpp.
#include "app/board_c_compare.hpp"

#include <algorithm>
#include <charconv>
#include <filesystem>
#include <format>
#include <system_error>

#include "app/board_c_functions.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"

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

// The line index of the "{" under a definition's signature at `at`, or npos.
std::size_t c_compare_body(const std::vector<std::string_view>& lines, std::size_t at) {
    if (lines[at].empty() || lines[at].back() == ';' || lines[at].front() == ' ' || lines[at].front() == '\t') return std::string_view::npos;
    for (std::size_t i = at + 1; i < lines.size() && i <= at + 2; ++i) {
        if (lines[i].empty()) continue;
        return lines[i].starts_with("{") ? i : std::string_view::npos;
    }
    return std::string_view::npos;
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
        for (std::size_t k = i; k < lines.size(); ++k) {
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
    if (const std::optional<std::string> label = c_function_label(function, base)) {
        CCodeSlice assembly = c_function_assembly(asm_text, *label, c_function_labels(c_text, base));
        if (assembly.lines.empty()) out.assembly_note = "The listing has no label at this function's address.";
        else if (assembly.cut) out.assembly_note = std::format("Showing the first {} lines.", kCCompareMostLines);
        out.assembly = std::move(assembly.lines);
    } else {
        out.assembly_note = "The listing labels code by address, and this function is known only by name.";
    }
    out.source = std::move(source.lines);
    out.source_spans = preview::highlight(preview::lang_for(".c"), out.source);
    out.assembly_spans = preview::highlight(preview::lang_for(".asm"), out.assembly);
    return out;
}

}  // namespace cb::app::board
