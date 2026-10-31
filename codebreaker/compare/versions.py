"""versions.py -- two builds of a module, function by function: what is the same, what changed,
what is new, and what is already known about the new one because of the old one.

    python -m codebreaker versions <module> --from CORPUS --to CORPUS    one module, two products
    python -m codebreaker versions <fileA> <fileB>                       any two files
    python -m codebreaker versions --all --from CORPUS --to CORPUS       every module of a product:
                                  [--sample N] [--jobs N]                how much of it is known
    python -m codebreaker versions --shared A B C ... [--level shape]    the functions several
                                  different programs share: a source module linked into each
                                  (16-bit, 32-bit or real-mode DOS programs)
    options:  --json FILE   everything a side-by-side viewer needs (shape below)
              --carry       file the verified reconstructions of identical functions in the
                            store for the other build, as candidates still to be judged there,
                            and give the names that carry to its bodies
              --least 0.6   how alike two functions must be to be called the same one changed

    from codebreaker.compare import versions
    c = versions.compare(path_a, path_b)       # .pairs .added .removed .counts() .report()
    side = versions.load(path)                 # .functions {key: Function}, as the store keys them

HOW. A function is everything from its start (the starts the decompiler would use) to the return
after which nothing of it is left. It is fingerprinted at three strictnesses:

    bytes   its machine code with only what the loader or linker fills in blanked: relocated
            fields and the displacements of calls and jumps that leave the function
    text    its instructions with every address replaced by what it stands for: an imported
            function by name, another function, "some address". Constants, stack and structure
            offsets, registers and the layout of its own jumps are kept
    shape   instruction kinds and operand kinds only (register, memory, constant, target)

Matching goes from certain to likely, and each step only considers what is still unmatched:
the same bytes, when one function on each side has them; the same exported name; the same text;
the same shape; then the call graph -- two functions matched to each other call the same things
in the same order, so the unmatched ones in corresponding places are each other, and a matched
function with one unmatched caller on each side matches those; then likeness of the shapes
(candidates share imports called, constants or runs of instructions); then position between two
matched neighbours. Functions with the same fingerprint as others (thunks, stubs) are told
apart by name and by who calls them.

A pair is `identical` when bytes or text agree (level says which), else `changed` with a score;
functions of the old build with no partner are `removed`, of the new one `added`.

WHAT IS CARRIED. A name the old build has and the new one lacks, for every pair (with whether
the bodies are identical). And, for identical pairs, the old function's reconstruction when the
store holds one with a verdict that makes it verified: reported, and with --carry filed for the
new build under the engine name "carried" with the verdict `inconclusive` -- its text still
names the old build's addresses, so it is a candidate to judge, not a result. The store keeps
reconstructions by module and address, so which function of the new build is which of the old
is decided here. Names it keeps by body as well (a body's digest, wherever it is linked):
--carry gives each carried name on an identical pair to the new build's body there.

THE --json FILE (one module pair)

    {"from": {"path", "name", "corpus", "sha", "kind", "functions", "bytes", "code"}, "to": {...},
                      bytes: in the functions found; code: all the code in the file
     "counts": {"identical", "identical_bytes", "identical_text", "changed", "added", "removed"},
     "code": {"to_bytes", "identical", "changed", "added"},        bytes of the NEW build's code
     "seconds": 1.2,
     "pairs": [{"from": key, "to": key, "from_name", "to_name", "status": "identical"|"changed",
                "level": "bytes"|"text"|"shape"|"", "how": "bytes"|"name"|"text"|"shape"|"calls"|
                "likeness"|"position", "score": 0..1, "sizes": [a, b],
                "ops": [[tag, a1, a2, b1, b2], ...]}],             changed pairs only: how the two
                                                                   listings line up (difflib tags)
     "added": [{"key", "name", "size"}], "removed": [...],
     "names": [{"to": key, "name", "from": key, "identical": bool}],
     "reconstructions": [{"from": key, "to": key, "class", "engine", "name", "source"}],
     "listings": {"from": {key: [[address, instruction, text token], ...]}, "to": {...}}}

Listings are given for changed, added and removed functions (identical ones need no viewer).
Addresses are hexadecimal strings; a 16-bit key is "segment:offset". With --all the file holds
{"from", "to", "modules": [{"name", "status", counts...}], "totals": {...}} instead.
"""
import collections
import difflib
import hashlib
import json
import os
import time

from capstone import x86 as X

from codebreaker.base import corpus
from codebreaker.compare import mask
from codebreaker.decompile import describe
from codebreaker.formats import detect, dos, pe_functions
from codebreaker.formats.ne import NEFile
from codebreaker.formats.pe import open_pe
from codebreaker.jobs import pool
from codebreaker.store import functions as store
from codebreaker import platforms
from codebreaker.verify import verdicts as V

LEVELS = ("bytes", "text", "shape")
ENGINE = "carried"                      # who a candidate carried across builds came from
_ALIKE = {"test": "cmp", "cmp": "cmp", "mov": "mov", "lea": "mov", "movzx": "mov", "movsx": "mov"}
_RETURN = {"ret", "retf", "lret", "iret", "iretd"}
_LEAVE = {"jmp", "ljmp"} | _RETURN


