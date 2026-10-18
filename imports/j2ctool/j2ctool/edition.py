# -*- coding: utf-8 -*-
"""Edition resolution -- pick the per-era target profile for a conversion.

An "edition" names one of the six Minecraft eras; each maps to a registered target profile
(``rubydung`` -> ``minecraft_rubydung``, ...). :func:`resolve` chooses one by precedence:

    explicit --edition flag   (user override, highest)
      -> version-id mapping    (when --mc-version names a version)
      -> source auto-detection (heuristic over the parsed corpus)
      -> None                  (nothing conclusive -> caller keeps the default profile, a no-op)

Returning ``None`` on the uncertain path is deliberate: ``convert`` then never calls
``profiles.use()``, so the default (mixture) profile and today's exact output are preserved.

This module is pure and read-only: no SDK, no network, no decompile. Auto-detection runs on the
already-parsed corpus (each class exposes ``.name`` and ``.package``), so it costs nothing extra.
"""

import re

# Short edition name (CLI choice / return value) -> registered target-profile name.
EDITIONS = {
    'rubydung': 'minecraft_rubydung',
    'indev': 'minecraft_indev',
    'infdev': 'minecraft_infdev',
    'alpha': 'minecraft_alpha',
    'beta': 'minecraft_beta',
    'release': 'minecraft_release',
}

# Ordered (version-id regex, edition) rules. FIRST match wins, so more specific prefixes come first --
# `inf-` before `in`, the era letters before the bare release-digit rule. The version id is the string
# the decompiler backend understands (e.g. a1.2.6, b1.6.6, c0.30_01c, in-20100223, inf-20100618).
_VERSION_RULES = [
    (re.compile(r'^rd[-_]', re.I),          'rubydung'),   # rd-132211 (RubyDung)
    (re.compile(r'^c0?\.', re.I),           'rubydung'),   # c0.30_01c (classic ~ pre-classic handling)
    (re.compile(r'^classic', re.I),         'rubydung'),
    (re.compile(r'^inf[-_]', re.I),         'infdev'),     # inf-20100618
    (re.compile(r'^infdev', re.I),          'infdev'),
    (re.compile(r'^in[-_]', re.I),          'indev'),      # in-20100223
    (re.compile(r'^indev', re.I),           'indev'),
    (re.compile(r'^a', re.I),               'alpha'),      # a1.2.6
    (re.compile(r'^b', re.I),               'beta'),       # b1.6.6
    (re.compile(r'^\d+\.\d'),               'release'),    # 1.2.5
]


def edition_for_version(version_id):
    """Map a decompiler version id to a short edition name, or ``None`` if unrecognized."""
    if not version_id:
        return None
    v = version_id.strip()
    for pat, ed in _VERSION_RULES:
        if pat.match(v):
            return ed
    return None


def _corpus_sets(classes):
    names, pkgs = set(), set()
    for c in classes or ():
        nm = getattr(c, 'name', None)
        if nm:
            names.add(nm)
        pkg = getattr(c, 'package', None)
        if pkg:
            pkgs.add(pkg)
    return names, pkgs


def detect_edition(classes):
    """Best-effort edition guess from the parsed corpus, or ``None`` when nothing is conclusive.

    Conservative on purpose: it would rather return ``None`` (keep the default profile) than
    mis-detect. The decisive axis is CHUNK STREAMING, not the world class name -- the real Indev
    decompile names its finite world class ``World`` (in ``net.minecraft.game.level``) and has NO
    ``Chunk``/``ChunkProvider`` at all, so "World present" alone does NOT mean Alpha. Markers:
      * ``com.mojang.rubydung`` package / ``RubyDung`` class                 -> rubydung (pre-classic)
      * ``net.minecraft.server`` split / block-state classes                -> release
      * ``BiomeGenBase``/``RegionFile``/``AnvilChunkLoader``/``WorldChunkManager`` -> beta
      * has ``Chunk``/``ChunkProvider`` (streamed world), no biome markers:
            ``net.minecraft.game.*`` layout -> infdev ; flat ``net.minecraft.src`` -> alpha
      * NO chunk streaming (finite world), ``net.minecraft.game.level`` / ``LevelLoader`` -> indev
    """
    names, pkgs = _corpus_sets(classes)
    if not names:
        return None

    def has_pkg(prefix):
        return any(p == prefix or p.startswith(prefix + '.') for p in pkgs)

    # Pre-classic: the package (or main class) is unmistakable.
    if has_pkg('com.mojang.rubydung') or 'RubyDung' in names:
        return 'rubydung'

    has_chunk = 'Chunk' in names or any(n.startswith('ChunkProvider') for n in names)
    # Beta-ONLY classes. WorldChunkManager / ChunkProviderGenerate are NOT markers: Alpha 1.2.x already has
    # both (biomes arrived there, as MobSpawnerBase), so they misfiled Alpha corpora as beta. These are
    # Beta additions: beds/cake/note/dispenser/dye (b1.2-1.3), McRegion RegionFile (b1.3), stats and
    # achievements (b1.5), the BiomeGenBase rename (b1.8), Anvil (carried into release).
    beta_markers = {'BiomeGenBase', 'RegionFile', 'AnvilChunkLoader', 'BlockBed', 'BlockCake',
                    'BlockNote', 'BlockDispenser', 'ItemDye', 'StatList', 'Achievement'}
    release_markers = {'IBlockState', 'BlockState', 'WorldProviderEnd'}

    # Release: dedicated-server package split or block-state classes.
    if has_pkg('net.minecraft.server') or (release_markers & names):
        return 'release'
    # Beta: biome / region-file / anvil world storage.
    if beta_markers & names:
        return 'beta'
    # Chunk-streamed but pre-biome worlds: infdev (early game-package layout) vs alpha (flat src).
    if has_chunk:
        if has_pkg('net.minecraft.game') and not has_pkg('net.minecraft.src'):
            return 'infdev'
        return 'alpha'
    # Finite-world era (no chunk streaming). Real Indev = net.minecraft.game.level.World + LevelLoader.
    if has_pkg('net.minecraft.game.level') or 'LevelLoader' in names:
        return 'indev'
    if ('World' in names or 'Level' in names) and ({'Block', 'Material', 'Tile'} & names):
        return 'indev'

    return None


def profile_name(edition):
    """Map a short edition name to its registered target-profile name, or ``None`` if unknown."""
    return EDITIONS.get(edition) if edition else None


def resolve(explicit=None, mc_version=None, classes=None):
    """Resolve the target-profile name to activate, or ``None`` to keep the current default.

    Precedence: explicit edition > version-id mapping > source auto-detection > None.
    ``explicit`` and the detected value may be either a short edition name (``'alpha'``) or an already
    -qualified profile name (``'minecraft_alpha'``); both resolve correctly.
    """
    # 1. Explicit override.
    if explicit:
        if explicit in EDITIONS:
            return EDITIONS[explicit]
        if explicit in EDITIONS.values():
            return explicit
        # Unknown explicit value: surface it rather than silently ignoring.
        raise ValueError('unknown edition %r (choices: %s)'
                         % (explicit, ', '.join(sorted(EDITIONS))))

    # 2. Version-id mapping.
    ed = edition_for_version(mc_version)
    if ed:
        return EDITIONS[ed]

    # 3. Source auto-detection.
    ed = detect_edition(classes)
    if ed:
        return EDITIONS[ed]

    # 4. Nothing conclusive -> caller keeps the default profile (a strict no-op).
    return None
