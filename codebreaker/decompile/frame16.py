"""frame16.py -- a 16-bit function's locals declared so that Microsoft C 8 lays them out as the original's.

    from codebreaker.decompile import frame16
    doc, said = frame16.lay_out(doc, code, start, end)

C 8 places a function's locals itself (measured with C 8.00c, 2026-10-29):

    /Os   in the order they are declared, from [bp-2] down, each taking a whole number of words
    /Ox   bytes first (two to a word), then by size; of one size, by first use
    /Od   (an order of its own, not read yet)

so a source gives the original's frame when its locals are the original's -- the same places, the
same sizes -- declared nearest the frame pointer first. The decompiler's are not: it splits a buffer
whose address is taken into the pieces read from it (SUWIN 3:5DAA's 16-byte buffer at [bp-12h]
came out as xStack_14, xStack_10, xStack_e), and gives a flag kept in a word of the frame no place at
all (a bool). The instructions say what the frame was:

  its places      every [bp-k] the function's instructions reach, as wide as they reach it; one whose
                  address is taken (lea) is a buffer, up to the next place above it
  each local's    a local the decompiler kept on the stack (L2 Local "at": {"stack": n}, BP being the
  place           entry SP less 2) at n + 2; one with none, the place the instruction of a statement
                  assigning it writes (L2 origin "at")

and the tree is rewritten to them: a local in a place of its own takes the place's width as its type
(a bool kept in a word is an int2); the pieces of a buffer become one array of its bytes, each
piece read through it at its offset; the locals are declared nearest the frame pointer first.
-> (the tree, what was done: [str]); unchanged, with the reason, where the frame cannot be read.
"""
import copy
import re

import capstone
import capstone.x86 as X

_WIDTH = {1: {"kind": "u8"}, 2: {"kind": "i16"}, 4: {"kind": "i32"}}


def _walk(n):
    if isinstance(n, dict):
        yield n
        for v in n.values():
            yield from _walk(v)
    elif isinstance(n, list):
        for x in n:
            yield from _walk(x)


def _frame(code, start, end):
    """-> (frame bytes, {bp offset: width read or written}, {bp offsets whose address is taken},
    {instruction address: (bp offset, width) it writes})"""
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    md.detail = True
    size, places, taken, writes = 0, {}, set(), {}
    for k, i in enumerate(md.disasm(bytes(code[start:end]), start)):
        if k == 0 and i.mnemonic == "enter":
            size = i.operands[0].imm
        elif k == 2 and i.mnemonic == "sub" and i.op_str.startswith("sp, ") and i.operands[1].type == X.X86_OP_IMM:
            size = i.operands[1].imm
        for n, op in enumerate(i.operands):
            if op.type != X.X86_OP_MEM or op.mem.base != X.X86_REG_BP or op.mem.index != 0:
                continue
            off = op.mem.disp
            if off >= 0:
                continue                                 # (a parameter's: the caller's)
            if i.mnemonic == "lea":
                taken.add(off)
                continue
            w = op.size or 2
            places[off] = max(places.get(off, 0), w)
            if n == 0 and op.access & capstone.CS_AC_WRITE:
                writes[i.address] = (off, w)
        if i.mnemonic in ("ret", "retf"):
            break
    return size, places, taken, writes


