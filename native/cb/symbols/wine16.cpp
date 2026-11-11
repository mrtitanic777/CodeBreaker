// cb/symbols/wine16.cpp -- Wine's 16-bit .spec files (see wine16.hpp).
#include "cb/symbols/wine16.hpp"

#include <algorithm>
#include <utility>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::symbols::wine16 {
namespace {

using std::string;
using std::string_view;

struct KindName {
    string_view spelling;
    ArgKind kind;
};
constexpr KindName kKinds[] = {{"word", ArgKind::Word},     {"s_word", ArgKind::SWord},   {"long", ArgKind::Long}, {"segptr", ArgKind::SegPtr},
                               {"segstr", ArgKind::SegStr}, {"ptr", ArgKind::Ptr},        {"str", ArgKind::Str}};

std::optional<ArgKind> kind_of(string_view word) {
    for (const KindName& k : kKinds) {
        if (k.spelling == word) return k.kind;
    }
    return std::nullopt;
}

// A name is letters, digits and underscores; the bytes of a letter that is not ASCII (the text was Latin-1, now UTF-8) are
// letters too.
bool is_word_char(char c) {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || static_cast<unsigned char>(c) >= 0x80;
}
bool is_digit(char c) { return c >= '0' && c <= '9'; }

// The argument kinds a parenthesised list says; nullopt when it holds a kind this reader does not know (double, int64 ...).
std::optional<std::vector<ArgKind>> kinds_of(string_view list) {
    std::vector<ArgKind> out;
    for (const string_view word : text::split_ws(list)) {
        const auto k = kind_of(word);
        if (!k) return std::nullopt;
        out.push_back(*k);
    }
    return out;
}

// The text of a `(...)` that starts at `at` (after any white space) and where it ends, or nothing when there is none.
std::optional<string_view> parenthesised(string_view line, std::size_t& at) {
    std::size_t p = at;
    while (p < line.size() && text::is_space(line[p])) ++p;
    if (p >= line.size() || line[p] != '(') return std::nullopt;
    const std::size_t close = line.find(')', p + 1);
    if (close == string_view::npos) return std::nullopt;
    at = close + 1;
    return line.substr(p + 1, close - p - 1);
}

// One line of a .spec file: `ordinal convention [-flag[=value]]... name [(args)] [# (args)]`, read from the start of the line;
// whatever follows is not looked at.
std::optional<std::pair<std::uint32_t, Export>> read_line(string_view line) {
    std::size_t p = 0;
    while (p < line.size() && text::is_space(line[p])) ++p;
    const std::size_t digits = p;
    while (p < line.size() && is_digit(line[p])) ++p;
    if (p == digits) return std::nullopt;
    const auto ordinal = text::parse_int<std::uint32_t>(line.substr(digits, p - digits));
    if (!ordinal) return std::nullopt;
    const std::size_t gap = p;
    while (p < line.size() && text::is_space(line[p])) ++p;
    if (p == gap) return std::nullopt;

    const std::size_t word = p;
    while (p < line.size() && !text::is_space(line[p])) ++p;
    const string_view convention = line.substr(word, p - word);
    Export e;
    bool stub = false;
    if (convention == "pascal") e.convention = Convention::Pascal;
    else if (convention == "cdecl") e.convention = Convention::Cdecl;
    else if (convention == "varargs") e.convention = Convention::Varargs;
    else if (convention == "stdcall") e.convention = Convention::Stdcall;
    else if (convention == "stub") stub = true;
    else return std::nullopt;
    const std::size_t gap2 = p;
    while (p < line.size() && text::is_space(line[p])) ++p;
    if (p == gap2) return std::nullopt;

    // the flags: `-name` or `-name=value`, each followed by white space
    const std::size_t flags_at = p;
    while (p < line.size() && line[p] == '-') {
        std::size_t q = p + 1;
        const std::size_t flag = q;
        while (q < line.size() && is_word_char(line[q])) ++q;
        if (q == flag) return std::nullopt;
        if (q < line.size() && line[q] == '=') {
            const std::size_t value = ++q;
            while (q < line.size() && !text::is_space(line[q])) ++q;
            if (q == value) return std::nullopt;
        }
        const std::size_t gap3 = q;
        while (q < line.size() && text::is_space(line[q])) ++q;
        if (q == gap3) return std::nullopt;
        p = q;
    }
    e.ret16 = line.substr(flags_at, p - flags_at).find("-ret16") != string_view::npos;

    const std::size_t name = p;
    while (p < line.size() && is_word_char(line[p])) ++p;
    if (p == name) return std::nullopt;
    e.name = string(line.substr(name, p - name));
    e.stub = stub;

    std::optional<string_view> listed = parenthesised(line, p);   // `name(args)`
    std::optional<string_view> commented;
    {
        std::size_t q = p;
        while (q < line.size() && text::is_space(line[q])) ++q;
        if (q < line.size() && line[q] == '#') {   // `# (args)`: a stub's arguments, written as a comment
            ++q;
            commented = parenthesised(line, q);
        }
    }
    const std::optional<string_view>& said = stub ? commented : listed;
    if (said) e.args = kinds_of(*said);
    return std::pair{*ordinal, std::move(e)};
}

// The module a .spec file is for: its name up to the first dot, in capitals; KRNL386 is KERNEL.
string module_of(string_view spec_name) {
    string stem = text::upper(spec_name.substr(0, spec_name.find('.')));
    return stem == "KRNL386" ? "KERNEL" : stem;
}

bool is_wine16_folder(string_view name) {
    for (const string_view suffix : {string_view(".dll16"), string_view(".exe16"), string_view(".drv16")}) {
        if (name.ends_with(suffix)) return true;
    }
    return false;
}

// The entries of a folder by name, the way the file system lists them (case-insensitively), so that what a later file says
// replaces what an earlier one did the same way on every machine.
std::vector<std::filesystem::directory_entry> entries_by_name(const std::filesystem::path& dir) {
    std::vector<std::filesystem::directory_entry> out;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) out.push_back(entry);
    std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) {
        const string x = text::lower(fs::to_text(a.path().filename()));
        const string y = text::lower(fs::to_text(b.path().filename()));
        return x < y;
    });
    return out;
}

}  // namespace

