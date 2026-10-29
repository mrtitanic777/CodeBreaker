"""win32.py -- what is particular to reconstructing a 32-bit Windows module.

One of the kinds reconstruct.py works with (see there for what a kind provides). Here: the
module is decompiled by decompile/decompiler.py, built by build/functions.py, and
each function is run beside the original in the 32-bit emulator (emulate/emulator.py) and compared by verify/compare.py
(calls and their arguments, result, memory changed, stack, saved registers, coverage).

What runs a function, how it is called, its compiler's options and what builds the rebuild are
the platform's (codebreaker/platforms/win32.py): its emulator, convention, compiler and packager.
"""
import hashlib
import os
from codebreaker import platforms
from codebreaker.base import corpus
from codebreaker.verify import compare
from codebreaker.verify import lane_common
from codebreaker.formats.pe import open_pe
from codebreaker.emulate import emulator as tracer
import struct as _struct
import hashlib as _hashlib


KIND = "pe"
P = platforms.of(KIND)          # 32-bit Windows: the six slots this lane asks
ARITY = P.convention.arguments  # the arguments every function is given (its count is not known)
PER_FUNCTION = True             # a verdict holds while the function and what it reaches are unchanged


def profile(module_path):
    return settings_profile("pe")


def prepare(module_path, work):
    from codebreaker.decompile import decompiler
    lane = LANES["pe"]
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
    # Only functions with a known address are built. The rest are library routines recognised
    # by signature (__allshl, _memcpy): they belong to the C runtime, which is linked.
    placed = [n for n in m.order if n in m.va]
    # A flat thunk's stubs are the thunk compiler's, not C: not rebuilt, so a call to one goes to
    # the original's (build32: a jump to it), which the judge enters as an import (_thunk_stubs).
    stubs = _thunk_stubs(module_path)[0]
    placed = [n for n in placed if m.va[n] not in stubs]
    from codebreaker.decompile import describe
    every = describe.function_names(module_path, LANES["pe"].functions(module_path))
    dll, where, dropped = P.packager.functions(module_path, m.header, m.texts, placed, m.imports, work,
                                               P.compiler.options(), originals={name: va for va, name in every.items()})
    items, dropped = {}, dict(dropped)
    for fn in placed:
        if fn in dropped:
            continue
        if fn not in where:
            dropped[fn] = "not in the build"
        else:
            items[fn] = (m.key[fn], m.va[fn], dll, where[fn], 2 if fn in m.in_edx else 1 if fn in m.in_ecx else 0)
    return module_path, items, dropped


def judge(module_path, item):
    """Worker: judge one function. item = (key, address, dll, entry)."""
    _key, va, dll, entry = item[:4]
    shape = (in_ecx_edx if item[4] == 2 else in_ecx) if len(item) > 4 and item[4] else (lambda v: v)

    def rebuilt(v):
        # A rebuild may spend many more instructions than the original on the same work -- a C
        # loop where the original filled memory with one `rep stosd` (dnr.exe's memset over
        # 64 KB: 16,418 steps for the original, 229,414 for the loop, the same 16,387 writes)
        # -- and stop at the step limit where the original returned. Such a run is tried again
        # with room before it counts as different -- also where the original itself stopped at
        # the limit: the rebuild must reach every call the original made by then.
        b = _fp_pe(dll, entry, shape(v), rebuilt=module_path)
        if b.get("truncated") and b.get("exit") != "wild-call" and \
                _fp_pe(module_path, va, shape(v)).get("exit") in ("ret", "truncated"):
            b = _fp_pe(dll, entry, dict(shape(v), room=ROOM), rebuilt=module_path)
        return b

    return lane_common.judged(lambda info: compare.compare(
        lambda v: _fp_pe(module_path, va, shape(v)),
        rebuilt,
        _vectors(ARITY, "pe"), _confirm_vectors(ARITY, "pe"),
        insns=insn_count(module_path, va),
        directed=lambda cov, rch: directed_and_guided(module_path, va, cov, rch,
                                                      shape if len(item) > 4 and item[4] else None),
        info=info, leftover=undefined_ret_bits))


