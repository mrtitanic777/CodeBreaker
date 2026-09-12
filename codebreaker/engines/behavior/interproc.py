#!/usr/bin/env python3
r"""interproc.py -- interprocedural reconstruction of caller ("orchestrator") functions.

The dominant floor cause is functions that CALL other functions: the leaf engines model a
function as self-contained (pure / struct / globals), so a caller whose meaning IS its sequence
of calls can't be expressed. This reconstructs it as a caller -- trace it with sentinel argument
values, capture each call's target + arguments (tracer args_stack) + observed return, resolve the
target to a name (an SDK import via symdb, else sub_<off>), and emit the call sequence with
arguments mapped back to the caller's own parameters. Call arg counts are exact via the SDK proto
DB (symdb.arity). Tier: 'structural' -- the call structure is observed, not oracle-re-validated
like the leaf tiers.
"""
import re
from codebreaker.engines.behavior import tracer

_SENT = [0xA5A50000 + i for i in range(6)]        # per-arg markers -> map call args back to aN
_RET = 0xdead0000
# a virtual dispatch: call through a vtable slot  ->  call [this + 0xN]  (COM / C++)
_VCALL = re.compile(r"call\s+(?:dword ptr\s+)?\[(e[a-z]{2})\s*\+\s*(0x[0-9a-f]+)\]")


def _is_junk(v):                                   # ret sentinel, external-stub landing, stale stack
    return v == _RET or (v & 0xFFF00000) == 0xE1000000 or 0x200000 <= v < 0x210000


def _demangle_short(nm):
    if nm and nm.startswith("?"):                  # MSVC-mangled C++ export -> CFoo::AddRef
        try:
            from codebreaker.engines.symbol import demangle
            return demangle.short(nm) or nm
        except Exception:
            return nm
    return nm


def _name_target(cr, va0, mnem=None):
    if cr.get("api"):                              # "KERNEL32.dll!lstrlenA" -> "lstrlenA"
        return _demangle_short(cr["api"].split("!")[-1])
    if mnem:                                        # virtual dispatch through a vtable slot (COM/C++)
        m = _VCALL.search(mnem)
        if m:
            return "vcall_%s" % m.group(2)          # virtual method at vtable offset 0xN
    t = cr.get("resolved_target") or cr.get("target")
    if t is None:
        return None
    return "sub_%x" % (t - va0 if t >= va0 else t)


def _arg(v):
    for i, s in enumerate(_SENT):
        if v == s:
            return "a%d" % i
    if v == 0xffffffff:
        return "-1"
    if 0xB1B10000 <= v <= 0xB1B1FFFF:      # an unnamed prior-call return that flowed here
        return "result"
    return "0x%x" % v


def _call_args(stack, argc):
    real = []
    for v in stack:
        if _is_junk(v):
            break
        real.append(v)
    if argc is not None:
        real = real[:argc]
    else:
        while real and real[-1] == 0:
            real.pop()
    return real


def reconstruct(module, func=None, arity=4):
    """recon-compatible: func is a hex VA string. Returns a result dict or {'reconstructed': False}.

    v2 -- value flow: external calls return a UNIQUE per-call marker (stub_ret_unique), so when a
    later call's argument or the function's return equals an earlier call's marker, we know that
    value IS the earlier call's result and emit `rN = f(...)` / `g(rN)` / `return rN`."""
    from codebreaker.engines.symbol import symdb
    va = int(func, 0) if isinstance(func, str) else int(func)
    _, va0, _code, _sec, _funcs, _sha, _imp = tracer._load(module)
    try:                                              # pin the parameter count from all call sites
        from codebreaker.engines.behavior import arity as aritymod
        con = aritymod.consensus(module, va)
        if con and con.get("argc") and 0 < con["argc"] <= 12:
            arity = max(arity, con["argc"])
    except Exception:
        pass
    try:
        tr = tracer.trace_function(module, func=hex(va), args=_SENT[:arity],
                                   stub_externals=True, stub_ret_unique=True, insn_cap=8000)
    except Exception as e:
        return {"reconstructed": False, "reason": "trace error: %r" % e}
    r = emit_from_trace(tr, va, va0, arity, module=module)
    if r.get("reconstructed"):                            # verify: single-path caller -> faithful
        from codebreaker.engines.behavior import structverify
        r["verified"] = (structverify.verify(module, va) == "verified")
    return r


