#!/usr/bin/env python3
r"""verify95.py -- step 2.2: the verification oracle. A reconstructed body is CORRECT only when it
BEHAVES like the original, proven on the Unicorn oracle -- not merely when it compiles. This is the
definition of "done" for the full reconstruction and the throughput gate for the whole program.

For a function it runs the ORIGINAL and the REBUILT under the same oracle with identical inputs and
compares a codegen-INDEPENDENT behavioral fingerprint: (return value, clean-vs-fault exit, and the
sequence of imported API calls). Instruction path and count are ignored -- MSC-vs-OpenWatcom codegen
differs there without any behavioral difference. PE/DOS bodies run on the behavior-engine tracer;
NE (16-bit) bodies on ne_dynoracle. Divergence on identical inputs is a real reconstruction defect.

    wt --target win95 verify95 <retail> <rebuilt> [--arity N] [--max N]   # pair-verify two modules
    wt --target win95 verify95 --ledger                                   # batch: verify every body
                                                                          # whose module is rebuilt,
                                                                          # update body_ledger.csv

No fabrication: a fingerprint records only what the oracle observed; a trace error is recorded as
itself, never scored as a pass.
"""
import os

from codebreaker.base import registry as cores
from codebreaker.base import settings as paths
from codebreaker.formats.pe import open_pe
from codebreaker.emulate import trace32 as tracer

# compiler/CRT-runtime imports the toolchain emits (not the body's own logic) -- ignored so
# MSC-vs-OpenWatcom startup noise is not scored as a behavioral divergence.
_TOOLCHAIN = {"KERNEL.GETVERSION", "KERNEL.GETWINFLAGS", "KERNEL.LOCALINIT", "KERNEL.GETVERSION16",
              "KERNEL32.GETVERSION", "KERNEL32.GETVERSIONEXA", "KERNEL32.GETCURRENTTHREADID"}


def _norm_dep(d):
    """Canonicalise an imported-call name so the two builds' import-table SPELLINGS of the same
    import compare equal: uppercase, drop a .DLL/.EXE on the module half, unify the separator.
    ('KERNEL32.dll!#21' and 'KERNEL32!#21' are the same ordinal import -> same behavior)."""
    s = str(d).upper().replace("!", ".").replace(":", ".")
    parts = s.split(".")
    parts = [p for p in parts if p not in ("DLL", "EXE", "DRV")]
    return ".".join(parts)


def _is_noise(api):
    """Toolchain/thunk imports that differ by CODEGEN (OpenWatcom vs MS), not by the body's logic --
    excluded from the behavioral fingerprint so they don't read as a divergence."""
    a = str(api).upper()
    if a in _TOOLCHAIN:
        return True
    return ("SMAPLS" in a or "SUNMAPLS" in a or "FT_" in a or "QT_" in a or "THUNK" in a
            or a.endswith(".@0") or "GETVERSION" in a or "GETWINFLAGS" in a)


def _argc(dll, name):
    """Argument count of an imported function, for the oracle's import stub: read from the retail
    module that exports it (apiarity), else the small hand table. None = unknown (the stub then
    removes nothing and the call's arguments are not compared)."""
    from codebreaker.symbols import api_arguments as apiarity
    from codebreaker.emulate import trace32 as env
    n = apiarity.argc(dll, name)
    return n if n is not None else env._builtin_argc(name)


def _norm_arg(v, ins, own=None):
    """An argument value in a form two builds of the same code agree on: which input it is, a small
    constant, a stack address ('S') or an address inside the rebuilt image ('p') -- those two
    differ by build -- and otherwise the value itself. Reconstructed code refers to the original's
    data by its literal address, so a pointer into the original image is the same number on both
    sides and is compared as one: passing the wrong string or the wrong table is a difference."""
    if v in ins:
        return "a%d" % ins.index(v)
    sv = v - (1 << 32) if v & 0x80000000 else v
    if -0x10000 < sv < 0x10000:
        return "%d" % sv
    # A rebuilt body's locals live in its stand-in frame, inside its own image: like a stack
    # address, that is "somewhere in this call's frame".
    if _STACK_LO <= v < _STACK_HI or (own and own[0] <= v < own[1]):
        return "S"
    return "%x" % v


