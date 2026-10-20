# -*- coding: utf-8 -*-
"""Release edition profile (1.x).

Release builds on the Beta vocabulary and add registries/subsystems (achievements/stats, more world
providers, the client/server package split). This profile starts from the proven Beta set and layers
release-era package roles on top.

SCAFFOLD NOTE: the release line spans many versions with real structural change (block states, the
flattening, data-driven registries). Verify and expand the static-init seed and nullable vocabulary
against a decompile of the specific release version being targeted.
"""

from codebreaker.java.profiles import Profile, register
from codebreaker.java.projects.minecraft.profiles import minecraft_base as _base
from codebreaker.java.projects.minecraft.profiles import minecraft_xbox360 as _canon

PACKAGE_ROLES = dict(_canon.PACKAGE_ROLES)
PACKAGE_ROLES.update({
    'net.minecraft.src': 'Client-side game classes (release single-package layout).',
    'net.minecraft.client': 'Client game shell, input and main loop.',
    'net.minecraft.server': 'Integrated server: world ticking, save, and player management.',
})

register(Profile(
    'minecraft_release',
    static_init_order=list(_canon.STATIC_INIT_ORDER),
    package_roles=PACKAGE_ROLES,
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
