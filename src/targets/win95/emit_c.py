#!/usr/bin/env python3
r"""emit_c.py -- Layer-2 pipeline: auto-generate clean C for the mechanical C
classes and confirm byte-exact with the cc32 (MSVC 2.0 /O1) oracle.

Closes the "clean C is per-function" gap for the tractable classes: const-return
and single-forward (a call/jmp whose arguments are the caller's args and/or
constants). The forwarder reader tracks the esp delta + frame + reg->arg copies
so each pushed value maps to a<i> or a literal, then reads the target. Whatever
the oracle confirms byte-exact is done unattended; the rest falls to the ASM
floor. Returns per function (ok, csrc, sym, flags).
"""
import re

import pe32util as H
import cc32
from src.engines.recon import segdiff

WIN = ("typedef unsigned long DWORD; typedef unsigned int UINT; typedef int BOOL;\n"
       "typedef void *HANDLE, *HDC, *HGDIOBJ, *LPVOID; typedef const char *LPCSTR;\n")
_ARG = re.compile(r"(?:dword ptr )?\[(esp|ebp)(?: \+ (0x[0-9a-f]+|\d+))?\]$")
_DEREF = re.compile(r"(?:dword ptr )?\[(eax|ecx|edx|esi|edi|ebx)(?: \+ (0x[0-9a-f]+|\d+))?\]$")


def _args(n):
    return "void" if n == 0 else ", ".join("int a%d" % i for i in range(n))


def split_unit(csrc):
    """Split a per-function compile unit (WIN prelude + optional extern/dllimport
    decls + one function definition) into (decl_lines, body_line) so a whole
    module can be assembled with the prelude written once and the target decls
    de-duplicated -- a clean single translation unit instead of N stitched stubs."""
    rest = csrc[len(WIN):] if csrc.startswith(WIN) else csrc
    if "\n" in rest.strip():
        head, body = rest.rstrip().rsplit("\n", 1)
        return [l.strip() for l in head.split("\n") if l.strip()], body.strip()
    return [], rest.strip()


def _ret_const(ins):
    val = None
    for i in ins:
        m, o = i.mnemonic, i.op_str
        if m == "xor" and o == "eax, eax":
            val = 0
        elif m == "mov" and o.startswith("eax, ") and "[" not in o:
            try:
                v = int(o.split(", ", 1)[1], 0)
            except ValueError:
                return None
            val = v - 0x100000000 if v > 0x7fffffff else v
        elif m in ("ret", "retn"):
            break
        elif m in ("push", "pop", "nop", "int3", "leave"):
            continue
        else:
            return None
    return val


def _tgt_cdecl(tail):
    """A single call whose stack args are cleaned by the CALLER (add esp,K or a
    run of `pop reg`) is a __cdecl target; no cleanup is __stdcall. Returns True
    for cdecl. `tail` is the instruction stream after the call."""
    for t in tail:
        m, o = t.mnemonic, t.op_str
        if m == "add" and o.startswith("esp,"):
            return int(o.split(",")[1], 0) > 0
        if m == "mov" and o == "esp, ebp":
            return True                                   # framed caller-clean (no locals) => cdecl
        if m == "pop" and o in ("ecx", "edx", "eax"):
            return True                                   # caller pops its own arg back off
        if m in ("ret", "retn", "nop", "int3", "leave"):
            continue
        if m in ("mov", "cmp", "sbb", "neg", "inc", "dec", "test", "and", "or", "xor", "lea"):
            continue                                      # BOOL-massage / return shaping
        break
    return False


