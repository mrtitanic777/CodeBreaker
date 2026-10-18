# -*- coding: utf-8 -*-
r"""
platform_coverage.py -- the platform API COVERAGE analyzer.

The converter emits SYMBOL-ONLY stubs for every platform subsystem (video/GL, input, audio, file
IO + save, networking, memory, display). This module scans the converted GAME code for every
platform symbol it actually uses, then classifies each one against two sources of truth:

  * the emitted STUB header      (`GL11.h`, `Keyboard.h`, ... in the output)   -> a NO-OP placeholder
  * the hand-written REFERENCE   (`docs/platform/reference/*.reference.{h,cpp}`) -> the REAL impl
    already written for the Alpha port (GL->D3D9 emulator, XACT audio, XInput, XNet socket, ...)

and emits `PLATFORM_COVERAGE.md`: per subsystem, every symbol the game needs with its status --

  REUSABLE  it's in the Alpha reference -> the real implementation already exists; ship it as-is.
  STUB      only a no-op placeholder exists -> works if the effect is cosmetic, silently wrong if
            it's load-bearing (the glPolygonOffset z-fight class of bug).
  MISSING   NEITHER the stub nor the reference provides it -> genuinely new work for this version
            (e.g. a GL call Beta uses that Alpha never did).

...plus a menu of implementation OPTIONS per subsystem, so a human decides once, up front, how each
platform question is answered -- instead of discovering the gaps one XDK build error at a time.
"""

import os
import re


# ---------------------------------------------------------------------------
# Subsystem map: which platform holders/types belong to each, which reference
# files hold the real Alpha implementation, and the implementation-choice menu.
# ---------------------------------------------------------------------------
SUBSYSTEMS = [
    {
        'key': 'video',
        'name': 'Video  --  OpenGL (LWJGL) -> Direct3D 9',
        'holders': ['GL11', 'GL12', 'GL15', 'GLU', 'GLContext', 'ARBOcclusionQuery',
                    'ARBVertexBufferObject', 'BufferUtils'],
        'types': ['GLAllocation', 'Tessellator'],
        'reference': ['GL11', 'Shaders', 'Mipmaps', 'NIOBuffers', 'ImageIO', 'BufferedImage'],
        'options': [
            'A) REUSE the Alpha GL->D3D9 emulator (docs/platform/reference/GL11.reference.cpp) -- '
            'the GL API is identical between Alpha and Beta, so this is the recommended path; only the '
            'MISSING symbols below are new work.',
            'B) Write a fresh Direct3D 9 backend from scratch.',
            'C) Stub to no-ops (renders nothing / placeholder) -- for bring-up only.'],
        'notes': [
            'HALF-PIXEL OFFSET (Direct3D 9). D3D9 maps screen coordinates to pixel CENTERS but texture '
            'samples to the top-left CORNER, so any 2D/HUD/text drawn without a -0.5 screen-space shift '
            'lands half a texel off -> blurry / smeared, MOST visible on font atlases and text. OpenGL '
            '(the source) has no such offset, so the GL->D3D9 port introduces it. On the Xbox 360 the '
            'fix is the hardware render state `D3DRS_HALFPIXELOFFSET = TRUE` (applied once at device '
            'setup -- see Main.reference.cpp) rather than baking -0.5 into every GUI/font vertex. '
            'D3D10/11 removed the offset entirely; a D3D9 backend on PC still needs the -0.5 vertex '
            'shift. VERIFY ON HARDWARE with the in-game font/GUI.'],
    },
    {
        'key': 'input',
        'name': 'Input  --  controller / keyboard / mouse',
        'holders': ['Keyboard', 'Mouse', 'Controllers'],
        'types': ['MouseHelper', 'MovementInput'],
        'reference': ['Input', 'OSK'],
        'options': [
            'A) XInput2 controller mapping + on-screen keyboard (docs/platform/reference/Input, OSK).',
            'B) Stub (no input) -- boots but is unplayable.'],
    },
    {
        'key': 'audio',
        'name': 'Audio  --  music / sound effects',
        'holders': ['SoundSystem', 'SoundSystemConfig'],
        'types': ['SoundSystem', 'SoundManager', 'CodecJOrbis', 'SoundPool'],
        'reference': ['Audio'],
        'options': [
            'A) XACT3 -- authored wave banks (.xwb) + sound banks (.xsb), cue-driven. The '
            'Audio.reference routes SoundManager -> XACT cues; the `j2ctool xact` tool builds the banks '
            'from Content.md + the .ogg/.wav sources. RECOMMENDED (the Alpha port uses XACT3).',
            'B) XAudio2 -- low-level source voices; you own mixing/streaming/format decode yourself '
            '(more code, no authoring pipeline).',
            'C) Stub-silent (no audio).'],
    },
    {
        'key': 'display',
        'name': 'Display / window / timing',
        'holders': ['Display', 'Sys'],
        'types': ['DisplayMode', 'Canvas', 'Frame', 'Applet', 'Panel', 'Timer'],
        'reference': ['Display'],
        'options': [
            'A) Fixed 1280x720 back buffer + vsync (docs/platform/reference/Display.reference).',
            'B) Stub.'],
    },
    {
        'key': 'fileio',
        'name': 'File I/O & world save / load',
        'holders': [],
        'types': ['File', 'RandomAccessFile', 'FileInputStream', 'FileOutputStream', 'FileReader',
                  'FileWriter', 'RegionFile', 'RegionFileCache', 'ZipFile', 'SaveHandler',
                  'ChunkLoader', 'McRegionChunkLoader'],
        'reference': ['FileIO', 'ZipFile', 'Inflate'],
        'options': [
            'A) STFS / Save360 XContent -- persistent saves across boots (needs the writable Save: mount).',
            'B) In-memory only -- worlds play but do NOT persist across reboots.',
            'C) Stub -- no save/load at all.'],
    },
    {
        'key': 'network',
        'name': 'Networking (SMP)',
        'holders': [],
        'types': ['Socket', 'URL', 'URLConnection', 'HttpURLConnection', 'NetworkManager',
                  'NetClientHandler', 'InetAddress'],
        'reference': ['Socket'],
        'options': [
            'A) XNet insecure socket -- LAN SMP (docs/platform/reference/Socket.reference.cpp). '
            'Off-subnet needs a portproxy relay.',
            'B) Stub-offline -- singleplayer only.'],
    },
    {
        'key': 'memory',
        'name': 'Memory / allocation',
        'holders': ['Runtime'],
        'types': [],
        'reference': [],
        'options': [
            'A) Report the real Xbox 360 title memory budget (XMemGetAllocationInfo / retail 512MB).',
            'B) Fixed constants (freeMemory/totalMemory return a nominal budget) -- fine unless the '
            'game gates behaviour on free memory.'],
    },
    {
        'key': 'user',
        'name': 'User / profile / session',
        'holders': [],                                  # System.* is java.lang, provided by JavaRT.h prelude (not a platform gap)
        'types': ['Session', 'StatsSyncher', 'MessageDigest'],
        'reference': ['XboxUser'],
        'options': [
            'A) XUserGetName / gamertag + local profile (docs/platform/reference/XboxUser.reference).',
            'B) Stub (fixed "Player" identity).'],
    },
]


