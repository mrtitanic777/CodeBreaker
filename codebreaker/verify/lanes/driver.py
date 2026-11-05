"""driver.py -- what is particular to judging a rebuilt virtual device driver.

    python -m codebreaker driver-judge <rebuilt.vxd> <original.vxd>     every entry point, judged
    python -m codebreaker driver-judge --self-check <original.vxd>      the judge judged: the original
                                                                        against itself, then against
                                                                        copies with one instruction broken

One of the kinds reconstruct.py works with (KIND, profile, prepare, build, judge). A driver is
flat 32-bit ring-0 code. It is entered at its control procedure (EAX = a system control
message, EBX = a virtual machine, EBP = the client's registers), at each service it offers
other drivers, and at its two API procedures; arguments are in registers, sometimes on the
stack; the answer is in EAX, the carry and zero flags and whatever registers the procedure
documents. It calls the system and other drivers with `int 20h` + service + device.

Each entry point is run in the emulator, in the original and in the rebuilt driver, from the
same registers, stack and memory, with both images laid out the same way and patched as the
loader would patch them (every object at an address of its own, every internal reference
resolved). What a run did is a fingerprint -- how it ended, EAX, the service calls in order with
the registers and stack words they were given, the port reads and writes, the memory left
behind, the flags and registers handed back, how far the stack moved -- and the two fingerprints
go to the one comparer (verify/compare.py), which gives the verdict.

    from codebreaker.verify.lanes import driver
    for label, (verdict, reason, limits, detail) in driver.judge_files(rebuilt, original).items(): ...

A service call returns what the input says (EAX, carry), so both sides see the same system; the
few services whose whole point is a register (the current virtual machine in EBX) return that.
Addresses inside the driver are compared as they are when the two files have the same objects,
and as "somewhere in the driver" when they do not (then the driver's own memory is compared as
the order of values stored, and the verdict carries the limit `layout-differs`).

The run itself is emulate/driver.py. What runs an entry point, what writes and links the
rebuild and where the entry points are come from the platform (codebreaker/platforms/vxd.py):
its emulator, packager and loader.
"""
import hashlib
import json
import os
import struct

from codebreaker import platforms
from codebreaker.base import corpus, settings
from codebreaker.decompile import describe
from codebreaker.emulate import emulator
from codebreaker.emulate.driver import (BUFFER, CLIENT, INSN_CAP, MAX_CALLS, OBJECT, REGISTERS, RESULT_REGISTERS, SAFE,
                                        SCRATCH, STACK_HI, STACK_LO, VM, load)
from codebreaker.formats import le
from codebreaker.symbols import driver_services
from codebreaker.verify import compare
from codebreaker.verify import lane_common
from codebreaker.verify import verdicts as V

KIND = "le"
P = platforms.of(KIND)            # device drivers: the six slots this lane asks
REV = 4                           # what a verdict means; raise when run() or the inputs change
                                  # (4: inputs found path by path beyond the fixed ones, explore_vectors)



# From a run to a fingerprint --------------------------------------------------------------------
def _value(image, v, aligned):
    """A value as two builds of the same code agree on it (lane_common.argument): an address in
    the stack is "S"; one in the driver is "P", somewhere in the driver, when the two files are
    not laid out alike."""
    return lane_common.argument(v, frame=((STACK_LO, STACK_HI),), image=None if aligned else image.holds)


def _service(device, service):
    if device is None:
        return "outside"
    dev = driver_services.device_name(device) or "%04X" % device
    return "%s.%s%s" % (dev, driver_services.label(device, service & 0x7FFF), "!" if service & 0x8000 else "")


