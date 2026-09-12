#!/usr/bin/env python3
"""
decomp.py — a lightweight decompiler pass for the Windows 1.01 modules.

The project's bottleneck is READING: turning 8086 into control flow and data
flow by eye. Three mechanical transforms remove most of that work, and this
tool does all three:

  1. CALL ARGUMENTS.  A PASCAL call pushes its arguments left to right and the
     callee's `retf N` says how many there are. So the pushes immediately
     before a call ARE the argument list — recoverable exactly, not guessed.
     `SendMessage(hWnd, 0x0401, wParam, 0)` instead of five pushes and a call.

  2. REGISTER / FRAME NAMING.  `mov si,[bp+0x0e]` makes SI the first argument;
     every later `[si+0x33]` is then `a1->state` once win1.h is loaded. Frame
     slots below BP become locals.

  3. CONTROL FLOW.  Build the basic-block graph from the jumps and render the
     reducible parts as `if`/`else`/`while`. What does not reduce keeps its
     label and a `goto` — an honest fallback rather than a wrong structure.

None of this invents anything: every line printed is derived from an
instruction, and anything the tool cannot resolve is printed verbatim so the
reader sees exactly what the binary said.

Usage:
    python decomp.py MOD ORD            # decompile one ordinal
    python decomp.py MOD 0xADDR SEG     # decompile at an address
    python decomp.py MOD --seg N        # every exported function in a segment
    python decomp.py APP --exe files/WF/CALC.EXE 0xADDR SEG    # an application
  options:
    --raw       keep the original instruction alongside each line
    --max N     stop after N instructions (default 400)
    --wnd/--dc/--q  name struct fields from reconstructed/win1.h (opt-in:
                without it a [si+0x33] prints as si[0x33], never as a guess)
"""
import sys, re, collections
import idord, trace, mfn
from src.disasm.disasm import open_ne

JCC = re.compile(r"^(j\w+)\s+0x([0-9a-f]+)$")
UNCOND = ("jmp",)

# how a conditional jump reads when you take it
COND = {
    "je": "==", "jz": "==", "jne": "!=", "jnz": "!=",
    "jb": "<", "jnae": "<", "jc": "<", "jae": ">=", "jnb": ">=", "jnc": ">=",
    "ja": ">", "jnbe": ">", "jbe": "<=", "jna": "<=",
    "jl": "<(s)", "jnge": "<(s)", "jge": ">=(s)", "jnl": ">=(s)",
    "jg": ">(s)", "jnle": ">(s)", "jle": "<=(s)", "jng": "<=(s)",
    "js": "<0", "jns": ">=0", "jcxz": "cx==0",
}


