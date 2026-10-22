# -*- coding: utf-8 -*-
"""
bugfix_ledger.py -- catalog every game-breaking bug fix so none is ever forgotten.

The standing rule (see memory: fixed-bug-marker-rule) is that EVERY game-breaking
fix is tagged in-code with a greppable `FIXED BUG <slug>` (or the older
`J2X-BUGFIX <slug>`) marker. Those markers are the source of truth. This module
walks the converter's AUTHORED sources -- the layers that survive a reconvert
(docs/platform references + canonicals + companions, and the per-project
overlays) -- collects every marker with its slug, location, and the explanatory
comment that follows it, and renders BUGFIX_LEDGER.md.

Because it is GENERATED from the live markers, the ledger can never drift out of
sync with the code: re-run it after any fix and it re-catalogs everything. It is
wired into pipeline.convert() so the ledger regenerates on every conversion.

    from codebreaker.java import bugfix_ledger
    bugfix_ledger.run()          # -> writes state/java/BUGFIX_LEDGER.md, returns [entries]
"""

import os
import re

# `FIXED BUG <slug>` or `J2X-BUGFIX <slug>`; slug is kebab-case or a YYYY-MM-DD date.
_MARK = re.compile(r'(FIXED BUG|J2X-BUGFIX)\s+([A-Za-z0-9][A-Za-z0-9\-]*)')

# Words that follow the marker token in META references to the marker system itself (e.g. a help/docstring
# line that says "grep for the J2X-BUGFIX marker") -- these are NOT real fix slugs, so skip them.
_NON_SLUGS = frozenset(('marker', 'markers'))

# The scanner's own file: it contains the marker regex + prose ABOUT markers, so exclude it from the walk.
_SELF = 'bugfix_ledger.py'

# Source extensions worth scanning (skip binaries, images, generated caches).
_EXTS = ('.h', '.hpp', '.cpp', '.cc', '.c', '.py', '.md')

# Layers scanned, most-durable first. The OUTPUT layers (canonical/reference/companion/overlay) survive a
# reconvert and the emitted tree is derived from them. The GENERATOR layer is the converter itself
# (`codebreaker/java/`) -- its emitter/runtime/toolchain fixes don't appear in the output tree but ARE revertible by an
# edit to the converter, so they belong in the ledger too (e.g. the section-sign unsigned-char cast, the
# SFX/music bypass pass, map-get-inserts). Listed last as a distinct "the converter itself" category.
_LAYERS = [
    ('canonical',  os.path.join('platform', 'canonical')),
    ('reference',  os.path.join('platform', 'reference')),
    ('companion',  os.path.join('platform', 'companions')),
    ('generator',  ''),
]


def _clean_comment(line):
    """Strip code/comment punctuation to recover the human explanation on a marker line."""
    s = line.strip()
    # drop a leading comment opener (// or /* or *) and any leftover code before it
    m = re.search(r'(FIXED BUG|J2X-BUGFIX)\s+[A-Za-z0-9][A-Za-z0-9\-]*[:\s]*(.*)$', s)
    tail = (m.group(2) if m else s).strip()
    tail = tail.rstrip('*/').strip()
    return tail


def _scan_file(path):
    entries = []
    try:
        with open(path, 'r', encoding='utf-8', errors='replace') as f:
            lines = f.readlines()
    except (IOError, OSError):
        return entries
    for i, line in enumerate(lines):
        m = _MARK.search(line)
        if not m:
            continue
        slug = m.group(2)
        if slug in _NON_SLUGS:                    # meta reference to the marker system, not a fix slug
            continue
        desc = _clean_comment(line)
        # If the marker line has no trailing prose, borrow the next comment line.
        if not desc and i + 1 < len(lines):
            nxt = lines[i + 1].strip().lstrip('/*').strip()
            if nxt and not _MARK.search(nxt):
                desc = nxt.rstrip('*/').strip()
        entries.append({'slug': slug, 'line': i + 1, 'desc': desc})
    return entries


def collect(repo_root):
    """Return [{'layer','file','slug','line','desc'}] across every authored layer."""
    out = []
    for layer, rel in _LAYERS:
        root = os.path.join(repo_root, rel)
        if not os.path.isdir(root):
            continue
        for dirpath, dirs, files in os.walk(root):
            if '__pycache__' in dirpath:
                continue
            if layer == 'generator' and dirpath == root:
                dirs[:] = [d for d in dirs if d not in ('platform', 'api_maps')]   # scanned as their own layers
            for fn in sorted(files):
                if not fn.endswith(_EXTS) or fn == _SELF:
                    continue
                full = os.path.join(dirpath, fn)
                rel_file = os.path.relpath(full, repo_root).replace('\\', '/')
                for e in _scan_file(full):
                    e['layer'] = layer
                    e['file'] = rel_file
                    out.append(e)
    return out


def render(entries):
    """Render the collected entries as BUGFIX_LEDGER.md text."""
    by_layer = {}
    for e in entries:
        by_layer.setdefault(e['layer'], []).append(e)

    total = len(entries)
    uniq = len(set(e['slug'] for e in entries))
    lines = []
    lines.append('# Bug-fix ledger')
    lines.append('')
    lines.append('_Auto-generated by `codebreaker/java/bugfix_ledger.py` from the live `FIXED BUG` / '
                 '`J2X-BUGFIX` markers in the converter\'s authored sources. Do not edit by '
                 'hand -- fix the marker in-code and re-run the converter (or '
                 '`python -m codebreaker java bugfix-ledger`) to regenerate._')
    lines.append('')
    lines.append('Every game-breaking fix carries a greppable in-code marker (standing rule). '
                 'This ledger is the index of those markers so a fix can never be silently '
                 'reverted by a reconvert without it showing up here as missing.')
    lines.append('')
    lines.append('**%d markers across %d distinct fixes.**' % (total, uniq))
    lines.append('')
    for layer, rel in _LAYERS:
        rows = by_layer.get(layer, [])
        if not rows:
            continue
        pretty = {'canonical': 'Canonical headers (real java.* replacements)',
                  'reference': 'Platform references (native 360/D3D/XInput/XACT layer)',
                  'companion': 'Companions (bolt-on platform helpers)',
                  'overlay':   'Overlays (per-project hand-fixes, re-applied every convert)',
                  'generator': 'Generator / emitter / runtime / toolchain (the converter itself)'}[layer]
        lines.append('## %s' % pretty)
        lines.append('')
        lines.append('| Fix (slug) | Location | What it fixes |')
        lines.append('| --- | --- | --- |')
        for e in sorted(rows, key=lambda r: (r['file'], r['line'])):
            desc = (e['desc'] or '').replace('|', '\\|')
            if len(desc) > 220:
                desc = desc[:217] + '...'
            lines.append('| `%s` | %s:%d | %s |' % (e['slug'], e['file'], e['line'], desc))
        lines.append('')
    return '\n'.join(lines) + '\n'


def run(repo_root=None, out_path=None):
    """Collect markers and write BUGFIX_LEDGER.md (default: state/java/BUGFIX_LEDGER.md). `repo_root` is
    the folder whose layers are scanned (default: this package). Returns the entry list."""
    if repo_root is None:
        repo_root = os.path.dirname(os.path.abspath(__file__))
    entries = collect(repo_root)
    if out_path is None:
        from codebreaker.base import settings
        out_path = settings.state('java', 'BUGFIX_LEDGER.md')
    with open(out_path, 'w', encoding='utf-8') as f:
        f.write(render(entries))
    return entries
