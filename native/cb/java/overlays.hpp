// cb/java/overlays.hpp -- per-project code overlays: re-apply hand-fixes on top of freshly generated output.
//
// WHY THIS EXISTS
//   Most CodeBreaker fixes belong in the emitter (a general transform), the runtime headers, or the platform
//   reference layer -- those are GENERAL and benefit every conversion. But a real port also accumulates fixes that
//   are GAME/VERSION-SPECIFIC edits to GENERATED files: a leak `delete` in one method, a value-comparing std::set
//   swapped in for a broken JSet, a UI tweak, an input remap. These don't generalise into an emitter rule, so a
//   reconversion would silently WIPE them. That is the failure this module prevents.
//
//   A file placed under overlays/<project_key>/<relpath> is copied over out_dir/<relpath> AFTER emission (and after
//   reference emission), so it is the final say. It may REPLACE a generated file or ADD a new one (a hand-written
//   Socket.cpp); because the vcxproj step runs AFTER this and enumerates out_dir, added .cpp files get compiled
//   automatically.
//
//   Precedence (last wins): runtime headers -> emitted classes -> platform references -> OVERLAYS.
//
//   This is the catch-all, not an excuse: when an overlaid fix turns out to be a general pattern, migrate it into
//   the emitter/runtime/reference and delete the overlay. MANIFEST.md in each project dir records what each overlay
//   is and whether it is slated for migration.
//
// The folder that holds every project's overlays (settings `java_overlays`, else state/java/overlays) is passed in
// by the caller as `overlays_root`.
#pragma once

#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::overlays {

// Stable per-project overlay key: the output folder's own name (e.g. "MinecraftBeta"). A relative "." names the
// current directory's folder. Empty when out_dir is empty.
std::string project_key(const std::filesystem::path& out_dir);

// The overlay folder to use: `explicit_dir` when given, else <overlays_root>/<project_key(out_dir)>; nullopt when
// there is no key.
std::optional<std::filesystem::path> resolve_overlay_dir(const std::filesystem::path& out_dir,
                                                         const std::filesystem::path& overlays_root,
                                                         const std::optional<std::filesystem::path>& explicit_dir = std::nullopt);

using Progress = std::function<void(std::string_view)>;

// Copy every file under overlay_dir over out_dir (same relative path), skipping MANIFEST.md and creating missing
// folders. Returns the applied relative paths, '/'-separated, sorted. A no-op when overlay_dir is absent.
std::vector<std::string> apply_overlays(const std::filesystem::path& out_dir,
                                        const std::optional<std::filesystem::path>& overlay_dir,
                                        const Progress& progress = nullptr);

}  // namespace cb::java::overlays
