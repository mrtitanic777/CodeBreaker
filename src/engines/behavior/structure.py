#!/usr/bin/env python3
r"""structure.py -- lift a goto/label CFG into structured if / if-else / while.

cfgstruct recovers correct control flow but emits it flat: every block gets a label and every
edge a `goto`. Correct, but not the authored source -- a human wrote `if`/`else`/`while`, and the
compiler lowered those to conditional jumps. This reverses that lowering for the REDUCIBLE cases
it can prove, and keeps goto for the rest (an irreducible edge in goto-form is right; a mis-lifted
`while` is wrong, so the bar to lift is a proof, not a guess):

  * if-then        cond skips a single-entry/single-exit forward region -> `if (!taken) { region }`
  * if-then-else   ... whose then-region ends by jumping past a second region -> `if/else`
  * pre-test while cond skips forward past a body that ends with a back edge to the cond -> `while`

The block model is abstract (addr, kind, taken-target, fall-target) with callbacks for the
predicate text and each block's body lines, so it is engine-independent and unit-testable.
"""

_NEG = {"==": "!=", "!=": "==", "<": ">=", ">=": "<", "<=": ">", ">": "<=",
        "< 0": ">= 0", ">= 0": "< 0"}


def _split_top(s, sep):
    """Split s on sep only at parenthesis depth 0."""
    parts, depth, last, i = [], 0, 0, 0
    while i < len(s):
        c = s[i]
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
        elif depth == 0 and s[i:i + len(sep)] == sep:
            parts.append(s[last:i]); i += len(sep); last = i; continue
        i += 1
    parts.append(s[last:])
    return parts


def negate(pred):
    """Negate a recovered predicate, applying De Morgan to && / || so compound conditions stay clean."""
    pred = pred.strip()
    for opjoin, dual in ((" || ", " && "), (" && ", " || ")):    # De Morgan on top-level operators
        parts = _split_top(pred, opjoin)
        if len(parts) > 1:
            return dual.join(negate(p.strip()) for p in parts)
    if pred.startswith("(") and pred.endswith(")") and len(_split_top(pred[1:-1], ")")) == 1:
        return "(%s)" % negate(pred[1:-1])                       # a wholly-parenthesized sub-expression
    for op in ("==", "!=", "<=", ">=", "<", ">"):
        key = " %s " % op                                        # spaced, so "<<" etc. aren't matched
        if key in pred:
            lhs, rhs = pred.split(key, 1)
            return "%s %s %s" % (lhs, _NEG[op], rhs)
    for tail in ("< 0", ">= 0"):
        if pred.endswith(tail):
            return pred[:-len(tail)] + _NEG[tail]
    return "!(%s)" % pred


class Blk:
    # extra slots carry engine payloads cfgstruct attaches: the block's rows (_body), a merged
    # short-circuit predicate (_predtext), and switch metadata (switch_reg / cases).
    __slots__ = ("addr", "kind", "taken", "fall", "_body", "_predtext", "switch_reg", "cases",
                 "switch_low", "switch_default")

    def __init__(self, addr, kind, taken=None, fall=None):
        self.addr, self.kind, self.taken, self.fall = addr, kind, taken, fall


