// cb/java/projects/package.hpp -- what belongs to one program family (a game, an application), kept out of the core.
//
// The converter's core knows Java, C++ and the target platform. Everything that knows a particular program -- its
// classes, its era, the fixes its own logic needs on a console, the reports only it makes sense for -- lives in a project
// under here, and the core reaches it only through the hooks of `Project` (docs/TRANSLATOR_DESIGN.md, phase 3: the project
// layer).
//
// A project is found by its `manifest.json`, read as data: the packages and class names that mark the program, and the
// convert options it claims. Only a project that matches (or is asked for) is used, so converting any other program runs
// none of its code. With no match the conversion is `generic`: no project, no profile data beyond the empty defaults.
//
//     ProjectSet projects("java/projects");                       // reads the manifests
//     projects.add("minecraft", std::make_unique<Minecraft>());   // the projects this build has
//     Project& p = projects.project(projects.choose(classes, "", {.edition = "alpha"}));
//
// A ProjectSet is an ordinary object (what the old module kept in globals is now its members): it is not shared between
// threads while projects are being added.
#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/arena.hpp"
#include "cb/java/ir.hpp"
#include "cb/java/projects/minecraft/applet_ping.hpp"
#include "cb/java/projects/minecraft/render_fix.hpp"
#include "cb/java/projects/minecraft/sound_fix.hpp"
#include "cb/translate/ir.hpp"

namespace cb::java::projects {

inline constexpr std::string_view kGeneric = "generic";

// What ir_patches answers: the reports of the IR fixes that ran.
struct Patched {
    std::optional<minecraft::sound_fix::Report> sound;
    std::optional<minecraft::applet_ping::Report> applet;
    std::optional<minecraft::render_fix::Report> render;
};

// Progress lines of a long step.
using Progress = std::function<void(std::string_view)>;

// The generic project, and the base every project extends. Each hook is a no-op here.
class Project {
public:
    virtual ~Project() = default;

    std::string name = std::string(kGeneric);
    // source char code -> the 8-bit code the program's font draws
    std::map<std::uint32_t, std::uint32_t> char_map;
    // calls whose arguments map ids/names to classes (reflection): patterns for the reflection pass
    std::vector<std::string> registrars{R"(register\w*)"};

    // The target-profile name for this conversion, or none to keep the active one. UsageError for an edition or version
    // the project does not take (the generic one takes neither).
    virtual std::optional<std::string> profile_for(std::string_view edition, std::string_view mc_version,
                                                   std::span<const ir::JClass> classes) const;
    // Change the lowered program before the back end writes it.
    virtual Patched ir_patches(Arena& arena, const std::vector<translate::ir::Node*>& units, std::optional<std::string_view> platform);
    // A last change to one emitted C++ file (before the GL namespace rename).
    virtual std::string finalize_text(std::string text, std::optional<std::string_view> platform) const;
    // After every class is written and before overlays: fixes and checks on the written program.
    virtual void after_emit(const std::filesystem::path& out_dir, std::optional<std::string_view> platform, const Patched& patched,
                            const Progress& progress);
    // After two-phase construction (the last rewrite): markers over the final code.
    virtual void after_construction(const std::filesystem::path& out_dir, std::optional<std::string_view> platform, const Progress& progress);
    // The project's own reports (assets, achievements, platform coverage).
    virtual void reports(const std::filesystem::path& java_dir, const std::filesystem::path& out_dir,
                         std::span<const std::string> runtime_files, const Progress& progress);
};

// The manifest.json of a project folder: the marks that identify the program and the options it claims.
struct Manifest {
    std::string name;                       // the manifest's "name", else its folder's
    std::string description;
    std::vector<std::string> packages;      // a class in a package starting with one of these belongs to the program
    std::vector<std::string> classes;       // ... and so does a class with one of these names
    std::vector<std::string> options;       // the convert options (edition, mc_version) that claim the program
    std::vector<std::string> profile_prefixes;   // the target profiles the project registers start with these
};

// Do the parsed classes carry the manifest's marks (a package prefix or a class name)?
bool matches(const Manifest& manifest, std::span<const ir::JClass> classes);

// The convert options a project can claim; an option is "given" when its value is not empty.
struct ChooseOptions {
    std::string edition;
    std::string mc_version;

    std::set<std::string, std::less<>> given() const;
};

class ProjectSet {
public:
    // Reads `<projects_dir>/<folder>/manifest.json` for each folder, in folder-name order; a folder without a manifest
    // is not a project. FormatError for a manifest that is not valid JSON. An absent folder holds no projects.
    explicit ProjectSet(const std::filesystem::path& projects_dir);

    const std::vector<Manifest>& manifests() const { return manifests_; }

    // Make `project` the one a conversion gets under `name` (a manifest's name).
    void add(std::string name, std::unique_ptr<Project> project);

    // The project called `name`: "generic" (or an empty name) or a registered one. UsageError for a name no manifest
    // has; UnsupportedError for a manifest whose project this build does not carry.
    Project& project(std::string_view name);

    // The name of the project for this conversion: the one asked for; else the one claiming a given option (an
    // `edition`, an `mc_version`); else the one whose marks the program carries; else "generic". UsageError when the
    // one asked for is not a project.
    std::string choose(std::span<const ir::JClass> classes, std::string_view requested = {}, const ChooseOptions& options = {}) const;

    // The name of the project that registers target profile `profile_name` (by its manifest's prefixes), if any.
    std::optional<std::string> for_profile(std::string_view profile_name) const;

private:
    std::vector<Manifest> manifests_;
    Project generic_;
    std::map<std::string, std::unique_ptr<Project>, std::less<>> registered_;
};

}  // namespace cb::java::projects