def _forward_call(ins, base, byva, iat):
    """Model a thin export that sets up args and tail-forwards to ONE target.
    Returns a dict {kind, tname, args, wrap, tgt_cdecl, ninc} or None. Bails
    (None) on shapes it cannot model faithfully -- register (fastcall/thiscall)
    args, or more than one call/tail-jmp -- so we never emit a wrong draft."""
    delta = 0
    ebp_frame = None
    regmap = {}
    pushes = []
    maxarg = [-1]
    globs = set()                                         # absolute [0xADDR] data refs -> externs
    live_reg = set()                                      # ecx/edx holding a value LIVE into
    #                                                       the call => a fastcall register arg

    def note(expr):
        m = re.match(r"a(\d+)$", expr or "")
        if m:
            maxarg[0] = max(maxarg[0], int(m.group(1)))

    def slot(seg, k):
        bd = delta if seg == "esp" else ebp_frame
        if bd is None:
            return None
        off = bd + k - 4
        return "a%d" % (off // 4) if off >= 0 and off % 4 == 0 else None

    for idx, i in enumerate(ins):
        m, o = i.mnemonic, i.op_str
        if m == "push" and o == "ebp" and idx + 1 < len(ins) and ins[idx + 1].op_str == "ebp, esp":
            delta -= 4; continue                     # frame prologue, not a call arg
        # ecx/edx liveness: a `mov/xor/lea ecx,..` produces a live value; any use of
        # the register as a source/base (push ecx, push [ecx+N], op X,ecx) consumes it
        # (it was scratch marshalled onto the stack). A register still live at the call
        # is a fastcall/thiscall argument we cannot model as a plain C forward.
        toks = re.split(r"[,\s\[\]+*]+", o)
        for rn in ("ecx", "edx"):
            is_dst = m in ("mov", "xor", "lea", "movzx", "movsx") and o.split(",", 1)[0].strip() == rn
            if rn in toks and not (is_dst and toks.count(rn) == 1):
                live_reg.discard(rn)                 # read/consumed as scratch
            if is_dst:
                live_reg.add(rn)                     # (re)loaded with a value
        if m == "call" or (m == "jmp" and (o.startswith("0x") or "[0x" in o)):
            tail = ins[idx + 1:]
            if any(t.mnemonic == "call" for t in tail):
                return None                          # multi-call wrapper -> not a simple forward
            if any(t.mnemonic == "jmp" and (t.op_str.startswith("0x") or "[0x" in t.op_str) for t in tail):
                return None
            regargs = []
            fastcall = False
            if live_reg:
                # __fastcall/__thiscall: ecx is the 1st arg, edx the 2nd. Model it only
                # when ecx is filled first and every live register has a KNOWN value
                # (a constant or an incoming arg slot) -- else bail rather than guess.
                if "ecx" not in live_reg:
                    return None
                for rr in (["ecx", "edx"] if "edx" in live_reg else ["ecx"]):
                    v = regmap.get(rr)
                    if not v or "?" in v:
                        return None
                    regargs.append(v)
                fastcall = True
            if o.startswith("0x"):
                t = int(o, 16) - base
                kind, tname = ("export", byva.get(base + t)) if (base + t) in byva else ("internal", None)
                tname = tname or "sub_%05x" % t
            else:
                mm = re.search(r"\[0x([0-9a-f]+)\]", o)
                if not mm or int(mm.group(1), 16) not in iat:
                    return None
                kind, tname = "import", iat[int(mm.group(1), 16)][1]
            post = ["%s %s" % (x.mnemonic, x.op_str) for x in tail]
            wrap = "%s"                                   # identity return
            if "sbb eax, eax" in post:                    # a BOOL-from-comparison massage
                if "inc eax" in post and "cmp eax, 1" in post:
                    wrap = "%s != -1"                     # CLR_INVALID idiom (e.g. SetPixelV)
                elif "neg eax" in post:
                    wrap = "%s != 0"
            else:                                         # post-call adjust of the return value
                for t in tail:                            #   (e.g. _errno: return tls_ptr + 8)
                    if t.mnemonic in ("add", "sub") and t.op_str.startswith("eax,") \
                            and re.match(r"eax, (0x[0-9a-f]+|\d+)$", t.op_str):
                        k = int(t.op_str.split(", ")[1], 0)
                        wrap = "(%%s) %s %d" % ("+" if t.mnemonic == "add" else "-", k)
                        break
                    if t.mnemonic in ("ret", "retn"):
                        break
                    if t.mnemonic not in ("mov", "nop", "lea"):
                        break
            return dict(kind=kind, tname=tname, args=regargs + list(reversed(pushes)), wrap=wrap,
                        tgt_cdecl=(False if fastcall else _tgt_cdecl(tail)),
                        fastcall=fastcall, ninc=maxarg[0] + 1, globals=globs)
        if m == "push":
            am = _ARG.match(o)
            dm = _DEREF.match(o)
            if am:
                expr = slot(am.group(1), int(am.group(2), 0) if am.group(2) else 0)
            elif dm and dm.group(1) in regmap and "?" not in regmap[dm.group(1)]:
                d = int(dm.group(2), 0) if dm.group(2) else 0     # push [argreg + N] -> arg->field
                ae = regmap[dm.group(1)]
                expr = ("((int *)%s)[%d]" % (ae, d // 4)) if d % 4 == 0 else \
                       ("*(int *)((char *)%s + %d)" % (ae, d))
            elif o in regmap and "?" not in regmap[o]:
                expr = regmap[o]
            elif re.match(r"-?(0x[0-9a-f]+|\d+)$", o):
                v = int(o, 0); expr = str(v - 0x100000000 if v > 0x7fffffff else v)
            elif re.match(r"(?:dword ptr )?\[0x[0-9a-f]+\]$", o):
                addr = int(re.search(r"0x[0-9a-f]+", o).group(0), 16)   # push [global] -> extern
                expr = "g_%x" % addr; globs.add(addr)
            else:
                expr = None
            if expr is None:
                return None
            note(expr); pushes.append(expr); delta -= 4
        elif m == "pop":
            delta += 4
        elif m in ("sub", "add") and o.startswith("esp,"):
            nn = int(o.split(",")[1], 0); delta += (-nn if m == "sub" else nn)
        elif m == "mov" and o == "ebp, esp":
            ebp_frame = delta
        elif m == "mov":
            dst, _, src = o.partition(", ")
            am = _ARG.match(src)
            if dst in ("eax", "ecx", "edx", "esi", "edi", "ebx") and am:
                sl = slot(am.group(1), int(am.group(2), 0) if am.group(2) else 0)
                regmap[dst] = sl or "?"; note(sl)
            elif dst in ("ecx", "edx") and re.match(r"-?(0x[0-9a-f]+|\d+)$", src):
                v = int(src, 0)                           # mov ecx/edx, imm -> register arg value
                regmap[dst] = str(v - 0x100000000 if v > 0x7fffffff else v)
            elif dst not in ("ebp",) and "[" not in dst:
                regmap.pop(dst, None)
        elif m == "xor" and o in ("ecx, ecx", "edx, edx"):
            regmap[o.split(",")[0]] = "0"                 # xor r,r -> register arg = 0
    return None


def _leaf_expr(ins):
    """Model a CALL-FREE leaf that computes eax from one arg: field deref
    (`return ((int*)a0)[k]`), arg arithmetic (`return a0 + K`), shifts. Returns
    (c_expr, ninc) or None. Bounded to a whitelist so we never emit a wrong body."""
    e = None
    maxa = -1
    globs = set()

    def argslot(k):                                       # entry esp: [esp+4] = a0
        off = k - 4
        return ("a%d" % (off // 4), off // 4) if off >= 0 and off % 4 == 0 else (None, -1)

    for i in ins:
        m, o = i.mnemonic, i.op_str
        if m == "mov" and o.startswith("eax, "):
            src = o.split(", ", 1)[1]
            am = _ARG.match(src); dm = _DEREF.match(src)
            if am and am.group(1) == "esp":
                nm, idx = argslot(int(am.group(2), 0) if am.group(2) else 0)
                if nm is None:
                    return None
                e = nm; maxa = max(maxa, idx)
            elif dm and dm.group(1) == "eax" and e is not None:
                d = int(dm.group(2), 0) if dm.group(2) else 0
                e = ("((int *)(%s))[%d]" % (e, d // 4)) if d % 4 == 0 else \
                    ("*(int *)((char *)(%s) + %d)" % (e, d))
            elif e is None and re.match(r"(?:dword ptr )?\[0x[0-9a-f]+\]$", src):
                a = int(re.search(r"0x[0-9a-f]+", src).group(0), 16)   # mov eax,[global] -> return g_
                e = "g_%x" % a; globs.add(a)
            else:
                return None
        elif m in ("add", "sub", "and", "or", "xor") and e is not None:
            mm = re.match(r"eax, (-?0x[0-9a-f]+|-?\d+)$", o)
            if not mm:
                return None
            v = int(mm.group(1), 0); v = v - 0x100000000 if v > 0x7fffffff else v
            e = "(%s) %s %d" % (e, {"add": "+", "sub": "-", "and": "&", "or": "|", "xor": "^"}[m], v)
        elif m in ("shl", "shr", "sar") and e is not None:
            mm = re.match(r"eax, (\d+)$", o)
            if not mm:
                return None
            e = "(%s) %s %d" % (e, "<<" if m == "shl" else ">>", int(mm.group(1)))
        elif m == "lea" and e is not None:
            mm = re.match(r"eax, \[eax \+ (0x[0-9a-f]+|\d+)\]$", o)
            if not mm:
                return None
            e = "(%s) + %d" % (e, int(mm.group(1), 0))
        elif m in ("ret", "retn"):
            return (e, maxa, globs) if e is not None else None
        elif m in ("nop", "int3"):
            continue
        else:
            return None
    return None


def _call_seq(ins, base, byva, iat):
    """Model a frameless straight-line sequence of >=2 independent forward calls
    -- `f1(args); ...; return fN(args)` -- each arg a stack slot / const / global,
    per-call cdecl/stdcall cleanup detected from a trailing `add esp`. Bails (None)
    on a frame, register/fastcall args, result threading, stores, or control flow,
    so a wrong body is never emitted. Returns dict{calls, ninc, globals} or None."""
    delta = 0
    pushes = []
    calls = []
    maxarg = [-1]
    globs = set()
    live = set()
    saved = []                                            # leading push esi/edi/ebx (callee-saved)
    capture = {}                                          # saved-reg -> call index whose eax it holds
    ret_call = [None]                                     # call index returned via a saved reg

    def slot(k):
        off = delta + k - 4
        return "a%d" % (off // 4) if off >= 0 and off % 4 == 0 else None

    def note(e):
        m = re.match(r"a(\d+)$", e or "")
        if m:
            maxarg[0] = max(maxarg[0], int(m.group(1)))

    for idx, i in enumerate(ins):
        m, o = i.mnemonic, i.op_str
        if m == "push" and o in ("esi", "edi", "ebx") and not calls and not pushes and o not in saved:
            saved.append(o); delta -= 4; continue         # prologue save of a callee-saved reg
        toks = re.split(r"[,\s\[\]+*]+", o)
        for rn in ("ecx", "edx"):
            is_dst = m in ("mov", "xor", "lea") and o.split(",", 1)[0].strip() == rn
            if rn in toks and not (is_dst and toks.count(rn) == 1):
                live.discard(rn)
            if is_dst:
                live.add(rn)
        if m == "push" and o == "ebp":
            return None                                   # frame -> not modeled here
        if m == "call" or (m == "jmp" and (o.startswith("0x") or "[0x" in o)):
            if live:
                return None                               # register (fastcall) args
            if o.startswith("0x"):
                t = int(o, 16) - base
                kind, tname = ("export", byva.get(base + t)) if (base + t) in byva else ("internal", None)
                tname = tname or "sub_%05x" % t
            else:
                mm = re.search(r"\[0x([0-9a-f]+)\]", o)
                if not mm or int(mm.group(1), 16) not in iat:
                    return None
                kind, tname = "import", iat[int(mm.group(1), 16)][1]
            args = list(reversed(pushes)); pushes = []
            nxt = ins[idx + 1] if idx + 1 < len(ins) else None
            cdecl = bool(nxt and nxt.mnemonic == "add" and nxt.op_str.startswith("esp,"))
            calls.append((kind, tname, args, cdecl))
            if not cdecl:
                delta += 4 * len(args)                    # stdcall: callee cleaned the args
            continue
        if m == "push":
            am = _ARG.match(o)
            if am and am.group(1) == "esp":
                expr = slot(int(am.group(2), 0) if am.group(2) else 0)
            elif re.match(r"-?(0x[0-9a-f]+|\d+)$", o):
                v = int(o, 0); expr = str(v - 0x100000000 if v > 0x7fffffff else v)
            elif re.match(r"(?:dword ptr )?\[0x[0-9a-f]+\]$", o):
                a = int(re.search(r"0x[0-9a-f]+", o).group(0), 16); expr = "g_%x" % a; globs.add(a)
            else:
                expr = None
            if expr is None:
                return None
            note(expr); pushes.append(expr); delta -= 4
        elif m in ("add", "sub") and o.startswith("esp,"):
            n = int(o.split(",")[1], 0); delta += (-n if m == "sub" else n)
        elif m == "mov" and o.split(",", 1)[0].strip() in saved and o.split(", ", 1)[-1] == "eax" and calls:
            capture[o.split(",", 1)[0].strip()] = len(calls) - 1   # save this call's result
        elif m == "mov" and o.startswith("eax, ") and o.split(", ", 1)[1] in saved:
            ret_call[0] = capture.get(o.split(", ", 1)[1])         # return the saved result
        elif m == "pop" and o in saved:
            delta += 4
        elif m in ("ret", "retn"):
            break
        elif m in ("nop", "int3"):
            continue
        else:
            return None                                   # store / other reg-op / control flow -> bail
    if len(calls) < 2 or pushes:
        return None
    return dict(calls=calls, ninc=maxarg[0] + 1, globals=globs, ret_call=ret_call[0])


def _self_variants(name, retbytes, ninc):
    """Candidate (convention, decorated-symbol, arg-count) for the emitted export
    itself, from observable evidence:
      * ret N (N>0)          -> __stdcall, N/4 args, symbol _Name@N (unambiguous)
      * plain ret + incoming  -> __cdecl, ninc args, symbol _Name (stdcall-w/args
                                 would have used ret N, so this is unambiguous)
      * plain ret, no args    -> ABI-identical either way; try __stdcall@0 then
                                 __cdecl so Win32 exports keep their WINAPI label."""
    if retbytes > 0:
        return [("__stdcall", "_%s@%d" % (name, retbytes), retbytes // 4)]
    if ninc > 0:
        return [("__cdecl", "_" + name, ninc)]
    # 0-arg: __stdcall and __cdecl are ABI-identical (same bytes, plain ret); one
    # variant suffices for byte verification -- keep the WINAPI label.
    return [("__stdcall", "_%s@0" % name, 0)]


def gen(module, row):
    c = H.carve(module); base = c.pe.image_base
    s = base + row["rva"]
    ins = H.disasm(H.func_bytes(c, s, s + row["size"]), s)
    # Internal (unexported) functions have no name -- synthesize sub_<rva>. The byte-check
    # compares CODE bytes (calls masked), so the symbol is irrelevant to correctness; skipping
    # unnamed functions needlessly dropped the whole internal-function c-route class.
    name = row["name"] or ("sub_%05x" % row["rva"]); retbytes = row["retbytes"]
    byva = {v: nm for _r, nm, v, _e in H.named_funcs(module)}
    iat = H.imports(module)
    cands = []
    v = _ret_const(ins)                                   # any cat: a const reached before any call
    if v is not None:                                     #   (also rescues carve-overrun const stubs)
        for conv, sym, na in _self_variants(name, retbytes, 0):
            cands.append((WIN + "int %s %s(%s){ return %s; }"
                          % (conv, name, _args(na), v), sym))
        return cands
    if all(i.mnemonic in ("ret", "retn", "nop", "int3") for i in ins) and ins:
        # a bare `ret`/`ret N` (+padding): an empty function. `ret N` -> __stdcall N/4 args;
        # an empty void body compiles to exactly that.
        for conv, sym, na in _self_variants(name, retbytes, 0):
            cands.append((WIN + "void %s %s(%s){ }" % (conv, name, _args(na)), sym))
        return cands
    # Try the structural models for ANY c-route shape (not just forwarder/wrapper): each modeler
    # bails on shapes it can't represent, and the byte-check gates correctness, so widening the
    # gate only adds coverage -- it can never land a wrong body as byte-exact.
    if True:
        fc = _forward_call(ins, base, byva, iat)
        if fc and not any("?" in a for a in fc["args"]):
            tconv = "__fastcall" if fc.get("fastcall") else ("__cdecl" if fc["tgt_cdecl"] else "__stdcall")
            store = ("__declspec(dllimport) " if fc["kind"] == "import" else "extern ")
            decl = store + "int %s %s(%s);\n" % (tconv, fc["tname"], _args(len(fc["args"])))
            decl += "".join("extern int g_%x;\n" % a for a in sorted(fc.get("globals", ())))
            expr = fc["wrap"] % ("%s(%s)" % (fc["tname"], ", ".join(fc["args"])))
            for conv, sym, na in _self_variants(name, retbytes, fc["ninc"]):
                cands.append((WIN + decl + "int %s %s(%s){ return %s; }"
                              % (conv, name, _args(max(na, fc["ninc"])), expr), sym))
        elif fc is None:
            seq = _call_seq(ins, base, byva, iat)         # multi-call: independent forward sequence
            le = _leaf_expr(ins) if seq is None else None  # call-free leaf: arg arith / field deref
            if seq is not None:
                decls = ""
                for k, (kind, tname, args, cdecl) in enumerate(seq["calls"]):
                    tconv = "__cdecl" if cdecl else "__stdcall"
                    store = "__declspec(dllimport) " if kind == "import" else "extern "
                    decls += store + "int %s %s(%s);\n" % (tconv, tname, _args(len(args)))
                decls += "".join("extern int g_%x;\n" % a for a in sorted(seq["globals"]))
                rc = seq.get("ret_call")
                if rc is None:                            # no saved result: last call is the return
                    body = "".join("%s(%s); " % (c[1], ", ".join(c[2])) for c in seq["calls"][:-1])
                    last = seq["calls"][-1]
                    body += "return %s(%s);" % (last[1], ", ".join(last[2]))
                else:                                     # a saved call result, returned after the rest
                    parts = ["r = %s(%s);" % (c[1], ", ".join(c[2])) if k == rc
                             else "%s(%s);" % (c[1], ", ".join(c[2]))
                             for k, c in enumerate(seq["calls"])]
                    body = "int r; " + " ".join(parts) + " return r;"
                for conv, sym, na in _self_variants(name, retbytes, seq["ninc"]):
                    cands.append((WIN + decls + "int %s %s(%s){ %s }"
                                  % (conv, name, _args(max(na, seq["ninc"])), body), sym))
            elif le is not None:
                expr, ninc, gl = le
                decls = "".join("extern int g_%x;\n" % a for a in sorted(gl))
                for conv, sym, na in _self_variants(name, retbytes, ninc + 1):
                    cands.append((WIN + decls + "int %s %s(%s){ return %s; }"
                                  % (conv, name, _args(max(na, ninc + 1)), expr), sym))
    return cands


def _batch_units(items):
    """Assemble candidate units into ONE compile unit: prelude once, and each
    function's call target(s) renamed uniquely (name_b<i>) so no two functions'
    extern decls collide. Calls are rel32/IAT masked at verify time, so renaming
    the target does NOT change the emitted bytes -- only avoids C redefinition
    clashes. Function (export) symbols stay intact for the byte-check."""
    out = [WIN.rstrip("\n")]
    for idx, csrc in items:
        decls, body = split_unit(csrc)
        for d in decls:
            d2 = re.sub(r"^__declspec\([^)]*\)\s*", "", d)   # skip storage attr's own '('
            m = re.search(r"\b(\w+)\s*\(", d2)
            if m:
                nm = m.group(1); new = "%s_b%d" % (nm, idx)
                d = re.sub(r"\b%s\b" % re.escape(nm), new, d, count=1)
                body = re.sub(r"\b%s\b" % re.escape(nm), new, body)
            out.append(d)
        out.append(body)
    return "\n".join(out) + "\n"


def emit_module(module, crows):
    """Batch clean-C for all 'c'-route rows of a module in ONE cl invocation
    (~1 spawn/module instead of thousands). Returns {rva: (status, csrc, sym, fl,
    note)} with status in byte-exact|draft|None. Falls back to per-function
    emit_best if the batch will not compile (rare: an odd body)."""
    prim = {}                                             # rva -> (csrc, sym): primary candidate
    rowmap = {r["rva"]: r for r in crows}
    for r in crows:
        nm = r["name"]
        if nm and not re.match(r"^[A-Za-z_]\w*$", nm):
            continue                                      # C++ mangled / non-identifier export: not C
                                                          # (nm is None -> internal fn, gen() names it sub_<rva>)
        try:
            cs = gen(module, r)
        except Exception:
            cs = []
        if cs:
            prim[r["rva"]] = cs[0]                         # variants are byte-identical; [0] suffices
    res = {r["rva"]: (None, None, None, None, None) for r in crows}
    if not prim:
        return res
    # Compile in CHUNKS, not one giant translation unit. A module like mfc40 has ~2,900 c-route
    # candidates; one TU that big is slow and, if it fails to compile, forced a per-function
    # fallback across ALL of them (thousands of cl spawns -> the sweep wedged). Chunking bounds
    # each compile and limits a compile failure's blast radius to its own chunk.
    rvas = list(prim.keys())
    CHUNK = 200
    for c0 in range(0, len(rvas), CHUNK):
        chunk = rvas[c0:c0 + CHUNK]
        tu = _batch_units([(i, prim[chunk[i]][0]) for i in range(len(chunk))])
        try:
            r = cc32.compile_c(tu, flags="/c /O1")
            funcs = cc32.Obj(r["obj"]).functions() if "obj" in r else None
        except Exception:
            funcs = None
        if funcs is None:                                 # this chunk won't compile -> per-fn (bounded)
            for rva in chunk:
                res[rva] = emit_best(module, rowmap[rva])
            continue
        for rva in chunk:
            csrc, sym = prim[rva]
            if sym not in funcs:
                continue                                  # leave as no-candidate
            ob, om = funcs[sym]
            sb, sm = cc32.shipped_pe_function(module, rva)
            if segdiff.diff(ob, om, sb, sm)["exact"]:
                res[rva] = ("byte-exact", csrc, sym, "/c /O1", None)
            else:
                res[rva] = ("draft", csrc, sym, "/c /O1",
                            "compiles + calls the shipped target; codegen differs (review)")
    return res


def try_emit(module, row):
    ok, csrc, sym, fl, _ = emit_best(module, row)
    return (ok == "byte-exact"), (csrc if ok == "byte-exact" else None), sym, fl


def emit_best(module, row):
    """Best clean-C result for a function:
      ('byte-exact', csrc, sym, flags, None)  -- oracle-confirmed, auto-land
      ('draft',      csrc, sym, flags, note)  -- compiles + calls the right target
                                                 but not byte-identical: a review
                                                 draft (functionally the intent),
                                                 NOT auto-landed
      (None, None, None, None, None)          -- no usable candidate
    A draft is safe to hand a human to confirm; it is never treated as done."""
    draft = None
    sb = sm = None
    try:
        cands = gen(module, row)
    except Exception:
        cands = []                                        # never abort a module on one odd fn
    for csrc, sym in cands:
        for fl in ("/c /O1",):                            # win95 built /O1; /O2 stragglers stay floor-covered
            try:
                r = cc32.compile_c(csrc, flags=fl)
                if "obj" not in r:
                    continue
                funcs = cc32.Obj(r["obj"]).functions()
                if sym not in funcs:
                    continue
                ob, om = funcs[sym]
                if sb is None:
                    sb, sm = cc32.shipped_pe_function(module, row["rva"])
                if segdiff.diff(ob, om, sb, sm)["exact"]:
                    return "byte-exact", csrc, sym, fl, None
                if draft is None:
                    draft = (csrc, sym, fl)
            except Exception:
                continue
    if draft:
        return "draft", draft[0], draft[1], draft[2], "compiles + calls the shipped target; codegen differs (review)"
    return None, None, None, None, None