_MAX_CALLS = 64


def _call_seq(calls, ins, own=None):
    """The imported-call part of a fingerprint: every call in order, with normalised arguments."""
    out = []
    for api, args in calls:
        if _is_noise(api):
            continue
        if len(out) >= _MAX_CALLS:
            out.append("...")
            break
        out.append(_norm_dep(api) if args is None else
                   "%s(%s)" % (_norm_dep(api), ",".join(_norm_arg(a, ins, own) for a in args)))
    return tuple(out)


def _exit_norm(e):
    if e is None:
        return "none"
    e = str(e)
    return "ret" if e == "ret" else ("fault" if e.startswith("exception") else e.split(":")[0])


import struct as _struct

# provisioned environment (mirrors envprov): a region of pointers that all resolve to a safe RET, so
# a pointer/handle argument that gets dereferenced OR called through lands on a clean return instead
# of a null fault. Both regions live inside the tracer's pre-mapped scratch window (0x200000..0x210000).
_OBJ, _SAFE = 0x206000, 0x207800


def _prov_meminit():
    return [(_OBJ, _struct.pack("<I", _SAFE) * 0x400),   # 4 KB of "-> _SAFE" pointers
            (_SAFE, b"\xc3" * 16)]                        # RET landing pad


# --- behavioral-fingerprint cache -------------------------------------------------------------
# A PE fingerprint is a pure function of (module bytes, func addr, input vector): the trace is
# deterministic given those. The RETAIL side (the corpus binary) is reconstruction-independent, so
# its fingerprint is identical across every body and every sweep re-run -- yet it was re-traced each
# time, making it ~half of the oracle cost (4 of 8 traces/body). Cache fingerprints keyed by the
# module's content hash so a re-run (the common case after any synth/lifter change) skips the retail
# traces entirely. Keying on content hash (not path) makes it correct for both sides and for shared
# bodies: a rebuilt DLL gets a fresh hash each build, so it never stales. Disabled by CB_NO_FP_CACHE.
import hashlib as _hashlib
import pickle as _pickle
import sqlite3 as _sqlite3

_SHA_MEMO = {}
_FP_DB = None


def _fp_cache_path():
    from codebreaker.base import settings
    return settings.state("fingerprints.sqlite")


def _fp_db():
    """Open (once per process) the WAL-mode sqlite fp cache, or False if it cannot be used. WAL lets
    the 10 sweep workers read concurrently; a write that loses the lock is swallowed (re-traced next
    time), so the cache never affects correctness."""
    global _FP_DB
    if _FP_DB is None:
        try:
            db = _sqlite3.connect(_fp_cache_path(), timeout=5.0)
            db.execute("PRAGMA journal_mode=WAL")
            db.execute("PRAGMA synchronous=NORMAL")
            db.execute("CREATE TABLE IF NOT EXISTS fp(k TEXT PRIMARY KEY, v BLOB)")
            db.commit()
            _FP_DB = db
        except Exception:
            _FP_DB = False
    return _FP_DB or None


def _mod_sha(path):
    """sha256 of a module file, memoized by (path, mtime, size) so a stable corpus binary is hashed
    once per process."""
    try:
        st = os.stat(path)
    except OSError:
        return None
    key = (path, int(st.st_mtime), st.st_size)
    s = _SHA_MEMO.get(key)
    if s is None:
        h = _hashlib.sha256()
        try:
            with open(path, "rb") as f:
                for chunk in iter(lambda: f.read(1 << 20), b""):
                    h.update(chunk)
        except OSError:
            return None
        s = h.hexdigest()
        _SHA_MEMO[key] = s
    return s


