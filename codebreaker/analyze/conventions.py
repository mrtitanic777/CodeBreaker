"""conventions.py -- how each function is called and what kind of function it is.

    from codebreaker.analyze import image, conventions
    facts = conventions.functions(image.load("comctl32.dll"))     # {start: fact}
    facts[va]["convention"]   "stdcall" | "cdecl" | "thiscall" | "fastcall" | "register"
                              16-bit: "pascal far" | "pascal near" | "cdecl far" | "cdecl near"
    facts[va]["args"]         number of stack arguments (words in 16-bit code), or None
    facts[va]["arg_source"]   what says so: "return" (`ret N`), "callers" (the bytes callers
                              remove after calling it), "frame" (the argument slots it reads)
    facts[va]["reg_args"]     registers it reads before writing them (ecx: `this`)
    facts[va]["result"]       "none" | "value" | "flag" | "float" | "wide" | None (not known)
    facts[va]["shape"]        "import thunk" | "jump thunk" | "empty" | "constant" | "leaf" | "calls"
    ... frame, locals, saved registers, loops, inline string operations, floating point use,
    whether it takes a variable argument list, a 16-bit function's entry sequence and, when it
    is a window, dialog, hook or enumeration procedure, which.

What each fact rests on is the code itself and its callers:

  * a function that removes its own arguments says how many in its return instruction;
  * one that leaves them to the caller is given away by its callers (`add esp, 8` after the
    call) and by the highest argument slot it reads;
  * a register read before anything wrote it was passed in;
  * a result is a value the callers use, or that the function sets just before returning;
    callers that all ignore it, and no path that sets it, mean there is none.

The decompiler's description (decompile/describe.py) already carries the bytes a function
removes on return. What it cannot yet be told, and this finds: the argument count of a
caller-cleans function, register arguments, and a function that returns nothing.
"""
import hashlib
import re

from codebreaker.analyze import call_graph, image
from codebreaker.analyze.image import family

_SCRATCH = ("eax", "ecx", "edx")
_INPUTS = ("eax", "ecx", "edx", "ebx", "esi", "edi")
_STRING_OPS = {"movs": "copy", "stos": "fill", "scas": "scan", "cmps": "compare", "lods": "load"}
_VA_LIST = re.compile(r"(^|!)(w?vs?n?w?printf|wvsprintf[AW]?|_vsn?w?printf|vfw?printf|_?vsprintf|FormatMessage[AW])$", re.I)
_THUNK_APIS = ("THUNKCONNECT32", "THUNKCONNECT16", "C16THKSL01", "C32THKSL01", "THUNKINITLS", "QT_THUNK",
               "FT_THUNK", "FT_PROLOG", "FT_EXIT", "SSONBIGSTACK", "SSCALL", "MAPLS", "UNMAPLS", "MAPSL",
               "GETVDMPOINTER32W", "ALLOCSLCALLBACK", "GETPROCADDRESS32W", "K32THK1632PROLOG", "FT_EXIT0")
_CALLBACK_NAMES = ((re.compile(r"(WND|WINDOW|MDICHILD|FRAME|SUBCLASS)PROC$|WNDPROC", re.I), "window procedure"),
                   (re.compile(r"(DLG|DIALOG)(PROC|FN)?$|DLGPROC", re.I), "dialog procedure"),
                   (re.compile(r"HOOK(PROC)?$", re.I), "hook procedure"),
                   (re.compile(r"ENUM\w*PROC$", re.I), "enumeration procedure"))
_CALLBACK_CALLS = {"DEFWINDOWPROC": "window procedure", "DEFMDICHILDPROC": "window procedure",
                   "DEFFRAMEPROC": "window procedure", "CALLWINDOWPROC": "window procedure",
                   "DEFDLGPROC": "dialog procedure", "ENDDIALOG": "dialog procedure",
                   "CALLNEXTHOOKEX": "hook procedure", "DEFHOOKPROC": "hook procedure"}


