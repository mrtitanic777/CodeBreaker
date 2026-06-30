# -*- coding: utf-8 -*-
"""
cli.py -- command-line entry for the j2x converter.

    python -m j2x convert    <java_dir> <out_dir> [--overrides DIR]
    python -m j2x report     <output_dir> [--overrides DIR] [--build-log FILE] [--out DIR]
    python -m j2x signatures <java_dir> [--out FILE]
    python -m j2x worklist   <out_dir> [--cc CC] [--std STD] [--out FILE]
    python -m j2x vcxproj    [--proj DIR]
"""

import argparse
import os
import sys

from .ledger import Ledger, Severity
from . import analyze, report

_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
_DEFAULT_OVERRIDES = os.path.join(_ROOT, 'overrides')


def _resolve_overrides(explicit):
    if explicit is not None:
        return explicit
    if os.path.isdir(_DEFAULT_OVERRIDES):
        return _DEFAULT_OVERRIDES
    return None


def _cmd_convert(args):
    from . import pipeline
    if not os.path.isdir(args.java_dir):
        print('ERROR: not a directory: %s' % args.java_dir)
        return 2
    ov = _resolve_overrides(args.overrides)
    if args.overrides and not os.path.isdir(ov):
        print('ERROR: overrides dir not found: %s' % args.overrides)
        return 2
    s = pipeline.convert(args.java_dir, args.out_dir, overrides_dir=ov)
    print('Converted %s -> %s' % (os.path.abspath(args.java_dir), os.path.abspath(args.out_dir)))
    print('  parsed=%d  headers=%d  sources=%d  overrides=%d'
          % (s['parsed'], s['headers'], s['sources'], s['overrides']))
    print('  verify (crash-class rule findings on output): %d' % s['rule_findings'])
    print('  ledger events=%d  (HIGH=%d)' % (s['events'], s['high']))
    if s['report']:
        print('  report: %s' % os.path.join(s['report'], 'index.md'))
    return 0


def _cmd_report(args):
    output_dir = args.output_dir
    if not os.path.isdir(output_dir):
        print('ERROR: not a directory: %s' % output_dir)
        return 2

    root = os.getcwd()
    override_dir = _resolve_overrides(args.overrides)

    ledger = Ledger()
    counts = analyze.analyze_all(
        ledger, output_dir,
        override_dir=override_dir,
        build_log=args.build_log,
        pristine_dir=args.pristine,
        root=root,
    )

    meta = {
        'output_dir': os.path.abspath(output_dir),
        'build_log': counts.get('_build_log_path') or '(none)',
        'profile': args.profile or '(unspecified)',
    }
    out_dir = report.write_reports(ledger, output_dir, report_dir=args.out, meta=meta)

    sevs = ledger.totals_by_severity()
    print('Analyzed %s' % os.path.abspath(output_dir))
    print('  markers=%d  rules=%d  lint=%d  build_log=%d  overrides=%d'
          % (counts['markers'], counts['rules'], counts['lint'],
             counts['build_log'], counts['overrides']))
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


def _cmd_signatures(args):
    from . import signatures
    if not os.path.isdir(args.java_dir):
        print('ERROR: not a directory: %s' % args.java_dir)
        return 2
    signatures.run(args.java_dir, args.out)
    return 0


def _cmd_worklist(args):
    from . import worklist
    if not os.path.isdir(args.out_dir):
        print('ERROR: not a directory: %s' % args.out_dir)
        return 2
    return worklist.run(args.out_dir, cc=args.cc, std=args.std, worklist_path=args.out)


def _cmd_vcxproj(args):
    from . import vcxproj
    vcxproj.run(proj_dir=args.proj)
    return 0


def main(argv=None):
    argv = argv if argv is not None else sys.argv[1:]
    ap = argparse.ArgumentParser(prog='j2x', description='Java -> Xbox 360 C++03 converter.')
    sub = ap.add_subparsers(dest='cmd')

    # convert
    cp = sub.add_parser('convert', help='Convert a Java source tree to C++03.')
    cp.add_argument('java_dir', help='Directory of .java source.')
    cp.add_argument('out_dir', help='Output directory for the generated .h/.cpp.')
    cp.add_argument('--overrides', default=None,
                    help='Override dir (default: overrides/ next to j2x).')
    cp.set_defaults(func=_cmd_convert)

    # report
    rp = sub.add_parser('report', help='Build the conversion report from existing output.')
    rp.add_argument('output_dir', help='Directory of converted .cpp/.h.')
    rp.add_argument('--overrides', default=None, help='Override dir (default: overrides/).')
    rp.add_argument('--build-log', help='MSVC build log to mine for errors.')
    rp.add_argument('--pristine', help='Pristine (no-override) output dir for debt measurement.')
    rp.add_argument('--out', help='Report output dir (default: <output_dir>/report).')
    rp.add_argument('--profile', help='Profile name for the report header.')
    rp.set_defaults(func=_cmd_report)

    # signatures
    sp = sub.add_parser('signatures', help='Generate signatures.md (codebase map).')
    sp.add_argument('java_dir', help='Directory of .java source.')
    sp.add_argument('--out', default=os.path.join(_ROOT, 'signatures.md'),
                    help='Output path (default: signatures.md at repo root).')
    sp.set_defaults(func=_cmd_signatures)

    # worklist
    wp = sub.add_parser('worklist', help='Compile output and write the failure worklist.')
    wp.add_argument('out_dir', help='Directory of converted .cpp/.h.')
    wp.add_argument('--cc', default=None, help='Compiler (default: $J2X_CC or g++).')
    wp.add_argument('--std', default='c++03', help='C++ standard (default: c++03).')
    wp.add_argument('--out', default=None, help='Worklist output path (default: overrides/WORKLIST.md).')
    wp.set_defaults(func=_cmd_worklist)

    # vcxproj
    vp = sub.add_parser('vcxproj', help='Regenerate the Xbox .vcxproj file list.')
    vp.add_argument('--proj', default=None, help='Xbox project dir (default: ../MinecraftAlpha/MinecraftAlpha).')
    vp.set_defaults(func=_cmd_vcxproj)

    args = ap.parse_args(argv)
    if not getattr(args, 'cmd', None):
        ap.print_help()
        return 1
    return args.func(args)
