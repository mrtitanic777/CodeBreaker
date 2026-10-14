"""dispatch.py -- how code chooses what to do: jump tables, compare chains, message and command
dispatch, keyword dispatch, and the system services a function asks for by number.

    from codebreaker.analyze import image, dispatch
    img = image.load("user.exe")
    dispatch.switches(img)        [{function, on, cases: {value: target}, default, table}]
    dispatch.messages(img)        window/dialog procedures: {message name: handler address}
    dispatch.keywords(img)        functions comparing a string against a list of literals
    dispatch.interrupts(img)      DOS, BIOS and DPMI services requested with `int`
    dispatch.control_messages(img)   a driver's control procedure: which system messages it handles

A switch reaches the binary in three shapes, all read here into one {case value: where it goes}:

    a jump table        `cmp eax, 7 / ja default / jmp [table + eax*4]`, with or without a byte
                        table in front and a `sub` that moves the first case to zero
    a compare chain     `cmp eax, 0x111 / je ...  cmp eax, 0x110 / je ...`
    a subtract chain    `sub eax, 5 / je ...  dec eax / je ...  sub eax, 0x10 / je ...`  (cases 5, 6, 0x16)

`on` says what is switched on when that is in sight: an argument, a global, a field. A switch
on the message argument of a window or dialog procedure (the second argument; a 16-bit procedure
takes hwnd, message, wParam, lParam at bp+0Eh, 0Ch, 0Ah, 6) is a message dispatch and its cases
get their WM_ names; so is a switch whose cases are nearly all message numbers, several of them
above the small integers and one of them among the handful every window handles. A switch on wParam inside one is the command dispatch: its cases
are menu and control identifiers.
"""
from codebreaker.analyze import call_graph, conventions, globals as global_variables, image, strings
from codebreaker.analyze.image import family
from codebreaker.decompile import describe