def _effects(image, r, aligned):
    """What the run left in memory outside its stack: (bytes changed, digest). Memory outside
    the driver by address; the driver's own by address too when the two files are laid out
    alike, else as the values stored in the order they were stored. None: too many to record."""
    if r.capped:
        return None
    state, before, own = {}, {}, []
    for w in r.writes:
        a, n, v = w.address, min(w.size, 8), w.value
        if STACK_LO <= a < STACK_HI:
            continue
        inside = image.holds(a)
        if inside and not aligned:
            own.append((n, _value(image, v, aligned) if n == 4 else v & ((1 << (8 * n)) - 1)))
            continue
        for i in range(n):
            before.setdefault(a + i, (w.old >> (8 * i)) & 0xFF)
        if n == 4 and _value(image, v, aligned) in ("S", "P"):
            for i in range(4):
                state[a + i] = _value(image, v, aligned)
        else:
            for i in range(n):
                state[a + i] = (v >> (8 * i)) & 0xFF
    state = {k: x for k, x in state.items() if x != before.get(k)}
    return len(state) + len(own), hashlib.sha1(repr((sorted(state.items()), own)).encode()).hexdigest()[:16]


def fingerprint(image, entry, v, aligned=True):
    """One run as the comparer wants it (see verify/compare.py). `aligned`: the other image
    has the same objects, so addresses inside the driver mean the same on both sides."""
    key = compare.cache_key(KIND, REV, image.digest, entry, aligned, sorted(v["regs"].items()), v.get("stack"),
                            v.get("flags"),
                            [(a, bytes(b)) for a, b in v.get("memory", ())], v.get("stub_ret"), v.get("stub_carry"))
    return lane_common.remembered(key, lambda: _run(image, entry, v, aligned))


def _run(image, entry, v, aligned):
    try:
        r = P.emulator.run(image, entry, v["regs"], v.get("stack", ()), v.get("memory", ()),
                v.get("stub_ret", 0), v.get("stub_carry", 0), flags=v.get("flags", 0))
    except ImportError:
        raise
    except Exception as e:
        return {"err": "%s: %s" % (type(e).__name__, e)}
    deps = list(lane_common.calls([(_service(*c.detail) if c.detail[0] is not None else
                                    "outside:" + _value(image, c.detail[1], aligned), c.args) for c in r.calls],
                                  lambda x: _value(image, x, aligned), cap=MAX_CALLS))
    deps += ["%s(%x,%d,%x)" % (p.kind, p.port, p.size, p.value) for p in r.io[:MAX_CALLS]]
    done = r.ended == "ret"
    memory = _effects(image, r, aligned)
    result = (r.results["flags"],) + tuple(_value(image, r.results[n], aligned) for n in RESULT_REGISTERS) \
        if done else ()
    # the comparer's "what it left behind": memory, and for a driver the flags and registers it
    # hands back, which are results here just as EAX is
    return lane_common.fingerprint(
        r, deps, ret=r.results["eax"], saved=None, cov=tuple(sorted(r.path)), writes=None if memory is None else
        "%d:%s" % (memory[0], hashlib.sha1(repr((memory[1], result)).encode()).hexdigest()[:16]),
        result=result, memory=memory)


def _mode():
    """The lift mode of the platform's processor (emulate/shadow.py mode: its cpu slot)."""
    from codebreaker.emulate import shadow
    return shadow.mode(P.cpu)


# Path by path (verify/concolic.py), as the Windows judges find them: the original run with its
# symbolic shadow (emulate/shadow.py ShadowLE, on the decompiler's x86-32-flat p-code) -- the
# registers it is entered with (EAX-EDI), the carry and zero flags, its stack words, the bytes of
# the scratch memory its pointers lead into, and what a service call answers in EAX and carry are
# the inputs -- and inputs solved for to take each path its conditions allow. The driver's own
# jumps are its paths (a procedure and what it calls inside the driver). Off without the lifter,
# or with CB_NO_CONCOLIC / CB_NO_GUIDED set: then only the fixed inputs, as before.
def concolic_ready():
    from codebreaker.emulate import pcode
    return lane_common.guided_on() and lane_common.concolic_on() and pcode.lifter(_mode()) is not None


def _paths_profile():
    from codebreaker.emulate import pcode
    ready = concolic_ready()
    return [ready, lane_common.PATH_BUDGET, lane_common.MAX_PATHS, list(pcode.stamp(_mode())) if ready else ""]