def emit_from_trace(tr, va, va0, arity, module=None, names=None):
    """Emit a caller reconstruction from an already-captured trace (shared by interproc + envprov).

    `names` (optional): {callee_off: proposed_name} from the bottom-up v3 pass, used to label an
    internal `sub_x(...)` call with the callee's recovered purpose (`/* CreateComObject */ ...`)."""
    from codebreaker.engines.symbol import symdb
    from codebreaker.engines.behavior import datarec
    calls = [c for c in tr.get("calls", []) if c.get("resolved_target") or c.get("target")]
    if not calls:
        return {"reconstructed": False, "reason": "no resolved calls (not a caller)"}
    ret = tr.get("ret")
    sent2r = {c["ret_sentinel"]: "r%d" % i for i, c in enumerate(calls) if c.get("ret_sentinel")}
    later_vals = [v for c in calls for v in (c.get("args_stack") or [])]
    used = {name for s, name in sent2r.items() if s in later_vals or s == ret}

    ptype = {}      # caller param index -> recovered C type (from the callee arg it's passed to)
    rtype = {}      # result index -> callee return type
    which_arg = {}  # caller param index -> the sentinel value (to look up in a call)

    def argname(v):
        if v in sent2r:
            return sent2r[v]
        if module is not None:                            # resolve string / data-pointer literals
            lit = datarec.render(module, v)
            if lit is not None:
                return lit
        return _arg(v)

    mnem = tr.get("mnemonics", {})
    lines = []
    for i, c in enumerate(calls):
        nm = _name_target(c, va0, mnem.get("0x%x" % c.get("pc", 0)))
        if not nm:
            continue
        argc = c.get("argc")
        sret, sparams = _sig(symdb, nm)
        if argc is None and c.get("api"):
            argc = symdb.arity(nm)                  # exact arg count from the SDK proto
        real = _call_args(c.get("args_stack") or [], argc)
        for p, v in enumerate(real):                # TYPE PROPAGATION: a caller param takes the
            for k, s in enumerate(_SENT):           # type of the callee arg position it's passed to
                if v == s and p < len(sparams) and k not in ptype:
                    ptype[k] = sparams[p]
        if ("r%d" % i) in used and sret and sret != "void":
            rtype[i] = sret
        from codebreaker.engines.behavior import constants
        rendered = [constants.render(nm, p, v) or argname(v) for p, v in enumerate(real)]
        # printf-family: type the varargs from the recovered format string literal
        from codebreaker.engines.behavior import fmtargs
        fi = fmtargs.fmt_arg_index(nm)
        if fi is not None and fi < len(rendered) and '"' in rendered[fi]:
            lits = rendered[fi]
            fmt = lits[lits.index('"') + 1:lits.rindex('"')]
            tv = fmtargs.type_varargs(nm, fmt)
            if tv:
                for k, ty in enumerate(tv[1]):
                    ai = fi + 1 + k
                    if ai < len(rendered) and not rendered[ai].startswith("/*"):
                        rendered[ai] = "/*%s*/%s" % (ty, rendered[ai])
        # thiscall/fastcall: ecx carries `this` (or fastcall arg0). Prepend it for a virtual call,
        # or when ecx holds a caller parameter -> a clear register-passed argument.
        ecx = c.get("ecx")
        # only thiscall targets carry `this` in ecx: a virtual call, or an INTERNAL function whose
        # ecx holds a caller value. Known stdcall/cdecl SDK APIs (c["api"]) never do -- skip them.
        if ecx is not None and (nm.startswith("vcall_")
                                or (not c.get("api") and (ecx in _SENT or ecx in sent2r))):
            rendered = ["/*this=*/%s" % argname(ecx)] + rendered
        label = ""                                            # v3: label an internal callee's purpose
        if names and nm.startswith("sub_"):
            try:
                coff = int(nm[4:], 16)
            except ValueError:
                coff = None
            if coff in names and names[coff]:
                label = "/* %s */ " % names[coff]
        call = "%s%s(%s)" % (label, nm, ", ".join(rendered))
        if ("r%d" % i) in used:
            lines.append("    %s r%d = %s;" % (rtype.get(i, "unsigned"), i, call))
        else:
            lines.append("    %s;" % call)
    if not lines:
        return {"reconstructed": False, "reason": "calls unresolved to names"}
    retexpr = argname(ret) if ret is not None else "0"
    off = va - va0
    # interprocedural unification: fill params still untyped from what internal callees expect
    if module is not None and any(i not in ptype for i in range(arity)):
        try:
            from codebreaker.engines.behavior import typeunify
            uni = typeunify.param_types(module, va, arity)
            for i in range(arity):
                if i not in ptype and i < len(uni) and uni[i] != "unsigned":
                    ptype[i] = uni[i]
        except Exception:
            pass
    params = ", ".join("%s a%d" % (ptype.get(i, "unsigned"), i) for i in range(arity))
    from codebreaker.engines.behavior import funcname               # hypothesized name from APIs
    hint = funcname.suggest_from_calls(calls)
    tag = ("/* likely: %s */\n" % hint) if hint else ""
    conv = ""                                                       # real calling convention keyword
    rt = "unsigned"                                                  # recovered return type
    if module is not None:
        try:
            from codebreaker.engines.behavior import callconv
            cc = callconv.detect(module, va)
            if cc and cc["conv"] in ("stdcall", "thiscall", "fastcall"):
                conv = "__%s " % cc["conv"]
        except Exception:
            pass
        try:
            from codebreaker.engines.behavior import rettype
            rt = rettype.infer(module, va) or "unsigned"
        except Exception:
            pass
    retline = "    return;" if rt == "void" else "    return %s;" % retexpr
    src = "%s%s %ssub_%x(%s)\n{\n%s\n%s\n}\n" % (tag, rt, conv, off, params, "\n".join(lines), retline)
    if module is not None:                                # wrap __try/__except if the frame is present
        from codebreaker.engines.behavior import seh
        h = seh.detect(module, va)
        if h is not None:
            src = seh.wrap(src, h, va0)
    return {"reconstructed": True, "verified": None, "ncalls": len(lines),
            "emitted_c": src, "func_va": va}


def _sig(symdb, name):
    """(ret_type, [param_types]) from the SDK proto, or (None, [])."""
    s = symdb.signature(name)
    if not s or "(" not in s:
        return None, []
    head, rest = s.split("(", 1)
    params = rest.rsplit(")", 1)[0].strip()
    plist = [] if params in ("", "void", "VOID") else [p.strip() for p in params.split(",")]
    ret = head.rsplit(" ", 1)[0].strip() if " " in head else None
    return ret, plist