def structure(blocks, predicate, body_lines, ret_line, indent="    "):
    """Emit structured C lines for an ordered block list.

    blocks      : [Blk] in ascending address order (kind in cond/goto/fall/ret/switch)
    predicate(b): the source-level predicate string for a cond block (taken == true)
    body_lines(b): [str] the non-control statements of the block (calls, assignments)
    ret_line(b) : the `return ...;` text for a ret block
    Returns (lines, used_labels): C lines, and the set of block addrs that still need a label.
    """
    idx = {b.addr: i for i, b in enumerate(blocks)}
    n = len(blocks)

    def succs(i):
        b = blocks[i]
        if b.kind == "cond":
            return [x for x in (idx.get(b.fall), idx.get(b.taken)) if x is not None]
        if b.kind == "goto":
            return [x for x in (idx.get(b.taken),) if x is not None]
        if b.kind == "fall":
            return [x for x in (idx.get(b.fall),) if x is not None]
        return []                                            # ret / switch: no structured succ

    preds = {i: set() for i in range(n)}
    for i in range(n):
        for j in succs(i):
            preds[j].add(i)

    def single_entry(lo, hi):
        """True if no block outside [lo,hi) enters (lo,hi); the only entry is to `lo` from before."""
        for k in range(lo, hi):
            for p in preds[k]:
                if (p < lo or p >= hi) and k != lo:
                    return False
                if k == lo and lo < p < hi:                  # a back edge into the header is a loop, not an if
                    return False
        return True

    def exits_to(lo, hi, target):
        """True if every out-edge of [lo,hi) that leaves the region goes to `target`."""
        for k in range(lo, hi):
            for j in succs(k):
                if not (lo <= j < hi) and j != target:
                    return False
        return True

    # ---- pre-test detection of do-while loops: a cond at the BOTTOM whose taken edge jumps back to
    # an earlier header, with the region single-entry and exiting only to the fall-through.
    def _dowhile_ok(ti, tail, fi):
        """[ti..tail] is a do-while body: only the header ti is entered from outside; exits go to fi."""
        for k in range(ti, tail + 1):
            for p in preds[k]:
                if k == ti:
                    continue                                 # header may be entered externally + back edge
                if not (ti <= p <= tail):
                    return False                             # a mid-loop block entered from outside
        return exits_to(ti, tail + 1, fi)

    dowhile = {}                                             # header idx -> (tail idx, exit idx)
    for i, b in enumerate(blocks):
        if b.kind != "cond":
            continue
        ti = idx.get(b.taken)
        fi = idx.get(b.fall)
        if ti is None or fi != i + 1 or ti > i:              # taken must be a back edge to an earlier block
            continue
        if _dowhile_ok(ti, i, fi):
            dowhile[ti] = (i, fi)

    state = {"used": set(), "emitted": [False] * n, "out": [], "labels": set(), "dw": set()}

    def emit_seq(i, stop, depth, drop_back=None, exit_to=None):
        """Emit a maximal structured run of blocks starting at index i, stopping before `stop`.

        `drop_back` (an index) is a loop header whose back-edge goto is implicit and suppressed.
        `exit_to` (an index) is a region follow reached from inside the region -- a goto/fall to it
        is the structured fall-out (used when the follow lies beyond `stop`, e.g. an if/else join)."""
        used = state["used"]; emitted = state["emitted"]; out = state["out"]
        pad = indent * depth
        while i is not None and i < stop and not emitted[i]:
            if i in dowhile and i not in state["dw"]:        # a post-test (do-while) loop header
                tail, ex = dowhile[i]
                state["dw"].add(i)
                out.append(pad + "do {")
                emit_seq(i, tail, depth + 1)                 # body blocks [header .. tail-1]
                tb = blocks[tail]                            # the tail cond block: statements only
                emitted[tail] = True
                if tb.addr in state["labels"]:
                    out.append("%s%s:" % (indent * (depth + 1), _lblname(tb.addr)))
                for ln in body_lines(tb):
                    out.append(indent * (depth + 1) + ln)
                out.append(pad + "} while (%s);" % predicate(tb))   # taken=back edge -> continue while
                i = ex
                continue
            b = blocks[i]
            emitted[i] = True
            if b.addr in state["labels"]:                     # a goto target (from pass 1): label it
                out.append("%s%s:" % (pad, _lblname(b.addr)))
            for ln in body_lines(b):
                out.append(pad + ln)
            if b.kind == "ret":
                out.append(pad + ret_line(b))
                return
            if b.kind == "switch":
                _emit_switch(b, pad)                          # keep switch in goto-form
                return
            if b.kind == "fall":
                nxt = idx.get(b.fall)
                if nxt == stop or nxt == exit_to:             # falls out to the region follow: implicit
                    return
                i = nxt; continue
            if b.kind == "goto":
                j = idx.get(b.taken)
                if j == drop_back:                            # the loop's implicit back edge
                    return
                if j == stop or j == exit_to:                 # to the region follow: implicit
                    return
                if j == i + 1:                                # to the very next block: implicit
                    i = j; continue
                out.append(pad + "goto %s;" % _lbl(b.taken)); return
            if b.kind == "cond":
                ni = _emit_cond(i, stop, depth, pad)
                if ni is None:                                # not liftable -> goto form, fall through
                    out.append(pad + "if (%s) goto %s;" % (predicate(b), _lbl(b.taken)))
                    i = idx.get(b.fall)
                else:
                    i = ni
                continue
            return
        return

    def _emit_cond(i, stop, depth, pad):
        """Try to lift blocks[i] (a cond) into if / if-else / while. Return next index, or None."""
        out = state["out"]
        b = blocks[i]
        ti = idx.get(b.taken)
        fi = idx.get(b.fall)
        if ti is None or fi is None or fi != i + 1:
            return None
        # ---- pre-test while: taken jumps forward past a body that loops back to i ----
        if i + 1 < ti <= stop:
            body_lo, body_hi = i + 1, ti
            last = blocks[body_hi - 1]
            back = (last.kind == "goto" and idx.get(last.taken) == i)
            if back and single_entry(body_lo, body_hi) and exits_to(body_lo, body_hi, i):
                out.append(pad + "while (%s) {" % negate(predicate(b)))   # taken == loop exit
                emit_seq(body_lo, body_hi, depth + 1, drop_back=i)
                out.append(pad + "}")
                return ti
        # ---- if-then / if-then-else: taken skips a forward then-region ----
        if i + 1 < ti <= stop and single_entry(i + 1, ti):
            then_lo, then_hi = i + 1, ti
            last = blocks[then_hi - 1]                         # if-then-else: then ends by jumping
            join = idx.get(last.taken) if last.kind == "goto" else None   # past an else-region
            if join is not None and ti < join <= stop and single_entry(ti, join) \
                    and exits_to(ti, join, join) and exits_to(then_lo, then_hi, join):
                out.append(pad + "if (%s) {" % negate(predicate(b)))
                emit_seq(then_lo, then_hi, depth + 1, exit_to=join)   # then ends by jumping to join
                out.append(pad + "} else {")
                emit_seq(ti, join, depth + 1)
                out.append(pad + "}")
                return join
            if exits_to(then_lo, then_hi, ti):                # plain if-then: region exits only to ti
                out.append(pad + "if (%s) {" % negate(predicate(b)))
                emit_seq(then_lo, then_hi, depth + 1)
                out.append(pad + "}")
                return ti
        return None

    def _emit_switch(b, pad):
        out = state["out"]
        low = getattr(b, "switch_low", 0) or 0
        out.append(pad + "switch (%s) {" % getattr(b, "switch_reg", "x"))
        for k, t in enumerate(getattr(b, "cases", []) or []):
            out.append(pad + indent + "case %d: goto %s;" % (low + k, _lbl(t)))   # real case value
        dflt = getattr(b, "switch_default", None)
        if dflt is not None:
            out.append(pad + indent + "default: goto %s;" % _lbl(dflt))
        out.append(pad + "}")

    def _lbl(addr):
        state["used"].add(addr)
        return _lblname(addr)

    def _tail_unemitted():
        """Append any block structured flow never reached, in goto-form, so no code is lost."""
        out, emitted = state["out"], state["emitted"]
        for i in range(n):
            if not emitted[i]:
                b = blocks[i]
                emitted[i] = True
                out.append("%s:" % _lbl(b.addr))
                for ln in body_lines(b):
                    out.append(indent + ln)
                if b.kind == "ret":
                    out.append(indent + ret_line(b))
                elif b.kind == "cond":
                    out.append(indent + "if (%s) goto %s;" % (predicate(b), _lbl(b.taken)))
                elif b.kind == "goto":
                    out.append(indent + "goto %s;" % _lbl(b.taken))

    def _run():
        emit_seq(0, n, 0)
        _tail_unemitted()

    # pass 1: discover which blocks remain goto targets; pass 2: emit, labelling exactly those.
    _run()
    state["labels"] = set(state["used"])
    state["used"] = set(); state["emitted"] = [False] * n; state["out"] = []; state["dw"] = set()
    _run()
    return state["out"], state["labels"]


