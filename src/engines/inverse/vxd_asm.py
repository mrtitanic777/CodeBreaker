#!/usr/bin/env python3
r"""vxd_asm.py -- recursive-descent 32-bit disassembler + MASM emitter for Win95 VxDs.

Takes an LEFile (engine.format_loaders.le) and reconstructs each Virtual Device Driver as
annotated 32-bit assembly. A VxD's executable object is a MIX of code and data (the DDB,
jump tables, the service table, string/pointer tables all live inside it), so a linear sweep
disassembles the data as garbage. This uses RECURSIVE DESCENT instead: it seeds from the real
entry points recorded in the DDB (control procedure, API procedures, service table) and follows
control flow, discovering jump-table dispatchers as it goes. Everything the descent never
reaches as code is emitted as data.

Three VxD-specific mechanisms are decoded:

  1. The DDB (Device Descriptor Block) -- the driver's identity -- becomes a Declare_Virtual_
     Device with its fields (name, version, device id, control/API procedures, service table).
  2. INT 20h service calls -- `int 20h` + an inline DWORD (svc lo word, device id hi word) --
     become `VMMCall`/`VxDCall <DEVICE>.Service_<n>`; the DWORD is consumed, not disassembled.
  3. Fixups (engine.format_loaders.le.fixups) resolve every zeroed operand to its real target;
     the control-dispatch `jmp [eax*4 + table]` and its table are recovered as a labelled jump.

    from src.formats import le
    from src.engines.inverse import vxd_asm
    print(vxd_asm.emit_asm(le.LEFile.from_file("vkd.vxd")))
"""
import os
import sys
import struct

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from src.engines.inverse import vxd_services
from src.core import paths

_MD = Cs(CS_ARCH_X86, CS_MODE_32)
_MD.detail = True
_SVC = vxd_services.load()          # DDK service-name tables (ordinal -> name)

# Canonical Win95 VxD device ids -> names (DDK VxD service tables). The high word of an INT 20h
# service DWORD is the device id; this names the callee module for `VxDCall`.
VXD_DEVICES = {
    0x0001: "VMM", 0x0002: "DEBUG", 0x0003: "VPICD", 0x0004: "VDMAD", 0x0005: "VTD",
    0x0006: "V86MMGR", 0x0007: "PAGESWAP", 0x0008: "PARITY", 0x0009: "REBOOT", 0x000A: "VDD",
    0x000B: "VSD", 0x000C: "VMD", 0x000D: "VKD", 0x000E: "VCD", 0x000F: "VPD",
    0x0010: "BLOCKDEV", 0x0011: "VMCPD", 0x0012: "EBIOS", 0x0013: "BIOSXLAT", 0x0014: "VNETBIOS",
    0x0015: "DOSMGR", 0x0016: "VMPOLL", 0x0017: "SHELL", 0x0018: "VMM_STRING", 0x0019: "VPROD",
    0x001A: "DOSNET", 0x001B: "VFD", 0x001C: "VDD2", 0x001D: "WINLOAD", 0x001E: "VXDLDR",
    0x0020: "INT13", 0x0021: "PAGEFILE", 0x0022: "SCSI", 0x0023: "MCA_POS", 0x0024: "SCSIFD",
    0x0026: "VPEND", 0x0027: "APM", 0x0028: "VPOWERD", 0x0029: "VXDLDR", 0x002A: "NDIS",
    0x002B: "BIOS", 0x002C: "VWIN32", 0x002D: "VCOMM", 0x002E: "SPOOLER", 0x0032: "VCOMM",
    0x0033: "CONFIGMG", 0x0036: "VFBACKUP", 0x0037: "ENABLE", 0x0038: "VCOND", 0x003D: "BIOS",
    0x0040: "IFSMgr", 0x0041: "VCDFSD", 0x0442: "VTDAPI", 0x0448: "PERF", 0x0449: "VWIN32",
    0x044A: "VFAT", 0x0450: "MRCI2", 0x0454: "NWLINK", 0x0480: "NDIS2SUP",
}

# DDB field offsets, relative to the DDB start (the DDB_Next dword). ddb()'s rva points here.
DDB_CONTROL_PROC = 0x18
DDB_V86_API_PROC = 0x1C
DDB_PM_API_PROC = 0x20
DDB_SERVICE_TABLE = 0x30
DDB_SERVICE_SIZE = 0x34

_TERMS = {"ret", "retn", "retf", "iret", "iretd", "iretw"}


