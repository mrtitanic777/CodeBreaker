"""ppc_calls.py -- what a PowerPC program's calls show about its functions, from the binary alone.

    python -m codebreaker ppc-calls <module.xex or image> [-o calls.tsv]
    from codebreaker.analyze import ppc_calls
    r = ppc_calls.analyse(path)
    r.arity        {address: argument registers r3.. its callers set up}   (internal functions)
    r.imports      {name: argument registers}                            (imported functions)
    r.clobber      {address: mask of r0, r4-r12 it may change}           (only those that keep some)
    r.fpclobber    {address: mask of f0-f13 it may change}               (only those that keep some)
    r.variadic     {address: fixed arguments before the variable ones}

Each is evidence, never a guess:

  * Arity: before a `bl`, the argument registers r3, r4, ... set within the 14 instructions back
    (stopping at an earlier call), counted while they run on without a gap. A function's arity is
    the count most of its call sites agree on: at least 3 sites, 60 % agreeing (85 % for 8, which
    a longer argument list also gives), and 1-7 (0 is a pass-through as often as no arguments). An
    import is a stub with no body to pass through, so 0 and 8 count there; it needs 3 sites and
    85 %, and the printf family, whose count varies by call, is left out.
  * Clobber: the volatile registers a function may change -- those it writes, and those every
    function it calls may change, to a fixed point over the call graph. A call to an import, through
    a pointer or out of the known functions may change them all. The compiler's register save and
    restore runs (__savegprlr_N ...) change nothing the caller sees, and neither does a register
    reloaded from where the function stored it on its frame. What is reported is what a function
    provably keeps: a caller may read those registers across the call.
  * Variadic: a function that stores r8, r9 and r10 to its caller's frame before its own `stwu`
    is homing the va_list area; its fixed arguments are the registers below the lowest one homed.

These passes come from the clean-room xam decompile (../xam, tools/infer_*.py), where they turned
thousands of refused functions into faithful C; here they run on CodeBreaker's own listing
(recompile/disassemble.py) and function discovery (recompile/discover.py), for any module.
"""
import collections
import re
import sys

ARG_REGS = range(3, 11)
VOLATILE = (0, 4, 5, 6, 7, 8, 9, 10, 11, 12)          # r3 is the return value: always changed
VOLATILE_MASK = sum(1 << r for r in VOLATILE)
FP_VOLATILE_MASK = (1 << 14) - 1                       # f0-f13
LOOKBACK = 14
_REG = re.compile(r"^r(\d+)$")
_FREG = re.compile(r"^f(?:r)?(\d+)$")
_MEM = re.compile(r"^(-?(?:0x)?[0-9a-fA-F]+)\((r\d+)\)$")
# instructions whose first operand is a register they set (for the call-site count)
_SETS = {"li", "lis", "mr", "addi", "addis", "subi", "ori", "oris", "lwz", "lhz", "lbz", "ld", "add", "subf",
         "or", "and", "rlwinm", "mulli", "extsb", "extsh", "neg", "lwzx", "divwu", "mullw", "srawi", "xor",
         "slw", "srw", "addic", "rlwimi", "clrlwi", "slwi", "srwi", "rotlwi", "lha", "lwa", "extsw", "nor", "not"}
_NO_WRITE_PREFIX = ("cmp", "b", "st", "dcb", "tw", "td", "tlb", "mt")
_NO_WRITE = {"nop", "sync", "isync", "eieio", "lwsync", "twi", "tdi", "ptesync", "tlbsync", "dcbz128"}
_UPDATE_STORE = {"stwu", "sthu", "stbu", "stdu", "stwux", "sthux", "stbux", "stdux", "stfsu", "stfdu", "stfsux",
                 "stfdux", "lfsu", "lfdu", "lfsux", "lfdux"}
