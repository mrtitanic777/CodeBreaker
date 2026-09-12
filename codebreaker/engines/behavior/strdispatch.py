#!/usr/bin/env python3
r"""strdispatch.py -- recognize string-comparison dispatch (a chain of strcmp branches).

A very common command/verb handler compares an input string against a series of literals and
branches on each -- `if (!strcmp(s,"open")) ...; else if (!strcmp(s,"print")) ...`. cfgstruct
recovers the branches, but the MEANING is the keyword->handler mapping, which is only clear when
the compared literals are surfaced together. This traces the function, finds the calls to the
strcmp family whose other argument is a recovered string literal, and reconstructs the dispatch as
a readable if/else-if chain over those keywords -- turning a wall of compares into the verb table
the author wrote.
"""
from codebreaker.engines.behavior import tracer, interproc as ip

_STRCMP = {"strcmp", "strncmp", "_strcmpi", "stricmp", "_stricmp", "strcmpi", "strcasecmp",
           "lstrcmpA", "lstrcmpW", "lstrcmpiA", "lstrcmpiW", "wcscmp", "_wcsicmp", "wcsicmp",
           "CompareStringA", "CompareStringW"}


def _is_strcmp(name):
    return name in _STRCMP or name.rstrip("AW") in {n.rstrip("AW") for n in _STRCMP}


def recognize_from_calls(calls, resolve):
    """[(cmpfunc, keyword, arg_index)] for each strcmp-against-a-literal in a trace's call records.

    calls: [{'api'|'name', 'args_stack':[...]}]; resolve(value)->literal string or None."""
    out = []
    for c in calls:
        nm = (c.get("api", "").split("!")[-1]) or c.get("name", "")
        if not _is_strcmp(nm):
            continue
        for i, v in enumerate(c.get("args_stack") or []):
            lit = resolve(v)
            if lit and (lit.startswith('"') or lit.startswith('L"')):
                kw = lit[lit.index('"') + 1:lit.rindex('"')]
                out.append((nm, kw, i))
                break
    return out


def reconstruct(module, func=None, arity=4):
    """Recover a string-dispatch function as an if/else-if chain over the compared keywords."""
    va = int(func, 0) if isinstance(func, str) else int(func)
    _, va0, _c, _s, _f, _sha, _imp = tracer._load(module)
    try:
        tr = tracer.trace_function(module, func=hex(va), args=ip._SENT[:arity],
                                   stub_externals=True, insn_cap=8000)
    except Exception as e:
        return {"reconstructed": False, "reason": "trace error: %r" % e}
    from codebreaker.engines.behavior import datarec
    entries = recognize_from_calls(tr.get("calls", []), lambda v: datarec.render(module, v))
    if len(entries) < 2:                                     # a dispatch is a CHAIN of compares
        return {"reconstructed": False, "reason": "not a string dispatch (<2 keyword compares)"}
    off = va - va0
    fn = entries[0][0]
    lines = ["/* string dispatch over %d keyword(s) */" % len(entries)]
    lines.append("int sub_%x(const char *s)" % off)
    lines.append("{")
    for k, (cmpf, kw, _i) in enumerate(entries):
        cond = 'if (%s(s, "%s") == 0)' % (fn, kw)
        lines.append("    %s%s" % ("" if k == 0 else "else ", cond))
        lines.append("        return handle_%s();  /* case \"%s\" */" % (_ident(kw), kw))
    lines.append("    return default_handler();")
    lines.append("}")
    return {"reconstructed": True, "verified": None, "kind": "string-dispatch",
            "keywords": [kw for _f, kw, _i in entries], "emitted_c": "\n".join(lines), "func_va": va}


def _ident(kw):
    return "".join(ch if ch.isalnum() else "_" for ch in kw)[:24] or "empty"
