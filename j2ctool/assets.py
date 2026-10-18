# -*- coding: utf-8 -*-
r"""
assets.py -- extract the game's shipped assets (textures, language files, fonts, misc resources)
out of the original Minecraft jar and lay them down in the converted project's Content folder with
the EXACT same relative paths and names.

Why exact paths matter: the port resolves a Java resource path `"/gui/gui.png"` to
`game:\Content\gui\gui.png` (FileIO / Class::getResourceAsStream). If we drop the jar entry
`gui/gui.png` at `Content\gui\gui.png`, the texture loader finds it with ZERO code changes -- the
folder layout IS the contract. So this tool never renames or reorganizes; it mirrors the jar.

Two jobs:
  * extract(jar, content_dir)        -- mirror every asset entry into Content\<same-path>.
  * coverage(jar, code_dir)          -- audit: which resource paths the CODE loads vs which the jar
                                        provides. A path the code loads but the jar lacks becomes a
                                        NULL image at runtime -> the loader crashes. This finds it
                                        before the game runs, the same spirit as PLATFORM_COVERAGE.

`coverage` scans either the Java corpus (.java) or the converted C++ tree (.cpp/.h) -- the resource
path string literal `"/gui/gui.png"` is identical in both, so the audit works pre- or post-convert.
"""

import os
import re
import zipfile

try:
    from PIL import Image
    _HAS_PIL = True
except ImportError:                                       # pragma: no cover - environment dependent
    _HAS_PIL = False

# Asset file extensions the game ships in its jar and loads at runtime.
_ASSET_EXTS = ('.png', '.jpg', '.jpeg', '.txt', '.lang', '.properties', '.fsh', '.vsh')

# A resource-path string literal as it appears in code: "/dir/name.ext" (leading slash, shipped file).
# Identical in Java source and in the converted C++ (JavaString literals keep the same text).
_RES_PATH = re.compile(r'"(/[\w./-]+\.(?:png|jpg|jpeg|txt|lang|properties|fsh|vsh))"')

_CODE_EXTS = ('.java', '.cpp', '.h', '.hpp', '.cc', '.cxx')

# strip // line comments and /* block */ comments so a documentation example like `"/x.png"` in a
# comment is not mistaken for a real resource load (false MISSING).
_LINE_COMMENT = re.compile(r'//[^\n]*')
_BLOCK_COMMENT = re.compile(r'/\*.*?\*/', re.DOTALL)


def _strip_comments(text):
    return _LINE_COMMENT.sub('', _BLOCK_COMMENT.sub('', text))


# --- PNG normalization: force to 8-bit straight-alpha RGBA (colortype 6) ------------------------
# WHY: the Xbox 360 texture loader expects 32-bit RGBA. A palettized (indexed, colortype 3) or
# grayscale PNG loaded raw is decoded as garbage/wrong colors -- the "RGB problem" from the Alpha
# port. Empirically (diffing Alpha's shipped Content vs the stock jar) the ONLY transform the Alpha
# pipeline applied was: convert every non-RGBA PNG to RGBA8888, honoring palette transparency (tRNS).
# No alpha-bleed, no channel swap, no premultiply -- those were all confirmed absent. So we replicate
# exactly that: convert colortype != 6 PNGs to RGBA; RGBA PNGs are copied byte-for-byte.

def _png_colortype(raw):
    """PNG IHDR color type (0 gray, 2 rgb, 3 indexed, 4 gray+alpha, 6 rgba), or None if not a PNG."""
    if len(raw) < 26 or raw[:8] != b'\x89PNG\r\n\x1a\n':
        return None
    return raw[25]


def png_needs_rgba(raw):
    """True if this PNG is not already 8-bit straight RGBA (i.e. would misrender on the 360)."""
    ct = _png_colortype(raw)
    return ct is not None and ct != 6


def normalize_png_to_rgba(raw):
    """Return (bytes, changed). If the PNG isn't already RGBA, decode it (honoring palette tRNS) and
    re-encode as 8-bit RGBA; otherwise return it unchanged. Requires Pillow to actually convert."""
    if not png_needs_rgba(raw):
        return raw, False
    if not _HAS_PIL:
        return raw, False                                 # can't convert here; run() warns + lists it
    import io
    img = Image.open(io.BytesIO(raw))
    rgba = img.convert('RGBA')                             # convert() applies the palette + tRNS alpha
    buf = io.BytesIO()
    rgba.save(buf, format='PNG')
    return buf.getvalue(), True


