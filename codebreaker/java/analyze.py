# -*- coding: utf-8 -*-
"""
analyze.py -- build a Ledger from artifacts the converter ALREADY produces.

Phase 1 is deliberately non-invasive: we do not touch java2xbox360.py or its
output. We mine the signal that's already sitting in the output tree (the 500+
`// PORT:` markers), the lint rules, the build log, and the handwritten-override
set -- and turn it into structured Events.

Later phases will move event production *into* the generator/rules so we capture
PORTED/GUESSED decisions at the moment they're made instead of reconstructing
them. Until then, these analyzers give you the per-file ledger today.

Each analyzer takes a Ledger and the relevant path(s) and appends Events.
"""

import os
import re

from codebreaker.java.ledger import Category, Severity


# ---------------------------------------------------------------------------
# 1. PORT / STUB / TODO markers embedded in the converted source
# ---------------------------------------------------------------------------

# Marker tokens, most-specific FIRST so 'PORT-BLOCKED' wins over 'PORT'.
# Each entry: (compiled regex matching the token, default Category).
_MARKER_TOKENS = [
    (re.compile(r'PORT-BLOCKED'),            Category.BLOCKED),
    (re.compile(r'PORT[- ]WARNING'),         Category.WARNING),
    (re.compile(r'PORT\s+stub'),             Category.STUBBED),
    (re.compile(r'PORT\s+NOTES?'),           Category.ANNOTATED),
    (re.compile(r'PORT\s+helpers?'),         Category.ANNOTATED),
    (re.compile(r'PORT\([^)]*\)'),           Category.ANNOTATED),  # PORT(post-vs-pre-multiply) etc.
    (re.compile(r'PORT'),                    Category.ANNOTATED),  # bare 'PORT:' -- refined below by content
    (re.compile(r'\bSTUB\b'),                Category.STUBBED),
    (re.compile(r'\bFIXME\b'),               Category.WARNING),
    (re.compile(r'\bTODO\b'),                Category.WARNING),
    (re.compile(r'\bXXX\b'),                 Category.WARNING),
]

# Content keywords that RE-CLASSIFY a bare `// PORT:` comment into a sharper bucket.
# These are the highest-value signals: a "guess" is a future bug, a "stub" is unfinished work.
_GUESS_HINTS = re.compile(
    r'\b(value type|by value|value-vector|pointer|determine|infer\w*|assume\w*|'
    r'guess\w*|element type|was bare|reclassif\w*|ambiguous|may be|might be)\b', re.I)
_STUB_HINTS = re.compile(
    r'\b(D3D9|XInput|XAudio|occlusion|reflection|no RTTI|no stack|dumpStack|Thread|'
    r'not implemented|unimplemented|removed|no such|replaced by|needs|implement\b|'
    r'LockRect|CreateTexture|gamepad)\b', re.I)

_COMMENT_RE = re.compile(r'(?://+|/\*+)\s*(.*?)\s*(?:\*/)?\s*$')


def _classify_marker(comment_text, base_category):
    """Refine a bare PORT marker by its content; return (category, severity)."""
    if base_category == Category.ANNOTATED:
        if _GUESS_HINTS.search(comment_text):
            return Category.GUESSED, Severity.HIGH
        if _STUB_HINTS.search(comment_text):
            return Category.STUBBED, Severity.HIGH
    return base_category, None  # None -> Event uses the category default


def scan_markers(ledger, output_dir):
    """Walk converted .cpp/.h and turn every PORT/STUB/TODO marker into an Event."""
    n = 0
    for fname in sorted(os.listdir(output_dir)):
        if not (fname.endswith('.cpp') or fname.endswith('.h')):
            continue
        path = os.path.join(output_dir, fname)
        try:
            with open(path, 'r', encoding='utf-8', errors='replace') as f:
                lines = f.read().split('\n')
        except (IOError, OSError):
            continue
        for i, raw in enumerate(lines, 1):
            if 'PORT' not in raw and 'STUB' not in raw and 'TODO' not in raw \
               and 'FIXME' not in raw and 'XXX' not in raw:
                continue
            # isolate the comment text (after // or inside /* */); skip if no comment
            cm = _COMMENT_RE.search(raw)
            comment = cm.group(1) if cm else raw
            for tok_re, base_cat in _MARKER_TOKENS:
                m = tok_re.search(comment)
                if not m:
                    continue
                cat, sev = _classify_marker(comment, base_cat)
                # message = the comment text after the marker token
                msg = comment[m.end():].lstrip(' :)-').strip() or comment.strip()
                ledger.add(fname, cat, msg, line=i, severity=sev,
                           source='marker', rule=m.group(0),
                           snippet=raw.strip()[:160])
                n += 1
                break  # one marker classification per line
    return n


# ---------------------------------------------------------------------------
# 2. Known crash/leak patterns (reuse the existing converter_lint rules)
# ---------------------------------------------------------------------------