class Function(object):
    """One function of one build. `calls` is what it calls, in order: ("f", key) another
    function of the module, ("i", name) an imported one."""
    __slots__ = ("key", "name", "named", "at", "size", "rows", "text", "shape", "ids", "calls",
                 "marks", "jumps")

    @property
    def count(self):
        return len(self.shape)


class Side(object):
    """One build of a module: `functions` {key: Function}, `order` (keys by address), `callers`
    {key: {keys}}, `code` (bytes of code in the file, in functions or not)."""

    def __init__(self, path, kind, code=0):
        self.path, self.kind, self.code = path, kind, code
        self.name = os.path.basename(path).lower()
        self.functions, self.order, self.callers = {}, [], collections.defaultdict(set)

    def add(self, f):
        self.functions[f.key] = f
        self.order.append(f.key)

    def done(self):
        for f in self.functions.values():
            for kind, key in f.calls:
                if kind == "f" and key in self.functions:
                    self.callers[key].add(f.key)
        return self

    @property
    def size(self):
        return sum(f.size for f in self.functions.values())


def _digest(parts):
    return hashlib.blake2b(b"\n".join(parts), digest_size=8).hexdigest()


def _body(stream, lo, hi):
    """The instructions of the function at `lo`: up to the return or jump away after which no
    jump inside it still points ahead (or to `hi`, where the next function starts)."""
    out, reach = [], lo
    for ins in stream:
        out.append(ins)
        mn, nxt = ins.mnemonic, ins.address + ins.size
        if mn[0] == "j" or mn.startswith("loop"):
            ops = ins.operands
            if len(ops) == 1 and ops[0].type == X.X86_OP_IMM and lo <= ops[0].imm < hi:
                reach = max(reach, ops[0].imm)
        if nxt >= hi or (mn in _LEAVE and nxt > reach):
            break
    return out


def _function(key, name, named, body, lo, hi, relocated, direct, site, slot, inside):
    """Fingerprint one function. `relocated` holds the addresses of bytes the loader fills in;
    `direct(address)` is the key of the function at an address, `site(address)` what a relocated
    byte refers to, `slot(address)` the import a memory operand reads -- each or None;
    `inside(value)` says whether a 32-bit constant is an address in the module (a module with
    no relocation table, as the system's own are, still holds addresses)."""
    f = Function()
    f.key, f.name, f.named, f.at = key, name, named, lo
    f.size = (body[-1].address + body[-1].size - lo) if body else 0
    index = {ins.address: i for i, ins in enumerate(body)}
    raw, f.rows, f.text, f.shape, f.calls, f.marks, f.jumps = [], [], [], [], [], set(), 0
    for i, ins in enumerate(body):
        mn, ops, addr = ins.mnemonic, ins.operands, ins.address
        hits = [k for k in range(ins.size) if addr + k in relocated]
        out = set(hits)                                          # the bytes left out of the `bytes` digest
        ref = next((r for r in (site(addr + k) for k in hits) if r), None) if hits else None
        calls = mn in ("call", "lcall")
        jumps = mn[0] == "j" or mn.startswith("loop") or mn == "ljmp"
        parts, kinds, called = [], [], None
        if mn in ("lcall", "ljmp") and len(ops) == 2:            # far, to segment:offset
            called = ref if ref and ref[0] in "fi" else None
            parts, kinds = [("!" + ref[1]) if ref and ref[0] == "i" else ">"], ["t"]
            ops = ()
        for op in ops:
            if op.type == X.X86_OP_REG:
                reg = ins.reg_name(op.reg)
                parts.append(reg)
                kinds.append("f" if reg in ("bp", "ebp") else "s" if reg in ("sp", "esp")
                             else "g" if reg[-1] == "s" and len(reg) == 2 else "r")
            elif op.type == X.X86_OP_IMM:
                lo_f, n_f = ins.imm_offset, ins.imm_size
                if (calls or jumps) and len(ops) == 1:
                    if not calls and lo <= op.imm < hi and op.imm in index:
                        parts.append("%+d" % (index[op.imm] - i))
                        f.jumps += 1
                    else:                                        # leaves the function
                        out.update(range(lo_f, lo_f + n_f))
                        tgt = direct(op.imm)
                        called = ("f", tgt) if tgt else None
                        parts.append(">")
                    kinds.append("t")
                elif any(lo_f <= k < lo_f + n_f for k in hits) or (n_f == 4 and inside(op.imm & 0xFFFFFFFF)):
                    out.update(range(lo_f, lo_f + n_f))
                    parts.append(("!" + ref[1]) if ref and ref[0] == "i" else ">" if direct(op.imm) else "@")
                    kinds.append("a")
                else:
                    v = op.imm & 0xFFFFFFFF
                    parts.append("%x" % v)
                    kinds.append("i")
                    if 0x100 <= v < 0xFFFF0000 and v not in (0xFFFF, 0xFF00, 0x8000, 0x7FFF):
                        f.marks.add("c%x" % v)
            elif op.type == X.X86_OP_MEM:
                m = op.mem
                lo_f, n_f = ins.disp_offset, ins.disp_size
                bare = not m.base and not m.index
                if bare or any(lo_f <= k < lo_f + n_f for k in hits) or (n_f == 4 and inside(m.disp & 0xFFFFFFFF)):
                    if n_f == 4:
                        out.update(range(lo_f, lo_f + n_f))
                    imp = slot(m.disp & 0xFFFFFFFF) if bare else None
                    if imp and (calls or jumps):
                        called = ("i", imp)
                    disp = ("!" + imp) if imp else ("!" + ref[1]) if ref and ref[0] == "i" else "@"
                else:
                    disp = "%x" % (m.disp & 0xFFFFFFFF)
                parts.append("[%s:%s+%s*%d+%s]" % (
                    ins.reg_name(m.segment) if m.segment else "", ins.reg_name(m.base) if m.base else "",
                    ins.reg_name(m.index) if m.index else "", m.scale, disp))
                kinds.append("m")
        if called:
            f.calls.append(called)
            if called[0] == "i":
                f.marks.add("i" + called[1])
        raw.append(mask.blank(ins.bytes, out))
        f.rows.append((addr, ("%s %s" % (mn, ins.op_str)).strip()))
        f.text.append("%s %s" % (mn, ",".join(parts)))
        f.shape.append("%s %s" % (_ALIKE.get(mn, "jcc" if jumps and mn not in ("jmp", "ljmp") else mn), "".join(kinds)))
    f.ids = {"bytes": _digest(raw), "text": _digest([t.encode() for t in f.text]),
             "shape": _digest([t.encode() for t in f.shape])}
    return f


