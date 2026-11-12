// cb/source/image_writer.cpp -- what the writers of a program image share (image_writer.hpp).

#include "cb/source/image_writer.hpp"

#include <algorithm>
#include <format>

#include "cb/compare/image.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::source::image_writer {

std::string Spelling::operator()(std::uint32_t address) const {
    return std::format("{}{:0{}X}", prefix, address, digits);
}

Functions name_functions(std::span<const std::uint32_t> starts, Names& names, const Spelling& spelling) {
    Functions out;
    for (const std::uint32_t a : starts) {
        const auto [slot, made] = names.try_emplace(a, std::string());   // (a name given is kept, else one is made up)
        if (made) slot->second = spelling(a);
        out[a] = slot->second;
    }
    return out;
}

Names make_labels(const std::set<std::uint32_t>& targets, const Names& names, const LineStart& line_start, const Spelling& spelling) {
    Names out;
    for (const std::uint32_t a : targets) {
        if (!line_start(a)) continue;
        const auto named = names.find(a);
        out[a] = named != names.end() ? named->second : spelling(a);
    }
    for (const auto& [a, name] : names) {   // (a name given wins over one made up)
        if (line_start(a)) out[a] = name;
    }
    return out;
}

namespace {

// A character every assembler reads alike inside quotes: printable, and not the quote or the backslash.
bool printable(std::uint8_t b) { return b >= 32 && b < 127 && b != 0x22 && b != 0x5C; }

// A row's bytes as items: runs of four or more printable characters as one string, the rest as $XX.
std::vector<std::string> row_items(std::span<const std::uint8_t> row) {
    std::vector<std::string> items;
    std::size_t i = 0;
    while (i < row.size()) {
        std::size_t j = i;
        while (j < row.size() && printable(row[j])) ++j;
        if (j - i >= 4) {
            items.push_back("\"" + std::string(as_text(row.subspan(i, j - i))) + "\"");
            i = j;
        } else {
            items.push_back(std::format("${:02X}", row[i]));
            ++i;
        }
    }
    return items;
}

}  // namespace

std::vector<std::string> data_lines(ByteView data, const DataSyntax& syntax) {
    std::vector<std::string> out;
    std::size_t i = 0;
    while (i < data.size()) {
        std::size_t run = 1;
        while (i + run < data.size() && data[i + run] == data[i]) ++run;
        if (run >= 16) {
            out.push_back(std::format("        {} {}{}${:02X}", syntax.fill, run, syntax.fill_separator, data[i]));
            i += run;
            continue;
        }
        std::size_t row_end = i;
        while (row_end < data.size() && row_end - i < 16) {
            std::size_t same = 1;
            while (row_end + same < data.size() && data[row_end + same] == data[row_end]) ++same;
            if (same >= 16) break;   // (the run after the row is a fill line of its own)
            ++row_end;
        }
        out.push_back("        " + syntax.bytes + " " + text::join(row_items(data.subspan(i, row_end - i)), ","));
        i = row_end;
    }
    return out;
}

std::vector<std::string> commented(std::vector<std::string> lines, std::string_view what) {
    if (!lines.empty()) lines[0] += std::format("  ; {}", what);
    return lines;
}

std::string text(const translate::l2::Assembly& out) { return text::join(out.lines(), "\n"); }

Written finish(const std::filesystem::path& folder, std::string_view name, std::string_view source, Functions functions,
               std::size_t code_bytes, std::size_t image_bytes) {
    Written r;
    r.source = folder / std::string(name);
    fs::write_file(r.source, as_bytes(source));
    r.functions = std::move(functions);
    r.code_bytes = code_bytes;
    r.image_bytes = image_bytes;
    return r;
}

std::optional<Bytes> assembled(Written& r, const std::function<Bytes()>& build, ByteView original,
                               const std::optional<std::filesystem::path>& built_path, std::string_view what,
                               const std::function<bool(ByteView)>& same) {
    Bytes built;
    try {
        built = build();
    } catch (const ParseError& e) {   // (source that does not assemble: said in the notes; any other fault is not ours to say)
        r.notes.push_back(std::format("does not assemble: {}", e.what()));
        return std::nullopt;
    }
    r.linked = true;
    if (built_path) {
        fs::write_file(*built_path, built);
        r.built = *built_path;
    }
    r.same = same ? same(built) : (built.size() == original.size() && std::equal(built.begin(), built.end(), original.begin()));
    if (!r.same) {
        const std::optional<std::size_t> at = compare::image::first_difference(built, original);
        r.notes.push_back(std::format("the built {} differs from the original at byte {:#x}", what, at.value_or(original.size())));
    }
    return built;
}

Report report(const std::filesystem::path& program, const Written& r, std::string_view unit) {
    Report out;
    out.text = std::format("{}: {} functions, {} of {} {} reached as code -> {}\n  rebuilt: {}\n", program.filename().string(), r.functions.size(),
                           r.code_bytes, r.image_bytes, unit, fs::to_text(r.source),
                           r.same ? std::string("identical to the original")
                                  : r.notes.empty() ? std::string("differs") : text::join(r.notes, "; "));
    out.exit_code = r.same ? 0 : 1;
    return out;
}

}  // namespace cb::source::image_writer