# Bump when the oracle changes what a trace of the SAME code observes, so fingerprints cached under
# the old behaviour are not reused. 2: every PE code section is mapped (calls into another code
# section now run instead of being stub-returned). 3: the import stub removes the arguments a
# stdcall import takes, and the fingerprint is the full call sequence WITH arguments, not the set of
# import names reached. 4: the fingerprint also carries the memory the function wrote and which of
# its instructions ran; inputs can set what imported calls return.
_ORACLE_REV = 16


def _fp_key(sha, func, spec):
    args = tuple(int(a) for a in spec.get("args", []))
    mi = tuple((int(a), bytes(b)) for a, b in (spec.get("mem_init") or []))
    return "r%d:%s:%s" % (_ORACLE_REV, sha, _hashlib.sha1(
        repr((func, args, mi, int(spec.get("stub_ret", 0))) + (
            (tuple(spec["sentinels"]),) if spec.get("sentinels") else ())).encode()).hexdigest())


def _fp_cache_get(key):
    db = _fp_db()
    if not db:
        return None
    try:
        row = db.execute("SELECT v FROM fp WHERE k=?", (key,)).fetchone()
        return _pickle.loads(row[0]) if row else None
    except Exception:
        return None


def _fp_cache_put(key, fp):
    db = _fp_db()
    if not db:
        return
    try:
        db.execute("INSERT OR REPLACE INTO fp(k,v) VALUES(?,?)", (key, _pickle.dumps(fp)))
        db.commit()
    except Exception:
        pass


def _arm():
    """The oracle resolves its engines through the active target. Called from a library (not
    through wt) nothing had set one, every trace failed inside the registry, and the failure
    was reported as an 'inconclusive' verdict. Default to win95 when no target is active."""
    cores.ensure_target(paths.target_dir("win95"))


# The oracle's stack (and the RET landing pad just under it) sits at the top of the scratch region.
# Writes there are the function's own frame, laid out differently by every compiler.
_STACK_LO, _STACK_HI = tracer._STACK_BASE, tracer._STACK_BASE + tracer._STACK_SIZE
_IMAGE_SPAN = {}
_INSN_COUNT = {}


def _image_span(path):
    """(lo, hi) addresses of a PE image as loaded."""
    if path not in _IMAGE_SPAN:
        try:
            pe = open_pe(path)
            top = max([s["vaddr"] + max(s["vsize"], s["rawsize"]) for s in pe.sections] or [0])
            _IMAGE_SPAN[path] = (pe.image_base, pe.image_base + top)
        except Exception:
            _IMAGE_SPAN[path] = (0, 0)
    return _IMAGE_SPAN[path]


def _effects(r, own):
    """What the function left in memory outside its own frame: a digest of the final value of every
    byte it wrote. A function that stores to a global or through a pointer argument is judged on
    what it stored and where -- the return value alone says nothing about either. A byte left
    holding what it held on entry is not an effect: one compiler clears a flag with a 16-bit
    `and`, another with an 8-bit one, and memory ends up the same.

    `own` is the address range of a REBUILT image: writes into it are that build's own statics
    (compiler temporaries, register stand-ins), which the original has no counterpart for. The
    original's globals are reached by their literal addresses on both sides, so they compare.
    A stored stack address is recorded as such (its value differs by build). None = too many
    writes were made to record them all."""
    ws = r.get("mem_writes") or []
    if len(ws) >= tracer._MAX_MEM:
        return None
    # A call through a pointer that resolves to nothing lands in zero bytes; the oracle returns
    # from it after one `add [eax], al`. That write is the harness's, not the function's.
    mn = r.get("mnemonics") or {}
    state, before = {}, {}
    for w in ws:
        a, n, v = w["addr"], w["size"], w["value"] or 0
        for i in range(min(n, 8)):
            before.setdefault(a + i, ((w.get("old") or 0) >> (8 * i)) & 0xFF if "old" in w else None)
        if mn.get("0x%x" % w["pc"]) == "add byte ptr [eax], al":
            continue
        if _STACK_LO <= a < _STACK_HI or (own and own[0] <= a < own[1]):
            continue
        if n == 4 and (_STACK_LO <= v < _STACK_HI or (own and own[0] <= v < own[1])):
            for i in range(4):
                state[a + i] = "S"
        else:
            for i in range(min(n, 8)):
                state[a + i] = (v >> (8 * i)) & 0xFF
    state = {k: x for k, x in state.items() if x != before.get(k)}
    if not state:
        return ""
    return "%d:%s" % (len(state), _hashlib.sha1(repr(sorted(state.items())).encode()).hexdigest()[:16])


