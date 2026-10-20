"""run.py -- the Java transpiler's tools: a Java program as C++03, and what its console port needs.

    python -m codebreaker java <tool> [arguments]      (`java <tool> --help` for a tool's options)

  Transpile
    convert JAVA OUT [--platform P] [--edition E]   Java source -> C++ (also --mc-version ID to
                                    decompile a Minecraft version first, --project for a buildable tree)
    decompile VERSION WORKSPACE     a Minecraft version as Java source (RetroMCP / Vineflower;
                                    settings.json: java, retromcp)
    scaffold JAVA OUT               structural skeletons in C, C# or Rust (not a translation)
    signatures JAVA                 the codebase map: class -> fields and methods, Java -> C++ types
  Judge and report     (classes judged class by class: `codebreaker module JAVA_FOLDER`)
    report OUT                      crash-class report of an emitted tree (and an MSVC build log)
    worklist OUT                    compile every .cpp with the host compiler, worst last
    bugfix-ledger                   every FIXED BUG / J2X-BUGFIX marker -> state/java/BUGFIX_LEDGER.md
    apimap ...                      the API-mapping registry: coverage, lookups
  Console port (Xbox 360)
    sdk                             is the console SDK there (settings.json: xbox360_sdk)?
    platform-config OUT             controller, storage and keyboard choices -> platform.config.json
    assets JAR OUT                  textures and resources from the game's jar, PNGs normalised
    xact ...                        XACT3 audio banks: dump, build, coverage, match
    achievements JAVA               achievements -> the console's achievement data + unlock calls
    project-layout ROOT             the Visual Studio solution layout over an emitted tree
    vcxproj [--proj DIR]            the project's file list and filters, regenerated
    build SLN                       build the solution with MSBuild (settings.json: msbuild)
    debug OUT                       the one switchable Debug.h/.cpp
  Rewrites of an emitted tree (each runs by itself in an xbox360 convert)
    reflect-factories, sound-fix, controller-nav, render-fix, applet-ping, ui-mark, keybind-mark
    scan-project FOLDER             a decompiled game's jar, sound folder, source and language file
"""

import argparse
import os
import sys

from codebreaker.java.ledger import Ledger, Severity
from codebreaker.java import analyze, report



def _cmd_xact(args):
    from codebreaker.java.console import xact
    act = args.action
    if act == 'dump':
        if not os.path.isfile(args.a):
            print('ERROR: not a file: %s' % args.a); return 2
        print(xact.dump(args.a)); return 0
    if act == 'build':
        run = getattr(args, 'run', False)
        sdk_root = None
        if run:
            from codebreaker.java.console import sdk as _sdk
            _info = _sdk.detect('xbox360')
            sdk_root = getattr(_info, 'root', None) if _info else None
        xap, cues = xact.build(args.a, args.b, autorun=run, sdk_root=sdk_root, log=print)
        print('Wrote %s (%d cues) + build_banks.bat + convert_audio.bat to %s' % (xap, len(cues), args.b))
        if not run:
            print('Next: run convert_audio.bat (ffmpeg ogg->wav) then build_banks.bat (XDK XACTBLD) '
                  '-- or pass --run to autorun both now.')
        return 0
    if act == 'coverage':
        cov = xact.cue_coverage(args.a, args.b)
        print('needed=%d  authored=%d  MISSING=%d  unused=%d'
              % (len(cov['needed']), len(cov['authored']), len(cov['missing']), len(cov['unused'])))
        for c in cov['missing']:
            print('  MISSING (silent): %s  <- %s' % (c, ', '.join(cov['needed'][c][:3])))
        return 0
    if act == 'match':
        # a = java_dir, b = resources_dir; optional out dir via args.c
        out = args.c or '.'
        m = xact.match_sources(args.a, args.b)
        bat = xact.write_ffmpeg_script(m, out, args.b)
        print('matched=%d  MISSING=%d  (index keys=%d)'
              % (len(m['matched']), len(m['missing']), m['index_keys']))
        for cue in sorted(m['missing']):
            print('  no source ogg: %s  <- %s' % (cue, ', '.join(m['missing'][cue][:3])))
        print('Wrote %s (ffmpeg .ogg->.wav per matched cue).' % bat)
        return 0
    print('ERROR: unknown xact action %r (dump/build/coverage/match)' % act); return 2


def _cmd_achievements(args):
    from codebreaker.java.projects.minecraft import achievements
    if not os.path.isdir(args.code_dir):
        print('ERROR: not a directory: %s' % args.code_dir); return 2
    res = achievements.run(args.code_dir, args.out_dir, lang_file=args.lang,
                           profile=args.profile, delivery=args.delivery,
                           title_id=args.title_id, title_name=args.title_name,
                           content_dir=args.content_dir)
    m = res['mapping']
    print('Found %d achievement(s) -> %s profile, %d gamerscore (limit %d)'
          % (res['count'], m['profile'], m['total_score'], m['limit']))
    for w in m['warnings']:
        print('  WARNING: %s' % w)
    ic = res['icons']
    print('  icons: %d auto-wired, %d need manual art%s'
          % (len(ic['wired']), len(ic['unresolved']),
             '   (atlases: %s)' % res['content_dir'] if res['content_dir'] else ''))
    for key, why in ic['unresolved'][:6]:                 # WHY, not just a count -- a silent 0 hid this
        print('    - %s: %s' % (key, why))
    if len(ic['unresolved']) > 6:
        print('    - ... and %d more (see ACHIEVEMENTS.md)' % (len(ic['unresolved']) - 6))
    print('  title id: 0x%s%s' % (res['title_id'], ' (suggested — verify no collision)'
                                   if res['title_suggested'] else ''))
    print('  report: %s' % res['report'])
    print('  xlast:  %s   code: %s' % (res['xlast'], ', '.join(os.path.basename(p) for p in res['code'])))
    return 0