MESSAGES = {
    0x0001: "WM_CREATE", 0x0002: "WM_DESTROY", 0x0003: "WM_MOVE", 0x0005: "WM_SIZE", 0x0006: "WM_ACTIVATE",
    0x0007: "WM_SETFOCUS", 0x0008: "WM_KILLFOCUS", 0x000A: "WM_ENABLE", 0x000B: "WM_SETREDRAW",
    0x000C: "WM_SETTEXT", 0x000D: "WM_GETTEXT", 0x000E: "WM_GETTEXTLENGTH", 0x000F: "WM_PAINT",
    0x0010: "WM_CLOSE", 0x0011: "WM_QUERYENDSESSION", 0x0012: "WM_QUIT", 0x0013: "WM_QUERYOPEN",
    0x0014: "WM_ERASEBKGND", 0x0015: "WM_SYSCOLORCHANGE", 0x0016: "WM_ENDSESSION", 0x0018: "WM_SHOWWINDOW",
    0x0019: "WM_CTLCOLOR", 0x001A: "WM_WININICHANGE", 0x001B: "WM_DEVMODECHANGE", 0x001C: "WM_ACTIVATEAPP",
    0x001D: "WM_FONTCHANGE", 0x001E: "WM_TIMECHANGE", 0x001F: "WM_CANCELMODE", 0x0020: "WM_SETCURSOR",
    0x0021: "WM_MOUSEACTIVATE", 0x0022: "WM_CHILDACTIVATE", 0x0024: "WM_GETMINMAXINFO", 0x0026: "WM_PAINTICON",
    0x0027: "WM_ICONERASEBKGND", 0x0028: "WM_NEXTDLGCTL", 0x002B: "WM_DRAWITEM", 0x002C: "WM_MEASUREITEM",
    0x002D: "WM_DELETEITEM", 0x002E: "WM_VKEYTOITEM", 0x002F: "WM_CHARTOITEM", 0x0030: "WM_SETFONT",
    0x0031: "WM_GETFONT", 0x0037: "WM_QUERYDRAGICON", 0x0039: "WM_COMPAREITEM", 0x0041: "WM_COMPACTING",
    0x0046: "WM_WINDOWPOSCHANGING", 0x0047: "WM_WINDOWPOSCHANGED", 0x004A: "WM_COPYDATA", 0x004E: "WM_NOTIFY",
    0x0053: "WM_HELP", 0x0055: "WM_NOTIFYFORMAT", 0x007B: "WM_CONTEXTMENU", 0x007C: "WM_STYLECHANGING",
    0x007D: "WM_STYLECHANGED", 0x007E: "WM_DISPLAYCHANGE", 0x007F: "WM_GETICON", 0x0080: "WM_SETICON",
    0x0081: "WM_NCCREATE", 0x0082: "WM_NCDESTROY", 0x0083: "WM_NCCALCSIZE", 0x0084: "WM_NCHITTEST",
    0x0085: "WM_NCPAINT", 0x0086: "WM_NCACTIVATE", 0x0087: "WM_GETDLGCODE", 0x00A0: "WM_NCMOUSEMOVE",
    0x00A1: "WM_NCLBUTTONDOWN", 0x00A2: "WM_NCLBUTTONUP", 0x00A3: "WM_NCLBUTTONDBLCLK", 0x00A4: "WM_NCRBUTTONDOWN",
    0x0100: "WM_KEYDOWN", 0x0101: "WM_KEYUP", 0x0102: "WM_CHAR", 0x0103: "WM_DEADCHAR", 0x0104: "WM_SYSKEYDOWN",
    0x0105: "WM_SYSKEYUP", 0x0106: "WM_SYSCHAR", 0x0110: "WM_INITDIALOG", 0x0111: "WM_COMMAND",
    0x0112: "WM_SYSCOMMAND", 0x0113: "WM_TIMER", 0x0114: "WM_HSCROLL", 0x0115: "WM_VSCROLL", 0x0116: "WM_INITMENU",
    0x0117: "WM_INITMENUPOPUP", 0x011F: "WM_MENUSELECT", 0x0120: "WM_MENUCHAR", 0x0121: "WM_ENTERIDLE",
    0x0132: "WM_CTLCOLORMSGBOX", 0x0133: "WM_CTLCOLOREDIT", 0x0134: "WM_CTLCOLORLISTBOX", 0x0135: "WM_CTLCOLORBTN",
    0x0136: "WM_CTLCOLORDLG", 0x0137: "WM_CTLCOLORSCROLLBAR", 0x0138: "WM_CTLCOLORSTATIC",
    0x0200: "WM_MOUSEMOVE", 0x0201: "WM_LBUTTONDOWN", 0x0202: "WM_LBUTTONUP", 0x0203: "WM_LBUTTONDBLCLK",
    0x0204: "WM_RBUTTONDOWN", 0x0205: "WM_RBUTTONUP", 0x0206: "WM_RBUTTONDBLCLK", 0x0207: "WM_MBUTTONDOWN",
    0x0208: "WM_MBUTTONUP", 0x0209: "WM_MBUTTONDBLCLK", 0x0210: "WM_PARENTNOTIFY", 0x0211: "WM_ENTERMENULOOP",
    0x0212: "WM_EXITMENULOOP", 0x0214: "WM_SIZING", 0x0215: "WM_CAPTURECHANGED", 0x0216: "WM_MOVING",
    0x0219: "WM_DEVICECHANGE", 0x0220: "WM_MDICREATE", 0x0221: "WM_MDIDESTROY", 0x0222: "WM_MDIACTIVATE",
    0x0231: "WM_ENTERSIZEMOVE", 0x0232: "WM_EXITSIZEMOVE", 0x0233: "WM_DROPFILES", 0x0300: "WM_CUT",
    0x0301: "WM_COPY", 0x0302: "WM_PASTE", 0x0303: "WM_CLEAR", 0x0304: "WM_UNDO", 0x0307: "WM_DESTROYCLIPBOARD",
    0x0308: "WM_DRAWCLIPBOARD", 0x030D: "WM_CHANGECBCHAIN", 0x030F: "WM_QUERYNEWPALETTE",
    0x0311: "WM_PALETTECHANGED", 0x0312: "WM_HOTKEY", 0x0317: "WM_PRINT", 0x0318: "WM_PRINTCLIENT",
}
_CORE = (0x000F, 0x0010, 0x0081, 0x0082, 0x0110, 0x0111, 0x0112)
CONTROL_MESSAGES = {
    0x00: "Sys_Critical_Init", 0x01: "Device_Init", 0x02: "Init_Complete", 0x03: "Sys_VM_Init",
    0x04: "Sys_VM_Terminate", 0x05: "System_Exit", 0x06: "Sys_Critical_Exit", 0x07: "Create_VM",
    0x08: "VM_Critical_Init", 0x09: "VM_Init", 0x0A: "VM_Terminate", 0x0B: "VM_Not_Executeable",
    0x0C: "Destroy_VM", 0x0D: "VM_Suspend", 0x0E: "VM_Resume", 0x0F: "Set_Device_Focus",
    0x10: "Begin_Message_Mode", 0x11: "End_Message_Mode", 0x12: "Reboot_Processor", 0x13: "Query_Destroy",
    0x14: "Debug_Query", 0x15: "Begin_PM_App", 0x16: "End_PM_App", 0x17: "Device_Reboot_Notify",
    0x18: "Crit_Reboot_Notify", 0x19: "Close_VM_Notify", 0x1A: "Power_Event", 0x1B: "Sys_Dynamic_Device_Init",
    0x1C: "Sys_Dynamic_Device_Exit", 0x1D: "Create_Thread", 0x1E: "Thread_Init", 0x1F: "Terminate_Thread",
    0x20: "Thread_Not_Executeable", 0x21: "Destroy_Thread", 0x22: "PNP_New_Devnode", 0x23: "W32_DeviceIoControl",
    0x24: "Sys_VM_Terminate2", 0x25: "System_Exit2", 0x26: "Sys_Critical_Exit2", 0x27: "VM_Terminate2",
    0x28: "VM_Not_Executeable2", 0x29: "Destroy_VM2", 0x2A: "VM_Suspend2", 0x2B: "End_Message_Mode2",
    0x2C: "End_PM_App2", 0x2D: "Device_Reboot_Notify2", 0x2E: "Crit_Reboot_Notify2", 0x2F: "Close_VM_Notify2",
    0x30: "Get_Contention_Handler", 0x31: "Kernel32_Initialized", 0x32: "Kernel32_Shutdown",
}
_DOS = {0x00: "terminate", 0x02: "display character", 0x09: "display string", 0x0E: "select disk",
        0x19: "get current disk", 0x1A: "set transfer address", 0x25: "set interrupt vector", 0x2A: "get date",
        0x2C: "get time", 0x2F: "get transfer address", 0x30: "get DOS version", 0x33: "break and boot drive",
        0x35: "get interrupt vector", 0x36: "get free disk space", 0x38: "country information",
        0x39: "create directory", 0x3A: "remove directory", 0x3B: "set current directory", 0x3C: "create file",
        0x3D: "open file", 0x3E: "close file", 0x3F: "read file", 0x40: "write file", 0x41: "delete file",
        0x42: "seek", 0x43: "file attributes", 0x44: "device control", 0x45: "duplicate handle",
        0x46: "redirect handle", 0x47: "get current directory", 0x48: "allocate memory", 0x49: "free memory",
        0x4A: "resize memory", 0x4B: "execute program", 0x4C: "exit", 0x4D: "get exit code", 0x4E: "find first file",
        0x4F: "find next file", 0x50: "set PSP", 0x51: "get PSP", 0x52: "get list of lists", 0x56: "rename file",
        0x57: "file date and time", 0x59: "extended error", 0x5A: "create temporary file", 0x5B: "create new file",
        0x5C: "lock file region", 0x5E: "network machine name", 0x5F: "network redirection", 0x60: "canonical path",
        0x62: "get PSP", 0x65: "extended country information", 0x67: "set handle count", 0x68: "commit file",
        0x6C: "extended open", 0x71: "long file name function", 0x73: "FAT32 function"}