def _load_pe(path):
    """A 32-bit module: the functions the reconstruction path uses (pe_functions), keyed as the
    store keys them (the address in hexadecimal)."""
    pe = open_pe(path)
    spans = pe_functions.function_spans(path)
    sections = pe.code_sections()
    relocated = set()
    for rva in pe.base_relocs():
        relocated.update(range(pe.image_base + rva, pe.image_base + rva + 4))
    imports = {va: "%s.%s" % (os.path.splitext(dll)[0].upper(), name) for va, (dll, name) in pe.imports().items()}
    low = pe.image_base
    high = low + max((s["vaddr"] + max(s["vsize"], s["rawsize"]) for s in pe.sections), default=0)
    md = platforms.decoder(32, detail=True)
    side = Side(path, "pe", sum(len(data) for _va, data, _name in sections))
    for va in sorted(spans):
        hi, name = spans[va]
        base, data = next((b, d) for b, d, _n in sections if b <= va < b + len(d))
        named = bool(name) and not name.startswith("Ord")
        side.add(_function("%x" % va, name if named else "sub_%06X" % (va - pe.image_base), named,
                           _body(md.disasm(data[va - base:hi - base], va), va, hi), va, hi, relocated,
                           lambda t: ("%x" % t) if t in spans else None, lambda a: None, imports.get,
                           lambda v: low <= v < high))
    return side.done()


def _key16(seg, off):
    return "%d:%04x" % (seg, off)


def _sites(nef, seg, segs, entries, refs):
    """{offset in the segment: what the relocated byte there refers to} -- ("i", "KERNEL.5") an
    import, ("f", key) a function of the module, ("d", "") anything else."""
    base, out = seg["file_off"], {}
    for r in nef.relocations(seg):
        kind = r["kind"]
        if kind in ("IMPORTORD", "IMPORTNAME"):
            mod = refs[r["module"] - 1] if 0 < r["module"] <= len(refs) else "?"
            what = ("i", "%s.%s" % (mod, r["ordinal"] if kind == "IMPORTORD" else nef.imported_name(r["name_off"])))
        elif kind == "INTERNAL":
            e = entries.get(r["entry"]) if r["seg"] == 0xFF else {"seg": r["seg"], "offset": r["entry"]}
            code = e and segs.get(e["seg"], {}).get("kind") == "CODE" and r["src"] == 3
            what = ("f", _key16(e["seg"], e["offset"])) if code else ("d", "")
        else:
            what = ("d", "")
        width = {3: 4, 2: 2, 5: 2, 0: 1}.get(r["src"], 2)
        off, hops = r["offset"], 0
        while off != 0xFFFF and off + 1 < seg["length"] and hops < 0x10000:
            for k in range(width):
                out[off + k] = what
            if r["additive"]:
                break
            off, hops = nef.u16(base + off), hops + 1
    return out