def _derive(v, values, names):
    """Input `v` with the named inputs (shadow.vars) set to `values`."""
    out = dict(v)
    regs, stack = dict(v["regs"]), list(v.get("stack") or ())
    flags = v.get("flags", 0)
    memory = list(v.get("memory") or ())
    changed = False
    for name, value in values.items():
        kind, where, bits, was = names.get(name, (None, None, 0, None))
        if kind == "arg" and where < len(stack):
            changed |= stack[where] != value & 0xFFFFFFFF
            stack[where] = value & 0xFFFFFFFF
        elif kind == "reg" and where in regs:
            changed |= regs[where] != value & 0xFFFFFFFF
            regs[where] = value & 0xFFFFFFFF
        elif kind == "reg" and where in ("cf", "zf"):
            bit = 1 if where == "cf" else 0x40
            new = (flags | bit) if value else (flags & ~bit)
            changed |= new != flags
            flags = new
        elif kind == "ret":
            changed |= int(v.get("stub_ret", 0)) != value & 0xFFFFFFFF
            out["stub_ret"] = value & 0xFFFFFFFF
        elif kind == "carry":
            changed |= int(v.get("stub_carry", 0)) != (1 if value else 0)
            out["stub_carry"] = 1 if value else 0
        elif kind == "mem":
            changed |= was != value & 0xFF
            memory.append((where, bytes([value & 0xFF])))
    if not changed:
        return None
    out.update(regs=regs, stack=tuple(stack), flags=flags, memory=memory,
               args=[regs[n] for n in REGISTERS[:6]] + stack)
    return out


def explore_vectors(image, entry, seeds):
    """(inputs, exploration): inputs taking paths of the original no seed took, found path by path
    (lane_common.explored), and how far the exploration got. Each seed is run again with its
    shadow (the driver's runs keep no branch log to know their paths by)."""
    from codebreaker.emulate import emulator as tracer
    from codebreaker.emulate import pcode, shadow
    lifter = pcode.lifter(_mode())
    scratch = (SCRATCH, SCRATCH + emulator._SCRATCH_SIZE)

    def run(v):
        sh = shadow.ShadowLE(lifter, [scratch])
        stack = list(v.get("stack") or ())
        esp = tracer._STACK_TOP - 4 * len(stack) - 4
        sh.enter([(esp + 4 + 4 * k, w & 0xFFFFFFFF, k) for k, w in enumerate(stack)] +
                 [(n.upper(), v["regs"][n] & 0xFFFFFFFF, n) for n in REGISTERS[:6] if n in v["regs"]], bits=32)
        sh.enter([("CF", v.get("flags", 0) & 1, "cf"), ("ZF", (v.get("flags", 0) >> 6) & 1, "zf")], bits=8)
        try:
            r = P.emulator.run(image, entry, v["regs"], v.get("stack", ()), v.get("memory", ()),
                               v.get("stub_ret", 0), v.get("stub_carry", 0), flags=v.get("flags", 0), shadow=sh)
        except ImportError:
            raise
        except Exception:
            return None
        lost = set(sh.lost)
        if r.truncated:
            lost.add("truncated")
        return {"trail": sh.trail, "path": sh.path, "vars": sh.vars, "lost": lost, "cut": r.truncated}
    got = lane_common.explored(run, seeds, _derive, own=image.holds, more=len(seeds))
    return list(got.kept), got


def explain(a, b):
    """One line on what differs between two fingerprints the comparer found different."""
    d = compare.difference_detail(a, b)
    if d.startswith("writes"):
        if a.get("memory") != b.get("memory"):
            return "memory left behind differs: %s bytes changed vs %s" % ((a.get("memory") or ("?",))[0],
                                                                         (b.get("memory") or ("?",))[0])
        ra, rb = a.get("result") or (), b.get("result") or ()
        if ra[:1] != rb[:1]:
            return "flags handed back differ: carry/zero %02x vs %02x" % (ra[0], rb[0])
        bad = [RESULT_REGISTERS[i] for i in range(len(RESULT_REGISTERS)) if ra[1 + i:2 + i] != rb[1 + i:2 + i]]
        return "registers handed back differ: %s" % ", ".join(
            "%s %s vs %s" % (n, ra[1 + RESULT_REGISTERS.index(n)], rb[1 + RESULT_REGISTERS.index(n)]) for n in bad)
    return d


