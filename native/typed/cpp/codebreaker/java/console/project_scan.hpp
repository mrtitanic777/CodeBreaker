// codebreaker/java/console/project_scan.hpp -- C++ port of codebreaker.java.console.project_scan:
// from one folder the user picks, resolve everything the conversion needs.
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::console::project_scan {

extern const std::tuple<py::str, py::str> JAR_PREFERRED_;     // Python's _JAR_PREFERRED
extern const py::set<py::str> SKIP_DIRS_;                     // Python's _SKIP_DIRS
extern const std::tuple<py::str, py::str, py::str> BASE_MARKERS_;   // Python's _BASE_MARKERS (a dir is a plausible
                                                                    // base if it directly contains one of these)

// What resolve() returns: the dict of possibly-None values {base, jar, resources, java_src, lang}.
struct Resolved {
    py::str base;
    std::optional<py::str> jar;
    std::optional<py::str> resources;
    std::optional<py::str> java_src;
    std::optional<py::str> lang;
};
template <class F> void for_each_field(const Resolved& r, F&& f) {
    f("base", r.base);
    f("jar", r.jar);
    f("resources", r.resources);
    f("java_src", r.java_src);
    f("lang", r.lang);
}

py::list<py::str> _listdir(const py::str& d);

// Walk up from `project_dir` and return the CLOSEST folder that looks like the decompiled base
// (contains jars/ or game/ or resources/). Falls back to `project_dir`.
py::str find_base(const py::str& project_dir);

// Python's _walk is a generator; here it is the list of what it would yield: (dirpath, dirnames, filenames),
// with the skipped folders and the folders past maxdepth already pruned from dirnames.
py::list<std::tuple<py::str, py::list<py::str>, py::list<py::str>>> _walk(const py::str& root, int64_t maxdepth = 5);

// The textures/resource jar: prefer deobfuscated.jar / a jar under a 'jars' dir; never a *-source.jar.
std::optional<py::str> find_jar(const py::str& base);

// The audio/resource tree: a dir named 'resources' that holds a newsound/ or sound/ (or music/) child.
std::optional<py::str> find_resources(const py::str& base);

// The Java source ROOT (the folder the packages live under, e.g. `src`).
std::optional<py::str> find_java_src(const py::str& project_dir, const py::str& base);

// A language file: **/lang/en_US.lang (or the first *.lang under a lang/ dir).
std::optional<py::str> find_lang(const py::str& base);

// Resolve everything from the picked folder.
Resolved resolve(const py::str& project_dir);

}  // namespace codebreaker::java::console::project_scan