from .textutil import read_text as _read_text


def _read(path):
    return _read_text(path)   # tolerant: errors='replace', missing -> ''


# Every no-op/placeholder in the emitted stubs AND the hand-written references carries a
# `// J2X-STUB[tag]: ...` marker. Scanning for it enumerates the ENTIRE placeholder surface, so a
# no-op that turns out to be load-bearing (the glPolygonOffset z-fight class) is never left silently
# undone -- `grep -rn J2X-STUB` is the audit list; this folds it into the coverage report by tag.
_STUB_MARKER = re.compile(r'J2X-STUB\[([a-z0-9-]+)\]\s*:\s*([^\n]*)')


def scan_stub_markers(output_dir):
    """Return {tag: [(file, line, note)]} for every J2X-STUB marker in the emitted tree."""
    out = {}
    for f in sorted(os.listdir(output_dir)):
        if not (f.endswith('.cpp') or f.endswith('.h')):
            continue
        text = _read(os.path.join(output_dir, f))
        if 'J2X-STUB' not in text:
            continue
        for i, line in enumerate(text.split('\n'), 1):
            m = _STUB_MARKER.search(line)
            if m:
                out.setdefault(m.group(1), []).append((f, i, m.group(2).strip()[:100]))
    return out


# ---------------------------------------------------------------------------
# What a header PROVIDES: declared function names + named constants.
# ---------------------------------------------------------------------------
_FN_DECL = re.compile(r'\b([A-Za-z_]\w*)\s*\(')
_CONST_DECL = re.compile(r'\b(?:const\s+\w[\w:<>]*|#define)\s+([A-Z][A-Z0-9_]{2,})\b')
_ENUM_CONST = re.compile(r'\b([A-Z][A-Z0-9_]{2,})\s*=')


