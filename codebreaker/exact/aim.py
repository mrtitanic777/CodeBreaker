"""aim.py -- where a candidate's code differs from the program's, and how: what the beam rewrites first.

The compiler's listing of a candidate (toolchain.compile(listing=True)) gives each instruction the line of the
source it came from. The candidate's instructions are lined up with the program's (both read by the same
disassembler, the numbers a link would change masked), and each stretch that differs is told by its kind:

    frame    the same instructions, only the places in the frame ([bp - N]) other
    width    a byte read where a word is, or the other way (byte ptr, cbw, sub ah,ah)
    sign     a jump of the other signedness (jl / jb ..), sar / shr, imul / mul
    order    the same instructions in another order or layout (the jumps, the blocks)
    shape    something else: the code of a statement itself

and by the lines of the candidate that stretch comes from. A diagnosis is one compile more (kept in the cache).
"""
import difflib
import re

from codebreaker.exact import x86

KINDS = ("frame", "width", "sign", "order", "shape")
SIGNED = {"jl": "jb", "jle": "jbe", "jg": "ja", "jge": "jae", "sar": "shr", "imul": "mul", "idiv": "div", "cwd": "xor", "cbw": "sub"}


def listing_code(text, name=None):
    """[(offset, bytes, line)] - the instructions of the listing's function (the first, or the one named), each
    with the source line before it ("; Line N")"""
    out, line, inside = [], None, name is None
    for l in text.split("\n"):
        m = re.match(r"\s*(_?\w+)\s+PROC\b", l)
        if m:
            inside = name is None or m.group(1).lstrip("_") == name.lstrip("_")
            continue
        if re.match(r"\s*_?\w+\s+ENDP\b", l):
            if inside and out:
                break
            continue
        m = re.match(r"; Line (\d+)", l)
        if m:
            line = int(m.group(1))
            continue
        m = re.match(r"\s*\*\*\*\s+([0-9a-f]{6})\s+((?:[0-9a-f]{2} )+)", l)
        if m and inside:
            out.append((int(m.group(1), 16), bytes.fromhex(m.group(2).replace(" ", "")), line))
    return out


def _num(text):
    """a number as the disassembler writes it: 0x.. hex, else decimal (int(text, 0) refuses a decimal with a leading zero)"""
    return int(text, 16) if text.startswith("0x") else int(text, 10)


def _norm(ins, base):
    """an instruction as compared: mnemonic and operands, the numbers a link changes (places of code and data,
    targets) masked, a frame's place and a small constant kept"""
    ops = ins.op_str
    if ins.mnemonic.startswith("j") or ins.mnemonic in ("call", "loop"):
        return ins.mnemonic + " T"

    def mem(m):                                             # (a place: a frame's kept; a displacement of the data -
        inner = m.group(1)                                  #  large, or the 0 of an unlinked object - masked)
        parts = [p.strip() for p in re.split(r"\s*\+\s*|\s*-\s*", inner)]
        regs = [p for p in parts if re.fullmatch(r"[a-z]{2}", p)]
        nums = [_num(p) for p in parts if re.fullmatch(r"0x[0-9a-f]+|\d+", p)]
        if "bp" in regs and nums:
            return "[%s%s%d]" % ("+".join(regs), "-" if " - " in inner else "+", nums[0])
        small = [n for n in nums if 0 < n < 0x80]
        return "[%s%s]" % ("+".join(regs) or "M", "+%d" % small[0] if small and regs else "")
    ops = re.sub(r"\[([^\]]*)\]", mem, ops)
    ops = re.sub(r"(?<![\w\[+-])(0x[0-9a-f]+|\d+)\b", lambda m: m.group(0) if 0 < _num(m.group(0)) < 0x80 else "N", ops)
    return (ins.mnemonic + " " + ops).strip()


def _insns(code):
    return list(x86.disasm(bytes(code), 0, len(code), True))


def _kind(a, b):
    """the kind of a differing stretch: a (the program's), b (the candidate's) - normalized instructions"""
    if len(a) == len(b):
        pairs = list(zip(a, b))
        sk = lambda x: re.sub(r"\[bp[-+]\d+\]", "[bp]", x)
        if all(sk(x) == sk(y) for x, y in pairs):
            return "frame"
        mn = lambda x: x.split(" ", 1)[0]
        if all(mn(x) == mn(y) or SIGNED.get(mn(x)) == mn(y) or SIGNED.get(mn(y)) == mn(x) for x, y in pairs) and \
           any(SIGNED.get(mn(x)) == mn(y) or SIGNED.get(mn(y)) == mn(x) for x, y in pairs):
            return "sign"
        if any(("byte ptr" in x) != ("byte ptr" in y) or re.search(r"\b(?:al|ah|bl|cl|dl)\b", x) and not
               re.search(r"\b(?:al|ah|bl|cl|dl)\b", y) for x, y in pairs):
            return "width"
    if sorted(a) == sorted(b):
        return "order"
    if any(x in ("cbw", "cwde") or "byte ptr" in x or x.startswith("sub ah") for x in a + b) and \
       abs(len(a) - len(b)) <= 2 and [x for x in a if "byte ptr" in x or x in ("cbw", "cwde")] != [x for x in b if "byte ptr" in x or x in ("cbw", "cwde")]:
        return "width"
    return "shape"


def diagnose(orig, code, listing):
    """-> {"kinds": {kind: n}, "lines": [source lines of the differing stretches, first first], "first": line} -
    orig the program's bytes of the function, code the candidate's (from its listing), listing its listing_code"""
    oi, ci = _insns(orig), _insns(code)
    on, cn = [_norm(i, 0) for i in oi], [_norm(i, 0) for i in ci]
    at = {off: ln for off, b, ln in listing}
    lines_of = []
    last = None
    for i in ci:                                            # (each candidate instruction's line: the last given)
        last = at.get(i.address, last)
        lines_of.append(last)
    kinds, lines = {}, []
    sm = difflib.SequenceMatcher(None, on, cn, autojunk=False)
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == "equal":
            continue
        k = _kind(on[i1:i2], cn[j1:j2])
        kinds[k] = kinds.get(k, 0) + max(i2 - i1, j2 - j1)
        for j in range(j1, max(j2, j1 + 1)):
            ln = lines_of[min(j, len(lines_of) - 1)] if lines_of else None
            if ln is not None and ln not in lines:
                lines.append(ln)
    # (the same instructions elsewhere: a block the layout moved - order, not shape)
    if kinds.get("shape") and sorted(on) == sorted(cn):
        kinds["order"] = kinds.pop("shape") + kinds.get("order", 0)
    return {"kinds": kinds, "lines": lines, "first": lines[0] if lines else None}


def code_of(listing):
    """the candidate's function bytes, from its listing"""
    if not listing:
        return b""
    base = listing[0][0]
    out = bytearray()
    for off, b, ln in listing:
        out[off - base:off - base + len(b)] = b
    return bytes(out)