def _load_ne(path):
    """A 16-bit module: the starts the decompiler is told (describe.functions) plus the targets
    of near calls found while reading them, keyed "segment:offset" as the store keys them."""
    nef = NEFile(path)
    segs = {s["idx"]: s for s in nef.segments()}
    entries = {e["ordinal"]: e for e in nef.entries() if e.get("kind") in ("FIXED", "MOVEABLE")}
    refs = nef.module_refs()
    exported = {}
    for name, ordinal in nef.resident_names()[1:] + nef.nonresident_names()[1:]:
        e = entries.get(ordinal)
        if e:
            exported.setdefault((e["seg"], e["offset"]), name)
    starts = collections.defaultdict(set)
    for seg, off in describe.functions(nef):
        starts[seg].add(off)
    md = platforms.decoder(16, detail=True)
    side = Side(path, "ne", sum(s["length"] for s in segs.values() if s["kind"] == "CODE" and s["file_off"]))
    for idx in sorted(starts):
        s = segs.get(idx)
        if not s or s["kind"] != "CODE" or not s["file_off"]:
            continue
        code = nef.data[s["file_off"]:s["file_off"] + s["length"]]
        sites = _sites(nef, s, segs, entries, refs)
        offs = {o for o in starts[idx] if o < len(code)}
        bodies = {}
        for _round in range(8):                                  # near calls name more starts
            order = sorted(offs)
            new = set()
            for n, lo in enumerate(order):
                hi = order[n + 1] if n + 1 < len(order) else len(code)
                if lo in bodies and bodies[lo][1] == hi:
                    continue
                body = _body(md.disasm(code[lo:hi], lo), lo, hi)
                bodies[lo] = (body, hi)
                for ins in body:
                    if ins.mnemonic == "call" and ins.operands[0].type == X.X86_OP_IMM:
                        t = ins.operands[0].imm
                        if t < len(code) and t not in offs:
                            new.add(t)
            if not new or _round == 7:
                break
            offs |= new
        for lo in sorted(offs):
            body, hi = bodies[lo]
            name = exported.get((idx, lo))
            side.add(_function(_key16(idx, lo), name or "sub_%d_%04X" % (idx, lo), bool(name), body, lo, hi,
                               sites, lambda t, i=idx, o=offs: _key16(i, t) if t in o else None,
                               sites.get, lambda a: None, lambda v: False))
    return side.done()


def _load_dos(path):
    """A real-mode program as DOS loads it (unpacked): the functions dos_functions finds, keyed
    by linear address. Each is read at its offset in its own code segment, as the program runs it,
    so near calls and jumps land where they do at run time."""
    from codebreaker.analyze import dos_functions
    p = dos.read(path)
    found = dos_functions.discover(p).functions
    starts = sorted(found)
    end = p.base + len(p.image)
    md = platforms.decoder(16, detail=True)
    side = Side(path, "dos", sum(n for _lo, n, _label in p.spans))
    for n, lin in enumerate(starts):
        fn, seg = found[lin], found[lin].cs             # (the code segment's linear base)
        hi = starts[n + 1] if n + 1 < len(starts) else end
        if not (p.base <= lin < end) or not (0 <= lin - seg < 0x10000):
            continue
        lo_off, hi_off = lin - seg, min(hi - seg, 0x10000)
        relocated = {r - seg for r in p.relocations if lin <= r < hi}
        body = _body(md.disasm(p.image[lin - p.base:hi - p.base], lo_off), lo_off, hi_off)
        named = bool(fn.name) and not fn.name.startswith("sub_")
        side.add(_function("%05x" % lin, fn.name if named else "sub_%05X" % lin, named, body, lo_off, hi_off,
                           relocated, lambda t, s=seg: ("%05x" % (s + t)) if (s + t) in found else None,
                           lambda a: None, lambda a: None, lambda v: False))
    return side.done()


def load(path):
    """Read a module and fingerprint its functions. -> Side"""
    kind = detect.kind(path)
    if kind == "pe":
        return _load_pe(path)
    if kind == "ne":
        return _load_ne(path)
    if kind == "dos":
        return _load_dos(path)
    raise ValueError("%s: functions of a %s file are not compared yet" % (os.path.basename(path), kind or "non-executable"))


def shared(paths, level="text", least=8):
    """The functions several programs have in common -- a source module linked into each of them,
    the same code up to where the linker put things. Functions of fewer than `least` instructions
    are left out (stubs and thunks match everywhere).
    -> [(instructions, {program name: [keys]})], only fingerprints in two or more, biggest first"""
    table, sizes = collections.defaultdict(lambda: collections.defaultdict(list)), {}
    for path in paths:
        side = load(path)
        for f in side.functions.values():
            if len(f.text) >= least:
                h = f.ids[level]
                table[h][side.name].append(f.key)
                sizes[h] = len(f.text)
    out = [(sizes[h], dict(v)) for h, v in table.items() if len(v) >= 2]
    return sorted(out, key=lambda x: (-x[0], sorted(x[1])))


# ---- matching ---------------------------------------------------------------------------------
def _ratio(sa, sb):
    m = difflib.SequenceMatcher(None, sa, sb, autojunk=False)
    return m.ratio() if m.real_quick_ratio() >= 0.3 and m.quick_ratio() >= 0.3 else 0.0


def likeness(fa, fb, memo=None):
    """0..1: how alike two functions' shapes are (1 = the same sequence)."""
    if fa.ids["shape"] == fb.ids["shape"]:
        return 1.0
    key = (fa.key, fb.key)
    if memo is not None and key in memo:
        return memo[key]
    v = _ratio(fa.shape, fb.shape)
    if memo is not None:
        memo[key] = v
    return v


def _grams(f, n=5):
    s = f.shape
    return {hash(tuple(s[i:i + n])) for i in range(len(s) - n + 1)}