def _cmd_ui_mark(args):
    from codebreaker.java.projects.minecraft import ui_mark
    if not os.path.isdir(args.dir):
        print('ERROR: not a directory: %s' % args.dir); return 2
    extra = [c.strip() for c in (args.extra or '').split(',') if c.strip()]
    res = ui_mark.run(args.dir, dry_run=args.dry_run, extra_classes=extra)
    total = sum(len(v) for v in res['marked'].values())
    print('UI classes: %d   methods %s: %d   (already-marked skipped: %d)'
          % (len(res['classes']), 'that would be marked' if args.dry_run else 'marked',
             total, res['skipped']))
    if args.dry_run:
        for cls in sorted(res['marked'])[:20]:
            print('  %s (%d)' % (cls, len(res['marked'][cls])))
    else:
        print('  index: %s' % res['index'])
    return 0


def _cmd_keybind_mark(args):
    from codebreaker.java.projects.minecraft import keybind_mark
    if not os.path.isdir(args.dir):
        print('ERROR: not a directory: %s' % args.dir); return 2
    res = keybind_mark.run(args.dir, dry_run=args.dry_run)
    print('keyboard key-press sites %s: %d   (already-marked skipped: %d)'
          % ('that would be tagged' if args.dry_run else 'tagged', len(res['sites']), res['skipped']))
    if not args.dry_run:
        print('  index: %s' % res['index'])
    return 0


def _cmd_bugfix_ledger(args):
    from codebreaker.java import bugfix_ledger
    from codebreaker.base import settings
    entries = bugfix_ledger.run(args.repo, out_path=args.out)
    out = args.out or settings.state('java', 'BUGFIX_LEDGER.md')
    print('bugfix-ledger: %d markers / %d distinct fixes -> %s'
          % (len(entries), len(set(e['slug'] for e in entries)), out))
    return 0


def _cmd_debug(args):
    from codebreaker.java import debug
    os.makedirs(args.out_dir, exist_ok=True)
    if args.off:
        paths = debug.write_debug(args.out_dir, enabled_default=False)
        print('Emitted debug files with DEBUG_ENABLED=0 (opt-in off): %s'
              % ', '.join(os.path.basename(p) for p in paths))
    else:
        paths = debug.write_debug(args.out_dir, enabled_default=True)
        print('Emitted %s (master toggle DEBUG_ENABLED=1; J2X-DEBUG markers).'
              % ', '.join(os.path.basename(p) for p in paths))
    markers = debug.scan_debug_markers(args.out_dir)
    print('  J2X-DEBUG marker tags present: %d (%s)'
          % (len(markers), ', '.join(sorted(markers))[:100]))
    return 0


def _cmd_reflect_factories(args):
    from codebreaker.java import reflection_factory
    r = reflection_factory.run(args.out_dir)
    if not r['total_sites']:
        print('No reflection sites to rewrite (already applied, or none present).')
        return 0
    for reg in r['registries']:
        print('  %-22s -> %s_createByReflection  (%d classes, %d site%s)'
              % (reg['file'], reg['registry'], len(reg['classes']),
                 reg['sites'], '' if reg['sites'] == 1 else 's'))
    for ru in r['reused']:
        print('  %-22s -> reuses a shared factory  (%d site%s)'
              % (ru['file'], ru['sites'], '' if ru['sites'] == 1 else 's'))
    print('Generated ReflectionFactory.h; %d sites rewritten across %d registries (%d classes).'
          % (r['total_sites'], len(r['registries']), r['total_classes']))
    return 0


def _cmd_sound_fix(args):
    from codebreaker.java.projects.minecraft import sound_fix
    r = sound_fix.run(args.out_dir)
    if not r['sites']:
        print('No sound guard sites to rewrite (already applied, or none present).')
    for fi in r['files']:
        print('  %-22s -> %d SFX + %d music site(s) routed to the XACT bridge'
              % (fi['file'], fi['sfx'], fi['music']))
    if r['sites']:
        print('Sound bypass applied: %d site(s) across %d file(s).' % (r['sites'], len(r['files'])))
    if r['stranded']:
        print('WARNING: soundPoolSounds null-guard survived in: %s (guard shape changed; update sound_fix.py).'
              % ', '.join(r['stranded']))
        return 1
    return 0


def _cmd_controller_nav(args):
    from codebreaker.java.projects.minecraft import controller_nav
    r = controller_nav.run(args.out_dir)
    print('controller menu nav: GuiScreen.h=%s GuiScreen.cpp=%s call-site=%s'
          % (r['guiscreen_h'], r['guiscreen_cpp'], r['callsite']))
    if r['missing']:
        print('WARNING: anchor(s) not found -> menus may be un-navigable: %s' % '; '.join(r['missing']))
        return 1
    return 0


def _cmd_render_fix(args):
    from codebreaker.java.projects.minecraft import render_fix
    r = render_fix.run(args.out_dir)
    print('render-fix: updateRenderers budget=%d site(s), hurt-camera NaN guard=%d site(s)'
          % (r['update'], r['hurt']))
    if r['stranded']:
        print('WARNING: a render-fix trigger survived in: %s (shape changed; update render_fix.py). The '
              'first in-game frame may hang on "Spawning" or render invisible.' % '; '.join(r['stranded']))
        return 1
    return 0


def _cmd_applet_ping(args):
    from codebreaker.java.projects.minecraft import applet_ping
    r = applet_ping.run(args.out_dir)
    if not r['sites']:
        print('No applet ping sites to rewrite (already applied, or none present).')
    for fi in r['files']:
        print('  %-22s -> %d applet ping site(s) neutralized' % (fi['file'], fi['sites']))
    if r['sites']:
        print('Applet ping removed: %d site(s) across %d file(s).' % (r['sites'], len(r['files'])))
    if r['stranded']:
        print('WARNING: a getDocumentBase()->openStream() try survived in: %s (shape changed; update '
              'applet_ping.py).' % ', '.join(r['stranded']))
        return 1
    return 0