_MAX_REACH = 6000


def _coverage(r):
    """(own, reach): offsets of the function's own instructions that ran, and the addresses of
    every instruction that ran anywhere in the module (the function and the callees it entered)."""
    lo, hi = r.get("func_va") or 0, r.get("func_end_va") or 0
    own, reach = [], []
    for k in r.get("mnemonics") or ():
        a = int(k, 16) if isinstance(k, str) else k
        reach.append(a)
        if lo <= a < hi:
            own.append(a - lo)
    return tuple(sorted(own)), tuple(sorted(reach)[:_MAX_REACH])


def insn_count(path, func):
    """How many instructions the function at `func` has (linear decode of its extent), or 0."""
    key = (path, func)
    if key not in _INSN_COUNT:
        n = 0
        try:
            off, va0, code, end, _sec = tracer._resolve_func(path, func)
            import capstone
            md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
            n = sum(1 for _ in md.disasm_lite(bytes(code[off:end]), va0 + off))
        except Exception:
            n = 0
        _INSN_COUNT[key] = n
    return _INSN_COUNT[key]


def _fp_pe(path, func, spec, rebuilt=False):
    """Fingerprint one run. For a REBUILT image pass `rebuilt=<path of the original>`: its own
    statics are then left out of the comparison and the original's code is mapped beside it."""
    _arm()
    key = None
    if not os.environ.get("CB_NO_FP_CACHE"):
        sha = _mod_sha(path)
        if sha:
            key = _fp_key(sha, func, spec)
            hit = _fp_cache_get(key)
            if hit is not None:
                return hit
    try:
        r = tracer.trace_function(path, func=func, args=spec["args"], mem_init=spec.get("mem_init"),
                                  stub_externals=True, sig_resolver=_argc,
                                  stub_ret=int(spec.get("stub_ret", 0)),
                                  overlay=rebuilt if isinstance(rebuilt, str) else None,
                                  sentinels=spec.get("sentinels"))
    except ImportError:
        raise                                     # a broken setup is not an observation
    except Exception as e:
        return {"err": type(e).__name__}          # transient trace errors are never cached
    # A call that landed in zero bytes went through a pointer nothing in this environment fills
    # in (a 32->16 thunk entry the loader would set, an uninitialised callback). What the ORIGINAL
    # does after that is the harness's doing, not the function's: the run is not an observation.
    # A REBUILT body that does it where the original did not has simply gone somewhere wrong.
    wild = bool(r.get("bogus_stubs") or r.get("ran_into_zeros"))
    if wild and not rebuilt:
        return {"err": "call-into-nothing"}
    deps = _call_seq(r.get("api_calls", []), [int(a) for a in spec["args"]],
                     _image_span(path) if rebuilt else None)
    fp = {"ret": r.get("ret"), "exit": "wild-call" if wild else _exit_norm(r.get("exit")), "deps": deps,
          "truncated": bool(r.get("truncated")),
          "writes": _effects(r, _image_span(path) if rebuilt else None),
          # how the function left the stack pointer (4 = the caller removes the arguments, 4 + N =
          # the function removed N bytes of them) and whether it preserved EBX/ESI/EDI/EBP
          "pop": r.get("esp_delta") if _exit_norm(r.get("exit")) == "ret" else None,
          "saved": (tuple(r.get("saved_regs") or ()) == tuple(spec.get("sentinels") or tracer.SAVED_SENTINELS)
                    if _exit_norm(r.get("exit")) == "ret" else None),
          "cov": (), "reach": ()}
    if not rebuilt:
        fp["cov"], fp["reach"] = _coverage(r)
    # Cache only settled results. A truncated trace can differ by wall-clock (the tracer's 2s cap),
    # so caching it could record a machine-dependent fingerprint -- re-trace those instead.
    if key and not fp["truncated"]:
        _fp_cache_put(key, fp)
    return fp


