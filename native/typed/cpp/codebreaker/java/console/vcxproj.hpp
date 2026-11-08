// codebreaker/java/console/vcxproj.hpp -- typed C++ port of codebreaker.java.console.vcxproj.
//
// vcxproj.py -- regenerate the Xbox-project .vcxproj file list after a CodeBreaker code swap.
//
//     python -m codebreaker java vcxproj [--proj <path>]
//
// Strips blank-line padding, removes old ClCompile/ClInclude ItemGroups, inserts fresh
// ones listing the current .cpp/.h, preserving stdafx.cpp's per-config PCH block.
//
// Notes on the port:
//   * codebreaker.java.profiles (the registry of target profiles, `profiles.active()` and the
//     `on_change` observers) has no typed port.  What this module reads of the active profile is its
//     `vcxproj_filters` (a list of (basename regex, filter path)).  _FILTER_RULES starts as the
//     GENERIC profile's (empty), as it does in Python when no project profile has been loaded, and
//     _reseed(filters) is what the `on_change` observer does: the caller that switches the profile
//     (profiles.use) hands the new profile's vcxproj_filters in, e.g.
//     `vcxproj::_reseed(minecraft_xbox360::PROFILE.vcxproj_filters)`;
//   * achievements.normalize_title_id is the typed port's
//     (codebreaker/java/projects/minecraft/achievements.hpp);
//   * json.load of platform.config.json uses py::json_parse; a title_id of any JSON kind is handled
//     (its repr appears in the "is not 1-8 hex digits" message, written as Python's repr does);
//   * run() returns nothing and prints its report, as the Python does.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::console::vcxproj {

// The (basename regex -> filter path) rules of the active profile: FIRST match wins.
extern py::list<std::tuple<py::str, py::str>> _FILTER_RULES;

// Re-derive the .filters classification rules when the active target profile changes
// (profiles.on_change).  `vcxproj_filters` is the new profile's.
void _reseed(const py::list<std::tuple<py::str, py::str>>& vcxproj_filters);

// Assign a class to a Solution-Explorer filter.
//
// First try the profile's name rules -- they anchor the well-known bases and all the
// runtime/platform/Java-stdlib buckets (which have no game inheritance to key off). If a class
// name matches nothing, walk its INHERITANCE chain (recovered from the emitted headers) and adopt
// the filter of its nearest ancestor that a rule DOES match.  (`parent_of` is Python's optional
// dict; an empty one is the same as None.)
py::str _classify(const py::str& basename, const py::dict<py::str, py::str>& parent_of = py::dict<py::str, py::str>());

// class X : public Y
const py::re::Pattern& _CLASS_DECL();

// Scan the emitted .h headers for `class X : public Y` and return {X: Y} (primary base only).
py::dict<py::str, py::str> _build_parent_map(const py::str& proj_dir);

py::str _guid(const py::str& name);

// Emit <proj_name>.vcxproj.filters grouping every .cpp/.h into subsystem virtual folders.
// Deterministic (stable filter GUIDs).  -> (its path, the number of filters)
std::tuple<py::str, int64_t> write_filters(const py::str& proj_dir, const py::str& proj_name,
                                           const py::list<py::str>& cpps, const py::list<py::str>& hs);

// The .vcxproj to operate on: <name>.vcxproj if given, else the single .vcxproj in the dir.
py::str _find_vcxproj(const py::str& proj_dir, const std::optional<py::str>& name = std::nullopt);

// The project's self-issued Title ID from platform.config.json, as (canonical_8_hex or None, note).
std::tuple<std::optional<py::str>, py::str> _title_id_for(const py::str& proj_dir);

// Recursively collect .cpp/.h under proj_dir as RELATIVE (backslash) paths, skipping build/output dirs.
std::tuple<py::list<py::str>, py::list<py::str>> _enum_sources(const py::str& proj_dir);

// Regenerate the .vcxproj ClCompile/ClInclude file list from the .cpp/.h in proj_dir, and (by
// default) also (re)write the subsystem <name>.vcxproj.filters so Solution Explorer shows folders.
// `proj_dir` defaults to the current directory.
void run(const std::optional<py::str>& proj_dir = std::nullopt, const std::optional<py::str>& name = std::nullopt,
         bool write_filters_too = true);

}  // namespace codebreaker::java::console::vcxproj
