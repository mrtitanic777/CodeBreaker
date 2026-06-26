# -*- coding: utf-8 -*-
"""
gen_signatures.py -- generate signatures.md, a single-file map of the whole game
codebase (every class, grouped by subsystem/package, with inheritance, fields and
method signatures, showing BOTH the Java spelling and the resolved C++03 type).

It does NOT re-scan the source by hand: it drives the existing j2x front-end
(parse -> IR) and the j2x type resolver (TypeResolver), the very same pieces the
converter uses, so the map stays faithful to what the converter actually sees.

Run from the repo root so `import j2x` works:

    python tools/gen_signatures.py

Reads:  original_java/*.java
Writes: signatures.md   (at the repo root)
"""

import os
import sys

# Make `import j2x` work no matter where this is launched from: the repo root is
# the parent of this tools/ directory.
_REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if _REPO_ROOT not in sys.path:
    sys.path.insert(0, _REPO_ROOT)

from j2x import parse, pipeline  # noqa: E402

JAVA_DIR = os.path.join(_REPO_ROOT, 'original_java')
OUT_PATH = os.path.join(_REPO_ROOT, 'signatures.md')

DEFAULT_PKG = '(default package)'


# ---------------------------------------------------------------------------
# Subsystem (package) roles -- the ONE place we add human judgement. A short,
# one-line role is inferred per package; we key on the package name and fall
# back to a generic note built from the trailing segment if unmapped.
# ---------------------------------------------------------------------------
_PACKAGE_ROLES = {
    'net.minecraft.game': 'Top-level game wiring and shared game-wide types.',
    'net.minecraft.game.entity': 'Entity base classes and the entity update/movement core.',
    'net.minecraft.game.entity.animal': 'Passive/animal mobs.',
    'net.minecraft.game.entity.monster': 'Hostile mobs.',
    'net.minecraft.game.entity.misc': 'Non-living entities (items, projectiles, TNT, particles).',
    'net.minecraft.game.entity.player': 'The player entity and player inventory.',
    'net.minecraft.game.item': 'Items and item stacks (the inventory-side counterpart of blocks).',
    'net.minecraft.game.level': 'The world/level model: world container, chunks, generation, save/load.',
    'net.minecraft.game.level.block': 'Block definitions and per-block behaviour.',
    'net.minecraft.game.level.material': 'Block material properties (solidity, liquid, replaceable).',
    'net.minecraft.game.physics': 'Math/collision primitives: vectors, bounding boxes, ray hits.',
    'net.minecraft.game.tile': 'Tile entities (blocks with extra per-instance state, e.g. chests).',
    'net.minecraft.client': 'Client-side game shell, input and main loop.',
    'net.minecraft.client.render': 'Rendering: world/entity renderers and render helpers.',
    'net.minecraft.client.gui': 'GUI screens and HUD.',
    'net.minecraft.client.sound': 'Sound engine and sound playback.',
    'util': 'Standalone utility helpers (math, noise, etc.).',
}


def _package_role(pkg, class_names):
    """One-line role for a package. Mapped roles win; otherwise synthesize one
    from the trailing package segment so every subsystem still gets a line."""
    if pkg in _PACKAGE_ROLES:
        return _PACKAGE_ROLES[pkg]
    if pkg == DEFAULT_PKG:
        return 'Classes declared without a package.'
    seg = pkg.rsplit('.', 1)[-1]
    return 'Subsystem: %s (%d classes).' % (seg, len(class_names))


# ---------------------------------------------------------------------------
# Formatting helpers
# ---------------------------------------------------------------------------
def _kind_word(cls):
    if cls.is_interface:
        return 'interface'
    if cls.is_enum:
        return 'enum'
    return ('abstract class' if cls.is_abstract else 'class')


def _inheritance(cls):
    bits = []
    if cls.parent:
        bits.append('extends %s' % cls.parent)
    if cls.interfaces:
        bits.append('implements %s' % ', '.join(cls.interfaces))
    return (' : ' + ' '.join(bits)) if bits else ''


def _field_line(field, resolver):
    static = 'static ' if field.is_static else ''
    final = 'final ' if field.is_final else ''
    try:
        cpp = resolver.field_type(field.jtype)
    except Exception as e:  # noqa: BLE001 - never let one weird type abort the map
        cpp = '<resolve-error: %s>' % e
    return '  - %s%s%s %s   // C++: %s' % (
        static, final, repr(field.jtype), field.name, cpp)


