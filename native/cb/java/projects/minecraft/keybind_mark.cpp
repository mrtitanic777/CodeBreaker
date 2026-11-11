#include "cb/java/projects/minecraft/keybind_mark.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/java/textutil.hpp"
#include "cb/java/tree.hpp"

namespace cb::java::projects::minecraft::keybind_mark {

namespace stdfs = std::filesystem;

namespace {

struct KeyAction {
    std::string_view key;
    std::string_view action;
};

// Hardcoded, non-rebindable keys and what they do in Beta. Looked up by key, never iterated into output.
constexpr auto kKeyActions = std::to_array<KeyAction>({
    {"F1", "hide GUI / HUD"},
    {"F2", "take screenshot"},
    {"F3", "toggle debug overlay (F3 panel)"},
    {"F5", "toggle third-person view"},
    {"F7", "debug / profiler timing"},
    {"F8", "toggle smooth (cinematic) camera"},
    {"F11", "toggle fullscreen"},
    {"ESCAPE", "pause / open in-game menu"},
    {"S", "reload chunks (with F3 held)"},
    {"LSHIFT", "sneak"},
    {"RSHIFT", "sneak"},
    {"LCONTROL", "sprint"},
    {"RCONTROL", "sprint"},
    {"TAB", "player list"},
    {"1", "hotbar slot 1"},
    {"2", "hotbar slot 2"},
    {"3", "hotbar slot 3"},
    {"4", "hotbar slot 4"},
    {"5", "hotbar slot 5"},
    {"6", "hotbar slot 6"},
    {"7", "hotbar slot 7"},
    {"8", "hotbar slot 8"},
    {"9", "hotbar slot 9"},
});

// `Keyboard::getEventKey() == Keyboard::KEY_x`  or  `Keyboard::isKeyDown(Keyboard::KEY_x)`
const Regex& key_site() {
    static const Regex re(
        R"(Keyboard::(?:getEventKey\s*\(\s*\)\s*==\s*Keyboard::KEY_((?:\w|[^\x00-\x7f])+))"
        R"(|isKeyDown\s*\(\s*Keyboard::KEY_((?:\w|[^\x00-\x7f])+)))");
    return re;
}

constexpr std::string_view kMarker = "J2X-KEYBIND[";

std::string label_of(const std::vector<std::string>& keys) {
    std::vector<std::string> parts;
    for (const std::string& k : keys) parts.push_back(std::format("KEY_{}={}", k, action_of(k)));
    return text::join(parts, " ");
}

}  // namespace

std::string_view action_of(std::string_view key) {
    for (const KeyAction& ka : kKeyActions) {
        if (ka.key == key) return ka.action;
    }
    return "(unknown action -- inspect the branch)";
}

std::vector<std::string> keys_on_line(std::string_view line) {
    std::vector<std::string> keys;
    key_site().for_each_match(line, [&](const RegexMatch& m) {
        // exactly one of the two alternatives took part
        keys.emplace_back(m.has(1) ? m[1] : m[2]);
        return true;
    });
    return keys;
}

Result mark(const stdfs::path& src_dir, bool dry_run) {
    Result result;
    for (const stdfs::path& path : tree::files_with_extension(src_dir, {".cpp"})) {
        const std::optional<std::string> source = textutil::try_read_text(path);
        if (!source) continue;
        const std::string file = fs::to_text(path.filename());
        std::vector<std::string> lines;
        for (const std::string_view l : text::split(*source, '\n')) lines.emplace_back(l);   // keeps the final piece
        bool changed = false;
        for (std::size_t i = 0; i < lines.size(); ++i) {
            std::vector<std::string> keys = keys_on_line(lines[i]);
            if (keys.empty()) continue;
            std::string label = label_of(keys);
            result.sites.push_back({file, i + 1, std::move(keys), label});
            if (lines[i].find(kMarker) != std::string::npos) {
                ++result.skipped;
                continue;
            }
            if (!dry_run) {
                lines[i] = std::string(text::rtrim(lines[i])) + "   // J2X-KEYBIND[" + label +
                           " | keyboard is DEAD on console -- ROUTE TO: controller button OR menu toggle]";
                changed = true;
            }
        }
        if (changed) textutil::write_text(path, text::join(lines, "\n"));
    }
    return result;
}

std::string render_index(const Result& result) {
    std::set<std::string> distinct_keys;
    for (const Site& s : result.sites) distinct_keys.insert(s.keys.begin(), s.keys.end());

    std::vector<std::string> L = {
        "# KEYBIND_INDEX.md -- every hardcoded keyboard action (route to controller OR menu)",
        "",
        "Minecraft binds these actions to physical keys. The console has NO keyboard, so each site is DEAD until "
        "re-routed. Every site carries a greppable `// J2X-KEYBIND[...]` marker (`grep -rn \"J2X-KEYBIND\" .`). "
        "Decide, per key, whether it becomes a **controller button** or a **menu toggle** (a GameSettings option, "
        "like Show FPS -> the debug panel), then wire it and clear the marker.",
        "",
        std::format("- key-press sites: **{}**   distinct keys: **{}**", result.sites.size(), distinct_keys.size()),
        "",
        "| Key | Action | Site | Route decision |",
        "|---|---|---|---|",
    };
    std::vector<const Site*> ordered;
    for (const Site& s : result.sites) ordered.push_back(&s);
    std::stable_sort(ordered.begin(), ordered.end(), [](const Site* a, const Site* b) {
        return a->file != b->file ? a->file < b->file : a->line < b->line;
    });
    for (const Site* s : ordered) {
        std::vector<std::string> descs, actions;
        for (const std::string& k : s->keys) {
            descs.push_back("KEY_" + k);
            actions.emplace_back(action_of(k));
        }
        L.push_back(std::format("| `{}` | {} | `{}:{}` | _controller button / menu toggle_ |", text::join(descs, ", "),
                                text::join(actions, " + "), s->file, s->line));
    }
    L.push_back("");
    return text::join(L, "\n") + "\n";
}

Result run(const stdfs::path& src_dir, bool dry_run, const std::optional<stdfs::path>& index_dir) {
    Result result = mark(src_dir, dry_run);
    if (!dry_run) {
        std::error_code ec;
        if (!index_dir && !stdfs::is_directory(src_dir, ec)) throw NotFoundError(src_dir, 0, "the source folder does not exist");
        const stdfs::path index = (index_dir ? *index_dir : src_dir) / "KEYBIND_INDEX.md";
        textutil::write_text(index, render_index(result));
        result.index = index;
    }
    return result;
}

}  // namespace cb::java::projects::minecraft::keybind_mark
