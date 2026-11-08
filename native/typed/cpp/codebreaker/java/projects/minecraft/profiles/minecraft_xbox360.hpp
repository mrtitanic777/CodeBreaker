// codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp -- typed C++ port of
// codebreaker.java.projects.minecraft.profiles.minecraft_xbox360.
//
// The bundled Minecraft / Xbox-360 target profile.
//
// This is DATA, not logic: the game- and platform-specific facts (class names, package roles, nullable
// fields/getters, static-init seed, project-filter rules) that the generic transpiler core reads through
// the `profiles` interface. Names and APIs are referenced as facts so the tool can recognize and
// translate them; no third-party source is embedded here.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_xbox360 {

// codebreaker.java.profiles.Profile has no typed port yet, so its fields are a record here (the Python
// class is a bag of target data; every field defaults to empty). `register(PROFILE)` -- adding the
// profile to the Python registry -- is not ported: the registry is Python state of the profiles package.
struct Profile {
    py::str name;
    py::list<py::str> static_init_order;
    py::dict<py::str, py::str> package_roles;
    py::list<py::str> heavy_types;                                // a tuple of names
    py::list<std::tuple<py::str, py::str>> vcxproj_filters;       // (basename regex, filter path)
    std::optional<py::str> dead_types;                            // anchored regex, or None
    std::optional<py::str> optional_types;                        // anchored regex, or None
    py::set<py::str> nullable;
    py::set<py::str> nullable_fields;
    py::set<py::str> nullable_getters;
    py::dict<py::str, py::str> nullable_getter_return_type;
    py::set<py::str> unsigned_byte_scalar_fields;
    py::set<py::str> default_value_types;
    py::list<py::str> heavy_types_high;                           // () by default
};

// Proven, ctor-MEDIATED static-init dependencies used to SEED the topological order (e.g. Block's
// ctor reads Material statics, so Material must init first).
extern const py::list<py::str> STATIC_INIT_ORDER;

// Package -> one-line role, for the signatures.md codebase map.
extern const py::dict<py::str, py::str> PACKAGE_ROLES;

// Types heavy enough that per-frame heap allocation of them is a hotpath-rule flag.
extern const py::list<py::str> HEAVY_TYPES;

// Registry arrays whose slots are NULL until an id is registered (deref of an unregistered slot is a
// virtual call on NULL = crash on the console).
extern const py::set<py::str> NULLABLE;

// Fields that are legitimately NULL at points in the lifecycle -- any 360 deref without a null check
// is a hard crash, so the converter guards them.
extern const py::set<py::str> NULLABLE_FIELDS;

// Getters that can return NULL (sparse-registry lookups / optional references).
extern const py::set<py::str> NULLABLE_GETTERS;

// Return (pointer) type of each pointer-returning nullable getter, so the null-guard hoist can declare
// a typed C++03 temp. getSmeltingResultItem is intentionally ABSENT (returns int, never derefs).
extern const py::dict<py::str, py::str> NULLABLE_GETTER_RETURN_TYPE;

// DEAD external desktop/GUI types: java.awt / javax.swing / java.applet + org.w3c.dom, plus the
// crash-report isometric-preview canvases. These cannot function on the console (no window, no AWT, no
// DOM). The pipeline DROPS any of these that is also unreferenced (reachability is the safety net), so
// no-op stubs for them are never brought into the port. PRECISE ON PURPOSE -- explicit class names, never
// a `\w*Listener`/`\w*Event` wildcard, so a real game type (LevelListener, a game FooEvent) is never
// swept in. Reachable ones (BufferedImage/Raster/Graphics/ImageIO on the texture-decode path) are kept
// and grouped by the matching filter rule below. Shared as the prune predicate (profile.dead_types).
extern const py::str DEAD_TYPES;

// Visual Studio .vcxproj/.filters classification: (basename regex, filter path). First match wins;
// EVERY file must resolve to a filter (fallback 'Game (misc)'), never the project root.
extern const py::list<std::tuple<py::str, py::str>> VCXPROJ_FILTERS;

extern const Profile PROFILE;

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_xbox360