def _lblname(addr):
    return "L_%x" % addr


import re as _re2


def forfold(src):
    """Fold `i = init; while (cond) { ... i++; }` into `for (i = init; cond; i++) { ... }`.

    Conservative and text-level: the counter's init must sit immediately before the while, the
    increment must be the loop's last statement, and the counter must appear in the condition."""
    lines = src.split("\n")
    out, i = [], 0
    init_re = _re2.compile(r"^(\s*)(?:unsigned |int |long |short )?([ijklm]) = (.+);\s*$")
    while_re = _re2.compile(r"^(\s*)while \((.+)\) \{\s*$")
    while i < len(lines):
        mi = init_re.match(lines[i])
        mw = while_re.match(lines[i + 1]) if i + 1 < len(lines) else None
        if mi and mw and _re2.search(r"\b%s\b" % mi.group(2), mw.group(2)):
            var, init, cond, pad = mi.group(2), mi.group(3), mw.group(2), mw.group(1)
            depth, j = 1, i + 2                              # find the matching close brace
            while j < len(lines):
                depth += lines[j].count("{") - lines[j].count("}")
                if depth == 0:
                    break
                j += 1
            inc = _re2.match(r"^\s*%s(\+\+|\s*\+=\s*(\d+));\s*$" % var, lines[j - 1]) if j - 1 > i + 1 else None
            if j < len(lines) and lines[j].strip() == "}" and inc:
                step = "%s++" % var if inc.group(1) == "++" else "%s +=%s" % (var, inc.group(2))
                out.append("%sfor (%s = %s; %s; %s) {" % (pad, var, init, cond, step))
                out.extend(lines[i + 2:j - 1])              # body without the trailing increment
                out.append(lines[j])                        # the close brace
                i = j + 1
                continue
        out.append(lines[i]); i += 1
    return "\n".join(out)