def stack_delta(ins):
    """Bytes `ins` adds to what is pushed (negative: removes), or None when it is not plain."""
    mn, ops = ins.mn, ins.ops
    if mn == "push":
        return ops[0][-1] if ops else 4
    if mn == "pop":
        return -(ops[0][-1] if ops else 4)
    if mn in ("add", "sub") and len(ops) == 2 and ops[0][0] == "r" and family(ops[0][1]) == "esp" and ops[1][0] == "i":
        n = image.signed(ops[1][1], ops[1][2])
        return n if mn == "sub" else -n
    return 0 if "esp" not in ins.wr else None


def _frame(fn, word):
    """(uses a frame pointer, bytes of locals, callee-saved registers pushed on entry)"""
    framed, local, saved = False, 0, []
    for ins in fn.insns[:12]:
        mn, ops = ins.mn, ins.ops
        if mn == "push" and ops and ops[0][0] == "r":
            fam = family(ops[0][1])
            if (fam in ("ebx", "esi", "edi") or (fam == "ebp" and not framed)) and fam not in saved:
                saved.append(fam)
                continue
            if fam in ("ds", "es"):
                continue
            break
        if mn == "mov" and len(ops) == 2 and ops[0][0] == "r" and ops[1][0] == "r" \
                and family(ops[0][1]) == "ebp" and family(ops[1][1]) == "esp":
            framed = True
        elif mn == "enter" and ops:
            framed, local = True, ops[0][1]
        elif mn in ("sub", "add") and len(ops) == 2 and ops[0][0] == "r" and family(ops[0][1]) == "esp" \
                and ops[1][0] == "i":
            n = image.signed(ops[1][1], ops[1][2])
            local = max(local, n if mn == "sub" else -n)
        elif not (ins.mn in ("inc", "nop", "pop") or (mn == "mov" and ops[0][0] == "r" and family(ops[0][1]) in ("eax", "edi", "ds", "es"))):
            break                                       # past the entry sequence
    return framed, max(local, 0), [r for r in saved if r != "ebp"]


def _argument_slots(fn, framed, first, word):
    """Offsets (from the first argument) of the argument slots the function reads or writes,
    with the widest access to each: {offset: size}."""
    slots = {}
    if framed:
        for ins in fn.insns:
            for op in ins.ops:
                if op[0] == "m" and op[1] and family(op[1]) == "ebp" and op[2] is None and op[4] >= first:
                    slots[op[4] - first] = max(slots.get(op[4] - first, 0), op[6])
        return slots
    depth = 0                                           # no frame: follow the stack through the entry code
    for ins in fn.insns:
        if ins.flow in ("call", "jmp", "ret"):
            break
        for op in ins.ops:
            if op[0] == "m" and op[1] and family(op[1]) == "esp" and op[2] is None and op[4] >= depth + first - word:
                off = op[4] - depth - (first - word)
                slots[off] = max(slots.get(off, 0), op[6])
        d = stack_delta(ins)
        if d is None:
            break
        depth += d
    return slots


def _sets_constant(ins):
    """Does `ins` give its register a value that does not depend on what it held?
    xor r, r / sub r, r / sbb r, r / or r, -1 / and r, 0"""
    ops = ins.ops
    if len(ops) != 2 or ops[0][0] != "r":
        return False
    if ins.mn in ("xor", "sub", "sbb"):
        return ops[0] == ops[1]
    if ops[1][0] != "i":
        return False
    return (ins.mn == "or" and image.signed(ops[1][1], ops[1][2]) == -1) or (ins.mn == "and" and ops[1][1] == 0)