# Inputs -----------------------------------------------------------------------------------------
def _memory(fill=None):
    """The objects register arguments point at: a virtual machine block, the client's
    registers, a buffer, and a table of pointers that each lead to a plain `ret`."""
    word = struct.pack("<I", SAFE if fill is None else fill)
    return [(VM, word * 0x100), (CLIENT, bytes(0x100)), (BUFFER, bytes(range(256)) * 2),
            (OBJECT, word * 0x400), (SAFE, b"\xc3" * 16)]


def _vector(eax=0, ebx=VM, ecx=0, edx=0, esi=BUFFER, edi=OBJECT, stack=(), fill=None, ret=0, carry=0, flags=1):
    """One input. The carry flag is SET on entry unless said otherwise: a procedure reports
    success by clearing it, and one that forgot to would pass unnoticed if it came in clear."""
    regs = {"eax": eax, "ebx": ebx, "ecx": ecx, "edx": edx, "esi": esi, "edi": edi, "ebp": CLIENT}
    return {"regs": regs, "stack": tuple(stack), "memory": _memory(fill), "stub_ret": ret, "stub_carry": carry,
            "flags": flags,
            "args": [regs[n] for n in REGISTERS[:6]] + list(stack)}       # what the comparer prints


def inputs(label):
    """(first inputs, further inputs run only when the first all agree) for an entry point.
    A control procedure gets every system control message; a service or API procedure gets
    registers and stack words of each sort -- zeros, small numbers, pointers to plain memory,
    pointers to objects, boundary values -- with service calls answering 0, 1 and failure."""
    if label == "control":
        first = [_vector(eax=m) for m in sorted(driver_services.messages())] or [_vector(eax=m) for m in range(0x33)]
        more = [_vector(eax=m, ret=1) for m in (0, 1, 2, 0x1B, 0x1C, 0x23)] + \
               [_vector(eax=m, carry=1) for m in (0, 1, 2, 0x1B)] + [_vector(eax=0x70000000), _vector(eax=0xFFFFFFFF)] + \
               [_vector(eax=m, flags=0x40) for m in (0, 1, 2, 3, 0x1B, 0x23)]
        return first, more
    objs = [OBJECT + 0x40 * i for i in range(6)]
    plain = [BUFFER + 0x20 * i for i in range(6)]
    first = [
        _vector(0, 0, 0, 0, 0, 0, stack=[0] * 6),
        _vector(1, 2, 3, 4, 5, 6, stack=[7, 8, 9, 10, 11, 12]),
        _vector(*plain, stack=[BUFFER + 0x100 + 0x20 * i for i in range(6)]),
        _vector(*objs, stack=[OBJECT + 0x200 + 0x40 * i for i in range(6)]),
        _vector(0, VM, 0, 0, BUFFER, OBJECT, stack=[VM, 0, 1, BUFFER, 0, 0]),
    ]
    more = [
        _vector(*objs, stack=objs, ret=1),
        _vector(*objs, stack=objs, ret=OBJECT + 0x800),
        _vector(*objs, stack=objs, carry=1),
        _vector(1, 2, 3, 4, 5, 6, stack=[7, 8, 9, 10, 11, 12], ret=1),
        _vector(0xFFFFFFFF, 0xFFFFFFFE, 0x80000000, 0x7FFFFFFF, 0xFFFF, 0x100, stack=[0xFFFFFFFF, 0x8000, 0xFF, 1, 0, 2]),
        _vector(*objs, stack=objs, fill=0xFFFFFFFF, ret=1),
        _vector(*objs, stack=objs, fill=3),
        _vector(6, 5, 4, 3, 2, 1, stack=[6, 5, 4, 3, 2, 1]),
        _vector(objs[0], 2, 3, objs[1], 5, 6, stack=[objs[2], 1, 2, 3, 4, 5]),
        _vector(*objs, stack=objs, flags=0x40),
        _vector(1, 2, 3, 4, 5, 6, stack=[7, 8, 9, 10, 11, 12], flags=0),
    ]
    return first, more


