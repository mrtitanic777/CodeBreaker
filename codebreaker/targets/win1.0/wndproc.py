#!/usr/bin/env python3
"""
wndproc.py — Decode a Windows 1.0 window procedure.

A WndProc is `LONG FAR PASCAL WndProc(HWND, WORD message, WORD wParam, LONG lParam)`.
The compiler turns the message switch into a chain of `cmp ax,<WM_*>` / conditional
jumps. This tool extracts that dispatch table, names each message, and for every
handler lists the KERNEL/GDI/USER calls it makes — a behavioral map of the app.

Usage: python3 wndproc.py FILE --entry ORDINAL
"""
import subprocess, sys, re
from codebreaker.disasm_engine.disasm import open_ne, build_fixup_map

WM = {
 0x0001:"WM_CREATE",0x0002:"WM_DESTROY",0x0003:"WM_MOVE",0x0005:"WM_SIZE",
 0x0006:"WM_ACTIVATE",0x0007:"WM_SETFOCUS",0x0008:"WM_KILLFOCUS",0x000A:"WM_ENABLE",
 0x000B:"WM_SETREDRAW",0x000C:"WM_SETTEXT",0x000D:"WM_GETTEXT",0x000E:"WM_GETTEXTLENGTH",
 0x000F:"WM_PAINT",0x0010:"WM_CLOSE",0x0011:"WM_QUERYENDSESSION",0x0012:"WM_QUIT",
 0x0013:"WM_QUERYOPEN",0x0014:"WM_ERASEBKGND",0x0018:"WM_SHOWWINDOW",
 0x001A:"WM_WININICHANGE",0x0020:"WM_SETCURSOR",0x0021:"WM_MOUSEACTIVATE",
 0x0024:"WM_GETMINMAXINFO",0x0081:"WM_NCCREATE",0x0083:"WM_NCCALCSIZE",
 0x0084:"WM_NCHITTEST",0x0085:"WM_NCPAINT",0x0086:"WM_NCACTIVATE",
 0x0100:"WM_KEYDOWN",0x0101:"WM_KEYUP",0x0102:"WM_CHAR",0x0104:"WM_SYSKEYDOWN",
 0x0105:"WM_SYSKEYUP",0x0106:"WM_SYSCHAR",0x0110:"WM_INITDIALOG",0x0111:"WM_COMMAND",
 0x0112:"WM_SYSCOMMAND",0x0113:"WM_TIMER",0x0114:"WM_HSCROLL",0x0115:"WM_VSCROLL",
 0x0116:"WM_INITMENU",0x0200:"WM_MOUSEMOVE",0x0201:"WM_LBUTTONDOWN",0x0202:"WM_LBUTTONUP",
 0x0203:"WM_LBUTTONDBLCLK",0x0204:"WM_RBUTTONDOWN",0x0205:"WM_RBUTTONUP",
 0x0206:"WM_RBUTTONDBLCLK",0x0207:"WM_MBUTTONDOWN",0x0208:"WM_MBUTTONUP",
 0x0300:"WM_CUT",0x0301:"WM_COPY",0x0302:"WM_PASTE",0x0303:"WM_CLEAR",0x0304:"WM_UNDO",
}

def disasm_lines(blob, vma):
    open("/tmp/_wp.bin","wb").write(blob)
    out = subprocess.run(
        ["objdump","-D","-b","binary","-mi386","-Maddr16,data16,intel",
         "--adjust-vma=%d"%vma,"/tmp/_wp.bin"],
        capture_output=True, text=True).stdout
    rows = []
    for l in out.splitlines():
        if ":\t" not in l: continue
        head, rest = l.split(":\t",1)
        try: off = int(head.strip(),16)
        except ValueError: continue
        asm = rest.split("\t")[-1].strip()
        rows.append((off, asm))
    return rows