_BIOS = {0x10: "video", 0x11: "equipment list", 0x12: "memory size", 0x13: "disk", 0x14: "serial port",
         0x15: "system services", 0x16: "keyboard", 0x17: "printer", 0x1A: "clock", 0x20: "DOS terminate",
         0x25: "absolute disk read", 0x26: "absolute disk write", 0x2A: "network", 0x2F: "multiplex",
         0x31: "DPMI", 0x33: "mouse", 0x3F: "overlay manager", 0x41: "debugger hook", 0x67: "expanded memory"}
_DPMI = {0x0000: "allocate descriptors", 0x0001: "free descriptor", 0x0002: "segment to descriptor",
         0x0006: "get segment base", 0x0007: "set segment base", 0x0008: "set segment limit",
         0x0009: "set descriptor access rights", 0x000A: "create alias descriptor", 0x000B: "get descriptor",
         0x000C: "set descriptor", 0x0100: "allocate DOS memory", 0x0101: "free DOS memory",
         0x0200: "get real-mode vector", 0x0201: "set real-mode vector", 0x0202: "get exception handler",
         0x0203: "set exception handler", 0x0204: "get protected-mode vector", 0x0205: "set protected-mode vector",
         0x0300: "simulate real-mode interrupt", 0x0301: "call real-mode procedure", 0x0302: "call real-mode interrupt procedure",
         0x0303: "allocate real-mode callback", 0x0304: "free real-mode callback", 0x0400: "get version",
         0x0500: "get free memory", 0x0501: "allocate memory", 0x0502: "free memory", 0x0503: "resize memory",
         0x0600: "lock region", 0x0601: "unlock region", 0x0800: "map physical address"}
