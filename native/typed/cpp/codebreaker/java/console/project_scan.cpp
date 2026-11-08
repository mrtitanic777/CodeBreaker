// codebreaker/java/console/project_scan.cpp -- C++ port of codebreaker.java.console.project_scan.
//
// A decompiled Minecraft tree keeps its parts in predictable places under a base folder, e.g.
//
//     <base>\jars\deobfuscated.jar          <- textures/resource jar
//     <base>\game\resources\newsound\...    <- the .ogg music/sound tree
//     <base>\src\*.java  (or deobfuscated-source.jar)   <- the Java source
//
// Hand-selecting each of those in the wizard is tedious, so given ONE folder (the base, or the src inside
// it) this finds the base and resolves the jar, the resources/music dir, the Java source, and the language
// file -- so the later questionnaires (Assets jar, Audio resources, Achievements lang) come pre-filled.
// The scan is depth-bounded and skips heavy build folders so it stays fast.

#include "codebreaker/java/console/project_scan.hpp"

namespace codebreaker::java::console::project_scan {

const std::tuple<py::str, py::str> JAR_PREFERRED_ = std::tuple<py::str, py::str>{py::str("deobfuscated.jar"), py::str("minecraft.jar")};
const py::set<py::str> SKIP_DIRS_ = {py::str(".git"), py::str(".svn"), py::str("node_modules"), py::str("bin"),
                                     py::str("obj"), py::str("debug"), py::str("release"), py::str("release_ltcg"),
                                     py::str("ipch"), py::str(".vs"), py::str("x64"), py::str("x86")};
const std::tuple<py::str, py::str, py::str> BASE_MARKERS_ = std::tuple<py::str, py::str, py::str>{py::str("jars"), py::str("game"), py::str("resources")};

py::list<py::str> _listdir(const py::str& d) {
    try {
        return py::os::listdir(d);
    } catch (const py::OSError&) {
        return py::list<py::str>{};
    }
}

py::str find_base(const py::str& project_dir) {
    py::str d = py::os::path::abspath(project_dir);
    for (int64_t _ = 0; _ < 8; ++_) {
        py::list<py::str> names;
        for (const py::str& x : _listdir(d)) {
            names.append(x.lower());
        }
        py::set<py::str> low = py::set<py::str>::from(names);
        bool found = false;
        for (const py::str& m : py::tuple_items(BASE_MARKERS_)) {
            if (low.contains(m)) {
                found = true;
                break;
            }
        }
        if (found) {
            return d;   // nearest match wins (don't overshoot upward)
        }
        py::str parent = py::os::path::dirname(d);
        if (parent == d) {
            break;
        }
        d = parent;
    }
    return py::os::path::abspath(project_dir);
}

py::list<std::tuple<py::str, py::list<py::str>, py::list<py::str>>> _walk(const py::str& root_in, int64_t maxdepth) {
    py::list<std::tuple<py::str, py::list<py::str>, py::list<py::str>>> out;
    py::str root = py::os::path::abspath(root_in);
    int64_t base = root.rstrip(py::os::sep).count(py::os::sep);
    for (const auto& step : py::os::walk(root)) {
        const py::str& dp = std::get<0>(step);
        const py::list<py::str>& dirs = std::get<1>(step);
        const py::list<py::str>& files = std::get<2>(step);
        if (dp.count(py::os::sep) - base >= maxdepth) {
            dirs.clear();
        }
        py::list<py::str> keep;
        for (const py::str& x : dirs) {
            if (!SKIP_DIRS_.contains(x.lower())) {
                keep.append(x);
            }
        }
        dirs.clear();   // dirs[:] = keep  (the walk goes on into what is left)
        dirs.extend(keep);
        out.append(std::make_tuple(dp, dirs.copy(), files));
    }
    return out;
}

std::optional<py::str> find_jar(const py::str& base) {
    std::optional<py::str> fallback;
    for (const auto& step : _walk(base, 5)) {
        const py::str& dp = std::get<0>(step);
        bool in_jars = py::os::path::basename(dp).lower() == "jars";
        for (const py::str& f : std::get<2>(step)) {
            py::str fl = f.lower();
            if (!fl.endswith(py::str(".jar")) || fl.contains(py::str("source"))) {
                continue;
            }
            py::str p = py::os::path::join(dp, f);
            if (py::in(fl, JAR_PREFERRED_)) {
                return p;
            }
            if (in_jars && !fallback) {
                fallback = p;
            } else if (!fallback) {
                fallback = p;
            }
        }
    }
    return fallback;
}

std::optional<py::str> find_resources(const py::str& base) {
    for (const auto& step : _walk(base, 5)) {
        const py::str& dp = std::get<0>(step);
        if (py::os::path::basename(dp).lower() == "resources") {
            py::list<py::str> names;
            for (const py::str& x : std::get<1>(step)) {
                names.append(x.lower());
            }
            py::set<py::str> low = py::set<py::str>::from(names);
            py::set<py::str> wanted = {py::str("newsound"), py::str("sound"), py::str("newmusic"), py::str("music"),
                                       py::str("streaming")};
            if (!(low & wanted).empty()) {
                return dp;
            }
        }
    }
    return std::nullopt;
}

std::optional<py::str> find_java_src(const py::str& project_dir, const py::str& base) {
    auto has_java = [](const py::str& d) -> bool {
        if (!py::os::path::isdir(d)) {
            return false;
        }
        for (const auto& step : _walk(d, 6)) {
            for (const py::str& f : std::get<2>(step)) {
                if (f.endswith(py::str(".java"))) {
                    return true;
                }
            }
        }
        return false;
    };
    for (const char* name : {"src", "source", "sources"}) {   // the package root, not a deep package
        py::str cand = py::os::path::join(base, py::str(name));
        if (has_java(cand)) {
            return cand;
        }
    }
    if (has_java(project_dir)) {
        return py::os::path::abspath(project_dir);
    }
    return std::nullopt;
}

std::optional<py::str> find_lang(const py::str& base) {
    std::optional<py::str> first;
    for (const auto& step : _walk(base, 6)) {
        const py::str& dp = std::get<0>(step);
        if (py::os::path::basename(dp).lower() == "lang") {
            for (const py::str& f : std::get<2>(step)) {
                if (f.lower().endswith(py::str(".lang"))) {
                    py::str p = py::os::path::join(dp, f);
                    if (f.lower() == "en_us.lang") {
                        return p;
                    }
                    if (!first) {
                        first = p;
                    }
                }
            }
        }
    }
    return first;
}

Resolved resolve(const py::str& project_dir) {
    py::str base = find_base(project_dir);
    return Resolved{base, find_jar(base), find_resources(base), find_java_src(project_dir, base), find_lang(base)};
}

}  // namespace codebreaker::java::console::project_scan
