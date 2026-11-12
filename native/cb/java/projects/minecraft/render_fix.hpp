// cb/java/projects/minecraft/render_fix.hpp -- two CONSOLE render-correctness fixes for the first in-game frame.
//
// Both are faithful Java->C++ translations that are correct on desktop LWJGL but WRONG on the Xbox 360 render path,
// both are present in every chunk-streaming edition (Indev/Alpha/Beta share the code), and both are fixed the same way
// in the proven OG hand-ports. Migrated here so every edition gets them.
//
// [1] RenderGlobal::updateRenderers -- THE frame-rate fix (the "stuck on Spawning / never draws" HANG).
//
//     The converted Java rebuilds EVERY dirty chunk in ONE frame:
//
//         int var2 = this->worldRenderersToUpdate.size() - 1;
//         int var3 = this->worldRenderersToUpdate.size();
//         for (int var4 = 0; var4 < var3; ++var4) {
//             WorldRenderer* var5 = ...worldRenderersToUpdate[var2 - var4];
//             if(var5->distanceToEntitySquared(var1) > 2500.0F && var4 > 4) return;  // desktop early-out
//             ...remove(var5); var5->updateRenderer(); var5->needsUpdate = false;
//         }
//
//     On a freshly generated Indev world that is HUNDREDS-to-1000+ dirty chunks, each a full 16x16x16 re-tessellation
//     and vertex-buffer bake, all in a single blocking call. sortAndRender is never reached -> the frame never presents
//     -> it looks hung on "Spawning". (On desktop the distance early-out bounds it because LWJGL display lists are
//     cheap; on console it is death.)
//
//     Fix (matches the OG Indev/Alpha hand-ports' documented "THE frame-rate fix"): cap chunk rebuilds to a small
//     BUDGET per frame, nearest-first. RenderSorter sorts nearest chunks to the BACK of the list (compare() returns 1
//     when the first argument is closer), so the loop pops from the back. Every not-yet-rebuilt chunk keeps
//     needsUpdate=true and gets its turn over the next frames; markBlocksForUpdate de-dupes the list so it drains
//     steadily.
//
// [2] EntityRenderer::hurtCameraEffect -- the INVISIBLE-WORLD NaN (the world renders but nothing is on screen).
//
//         float var3 = (float)var2->hurtTime - var1;   // spawn: hurtTime=0, partialTick=0 -> var3 = 0
//         ...
//         if(var3 >= 0.0F) {                            // 0 >= 0 -> TRUE, enters
//             var3 /= (float)var2->maxHurtTime;         // maxHurtTime=0 on spawn -> 0/0 = NaN
//             var3 = MathHelper::sin(var3 * ...);       // NaN propagates into a glRotatef -> the camera
//         }                                             // matrix becomes NaN -> ALL terrain maps off-screen
//
//     Fix (matches the reference / OG hand-port): also require hurtTime > 0 before entering, so the 0/0 never happens
//     on spawn.
//
// The conversion applies them on the IR before the back end writes (patch(), by tree shape), then checks the output for
// a surviving trigger (stranded()). rewrite()/run() are the same fixes on C++ text, for `codebreaker java render-fix
// <dir>` on a tree converted elsewhere; that text form runs over the converted tree (xbox360), BEFORE overlays.
// Idempotent (after each rewrite its trigger no longer matches) and fail-loud through the survivor checks.
#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/arena.hpp"
#include "cb/translate/ir.hpp"

namespace cb::java::projects::minecraft::render_fix {

// Chunks rebuilt per frame. 8 mirrors the OG hand-port; enough to fill the near view in a few frames, small enough that
// a single frame never stalls.
inline constexpr int kChunkBudget = 8;

// How many updateRenderers loops and hurtCameraEffect guards one file (or one class) had rewritten.
struct FileSites {
    std::string file;
    std::size_t update = 0;
    std::size_t hurt = 0;
};

struct Report {
    std::vector<FileSites> files;
    std::size_t update = 0;
    std::size_t hurt = 0;
    // Files whose trigger pattern SURVIVED its rewrite (the structure drifted: fail loud).
    std::vector<std::string> stranded;
};

struct Rewritten {
    std::string text;
    std::size_t update = 0;
    std::size_t hurt = 0;
};

// Apply both render fixes to one .cpp's text.
Rewritten rewrite(std::string_view text);

// Apply the console render fixes across the *.cpp files directly in a converted tree (rewritten in place).
Report run(const std::filesystem::path& out_dir);

// Both fixes on the lowered program (`units` are Unit nodes; new nodes are made in `arena`, the arena the tree lives
// in), matched by tree shape. One report entry per class, named <class>.cpp; `stranded` is empty.
Report patch(Arena& arena, const std::vector<translate::ir::Node*>& units);

// The *.cpp files of a converted tree where a render-fix trigger survived.
std::vector<std::string> stranded(const std::filesystem::path& out_dir);

}  // namespace cb::java::projects::minecraft::render_fix
