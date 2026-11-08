// codebreaker/java/projects/minecraft/content.cpp -- C++ port of codebreaker.java.projects.minecraft.content.
//
// content.py -- scan the Java corpus for the ASSETS the game loads at runtime (textures, sounds,
// music, language files, misc resources) and emit `Content.md`: a complete manifest of what has to
// ship in the content folder next to the compiled XEX, and where each file goes.
//
// The port resolves a Java resource path like `"/gui/gui.png"` to `game:\Content\gui\gui.png` (see
// FileIO / Class::getResourceAsStream). Sound events like `"random.click"` are XACT/ogg assets under
// `game:\Content\sound\`. This scanner finds every such reference so nothing is silently missing at
// runtime (a missing texture -> NULL image -> the loader crashes; a missing sound -> silent).

#include "codebreaker/java/projects/minecraft/content.hpp"

namespace codebreaker::java::projects::minecraft::content {

// A Java resource path literal: "/dir/name.ext" for a shipped file.
static const py::re::Pattern& RES_PATH_() {
    static const py::re::Pattern p =
        py::re::compile(py::str(R"RX("(/[\w./-]+\.(?:png|jpg|jpeg|txt|lang|properties|fsh|vsh))")RX"));
    return p;
}
// A sound/music EVENT name (dotted, lowercase) used at a sound call site.
static const py::re::Pattern& SOUND_CALL_() {
    static const py::re::Pattern p = py::re::compile(
        py::str(R"RX((?:playSound(?:FX|AtEntity|Effect)?|playStreaming|playNoteBlock))RX"
                R"RX(\s*\([^;]*?"([a-z][a-z0-9_]*(?:\.[a-z0-9_]+)+)")RX"));
    return p;
}
// StepSound("stone", ...) -> block step/dig/... sound family keyed by the first arg.
static const py::re::Pattern& STEP_SOUND_() {
    static const py::re::Pattern p = py::re::compile(py::str(R"RX(\bnew\s+StepSound\s*\(\s*"([a-z][a-z0-9_]*)")RX"));
    return p;
}

// path prefix -> asset category (for grouping in the manifest)
static const py::list<std::tuple<py::str, py::str>>& CATEGORY_() {
    static const py::list<std::tuple<py::str, py::str>> c = {
        {"/lang/", "Language files"}, {"/font", "Font"},
        {"/gui/", "GUI textures"}, {"/title/", "Title / menu textures"},
        {"/terrain", "Terrain atlas"}, {"/item", "Item textures"},
        {"/mob/", "Mob textures"}, {"/armor/", "Armor textures"},
        {"/environment/", "Environment (sky/weather)"}, {"/art/", "Painting art"},
        {"/achievement/", "Achievement textures"}, {"/misc/", "Misc textures"},
    };
    return c;
}

py::str _category_of(const py::str& path) {
    for (const auto& [prefix, name] : CATEGORY_()) {
        if (path.startswith(prefix)) {
            return name;
        }
    }
    return py::str("Other resources");
}

// {key: sorted(values)} of a dict of sets
static py::dict<py::str, py::list<py::str>> sort_(const py::dict<py::str, py::set<py::str>>& d) {
    py::dict<py::str, py::list<py::str>> out;
    for (const auto& [k, v] : d.items()) {
        out.set(k, py::sorted(v));
    }
    return out;
}

// Return the assets across the corpus: resources, sounds and step_sounds, each {name: sorted[classes]}.
Assets scan_assets(const py::str& java_dir) {
    py::dict<py::str, py::set<py::str>> resources, sounds, steps;
    for (const auto& [dp, _dirs, files] : py::os::walk(java_dir)) {
        for (const py::str& f : files) {
            if (!f.endswith(py::str(".java"))) {
                continue;
            }
            py::str cls = f.slice(0, f.size() - 5);
            py::str text;
            try {
                text = py::open(py::os::path::join(dp, f), py::str("r"), py::str("utf-8"), py::str("replace")).read_text();
            } catch (const py::OSError&) {
                continue;
            }
            for (const py::re::Match& m : RES_PATH_().finditer(text)) {
                resources.setdefault(m.group(1), py::set<py::str>()).add(cls);
            }
            for (const py::re::Match& m : SOUND_CALL_().finditer(text)) {
                sounds.setdefault(m.group(1), py::set<py::str>()).add(cls);
            }
            for (const py::re::Match& m : STEP_SOUND_().finditer(text)) {
                steps.setdefault(m.group(1), py::set<py::str>()).add(cls);
            }
        }
    }
    return Assets{sort_(resources), sort_(sounds), sort_(steps)};
}

// ", ".join(first n of users) plus " …" when there are more than n
static py::str users_(const py::list<py::str>& users, int64_t n) {
    return py::join(py::str(", "), users.slice(0, n)) + (users.size() > n ? " …" : "");
}

py::str render_content_md(const py::str& java_dir, const py::str& version_label) {
    Assets a = scan_assets(java_dir);
    const py::dict<py::str, py::list<py::str>>& res = a.resources;
    const py::dict<py::str, py::list<py::str>>& snd = a.sounds;
    const py::dict<py::str, py::list<py::str>>& steps = a.step_sounds;

    // group resources by category
    py::dict<py::str, py::list<py::str>> by_cat;
    for (const py::str& path : py::sorted(py::list<py::str>(res.keys()))) {
        by_cat.setdefault(_category_of(path), py::list<py::str>()).append(path);
    }

    py::list<py::str> L = {
        py::percent("# Content.md — asset manifest for %s", version_label), "",
        "Everything the converted game loads at runtime, and where it must live in the content",
        "folder shipped next to the compiled `default.xex`. A Java resource path `\"/gui/gui.png\"`",
        "resolves to `game:\\Content\\gui\\gui.png`; sound events resolve under `game:\\Content\\sound\\`.",
        "Anything missing here fails at runtime (a missing texture becomes a NULL image and crashes",
        "the loader; a missing sound is silent).", "",
        py::percent("> Generated by CodeBreaker from the Java corpus. Counts: %d resource files, %d sound events, %d step-sound families.",
                    py::len(res), py::len(snd), py::len(steps)),
        "",
        "## Layout", "",
        "```", "Content\\               <- next to default.xex (game:\\Content\\)",
        "  gui\\  title\\  item\\  mob\\  armor\\  environment\\  art\\  misc\\  terrain.png ...",
        "  lang\\   en_US.lang, stats_US.lang", "  font.txt",
        "  sound\\  <domain>\\<name>.ogg   (or XACT wave banks)", "```", ""};

    L.extend(py::list<py::str>{"## Textures & resource files", ""});
    for (const py::str& cat : py::sorted(py::list<py::str>(by_cat.keys()))) {
        L.append(py::percent("### %s", cat));
        L.append("");
        L.append("| Content path | Referenced by |");
        L.append("|---|---|");
        for (const py::str& path : by_cat.at(cat)) {
            py::str dest = py::percent("Content%s", path.replace(py::str("/"), py::str("\\")));
            py::str users = users_(res.at(path), 4);
            L.append(py::percent("| `%s` | %s |", dest, users));
        }
        L.append("");
    }

    L.extend(py::list<py::str>{
        "## Sound events", "",
        "Each event maps to one or more `.ogg` files (Minecraft appends a number: `random.click`",
        "-> `sound/random/click1.ogg`, `click2.ogg`, …) or an XACT cue of the same name.", "",
        "| Sound event | Suggested file(s) | Referenced by |", "|---|---|---|"});
    for (const py::str& ev : py::sorted(py::list<py::str>(snd.keys()))) {
        py::list<py::str> parts = py::split(ev, py::str("."));
        py::str fpath = py::percent("sound\\%s\\%s.ogg", py::join(py::str("\\"), parts.slice(0, -1)), parts[-1]);
        py::str users = users_(snd.at(ev), 3);
        L.append(py::percent("| `%s` | `Content\\%s` | %s |", ev, fpath, users));
    }
    L.append("");

    if (py::truthy(steps)) {
        L.extend(py::list<py::str>{
            "## Block step / dig sound families", "",
            "A `StepSound(\"<name>\", …)` drives the walk/dig/place sounds for a block; each name",
            "expands to `step.<name>`, `dig.<name>` events (ogg or XACT).", "",
            "| Family | Events | Blocks |", "|---|---|---|"});
        for (const py::str& nm : py::sorted(py::list<py::str>(steps.keys()))) {
            py::str users = users_(steps.at(nm), 4);
            L.append(py::percent("| `%s` | `step.%s`, `dig.%s` | %s |", nm, nm, nm, users));
        }
        L.append("");
    }

    L.extend(py::list<py::str>{
        "## Music", "",
        "Background/menu music streams via `playStreaming`. Provide the streamed tracks under",
        "`Content\\music\\` (or an XACT streaming bank) and the title/menu loop.", ""});
    return py::join(py::str("\n"), L) + "\n";
}

py::str write_content_md(const py::str& java_dir, const py::str& out_dir, const py::str& version_label) {
    py::str path = py::os::path::join(out_dir, py::str("Content.md"));
    py::file f = py::open(path, py::str("w"), py::str("utf-8"));
    f.write(render_content_md(java_dir, version_label));
    f.close();
    return path;
}

}  // namespace codebreaker::java::projects::minecraft::content
