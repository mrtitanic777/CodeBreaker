#!/usr/bin/env python3
r"""structure2.py — deeper control-flow structuring for the raiser.

Drop-in replacement for raiser.structure(body): takes the raiser's already-rewritten body
(globals named, calls externs, registers folded — structure runs LAST in candidate()) and
folds the lift's goto-graph into idiomatic C that MSVC 2.0 recompiles to the SAME branch
layout, or returns None when the shape isn't provably equivalent (left out of scope).

Handled shapes (all SOUND — only reductions that preserve control flow):
  * guard-skip           if (G) goto L; BODY; L:            -> if (!(G)) { BODY }
      (generalized: BODY may be several stmts or nested braced if-blocks; label L is kept
       if other gotos still target it, dropped when this was its only reference)
  * if-else (two-way)    if (G) goto L1; A; goto L2; L1: B; L2:
                                                            -> if (!(G)) { A } else { B }
  * sequential guard-skips     applied repeatedly to a fixpoint
  * redundant goto             goto L; L:      (jump to the very next label) -> drop the goto
  * dead label                 L: with no goto targeting it anywhere         -> drop the label
  * unreachable tail           a stmt sitting after an unconditional goto/return, before the
                               next label, can never execute                -> drop it
  * already-braced if-blocks   pass straight through (opaque, control flow untouched)

Deliberately NOT handled (return None — genuinely out of scope, never emit wrong C):
  * computed / indirect jumps  goto *(...)          (jump tables, tail-dispatch)
  * loops                      any goto to an earlier (backward) label
  * switch dispatch ladders    if(x==k) goto Lk; ... that don't reduce to the above
  * any residual goto/label the reductions can't eliminate

Integration: in raiser.candidate(), replace `st = structure(rw)` with
`st = structure2.structure2(rw)` (or `raiser.structure = structure2.structure2`). The rest of
candidate() (u-type expansion, void detection, decls) is unchanged.
"""
import re

_LABEL_RE   = re.compile(r"^(\w+):$")
_IFGOTO_RE  = re.compile(r"^if \((.+)\) goto (\w+);$")
_GOTO_RE    = re.compile(r"^goto (\w+);$")


def _parse(body):
    """Tokenize body into a flat node list. Each node is a dict with 'kind' in
    {stmt, label, goto, ifgoto} plus 'text' (verbatim, for stmt/label re-emission) and,
    for branches, 'cond'/'target'. Multi-line braced blocks (recovered `if(c){...}`) are
    kept whole as a single opaque 'stmt'. Returns None on a computed/indirect jump."""
    lines = body.split("\n")
    nodes = []
    i = 0
    while i < len(lines):
        raw = lines[i]
        s = raw.strip()
        if s == "":
            i += 1
            continue
        if s.startswith("goto *") or "goto *" in s:
            return None                                  # computed jump -> out of scope
        m = _IFGOTO_RE.match(s)
        if m:
            nodes.append({"kind": "ifgoto", "cond": m.group(1), "target": m.group(2), "text": raw})
            i += 1
            continue
        m = _GOTO_RE.match(s)
        if m:
            nodes.append({"kind": "goto", "target": m.group(1), "text": raw})
            i += 1
            continue
        m = _LABEL_RE.match(s)
        if m:
            nodes.append({"kind": "label", "name": m.group(1), "text": raw})
            i += 1
            continue
        # a braced block (recovered if / else) -> consume to matching brace, keep opaque
        depth = raw.count("{") - raw.count("}")
        if depth > 0:
            block = [raw]
            i += 1
            while i < len(lines) and depth > 0:
                block.append(lines[i])
                depth += lines[i].count("{") - lines[i].count("}")
                i += 1
            text = "\n".join(block)
            if "goto *" in text:
                return None
            nodes.append({"kind": "stmt", "text": text,
                          "is_return": False, "is_transfer": False})
            continue
        nodes.append({"kind": "stmt", "text": raw,
                      "is_return": s.startswith("return"),
                      "is_transfer": s.startswith("return")})
        i += 1
    return nodes