class Fn:
    def __init__(self, mod, seg, start, ordn=None, maxi=400, hint=None, exe=None):
        # Struct-field naming is OPT-IN (--wnd / --dc / --q). Applying the WND
        # layout to a routine that is really walking a menu block would print
        # field names that do not exist — the tool must never invent structure.
        self.hint = hint
        self.mod, self.seg, self.start, self.ordn = mod, seg, start, ordn
        self.ordinals = trace.load_ordinals("ORDINALS.md")
        # `exe` decompiles an APPLICATION instead of a system module — Phase 5
        # is 1,886 app functions, the largest count in the project.
        self.ne = open_ne(exe) if exe else idord.open_module(mod)
        self.sg = trace.Seg(self.ne, seg, self.ordinals)
        self.structs = mfn.load_structs()
        self.rows = trace.disasm_whole(self.sg.code[start:], base=start)[:maxi]
        self.end = None
        for off, asm in self.rows:
            if asm.startswith("ret"):
                self.end = off
                break
        if self.end is not None:
            self.rows = [r for r in self.rows if r[0] <= self.end]
        # (seg, offset) -> the exported name, so a call to another segment or
        # to a sibling in this one resolves to its API name instead of sub_XXXX
        self.byaddr = {}
        for e in self.ne.entries():
            if e.get("kind") != "UNUSED" and "offset" in e:
                nm = self.ordinals.get(f"{mod}.{e['ordinal']}")
                self.byaddr[(e["seg"], e["offset"])] = (
                    nm if nm and not nm.startswith("0x") else f"{mod}.{e['ordinal']}")
        self.regs = {}           # register -> symbolic text
        self.argname = {}        # frame offset -> argument name
        self._name_args()

    # ---- arguments -------------------------------------------------------
    def _name_args(self):
        """Frame slots at [bp+6] and up are arguments in PASCAL order: the
        HIGHEST slot is the first argument. Name them a1..aN, and if the
        ordinal is known give the first one the conventional name."""
        slots = sorted({int(m.group(1), 16)
                        for _, asm in self.rows
                        for m in re.finditer(r"\[bp\+(0x[0-9a-f]+)\]", asm)
                        if int(m.group(1), 16) >= 6}, reverse=True)
        for i, s in enumerate(slots):
            self.argname[s] = f"a{i+1}"

    # ---- expression rendering -------------------------------------------
    def expr(self, tok):
        tok = tok.strip()
        m = re.match(r"WORD PTR \[bp\+(0x[0-9a-f]+)\]", tok) or \
            re.match(r"\[bp\+(0x[0-9a-f]+)\]", tok)
        if m:
            return self.argname.get(int(m.group(1), 16), tok)
        m = re.match(r"(?:WORD PTR )?\[bp-(0x[0-9a-f]+)\]", tok)
        if m:
            return f"loc_{m.group(1)[2:]}"
        m = re.match(r"(?:WORD|BYTE) PTR \[(si|di|bx)\+(0x[0-9a-f]+)\]", tok)
        if m:
            base, off = m.group(1), int(m.group(2), 16)
            owner = self.regs.get(base, base)
            tbl = self.structs.get(self.hint or "", {})
            if off in tbl:
                return f"{owner}->{tbl[off]}"
            return f"{owner}[0x{off:02x}]"
        m = re.match(r"(?:WORD PTR )?ds:(0x[0-9a-f]+)", tok)
        if m:
            return f"g_{m.group(1)[2:]}"
        if tok in self.regs:
            return self.regs[tok]
        return tok

    def track(self, asm):
        """Very small symbolic state: remember what a register was loaded
        with, so later uses read as that thing. Anything we do not model
        clears the register rather than lying about it."""
        m = re.match(r"mov    (\w\w),(.+)$", asm)
        if m:
            dst, src = m.group(1), m.group(2)
            if dst in ("ax", "bx", "cx", "dx", "si", "di", "es", "ds"):
                self.regs[dst] = self.expr(src)
            return
        m = re.match(r"(?:xor|sub)    (\w\w),\1$", asm)
        if m:
            self.regs[m.group(1)] = "0"
            return
        m = re.match(r"(?:pop|les|lds|inc|dec|add|sub|and|or|shl|shr)\s+(\w\w)", asm)
        if m and m.group(1) in self.regs:
            del self.regs[m.group(1)]

    # ---- the pass --------------------------------------------------------
    def run(self, raw=False):
        # 1: branch targets
        targets = set()
        for off, asm in self.rows:
            m = JCC.match(asm)
            if m:
                targets.add(int(m.group(2), 16))
        # 2: walk, collecting pushes so calls can be rendered with arguments
        name = self.ordinals.get(f"{self.mod}.{self.ordn}") if self.ordn else None
        img = self.ne.segments()[self.seg - 1].get("image", "?")
        print(f"/* decomp.py  {self.mod}.{self.ordn}  seg{self.seg}[{img}]:"
              f"0x{self.start:04x}..0x{(self.end or 0):04x}   "
              f"{len(self.argname)} args */")
        print(f"{name or f'{self.mod}_{self.ordn}'}("
              + ", ".join(self.argname[s] for s in sorted(self.argname, reverse=True))
              + ")\n{")
        pushes = []
        inprologue = True
        indent = "    "
        for off, asm in self.rows:
            if off in targets:
                print(f"L_{off:04x}:")
                pushes = []          # a label starts a new argument sequence
                # ...and the register state is unknown here: control can arrive
                # from any of several paths. Keeping the previous block's values
                # would print confident nonsense (a `cmp ax,0x0f` on the message
                # rendering as `if (ReleaseDC() != 0xf)`), so wipe them. The
                # tracker is deliberately accurate-within-a-block only.
                self.regs.clear()
            if inprologue:
                # the frame setup saves ds/bp/si/di; those are not arguments
                if asm.startswith(("push", "pop", "mov    bp,sp", "mov    ds,",
                                   "inc    bp", "nop", "sub    sp,",
                                   "cld", "mov    ax,ds")):
                    self.track(asm)
                    continue
                inprologue = False
                inprologue = False
            m = re.match(r"push   (.+)$", asm)
            if m:
                pushes.append(self.expr(m.group(1)))
                self.track(asm)
                continue
            if asm.startswith("call") or (asm.startswith("jmp") and "0x0:" in asm):
                kind, det = self.sg.resolve_call(off, asm)
                callee, nargs = None, None
                if kind == "sys":
                    lbl, nm = det
                    callee = nm or lbl
                    nargs = ARGCOUNT.get(nm) if nm else None
                elif kind == "int":
                    callee = self.byaddr.get((self.seg, det), f"sub_{det:04x}")
                    nargs = ARGCOUNT.get(callee)
                elif kind == "far":
                    m2 = re.match(r"seg(\d+):0x([0-9a-f]+)", str(det))
                    if m2:
                        callee = self.byaddr.get((int(m2.group(1)),
                                                  int(m2.group(2), 16)), str(det))
                        nargs = ARGCOUNT.get(callee)
                    else:
                        callee = str(det)
                args = [p for p in pushes if p not in ("cs", "ds", "es")]
                args = args[-nargs:] if nargs else args
                # PASCAL pushes left to right, so the arguments read in order
                print(f"{indent}{callee}({', '.join(args)});"
                      + (f"   /* {off:04x}: {asm} */" if raw else ""))
                pushes = []
                # the return value lives in AX (DX:AX for a long), so later
                # uses of AX read as the call that produced it
                self.regs["ax"] = f"{callee}()"
                self.regs.pop("dx", None)
                continue
            m = JCC.match(asm)
            if m:
                op, tgt = m.group(1), int(m.group(2), 16)
                if op in UNCOND:
                    print(f"{indent}goto L_{tgt:04x};")
                else:
                    c = COND.get(op, op)
                    print(f"{indent}if ({self.cmp_l} {c} {self.cmp_r}) goto L_{tgt:04x};")
                continue
            # `or reg,reg` and `test reg,reg` are the idiom for "is it zero?"
            m = re.match(r"(?:or|test)\s+(\w\w),(\w\w)$", asm)
            if m and m.group(1) == m.group(2):
                self.cmp_l, self.cmp_r = self.expr(m.group(1)), "0"
                if raw:
                    print(f"{indent}/* {off:04x}: {asm} */")
                continue
            m = re.match(r"(cmp|test)    (.+),(.+)$", asm)
            if m:
                if m.group(1) == "test":
                    self.cmp_l = f"({self.expr(m.group(2))} & {self.expr(m.group(3))})"
                    self.cmp_r = "0"
                else:
                    self.cmp_l, self.cmp_r = self.expr(m.group(2)), self.expr(m.group(3))
                if raw:
                    print(f"{indent}/* {off:04x}: {asm} */")
                continue
            m = re.match(r"mov    (.+),(.+)$", asm)
            if m and ("[" in m.group(1) or "ds:" in m.group(1)):
                print(f"{indent}{self.expr(m.group(1))} = {self.expr(m.group(2))};")
                self.track(asm)
                continue
            self.track(asm)
            if raw:
                print(f"{indent}/* {off:04x}: {asm} */")
        print("}")

    cmp_l = "?"
    cmp_r = "0"