def _cmd_scaffold(args):
    from codebreaker.java import scaffold
    r = scaffold.run(args.java_dir, args.out_dir, args.language)
    print('scaffold: %d %s skeleton file(s) for %d class(es) -> %s'
          % (len(r['written']), scaffold.LANG_LABEL[args.language], r['classes'], args.out_dir))
    return 0


def _cmd_scan_project(args):
    from codebreaker.java.console import project_scan
    r = project_scan.resolve(args.folder)
    for k in sorted(r):
        print('  %-10s %s' % (k, r[k] or '(not found)'))
    return 0


def _cmd_build(args):
    """MSBuild over a solution -- what the transpiler's old window did with its Build button."""
    import subprocess
    from codebreaker.base import settings
    msbuild = settings.get('msbuild')
    if not msbuild or not os.path.isfile(msbuild):
        print('MSBuild is not set up (settings.json: msbuild)')
        return 2
    log = args.log or settings.state('java', 'build.log')
    cmd = [msbuild, args.sln, '/m', '/nologo', '/p:Configuration=%s' % args.config,
           '/p:Platform=%s' % args.platform, '/flp:LogFile=%s;Verbosity=normal' % log]
    print(' '.join('"%s"' % c if ' ' in c else c for c in cmd))
    code = subprocess.call(cmd)
    print('build %s -- log: %s' % ('OK' if code == 0 else 'FAILED (exit %d)' % code, log))
    print('crash-class report with the build log: codebreaker java report OUT --build-log %s' % log)
    return code


def _cmd_project_layout(args):
    from codebreaker.java.console import project_layout
    r = project_layout.layout(args.root, name=args.name)
    if r['moved']:
        print('Moved %d flat entr%s down into %s\\'
              % (r['moved'], 'y' if r['moved'] == 1 else 'ies', r['name']))
    print('Solution layout ready:')
    print('  %s' % r['sln'])
    print('  %s\\  (%s.vcxproj)' % (r['proj_dir'], r['name']))
    return 0


def _cmd_platform_config(args):
    from codebreaker.java.console import platform_config, sdk
    os.makedirs(args.out_dir, exist_ok=True)
    updates = {}
    if args.layout:            updates['layout'] = args.layout
    if args.invert_y:          updates['invert_y'] = True
    if args.no_invert_y:       updates['invert_y'] = False
    if args.sensitivity is not None: updates['look_sensitivity'] = args.sensitivity
    if args.keyboard:          updates['keyboard'] = args.keyboard
    if args.storage:           updates['storage'] = args.storage
    for b in (args.bind or []):
        if '=' in b:
            act, ctrl = b.split('=', 1)
            updates['bind.%s' % act.strip()] = ctrl.strip()
    if args.list:
        cfg, _w, _i, _u = platform_config.run(args.out_dir, updates=updates, list_only=True)
        print(platform_config._choices_md(cfg))
        return 0
    try:
        cfg, written, info, unverified = platform_config.run(
            args.out_dir, updates=updates, platform=args.platform, sdk_override=args.sdk)
    except sdk.SdkNotFound as e:
        print('ERROR (SDK gate): %s' % e)
        return 3
    print('Wrote %d file(s) to %s' % (len(written), os.path.abspath(args.out_dir)))
    print('  controller layout=%s invertY=%s | keyboard=%s | storage=%s'
          % (cfg['controller']['layout'], cfg['controller']['invert_y'],
             cfg['keyboard'], cfg['storage']))
    if info and info.root:
        print('  SDK: %s @ %s (via %s)' % (info.name, info.root, info.source))
    if unverified:
        print('  WARNING: symbols not found in the located SDK: %s' % ', '.join(unverified))
    print('  see %s' % os.path.join(args.out_dir, 'PLATFORM_CHOICES.md'))
    return 0


def _cmd_sdk(args):
    from codebreaker.java.console import sdk
    print(sdk.describe(args.platform, override=args.sdk))
    info = sdk.detect(args.platform, override=args.sdk)
    if info and info.include_dirs:
        print('  include dirs: %s' % ', '.join(info.include_dirs))
    return 0 if info is not None else 1


def _cmd_assets(args):
    from codebreaker.java.console import assets
    if not os.path.isfile(args.jar):
        print('ERROR: not a file: %s' % args.jar); return 2
    res = assets.run(args.jar, args.content_dir, code_dir=args.code, only_needed=args.only_needed,
                     normalize_png=not args.no_rgba)
    print('Extracted %d asset(s) -> %s' % (len(res['written']), os.path.abspath(args.content_dir)))
    if res.get('console'):
        print('  + %d console-only asset(s) from the converter bundle: %s'
              % (len(res['console']), ', '.join(res['console'])))
    if res['converted']:
        print('  normalized %d non-RGBA PNG(s) to RGBA (360 render fix): %s'
              % (len(res['converted']), ', '.join(res['converted'][:6])
                 + (' ...' if len(res['converted']) > 6 else '')))
    if res['unconverted']:
        print('  WARNING: %d non-RGBA PNG(s) could NOT be converted (Pillow missing) -> will misrender'
              % len(res['unconverted']))
    cov = res['coverage']
    if cov is not None:
        print('  coverage: code needs %d, jar provides %d, present %d, MISSING %d, jar extras %d'
              % (len(cov['needed']), cov['jar_count'], len(cov['present']),
                 len(cov['missing']), len(cov['extra'])))
        for p in cov['missing']:
            print('    MISSING (runtime NULL image): %s' % p)
        if res['report']:
            print('  report: %s' % res['report'])
    return 0


