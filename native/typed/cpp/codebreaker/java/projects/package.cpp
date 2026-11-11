// codebreaker/java/projects/package.cpp -- typed C++ port of codebreaker/java/projects/__init__.py (see package.hpp).
#include "codebreaker/java/projects/package.hpp"

#include <map>

#include "codebreaker/java/_paths.hpp"
#include "codebreaker/java/projects/minecraft/package.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "py_os.hpp"

namespace codebreaker::java::projects {

using py::str;
using sources_Json = knowledge::sources::Json;

std::optional<str> Project::profile_for(const std::optional<str>& edition, const std::optional<str>& mc_version,
                                        const py::list<ClassRef>&) {
    if ((edition && !edition->empty()) || (mc_version && !mc_version->empty())) {
        auto r = [](const std::optional<str>& x) { return x ? py::repr(*x) : str("None"); };
        throw py::ValueError(py::percent(str("the generic project takes no edition or version (%s, %s)"), r(edition), r(mc_version)).utf8());
    }
    return std::nullopt;
}
Patched Project::ir_patches(const py::list<translate::ir::NodeP>&, const std::optional<str>&) { return Patched(); }
str Project::finalize_text(const str& text, const std::optional<str>&) { return text; }
void Project::after_emit(const str&, const std::optional<str>&, const Patched&, const Progress&) {}
void Project::after_construction(const str&, const std::optional<str>&, const Progress&) {}
void Project::reports(const str&, const str&, const py::list<str>&, const Progress&) {}

const std::shared_ptr<Project>& GENERIC() {
    static const std::shared_ptr<Project> g = std::make_shared<Project>();
    return g;
}

namespace {

std::map<std::string, std::shared_ptr<Project>>& loaded() {   // _LOADED
    static std::map<std::string, std::shared_ptr<Project>> m;
    return m;
}

py::list<str> strings_of(const sources_Json& d, const char* key) {
    py::list<str> out;
    if (d.kind != sources_Json::Dict || !d.dict.contains(str(key))) return out;
    const sources_Json& v = d.dict.at(str(key));
    if (v.kind == sources_Json::List)
        for (const sources_Json& x : v.list)
            if (x.kind == sources_Json::Str) out.append(x.s);
    return out;
}

}  // namespace

py::list<Manifest> manifests() {
    py::list<Manifest> out;
    str here = py::os::path::join(_paths::_HERE(), str("projects"));
    for (const str& d : py::sorted(py::os::listdir(here))) {
        str p = py::os::path::join(here, d, str("manifest.json"));
        if (py::os::path::isfile(p)) {
            sources_Json m = knowledge::sources::_json(p);
            Manifest mf;
            mf.name = d;                                       // m.setdefault('name', d)
            if (m.kind == sources_Json::Dict && m.dict.contains(str("name")) && m.dict.at(str("name")).kind == sources_Json::Str)
                mf.name = m.dict.at(str("name")).s;
            if (m.kind == sources_Json::Dict && m.dict.contains(str("description")) && m.dict.at(str("description")).kind == sources_Json::Str)
                mf.description = m.dict.at(str("description")).s;
            mf.packages = strings_of(m, "packages");
            mf.classes = strings_of(m, "classes");
            mf.options = strings_of(m, "options");
            mf.profile_prefixes = strings_of(m, "profile_prefixes");
            out.append(mf);
        }
    }
    return out;
}

std::shared_ptr<Project> load(const std::optional<str>& name) {
    if (!name || name->empty() || *name == str("generic")) return GENERIC();
    auto& cache = loaded();
    std::string key = name->utf8();
    if (cache.find(key) == cache.end()) {
        std::optional<Manifest> m;
        for (const Manifest& x : manifests())
            if (x.name == *name) {
                m = x;
                break;
            }
        if (!m) {
            py::list<str> names;
            for (const Manifest& x : manifests()) names.append(x.name);
            throw py::ValueError(py::percent(str("unknown project %r (choices: generic, %s)"), *name, py::join(str(", "), names)).utf8());
        }
        // importlib.import_module('codebreaker.java.projects.%s' % name).PROJECT
        if (*name != str("minecraft")) throw py::ImportError("No module named 'codebreaker.java.projects." + name->utf8() + "'");
        cache[key] = minecraft::PROJECT();
    }
    return cache[key];
}

std::shared_ptr<Project> for_profile(const str& profile_name) {
    for (const Manifest& m : manifests()) {
        bool hit = false;
        for (const str& p : m.profile_prefixes)
            if (profile_name.startswith(p)) hit = true;
        if (hit) return load(m.name);
    }
    return nullptr;
}

bool matches(const Manifest& manifest, const py::list<ClassRef>& classes) {
    for (const ClassRef& c : classes) {
        str pkg = c->package ? *c->package : str();          // getattr(c, 'package', None) or ''
        bool by_pkg = false;
        if (py::len(manifest.packages) > 0) {
            for (const str& p : manifest.packages)
                if (pkg.startswith(p)) by_pkg = true;
        }
        if (by_pkg || manifest.classes.contains(c->name)) return true;
    }
    return false;
}

std::shared_ptr<Project> choose(const py::list<ClassRef>& classes, const std::optional<str>& project,
                                const py::dict<str, str>& options) {
    if (project && !project->empty()) return load(project);
    py::set<str> given;
    for (const auto& kv : options.items())
        if (!std::get<1>(kv).empty()) given.add(std::get<0>(kv));
    py::list<Manifest> ms = manifests();
    for (const Manifest& m : ms) {
        if (!given.empty() && given.issubset(py::set<str>::from(m.options))) return load(m.name);
    }
    for (const Manifest& m : ms) {
        if (matches(m, classes)) return load(m.name);
    }
    return GENERIC();
}

std::shared_ptr<Project> choose(const py::list<ClassRef>& classes, const std::optional<str>& project,
                                const std::optional<str>& edition, const std::optional<str>& mc_version) {
    py::dict<str, str> options;
    if (edition) options.set(str("edition"), *edition);
    if (mc_version) options.set(str("mc_version"), *mc_version);
    return choose(classes, project, options);
}

}  // namespace codebreaker::java::projects
