// codebreaker/java/projects/minecraft/keybind_mark.cpp -- C++ port of codebreaker.java.projects.minecraft.keybind_mark
// (see the header).

#include "codebreaker/java/projects/minecraft/keybind_mark.hpp"

namespace codebreaker::java::projects::minecraft::keybind_mark {

const py::dict<py::str, py::str> KEY_ACTIONS = {
    {py::str("F1"), py::str("hide GUI / HUD")},
    {py::str("F2"), py::str("take screenshot")},
    {py::str("F3"), py::str("toggle debug overlay (F3 panel)")},
    {py::str("F5"), py::str("toggle third-person view")},
    {py::str("F7"), py::str("debug / profiler timing")},
    {py::str("F8"), py::str("toggle smooth (cinematic) camera")},
    {py::str("F11"), py::str("toggle fullscreen")},
    {py::str("ESCAPE"), py::str("pause / open in-game menu")},
    {py::str("S"), py::str("reload chunks (with F3 held)")},
    {py::str("LSHIFT"), py::str("sneak")},
    {py::str("RSHIFT"), py::str("sneak")},
    {py::str("LCONTROL"), py::str("sprint")},
    {py::str("RCONTROL"), py::str("sprint")},
    {py::str("TAB"), py::str("player list")},
    {py::str("1"), py::str("hotbar slot 1")},
    {py::str("2"), py::str("hotbar slot 2")},
    {py::str("3"), py::str("hotbar slot 3")},
    {py::str("4"), py::str("hotbar slot 4")},
    {py::str("5"), py::str("hotbar slot 5")},
    {py::str("6"), py::str("hotbar slot 6")},
    {py::str("7"), py::str("hotbar slot 7")},
    {py::str("8"), py::str("hotbar slot 8")},
    {py::str("9"), py::str("hotbar slot 9")},
};

const py::re::Pattern _KEY_SITE = py::re::compile(py::str(
    R"(Keyboard::(?:getEventKey\s*\(\s*\)\s*==\s*Keyboard::KEY_(\w+)|isKeyDown\s*\(\s*Keyboard::KEY_(\w+)))"));
const py::str _MARKER = "J2X-KEYBIND[";

py::list<py::str> _keys_on_line(const py::str& line) {
    py::list<py::str> keys;
    for (const py::re::Match& m : _KEY_SITE.finditer(line)) {
        // m.group(1) or m.group(2): exactly one of the two alternatives took part (and \w+ is not empty)
        std::optional<py::str> first = m.group_opt(1);
        keys.append(first ? *first : m.group(2));
    }
    return keys;
}

py::str _action(const py::str& key) {
    return KEY_ACTIONS.get(key, py::str("(unknown action -- inspect the branch)"));
}

Result mark(const py::str& src_dir, bool dry_run) {
    py::list<Site> sites;
    int64_t skipped = 0;
    for (const auto& walked : py::os::walk(src_dir)) {
        const py::str& dp = std::get<0>(walked);
        for (const py::str& f : py::sorted(std::get<2>(walked))) {
            if (!f.endswith(py::str(".cpp"))) {
                continue;
            }
            py::str path = py::os::path::join(dp, f);
            py::list<py::str> lines;
            try {
                lines = py::split(py::open(path, py::str("r"), py::str("utf-8"), py::str("replace")).read_text(), py::str("\n"));
            } catch (const py::OSError&) {                 // (IOError, OSError)
                continue;
            }
            bool changed = false;
            for (int64_t i = 0; i < py::len(lines); ++i) {
                const py::str line = lines[i];
                py::list<py::str> keys = _keys_on_line(line);
                if (!py::truthy(keys)) {
                    continue;
                }
                py::list<py::str> parts;
                for (const py::str& k : keys) {
                    parts.append(py::percent(py::str("KEY_%s=%s"), k, _action(k)));
                }
                py::str label = py::join(py::str(" "), parts);
                sites.append({f, i + 1, keys, label});
                if (line.contains(_MARKER)) {
                    skipped += 1;
                    continue;
                }
                if (!dry_run) {
                    lines[i] = line.rstrip() +
                               py::percent(py::str("   // J2X-KEYBIND[%s | keyboard is DEAD on console -- ROUTE TO: controller"
                                                   " button OR menu toggle]"),
                                           label);
                    changed = true;
                }
            }
            if (changed && !dry_run) {
                py::file out = py::open(path, py::str("w"), py::str("utf-8"));
                out.write(py::join(py::str("\n"), lines));
                out.close();
            }
        }
    }
    Result r;
    r.sites = sites;
    r.skipped = skipped;
    return r;
}

py::str render_index(const Result& result) {
    const py::list<Site>& sites = result.sites;
    // unique keys across all sites
    py::list<py::str> keys;
    for (const Site& s : sites) {
        for (const py::str& k : std::get<2>(s)) {
            if (!keys.contains(k)) {
                keys.append(k);
            }
        }
    }
    py::list<py::str> L = {
        py::str("# KEYBIND_INDEX.md -- every hardcoded keyboard action (route to controller OR menu)"),
        py::str(""),
        py::str("Minecraft binds these actions to physical keys. The console has NO keyboard, so each site is DEAD "
                "until re-routed. Every site carries a greppable `// J2X-KEYBIND[...]` marker "
                "(`grep -rn \"J2X-KEYBIND\" .`). Decide, per key, whether it becomes a **controller button** or a "
                "**menu toggle** (a GameSettings option, like Show FPS -> the debug panel), then wire it and clear "
                "the marker."),
        py::str(""),
        py::percent(py::str("- key-press sites: **%d**   distinct keys: **%d**"), py::len(sites), py::len(keys)),
        py::str(""),
        py::str("| Key | Action | Site | Route decision |"),
        py::str("|---|---|---|---|"),
    };
    auto by_site = [](const Site& s) { return std::make_tuple(std::get<0>(s), std::get<1>(s)); };
    for (const Site& s : py::sorted(sites, by_site)) {
        const py::str& f = std::get<0>(s);
        int64_t ln = std::get<1>(s);
        const py::list<py::str>& ks = std::get<2>(s);
        py::list<py::str> descs, actions;
        for (const py::str& k : ks) {
            descs.append(py::percent(py::str("KEY_%s"), k));
            actions.append(_action(k));
        }
        py::str keydesc = py::join(py::str(", "), descs);
        py::str action = py::join(py::str(" + "), actions);
        L.append(py::percent(py::str("| `%s` | %s | `%s:%d` | _controller button / menu toggle_ |"), keydesc, action, f, ln));
    }
    L.append(py::str(""));
    return py::join(py::str("\n"), L) + py::str("\n");
}

Result run(const py::str& src_dir, bool dry_run, const std::optional<py::str>& index_dir) {
    Result result = mark(src_dir, dry_run);
    std::optional<py::str> index_path;
    if (!dry_run) {
        index_path = py::os::path::join(py::truthy(index_dir) ? *index_dir : src_dir, py::str("KEYBIND_INDEX.md"));
        py::file f = py::open(*index_path, py::str("w"), py::str("utf-8"));
        f.write(render_index(result));
        f.close();
    }
    result.index = index_path;
    return result;
}

}  // namespace codebreaker::java::projects::minecraft::keybind_mark