def _string_ranges(code):
    """[start, end) byte ranges in `code` that are ASCII string DATA the disassembler would otherwise
    decode as instructions. A run of >=8 printable bytes qualifies only if it is NUL-terminated or long
    (>=20), is alpha-majority (>=60%), and contains a vowel -- the three gates that reject genuine code
    whose bytes happen to be printable (e.g. an `[esp+disp]` compare chain decoding as `D$ ;D$$u`).
    ~100% precision on the Win95 corpus, ~86% recall (short vowel-less tags like `CD-XA001` are missed)."""
    P = lambda b: 0x20 <= b <= 0x7E
    A = lambda b: 0x41 <= b <= 0x5A or 0x61 <= b <= 0x7A
    V = frozenset(b"AEIOUaeiou")
    out = []
    n = len(code)
    i = 0
    while i < n:
        if not P(code[i]):
            i += 1
            continue
        j = i
        while j < n and P(code[j]):
            j += 1
        seg = code[i:j]
        L = j - i
        if (L >= 8 and ((j < n and code[j] == 0) or L >= 20)      # NUL-terminated or long
                and sum(1 for b in seg if A(b)) >= 0.60 * L        # alpha-majority
                and any(b in V for b in seg)):                     # contains a vowel
            out.append((i, j))
        i = j
    return out


