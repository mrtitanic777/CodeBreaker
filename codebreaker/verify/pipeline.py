#!/usr/bin/env python3
r"""pipeline.py -- the one behavioural-verification pipeline, for every lane.

For a set of functions of one retail module:

    synthesise C  ->  (skip if this exact input was verified before)  ->  compile  ->
    run retail and rebuilt on the oracle with the same inputs  ->  verdict + reason  ->
    bank the bodies that behave identically  ->  record every result

The 32-bit (PE) and 16-bit (NE) lanes used to be two separate copies of this sequence that had
drifted apart. The sequence now lives here once; a lane only supplies what really differs: how to
list functions, synthesise a body, compile, and take a fingerprint.

    from codebreaker.verify import pipeline, store
    res = pipeline.verify("pe", path, funcs, outdir, store=store.open())
    # res: {key: {"verdict", "fname", "reason", "cached"}}

Verdicts: equivalent (a clean return matched), consistent (matched, but only through an identical
fault or truncation), divergent, inconclusive (nothing could be observed), no-synth, no-compile.
A divergent body carries the first thing that differed as its reason: exit-differs, deps-differ
(the sequence of external calls) or ret-differs.
"""
import hashlib
import os
import sys

from codebreaker.base import paths
from codebreaker.base import period_tools as toolchain

PIPE_REV = 19                   # what a verdict means (13: one build per batch; bodies the compiler rejects are dropped by name)
MAX_SUPPORT = 1500              # out-of-batch callee bodies pulled in per batch
GOOD = ("equivalent", "consistent")


_PROFILES = {}


def profile(lane):
    """(digest, settings) -- everything OTHER than a body's own source that a verdict depends on:
    the pipeline and oracle revisions, the inputs each body is run on, the compiler and its flags.
    The digest is part of every body's signature, so changing any of these re-verifies the corpus
    without anyone having to remember to bump a number; the settings are stored beside it."""
    if lane not in _PROFILES:
        import json
        from codebreaker.build import watcom as B
        from codebreaker.verify import oracle as verify95
        cc = toolchain.ow_tool("wcc386" if lane == "pe" else "wcc")
        try:
            st = os.stat(cc)
            compiler = "%s:%d:%d" % (os.path.basename(cc), st.st_size, int(st.st_mtime))
        except OSError:
            compiler = ""
        settings = {
            "lane": lane, "pipeline": PIPE_REV, "oracle": verify95._ORACLE_REV,
            "inputs": [v["args"] for v in verify95._vectors(4, lane)],
            "confirm_inputs": [v["args"] for v in verify95._confirm_vectors(4, lane)],
            "max_calls": verify95._MAX_CALLS, "max_support": MAX_SUPPORT,
            "directed": [verify95.MAX_DIRECTED, verify95._HARVEST_DEPTH] if lane == "pe" else [],
            "cflags": list(B._CFLAGS) if lane == "pe" else [], "runtime": B._RTL if lane == "pe" else "",
            "compiler": compiler,
        }
        text = json.dumps(settings, sort_keys=True)
        _PROFILES[lane] = (hashlib.sha1(text.encode()).hexdigest()[:12], text)
    return _PROFILES[lane]


def _arm():
    from codebreaker.verify import oracle as verify95
    verify95._arm()
    tdir = paths.target_dir("win95")
    if tdir not in sys.path:
        sys.path.insert(0, tdir)                 # several win95 tools still import siblings bare


# =============================================================================================
# Lanes
# =============================================================================================
class PELane(object):
    """32-bit PE. Bodies are compiled together in batches, so a body that calls a batch-mate is
    verified against that mate's real reconstruction rather than a stub."""
    name = "pe"

    def functions(self, path):
        _arm()
        from codebreaker.formats import pe_util as pe32util
        return sorted(pe32util.carve(path).funcs)

    def key(self, va):
        return "%x" % va


LANES = {"pe": PELane()}


# =============================================================================================
# The pipeline
# =============================================================================================
def functions(lane, path):
    return LANES[lane].functions(path)


