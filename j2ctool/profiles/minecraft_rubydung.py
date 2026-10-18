# -*- coding: utf-8 -*-
"""RubyDung (pre-classic) edition profile.

Package: ``com.mojang.rubydung`` (+ ``.level`` / ``.character`` / ``.phys``). The world is a single
flat ``Level`` (no chunks, no Block/Item registries yet); blocks are the pre-classic ``Tile`` class
whose static instances (``Tile.rock``/``grass``/...) populate ``Tile.tiles[]``. Rendering is the
one-'l' ``Tesselator`` + immediate-mode GL.

SCAFFOLD NOTE: the nullable vocabulary below is intentionally minimal and should be verified/expanded
against a real RubyDung decompile (see the rubydung-reconvert-setup memory for the source path). The
crash-class RULES (uninit arrays, Rule-of-Three, ctor delegation, static-init) are engine-level and
apply regardless of this vocabulary.
"""

from . import Profile, register
from . import minecraft_base as _base

PACKAGE_ROLES = dict(_base.SHARED_PACKAGE_ROLES)
PACKAGE_ROLES.update({
    'com.mojang.rubydung': 'Top-level pre-classic game shell, main loop and input.',
    'com.mojang.rubydung.level': 'The flat Level model, tiles, level renderer and frustum.',
    'com.mojang.rubydung.character': 'Player and mob (Zombie) entities.',
    'com.mojang.rubydung.phys': 'Collision primitives (AABB).',
})

# Tile holds the static tile instances (Tile.rock/grass/...) and the Tile.tiles[] registry, so its
# statics must init first (they are read when the Level/renderer builds).
STATIC_INIT_ORDER = ['Tile']

# Tile.tiles[] has NULL slots for unregistered tile ids -- deref of an unregistered slot is a virtual
# call on NULL (IP 0x0 on the console).
NULLABLE = {'tiles'}

# Fields legitimately NULL at points in the lifecycle (set after ctor / cleared on shutdown).
NULLABLE_FIELDS = {'level', 'player', 'levelRenderer', 'particleEngine'}

NULLABLE_GETTERS = set()
NULLABLE_GETTER_RETURN_TYPE = {}

register(Profile(
    'minecraft_rubydung',
    static_init_order=STATIC_INIT_ORDER,
    package_roles=PACKAGE_ROLES,
    heavy_types=_base.SHARED_HEAVY_TYPES,
    vcxproj_filters=_base.build_filters(),
    dead_types=_base.SHARED_DEAD_TYPES,
    nullable=NULLABLE,
    nullable_fields=NULLABLE_FIELDS,
    nullable_getters=NULLABLE_GETTERS,
    nullable_getter_return_type=NULLABLE_GETTER_RETURN_TYPE,
    unsigned_byte_scalar_fields=set(),
    default_value_types=set(),
))
