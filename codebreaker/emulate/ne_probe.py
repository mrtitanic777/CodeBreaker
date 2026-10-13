#!/usr/bin/env python3
r"""ne_dynoracle.py -- the dynamic NE oracle (Cause C).

Static analysis (raise95 + neraise95) closes the mechanical gap, but three things can only be
settled by RUNNING the code: near vs far memory (which selector a buffer really lives in), the true
meaning of a magic return value, and what a lost register (`unaff_CS`) actually holds. This executes
a 16-bit NE function on the tracer16 Unicorn substrate inside a CONTROLLED Win16-API environment --
GlobalAlloc hands out real, distinct FAR heap blocks; the file APIs return fed data -- then observes
the run and classifies every memory access by the selector/region it hit (near DGROUP vs far heap).

    probe(dll, ordinal=7)  -> TraceRecord (with a Win16 environment)
    classify(rec)          -> {near:[..], far:[..], ...} region summary of the memory accesses

The answers feed back to the raise/coalesce engines (e.g. "the ring is FAR, selector = the
GlobalAlloc handle") and, ultimately, to a rebuild-and-diff verification.
"""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.emulate import trace16 as T   # the validated 16-bit substrate

HEAP_BASE = 0xB00000        # first far-heap block (distinct from seg/stack/scratch/sentinel windows)
HEAP_STRIDE = 0x10000
SAFE_ARG = T._sel(200)      # a selector that names a real, writable scratch segment

                            # returns to the sentinel regardless, so surplus args are harmless


class _Ctx(object):
    """Per-run oracle state: a far-heap allocator that installs real GDT descriptors so GlobalAlloc
    returns genuinely distinct far selectors (not the shared scratch pad), and fed file input."""
    def __init__(self, uc, U):
        self.uc, self.U = uc, U
        self.n = 0
        self.heaps = {}                    # selector -> (base, size)
        self.feed = b""                    # bytes handed to _lread
        self.feed_pos = 0

    def alloc_heap(self, size=0x10000):
        # take a GDT index near the top (the harness padded those with scratch; overwrite with a
        # real data descriptor pointing at a fresh linear window) and map that window.
        idx = T._N_GDT - 8 - self.n
        base = HEAP_BASE + self.n * HEAP_STRIDE
        self.n += 1
        try:
            self.uc.mem_map(base, ((size + 0xFFF) // 0x1000) * 0x1000)
        except self.U.UcError:
            pass
        self.uc.mem_write(T.GDT_ADDR + idx * 8, bytes(T._desc(base, 0xFFFF, False)))
        sel = T._sel(idx)
        self.heaps[sel] = (base, size)
        return sel


# Well-known Win16 KERNEL.EXE ordinals -- fallback when the SDK ordinal oracle (symdb) doesn't
# resolve them (it can come back as 'KERNEL.@15'). These are the stable KERNEL export ordinals.
KERNEL_ORD = {
    15: "GLOBALALLOC", 16: "GLOBALREALLOC", 17: "GLOBALFREE", 18: "GLOBALLOCK",
    19: "GLOBALUNLOCK", 20: "GLOBALSIZE", 21: "GLOBALHANDLE", 22: "GLOBALFLAGS",
    23: "LOCKSEGMENT", 24: "UNLOCKSEGMENT", 26: "GLOBALDOSALLOC",
    81: "_LCLOSE", 82: "_LREAD", 83: "_LCREAT", 84: "_LLSEEK", 85: "_LOPEN", 86: "_LWRITE",
}


def _api(hit):
    """Bare API function name, upper-cased (drop the 'MODULE.' prefix); resolve KERNEL @ordinals."""
    api = (hit.get("api") or "").upper()
    fn = api.split(".", 1)[1] if "." in api else api
    if fn.startswith("@") and api.startswith("KERNEL"):
        try:
            fn = KERNEL_ORD.get(int(fn[1:]), fn)
        except ValueError:
            pass
    return fn


def default_stub(hit, uc, U, ctx):
    """Model the Win16 APIs an LZ-class module calls. Returns {ax, dx, pop}. `pop` = total Pascal
    arg bytes the callee removes (WORD=2, DWORD / far ptr = 4)."""
    fn = _api(hit)
    a = hit.get("args_stack", [])
    if fn in ("GLOBALALLOC", "GLOBALDOSALLOC"):        # (wFlags:W, dwBytes:D) -> HGLOBAL(sel)
        return {"ax": ctx.alloc_heap(), "pop": 6}
    if fn in ("GLOBALLOCK",):                            # (hMem:W) -> far ptr sel:0000  (DX:AX)
        return {"ax": 0, "dx": a[0] if a else 0, "pop": 2}
    if fn in ("GLOBALFREE", "GLOBALUNLOCK", "GLOBALHANDLE"):
        return {"ax": 0, "pop": 2}
    if fn in ("_LOPEN", "_LCREAT"):                      # -> HFILE (pretend a valid handle)
        return {"ax": 0x0080, "pop": 6}
    if fn in ("_LCLOSE",):
        return {"ax": 0, "pop": 2}
    if fn in ("_LLSEEK",):                               # (hf:W, off:D, org:W) -> LONG new pos
        return {"ax": (a[1] if len(a) > 1 else 0), "dx": (a[2] if len(a) > 2 else 0), "pop": 8}
    if fn in ("_LREAD", "_HREAD"):                       # (hf:W, buf:far, cb:W) -> bytes read
        cb = a[3] if len(a) > 3 else 0
        return {"ax": cb, "pop": 8}
    if fn in ("_LWRITE", "_HWRITE"):
        cb = a[3] if len(a) > 3 else 0
        return {"ax": cb, "pop": 8}
    return {"ax": 0}                                     # unknown: benign 0 (stack may drift)


def probe(dll, ordinal=None, seg=None, offset=None, args=None, stub=default_stub, insn_cap=None,
          prelude=None, cdecl=False, near=False, data_image=None):
    """Run one NE function under the controlled Win16 environment. Returns the tracer16 TraceRecord
    (with rec['layout'] + rec['imports_hit'] + rec['mem_*'] + rec['ret']).

    cdecl=True reverses `args` before pushing: the tracer pushes Pascal order (left-to-right), but
    INTERNAL functions are often __cdecl16far (args pushed right-to-left), so their args must be
    reversed to land in the right slots (this is how the LZDecompress reset path was reached)."""
    if cdecl and args:
        args = list(reversed(args))
    ctx_box = {}

    def _stub(hit, uc, u):
        if "ctx" not in ctx_box:
            ctx_box["ctx"] = _Ctx(uc, u)
        res = stub(hit, uc, u, ctx_box["ctx"]) or {}
        if "pop" not in res:                                 # remove the arguments, as the real one would
            from codebreaker.emulate import api16
            size = api16.of(hit.get("api"))
            hit["arg_bytes"] = size
            if size:
                res = dict(res, pop=size)
        else:
            hit["arg_bytes"] = res["pop"]
        return res

    kw = {}
    if insn_cap:
        kw["insn_cap"] = insn_cap
    rec = T.trace_function16(dll, ordinal=ordinal, seg=seg, offset=offset,
                             args=args, arity=len(args or []), import_stub=_stub,
                             prelude=prelude, near=near, data_image=data_image, **kw)
    rec["heaps"] = {"base": HEAP_BASE, "stride": HEAP_STRIDE}
    ctx = ctx_box.get("ctx")
    rec["heap_sels"] = {s: b for s, (b, _sz) in ctx.heaps.items()} if ctx else {}
    return rec
