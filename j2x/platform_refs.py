# -*- coding: utf-8 -*-
r"""
platform_refs.py -- emit the hand-written platform layer instead of no-op stubs.

The converter normally emits SYMBOL-ONLY stubs for the platform subsystems (GL11, Audio, Input,
Socket, ...). When `--platform-refs` is on, this module emits the REAL, hand-written implementations
from `docs/platform/reference/*.reference.{h,cpp}` in their place -- the GL->D3D9 emulator, XACT audio,
XInput, XNet socket, etc., already proven on the Alpha port. The GL/platform API is version-independent,
so these are directly reusable; `platform_coverage.py` reports the small per-version delta (the MISSING
symbols) that still needs hand-filling.

This is what makes the port NOT blind: the converter ships the reference code AND knows (via
platform_coverage) exactly which symbols each version's game code needs that the reference doesn't yet
provide -- instead of discovering it one XDK build error at a time.

Caveat: a reference `.cpp` links against companion platform files that are NOT in the reference set
(stdafx.h, Common.h, Textures.h/.cpp, Tesselator.h/.cpp) -- those are game-agnostic and reused wholesale
from the Alpha live tree. `emit_references` writes a PLATFORM_LAYER.md listing what companions to copy in.
"""

import os


# A reference file often provides MORE than one header's worth of symbols. The game #includes the
# per-type stub header (Mouse.h / Keyboard.h / SoundSystem.h), so emitting only `Input.h` / `Audio.h`
# would leave the no-op stub shadowing the real impl -- the mouse-input-dead-on-controller trap. For
# each such reference we ALSO overwrite the stubs it replaces with a thin `#include` shim.
REFERENCE_REPLACES = {
    'Input': ['Keyboard', 'Mouse', 'Controllers'],   # Input.reference defines namespace Keyboard/Mouse/Controllers
    'Audio': ['SoundSystem'],                          # Audio.reference defines class SoundSystem
    'ImageIO': ['ImageIO'],
    'BufferedImage': ['BufferedImage'],
}


def available_references(reference_dir):
    """Base names X that have an X.reference.h in reference_dir (e.g. {'GL11','Audio','Socket',...})."""
    if not reference_dir or not os.path.isdir(reference_dir):
        return set()
    return set(f[:-len('.reference.h')] for f in os.listdir(reference_dir)
               if f.endswith('.reference.h'))


def _banner(base, ext):
    return ('// ============================ PLATFORM REFERENCE ============================\n'
            '// Hand-written Xbox 360 platform layer, emitted by j2x from\n'
            '//   docs/platform/reference/%s.reference.%s\n'
            '// This is the REAL implementation -- it REPLACES the converter no-op stub for %s.\n'
            '// (NOT converter-generated game code; edit the .reference.%s source, then reconvert.)\n'
            '// ==========================================================================\n\n'
            % (base, ext, base, ext))


# companion platform files a reference .cpp links against but that are NOT themselves references --
# reused game-agnostically from the Alpha live tree (…\MinecraftAlpha\MinecraftAlpha\).
_COMPANIONS = ['stdafx.h', 'stdafx.cpp', 'Common.h', 'Textures.h', 'Textures.cpp',
               'Tesselator.h', 'Tesselator.cpp', 'Main.cpp']


def emit_references(out_dir, reference_dir):
    """Write each X.reference.{h,cpp} to out_dir/X.{h,cpp} (with a provenance banner), replacing the
    emitted stub. Returns the set of base names emitted. Also writes PLATFORM_LAYER.md."""
    emitted = set()
    files_written = []
    for base in sorted(available_references(reference_dir)):
        for ext in ('h', 'cpp'):
            src = os.path.join(reference_dir, '%s.reference.%s' % (base, ext))
            if not os.path.isfile(src):
                continue
            with open(src, 'r', encoding='utf-8', errors='replace') as f:
                content = f.read()
            dst = os.path.join(out_dir, '%s.%s' % (base, ext))
            with open(dst, 'w', encoding='utf-8') as f:
                f.write(_banner(base, ext) + content)
            files_written.append('%s.%s' % (base, ext))
        # overwrite the per-type stub headers this reference replaces with a shim -> the real impl,
        # so the game's `#include "Mouse.h"` gets the controller mapping, not the no-op stub.
        for repl in REFERENCE_REPLACES.get(base, []):
            if repl == base:
                continue
            guard = repl.upper() + '_H'
            shim = ('// J2X: %s.h is provided by the hand-written %s platform reference.\n'
                    '#ifndef %s\n#define %s\n#include "%s.h"\n#endif // %s\n'
                    % (repl, base, guard, guard, base, guard))
            with open(os.path.join(out_dir, '%s.h' % repl), 'w', encoding='utf-8') as f:
                f.write(shim)
            files_written.append('%s.h (-> %s.h)' % (repl, base))
        emitted.add(base)
    _write_platform_layer_md(out_dir, sorted(emitted), files_written)
    return emitted


def _write_platform_layer_md(out_dir, emitted, files_written):
    L = ['# PLATFORM_LAYER.md -- the hand-written platform layer (emitted from references)', '',
         'The converter emitted the REAL Xbox 360 platform implementations (not no-op stubs) for these',
         'subsystems, from `docs/platform/reference/`:', '',
         '| Subsystem file | Emitted |', '|---|---|']
    for base in emitted:
        cpp = '%s.cpp' % base
        L.append('| `%s.h` %s | yes |' % (base, ('+ `%s`' % cpp) if cpp in files_written else '(header-only)'))
    L += ['', '## Companion files you must copy in', '',
          'A reference `.cpp` links against these game-agnostic platform files, which are NOT part of the',
          'reference set. Copy them from the Alpha live tree (`…\\MinecraftAlpha\\MinecraftAlpha\\`) -- they',
          'are version-independent and reused wholesale:', '']
    for c in _COMPANIONS:
        L.append('- `%s`' % c)
    L += ['', '## What is still missing', '',
          'See `PLATFORM_COVERAGE.md` -> the **MISSING** rows: symbols the game uses that neither the',
          'reference nor the stub provides. Those are the only per-version hand-fills left.', '']
    with open(os.path.join(out_dir, 'PLATFORM_LAYER.md'), 'w', encoding='utf-8') as f:
        f.write('\n'.join(L) + '\n')
