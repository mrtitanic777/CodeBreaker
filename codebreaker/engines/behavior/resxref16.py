#!/usr/bin/env python3
r"""resxref16.py -- cross-reference recovered 16-bit CODE with recovered 16-bit RESOURCES.

neresources recovers a Win16 app's menus/dialogs/strings/accelerators; interproc16 recovers its
code. This weaves them together -- the synthesis of both halves:
  1. WM_COMMAND / control-ID dispatch  -> the menu / accelerator item each `case` implements
  2. LoadString(id)                    -> the actual string from the STRINGTABLE
  3. GetDlgItem(id)                    -> the dialog control it fetches
  4. DialogBox(templateId, .., dlgproc)-> links a dialog resource to the DlgProc that runs it
The IDs are ground truth (from neresources); the ID references in code are found by the same
conservative byte-scan used for WM_ dispatch (a `cmp`/`push imm16` whose value matches a known
resource id), so no dependence on drifty 16-bit disassembly.
"""
import struct

_STR_APIS = {"LOADSTRING"}
_DLGITEM_APIS = {"GETDLGITEM", "GETDLGITEMTEXT", "SETDLGITEMTEXT", "GETDLGITEMINT",
                 "SENDDLGITEMMESSAGE", "CHECKDLGBUTTON", "ISDLGBUTTONCHECKED"}
_DLGBOX_APIS = {"DIALOGBOX", "DIALOGBOXPARAM", "CREATEDIALOG", "CREATEDIALOGPARAM"}


def _pushed_ids(code, call_off, id_set, window=40):
    """Known resource ids pushed (68 imm16) in the `window` bytes before a call at call_off."""
    out = []
    lo = max(0, call_off - window)
    i = lo
    while i + 3 <= call_off:
        if code[i] == 0x68:                                  # push imm16
            v = code[i + 1] | (code[i + 2] << 8)
            if v in id_set:
                out.append(v)
            i += 3
        else:
            i += 1
    return out


def _cmp_ids(code, id_set):
    """Known command ids the code compares against (cmp ax,imm16 / cmp r16,imm16)."""
    out, n, i = set(), len(code), 0
    while i + 3 <= n:
        if code[i] == 0x3D:
            v = code[i + 1] | (code[i + 2] << 8)
            if v in id_set:
                out.add(v)
            i += 3
        elif code[i] == 0x81 and 0xF8 <= code[i + 1] <= 0xFF and i + 4 <= n:
            v = code[i + 2] | (code[i + 3] << 8)
            if v in id_set:
                out.add(v)
            i += 4
        else:
            i += 1
    return out


def correlate(module, path=None):
    """{'ok','commands','strings','controls','dialogs'} linking code IDs to resource meanings."""
    from codebreaker.formats.ne import NEFile
    from codebreaker.core import cores
    from codebreaker.engines.recon import neresources
    from codebreaker.engines.behavior.interproc16 import _namer
    p = path or (module if str(module).lower().endswith((".exe", ".dll", ".drv"))
                 else cores.get("target").resolve(module))
    try:
        nef = NEFile(p)
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    menu, accel = neresources.menu_ids(nef), neresources.accel_ids(nef)
    strings, controls = neresources.string_table(nef), neresources.dialog_ctl_ids(nef)
    cmd_ids = {**menu, **accel}
    name = _namer(nef)
    out = {"commands": {}, "strings": {}, "controls": {}, "dialogs": {}}
    for s in nef.segments():
        if s["kind"] != "CODE":
            continue
        code = nef.data[s["file_off"]:s["file_off"] + s["length"]]
        # 1) WM_COMMAND / control-id dispatch -> menu/accel item
        for cid in _cmp_ids(code, cmd_ids):
            out["commands"][cid] = cmd_ids[cid]
        # 2/3/4) resolve an id pushed just before a LoadString / GetDlgItem / DialogBox call
        for r in nef.relocations(s):
            nm = name(r)
            if not nm:
                continue
            api = nm.split(".")[-1].upper()
            off = r["offset"]
            if api in _STR_APIS:
                for pid in _pushed_ids(code, off, strings):
                    out["strings"][pid] = strings[pid]
            elif api in _DLGITEM_APIS:
                for pid in _pushed_ids(code, off, controls):
                    out["controls"][pid] = controls[pid]
            elif api in _DLGBOX_APIS:
                # a dialog template id pushed before DialogBox -> that dialog has a handler here
                dlg_ids = {int(rr["id"][1:]) for rr in nef.resources()
                           if rr["type"] == "DIALOG" and str(rr["id"])[1:].isdigit()}
                for pid in _pushed_ids(code, off, dlg_ids):
                    out["dialogs"].setdefault(pid, "DIALOG #%d has a DlgProc handler" % pid)
    if not any(out.values()):
        return {"ok": False, "reason": "no resource<->code references found"}
    out["ok"] = True
    return out


def report(module, path=None):
    c = correlate(module, path)
    if not c.get("ok"):
        return c.get("reason", "no correlations")
    lines = ["// resource <-> code correlation -- %s" % module]
    if c["commands"]:
        lines.append("// WM_COMMAND handlers:")
        for cid, txt in sorted(c["commands"].items()):
            lines.append('//   case 0x%X: // "%s"' % (cid, txt))
    if c["strings"]:
        lines.append("// LoadString ids:")
        for sid, txt in sorted(c["strings"].items()):
            lines.append('//   %d = "%s"' % (sid, txt))
    if c["controls"]:
        lines.append("// dialog controls referenced:")
        for cid, txt in sorted(c["controls"].items()):
            lines.append('//   id %d = "%s"' % (cid, txt))
    if c["dialogs"]:
        lines.append("// dialogs with handlers: %s" % ", ".join("#%d" % d for d in sorted(c["dialogs"])))
    return "\n".join(lines)