# The kind interface (see verify/reconstruct.py) -------------------------------------------------
def profile(module_path=None):
    """(digest, settings): everything but the source a verdict depends on."""
    from codebreaker.build import driver as builder
    tools = []
    for exe in P.packager.tools().values():
        try:
            st = os.stat(exe)
            tools.append("%s:%d:%d" % (os.path.basename(exe), st.st_size, int(st.st_mtime)))
        except OSError:
            tools.append("")
    first, more = inputs("control")
    sfirst, smore = inputs("service_0")
    text = json.dumps({"kind": KIND, "judge": REV, "comparer": compare.REV, "cap": INSN_CAP,
                       "inputs": [len(first), len(more), len(sfirst), len(smore)],
                       "segments": builder.SEGMENT_CLASSES, "tools": tools,
                       "services": driver_services.size(), "paths": _paths_profile()}, sort_keys=True)
    return hashlib.sha1(text.encode()).hexdigest()[:12], text


def key_of(where):
    return "%d:%x" % where


def _entry_order(label):
    return (label != "control", not label.startswith("service"), len(label), label)


def source_folder(module_path):
    """Where the assembly source of a corpus driver is: <sources>/<name>/, with <sources> from
    CB_DRIVER_SOURCES, else the vmm32 folder of the source tree beside the corpus. '' if none."""
    name = os.path.splitext(os.path.basename(module_path))[0].lower()
    roots = [os.environ.get("CB_DRIVER_SOURCES") or "", os.path.join(os.path.dirname(corpus.design_dir()), "vmm32")]
    for root in roots:
        if root and os.path.isfile(os.path.join(root, name, name + ".asm")):
            return os.path.join(root, name)
    return ""


class Prepared(object):
    """What reconstruct.run needs of a module before building: here the units are the
    driver's entry points and the text is its one assembly file."""


def prepare(module_path, work):
    """The driver's source: the assembly file in the source tree when CB_DRIVER_SOURCES names
    one to judge, otherwise written from the image (source/driver_writer.py) and built."""
    m = Prepared()
    m.source = source_folder(module_path) if os.environ.get("CB_DRIVER_SOURCES") else ""
    m.written = None
    text = ""
    if m.source:
        with open(os.path.join(m.source, os.path.basename(m.source) + ".asm"), encoding="latin1") as f:
            text = f.read()
    else:
        m.written = P.packager.writer(module_path, os.path.join(work, "source"))
        if m.written.get("linked"):
            with open(m.written["source"], encoding="latin1") as f:
                text = f.read()
    points, taken = P.loader.entries(module_path), set()
    m.header = text
    m.order = []
    for n in sorted(points, key=_entry_order):               # two labels, one procedure: judged once
        if points[n] not in taken:
            taken.add(points[n])
            m.order.append(n)
    # the unit judged is an entry point; its text is the whole assembly file (in the header)
    m.texts = {n: "; entry point %s of %s" % (n, os.path.basename(m.source or m.written["source"]))
               if text else "" for n in m.order}
    m.key = {n: key_of(points[n]) for n in m.order} if text else {}
    m.all_keys = [key_of(points[n]) for n in m.order]
    m.sig = hashlib.sha1(("%d\0%s" % (REV, text)).encode("latin1", "replace")).hexdigest()
    return m


def build(m, module_path, work):
    """-> (context for judge, {label: item}, {label: why it is not in the build})"""
    if m.written is not None:
        if not m.written.get("linked"):
            why = (m.written.get("notes") or ["did not build"])[-1][:200]
            return (module_path, None), {}, {n: why for n in m.order}
        return (module_path, m.written["image"]), {n: (m.key[n], n) for n in m.order}, {}
    r = P.packager.functions(m.source, original=module_path, out=os.path.join(work, "build"))
    if not r["linked"]:
        why = (r["log"].splitlines() or ["did not build"])[-1][:200]
        return (module_path, None), {}, {n: why for n in m.order}
    return (module_path, r["path"]), {n: (m.key[n], n) for n in m.order}, {}


_IMAGES = {}


