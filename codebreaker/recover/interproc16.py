#!/usr/bin/env python3
r"""interproc16.py -- reconstruct 16-bit callers as their (static) Win16 call sequence.

The 16-bit leaf synth declines any function that calls imports -- but those are the majority: a
Win16 function's meaning IS its sequence of API calls. Dynamic tracing under-captures (a WndProc
never hits its message-specific calls in a bare sandbox), so this recovers the call sequence
STATICALLY: it disassembles the function (16-bit), and for every call instruction resolves the
target through the NE relocation table -- an IMPORTORDINAL/IMPORTNAME fixup names the Win16 API
(via the ordinal oracle), an INTERNALREF names the sub. The result is the ordered, named call list
the author wrote (CreateWindow -> ShowWindow -> UpdateWindow), independent of execution path.
"""


_SENT = [0xA5A0 + i for i in range(12)]           # per-parameter markers (distinctive 16-bit words)


def _arg(v, strfn=None):
    for i, s in enumerate(_SENT):
        if v == s:
            return "a%d" % i
    if v == 0xFFFF:
        return "-1"
    if strfn:                                       # a DS offset that resolves to a string literal
        lit = strfn(v)
        if lit is not None:
            return lit
    return "0x%x" % v


def _meaningful(v, strfn):
    return v in _SENT or v == 0xFFFF or (strfn and strfn(v) is not None)


def _render_args(args_stack, strfn=None):
    """Left-to-right args from the captured Pascal window. args_stack[0] = rightmost (last-pushed).

    Take the contiguous arg run from the top of the stack, then trim trailing bare-constant words
    (the caller's frame below the real args) -- keeping constants only when they precede a sentinel
    or string, so a real `MB_OK` before an hWnd survives but trailing zeros don't."""
    pre = []
    for v in args_stack:
        if _meaningful(v, strfn) or v < 0x2000 or 0xA000 <= v <= 0xAFFF:
            pre.append(v)
        else:
            break
    while pre and not _meaningful(pre[-1], strfn):  # drop trailing noise (non-sentinel/string consts)
        pre.pop()
    return [_arg(v, strfn) for v in reversed(pre)]


def _render_proto(api, args_stack, strfn):
    """Proto-aware arg rendering: exact param count, string resolution only at string params."""
    from codebreaker.symbols import win16_prototypes as win16proto
    p = win16proto.proto(api)
    if not p:
        return _render_args(args_stack, None)                # no proto -> heuristic, no string (unsafe)
    n = len(p["params"])
    src = list(reversed(args_stack[:n]))                     # top N words (rightmost..leftmost) -> source order
    out = []
    for i, v in enumerate(src):
        if strfn and win16proto.is_str_param(api, i):        # resolve a string ONLY at a string param
            lit = strfn(v)
            out.append(lit if lit is not None else _arg(v))
        else:
            out.append(_arg(v))
    return out


def _observed_args(path, seg, off, ordinal, arity, strfn=None):
    """{api_name: [rendered-arg-list, ...]} in call order, from a sentinel-arg dynamic trace."""
    from codebreaker.emulate import trace16 as tracer16
    try:
        if ordinal is not None:
            tr = tracer16.trace_function16(path, ordinal=ordinal, args=_SENT[:arity])
        else:
            tr = tracer16.trace_function16(path, seg=seg, offset=off, args=_SENT[:max(arity, 4)])
    except Exception:
        return {}
    by = {}
    for h in tr.get("imports_hit", []):
        api = h.get("api")
        if api:
            by.setdefault(api, []).append(_render_proto(api, h.get("args_stack", []), strfn))
    return by


def _md16():
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    return md


_SYS_ALIAS = {"KERNEL": "KRNL386", "KRNL286": "KRNL386"}
_SYS_EXPORTS = {}


def _sys_exports(mod):
    """{ordinal: NAME} for a Win16 system module's OWN exports (USER/GDI/KRNL386/COMMDLG/...),
    read straight from its NE name tables in the corpus -- the ground-truth ordinal oracle when the
    SDK import-lib map (symdb.ord16) is absent. Cached per module; '' path -> empty."""
    key = mod.upper()
    if key in _SYS_EXPORTS:
        return _SYS_EXPORTS[key]
    out = {}
    try:
        import os as _os
        from codebreaker.base import registry as cores
        from codebreaker.formats.ne import NEFile
        from codebreaker.recover import recon16
        tgt = cores.get("target")
        base = _SYS_ALIAS.get(key, key)
        for ext in (".EXE", ".DLL", ".DRV", ".386", ".VXD", ""):
            try:
                p = tgt.resolve(base + ext)
            except Exception:
                p = None
            if p and _os.path.exists(p):
                out = recon16.export_names(NEFile(p))
                break
    except Exception:
        out = {}
    _SYS_EXPORTS[key] = out
    return out


