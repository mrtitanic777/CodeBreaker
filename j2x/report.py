# -*- coding: utf-8 -*-
"""
report.py -- render a Ledger into the report bundle.

Outputs (under <output_dir>/report/ by default):
  index.md        -- run summary: totals, severity, the RISK-RANKED file list,
                     and the override / backport-debt section. Read this first.
  <File>.md       -- per-file ledger: every event for that file, grouped by
                     category, with line numbers + snippets.
  report.json     -- the whole ledger, machine-readable (for the GUI / diffs).

The risk ranking is the point: it tells you WHICH few files to inspect before
you build, instead of discovering it by crashing on hardware.
"""

import os

from .ledger import Category, Severity


# Friendly one-liners describing each category, shown in the report legend.
_CAT_BLURB = {
    Category.ERROR:      'compiler rejected it (from the build log)',
    Category.GUESSED:    'converter made a heuristic call that may be wrong -- INSPECT',
    Category.STUBBED:    'placeholder; needs a hand implementation',
    Category.BLOCKED:    'explicitly could not be ported',
    Category.RISK:       'matches a known console-crash / leak pattern',
    Category.WARNING:    'suspicious; verify by hand',
    Category.OVERRIDDEN: 'a hand-written file is in use for this class',
    Category.ANNOTATED:  'a note about a decision already taken',
    Category.PORTED:     'translated literally and cleanly',
    Category.RULE:       'an automatic fix rule fired',
}

_SEV_TAG = {Severity.HIGH: '🔴 HIGH', Severity.MED: '🟡 MED', Severity.LOW: '⚪ LOW'}


def _safe(name):
    return name.replace('/', '_').replace('\\', '_')


def write_reports(ledger, output_dir, report_dir=None, meta=None):
    report_dir = report_dir or os.path.join(output_dir, 'report')
    os.makedirs(report_dir, exist_ok=True)
    files_dir = os.path.join(report_dir, 'files')
    os.makedirs(files_dir, exist_ok=True)

    _write_index(ledger, report_dir, meta or {})
    for fname, evs in ledger.by_file().items():
        _write_file_report(fname, evs, ledger, files_dir)
    with open(os.path.join(report_dir, 'report.json'), 'w', encoding='utf-8') as f:
        f.write(ledger.to_json())
    return report_dir


def _bar(n, width=24, total=None):
    if not total:
        return ''
    fill = int(round(width * n / float(total))) if total else 0
    return '█' * fill + '·' * (width - fill)


def _write_index(ledger, report_dir, meta):
    L = []
    L.append('# Conversion Report')
    L.append('')
    if meta:
        for k in ('profile', 'output_dir', 'build_log', 'generated'):
            if meta.get(k):
                L.append('- **%s:** %s' % (k.replace('_', ' ').title(), meta[k]))
        L.append('')

    cats = ledger.totals_by_category()
    sevs = ledger.totals_by_severity()
    L.append('## Totals')
    L.append('')
    L.append('| Severity | Count |')
    L.append('|---|---|')
    L.append('| 🔴 HIGH | %d |' % sevs[Severity.HIGH])
    L.append('| 🟡 MED  | %d |' % sevs[Severity.MED])
    L.append('| ⚪ LOW  | %d |' % sevs[Severity.LOW])
    L.append('| **Total events** | **%d** |' % len(ledger))
    L.append('')
    total_ev = max(1, len(ledger))
    L.append('| Category | Count | | What it means |')
    L.append('|---|---:|---|---|')
    for c in Category.ALL:
        n = cats.get(c, 0)
        if n:
            L.append('| `%s` | %d | %s | %s |' % (c, n, _bar(n, 18, total_ev), _CAT_BLURB.get(c, '')))
    L.append('')

    # Risk-ranked files -- the headline. Inspect the top of this list first.
    rows = ledger.file_risk_table()
    risky = [r for r in rows if r[1] > 0]
    L.append('## Files by risk (inspect top-down)')
    L.append('')
    L.append('%d files have findings; %d carry HIGH-severity risk.' %
             (len(risky), sum(1 for r in rows if r[3].get(Severity.HIGH))))
    L.append('')
    L.append('| Risk | File | 🔴 | 🟡 | guess | stub | risk | err | override |')
    L.append('|---:|---|---:|---:|---:|---:|---:|---:|---:|')
    for fname, score, c, s in rows[:40]:
        if score == 0:
            continue
        link = 'files/%s.md' % _safe(fname)
        L.append('| %d | [%s](%s) | %d | %d | %d | %d | %d | %d | %d |' % (
            score, fname, link,
            s.get(Severity.HIGH, 0), s.get(Severity.MED, 0),
            c.get(Category.GUESSED, 0), c.get(Category.STUBBED, 0),
            c.get(Category.RISK, 0), c.get(Category.ERROR, 0),
            c.get(Category.OVERRIDDEN, 0)))
    L.append('')

    # Override / backport-debt section
    ov = [e for e in ledger.events if e.category == Category.OVERRIDDEN]
    if ov:
        retire = [e for e in ov if e.rule == 'retire']
        backport = [e for e in ov if e.rule in ('backport', 'backport-new')]
        undet = [e for e in ov if e.rule == 'override']
        L.append('## Override / back-port debt')
        L.append('')
        L.append('- Hand-written overrides in use: **%d**' % len(ov))
        if retire:
            L.append('- ✅ Redundant (byte-identical to generated output, **safe to retire**): **%d**' % len(retire))
        if backport:
            L.append('- 🔧 Contain a hand-fix not yet back-ported (**real debt**): **%d**' % len(backport))
        if undet:
            L.append('- ❔ Undetermined — re-run with `--pristine <dir>` to classify: **%d**' % len(undet))
        L.append('')
        if backport:
            L.append('Back-port these into the generator (then the override can be deleted):')
            L.append('')
            for e in sorted(backport, key=lambda e: e.file):
                L.append('- `%s`' % e.file)
            L.append('')

    L.append('## Legend')
    L.append('')
    L.append('Sources: `marker` (PORT/STUB/TODO comment), `rule` (structural crash-class check), '
             '`lint` (known crash/leak pattern), `build` (compiler error), `override` (hand-written file).')
    L.append('')

    with open(os.path.join(report_dir, 'index.md'), 'w', encoding='utf-8') as f:
        f.write('\n'.join(L))


def _write_file_report(fname, evs, ledger, files_dir):
    L = []
    L.append('# %s' % fname)
    L.append('')
    L.append('Risk score: **%d**  ·  %d findings' % (ledger.risk_score(evs), len(evs)))
    L.append('')
    by_cat = {}
    for e in evs:
        by_cat.setdefault(e.category, []).append(e)
    for c in Category.ALL:
        group = by_cat.get(c)
        if not group:
            continue
        L.append('## %s — %s' % (c, _CAT_BLURB.get(c, '')))
        L.append('')
        for e in group:
            loc = ('L%d' % e.line) if e.line else '—'
            tag = _SEV_TAG.get(e.severity, e.severity)
            L.append('- **%s** `%s` (%s, `%s`): %s' % (tag, loc, e.source, e.rule, e.message))
            if e.snippet:
                L.append('  ```cpp')
                L.append('  %s' % e.snippet)
                L.append('  ```')
        L.append('')
    with open(os.path.join(files_dir, _safe(fname) + '.md'), 'w', encoding='utf-8') as f:
        f.write('\n'.join(L))