class VxDDisasm:
    def __init__(s, v):
        s.v = v
        s.fx = v.fixups()
        s.ddb = v.ddb()
        s.code = {o["idx"]: v.object_bytes(o) for o in v.objects}
        s.objs = {o["idx"]: o for o in v.objects}
        s.insns = {}                # (oi, off) -> row dict
        s.mark = set()              # (oi, off) bytes that are code
        s.data = set()              # (oi, off) bytes KNOWN to be data (tables, DDB) -- never code
        s.labels = {}               # (oi, off) -> label name
        s.jtables = {}              # (oi, base) -> [(oi, target), ...]
        s.svctab = None             # (oi, base, count) if a service table was found
        s.entries = set()           # (oi, off) authoritative entry points (DDB procs + service table)
        #                             -- no instruction may SPAN one, so an entry always starts an insn

    def fsite(s, oi, off):
        return s.fx.get((oi, off))

    def _free(s, oi, a, sz):
        """True iff [a, a+sz) is entirely unclaimed -- so decoding here creates no overlap."""
        return all((oi, k) not in s.mark for k in range(a, a + sz))

    def _mark_data(s, oi, start, length):
        for k in range(start, start + length):
            s.data.add((oi, k))

    def _remark_data_from_fixups(s):
        """A relocation always targets a clean address (a label the linker resolves), never the
        middle of a real instruction. So an INTERNAL fixup target that lands INSIDE a decoded
        instruction proves that 'instruction' is actually DATA the linear sweep mis-decoded (a
        pointer/variable in a data region). Reclassify the whole covering instruction as data --
        drop it from insns and mark its bytes -- so _collect_labels can place a label at the
        target and the loadable emit preserves the region's bytes / emits `dd offset` for its
        pointers instead of re-encoding it as an instruction (which can change the bytes and leave
        a data pointer holding a garbage address -> a mid-init fault, e.g. VKD's hot-key list).
        Iterate: reclassifying one span can expose targets a longer mis-decode had shadowed."""
        for _ in range(32):
            cover = {}                                      # (oi, byte) -> covering instruction start
            for (oi, a), row in s.insns.items():
                for k in range(a, a + row["size"]):
                    cover[(oi, k)] = a
            victims = set()
            for (soi, soff), f in s.fx.items():
                if f["kind"] != "internal" or f.get("target") is None:
                    continue
                start = cover.get((f["obj"], f["target"]))
                if start is not None and start != f["target"]:   # target sits mid-instruction
                    victims.add((f["obj"], start))
            if not victims:
                break
            for (oi, a) in victims:
                row = s.insns.pop((oi, a), None)
                if row:
                    for k in range(a, a + row["size"]):
                        s.mark.discard((oi, k))
                    s._mark_data(oi, a, row["size"])

    def _remark_pointer_tables(s):
        """A run of consecutive INTERNAL-fixup SOURCES at 4-byte stride is a pointer table (dispatch
        table, vtable, handler array): each source dword is a relocated pointer, i.e. DATA. The linear
        sweep can mis-decode such a table as code (it is reached only through an indirect `[base+i*4]`
        load, never by the control graph). When it does, the per-instruction loadable emit can only
        relocate the entries that happen to fall on an imm32 operand and silently drops the rest -- in
        practice every OTHER entry loses its fixup, leaving an unrelocated pointer that faults the
        moment the dispatcher jumps/returns through it (vpicd's IRQ dispatch, and the whole 9-module
        divergence tail). `_remark_data_from_fixups` only catches fixup TARGETS landing mid-instruction,
        never the SOURCES, so a cleanly-decoded-but-wrong pointer table slips past it. Reclassify the
        whole run as data so the walk emits it through `_loadable_data` (a `dd offset` per entry = one
        real relocation each). A no-op when the table is already data (the common, already-correct case)."""
        from collections import defaultdict
        srcs = defaultdict(list)
        for (oi, off), f in s.fx.items():
            if f.get("kind") == "internal" and f.get("target") is not None:
                srcs[oi].append(off)
        for oi, offs in srcs.items():
            offs = sorted(set(offs))
            i = 0
            while i < len(offs):
                j = i
                while j + 1 < len(offs) and offs[j + 1] == offs[j] + 4:
                    j += 1
                if j - i + 1 >= 3:                              # >=3 stride-4 pointers = a table
                    lo, hi = offs[i], offs[j] + 4
                    for (moi, a), row in [(k, r) for k, r in s.insns.items() if k[0] == oi]:
                        if a < hi and a + row["size"] > lo:    # instruction overlaps the table -> mis-decode
                            s.insns.pop((moi, a), None)
                            for k in range(a, a + row["size"]):
                                s.mark.discard((moi, k))
                            s._mark_data(moi, a, row["size"])
                i = j + 1

    def _remark_split_fixup_sources(s):
        """A single INTERNAL fixup SOURCE dword [S,S+4) is a relocated pointer -- four contiguous DATA
        bytes. If the linear sweep decoded code that only PARTIALLY overlaps it (the dword straddles a
        gap/code boundary or two instructions -- e.g. a data pointer whose high bytes the sweep read as
        the start of the next function), neither the per-instruction emit nor `_loadable_data` can place
        the relocation: a `dd offset` would cross the boundary, so the pointer is byte-preserved and
        loses its fixup -> unrelocated address -> fault. Reclassify every instruction straddling such a
        dword as data and mark the dword's bytes data, so `_loadable_data` emits one clean `dd offset`
        (ifsmgr's indexed table-base pointer straddling a mis-decoded function entry). A dword fully
        inside ONE instruction's operand (mov/push offset X) is the normal relocation -- left untouched."""
        for _ in range(8):
            cover = {}
            for (oi, a), row in s.insns.items():
                for k in range(a, a + row["size"]):
                    cover[(oi, k)] = a
            victims = set()
            for (soi, soff), f in list(s.fx.items()):
                if f.get("kind") != "internal" or f.get("target") is None:
                    continue
                starts = {cover.get((soi, soff + d)) for d in range(4)}
                starts.discard(None)
                if not starts:
                    continue                               # dword entirely in a gap -> _loadable_data handles
                if len(starts) == 1:
                    st = next(iter(starts))
                    row = s.insns.get((soi, st))
                    if row and st <= soff and soff + 4 <= st + row["size"]:
                        continue                           # dword fully inside one instruction's operand -> ok
                for st in starts:                          # straddles a boundary -> reclassify the overlap
                    row = s.insns.pop((soi, st), None)
                    if row:
                        for k in range(st, st + row["size"]):
                            s.mark.discard((soi, k))
                        s._mark_data(soi, st, row["size"])
                s._mark_data(soi, soff, 4)
                victims.add((soi, soff))
            if not victims:
                break

    # ---- recursive descent -----------------------------------------------------
    def run(s):
        seeds = s._seeds()                                 # resolve entry points (control/service) FIRST
        for o in s.objs.values():                          # then carve ASCII string data out of every
            if not o["exec"]:                              # exec object BEFORE descent, so the gap
                continue                                    # seeder's fixup-to-string targets can't
            oi = o["idx"]                                   # decode strings as bogus code and cascade
            for rs, re in _string_ranges(s.code[oi]):       # past them. NEVER carve across an
                if not any((oi, k) in s.entries for k in range(rs, re)):   # authoritative entry point.
                    s._mark_data(oi, rs, re - rs)
        s._descend(seeds)
        for _ in range(8):                                 # gap recovery: reach code the control
            more = s._gap_seeds()                           # graph doesn't (indirect/computed calls)
            if not more:
                break
            s._descend(more)
        s._remark_data_from_fixups()                       # reclassify data the sweep decoded as code
        s._remark_pointer_tables()                          # reclassify mis-swept pointer tables as data
        s._remark_split_fixup_sources()                     # reclassify pointers straddling a mis-decode
        s._collect_labels()
        s._apply_ddk_names()
        return s

    def _apply_ddk_names(s):
        """Overlay real names from the DDK onto the anonymous sub_/loc_ labels: control-dispatch
        handlers get the message they service (DEVICE_INIT, SYS_VM_INIT, ...), and the 12 core VxDs
        with a debug .SYM get their real internal function names where a symbol's offset lands on a
        recovered function start."""
        try:
            from src.engines.inverse import vxd_symbols
        except Exception:
            return
        db = s.ddb
        if not db:
            return
        dev = db["name"]
        used = set(s.labels.values())                      # never introduce a duplicate label name

        def _assign(key, name):
            if name in used or key not in s.insns:
                return
            if not s.labels.get(key, "").startswith(("sub_", "loc_")):   # keep semantic names
                return
            s.labels[key] = name
            used.add(name)

        cp = next((k for k, nm in s.labels.items() if nm == dev + "_Control"), None)
        if cp:                                             # name control handlers by their message
            msgs = vxd_symbols.control_messages()
            oi, a = cp
            base = 0
            for _ in range(10):
                row = s.insns.get((oi, a))
                if not row:
                    break
                if row["mnem"] == "sub" and row["ops"].replace(" ", "").startswith("eax,"):
                    try:
                        base = int(row["ops"].split(",")[1].strip(), 0)
                    except ValueError:
                        pass
                if row["mnem"] == "jmp" and "*4" in row["ops"]:
                    jt = None
                    for k in range(a + 1, a + row["size"]):
                        f = s.fx.get((oi, k))
                        if f and f["kind"] == "internal":
                            jt = s.jtables.get((f["obj"], f["target"]))
                            break
                    for i, tk in enumerate(jt or []):
                        msg = i + base
                        if msg in msgs and jt.count(tk) == 1:            # unique handler only
                            _assign(tk, "%s_%s" % (dev, msgs[msg]))
                    break
                a += row["size"]
        for off, name in vxd_symbols.sym_names(dev).items():   # real internal names where the offset
            _assign((1, off), name)                            #  lands on a function start

    def _gap_seeds(s):
        """Find plausible code entry points the descent missed: a fixup target or a function
        prologue (55 8B EC) sitting in an unreached, non-DDB gap of an executable object."""
        seeds = []
        for oi, o in s.objs.items():
            if not o["exec"]:
                continue
            code = s.code[oi]
            n = len(code)
            # internal fixup targets that are unclaimed and look like an instruction start
            for (foi, foff), f in s.fx.items():
                if f["kind"] != "internal" or f["obj"] != oi:
                    continue
                t = f["target"]
                if t is None or not (0 <= t < n) or (oi, t) in s.mark or (oi, t) in s.data:
                    continue
                if s._looks_code(code, t):
                    seeds.append((oi, t))
            # bare `push ebp; mov ebp, esp` prologues in gaps
            i = 0
            while i < n - 2:
                if code[i] == 0x55 and code[i + 1] == 0x8B and code[i + 2] == 0xEC \
                        and (oi, i) not in s.mark and (oi, i) not in s.data:
                    seeds.append((oi, i))
                    i += 3
                else:
                    i += 1
        return seeds

    def _looks_code(s, code, t):
        """Cheap plausibility gate for a gap seed: a valid instruction decodes and it isn't a run
        of NUL padding (which decodes as `add [eax],al` forever)."""
        if code[t] == 0x00:
            return False
        ins = next(_MD.disasm(code[t:t + 16], t), None)
        return ins is not None

    def _seeds(s):
        seeds = []
        db = s.ddb
        if not db:
            for o in s.objs.values():                      # no DDB: fall back to object entry 0
                if o["exec"]:
                    seeds.append((o["idx"], 0))
            return seeds
        oi = db["obj"]
        s._mark_data(oi, db["rva"], 0x54)                  # the DDB itself is data, not code
        for fld, nm in ((DDB_CONTROL_PROC, "_Control"), (DDB_V86_API_PROC, "_V86_API"),
                        (DDB_PM_API_PROC, "_PM_API")):
            f = s.fsite(oi, db["rva"] + fld)
            if f and f["kind"] == "internal" and s.objs.get(f["obj"], {}).get("exec"):
                seeds.append((f["obj"], f["target"]))
                s.entries.add((f["obj"], f["target"]))
                s.labels[(f["obj"], f["target"])] = (db["name"] or "Device") + nm
        # service table: DDB_Service_Table_Ptr -> array of DDB_Service_Size dword service procs
        st = s.fsite(oi, db["rva"] + DDB_SERVICE_TABLE)
        if st and st["kind"] == "internal":
            base = st["target"]; boi = st["obj"]
            cnt = 0
            size = int.from_bytes(s.code[oi][db["rva"] + DDB_SERVICE_SIZE:
                                             db["rva"] + DDB_SERVICE_SIZE + 4], "little")
            known = 0 < size <= 8192                        # DDB_Service_Table_Size is authoritative
            for j in range(size if known else 4096):        # (VMM has 406; a fixed 256 cap dropped 256+)
                e = s.fsite(boi, base + j * 4)
                if not (e and e["kind"] == "internal"):     # a reserved slot (dd 0, no fixup): with a
                    if known:                               # known size skip it and keep going, else the
                        cnt += 1; continue                  # first gap marks the end of the run
                    break
                if s.objs.get(e["obj"], {}).get("exec"):
                    seeds.append((e["obj"], e["target"]))
                    s.entries.add((e["obj"], e["target"]))  # a service handler always starts an insn
                    sname = _SVC.name(db["name"], j)        # this driver's own DDK service name
                    lbl = sname or "%s_Service_%02d" % (db["name"] or "Dev", j)   # already qualified
                    s.labels.setdefault((e["obj"], e["target"]), lbl)
                cnt += 1
            if cnt:
                s.svctab = (boi, base, cnt)
                s._mark_data(boi, base, cnt * 4)           # the service table is a pointer array
        return seeds

    def _descend(s, seeds):
        work = list(seeds)
        while work:
            oi, a = work.pop()
            code = s.code.get(oi)
            if code is None or a < 0 or a >= len(code):
                continue
            n = len(code)
            while a < n:
                if (oi, a) in s.insns:
                    break
                if (oi, a) in s.mark or (oi, a) in s.data:  # mid-instruction of prior code, or a
                    break                                   #  known data table -- abandon this walk
                # VxD service call: CD 20 + inline DWORD
                if code[a] == 0xCD and a + 5 < n and code[a + 1] == 0x20:
                    if not s._free(oi, a, 6):
                        break
                    if s.entries and any((oi, a + k) in s.entries for k in range(1, 6)):
                        break                               # must not span a service/DDB entry point
                    imp = s.fsite(oi, a + 2)
                    if imp and imp["kind"] == "import":
                        dev, svc, via = imp["module"], imp["ordinal"], "import"
                    else:
                        dw = struct.unpack_from("<I", code, a + 2)[0]
                        dev, svc, via = dw >> 16, dw & 0xFFFF, "inline"
                    s.insns[(oi, a)] = {"off": a, "size": 6, "mnem": "int", "ops": "20h",
                                        "raw": code[a:a + 6], "fx": None, "vxdcall": (dev, svc, via)}
                    for k in range(a, a + 6):
                        s.mark.add((oi, k))
                    a += 6
                    continue
                ins = next(_MD.disasm(code[a:a + 16], a), None)
                if ins is None:
                    break
                if not s._free(oi, a, ins.size):            # would overlap authoritative code
                    break
                if s.entries and any((oi, a + k) in s.entries for k in range(1, ins.size)):
                    break                                   # would swallow an authoritative entry point
                #                                             (service proc / DDB proc) -> let it start fresh
                fx = None
                for k in range(a + 1, a + ins.size):
                    if (oi, k) in s.fx:
                        fx = s.fx[(oi, k)]
                        break
                row = {"off": a, "size": ins.size, "mnem": ins.mnemonic, "ops": ins.op_str,
                       "raw": code[a:a + ins.size], "fx": fx, "vxdcall": None}
                s.insns[(oi, a)] = row
                for k in range(a, a + ins.size):
                    s.mark.add((oi, k))
                m = ins.mnemonic
                # indirect jump through a table: jmp dword ptr [reg*4 (+ base)]
                if m == "jmp" and "[" in ins.op_str and "*4" in ins.op_str:
                    s._jump_table(oi, a, ins, work)
                    break
                if m in _TERMS:
                    break
                if m == "jmp":                              # direct unconditional jump = tail
                    s._follow(oi, ins, fx, work)
                    break
                if m == "call" or m[0] == "j" or m.startswith("loop"):
                    s._follow(oi, ins, fx, work)            # cross-object call/jcc uses the fixup
                    # call / jcc fall through to the next instruction
                a += ins.size

    def _follow(s, oi, ins, fx, work):
        """Queue a branch target. A relocated (cross-object) branch has rel32=0 and a fixup that
        names the real destination; a plain intra-object branch uses the disassembled operand."""
        if fx and fx["kind"] == "internal":
            work.append((fx["obj"], fx["target"]))
            return
        t = _direct_target(ins)
        if t is not None:
            work.append((oi, t))

    def _jump_table(s, oi, a, ins, work):
        base = None                                        # the table base = the fixup on this insn
        for k in range(a + 1, a + ins.size):
            f = s.fx.get((oi, k))
            if f and f["kind"] == "internal":
                base = f["target"]; boi = f["obj"]
                break
        if base is None:
            return
        targets = []
        for j in range(1024):
            e = s.fsite(boi, base + j * 4)
            if not (e and e["kind"] == "internal"):
                break
            targets.append((e["obj"], e["target"]))
            if s.objs.get(e["obj"], {}).get("exec"):
                work.append((e["obj"], e["target"]))
        if targets:
            s.jtables[(boi, base)] = targets
            s._mark_data(boi, base, len(targets) * 4)      # the table bytes are data, not code

    def _collect_labels(s):
        calltgt = set()
        for (oi, a), row in s.insns.items():               # a call destination reads as a proc
            if row["mnem"] == "call":
                if row["fx"] and row["fx"]["kind"] == "internal":
                    calltgt.add((row["fx"]["obj"], row["fx"]["target"]))
                else:
                    t = _direct_target_row(row)
                    if t is not None:
                        calltgt.add((oi, t))
        for (oi, a), row in s.insns.items():               # branch/call targets
            t = _direct_target_row(row)
            if t is not None and (oi, t) in s.insns:
                s.labels.setdefault((oi, t), s._lname((oi, t), calltgt))
            if row["fx"] and row["fx"]["kind"] == "internal":   # cross-object branch/ref target
                key = (row["fx"]["obj"], row["fx"]["target"])
                if key in s.insns:
                    s.labels.setdefault(key, s._lname(key, calltgt))
        for (boi, base), targets in s.jtables.items():      # jump-table destinations
            for (toi, toff) in targets:
                if (toi, toff) in s.insns:
                    s.labels.setdefault((toi, toff), s._lname((toi, toff), calltgt))
        for (oi, off), f in s.fx.items():                   # fixup targets that are code
            if f["kind"] == "internal" and (f["obj"], f["target"]) in s.insns:
                s.labels.setdefault((f["obj"], f["target"]), s._lname((f["obj"], f["target"]), calltgt))

    def _lname(s, key, calltgt):
        # object-qualified: two objects can share an offset (both often start at 0), so the bare
        # offset is ambiguous across segments -- qualify with the object to keep labels unique.
        return ("sub_%d_%04X" if key in calltgt else "loc_%d_%04X") % key


