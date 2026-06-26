#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
update_version.py - Version-update helper for the Java -> Xbox 360 C++ converter.

Point this at a NEWER version's Java source tree. It runs THREE phases before touching
the converter output:

  1) SYNTAX CHECK  - parse-checks every .java in the new tree with the converter's OWN
                     parser (the meaningful "can this be converted" check). Optionally also
                     runs the C++ syntax-check (syntaxcheck.ps1) on the output AFTER convert.
  2) DIFF          - compares the new tree against the BASELINE (the currently-converted
                     Java) and reports, per file:
                       ADDED   (new file not in baseline)
                       DELETED (baseline file gone in new)
                       CHANGED (content differs; with +added / -deleted line counts; full
                                unified diff with --diff)
                       UNCHANGED
  3) CONVERT       - if the syntax check passed (no parse failures), runs the converter on
                     the new tree -> output. With --promote, then rolls the baseline forward
                     to the new version so the NEXT update diffs against this one.

Typical use:
  # see what changed + convert
  python update_version.py --new ..\\NewVersionJava
  # review only, no conversion
  python update_version.py --new ..\\NewVersionJava --dry-run --diff
  # convert and make it the new baseline, then C++ compile-check the output
  python update_version.py --new ..\\NewVersionJava --promote --cppcheck