def _provided_symbols(header_text):
    """Symbols a stub/reference header declares (function names + ALL_CAPS constants)."""
    syms = set(_FN_DECL.findall(header_text))
    syms |= set(_CONST_DECL.findall(header_text))
    syms |= set(_ENUM_CONST.findall(header_text))
    syms.discard('if'); syms.discard('for'); syms.discard('while'); syms.discard('return')
    syms.discard('sizeof'); syms.discard('switch')
    return syms


# ---------------------------------------------------------------------------
# What the GAME code USES from each holder / platform type.
# ---------------------------------------------------------------------------
def _used_holder_symbols(game_text, holder):
    """`Holder::symbol` occurrences in the game code -> {symbol: count}."""
    out = {}
    for m in re.finditer(r'\b%s\s*::\s*([A-Za-z_]\w*)' % re.escape(holder), game_text):
        out[m.group(1)] = out.get(m.group(1), 0) + 1
    return out


def _type_usage_count(game_text, typ):
    """How many times a platform TYPE is referenced (new T / T* / T( / <T>)."""
    return len(re.findall(r'(?<![\w:])%s\b' % re.escape(typ), game_text))


def analyze(output_dir, reference_dir=None, runtime_files=None):
    """Scan the converted output for platform symbol usage and classify coverage. Returns a list of
    per-subsystem dicts. `runtime_files` = set of emitted runtime/stub header basenames to treat as
    the STUB source-of-truth (and to exclude from the 'game code' scan)."""
    runtime_files = set(runtime_files or ())
    files = [f for f in os.listdir(output_dir) if f.endswith('.cpp') or f.endswith('.h')]
    # game code = everything except the emitted runtime/stub headers
    game_texts = [_read(os.path.join(output_dir, f)) for f in files if f not in runtime_files]
    game_blob = '\n'.join(game_texts)

    def stub_provides(holder):
        return _provided_symbols(_read(os.path.join(output_dir, holder + '.h')))

    def reference_provides(names):
        if not reference_dir:
            return set()
        s = set()
        for n in names:
            s |= _provided_symbols(_read(os.path.join(reference_dir, n + '.reference.h')))
            s |= _provided_symbols(_read(os.path.join(reference_dir, n + '.reference.cpp')))
        return s

    results = []
    for sub in SUBSYSTEMS:
        ref_syms = reference_provides(sub.get('reference', []))
        ref_exists = bool(sub.get('reference')) and bool(ref_syms)
        symbols = []                                    # (symbol, count, status)
        for holder in sub.get('holders', []):
            provided = stub_provides(holder)
            for sym, cnt in sorted(_used_holder_symbols(game_blob, holder).items(),
                                   key=lambda kv: -kv[1]):
                if sym in ref_syms:
                    status = 'REUSABLE'
                elif sym in provided:
                    status = 'STUB'
                else:
                    status = 'MISSING'
                symbols.append(('%s::%s' % (holder, sym), cnt, status))
        types = []                                      # (type, count, has_ref)
        for typ in sub.get('types', []):
            cnt = _type_usage_count(game_blob, typ)
            if cnt:
                types.append((typ, cnt, typ in sub.get('reference', []) or typ in ref_syms))
        results.append({
            'sub': sub, 'symbols': symbols, 'types': types, 'ref_exists': ref_exists,
            'missing': [s for s in symbols if s[2] == 'MISSING'],
            'stub': [s for s in symbols if s[2] == 'STUB'],
            'reusable': [s for s in symbols if s[2] == 'REUSABLE'],
        })
    return results


# ---------------------------------------------------------------------------
def _render_stub_markers(markers):
    if not markers:
        return []
    total = sum(len(v) for v in markers.values())
    L = ['---', '', '## J2X-STUB markers -- the explicit placeholder surface (%d)' % total, '',
         'Every no-op / default-return in the emitted stubs and the hand-written references is marked '
         '`// J2X-STUB[tag]: ...` at the exact function. `grep -rn "J2X-STUB" .` is the full audit list; '
         'grouped by tag here. A no-op is only safe if its effect is cosmetic -- verify the load-bearing '
         'ones (the glPolygonOffset z-fight class of bug lived in an unmarked no-op).', '',
         '| Tag | Count | Example site |', '|---|---:|---|']
    for tag in sorted(markers, key=lambda t: -len(markers[t])):
        sites = markers[tag]
        ex = '`%s:%d`' % (sites[0][0], sites[0][1])
        L.append('| `%s` | %d | %s -- %s |' % (tag, len(sites), ex, sites[0][2]))
    L.append('')
    return L