def match(a, b, least=0.6):
    """Pair the functions of two builds. -> {key in a: (key in b, how it was found)}"""
    pair, back, memo = {}, {}, {}
    A, B = a.functions, b.functions

    def link(x, y, how):
        fa, fb = A[x], B[y]
        if x in pair or y in back or (fa.named and fb.named and fa.name != fb.name):
            return
        pair[x] = (y, how)
        back[y] = x

    def unique(of, how, least_count):
        ga, gb = collections.defaultdict(list), collections.defaultdict(list)
        for funcs, taken, out in ((A, pair, ga), (B, back, gb)):
            for f in funcs.values():
                if f.key not in taken and f.count >= least_count:
                    out[of(f)].append(f)
        for h, xs in ga.items():
            ys = gb.get(h, ())
            if h is not None and len(xs) == 1 and len(ys) == 1:
                link(xs[0].key, ys[0].key, how)

    def calls_agree(fa, fb):
        """(places where the two call the same thing, places where they call different things)"""
        plus = minus = 0
        if len(fa.calls) == len(fb.calls):
            for ca, cb in zip(fa.calls, fb.calls):
                if ca[0] != cb[0]:
                    minus += 1
                elif ca[0] == "i":
                    plus, minus = plus + (ca == cb), minus + (ca != cb)
                elif ca[1] in pair:
                    plus, minus = plus + (pair[ca[1]][0] == cb[1]), minus + (pair[ca[1]][0] != cb[1])
                elif cb[1] in back:
                    minus += 1
        return plus, minus

    def by_calls():
        votes = collections.Counter()
        for x, (y, _how) in list(pair.items()):
            ca, cb = A[x].calls, B[y].calls
            if ca and cb:
                la = [c if c[0] == "i" else ("p", pair[c[1]][0]) if c[1] in pair else ("?",) for c in ca]
                lb = [c if c[0] == "i" else ("p", c[1]) if c[1] in back else ("?",) for c in cb]
                ops = [("equal", 0, len(la), 0, len(lb))] if la == lb else                     difflib.SequenceMatcher(None, la, lb, autojunk=False).get_opcodes()
                for tag, i1, i2, j1, j2 in ops:
                    if tag == "equal" or (tag == "replace" and i2 - i1 == j2 - j1):
                        for i, j in zip(range(i1, i2), range(j1, j2)):
                            if la[i] == ("?",) == lb[j] and ca[i][1] in A and cb[j][1] in B:
                                votes[(ca[i][1], cb[j][1])] += 2 if tag == "equal" else 1
            ua = [c for c in a.callers.get(x, ()) if c not in pair]
            ub = [c for c in b.callers.get(y, ()) if c not in back]
            if len(ua) == 1 and len(ub) == 1:
                votes[(ua[0], ub[0])] += 1
        best_a, best_b = {}, {}
        for (x, y), n in votes.items():
            same = any(A[x].ids[lv] == B[y].ids[lv] for lv in LEVELS)
            score = (n + (2 if same else 0), likeness(A[x], B[y], memo) if not same else 1.0)
            if (not same and score[1] < 0.4) or (A[x].named and B[y].named and A[x].name != B[y].name):
                continue
            if score > best_a.get(x, ((0, 0), None))[0]:
                best_a[x] = (score, y)
            if score > best_b.get(y, ((0, 0), None))[0]:
                best_b[y] = (score, x)
        for x, (_score, y) in best_a.items():
            if best_b[y][1] == x:
                link(x, y, "calls")

    def by_likeness():
        posting = collections.defaultdict(list)
        for f in B.values():
            if f.key not in back and f.count >= 5:
                for mark in f.marks | _grams(f):
                    posting[mark].append(f.key)
        of_a, of_b = collections.defaultdict(list), collections.defaultdict(list)
        for f in A.values():
            if f.key in pair or f.count < 5:
                continue
            cand = collections.Counter()
            for mark in f.marks | _grams(f):
                hits = posting.get(mark, ())
                if len(hits) <= 40:
                    cand.update(hits)
            for y, _n in cand.most_common(10):
                g = B[y]
                if 2 * min(f.count, g.count) < max(f.count, g.count) or (f.named and g.named and f.name != g.name):
                    continue
                v = likeness(f, g, memo)
                plus, minus = calls_agree(f, g)
                if v < least or minus > plus or (f.count < 12 and not plus and not f.marks & g.marks):
                    continue                                     # a small one needs more than its shape
                t = 1.0 if f.ids["text"] == g.ids["text"] else _ratio(f.text, g.text)
                score = (v + t) / 2 + (0.1 if plus else 0.0)
                of_a[f.key].append((score, y))
                of_b[y].append((score, f.key))

        def clear(found):                                        # one candidate, or one well ahead
            found.sort(reverse=True)
            return len(found) == 1 or found[0][0] - found[1][0] >= 0.03
        for x, found in of_a.items():
            if clear(found):
                y = found[0][1]
                if clear(of_b[y]) and of_b[y][0][1] == x:
                    link(x, y, "likeness")

    def by_order():
        """Unmatched functions of the same shape standing in the same place in both builds."""
        la = [("p", pair[x][0]) if x in pair else A[x].ids["shape"] for x in a.order]
        lb = [("p", y) if y in back else B[y].ids["shape"] for y in b.order]
        for i, j, n in difflib.SequenceMatcher(None, la, lb, autojunk=False).get_matching_blocks():
            run = [(a.order[i + k], b.order[j + k]) for k in range(n)]
            free = [(x, y) for x, y in run if x not in pair]
            if free and (len(free) < n or n >= 3):               # beside a matched one, or three in a row
                for x, y in free:
                    link(x, y, "position")

    def by_gaps():
        """The same few unmatched functions between two matched neighbours, if at all alike."""
        where = {k: i for i, k in enumerate(b.order)}
        run, last = [], -1
        for x in a.order + [None]:
            if x is not None and x not in pair:
                run.append(x)
                continue
            here = where[pair[x][0]] if x is not None else len(b.order)
            gap = [k for k in b.order[last + 1:here] if k not in back] if here > last else []
            if run and len(run) == len(gap) and len(run) <= 4:
                for p, q in zip(run, gap):
                    if likeness(A[p], B[q], memo) >= 0.4:
                        link(p, q, "position")
            run, last = [], max(last, here)

    unique(lambda f: f.name if f.named else None, "name", 0)
    unique(lambda f: f.ids["bytes"], "bytes", 4)
    unique(lambda f: f.ids["text"], "text", 4)
    unique(lambda f: f.ids["shape"], "shape", 8)
    for _round in range(64):
        n = len(pair)
        for step in (by_calls, by_likeness, by_order):
            step()
            if len(pair) > n:
                break
        if len(pair) == n:
            break
    by_gaps()
    by_calls()
    return pair