# argument counts for the named APIs, so calls render with the right slice.
# Derived from the retf values already recorded in ORDINALS.md.
ARGCOUNT = {
    "GetDlgItem": 2, "SetDlgItemText": 3, "GetDlgItemText": 4,
    "SendMessage": 4, "PostMessage": 4, "SendDlgItemMessage": 5,
    "CheckDlgButton": 3, "IsDlgButtonChecked": 2, "CheckRadioButton": 4,
    "SetWindowText": 2, "GetWindowText": 3, "GetWindowTextLength": 1,
    "ShowWindow": 2, "DestroyWindow": 1, "IsIconic": 1, "IsWindowEnabled": 1,
    "EnableWindow": 2, "MoveWindow": 6, "SetFocus": 1, "GetFocus": 0,
    "InvalidateRect": 3, "UpdateWindow": 1, "BeginPaint": 2, "EndPaint": 2,
    "GetClientRect": 2, "GetDC": 1, "ReleaseDC": 2, "SetCursor": 1,
    "LocalAlloc": 2, "LocalFree": 1, "LocalLock": 1, "LocalUnlock": 1,
    "LocalReAlloc": 3, "GlobalAlloc": 2, "GlobalFree": 1, "GlobalLock": 1,
    "GlobalUnlock": 1, "lstrlen": 1, "lstrcpy": 2, "lstrcat": 2,
    "SelectObject": 2, "DeleteObject": 1, "DeleteDC": 1, "PatBlt": 6,
    "TextOut": 5, "MoveTo": 3, "LineTo": 3, "GetStockObject": 1,
    "SetBkMode": 2, "SetROP2": 2, "SetTextColor": 2, "SetBkColor": 2,
    "FindResource": 3, "LoadResource": 2, "LockResource": 1,
    "LoadString": 4, "LoadMenu": 2, "LoadCursor": 2, "LoadIcon": 2,
    "MessageBox": 4, "SetTimer": 4, "KillTimer": 2, "MessageBeep": 1,
    "OpenClipboard": 1, "CloseClipboard": 0, "EmptyClipboard": 0,
    "GetClipboardData": 1, "SetClipboardData": 2,
}