def _vectors(arity, lane):
    """Input vectors to exercise a function past the immediate null-fault (no per-function signatures
    needed). Each is {args, mem_init}. Beyond zeros/handles, the PROVISIONED vector points every arg
    at a valid object region whose contents resolve dereference/call chains to a safe RET -- so a
    pointer-taking function runs its real path to completion instead of faulting (env-provisioned)."""
    if arity <= 0:
        return [{"args": []}]
    n = arity
    # Every argument gets a DIFFERENT value wherever the environment allows it: with one value in
    # every slot, a body that uses its parameters in the wrong order behaves identically.
    vs = [{"args": [0] * n}]
    if lane in ("pe", "dos"):
        vs.append({"args": [0x200000 + 0x100 * i for i in range(n)]})        # bare scratch pointers
        vs.append({"args": [_OBJ + 0x40 * (i % 32) for i in range(n)],        # provisioned objects
                   "mem_init": _prov_meminit()})
    else:                                                        # NE controlled env: valid handles
        vs.append({"args": [0x0400] * n})                        # LZ handle base
        vs.append({"args": [0x0080] * n})                        # open-file handle
    vs.append({"args": [i + 1 for i in range(n)]})
    return vs


def _confirm_vectors(arity, lane):
    """More inputs, run only for a body that already matched on every basic vector. Four inputs
    are few; these add boundary integers, a reversed ordering and mixes of pointers and integers,
    so a body is not called verified on the strength of four lucky inputs."""
    if arity <= 0 or lane not in ("pe", "dos"):
        return []
    n = arity
    mi = _prov_meminit()
    obj = [_OBJ + 0x40 * (i % 32) for i in range(n)]
    edge = [0x7FFFFFFF, 0x80000000, 0x10000, 0xFFFF, 0xFF, 0x100, 0x8000, 0x7FFF]
    ints = [i + 1 for i in range(n)]
    # Objects whose fields are not pointers: all-ones (every flag bit set, counts large) and small
    # counts. A field tested with `& 0x40` or compared with an index is false on the pointer-filled
    # object above, so everything behind such a test never ran.
    ones = [(_OBJ, b"\xff" * 0x1000), (_SAFE, b"\xc3" * 16)]
    small = [(_OBJ, _struct.pack("<I", 3) * 0x400), (_SAFE, b"\xc3" * 16)]
    return [
        # Imported calls return 0 by default, so the code after `if (!CreateThing()) return` never
        # ran. Run again with imports succeeding: returning 1, and returning a valid object.
        {"args": obj, "mem_init": mi, "stub_ret": 1},
        {"args": obj, "mem_init": mi, "stub_ret": _OBJ + 0x800},
        {"args": ints, "stub_ret": 1},
        {"args": obj, "mem_init": ones, "stub_ret": 1},
        {"args": obj, "mem_init": small, "stub_ret": 1},
        {"args": obj, "mem_init": small},
        {"args": [0xFFFFFFFF - i for i in range(n)]},                          # small negatives
        {"args": [edge[i % len(edge)] for i in range(n)]},                     # boundary integers
        {"args": [n - i for i in range(n)]},                                   # distinct, reversed
        {"args": [obj[0]] + [i + 2 for i in range(1, n)], "mem_init": mi},     # pointer, then integers
        {"args": [5] + obj[1:], "mem_init": mi},                               # integer, then pointers
        {"args": [obj[i] if i % 2 else 0x100 + i for i in range(n)], "mem_init": mi},
    ]


_HARVEST = {}
MAX_DIRECTED = 12                # directed inputs kept per body
_HARVEST_DEPTH = 2               # how far into callees to look for what branches test
_HARVEST_FUNCS = 12