PIPE_REV = 47                  # what a verdict means (13: one build per batch; bodies the compiler rejects are dropped by name;
                                # 24, 25: a rebuild stopped at the step limit where the original returned is run again with ROOM;
                                # 43-45: striped objects, globals with values of their own, the original's data filled with pads;
                                # 46: an original cut off at the limit is matched by the calls it made, the rebuild given ROOM;
                                # 47: the branch tracer enters a flat thunk's stubs as the fingerprint runs do)
ROOM = 25 * tracer._MAX_INSNS   # the steps a rebuild is given then
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
        cc = platforms.of("pe" if lane == "pe" else "ne").compiler.tool()
        try:
            st = os.stat(cc)
            compiler = "%s:%d:%d" % (os.path.basename(cc), st.st_size, int(st.st_mtime))
        except OSError:
            compiler = ""
        settings = {
            "lane": lane, "pipeline": PIPE_REV, "oracle": compare.REV,
            "inputs": [v["args"] for v in _vectors(ARITY, lane)],
            "confirm_inputs": [v["args"] for v in _confirm_vectors(ARITY, lane)],
            # (and what they put in memory: two inputs with the same arguments can differ only there)
            "input_memory": hashlib.sha1(repr([sorted(v.items()) for v in _vectors(ARITY, lane) + _confirm_vectors(ARITY, lane)])
                                         .encode()).hexdigest()[:16],
            "max_calls": _MAX_CALLS, "max_support": MAX_SUPPORT,
            "directed": [MAX_DIRECTED, _HARVEST_DEPTH] if lane == "pe" else [],
            "guided": [lane_common.GUIDED_BUDGET, lane_common.MAX_GUIDED] if lane == "pe" else [],
            # (not the decompiler's revision: a verdict is for a text, whichever revision wrote it)
            "cflags": P.compiler.options(), "runtime": B._RTL,
            "compiler": compiler,
        }
        text = json.dumps(settings, sort_keys=True)
        _PROFILES[lane] = (hashlib.sha1(text.encode()).hexdigest()[:12], text)
    return _PROFILES[lane]


# =============================================================================================
# Lanes
# =============================================================================================
class PELane(object):
    """32-bit PE. Bodies are compiled together in batches, so a body that calls a batch-mate is
    verified against that mate's real reconstruction rather than a stub."""
    name = "pe"

    def functions(self, path):
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


# compiler/CRT-runtime imports the toolchain emits (not the body's own logic) -- ignored so
# MSC-vs-OpenWatcom startup noise is not scored as a behavioral divergence.
_TOOLCHAIN = {"KERNEL.GETVERSION", "KERNEL.GETWINFLAGS", "KERNEL.LOCALINIT", "KERNEL.GETVERSION16",
              "KERNEL32.GETVERSION", "KERNEL32.GETVERSIONEXA", "KERNEL32.GETCURRENTTHREADID"}


def _norm_dep(d):
    """Canonicalise an imported-call name so the two builds' import-table SPELLINGS of the same
    import compare equal: uppercase, drop a .DLL/.EXE/.SYS (NDIS.SYS is NDIS) on the module half, unify the separator.
    ('KERNEL32.dll!#21' and 'KERNEL32!#21' are the same ordinal import -> same behavior)."""
    s = str(d).upper().replace("!", ".").replace(":", ".")
    parts = s.split(".")
    parts = [p for p in parts if p not in ("DLL", "EXE", "DRV", "SYS", "VXD", "OCX", "CPL")]
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
    from codebreaker.emulate import emulator as env
    n = apiarity.argc(dll, name)
    return n if n is not None else env._builtin_argc(name)


_MAX_CALLS = 64


