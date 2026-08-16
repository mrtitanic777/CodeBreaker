# -*- coding: utf-8 -*-
r"""
apimap.py -- the API Mapping Registry: a data-driven, cross-language / cross-API function
equivalence database.

WHY THIS EXISTS
---------------
The converter used to bake "what maps to what" into the engine, tuned to one game's exact API
surface. A different game -- or a different *version* of the same game -- touches different library
calls and falls off the edge of what was hardcoded. That is a per-version treadmill.

The registry breaks that: the *engine* only translates language SYNTAX (statements, types, generics).
Everything about which library/API call becomes which other call is DATA -- rows in this registry --
so a new version is just a different set of rows resolved by the same code, and "what's still
unmapped" is an enumerable report instead of a mystery compile error.

WHAT A ROW IS
-------------
Not name->name. A mapping carries a KIND, because equivalence is rarely 1:1:

    direct            1:1 rename, same semantics          Math.sqrt      -> sqrtf
    signature-adapted same idea, different args/semantics String.substring (bounds differ)
    macro             expands to a small expression       list.size()    -> (int)v.size()
    constant          a value/enum constant, not a call   GL_TRIANGLES   -> D3DPT_TRIANGLELIST
    shim              resolves to a hand-written helper    glVertex3f     -> j2c::gl::vertex3f
    unsupported       no mapping; emit a clear diagnostic  (enumerable gap, not a build break)

The hard cases (immediate-mode GL -> Direct3D) still need hand-written shims -- the registry does not
eliminate them, it *indexes* them so they are written once, registered, and never silently re-break.

DATA LAYOUT
-----------
JSON under the bundled ``apimap/`` directory (resolved via _paths.data_path, so it ships in the exe):

    apimap/languages.json         the catalog of language/API "slots" (java, cpp, opengl, direct3d9,
                                  python, csharp, c, asm.x86, ...): the whole idea's namespace.
    apimap/<subsystem>.json       one file per source subsystem (java.util, opengl, nio, ...), each a
                                  set of symbols and, per symbol, its mapping into one or more targets.

A subsystem file:

    {
      "subsystem": "opengl",
      "language":  "opengl",
      "description": "...",
      "symbols": {
        "glVertex3f": {
          "signature": "(float,float,float)",
          "doc": "Immediate-mode vertex.",
          "maps": {
            "direct3d9": {"target": "j2c::gl::vertex3f", "kind": "shim",
                          "note": "accumulates into a vertex buffer, flushed on glEnd"}
          }
        }
      }
    }

The legacy per-CLASS Java->Xbox360 intent map (platform_equivalents.EQUIV) is BRIDGED in at load so
none of that knowledge is lost; new work is authored as per-function JSON rows.
"""

import json
import os
import re

from ._paths import data_path

KINDS = ('direct', 'signature-adapted', 'macro', 'constant', 'shim', 'unsupported')


class Mapping(object):
    """One directed equivalence: source symbol (in src_lang) -> target (in dst_lang).

    ``dialect`` records which language *version* the resolved target was selected for (e.g. the
    ``nullptr`` target of a null literal is chosen only for a c++11+ dialect); ``None`` means the
    row is version-agnostic."""
    __slots__ = ('name', 'src_lang', 'dst_lang', 'target', 'kind', 'note',
                 'adapter', 'since', 'subsystem', 'reverse', 'dialect')

    def __init__(self, name, src_lang, dst_lang, target, kind='direct', note='',
                 adapter=None, since=None, subsystem='', reverse=False, dialect=None):
        self.name = name
        self.src_lang = src_lang
        self.dst_lang = dst_lang
        self.target = target
        self.kind = kind
        self.note = note
        self.adapter = adapter
        self.since = since
        self.subsystem = subsystem
        self.reverse = reverse            # True if synthesized by inverting a forward row
        self.dialect = dialect            # dialect the target was selected for (or None)

    @property
    def supported(self):
        return self.kind != 'unsupported'

    def __repr__(self):
        arrow = '<-' if self.reverse else '->'
        d = ('@' + self.dialect) if self.dialect else ''
        return 'Mapping(%s:%s %s %s:%s%s [%s])' % (
            self.src_lang, self.name, arrow, self.dst_lang, self.target, d, self.kind)