def _direct_target(ins):
    t = ins.op_str.strip()
    if t.startswith("0x"):
        try:
            return int(t, 16)
        except ValueError:
            return None
    return None


def _direct_target_row(row):
    m = row["mnem"]
    if m == "call" or m[0:1] == "j" or m.startswith("loop"):
        t = row["ops"].strip()
        if t.startswith("0x"):
            try:
                return int(t, 16)
            except ValueError:
                return None
    return None


def _svc_name(dev, svc, via):
    dname = VXD_DEVICES.get(dev, "Dev_%04X" % dev)
    kind = "VMMCall" if dev == 0x0001 else "VxDCall"
    sname = _SVC.name(dname, svc)                       # DDK name if the device+ordinal is known
    ref = "%s.%s" % (dname, sname) if sname else "%s.Service_%04Xh" % (dname, svc)
    tag = "" if via == "inline" else "  ; (import-fixup)"
    return "%s  %s" % (kind, ref) + tag


def emit_asm(s_v):
    """Emit annotated 32-bit assembly for the whole VxD (accepts an LEFile)."""
    D = VxDDisasm(s_v).run()
    v = s_v
    db = D.ddb
    name = (db["name"] if db else None) or v.resident_name()
    L = ["; %s -- reconstructed 32-bit disassembly (VxD / LE format)" % name,
         "; %d objects, %d instructions recovered by recursive descent" % (
             len(v.objects), len(D.insns))]
    if db:
        L += ["", "Declare_Virtual_Device %s, %d, %d, %s_Control, 0%04Xh, Undefined_Init_Order" % (
            db["name"], db["major"], db["minor"], db["name"], db["device_id"]),
            ";   DDK version 0%03Xh   init_order 0%08Xh   flags 0%04Xh" % (
            db["version"], db["init_order"], db["flags"])]
        if D.svctab:
            boi, base, cnt = D.svctab
            L.append(";   service table: obj%d:0x%x, %d services" % (boi, base, cnt))
    L.append("")
    for o in v.objects:
        oi = o["idx"]
        seg = "_LTEXT" if o["exec"] else ("_LDATA" if o["write"] else "_LRDATA")
        L.append("VxD_%s SEGMENT   ; object %d  vsize=0x%x  %s%s%s" % (
            seg, oi, o["vsize"], "R" if o["read"] else "-", "W" if o["write"] else "-",
            "X" if o["exec"] else "-"))
        code = D.code[oi]
        a = 0
        n = len(code)
        while a < n:
            lab = D.labels.get((oi, a))
            row = D.insns.get((oi, a))
            if row is None:                                 # a data span: coalesce to next code/label
                start = a
                while a < n and (oi, a) not in D.mark:
                    a += 1
                    if (oi, a) in D.labels and (oi, a) in D.insns:
                        break
                L += _emit_data(D, oi, start, a)
                continue
            if lab:
                L.append("%s:" % lab)
            if row["vxdcall"]:
                dev, svc, via = row["vxdcall"]
                L.append("        %-40s ; %s" % (_svc_name(dev, svc, via), row["raw"].hex()))
            else:
                ops, resolved = _render_ops(D, oi, row)
                line = "        %-6s %s" % (row["mnem"], ops)
                if row["fx"] and not resolved:              # non-branch fixup: annotate the symbol
                    line = "%-44s ; -> %s" % (line, _fx_sym(D, row["fx"]))
                L.append(line)
            a += row["size"]
        L.append("VxD_%s ENDS" % seg)
        L.append("")
    return "\n".join(L)