Defaults: --baseline original_java, --out output, --version indev.
"""

import os
import re
import sys
import hashlib
import difflib
import argparse
import subprocess

HERE = os.path.dirname(os.path.abspath(__file__))
CONVERTER = os.path.join(HERE, 'java2xbox360.py')

# Reuse the converter's parser for the syntax check (import is safe: it has an
# `if __name__ == '__main__'` guard, so importing does NOT run a conversion).
sys.path.insert(0, HERE)
import java2xbox360 as conv   # noqa: E402


# ---------------------------------------------------------------------------
# helpers
# ---------------------------------------------------------------------------
def list_java(root):
    """relpath(posix) -> abspath for every *.java under root (recursive)."""
    out = {}
    if not root or not os.path.isdir(root):
        return out
    for dirpath, _dirs, files in os.walk(root):
        for fn in files:
            if fn.lower().endswith('.java'):
                ap = os.path.join(dirpath, fn)
                rel = os.path.relpath(ap, root).replace('\\', '/')
                out[rel] = ap
    return out


def sha1_of(path):
    h = hashlib.sha1()
    with open(path, 'rb') as f:
        for chunk in iter(lambda: f.read(65536), b''):
            h.update(chunk)
    return h.hexdigest()


def read_text(path):
    with open(path, 'r', encoding='utf-8', errors='replace') as f:
        return f.read()


def syntax_check(files):
    """Parse-check each .java with the converter parser. Returns (ok_list, fail_list)."""
    ok, fail = [], []
    for rel, ap in sorted(files.items()):
        try:
            cls = conv.parse_java_file(ap)
            if cls is not None and getattr(cls, 'name', None):
                ok.append(rel)
                continue
            # The parser only models class/interface. A file with no parsed class is still fine if it
            # is an enum or annotation type (the converter handles those via a different path) -- only
            # FAIL when there's no type declaration at all (truly malformed / unsupported).
            if re.search(r'\b(enum|@interface)\s+\w+', read_text(ap)):
                ok.append(rel)
            else:
                fail.append((rel, 'no class/interface/enum declaration found'))
        except Exception as e:                       # noqa: BLE001 - report any parser failure
            fail.append((rel, '%s: %s' % (type(e).__name__, e)))
    return ok, fail


def line_delta(base_path, new_path):
    """Return (added_lines, deleted_lines) between two text files."""
    a = read_text(base_path).splitlines()
    b = read_text(new_path).splitlines()
    sm = difflib.SequenceMatcher(a=a, b=b, autojunk=False)
    adds = dels = 0
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag in ('replace', 'delete'):
            dels += (i2 - i1)
        if tag in ('replace', 'insert'):
            adds += (j2 - j1)
    return adds, dels


def method_diff(base_path, new_path):
    """Method-level diff between two versions of a .java (via the converter's parser).
    Returns (added, removed, changed) lists of 'name(types)' signatures. `changed` = same signature,
    different body. Far more actionable than +/- line counts for deciding what to re-port."""
    try:
        bc = conv.parse_java_file(base_path)
        nc = conv.parse_java_file(new_path)
    except Exception:
        return [], [], []

    def sigs(cls):
        d = {}
        for m in (getattr(cls, 'methods', None) or []):
            key = '%s(%s)' % (m.name, ','.join(pt for pt, _pn in m.params))
            d[key] = '\n'.join(getattr(m, 'body_lines', []) or [])
        return d

    b, n = sigs(bc), sigs(nc)
    added = sorted(set(n) - set(b))
    removed = sorted(set(b) - set(n))
    changed = sorted(k for k in (set(b) & set(n)) if b[k] != n[k])
    return added, removed, changed


def hand_fix_report(changed_rels, live_dir, handwritten_dir):
    """Clobber guard. For each CHANGED .java class, check whether reconverting it would lose hand-work:
      'override' - a handwritten/<Class>.{cpp,h} exists -> SAFE (the override wins after convert).
      'at-risk'  - no override AND the live C++ (live_dir/<Class>.cpp) has N `// PORT` hand-fixes that a
                   fresh conversion will NOT reproduce -> they'd be lost if you sync output over live.
    Returns sorted list of (class, port_marker_count, status)."""
    overrides = set()
    if os.path.isdir(handwritten_dir):
        overrides = set(os.path.splitext(f)[0] for f in os.listdir(handwritten_dir)
                        if f.endswith('.cpp') or f.endswith('.h'))
    out = []
    for rel in changed_rels:
        cls = os.path.splitext(os.path.basename(rel))[0]
        if cls in overrides:
            out.append((cls, 0, 'override'))
            continue
        live_cpp = os.path.join(live_dir, cls + '.cpp')
        if os.path.isfile(live_cpp):
            n = len(re.findall(r'//\s*PORT', read_text(live_cpp)))
            if n > 0:
                out.append((cls, n, 'at-risk'))
    return sorted(out, key=lambda t: (t[2] != 'at-risk', -t[1], t[0]))


def _classify_class(name, path):
    """Classify one NEW Java class for a scaffolded profile, mirroring indev's conventions.
    Returns one of: 'interface' | 'enum' | 'skip' | 'platform' | 'reference'."""
    try:
        txt = read_text(path)
    except Exception:                                 # noqa: BLE001
        txt = ''
    head = txt[:4000]
    if re.search(r'\b(public\s+)?interface\s+%s\b' % re.escape(name), txt):
        return 'interface'
    if re.search(r'\b(public\s+)?enum\s+%s\b' % re.escape(name), txt):
        # OS-detection enums are desktop-only; real game enums are left for the converter default.
        return 'skip' if name.startswith('EnumOS') else 'enum'
    awt = bool(re.search(r'import\s+(java\.awt|javax\.|java\.applet)', head))
    net = (name.startswith('Packet') or
           re.match(r'^(Net|Network|Tcp|ThreadConnect|ThreadRun)', name) or
           name in ('NetClientHandler', 'NetHandler', 'NetworkManager', 'MemoryConnection') or
           bool(re.search(r'import\s+java\.net', head)) or 'java.net.Socket' in head)
    thread = 'extends Thread' in head
    if awt or net or thread:
        return 'skip'
    gl = ('org.lwjgl' in head) or ('GL11.' in txt) or ('GL12.' in txt) or ('GLU.' in txt)
    render_name = bool(re.match(r'^(Render|Model|Gui|Tessellator|FontRenderer|ItemRenderer|'
                                r'EffectRenderer|EntityRenderer|TexturedQuad|PositionTexture)', name)) \
        or name.endswith('FX')
    if gl or render_name:
        return 'platform'
    return 'reference'


def scaffold_profile(new_java_dir, base='indev', baseline_java_dir=None):
    """Auto-generate a custom-profile dict by diffing a NEW Java tree against the baseline (the classes
    the `base` profile already covers) and classifying only the NEW classes the same way indev's profile
    is organised: AWT/networking/threads/OS-enums -> skip_files; interfaces -> interface_types; GL/render
    classes -> platform_file_classes (+reference_types); everything else gameplay -> pure_logic_files
    (+reference_types). New classes default to nullable POINTER (reference) types -- the safe choice in
    this codebase (value-type promotion is a manual optimisation). Returns the JSON-able profile dict."""
    baseline_java_dir = baseline_java_dir or os.path.join(HERE, 'original_java')
    new_classes = {}
    for rel, ap in list_java(new_java_dir).items():
        new_classes[os.path.splitext(os.path.basename(rel))[0]] = ap
    base_classes = set(os.path.splitext(os.path.basename(rel))[0]
                       for rel in list_java(baseline_java_dir))
    added = sorted(set(new_classes) - base_classes)

    skip, ifaces, platform, pure, enums = [], [], [], [], []
    for c in added:
        cat = _classify_class(c, new_classes[c])
        if cat == 'interface':
            ifaces.append(c)
        elif cat == 'enum':
            enums.append(c)
        elif cat == 'skip':
            skip.append(c + '.java')          # skip_files holds FILE names
        elif cat == 'platform':
            platform.append(c)
        else:
            pure.append(c)
    reference = sorted(platform + pure)        # both become C++ pointer types, like indev

    return {
        'base': base,
        '_help': ("Scaffolded starter profile. New classes default to nullable POINTER (reference_types); "
                  "AWT/networking/threads were skipped; GL/render classes are platform_file_classes. "
                  "Refine by hand: promote tiny data holders to value_types, un-skip anything you need, "
                  "and fill static_class_fields / parent_fields / extra_type_map. See profiles/README.txt."),
        '_generated_from': os.path.abspath(new_java_dir),
        '_new_classes': len(added),
        '_enums_left_to_converter_default': sorted(enums),
        'skip_files': sorted(skip),
        'interface_types': sorted(ifaces),
        'platform_file_classes': sorted(platform),
        'pure_logic_files': sorted(pure),
        'reference_types': reference,
    }


def write_profile(name, data, profiles_dir=None):
    """Write a profile dict to profiles/<name>.json (next to the converter / exe). Returns the path."""
    import json
    profiles_dir = profiles_dir or conv.profiles_dir()
    os.makedirs(profiles_dir, exist_ok=True)
    path = os.path.join(profiles_dir, name.lower() + '.json')
    with open(path, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=2)
    return path


def promote_baseline(new_files, baseline_dir):
    """Roll the baseline forward: copy every new .java into the baseline at the same relpath,
    and delete baseline .java that no longer exist in the new tree. Non-.java files untouched."""
    base_files = list_java(baseline_dir)
    copied = removed = 0
    for rel, ap in new_files.items():
        dst = os.path.join(baseline_dir, rel)
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        with open(ap, 'rb') as s, open(dst, 'wb') as d:
            d.write(s.read())
        copied += 1
    for rel, ap in base_files.items():
        if rel not in new_files:
            try:
                os.remove(ap)
                removed += 1
            except OSError:
                pass
    return copied, removed


# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------
def main():
    p = argparse.ArgumentParser(
        description='Diff + syntax-check + convert a newer Java version against the current baseline.')
    p.add_argument('--new', required=True, help='Path to the NEW version Java source tree')
    p.add_argument('--baseline', default=os.path.join(HERE, 'original_java'),
                   help='Currently-converted Java tree (default: original_java)')
    p.add_argument('--out', default=os.path.join(HERE, 'output'),
                   help='Converter output dir (default: output)')
    p.add_argument('--version', default='indev', help='Converter version profile (default: indev)')
    p.add_argument('--dry-run', action='store_true', help='Report only; do NOT convert')
    p.add_argument('--diff', action='store_true', help='Print full unified diffs for changed files')
    p.add_argument('--promote', action='store_true',
                   help='After a successful convert, roll the baseline forward to the new version')
    p.add_argument('--force', action='store_true',
                   help='Convert even if the syntax check found parse failures')
    p.add_argument('--cppcheck', action='store_true',
                   help='After convert, run syntaxcheck.ps1 (C++ compile-check of the output)')
    p.add_argument('--scaffold', metavar='NAME',
                   help='Auto-generate profiles/<NAME>.json from --new (vs --base profile) and exit')
    p.add_argument('--base', default='indev',
                   help='Base profile a scaffolded profile inherits from (default: indev)')
    args = p.parse_args()

    if not os.path.isdir(args.new):
        print('ERROR: --new directory not found: %s' % args.new)
        sys.exit(1)

    # ---- profile scaffolding (short-circuit) -------------------------------
    if args.scaffold:
        data = scaffold_profile(args.new, base=args.base, baseline_java_dir=args.baseline)
        path = write_profile(args.scaffold, data)
        print('Scaffolded profile "%s" (base=%s): %d new classes' % (args.scaffold, args.base, data['_new_classes']))
        print('  skip=%d  interface=%d  platform=%d  pure_logic=%d  reference=%d  enums(left default)=%d'
              % (len(data['skip_files']), len(data['interface_types']), len(data['platform_file_classes']),
                 len(data['pure_logic_files']), len(data['reference_types']),
                 len(data['_enums_left_to_converter_default'])))
        print('  -> %s' % path)
        print('  Refine it by hand, then use it via --version %s or the GUI profile combo.' % args.scaffold)
        return

    # Activate the version profile so the parser sees the right type model.
    try:
        conv.set_active_profile(args.version)
        conv.init_profile_globals()
    except Exception as e:                            # noqa: BLE001
        print('WARN: profile init failed (%s); parse check may be degraded.' % e)

    new_files = list_java(args.new)
    base_files = list_java(args.baseline)

    print('=' * 72)
    print('VERSION UPDATE')
    print('  new:      %s' % os.path.abspath(args.new))
    print('  baseline: %s' % os.path.abspath(args.baseline))
    print('  output:   %s' % os.path.abspath(args.out))
    print('  profile:  %s' % args.version)
    print('=' * 72)
    print('new tree:      %d .java files' % len(new_files))
    print('baseline tree: %d .java files' % len(base_files))
    print()

    # ---- 1) SYNTAX CHECK ----------------------------------------------------
    print('--- 1) SYNTAX CHECK (converter parser) ---')
    ok, fail = syntax_check(new_files)
    print('  parsed OK : %d' % len(ok))
    print('  FAILED    : %d' % len(fail))
    for rel, why in fail:
        print('    ! %s  (%s)' % (rel, why))
    print()

    # ---- 2) DIFF vs baseline ------------------------------------------------
    print('--- 2) DIFF vs baseline ---')
    ns, bs = set(new_files), set(base_files)
    added = sorted(ns - bs)
    deleted = sorted(bs - ns)
    common = sorted(ns & bs)
    changed = []
    for rel in common:
        if sha1_of(new_files[rel]) != sha1_of(base_files[rel]):
            a, d = line_delta(base_files[rel], new_files[rel])
            changed.append((rel, a, d))

    print('  ADDED     : %d' % len(added))
    for rel in added:
        print('    + %s' % rel)
    print('  DELETED   : %d' % len(deleted))
    for rel in deleted:
        print('    - %s' % rel)
    print('  CHANGED   : %d' % len(changed))
    for rel, a, d in changed:
        print('    ~ %s  (+%d / -%d lines)' % (rel, a, d))
    print('  UNCHANGED : %d' % (len(common) - len(changed)))
    print()

    if args.diff and changed:
        print('--- unified diffs (changed files) ---')
        for rel, _a, _d in changed:
            a = read_text(base_files[rel]).splitlines(keepends=True)
            b = read_text(new_files[rel]).splitlines(keepends=True)
            sys.stdout.writelines(difflib.unified_diff(a, b, 'baseline/' + rel, 'new/' + rel))
            print()

    # ---- 3) CONVERT ---------------------------------------------------------
    if args.dry_run:
        print('[dry-run] checks complete; NOT converting.')
        return

    if fail and not args.force:
        print('ABORT: %d file(s) failed the syntax check.' % len(fail))
        print('       Fix them, or re-run with --force to convert anyway.')
        sys.exit(2)

    print('--- 3) CONVERT new version -> %s ---' % os.path.abspath(args.out))
    os.makedirs(args.out, exist_ok=True)
    cmd = [sys.executable, CONVERTER, args.new, args.out, '--version', args.version]
    print('  $ %s' % ' '.join(cmd))
    rc = subprocess.call(cmd)
    if rc != 0:
        print('CONVERTER FAILED (exit %d)' % rc)
        sys.exit(rc)
    print('  convert OK')
    print()

    # ---- optional C++ compile-check of the output ---------------------------
    if args.cppcheck:
        sc = os.path.join(HERE, 'syntaxcheck.ps1')
        if os.path.isfile(sc):
            print('--- C++ syntax check (syntaxcheck.ps1) ---')
            subprocess.call(['powershell', '-NoProfile', '-File', sc])
        else:
            print('WARN: --cppcheck requested but syntaxcheck.ps1 not found at %s' % sc)
        print()

    # ---- promote baseline ---------------------------------------------------
    if args.promote:
        copied, removed = promote_baseline(new_files, args.baseline)
        print('Promoted baseline: copied %d, removed %d -> %s' % (copied, removed, args.baseline))
    else:
        print('Tip: re-run with --promote to roll the baseline forward to this version')
        print('     (so the NEXT update diffs against what you just converted).')


if __name__ == '__main__':
    main()
