"""driver.py -- run flat 32-bit ring-0 code: a virtual device driver's entry points.

    from codebreaker.emulate import driver
    image = driver.load("vkd.vxd")                    # the driver as the loader leaves it in memory
    r = driver.run(image, image.entries["control"], {"eax": 1}, stack, memory)

The flat-code counterpart of emulator.trace_function: it takes memory regions and registers
where that takes a module file and stack arguments. A call to the system or to another driver
(`int 20h`, service, device) is answered with what the input says and recorded with the
registers and stack words it was given; port reads and writes are recorded; memory is kept by
the emulator's own hooks. What a run did is judged in verify/lanes/driver.py.
"""
import hashlib
import struct

from codebreaker.decompile import describe
from codebreaker.emulate import emulator
from codebreaker.formats import le
from codebreaker.symbols import driver_services

MAX_CALLS = 64                   # service calls compared per run
RESULT_REGISTERS = ("ebx", "ecx", "edx", "esi", "edi", "ebp")     # EAX is the fingerprint's `ret`
STACK_LO, STACK_HI = emulator._STACK_BASE, emulator._STACK_BASE + emulator._STACK_SIZE


INSN_CAP = 10000


MAX_CALLS_RUN = 400              # service calls and port accesses after which a run is cut off (a wait loop)


MAX_COVER = 6000


MAX_OUTSIDE = 64                 # calls to nowhere stood in for before a run is given up


MAX_MEMORY_STEPS = 200000        # memory accesses in one run (one `rep` can make millions)


# Places the inputs point at, inside the emulator's scratch region.
SCRATCH = emulator._SCRATCH_BASE
VM, CLIENT, BUFFER, OBJECT, SAFE = SCRATCH + 0x1000, SCRATCH + 0x2000, SCRATCH + 0x3000, SCRATCH + 0x6000, SCRATCH + 0x7800


RETURN = emulator._RET


REGISTERS = ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp")


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