_BRANCHES = {"call", "jmp", "je", "jne", "jz", "jnz", "ja", "jae", "jb", "jbe", "jg", "jge",
             "jl", "jle", "jo", "jno", "js", "jns", "jp", "jnp", "jc", "jnc", "jcxz", "jecxz",
             "loop", "loope", "loopne"}


def _render_ops(D, oi, row):
    """Return (operand text, resolved?). A branch to a labelled location renders as that label
    -- for a cross-object branch (rel32=0 + fixup) the label comes from the fixup target -- so
    the listing reads as symbolic control flow rather than raw offsets."""
    if row["mnem"] in _BRANCHES:
        if row["fx"] and row["fx"]["kind"] == "internal":
            sym = D.labels.get((row["fx"]["obj"], row["fx"]["target"]))
            if sym:
                return sym, True
        t = _direct_target_row(row)
        if t is not None and (oi, t) in D.labels:
            return D.labels[(oi, t)], True
    return row["ops"], False


def _fx_sym(D, f):
    if f["kind"] == "internal":
        nm = D.labels.get((f["obj"], f["target"]))
        return nm or ("obj%d:0x%04X" % (f["obj"], f["target"] or 0))
    dname = VXD_DEVICES.get(f["module"], "Mod_%04X" % f["module"])
    return "%s.Service_%04Xh" % (dname, f["ordinal"] or 0)


