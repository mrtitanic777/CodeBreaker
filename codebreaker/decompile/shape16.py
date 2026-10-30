"""shape16.py -- a 16-bit function's statements in the form its instructions say they were written in.

    from codebreaker.decompile import shape16
    doc, said = shape16.reshape(doc, code)

The decompiler says what a statement does; the compiler's choice of instruction says how it was
written, where C 8 makes different code of two forms with one meaning (measured with C 8.00c):

  ++ / --        `inc` / `dec` of a variable's place: x++ / x-- (the decompiler, knowing x was 1,
                 writes x = 0 -- which C 8 makes `mov word ptr [bp-2], 0`). Only where the
                 statement is x = x + 1 / x - 1 itself, or (a place in the frame) x = a number:
                 the instruction a statement came from may be one of several (`x = x + 3` of
                 three, LZEXPAND 8:00AA), another variable's value (`x = in_AX + 1`, SUWIN
                 9:0238; `b = p == 0` from a dec that sets the flag, KEYBOARD's VKKEYSCAN), or
                 a pointer's own step (`p = (int4 *)((int2)p + 6)`, `p = (int2 *)0x32` from a
                 dec of the global, LZDONE), where x++ means something else
  if-return      an if whose then-part returns, followed by a return: C 8 lays the then-part out
                 right after its test, so the part the original has first (the lower addresses) is
                 the then-part -- else the condition is turned round and the two returns traded

Each statement's instructions are L2's origin "at" (the decompiler's PrintL2). A pass on the
function's L2 tree before the C back end writes it, after frame16 (decompiler.c_from_l2, CB_FRAME16=1).
-> (the tree, what was done: [str])
"""
import copy

from codebreaker.decompile import describe


def _walk(n):
    if isinstance(n, dict):
        yield n
        for v in n.values():
            yield from _walk(v)
    elif isinstance(n, list):
        for x in n:
            yield from _walk(x)


def _lists(n):
    if isinstance(n, dict):
        for k, v in n.items():
            if k in ("body", "then", "orelse", "stmts") and isinstance(v, list):
                yield v
            yield from _lists(v)
    elif isinstance(n, list):
        for x in n:
            yield from _lists(x)


def _at(s):
    return sorted(a & 0xFFFF for a in (s.get("origin") or {}).get("at") or ())


def _mnemonics(code, at):
    """The mnemonic of each instruction at `at` -- an inc or dec of anything but a place in the frame
    ([bp-k]: a register, a global) as "inc reg"."""
    md = describe.decoder16()
    out = []
    for a in at:
        for i in md.disasm(bytes(code[a:a + 8]), a):
            out.append(i.mnemonic + (" reg" if i.mnemonic in ("inc", "dec") and "[bp" not in i.op_str else ""))
            break
    return out


def _steps(e):
    """"inc" / "dec" where the assignment is x = x + 1 / x - 1 (x + -1) of its own target."""
    v, t = e.get("value") or {}, e.get("target") or {}
    if v.get("node") != "Binary" or v.get("op") not in ("+", "-"):
        return None
    left, right = v.get("left") or {}, v.get("right") or {}
    if left.get("node") != "Name" or (left.get("sym") or {}).get("name") != (t.get("sym") or {}).get("name") or \
            right.get("node") != "Lit" or right.get("value") not in (1, -1):
        return None
    return "inc" if (v["op"] == "+") == (right["value"] == 1) else "dec"


def reshape(doc, code):
    doc = copy.deepcopy(doc)
    said = []
    for st in _walk(doc.get("body")):
        if st.get("node") != "ExprStmt" or (st.get("expr") or {}).get("node") != "Assign":
            continue
        e = st["expr"]
        if e.get("op") or (e.get("target") or {}).get("node") != "Name":
            continue
        ms = _mnemonics(code, _at(st))
        if ms in (["inc reg"], ["dec reg"]) and _steps(e) == ms[0][:3]:
            ms = [ms[0][:3]]
        if ms in (["inc"], ["dec"]) and (_steps(e) == ms[0] or (e.get("value") or {}).get("node") == "Lit"):
            st["expr"] = {"node": "Unary", "op": "++" if ms == ["inc"] else "--", "postfix": True,
                          "x": e["target"], "type": e["target"].get("type")}
            said.append("%s%s" % (e["target"]["sym"]["name"], "++" if ms == ["inc"] else "--"))
    for body in [doc.get("body") or []] + list(_lists(doc.get("body"))):
        for k in range(len(body) - 1):
            a, b = body[k], body[k + 1]
            then = a.get("then")
            then = [then] if isinstance(then, dict) else then
            if a.get("node") != "If" or a.get("orelse") or not then or len(then) != 1 or \
                    then[0].get("node") != "Return" or b.get("node") != "Return":
                continue
            ta, tb = _at(then[0]), _at(b)
            if ta and tb and ta[0] > tb[0]:                  # (the original has the other return first)
                cond = a["cond"]
                cond = cond["x"] if cond.get("node") == "Unary" and cond.get("op") == "!" else \
                    {"node": "Unary", "op": "!", "x": cond, "type": {"kind": "bool"}}
                body[k] = dict(a, cond=cond, then=[b] if isinstance(a.get("then"), list) else b)
                body[k + 1] = then[0]
                said.append("an if-return turned round")
    return doc, said