def _image(path):
    if path not in _IMAGES:
        _IMAGES[path] = load(path)
    return _IMAGES[path]


def judge(context, item):
    """Worker: judge one entry point. context = (original file, rebuilt file); item = (key,
    label). -> (verdict, reason, limits, detail)"""
    original, rebuilt = context
    _key, label = item
    try:
        a, b = _image(original), _image(rebuilt)
    except Exception as e:
        return V.INCONCLUSIVE, "judge: %s: %s" % (type(e).__name__, e), [], ""
    if label not in a.entries:
        return V.INCONCLUSIVE, "the original has no such entry point", [], ""
    if label not in b.entries:
        return V.DIVERGENT, "entry-missing", [], "the rebuilt driver has no %s" % label
    aligned = a.shape == b.shape
    first, more = inputs(label)
    info, seen = {}, {}

    def side(image, entry, tag):
        def fp(v):
            f = fingerprint(image, entry, v, aligned)
            seen[tag] = f
            if tag == "a" and "cov" in f:
                info.setdefault("ran", set()).update(f["cov"])
            return f
        return fp

    found = {}

    def aimed(_covered, _reached):
        if not concolic_ready():
            return []
        kept, found["got"] = explore_vectors(a, a.entries[label], list(first) + list(more))
        return kept
    try:
        verdict, reason, limits = compare.compare(side(a, a.entries[label], "a"), side(b, b.entries[label], "b"),
                                                  first, more, info=info, directed=aimed)
    except ImportError:
        raise
    except Exception as e:                                  # one entry point must not sink the rest
        return V.INCONCLUSIVE, "judge: %s: %s" % (type(e).__name__, e), [], ""
    limits = list(limits)
    if verdict != "divergent":
        limits += lane_common.path_limits(found.get("got"))
    if not aligned:
        limits.append("layout-differs")
    detail = info.get("detail", "")
    if verdict == V.DIVERGENT and reason == "writes-differ" and "a" in seen and "b" in seen:
        detail = explain(seen["a"], seen["b"]) + detail[detail.find("  [input"):] if "  [input" in detail else \
            explain(seen["a"], seen["b"])
    if verdict != "divergent":
        detail = "%d instructions of the original ran" % len(info.get("ran", ()))
    return verdict, reason, limits, detail


def judge_files(rebuilt, original, labels=None):
    """Judge every entry point of `original` (or those in `labels`) in `rebuilt`.
    -> {label: (verdict, reason, limits, detail)} in entry order."""
    from codebreaker.jobs import pool
    a = _image(original)
    order = sorted(a.entries, key=_entry_order)
    order = [n for n in order if labels is None or n in labels]
    got = pool.run(judge, [(n, n) for n in order], context=(original, rebuilt),
                   crashed=lambda item: (V.INCONCLUSIVE, "judge: the emulator crashed on this entry point", [], ""))
    return {n: got[n] for n in order}


# The judge judged -------------------------------------------------------------------------------
def break_instruction(original, out, label, nth=0):
    """Write a copy of `original` with one instruction of the procedure at `label` broken:
    the nth instruction (in address order) that runs on the first input, with one bit of its
    last byte flipped (a register, a displacement or an immediate becomes another).
    -> (file offset changed, the instruction's text) or None when there is no such one."""
    lf = le.members(original)[0][1]
    image = _image(original)
    if label not in image.entries or le.is_member(lf):
        return None
    cover = set()
    for v in inputs(label)[0]:
        cover |= set(P.emulator.run(image, image.entries[label], v["regs"], v["stack"], v["memory"],
                                    flags=v["flags"]).path)
    md = P.cpu.disassembler()
    patched = set()
    for obj, off, _kind, _to, _ta in le.references(lf):
        patched.update(describe.le_address(obj, off) + i for i in range(4))
    picked = []
    for address in sorted(cover):
        lo, _hi, obj = next(s for s in image.spans if s[0] <= address < s[1])
        data = next(b for a, b in image.regions if a == lo)
        ins = next(md.disasm(data[address - lo:address - lo + 15], address), None)
        if ins is None or ins.mnemonic in ("nop", "int3") or (ins.address + ins.size - 1) in patched:
            continue
        picked.append((obj, address - lo, ins))
    if nth >= len(picked):
        return None
    obj, off, ins = picked[nth]
    at = le.file_offset(lf, obj, off + ins.size - 1)
    if at is None:
        return None
    data = bytearray(lf.d)
    data[at] ^= 0x01 if ins.size > 1 else 0x08
    with open(out, "wb") as f:
        f.write(data)
    return at, ("%s %s" % (ins.mnemonic, ins.op_str)).strip()


