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
    # FIXED BUG audio-not-emitted: Audio.reference DEFINES the real SoundSystem (paulscode -> XACT3 bridge),
    # but the game #includes "SoundSystem.h" -- so shim that stub to the real Audio.h, else the no-op
    # SoundSystem stub shadows the bridge and every playSoundFX is silent (and the empty pool crashed).
    'Audio': ['SoundSystem'],
    # NIOBuffers.reference DEFINES ByteBuffer/FloatBuffer/IntBuffer (the real byte-granular NIO buffers the
    # GL emulator + Tessellator use). Without this, its class defs collide with the converter's ByteBuffer.h/
    # FloatBuffer.h/IntBuffer.h stubs -> C2011 redefinition in every TU that pulls both.
    'NIOBuffers': ['ByteBuffer', 'FloatBuffer', 'IntBuffer'],
    # FileIO no longer SHIMS the byte streams. FIXED BUG filestream-shim-redefines-FileOutputStream: the old
    # shim (OutputStream.h -> #include FileIO.h) exploded in save-path TUs -- FileOutputStream.h includes
    # OutputStream.h -> pulled in all of FileIO.h -> which defines FileOutputStream -> then FileOutputStream.h
    # redefined it (C2011). The real java.io byte streams (OutputStream/BufferedOutputStream/DataInput/DataOutput/
    # DataInputStream/DataOutputStream) are now CANONICAL standalone headers emitted by runtime.py (loaded from
    # docs/platform/canonical/), and FileIO.reference.h just #includes them -- one definition each, no shim, no
    # redefinition. Same pattern as InputStream/File/FileInputStream.
}

# References that lack a Beta-specific method the converter's STUB already provides (and would otherwise
# shadow it), or that need companion types not yet bundled. SKIP emitting them -> the converter's stub
# (which the game already compiles+links against) stands. First-boot tradeoff: rendering/input/file-IO are
# real; audio/zip/socket/threading/image-decode fall back to the no-op stubs until each is reconciled.
#   ZipFile     -> getEntry()->ZipEntry* (stub) + reference needs full InputStream/FileInputStream
#   Socket      -> getRemoteSocketAddress()->SocketAddress* (stub)
#   Thread      -> dumpStack()/join(1-arg) (stub). REVERTED to the STUB: making java.lang.Thread real
#                  (CreateThread on every start()) turned on the dead-on-console daemon threads
#                  (StatsSyncher/ThreadDownload*/timer) and froze the first menu render frame. The game's
#                  real worker pool is j2x_pool (ThreadPool.h, CreateThread directly) -- it does NOT need
#                  java Thread to be real. When multiplayer is reachable, give the net reader/writer threads
#                  a controlled real-thread path rather than flipping this global. (Reference kept for that.)
# NOT skipped: BufferedImage -- its reference is the REAL ARGB pixel buffer that ImageIO.reference decodes
# PNGs into (w/h/pixels fields), and it now carries getRaster()/flush()/createGraphics() too.
# FIXED BUG audio-not-emitted: Audio was UN-SKIPPED. Its two blockers are resolved -- the Beta-new
# SoundSystem::backgroundMusic(...) catch-all now exists in Audio.reference.h, and the XactAudio.h/.cpp
# companion (the real IXACT3Engine + wave-bank loader) is now bundled in docs/platform/companions/. So the
# XACT3 questionnaire choice finally emits the real paulscode->XACT3 bridge instead of the no-op stub.
# (Route A / XACT3 is THE Xbox-360 audio path; reference emission only runs for the xbox360 platform.)
# FIXED BUG net-socket-stubbed: Socket was UN-SKIPPED. Its blocker (the reference's getRemoteSocketAddress()
# returned void* but the game assigns it to a SocketAddress*) is resolved -- Socket.reference.h now returns
# SocketAddress*/InetAddress*, and the InetAddress shim carries the host so Socket(InetAddress,port) connects.
# So the real Winsock/XNet TCP socket finally emits instead of the no-op stub -> multiplayer can connect.
# FIXED BUG mp-threads-never-run: Thread was UN-SKIPPED. It was skipped because the OLD reference made EVERY
# start() a real CreateThread, which spun up the dead download/stats daemons and froze the first menu frame.
# Thread.reference.h is now the GATED version -- start() is a virtual NO-OP by default (daemons stay dead), and
# only the net reader/writer/connect thread classes override start() (in the overlay) to spawn a real thread.
# So MP's threads run for real while the menu-freeze does not return.
REFERENCE_SKIP = {'ZipFile'}