def list_jar_assets(jar_path):
    """Return the sorted list of asset entry paths (png/txt/lang/...) inside the jar, using forward
    slashes and no leading slash -- exactly how they map under Content\\."""
    with zipfile.ZipFile(jar_path) as z:
        names = [n for n in z.namelist()
                 if not n.endswith('/') and n.lower().endswith(_ASSET_EXTS)]
    return sorted(names)


def code_resource_refs(code_dir):
    """Every resource path the code loads, as a set WITHOUT the leading slash (so it lines up with
    jar entry names). Scans .java and/or converted .cpp/.h -- the literal is the same in both."""
    refs = {}
    for dp, _dirs, files in os.walk(code_dir):
        for f in files:
            if not f.lower().endswith(_CODE_EXTS):
                continue
            try:
                text = open(os.path.join(dp, f), 'r', encoding='utf-8', errors='replace').read()
            except (IOError, OSError):
                continue
            for m in _RES_PATH.finditer(_strip_comments(text)):
                refs.setdefault(m.group(1).lstrip('/'), set()).add(f)
    return dict((k, sorted(v)) for k, v in refs.items())


# CONSOLE-ONLY assets that no vanilla jar ships but the console platform layer needs (e.g. the controller
# cursor icon drawn by EntityRenderer in container GUIs -- see controller_nav.py). Bundled in the converter
# and copied into Content alongside the jar extraction so the feature is never a per-project manual asset.
_CONSOLE_ASSETS_DIR = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                                   'docs', 'platform', 'assets')


def place_console_assets(content_dir):
    """Mirror the bundled console-only assets into content_dir (same relative paths). Returns the list of
    relative paths written. Skips silently if the bundle dir is absent."""
    if not os.path.isdir(_CONSOLE_ASSETS_DIR):
        return []
    placed = []
    for root, _dirs, files in os.walk(_CONSOLE_ASSETS_DIR):
        for fn in files:
            src = os.path.join(root, fn)
            rel = os.path.relpath(src, _CONSOLE_ASSETS_DIR)
            dst = os.path.join(content_dir, rel)
            d = os.path.dirname(dst)
            if d and not os.path.isdir(d):
                os.makedirs(d)
            with open(src, 'rb') as fh:
                data = fh.read()
            with open(dst, 'wb') as out:
                out.write(data)
            placed.append(rel.replace(os.sep, '/'))
    return sorted(placed)


def extract(jar_path, content_dir, only=None, normalize_png=True, converted=None):
    """Mirror the jar's asset entries into content_dir, preserving each entry's relative path and
    name (jar `gui/gui.png` -> content_dir/gui/gui.png). If `only` (a set of slash-less paths) is
    given, extract just those; otherwise extract every asset. When normalize_png is on, any non-RGBA
    PNG is converted to RGBA8888 on the way out (the 360 render fix). If `converted` is a list, the
    names of PNGs that were normalized are appended to it. Returns the sorted list of paths written."""
    written = []
    with zipfile.ZipFile(jar_path) as z:
        for name in z.namelist():
            if name.endswith('/') or not name.lower().endswith(_ASSET_EXTS):
                continue
            if only is not None and name not in only:
                continue
            data = z.read(name)
            if normalize_png and name.lower().endswith('.png'):
                data, changed = normalize_png_to_rgba(data)
                if changed and converted is not None:
                    converted.append(name)
            rel = name.replace('/', os.sep)
            dst = os.path.join(content_dir, rel)
            d = os.path.dirname(dst)
            if d and not os.path.isdir(d):
                os.makedirs(d)
            with open(dst, 'wb') as out:
                out.write(data)
            written.append(name)
    return sorted(written)


def list_console_assets():
    """Bundled console-only asset rel-paths (forward slash, no leading slash). Part of the coverage
    audit's SUPPLY, since these ship from the converter rather than the jar."""
    if not os.path.isdir(_CONSOLE_ASSETS_DIR):
        return []
    out = []
    for root, _dirs, files in os.walk(_CONSOLE_ASSETS_DIR):
        for fn in files:
            rel = os.path.relpath(os.path.join(root, fn), _CONSOLE_ASSETS_DIR)
            out.append(rel.replace(os.sep, '/'))
    return sorted(out)


