// codebreaker/java/projects/package.hpp -- SHIM over cb/java/projects/package.hpp (docs/CPP_DESIGN.md 6.3); deleted
// with its last py:: consumer.
//
// What belongs to one program family, kept out of the core. The old `Project` (a class with py:: hooks that the
// Minecraft project overrides) stays here as it was, since the project itself is not refactored yet; the decisions
// -- reading the manifests, which program a conversion is, which project registers a profile -- are the new
// module's (cb::java::projects::ProjectSet), asked afresh on each call as the Python re-read the folder each time.
// `choose` takes the edition and the version (the dict-of-options form had no caller).
#pragma once

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/projects/package.hpp"
#include "codebreaker/java/_paths.hpp"
#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/projects/minecraft/applet_ping.hpp"
#include "codebreaker/java/projects/minecraft/render_fix.hpp"
#include "codebreaker/java/projects/minecraft/sound_fix.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::java::projects {

using ClassRef = py::ref<ir::JClass>;
using Progress = std::function<void(const py::str&)>;

// What ir_patches answers: the IR fixes' reports by name.
struct Patched {
    std::optional<minecraft::sound_fix::Result> sound;
    std::optional<minecraft::applet_ping::Result> applet;
    std::optional<minecraft::render_fix::Result> render;
};

// The generic project, and the base every project extends. Each hook is a no-op here.
class Project {
  public:
    virtual ~Project() = default;
    py::str name = py::str("generic");
    py::dict<int64_t, int64_t> char_map;                          // source char code -> the 8-bit code the program's font draws
    py::list<py::str> registrars{py::str("register\\w*")};        // calls whose arguments map ids/names to classes (reflection)

    // The target-profile name for this conversion, or None to keep the active one. (ValueError: the generic project
    // takes no edition or version.)
    virtual std::optional<py::str> profile_for(const std::optional<py::str>& edition, const std::optional<py::str>& mc_version,
                                               const py::list<ClassRef>&) {
        return cb::compat::guard([&] {
            cb::java::projects::Project generic;
            (void)generic.profile_for(edition ? cb::compat::from_py(*edition) : std::string(),
                                      mc_version ? cb::compat::from_py(*mc_version) : std::string(), {});
            return std::optional<py::str>();
        });
    }
    // Change the lowered program before the back end writes it. -> {report name: report}.
    virtual Patched ir_patches(const py::list<translate::ir::NodeP>&, const std::optional<py::str>&) { return Patched(); }
    // A last change to one emitted C++ file (before the GL namespace rename).
    virtual py::str finalize_text(const py::str& text, const std::optional<py::str>&) { return text; }
    // After every class is written and before overlays: fixes and checks on the written program.
    virtual void after_emit(const py::str&, const std::optional<py::str>&, const Patched&, const Progress&) {}
    // After two-phase construction (the last rewrite): markers over the final code.
    virtual void after_construction(const py::str&, const std::optional<py::str>&, const Progress&) {}
    // The project's own reports (assets, achievements, platform coverage).
    virtual void reports(const py::str&, const py::str&, const py::list<py::str>&, const Progress&) {}
};

// manifest.json of a project package, read as data
struct Manifest {
    py::str name;
    py::str description;
    py::list<py::str> packages, classes, options, profile_prefixes;
};

namespace minecraft {
std::shared_ptr<Project> PROJECT();   // the Minecraft project (java/projects/minecraft/package.hpp)
}

namespace shim_ {
inline cb::java::projects::ProjectSet set_() {
    return cb::java::projects::ProjectSet(cb::fs::from_text(cb::compat::from_py(_paths::_HERE())) / "projects");
}
}  // namespace shim_

inline const std::shared_ptr<Project>& GENERIC() {
    static const std::shared_ptr<Project> g = std::make_shared<Project>();
    return g;
}

// [Manifest] of every project package, by name.
inline py::list<Manifest> manifests() {
    return cb::compat::guard([&] {
        py::list<Manifest> out;
        for (const auto& m : shim_::set_().manifests()) {
            Manifest o;
            o.name = cb::compat::to_py(m.name);
            o.description = cb::compat::to_py(m.description);
            o.packages = cb::compat::to_py(m.packages);
            o.classes = cb::compat::to_py(m.classes);
            o.options = cb::compat::to_py(m.options);
            o.profile_prefixes = cb::compat::to_py(m.profile_prefixes);
            out.append(o);
        }
        return out;
    });
}

// The project called `name` ('generic' or a project package). ValueError for an unknown one; ImportError for a
// manifest whose package this build lacks.
inline std::shared_ptr<Project> load(const std::optional<py::str>& name) {
    static std::map<std::string, std::shared_ptr<Project>> loaded;   // _LOADED
    return cb::compat::guard([&]() -> std::shared_ptr<Project> {
        const std::string wanted = name ? cb::compat::from_py(*name) : std::string();
        const std::string chosen = shim_::set_().choose({}, wanted);   // UsageError: not a project
        if (chosen == cb::java::projects::kGeneric) return GENERIC();
        auto it = loaded.find(chosen);
        if (it == loaded.end()) {
            if (chosen != "minecraft") throw py::ImportError("No module named 'codebreaker.java.projects." + chosen + "'");
            it = loaded.emplace(chosen, minecraft::PROJECT()).first;
        }
        return it->second;
    });
}

// Load the project that registers target profile `profile_name` (by its manifest's prefix); null when none does.
inline std::shared_ptr<Project> for_profile(const py::str& profile_name) {
    const std::optional<std::string> found =
        cb::compat::guard([&] { return shim_::set_().for_profile(cb::compat::from_py(profile_name)); });
    return found ? load(cb::compat::to_py(*found)) : nullptr;
}

// The project for this conversion: the one asked for; else the one claiming a given option (an `edition`, an
// `mc_version`); else the one whose marks the program carries; else generic.
inline std::shared_ptr<Project> choose(const py::list<ClassRef>& classes, const std::optional<py::str>& project = std::nullopt,
                                       const std::optional<py::str>& edition = std::nullopt,
                                       const std::optional<py::str>& mc_version = std::nullopt) {
    const std::string chosen = cb::compat::guard([&] {
        std::vector<cb::java::ir::JClass> news;
        for (const ClassRef& c : classes) news.push_back(ir::to_new(*c));
        cb::java::projects::ChooseOptions options{edition ? cb::compat::from_py(*edition) : std::string(),
                                                  mc_version ? cb::compat::from_py(*mc_version) : std::string()};
        return shim_::set_().choose(news, project ? cb::compat::from_py(*project) : std::string(), options);
    });
    return load(cb::compat::to_py(chosen));
}

}  // namespace codebreaker::java::projects