def _harvest(path, func):
    """(constants, globals) read off the function's own instructions: the immediates it compares
    or masks with, and the absolute data addresses it reads. These are what its branches test, so
    they are what an input has to contain to get past them."""
    key = (path, func)
    if key not in _HARVEST:
        consts, globs = [], []
        try:
            import capstone
            from capstone import x86_const as X
            lo, hi = _image_span(path)
            iat = set(open_pe(path).imports())
            md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
            md.detail = True
            # The function first, then the functions it calls: a caller often cannot get past
            # `if (!helper(x)) return` until the input satisfies what the HELPER tests.
            todo, seen = [(func, 0)], set()
            while todo and len(seen) < _HARVEST_FUNCS:
                f, depth = todo.pop(0)
                if f in seen:
                    continue
                seen.add(f)
                try:
                    off, va0, code, end, _sec = tracer._resolve_func(path, f)
                except Exception:
                    continue
                for ins in md.disasm(bytes(code[off:end]), va0 + off):
                    if ins.mnemonic == "call" and depth < _HARVEST_DEPTH and ins.operands and \
                            ins.operands[0].type == X.X86_OP_IMM:
                        todo.append((ins.operands[0].imm & 0xFFFFFFFF, depth + 1))
                        continue
                    for op in ins.operands:
                        if op.type == X.X86_OP_IMM and ins.mnemonic in ("cmp", "test", "sub", "and"):
                            v = op.imm & 0xFFFFFFFF
                            if v and v not in consts:
                                consts.append(v)
                        elif op.type == X.X86_OP_MEM and not op.mem.base and not op.mem.index:
                            a = op.mem.disp & 0xFFFFFFFF
                            if lo <= a < hi and a not in iat and a not in globs and ins.mnemonic != "lea":
                                globs.append(a)
        except Exception:
            pass
        _HARVEST[key] = (consts[:10], globs[:24])
    return _HARVEST[key]


def directed_vectors(path, func, covered, arity=4, reached=()):
    """Inputs chosen for THIS function. Generic inputs stop at the first guard (`if (msg != 0x4E)
    return`, `if (!g_initialised) return`), so most of a body never ran and was 'verified' on its
    first few instructions. Candidates put each constant the function compares against into each
    argument, and set the globals it reads to non-zero values; a candidate is kept only if the
    ORIGINAL then runs instructions no earlier input reached. Returns at most MAX_DIRECTED."""
    consts, globs = _harvest(path, func)
    if not consts and not globs:
        return []
    n = arity
    mi = _prov_meminit()
    obj = [_OBJ + 0x40 * (i % 32) for i in range(n)]
    ints = [i + 1 for i in range(n)]
    cands = []
    gsets = [[(a, _struct.pack("<I", val)) for a in globs] for val in (1, _SAFE, _OBJ + 0x800)] if globs else []
    for g in gsets:
        cands.append({"args": obj, "mem_init": mi + g, "stub_ret": 1})
        cands.append({"args": ints, "mem_init": mi + g})
    for k in consts:
        for i in range(n):
            a = list(obj)
            a[i] = k
            cands.append({"args": a, "mem_init": mi, "stub_ret": 1})
            if gsets:
                cands.append({"args": a, "mem_init": mi + gsets[1], "stub_ret": 1})
        cands.append({"args": [k] * n, "stub_ret": 1})
        # ...and into every field of the objects the arguments point at (`if (p->flags & 0x40)`).
        fill = [(_OBJ, _struct.pack("<I", k) * 0x400), (_SAFE, b"\xc3" * 16)]
        cands.append({"args": obj, "mem_init": fill, "stub_ret": 1})
        cands.append({"args": obj, "mem_init": fill})
    # "New code" is counted over everything that ran, callees included: an input that drives a
    # helper down a different path exercises the caller's use of it even at 100% own coverage.
    keep, seen = [], set(reached)
    for v in cands:
        f = _fp_pe(path, func, v)
        if "err" in f:
            continue
        new = set(f.get("reach") or ()) - seen
        if new:
            keep.append(v)
            seen |= new
            if len(keep) >= MAX_DIRECTED:
                break
    return keep