def _namer(nef):
    """reloc-record -> 'MODULE.API' / 'sub_seg_entry', using the Win16 ordinal oracle."""
    try:
        modrefs = nef.module_refs()
    except Exception:
        modrefs = []
    ord16 = {}
    try:
        from codebreaker.symbols import symdb
        ord16 = symdb._load().get("ord16", {})
    except Exception:
        ord16 = {}

    def name(r):
        k = r.get("kind")
        if k == "IMPORTORD":
            mi = r.get("module")
            mod = modrefs[mi - 1] if mi and 0 < mi <= len(modrefs) else "MOD%s" % mi
            o = r.get("ordinal")
            nm = ord16.get(mod.upper(), {}).get(o)
            if not nm:                                        # SDK OMF map absent -> corpus export table
                nm = _sys_exports(mod).get(o)
            return "%s.%s" % (mod, nm or ("@%d" % o))
        if k == "IMPORTNAME":
            mi = r.get("module")
            mod = modrefs[mi - 1] if mi and 0 < mi <= len(modrefs) else "MOD%s" % mi
            try:
                return "%s.%s" % (mod, nef.imported_name(r.get("name_off", 0)))
            except Exception:
                return "%s.?" % mod
        if k == "INTERNAL":
            return "sub_%s_%x" % (r.get("seg"), r.get("entry", 0))
        return None
    return name


def _func_range(nef, seg_idx, off):
    """(start, end) file-relative code range of the function at (seg, off): to the next entry / retf."""
    segs = {s["idx"]: s for s in nef.segments()}
    s = segs[seg_idx]
    seg_start, seg_len = s["file_off"], s["length"]
    # end: the next entry-point offset in this segment above `off`, else the segment end
    nexts = sorted(o for e in nef.entries() if e.get("seg") == seg_idx
                   for o in [e.get("offset", 0)] if o > off)
    end_off = nexts[0] if nexts else seg_len
    return seg_start + off, seg_start + min(end_off, seg_len), off


def all_functions(nef):
    """Every 16-bit function start (seg, off): entry table + INTERNALREF targets + near-call targets."""
    segs = {s["idx"]: s for s in nef.segments()}
    funcs = set()
    for e in nef.entries():
        if e.get("kind") in ("FIXED", "MOVEABLE") and segs.get(e.get("seg"), {}).get("kind") == "CODE":
            funcs.add((e["seg"], e["offset"]))
    for s in nef.segments():
        if s["kind"] != "CODE":
            continue
        for r in nef.relocations(s):
            if r.get("kind") == "INTERNAL" and r.get("seg") not in (None, 0xFF):
                if segs.get(r["seg"], {}).get("kind") == "CODE":
                    funcs.add((r["seg"], r.get("entry", 0)))
        funcs |= _near_call_targets(nef, s)                  # purely near-called internals
    return sorted(funcs)


def _near_call_targets(nef, s):
    """Segment-internal near-call (E8 rel16) targets referenced by >= 2 distinct call sites.

    16-bit linear disasm drifts, but a target that two different `E8` sites agree on is a real
    function start (a stray 0xE8 in data won't coincide), so this recovers near-called internals."""
    import struct
    code = nef.data[s["file_off"]:s["file_off"] + s["length"]]
    n = len(code)
    sites = {}
    for i in range(n - 2):
        if code[i] == 0xE8:                                  # call rel16
            rel = struct.unpack_from("<h", code, i + 1)[0]
            tgt = (i + 3 + rel) & 0xFFFF
            if 0 <= tgt < n:
                sites.setdefault(tgt, set()).add(i)
    return {(s["idx"], t) for t, srcs in sites.items() if len(srcs) >= 2}


def _func_range_int(nef, seg_idx, off):
    """(lo_off, hi_off, base_off): the function's segment-offset range, bounded by the NEXT function
    start in the same segment (entry or internal-call target) -- tighter than entry-to-entry."""
    seg_len = {s["idx"]: s["length"] for s in nef.segments()}.get(seg_idx, 0x10000)
    starts = sorted(o for (sg, o) in all_functions(nef) if sg == seg_idx and o > off)
    hi = starts[0] if starts else seg_len
    return off, min(hi, seg_len), off


def reconstruct(path, ordinal):
    """Reconstruct an EXPORTED 16-bit caller (by ordinal) as its call sequence."""
    from codebreaker.formats.ne import NEFile
    nef = NEFile(path)
    ents = nef.entries()
    e = ents[ordinal - 1] if 0 < ordinal <= len(ents) else None
    if not e or e.get("kind") == "UNUSED":
        return {"reconstructed": False, "reason": "no live entry"}
    return reconstruct_at(path, e["seg"], e["offset"], ordinal=ordinal, _nef=nef)