def scan_lint(ledger, output_dir):
    """Fold an optional external linter's findings into the ledger.

    `converter_lint` is an optional, external rule module (the legacy monolithic linter). It is
    not bundled with CodeBreaker; when it is not importable this pass is simply skipped. Provide your own
    module exposing `lint_tree(dir) -> [Finding(file, line, message, severity, rule)]` to enable it.
    """
    try:
        import converter_lint
    except ImportError:
        return 0
    sev_map = {'CRASH': Severity.HIGH, 'LEAK': Severity.HIGH, 'WARN': Severity.MED}
    n = 0
    for fnd in converter_lint.lint_tree(output_dir):
        ledger.add(os.path.basename(fnd.file), Category.RISK, fnd.message,
                   line=fnd.line, severity=sev_map.get(fnd.severity, Severity.MED),
                   source='lint', rule='%s/%s' % (fnd.severity.lower(), fnd.rule),
                   snippet='')
        n += 1
    return n


# Catches the converter's member-access (`.` -> `->`) translation leaking into a STRING LITERAL --
# a rare AST edge case that silently breaks a resource path, e.g. "/gui/furnace.png" emitted as
# "/gui/furnace->png": the texture load fails, returns 0, and the engine samples the last-bound
# atlas (the font sheet) full-screen. Real occurrences: GuiFurnace (furnace.png), CanvasMajongLogo
# (logo.png) -- fixed via overrides. This flags any recurrence on regen so it can't ship silently.
_CORRUPT_STR_RE = re.compile(
    r'"[^"\n]*->(?:png|txt|dat|zip|lvl|class|gif|ogg|wav|properties|lang|nbt|mclevel)[^"\n]*"')

def scan_corrupted_strings(ledger, output_dir):
    """Flag string literals corrupted by member-access translation (broken resource paths)."""
    n = 0
    for fname in sorted(os.listdir(output_dir)):
        if not (fname.endswith('.cpp') or fname.endswith('.h')):
            continue
        try:
            with open(os.path.join(output_dir, fname), 'r', errors='replace') as fh:
                for i, raw in enumerate(fh, 1):
                    m = _CORRUPT_STR_RE.search(raw)
                    if m:
                        ledger.add(fname, Category.RISK,
                                   'corrupted string literal (member-access "." -> "->" leaked into a '
                                   'string -> broken resource path): %s' % m.group(0),
                                   line=i, severity=Severity.HIGH, source='lint',
                                   rule='high/corrupt-string-literal', snippet=raw.strip())
                        n += 1
        except (IOError, OSError):
            pass
    return n


# ---------------------------------------------------------------------------
# 3. Compiler errors from a build log (optional -- newest log by default)
# ---------------------------------------------------------------------------

_MSVC_RE = re.compile(
    r'([A-Za-z0-9_]+\.(?:cpp|h))\((\d+)\)\s*:\s*(?:fatal\s+)?error\s+(C\d+)\s*:\s*(.*)')


def scan_build_log(ledger, log_path):
    """Parse MSVC `File.cpp(line): error CNNNN: msg` lines into ERROR events."""
    if not log_path or not os.path.isfile(log_path):
        return 0
    n = 0
    seen = set()
    with open(log_path, 'r', encoding='utf-8', errors='replace') as f:
        for raw in f:
            m = _MSVC_RE.search(raw)
            if not m:
                continue
            fname, line, code, msg = m.group(1), m.group(2), m.group(3), m.group(4)
            key = (fname, line, code, msg[:40])
            if key in seen:        # MSVC repeats errors across the /MP cl invocations
                continue
            seen.add(key)
            ledger.add(fname, Category.ERROR, msg.strip()[:200], line=line,
                       source='build', rule=code, snippet='')
            n += 1
    return n


def newest_build_log(root):
    """Return the path of the most-recently-modified build_alpha*.log, or None."""
    logs = [os.path.join(root, f) for f in os.listdir(root)
            if f.startswith('build_alpha') and f.endswith('.log')]
    if not logs:
        return None
    return max(logs, key=os.path.getmtime)



# ---------------------------------------------------------------------------
# Orchestration
# ---------------------------------------------------------------------------

def _detect_platform(output_dir):
    """Try to detect the target platform from a generated header's comment line."""
    from codebreaker.java.platforms import PLATFORMS
    for fname in sorted(os.listdir(output_dir)):
        if not fname.endswith('.h'):
            continue
        path = os.path.join(output_dir, fname)
        try:
            with open(path, 'r', encoding='utf-8', errors='replace') as f:
                for line_no, line in enumerate(f):
                    if line_no > 10:
                        break
                    if '// Target:' in line:
                        for pid, plat in PLATFORMS.items():
                            if plat['label'] in line:
                                return pid
        except Exception:
            continue
    return None


def analyze_all(ledger, output_dir, build_log=None, root=None, platform=None):
    """Run every available analyzer against the artifacts and return per-source counts."""
    root = root or os.getcwd()
    if build_log is None:
        build_log = newest_build_log(root)
    if platform is None:
        platform = _detect_platform(output_dir)
    from codebreaker.java import rules
    counts = {
        'markers':   scan_markers(ledger, output_dir),
        'rules':     rules.scan_rules(ledger, output_dir, platform=platform),
        'lint':      scan_lint(ledger, output_dir),
        'corrupt_strings': scan_corrupted_strings(ledger, output_dir),
        'build_log': scan_build_log(ledger, build_log),
    }
    counts['_build_log_path'] = build_log
    return counts
