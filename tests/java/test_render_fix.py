# -*- coding: utf-8 -*-
"""Regressions for render_fix.py -- the two console render fixes for the first in-game frame:
(1) RenderGlobal::updateRenderers chunk-rebuild budget (else the frame never presents -> "Spawning" hang),
(2) EntityRenderer::hurtCameraEffect hurtTime>0 guard (else 0/0 NaN -> invisible world on spawn)."""

import os
import tempfile

from codebreaker.java.console import render_fix


# The converter's all-chunks-in-one-frame updateRenderers (flat single-tab brace style, as emitted).
_UPDATE = (
    'void RenderGlobal::updateRenderers(EntityPlayer* var1)\n{\n'
    '\tCollections::sort(this->worldRenderersToUpdate, new RenderSorter(var1));\n'
    '\tint var2 = this->worldRenderersToUpdate.size() - 1;\n'
    '\tint var3 = this->worldRenderersToUpdate.size();\n'
    '\tfor (int var4 = 0; var4 < var3; ++var4) {\n'
    '\tWorldRenderer* var5 = (WorldRenderer*)this->worldRenderersToUpdate[var2 - var4];\n'
    '\tif(var5->distanceToEntitySquared(var1) > 2500.0F && var4 > 4) {\n'
    '\treturn;\n'
    '\t}\n'
    '\tthis->worldRenderersToUpdate.remove(var5);\n'
    '\tvar5->updateRenderer();\n'
    '\tvar5->needsUpdate = false;\n'
    '\t}\n}\n'
)

_HURT = (
    'void EntityRenderer::hurtCameraEffect(float var1)\n{\n'
    '\tEntityPlayerSP* var2 = this->mc->thePlayer;\n'
    '\tfloat var3 = (float)var2->hurtTime - var1;\n'
    '\tif(var2->health <= 0) {\n\tvar1 += (float)var2->deathTime;\n\t}\n'
    '\tif(var3 >= 0.0F) {\n'
    '\tvar3 /= (float)var2->maxHurtTime;\n'
    '\tvar3 = MathHelper::sin(var3 * var3 * (float)3.1415927F);\n'
    '\t}\n}\n'
)


def test_update_renderers_budget():
    new, n_u, n_h = render_fix.process(_UPDATE)
    assert n_u == 1 and n_h == 0
    # the desktop all-chunks early-out is gone
    assert '> 2500.0F && var4 > 4' not in new
    assert 'size() - 1' not in new
    # a fixed per-frame budget now bounds it, and the sort (nearest-to-back) is preserved
    assert 'int var3 = 8;' in new
    assert 'Collections::sort(this->worldRenderersToUpdate' in new
    # nearest-first: pops the back of the sorted list
    assert 'this->worldRenderersToUpdate[var2 - 1]' in new
    # still rebuilds + clears the dirty flag, now NULL-guarded
    assert 'var5->updateRenderer();' in new
    assert 'var5->needsUpdate = false;' in new
    assert 'if(var5 != NULL) {' in new


def test_hurt_guard():
    new, n_u, n_h = render_fix.process(_HURT)
    assert n_u == 0 and n_h == 1
    assert 'if(var3 >= 0.0F && var2->hurtTime > 0) {' in new
    # the division is preserved (now unreachable on spawn)
    assert 'var3 /= (float)var2->maxHurtTime;' in new


def test_idempotent():
    once, _, _ = render_fix.process(_UPDATE + _HURT)
    twice, n_u, n_h = render_fix.process(once)
    assert n_u == 0 and n_h == 0 and twice == once


def test_run_reports_and_no_survivor():
    d = tempfile.mkdtemp()
    with open(os.path.join(d, 'RenderGlobal.cpp'), 'w', encoding='utf-8') as f:
        f.write(_UPDATE)
    with open(os.path.join(d, 'EntityRenderer.cpp'), 'w', encoding='utf-8') as f:
        f.write(_HURT)
    with open(os.path.join(d, 'Other.cpp'), 'w', encoding='utf-8') as f:
        f.write('void Other::f() {}\n')
    res = render_fix.run(d)
    assert res['update'] == 1 and res['hurt'] == 1
    assert res['stranded'] == []


def test_unrelated_code_untouched():
    src = ('void RenderGlobal::g() {\n\tint var2 = this->list.size() - 1;\n'
           '\tfor (int i = 0; i < 3; ++i) {}\n}\n')
    new, n_u, n_h = render_fix.process(src)
    assert n_u == 0 and n_h == 0 and new == src