def _register_inputs(fn):
    """Registers read before anything wrote them, near the entry: on a path of at most a few
    branches and a few dozen instructions. (Further in, "no write seen on this path" says more
    about the paths not followed -- jump tables, loops -- than about the function's inputs.)"""
    index = fn.index
    found, seen = set(), set()
    work = [(0, frozenset(), 0)]
    budget = 90
    while work and budget > 0:
        i, written, depth = work.pop()
        while i is not None and i < len(fn.insns) and budget > 0:
            key = (i, written)
            if key in seen:
                break
            seen.add(key)
            budget -= 1
            ins = fn.insns[i]
            ops = ins.ops
            zeroing = _sets_constant(ins)
            if ins.mn != "push" and not zeroing:
                for r in ins.rd:
                    if r in _INPUTS and r not in written:
                        found.add(r)
            elif ins.mn == "push" and ops and ops[0][0] == "m":
                for r in ins.rd:
                    if r in _INPUTS and r not in written:
                        found.add(r)
            if ins.flow == "call":
                written = written | frozenset(_SCRATCH)
            elif ins.flow == "ret":
                break
            new = written | (ins.wr & frozenset(_INPUTS))
            if ins.mn == "pop" and ops and ops[0][0] == "r":
                new = new | {family(ops[0][1])}
            written = new
            if ins.flow == "jmp":
                i = index.get(ins.target) if ins.target is not None else None
                continue
            if ins.flow == "jcc" and ins.target in index and depth < 3:
                work.append((index[ins.target], written, depth + 1))
            nxt = ins.addr + ins.size
            i = index.get(nxt)
    return found


def _returns(fn):
    """(pops, what each return leaves in the result register)"""
    pops, kinds = [], []
    labels = {ins.target for ins in fn.insns if ins.flow in ("jcc", "jmp") and ins.target is not None}
    for i, ins in enumerate(fn.insns):
        if ins.flow != "ret":
            continue
        pops.append(ins.ops[0][1] if ins.ops else 0)
        kind, wide, flt = "unknown", False, None
        for j in range(i - 1, max(-1, i - 12), -1):
            b = fn.insns[j]
            if b.mn[0] == "f" and flt is None and b.mn not in ("fwait", "fnclex", "fldcw", "fnstcw", "fstcw"):
                flt = not (b.mn.startswith("fstp") or b.mn.startswith("fistp") or b.mn in ("fcompp", "fucompp"))
            if "edx" in b.wr and b.flow != "call" and b.mn not in ("pop", "cdq", "cwd"):
                wide = True
            if b.flow == "call":
                kind = "call"
                break
            if "eax" in b.wr and b.mn != "pop":
                ops = b.ops
                if b.mn in ("xor", "sub") and len(ops) == 2 and ops[0] == ops[1]:
                    kind = ("const", 0)
                elif b.mn == "mov" and ops[1][0] == "i":
                    kind = ("const", ops[1][1])
                elif b.mn == "or" and ops[1][0] == "i" and image.signed(ops[1][1], ops[1][2]) == -1:
                    kind = ("const", -1)
                else:
                    kind = "value"
                break
            if b.flow in ("jmp", "ret") or (b.addr in labels and j < i - 1):
                break
        kinds.append((kind, wide, bool(flt)))
    return pops, kinds


def _result_use(img, graph):
    """{callee: [uses, ignores]} -- what callers do with the result register after a call."""
    out = {}
    for f in img.functions:
        fn = img.function(f)
        for site, kind, dest in graph.calls[f]:
            if kind != "call":
                continue
            i = fn.at(site)
            tally = out.setdefault(dest, [0, 0])
            for b in fn.insns[i + 1:i + 5]:
                if "eax" in b.rd and not _sets_constant(b) and b.flow != "ret":
                    tally[0] += 1
                    break
                if "eax" in b.wr or b.flow == "call":
                    tally[1] += 1
                    break
                if b.flow:
                    break
    return out


