#include "cb/exact/readable.hpp"

#include <algorithm>
#include <cctype>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::exact::readable {
namespace {

bool is_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }
bool is_lower_hex(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }
bool all_lower_hex(std::string_view s) { return std::ranges::all_of(s, is_lower_hex); }

// Where a comment, string or character constant that begins at `open` ends, or nullopt if it is not closed: a
// string or constant holds no raw newline (a backslash takes the next character, newline too); a comment ends at
// the first star-slash.
std::optional<std::size_t> literal_end(std::string_view s, std::size_t open) {
    if (s[open] == '/') {
        if (open + 1 >= s.size() || s[open + 1] != '*') return std::nullopt;
        const std::size_t close = s.find("*/", open + 2);
        return close == std::string_view::npos ? std::nullopt : std::optional<std::size_t>(close + 2);
    }
    const char quote = s[open];
    for (std::size_t j = open + 1; j < s.size();) {
        if (s[j] == quote) return j + 1;
        if (s[j] == '\\') {
            if (j + 1 >= s.size()) return std::nullopt;
            j += 2;
        } else if (s[j] == '\n') {
            return std::nullopt;
        } else {
            ++j;
        }
    }
    return std::nullopt;
}

}  // namespace

std::optional<Place> parse_address_name(std::string_view w) {
    // fXXXXX or _fXXXXX: five lower-case hex digits; g_XXXX: one to four.
    std::string_view digits;
    if (w.starts_with("_f")) digits = w.substr(2);
    else if (w.starts_with('f')) digits = w.substr(1);
    if (digits.size() == 5 && all_lower_hex(digits)) return Place{Place::Kind::Function, *text::parse_int<std::uint32_t>(digits, 16)};
    if (w.starts_with("g_") && w.size() >= 3 && w.size() <= 6 && all_lower_hex(w.substr(2)))
        return Place{Place::Kind::Variable, *text::parse_int<std::uint32_t>(w.substr(2), 16)};
    return std::nullopt;
}

NameMap mapping(const SymbolTable& symbols) {
    NameMap out;
    for (const auto& [name, where] : symbols) {
        if (name.find_first_of(":@") != std::string::npos) continue;
        const std::size_t first_plain = std::min(name.find_first_not_of('_'), name.size());
        if (parse_address_name(std::string_view(name).substr(first_plain)) || parse_address_name(name)) continue;   // (already an address-name)
        std::string_view c = name;
        if (c.starts_with('_')) c.remove_prefix(1);   // (the C name of a public _name)
        if (!text::is_identifier(c)) continue;
        if (where.kind == SymKind::Code || where.kind == SymKind::Unknown)
            out.try_emplace(Place{Place::Kind::Function, static_cast<std::uint32_t>(where.address)}, std::string(c));
        if ((where.kind == SymKind::Data || where.kind == SymKind::Unknown) && where.address < 0x10000)
            out.try_emplace(Place{Place::Kind::Variable, static_cast<std::uint32_t>(where.address)}, std::string(c));
    }
    return out;
}

std::string rename(std::string_view text, const NameMap& names) {
    // Every identifier the text has already (a new name must not meet one of them).
    std::unordered_set<std::string_view> have;
    for (std::size_t i = 0; i < text.size();) {
        if (!is_word(text[i])) {
            ++i;
            continue;
        }
        std::size_t end = i;
        while (end < text.size() && is_word(text[end])) ++end;
        if (!std::isdigit(static_cast<unsigned char>(text[i]))) have.insert(text.substr(i, end - i));
        i = end;
    }
    std::unordered_map<std::string_view, Place> done;   // the places each new name was given to

    const auto rename_code = [&](std::string_view piece, std::string& out) {
        for (std::size_t i = 0; i < piece.size();) {
            if (!is_word(piece[i])) {
                out += piece[i++];
                continue;
            }
            std::size_t end = i;
            while (end < piece.size() && is_word(piece[end])) ++end;
            const std::string_view word = piece.substr(i, end - i);
            i = end;
            const std::optional<Place> place = parse_address_name(word);
            const auto named = place ? names.find(*place) : names.end();
            if (named != names.end()) {
                const std::string_view n = named->second;
                const auto taken = done.find(n);
                if (!have.contains(n) || (taken != done.end() && taken->second == *place)) {
                    done[n] = *place;
                    out += n;
                    continue;
                }
            }
            out += word;
        }
    };

    std::string out;
    std::size_t piece_from = 0;
    for (std::size_t i = 0; i < text.size();) {
        if (text[i] == '/' || text[i] == '"' || text[i] == '\'') {
            if (const auto end = literal_end(text, i)) {
                rename_code(text.substr(piece_from, i - piece_from), out);
                out += text.substr(i, *end - i);   // (not in a remark or a string: another program's place, or text)
                piece_from = i = *end;
                continue;
            }
        }
        ++i;
    }
    rename_code(text.substr(piece_from), out);
    return out;
}

namespace {

using Counts = std::map<std::pair<std::string, std::string>, std::string>;

Counts counts(const Project& project) {
    Counts out;
    for (LedgerRow& r : project.ledger()) out[{r.file, r.function}] = std::move(r.verdict);
    return out;
}

}  // namespace

int run(const Project& project, const Options& options) {
    const NameMap names = mapping(project.symbols());
    if (names.empty()) {
        out("SYMBOLS has no names for the address-names (the names stage puts them there)\n");
        return kExitFailed;
    }
    const std::vector<SourceFile> files = project.sources();
    std::vector<std::pair<std::filesystem::path, std::string>> renamed;   // (a file's path, its new text), in the project's order
    for (const SourceFile& f : files) {
        const Bytes bytes = fs::read_file(f.path);
        const std::string_view old_text = as_text(bytes);
        std::string new_text = rename(old_text, names);
        if (new_text != old_text) renamed.emplace_back(f.path, std::move(new_text));
    }
    out("{} of {} files would take names ({} names known)\n", renamed.size(), files.size(), names.size());
    if (renamed.empty()) return kExitOk;

    const Counts before = counts(project);
    Counts after;
    {   // (the project again, its sources renamed)
        fs::ScopedTempDir work("readable");
        const std::filesystem::path src = work.path() / "src";
        fs::copy_tree(project.source_dir(), src);
        for (const auto& [path, new_text] : renamed)
            fs::write_text(src / std::filesystem::relative(path, project.source_dir()), new_text);
        after = counts(*project.with_source_dir(src));
    }
    const std::size_t identical_before = std::ranges::count_if(before, [](const auto& kv) { return kv.second == kIdentical; });
    const std::size_t identical_after = std::ranges::count_if(after, [](const auto& kv) { return kv.second == kIdentical; });
    out("identical before: {}, after the renaming: {}\n", identical_before, identical_after);
    if (identical_before > identical_after) {
        out("not kept: the renaming loses {}\n", identical_before - identical_after);
        return kExitFailed;
    }
    if (options.write) {
        for (const auto& [path, new_text] : renamed) fs::write_text(path, new_text);
        out("written: {} files\n", renamed.size());
    }
    return kExitOk;
}

}  // namespace cb::exact::readable
