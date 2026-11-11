// codebreaker/java/projects/package.hpp -- typed C++ port of codebreaker/java/projects/__init__.py.
//
// projects -- what belongs to one program family (a game, an application), kept out of the core.
//
// The converter's core knows Java, C++ and the target platform. Everything that knows a particular program --
// its classes, its era, the fixes its own logic needs on a console, the reports only it makes sense for -- lives
// in a project package under here, and the core reaches it only through the hooks of `Project`
// (docs/TRANSLATOR_DESIGN.md, phase 3: the project layer).
//
// A project is found by its `manifest.json`, read as data: the packages and class names that mark the program,
// and the convert options it claims. Only a project that matches (or is asked for) is imported, so converting
// any other program loads none of its code. With no match the conversion is `generic`: no project, no profile
// data beyond the empty defaults.
//
// Typing notes:
//   * A manifest is a dict of fixed keys in practice (name, description, packages, classes, options,
//     profile_prefixes): struct Manifest. `manifests()` reads them from the folder of this package
//     (`_paths::_HERE()` + "projects"), a manifest.json per project folder, sorted by folder name.
//   * `importlib.import_module('codebreaker.java.projects.<name>').PROJECT` cannot be done by name: load() knows the
//     project packages the repository has (minecraft) and raises ImportError "No module named ..." for a manifest
//     whose package it does not have.
//   * Project is a class with virtual hooks (the Python's class attributes `name`, `char_map`, `registrars` are
//     fields). ClassRef is a parsed class (java/ir.hpp JClass: the Python reads `.package` and `.name`).
//   * `Patched` is what ir_patches answers: the IR fixes' reports by name ({'sound': ..., 'applet': ..., 'render':
//     ...} in the Python; empty for the generic project).
//   * choose(**options): the options are {name: value}; a value is "given" when it is not empty (Python truthiness).
//     choose(classes, project, edition, mc_version) is the call the converter makes.
#pragma once

#include <functional>
#include <memory>
#include <optional>

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
                                               const py::list<ClassRef>& classes);
    // Change the lowered program before the back end writes it. -> {report name: report}.
    virtual Patched ir_patches(const py::list<translate::ir::NodeP>& units, const std::optional<py::str>& platform);
    // A last change to one emitted C++ file (before the GL namespace rename).
    virtual py::str finalize_text(const py::str& text, const std::optional<py::str>& platform);
    // After every class is written and before overlays: fixes and checks on the written program.
    virtual void after_emit(const py::str& out_dir, const std::optional<py::str>& platform, const Patched& patched,
                            const Progress& prog);
    // After two-phase construction (the last rewrite): markers over the final code.
    virtual void after_construction(const py::str& out_dir, const std::optional<py::str>& platform, const Progress& prog);
    // The project's own reports (assets, achievements, platform coverage).
    virtual void reports(const py::str& java_dir, const py::str& out_dir, const py::list<py::str>& runtime_files,
                         const Progress& prog);
};

// manifest.json of a project package, read as data
struct Manifest {
    py::str name;
    py::str description;
    py::list<py::str> packages, classes, options, profile_prefixes;
};

const std::shared_ptr<Project>& GENERIC();

// [Manifest] of every project package, by name.
py::list<Manifest> manifests();

// The project called `name` ('generic' or a project package). ValueError for an unknown one.
std::shared_ptr<Project> load(const std::optional<py::str>& name);

// Load the project that registers target profile `profile_name` (by its manifest's prefix); null when none does.
std::shared_ptr<Project> for_profile(const py::str& profile_name);

// Do the parsed classes carry the manifest's marks (a package prefix or a class name)?
bool matches(const Manifest& manifest, const py::list<ClassRef>& classes);

// The project for this conversion: the one asked for; else the one claiming a given option (an `edition`, an
// `mc_version`); else the one whose marks the program carries; else generic.
std::shared_ptr<Project> choose(const py::list<ClassRef>& classes, const std::optional<py::str>& project,
                                const py::dict<py::str, py::str>& options);
std::shared_ptr<Project> choose(const py::list<ClassRef>& classes, const std::optional<py::str>& project = std::nullopt,
                                const std::optional<py::str>& edition = std::nullopt,
                                const std::optional<py::str>& mc_version = std::nullopt);

}  // namespace codebreaker::java::projects
