#!/usr/bin/env python3
r"""vmminit.py -- trace VMM's Sys_Critical_Init on Unicorn (VMM32 seam, init-environment RE).

The winmin2 loader reaches VMM (boot-tested); VMMINIT.md maps the init dispatch: the real entry is
the DDB control_proc at linear 0x1014F8, dispatching on the control message in EAX via a jump table,
into a common handler that reads an EBP-relative Client Register Structure and calls VMM services via
INT 20h. This harness stands up a MINIMAL such environment, calls control_proc with EAX = the given
message (default Sys_Critical_Init = 0), and records: how far it runs, the sequence of INT-20h VxD
services it invokes (the environment it needs), any fault, and the return. Each service discovered is
the next thing to implement -- iterate.

    wt --target win95 vmminit [--msg N]        # trace control message N (0 = Sys_Critical_Init)
"""
import os
import struct

IMG_BASE = 0x00100000        # matches vmmimage load_base
CONTROL_PROC = 0x001014F8    # VMM DDB control_proc (from VMMINIT.md)
CRS = 0x00300000             # Client Register Structure
VMSTK = 0x00320000           # a VM stack region (for Client_SS:ESP)
STACK = 0x00400000           # our call stack
RET_SENTINEL = 0x00BEEF00    # control_proc returns here -> we stop
INSN_CAP = 200000


def _vmm_image():
    from src.targets.win95 import vmmimage
    img, base, entry, layout = vmmimage.build(IMG_BASE)
    return img, base, entry, layout


def trace(msg=0):
    import unicorn as U
    from unicorn.x86_const import (UC_X86_REG_EAX, UC_X86_REG_EBP, UC_X86_REG_ESP,
                                   UC_X86_REG_EIP, UC_X86_REG_EBX, UC_X86_REG_ECX, UC_X86_REG_EDX)
    img, base, _entry, layout = _vmm_image()
    uc = U.Uc(U.UC_ARCH_X86, U.UC_MODE_32)
    PAGE = 0x1000

    def mapspan(addr, size):
        lo = addr & ~(PAGE - 1)
        hi = (addr + size + PAGE - 1) & ~(PAGE - 1)
        uc.mem_map(lo, hi - lo)
        return lo

    # image (round up to page)
    isz = (len(img) + PAGE - 1) & ~(PAGE - 1)
    uc.mem_map(base, isz)
    uc.mem_write(base, img)
    for a, sz in ((CRS, 0x1000), (VMSTK - 0x4000, 0x8000), (STACK - 0x4000, 0x8000)):
        mapspan(a, sz)

    # minimal Client Register Structure: SS:ESP point at the VM stack (seg<<4 model the handler uses)
    crs = bytearray(0x80)
    struct.pack_into("<I", crs, 0x30, 0x1000)            # Client_ESP (offset within Client_SS<<4)
    struct.pack_into("<I", crs, 0x34, (VMSTK >> 4))      # Client_SS  (so SS<<4 == VMSTK)
    uc.mem_write(CRS, bytes(crs))                         # (VMSTK region already mapped above)

    esp = STACK
    uc.mem_write(esp - 4, struct.pack("<I", RET_SENTINEL))
    esp -= 4
    uc.reg_write(UC_X86_REG_ESP, esp)
    uc.reg_write(UC_X86_REG_EBP, CRS)                     # EBP -> Client_Reg_Struc
    uc.reg_write(UC_X86_REG_EAX, msg)                     # control message
    uc.reg_write(UC_X86_REG_EBX, VMSTK)                   # EBX often = current VM handle
    uc.reg_write(UC_X86_REG_EIP, CONTROL_PROC)

    rec = {"services": [], "path_len": 0, "exit": None, "faults": [], "int41": 0}
    mapped = set()

    def hook_code(uc, address, size, ud):
        rec["path_len"] += 1
        if address == RET_SENTINEL:
            rec["exit"] = "ret"; uc.emu_stop(); return
        if rec["path_len"] > INSN_CAP:
            rec["exit"] = "insn-cap"; uc.emu_stop()

    def hook_intr(uc, intno, ud):
        eip = uc.reg_read(UC_X86_REG_EIP)
        if intno == 0x20:                                # VxDcall: dword after int 20h = VxD:service
            try:
                svc = struct.unpack("<I", uc.mem_read(eip, 4))[0]
            except Exception:
                svc = None
            rec["services"].append({"at": eip - IMG_BASE, "svc": svc})
            uc.reg_write(UC_X86_REG_EAX, 0)              # stub the service: return 0
            uc.reg_write(UC_X86_REG_EIP, eip + 4)        # skip the inline service dword
            if len(rec["services"]) > 400:
                rec["exit"] = "many-services"; uc.emu_stop()
        elif intno == 0x41:
            rec["int41"] += 1                            # debugger interface -> ignore
        else:
            rec["exit"] = "int-%#x" % intno; uc.emu_stop()

    def hook_unmapped(uc, access, address, size, value, ud):
        pg = address & ~(PAGE - 1)
        if pg in mapped or len(mapped) > 2048:
            rec["exit"] = "fault@%#x" % address; return False
        try:
            uc.mem_map(pg, PAGE); mapped.add(pg)
            if len(rec["faults"]) < 40:
                rec["faults"].append(address)
            return True
        except Exception:
            rec["exit"] = "map-fail@%#x" % address; return False

    uc.hook_add(U.UC_HOOK_CODE, hook_code)
    uc.hook_add(U.UC_HOOK_INTR, hook_intr)
    uc.hook_add(U.UC_HOOK_MEM_READ_UNMAPPED | U.UC_HOOK_MEM_WRITE_UNMAPPED
                | U.UC_HOOK_MEM_FETCH_UNMAPPED, hook_unmapped)
    try:
        uc.emu_start(CONTROL_PROC, 0, count=INSN_CAP + 10)
    except U.UcError as e:
        if rec["exit"] is None:
            rec["exit"] = "ucerr:%s@%#x" % (e, uc.reg_read(UC_X86_REG_EIP))
    rec["eip_final"] = uc.reg_read(UC_X86_REG_EIP)
    rec["eax_final"] = uc.reg_read(UC_X86_REG_EAX)
    return rec, layout


def main(argv):
    from src.core import cores
    cores.set_target(os.path.join(os.path.dirname(os.path.dirname(__file__)), "win95"))
    msg = int(argv[argv.index("--msg") + 1], 0) if "--msg" in argv else 0
    names = {0: "Sys_Critical_Init", 1: "Device_Init", 2: "Init_Complete", 3: "Sys_VM_Init"}
    rec, layout = trace(msg)
    print("=== VMM control message %d (%s) ===" % (msg, names.get(msg, "msg_%d" % msg)))
    print("  image: %d objects, %d fixups, entry-stub 0x%X" %
          (layout["objects"], layout["fixups_applied"], IMG_BASE + 0x134FA))
    print("  instructions run : %d" % rec["path_len"])
    print("  exit             : %s" % rec["exit"])
    print("  final EIP / EAX  : 0x%08X / 0x%08X" % (rec["eip_final"], rec["eax_final"]))
    print("  INT 41h (debug)  : %d" % rec["int41"])
    print("  INT 20h services : %d" % len(rec["services"]))
    for s in rec["services"][:12]:
        sv = s["svc"]
        print("    at img+0x%05X  service 0x%08X" % (s["at"], sv if sv is not None else 0))
    if rec["faults"]:
        print("  first faults     : %s" % ", ".join("0x%X" % f for f in rec["faults"][:8]))


if __name__ == "__main__":
    import sys
    main(sys.argv[1:])