def _goto_count(nodes, name):
    """Total gotos targeting `name` anywhere — top-level branches AND gotos buried inside
    opaque braced stmt blocks (found by text scan). Used to decide if a label is dead."""
    n = 0
    pat = re.compile(r"\bgoto %s;" % re.escape(name))
    for nd in nodes:
        if nd["kind"] in ("goto", "ifgoto") and nd.get("target") == name:
            n += 1
        elif nd["kind"] == "stmt":
            n += len(pat.findall(nd["text"]))
    return n


def _label_index(nodes, name):
    for idx, nd in enumerate(nodes):
        if nd["kind"] == "label" and nd["name"] == name:
            return idx
    return None


def _all_stmt(nodes):
    return all(nd["kind"] == "stmt" for nd in nodes)


def _wrap(cond, inner_nodes, else_nodes=None):
    """Build an opaque structured stmt node: if (!(cond)) { inner } [else { else }]."""
    body = "\n".join(nd["text"] for nd in inner_nodes)
    text = "if (!(%s)) {\n%s\n}" % (cond, body)
    if else_nodes is not None:
        eb = "\n".join(nd["text"] for nd in else_nodes)
        text += " else {\n%s\n}" % eb
    return {"kind": "stmt", "text": text, "is_return": False, "is_transfer": False}


def _reduce_once(nodes):
    """One reduction step; returns a NEW node list if something changed, else None."""
    # 1. redundant goto to the immediately-following label
    for i in range(len(nodes) - 1):
        a, b = nodes[i], nodes[i + 1]
        if a["kind"] == "goto" and b["kind"] == "label" and a["target"] == b["name"]:
            return nodes[:i] + nodes[i + 1:]

    # 2. dead label (no goto targets it anywhere)
    for i, nd in enumerate(nodes):
        if nd["kind"] == "label" and _goto_count(nodes, nd["name"]) == 0:
            return nodes[:i] + nodes[i + 1:]

    # 3. unreachable stmt right after an unconditional transfer (goto / return)
    for i in range(len(nodes) - 1):
        a, b = nodes[i], nodes[i + 1]
        transfer = a["kind"] == "goto" or (a["kind"] == "stmt" and a.get("is_transfer"))
        if transfer and b["kind"] == "stmt":
            return nodes[:i + 1] + nodes[i + 2:]

    # 4. structuring an if-goto (forward only)
    for i, nd in enumerate(nodes):
        if nd["kind"] != "ifgoto":
            continue
        L = nd["target"]
        k = _label_index(nodes, L)
        if k is None or k <= i:
            continue                                     # backward = loop, or unresolved
        between = nodes[i + 1:k]

        # 4a. if-else: between = A(all stmt) + `goto L2` ; then B(all stmt) up to label L2
        if (len(between) >= 1 and between[-1]["kind"] == "goto"
                and _all_stmt(between[:-1]) and _goto_count(nodes, L) == 1):
            L2 = between[-1]["target"]
            m = _label_index(nodes, L2)
            if m is not None and m > k:
                B = nodes[k + 1:m]
                if _all_stmt(B):
                    A = between[:-1]
                    new = _wrap(nd["cond"], A, else_nodes=B)
                    # consume ifgoto..(just before L2 label); leave label L2 in place
                    return nodes[:i] + [new] + nodes[m:]

        # 4b. plain guard-skip: between all straight-line stmts
        if _all_stmt(between):
            new = _wrap(nd["cond"], between)
            rest = nodes[k:]                             # label L (+ tail) preserved
            if _goto_count(nodes, L) == 1:               # this was its only reference -> drop L
                rest = rest[1:]
            return nodes[:i] + [new] + rest

    return None


def structure2(body):
    """Structure the control flow of a rewritten lift body. Returns the structured C body
    (str) on success, or None when a residual goto/label/computed-jump means the shape isn't
    provably equivalent (out of scope)."""
    nodes = _parse(body)
    if nodes is None:
        return None                                      # computed jump

    # fixpoint of sound reductions
    for _ in range(400):
        nxt = _reduce_once(nodes)
        if nxt is None:
            break
        nodes = nxt

    # success only if fully structured: no residual control-flow tokens
    if any(nd["kind"] in ("goto", "ifgoto", "label") for nd in nodes):
        return None

    return "\n".join(nd["text"] for nd in nodes)