def _cmd_convert(args):
    from codebreaker.java import transpile as pipeline
    from codebreaker.java.console import sdk
    java_dir, out_dir = args.java_dir, args.out_dir
    if getattr(args, 'mc_version', None):
        # `convert --mc-version <id> <out_dir>`: the single positional is the output dir.
        if out_dir is None:
            out_dir = java_dir
        if not out_dir:
            print('ERROR: output directory required, e.g. `convert --mc-version a1.2.6 out/`')
            return 2
        from codebreaker.decompile import jar as decompile
        ws = args.mc_workspace or os.path.join(out_dir, '_decompiled')
        print('Decompiling Minecraft %s via the bundled RetroMCP/Vineflower backend...' % args.mc_version)
        try:
            java_dir = decompile.decompile(args.mc_version, ws, java=getattr(args, 'java', None),
                                           on_output=lambda ln: print('  ' + ln))
        except decompile.DecompileError as e:
            print('ERROR (decompile): %s' % e)
            return 4
    if not java_dir or not out_dir:
        print('ERROR: usage: convert <java_dir> <out_dir>   (or: convert --mc-version <id> <out_dir>)')
        return 2
    if not os.path.isdir(java_dir):
        print('ERROR: not a directory: %s' % java_dir)
        return 2
    project = getattr(args, 'project', False)
    try:
        s = pipeline.convert(java_dir, out_dir,
                             cpp_std=getattr(args, 'cpp_std', 'C++03'),
                             platform=getattr(args, 'platform', None),
                             platform_refs=getattr(args, 'platform_refs', False) or project,
                             sdk_override=getattr(args, 'sdk', None),
                             edition=getattr(args, 'edition', None),
                             mc_version=getattr(args, 'mc_version', None))
    except sdk.SdkNotFound as e:
        print('ERROR (SDK gate): %s' % e)
        return 3
    print('Converted %s -> %s' % (os.path.abspath(java_dir), os.path.abspath(out_dir)))
    if project:
        # One-step buildable project: preserve/re-emit config, then (re)generate the VS project. Each
        # step is best-effort -- a missing SDK is reported, never fatal to the conversion.
        from codebreaker.java.console import platform_config, vcxproj, project_layout
        try:
            _cfg, written, _info, _unv = platform_config.run(
                out_dir, platform=(getattr(args, 'platform', None) or 'xbox360'),
                sdk_override=getattr(args, 'sdk', None))
            print('  project: config emitted (%d file(s), controller binds preserved)' % len(written))
        except sdk.SdkNotFound as e:
            print('  project: platform-config skipped (SDK gate): %s' % e)
        except Exception as e:                       # noqa: BLE001 -- config must not abort the reconvert
            print('  project: platform-config FAILED: %s' % e)
        # If out_dir already holds a .vcxproj, this is a RECONVERT -> just refresh its file list. If not
        # (a brand-new edition/project), BOOTSTRAP the full solution layout (.sln + .vcxproj from the
        # template) so the first convert of any edition is buildable without a manual `project-layout`.
        proj_abs = os.path.abspath(out_dir)
        has_vcxproj = os.path.isdir(proj_abs) and any(
            f.endswith('.vcxproj') for f in os.listdir(proj_abs))
        try:
            if has_vcxproj:
                vcxproj.run(out_dir)                  # prints its own byte/file-count status
                print('  project: VS project + filters refreshed')
            else:
                # out_dir is the project subfolder (the reference layout is <root>/<Name>/<Name>.vcxproj),
                # so the solution root is its parent and the project name is its basename.
                info = project_layout.layout(os.path.dirname(proj_abs),
                                             name=os.path.basename(os.path.normpath(proj_abs)))
                print('  project: VS solution bootstrapped -> %s (+ %s)'
                      % (os.path.basename(info['sln']), os.path.basename(info['vcxproj'])))
        except Exception as e:                       # noqa: BLE001 -- project step must not abort convert
            print('  project: VS project step FAILED: %s' % e)
    print('  target: %s / %s / profile %s' % (s['platform'], s['cpp_std'], s.get('profile', '?')))
    print('  parsed=%d  headers=%d  sources=%d'
          % (s['parsed'], s['headers'], s['sources']))
    print('  verify (crash-class rule findings on output): %d' % s['rule_findings'])
    print('  ledger events=%d  (HIGH=%d)' % (s['events'], s['high']))
    if s['report']:
        print('  report: %s' % os.path.join(s['report'], 'index.md'))
    return 0


def _cmd_decompile(args):
    from codebreaker.decompile import jar as decompile
    if not decompile.find_java():
        print('ERROR: no Java runtime found. Install a JRE/JDK (8+) and put `java` on PATH or set '
              'JAVA_HOME. Only the decompile backend needs Java; the transpiler does not.')
        return 4
    try:
        src = decompile.decompile(args.version, args.workspace, java=getattr(args, 'java', None),
                                  on_output=lambda ln: print('  ' + ln))
    except decompile.DecompileError as e:
        print('ERROR: %s' % e)
        return 4
    print('Decompiled Minecraft %s -> %s' % (args.version, src))
    print('  feed it to: j2ctool convert "%s" <out_dir>' % src)
    return 0


def _cmd_report(args):
    output_dir = args.output_dir
    if not os.path.isdir(output_dir):
        print('ERROR: not a directory: %s' % output_dir)
        return 2

    root = os.getcwd()

    ledger = Ledger()
    counts = analyze.analyze_all(
        ledger, output_dir,
        build_log=args.build_log,
        root=root,
        platform=getattr(args, 'platform', None),
    )

    meta = {
        'output_dir': os.path.abspath(output_dir),
        'build_log': counts.get('_build_log_path') or '(none)',
        'profile': args.profile or '(unspecified)',
    }
    out_dir = report.write_reports(ledger, output_dir, report_dir=args.out, meta=meta)

    sevs = ledger.totals_by_severity()
    print('Analyzed %s' % os.path.abspath(output_dir))
    print('  markers=%d  rules=%d  lint=%d  build_log=%d'
          % (counts['markers'], counts['rules'], counts['lint'],
             counts['build_log']))
    print('  events: %d  (HIGH=%d MED=%d LOW=%d)'
          % (len(ledger), sevs[Severity.HIGH], sevs[Severity.MED], sevs[Severity.LOW]))
    top = [r for r in ledger.file_risk_table() if r[1] > 0][:8]
    if top:
        print('  highest-risk files:')
        for fname, score, _c, _s in top:
            print('    %4d  %s' % (score, fname))
    print('Report written to %s' % os.path.abspath(out_dir))
    print('  open %s' % os.path.join(out_dir, 'index.md'))
    return 0