def _emit_data(D, oi, start, end):
    """Emit a data span [start,end): the DDB structured, jump tables as `dd loc_X`, dword-fixup
    runs as symbol pointers, and everything else as raw bytes."""
    out = []
    code = D.code[oi]
    a = start
    db = D.ddb
    while a < end:
        lab = D.labels.get((oi, a))
        if lab:
            out.append("%s:" % lab)
        if db and oi == db["obj"] and a == db["rva"]:       # the DDB itself
            out.append("        ; ---- Device Descriptor Block ----")
            out.append("        dd  0                          ; DDB_Next")
            out.append("        dw  0%04Xh                      ; DDB_SDK_Version" % db["version"])
            out.append("        dw  0%04Xh                      ; DDB_Req_Device_Number" % db["device_id"])
            out.append("        db  %d, %d                       ; DDB_Dev_Major/Minor" % (db["major"], db["minor"]))
            out.append("        dw  0%04Xh                      ; DDB_Flags" % db["flags"])
            out.append("        db  '%-8s'                  ; DDB_Name" % db["name"])
            out.append("        dd  0%08Xh                  ; DDB_Init_Order" % db["init_order"])
            for fld, nm in ((DDB_CONTROL_PROC, "DDB_Control_Proc"), (DDB_V86_API_PROC, "DDB_V86_API_Proc"),
                            (DDB_PM_API_PROC, "DDB_PM_API_Proc"), (DDB_SERVICE_TABLE, "DDB_Service_Table_Ptr")):
                f = D.fsite(oi, db["rva"] + fld)
                sym = _fx_sym(D, f) if f else "0"
                out.append("        dd  %-24s ; %s" % (sym, nm))
            a = db["rva"] + 0x38
            continue
        jt = D.jtables.get((oi, a))
        if jt:                                              # a jump/pointer table
            out.append("        ; ---- dispatch table (%d entries) ----" % len(jt))
            for (toi, toff) in jt:
                sym = D.labels.get((toi, toff)) or ("obj%d:0x%04X" % (toi, toff))
                out.append("        dd  %s" % sym)
            a += len(jt) * 4
            continue
        f = D.fsite(oi, a)                                  # a lone pointer slot
        if f and f["kind"] == "internal":
            out.append("        dd  %-24s ; fixup" % _fx_sym(D, f))
            a += 4
            continue
        # raw bytes up to the next interesting boundary
        b = a
        while b < end and not D.labels.get((oi, b)) and not D.jtables.get((oi, b)) \
                and not (D.fsite(oi, b) and D.fsite(oi, b)["kind"] == "internal") \
                and not (db and oi == db["obj"] and b == db["rva"]):
            b += 1
        chunk = code[a:b if b > a else a + 1]
        for i in range(0, len(chunk), 16):
            out.append("        db  " + ", ".join("0%02Xh" % c for c in chunk[i:i + 16]))
        a = b if b > a else a + 1
    return out


