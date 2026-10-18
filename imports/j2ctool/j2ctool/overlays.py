"""Per-project code overlays -- re-apply hand-fixes on top of freshly generated output.

WHY THIS EXISTS
    Most j2ctool fixes belong in the emitter (a general transform), the runtime headers, or the platform
    reference layer (docs/platform/reference/*) -- those are GENERAL and benefit every conversion.
    But a real port also accumulates fixes that are GAME/VERSION-SPECIFIC edits to GENERATED files:
    a leak `delete` in one method, a value-comparing std::set swapped in for a broken JSet, a UI tweak,
    an input remap. These don't generalize into an emitter rule, so a reconversion would silently WIPE
    them. That is the failure this module prevents.

    A file placed under overlays/<project_key>/<relpath> is copied over out_dir/<relpath> AFTER emission
    (and after reference emission), so it is the final say. It may REPLACE a generated file or ADD a new
    one (e.g. a hand-written Socket.cpp); because the vcxproj step runs AFTER this and enumerates out_dir,
    added .cpp files get compiled automatically.

    Precedence (last wins): runtime headers -> emitted classes -> platform references -> OVERLAYS.

    This is the catch-all, not an excuse: when an overlaid fix turns out to be a general pattern, migrate
    it into the emitter/runtime/reference and delete the overlay. MANIFEST.md in each project dir records
    what each overlay is and whether it's slated for migration.
"""
import os
import shutil


def project_key(out_dir, java_dir=None):
    """Stable per-project overlay key. Uses the output project's basename (e.g. 'MinecraftBeta')."""
    base = os.path.basename(os.path.normpath(out_dir or '')) or ''
    return base


def overlays_root():
    """<converter_repo>/overlays -- sibling of the j2ctool package."""
    return os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'overlays')


def resolve_overlay_dir(out_dir, java_dir=None, explicit=None):
    """Pick the overlay dir: explicit arg wins; else <converter>/overlays/<project_key>."""
    if explicit:
        return explicit
    key = project_key(out_dir, java_dir)
    if not key:
        return None
    return os.path.join(overlays_root(), key)


def apply_overlays(out_dir, overlay_dir, prog=None):
    """Copy every file under overlay_dir over out_dir (same relative path). Returns the list of applied
    relpaths. Skips MANIFEST.md. Creates missing subdirs. No-op if overlay_dir is absent/empty."""
    prog = prog or (lambda _s: None)
    if not overlay_dir or not os.path.isdir(overlay_dir):
        return []
    applied = []
    added = 0
    for root, _dirs, files in os.walk(overlay_dir):
        for fn in files:
            if fn == 'MANIFEST.md':
                continue
            src = os.path.join(root, fn)
            rel = os.path.relpath(src, overlay_dir)
            dst = os.path.join(out_dir, rel)
            dst_parent = os.path.dirname(dst)
            if dst_parent:
                os.makedirs(dst_parent, exist_ok=True)
            existed = os.path.exists(dst)
            shutil.copyfile(src, dst)
            applied.append(rel)
            if not existed:
                added += 1
            prog('[overlay] %s %s' % ('replaced' if existed else 'ADDED   ', rel))
    if applied:
        prog('[overlay] applied %d project overlay file(s) (%d new) from %s'
             % (len(applied), added, overlay_dir))
    return applied