def _apply_edition(args):
    """Activate the target profile for a standalone post-hoc command (signatures/vcxproj) that runs
    outside `convert`. Only an explicit --edition is honored here; without it the default profile
    stays active. Returns the previous profile name (or None) so the caller can restore it."""
    ed = getattr(args, 'edition', None)
    if not ed:
        return None
    from codebreaker.java import profiles as _profiles
    from codebreaker.java.projects.minecraft import edition as _edition   # --edition is Minecraft's
    prev = _profiles.active().name
    name = _edition.resolve(ed)
    if name and name != prev:
        _profiles.use(name)
        print('  edition: active target profile: %s' % name)
    return prev


def _cmd_signatures(args):
    from codebreaker.java import signatures, profiles as _profiles
    if not os.path.isdir(args.java_dir):
        print('ERROR: not a directory: %s' % args.java_dir)
        return 2
    _prev = _apply_edition(args)
    try:
        from codebreaker.base import settings
        signatures.run(args.java_dir, args.out or settings.state('java', 'signatures.md'))
    finally:
        if _prev is not None and _profiles.active().name != _prev:
            _profiles.use(_prev)
    return 0


def _cmd_worklist(args):
    from codebreaker.java.console import worklist
    if not os.path.isdir(args.out_dir):
        print('ERROR: not a directory: %s' % args.out_dir)
        return 2
    return worklist.run(args.out_dir, cc=args.cc, std=args.std, worklist_path=args.out)


def _cmd_vcxproj(args):
    from codebreaker.java.console import vcxproj
    from codebreaker.java import profiles as _profiles
    _prev = _apply_edition(args)
    try:
        vcxproj.run(proj_dir=args.proj)
    finally:
        if _prev is not None and _profiles.active().name != _prev:
            _profiles.use(_prev)
    return 0


def _cmd_apimap(args):
    """Query the API-mapping registry: resolve a symbol, list a subsystem, or report coverage."""
    from codebreaker.java import apimap
    reg = apimap.load()
    act = args.action
    if act == 'languages':
        for lid in sorted(reg.languages):
            info = reg.languages[lid]
            dl = info.get('dialects')
            dl = ('  dialects: ' + ', '.join(dl)) if dl else ''
            print('  %-12s [%s] %s%s' % (lid, info.get('kind', '?'), info.get('desc', ''), dl))
        return 0
    if act == 'subsystems':
        for sid in sorted(reg.subsystems):
            s = reg.subsystems[sid]
            print('  %-24s (%s)  %s' % (sid, s.get('language', ''), s.get('description', '')[:70]))
        return 0
    if act == 'list':
        if not args.a:
            print('ERROR: apimap list <subsystem>'); return 2
        rows = reg.symbols_in(args.a)
        if not rows:
            print('No symbols in subsystem %r. Try: apimap subsystems' % args.a); return 2
        for name, rec in rows:
            tgts = ', '.join('%s=%s' % (d, m.get('target', '?')) for d, m in rec['maps'].items())
            print('  %-28s -> %s' % (name, tgts))
        return 0
    if act == 'resolve':
        if not (args.a and args.src and args.dst):
            print('ERROR: apimap resolve <symbol> --from <lang> --to <lang> [--dialect D]'); return 2
        m = reg.resolve(args.a, args.src, args.dst, args.dialect)
        if not m:
            print('UNMAPPED: %s (%s -> %s)' % (args.a, args.src, args.dst)); return 1
        print('%s  (%s -> %s%s)' % (args.a, args.src, args.dst, ('@' + m.dialect if m.dialect else '')))
        print('  target : %s' % m.target)
        print('  kind   : %s' % m.kind)
        if m.note:
            print('  note   : %s' % m.note)
        return 0
    if act == 'coverage':
        if not (args.a and args.src and args.dst):
            print('ERROR: apimap coverage <java_dir> --from <lang> --to <lang> [--dialect D] [--out FILE]'); return 2
        if not os.path.isdir(args.a):
            print('ERROR: not a directory: %s' % args.a); return 2
        syms = apimap.extract_symbols_from_dir(args.a)
        md = apimap.render_coverage_md(syms, args.src, args.dst, args.dialect, reg=reg)
        out = args.out or os.path.join(args.a, 'APIMAP.md')
        with open(out, 'w', encoding='utf-8') as f:
            f.write(md)
        cov = reg.coverage(syms, args.src, args.dst, args.dialect)
        print('symbols=%d  mapped=%d  unsupported=%d  unmapped=%d  ->  %s'
              % (len(syms), len(cov['mapped']), len(cov['unsupported']), len(cov['unmapped']), out))
        return 0
    print('ERROR: unknown apimap action %r' % act); return 2


    return 0


