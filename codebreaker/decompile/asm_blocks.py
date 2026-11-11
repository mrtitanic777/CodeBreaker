"""asm_blocks.py -- a 16-bit function's _asm blocks placed in its L2 tree, where the instructions say.

    from codebreaker.decompile import asm_blocks
    doc, said = asm_blocks.place(doc, code, start, fixups, name_at, window=(0, 0))

A function of Microsoft C 8 holding an _asm block (analyze/origin: INT n, IN/OUT, CLI/STI, the carry
-- inside the frame C 8 gives every such function) comes from the decompiler as C round an interrupt
helper (`int21(0x4405, ...)`) whose carry it reads back as a variable (`int_CF`). What the programmer
wrote is the interrupt's own instructions, inline. The tree says which statements came from which
instructions (L2 origin "at", the decompiler's PrintL2), so the block can take their place:

  the block       from the first instruction that loads what the interrupt is given (walking back
                  from the INT over instructions the inline assembler takes that set registers, and
                  push/pop of segment registers -- not a store to memory, a call, a jump or a place
                  jumped to) to the INT and the pop that puts DS back; the statements made only of
                  those instructions leave the tree, a MachineFragment (reason "inline") stands there
  its carry       an If whose condition reads int_CF / int_ZF: the test was assembly as well (jc,
                  cmp, je), a second block -- from the first instruction of its condition to where
                  its then-part begins -- its jumps out of the block to C labels set at the then-part
                  and after it

`window` moves the block's two ends by instructions (the boundary search: which of the nearby
choices is what was written, the compiler says, byte for byte -- verify/authored/match16.py).

In the block, a place in the frame is named by the C variable there (a parameter at [bp+k], a local
Stack_N at [bp+2-N]) so the compiler can lay the frame out as it does; one with no variable stays
[bp+k]. Calls and data are named by the module's fix-ups as in decompile/inline_asm.py.
-> (the tree, what was done: [str]) -- the tree unchanged, and why, where nothing could be placed.
"""
import copy
import re

import capstone
import capstone.x86 as X

from codebreaker.decompile import inline_asm
from codebreaker import platforms

_HELPER = re.compile(r"^int([0-9a-f]{2})(_[a-z_]+)?$")
_FLAGS = ("int_CF", "int_ZF")
_SEGS = ("ds", "es", "ss", "cs")


def _walk(n):
    if isinstance(n, dict):
        yield n
        for v in n.values():
            yield from _walk(v)
    elif isinstance(n, list):
        for x in n:
            yield from _walk(x)


def _lists(n):
    """Every statement list of the tree (a body, a then-part, an else-part)."""
    if isinstance(n, dict):
        for k, v in n.items():
            if k in ("body", "then", "orelse", "stmts") and isinstance(v, list):
                yield v
            yield from _lists(v)
    elif isinstance(n, list):
        for x in n:
            yield from _lists(x)


def _at(stmt):
    return set((stmt.get("origin") or {}).get("at") or ())


def _calls_helper(stmt):
    for n in _walk(stmt.get("expr")):
        if n.get("node") == "Call":
            nm = ((n.get("target") or {}).get("sym") or {}).get("name", "")
            m = _HELPER.match(nm)
            if m:
                return int(m.group(1), 16)
    return None


def _reads(stmt, names):
    return any(n.get("node") == "Name" and (n.get("sym") or {}).get("name") in names for n in _walk(stmt.get("cond")))


def _frame_names(doc, far):
    """{bp offset: C name} -- the parameters (their entry-stack place) and Stack_N locals."""
    out = {}
    ft = ((doc.get("sym") or {}).get("type") or {})
    for p in ft.get("params") or ():
        at = (p.get("at") or {}).get("stack")
        if at is not None:
            out[at + 2] = p["name"]                    # (BP is SP at entry less the BP pushed)
    for n in _walk(doc.get("body")):
        if n.get("node") == "Name":
            nm = (n.get("sym") or {}).get("name", "")
            m = re.match(r"^[a-z]*Stack_([0-9a-f]+)$", nm)
            if m:
                out.setdefault(2 - int(m.group(1), 16), nm)
    return out