def available_references(reference_dir):
    """Base names X that have an X.reference.h in reference_dir (e.g. {'GL11','Audio','Socket',...})."""
    if not reference_dir or not os.path.isdir(reference_dir):
        return set()
    return set(f[:-len('.reference.h')] for f in os.listdir(reference_dir)
               if f.endswith('.reference.h'))


def detect_main_class(classes):
    """The game's entry class: the one declaring `public static void main(String[])`. Returns its
    name, or None. Detection-driven -- NO hardcoded game name -- so the generic entry point can invoke
    whatever main class the converted game actually has (RubyDung, Minecraft, anything)."""
    for c in classes:
        for m in getattr(c, 'methods', ()):
            if getattr(m, 'name', None) != 'main' or not getattr(m, 'is_static', False):
                continue
            params = getattr(m, 'params', [])
            if len(params) != 1:
                continue
            pt = params[0].jtype
            if getattr(pt, 'is_array', False) and pt.name in ('String', 'java.lang.String'):
                return c.name
    return None


def generate_entry_point(main_class):
    """The GENERIC Xbox 360 entry point, auto-generated from the detected main class -- no game
    hardcoding. It runs ordered static init then hands control to the game's OWN `main(String[])`,
    which drives display creation + the game loop through the platform shims (Display/GL/Input).
    The platform device is created lazily by the Display/GL layer when the game calls Display.create,
    so the entry itself carries no Beta-specific bootstrap (texture preloads, MinecraftImpl, Shaders)."""
    return (
        '// ============================ GENERIC ENTRY POINT ============================\n'
        '// Auto-generated by j2ctool. The 360 title entry: run ordered Java static init, then call\n'
        '// the game\'s detected main class. Device/display setup is driven by the game\'s own code\n'
        '// through the platform shims (Display/GL11/Keyboard) -- no game-specific bootstrap here.\n'
        '// (Detected main class: %s. Regenerated every convert from whatever main() the game has.)\n'
        '// ==========================================================================\n\n'
        '#include "stdafx.h"\n'
        '#include "JavaRT.h"\n'
        '#include "j2x_init.h"     // generated: void j2x_initAll();\n'
        '#include "%s.h"\n\n'
        'void __cdecl main() {\n'
        '    // Ordered static init: every class\'s Java static{} blocks + static field initializers,\n'
        '    // in dependency order (prevents the static-init-order null-vtable crash class).\n'
        '    j2x_initAll();\n\n'
        '    // Hand off to the game\'s own entry point. No command-line args on console.\n'
        '    JArray<JavaString> args(0);   // Java String[] -> JArray<JavaString> (JavaString is a value type)\n'
        '    %s::main(args);\n'
        '}\n'
        % (main_class, main_class, main_class))


# References that are legitimately .cpp-only (no stub .h to replace): Main is the standalone entry point.
_SPECIAL_CPP_ONLY = frozenset(['Main'])


def audit_references(reference_dir):
    """Classify every reference so NONE can silently fail to integrate (the GLU flat-world bug: a
    GLU.reference.cpp existed but its GLU.reference.h did NOT, so emit_references -- which keys off .h --
    never emitted it and the game kept the no-op GLU stub -> flat 3D world).

    A reference X REPLACES the no-op X.h stub, so it needs an X.reference.h. An X.reference.cpp WITHOUT a
    matching .h can't replace the stub -> its real definitions are shadowed by the stub's inline no-ops.
    Returns {'complete': set(bases with .h), 'orphans': sorted[.cpp-only bases, excl. Main / *.ext]}."""
    if not reference_dir or not os.path.isdir(reference_dir):
        return {'complete': set(), 'orphans': []}
    files = os.listdir(reference_dir)
    hs = set(f[:-len('.reference.h')] for f in files if f.endswith('.reference.h'))
    cpps = set(f[:-len('.reference.cpp')] for f in files
               if f.endswith('.reference.cpp') and not f.endswith('.ext.reference.cpp'))
    # a base whose header ships as a COMPANION (e.g. Textures.h) is already covered -> not an orphan
    comp_dir = os.path.join(os.path.dirname(reference_dir), 'companions')
    comp_hs = set(f[:-2] for f in os.listdir(comp_dir) if f.endswith('.h')) if os.path.isdir(comp_dir) else set()
    orphans = sorted(b for b in (cpps - hs) if b not in _SPECIAL_CPP_ONLY and b not in comp_hs)
    return {'complete': hs, 'orphans': orphans}


