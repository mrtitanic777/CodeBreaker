// codebreaker/java/projects/minecraft/ui_mark.cpp -- C++ port of codebreaker.java.projects.minecraft.ui_mark
// (see the header).

#include "codebreaker/java/projects/minecraft/ui_mark.hpp"

namespace codebreaker::java::projects::minecraft::ui_mark {

const py::set<py::str> UI_BASES = {py::str("Gui"),          py::str("GuiScreen"),     py::str("GuiButton"),
                                   py::str("GuiContainer"), py::str("GuiSlot"),       py::str("GuiTextField"),
                                   py::str("GuiSmallButton"), py::str("GuiHandler"),  py::str("Screen"),
                                   py::str("GuiComponent")};

const py::re::Pattern _CLASS_DECL = py::re::compile(py::str(R"(\bclass\s+([A-Za-z_]\w*)\s*(?::\s*([^\{]+))?\{)"), py::re::DOTALL);
const py::re::Pattern _EXTENDS = py::re::compile(py::str(R"(\b(?:public|protected|private)?\s*([A-Za-z_]\w*))"));

bool is_ui_class_name(const py::str& name) {
    return name.startswith(py::str("Gui")) || name.endswith(py::str("Screen")) || name.endswith(py::str("Menu"));
}

py::set<py::str> discover_ui_classes(const py::str& src_dir, const std::optional<py::list<py::str>>& extra) {
    py::set<py::str> ui;
    if (extra) {
        ui.update(*extra);
    }
    py::dict<py::str, py::set<py::str>> bases_by_class;
    py::set<py::str> names;
    for (const auto& walked : py::os::walk(src_dir)) {
        const py::str& dp = std::get<0>(walked);
        for (const py::str& f : std::get<2>(walked)) {
            if (!(f.endswith(py::str(".h")) || f.endswith(py::str(".hpp")))) {
                continue;
            }
            py::str text;
            try {
                text = py::open(py::os::path::join(dp, f), py::str("r"), py::str("utf-8"), py::str("replace")).read_text();
            } catch (const py::OSError&) {                 // (IOError, OSError)
                continue;
            }
            for (const py::re::Match& m : _CLASS_DECL.finditer(text)) {
                py::str cls = m.group(1);
                std::optional<py::str> g2 = m.group_opt(2);
                py::str bases = g2 ? *g2 : py::str("");
                names.add(cls);
                py::set<py::str> found = py::set<py::str>::from(_EXTENDS.findall(bases));
                bases_by_class.set(cls, found - py::set<py::str>{py::str("public"), py::str("protected"), py::str("private")});
            }
        }
    }
    // seed from name pattern
    for (const py::str& cls : names) {
        if (is_ui_class_name(cls)) {
            ui.add(cls);
        }
    }
    // transitive closure over inheritance into a UI base or an already-UI class
    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& kv : bases_by_class.items()) {
            const py::str& cls = std::get<0>(kv);
            const py::set<py::str>& bases = std::get<1>(kv);
            if (ui.contains(cls)) {
                continue;
            }
            if (!(bases & (UI_BASES | ui)).empty()) {
                ui.add(cls);
                changed = true;
            }
        }
    }
    return ui;
}

py::re::Pattern _def_regex(const py::str& cls) {
    return py::re::compile(py::percent(py::str(R"((^|[^\w:])%s::(~?[A-Za-z_]\w*)\s*\()"), py::re::escape(cls)));
}