def main(argv=None):
    argv = argv if argv is not None else sys.argv[1:]
    ap = argparse.ArgumentParser(prog='codebreaker java', description='The Java transpiler: a Java program as C++03.')
    sub = ap.add_subparsers(dest='cmd')

    # convert
    cp = sub.add_parser('convert', help='Convert a Java source tree to C++.')
    cp.add_argument('java_dir', nargs='?', default=None,
                    help='Directory of .java source. Omit and use --mc-version to decompile a '
                         'Minecraft version first.')
    cp.add_argument('out_dir', nargs='?', default=None,
                    help='Output directory for the generated .h/.cpp.')
    cp.add_argument('--mc-version', dest='mc_version', default=None,
                    help='Decompile this Minecraft version (bundled RetroMCP/Vineflower backend) and '
                         'convert its source in one step. Requires a Java runtime + network. '
                         'Usage: convert --mc-version <id> <out_dir>')
    cp.add_argument('--mc-workspace', dest='mc_workspace', default=None,
                    help='Workspace dir for the decompile step (default: <out_dir>/_decompiled).')
    cp.add_argument('--java', default=None,
                    help='java executable for the decompile step (else $JAVA_HOME / PATH).')
    cp.add_argument('--platform', default=None,
                    help='Target platform id (xbox360, pc_windows, ps3, wii). Drives endianness/'
                         'null-page rule messages + stub annotations.')
    cp.add_argument('--cpp-std', dest='cpp_std', default='C++03',
                    help='Target C++ standard (C++98, C++03, C++11, ...). Default C++03 (Xbox 360 XDK).')
    cp.add_argument('--platform-refs', dest='platform_refs', action='store_true',
                    help='Emit the REAL hand-written platform layer (GL->D3D9 emulator, audio, input, '
                         'socket) from docs/platform/reference/ instead of no-op stubs.')
    cp.add_argument('--project', action='store_true',
                    help='One-step buildable project: after converting, also (re)emit the platform '
                         'config (preserving controller binds) and produce the Visual Studio project -- '
                         'BOOTSTRAP a new .sln + .vcxproj from the template if none exists yet, or refresh '
                         'the .vcxproj/.filters file list if it does. No GUI needed. Implies --platform-refs.')
    cp.add_argument('--sdk', default=None,
                    help='Path to the target platform SDK root (else auto-detected via env/standard '
                         'paths). A console platform with no located SDK is refused.')
    cp.add_argument('--edition', default=None,
                    choices=['rubydung', 'indev', 'infdev', 'alpha', 'beta', 'release'],
                    help='Force the target Minecraft edition profile. Omit to infer from --mc-version, '
                         'else auto-detect from the source, else keep the default profile.')
    cp.set_defaults(func=_cmd_convert)

    # decompile (RetroMCP/Vineflower backend: Minecraft version -> Java source)
    dcp = sub.add_parser('decompile',
                         help='Decompile a Minecraft version to Java source (bundled RetroMCP/Vineflower).')
    dcp.add_argument('version', help='Minecraft version id (e.g. a1.2.6, b1.6.6, c0.30_01c, inf-20100618).')
    dcp.add_argument('workspace', help='Workspace dir; decompiled source lands under <workspace>/minecraft_*/src.')
    dcp.add_argument('--java', default=None, help='java executable (else $JAVA_HOME / PATH).')
    dcp.set_defaults(func=_cmd_decompile)

    # report
    rp = sub.add_parser('report', help='Build the conversion report from existing output.')
    rp.add_argument('output_dir', help='Directory of converted .cpp/.h.')
    rp.add_argument('--build-log', help='MSVC build log to mine for errors.')
    rp.add_argument('--out', help='Report output dir (default: <output_dir>/report).')
    rp.add_argument('--profile', help='Profile name for the report header.')
    rp.add_argument('--platform', default=None, help='Target platform id (xbox360, pc_windows, etc.).')
    rp.set_defaults(func=_cmd_report)

    # signatures
    sp = sub.add_parser('signatures', help='Generate signatures.md (codebase map).')
    sp.add_argument('java_dir', help='Directory of .java source.')
    sp.add_argument('--out', default=None,
                    help='Output path (default: signatures.md at repo root).')
    sp.add_argument('--edition', default=None,
                    choices=['rubydung', 'indev', 'infdev', 'alpha', 'beta', 'release'],
                    help='Use this edition profile\'s package roles (else the default profile).')
    sp.set_defaults(func=_cmd_signatures)

    # worklist
    wp = sub.add_parser('worklist', help='Compile output and write the failure worklist.')
    wp.add_argument('out_dir', help='Directory of converted .cpp/.h.')
    wp.add_argument('--cc', default=None, help='Compiler (default: settings.json host_cxx, else g++).')
    wp.add_argument('--std', default='c++03', help='C++ standard (default: c++03).')
    wp.add_argument('--out', default=None, help='Worklist output path.')
    wp.set_defaults(func=_cmd_worklist)

    # vcxproj
    vp = sub.add_parser('vcxproj', help='Regenerate the Xbox .vcxproj file list.')
    vp.add_argument('--proj', default=None, help='Xbox project dir (default: ../MinecraftAlpha/MinecraftAlpha).')
    vp.add_argument('--edition', default=None,
                    choices=['rubydung', 'indev', 'infdev', 'alpha', 'beta', 'release'],
                    help='Classify files with this edition profile\'s .filters taxonomy (else default).')
    vp.set_defaults(func=_cmd_vcxproj)

    # xact (audio bank tool)
    xp = sub.add_parser('xact', help='XACT3 audio banks: dump/build/coverage/match.')
    xp.add_argument('action', choices=['dump', 'build', 'coverage', 'match'],
                    help='dump <bank> | build <java_dir> <out_dir> | coverage <java_dir> <bank.xsb> '
                         '| match <java_dir> <resources_dir> [out_dir]')
    xp.add_argument('a', help='dump: bank file; build/coverage/match: java source dir.')
    xp.add_argument('b', nargs='?', help='build: out dir; coverage: .xsb; match: resources dir.')
    xp.add_argument('c', nargs='?', help='match: output dir for convert_audio.bat (default: cwd).')
    xp.add_argument('--run', dest='run', action='store_true',
                    help='build: autorun convert_audio.bat (ffmpeg) then build_banks.bat (XACTBLD) now.')
    xp.set_defaults(func=_cmd_xact)

    # assets (texture/resource extraction from the game jar)
    asp = sub.add_parser('assets', help='Extract textures/resources from the game jar into Content\\.')
    asp.add_argument('jar', help='Path to the game jar (e.g. deobfuscated.jar).')
    asp.add_argument('content_dir', help='Destination Content directory (assets land at Content\\<jar-path>).')
    asp.add_argument('--code', default=None,
                     help='Source tree (.java or converted .cpp/.h) to audit coverage against.')
    asp.add_argument('--only-needed', dest='only_needed', action='store_true',
                     help='Extract only paths the code references (needs --code); default extracts all assets.')
    asp.add_argument('--no-rgba', dest='no_rgba', action='store_true',
                     help='Do NOT normalize palettized/grayscale PNGs to RGBA (default normalizes them; '
                          'the 360 loader needs 32-bit RGBA or textures misrender).')
    asp.set_defaults(func=_cmd_assets)

    # platform-config (controller mapping + kernel-call selection)
    pcp = sub.add_parser('platform-config',
                         help='Select controller mapping + kernel calls (keyboard/storage); emit them.')
    pcp.add_argument('out_dir', help='Output dir for ControllerConfig.h/KernelConfig.h/Kernel.cpp.')
    pcp.add_argument('--platform', default='xbox360', help='Target platform (default xbox360).')
    pcp.add_argument('--sdk', default=None, help='SDK root override (else auto-detect).')
    pcp.add_argument('--layout', choices=['default', 'southpaw', 'legacy', 'legacy_southpaw'],
                     help='Stick layout preset (default/southpaw/legacy/legacy_southpaw).')
    pcp.add_argument('--invert-y', dest='invert_y', action='store_true', help='Invert look Y.')
    pcp.add_argument('--no-invert-y', dest='no_invert_y', action='store_true', help='Do not invert look Y.')
    pcp.add_argument('--sensitivity', type=float, default=None, help='Look sensitivity multiplier.')
    pcp.add_argument('--keyboard', choices=['xshowkeyboard', 'osk', 'stub'], help='Text-input medium.')
    pcp.add_argument('--storage', choices=['stfs', 'rawhdd', 'memory'], help='Save/load medium.')
    pcp.add_argument('--bind', action='append', metavar='ACTION=CONTROL',
                     help='Rebind an action, e.g. --bind jump=A (repeatable).')
    pcp.add_argument('--list', action='store_true', help='Print the menu + current choices, do not emit.')
    pcp.set_defaults(func=_cmd_platform_config)

    # achievements (analyze + map to console achievement system)
    acp = sub.add_parser('achievements',
                         help='Map the program\'s achievements to the console system (XLAST + unlock seam).')
    acp.add_argument('code_dir', help='Converted C++ (or Java) tree to scan for achievements.')
    acp.add_argument('out_dir', help='Output dir for ACHIEVEMENTS.md / achievements.xlast / Achievements.*')
    acp.add_argument('--lang', default=None, help='Optional .lang file for achievement names/descriptions.')
    acp.add_argument('--profile', choices=['retail', 'arcade'], default='retail',
                     help='Gamerscore budget profile (retail=1000/99, arcade=200/12).')
    acp.add_argument('--delivery', choices=['online', 'local', 'off'], default='online',
                     help='online = write to profile over LIVE; local = track locally; off.')
    acp.add_argument('--title-id', dest='title_id', default=None,
                     help='Self-issued 32-bit Title ID (hex, e.g. FFFE1234). Omit to auto-suggest one.')
    acp.add_argument('--title-name', dest='title_name', default=None,
                     help='Game name used to derive a suggested Title ID (when --title-id is omitted).')
    acp.add_argument('--content-dir', dest='content_dir', default=None,
                     help='Content dir holding the texture atlases (terrain.png, gui/items.png) that the '
                          'achievement tile art is cropped from. Omit to look next to out_dir '
                          '(Debug\\Content first -- the canonical deploy dir).')
    acp.set_defaults(func=_cmd_achievements)

    # ui-mark (tag UI functions with greppable markers)
    ump = sub.add_parser('ui-mark', help='Tag every UI function with a greppable J2X-UI marker + index.')
    ump.add_argument('dir', help='Converted C++ tree to mark.')
    ump.add_argument('--dry-run', dest='dry_run', action='store_true',
                     help='Report what would be marked without editing.')
    ump.add_argument('--extra', default=None,
                     help='Comma-separated extra UI class names to include.')
    ump.set_defaults(func=_cmd_ui_mark)

    # keybind-mark (tag hardcoded keyboard key-press sites for controller/menu routing)
    kbp = sub.add_parser('keybind-mark',
                         help='Tag every hardcoded keyboard key-press with a greppable J2X-KEYBIND marker + '
                              'KEYBIND_INDEX.md (route each to a controller button or menu toggle). '
                              'Runs automatically on every xbox360 convert.')
    kbp.add_argument('dir', help='Converted C++ tree to mark.')
    kbp.add_argument('--dry-run', dest='dry_run', action='store_true',
                     help='Report what would be tagged without editing.')
    kbp.set_defaults(func=_cmd_keybind_mark)

    # bugfix-ledger (catalog every FIXED BUG / J2X-BUGFIX marker into BUGFIX_LEDGER.md)
    blp = sub.add_parser('bugfix-ledger',
                         help='Catalog every FIXED BUG / J2X-BUGFIX marker in the converter\'s authored '
                              'sources into BUGFIX_LEDGER.md. Runs automatically on every convert.')
    blp.add_argument('--repo', default=None, help='Converter repo root (default: auto-detect).')
    blp.add_argument('--out', default=None, help='Output path (default: <repo>/BUGFIX_LEDGER.md).')
    blp.set_defaults(func=_cmd_bugfix_ledger)

    # debug (emit the single debug file / toggle)
    dbp = sub.add_parser('debug', help='Emit the single toggleable debug file (Debug.h/.cpp + DEBUG.md).')
    dbp.add_argument('out_dir', help='Directory to emit the debug files into.')
    dbp.add_argument('--off', action='store_true', help='Bake DEBUG_ENABLED=0 (debug support present but off).')
    dbp.set_defaults(func=_cmd_debug)

    # sdk (detect/gatekeep)
    sdp = sub.add_parser('sdk', help='Detect the platform SDK the converter would gate on.')
    sdp.add_argument('platform', help='Platform id (xbox360, ps3, wii, pc_windows).')
    sdp.add_argument('--sdk', default=None, help='SDK root override to test.')
    sdp.set_defaults(func=_cmd_sdk)

    # project-layout (solution root + project subfolder + .sln, fixes flat dumps)
    plp = sub.add_parser('project-layout',
                         help='Give a converted tree the VS solution layout (<root>/<Name>.sln + <root>/<Name>/).')
    plp.add_argument('root', help='Solution root; its basename is the project name.')
    plp.add_argument('--name', default=None, help='Override the project name (default: root folder name).')
    plp.set_defaults(func=_cmd_project_layout)

    # reflect-factories (turn class-registry reflection into real factories)
    rfp = sub.add_parser('reflect-factories',
                         help='Turn Java reflection registries (newInstance) into generated C++ factories.')
    rfp.add_argument('out_dir', help='Converted output directory to rewrite in place.')
    rfp.set_defaults(func=_cmd_reflect_factories)

    # sound-fix (console SFX bypass: route sound names to the XACT bridge past the empty OGG pool)
    sfp = sub.add_parser('sound-fix',
                         help='Drop the empty-OGG-pool null-guard in SoundManager so sound effects route to '
                              'the XACT bridge (fixes silent SFX on console). Runs automatically on every '
                              'xbox360 convert.')
    sfp.add_argument('out_dir', help='Converted output directory to rewrite in place.')
    sfp.set_defaults(func=_cmd_sound_fix)

    # controller-nav (console D-pad menu navigation: inject the GuiScreen adapter + per-frame call)
    cnp = sub.add_parser('controller-nav',
                         help='Inject console controller MENU navigation into GuiScreen (D-pad moves the '
                              'selection, snaps the cursor so hover+A-click work). The nav algorithm lives in '
                              'Input.reference; this injects the thin adapter. Runs automatically on every '
                              'xbox360 convert.')
    cnp.add_argument('out_dir', help='Converted output directory to rewrite in place.')
    cnp.set_defaults(func=_cmd_controller_nav)

    # render-fix (console first-frame render: chunk-rebuild budget + hurt-camera NaN guard)
    rfp = sub.add_parser('render-fix',
                         help='Apply the two console render fixes: cap RenderGlobal::updateRenderers to a few '
                              'chunk rebuilds per frame (else a fresh world never presents -- stuck on '
                              '"Spawning"), and guard EntityRenderer::hurtCameraEffect against the spawn 0/0 '
                              'NaN (else the world renders invisible). Runs automatically on every xbox360 '
                              'convert.')
    rfp.add_argument('out_dir', help='Converted output directory to rewrite in place.')
    rfp.set_defaults(func=_cmd_render_fix)

    # applet-ping (console setLevel: drop the applet host ping that never answers on 360)
    app = sub.add_parser('applet-ping',
                         help='Neutralize the applet host ping in setLevel (getDocumentBase()->openStream()) so '
                              'the world is set unconditionally on console. Runs automatically on every '
                              'xbox360 convert.')
    app.add_argument('out_dir', help='Converted output directory to rewrite in place.')
    app.set_defaults(func=_cmd_applet_ping)

    # gui
    # apimap (API-mapping registry: cross-language / cross-API function equivalence)
    amp = sub.add_parser('apimap',
                         help='Query the API-mapping registry (languages/subsystems/resolve/coverage).')
    amp.add_argument('action', choices=['languages', 'subsystems', 'list', 'resolve', 'coverage'],
                     help='languages | subsystems | list <subsystem> | resolve <symbol> --from L --to L '
                          '| coverage <java_dir> --from L --to L')
    amp.add_argument('a', nargs='?', help='subsystem id / symbol / java_dir depending on action.')
    amp.add_argument('--from', dest='src', default=None, help='source language/api id.')
    amp.add_argument('--to', dest='dst', default=None, help='target language/api id.')
    amp.add_argument('--dialect', default=None, help='target dialect (e.g. c++03, c++11).')
    amp.add_argument('--out', default=None, help='coverage: output path (default <java_dir>/APIMAP.md).')
    amp.set_defaults(func=_cmd_apimap)

    # build (MSBuild over the solution)
    bp = sub.add_parser('build', help='Build a Visual Studio solution with MSBuild (settings.json: msbuild).')
    bp.add_argument('sln', help='The .sln to build.')
    bp.add_argument('--config', default='Release')
    bp.add_argument('--platform', default='Xbox 360')
    bp.add_argument('--log', default=None, help='Build log (default: state/java/build.log).')
    bp.set_defaults(func=_cmd_build)

    # scaffold (structural skeletons in other languages -- not a working translation)
    scp = sub.add_parser('scaffold', help='Emit one STRUCTURAL skeleton file per Java class in C, C# or Rust '
                                          '(types, fields, method signatures; not a working translation).')
    scp.add_argument('java_dir')
    scp.add_argument('out_dir')
    scp.add_argument('--language', choices=['c', 'csharp', 'rust'], default='c')
    scp.set_defaults(func=_cmd_scaffold)

    # scan-project (one folder -> the jar, sound resources, Java source and language file in it)
    spp = sub.add_parser('scan-project', help='From one folder of a decompiled game, find its jar, sound '
                                              'resources, Java source and language file.')
    spp.add_argument('folder')
    spp.set_defaults(func=_cmd_scan_project)

    args = ap.parse_args(argv)
    if not getattr(args, 'cmd', None):
        print(__doc__)
        return 2
    return args.func(args)