def analyze(path, ordinal, quiet=False):
    ne = open_ne(path)
    ents = {e["ordinal"]: e for e in ne.entries()}
    e = ents[ordinal]
    seg = ne.segments()[e["seg"]-1]
    modname = ne.resident_names()[0][0]
    mods = ne.module_refs()
    fixups = build_fixup_map(ne, seg, mods)
    code = ne.data[seg["file_off"]:seg["file_off"]+seg["length"]]
    rows = disasm_lines(code[e["offset"]:], e["offset"])

    # --- parse the dispatch chain, tracking AX provenance ------------------
    # A `cmp ax,IMM` is a message dispatch only when AX currently holds the
    # `message` argument (last loaded from [bp+0c]). This cleanly rejects the
    # menu-id compares inside WM_COMMAND (those compare wParam, [bp+0a]).
    dispatch = []
    seen = set()
    ax_is_msg = False
    for i,(off,asm) in enumerate(rows):
        if re.match(r"mov    ax,WORD PTR \[bp\+0xc\]", asm):
            ax_is_msg = True; continue
        # any other load into AX (or a call) loses the message provenance
        if re.match(r"(mov    ax,|pop    ax|lea    ax|call|xor    ax,ax|sub    ax,ax|xchg .*ax)", asm):
            if not asm.startswith("cmp"): ax_is_msg = False
        m = re.match(r"cmp    ax,0x([0-9a-f]+)$", asm)
        if not m or not ax_is_msg:
            continue
        val = int(m.group(1),16)
        window = rows[i+1:i+4]
        handler = None
        for j,(o2,a2) in enumerate(window):
            jm = re.match(r"je     0x([0-9a-f]+)", a2)
            if jm: handler = int(jm.group(1),16); break
            jn = re.match(r"jne    0x([0-9a-f]+)", a2)
            if jn and j+1 < len(window):
                jm2 = re.match(r"jmp    0x([0-9a-f]+)", window[j+1][1])
                if jm2: handler = int(jm2.group(1),16)
                break
        if handler is not None and val not in seen:
            dispatch.append((val, handler)); seen.add(val)

    if not quiet:
        print(f"# {modname}  ordinal {ordinal}  ({path})")
        print(f"# WndProc at seg{e['seg']}:0x{e['offset']:x}  handles {len(dispatch)} messages\n")
        print(f"  frame: hWnd=[bp+0e]  message=[bp+0c]  wParam=[bp+0a]  lParam=[bp+06]\n")

    handler_starts = sorted(h for _,h in dispatch)
    row_at = {off:k for k,(off,_) in enumerate(rows)}
    def calls_from(h):
        """Walk a handler from its entry until the first ret/retf or a jump out
        to the shared epilogue (target outside this handler's local span)."""
        nxt_start = next((s for s in handler_starts if s > h), None)
        made = []
        k = row_at.get(h)
        if k is None: return made
        while k < len(rows):
            off, asm = rows[k]
            if off != h and off in handler_starts:  # ran into the next case
                break
            if asm.startswith("ret") or asm.startswith("retf"):
                break
            jm = re.match(r"jmp    0x([0-9a-f]+)", asm)
            if jm:
                tgt = int(jm.group(1),16)
                if tgt < h or (nxt_start and tgt >= nxt_start):
                    break     # jump to shared epilogue / out of this case
            if asm.startswith("call"):
                lbl = None
                for kk in range(off, off+6):
                    if kk in fixups: lbl = fixups[kk]; break
                if lbl: made.append(lbl)
                elif "0x0:0x" not in asm: made.append("(internal)")
            k += 1
        return made

    result = []
    for val,h in sorted(dispatch):
        name = WM.get(val, f"msg_0x{val:04x}")
        made = calls_from(h)
        seen=[]
        for c in made:
            if not seen or seen[-1]!=c: seen.append(c)
        syscalls = [c for c in seen if c!="(internal)"]
        ninternal = made.count("(internal)")
        result.append(dict(msg=val, name=name, offset=h,
                           syscalls=syscalls, internal=ninternal))
    if not quiet:
        for r in result:
            summary = ", ".join(r["syscalls"][:12]) + (" ..." if len(r["syscalls"])>12 else "")
            extra = f"  +{r['internal']} internal" if r["internal"] else ""
            print(f"  0x{r['msg']:04x} {r['name']:18} @0x{r['offset']:04x} -> {summary}{extra}")
    return dict(module=modname, mods=mods, handlers=result)

if __name__ == "__main__":
    a = sys.argv[1:]
    path = a[0]
    ordinal = int(a[a.index("--entry")+1],0) if "--entry" in a else 1
    analyze(path, ordinal)
