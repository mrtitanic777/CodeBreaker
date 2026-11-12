// cb-unity: alone (helpers named for what they do here)
#include "cb/port/tool_tree.hpp"

#include <algorithm>
#include <cctype>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::port::tool {

namespace stdfs = std::filesystem;

namespace {

// The files of a folder whose extension is one of `extensions` (lower case, with the dot), by name, as the folder lists them
// sorted by their full path.
std::vector<stdfs::path> files_with(const stdfs::path& dir, std::initializer_list<std::string_view> extensions) {
    std::vector<stdfs::path> out;
    std::error_code ec;
    if (!stdfs::is_directory(dir, ec)) return out;
    for (const stdfs::directory_entry& e : stdfs::directory_iterator(dir, ec)) {
        if (!e.is_regular_file(ec)) continue;
        const std::string ext = text::lower(fs::to_text(e.path().extension()));
        if (std::find(extensions.begin(), extensions.end(), ext) != extensions.end()) out.push_back(e.path());
    }
    std::sort(out.begin(), out.end(), [](const stdfs::path& a, const stdfs::path& b) { return fs::to_text(a) < fs::to_text(b); });
    return out;
}

std::vector<std::string> words_before_comment(std::string_view line) {
    std::vector<std::string> out;
    const std::string_view code = line.substr(0, line.find('#'));
    std::size_t i = 0;
    while (i < code.size()) {
        while (i < code.size() && (code[i] == ' ' || code[i] == '\t' || code[i] == '\r')) ++i;
        std::size_t j = i;
        while (j < code.size() && code[j] != ' ' && code[j] != '\t' && code[j] != '\r') ++j;
        if (j > i) out.emplace_back(code.substr(i, j - i));
        i = j;
    }
    return out;
}

std::optional<stdfs::path> list_file(const stdfs::path& source_dir, std::string_view name) {
    for (const stdfs::path& d : {source_dir, source_dir / "src"}) {
        const stdfs::path p = d / std::string(name);
        std::error_code ec;
        if (stdfs::is_regular_file(p, ec)) return p;
    }
    return std::nullopt;
}

std::string bare(std::string_view name) { return std::string(name.starts_with("_") ? name.substr(1) : name); }

}  // namespace

std::string normalize_newlines(std::string_view s) {
    std::string out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\r') {
            out.push_back('\n');
            if (i + 1 < s.size() && s[i + 1] == '\n') ++i;
        } else {
            out.push_back(s[i]);
        }
    }
    return out;
}

SourceTree read_tree(const stdfs::path& source_dir) {
    SourceTree tree;
    for (const stdfs::path& d : {source_dir, source_dir / "src"}) {
        for (const stdfs::path& p : files_with(d, {".c", ".h"}))
            tree.c.insert_or_assign(fs::to_text(p.filename()), normalize_newlines(fs::read_text(p)));
    }
    for (const stdfs::path& d : {source_dir, source_dir / "asm", source_dir / "src"}) {
        for (const stdfs::path& p : files_with(d, {".asm"})) {
            const std::string name = fs::to_text(p.filename());
            tree.assembler.insert_or_assign(name, normalize_newlines(fs::read_text(p)));
            tree.assembler_paths[name] = p;
        }
    }
    return tree;
}

OrderedMap<std::string, std::uint64_t> read_symbols(const stdfs::path& source_dir) {
    OrderedMap<std::string, std::uint64_t> out;
    const std::optional<stdfs::path> p = list_file(source_dir, "SYMBOLS");
    if (!p) return out;
    const std::string content = fs::read_text(*p);   // (the lines are views into it: it must outlive the loop)
    for (const std::string_view line : text::lines(content)) {
        const std::vector<std::string> w = words_before_comment(line);
        if (w.size() != 3 || w[1] != "D") continue;
        if (const std::optional<std::uint64_t> address = text::parse_int<std::uint64_t>(w[2], 16)) out.insert_or_assign(bare(w[0]), *address);
    }
    return out;
}

OrderedMap<std::string, std::uint64_t> read_bss(const stdfs::path& source_dir) {
    OrderedMap<std::string, std::uint64_t> out;
    const std::optional<stdfs::path> p = list_file(source_dir, "BSS");
    if (!p) return out;
    std::vector<std::pair<std::uint64_t, std::string>> entries;
    std::optional<std::pair<std::uint64_t, std::uint64_t>> range;
    std::vector<std::pair<std::uint64_t, std::uint64_t>> holes;
    const auto hex = [](const std::string& s) { return text::parse_int<std::uint64_t>(s, 16).value_or(0); };
    const std::string content = fs::read_text(*p);   // (the lines are views into it: it must outlive the loop)
    for (const std::string_view line : text::lines(content)) {
        const std::vector<std::string> w = words_before_comment(line);
        if (w.size() == 3 && w[0] == "range") range = {hex(w[1]), hex(w[2])};
        else if (w.size() == 3 && w[0] == "hole") holes.emplace_back(hex(w[1]), hex(w[2]));
        else if (w.size() == 2) entries.emplace_back(hex(w[1]), w[0]);
    }
    std::stable_sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    for (std::size_t i = 0; i < entries.size(); ++i) {
        const auto& [offset, name] = entries[i];
        std::uint64_t end = i + 1 < entries.size() ? entries[i + 1].first : (range ? range->second : offset + 2);
        for (const auto& [at, length] : holes)
            if (offset < at && at < end) end = at;   // a hole that begins first cuts the variable short
        out.insert_or_assign(bare(name), end - offset);
    }
    return out;
}

std::vector<std::pair<std::uint32_t, std::string>> data_places(const stdfs::path& assembler_module) {
    std::vector<std::pair<std::uint32_t, std::string>> out;
    const stdfs::path p = assembler_module.parent_path() / "SYMBOLS";
    std::error_code ec;
    if (!stdfs::is_regular_file(p, ec)) return out;
    const std::string content = fs::read_text(p);
    for (const std::string_view line : text::lines(content)) {
        // whitespace-separated words of the whole line (no comment is cut here)
        std::vector<std::string> w;
        std::size_t i = 0;
        while (i < line.size()) {
            while (i < line.size() && (line[i] == ' ' || line[i] == '\t' || line[i] == '\r')) ++i;
            std::size_t j = i;
            while (j < line.size() && line[j] != ' ' && line[j] != '\t' && line[j] != '\r') ++j;
            if (j > i) w.emplace_back(line.substr(i, j - i));
            i = j;
        }
        if (w.size() != 3 || w[1] != "D") continue;
        const bool all_hex = !w[2].empty() && std::all_of(w[2].begin(), w[2].end(), [](unsigned char c) { return std::isxdigit(c) != 0; });
        if (!all_hex) continue;
        if (const auto a = text::parse_int<std::uint32_t>(w[2], 16)) out.emplace_back(*a, w[0]);
    }
    std::sort(out.begin(), out.end());
    return out;
}

}  // namespace cb::port::tool
