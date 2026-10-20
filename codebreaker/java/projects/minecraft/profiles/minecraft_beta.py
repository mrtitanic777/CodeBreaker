# -*- coding: utf-8 -*-
"""Beta edition profile.

The canonical :mod:`minecraft_xbox360` profile is predominantly a Beta port (World/Chunk/Region-NBT,
biomes, the full block/entity/GUI taxonomy). This profile reuses that proven vocabulary verbatim; it
is the reference edition and the natural target of the (optional, later) `minecraft_xbox360` alias.
"""

from codebreaker.java.profiles import Profile, register
from codebreaker.java.projects.minecraft.profiles import minecraft_base as _base
from codebreaker.java.projects.minecraft.profiles import minecraft_xbox360 as _canon

register(Profile(
    'minecraft_beta',
    static_init_order=list(_canon.STATIC_INIT_ORDER),
    package_roles=dict(_canon.PACKAGE_ROLES),
    heavy_types=_base.SHARED_HEAVY_TYPES,
    vcxproj_filters=_base.build_filters(),
    dead_types=_base.SHARED_DEAD_TYPES,
    nullable=set(_canon.NULLABLE),
    nullable_fields=set(_canon.NULLABLE_FIELDS),
    nullable_getters=set(_canon.NULLABLE_GETTERS),
    nullable_getter_return_type=dict(_canon.NULLABLE_GETTER_RETURN_TYPE),
    unsigned_byte_scalar_fields=set(),
    default_value_types=set(),
))
