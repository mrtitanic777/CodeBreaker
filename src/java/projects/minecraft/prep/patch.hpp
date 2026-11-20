// java/projects/minecraft/prep/patch.hpp -- applying a patch file to a source folder as RetroMCP's TaskApplyPatch does (DiffPatch's PatchOperation
// with PatchMode.OFFSET: one patch file holding many files' hunks, applied to the folder in place, and the summary it prints).
//
//     prep::PatchOutcome r = prep::apply_patch("src_original", "src_original", "conf/client.patch");   // base, out, patches (base == out: in place)
//     if (!r.success) report(r.log + "Patching failed!");
//
// Only what applying needs is here: the hunk matching (exact, then ACCESS (a changed public/protected/private/final/whitespace), then OFFSET; PatchMode
// FUZZY is not ported, RetroMCP never asks for it), the patch file reader, and the folder walk. Not ported: the diff maker, rejects files, and
// patching a single file given as base (RetroMCP always passes a folder).
//
// Ported from DiffPatch (codechicken.diffpatch: PatchOperation, patch.Patcher, util.PatchFile / Patch / LineRange / CharRepresenter / FileCollector)
// as RetroMCP's TaskApplyPatch (org.mcphackers.mcp.tasks) uses it, MIT License, (c) 2018 covers1624, Chicken-Bones.
#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace cb::java::minecraft::prep {

// DiffPatch's PatchMode: how hard a hunk is looked for (each includes the ones before it).
enum class PatchMode { EXACT, ACCESS, OFFSET, FUZZY };

// What a patch run counted (DiffPatch's PatchOperation.PatchesSummary). unchanged/changed/added/removed files are declared by DiffPatch but never counted.
struct PatchesSummary {
    int unchanged_files = 0;
    int changed_files = 0;
    int added_files = 0;
    int removed_files = 0;
    int missing_files = 0;
    int failed_matches = 0;
    int exact_matches = 0;
    int access_matches = 0;
    int offset_matches = 0;
    int fuzzy_matches = 0;
    double overall_quality = 0;

    // The text DiffPatch prints (PrintStream.println: `eol` ends each line).
    std::string print(bool slim, const std::string& eol = "\r\n") const;
};

struct PatchOptions {
    PatchMode mode = PatchMode::OFFSET;       // as TaskApplyPatch asks
    std::string line_separator = "\r\n";      // DiffPatch: System.lineSeparator() of the machine RetroMCP ran on (Windows)
    std::string a_prefix = "a/";              // stripped from a patch file's "--- " path
    std::string b_prefix = "b/";              // stripped from its "+++ " path
};

struct PatchOutcome {
    bool success = false;      // PatchOperation.doPatch's result: every hunk of every file applied
    PatchesSummary summary;
    std::string log;           // summary.print(false): what TaskApplyPatch reports when it did not succeed
};

// TaskApplyPatch.patch: apply `patches` (a patch file, or a folder of them) to the files of `base`, writing the result to `out` (the folder `out` is
// removed first and rewritten from what was read, so base == out patches in place; as DiffPatch does). Throws std::runtime_error where DiffPatch
// throws (a patch file DiffPatch cannot read as patches, an inconsistent hunk). A missing base or patch path gives success == false.
PatchOutcome apply_patch(const std::filesystem::path& base, const std::filesystem::path& out, const std::filesystem::path& patches,
                         const PatchOptions& options = {});

}  // namespace cb::java::minecraft::prep
