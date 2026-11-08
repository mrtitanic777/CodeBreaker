// codebreaker/java/projects/minecraft/render_fix.hpp -- C++ port of codebreaker.java.projects.minecraft.render_fix.
//
// render_fix.py -- two CONSOLE render-correctness fixes for the first in-game frame.
//
// Both are faithful Java->C++ translations that are correct on desktop LWJGL but WRONG on the Xbox 360
// render path, both are present in every chunk-streaming edition (Indev/Alpha/Beta share the code), and
// both are fixed the same way in the proven OG hand-ports. Migrated here so every edition gets them.
//
// -------------------------------------------------------------------------------------------------------
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
//     On a freshly generated Indev world that is HUNDREDS-to-1000+ dirty chunks, each a full 16x16x16
//     re-tessellation + vertex-buffer bake, all in a single blocking call. sortAndRender is never reached
//     -> the frame never presents -> looks hung on "Spawning". (On desktop the distance early-out bounds
//     it because LWJGL display lists are cheap; on console it is death.)
//
//     Fix (matches the OG Indev/Alpha hand-ports' documented "THE frame-rate fix"): cap chunk rebuilds to
//     a small BUDGET per frame, nearest-first. RenderSorter sorts nearest chunks to the BACK of the list
//     (compare() returns 1 when the first arg is closer), so we pop from the back. Every not-yet-rebuilt
//     chunk keeps needsUpdate=true and gets its turn over the next frames; markBlocksForUpdate de-dupes
//     the list so it drains steadily.
//
// -------------------------------------------------------------------------------------------------------
// [2] EntityRenderer::hurtCameraEffect -- the INVISIBLE-WORLD NaN (world renders but nothing is on screen).
//
//         float var3 = (float)var2->hurtTime - var1;   // spawn: hurtTime=0, partialTick=0 -> var3 = 0
//         ...
//         if(var3 >= 0.0F) {                            // 0 >= 0 -> TRUE, enters
//             var3 /= (float)var2->maxHurtTime;         // maxHurtTime=0 on spawn -> 0/0 = NaN
//             var3 = MathHelper::sin(var3 * ...);       // NaN propagates into a glRotatef -> the camera
//         }                                             // matrix becomes NaN -> ALL terrain maps off-screen
//
//     Fix (matches the reference / OG hand-port): also require hurtTime > 0 before entering, so the 0/0
//     never happens on spawn.
//
// -------------------------------------------------------------------------------------------------------
// convert applies it on the IR before the back end writes (patch(), by tree shape), then checks the output
// for a surviving trigger (stranded()). run()/process() are the same fix on C++ text, for `codebreaker java <fix> <dir>` on
// a tree converted elsewhere; that text form runs over the converted tree (xbox360), BEFORE overlays. Idempotent (after each
// rewrite its trigger no longer matches) and fail-loud via run()'s survivor checks. Also exposed as
// `codebreaker java render-fix <dir>`.
//
// Typing notes:
//   * the Python's result dicts {'files': [{'file','update','hurt'}...], 'update': N, 'hurt': N, 'stranded': [files]}
//     are the structs Touched and Result (run() and patch() return the same record);
//   * patch() takes the units (Unit nodes) of the lowered program; _budget and _hurt take the statement list /
//     statement they test (the Python also passed the ir and rewrite modules, which are plain namespaces here).
#pragma once

#include <cstdint>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"
#include "codebreaker/translate/ir.hpp"

namespace codebreaker::java::projects::minecraft::render_fix {

// --- [1] updateRenderers: the all-chunks-in-one-frame rebuild loop --------------------------------
// Captures the converter-generated names so it is edition-agnostic:
//   G1 field (worldRenderersToUpdate)  G2 size-holder var  G3 loop-bound var  G4 loop var
//   G5 WorldRenderer type              G6 chunk var        G7 player var
extern const py::re::Pattern _UPDATE_RENDERERS;

// Rebuild budget per frame. 8 mirrors the OG hand-port; enough to fill the near view in a few frames,
// small enough that a single frame never stalls.
extern const int64_t _BUDGET;

py::str _update_renderers_repl(const py::re::Match& m);

// survivor: the original all-chunks loop's tell-tale (the desktop distance early-out) still present.
extern const py::re::Pattern _UPDATE_SURVIVOR;

// --- [2] hurtCameraEffect: the 0/0 NaN on spawn ---------------------------------------------------
//   G1 var3   G2 whitespace   G3 var3 again   G4 var2 (player)
extern const py::re::Pattern _HURT;

py::str _hurt_repl(const py::re::Match& m);

// survivor: an unguarded `if(x >= 0.0F) {  x /= (float)y->maxHurtTime;` still present.
extern const py::re::Pattern _HURT_SURVIVOR;

// Apply both render fixes to one .cpp. Returns (new_text, n_update, n_hurt).
std::tuple<py::str, int64_t, int64_t> process(const py::str& text);

// One file the pass changed: {'file': name, 'update': n, 'hurt': n}.
struct Touched {
    py::str file;
    int64_t update = 0;
    int64_t hurt = 0;
};

// {'files': [...], 'update': N, 'hurt': N, 'stranded': [files]}
struct Result {
    py::list<Touched> files;
    int64_t update = 0;
    int64_t hurt = 0;
    py::list<py::str> stranded;
};

// Apply the console render fixes across the converted tree. Returns
// {'files': [{'file','update','hurt'}...], 'update': N, 'hurt': N, 'stranded': [files]}.
// `stranded` = a file whose trigger pattern SURVIVED its rewrite (structure drifted -> fail loud).
Result run(const py::str& out_dir);

// ---- the same fixes on the IR, before the back end writes (what convert runs) ----------------------------
// Both render fixes on the lowered program, matched by tree shape. Returns
// {'files': [{'file','update','hurt'}], 'update': N, 'hurt': N, 'stranded': []} like run().
Result patch(const py::list<codebreaker::translate::ir::NodeP>& units);

// [1] at ss[i]:
//     int A = this.F.size() - 1;  int B = this.F.size();
//     for (int L = 0; L < B; ++L) {
//         T W = (T)this.F.get(A - L);
//         if (W.distanceToEntitySquared(P) > 2500.0F && L > 4) return;
//         this.F.remove(W); W.updateRenderer(); W.needsUpdate = false;
//     }
// ->  int B = _BUDGET;
//     for (int L = 0; L < B; ++L) {
//         int A = this.F.size();  if (A == 0) return;
//         T W = (T)this.F.get(A - 1);  this.F.remove(W);
//         if (W != null) { W.updateRenderer(); W.needsUpdate = false; }
//     }
bool _budget(const py::list<codebreaker::translate::ir::NodeP>& ss, int64_t i);

// [2] `if (X >= 0.0F) { X /= (float)Y.maxHurtTime; ...` -> `if (X >= 0.0F && Y.hurtTime > 0)`.
bool _hurt(const codebreaker::translate::ir::NodeP& s);

// The .cpp files where a render-fix trigger survived.
py::list<py::str> stranded(const py::str& out_dir);

}  // namespace codebreaker::java::projects::minecraft::render_fix