def render_report(results, reference_dir=None, stub_markers=None, sdk_caps=None):
    have_ref = reference_dir and os.path.isdir(reference_dir)
    L = ['# PLATFORM_COVERAGE.md -- platform-layer coverage & decisions', '',
         'Every platform subsystem the converted game touches, and what still has to be answered to',
         'make it run on real hardware. The converter emits **no-op stubs** for all of these; this',
         'report says, per symbol, whether the real implementation already exists in the Alpha',
         '`docs/platform/reference/` layer (**REUSABLE**), is only a placeholder (**STUB**), or is',
         '**MISSING** (new to this version -> new work).', '']
    if not have_ref:
        L += ['> NOTE: the Alpha reference layer was not found, so nothing could be marked REUSABLE. '
              'Pass its path (`--reference docs/platform/reference`) to classify against the real impls.', '']

    # scoreboard
    L += ['## Scoreboard', '', '| Subsystem | Reference impl | REUSABLE | STUB | MISSING |',
          '|---|---|---:|---:|---:|']
    for r in results:
        L.append('| %s | %s | %d | %d | %d |' % (
            r['sub']['name'].split('  --')[0].strip(),
            'yes' if r['ref_exists'] else 'no',
            len(r['reusable']), len(r['stub']), len(r['missing'])))
    L.append('')
    total_missing = sum(len(r['missing']) for r in results)
    L += ['**%d symbol(s) are MISSING** (used by the game, absent from both the stub and the Alpha '
          'reference) -- those are the concrete new work items, listed per subsystem below.' % total_missing, '']

    from . import platform_equivalents as _pe

    # --- Platform REQUIREMENTS -- AUTO-DETECTED from the installed SDK (xdk_scan), NOT hardcoded. Each is
    #     claimed ONLY because its XDK function was actually found in the scanned SDK headers. ---------------
    _found = (sdk_caps or {}).get('found', [])
    _reqs = [r for r in _found if r['kind'] == 'requirement']
    _opps = [r for r in _found if r['kind'] == 'opportunity']
    if sdk_caps and sdk_caps.get('include'):
        L += ['---', '', '## Xbox 360 platform requirements -- auto-detected from the installed SDK', '',
              'Rules the console/XDK imposes that are NOT a single stub-swap. **Each row is here because its API '
              'was found in the scanned SDK** (`%s`, %d functions) -- derived from the SDK, not from memory. Ignore '
              'one and the feature silently breaks or the title fails cert.'
              % (sdk_caps['include'], len(sdk_caps.get('funcs', {}))),
              '', '| Requirement | Detected in SDK | Severity | If ignored |', '|---|---|:-:|---|']
        for r in _reqs:
            fam = ('  (+%d in family)' % (r['func_count'] - 1)) if r['func_count'] > 1 else ''
            L.append('| **%s** | `%s`%s -- %s | %s | %s |' % (
                r['title'], r['needs'], fam, r['header'], r['sev'].upper(), r['why']))
        L.append('')
    else:
        L += ['---', '', '## Xbox 360 platform requirements', '',
              '> No SDK was scanned, so requirements could not be auto-detected. (They are derived from the SDK '
              'headers, not hardcoded -- supply the SDK to populate this.)', '']

    # --- Xbox 360 XDK equivalents (the authoritative Java-symbol -> XDK-API map) -------------------
    L += ['---', '', '## Xbox 360 XDK equivalents -- the real API for every platform symbol', '',
          'For each Java/LWJGL platform type the port stubs, the exact XDK function to resolve it to '
          '(this same line is emitted in the stub header as `// [360] equivalent:`). '
          '**real** = the port implements it; **stub** = still a no-op but the API named is the drop-in; '
          '**dead** = no console equivalent.', '',
          '| Subsystem | Java symbol | Xbox 360 XDK API | Lib | Status |', '|---|---|---|---|---|']
    _ord = {'real': 0, 'stub': 1, 'dead': 2}
    _subs = list(_pe.EQUIV)
    for subk, name, e in sorted(_pe.all_entries(), key=lambda x: (_subs.index(x[0]), _ord.get(x[2]['status'], 3), x[1])):
        badge = {'real': 'REAL', 'stub': 'stub', 'dead': 'dead'}.get(e['status'], e['status'])
        L.append('| %s | `%s` | %s | %s | %s |' % (subk, name, e['xdk'], e.get('lib', ''), badge))
    L.append('')

    # --- Untapped XDK capabilities -- AUTO-DETECTED in the scanned SDK (not hardcoded) --------------
    if sdk_caps and _opps:
        L += ['---', '', '## Untapped XDK capabilities -- 360-native wins, auto-detected in the SDK', '',
              'Found in the scanned SDK headers; not stubs to fill -- features/optimizations worth adopting.', '',
              '| Feature | Detected in SDK | Effort | Why it helps |', '|---|---|:-:|---|']
        for o in _opps:
            L.append('| **%s** | `%s` -- %s | %s | %s |' % (
                o['title'], o['needs'], o['header'], o.get('effort', '?'), o['why']))
        L.append('')

    for r in results:
        sub = r['sub']
        if not r['symbols'] and not r['types'] and not sub.get('notes'):
            continue
        L += ['---', '', '## %s' % sub['name'], '']
        L.append('**Reference implementation:** %s' %
                 (('`' + '`, `'.join('%s.reference.*' % n for n in sub['reference']) + '`') if sub.get('reference')
                  else 'none (platform-native only)'))
        L.append('')
        L.append('**Choose an implementation:**')
        for opt in sub['options']:
            L.append('- %s' % opt)
        L.append('')
        if sub.get('notes'):
            L += ['**Known considerations (verify these even when the symbols are all covered):**', '']
            for note in sub['notes']:
                L.append('- ⚠️ %s' % note)
            L.append('')
        if r['missing']:
            L += ['### MISSING -- new work for this version (%d)' % len(r['missing']), '',
                  '| Symbol | Used | Note |', '|---|---:|---|']
            for name, cnt, _ in r['missing']:
                L.append('| `%s` | %d | not in the stub OR the Alpha reference -- implement it |' % (name, cnt))
            L.append('')
        if r['reusable']:
            L += ['### REUSABLE -- already implemented in the Alpha reference (%d)' % len(r['reusable']),
                  '', 'These compile+work as soon as the reference file replaces the stub. Top by use:', '',
                  '| Symbol | Used |', '|---|---:|']
            for name, cnt, _ in r['reusable'][:20]:
                L.append('| `%s` | %d |' % (name, cnt))
            if len(r['reusable']) > 20:
                L.append('| … (%d more) | |' % (len(r['reusable']) - 20))
            L.append('')
        if r['stub']:
            L += ['### STUB -- placeholder only, verify it is not load-bearing (%d)' % len(r['stub']),
                  '', 'A no-op here is fine if the effect is cosmetic, but silently wrong if it matters '
                  '(the glPolygonOffset z-fight class). Review the high-use ones:', '',
                  '| Symbol | Used |', '|---|---:|']
            for name, cnt, _ in r['stub'][:15]:
                L.append('| `%s` | %d |' % (name, cnt))
            L.append('')
        if r['types']:
            L += ['### Platform types referenced', '', '| Type | Used | In reference? |', '|---|---:|---|']
            for typ, cnt, hr in r['types']:
                L.append('| `%s` | %d | %s |' % (typ, cnt, 'yes' if hr else 'no'))
            L.append('')
    L += _render_stub_markers(stub_markers or {})
    return '\n'.join(L) + '\n'


def write_report(output_dir, reference_dir=None, runtime_files=None, sdk_root=None):
    results = analyze(output_dir, reference_dir=reference_dir, runtime_files=runtime_files)
    markers = scan_stub_markers(output_dir)
    # AUTO-DETECT platform requirements/capabilities from the installed SDK (not hardcoded). Best-effort:
    # if no SDK is resolvable, the report says so rather than inventing requirements.
    sdk_caps = None
    try:
        from . import xdk_scan
        if not sdk_root:
            from . import sdk as _sdk
            _r = _sdk.detect('xbox360')
            sdk_root = getattr(_r, 'root', _r if isinstance(_r, str) else None)
        if sdk_root:
            sdk_caps = xdk_scan.detect(sdk_root)
    except Exception:
        sdk_caps = None
    path = os.path.join(output_dir, 'PLATFORM_COVERAGE.md')
    with open(path, 'w', encoding='utf-8') as f:
        f.write(render_report(results, reference_dir=reference_dir, stub_markers=markers, sdk_caps=sdk_caps))
    return path, results