_COMPARERS = ("lstrcmp", "lstrcmpi", "strcmp", "_stricmp", "stricmp", "_strcmpi", "strcmpi", "strncmp", "_strnicmp",
              "strnicmp", "wcscmp", "_wcsicmp", "wcsicmp", "_mbscmp", "_mbsicmp", "comparestring")


def _origin(fn, i, op):
    """What an operand of fn.insns[i] really is: the place a register was loaded from when that
    is in sight. -> a hashable key and a text."""
    if op[0] == "r":
        d = image.definition(fn, i, op[1])
        if d and d[0] == "m":
            op = d
        else:
            return ("r", family(op[1])), family(op[1])
    base = family(op[1]) if op[1] else None
    if op[2] is not None:
        return ("m", base, family(op[2]), op[4]), "[%s+%s*%d%+#x]" % (base, family(op[2]), op[3], op[4])
    return ("m", base, op[4]), ("[%s%+#x]" % (base, op[4])) if base else "[%#x]" % op[4]


def _chains(img, fn):
    """{key: {"on": text, "cases": {value: target}}} from the compare and subtract chains of one function."""
    out = {}
    labels = {i.target for i in fn.insns if i.flow in ("jcc", "jmp") and i.target is not None}
    acc = {}                                    # register -> (key, text, amount subtracted so far)
    pending = None
    for i, ins in enumerate(fn.insns):
        mn, ops = ins.mn, ins.ops
        if ins.addr in labels:
            acc.clear()
        if ins.flow == "jcc":
            if pending and mn in ("je", "jz", "jne", "jnz") and ins.target is not None:
                key, text, value = pending
                slot = out.setdefault(key, {"on": text, "cases": {}})
                slot["cases"].setdefault(value, ins.target if mn in ("je", "jz") else ins.addr + ins.size)
            continue                            # flags survive a conditional jump
        pending = None
        if mn == "cmp" and len(ops) == 2 and ops[1][0] == "i" and ops[0][0] in ("r", "m"):
            if ops[0][0] == "r" and family(ops[0][1]) in acc:
                key, text, base = acc[family(ops[0][1])]
            else:
                (key, text), base = _origin(fn, i, ops[0]), 0
            pending = (key, text, (ops[1][1] + base) & 0xFFFFFFFF)
        elif mn in ("sub", "dec", "add", "inc") and ops and ops[0][0] == "r" and (len(ops) == 1 or ops[1][0] == "i"):
            fam = family(ops[0][1])
            if fam in ("esp", "ebp"):
                continue
            if fam not in acc:
                key, text = _origin(fn, i, ops[0])
                acc[fam] = (key, text, 0)
            key, text, base = acc[fam]
            step = 1 if mn == "dec" else -1 if mn == "inc" else \
                ops[1][1] if mn == "sub" else -image.signed(ops[1][1], ops[1][2])
            acc[fam] = (key, text, base + step)
            pending = (key, text, (base + step) & 0xFFFFFFFF)
        elif mn in ("test", "or", "and") and len(ops) == 2 and ops[0] == ops[1] and ops[0][0] == "r":
            fam = family(ops[0][1])
            if fam in acc:
                pending = acc[fam]
            else:
                key, text = _origin(fn, i, ops[0])
                pending = (key, text, 0)
        else:
            for r in ins.wr:
                acc.pop(r, None)
    return out