def reconstruct_at(path, seg_idx, off, ordinal=None, _nef=None):
    """Reconstruct the caller at (seg, offset) -- works for internal functions too."""
    from codebreaker.formats.ne import NEFile
    from codebreaker.recover import recon16
    nef = _nef or NEFile(path)
    segs = {s["idx"]: s for s in nef.segments()}
    if segs.get(seg_idx, {}).get("kind") != "CODE":
        return {"reconstructed": False, "reason": "not a code segment"}
    _lo, _hi, base_off = _func_range_int(nef, seg_idx, off)
    end_off = _hi
    # The relocations pinpoint every import / internal reference at an exact segment offset -- more
    # reliable than 16-bit linear disassembly (which drifts). Take the fixups in the function's
    # offset range, in program order; in a code segment these are overwhelmingly far calls.
    namer = _namer(nef)
    calls = []
    for r in sorted(nef.relocations(segs[seg_idx]), key=lambda r: r["offset"]):
        if base_off <= r["offset"] < end_off and r.get("kind") in ("IMPORTORD", "IMPORTNAME", "INTERNAL"):
            nm = namer(r)
            if nm:
                calls.append((r["offset"], nm))
    if len(calls) < 1:
        return {"reconstructed": False, "reason": "no import/internal fixups in range (pure leaf)"}
    if ordinal is not None:
        mod, nm = recon16._name(nef, ordinal)
        fname = nm or ("sub_%d" % ordinal)
        label = "%s.%s" % (mod, nm or ("@%d" % ordinal))
    else:                                                     # internal function (not exported)
        fname = "sub_%d_%x" % (seg_idx, off)
        label = "%s!%s" % (nef.module_name(), fname)
    aw = recon16._arity_words(nef, seg_idx, off)
    arity = aw if aw is not None else 0
    params = ", ".join("WORD a%d" % i for i in range(arity)) if arity else "void"
    # recover each call's ARGUMENTS from a sentinel-arg dynamic trace (FIFO per API, in call order).
    # The DS-offset string resolver is applied ONLY at string-typed parameters (win16proto), so a
    # numeric flag that coincides with a DGROUP offset never becomes a bogus literal.
    strfn = None
    try:
        from codebreaker.recover import datarec16
        strfn = datarec16.resolver(nef)
    except Exception:
        strfn = None
    observed = _observed_args(path, seg_idx, off, ordinal, max(arity, 4), strfn)
    queues = {k: list(v) for k, v in observed.items()}
    lines = []
    for _o, nm2 in calls:
        argstr = ""
        q = queues.get(nm2)
        if q:
            argstr = ", ".join(q.pop(0))                     # next observed call of this API
        lines.append("    %s(%s);" % (nm2.replace(".", "_"), argstr))
    body = "\n".join(lines)
    napis = sum(1 for _a, n in calls if "." in n)
    # if this function is a Win16 callback (WndProc/DlgProc/hook/enum), emit its real signature
    from codebreaker.recover import callbacksig16
    kind = callbacksig16.classify(nm, [c[1] for c in calls])
    if kind:
        ret, sig, body2 = callbacksig16.apply(fname, kind, body)
        msgs = ""
        if kind in ("wndproc", "dlgproc"):                    # which WM_ messages it dispatches on
            try:
                from codebreaker.recover import wmdispatch16
                wm = wmdispatch16.messages_for(nef, seg_idx, base_off, end_off)
                if wm:
                    msgs = "\n/* handles: %s */" % ", ".join(sorted(wm.values()))
            except Exception:
                pass
        src = "/* Win16 %s: %d call(s) */%s\n%s\n{\n%s\n    return 0;\n}\n" % (kind, len(calls), msgs, sig, body2)
    else:
        src = ("/* Win16 caller: %d call(s) */\nWORD %s(%s)\n{\n%s\n    return 0;\n}\n"
               % (len(calls), fname, params, body))
    return {"reconstructed": True, "verified": None, "ncalls": len(calls), "napis": napis,
            "calls": [n for _a, n in calls], "emitted_c": src, "label": label}


def reconstruct_module(path):
    """Reconstruct every 16-bit function (exports + internal) as a caller. {'ok','n','functions'}."""
    from codebreaker.formats.ne import NEFile
    nef = NEFile(path)
    out = []
    for seg, off in all_functions(nef):
        r = reconstruct_at(path, seg, off, _nef=nef)
        if r.get("reconstructed"):
            out.append(r)
    return {"ok": bool(out), "n": len(out), "functions": out}
