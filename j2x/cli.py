# -*- coding: utf-8 -*-
"""
cli.py -- command-line entry for the j2x reporting layer.

    python -m j2x report <output_dir> [--overrides DIR] [--build-log FILE]
                                      [--pristine DIR] [--out DIR]

Reads the artifacts already sitting in <output_dir> (converted .cpp/.h, PORT
markers), runs the lint + structural rules, optionally folds in compiler errors
from a build log and override/back-port debt, and writes the report bundle.

Nothing here mutates the converter or its output -- it only reads + reports.
"""

import argparse
import os
import sys

from .ledger import Ledger, Severity
from . import analyze, report


def _cmd_convert(args):
    from . import pipeline
    if not os.path.isdir(args.java_dir):
        print('ERROR: not a directory: %s' % args.java_dir)
        return 2
    if args.overrides and not os.path.isdir(args.overrides):
        print('ERROR: overrides dir not found: %s' % args.overrides)
        return 2
    s = pipeline.convert(args.java_dir, args.out_dir, overrides_dir=args.overrides)
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
    override_dir = args.overrides
    if override_dir is None:
        # sensible default: handwritten_<profile> next to cwd, guessed from output dir name
        guess = 'handwritten_alpha' if 'alpha' in os.path.basename(output_dir.rstrip('/\\')) else None
        if guess and os.path.isdir(os.path.join(root, guess)):
            override_dir = os.path.join(root, guess)

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


def main(argv=None):
    argv = argv if argv is not None else sys.argv[1:]
    ap = argparse.ArgumentParser(prog='j2x', description='Modular Java->Xbox360 converter tooling.')
    sub = ap.add_subparsers(dest='cmd')

    cp = sub.add_parser('convert', help='Convert a Java source tree to C++03 (the j2x rewrite).')
    cp.add_argument('java_dir', help='Directory of .java source.')
    cp.add_argument('out_dir', help='Output directory for the generated .h/.cpp.')
    cp.add_argument('--overrides', help='Handwritten override dir: <Class>.h/.cpp used verbatim '
                    'instead of generated output (for the genuinely-hard cases).')
    cp.set_defaults(func=_cmd_convert)

    rp = sub.add_parser('report', help='Build the conversion report from existing output.')
    rp.add_argument('output_dir', help='Directory of converted .cpp/.h (e.g. output_alpha).')
    rp.add_argument('--overrides', help='Hand-written override dir (default: auto-detect handwritten_alpha).')
    rp.add_argument('--build-log', help='MSVC build log to mine for errors (default: newest build_alpha*.log).')
    rp.add_argument('--pristine', help='A pristine (no-override) converter output dir, to measure back-port debt.')
    rp.add_argument('--out', help='Report output dir (default: <output_dir>/report).')
    rp.add_argument('--profile', help='Profile name, for the report header.')
    rp.set_defaults(func=_cmd_report)

    args = ap.parse_args(argv)
    if not getattr(args, 'cmd', None):
        ap.print_help()
        return 1
    return args.func(args)