def _norm(name):
    """Normalize a source symbol to registry lookup keys, most-specific first.

    Accepts things the emitter actually sees -- ``org.lwjgl.opengl.GL11.glVertex3f``,
    ``GL11.glVertex3f``, ``ArrayList.get``, ``glVertex3f`` -- and yields candidate keys:
    the fully-qualified tail ``Holder.method`` and the bare leaf ``method``.
    """
    name = name.strip()
    keys = [name]
    parts = name.split('.')
    if len(parts) >= 2:
        holder_method = parts[-2] + '.' + parts[-1]
        if holder_method not in keys:
            keys.append(holder_method)
    leaf = parts[-1]
    if leaf not in keys:
        keys.append(leaf)
    return keys


def _leaf(target):
    """Last identifier of a target symbol, for building the reverse index.
    ``std::vector::push_back`` -> ``push_back``; ``j2c::gl::vertex3f`` -> ``vertex3f``."""
    t = re.split(r'::|\.', target.strip())[-1]
    t = re.sub(r'\(.*$', '', t)               # drop any trailing signature
    return t.strip()


class Registry(object):
    def __init__(self):
        self.languages = {}       # id -> {'kind':'language'|'api', 'desc':str}
        self.subsystems = {}      # subsystem id -> {'language':.., 'description':..}
        self._sym = {}            # (src_lang, key) -> {'maps': {...}, 'subsystem':.., 'doc':..}
        self._rev = {}            # (dst_lang, leaf, src_lang) -> Mapping (synthesized)

    # ---- population -------------------------------------------------------
    def _add_symbol(self, src_lang, name, entry, subsystem):
        maps = entry.get('maps', {})
        rec = {'maps': maps, 'subsystem': subsystem,
               'doc': entry.get('doc', ''), 'signature': entry.get('signature', '')}
        for key in _norm(name):
            self._sym.setdefault((src_lang, key), rec)
        for dst_lang, m in maps.items():
            # index the base target AND every by_version override target for reverse lookup
            variants = [(m.get('target', ''), m.get('kind', 'direct'))]
            for o in (m.get('by_version') or {}).values():
                if o.get('target'):
                    variants.append((o['target'], o.get('kind', m.get('kind', 'direct'))))
            for tgt, kind in variants:
                if not tgt:
                    continue
                rmap = Mapping(name=tgt, src_lang=dst_lang, dst_lang=src_lang, target=name,
                               kind=kind, note=m.get('note', ''), subsystem=subsystem, reverse=True)
                self._rev.setdefault((dst_lang, _leaf(tgt), src_lang), rmap)

    def add_subsystem_doc(self, sub_id, language, description):
        self.subsystems[sub_id] = {'language': language, 'description': description}

    # ---- version / dialect axis -----------------------------------------
    def dialect_order(self, lang):
        """The ordered (oldest -> newest) list of dialect ids a language declares, e.g. cpp ->
        ['c++98','c++03','c++11',...]. Empty if the language declares no versions."""
        return self.languages.get(lang, {}).get('dialects', []) or []

    def _select_version(self, m, dst_lang, dialect):
        """Given a raw map entry ``m`` and a requested target ``dialect``, return the effective
        (target, kind, note, adapter, since, chosen_dialect). A row may carry ``by_version``
        overrides keyed by dialect id; the NEWEST override at-or-below the requested dialect wins,
        so a c++11 request picks up a c++11 row and a c++98 request falls back to the base."""
        base = (m.get('target', ''), m.get('kind', 'direct'), m.get('note', ''),
                m.get('adapter'), m.get('since'), None)
        by = m.get('by_version')
        if not by or not dialect:
            return base
        order = self.dialect_order(dst_lang)
        if dialect in order:
            # walk newest->oldest within the prefix up to the requested dialect
            for ver in reversed(order[:order.index(dialect) + 1]):
                if ver in by:
                    o = by[ver]
                    return (o.get('target', base[0]), o.get('kind', base[1]),
                            o.get('note', base[2]), o.get('adapter', base[3]),
                            o.get('since', ver), ver)
            return base
        # no ordering info: honor only an exact match
        if dialect in by:
            o = by[dialect]
            return (o.get('target', base[0]), o.get('kind', base[1]), o.get('note', base[2]),
                    o.get('adapter', base[3]), o.get('since', dialect), dialect)
        return base

    # ---- queries ---------------------------------------------------------
    def resolve(self, name, src_lang, dst_lang, dialect=None):
        """Forward lookup: how does ``name`` (a symbol in src_lang) map into dst_lang, optionally
        for a specific target ``dialect`` (e.g. 'c++03', 'c++11')? Returns a Mapping, or None."""
        for key in _norm(name):
            rec = self._sym.get((src_lang, key))
            if rec and dst_lang in rec['maps']:
                tgt, kind, note, adapter, since, chosen = self._select_version(
                    rec['maps'][dst_lang], dst_lang, dialect)
                return Mapping(name=key, src_lang=src_lang, dst_lang=dst_lang,
                               target=tgt, kind=kind, note=note, adapter=adapter,
                               since=since, subsystem=rec['subsystem'], dialect=chosen)
        return None

    def reverse(self, target, dst_lang, src_lang):
        """Inverse lookup (best-effort): a dst_lang symbol -> the src_lang symbol it came from."""
        return self._rev.get((dst_lang, _leaf(target), src_lang))

    def known(self, name, src_lang):
        """True if the registry has ANY mapping for this source symbol (any target)."""
        return any((src_lang, k) in self._sym for k in _norm(name))

    def coverage(self, names, src_lang, dst_lang, dialect=None):
        """Classify a set of used source symbols against the src->dst map (for a given dialect).
        Returns {'mapped':[(name,Mapping)], 'unsupported':[(name,Mapping)], 'unmapped':[name]}."""
        mapped, unsupported, unmapped = [], [], []
        for name in sorted(set(names)):
            m = self.resolve(name, src_lang, dst_lang, dialect)
            if m is None:
                unmapped.append(name)
            elif m.kind == 'unsupported':
                unsupported.append((name, m))
            else:
                mapped.append((name, m))
        return {'mapped': mapped, 'unsupported': unsupported, 'unmapped': unmapped}

    def symbols_in(self, subsystem):
        """All (canonical_name, entry) for a subsystem id. Each record is indexed under several
        keys (qualified + leaf aliases); collapse to one row per record, preferring the most
        qualified (longest, dotted) key as the canonical name."""
        best = {}
        for (lang, key), rec in self._sym.items():
            if rec['subsystem'] != subsystem:
                continue
            rid = id(rec)
            cur = best.get(rid)
            if cur is None or ('.' in key and '.' not in cur[0]) or len(key) > len(cur[0]):
                best[rid] = (key, rec)
        return sorted(best.values())

    def stats(self):
        pairs = {}
        for rec in {id(r): r for r in self._sym.values()}.values():
            for dst in rec['maps']:
                pairs[(rec['subsystem'], dst)] = pairs.get((rec['subsystem'], dst), 0) + 1
        return pairs