def _switches(img):
    out = []
    for fn in img.each():
        chains = _chains(img, fn)
        tabled = set()
        for t in fn.tables:
            i = fn.at(t["site"])
            key, text = None, t["selector"]
            for j in range(i - 1, max(-1, i - 10), -1):
                b = fn.insns[j]
                if b.mn == "cmp" and b.ops[1][0] == "i":
                    key, text = _origin(fn, j, b.ops[0])
                    for p in range(j - 1, max(-1, j - 4), -1):          # past the `sub reg, low`
                        q = fn.insns[p]
                        if q.mn in ("sub", "dec", "add", "lea") and q.ops and q.ops[0][0] == "r" and b.ops[0][0] == "r" \
                                and family(q.ops[0][1]) == family(b.ops[0][1]):
                            key, text = _origin(fn, p, q.ops[0])
                    break
            cases = {}
            if t["bytes"] is not None:
                for n, slot in enumerate(t["bytes"]):
                    if slot < len(t["targets"]) and t["targets"][slot] != t["default"]:
                        cases[t["low"] + n] = t["targets"][slot]
            else:
                cases = {t["low"] + n: tg for n, tg in enumerate(t["targets"])}
            tabled.add(key)
            merged = dict(chains.get(key, {}).get("cases", {})) if key in chains else {}
            merged.update(cases)
            out.append({"function": fn.start, "on": text, "key": key, "cases": merged, "default": t["default"],
                        "table": t["table"], "site": t["site"], "bounded": t["bounded"]})
        for key, c in chains.items():
            if key not in tabled and len(c["cases"]) >= 3:
                out.append({"function": fn.start, "on": c["on"], "key": key, "cases": c["cases"], "default": None,
                            "table": None, "site": None, "bounded": True})
    return out


def switches(img):
    return img.memo("dispatch.switches", _switches)


def _is_message_argument(img, key, far):
    if not key or key[0] != "m" or len(key) != 3:
        return False
    if img.bits == 16:
        return key[1] == "ebp" and key[2] == (0x0C if far else 0x0A)
    return (key[1] == "ebp" and key[2] == 0x0C) or (key[1] == "esp" and key[2] == 8)


def _is_command_argument(img, key, far):
    if not key or key[0] != "m" or len(key) != 3:
        return False
    if img.bits == 16:
        return key[1] == "ebp" and key[2] == (0x0A if far else 0x08)
    return (key[1] == "ebp" and key[2] == 0x10) or (key[1] == "esp" and key[2] == 0x0C)


