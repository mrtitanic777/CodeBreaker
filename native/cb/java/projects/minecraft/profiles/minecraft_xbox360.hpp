// cb/java/projects/minecraft/profiles/minecraft_xbox360.hpp -- the bundled Minecraft / Xbox-360 target profile.
//
// This is DATA, not logic: the game- and platform-specific facts (class names, package roles, nullable
// fields/getters, static-init seed, project-filter rules) that the generic transpiler core reads through a
// `profiles::Profile`. Names and APIs are referenced as facts so the tool can recognise and translate them; no
// third-party source is embedded here.
//
// Each table is a function returning a const reference to a table built on first use; `profile()` assembles them
// into the Profile a project registers in its ProfileSet.
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/java/profiles/package.hpp"
#include "cb/java/typemap.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_xbox360 {

// Proven, ctor-MEDIATED static-init dependencies used to SEED the topological order (e.g. Block's ctor reads
// Material statics, so Material must init first).
const std::vector<std::string>& static_init_order();

// Package -> one-line role, for the signatures.md codebase map (listed in this order).
const OrderedMap<std::string, std::string>& package_roles();

// Types heavy enough that per-frame heap allocation of them is a hot-path-rule flag.
const std::vector<std::string>& heavy_types();

// Registry arrays whose slots are NULL until an id is registered (deref of an unregistered slot is a virtual call
// on NULL = crash on the console).
const typemap::NameSet& nullable();

// Fields that are legitimately NULL at points in the lifecycle -- any 360 deref without a null check is a hard
// crash, so the converter guards them.
const typemap::NameSet& nullable_fields();

// Getters that can return NULL (sparse-registry lookups / optional references).
const typemap::NameSet& nullable_getters();

// Return (pointer) type of each pointer-returning nullable getter, so the null-guard hoist can declare a typed
// C++03 temp. getSmeltingResultItem is intentionally ABSENT (returns int, never derefs).
const typemap::NameMap& nullable_getter_return_type();

// The anchored regex of the DEAD external desktop/GUI types (java.awt / javax.swing / java.applet, org.w3c.dom,
// the crash-report isometric canvases): the prune predicate (profile.dead_types).
std::string_view dead_types();

// Visual Studio .vcxproj/.filters classification: (basename regex, filter path). First match wins; EVERY file must
// resolve to a filter (fallback 'Game (misc)'), never the project root.
const std::vector<java::profiles::FilterRule>& vcxproj_filters();

// The profile named "minecraft_xbox360", assembled from the tables above. It has no optional_types, no
// unsigned-byte overrides, no value types and no high-heavy types.
java::profiles::Profile profile();

}  // namespace cb::java::projects::minecraft::profiles::minecraft_xbox360
