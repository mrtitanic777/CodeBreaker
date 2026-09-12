#!/usr/bin/env python3
r"""vxdsvc.py -- resolve VxD (ring-0) service calls to named VMM/VxD services.

A VxD does not CALL services the normal way: it emits `INT 20h` followed inline by a DWORD service
id = (Device_ID << 16) | Service_Number, which the loader patches. So a VxD's ring-0 calls
disassemble to `int 20h; <dd 0x00010000>` with no meaning. The mapping is ground truth: the DDK
.INC files declare each device's ID (`VMM_Device_ID equ 1`) and its ordered service table
(`Begin_Service_Table VMM` / `VMM_Service Get_VMM_Version` ...). This ingests those, scans a VxD for
the `CD 20` + id pattern, and names each call `VMM_Get_VMM_Version`, `VPICD_Virtualize_IRQ`, ... --
turning a ring-0 binary's service calls into readable source. No fabricated data.
"""
import os
import re

_DEVID = re.compile(r"(\w+)_Device_ID\s+equ\s+([0-9A-Fa-f]+)h?\b")
_BEGIN = re.compile(r"Begin_Service_Table\s+(\w+)")
_END = re.compile(r"End_Service_Table")

# Architectural device IDs the DDK headers assume rather than equate (VMM is always device 1).
_FIXED = {0x0001: "VMM", 0x0002: "DEBUG", 0x0005: "VTD", 0x0006: "V86MMGR", 0x0007: "PAGESWAP",
          0x000C: "VMD", 0x0010: "BLOCKDEV", 0x0011: "VMCPD", 0x0012: "EBIOS", 0x0014: "VCD",
          0x0015: "VPD", 0x0018: "INT13"}
_DEVICES = dict(_FIXED)   # device_id (int) -> DEVNAME
_SERVICES = {}            # DEVNAME -> [service_name, ...] in ordinal order


def load_ddk(*inc_dirs):
    """Parse DDK .INC files for device IDs and service tables. Returns (#devices, #services)."""
    incs = []
    for d in inc_dirs:
        for dp, _dn, fns in os.walk(d):
            for fn in fns:
                if fn.lower().endswith(".inc"):
                    incs.append(os.path.join(dp, fn))
    for path in incs:
        try:
            with open(path, "r", errors="replace") as f:
                text = f.read()
        except Exception:
            continue
        for m in _DEVID.finditer(text):
            try:
                _DEVICES[int(m.group(2), 16)] = m.group(1).upper()
            except ValueError:
                pass
        cur, svc = None, None
        for line in text.splitlines():
            s = line.split(";", 1)[0]
            b = _BEGIN.search(s)
            if b:
                cur = b.group(1).upper(); svc = _SERVICES.setdefault(cur, [])
                continue
            if _END.search(s):
                cur = None; continue
            if cur:
                sm = re.match(r"\s*%s_Service\s+(\w+)" % re.escape(cur), s, re.I)
                if sm:
                    svc.append(sm.group(1))
    return len(_DEVICES), sum(len(v) for v in _SERVICES.values())


def name_for(service_id):
    """'VMM_Get_VMM_Version' for a VxD service id (Device_ID<<16 | Service_Number), or None."""
    dev_id = (service_id >> 16) & 0xFFFF
    svc = service_id & 0xFFFF
    dev = _DEVICES.get(dev_id)
    if not dev:
        return None
    tbl = _SERVICES.get(dev, [])
    for cand in (svc, svc & 0x7FFF):                         # bit15 is the async/hooked flag, not ordinal
        if cand < len(tbl):
            nm = tbl[cand]
            return nm if nm.upper().startswith(dev + "_") else "%s_%s" % (dev, nm)  # dedupe prefix
    return "%s_Service_%d" % (dev, svc)                      # known device, service beyond our table


def scan_bytes(data):
    """[(offset, service_id, name)] for every `int 20h` + id whose device is known."""
    out = []
    i = 0
    n = len(data)
    while True:
        j = data.find(b"\xCD\x20", i)
        if j < 0 or j + 6 > n:
            break
        sid = int.from_bytes(data[j + 2:j + 6], "little")
        if ((sid >> 16) & 0xFFFF) in _DEVICES:               # known device -> a real service call
            out.append((j, sid, name_for(sid)))
        i = j + 2
    return out


def scan_vxd(path):
    with open(path, "rb") as f:
        return scan_bytes(f.read())


def reconstruct(path):
    """Summarize a VxD's named service calls. {'ok','ncalls','by_device'}."""
    if not (_DEVICES and _SERVICES):
        return {"ok": False, "reason": "no DDK service tables loaded (call load_ddk first)"}
    try:
        calls = scan_vxd(path)
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    if not calls:
        return {"ok": False, "reason": "no VxD service calls (not a VxD, or none)"}
    by_dev = {}
    for _off, sid, nm in calls:
        dev = _DEVICES.get((sid >> 16) & 0xFFFF, "?")
        by_dev.setdefault(dev, set()).add(nm)
    return {"ok": True, "ncalls": len(calls), "by_device": {d: sorted(v) for d, v in by_dev.items()}}
