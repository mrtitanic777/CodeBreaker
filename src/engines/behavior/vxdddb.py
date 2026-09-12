#!/usr/bin/env python3
r"""vxdddb.py -- recover a VxD's identity (Device Descriptor Block) and control-message handlers.

vxdsvc names the services a VxD CALLS; this recovers what the VxD IS. Every VxD embeds a Device
Descriptor Block: its 8-char device name, device ID, version, and the entry points for its control
procedure and V86/PM APIs. And its control procedure dispatches on the system control message in
EAX (Sys_Critical_Init, Device_Init, PnP_New_Devnode, ...). Recovering the DDB gives each of the
223 VxDs its identity; recognizing the control-message constants it tests gives its lifecycle
handlers -- the structural complement to service-call naming.
"""
import re
import struct

# system control messages (the values a VxD's control proc switches EAX on)
_SYSCTL = {
    0x00: "Sys_Critical_Init", 0x01: "Device_Init", 0x02: "Init_Complete",
    0x03: "Sys_VM_Init", 0x04: "Sys_VM_Terminate", 0x05: "Create_VM", 0x06: "VM_Critical_Init",
    0x07: "VM_Init", 0x08: "VM_Terminate", 0x09: "VM_Not_Executeable", 0x0A: "Destroy_VM",
    0x0B: "VM_Suspend", 0x0C: "VM_Resume", 0x0D: "Set_Device_Focus", 0x0E: "Begin_Message_Mode",
    0x0F: "End_Message_Mode", 0x10: "Reboot_Processor", 0x11: "Query_Destroy",
    0x12: "Debug_Query", 0x13: "Begin_PM_App", 0x14: "End_PM_App", 0x15: "Device_Reboot_Notify",
    0x16: "Crit_Reboot_Notify", 0x17: "Close_VM_Notify", 0x18: "Power_Event",
    0x19: "Sys_Dynamic_Device_Init", 0x1A: "Sys_Dynamic_Device_Exit", 0x1B: "Create_Thread",
    0x1C: "Destroy_Thread", 0x1D: "Thread_Init", 0x1E: "Terminate_Thread", 0x1F: "Get_Contention_Handler",
    0x20: "Sys_VM_Terminate2", 0x21: "System_Exit", 0x22: "Sys_Critical_Exit",
    0x23: "PnP_New_Devnode", 0x24: "W32_DeviceIoControl", 0x25: "Sys_Dynamic_Device_Init2",
    0x40: "PNP_New_Devnode",
}


def _plausible_name(b):
    """True if 8 bytes look like a space-padded VxD device name (uppercase/digit/underscore/space)."""
    if not b or b[0] in (0, 0x20):
        return False
    s = 0
    for c in b:
        if 65 <= c <= 90 or 48 <= c <= 57 or c in (0x20, 0x5F, 0x24):    # A-Z 0-9 _ $ space
            s += 1
        else:
            return False
    return s == 8


def find_ddb(data):
    """(offset, ddb_dict) for the VxD Device Descriptor Block, or None. Signature: SDK version + name."""
    for p in range(0, len(data) - 56, 4):
        ver = struct.unpack_from("<H", data, p + 4)[0]
        if not (0x0300 <= ver <= 0x040F):                    # DDB_SDK_Version (VxD SDK 3.0-4.x)
            continue
        name = data[p + 12:p + 20]
        if not _plausible_name(name):
            continue
        dev_id = struct.unpack_from("<H", data, p + 6)[0]
        maj, minor = data[p + 8], data[p + 9]
        ctrl, v86, pm = struct.unpack_from("<III", data, p + 24)
        svc_tab, svc_sz = struct.unpack_from("<II", data, p + 48)
        return p, {"name": name.decode("latin1").rstrip(), "device_id": dev_id,
                   "version": "%d.%02d" % (maj, minor), "sdk_version": "%d.%02d" % (ver >> 8, ver & 0xFF),
                   "control_proc": ctrl, "v86_api": v86, "pm_api": pm,
                   "service_table": svc_tab, "service_count": svc_sz}
    return None


def control_messages_in(data):
    """The set of named system-control messages the code tests EAX against (cmp eax, imm)."""
    found = set()
    # cmp eax, imm32  (3D xx xx xx xx)  and  cmp eax, imm8  (83 F8 xx)
    for m in re.finditer(rb"\x3d(....)", data, re.S):
        v = struct.unpack("<I", m.group(1))[0]
        if v in _SYSCTL:
            found.add(v)
    for m in re.finditer(rb"\x83\xf8(.)", data, re.S):
        v = m.group(1)[0]
        if v in _SYSCTL:
            found.add(v)
    # require several distinct known messages -> a real control-proc dispatch (else noise)
    return {v: _SYSCTL[v] for v in found} if len(found) >= 3 else {}


def reconstruct(path):
    """Recover a VxD's DDB identity + control-message handlers. {'ok','ddb','control_messages'}."""
    try:
        with open(path, "rb") as f:
            data = f.read()
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    ddb = find_ddb(data)
    if not ddb:
        return {"ok": False, "reason": "no Device Descriptor Block found (not a VxD, or packed)"}
    off, info = ddb
    msgs = control_messages_in(data)
    decl = ("Declare_Virtual_Device %s, %s, Control_Proc=0x%08X%s"
            % (info["name"], info["version"], info["control_proc"],
               (", API=0x%08X" % info["pm_api"]) if info["pm_api"] else ""))
    return {"ok": True, "ddb_offset": off, "ddb": info, "declaration": decl,
            "control_messages": sorted(msgs.values())}