def lay_out(doc, code, start, end):
    """See the module's doc."""
    doc = copy.deepcopy(doc)
    size, places, taken, writes = _frame(code, start, end)
    if not size:
        return doc, ["no frame"]
    body = doc.get("body") or []
    locals_ = [s for s in body if s.get("node") == "Local"]
    by_name = {(s.get("sym") or {}).get("name"): s for s in locals_}
    # each local's place in the frame
    place = {}
    for nm, s in by_name.items():
        at = s.get("at")
        if isinstance(at, dict) and "stack" in at:
            place[nm] = at["stack"] + 2
    for st in _walk(body):
        if st.get("node") != "ExprStmt" or (st.get("expr") or {}).get("node") != "Assign":
            continue
        tgt = st["expr"].get("target") or {}
        nm = (tgt.get("sym") or {}).get("name")
        if tgt.get("node") != "Name" or nm not in by_name or nm in place:
            continue
        hits = {writes[a & 0xFFFF] for a in (st.get("origin") or {}).get("at") or () if a & 0xFFFF in writes}
        if len(hits) == 1:
            place[nm] = next(iter(hits))[0]
    if not place:
        return doc, ["no local the frame places"]
    # the original's places: a buffer (its address taken) runs to the next place above it
    points = sorted(set(places) | taken | set(place.values()))
    span = {}
    for k, at in enumerate(points):
        top = points[k + 1] if k + 1 < len(points) else 0
        span[at] = top - at
    buffers = {}
    for b in sorted(taken):
        # the places inside a buffer: from it up to the next taken place or a place written as a
        # variable of its own (one assigned by a statement: a C variable, not a piece)
        own = {p for nm, p in place.items() if by_name[nm].get("at") in (None, "none")}
        top = min([x for x in points if x > b and (x in taken or x in own)] + [0])
        buffers[b] = top - b
    said = []
    piece_of = {}
    for nm, p in place.items():
        for b, n in buffers.items():
            if b <= p < b + n and not (p == b and len([q for q in place.values() if b <= q < b + n]) == 1 and
                                        span.get(b) == n):
                piece_of[nm] = (b, p - b)
    # rewrite: pieces read through one array; scalars typed by their width
    names = {b: "local_%x" % -b for b in buffers if any(v[0] == b for v in piece_of.values())}
    for nm, (b, off) in piece_of.items():
        width = by_name[nm].get("type")
        for n in _walk(body):
            if n.get("node") == "Name" and (n.get("sym") or {}).get("name") == nm:
                arr = {"node": "Name", "sym": {"kind": "local", "name": names[b]},
                       "type": {"kind": "array", "elem": {"kind": "u8"}, "count": buffers[b]}}
                addr = {"node": "Binary", "op": "+", "left": arr, "right": {"node": "Lit", "value": off, "text": str(off)}} \
                    if off else arr
                cast = {"node": "Cast", "type": {"kind": "ptr", "space": "near", "seg": "ss", "to": width}, "x": addr}
                n.clear()
                n.update({"node": "Unary", "op": "*", "x": cast, "type": width})
        for n in _walk(body):                            # (and in an _asm block: the array, at the offset)
            if n.get("node") == "MachineFragment":
                n["asm"] = [re.sub(r"\b%s\b" % re.escape(nm), names[b] + ("[%d]" % off if off else ""), ln)
                            for ln in n.get("asm") or ()]
        said.append("%s: %s's bytes %d.." % (nm, names[b], off))
    for nm, p in place.items():
        if nm in piece_of:
            continue
        w = places.get(p)
        t = by_name[nm].get("type") or {}
        if w in _WIDTH and (t.get("kind") == "bool" or t.get("kind") == "class"):
            by_name[nm]["type"] = dict(_WIDTH[w])
            said.append("%s: %d bytes at [bp%d]" % (nm, w, p))
    # declarations: the frame's, nearest the frame pointer first; the rest as they were, after
    keep = [s for s in locals_ if (s.get("sym") or {}).get("name") not in piece_of]
    decl = []
    for b, nm in names.items():
        decl.append((b, {"node": "Local", "sym": {"kind": "local", "name": nm},
                         "type": {"kind": "array", "elem": {"kind": "u8"}, "count": buffers[b]},
                         "at": {"stack": b - 2}}))
    for s in keep:
        nm = (s.get("sym") or {}).get("name")
        decl.append((place.get(nm, 1), s))
    decl.sort(key=lambda x: -x[0] if x[0] <= 0 else 1)
    rest = [s for s in body if s.get("node") != "Local"]
    doc["body"] = [s for _k, s in decl] + rest
    return doc, said
