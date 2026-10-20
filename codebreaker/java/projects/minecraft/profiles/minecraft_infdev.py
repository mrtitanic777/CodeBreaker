# -*- coding: utf-8 -*-
"""Infdev edition profile.

Infdev introduces INFINITE worlds: the flat finite Level is replaced by chunk streaming
(``Chunk`` + a chunk provider), and world save/load moves to per-chunk storage. Block/Item/Material
registries carry over from Indev.

SCAFFOLD NOTE: verify the nullable vocabulary (esp. the chunk getters) and static-init seed against a
real Infdev decompile.
"""

from codebreaker.java.profiles import Profile, register
from codebreaker.java.projects.minecraft.profiles import minecraft_base as _base

PACKAGE_ROLES = dict(_base.SHARED_PACKAGE_ROLES)
PACKAGE_ROLES.update({
    'com.mojang.minecraft': 'Top-level game shell, main loop and input.',
    'com.mojang.minecraft.level': 'The infinite world model, chunk streaming, generation, save/load.',
    'com.mojang.minecraft.level.tile': 'Block/Tile definitions and per-block behaviour.',
    'com.mojang.minecraft.character': 'Player and mob entities.',
    'com.mojang.minecraft.phys': 'Collision primitives (AABB).',
})

STATIC_INIT_ORDER = ['Material', 'Block', 'Item']

# Registry arrays with NULL slots.
NULLABLE = {'blocksList', 'itemsList'}

NULLABLE_FIELDS = {'level', 'player', 'levelRenderer', 'particleEngine', 'textures'}

# Chunk lookups can miss (unloaded/ungenerated chunk) -> NULL.
NULLABLE_GETTERS = {'getChunk', 'getChunkFromChunkCoords', 'getChunkFromBlockCoords'}
NULLABLE_GETTER_RETURN_TYPE = {
    'getChunk': 'Chunk',
    'getChunkFromChunkCoords': 'Chunk',
    'getChunkFromBlockCoords': 'Chunk',
}

register(Profile(
    'minecraft_infdev',
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
