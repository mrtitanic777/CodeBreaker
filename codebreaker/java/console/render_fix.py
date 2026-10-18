# -*- coding: utf-8 -*-
r"""
render_fix.py -- two CONSOLE render-correctness fixes for the first in-game frame.

Both are faithful Java->C++ translations that are correct on desktop LWJGL but WRONG on the Xbox 360
render path, both are present in every chunk-streaming edition (Indev/Alpha/Beta share the code), and
both are fixed the same way in the proven OG hand-ports. Migrated here so every edition gets them.

-------------------------------------------------------------------------------------------------------
[1] RenderGlobal::updateRenderers -- THE frame-rate fix (the "stuck on Spawning / never draws" HANG).

    The converted Java rebuilds EVERY dirty chunk in ONE frame:

        int var2 = this->worldRenderersToUpdate.size() - 1;
        int var3 = this->worldRenderersToUpdate.size();
        for (int var4 = 0; var4 < var3; ++var4) {
            WorldRenderer* var5 = ...worldRenderersToUpdate[var2 - var4];
            if(var5->distanceToEntitySquared(var1) > 2500.0F && var4 > 4) return;  // desktop early-out
            ...remove(var5); var5->updateRenderer(); var5->needsUpdate = false;
        }

    On a freshly generated Indev world that is HUNDREDS-to-1000+ dirty chunks, each a full 16x16x16
    re-tessellation + vertex-buffer bake, all in a single blocking call. sortAndRender is never reached
    -> the frame never presents -> looks hung on "Spawning". (On desktop the distance early-out bounds
    it because LWJGL display lists are cheap; on console it is death.)

    Fix (matches the OG Indev/Alpha hand-ports' documented "THE frame-rate fix"): cap chunk rebuilds to
    a small BUDGET per frame, nearest-first. RenderSorter sorts nearest chunks to the BACK of the list
    (compare() returns 1 when the first arg is closer), so we pop from the back. Every not-yet-rebuilt
    chunk keeps needsUpdate=true and gets its turn over the next frames; markBlocksForUpdate de-dupes
    the list so it drains steadily.

-------------------------------------------------------------------------------------------------------
[2] EntityRenderer::hurtCameraEffect -- the INVISIBLE-WORLD NaN (world renders but nothing is on screen).

        float var3 = (float)var2->hurtTime - var1;   // spawn: hurtTime=0, partialTick=0 -> var3 = 0
        ...
        if(var3 >= 0.0F) {                            // 0 >= 0 -> TRUE, enters
            var3 /= (float)var2->maxHurtTime;         // maxHurtTime=0 on spawn -> 0/0 = NaN
            var3 = MathHelper::sin(var3 * ...);       // NaN propagates into a glRotatef -> the camera
        }                                             // matrix becomes NaN -> ALL terrain maps off-screen

    Fix (matches the reference / OG hand-port): also require hurtTime > 0 before entering, so the 0/0
    never happens on spawn.

-------------------------------------------------------------------------------------------------------
Runs as a post-emit pass over the converted tree (xbox360), BEFORE overlays. Idempotent (after each
rewrite its trigger no longer matches) and fail-loud via run()'s survivor checks. Also exposed as
`j2ctool render-fix <dir>`.
"""

import os
import re

# --- [1] updateRenderers: the all-chunks-in-one-frame rebuild loop --------------------------------
# Captures the converter-generated names so it is edition-agnostic:
#   G1 field (worldRenderersToUpdate)  G2 size-holder var  G3 loop-bound var  G4 loop var
#   G5 WorldRenderer type              G6 chunk var        G7 player var
_UPDATE_RENDERERS = re.compile(
    r'int\s+(\w+)\s*=\s*this->(\w+)\.size\(\)\s*-\s*1;\s*'          # int var2 = this->FIELD.size() - 1;
    r'int\s+(\w+)\s*=\s*this->\2\.size\(\);\s*'                     # int var3 = this->FIELD.size();
    r'for\s*\(int\s+(\w+)\s*=\s*0;\s*\4\s*<\s*\3;\s*\+\+\4\)\s*\{\s*'
    r'(\w+)\*\s*(\w+)\s*=\s*\(\5\*\)this->\2\[\1\s*-\s*\4\];\s*'    # WorldRenderer* var5 = (..)FIELD[var2-var4];
    r'if\(\6->distanceToEntitySquared\((\w+)\)\s*>\s*2500\.0F\s*&&\s*\4\s*>\s*4\)\s*\{\s*'
    r'return;\s*\}\s*'
    r'this->\2\.remove\(\6\);\s*'
    r'\6->updateRenderer\(\);\s*'
    r'\6->needsUpdate\s*=\s*false;\s*'
    r'\}')

