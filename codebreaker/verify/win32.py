"""win32.py -- what is particular to reconstructing a 32-bit Windows module.

One of the kinds reconstruct.py works with (see there for what a kind provides). Here: the
module is decompiled by decompile/decompiler.py, built by build/functions.py, and
each function is run beside the original in the 32-bit emulator and compared by verify/oracle.py
(calls and their arguments, result, memory changed, stack, saved registers, coverage).
"""
import hashlib
import os
import sys
from codebreaker.base import settings as paths
from codebreaker.build import watcom as toolchain


KIND = "pe"


def profile(module_path):
    from codebreaker.verify import win32 as pipeline
    return pipeline.settings_profile("pe")


def prepare(module_path, work):
    from codebreaker.decompile import decompiler
    from codebreaker.verify import win32 as pipeline
    pipeline._arm()
    lane = pipeline.LANES["pe"]
    addrs = lane.functions(module_path)
    m = decompiler.decompile(module_path, addrs, work)
    m.key = {name: lane.key(va) for va, name in m.by_va.items()}
    m.va = {name: va for va, name in m.by_va.items()}
    m.all_keys = [lane.key(va) for va in addrs]
    # The module is one translation unit: any change to it may change any function's build.
    m.sig = hashlib.sha1(("%s\0%s\0%s" % (decompiler.REV, m.header, "".join(m.texts[n] for n in m.order)))
                         .encode("utf-8", "replace")).hexdigest()
    return m


def build(m, module_path, work):
    """-> (context for judge, {name: item}, {name: why it is not in the build})"""
    from codebreaker.build import functions
    from codebreaker.decompile import decompiler
    # Only functions with a known address are built. The rest are library routines recognised
    # by signature (__allshl, _memcpy): they belong to the C runtime, which is linked.
    placed = [n for n in m.order if n in m.va]
    dll, where, dropped = functions.build32(module_path, m.header, m.texts, placed, m.imports, work,
                                            decompiler.CFLAGS)
    items, dropped = {}, dict(dropped)
    for fn in placed:
        if fn in dropped:
            continue
        if fn not in where:
            dropped[fn] = "not in the build"
        else:
            items[fn] = (m.key[fn], m.va[fn], dll, where[fn])
    return module_path, items, dropped


def judge(module_path, item):
    """Worker: judge one function. item = (key, address, dll, entry)."""
    from codebreaker.verify import oracle as V
    from codebreaker.verify import win32 as pipeline
    pipeline._arm()
    _key, va, dll, entry = item
    info = {}
    try:
        verdict, reason, limits = V.compare(
            lambda v: V._fp_pe(module_path, va, v),
            lambda v: V._fp_pe(dll, entry, v, rebuilt=module_path),
            V._vectors(4, "pe"), V._confirm_vectors(4, "pe"),
            insns=V.insn_count(module_path, va),
            directed=lambda cov, rch: V.directed_vectors(module_path, va, cov, reached=rch),
            info=info)
    except ImportError:
        raise
    except Exception as e:                                  # one function must not sink the rest
        verdict, reason, limits = "inconclusive", "judge: %s" % type(e).__name__, []
    return verdict, reason, list(limits), info.get("detail", "")


PIPE_REV = 19                   # what a verdict means (13: one build per batch; bodies the compiler rejects are dropped by name)
MAX_SUPPORT = 1500              # out-of-batch callee bodies pulled in per batch
GOOD = ("equivalent", "consistent")


_PROFILES = {}


def settings_profile(lane):
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
        from codebreaker.formats import pe_functions as pe32util
        return sorted(pe32util.carve(path).funcs)

    def key(self, va):
        return "%x" % va


LANES = {"pe": PELane()}


# =============================================================================================
# The pipeline
# =============================================================================================
def functions(lane, path):
    return LANES[lane].functions(path)