def verify(v):
    """Faithfulness check: the disassembly is a VIEW of the real bytes, so every executable byte
    must be accounted for exactly once -- claimed by exactly one decoded instruction (code) or by
    none (data). Overlapping instructions mean a mis-seeded entry point disassembled bytes that
    are really mid-instruction; a byte-range past vsize means over-read. Returns per-object
    {n, code, data, overlap, overrun} -- overlap==overrun==0 across the module is the proof."""
    D = VxDDisasm(v).run()
    rep = []
    for o in v.objects:
        if not o["exec"]:
            continue
        oi = o["idx"]; n = o["vsize"]
        occ = bytearray(n)
        overlap = overrun = 0
        for (moi, off), row in D.insns.items():
            if moi != oi:
                continue
            for k in range(off, off + row["size"]):
                if k >= n:
                    overrun += 1
                elif occ[k]:
                    overlap += 1
                else:
                    occ[k] = 1
        code = sum(occ)
        rep.append({"obj": oi, "n": n, "code": code, "data": n - code,
                    "overlap": overlap, "overrun": overrun})
    return rep


def disassemble_file(path, outdir=None):
    """Disassemble a VxD file (LE, LX, or a W3 bundle of several) and, if outdir is given, write
    one <module>.asm per embedded module. Returns a list of per-module stat dicts."""
    from src.formats import le
    stats = []
    for name, v in le.open_vxds(path):
        D = VxDDisasm(v).run()
        tot = sum(o["vsize"] for o in v.objects if o["exec"]) or 1
        st = {"name": name, "insns": len(D.insns), "cov": 100 * len(D.mark) / tot,
              "labels": len(D.labels), "jtables": len(D.jtables),
              "device": (D.ddb or {}).get("name"), "device_id": (D.ddb or {}).get("device_id")}
        if outdir:
            os.makedirs(outdir, exist_ok=True)
            with open(os.path.join(outdir, name.lower() + ".asm"), "w") as f:
                f.write(emit_asm(v))
        stats.append(st)
    return stats


