#!/usr/bin/env python3
r"""neraise95.py -- the 16-bit NE raise engines.

Ghidra's 16-bit far-data analysis is its weakest area: it splits every far pointer into two
DGROUP word slots (offset at address A, selector at A+2), reconstructs the pointer with
MAKELONG/CONCAT22 glue, and renders far accesses as near. That machinery is the bulk of the
"hand curation" on an NE module -- and it is a recognisable PATTERN, so it can be automated.

These passes run AFTER raise95's mechanical layer (CONCAT22 already -> MAKELONG) and BEFORE its
DAT_-slot structural map, so they consume the far-pair slots first:

  analyze_far_pairs(text) -> {off_addr: sel_addr}          # module-wide detection
  coalesce_far_pointers(body, pairs, names, types) -> body  # per-function rewrite

A pair (A, A+2) becomes one `TYPE FAR *P`; the recognised idioms rewrite to clean pointer ops:
  _DAT_A                                   -> P
  _DAT_A = (cast)MAKELONG(DAT_A + k, DAT_A2) -> P = P + k
  _DAT_A = (cast)MAKELONG(DAT_X, DAT_X2)   -> P = Q          (assign from pair X)
  DAT_A = (cast)Y ; DAT_A2 = ...>>0x10     -> P = Y          (store a local far ptr, 2-line idiom)
  DAT_A </<=/>/>= DAT_B                     -> P < Q          (both pair-offsets: pointer compare)
  MAKELONG(DAT_A + k, DAT_A2)              -> (P + k)         (rvalue)
  DAT_A (bare)                              -> FP_OFF(P)
  DAT_A2 (bare)                             -> FP_SEG(P)
Anything not matching a known idiom is left untouched (safe: never emit wrong code).
"""
import re

_MK = re.compile(r"MAKELONG\(")


def _split_args(s):
    args, depth, cur = [], 0, ""
    for ch in s:
        if ch in "([{":
            depth += 1; cur += ch
        elif ch in ")]}":
            depth -= 1; cur += ch
        elif ch == "," and depth == 0:
            args.append(cur); cur = ""
        else:
            cur += ch
    if cur.strip() != "" or args:
        args.append(cur)
    return [a.strip() for a in args]


def _makelong_args(text, start):
    """Given text and the index of 'MAKELONG(', return (lo, hi, end_index) or None."""
    k, depth = start + len("MAKELONG("), 1
    n = len(text)
    while k < n and depth > 0:
        if text[k] == "(":
            depth += 1
        elif text[k] == ")":
            depth -= 1
        k += 1
    inner = text[start + len("MAKELONG("):k - 1]
    a = _split_args(inner)
    if len(a) != 2:
        return None
    return a[0], a[1], k


_DAT = r"_?DAT_([0-9a-fA-F]{2,4}_[0-9a-fA-F]{2,4})"


def _plus2(addr):
    """'1058_0050' -> '1058_0052' (selector = offset address + 2), preserving field width."""
    fr, a = addr.split("_")
    return "%s_%0*x" % (fr, len(a), int(a, 16) + 2)


def analyze_far_pairs(text, ptr_slots=None, all_slots=None):
    """Detect far-pointer pairs (offset slot A + selector slot A+2). Two evidence sources:
      1. MAKELONG(<expr with DAT_A>, DAT_{A+2}) -- the pointer is reconstructed (strong).
      2. Ghidra typed slot A as a pointer (ptr_slots) and slot A+2 exists (all_slots) -- catches
         far pointers that are only compared, never rebuilt. This type signal correctly EXCLUDES
         non-pointer word pairs like a near buffer + its GlobalAlloc handle (the ring), which stay
         manual (a genuine near/far ambiguity).
    Returns {A: A+2} as address strings (e.g. '1058_0050': '1058_0052')."""
    pairs = {}
    for m in _MK.finditer(text):
        got = _makelong_args(text, m.start())
        if not got:
            continue
        lo, hi, _ = got
        mh = re.fullmatch(_DAT, hi.strip())
        ml = re.search(_DAT, lo)
        if not (mh and ml):
            continue
        sel, off = mh.group(1), ml.group(1)
        try:
            if _plus2(off) == sel:
                pairs[off] = sel
        except ValueError:
            pass
    if ptr_slots and all_slots:
        for off in ptr_slots:
            try:
                sel = _plus2(off)
            except ValueError:
                continue
            if off not in pairs and sel in all_slots:
                pairs[off] = sel
    return pairs