def _named(text, frame):
    """[bp + k] / [bp - k] in an operand by the C name of that place."""
    def one(m):
        n = m.group(2)
        k = (int(n, 16) if n.startswith("0x") else int(n, 10)) * (-1 if m.group(1) == "-" else 1)   # (a decimal with a leading zero is no error)
        return frame.get(k, m.group(0))
    return re.sub(r"\[bp ([+-]) (0x[0-9a-f]+|\d+)\]", one, text)


def _decode(code, start, end):
    md = platforms.decoder(16, detail=True)
    return list(md.disasm(bytes(code[start:end]), start))


def _feeds(i):
    """An instruction the inline assembler takes that only sets registers for what follows (or saves
    and loads a segment register): part of an interrupt's set-up."""
    if inline_asm._386(i):
        return False
    mn = i.mnemonic
    if mn in ("push", "pop"):
        return i.op_str in _SEGS or i.op_str in ("ax", "bx", "cx", "dx", "si", "di")
    if mn in inline_asm._JUMPS or mn in ("call", "lcall", "ret", "retf", "int"):
        return False
    for op in i.operands:
        if op.type == X.X86_OP_MEM and op.access & capstone.CS_AC_WRITE:
            return False
    return bool(i.operands) and i.operands[0].type == X.X86_OP_REG


def _line(i, frame, labels, fixups, name_at, seg):
    """One instruction as the inline assembler takes it; jumps to `labels`. -> text or None."""
    mn = i.mnemonic.split()[-1]
    if mn in inline_asm._JUMPS:
        to = i.operands[0].imm & 0xFFFF
        return "%s %s" % (mn, labels[to]) if to in labels else None
    site = [a for a in range(i.address, i.address + i.size) if a in fixups]
    if site or mn in ("call", "lcall"):
        return None
    ops = inline_asm._operands(i, fixups, name_at, seg)
    prefix = ""
    if i.prefix[0] in (0xF2, 0xF3) and mn in inline_asm._STRING:
        prefix = "rep " if i.prefix[0] == 0xF3 else "repne "
    return _named("%s%s%s" % (prefix, mn, (" " + ops) if ops else ""), frame)


