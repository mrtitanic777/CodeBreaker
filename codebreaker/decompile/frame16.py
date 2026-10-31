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
  each local's    a local the decompiler kept on the stack (L2 Local "at": {"stack": n}, which the
  place           decompiler gives with CB_FRAME16=1; BP being the entry SP less 2) at n + 2; one
                  with none, the place the instruction of a statement assigning it writes (L2
                  origin "at")

and the tree is rewritten to them: a local in a place of its own takes the place's width as its type
(a bool kept in a word is an int2); the pieces of a buffer become one array of its bytes, each
piece read through it at its offset; the locals are declared nearest the frame pointer first.
-> (the tree, what was done: [str]); unchanged, with the reason, where the frame cannot be read.

A pass on the function's L2 tree before the C back end writes it (decompiler.c_from_l2, CB_FRAME16=1;
the module's L2 tree is made of the trees so laid out).
"""
import copy
import re

import capstone
import capstone.x86 as X

from codebreaker.decompile import describe

_WIDTH = {1: {"kind": "u8"}, 2: {"kind": "i16"}, 4: {"kind": "i32"}}
_UNSIGNED = {1: {"kind": "u8"}, 2: {"kind": "u16"}, 3: {"kind": "u16"}, 4: {"kind": "u32"}}   # (3: as the back end writes x._1_3_)
_PIECE = re.compile(r"_(\d+)_(\d+)_")                # (the decompiler's x._2_2_: 2 bytes of x from byte 2)
_SIZES = {"u8": 1, "i8": 1, "char": 1, "bool": 1, "u16": 2, "i16": 2, "u32": 4, "i32": 4, "ptr": 2}


def _bytes(t):
    """A type's size in bytes (an array's elements times their size; a far pointer 4; xunknownN N),
    or 0 where it is not known."""
    t = t or {}
    k = t.get("kind")
    if k == "array":
        return _bytes(t.get("elem")) * int(t.get("count") or 0)
    if k == "ptr":
        return 4 if t.get("space") == "far" else 2
    if k == "class":
        m = re.fullmatch(r"\w*?(\d+)", t.get("name") or "")
        return int(m.group(1)) if m else 0
    return _SIZES.get(k, 0)


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
    md = describe.decoder16(detail=True)
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
    # a buffer one of whose pieces holds a far pointer -- declared so, or used so, or the decompiler's
    # `_x` (x and the word above it, joined: a far pointer's halves) -- is left as it was: the back end
    # writes a far pointer's __far on a declaration, and inside a cast `*(T __far **)` would lose it
    # and move two bytes for four (SUWIN 2:410C passed SETUPX.2 a pointer's offset and 0)
    def _far(t):
        return isinstance(t, dict) and t.get("kind") == "ptr" and t.get("space") == "far"
    held = {b for nm, (b, _o) in piece_of.items() if _far(by_name[nm].get("type"))}
    for n in _walk(body):
        nm = (n.get("sym") or {}).get("name") if n.get("node") == "Name" else None
        if nm in piece_of and _far(n.get("type")):
            held.add(piece_of[nm][0])
        elif nm and nm.startswith("_") and nm[1:] in piece_of and nm not in by_name:
            held.add(piece_of[nm[1:]][0])
    # In a frame with an array local the buffers are left too, as is one another local overlaps: the
    # back end lays such a frame out itself (backends/c._buffer_fields, after this pass was written):
    # an array the function reads past the end of made to reach over what it reads, the words a
    # callee fills above an array whose address is passed on read as its fields, two buffers or more
    # kept in the frame's order and distances as one packed structure (codebreaker_frame). Cut into
    # pieces under it, the bytes were no longer shared -- SUWIN 4:503E's axStack_52 + 0x3c reached
    # over the pieces at [bp-30h], VERX's VERQUERYVALUE lost the words its callee fills -- or C 8 at
    # /Ox placed them elsewhere (SUWIN 6:6562, which matched byte for byte, no longer did).
    for nm, p in place.items():
        t = by_name[nm].get("type") or {}
        size = _bytes(t) or 2
        for b, n in buffers.items():
            if not any(v[0] == b for v in piece_of.values()):
                continue
            if t.get("kind") == "array" or (nm not in piece_of and p < b + n and b < p + size and p != b):
                held.add(b)
    for nm in [nm for nm, (b, _o) in piece_of.items() if b in held]:
        del piece_of[nm]
    if held:
        said.append("a buffer left as it was: a far pointer in it, an array in the frame, or a local over it (%s)" % ", ".join(
            "[bp%d]" % b for b in sorted(held)))
    # rewrite: pieces read through one array; scalars typed by their width
    names = {b: "local_%x" % -b for b in buffers if any(v[0] == b for v in piece_of.values())}

    def piece(b, off, width):
        arr = {"node": "Name", "sym": {"kind": "local", "name": names[b]},
               "type": {"kind": "array", "elem": {"kind": "u8"}, "count": buffers[b]}}
        addr = {"node": "Binary", "op": "+", "left": arr, "right": {"node": "Lit", "value": off, "text": str(off)}} \
            if off else arr
        cast = {"node": "Cast", "type": {"kind": "ptr", "space": "near", "seg": "ss", "to": width}, "x": addr}
        return {"node": "Unary", "op": "*", "x": cast, "type": width}

    for n in list(_walk(body)):                          # (a piece of one first: x._2_2_, 2 bytes at x + 2 --
        if n.get("node") != "Select":                    # written `->_2_2_` of the cast, it did not compile)
            continue
        obj = n.get("obj") or {}
        m = _PIECE.fullmatch((n.get("sym") or {}).get("name") or "")
        nm = (obj.get("sym") or {}).get("name")
        if m and obj.get("node") == "Name" and nm in piece_of and int(m.group(2)) in _UNSIGNED:
            b, off = piece_of[nm]
            n.clear()
            n.update(piece(b, off + int(m.group(1)), dict(_UNSIGNED[int(m.group(2))])))
    for nm, (b, off) in piece_of.items():
        width = by_name[nm].get("type")                  # (at its declared type)
        for n in _walk(body):
            if n.get("node") == "Name" and (n.get("sym") or {}).get("name") == nm:
                n.clear()
                n.update(piece(b, off, width))
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
        # (a bool only: a local of the decompiler's unknown type, xunknownN, takes the type its uses
        # give it in the back end -- `xStack_16`, a pointer, made an int2 was dereferenced, SUWIN 9:0924)
        if w in _WIDTH and t.get("kind") == "bool":
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
