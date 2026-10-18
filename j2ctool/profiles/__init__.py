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

# Observers reseeded whenever the active profile changes. Core modules (typemap, rules, vcxproj,
# signatures, pipeline) freeze profile data into module-level globals for speed; without a
# notification they would keep the profile that was active AT IMPORT TIME forever (and `use()` would
# be a silent no-op). Each such module registers a `_reseed(profile)` callback here so switching the
# active profile actually re-derives those globals. See use().
_ON_CHANGE = []


def on_change(fn):
    """Register a callback invoked as ``fn(profile)`` every time the active profile changes (and it is
    the caller's job to call it once for the current profile if they need an immediate seed -- the
    import-time global assignment already does that). Returns ``fn`` so it can be used as a decorator."""
    _ON_CHANGE.append(fn)
    return fn


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

# Per-edition Minecraft profiles (shared minecraft_base data + era overrides). Each self-registers on
# import. minecraft_base is data-only (imported by the era modules), so it need not be imported here.
from . import (  # noqa: E402,F401
    minecraft_rubydung, minecraft_indev, minecraft_infdev,
    minecraft_alpha, minecraft_beta, minecraft_release,
)

# Default active profile preserves the tool's out-of-the-box (Minecraft/Xbox-360) behavior.
_active = _REGISTRY.get('minecraft_xbox360', GENERIC)


def active():
    """The currently-active target profile."""
    return _active


def use(name):
    """Switch the active target profile by name; returns it.

    Fires every ``on_change`` observer so the core modules re-derive their frozen profile globals
    from the new profile -- this is what makes a runtime profile switch actually take effect."""
    global _active
    _active = get(name)
    for fn in _ON_CHANGE:
        fn(_active)
    return _active
