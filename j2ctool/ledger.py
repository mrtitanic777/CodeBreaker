# -*- coding: utf-8 -*-
"""
ledger.py -- the structured record of everything the converter does to a file.

The whole point of the restructure is that the converter must TELL YOU what it
did. Every observable action becomes an Event with a Category, a Severity, a
location (file + line), and a human message. A Ledger is just a queryable bag of
Events plus the roll-ups the reports need (per-file grouping, risk scoring,
totals).

Nothing here knows *how* events are produced -- analyzers (post-hoc, Phase 1) and
eventually the generator/rules (instrumented, later phases) both just call
ledger.add(...). That separation is the contract that lets us swap the monolith
out underneath the reporting without touching the reports.
"""

import json


# ---------------------------------------------------------------------------
# Categories -- WHAT kind of thing happened to the code.
# Ordered roughly by how much it should worry you, worst first.
# ---------------------------------------------------------------------------
class Category(object):
    ERROR      = 'ERROR'       # compiler said no (from a build log) -- definitely broken
    GUESSED    = 'GUESSED'     # converter made a heuristic call that could be wrong (value/pointer,
                               #   inferred element type) -- THE category most likely to be a latent bug
    STUBBED    = 'STUBBED'     # replaced with a placeholder; needs a hand implementation
    BLOCKED    = 'BLOCKED'     # explicitly could-not-port (PORT-BLOCKED)
    RISK       = 'RISK'        # matches a known crash/leak pattern (from converter_lint)
    WARNING    = 'WARNING'     # suspicious; verify by hand (PORT-WARNING, TODO/XXX/FIXME)
    OVERRIDDEN = 'OVERRIDDEN'  # a hand-written file replaced generated output for this class
    ANNOTATED  = 'ANNOTATED'   # an explanatory note about a decision already taken
    PORTED     = 'PORTED'      # translated literally and cleanly (the boring, good case)
    RULE       = 'RULE'        # a named fix/rule fired (before -> after)

    ALL = [ERROR, GUESSED, STUBBED, BLOCKED, RISK, WARNING,
           OVERRIDDEN, ANNOTATED, PORTED, RULE]


# ---------------------------------------------------------------------------
# Severities -- HOW MUCH you should care. Drives the per-file risk score.
# ---------------------------------------------------------------------------
class Severity(object):
    HIGH = 'HIGH'
    MED  = 'MED'
    LOW  = 'LOW'

    WEIGHT = {'HIGH': 5, 'MED': 2, 'LOW': 0}
    ORDER  = {'HIGH': 0, 'MED': 1, 'LOW': 2}


# Default severity for each category (an analyzer may override per-event).
_DEFAULT_SEVERITY = {
    Category.ERROR:      Severity.HIGH,
    Category.GUESSED:    Severity.HIGH,
    Category.STUBBED:    Severity.HIGH,
    Category.BLOCKED:    Severity.HIGH,
    Category.RISK:       Severity.MED,
    Category.WARNING:    Severity.MED,
    Category.OVERRIDDEN: Severity.MED,
    Category.ANNOTATED:  Severity.LOW,
    Category.PORTED:     Severity.LOW,
    Category.RULE:       Severity.LOW,
}


class Event(object):
    """One thing that happened to one place in the code."""

    __slots__ = ('file', 'line', 'category', 'severity', 'source',
                 'rule', 'message', 'snippet')

    def __init__(self, file, category, message, line=0, severity=None,
                 source='', rule='', snippet=''):
        self.file = file
        self.line = int(line) if line else 0
        self.category = category
        self.severity = severity or _DEFAULT_SEVERITY.get(category, Severity.LOW)
        self.source = source        # which analyzer/stage produced this ('marker', 'lint', ...)
        self.rule = rule            # rule id / error code / marker keyword
        self.message = message
        self.snippet = snippet      # the offending source line, stripped

    def as_dict(self):
        return {
            'file': self.file, 'line': self.line, 'category': self.category,
            'severity': self.severity, 'source': self.source, 'rule': self.rule,
            'message': self.message, 'snippet': self.snippet,
        }


class Ledger(object):
    """A queryable collection of Events, plus the roll-ups the reports need."""

    def __init__(self):
        self.events = []

    # -- recording -----------------------------------------------------------
    def add(self, file, category, message, **kw):
        self.events.append(Event(file, category, message, **kw))

    def extend(self, events):
        self.events.extend(events)

    def __len__(self):
        return len(self.events)

    # -- queries -------------------------------------------------------------
    def files(self):
        return sorted({e.file for e in self.events})

    def by_file(self):
        out = {}
        for e in self.events:
            out.setdefault(e.file, []).append(e)
        for evs in out.values():
            evs.sort(key=lambda e: (e.line, Severity.ORDER.get(e.severity, 9)))
        return out

    def totals_by_category(self):
        out = dict((c, 0) for c in Category.ALL)
        for e in self.events:
            out[e.category] = out.get(e.category, 0) + 1
        return out

    def totals_by_severity(self):
        out = {Severity.HIGH: 0, Severity.MED: 0, Severity.LOW: 0}
        for e in self.events:
            out[e.severity] = out.get(e.severity, 0) + 1
        return out

    def risk_score(self, file_events):
        """Higher = inspect sooner. Weighted by severity."""
        return sum(Severity.WEIGHT.get(e.severity, 0) for e in file_events)

    def file_risk_table(self):
        """[(file, risk, {category: count}, {severity: count}), ...] sorted worst-first."""
        rows = []
        for f, evs in self.by_file().items():
            cats = {}
            sevs = {}
            for e in evs:
                cats[e.category] = cats.get(e.category, 0) + 1
                sevs[e.severity] = sevs.get(e.severity, 0) + 1
            rows.append((f, self.risk_score(evs), cats, sevs))
        rows.sort(key=lambda r: (-r[1], r[0]))
        return rows

    # -- serialization -------------------------------------------------------
    def to_json(self):
        return json.dumps({
            'events': [e.as_dict() for e in self.events],
            'totals_by_category': self.totals_by_category(),
            'totals_by_severity': self.totals_by_severity(),
        }, indent=2)
