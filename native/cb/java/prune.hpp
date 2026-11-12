// cb/java/prune.hpp -- passes over the emitted tree that fold and cut files, by their #include graph.
//
// After every class, the runtime and the platform layer are written, two cleanups make the tree smaller without changing what it
// builds:
//   * `coalesce_forwarders` folds pure FORWARDER headers into their targets;
//   * `prune_unreferenced` cuts the dead subsystems no live code reaches.
// Both work on the flat folder of .h and .cpp files, rewrite or delete files in place, and say what they did on the progress
// callback. They are the console port's size discipline: a 360 title ships only what it links.
#pragma once

#include <filesystem>
#include <functional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::prune {

using Progress = std::function<void(std::string_view)>;

// The top-level symbols a translation unit DEFINES: class and struct names, the owner class of every `X::` method definition, and
// free-function names (`<ret> name(args) {`). It lets the prune keep an implementation file whose symbols live code links against
// even when NO file #includes it (FileInputStream's constructor and the free function J2X_getResourceAsStream both live in
// FileIO.cpp; Level and ImageIO reference them by name, not by including FileIO.h). Over-extraction only over-KEEPS (safe): the goal
// is never to miss a real link dependency. Only DEFINITIONS match (a trailing `{`), so a usage like `std::vector v;` or
// `d3d9::glClear(...);` is not captured -- otherwise every file would "provide" std and d3d9 and nothing could be pruned. The text
// is expected without comments.
std::set<std::string> defined_symbols(std::string_view text);

// Folds pure FORWARDER headers into their targets. A forwarder is a header whose only real content is a single `#include "target.h"`
// -- a redirect that exists only because the Java code imported a name whose C++ definition lives elsewhere (ByteBuffer.h ->
// NIOBuffers.h, Keyboard.h and Mouse.h -> Input.h, Iterator.h -> JIterator.h). Every `#include "forwarder.h"` across the tree is
// rewritten to the resolved target (a chain of forwarders is followed to the end) and the forwarder is deleted. Safe and cosmetic:
// the TYPE stays defined in the target, so no code changes, only fewer files. Auto-detected, no curated list. Returns the removed
// file names, sorted. FormatError if a file is not valid UTF-8.
std::vector<std::string> coalesce_forwarders(const std::filesystem::path& out_dir, const Progress& progress = nullptr);

// The full-product prune: an automatic dead-subsystem cut, no curated library list. The ROOTS are the converted game corpus
// (`game_classes`), the entry and build spine (stdafx, JavaRT, JavaString, JArray, JList, JMap, JSet, JObject, JCat, Common,
// j2x_init, j2x_rtti, Main) and any file marked `J2X-ALWAYS-LINK`. A file is KEPT if either
//   (1) it is #include-reachable from a root (a header drags its .cpp partner, the same translation unit), or
//   (2) it DEFINES a symbol that live code references by name (the cross-named implementation case; see defined_symbols).
// Everything else -- whole platform bundles (audio, net) and unused java.* stubs the game never touches -- is a dead island and is
// deleted. Soundness: rule (1) keeps a file's own #includes (the closure leaves no include dangling); rule (2) keeps any translation
// unit that supplies a symbol live code links against (no unresolved externals). The residual is link-only side effects with no name
// reference at all (HeapTrack's operator new), handled by the J2X-ALWAYS-LINK marker.
//
// `overlay_files` are the relative paths a project overlay just wrote. They are roots by DEFINITION: a human put the file there
// deliberately, which outranks include-reachability. Without them every overlay-ADDED file was silently deleted on every convert --
// the 360 boot and config glue (Kernel.cpp, KernelConfig.h, ControllerConfig.h) and the whole UDP scaffold, which nothing includes
// yet BY DESIGN -- defeating the "added .cpp are compiled" guarantee of overlays.hpp.
//
// Returns the deleted file names, sorted.
std::vector<std::string> prune_unreferenced(const std::filesystem::path& out_dir, const std::set<std::string, std::less<>>& game_classes,
                                            const std::vector<std::string>& overlay_files = {}, const Progress& progress = nullptr);

}  // namespace cb::java::prune
