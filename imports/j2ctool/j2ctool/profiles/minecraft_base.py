# -*- coding: utf-8 -*-
"""Shared, era-invariant building blocks for the per-edition Minecraft target profiles.

This module is DATA, not a registered profile. The six edition profiles
(``minecraft_rubydung`` ... ``minecraft_release``) compose their own `Profile` from these shared
pieces plus their era-specific overrides (static-init seed, nullable vocabulary, package roles).

The canonical, proven taxonomy already lives in :mod:`minecraft_xbox360` (a RubyDung + Alpha + Beta
*mixture* hand-tuned over the alpha port). Rather than duplicate ~55 filter rules and the AWT/DOM
dead-type regex, we import them from there and re-expose the era-invariant subset. `minecraft_xbox360`
stays the untouched default profile; this layer sits *on top* of it.

What is genuinely era-invariant (shared here):
  * the .vcxproj/.filters classification taxonomy -- extra regex rules for classes an era doesn't
    have are simply inert, so a generous SUPERSET is correct and robust for every edition;
  * the AWT/swing/applet/DOM dead-type regex (no console has a desktop window in any era);
  * the heavy/owning types that must never be per-frame heap-allocated (Tesselator/NBT...).

What is era-SPECIFIC (each edition profile overrides): ``static_init_order`` (the class set and
ctor-mediated deps differ by era), the nullable field/getter vocabulary (API identifiers differ), and
``package_roles`` (package layout differs: com.mojang.rubydung vs net.minecraft.* vs ...).
"""

from .minecraft_xbox360 import (
    DEAD_TYPES,
    HEAVY_TYPES,
    PACKAGE_ROLES as _XBOX_PACKAGE_ROLES,
    VCXPROJ_FILTERS as _FULL_FILTERS,
)

# Era-invariant re-exports (single source of truth = minecraft_xbox360).
SHARED_DEAD_TYPES = DEAD_TYPES
SHARED_HEAVY_TYPES = HEAVY_TYPES

# The full proven filter taxonomy already spans RubyDung/pre-classic AND Beta rules, so it classifies
# every era's class names. Editions reuse it via build_filters(); an edition may PREPEND a few
# era-specific rules (first-match-wins) without restating the whole table.
FULL_FILTERS = list(_FULL_FILTERS)

# Package roles that are not tied to one era's package layout (the platform/runtime/util buckets).
# Editions supply their own game-package roles on top of these.
SHARED_PACKAGE_ROLES = {
    'util': _XBOX_PACKAGE_ROLES.get('util', 'Standalone utility helpers (math, noise, etc.).'),
}


def build_filters(era_rules=()):
    """The .vcxproj/.filters rules for an edition: era-specific rules FIRST (they win under
    first-match-wins), then the shared full taxonomy. Pass ``era_rules`` to add a handful of
    edition-only classifications; omit it to use the shared taxonomy verbatim."""
    return list(era_rules) + FULL_FILTERS