_UPDATE_LOAD = {"lwzu", "lhzu", "lbzu", "ldu", "lhau", "lwau", "lwzux", "lhzux", "lbzux", "ldux", "lhaux"}
_INDIRECT = {"bctrl", "bctr", "bcctr", "bcctrl", "bclrl"}
_VARIADIC_IMPORTS = re.compile(r"printf|scanf|DbgPrint|ProducerLog|vscw|vsnw|_vsn|_vsc")


class Result:
    def __init__(self):
        self.arity, self.imports, self.clobber, self.fpclobber, self.variadic = {}, {}, {}, {}, {}
        self.functions = 0


def _ops(text):
    return [t.strip() for t in text.split(",")] if text.strip() else []


def _reg(tok, pattern=_REG):
    m = pattern.match(tok or "")
    return int(m.group(1)) if m else None


def _target(ops):
    return int(ops[0], 16) if ops and ops[0].lower().startswith("0x") else None


def gpr_writes(mnem, ops):
    """The general registers an instruction writes (an over-approximation)."""
    b = mnem.rstrip(".")
    if not ops:
        return set()
    if b == "lmw":
        r = _reg(ops[0])
        return set(range(r, 32)) if r is not None else set()
    base = None
    if len(ops) >= 2:
        m = _MEM.match(ops[1])
        base = int(m.group(2)[1:]) if m else _reg(ops[1])
    if b in _UPDATE_STORE:
        return {base} if base is not None else set()
    if b in _UPDATE_LOAD:
        return {r for r in (_reg(ops[0]), base) if r is not None}
    if b.startswith(_NO_WRITE_PREFIX) or b in _NO_WRITE:
        return set()
    r = _reg(ops[0])
    return {r} if r is not None else set()


def fpr_writes(mnem, ops):
    b = mnem.rstrip(".")
    if not ops or b.startswith(("stf", "fcmp", "mtfsf", "b")):
        return set()
    r = _reg(ops[0], _FREG)
    return {r} if r is not None else set()


