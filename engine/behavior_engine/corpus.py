#!/usr/bin/env python3
r"""corpus.py -- the behavioral engine's durable evidence store.

The spec's central idea is that observations must persist as RAW evidence, because a later
experiment may prove an earlier interpretation wrong (so we keep the observation, not only
its reading). Equally central: raw evidence and DERIVED information must be architecturally
separated, so derived data (hypotheses, graphs, inferred structures, reconstructions) can
always be regenerated from the raw observations without re-running the original software.

    <data>/behavior/
        raw/
            traces.jsonl             APPEND-ONLY, IMMUTABLE log of every trace ever recorded.
                                     This is the source of truth -- never rewritten in place.
            <module>/<func_va>.json  the latest raw trace per function (a convenience index
                                     INTO the raw log -- still pure observation, no inference).
        derived/                     reserved for regenerable interpretation: hypotheses,
                                     dependency/execution graphs, function roles, candidate
                                     source. Empty in v1; everything here is rebuildable from
                                     raw/, so it may be deleted and regenerated at will.

<data> is WT_DATA_ROOT (the win95 data context the `wt behavior` dispatcher establishes),
so the corpus lives alongside the rest of the win95 reconstruction data and accumulates
indefinitely across sessions.
"""
import json
import os


def corpus_dir():
    root = os.environ.get("WT_DATA_ROOT") or os.getcwd()
    d = os.path.join(root, "behavior")
    os.makedirs(d, exist_ok=True)
    return d


def raw_dir():
    d = os.path.join(corpus_dir(), "raw")
    os.makedirs(d, exist_ok=True)
    return d


def derived_dir():
    """Reserved for regenerable interpretation. Created lazily so its mere existence signals
    that a later slice (graph/hypotheses/synthesis) has run."""
    d = os.path.join(corpus_dir(), "derived")
    os.makedirs(d, exist_ok=True)
    return d


def _func_path(rec):
    d = os.path.join(raw_dir(), os.path.basename(rec["module"]))
    os.makedirs(d, exist_ok=True)
    return os.path.join(d, "0x%x.json" % rec["func_va"])


def record(rec):
    """Persist a TraceRecord: append to the immutable raw log and refresh the per-function
    latest index. Returns the path of the per-function file."""
    import time
    rec = dict(rec)
    rec.setdefault("recorded_at", time.strftime("%Y-%m-%dT%H:%M:%S"))
    with open(os.path.join(raw_dir(), "traces.jsonl"), "a", encoding="utf-8") as fh:
        fh.write(json.dumps(rec) + "\n")                     # append-only: raw evidence is never edited
    fp = _func_path(rec)
    with open(fp, "w", encoding="utf-8") as fh:
        json.dump(rec, fh, indent=1)
    return fp


def load(module, func_va):
    """Load the latest stored trace for a function, or None."""
    fp = os.path.join(raw_dir(), module, "0x%x.json" % int(func_va))
    if not os.path.exists(fp):
        return None
    with open(fp, encoding="utf-8") as fh:
        return json.load(fh)


def load_by_selector(module, func=None):
    """Resolve a `show`-style selector to a stored trace. func may be a VA/offset hex or None
    (the only stored trace for the module, if unambiguous)."""
    d = os.path.join(raw_dir(), module)
    if not os.path.isdir(d):
        return None
    files = sorted(f for f in os.listdir(d) if f.endswith(".json"))
    if not files:
        return None
    if func is None:
        # unambiguous single, else the lowest VA
        target = files[0]
    else:
        val = int(func, 0) if isinstance(func, str) else int(func)
        cand = "0x%x.json" % val
        target = cand if cand in files else None
        if target is None:                       # maybe they passed a section offset, not a VA
            for f in files:
                rec = load(module, int(f[:-5], 16))
                if rec and rec.get("func_off") == val:
                    return rec
            return None
    with open(os.path.join(d, target), encoding="utf-8") as fh:
        return json.load(fh)
