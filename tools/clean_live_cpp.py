# -*- coding: utf-8 -*-
"""Apply the warning-cleanup passes (C4554 parens, C4101 unused catch vars, C4102 dead labels,
C4566 unicode, C4800 count-as-bool) DIRECTLY to the live Xbox project's game .cpp files.

We patch the live tree in place rather than regenerating + re-swapping, because the live project has
hand divergences that the converter/overrides don't carry (e.g. Minecraft.h's concrete
displayUnexpectedThrowable, the platform shim headers) -- a wholesale resync would clobber them. The
passes here are identical and idempotent to what the converter now emits, so the live game files end
up matching canonical output without disturbing the platform layer.

The platform layer .cpp (real D3D9/XInput/IO/audio impls) are EXCLUDED -- they are hand-written and
synced from docs/platform/*.reference.*, and have none of these decompiler-artifact warnings.

Run from the ConversionTool dir:  python tools/clean_live_cpp.py
"""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)
from tools.clean_override_warnings import clean   # noqa: E402

LIVE = os.path.normpath(os.path.join(ROOT, '..', 'MinecraftAlpha'))

# platform-layer .cpp (hand-written; synced from docs/platform) -- never auto-rewrite these.
PLATFORM = {
    'GL11.cpp', 'GLU.cpp', 'NIOBuffers.cpp', 'FileIO.cpp', 'ImageIO.cpp', 'Audio.cpp', 'Input.cpp',
    'Shaders.cpp', 'Tesselator.cpp', 'Tessellator.cpp', 'Save360.cpp', 'ContentStorage.cpp',
    'Debug.cpp', 'Textures.cpp', 'Textures_ext.cpp', 'Main.cpp', 'stdafx.cpp', 'XactAudio.cpp',
}


def main():
    if not os.path.isdir(LIVE):
        print('LIVE project not found:', LIVE)
        return
    changed = []
    for fn in sorted(os.listdir(LIVE)):
        if not fn.endswith('.cpp') or fn in PLATFORM:
            continue
        p = os.path.join(LIVE, fn)
        with open(p, encoding='utf-8', newline='') as f:
            src = f.read()
        nl = '\r\n' if '\r\n' in src else '\n'
        body = src.replace('\r\n', '\n')
        out = clean(body)
        if out != body:
            with open(p, 'w', encoding='utf-8', newline='') as f:
                f.write(out.replace('\n', nl))
            changed.append(fn)
    print('cleaned %d live .cpp files' % len(changed))
    for c in changed:
        print('  ', c)


if __name__ == '__main__':
    main()
