#include "cb/java/projects/minecraft/ui_mark.hpp"

#include <algorithm>
#include <array>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/java/textutil.hpp"
#include "cb/java/tree.hpp"

namespace cb::java::projects::minecraft::ui_mark {

namespace stdfs = std::filesystem;

namespace {

// UI base classes: a class extending one of these is a UI class even if its name doesn't match.
constexpr auto kUiBases = std::to_array<std::string_view>({
    "Gui", "GuiScreen", "GuiButton", "GuiContainer", "GuiSlot", "GuiTextField", "GuiSmallButton", "GuiHandler",
    "Screen", "GuiComponent",
});

bool is_ui_base(const std::string& name) { return std::find(kUiBases.begin(), kUiBases.end(), name) != kUiBases.end(); }

// `class Name {` or `class Name : bases... {` -- group 1 the name, group 2 the base clause (up to the brace).
const Regex& class_decl() {
    static const Regex re(R"(\bclass\s+([A-Za-z_](?:\w|[^\x00-\x7f])*)\s*(?::\s*([^\{]+))?\{)", RegexFlags::DotAll);
    return re;
}

// An identifier character: an ASCII letter, digit or underscore, or any byte of a non-ASCII character (a class named
// with a letter beyond ASCII is one name).
bool is_word(char c) {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || (static_cast<unsigned char>(c) >= 0x80);
}

bool is_ident_start(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }

// The identifiers of a base clause ("public Gui, private Foo<Bar>" -> Gui, Foo, Bar), without the access keywords.
// A run of word characters that does not start with a letter or underscore is not an identifier.
std::set<std::string> base_names(std::string_view bases) {
    std::set<std::string> out;
    std::size_t i = 0;
    while (i < bases.size()) {
        if (!is_word(bases[i])) {
            ++i;
            continue;
        }
        std::size_t end = i;
        while (end < bases.size() && is_word(bases[end])) ++end;
        const std::string_view word = bases.substr(i, end - i);
        if (is_ident_start(word[0]) && word != "public" && word != "protected" && word != "private") {
            out.emplace(word);
        }
        i = end;
    }
    return out;
}

// A method definition on a line: `Class::method(` where `Class` is a UI class that begins a token (not `A::Class::`
// and not the tail of a longer name) and `method` may be a destructor.
struct Definition {
    std::string cls;
    std::string method;
};

std::optional<Definition> definition_on_line(std::string_view line, const std::set<std::string>& ui) {
    std::optional<Definition> best;
    std::size_t pos = 0;
    while ((pos = line.find("::", pos)) != std::string_view::npos) {
        const std::size_t colons = pos;
        pos += 2;
        std::size_t begin = colons;
        while (begin > 0 && is_word(line[begin - 1])) --begin;
        if (begin == colons || (begin > 0 && line[begin - 1] == ':')) continue;
        std::size_t m = colons + 2;
        const std::size_t name_begin = m < line.size() && line[m] == '~' ? m + 1 : m;
        if (name_begin >= line.size()) continue;
        if (!is_ident_start(line[name_begin])) continue;
        std::size_t name_end = name_begin;
        while (name_end < line.size() && is_word(line[name_end])) ++name_end;
        std::size_t paren = name_end;
        while (paren < line.size() && text::is_space(line[paren])) ++paren;
        if (paren >= line.size() || line[paren] != '(') continue;
        const auto it = ui.find(std::string(line.substr(begin, colons - begin)));
        if (it == ui.end()) continue;
        // among the UI classes defined on this line, the first by name wins (then the leftmost)
        if (!best || *it < best->cls) best = Definition{*it, std::string(line.substr(m, name_end - m))};
    }
    return best;
}

std::vector<std::string> split_lines(std::string_view source) {
    std::vector<std::string> lines;
    for (const std::string_view l : text::split(source, '\n')) lines.emplace_back(l);   // keeps the final piece
    return lines;
}

}  // namespace

bool is_ui_class_name(std::string_view name) {
    return name.starts_with("Gui") || name.ends_with("Screen") || name.ends_with("Menu");
}

std::set<std::string> discover_ui_classes(const stdfs::path& src_dir, const std::vector<std::string>& extra) {
    std::set<std::string> ui(extra.begin(), extra.end());
    std::map<std::string, std::set<std::string>> bases_by_class;   // a class declared twice keeps its last bases
    for (const stdfs::path& header : tree::files_with_extension(src_dir, {".h", ".hpp"})) {
        const std::optional<std::string> source = textutil::try_read_text(header);
        if (!source) continue;
        class_decl().for_each_match(*source, [&](const RegexMatch& m) {
            bases_by_class[std::string(m[1])] = base_names(m.has(2) ? m[2] : std::string_view());
            return true;
        });
    }
    // seed from the name pattern
    for (const auto& entry : bases_by_class) {
        if (is_ui_class_name(entry.first)) ui.insert(entry.first);
    }
    // the closure over inheritance into a UI base or an already-UI class
    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& [cls, bases] : bases_by_class) {
            if (ui.contains(cls)) continue;
            const bool extends_ui = std::any_of(bases.begin(), bases.end(),
                                                [&](const std::string& b) { return is_ui_base(b) || ui.contains(b); });
            if (extends_ui) {
                ui.insert(cls);
                changed = true;
            }
        }
    }
    return ui;
}