# --------------------------------------------------------------------------- #
#  Loading                                                                     #
# --------------------------------------------------------------------------- #

_CACHE = None


def _bridge_equiv(reg):
    """Fold the legacy per-CLASS Java->Xbox360 intent map into the registry as coarse
    java->direct3d9/xdk rows, so its knowledge is queryable through the same API. These are
    HOLDER-level (a class, e.g. GL11/Socket/File), complementing the per-function JSON rows."""
    try:
        from . import platform_equivalents as pe
    except Exception:
        return
    status_kind = {'real': 'shim', 'stub': 'shim', 'dead': 'unsupported'}
    for subsystem, basename, e in pe.all_entries():
        kind = status_kind.get(e.get('status'), 'shim')
        note = (e.get('note') or '').strip()
        entry = {'doc': note, 'maps': {'xbox360': {
            'target': e.get('xdk', ''), 'kind': kind,
            'note': ('%s [%s]' % (note, e.get('lib', ''))).strip()}}}
        reg._add_symbol('java', basename, entry, 'java.platform:' + subsystem)
    reg.add_subsystem_doc('java.platform', 'java',
                          'Legacy per-class Java-platform -> Xbox 360 XDK intent map (bridged from '
                          'platform_equivalents.EQUIV).')


def load(force=False):
    """Load (and cache) the registry from the bundled apimap/ JSON plus the EQUIV bridge."""
    global _CACHE
    if _CACHE is not None and not force:
        return _CACHE
    reg = Registry()
    root = data_path('apimap')

    # 1. language / API catalog
    lang_file = os.path.join(root, 'languages.json')
    if os.path.isfile(lang_file):
        try:
            with open(lang_file, 'r', encoding='utf-8') as f:
                cat = json.load(f)
            reg.languages = cat.get('languages', {})
        except (IOError, OSError, ValueError):
            pass

    # 2. every subsystem file (any *.json except the catalog)
    try:
        files = sorted(f for f in os.listdir(root)
                       if f.endswith('.json') and f != 'languages.json')
    except OSError:
        files = []
    for fn in files:
        try:
            with open(os.path.join(root, fn), 'r', encoding='utf-8') as f:
                data = json.load(f)
        except (IOError, OSError, ValueError):
            continue
        sub_id = data.get('subsystem', os.path.splitext(fn)[0])
        lang = data.get('language', sub_id.split('.')[0])
        reg.add_subsystem_doc(sub_id, lang, data.get('description', ''))
        for name, entry in data.get('symbols', {}).items():
            reg._add_symbol(lang, name, entry, sub_id)

    # 3. bridge the legacy EQUIV table
    _bridge_equiv(reg)

    _CACHE = reg
    return reg