def verify_integrated(out_dir, bases):
    """After emission, confirm each reference base's tree X.h actually carries the PLATFORM REFERENCE banner
    -- i.e. the stub was REALLY replaced, not silently left as the no-op. Returns the bases that did NOT
    integrate (empty list = every reference landed)."""
    bad = []
    for base in sorted(bases):
        fp = os.path.join(out_dir, '%s.h' % base)
        try:
            with open(fp, 'r', encoding='utf-8', errors='replace') as f:
                head = f.read(400)
        except (IOError, OSError):
            bad.append(base)
            continue
        if 'PLATFORM REFERENCE' not in head:
            bad.append(base)
    return bad


def _banner(base, ext):
    return ('// ============================ PLATFORM REFERENCE ============================\n'
            '// Hand-written Xbox 360 platform layer, emitted by j2ctool from\n'
            '//   docs/platform/reference/%s.reference.%s\n'
            '// This is the REAL implementation -- it REPLACES the converter no-op stub for %s.\n'
            '// (NOT converter-generated game code; edit the .reference.%s source, then reconvert.)\n'
            '// ==========================================================================\n\n'
            % (base, ext, base, ext))


# companion platform files a reference .cpp links against but that are NOT themselves references --
# reused game-agnostically from the Alpha live tree (…\MinecraftAlpha\MinecraftAlpha\).
_COMPANIONS = ['stdafx.h', 'stdafx.cpp', 'Common.h', 'Textures.h', 'Textures.cpp',
               'Tesselator.h', 'Tesselator.cpp', 'Main.cpp',
               'HeapTrack.h', 'HeapTrack.cpp',   # global heap-leak watchdog (overrides operator new/delete)
               'J2xPool.h']   # slab pool for high-churn small objects (scheduled ticks) -> no heap fragmentation