def _call_seq(calls, ins, own=None):
    """The imported-call part of a fingerprint: every call in order, its name as both builds spell
    it, toolchain noise left out, and each argument as lane_common.argument writes it -- which input
    it is, a small constant, or a stack address or an address inside the rebuilt image ("S": those
    differ by build; a rebuilt body's locals live in its stand-in frame, inside its own image);
    otherwise the value itself. Reconstructed code refers to the original's data by its literal
    address, so a pointer into the original image is the same number on both sides and is compared
    as one: passing the wrong string or the wrong table is a difference."""
    frame = ((_STACK_LO, _STACK_HI), own)
    return lane_common.calls(calls, lambda a: lane_common.argument(a, ins, small=True, frame=frame),
                             name=_norm_dep, skip=_is_noise, cap=_MAX_CALLS)


# provisioned environment (mirrors envprov): a region of pointers that all resolve to a safe RET, so
# a pointer/handle argument that gets dereferenced OR called through lands on a clean return instead
# of a null fault. Both regions live inside the tracer's pre-mapped scratch window (0x200000..0x210000).
_OBJ, _SAFE = 0x206000, 0x207800


_PADS = (_SAFE, _SAFE + 0x1000)
_PLAIN = lane_common.PLAIN       # one instruction at a time on a fresh machine (CB_PLAIN)


def _prov_meminit():
    """A pretend object whose every field is a pointer to a landing pad of its own: a call
    through a field returns, and WHICH field was called through is seen (emulator `pads`)."""
    return [(_OBJ, b"".join(_struct.pack("<I", _SAFE + 4 * i) for i in range(0x400))),
            (_SAFE, b"\xc3" * 0x1000)]


# --- behavioral-fingerprint cache -------------------------------------------------------------
# A PE fingerprint is a pure function of (module bytes, func addr, input vector): the trace is
# deterministic given those. The RETAIL side (the corpus binary) is reconstruction-independent, so
# its fingerprint is identical across every body and every sweep re-run -- yet it was re-traced each
# time, making it ~half of the oracle cost (4 of 8 traces/body). Cache fingerprints keyed by the
# module's content hash so a re-run (the common case after any synth/lifter change) skips the retail
# traces entirely. Keying on content hash (not path) makes it correct for both sides and for shared
# bodies: a rebuilt DLL gets a fresh hash each build, so it never stales. (lane_common.remembered;
# not with CB_NO_FP_CACHE.)


def _fp_key(sha, func, spec):
    args = tuple(int(a) for a in spec.get("args", []))
    mi = tuple((int(a), bytes(b)) for a, b in (spec.get("mem_init") or []))
    return "r%d:%s:%s" % (compare.REV, sha, _hashlib.sha1(
        repr((func, args, mi, int(spec.get("stub_ret", 0))) + (
            (tuple(spec["sentinels"]),) if spec.get("sentinels") else ()) + (
            ("ecx", int(spec["ecx"])) if spec.get("ecx") is not None else ()) + (
            ("edx", int(spec["edx"])) if spec.get("edx") is not None else ()) + (
            ("room", int(spec["room"])) if spec.get("room") else ())).encode()).hexdigest())


def in_ecx(v):
    """The same input for a function that takes its first argument in ECX (a member function's
    object): the first value there, the rest on the stack."""
    if "ecx" in v:
        return v
    return dict(v, ecx=v["args"][0], args=list(v["args"][1:]) + [0], given=list(v["args"]))


def in_ecx_edx(v):
    """... and for one that takes its first two in ECX and EDX."""
    if "ecx" in v:
        return v
    return dict(v, ecx=v["args"][0], edx=v["args"][1], args=list(v["args"][2:]) + [0, 0], given=list(v["args"]))


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
    if r.get("writes_seen", len(ws)) >= tracer._MAX_MEM:
        return None
    if r.get("pages0") is not None:
        return _effects_lean(r, ws, own)
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
    return lane_common.writes({k: x for k, x in state.items() if x != before.get(k)})