# --------------------------------------------------------------------------- #
#  Symbol extraction (for coverage reports over real source)                  #
# --------------------------------------------------------------------------- #

# Holder.method(  and  bareStaticCall(  patterns as they appear in Java / decompiled Java.
_CALL_QUALIFIED = re.compile(r'\b([A-Z][A-Za-z0-9_]*)\.([a-z][A-Za-z0-9_]*)\s*\(')
_CALL_CONST = re.compile(r'\b([A-Z][A-Za-z0-9_]*)\.([A-Z][A-Z0-9_]{2,})\b')


def extract_symbols(text):
    """Best-effort set of external API symbols referenced in a chunk of Java source:
    ``Holder.method`` calls and ``Holder.CONSTANT`` references."""
    out = set()
    for m in _CALL_QUALIFIED.finditer(text):
        out.add(m.group(1) + '.' + m.group(2))
    for m in _CALL_CONST.finditer(text):
        out.add(m.group(1) + '.' + m.group(2))
    return out


def extract_symbols_from_dir(java_dir):
    """Union of extract_symbols() over every .java under java_dir."""
    syms = set()
    for root, _dirs, files in os.walk(java_dir):
        for fn in files:
            if not fn.endswith('.java'):
                continue
            try:
                with open(os.path.join(root, fn), 'r', encoding='utf-8', errors='replace') as f:
                    syms |= extract_symbols(f.read())
            except (IOError, OSError):
                continue
    return syms


# --------------------------------------------------------------------------- #
#  Coverage report                                                            #
# --------------------------------------------------------------------------- #

def render_coverage_md(names, src_lang, dst_lang, dialect=None, reg=None, title=None):
    """Render an APIMAP.md coverage report: which used source symbols the registry can map into
    the target language/dialect, which are explicitly unsupported, and which are still unmapped
    (the enumerable gap list). Unknown/local symbols that resolve to nothing are folded into
    'unmapped' -- most are the game's own classes, so this is a superset worth eyeballing, not a
    strict TODO."""
    reg = reg or load()
    cov = reg.coverage(names, src_lang, dst_lang, dialect)
    dia = (' (%s)' % dialect) if dialect else ''
    L = ['# API Mapping Coverage',
         '',
         '**%s -> %s%s**' % (src_lang, dst_lang, dia),
         '',
         (title or 'Symbols referenced by the source, resolved against the API-mapping registry.'),
         '',
         '| status | count |', '|---|---|',
         '| mapped | %d |' % len(cov['mapped']),
         '| unsupported (no target) | %d |' % len(cov['unsupported']),
         '| unmapped (unknown / local) | %d |' % len(cov['unmapped']),
         '']
    if cov['mapped']:
        L += ['## Mapped', '', '| symbol | target | kind | note |', '|---|---|---|---|']
        for name, m in cov['mapped']:
            L.append('| `%s` | `%s` | %s | %s |' % (name, m.target, m.kind, (m.note or '').replace('|', '\\|')))
        L.append('')
    if cov['unsupported']:
        L += ['## Unsupported (registered, but no target equivalent)', '',
              '| symbol | note |', '|---|---|']
        for name, m in cov['unsupported']:
            L.append('| `%s` | %s |' % (name, (m.note or '').replace('|', '\\|')))
        L.append('')
    if cov['unmapped']:
        L += ['## Unmapped (unknown to the registry -- mostly the source\'s own classes)', '']
        L += ['- `%s`' % n for n in cov['unmapped']]
        L.append('')
    return '\n'.join(L)
