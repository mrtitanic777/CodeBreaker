// codebreaker/java/projects/minecraft/package.hpp -- typed C++ port of codebreaker/java/projects/minecraft/__init__.py.
//
// The Minecraft project: RubyDung, Indev, Infdev, Alpha, Beta and release.
//
// What only Minecraft needs, behind the project hooks (codebreaker/java/projects):
//   * its target profiles (profiles/), one per era, chosen by edition (--edition, --mc-version, or the source's
//     own marks);
//   * the console fixes to the game's own logic -- on the IR (sound_fix, applet_ping, render_fix) and on the
//     written program (controller_nav, keybind_mark, raycast);
//   * its reports: the asset manifest (content), achievements, platform-layer coverage;
//   * data: the font's house glyph, the class registrars its reflection uses.
//
// Typing notes:
//   * Importing the Python package registers the era profiles with codebreaker.java.profiles and then sets the
//     Tessellator's heavy_types_high on every registered `minecraft_*` profile. load_profiles() does the same,
//     once (the first time PROJECT() is asked for, which is what importing the package is for the converter).
//     The era profile ports hold their Profile as minecraft_xbox360::Profile; it is registered as the
//     profiles::Profile (same fields) the registry holds.
//   * PROJECT is `PROJECT = Minecraft()`: a function giving the one instance.
#pragma once

#include <memory>

#include "codebreaker/java/projects/package.hpp"

namespace codebreaker::java::projects::minecraft {

class Minecraft : public Project {
  public:
    Minecraft();
    py::str default_profile = py::str("minecraft_xbox360");

    std::optional<py::str> profile_for(const std::optional<py::str>& edition, const std::optional<py::str>& mc_version,
                                       const py::list<ClassRef>& classes) override;
    Patched ir_patches(const py::list<translate::ir::NodeP>& units, const std::optional<py::str>& platform) override;
    py::str finalize_text(const py::str& text, const std::optional<py::str>& platform) override;
    void after_emit(const py::str& out_dir, const std::optional<py::str>& platform, const Patched& patched,
                    const Progress& prog) override;
    void after_construction(const py::str& out_dir, const std::optional<py::str>& platform, const Progress& prog) override;
    void reports(const py::str& java_dir, const py::str& out_dir, const py::list<py::str>& runtime_files,
                 const Progress& prog) override;
};

// Register the era profiles (xbox360, rubydung, indev, infdev, alpha, beta, release) with java/profiles and give
// the minecraft_* ones the Tessellator's heavy_types_high. Idempotent.
void load_profiles();

// PROJECT = Minecraft()
std::shared_ptr<Project> PROJECT();

}  // namespace codebreaker::java::projects::minecraft