def place(doc, code, start, end, fixups=None, name_at=None, seg=None, window=(0, 0)):
    """See the module's doc. code: the segment's bytes; start, end: the function's place in it."""
    fixups, name_at = fixups or {}, name_at or (lambda s, o: None)
    doc = copy.deepcopy(doc)
    ins = _decode(code, start, end)
    if not ins:
        return doc, ["no instructions"]
    index = {i.address: k for k, i in enumerate(ins)}
    far = (doc.get("sym") or {}).get("type", {}).get("distance") == "far"
    frame = _frame_names(doc, far)
    said = []
    targets = {i.operands[0].imm & 0xFFFF for i in ins if i.mnemonic in inline_asm._JUMPS and i.operands and
               i.operands[0].type == X.X86_OP_IMM}
    for body in list(_lists(doc)):
        k = 0
        while k < len(body):
            st = body[k]
            vec = _calls_helper(st) if st.get("node") == "ExprStmt" else None
            if vec is None:
                k += 1
                continue
            at = {a & 0xFFFF for a in _at(st)}
            ints = [a for a in sorted(at) if a in index and ins[index[a]].mnemonic == "int"]
            if not ints:
                said.append("an interrupt helper with no INT among its instructions")
                k += 1
                continue
            p = index[ints[0]]
            lo = p
            while lo > 0 and _feeds(ins[lo - 1]) and ins[lo - 1].address not in targets:
                lo -= 1
            hi = p
            while hi + 1 < len(ins) and ins[hi + 1].mnemonic in ("pop", "push") and ins[hi + 1].op_str in _SEGS:
                hi += 1
            lo, hi = max(0, lo + window[0]), min(len(ins) - 1, hi + window[1])
            if hi < lo:                           # (the window moved the two ends past each other: no block, not an empty one)
                said.append("the window leaves no instruction in the block")
                k += 1
                continue
            span = {ins[x].address for x in range(lo, hi + 1)}
            lines = [_line(ins[x], frame, {}, fixups, name_at, seg) for x in range(lo, hi + 1)]
            if None in lines:
                said.append("an instruction of the block the inline assembler cannot say (%04x)" %
                            ins[lo + lines.index(None)].address)
                k += 1
                continue
            # what the interrupt leaves in registers read by the C after it (int_BX, int_DS): not a block
            # of this shape -- the C would read registers no variable holds
            later = [n for x in body[k + 1:] for n in _walk(x) if n.get("node") == "Name" and
                     re.match(r"^int_(?!CF|ZF)[A-Z]{2}", (n.get("sym") or {}).get("name", ""))]
            if later:
                said.append("INT %02Xh: the C after it reads %s" % (vec, later[0]["sym"]["name"]))
                k += 1
                continue
            # the statements made only of the block's instructions go; the block stands where the first was
            first = k
            while first > 0 and body[first - 1].get("node") == "ExprStmt" and _at(body[first - 1]) and \
                    {a & 0xFFFF for a in _at(body[first - 1])} <= span:
                first -= 1
            frag = {"node": "MachineFragment", "reason": "inline", "length": sum(ins[x].size for x in range(lo, hi + 1)),
                    "why": "INT %02Xh" % vec, "asm": lines, "origin": {"at": sorted(span)}}
            body[first:k + 1] = [frag]
            said.append("INT %02Xh: an _asm block %04x-%04x" % (vec, ins[lo].address, ins[hi].address))
            k = first + 1
            # its carry: the first If after it reading int_CF / int_ZF
            for j in range(k, len(body)):
                cand = body[j]
                if cand.get("node") != "If" or not _reads(cand, _FLAGS):
                    continue
                cat = sorted(a & 0xFFFF for a in _at(cand))
                then = cand.get("then")
                then = then if isinstance(then, list) else [then] if then else []
                then_at = sorted(a & 0xFFFF for s in then for a in _at(s))
                if not cat or not then_at or cat[0] not in index:
                    said.append("its carry: an If the instructions do not place")
                    break
                a, b = index[cat[0]], index.get(then_at[0])
                if b is None or b <= a:
                    said.append("its carry: a then-part before its test")
                    break
                after = ins[index[then_at[-1]] + 1].address if index.get(then_at[-1], len(ins)) + 1 < len(ins) else None
                labels = {then_at[0]: "L_%04x" % then_at[0]}
                if after is not None:
                    labels[after] = "L_%04x" % after
                inner = {ins[x].address for x in range(a, b)}
                labels.update({t: "L_%04x" % t for t in targets if t in inner})
                test = []
                for x in range(a, b):
                    if ins[x].address in inner and ins[x].address in targets and x != a:
                        test.append("L_%04x:" % ins[x].address)
                    test.append(_line(ins[x], frame, labels, fixups, name_at, seg))
                if None in test:
                    said.append("its carry: an instruction the block cannot say")
                    break
                parts = [{"node": "MachineFragment", "reason": "inline", "why": "the carry INT %02Xh left" % vec,
                          "length": sum(ins[x].size for x in range(a, b)), "asm": test,
                          "origin": {"at": sorted(inner)}},
                         {"node": "Labeled", "label": labels[then_at[0]], "body": then}]
                if after is not None:
                    parts.append({"node": "Labeled", "label": labels[after], "body": []})
                body[j:j + 1] = parts
                said.append("its carry: an _asm block %04x-%04x, the then-part at %s" % (
                    ins[a].address, ins[b - 1].address, labels[then_at[0]]))
                break
    if any("block" in s for s in said):
        _unused(doc)
    return doc, said


def _unused(doc):
    """The locals the blocks took the last use of (in_BX, unaff_SS, the helper's temporaries) left
    out: declared, C 8 would give them room in the frame, and a codebreaker_ss() store."""
    body = doc.get("body") or []
    used = {(n.get("sym") or {}).get("name") for n in _walk(body) if n.get("node") != "Local" and isinstance(n.get("sym"), dict)}
    text = " ".join(" ".join(n.get("asm") or ()) for n in _walk(body) if n.get("node") == "MachineFragment")
    used |= set(re.findall(r"[A-Za-z_]\w*", text))
    # (the decompiler's `_x` is x joined with the word above it, a far pointer's halves: the back end
    # declares it in x's place, so x is used where `_x` is -- SUWIN 8:1704's _pcStack_10c)
    used |= {n[1:] for n in used if n and n.startswith("_")}
    for lst in [body] + list(_lists(body)):
        lst[:] = [s for s in lst if not (s.get("node") == "Local" and (s.get("sym") or {}).get("name") not in used)]