# The saved registers hold these instead when asking which bits of a return value are leftovers.
_ALT_SENTINELS = tuple(x ^ 0xFFFFFFFF for x in tracer.SAVED_SENTINELS)


def undefined_ret_bits(fp_retail, v, a):
    """Bits of the original's return value that are not a result at all.

    A function returning a 16- or 8-bit value sets AX or AL and leaves whatever was in the rest of
    EAX -- often a copy of a register the caller owns. Run the original again with those registers
    holding the opposite bits: every bit of the return value that flips is such a leftover. Only
    whole upper parts count (the low byte must stay put), so a real result is never excused."""
    if a.get("exit") != "ret" or a.get("ret") is None:
        return 0
    c = fp_retail(dict(v, sentinels=_ALT_SENTINELS))
    if "err" in c or c.get("exit") != "ret" or c.get("ret") is None or c.get("deps") != a.get("deps"):
        return 0
    flips = (a["ret"] ^ c["ret"]) & 0xFFFFFFFF
    for mask in (0xFFFF0000, 0xFFFFFF00):
        if flips and not flips & ~mask:
            return mask
    # the whole upper half follows the caller's registers and the low half moves too: the
    # function returns nothing, and EAX is simply what its last instructions left there
    if flips & 0xFFFF0000 == 0xFFFF0000 and flips & 0xFFFF:
        return 0xFFFFFFFF
    return 0


def first_difference(a, b, ret_mask=0):
    """The first observable difference between two fingerprints of the same input, or ''.
    `ret_mask` marks bits of the return value that are leftovers, not results."""
    if a["exit"] != b["exit"]:
        return "exit-differs"
    if a["deps"] != b["deps"]:
        return "deps-differ"
    # EAX is a return value only when the function returned: at a fault it holds whatever the
    # compiler happened to keep there, which says nothing about the function.
    if a["exit"] == "ret" and ((a["ret"] or 0) ^ (b["ret"] or 0)) & ~ret_mask & 0xFFFFFFFF:
        return "ret-differs"
    # A run cut off at the step limit stopped in the middle of something: the two builds spend
    # different numbers of instructions per loop pass, so memory at that instant is not comparable.
    if a.get("truncated") or b.get("truncated"):
        return ""
    wa, wb = a.get("writes"), b.get("writes")
    if wa is not None and wb is not None and wa != wb:
        return "writes-differ"
    if a.get("pop") is not None and b.get("pop") is not None and a["pop"] != b["pop"]:
        return "stack-differs"                      # a different calling convention or argument count
    if a.get("saved") and b.get("saved") is False:
        return "registers-differ"                   # the original preserves them; this does not
    return ""


def difference_detail(a, b, v=None, ret_mask=0):
    """One line saying exactly what differed between two fingerprints, for triage by cause:
    which call first differs and how, or the two return values / exits."""
    d = first_difference(a, b, ret_mask)
    if d == "exit-differs":
        out = "exit %s vs %s" % (a["exit"], b["exit"])
    elif d == "ret-differs":
        out = "ret %s vs %s" % (_norm_arg(a["ret"] or 0, v["args"] if v else ()),
                                _norm_arg(b["ret"] or 0, v["args"] if v else ()))
    elif d == "writes-differ":
        out = "writes %s vs %s" % ((a.get("writes") or "none").split(":")[0],
                                   (b.get("writes") or "none").split(":")[0])
    elif d == "stack-differs":
        out = "removes %s bytes on return vs %s" % (a.get("pop"), b.get("pop"))
    elif d == "registers-differ":
        out = "saved registers preserved vs changed"
    elif d == "deps-differ":
        da, db = list(a["deps"]), list(b["deps"])
        i = 0
        while i < len(da) and i < len(db) and da[i] == db[i]:
            i += 1
        ra = da[i] if i < len(da) else "(nothing)"
        rb = db[i] if i < len(db) else "(nothing)"
        na, nb = ra.split("(")[0], rb.split("(")[0]
        if rb == "(nothing)":
            kind = "rebuilt stops calling"
        elif ra == "(nothing)":
            kind = "rebuilt calls extra"
        elif na != nb:
            kind = "different call"
        else:
            aa, ab = ra[len(na):].strip("()").split(","), rb[len(nb):].strip("()").split(",")
            bad = [str(k) for k in range(min(len(aa), len(ab))) if aa[k] != ab[k]]
            kind = "same call, argument %s differs" % "+".join(bad)
        out = "call #%d %s: %s vs %s" % (i, kind, ra, rb)
    else:
        return ""
    if v is not None:
        out += "  [input %s%s]" % (",".join("%x" % x for x in v["args"]),
                                    " ret=%x" % v["stub_ret"] if v.get("stub_ret") else "")
    return out[:300]