def _cleanup(img, graph):
    """{callee: {bytes: sites}} -- what callers remove from the stack right after a call."""
    out = {}
    word = 4 if img.bits == 32 else 2
    for f in img.functions:
        fn = img.function(f)
        for site, kind, dest in graph.calls[f]:
            if kind != "call":
                continue
            i, n = fn.at(site) + 1, 0
            while i < len(fn.insns):
                b = fn.insns[i]
                if b.mn == "add" and len(b.ops) == 2 and b.ops[0][0] == "r" and family(b.ops[0][1]) == "esp" \
                        and b.ops[1][0] == "i":
                    n += image.signed(b.ops[1][1], b.ops[1][2])
                elif b.mn == "pop" and b.ops and b.ops[0][0] == "r" and family(b.ops[0][1]) in ("ecx", "ebx") and n < 2 * word:
                    n += word
                    i += 1
                    continue
                break
            votes = out.setdefault(dest, {})
            votes[n] = votes.get(n, 0) + 1
    return out


def _loaded_before(img, graph):
    """{callee: {register: (sites that load it just before the call, sites)}} -- ECX and EDX
    written by one of the three instructions before a call (and not by a call among them)."""
    out = {}
    for f in img.functions:
        fn = img.function(f)
        for site, kind, dest in graph.calls[f]:
            if kind != "call":
                continue
            i = fn.at(site)
            loaded = set()
            for b in fn.insns[max(0, i - 3):i][::-1]:
                if b.flow:
                    break
                loaded |= set(b.wr) & {"ecx", "edx"}
            votes = out.setdefault(dest, {"sites": 0, "ecx": 0, "edx": 0})
            votes["sites"] += 1
            for r in loaded:
                votes[r] += 1
    return out


def _pushed_inputs(fn, loads):
    """Registers a function pushes at its entry that its callers load for it: an argument passed
    in a register and kept on the stack (`push ecx / push 40h / call [LocalAlloc]`, awfaxp32's
    allocator taking its size in ECX). A `push ecx` alone also makes room for a local, so it
    counts only when nearly every caller (at least two) loads the register just before the call."""
    out = set()
    real = [i for i in fn.insns if i.mn not in ("nop", "int3")]
    for ins in real[:4]:
        if ins.mn == "push" and ins.ops and ins.ops[0][0] == "i":
            continue                                       # (a constant pushed among them: `push 42h / push edx`)
        if ins.mn != "push" or not ins.ops or ins.ops[0][0] != "r":
            break
        r = family(ins.ops[0][1])
        v = loads or {}
        if r in ("ecx", "edx") and v.get("sites", 0) >= 2 and v.get(r, 0) * 10 >= v["sites"] * 9:
            out.add(r)
    return out


def _entry16(img, fn):
    """What a 16-bit function's first bytes say about how it is entered."""
    head = img.read(fn.start, 8)
    notes = []
    if head[:3] in (b"\x1e\x58\x90", b"\x8c\xd8\x90"):          # push ds / pop ax / nop;  mov ax, ds / nop
        notes.append("exported entry (the loader patches in the data segment)")
        head = head[3:]
    if head[:1] == b"\xb8" and b"\x8e\xd8" in img.read(fn.start, 12):       # mov ax, SEG / ... / mov ds, ax
        notes.append("loads its own data segment")
    if head[:4] == b"\x45\x55\x8b\xec":                                    # inc bp / push bp / mov bp, sp
        notes.append("far frame marked for the stack walker")
    return notes


def _callback(name, apis, far, pop):
    """Which kind of system-called procedure a function is, when its name or what it calls says."""
    for rx, kind in _CALLBACK_NAMES:
        if name and not name.startswith("sub_") and rx.search(name):
            return kind
    for api in apis:
        kind = _CALLBACK_CALLS.get(api.split("!")[-1].upper().rstrip("AW") if "!" in api else "")
        if kind and (pop in (10, 16) or (kind == "hook procedure" and pop in (8, 12))):
            return kind
    return None