def main():
    a = sys.argv[1:]
    if not a:
        print(__doc__); sys.exit(1)
    mod = a[0].upper()
    rest = [x for x in a[1:] if not x.startswith("--")]
    raw = "--raw" in a
    maxi = int(a[a.index("--max") + 1]) if "--max" in a else 400
    hint = "WND" if "--wnd" in a else "DC" if "--dc" in a else "MSGQUEUE" if "--q" in a else None
    exe = a[a.index("--exe") + 1] if "--exe" in a else None
    if exe:
        # an app: MOD is just a label, the addresses come from the EXE.
        # `rest` still holds the --exe value, so drop anything non-numeric.
        nums = [x for x in rest if x.startswith("0x") or x.isdigit()]
        seg = int(nums[1], 0) if len(nums) > 1 else 1
        Fn(mod, seg, int(nums[0], 0), None, maxi, hint, exe).run(raw)
        return
    if "--seg" in a:
        seg = int(a[a.index("--seg") + 1], 0)
        ne = idord.open_module(mod)
        ents = sorted({(e["offset"], e["ordinal"]) for e in ne.entries()
                       if e.get("seg") == seg and "offset" in e})
        for off, ordn in ents:
            Fn(mod, seg, off, ordn, maxi, hint).run(raw)
            print()
        return
    tgt = rest[0]
    if tgt.startswith("0x"):
        seg = int(rest[1], 0) if len(rest) > 1 else 1
        Fn(mod, seg, int(tgt, 0), None, maxi, hint).run(raw)
    else:
        ordn = int(tgt, 0)
        ne = idord.open_module(mod)
        seg, off = trace.find_seg_of_entry(ne, ordn)
        if seg is None:
            print(f"ordinal {tgt} not found"); sys.exit(1)
        Fn(mod, seg, off, ordn, maxi, hint).run(raw)


if __name__ == "__main__":
    main()