Result mark(const py::str& src_dir, bool dry_run, const std::optional<py::list<py::str>>& extra_classes) {
    py::set<py::str> ui = discover_ui_classes(src_dir, extra_classes);
    std::vector<std::pair<py::str, py::re::Pattern>> regexes;
    for (const py::str& c : py::sorted(ui)) {
        regexes.push_back({c, _def_regex(c)});
    }
    py::dict<py::str, py::list<Mark>> marked;
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
                py::str stripped = line.strip();
                // only definition-ish lines: contains '::method(' and opens a body (has '{' now or is
                // a signature continued onto the next line) -- skip pure prototypes/calls.
                if (!line.contains(py::str("::")) || stripped.endswith(py::str(";"))) {
                    continue;
                }
                for (const auto& cr : regexes) {
                    const py::str& cls = cr.first;
                    std::optional<py::re::Match> m = cr.second.search(line);
                    if (!m) {
                        continue;
                    }
                    py::str method = m->group(2);
                    // must look like a definition: this line or a later line has '{' before any ';'
                    py::str tail = py::join(py::str("\n"), lines.slice(i, i + 4));
                    int64_t brace = tail.find(py::str("{"));
                    int64_t semi = tail.find(py::str(";"));
                    if (brace == -1 || (semi != -1 && semi < brace)) {
                        continue;
                    }
                    if (line.contains(py::str("J2X-UI["))) {
                        skipped += 1;
                        marked.setdefault(cls, py::list<Mark>{}).append({f, i + 1, method});
                        break;
                    }
                    if (!dry_run) {
                        lines[i] = line.rstrip() + py::percent(py::str("  // J2X-UI[%s::%s]"), cls, method);
                    }
                    marked.setdefault(cls, py::list<Mark>{}).append({f, i + 1, method});
                    changed = true;
                    break;
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
    r.classes = ui;
    r.marked = marked;
    r.skipped = skipped;
    return r;
}

py::str render_index(const Result& result) {
    const py::dict<py::str, py::list<Mark>>& marked = result.marked;
    int64_t total = 0;
    for (const auto& v : marked.values()) {
        total += py::len(v);
    }
    py::list<py::str> L = {
        py::str("# UI_INDEX.md -- catalogue of every UI function"),
        py::str(""),
        py::str("Every UI method carries a greppable `// J2X-UI[Class::method]` marker; "
                "`grep -rn \"J2X-UI\" .` lists them all. Regenerate after each UI merge."),
        py::str(""),
        py::percent(py::str("- UI classes: **%d**   UI methods marked: **%d**"), py::len(result.classes), total),
        py::str(""),
    };
    for (const py::str& cls : py::sorted(marked)) {
        L.append(py::percent(py::str("## %s (%d)"), cls, py::len(marked.at(cls))));
        L.append(py::str(""));
        L.append(py::str("| Method | Location |"));
        L.append(py::str("|---|---|"));
        for (const Mark& mk : py::sorted(marked.at(cls))) {
            L.append(py::percent(py::str("| `%s::%s` | `%s:%d` |"), cls, std::get<2>(mk), std::get<0>(mk), std::get<1>(mk)));
        }
        L.append(py::str(""));
    }
    py::set<py::str> marked_names = py::set<py::str>::from(marked);
    py::list<py::str> unmarked = py::sorted(result.classes - marked_names);
    if (py::truthy(unmarked)) {
        L.append(py::str("## UI classes with no marked methods (header-only or all inline)"));
        L.append(py::str(""));
        py::list<py::str> names;
        for (const py::str& c : unmarked) {
            names.append(py::percent(py::str("`%s`"), c));
        }
        L.append(py::join(py::str(", "), names));
        L.append(py::str(""));
    }
    return py::join(py::str("\n"), L) + py::str("\n");
}

Result run(const py::str& src_dir, bool dry_run, const std::optional<py::list<py::str>>& extra_classes,
           const std::optional<py::str>& index_dir) {
    Result result = mark(src_dir, dry_run, extra_classes);
    std::optional<py::str> index_path;
    if (!dry_run) {
        index_path = py::os::path::join(py::truthy(index_dir) ? *index_dir : src_dir, py::str("UI_INDEX.md"));
        py::file f = py::open(*index_path, py::str("w"), py::str("utf-8"));
        f.write(render_index(result));
        f.close();
    }
    result.index = index_path;
    return result;
}

}  // namespace codebreaker::java::projects::minecraft::ui_mark