def emit_references(out_dir, reference_dir, game_classes=frozenset(), main_class=None):
    """Write each X.reference.{h,cpp} to out_dir/X.{h,cpp} (with a provenance banner), replacing the
    emitted stub. Returns the set of base names emitted. Also writes PLATFORM_LAYER.md.

    Detection-driven, NO game hardcoding:
      * game_classes -- names the CONVERTED game defines itself. A platform reference/companion whose
        base collides with one of these is SKIPPED: the game's own converted class wins (e.g. RubyDung
        defines Textures/Tesselator/Vertex, so we never emit a Beta Textures/Tesselator over them --
        that was the C2039 genId / Textures.ext mismatch). The agnostic platform layer only provides
        the API surface the game IMPORTS but does not define (GL/GLU/NIO/Display/input/system).
      * main_class -- if given, the 360 entry point is GENERATED generically to call
        <main_class>::main(...), instead of the Beta-specific Main.reference.cpp (Minecraft.h include)."""
    emitted = set()
    files_written = []
    game_classes = set(game_classes or ())

    def _collides(base):
        """A platform file collides if the game defines a class of the same name (its converted
        .h/.cpp already occupies that slot). Also covers X.ext against a game-defined X."""
        root = base[:-4] if base.endswith('.ext') else base
        return base in game_classes or root in game_classes
    # Clean up references that USED to be emitted but are now skipped: a prior convert wrote X.cpp/X.h
    # (carrying the PLATFORM REFERENCE banner); leaving them makes the vcxproj still compile the stale
    # reference (e.g. a skipped Audio.cpp -> unresolved XactAudio.h). Delete only OUR banner'd files, never
    # a stub the current run just wrote (stubs carry no banner) or a user file.
    for base in sorted(REFERENCE_SKIP):
        for ext in ('cpp', 'h'):
            fp = os.path.join(out_dir, '%s.%s' % (base, ext))
            if os.path.isfile(fp):
                try:
                    with open(fp, 'r', encoding='utf-8', errors='replace') as f:
                        head = f.read(400)
                    if 'PLATFORM REFERENCE' in head or 'PLATFORM COMPANION' in head:
                        os.remove(fp)
                except OSError:
                    pass
    skipped_collisions = []
    for base in sorted(available_references(reference_dir)):
        if base in REFERENCE_SKIP:                    # keep the converter stub (see REFERENCE_SKIP note)
            continue
        if _collides(base):                           # game defines its own -> its converted class wins
            skipped_collisions.append(base)
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
            comp_base = fn[:-4] if fn.endswith('.cpp') else fn[:-2]
            if comp_base == 'Main' and main_class:    # generic entry replaces the Beta Main companion
                continue
            if _collides(comp_base):                  # game defines its own -> skip the Beta companion
                skipped_collisions.append(comp_base)
                continue
            with open(os.path.join(comp_dir, fn), 'r', encoding='utf-8', errors='replace') as f:
                content = f.read()
            with open(os.path.join(out_dir, fn), 'w', encoding='utf-8') as f:
                f.write(_companion_banner(fn) + content)
            files_written.append(fn)
            emitted.add(fn[:-len('.cpp')] if fn.endswith('.cpp') else fn[:-len('.h')])

    # Extension .cpp companions (X.ext.reference.cpp -> X.ext.cpp): extra out-of-line definitions bolted
    # onto a companion class -- Textures.ext defines genId/deleteId/uploadRGBA/uploadSubRGBA (declared in
    # Textures.h) that the GL11->D3D9 emulator forwards to. No .h of its own. Without it: LNK2019.
    for fn in sorted(os.listdir(reference_dir)):
        if fn.endswith('.ext.reference.cpp'):
            base = fn[:-len('.reference.cpp')]        # e.g. 'Textures.ext'
            if _collides(base):                       # Textures.ext collides with a game-defined Textures
                skipped_collisions.append(base)
                continue
            with open(os.path.join(reference_dir, fn), 'r', encoding='utf-8', errors='replace') as f:
                content = f.read()
            with open(os.path.join(out_dir, base + '.cpp'), 'w', encoding='utf-8') as f:
                f.write(_banner(base, 'cpp') + content)
            files_written.append(base + '.cpp')
            emitted.add(base)

    # Main.cpp: the 360 entry point. Detection-driven -- if the game has a detected main class, GENERATE
    # a generic entry that calls <main_class>::main(...) (no game hardcoding). Only fall back to the
    # Beta-specific Main.reference.cpp when NO main class was detected (and the game doesn't define Main).
    if main_class and not _collides('Main'):
        with open(os.path.join(out_dir, 'Main.cpp'), 'w', encoding='utf-8') as f:
            f.write(generate_entry_point(main_class))
        files_written.append('Main.cpp (generic entry -> %s::main)' % main_class)
        emitted.add('Main')
    elif not _collides('Main'):
        main_ref = os.path.join(reference_dir, 'Main.reference.cpp')
        if os.path.isfile(main_ref):
            with open(main_ref, 'r', encoding='utf-8', errors='replace') as f:
                content = f.read()
            with open(os.path.join(out_dir, 'Main.cpp'), 'w', encoding='utf-8') as f:
                f.write(_banner('Main', 'cpp') + content)
            files_written.append('Main.cpp')
            emitted.add('Main')

    _write_platform_layer_md(out_dir, sorted(emitted), files_written)
    emit_references.last_skipped = sorted(set(skipped_collisions))   # for the pipeline progress line
    return emitted


def _companion_banner(fn):
    return ('// ============================ PLATFORM COMPANION ============================\n'
            '// Game-agnostic Xbox 360 platform file, emitted by j2ctool from\n'
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