def coverage(jar_path, code_dir):
    """Audit the code's texture/resource demand against the jar's supply. Returns:
      needed   {path: [files that reference it]}   (slash-less)
      present  sorted[paths the code needs AND the jar has]
      missing  sorted[paths the code needs but the jar LACKS]  <- runtime NULL-image crash
      extra    sorted[asset paths in the jar the code never references]  (safe to ship anyway)"""
    needed = code_resource_refs(code_dir)
    jar = set(list_jar_assets(jar_path)) | set(list_console_assets())   # supply = jar + console bundle
    jar_lower = dict((n.lower(), n) for n in jar)
    needset = set(needed)
    present, missing = [], []
    for p in sorted(needset):
        if p in jar or p.lower() in jar_lower:            # tolerate case drift (Windows FS is case-insensitive)
            present.append(p)
        else:
            missing.append(p)
    extra = sorted(jar - set(present) - set(p for p in needset))
    return {'needed': needed, 'present': present, 'missing': missing, 'extra': extra,
            'jar_count': len(jar)}


def render_coverage(cov, converted=None, unconverted=None):
    L = ['# TEXTURE_COVERAGE.md -- shipped-asset audit', '',
         'Resource paths the game code loads vs assets present in the jar. A **MISSING** path is one '
         'the code opens but the jar does not contain -> at runtime the loader gets a NULL image and '
         'crashes. Everything here is copied into `Content\\` with its exact jar path so the loader '
         'needs no changes.', '',
         '- code needs: %d   jar provides: %d   **present: %d   MISSING: %d**   (jar extras: %d)'
         % (len(cov['needed']), cov['jar_count'], len(cov['present']),
            len(cov['missing']), len(cov['extra'])), '']
    if cov['missing']:
        L += ['## MISSING -- code loads these but the jar lacks them (runtime crash)', '',
              '| Resource path | Loaded by |', '|---|---|']
        for p in cov['missing']:
            L.append('| `%s` | %s |' % (p, ', '.join(cov['needed'][p][:4])))
        L.append('')
    else:
        L += ['> No missing textures: the jar covers every resource path the code loads.', '']
    if converted:
        L += ['## PNGs normalized to RGBA (the 360 render fix)', '',
              'These were palettized/grayscale in the jar and would misrender as garbage colors if '
              'copied raw (the 360 texture loader expects 32-bit RGBA). They were decoded to RGBA8888 '
              '(honoring palette transparency) on extraction:', '']
        for p in converted:
            L.append('- `%s`' % p)
        L.append('')
    if unconverted:
        L += ['## WARNING -- non-RGBA PNGs that could NOT be converted (Pillow not installed)', '',
              'These will misrender on the 360 until converted to RGBA. Install Pillow (`pip install '
              'Pillow`) and re-run, or convert them by hand:', '']
        for p in unconverted:
            L.append('- `%s`' % p)
        L.append('')
    if cov['extra']:
        L += ['## Jar extras (shipped anyway, not referenced by code)', '']
        for p in cov['extra']:
            L.append('- `%s`' % p)
        L.append('')
    return '\n'.join(L) + '\n'


def indexed_pngs(jar_path):
    """Names of PNG entries in the jar that are NOT already RGBA (would misrender raw on the 360)."""
    out = []
    with zipfile.ZipFile(jar_path) as z:
        for name in z.namelist():
            if name.lower().endswith('.png') and png_needs_rgba(z.read(name)):
                out.append(name)
    return sorted(out)


def run(jar_path, content_dir, code_dir=None, only_needed=False, normalize_png=True):
    """One-shot: extract assets into content_dir (all, or only code-referenced if only_needed),
    normalizing non-RGBA PNGs to RGBA (the 360 render fix), and -- when code_dir is given -- write
    TEXTURE_COVERAGE.md next to content_dir. Returns a summary dict."""
    cov = coverage(jar_path, code_dir) if code_dir else None
    only = set(cov['present']) if (only_needed and cov) else None
    converted = []
    need_convert = indexed_pngs(jar_path) if normalize_png else []
    written = extract(jar_path, content_dir, only=only,
                      normalize_png=normalize_png, converted=converted)
    # console-only assets (controller cursor, etc.) ship from the converter's bundle, not the jar.
    console = place_console_assets(content_dir)
    # PNGs that needed conversion but couldn't be (Pillow absent) -> flag them, they will misrender.
    unconverted = sorted(set(need_convert) - set(converted)
                         - (set(need_convert) - set(written))) if normalize_png else []
    report_path = None
    if cov is not None:
        report_path = os.path.join(os.path.dirname(content_dir.rstrip(os.sep)) or '.',
                                   'TEXTURE_COVERAGE.md')
        with open(report_path, 'w', encoding='utf-8') as f:
            f.write(render_coverage(cov, converted=converted, unconverted=unconverted))
    return {'written': written, 'coverage': cov, 'content_dir': content_dir, 'report': report_path,
            'converted': sorted(converted), 'unconverted': unconverted, 'has_pil': _HAS_PIL,
            'console': console}
