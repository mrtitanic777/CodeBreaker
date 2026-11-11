// cb/java/bugfix_ledger.hpp -- catalog every game-breaking bug fix so none is ever forgotten.
//
// The standing rule is that EVERY game-breaking fix is tagged in-code with a greppable `FIXED BUG <slug>` (or the
// older `J2X-BUGFIX <slug>`) marker. Those markers are the source of truth. This module walks the converter's
// AUTHORED sources -- the layers that survive a reconvert (the platform references + canonicals + companions, and the
// per-project overlays) -- collects every marker with its slug, location, and the explanatory comment that follows
// it, and renders BUGFIX_LEDGER.md.
//
// Because it is GENERATED from the live markers, the ledger can never drift out of sync with the code: re-run it
// after any fix and it re-catalogs everything. It is wired into the conversion so the ledger regenerates every time.
#pragma once

#include <cstdint>
#include <filesystem>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::bugfix_ledger {

// The layers scanned, most durable first. The OUTPUT layers (canonical/reference/companion) survive a reconvert and
// the emitted tree is derived from them. The GENERATOR layer is the converter itself -- its emitter/runtime/toolchain
// fixes don't appear in the output tree but ARE revertible by an edit to the converter, so they belong in the ledger
// too (the section-sign unsigned-char cast, the SFX/music bypass pass, map-get-inserts).
enum class Layer { Canonical, Reference, Companion, Generator };

struct MarkerEntry {
    Layer layer = Layer::Generator;
    std::string file;          // relative to the scanned root, '/'-separated
    std::string slug;          // kebab-case or a YYYY-MM-DD date
    std::uint32_t line = 0;    // 1-based
    std::string description;   // the comment on the marker line, or on the line after it
};

// The human explanation on a marker line: the text after `FIXED BUG <slug>` with comment punctuation stripped.
std::string clean_comment(std::string_view line);

// The markers in one file (layer and file are left for the caller to fill in); empty if it cannot be read. A marker
// line with no prose borrows the next comment line.
std::vector<MarkerEntry> scan_file(const std::filesystem::path& path);

// Every marker across every layer of the tree at `repo_root` (the java lane's folder), layers in order and, within
// a layer, files in name order. Layers whose folder is missing are skipped.
std::vector<MarkerEntry> collect(const std::filesystem::path& repo_root);

// The BUGFIX_LEDGER.md text for the entries.
std::string render(std::span<const MarkerEntry> entries);

// collect + render + write to out_path (folders created). Returns the entries.
std::vector<MarkerEntry> run(const std::filesystem::path& repo_root, const std::filesystem::path& out_path);

}  // namespace cb::java::bugfix_ledger

