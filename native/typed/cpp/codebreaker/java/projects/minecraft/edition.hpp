// codebreaker/java/projects/minecraft/edition.hpp -- typed C++ port of codebreaker.java.projects.minecraft.edition.
//
// Edition resolution -- pick the per-era target profile for a conversion.
//
// An "edition" names one of the six Minecraft eras; each maps to a registered target profile
// (``rubydung`` -> ``minecraft_rubydung``, ...). :func:`resolve` chooses one by precedence:
//
//     explicit --edition flag   (user override, highest)
//       -> version-id mapping    (when --mc-version names a version)
//       -> source auto-detection (heuristic over the parsed corpus)
//       -> None                  (nothing conclusive -> caller keeps the default profile, a no-op)
//
// Returning ``None`` on the uncertain path is deliberate: ``convert`` then never calls
// ``profiles.use()``, so the default (mixture) profile and today's exact output are preserved.
//
// This module is pure and read-only: no SDK, no network, no decompile. Auto-detection runs on the
// already-parsed corpus (each class exposes ``.name`` and ``.package``), so it costs nothing extra.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"
#include "codebreaker/java/ir.hpp"

namespace codebreaker::java::projects::minecraft::edition {

// A parsed class: the Python reads ``.name`` and ``.package`` off each one (getattr with a None default); a
// class here is the typed port of codebreaker.java.ir.JClass, whose name is always a str.
using ClassInfo = py::ref<java::ir::JClass>;

// Short edition name (CLI choice / return value) -> registered target-profile name.
extern const py::dict<py::str, py::str> EDITIONS;

// Ordered (version-id regex, edition) rules. FIRST match wins, so more specific prefixes come first --
// `inf-` before `in`, the era letters before the bare release-digit rule. The version id is the string
// the decompiler backend understands (e.g. a1.2.6, b1.6.6, c0.30_01c, in-20100223, inf-20100618).
extern const py::list<std::tuple<py::re::Pattern, py::str>> _VERSION_RULES;

// Map a decompiler version id to a short edition name, or ``None`` if unrecognized.
std::optional<py::str> edition_for_version(const std::optional<py::str>& version_id);

// (the set of class names, the set of packages) of the corpus
std::tuple<py::set<py::str>, py::set<py::str>> _corpus_sets(const std::optional<py::list<ClassInfo>>& classes);

// Best-effort edition guess from the parsed corpus, or ``None`` when nothing is conclusive.
//
// Conservative on purpose: it would rather return ``None`` (keep the default profile) than
// mis-detect. The decisive axis is CHUNK STREAMING, not the world class name -- the real Indev
// decompile names its finite world class ``World`` (in ``net.minecraft.game.level``) and has NO
// ``Chunk``/``ChunkProvider`` at all, so "World present" alone does NOT mean Alpha. Markers:
//   * ``com.mojang.rubydung`` package / ``RubyDung`` class                 -> rubydung (pre-classic)
//   * ``net.minecraft.server`` split / block-state classes                -> release
//   * ``BiomeGenBase``/``RegionFile``/``AnvilChunkLoader``/``WorldChunkManager`` -> beta
//   * has ``Chunk``/``ChunkProvider`` (streamed world), no biome markers:
//         ``net.minecraft.game.*`` layout -> infdev ; flat ``net.minecraft.src`` -> alpha
//   * NO chunk streaming (finite world), ``net.minecraft.game.level`` / ``LevelLoader`` -> indev
std::optional<py::str> detect_edition(const std::optional<py::list<ClassInfo>>& classes);

// Map a short edition name to its registered target-profile name, or ``None`` if unknown.
std::optional<py::str> profile_name(const std::optional<py::str>& edition);

// Resolve the target-profile name to activate, or ``None`` to keep the current default.
//
// Precedence: explicit edition > version-id mapping > source auto-detection > None.
// ``explicit`` and the detected value may be either a short edition name (``'alpha'``) or an already
// -qualified profile name (``'minecraft_alpha'``); both resolve correctly.
// Raises py::ValueError for an unknown explicit edition. (``explicit`` is a C++ keyword: explicit_.)
std::optional<py::str> resolve(const std::optional<py::str>& explicit_ = std::nullopt,
                               const std::optional<py::str>& mc_version = std::nullopt,
                               const std::optional<py::list<ClassInfo>>& classes = std::nullopt);

}  // namespace codebreaker::java::projects::minecraft::edition