def _effects_lean(r, ws, own):
    """_effects for a run recorded lean (emulator.trace_function): writes as (address, size,
    value, pc), no stack writes among them, and what memory held before in r["pages0"]."""
    mn = r.get("mnemonics") or {}
    latest = {}                                      # (address, size) -> the last write of that shape
    for i, (a, n, v, pc) in enumerate(ws):
        if pc and mn.get("0x%x" % pc) == "add byte ptr [eax], al":
            continue
        if own and own[0] <= a < own[1]:
            continue
        latest[(a, n)] = (i, v)
    state = {}
    for (a, n), (_i, v) in sorted(latest.items(), key=lambda kv: kv[1][0]):
        if n == 4 and (_STACK_LO <= v < _STACK_HI or (own and own[0] <= v < own[1])):
            for i in range(4):
                state[a + i] = "S"
        else:
            for i in range(min(n, 8)):
                state[a + i] = (v >> (8 * i)) & 0xFF
    pages = r["pages0"]
    return lane_common.writes({k: x for k, x in state.items() if x != pages[k & ~0xFFF][k & 0xFFF]})


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
    """How many instructions the function at `func` has, or 0: those reachable from its entry
    through its own jumps (a decode to the next known function also counted what lies after the
    function's last jump or return -- the rest of an import-thunk table, padding, the next
    function's data -- and so called a one-instruction thunk 2% covered). Where the function
    jumps through a register or a table, its reach is not known: then every instruction to the
    end of its extent, as before."""
    key = (path, func)
    if key not in _INSN_COUNT:
        n = 0
        try:
            off, va0, code, end, _sec = tracer._resolve_func(path, func)
            n = _reachable_insns(bytes(code[off:end]), va0 + off)
        except Exception:
            n = 0
        _INSN_COUNT[key] = n
    return _INSN_COUNT[key]


def _reachable_insns(code, start):
    from capstone import x86_const as X
    md = P.cpu.disassembler(detail=True)
    lo, hi = start, start + len(code)
    seen, todo = set(), [start]
    while todo:
        a = todo.pop()
        while lo <= a < hi and a not in seen:
            try:
                ins = next(md.disasm(code[a - lo:a - lo + 16], a, 1))
            except StopIteration:
                break
            seen.add(a)
            mn = ins.mnemonic
            op = ins.operands[0] if ins.operands else None
            if mn in ("ret", "retf", "iret", "iretd", "hlt", "int3", "ud2"):
                break
            if mn == "jmp" or (mn.startswith("j") and op is not None):
                if op is not None and op.type == X.X86_OP_IMM:
                    todo.append(op.imm & 0xFFFFFFFF)
                elif mn == "jmp" and not (op is not None and op.type == X.X86_OP_MEM and not op.mem.base
                                          and not op.mem.index):
                    # through a register or a table: where it goes is not known here
                    return sum(1 for _ in md.disasm_lite(code, start))
                if mn == "jmp":
                    break
            a += ins.size
    return len(seen)


_THUNKS = {}


def _thunk_stubs(original):
    """{address: ("THUNK", name)} and {name: argument count} for the stubs a flat thunk's compiler
    generated (symbols/runtime_library: identified, not rebuilt). A call into one goes on into
    16-bit code through a pointer only Windows fills in; the judge enters it as it enters an
    import -- removing the arguments its `ret N` removes -- so the functions that call the 16-bit
    side can be judged at all (mnmcpi32.dll: 76 of them could not)."""
    if original not in _THUNKS:
        at, argc = {}, {}
        try:
            from codebreaker.symbols import runtime_library
            from codebreaker.formats.pe import open_pe
            pe = open_pe(original)
            sections = pe.code_sections()
            md = P.cpu.disassembler()
            for va in runtime_library._flat_thunk_stubs(original, pe, sections):
                name = "stub_%x" % va
                at[va] = ("THUNK", name)
                for base, raw, _n in sections:
                    if base <= va < base + len(raw):
                        for ins in md.disasm(raw[va - base:va - base + 512], va):
                            if ins.mnemonic in ("ret", "retn"):
                                n = int(ins.op_str, 0) if ins.op_str else 0
                                argc[name] = n // 4 if n % 4 == 0 and n <= 256 else None
                                break
                        break
        except Exception:
            at, argc = {}, {}
        _THUNKS[original] = (at, argc)
    return _THUNKS[original]