Result mark(const stdfs::path& src_dir, bool dry_run, const std::vector<std::string>& extra_classes) {
    Result result;
    result.classes = discover_ui_classes(src_dir, extra_classes);
    for (const stdfs::path& path : tree::files_with_extension(src_dir, {".cpp"})) {
        const std::optional<std::string> source = textutil::try_read_text(path);
        if (!source) continue;
        const std::string file = fs::to_text(path.filename());
        std::vector<std::string> lines = split_lines(*source);
        bool changed = false;
        for (std::size_t i = 0; i < lines.size(); ++i) {
            const std::string_view line = lines[i];
            // only definition-ish lines: contains '::method(' and opens a body (has '{' now or is a signature
            // continued onto the next line) -- skip pure prototypes and calls
            if (line.find("::") == std::string_view::npos || text::trim(line).ends_with(';')) continue;
            const std::optional<Definition> def = definition_on_line(line, result.classes);
            if (!def) continue;
            // it must look like a definition: this line or a later one has '{' before any ';'
            std::string tail;
            for (std::size_t k = i; k < std::min(lines.size(), i + 4); ++k) {
                if (k > i) tail += '\n';
                tail += lines[k];
            }
            const std::size_t brace = tail.find('{');
            const std::size_t semi = tail.find(';');
            if (brace == std::string::npos || (semi != std::string::npos && semi < brace)) continue;
            result.marked[def->cls].push_back({file, i + 1, def->method});
            if (line.find("J2X-UI[") != std::string_view::npos) {
                ++result.skipped;
                continue;
            }
            changed = true;
            if (!dry_run) lines[i] = std::string(text::rtrim(line)) + std::format("  // J2X-UI[{}::{}]", def->cls, def->method);
        }
        if (changed && !dry_run) textutil::write_text(path, text::join(lines, "\n"));
    }
    return result;
}

std::string render_index(const Result& result) {
    std::size_t total = 0;
    for (const auto& entry : result.marked) total += entry.second.size();
    std::vector<std::string> L = {
        "# UI_INDEX.md -- catalogue of every UI function",
        "",
        "Every UI method carries a greppable `// J2X-UI[Class::method]` marker; `grep -rn \"J2X-UI\" .` lists them all. "
        "Regenerate after each UI merge.",
        "",
        std::format("- UI classes: **{}**   UI methods marked: **{}**", result.classes.size(), total),
        "",
    };
    for (const auto& [cls, marks] : result.marked) {
        L.push_back(std::format("## {} ({})", cls, marks.size()));
        L.push_back("");
        L.push_back("| Method | Location |");
        L.push_back("|---|---|");
        std::vector<Mark> sorted = marks;
        std::sort(sorted.begin(), sorted.end());
        for (const Mark& mk : sorted) L.push_back(std::format("| `{}::{}` | `{}:{}` |", cls, mk.method, mk.file, mk.line));
        L.push_back("");
    }
    std::vector<std::string> unmarked;
    for (const std::string& c : result.classes) {
        if (!result.marked.contains(c)) unmarked.push_back("`" + c + "`");
    }
    if (!unmarked.empty()) {
        L.push_back("## UI classes with no marked methods (header-only or all inline)");
        L.push_back("");
        L.push_back(text::join(unmarked, ", "));
        L.push_back("");
    }
    return text::join(L, "\n") + "\n";
}

Result run(const stdfs::path& src_dir, bool dry_run, const std::vector<std::string>& extra_classes,
           const std::optional<stdfs::path>& index_dir) {
    Result result = mark(src_dir, dry_run, extra_classes);
    if (!dry_run) {
        std::error_code ec;
        if (!index_dir && !stdfs::is_directory(src_dir, ec)) throw NotFoundError(src_dir, 0, "the source folder does not exist");
        const stdfs::path index = (index_dir ? *index_dir : src_dir) / "UI_INDEX.md";
        textutil::write_text(index, render_index(result));
        result.index = index;
    }
    return result;
}

}  // namespace cb::java::projects::minecraft::ui_mark
