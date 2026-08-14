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
    'ImageIO': ['ImageIO'],
    # NIOBuffers.reference DEFINES ByteBuffer/FloatBuffer/IntBuffer (the real byte-granular NIO buffers the
    # GL emulator + Tessellator use). Without this, its class defs collide with the converter's ByteBuffer.h/
    # FloatBuffer.h/IntBuffer.h stubs -> C2011 redefinition in every TU that pulls both.
    'NIOBuffers': ['ByteBuffer', 'FloatBuffer', 'IntBuffer'],
}

# References that lack a Beta-specific method the converter's STUB already provides (and would otherwise
# shadow it), or that need companion types not yet bundled. SKIP emitting them -> the converter's stub
# (which the game already compiles+links against) stands. First-boot tradeoff: rendering/input/file-IO are
# real; audio/zip/socket/threading/image-decode fall back to the no-op stubs until each is reconciled.
#   Audio       -> SoundSystem.backgroundMusic (Beta-new) + needs XactAudio.h/.cpp companions
#   BufferedImage -> getRaster()/Raster/DataBuffer chain (added to the stub for Beta)
#   ZipFile     -> getEntry()->ZipEntry* (stub) + reference needs full InputStream/FileInputStream
#   Socket      -> getRemoteSocketAddress()->SocketAddress* (stub)
#   Thread      -> dumpStack()/join(1-arg) (stub)
REFERENCE_SKIP = {'Audio', 'BufferedImage', 'ZipFile', 'Socket', 'Thread'}


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
        if base in REFERENCE_SKIP:                    # keep the converter stub (see REFERENCE_SKIP note)
            continue
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

    # COMPANIONS: game-agnostic platform files a reference .cpp links against but that aren't themselves
    # references (Common.h, Textures.h/.cpp, Tesselator.h/.cpp). Bundled in docs/platform/companions/ and
    # emitted verbatim so the platform layer is SELF-CONTAINED -- no hand-copying from the Alpha tree.
    comp_dir = os.path.join(os.path.dirname(reference_dir), 'companions')
    if os.path.isdir(comp_dir):
        for fn in sorted(os.listdir(comp_dir)):
            if not (fn.endswith('.h') or fn.endswith('.cpp')):
                continue
            with open(os.path.join(comp_dir, fn), 'r', encoding='utf-8', errors='replace') as f:
                content = f.read()
            with open(os.path.join(out_dir, fn), 'w', encoding='utf-8') as f:
                f.write(_companion_banner(fn) + content)
            files_written.append(fn)
            emitted.add(fn[:-len('.cpp')] if fn.endswith('.cpp') else fn[:-len('.h')])

    # Main.cpp: the 360 entry point (defines main() + the D3D9 device + the boot sequence). It has a
    # Main.reference.cpp but no companion .h, so emit it here from the reference.
    main_ref = os.path.join(reference_dir, 'Main.reference.cpp')
    if os.path.isfile(main_ref):
        with open(main_ref, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
        with open(os.path.join(out_dir, 'Main.cpp'), 'w', encoding='utf-8') as f:
            f.write(_banner('Main', 'cpp') + content)
        files_written.append('Main.cpp')
        emitted.add('Main')

    _write_platform_layer_md(out_dir, sorted(emitted), files_written)
    return emitted


def _companion_banner(fn):
    return ('// ============================ PLATFORM COMPANION ============================\n'
            '// Game-agnostic Xbox 360 platform file, emitted by j2x from\n'
            '//   docs/platform/companions/%s\n'
            '// A reference .cpp links against this; it is version-independent (edit the companion\n'
            '// source, then reconvert). NOT converter-generated game code.\n'
            '// ==========================================================================\n\n' % fn)


def _write_platform_layer_md(out_dir, emitted, files_written):
    L = ['# PLATFORM_LAYER.md -- the hand-written platform layer (emitted from references)', '',
         'The converter emitted the REAL Xbox 360 platform implementations (not no-op stubs) for these',
         'subsystems, from `docs/platform/reference/`:', '',
         '| Subsystem file | Emitted |', '|---|---|']
    for base in emitted:
        cpp = '%s.cpp' % base
        L.append('| `%s.h` %s | yes |' % (base, ('+ `%s`' % cpp) if cpp in files_written else '(header-only)'))
    L += ['', '## Companion files (auto-emitted -- no hand-copying)', '',
          'The game-agnostic platform files a reference `.cpp` links against (Common.h, Textures.h/.cpp,',
          'Tesselator.h/.cpp) AND the 360 entry point (Main.cpp) are now emitted automatically from',
          '`docs/platform/companions/` + `Main.reference.cpp`. The full platform layer is self-contained;',
          'nothing needs to be copied from the Alpha tree.', '',
          '## What is still missing', '',
          'See `PLATFORM_COVERAGE.md` -> the **MISSING** rows: symbols the game uses that neither the',
          'reference nor the stub provides. Those are the only per-version hand-fills left.', '']
    with open(os.path.join(out_dir, 'PLATFORM_LAYER.md'), 'w', encoding='utf-8') as f:
        f.write('\n'.join(L) + '\n')