def _stub_options(original):
    """How a run enters the stubs: as imports (_thunk_stubs), each with its argument count. The
    same for a fingerprint run and a branch-logged one, so that the inputs aimed at branches are
    chosen on the program the judge runs."""
    stubs, stub_argc = _thunk_stubs(original)
    return {"stub_at": stubs or None,
            "sig_resolver": lambda d, n: stub_argc.get(n) if d == "THUNK" else _argc(d, n)}


def _fp_pe(path, func, spec, rebuilt=False):
    """Fingerprint one run. For a REBUILT image pass `rebuilt=<path of the original>`: its own
    statics are then left out of the comparison and the original's code is mapped beside it.

    Kept across runs under the module's content hash (lane_common.remembered, _fp_key). A run
    cut off at the step limit is kept too: a run stops on counts alone (the tracer's old
    wall-clock cap is gone), the same on every machine, so a truncated run is as settled as any
    other -- and they are the slowest to make again."""
    sha = corpus.sha(path)
    return lane_common.remembered(_fp_key(sha, func, spec) if sha else None,
                                  lambda: _trace_pe(path, func, spec, rebuilt))


def _trace_pe(path, func, spec, rebuilt):
    try:
        r = P.emulator.run(path, func=func, args=spec["args"], mem_init=spec.get("mem_init"),
                                  insn_cap=int(spec.get("room") or tracer._MAX_INSNS),
                                  stub_externals=True, stub_ret=int(spec.get("stub_ret", 0)),
                                  overlay=rebuilt if isinstance(rebuilt, str) else None,
                                  sentinels=spec.get("sentinels"), reads=False, stop_wild=True, ecx=spec.get("ecx"), edx=spec.get("edx"),
                                  quiet_stack=True, reuse=not _PLAIN, fast=not _PLAIN, lean=not _PLAIN, pads=_PADS,
                                  **_stub_options(rebuilt if isinstance(rebuilt, str) else path))
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
    deps = _call_seq(r.get("api_calls", []), [int(a) for a in spec.get("given") or spec["args"]],
                     _image_span(path) if rebuilt else None)
    fp = {"ret": r.get("ret"), "exit": "wild-call" if wild else compare._exit_norm(r.get("exit")), "deps": deps,
          "truncated": bool(r.get("truncated")), "steps": r.get("insn_count"),
          "writes": _effects(r, _image_span(path) if rebuilt else None),
          # how the function left the stack pointer (4 = the caller removes the arguments, 4 + N =
          # the function removed N bytes of them) and whether it preserved EBX/ESI/EDI/EBP
          "pop": r.get("esp_delta") if compare._exit_norm(r.get("exit")) == "ret" else None,
          "saved": (tuple(r.get("saved_regs") or ()) == tuple(spec.get("sentinels") or tracer.SAVED_SENTINELS)
                    if compare._exit_norm(r.get("exit")) == "ret" else None),
          "cov": (), "reach": ()}
    if not rebuilt:
        fp["cov"], fp["reach"] = _coverage(r)
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
        vs.append({"args": lane_common.counting(n, 0x200000, 0x100)})        # bare scratch pointers
        vs.append({"args": [_OBJ + 0x40 * (i % 32) for i in range(n)],        # provisioned objects
                   "mem_init": _prov_meminit()})
    else:                                                        # NE controlled env: valid handles
        vs.append({"args": [0x0400] * n})                        # LZ handle base
        vs.append({"args": [0x0080] * n})                        # open-file handle
    vs.append({"args": lane_common.counting(n)})
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
    ints = lane_common.counting(n)
    # Objects whose fields are not pointers: all-ones (every flag bit set, counts large) and small
    # counts. A field tested with `& 0x40` or compared with an index is false on the pointer-filled
    # object above, so everything behind such a test never ran.
    ones = [(_OBJ, b"\xff" * 0x1000), (_SAFE, b"\xc3" * 16)]
    small = [(_OBJ, _struct.pack("<I", 3) * 0x400), (_SAFE, b"\xc3" * 16)]
    # Objects whose words are zero and non-zero by turns, both ways round. On an object that is the
    # same all through, a field read one byte off (`p + 0x3d` for `p + 0x3c`) reads what the right
    # one holds, and `if (p->x == 0)` goes the same way whichever field it tests.
    striped = [[(_OBJ, b"".join(_struct.pack("<I", 0 if i % 2 == phase else _SAFE + 4 * i) for i in range(0x400))),
                (_SAFE, b"\xc3" * 0x1000)] for phase in (0, 1)]
    return [
        # Imported calls return 0 by default, so the code after `if (!CreateThing()) return` never
        # ran. Run again with imports succeeding: returning 1, and returning a valid object.
        {"args": obj, "mem_init": mi, "stub_ret": 1},
        {"args": obj, "mem_init": mi, "stub_ret": _OBJ + 0x800},
        {"args": ints, "stub_ret": 1},
        {"args": obj, "mem_init": ones, "stub_ret": 1},
        {"args": obj, "mem_init": small, "stub_ret": 1},
        {"args": obj, "mem_init": small},
        {"args": obj, "mem_init": striped[0], "stub_ret": 1},
        {"args": obj, "mem_init": striped[1], "stub_ret": 1},
        {"args": lane_common.descending(n, 0xFFFFFFFF)},                      # small negatives
        {"args": [edge[i % len(edge)] for i in range(n)]},                     # boundary integers
        {"args": lane_common.descending(n, n)},                                # distinct, reversed
        {"args": [obj[0]] + [i + 2 for i in range(1, n)], "mem_init": mi},     # pointer, then integers
        {"args": [5] + obj[1:], "mem_init": mi},                               # integer, then pointers
        {"args": lane_common.alternating(n, lambda i: 0x100 + i, lambda i: obj[i]), "mem_init": mi},
    ]