def _messages(img):
    facts = conventions.functions(img)
    by_fn = {}
    for s in switches(img):
        by_fn.setdefault(s["function"], []).append(s)
    out = []
    for f, sws in sorted(by_fn.items()):
        fact = facts[f]
        far = (fact.get("convention") or "").endswith("far")
        proc = fact.get("procedure")
        found, commands, why = {}, {}, None
        for s in sws:
            values = set(s["cases"])
            known = values & set(MESSAGES)
            telling = {v for v in known if v >= 0x0F}        # 1, 2, 3 are also just small numbers
            on_message = _is_message_argument(img, s["key"], far) and (
                fact.get("removes") == (16 if img.bits == 32 else 10) or bool(telling))
            if img.kind == "le":
                continue
            if (on_message and len(known) >= 2) or (proc and len(telling) >= 2 and len(known) * 2 >= len(values)) \
                    or (len(telling) >= 3 and telling & set(_CORE) and len(known) * 4 >= len(values) * 3):
                why = why or ("switch on the message argument" if on_message else
                              "a %s" % proc if proc else "cases are message numbers")
                for v, t in s["cases"].items():
                    found[v] = t
            elif _is_command_argument(img, s["key"], far):
                commands.update(s["cases"])
        if found:
            out.append({"function": f, "procedure": proc, "why": why, "messages": found, "commands": commands})
    return out


def messages(img):
    return img.memo("dispatch.messages", _messages)


def _keywords(img):
    graph = call_graph.build(img)
    out = []
    for f in img.functions:
        fn = img.function(f)
        words = []
        for site, _kind, dest in graph.calls[f]:
            if dest not in img.imports:
                continue
            api = img.imports[dest].split("!")[-1].lower()
            api = api[:-1] if api[-1:] in "aw" and api[:-1] in _COMPARERS else api
            if api not in _COMPARERS:
                continue
            i = fn.at(site)
            args = image.arguments(fn, i, 4 if img.bits == 16 else 2)
            for k in (0, 1):
                lit = strings.literal(img, args, k)
                if lit:
                    words.append((site, lit[1], api))
                    break
        if len({w for _s, w, _a in words}) >= 2:
            out.append({"function": f, "compares_with": words[0][2], "keywords": [w for _s, w, _a in words]})
    # tables in the data that pair a string with a function
    text = strings.table(img)
    for t in global_variables.pointer_tables(img):
        if t["shape"] == "run" and t["count"] >= 4 and t["count"] % 2 == 0 \
                and all(t["kinds"][k] == ("string" if k % 2 == 0 else "function") for k in range(t["count"])):
            out.append({"function": None, "table": t["at"],
                        "keywords": [text[t["targets"][k]][0] for k in range(0, t["count"], 2)],
                        "handlers": [t["targets"][k] for k in range(1, t["count"], 2)]})
    return out


def keywords(img):
    return img.memo("dispatch.keywords", _keywords)


def interrupts(img):
    """[(function, site, interrupt, function number or None, name)] for every `int` instruction."""
    out = []
    for fn in img.each():
        for i, ins in enumerate(fn.insns):
            if ins.mn != "int" or not ins.ops or ins.ops[0][0] != "i":
                continue
            n = ins.ops[0][1] & 0xFF
            if img.kind == "le" and n == 0x20:
                continue                                # a driver's service call, already an import
            code = None
            for b in reversed(fn.insns[max(0, i - 8):i]):
                if b.flow or b.mn == "int":
                    break
                if b.mn == "mov" and len(b.ops) == 2 and b.ops[0][0] == "r" and b.ops[1][0] == "i":
                    if b.ops[0][1] == "ah":
                        code = b.ops[1][1] if n != 0x31 and n != 0x2F else None
                        break
                    if b.ops[0][1] in ("ax", "eax"):
                        code = b.ops[1][1] & 0xFFFF if n in (0x31, 0x2F) else (b.ops[1][1] >> 8) & 0xFF
                        break
            if n == 0x21:
                name = "DOS: " + _DOS.get(code, "function %s" % ("%02Xh" % code if code is not None else "?"))
            elif n == 0x31:
                name = "DPMI: " + _DPMI.get(code, "function %s" % ("%04Xh" % code if code is not None else "?"))
            else:
                name = _BIOS.get(n, "interrupt %02Xh" % n) + (" function %02Xh" % code if code is not None else "")
            out.append((fn.start, ins.addr, n, code, name))
    return out


