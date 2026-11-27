// java/transpile.hpp -- the end-to-end converter: a Java source tree to C++.
//
//   discover .java -> parse -> resolve the program -> write the runtime and platform layer -> translate the bodies
//     -> emit .h/.cpp -> driver, reflection, overlays, two-phase construction -> prune -> verify -> report
//
// This is the orchestrator: every real decision lives in the modules it calls (parse, corpus, emit, rules, ...), so this sequences
// them and collects the ledger. The verify step runs the crash-class rules (rules.hpp) over the EMITTED output as an acceptance
// check -- a correct generator produces about zero findings (arrays get `new T[n]()`, owners get a destructor and a disabled copy).
//
// Nothing here is global. A conversion is handed what it works with (`Services`): the lane's data, the runtime, the target profiles,
// the projects and the translator; the profile and the project of one conversion are chosen from them and handed down, so two
// conversions can run side by side. The translator -- the stage that lowers every method body, constructor and initialiser from javac's
// typed tree through the translator's IR to C++ -- is an extension point (`BodyTranslator`): the C++03 and C++17 back ends provide it.
//
//     Services services{.data = data, .runtime = runtime, .profiles = profiles, .projects = projects, .translator = back_end};
//     Summary done = transpile::convert(java_dir, out_dir, {.cpp_std = "C++03", .platform = "xbox360"}, services);
#pragma once

#include <cstddef>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "java/emit/header.hpp"
#include "java/emit/source.hpp"
#include "java/ir.hpp"
#include "java/ledger.hpp"
#include "java/paths.hpp"
#include "java/profiles/package.hpp"
#include "java/projects/package.hpp"
#include "java/reflection_factory.hpp"
#include "java/runtime.hpp"
#include "java/typemap.hpp"
#include "java/types.hpp"

namespace cb::java::transpile {

using Progress = projects::Progress;   // per-class and per-stage progress lines

// The C++ standard named by a label (`C++98`, `C++03`, `C++11`, `C++14`, `C++17`, `C++20`, `C++23`). UsageError for another label.
emit::header::CppStd parse_standard(std::string_view label);

// The API a --graphics value names: "OpenGL" for opengl, "Direct3D 11" for d3d11.
std::string graphics_api_name(std::string_view graphics);

// ---------------------------------------------------------------------------------------------- the translator

// What the translator is asked to lower.
struct TranslateRequest {
    const std::filesystem::path& java_dir;
    std::span<const ir::JClass> classes;                 // the whole program, as parsed
    const types::TypeResolver& resolver;
    const typemap::TypeProfile& type_profile;            // the target's, with the nullable getters this corpus derived
    ledger::Ledger& ledger;
    projects::Project& project;                          // its char_map, and the IR fixes (ir_patches) the translator applies to the lowered tree
    std::optional<std::string_view> platform;
    int cpp_year = 2003;                                 // a year of 2017 or later asks for the C++17 forms
};

// What the translator produced: the back end the source emitter asks per member, the project's IR-fix reports, and the tally of what it
// declined.
struct TranslatedBodies {
    std::unique_ptr<emit::source::BodySource> bodies;
    projects::Patched patched;                           // what the project's fixes on the lowered tree did
    std::size_t classes_lowered = 0;
    std::size_t methods_written = 0;
    std::vector<std::pair<std::string, std::size_t>> declined;   // why a body was declined -> how many, in the order first met
};

class BodyTranslator {
public:
    virtual ~BodyTranslator() = default;
    virtual TranslatedBodies translate(const TranslateRequest& request) = 0;
};

// ---------------------------------------------------------------------------------------------- a conversion

// What a conversion works with. All of it outlives the call.
struct Services {
    const JavaData& data;                  // the lane's platform layer and API maps
    const runtime::Runtime& runtime;
    profiles::ProfileSet& profiles;        // the target profiles; a conversion reads one, it never switches the active one
    projects::ProjectSet& projects;
    BodyTranslator& translator;
};

struct Options {
    std::string cpp_std = "C++03";
    // The target platform key ("xbox360", "pc_opengl", "pc_windows", "ps3" ...); empty for none. A console platform with no SDK
    // located is REFUSED (console::sdk::SdkNotFoundError): the converter ships no SDK headers.
    std::string platform;
    // The PC's graphics API: "opengl" (the game's own OpenGL through SDL), "d3d9", "d3d10", "d3d11" or "d3d12" (the GL layer the
    // Xbox 360 uses, on that Direct3D in the SDL window). Only the PC reads it.
    std::string graphics = "opengl";
    // The class names to treat as C++ value types; none: the target profile's.
    std::optional<typemap::NameSet> value_types;
    bool write_report = true;
    // Write the real hand-written platform layer (docs/platform/reference: the GL->D3D9 emulator, audio, input, sockets) instead of the
    // no-op stubs. The 360 and the PC (SDL2/OpenGL) targets always get it.
    bool platform_refs = false;
    std::optional<std::filesystem::path> sdk_override;     // an explicit SDK root, else auto-detected
    std::optional<std::filesystem::path> sdk_configured;   // the settings' SDK root
    // Debug.h/.cpp and DEBUG.md: one master switch gating every DBG_* macro. Opt-in: the caller decides whether the port gets debug support.
    bool emit_debug = true;
    std::optional<std::filesystem::path> overlay_dir;      // an explicit overlay folder
    std::filesystem::path overlays_root;                   // where the per-project overlay folders live (settings `java_overlays`)
    std::optional<std::filesystem::path> bugfix_ledger;    // where BUGFIX_LEDGER.md is written; none: not regenerated
    // The Minecraft era to target ('rubydung', 'indev', 'infdev', 'alpha', 'beta', 'release', or a profile name), else resolved from the
    // decompiler version id, else detected from the source, else the active profile is kept.
    std::string edition;
    std::string mc_version;
    Progress progress;                                     // none: silent
};

struct Summary {
    std::size_t parsed = 0;                // classes parsed (after the synthetic markers were dropped)
    std::size_t headers = 0;
    std::size_t sources = 0;
    std::vector<std::string> markers;      // the empty decompiler marker classes dropped on purpose, sorted
    std::size_t methods_written = 0;       // bodies the translator wrote
    std::vector<std::pair<std::string, std::size_t>> declined;   // why it declined the others
    std::size_t rule_findings = 0;
    std::size_t events = 0;
    std::size_t high = 0;                  // the high-severity events
    std::optional<std::filesystem::path> report;
    ledger::Ledger ledger;
    std::string cpp_std;                   // the label
    std::string platform;                  // `none` when there was none
    std::string profile;                   // the target profile active DURING this conversion
};

// Converts a Java source tree to C++. The profile and the project of the conversion are chosen first (the edition asked for, else the
// decompiler version, else the program's own marks), so everything after reads the target's data.
Summary convert(const std::filesystem::path& java_dir, const std::filesystem::path& out_dir, const Options& options, const Services& services);

// The reflection pass's registrars of a project: its patterns `register\w*` (a name prefix) and plain identifiers (a whole name).
// UnsupportedError for any other pattern.
reflection_factory::Registrars registrars_of(const projects::Project& project);

}  // namespace cb::java::transpile
