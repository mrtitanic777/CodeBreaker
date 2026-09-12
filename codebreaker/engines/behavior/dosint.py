#!/usr/bin/env python3
r"""dosint.py -- name DOS / BIOS interrupt calls (INT 21h function, INT 10h/13h/16h/1Ah).

A DOS or real-mode binary makes system calls via software interrupts: `mov ah, 9; int 21h` is
"display string", `mov ah, 0x40; int 21h` is "write file", `int 10h`/`13h`/`16h` are BIOS video /
disk / keyboard. Disassembled, these are opaque `int 21h`. The function is selected by AH (set just
before the INT), and the AH->service mapping is the standardized, stable DOS/BIOS API. This finds
each `int <n>`, recovers the AH set before it, and names the call `DOS_WriteFile`,
`BIOS_Video_TeletypeOutput`, ... -- so a DOS binary's system calls read as source.
"""

# INT 21h (DOS) AH -> service name (the standardized DOS API; stable across DOS/Win9x DOS box)
_INT21 = {
    0x00: "Terminate", 0x01: "ReadCharEcho", 0x02: "DisplayChar", 0x05: "PrintChar",
    0x06: "DirectConsoleIO", 0x07: "DirectCharInput", 0x08: "CharInputNoEcho", 0x09: "DisplayString",
    0x0A: "BufferedInput", 0x0B: "CheckInputStatus", 0x0C: "FlushAndInput", 0x0D: "DiskReset",
    0x0E: "SelectDisk", 0x19: "GetCurrentDisk", 0x1A: "SetDTA", 0x25: "SetInterruptVector",
    0x2A: "GetDate", 0x2B: "SetDate", 0x2C: "GetTime", 0x2D: "SetTime", 0x2F: "GetDTA",
    0x30: "GetDOSVersion", 0x31: "TSR", 0x33: "CtrlBreakCheck", 0x35: "GetInterruptVector",
    0x36: "GetFreeDiskSpace", 0x39: "MkDir", 0x3A: "RmDir", 0x3B: "ChDir", 0x3C: "CreateFile",
    0x3D: "OpenFile", 0x3E: "CloseFile", 0x3F: "ReadFile", 0x40: "WriteFile", 0x41: "DeleteFile",
    0x42: "SeekFile", 0x43: "GetSetFileAttr", 0x44: "IOCTL", 0x45: "DupHandle", 0x46: "ForceDup",
    0x47: "GetCurrentDir", 0x48: "AllocateMemory", 0x49: "FreeMemory", 0x4A: "ResizeMemory",
    0x4B: "Exec", 0x4C: "TerminateWithCode", 0x4D: "GetReturnCode", 0x4E: "FindFirst",
    0x4F: "FindNext", 0x56: "RenameFile", 0x57: "GetSetFileDate", 0x59: "GetExtendedError",
    0x5A: "CreateTempFile", 0x5B: "CreateNewFile", 0x62: "GetPSP",
}
_BIOS = {
    0x10: ("Video", {0x00: "SetVideoMode", 0x01: "SetCursorShape", 0x02: "SetCursorPos",
                     0x03: "GetCursorPos", 0x05: "SelectPage", 0x06: "ScrollUp", 0x07: "ScrollDown",
                     0x08: "ReadCharAttr", 0x09: "WriteCharAttr", 0x0A: "WriteChar",
                     0x0E: "TeletypeOutput", 0x0F: "GetVideoMode", 0x13: "WriteString"}),
    0x13: ("Disk", {0x00: "ResetDisk", 0x01: "GetStatus", 0x02: "ReadSectors", 0x03: "WriteSectors",
                    0x04: "VerifySectors", 0x08: "GetDriveParams", 0x0C: "Seek", 0x15: "GetDiskType"}),
    0x16: ("Keyboard", {0x00: "ReadKey", 0x01: "CheckKey", 0x02: "GetShiftFlags",
                        0x10: "ReadKeyExtended", 0x11: "CheckKeyExtended"}),
    0x1A: ("Clock", {0x00: "GetTickCount", 0x02: "GetRTCTime", 0x04: "GetRTCDate"}),
    0x33: ("Mouse", {0x00: "MouseReset", 0x01: "ShowCursor", 0x02: "HideCursor",
                     0x03: "GetMousePos", 0x0B: "GetMouseMotion"}),
}


def _ah_before(data, pos, window=12):
    """Recover the AH value set before an INT at `pos`: `mov ah,imm` (B4) or `mov ax,imm` (B8)."""
    lo = max(0, pos - window)
    ah = None
    for k in range(pos - 2, lo - 1, -1):                     # scan backward for the AH setter
        if data[k] == 0xB4 and k + 1 < pos:                  # mov ah, imm8
            return data[k + 1]
        if data[k] == 0xB8 and k + 2 < pos:                  # mov ax, imm16 -> ah = high byte
            return data[k + 2]
    return ah


def name_int(int_no, ah):
    """Name an interrupt call from its number and AH, or None."""
    if int_no == 0x21:
        return "DOS_%s" % _INT21[ah] if ah in _INT21 else ("DOS_Func_%02Xh" % ah if ah is not None else "DOS_call")
    if int_no in _BIOS:
        grp, tbl = _BIOS[int_no]
        if ah in tbl:
            return "BIOS_%s_%s" % (grp, tbl[ah])
        return "BIOS_%s_Func_%02Xh" % (grp, ah) if ah is not None else "BIOS_%s" % grp
    return None


def scan_bytes(data):
    """[(offset, int_no, ah, name)] for every recognized DOS/BIOS interrupt call."""
    out = []
    i = 0
    n = len(data)
    while i + 1 < n:
        if data[i] == 0xCD:                                  # INT imm8
            int_no = data[i + 1]
            if int_no == 0x21 or int_no in _BIOS:
                ah = _ah_before(data, i)
                nm = name_int(int_no, ah)
                if nm:
                    out.append((i, int_no, ah, nm))
            i += 2
        else:
            i += 1
    return out


def scan(path):
    with open(path, "rb") as f:
        return scan_bytes(f.read())


def reconstruct(path):
    """Summarize a DOS/real-mode binary's named interrupt calls. {'ok','ncalls','services'}."""
    try:
        calls = scan(path)
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    if not calls:
        return {"ok": False, "reason": "no DOS/BIOS interrupt calls"}
    services = {}
    for _o, _n, _ah, nm in calls:
        services[nm] = services.get(nm, 0) + 1
    return {"ok": True, "ncalls": len(calls),
            "services": dict(sorted(services.items(), key=lambda kv: -kv[1]))}