# Rebuild budget per frame. 8 mirrors the OG hand-port; enough to fill the near view in a few frames,
# small enough that a single frame never stalls.
_BUDGET = 8


def _update_renderers_repl(m):
    field, szv, boundv, loopv, wr, chunk = (
        m.group(2), m.group(1), m.group(3), m.group(4), m.group(5), m.group(6))
    return (
        '// J2X render-budget: rebuild at most {n} chunks/frame (nearest first), not the whole dirty\n'
        '\t// set in one blocking frame -- a fresh world is 1000+ dirty chunks and would never present\n'
        '\t// (stuck on "Spawning"). The rest keep needsUpdate and rebuild over the next frames. '
        '[generator: render_fix.py]\n'
        '\tint {bound} = {n};\n'
        '\tfor (int {loop} = 0; {loop} < {bound}; ++{loop}) {{\n'
        '\tint {sz} = this->{field}.size();\n'
        '\tif({sz} == 0) {{\n'
        '\treturn;\n'
        '\t}}\n'
        '\t{wr}* {chunk} = ({wr}*)this->{field}[{sz} - 1];\n'
        '\tthis->{field}.remove({chunk});\n'
        '\tif({chunk} != NULL) {{\n'
        '\t{chunk}->updateRenderer();\n'
        '\t{chunk}->needsUpdate = false;\n'
        '\t}}\n'
        '\t}}'
    ).format(n=_BUDGET, field=field, sz=szv, bound=boundv, loop=loopv, wr=wr, chunk=chunk)


# survivor: the original all-chunks loop's tell-tale (the desktop distance early-out) still present.
_UPDATE_SURVIVOR = re.compile(r'distanceToEntitySquared\(\w+\)\s*>\s*2500\.0F\s*&&\s*\w+\s*>\s*4')


# --- [2] hurtCameraEffect: the 0/0 NaN on spawn ---------------------------------------------------
#   G1 var3   G2 whitespace   G3 var3 again   G4 var2 (player)
_HURT = re.compile(
    r'if\((\w+)\s*>=\s*0\.0F\)\s*\{(\s*)(\w+)\s*/=\s*\(float\)(\w+)->maxHurtTime;')


def _hurt_repl(m):
    v3, ws, v3b, v2 = m.group(1), m.group(2), m.group(3), m.group(4)
    return ('if({v3} >= 0.0F && {v2}->hurtTime > 0) {{  // J2X hurt-NaN guard: on spawn hurtTime=0 & '
            'partialTick=0 -> {v3}=0 entered -> 0/0 NaN camera -> invisible world [render_fix.py]{ws}'
            '{v3b} /= (float){v2}->maxHurtTime;').format(v3=v3, v2=v2, v3b=v3b, ws=ws)


# survivor: an unguarded `if(x >= 0.0F) {  x /= (float)y->maxHurtTime;` still present.
_HURT_SURVIVOR = re.compile(r'if\(\w+\s*>=\s*0\.0F\)\s*\{\s*\w+\s*/=\s*\(float\)\w+->maxHurtTime;')


def process(text):
    """Apply both render fixes to one .cpp. Returns (new_text, n_update, n_hurt)."""
    text, n_update = _UPDATE_RENDERERS.subn(_update_renderers_repl, text)
    text, n_hurt = _HURT.subn(_hurt_repl, text)
    return text, n_update, n_hurt


def run(out_dir):
    """Apply the console render fixes across the converted tree. Returns
    {'files': [{'file','update','hurt'}...], 'update': N, 'hurt': N, 'stranded': [files]}.
    `stranded` = a file whose trigger pattern SURVIVED its rewrite (structure drifted -> fail loud)."""
    files = [f for f in sorted(os.listdir(out_dir)) if f.endswith('.cpp')]
    touched, tot_u, tot_h, stranded = [], 0, 0, []
    for f in files:
        path = os.path.join(out_dir, f)
        try:
            text = open(path, 'r', encoding='utf-8', errors='replace').read()
        except (IOError, OSError):
            continue
        has_update = 'worldRenderersToUpdate' in text and 'distanceToEntitySquared' in text
        has_hurt = 'maxHurtTime' in text
        if not (has_update or has_hurt):
            continue
        new, n_u, n_h = process(text)
        if new != text:
            with open(path, 'w', encoding='utf-8') as fh:
                fh.write(new)
            touched.append({'file': f, 'update': n_u, 'hurt': n_h})
            tot_u += n_u
            tot_h += n_h
        if _UPDATE_SURVIVOR.search(new) or _HURT_SURVIVOR.search(new):
            stranded.append(f)
    return {'files': touched, 'update': tot_u, 'hurt': tot_h, 'stranded': stranded}