def compare(fp_retail, fp_rebuilt, vectors, confirm=(), insns=0, directed=None, info=None):
    """Judge one body. `fp_retail(v)` / `fp_rebuilt(v)` fingerprint each side on input `v`.
    Returns (verdict, reason, limits). With `insns` (the original function's instruction count),
    the share of them that ran on some input is appended to limits as `coverage=NN`.

    Stops at the first input on which the two sides differ -- one difference settles it, and most
    bodies that fail do so on the first input. A body that matches every basic input with at least
    one clean return is then run on the `confirm` inputs too.

    `limits` are the reasons a match is weaker than it looks, as codes that can be counted:
      fault-only      no input returned cleanly; the two sides only fault/stop alike
      truncated       a trace hit the step or time cap
      argc-unknown    an imported call was compared by name only (its argument count is unknown)
      calls-capped    the call sequence was longer than is compared
      unobserved      some inputs could not be traced on one side
      writes-capped   a trace wrote more memory than is recorded, so its writes were not compared
      low-coverage    under half of the original's instructions ran on any input
    """
    limits = set()
    covered = set()
    reached = set()
    seen = clean = 0
    errs = set()

    def run(vs):
        nonlocal seen, clean
        for v in vs:
            a = fp_retail(v)
            b = fp_rebuilt(v)
            if "err" in a or "err" in b:
                limits.add("unobserved")
                if "err" in a:
                    errs.add("retail: " + a["err"])
                if "err" in b:
                    errs.add("rebuilt: " + b["err"])
                continue
            seen += 1
            d, mask = first_difference(a, b), 0
            if d == "ret-differs":
                mask = undefined_ret_bits(fp_retail, v, a)
                if mask:
                    d = first_difference(a, b, mask)
                    if info is not None:
                        info["ret_bits"] = {0xFFFF0000: 16, 0xFFFFFF00: 8}.get(mask, 0)
            if d:
                if info is not None:
                    info["detail"] = difference_detail(a, b, v, mask)
                return d
            covered.update(a.get("cov") or ())
            reached.update(a.get("reach") or ())
            if a.get("writes") is None or b.get("writes") is None:
                limits.add("writes-capped")
            if a.get("truncated") or b.get("truncated"):
                limits.add("truncated")
            elif a.get("exit") == "ret":
                clean += 1
            for dep in a.get("deps", ()):
                if dep == "...":
                    limits.add("calls-capped")
                elif "(" not in dep:
                    limits.add("argc-unknown")
        return ""

    d = run(vectors)
    if d:
        return "divergent", d, []
    if not seen:
        return "inconclusive", "; ".join(sorted(errs)), sorted(limits)
    if clean and confirm:
        d = run(confirm)
        if d:
            return "divergent", d, []
    # `directed(covered, reached)` supplies inputs aimed at code no input has reached yet.
    if directed and clean:
        d = run(directed(covered, reached))
        if d:
            return "divergent", d, []
    if insns:
        pct = min(100, 100 * len(covered) // insns)
        if pct < 50:
            limits.add("low-coverage")
        limits.add("coverage=%d" % pct)
    if not clean:
        limits.add("fault-only")
        return "consistent", "", sorted(limits)
    return "equivalent", "", sorted(limits)
