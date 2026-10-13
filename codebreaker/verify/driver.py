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

    from codebreaker.verify import driver
    for label, (verdict, reason, limits, detail) in driver.judge_files(rebuilt, original).items(): ...

A service call returns what the input says (EAX, carry), so both sides see the same system; the
few services whose whole point is a register (the current virtual machine in EBX) return that.
Addresses inside the driver are compared as they are when the two files have the same objects,
and as "somewhere in the driver" when they do not (then the driver's own memory is compared as
the order of values stored, and the verdict carries the limit `layout-differs`).

The run itself is `run()` below, on unicorn with the emulator's memory hooks. It belongs in
emulate/emulator.py (as the flat-code counterpart of trace_function that takes memory regions
and registers instead of a PE path and stack arguments) and should move there.
"""
import hashlib
import json
import os
import struct

from codebreaker.base import corpus, settings
from codebreaker.decompile import describe
from codebreaker.emulate import emulator
from codebreaker.formats import le
from codebreaker.symbols import driver_services
from codebreaker.verify import compare

KIND = "le"
REV = 3                           # what a verdict means; raise when run() or the inputs change
INSN_CAP = 10000
MAX_CALLS = 64                   # service calls compared per run
MAX_CALLS_RUN = 400              # service calls and port accesses after which a run is cut off (a wait loop)
MAX_COVER = 6000
MAX_OUTSIDE = 64                 # calls to nowhere stood in for before a run is given up
MAX_MEMORY_STEPS = 200000        # memory accesses in one run (one `rep` can make millions)

# Places the inputs point at, inside the emulator's scratch region.
SCRATCH = emulator._SCRATCH_BASE
VM, CLIENT, BUFFER, OBJECT, SAFE = SCRATCH + 0x1000, SCRATCH + 0x2000, SCRATCH + 0x3000, SCRATCH + 0x6000, SCRATCH + 0x7800
STACK_LO, STACK_HI = emulator._STACK_BASE, emulator._STACK_BASE + emulator._STACK_SIZE
RETURN = emulator._RET
REGISTERS = ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp")
RESULT_REGISTERS = ("ebx", "ecx", "edx", "esi", "edi", "ebp")     # EAX is the fingerprint's `ret`
# Services that answer in a register other than EAX: without the answer the code after them
# has nothing to work on. (What the kit documents; everything else returns EAX and carry.)
ANSWERS_IN = {"Get_Cur_VM_Handle": "ebx", "Get_Sys_VM_Handle": "ebx", "Get_Next_VM_Handle": "ebx",
              "Get_Execution_Focus": "ebx", "Get_Cur_Thread_Handle": "edi", "Get_Sys_Thread_Handle": "edi"}


class Image(object):
    """A driver as the loader leaves it in memory: regions [(address, bytes)], spans
    [(address, end, object)], entries {label: address}, shape (sizes of its objects)."""

    def __init__(self, lf):
        bufs = {o["idx"]: bytearray(le.object_data(lf, o)) for o in lf.objects}
        for obj, off, kind, to, ta in le.references(lf):
            buf = bufs.get(obj)
            if buf is None or off < 0 or off + 4 > len(buf) or kind not in (7, 8):
                continue
            target = describe.le_address(to, ta)
            if kind == 8:                                    # the distance to it, as in a call
                target -= describe.le_address(obj, off) + 4
            struct.pack_into("<I", buf, off, target & 0xFFFFFFFF)
        self.regions = [(describe.le_address(i, 0), bytes(b)) for i, b in sorted(bufs.items()) if b]
        self.spans = [(a, a + len(b), i) for (a, b), i in zip(self.regions, sorted(i for i, b in bufs.items() if b))]
        self.entries = {label: describe.le_address(*where) for label, where in le.entry_points(lf).items()}
        self.shape = tuple(o["vsize"] for o in lf.objects)
        self.digest = hashlib.sha1(b"".join(b for _a, b in self.regions)).hexdigest()

    def holds(self, address):
        return any(lo <= address < hi for lo, hi, _i in self.spans)


def load(path, member=None):
    """The Image of a driver file (of one member of a container: by name, else the first)."""
    found = le.members(path)
    lf = next((x for n, x in found if member and n.lower() == member.lower()), found[0][1])
    return Image(lf)


# One run ---------------------------------------------------------------------------------------
def run(image, entry, registers=None, stack=(), memory=(), service_eax=0, service_carry=0, cap=INSN_CAP,
        flags=0):
    """Run flat 32-bit code at `entry` of `image` until it returns, and record what it did:
    {"exit", "eax", "flags", "registers", "esp_delta", "calls" [(device, service, registers,
    stack words)], "ports" [...], "mem_writes" [...], "cover" (addresses run inside the image),
    "truncated"}. `registers` {name: value}; `stack` words above the return address; `memory`
    [(address, bytes)] set before the run; `flags` the carry (1) and zero (0x40) flags on
    entry. Every `int 20h` service call is recorded and
    returns EAX = service_eax, carry = service_carry; a call that leaves the image for
    nowhere is recorded and returned from the same way."""
    import unicorn as U
    from unicorn import x86_const as X
    reg = {n: getattr(X, "UC_X86_REG_" + n.upper()) for n in REGISTERS + ("esp", "eip", "eflags")}
    page = 0x1000
    uc = U.Uc(U.UC_ARCH_X86, U.UC_MODE_32)
    mapped = set()

    def provide(address, size):
        want = [b for b in range(address & ~(page - 1), address + size, page) if b not in mapped]
        while want:                                          # each run of new pages in one piece
            n = 1
            while n < len(want) and want[n] == want[0] + n * page:
                n += 1
            uc.mem_map(want[0], n * page)
            mapped.update(want[:n])
            del want[:n]

    for address, data in image.regions:
        provide(address, len(data))
        uc.mem_write(address, data)
    provide(SCRATCH, emulator._SCRATCH_SIZE)
    provide(STACK_LO, STACK_HI - STACK_LO)
    for address, data in memory:
        provide(address, len(data))
        uc.mem_write(address, bytes(data))
    esp = emulator._STACK_TOP
    for word in reversed(list(stack)):
        esp -= 4
        uc.mem_write(esp, struct.pack("<I", word & 0xFFFFFFFF))
    esp -= 4
    uc.mem_write(esp, struct.pack("<I", RETURN))
    uc.reg_write(reg["esp"], esp)
    for name, value in (registers or {}).items():
        uc.reg_write(reg[name], value & 0xFFFFFFFF)
    uc.reg_write(reg["eflags"], (uc.reg_read(reg["eflags"]) & ~0x41) | (flags & 0x41))
    rec = {"exit": None, "eax": None, "calls": [], "ports": [], "mem_reads": [], "mem_writes": [],
           "truncated": False, "insns": 0, "cover": set(), "outside": 0}

    def given():
        sp = uc.reg_read(reg["esp"])
        try:
            words = struct.unpack("<4I", uc.mem_read(sp, 16))
        except U.UcError:
            words = ()
        return tuple(uc.reg_read(reg[n]) for n in REGISTERS[:6]), words

    def answer(jump_back):
        """Return from a call that was not run: EAX and carry as the input says."""
        uc.reg_write(reg["eax"], service_eax & 0xFFFFFFFF)
        uc.reg_write(reg["eflags"], (uc.reg_read(reg["eflags"]) & ~1) | (service_carry & 1))
        if jump_back:
            sp = uc.reg_read(reg["esp"])
            uc.reg_write(reg["eip"], struct.unpack("<I", uc.mem_read(sp, 4))[0])
            uc.reg_write(reg["esp"], sp + 4)

    def on_code(_uc, address, _size, _user):
        if rec["insns"] >= cap:
            rec["truncated"] = True
            uc.emu_stop()
            return
        rec["insns"] += 1
        if image.holds(address):
            if len(rec["cover"]) < MAX_COVER:
                rec["cover"].add(address)
        elif not SCRATCH <= address < SCRATCH + emulator._SCRATCH_SIZE:
            # Through a pointer nothing here fills in (a callback, a hooked vector): a call out.
            rec["outside"] += 1
            if rec["outside"] > MAX_OUTSIDE:
                rec["exit"] = "wild"
                uc.emu_stop()
                return
            rec["calls"].append((None, address) + given())
            try:
                answer(True)
            except U.UcError:
                rec["exit"] = "wild"
                uc.emu_stop()

    def on_interrupt(_uc, number, _user):
        eip = uc.reg_read(reg["eip"])                        # just past the two-byte `int`
        if number == 0x20:
            try:
                service, device = struct.unpack("<HH", uc.mem_read(eip, 4))
            except U.UcError:
                rec["exit"] = "exception: service number unreadable"
                uc.emu_stop()
                return
            if len(rec["calls"]) >= MAX_CALLS_RUN:
                rec["truncated"] = True
                uc.emu_stop()
                return
            rec["calls"].append((device, service) + (given() if len(rec["calls"]) <= MAX_CALLS else ((), ())))
            where = ANSWERS_IN.get(driver_services.service_name(device, service) or "")
            if service & 0x8000:                             # jump: the service returns to our caller
                answer(True)
            else:
                answer(False)
                uc.reg_write(reg["eip"], eip + 4)
            if where:
                uc.reg_write(reg[where], VM)
        elif number != 0x41:                                 # 41h talks to a debugger: nothing happens
            rec["exit"] = "int:%02x" % number
            uc.emu_stop()

    def on_in(_uc, port, size, _user):
        if len(rec["ports"]) >= MAX_CALLS_RUN:
            rec["truncated"] = True
            uc.emu_stop()
        else:
            rec["ports"].append(("in", port, size, 0))
        return 0

    def on_out(_uc, port, size, value, _user):
        if len(rec["ports"]) >= MAX_CALLS_RUN:
            rec["truncated"] = True
            uc.emu_stop()
        else:
            rec["ports"].append(("out", port, size, value & ((1 << (8 * size)) - 1)))

    record, on_unmapped = emulator._memory_hooks(U, rec, set())

    def on_memory(_uc, access, address, size, value, user):
        if rec.get("memops", 0) >= MAX_MEMORY_STEPS:
            rec["truncated"] = True
            uc.emu_stop()
        else:
            record(_uc, access, address, size, value, user)

    uc.hook_add(U.UC_HOOK_CODE, on_code)
    uc.hook_add(U.UC_HOOK_MEM_READ | U.UC_HOOK_MEM_WRITE, on_memory)
    uc.hook_add(U.UC_HOOK_MEM_READ_UNMAPPED | U.UC_HOOK_MEM_WRITE_UNMAPPED | U.UC_HOOK_MEM_FETCH_UNMAPPED,
                on_unmapped)
    uc.hook_add(U.UC_HOOK_INTR, on_interrupt)
    uc.hook_add(U.UC_HOOK_INSN, on_in, None, 1, 0, X.UC_X86_INS_IN)
    uc.hook_add(U.UC_HOOK_INSN, on_out, None, 1, 0, X.UC_X86_INS_OUT)
    try:
        # No clock: a run ends by returning, faulting, or at a count (instructions, memory
        # accesses, pages) -- so the same run always ends the same way and can be remembered.
        uc.emu_start(entry, RETURN, count=cap + 1)
        at = uc.reg_read(reg["eip"])
        if rec["exit"] is None:
            rec["exit"] = "truncated" if rec["truncated"] else "ret" if at == RETURN else \
                "exception: stopped at 0x%x" % at
    except U.UcError as e:
        if rec["exit"] is None:
            rec["exit"] = "exception: %s" % e
    rec["eax"] = uc.reg_read(reg["eax"])
    rec["flags"] = uc.reg_read(reg["eflags"]) & 0x41         # carry and zero
    rec["registers"] = tuple(uc.reg_read(reg[n]) for n in RESULT_REGISTERS)
    rec["esp_delta"] = (uc.reg_read(reg["esp"]) - esp) & 0xFFFFFFFF
    return rec


# From a run to a fingerprint --------------------------------------------------------------------
def _value(image, v, aligned):
    """A value as two builds of the same code agree on it."""
    if STACK_LO <= v < STACK_HI:
        return "S"
    if not aligned and image.holds(v):
        return "P"                                           # somewhere in the driver
    return "%x" % v


def _service(device, service):
    if device is None:
        return "outside"
    dev = driver_services.device_name(device) or "%04X" % device
    return "%s.%s%s" % (dev, driver_services.label(device, service & 0x7FFF), "!" if service & 0x8000 else "")


def _effects(image, r, aligned):
    """What the run left in memory outside its stack: (bytes changed, digest). Memory outside
    the driver by address; the driver's own by address too when the two files are laid out
    alike, else as the values stored in the order they were stored. None: too many to record."""
    ws = r["mem_writes"]
    if len(ws) >= emulator._MAX_MEM:
        return None
    state, before, own = {}, {}, []
    for w in ws:
        a, n, v = w["addr"], min(w["size"], 8), w["value"] or 0
        if STACK_LO <= a < STACK_HI:
            continue
        inside = image.holds(a)
        if inside and not aligned:
            own.append((n, _value(image, v, aligned) if n == 4 else v & ((1 << (8 * n)) - 1)))
            continue
        for i in range(n):
            before.setdefault(a + i, ((w.get("old") or 0) >> (8 * i)) & 0xFF)
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
    key = None
    if not os.environ.get("CB_NO_FP_CACHE"):                 # the same switch the other kinds obey
        key = compare.cache_key(KIND, REV, image.digest, entry, aligned, sorted(v["regs"].items()), v.get("stack"),
                                v.get("flags"),
                                [(a, bytes(b)) for a, b in v.get("memory", ())], v.get("stub_ret"), v.get("stub_carry"))
        hit = compare.cache_get(key)
        if hit is not None:
            return hit
    try:
        r = run(image, entry, v["regs"], v.get("stack", ()), v.get("memory", ()),
                v.get("stub_ret", 0), v.get("stub_carry", 0), flags=v.get("flags", 0))
    except ImportError:
        raise
    except Exception as e:
        return {"err": "%s: %s" % (type(e).__name__, e)}
    deps = []
    for device, service, regs, words in r["calls"][:MAX_CALLS]:
        deps.append("%s(%s)" % (_service(device, service) if device is not None else
                                "outside:" + _value(image, service, aligned),
                                ",".join(_value(image, x, aligned) for x in regs + tuple(words))))
    if len(r["calls"]) > MAX_CALLS:
        deps.append("...")
    deps += ["%s(%x,%d,%x)" % p for p in r["ports"][:MAX_CALLS]]
    done = compare._exit_norm(r["exit"]) == "ret"
    memory = _effects(image, r, aligned)
    result = (r["flags"],) + tuple(_value(image, x, aligned) for x in r["registers"]) if done else ()
    fp = {"exit": compare._exit_norm(r["exit"]), "ret": r["eax"], "deps": tuple(deps),
          "truncated": bool(r["truncated"]),
          # the comparer's "what it left behind": memory, and for a driver the flags and
          # registers it hands back, which are results here just as EAX is
          "writes": None if memory is None else "%d:%s" % (memory[0], hashlib.sha1(
              repr((memory[1], result)).encode()).hexdigest()[:16]),
          "memory": memory, "result": result,
          "pop": r["esp_delta"] if done else None, "saved": None,
          "cov": tuple(sorted(r["cover"])), "reach": ()}
    if key:
        compare.cache_put(key, fp)
    return fp


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
    for name in ("wasm", "wlink"):
        exe = builder.tool(name)
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
                       "services": driver_services.size()}, sort_keys=True)
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
    from codebreaker.source import driver_writer
    lf = le.members(module_path)[0][1]
    m = Prepared()
    m.source = source_folder(module_path) if os.environ.get("CB_DRIVER_SOURCES") else ""
    m.written = None
    text = ""
    if m.source:
        with open(os.path.join(m.source, os.path.basename(m.source) + ".asm"), encoding="latin1") as f:
            text = f.read()
    else:
        m.written = driver_writer.reconstruct(module_path, os.path.join(work, "source"))
        if m.written.get("linked"):
            with open(m.written["source"], encoding="latin1") as f:
                text = f.read()
    points, taken = le.entry_points(lf), set()
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
    from codebreaker.build import driver as builder
    if m.written is not None:
        if not m.written.get("linked"):
            why = (m.written.get("notes") or ["did not build"])[-1][:200]
            return (module_path, None), {}, {n: why for n in m.order}
        return (module_path, m.written["image"]), {n: (m.key[n], n) for n in m.order}, {}
    r = builder.build(m.source, original=module_path, out=os.path.join(work, "build"))
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
        return "inconclusive", "judge: %s: %s" % (type(e).__name__, e), [], ""
    if label not in a.entries:
        return "inconclusive", "the original has no such entry point", [], ""
    if label not in b.entries:
        return "divergent", "entry-missing", [], "the rebuilt driver has no %s" % label
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

    try:
        verdict, reason, limits = compare.compare(side(a, a.entries[label], "a"), side(b, b.entries[label], "b"),
                                                  first, more, info=info)
    except ImportError:
        raise
    except Exception as e:                                  # one entry point must not sink the rest
        return "inconclusive", "judge: %s: %s" % (type(e).__name__, e), [], ""
    limits = list(limits)
    if not aligned:
        limits.append("layout-differs")
    detail = info.get("detail", "")
    if verdict == "divergent" and reason == "writes-differ" and "a" in seen and "b" in seen:
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
                   crashed=lambda item: ("inconclusive", "judge: the emulator crashed on this entry point", [], ""))
    return {n: got[n] for n in order}


# The judge judged -------------------------------------------------------------------------------
def break_instruction(original, out, label, nth=0):
    """Write a copy of `original` with one instruction of the procedure at `label` broken:
    the nth instruction (in address order) that runs on the first input, with one bit of its
    last byte flipped (a register, a displacement or an immediate becomes another).
    -> (file offset changed, the instruction's text) or None when there is no such one."""
    import capstone
    lf = le.members(original)[0][1]
    image = _image(original)
    if label not in image.entries or le.is_member(lf):
        return None
    cover = set()
    for v in inputs(label)[0]:
        cover |= run(image, image.entries[label], v["regs"], v["stack"], v["memory"], flags=v["flags"])["cover"]
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
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
    judged -- a judge with teeth reports them divergent."""
    work = work or settings.state("driver_judge", os.path.splitext(os.path.basename(original))[0].lower())
    os.makedirs(work, exist_ok=True)
    own = judge_files(original, original)
    out = {"self": {}, "broken": 0, "caught": 0, "missed": []}
    for verdict, _reason, _limits, _detail in own.values():
        out["self"][verdict] = out["self"].get(verdict, 0) + 1
    good = [n for n, v in own.items() if v[0] == "equivalent"][:entries]
    ext = os.path.splitext(original)[1]
    for label in good:
        for nth in range(per_entry):
            copy = os.path.join(work, "broken_%s_%d%s" % (label, nth, ext))
            changed = break_instruction(original, copy, label, nth)
            if not changed:
                break
            _IMAGES.pop(copy, None)
            verdict = judge((original, copy), (label, label))[0]
            out["broken"] += 1
            if verdict == "divergent":
                out["caught"] += 1
            else:
                out["missed"].append((label, changed[1], verdict))
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
        return 0 if not r["self"].get("divergent") and not r["self"].get("inconclusive") else 1
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
    return 1 if counts.get("divergent") else 0