class Comparison(object):
    """What compare() found. `pairs` are dicts as in the --json file (without ops)."""

    def __init__(self, a, b, pair, seconds):
        self.a, self.b, self.seconds = a, b, seconds
        self.pairs = []
        for x in a.order:
            if x not in pair:
                continue
            y, how = pair[x]
            fa, fb = a.functions[x], b.functions[y]
            level = next((lv for lv in LEVELS if fa.ids[lv] == fb.ids[lv]), "")
            same = level in ("bytes", "text")
            self.pairs.append({"from": x, "to": y, "from_name": fa.name, "to_name": fb.name,
                               "status": "identical" if same else "changed", "level": level, "how": how,
                               "score": 1.0 if same else round(likeness(fa, fb), 3), "sizes": [fa.size, fb.size]})
        matched = {p["to"] for p in self.pairs}
        self.removed = [x for x in a.order if x not in pair]
        self.added = [y for y in b.order if y not in matched]

    def counts(self):
        c = collections.Counter(p["status"] for p in self.pairs)
        lv = collections.Counter(p["level"] for p in self.pairs if p["status"] == "identical")
        return {"identical": c["identical"], "identical_bytes": lv["bytes"], "identical_text": lv["text"],
                "changed": c["changed"], "added": len(self.added), "removed": len(self.removed)}

    def code(self):
        """Bytes of the new build's code: all of it, and what is identical / changed / added."""
        out = {"to_bytes": self.b.size, "identical": 0, "changed": 0,
               "added": sum(self.b.functions[y].size for y in self.added)}
        for p in self.pairs:
            out[p["status"]] += p["sizes"][1]
        return out

    def names(self):
        """Names the old build has for functions the new build has none for."""
        return [{"to": p["to"], "name": p["from_name"], "from": p["from"], "identical": p["status"] == "identical"}
                for p in self.pairs if self.a.functions[p["from"]].named and not self.b.functions[p["to"]].named]

    def reconstructions(self, fs=None):
        """The old build's verified reconstructions of functions the new build has unchanged."""
        from codebreaker.verify import reconstruct
        try:
            sha, digest = reconstruct.identity(self.a.path)
        except (ValueError, OSError):
            return []
        fs = fs or store.open()
        best = fs.best(sha, digest)
        out = []
        for p in self.pairs:
            row = best.get(p["from"])
            if row and p["status"] == "identical" and row["class"] in ("verified", "weak") and row["source"]:
                out.append({"from": p["from"], "to": p["to"], "class": row["class"], "engine": row["engine"],
                            "name": row["name"], "source": row["source"]})
        return out

    def carry_names(self, fs=None):
        """Give the carried names to the new build's bodies in the store (identical pairs only;
        a name given to a body holds in every module that has the body). -> how many, or None
        when the store has no bodies for the new build (`names` records them)."""
        fs = fs or store.open()
        bodies = fs.bodies(corpus.sha(self.b.path) or "") if hasattr(fs, "bodies") else {}
        if not bodies:
            return None
        n = 0
        for row in self.names():
            if row["identical"] and row["to"] in bodies:
                fs.name_body(bodies[row["to"]][0], row["name"], "the same function in %s of %s" % (
                    self.a.name, corpus.owner(self.a.path) or "another build"))
                n += 1
        return n

    def carry(self, fs=None):
        """File those reconstructions for the new build, to be judged there. -> how many"""
        from codebreaker.verify import reconstruct
        fs = fs or store.open()
        found = self.reconstructions(fs)
        if not found:
            return 0
        sha, digest = reconstruct.identity(self.b.path)
        fs.add_module(sha, self.b.name, self.b.path, self.b.kind)
        fs.add_functions(sha, list(self.b.order))
        old = corpus.sha(self.a.path) or ""
        fs.record_many(sha, ENGINE, old[:12], digest, [
            dict(addr=r["to"], name=self.b.functions[r["to"]].name if self.b.functions[r["to"]].named else r["name"],
                 source=r["source"], sig=hashlib.sha1(r["source"].encode("utf-8", "replace")).hexdigest(),
                 verdict=V.INCONCLUSIVE, reason="carried: identical to %s %s (%s there); not judged here" % (
                     self.a.name, r["from"], r["class"])) for r in found])
        return len(found)

    def report(self):
        c, code = self.counts(), self.code()
        pc = (lambda n: 100.0 * n / code["to_bytes"]) if code["to_bytes"] else (lambda n: 0.0)
        return ("%s: %d functions -> %s: %d\n  identical %d (same bytes %d, same instructions %d)   changed %d   "
                "added %d   removed %d\n  of the new build's %d bytes of code: %.1f%% identical, %.1f%% changed, "
                "%.1f%% added   (%.1fs)" % (
                    self.a.name, len(self.a.order), self.b.name, len(self.b.order), c["identical"],
                    c["identical_bytes"], c["identical_text"], c["changed"], c["added"], c["removed"],
                    code["to_bytes"], pc(code["identical"]), pc(code["changed"]), pc(code["added"]), self.seconds))

    def as_json(self, listings=True, fs=None):
        def about(side):
            return {"path": side.path, "name": side.name, "corpus": corpus.owner(side.path),
                    "sha": corpus.sha(side.path), "kind": side.kind, "functions": len(side.order), "bytes": side.size,
                    "code": side.code}

        def lines(side, keys):
            return {k: [["%x" % addr, row, tok] for (addr, row), tok in zip(side.functions[k].rows, side.functions[k].text)]
                    for k in keys}
        pairs = []
        for p in self.pairs:
            p = dict(p)
            if p["status"] == "changed" and listings:
                m = difflib.SequenceMatcher(None, self.a.functions[p["from"]].text, self.b.functions[p["to"]].text, autojunk=False)
                p["ops"] = [list(op) for op in m.get_opcodes()]
            pairs.append(p)
        out = {"from": about(self.a), "to": about(self.b), "counts": self.counts(), "code": self.code(),
               "seconds": round(self.seconds, 2), "pairs": pairs,
               "added": [{"key": k, "name": self.b.functions[k].name, "size": self.b.functions[k].size} for k in self.added],
               "removed": [{"key": k, "name": self.a.functions[k].name, "size": self.a.functions[k].size} for k in self.removed],
               "names": self.names(), "reconstructions": self.reconstructions(fs)}
        if listings:
            changed = [p for p in self.pairs if p["status"] == "changed"]
            out["listings"] = {"from": lines(self.a, [p["from"] for p in changed] + self.removed),
                               "to": lines(self.b, [p["to"] for p in changed] + self.added)}
        return out