if __name__ == "__main__":
    from src.formats import le
    import glob
    args = sys.argv[1:]
    if args and args[0] == "--verify":
        srcdir = args[1] if len(args) > 1 else paths.data_root() + "/oracle"
        files = sorted(glob.glob(os.path.join(srcdir, "*.vxd")) + glob.glob(os.path.join(srcdir, "*.386")))
        clean = dirty = mods = 0
        tcode = tdata = tov = torn = 0
        for p in files:
            try:
                for name, v in le.open_vxds(p):
                    rep = verify(v)
                    ov = sum(r["overlap"] for r in rep); orn = sum(r["overrun"] for r in rep)
                    c = sum(r["code"] for r in rep); dta = sum(r["data"] for r in rep)
                    tcode += c; tdata += dta; tov += ov; torn += orn; mods += 1
                    if ov or orn:
                        dirty += 1
                        print("  DIRTY %-12s overlap=%d overrun=%d" % (name, ov, orn))
                    else:
                        clean += 1
            except Exception as e:
                print("  ERROR %-16s %s" % (os.path.basename(p), str(e)[:50]))
        print("== verify: %d modules, %d CLEAN, %d dirty | code=%d data=%d overlap=%d overrun=%d ==" % (
            mods, clean, dirty, tcode, tdata, tov, torn))
    elif args and args[0] == "--all":
        srcdir = args[1] if len(args) > 1 else paths.data_root() + "/oracle"
        outdir = args[2] if len(args) > 2 else "recovered/vxd"
        files = sorted(glob.glob(os.path.join(srcdir, "*.vxd")) + glob.glob(os.path.join(srcdir, "*.386")))
        done = errs = 0
        for p in files:
            try:
                for st in disassemble_file(p, outdir):
                    done += 1
                    print("  %-12s %-9s id=0x%04x  %6d insns  %3.0f%% cov  %d jt" % (
                        st["name"], st["device"] or "?", st["device_id"] or 0, st["insns"],
                        st["cov"], st["jtables"]))
            except Exception as e:
                errs += 1
                print("  ERROR %-16s %s" % (os.path.basename(p), str(e)[:50]))
        print("== %d modules -> %s, %d errors ==" % (done, outdir, errs))
    else:
        for p in args:
            print(emit_asm(le.LEFile.from_file(p)))