def _method_line(method, resolver):
    flags = []
    if method.is_static:
        flags.append('static')
    if method.is_abstract:
        flags.append('abstract')
    prefix = ('[%s] ' % ' '.join(flags)) if flags else ''

    # Java spelling
    if method.is_ctor:
        jret = '(ctor)'
    elif method.return_type is None:
        jret = 'void'
    else:
        jret = repr(method.return_type)
    jparams = ', '.join('%s %s' % (repr(p.jtype), p.name) for p in method.params)

    # C++ spelling
    if method.is_ctor:
        cret = '(ctor)'
    elif method.return_type is None:
        cret = 'void'
    else:
        try:
            cret = resolver.return_type(method.return_type)
        except Exception as e:  # noqa: BLE001
            cret = '<resolve-error: %s>' % e
    cpp_params = []
    for p in method.params:
        try:
            cpp_params.append('%s %s' % (resolver.param_type(p.jtype), p.name))
        except Exception as e:  # noqa: BLE001
            cpp_params.append('<resolve-error: %s> %s' % (e, p.name))
    cparams = ', '.join(cpp_params)

    return '  - %s%s %s(%s)   // C++: %s %s(%s)' % (
        prefix, jret, method.name, jparams, cret, method.name, cparams)


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
def main():
    paths = pipeline.discover(JAVA_DIR)

    classes = []
    failures = []  # (basename, error message)
    for path in paths:
        try:
            classes.append(parse.parse_file(path))
        except Exception as e:  # noqa: BLE001 - one bad file shouldn't abort the map
            failures.append((os.path.basename(path), str(e)))

    # Resolver needs the FULL class set so user types resolve to value/pointer.
    resolver = pipeline.build_resolver(classes)

    # Group by package (the subsystems). Empty/None package -> default bucket.
    by_pkg = {}
    for c in classes:
        pkg = c.package or DEFAULT_PKG
        by_pkg.setdefault(pkg, []).append(c)
    for pkg in by_pkg:
        by_pkg[pkg].sort(key=lambda c: c.name)
    ordered_pkgs = sorted(by_pkg, key=lambda p: (p == DEFAULT_PKG, p))

    out = []
    w = out.append

    # --- Header -----------------------------------------------------------
    w('# Game codebase signatures')
    w('')
    w('Generated by tools/gen_signatures.py from original_java/ '
      '— regenerate with `python tools/gen_signatures.py`.')
    w('')
    w('Classes mapped: %d (across %d subsystems/packages).'
      % (len(classes), len(by_pkg)))
    if failures:
        w('Parse failures: %d (listed at the bottom).' % len(failures))
    w('')
    w('Each field/method shows the Java spelling and, after `// C++:`, the type '
      'the converter resolves it to (value vs `T*` pointer — i.e. `.` vs `->`).')
    w('')

    # --- Subsystem overview ----------------------------------------------
    w('## Subsystem overview')
    w('')
    for pkg in ordered_pkgs:
        cls_list = by_pkg[pkg]
        names = [c.name for c in cls_list]
        w('### %s' % pkg)
        w('%s' % _package_role(pkg, names))
        w('')
        w('Classes (%d): %s' % (len(names), ', '.join(names)))
        w('')

    # --- Classes ----------------------------------------------------------
    w('## Classes')
    w('')
    for pkg in ordered_pkgs:
        for cls in by_pkg[pkg]:
            w('### %s' % cls.name)
            w('%s%s' % (_kind_word(cls), _inheritance(cls)))
            w('package: %s' % pkg)
            w('')

            if cls.is_enum and cls.enum_consts:
                consts = ', '.join(
                    (ec.name + ('(%d)' % len(ec.args) if ec.args else ''))
                    for ec in cls.enum_consts)
                w('Enum constants (%d): %s' % (len(cls.enum_consts), consts))
                w('')

            w('Fields (%d):' % len(cls.fields))
            if cls.fields:
                for f in cls.fields:
                    w(_field_line(f, resolver))
            else:
                w('  - (none)')
            w('')

            w('Methods (%d):' % len(cls.methods))
            if cls.methods:
                for m in cls.methods:
                    w(_method_line(m, resolver))
            else:
                w('  - (none)')
            w('')

    # --- Parse failures ---------------------------------------------------
    if failures:
        w('## Parse failures')
        w('')
        for name, err in failures:
            w('  - %s: %s' % (name, err))
        w('')

    text = '\n'.join(out) + '\n'
    with open(OUT_PATH, 'w', encoding='utf-8') as fh:
        fh.write(text)

    print('Wrote %s' % OUT_PATH)
    print('Classes mapped: %d across %d packages.' % (len(classes), len(by_pkg)))
    print('Subsystems (packages):')
    for pkg in ordered_pkgs:
        print('  - %s (%d classes)' % (pkg, len(by_pkg[pkg])))
    if failures:
        print('Parse failures: %d' % len(failures))
        for name, err in failures:
            print('  - %s: %s' % (name, err))
    else:
        print('Parse failures: 0')


if __name__ == '__main__':
    main()
