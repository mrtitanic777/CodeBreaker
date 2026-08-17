# -*- coding: utf-8 -*-
"""Target profiles -- the seam that keeps the transpiler core game-agnostic.

Game- and platform-specific data (class names, package roles, nullable fields/getters, the
static-init seed, project-filter rules) lives in a *profile* rather than being hardcoded in the
core logic modules. The core reads it through `active()`; swapping the active profile retargets the
tool. The bundled default is `minecraft_xbox360`; a `generic` profile supplies empty defaults.

NOTE: this is distinct from the repo-root ``profiles/`` directory, which holds per-conversion TYPE
override configs (e.g. ``alpha.json``). This package is the built-in TARGET-profile registry.
"""


class Profile(object):
    """A bag of target-specific data. Every field defaults to empty, so the generic core behaves
    game-agnostically until a profile supplies real data."""

    def __init__(self, name, static_init_order=None, package_roles=None, heavy_types=None,
                 vcxproj_filters=None, nullable=None, nullable_fields=None, nullable_getters=None,
                 nullable_getter_return_type=None, unsigned_byte_scalar_fields=None,
                 default_value_types=None, dead_types=None, optional_types=None):
        self.name = name
        self.static_init_order = list(static_init_order or [])
        self.package_roles = dict(package_roles or {})
        self.heavy_types = tuple(heavy_types or ())
        self.vcxproj_filters = list(vcxproj_filters or [])
        # Anchored regex (string) matching class BASENAMES that are DEAD on this target -- external
        # desktop/GUI types (java.awt / javax.swing / java.applet, org.w3c.dom) that cannot function on
        # the console. The pipeline drops any dead type that is ALSO unreferenced (reachability is the
        # safety net). Precise by design: explicit class names, never `\w*Listener`-style wildcards that
        # would sweep in real game types. None -> the profile prunes nothing.
        self.dead_types = dead_types
        # Anchored regex (string) matching class BASENAMES of OPTIONAL platform SUBSYSTEMS -- self-contained
        # 360 backend bundles (audio, networking, ...) that only matter if the game actually uses them.
        # Pruned by the SAME reachability pass as dead_types: if no live file reaches the bundle, the whole
        # bundle is dropped (a title with no sound / no netplay never ships the audio / net layer); if the
        # game does use it, reachability keeps it. None -> nothing optional to prune.
        self.optional_types = optional_types
        self.nullable = set(nullable or set())
        self.nullable_fields = set(nullable_fields or set())
        self.nullable_getters = set(nullable_getters or set())
        self.nullable_getter_return_type = dict(nullable_getter_return_type or {})
        self.unsigned_byte_scalar_fields = set(unsigned_byte_scalar_fields or set())
        self.default_value_types = set(default_value_types or set())


_REGISTRY = {}


def register(profile):
    _REGISTRY[profile.name] = profile
    return profile


def get(name):
    return _REGISTRY[name]


def names():
    return sorted(_REGISTRY)


# The empty, game-agnostic baseline.
GENERIC = register(Profile('generic'))

# Register the bundled target profile(s). (minecraft_xbox360 self-registers on import.)
from . import minecraft_xbox360  # noqa: E402,F401

# Default active profile preserves the tool's out-of-the-box (Minecraft/Xbox-360) behavior.
_active = _REGISTRY.get('minecraft_xbox360', GENERIC)


def active():
    """The currently-active target profile."""
    return _active


def use(name):
    """Switch the active target profile by name; returns it."""
    global _active
    _active = get(name)
    return _active