def control_messages(img):
    """A driver's control procedure: {message name: handler address}, or None."""
    if img.kind != "le" or not img.device:
        return None
    proc = img.u32(describe.le_address(img.device["obj"], img.device["rva"]) + 0x18)
    if proc not in img.names:
        return None
    best = {}
    for s in switches(img):
        if s["function"] == proc and s["key"] == ("r", "eax"):
            best.update(s["cases"])
    fn = img.function(proc)                         # short procedures test one or two messages only
    for c in _chains(img, fn).values():
        if c["on"] == "eax":
            for v, t in c["cases"].items():
                best.setdefault(v, t)
    return {"procedure": proc, "handles": {CONTROL_MESSAGES.get(v, "message %#x" % v): t for v, t in sorted(best.items())}}


def analyse(img):
    sw, msgs, kw, ints = switches(img), messages(img), keywords(img), interrupts(img)
    ctl = control_messages(img)
    facts = []
    for s in sw:
        facts.append({"addr": s["function"], "kind": "switch", "on": s["on"], "site": s["site"], "table": s["table"],
                      "default": s["default"], "cases": {("%#x" % v): t for v, t in sorted(s["cases"].items())}})
    for m in msgs:
        facts.append({"addr": m["function"], "kind": "message_dispatch", "procedure": m["procedure"], "why": m["why"],
                      "messages": {MESSAGES.get(v, "%#06x" % v): t for v, t in sorted(m["messages"].items())},
                      "commands": {("%d" % v): t for v, t in sorted(m["commands"].items())}})
    for k in kw:
        facts.append(dict(k, addr=k["function"], kind="keyword_dispatch"))
    by_fn = {}
    for f, site, n, code, name in ints:
        by_fn.setdefault(f, []).append(name)
    for f, names in sorted(by_fn.items()):
        facts.append({"addr": f, "kind": "system_services", "services": sorted(set(names))})
    if ctl:
        facts.append({"addr": ctl["procedure"], "kind": "driver_control", "device": img.device.get("name"),
                      "handles": ctl["handles"]})
    tables = [s for s in sw if s["table"] is not None]
    counts = {"jump_tables": len(tables), "jump_table_cases": sum(len(s["cases"]) for s in tables),
              "jump_tables_with_bound": sum(1 for s in tables if s["bounded"]),
              "compare_chains": len(sw) - len(tables),
              "compare_chain_cases": sum(len(s["cases"]) for s in sw if s["table"] is None),
              "switches_on_an_argument": sum(1 for s in sw if s["key"] and s["key"][0] == "m" and s["key"][1] in ("ebp", "esp") and s["key"][-1] > 0),
              "message_dispatchers": len(msgs), "messages_handled": sum(len(m["messages"]) for m in msgs),
              "named_messages": sum(1 for m in msgs for v in m["messages"] if v in MESSAGES),
              "command_dispatchers": sum(1 for m in msgs if m["commands"]),
              "command_ids": sum(len(m["commands"]) for m in msgs),
              "keyword_dispatchers": len(kw), "keywords": sum(len(k["keywords"]) for k in kw),
              "interrupt_sites": len(ints), "named_services": sum(1 for x in ints if "function ?" not in x[4] and "interrupt " not in x[4]),
              "driver_control_messages": len(ctl["handles"]) if ctl else 0}
    text = []
    for m in msgs[:6]:
        text.append("%s (%s): %s" % (img.label(m["function"]), m["why"],
                                     ", ".join(MESSAGES.get(v, "%#x" % v) for v in sorted(m["messages"])[:10])))
    for k in kw[:4]:
        text.append("keywords in %s: %s" % (img.label(k["function"]) if k["function"] else "table %x" % k["table"],
                                            ", ".join(repr(w) for w in k["keywords"][:8])))
    if ctl:
        text.append("device %s control procedure %s handles: %s" % (img.device.get("name"), img.label(ctl["procedure"]),
                                                                     ", ".join(ctl["handles"])))
    if ints:
        seen = {}
        for x in ints:
            seen[x[4]] = seen.get(x[4], 0) + 1
        text.append("system services: " + ", ".join("%s x%d" % kv for kv in sorted(seen.items(), key=lambda kv: -kv[1])[:10]))
    return {"counts": counts, "facts": facts, "text": text}