_HARVEST = {}
MAX_DIRECTED = 16                # directed inputs kept per body
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
            from capstone import x86_const as X
            lo, hi = _image_span(path)
            iat = set(open_pe(path).imports())
            md = P.cpu.disassembler(detail=True)
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


_FILL = {}
_FILL_MAX = 0x40000


def _data_filled(path):
    """mem_init that gives every word of the original's writable sections a value of its own: the
    address of a landing pad, so one used as a function pointer is called and returns, and which
    was called is seen. The emulator does not load a module's data, so its globals all read as
    zero: two of them passed in each other's places, or a loop that clears a table it never
    reaches, look the same as the right code while both are zero. (The file's own data is not
    used: its tables of initialisers would be run, which is the harness's doing, not the body's.)"""
    if path not in _FILL:
        out = []
        try:
            pe = open_pe(path)
            for s in pe.sections:
                if not s["chars"] & 0x80000000 or s["chars"] & 0x20000000 or not s["vsize"]:
                    continue                                    # writable data only, not code
                at = pe.image_base + s["vaddr"]
                out.append((at, b"".join(_struct.pack("<I", _SAFE + 4 * (((at >> 2) + i) % 0x400))
                                         for i in range(min(s["vsize"], _FILL_MAX) // 4))))
        except Exception:
            out = []
        _FILL[path] = out
    return _FILL[path]


def directed_vectors(path, func, covered, arity=4, reached=(), shape=None):
    """Inputs chosen for THIS function. Generic inputs stop at the first guard (`if (msg != 0x4E)
    return`, `if (!g_initialised) return`), so most of a body never ran and was 'verified' on its
    first few instructions. Candidates put each constant the function compares against into each
    argument, and set the globals it reads to non-zero values; a candidate is kept only if the
    ORIGINAL then runs instructions no earlier input reached. Returns at most MAX_DIRECTED."""
    consts, globs = _harvest(path, func)
    if not consts and not globs and not _data_filled(path):
        return []
    n = arity
    mi = _prov_meminit()
    obj = [_OBJ + 0x40 * (i % 32) for i in range(n)]
    ints = lane_common.counting(n)
    cands = []
    gsets = [[(a, _struct.pack("<I", val)) for a in globs] for val in (1, _SAFE, _OBJ + 0x800)] if globs else []
    if globs:                                    # each global a value of its own (see _data_filled)
        gsets.append([(a, _struct.pack("<I", _SAFE + 4 * i)) for i, a in enumerate(globs)])
    filled = _data_filled(path)
    if filled:
        cands.append({"args": obj, "mem_init": mi + filled, "stub_ret": 1})
        cands.append({"args": ints, "mem_init": mi + filled})
    for g in gsets:
        cands.append({"args": obj, "mem_init": mi + g, "stub_ret": 1})
        cands.append({"args": ints, "mem_init": mi + g})
    # (Every position is tried, also past the arguments the function removes on return: a
    # function does sometimes read further up the stack, and one difference was only found so.)
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
    if shape:                                    # (a member function: the first value goes in ECX)
        cands = [shape(v) for v in cands]
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
                return keep
    # Then inputs that make the original DO something no earlier input made it do, though no
    # new code runs: the values either side of each constant it compares with. `x < 100` and
    # `x < 101` run the same instructions on every input but 100; only that input tells them
    # apart, and no count of instructions reached would ever ask for it.

    def did(f):
        return (f.get("exit"), f.get("ret"), f.get("deps"), f.get("writes"))
    done = set()
    for v in [shape(v) if shape else v for v in list(_vectors(arity, "pe")) + list(_confirm_vectors(arity, "pe"))] + keep:
        f = _fp_pe(path, func, v)
        if "err" not in f:
            done.add(did(f))
    edges = []
    for k in consts:
        for near in ((k - 1) & 0xFFFFFFFF, (k + 1) & 0xFFFFFFFF, k):
            for i in range(n):
                a = list(ints)
                a[i] = near
                edges.append({"args": a})
                a = list(obj)
                a[i] = near
                edges.append({"args": a, "mem_init": mi, "stub_ret": 1})
    for v in cands + [shape(v) if shape else v for v in edges]:
        f = _fp_pe(path, func, v)
        if "err" in f or did(f) in done:
            continue
        done.add(did(f))
        keep.append(v)
        if len(keep) >= MAX_DIRECTED:
            break
    return keep


def _branch_run(path, func, spec):
    """The original run on `spec` with its branches logged: {"branches", "reach"}, or None when
    the run is not an observation (as _fp_pe: an error, or a call into nothing). A flat thunk's
    stubs are entered as _fp_pe enters them (_stub_options)."""
    try:
        r = P.emulator.run(path, func=func, args=spec["args"], mem_init=spec.get("mem_init"),
                                  insn_cap=int(spec.get("room") or tracer._MAX_INSNS),
                                  stub_externals=True, stub_ret=int(spec.get("stub_ret", 0)),
                                  sentinels=spec.get("sentinels"), reads=False, stop_wild=True,
                                  ecx=spec.get("ecx"), edx=spec.get("edx"), quiet_stack=True,
                                  reuse=not _PLAIN, fast=not _PLAIN, lean=not _PLAIN, pads=_PADS, branches=True,
                                  **_stub_options(path))
    except ImportError:
        raise
    except Exception:
        return None
    if r.get("bogus_stubs") or r.get("ran_into_zeros"):
        return None
    return {"branches": r.get("branches") or [], "reach": set(_coverage(r)[1]),
            "args_at": (r.get("entry") or {}).get("esp", 0) + 4, "stack": (_STACK_LO, _STACK_HI)}


def guided_vectors(path, func, seeds, shape=None):
    """Inputs that take branches of the original no seed took (verify/guided.py)."""
    return lane_common.guided(lambda v: _branch_run(path, func, shape(v) if shape else v), seeds, func & 0xFFFF)


def directed_and_guided(path, func, covered, reached, shape=None):
    """The directed inputs, then branch-guided ones seeded with every input run so far."""
    d = directed_vectors(path, func, covered, reached=reached, shape=shape)
    seeds = list(_vectors(ARITY, "pe")) + list(_confirm_vectors(ARITY, "pe")) + list(d)
    return list(d) + guided_vectors(path, func, seeds, shape)


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