def self_check(original, work=None, per_entry=3, entries=12):
    """Judge the judge on one driver. -> {"self": {verdict: count}, "broken": tried,
    "caught": reported divergent, "missed": [(label, instruction)]}. The original against
    itself must come out with nothing divergent; then, for up to `entries` entry points that
    came out equivalent, `per_entry` copies each with one executed instruction broken are
    judged -- a judge with teeth reports them divergent (verify/teeth.plant, the harness)."""
    from codebreaker.verify import teeth
    work = work or settings.state("driver_judge", os.path.splitext(os.path.basename(original))[0].lower())
    os.makedirs(work, exist_ok=True)
    own = judge_files(original, original)
    out = {"self": {}, "broken": 0, "caught": 0, "missed": []}
    for verdict, _reason, _limits, _detail in own.values():
        out["self"][verdict] = out["self"].get(verdict, 0) + 1
    good = [n for n, v in own.items() if v[0] == V.EQUIVALENT][:entries]
    ext = os.path.splitext(original)[1]

    def broken():                                            # (a round each: one entry point, one break)
        for label in good:
            for nth in range(per_entry):
                copy = os.path.join(work, "broken_%s_%d%s" % (label, nth, ext))
                changed = break_instruction(original, copy, label, nth)
                if not changed:
                    break
                _IMAGES.pop(copy, None)
                yield [(label, changed[1], copy)]

    def judged(batch):
        (label, _text, copy), = batch
        return {label: judge((original, copy), (label, label))[0]}

    for (label, text, _copy, verdict, got), in teeth.plant(broken(), judged,
                                                           lambda v: "caught" if v == V.DIVERGENT else "missed"):
        out["broken"] += 1
        if got == "caught":
            out["caught"] += 1
        else:
            out["missed"].append((label, text, verdict))
    return out


def main(argv):
    """python -m codebreaker driver-judge <rebuilt.vxd> <original.vxd> | --self-check <original.vxd>"""
    if len(argv) == 2 and argv[0] == "--self-check":
        path = corpus.retail(argv[1])
        if not path:
            print("%s: not found" % argv[1])
            return 2
        r = self_check(path)
        print("%s against itself: %s" % (os.path.basename(path), ", ".join("%s %d" % kv for kv in sorted(r["self"].items()))))
        print("one instruction broken: %d copies, %d reported divergent" % (r["broken"], r["caught"]))
        for label, text, verdict in r["missed"]:
            print("    not caught: %s with `%s` changed -> %s" % (label, text, verdict))
        return 0 if not r["self"].get(V.DIVERGENT) and not r["self"].get(V.INCONCLUSIVE) else 1
    if len(argv) != 2:
        print(main.__doc__)
        return 2
    rebuilt, original = argv[0], corpus.retail(argv[1])
    if not os.path.isfile(rebuilt) or not original:
        print("%s: not found" % (argv[1] if os.path.isfile(rebuilt) else rebuilt))
        return 2
    counts = {}
    for label, (verdict, reason, limits, detail) in judge_files(rebuilt, original).items():
        counts[verdict] = counts.get(verdict, 0) + 1
        print("%-12s %-12s %s%s%s" % (label, verdict, reason, ("  [%s]" % ",".join(limits)) if limits else "",
                                      ("  " + detail) if detail else ""))
    print("%s vs %s: %s" % (os.path.basename(rebuilt), os.path.basename(original),
                             ", ".join("%s %d" % kv for kv in sorted(counts.items())) or "no entry points"))
    return 1 if counts.get(V.DIVERGENT) else 0