def compare(path_a, path_b, least=0.6):
    """Match the functions of two builds of a module. -> Comparison"""
    t = time.time()
    a, b = load(path_a), load(path_b)
    return Comparison(a, b, match(a, b, least), time.time() - t)


# ---- a whole product ---------------------------------------------------------------------------
def _one(context, item):
    """Worker: one module of the new product against the old product's copy. -> a row"""
    name, old, new = item
    row = {"name": name, "status": "compared"}
    try:
        if corpus.sha(old) == corpus.sha(new):
            side = load(new)
            row.update(status="same file", functions=len(side.order), bytes=side.size, identical=len(side.order),
                       identical_code=side.size, changed=0, changed_code=0, added=0, removed=0, seconds=0.0)
            return row
        c = compare(old, new, context)
        n, code = c.counts(), c.code()
        row.update(functions=len(c.b.order), bytes=code["to_bytes"], identical=n["identical"],
                   identical_code=code["identical"], changed=n["changed"], changed_code=code["changed"],
                   added=n["added"], removed=n["removed"], seconds=round(c.seconds, 1))
    except Exception as e:                                       # one module must not end the run
        row.update(status="failed", error="%s: %s" % (type(e).__name__, str(e)[:120]))
    return row


def product(old, new, sample=0, jobs=None, least=0.6, log=print):
    """How much of product `new` is already known from product `old`: every 16- and 32-bit
    module of `new` (or an evenly spread `sample` of them) against the same-named module of
    `old`. -> {"from", "to", "modules": [row], "totals": {...}}"""
    with corpus.selected(new):
        paths = sorted(corpus.modules("pe") + corpus.modules("ne"), key=lambda p: os.path.basename(p).lower())
    if sample and sample < len(paths):
        paths = [paths[i * len(paths) // sample] for i in range(sample)]
    items, rows = [], []
    with corpus.selected(old):
        for p in paths:
            name = os.path.basename(p).lower()
            before = corpus.retail(name)
            if before and corpus.lane(before) in ("pe", "ne"):
                items.append((name, before, p))
            else:
                rows.append({"name": name, "status": "new module", "path": p})
    done = pool.run(_one, items, context=least, jobs=min(jobs or pool.default_jobs(), 6), small=4,
                    crashed=lambda item: {"name": item[0], "status": "failed", "error": "the worker died"})
    for row in rows:                                             # a new module is all added code
        try:
            side = load(row.pop("path"))
            row.update(functions=len(side.order), bytes=side.size, added=len(side.order))
        except Exception as e:
            row.update(status="failed", error="%s: %s" % (type(e).__name__, str(e)[:120]))
    rows = sorted(rows + list(done.values()), key=lambda r: r["name"])
    tot = collections.Counter()
    for r in rows:
        tot["modules"] += 1
        tot[r["status"]] += 1
        for k in ("functions", "bytes", "identical", "identical_code", "changed", "changed_code", "added", "removed"):
            tot[k] += r.get(k, 0)
    for r in rows:
        if r["status"] == "failed":
            log("  %-14s FAILED %s" % (r["name"], r["error"]))
        else:
            log("  %-14s %-10s %5d functions: identical %5d  changed %5d  added %5d  removed %5d" % (
                r["name"], r["status"], r.get("functions", 0), r.get("identical", 0), r.get("changed", 0),
                r.get("added", 0), r.get("removed", 0)))
    return {"from": old, "to": new, "modules": rows, "totals": dict(tot)}


def _option(argv, name, default=None):
    if name in argv:
        i = argv.index(name)
        value = argv[i + 1]
        del argv[i:i + 2]
        return value
    return default


def main(argv):
    """python -m codebreaker versions <module> --from CORPUS --to CORPUS [--json FILE] [--carry]
    python -m codebreaker versions <fileA> <fileB> [--json FILE] [--carry]
    python -m codebreaker versions --all --from CORPUS --to CORPUS [--sample N] [--jobs N] [--json FILE]
    python -m codebreaker versions --shared <program> <program> ... [--level text|shape|bytes] [--json FILE]

    Match the functions of two builds of a module and say which are identical, changed, added
    and removed; with --all, do it for every module of a product and total it; with --shared,
    list the functions several different programs have in common."""
    argv = list(argv)
    old, new = _option(argv, "--from"), _option(argv, "--to")
    out, least = _option(argv, "--json"), float(_option(argv, "--least", "0.6"))
    sample, jobs = int(_option(argv, "--sample", "0")), int(_option(argv, "--jobs", "0"))
    level = _option(argv, "--level", "text")
    flags = {a for a in argv if a.startswith("--")}
    names = [a for a in argv if not a.startswith("--")]

    def save(data):
        if out:
            with open(out, "w", encoding="utf-8", newline="\n") as f:
                json.dump(data, f)
            print("wrote %s" % out)

    if "--shared" in flags:
        if len(names) < 2:
            raise SystemExit("--shared needs two programs or more")
        if level not in LEVELS:
            raise SystemExit("--level is one of %s" % ", ".join(LEVELS))
        paths = [corpus.retail(n) or n for n in names]
        found = shared(paths, level)
        stems = [os.path.basename(p).lower() for p in paths]
        print("%s: %d functions in two programs or more (%s)" % (", ".join(stems), len(found), level))
        for i in range(len(stems)):
            for j in range(i + 1, len(stems)):
                both = sum(1 for _n, where in found if stems[i] in where and stems[j] in where)
                print("  %s and %s: %d" % (stems[i], stems[j], both))
        if found:
            print("largest (instructions: program@keys):")
        for n, where in found[:12]:
            print("  %4d  %s" % (n, "  ".join("%s@%s" % (s, ",".join(k)) for s, k in sorted(where.items()))))
        save({"level": level, "programs": stems,
              "shared": [{"instructions": n, "where": where} for n, where in found]} if out else None)
        return 0
    if "--all" in flags and old and new:
        t = time.time()
        result = product(old, new, sample, jobs, least)
        tot = result["totals"]
        result["totals"]["seconds"] = round(time.time() - t, 1)
        code = tot.get("bytes", 0) or 1
        print("%s -> %s: %d modules (%d compared, %d the same file, %d new, %d failed)\n"
              "  %d functions: identical %d, changed %d, added %d; removed from the old ones %d\n"
              "  of %d bytes of code: %.1f%% identical to %s, %.1f%% changed   (%.0fs)" % (
                  old, new, tot["modules"], tot.get("compared", 0), tot.get("same file", 0), tot.get("new module", 0),
                  tot.get("failed", 0), tot.get("functions", 0), tot.get("identical", 0), tot.get("changed", 0),
                  tot.get("added", 0), tot.get("removed", 0), tot.get("bytes", 0),
                  100.0 * tot.get("identical_code", 0) / code, old, 100.0 * tot.get("changed_code", 0) / code,
                  tot["seconds"]))
        save(result)
        return 1 if tot.get("failed") else 0
    if len(names) == 2 and all(os.path.isfile(n) for n in names):
        path_a, path_b = names
    elif len(names) == 1 and old and new:
        with corpus.selected(old):
            path_a = corpus.retail(names[0])
        with corpus.selected(new):
            path_b = corpus.retail(names[0])
        if not path_a or not path_b:
            raise SystemExit("no %s in %s" % (names[0], old if not path_a else new))
    else:
        print(main.__doc__)
        return 2
    c = compare(path_a, path_b, least)
    print(c.report())
    carried = c.names()
    known = c.reconstructions()
    print("  names carried to the new build: %d (%d onto identical bodies)   verified reconstructions that "
          "carry: %d" % (len(carried), sum(1 for n in carried if n["identical"]), len(known)))
    if "--carry" in flags:
        print("  filed %d reconstructions for %s in the store, to be judged" % (c.carry(), c.b.name))
        named = c.carry_names()
        print("  names given to its bodies in the store: %s" % (
            named if named is not None else "none -- its bodies are not recorded there yet"))
    save(c.as_json() if out else None)
    return 0