std::uint32_t width(ArgKind kind) {
    switch (kind) {
        case ArgKind::Word:
        case ArgKind::SWord: return 2;
        default: return 4;
    }
}

bool is_pointer(ArgKind kind) { return kind == ArgKind::Ptr || kind == ArgKind::Str || kind == ArgKind::SegPtr || kind == ArgKind::SegStr; }

std::string_view c_type(ArgKind kind) {
    if (is_pointer(kind)) return "void __far *";
    if (kind == ArgKind::Word) return "uint2";
    if (kind == ArgKind::SWord) return "int2";
    return "uint4";
}

std::string_view spelling(ArgKind kind) {
    for (const KindName& k : kKinds) {
        if (k.kind == kind) return k.spelling;
    }
    return {};
}

Exports read_spec(std::string_view text) {
    Exports out;
    for (const string_view line : text::split(text, '\n')) {
        if (auto parsed = read_line(line)) out.insert_or_assign(parsed->first, std::move(parsed->second));
    }
    return out;
}

void Specs::load() const {
    std::error_code ec;
    if (folder_.empty() || !std::filesystem::is_directory(folder_, ec)) return;
    for (const auto& dir : entries_by_name(folder_)) {
        if (!dir.is_directory(ec) || !is_wine16_folder(fs::to_text(dir.path().filename()))) continue;
        for (const auto& file : entries_by_name(dir.path())) {
            const string name = fs::to_text(file.path().filename());
            if (!name.ends_with(".spec")) continue;
            Exports exports = read_spec(text::from_latin1(fs::read_file(file.path())));
            Exports& into = modules_[module_of(name)];
            for (auto& [ordinal, e] : exports) into.insert_or_assign(ordinal, std::move(e));
        }
    }
}

const Exports& Specs::exports(std::string_view module) const {
    static const Exports kNone;
    std::call_once(loaded_, [this] { load(); });
    const auto it = modules_.find(text::upper(module));
    return it == modules_.end() ? kNone : it->second;
}

}  // namespace cb::symbols::wine16