def _signature(img, fn):
    """A digest of a function's code that two copies of the same function share."""
    parts = []
    for ins in fn.insns:
        if ins.target is not None and ins.flow:
            where = ("+%d" % (ins.target - fn.start)) if fn.start <= ins.target < fn.limit else img.label(ins.target)
            parts.append("%s %s" % (ins.mn, where))
        else:
            parts.append(ins.text)
    return hashlib.sha1("\n".join(parts).encode()).hexdigest()


def _functions(img):
    graph = call_graph.build(img)
    word = 4 if img.bits == 32 else 2
    uses, cleanup = _result_use(img, graph), _cleanup(img, graph)
    loads = _loaded_before(img, graph) if img.bits == 32 else {}
    facts = {}
    for fn in img.each():
        fact = facts[fn.start] = {"addr": fn.start, "kind": "convention", "name": fn.name}
        if not fn.insns:
            fact.update(shape="not code", convention=None, args=None, arg_source=None)
            continue
        pops, ret_kinds = _returns(fn)
        far = any(ins.mn in ("retf", "lret") for ins in fn.insns) if img.bits == 16 else False
        framed, local, saved = _frame(fn, word)
        first = 2 * word if img.bits == 32 else (6 if far else 4)
        slots = _argument_slots(fn, framed, first, word)
        regs = sorted((_register_inputs(fn) | _pushed_inputs(fn, loads.get(fn.start))) - set(saved) - {"ebp", "esp"})
        calls = graph.calls[fn.start]
        apis = sorted({img.imports[d] for _s, _k, d in calls if d in img.imports})
        real = [i for i in fn.insns if i.mn not in ("nop", "int3")]
        # shape
        if len(real) == 1 and real[0].flow == "jmp" and image.destination(img, real[0]) in img.imports:
            shape = "import thunk"
            fact["thunk_of"] = img.imports[image.destination(img, real[0])]
        elif len(real) == 1 and real[0].flow == "jmp" and real[0].target in img.names:
            shape = "jump thunk"
            fact["thunk_of"] = img.label(real[0].target)
        elif len(real) == 1 and real[0].flow == "ret":
            shape = "empty"
        elif not real:                                   # (only padding: int3 / nop where a function was thought to start)
            shape = "empty"
        elif len(real) <= 3 and real[-1].flow == "ret" and ret_kinds and isinstance(ret_kinds[0][0], tuple) \
                and all(i.flow is None for i in real[:-1]) and len(real) == 2:
            shape = "constant"
            fact["constant"] = ret_kinds[0][0][1]
        else:
            shape = "calls" if any(k in ("call", "import", "indirect") for _s, k, _d in calls) else "leaf"
        fact["shape"] = shape
        tails = [d for _s, k, d in calls if k == "tail"]
        if tails:
            fact["tail_calls"] = sorted({img.label(d) for d in tails})
        # convention and arguments
        pop = max(set(pops), key=pops.count) if pops else None
        this = "ecx" in regs
        if img.bits == 32:
            others = [r for r in regs if r not in ("ecx", "edx")]
            if pop is None:
                conv = None
            elif this and "edx" in regs:
                conv = "fastcall"
            elif this and not others:
                conv = "thiscall"
            elif regs:
                conv = "register"
            else:
                conv = "stdcall" if pop else "cdecl"
        else:
            conv = None if pop is None else "%s %s" % ("pascal" if pop else "cdecl", "far" if far else "near")
        fact["convention"], fact["reg_args"] = conv, regs
        fact["removes"] = pop
        args = source = None
        if pop:
            args, source = pop // word, "return"
        elif pop == 0:
            votes = {n: c for n, c in cleanup.get(fn.start, {}).items() if n > 0}
            if votes:
                args, source = max(votes, key=lambda n: (votes[n], -n)) // word, "callers"
            elif slots:
                args, source = (max(slots) + max(slots[max(slots)], word)) // word, "frame"
            elif cleanup.get(fn.start, {}).get(0, 0) >= 2 or shape in ("empty", "constant"):
                args, source = 0, "callers" if cleanup.get(fn.start) else "shape"
        fact["args"], fact["arg_source"] = args, source
        if slots:
            fact["arg_slots"] = {("+%d" % o): s for o, s in sorted(slots.items())}
        fact["frame"], fact["locals"], fact["saved"] = framed, local, saved
        # result
        used, ignored = uses.get(fn.start, (0, 0))
        consts = [k[0][1] for k in ret_kinds if isinstance(k[0], tuple)]
        sets = [k[0] for k in ret_kinds if k[0] != "unknown"]
        explicit = [k for k in sets if k != "call"]
        if any(k[2] for k in ret_kinds) and not explicit:
            result = "float"
        elif used:
            result = "wide" if img.bits == 16 and any(k[1] for k in ret_kinds) else "value"
        elif ignored >= 2 and not explicit:
            result = "none"
        elif ret_kinds and len(consts) == len(ret_kinds) and set(consts) <= {0, 1}:
            result = "flag"
        elif sets and not ignored:
            result = "wide" if img.bits == 16 and any(k[1] for k in ret_kinds) else "value"
        else:
            result = None
        fact["result"] = result
        fact["result_evidence"] = {"callers_use": used, "callers_ignore": ignored}
        # what the body does
        loops = sum(1 for i in fn.insns if i.flow in ("jcc", "jmp") and i.target is not None
                    and fn.start <= i.target <= i.addr)
        strops = sorted({_STRING_OPS[i.mn[-5:-1]] for i in fn.insns
                         if i.mn.startswith("rep") and i.mn[-5:-1] in _STRING_OPS})
        floats = sum(1 for i in fn.insns if i.mn[0] == "f" and i.mn != "fwait")
        if loops:
            fact["loops"] = loops
        if strops:
            fact["string_ops"] = strops
        if floats:
            fact["float_instructions"] = floats
            fl = sorted({op[4] - first for i in fn.insns if i.mn in ("fld", "fadd", "fsub", "fmul", "fdiv", "fcomp", "fcom")
                         for op in i.ops if framed and op[0] == "m" and op[1] and family(op[1]) == "ebp"
                         and op[4] >= first and op[6] in (4, 8)})
            if fl:
                fact["float_args"] = fl
        takes_slot = any(i.mn == "lea" and i.ops[1][0] == "m" and i.ops[1][1] and family(i.ops[1][1]) == "ebp"
                         and i.ops[1][4] >= first for i in fn.insns if len(i.ops) == 2) if framed else False
        # a variable argument list: it takes the address of an argument slot and hands the list
        # on, or its callers remove different amounts after calling it
        removed = sorted(n for n in cleanup.get(fn.start, {}) if n > 0) if pop == 0 else []
        listed = None                               # the slot where the unnamed arguments start
        if framed and pop == 0:
            direct = {op[4] - first for i in fn.insns if i.mn != "lea" for op in i.ops
                      if op[0] == "m" and op[1] and family(op[1]) == "ebp" and op[2] is None and op[4] >= first}
            taken = {i.ops[1][4] - first for i in fn.insns if i.mn == "lea" and len(i.ops) == 2 and i.ops[1][0] == "m"
                     and i.ops[1][1] and family(i.ops[1][1]) == "ebp" and i.ops[1][2] is None and i.ops[1][4] >= first}
            for at in sorted(taken):
                if at >= word and at - word in direct and not any(d >= at for d in direct):
                    listed = at
        if (takes_slot and (any(_VA_LIST.search(a) for a in apis) or len(removed) >= 2)) or len(removed) >= 3 \
                or (listed is not None and shape == "calls"):
            fact["variadic"] = True
            if listed is not None:
                fact["args"], fact["arg_source"] = listed // word, "frame (named arguments)"
            elif removed:
                fact["args"], fact["arg_source"] = min(min(removed) // word, args or 99), "callers (at least)"
        if img.bits == 16:
            notes = _entry16(img, fn)
            if notes:
                fact["entry"] = notes
        cb = _callback(fn.name, apis, far, pop)
        if cb:
            fact["procedure"] = cb
    return facts


def functions(img):
    """{start: fact} for every function of the image."""
    return img.memo("conventions", _functions)


def duplicates(img, least=6):
    """Groups of functions with the same code (the same source compiled more than once)."""
    groups = {}
    for fn in img.each():
        if len(fn.insns) >= least:
            groups.setdefault(_signature(img, fn), []).append(fn.start)
    return sorted((g for g in groups.values() if len(g) > 1), key=lambda g: g[0])


def thunk_layer(img):
    """The 16/32-bit bridge functions a module imports ([] when it is not such a layer)."""
    names = {n.split("!")[-1].upper() for n in img.imports.values()}
    return sorted(n for n in names if n in _THUNK_APIS)


def analyse(img):
    facts = functions(img)
    dup = duplicates(img)
    bridge = thunk_layer(img)
    rows = list(facts.values())
    n = lambda test: sum(1 for f in rows if test(f))         # noqa: E731
    counts = {"functions": len(rows),
              "convention_known": n(lambda f: f.get("convention")),
              "callee_removes_arguments": n(lambda f: f.get("removes")),
              "caller_removes_arguments": n(lambda f: f.get("removes") == 0),
              "argument_count_known": n(lambda f: f.get("args") is not None),
              "argument_count_from_return": n(lambda f: f.get("arg_source") == "return"),
              "argument_count_from_callers": n(lambda f: f.get("arg_source") == "callers"),
              "argument_count_from_frame": n(lambda f: f.get("arg_source") == "frame"),
              "register_arguments": n(lambda f: f.get("reg_args")),
              "this_in_ecx": n(lambda f: f.get("convention") == "thiscall"),
              "result_known": n(lambda f: f.get("result")),
              "result_none": n(lambda f: f.get("result") == "none"),
              "import_thunks": n(lambda f: f.get("shape") == "import thunk"),
              "jump_thunks": n(lambda f: f.get("shape") == "jump thunk"),
              "tail_calling": n(lambda f: f.get("tail_calls")),
              "empty_or_constant": n(lambda f: f.get("shape") in ("empty", "constant")),
              "leaf": n(lambda f: f.get("shape") == "leaf"),
              "with_loops": n(lambda f: f.get("loops")),
              "inline_string_operations": n(lambda f: f.get("string_ops")),
              "floating_point": n(lambda f: f.get("float_instructions")),
              "variadic": n(lambda f: f.get("variadic")),
              "system_called_procedures": n(lambda f: f.get("procedure")),
              "duplicate_groups": len(dup), "duplicate_functions": sum(len(g) for g in dup),
              "bridge_imports": len(bridge)}
    out = rows + [{"addr": g[0], "kind": "duplicates", "functions": [img.label(f) for f in g]} for g in dup]
    if bridge:
        out.append({"addr": None, "kind": "thunk_layer", "imports": bridge})
    by = {}
    for f in rows:
        by[f.get("convention") or "unknown"] = by.get(f.get("convention") or "unknown", 0) + 1
    text = ["conventions: " + ", ".join("%s %d" % kv for kv in sorted(by.items(), key=lambda kv: -kv[1]))]
    procs = [f for f in rows if f.get("procedure")]
    if procs:
        text.append("system-called procedures: " + ", ".join("%s (%s)" % (f["name"], f["procedure"]) for f in procs[:12]))
    if bridge:
        text.append("16/32-bit bridge imports: " + ", ".join(bridge))
    if dup:
        text.append("identical functions: %d groups, largest %d copies" % (len(dup), max(len(g) for g in dup)))
    return {"counts": counts, "facts": out, "text": text}