def _functions(path):
    """{start: [(address, mnemonic, operands)]} over discovered functions, and the image."""
    from codebreaker.recompile import disassemble, discover
    funcs, _sources, img = discover.discover(path)
    lo, hi = img.code
    code = img.bytes_at(lo, hi - lo)
    starts = sorted(a for a in funcs if lo <= a < hi and a not in img.imports and a not in img.run_names)
    out = {}
    for i, a in enumerate(starts):
        size = funcs[a] or 0
        end = a + size if size else (starts[i + 1] if i + 1 < len(starts) else hi)
        if i + 1 < len(starts):
            end = min(end, starts[i + 1])
        end = min(end, hi)
        out[a] = [(va, mn, _ops(op)) for va, _raw, mn, op in disassemble.listing(code, lo, a - lo, (end - a) // 4)]
    return out, img


def _site_arity(body, i):
    regs = set()
    for j in range(i - 1, max(-1, i - LOOKBACK), -1):
        _va, mn, ops = body[j]
        if mn in ("bl", "bla"):
            break
        if mn.rstrip(".") in _SETS and ops:
            r = _reg(ops[0])
            if r is not None and r in ARG_REGS:
                regs.add(r)
    k = 0
    while 3 + k in regs:
        k += 1
    return k


def _consensus(arities, stub):
    if len(arities) < 3:
        return None
    top, n = collections.Counter(arities).most_common(1)[0]
    share = n / len(arities)
    if stub:
        return top if share >= 0.85 else None
    if top == 8:
        return 8 if share >= 0.85 else None
    return top if share >= 0.6 and 1 <= top <= 7 else None


def analyse(path):
    from codebreaker.formats import xex, xex_imports
    funcs, img = _functions(path)
    names = {}
    if xex.is_xex(path):
        for _lib, _ordinal, name, kind, _slot, thunk in xex_imports.imports(path):
            if thunk:
                names[thunk] = name
    for a, n in img.imports.items():
        names.setdefault(a, n)
    transparent = set(img.run_names)
    sites = collections.defaultdict(list)
    local, fplocal = collections.defaultdict(int), collections.defaultdict(int)
    callees, everything = collections.defaultdict(set), set()
    r = Result()
    r.functions = len(funcs)
    for a, body in funcs.items():
        saved = {(_reg(ops[0]), _MEM.match(ops[1]).group(1)) for _va, mn, ops in body
                 if mn == "stw" and len(ops) >= 2 and _MEM.match(ops[1]) and _MEM.match(ops[1]).group(2) == "r1"}
        homed = set()
        for _va, mn, ops in body[:16]:
            if mn == "stwu":
                break
            if mn in ("stw", "std") and len(ops) >= 2 and ops[1].endswith("(r1)"):
                reg = _reg(ops[0])
                if reg is not None:
                    homed.add(reg)
        if {8, 9, 10} <= homed:
            fixed = min(reg for reg in homed if 3 <= reg <= 10) - 3
            if 0 <= fixed <= 7:
                r.variadic[a] = fixed
        for i, (_va, mn, ops) in enumerate(body):
            b = mn.rstrip(".")
            if b in ("bl", "bla"):
                t = _target(ops)
                if t is not None:
                    sites[t].append(_site_arity(body, i))
                if t in transparent:
                    continue
                if t in funcs:
                    callees[a].add(t)
                else:
                    everything.add(a)
                continue
            if b in _INDIRECT:
                everything.add(a)
                continue
            if b in ("lwz", "lhz", "lbz", "ld") and len(ops) >= 2:
                m = _MEM.match(ops[1])
                if m and m.group(2) == "r1" and (_reg(ops[0]), m.group(1)) in saved:
                    continue                          # the value the function saved: not a change
            for w in gpr_writes(mn, ops):
                if w in VOLATILE:
                    local[a] |= 1 << w
            for w in fpr_writes(mn, ops):
                if w < 14:
                    fplocal[a] |= 1 << w
    for t, ars in sites.items():
        if t in names:
            if not _VARIADIC_IMPORTS.search(names[t]):
                n = _consensus(ars, stub=True)
                if n is not None:
                    r.imports[names[t]] = n
        elif t in funcs:
            n = _consensus(ars, stub=False)
            if n is not None:
                r.arity[t] = n
    for out, loc, full in ((r.clobber, local, VOLATILE_MASK), (r.fpclobber, fplocal, FP_VOLATILE_MASK)):
        mask = {a: (full if a in everything else loc.get(a, 0)) for a in funcs}
        changed = True
        while changed:
            changed = False
            for a in funcs:
                m = mask[a]
                if m == full:
                    continue
                for g in callees[a]:
                    m |= mask.get(g, full)
                if m != mask[a]:
                    mask[a], changed = m, True
        out.update({a: m for a, m in mask.items() if m != full})
    return r


def write_tsv(r, out):
    with open(out, "w", encoding="utf-8") as f:
        f.write("kind\taddress or name\tvalue\n")
        for a, n in sorted(r.arity.items()):
            f.write("arity\t%08X\t%d\n" % (a, n))
        for name, n in sorted(r.imports.items()):
            f.write("import arity\t%s\t%d\n" % (name, n))
        for a, n in sorted(r.variadic.items()):
            f.write("variadic\t%08X\t%d\n" % (a, n))
        for a, m in sorted(r.clobber.items()):
            f.write("clobber\t%08X\t%s\n" % (a, " ".join("r%d" % k for k in VOLATILE if m >> k & 1) or "-"))
        for a, m in sorted(r.fpclobber.items()):
            f.write("fpclobber\t%08X\t%s\n" % (a, " ".join("f%d" % k for k in range(14) if m >> k & 1) or "-"))


def main(argv):
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__)
        return 0 if argv else 2
    out = argv[argv.index("-o") + 1] if "-o" in argv else None
    r = analyse(argv[0])
    print("%d functions: arity for %d, import arity for %d, %d variadic, %d keep a volatile register, "
          "%d keep a volatile FP register" % (r.functions, len(r.arity), len(r.imports), len(r.variadic),
                                               len(r.clobber), len(r.fpclobber)))
    if out:
        write_tsv(r, out)
        print("written: %s" % out)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