def coalesce_far_pointers(body, pairs, names, types=None):
    """Rewrite one function body, coalescing the given far pairs into single FAR pointers.
    names: {off_addr: ptr_name}; types: {off_addr: elem_type} (default BYTE)."""
    types = types or {}
    offs = sorted(pairs, key=lambda a: -int(a.split("_")[1], 16))   # longest-addr first is moot; stable

    # (0) init idiom -- build one pointer from another's base + an offset expr, selector copied:
    #     DAT_Aoff = [cast](DAT_Boff TAIL);  DAT_Asel = DAT_Bsel;   ->   PA = PB TAIL;
    # This must run first: it turns the separate off/sel word WRITES (which would otherwise become
    # invalid `FP_OFF(x) = ...` lvalues) into a single valid pointer assignment.
    for aoff in offs:
        asel = pairs[aoff]
        for boff in offs:
            bsel = pairs[boff]
            PA, PB = names[aoff], names[boff]
            for pat in (
                r"(?m)^([ \t]*)DAT_%s = \([^)]*\)\(DAT_%s([^;]*)\);\n[ \t]*DAT_%s = DAT_%s;",
                r"(?m)^([ \t]*)DAT_%s = \([^)]*\)DAT_%s([^;]*);\n[ \t]*DAT_%s = DAT_%s;",
                r"(?m)^([ \t]*)DAT_%s = DAT_%s([^;]*);\n[ \t]*DAT_%s = DAT_%s;"):
                body = re.sub(pat % (re.escape(aoff), re.escape(boff), re.escape(asel), re.escape(bsel)),
                              lambda m, PA=PA, PB=PB: "%s%s = %s%s;" % (m.group(1), PA, PB, m.group(2)),
                              body)

    for off in offs:
        sel = pairs[off]
        P = names[off]
        # (1) two-line store of a local far pointer into the pair:
        #     DAT_off = (cast)X;  DAT_sel = (int)((DWORD)X >> 0x10);   (either order) -> P = X;
        body = re.sub(
            r"(?m)^([ \t]*)DAT_%s = \([^)]*\)([A-Za-z_]\w*);\n[ \t]*DAT_%s = \(int\)\(\(DWORD\)\2 >> 0x10\);"
            % (re.escape(off), re.escape(sel)),
            r"\1%s = \2;" % P, body)
        body = re.sub(
            r"(?m)^([ \t]*)DAT_%s = \(int\)\(\(DWORD\)([A-Za-z_]\w*) >> 0x10\);\n[ \t]*DAT_%s = \([^)]*\)\2;"
            % (re.escape(sel), re.escape(off)),
            r"\1%s = \2;" % P, body)
        # (2) pointer (re)assignment via MAKELONG: _DAT_off = (cast)MAKELONG(DAT_off +/- k, DAT_sel)
        def _assign(m):
            k = m.group("k")
            if k is None:
                return "%s = %s;" % (P, P)
            k = k.replace(" ", "")
            if k.startswith("+-") or k.startswith("-"):
                return "%s = %s - %s;" % (P, P, k.lstrip("+-"))
            return "%s = %s + %s;" % (P, P, k.lstrip("+"))
        body = re.sub(
            r"_DAT_%s = \([^)]*\)MAKELONG\(DAT_%s(?P<k>\s*\+\s*-?\w+)?,\s*DAT_%s\);"
            % (re.escape(off), re.escape(off), re.escape(sel)),
            _assign, body)
        # (3) assign from another pair X: _DAT_off = (cast)MAKELONG(DAT_X, DAT_X2) handled generically below
        # (4) rvalue MAKELONG(DAT_off +/- k, DAT_sel) -> (P + k) / P
        def _rvalue(m):
            k = m.group("k")
            if k is None:
                return P
            k = k.replace(" ", "")
            if k.startswith("+-") or k.startswith("-"):
                return "(%s - %s)" % (P, k.lstrip("+-"))
            return "(%s + %s)" % (P, k.lstrip("+"))
        body = re.sub(
            r"\([^)]*\)MAKELONG\(DAT_%s(?P<k>\s*\+\s*-?\w+)?,\s*DAT_%s\)"
            % (re.escape(off), re.escape(sel)),
            _rvalue, body)
        body = re.sub(
            r"MAKELONG\(DAT_%s(?P<k>\s*\+\s*-?\w+)?,\s*DAT_%s\)"
            % (re.escape(off), re.escape(sel)),
            _rvalue, body)
        # (5) dereference / pointer identity
        body = re.sub(r"_DAT_%s\b" % re.escape(off), P, body)
        # (6) bare selector -> FP_SEG
        body = re.sub(r"\bDAT_%s\b" % re.escape(sel), "FP_SEG(%s)" % P, body)

    # (7) pointer-offset comparisons between two pairs: DAT_A </<=/>/>= DAT_B -> Pa <op> Pb
    off_names = {a: names[a] for a in pairs}
    def _cmp(m):
        a, op, b = m.group(1), m.group(2), m.group(3)
        if a in off_names and b in off_names:
            return "%s %s %s" % (off_names[a], op, off_names[b])
        return m.group(0)
    body = re.sub(r"DAT_([0-9a-fA-F]{2,4}_[0-9a-fA-F]{2,4})\s*(<=|>=|<|>)\s*DAT_([0-9a-fA-F]{2,4}_[0-9a-fA-F]{2,4})",
                  _cmp, body)
    # (8) any remaining bare offset -> FP_OFF(P)
    for off in offs:
        body = re.sub(r"\bDAT_%s\b" % re.escape(off), "FP_OFF(%s)" % names[off], body)
    # (9) residual MAKELONG cast to a pointer must be FAR (medium model)
    body = re.sub(r"\(([A-Za-z_]\w*) \*\)MAKELONG\(", r"(\1 FAR *)MAKELONG(", body)
    return body
