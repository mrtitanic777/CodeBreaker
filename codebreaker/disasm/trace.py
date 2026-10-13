#!/usr/bin/env python3
"""
trace.py — Call-graph-tracing disassembler for the Windows 1.0 RE project.

Where disasm.py dumps one segment and wndproc.py decodes one dispatch table,
trace.py automates the tedious part of reconstruction: given an exported entry,
it discovers *every* function reachable from it, carves each function's
boundaries, disassembles it, and resolves *every* call to a real name —
KERNEL/GDI/USER ordinals via ORDINALS.md, internal calls to their function
label. One command replaces the dozens of hand-run disasm.py invocations and
the manual call-graph chase that Level-2 reconstruction otherwise needs.

Function discovery combines four signals: exported entry points, internal
relocation targets (segN:0xNNNN fixups), intra-segment near-call targets, and
`push bp`/`mov bp,sp` prologue scanning. Boundaries run start-to-next-start,
trimming trailing padding.

Usage:
    python3 trace.py FILE --map                 # exports, segments, function inventory
    python3 trace.py FILE --list [seg]          # every discovered function in a segment
    python3 trace.py FILE --fn 0xADDR [seg]     # one function, annotated
    python3 trace.py FILE --trace ORD           # an export + its whole reachable call graph
    python3 trace.py FILE --trace 0xADDR [seg]  # same, from a raw address
Add --ords PATH to point at a non-default ORDINALS.md.
"""
import re, sys, os, subprocess, tempfile
from codebreaker.disasm.disasm import open_ne, build_fixup_map

# Window message names (mirrors wndproc.py's table).
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
# DOS INT 21h function names, for annotating the KERNEL wrappers' actual syscalls.
DOS21 = {
 0x02:"DISPLAY_CHAR",0x09:"DISPLAY_STRING",0x19:"GET_DEFAULT_DRIVE",0x1A:"SET_DTA",
 0x25:"SET_VECTOR",0x2A:"GET_DATE",0x2C:"GET_TIME",0x30:"GET_VERSION",0x35:"GET_VECTOR",
 0x36:"GET_DISK_SPACE",0x39:"MKDIR",0x3A:"RMDIR",0x3B:"CHDIR",0x3C:"CREATE",0x3D:"OPEN",
 0x3E:"CLOSE",0x3F:"READ",0x40:"WRITE",0x41:"UNLINK",0x42:"LSEEK",0x43:"GET_SET_ATTR",
 0x44:"IOCTL",0x47:"GET_CUR_DIR",0x48:"ALLOC",0x49:"FREE",0x4A:"REALLOC",0x4B:"EXEC",
 0x4C:"EXIT",0x4E:"FINDFIRST",0x4F:"FINDNEXT",0x56:"RENAME",0x57:"FILE_TIME",
}


def load_ordinals(path):
    """Parse ORDINALS.md tables into {'USER.39': 'BeginPaint', ...}.

    Retargeted for Windows 3.1: the module name is matched generically rather
    than against a fixed list. Windows 1.0 had nine system modules and they
    could be enumerated; 3.1 has 254 NE modules whose names are neither fixed
    nor unique, so anything hardcoded here would silently drop exports.

    (The Windows 1.0 ORDINALS_SDK.md merge is gone with it. In 3.1 the names
    come from the binaries' own export tables — see genordinals.py — so there
    is no second, more-authoritative source to layer underneath.)"""
    names = {}
    if not os.path.exists(path):
        return names
    rx = re.compile(
        r'\|\s*([A-Za-z_][A-Za-z0-9_]*)\.(\d+)\s*'
        r'\|\s*`?([^|`]+?)`?\s*\|')
    for line in open(path, encoding="utf-8", errors="replace"):
        m = rx.search(line)
        if m:
            names[f"{m.group(1)}.{int(m.group(2))}"] = m.group(3).strip()
    return names


def disasm_whole(code, base=0):
    """objdump the whole blob as 16-bit code; return [(offset, asm_text), ...].
    Uses a unique temp file so parallel invocations never collide."""
    fd, tmp = tempfile.mkstemp(suffix="_trace.bin")
    try:
        with os.fdopen(fd, "wb") as f:
            f.write(code)
        out = subprocess.run(
            ["objdump", "-D", "-b", "binary", "-mi386", "-Maddr16,data16,intel",
             "--adjust-vma=%d" % base, tmp],
            capture_output=True, text=True).stdout
    finally:
        try: os.remove(tmp)
        except OSError: pass
    rows = []
    for l in out.splitlines():
        if ":\t" not in l:
            continue
        head, rest = l.split(":\t", 1)
        try:
            off = int(head.strip(), 16)
        except ValueError:
            continue
        asm = rest.split("\t")[-1].strip()
        rows.append((off, asm))
    return rows


class Seg:
    """One CODE segment: disassembled, with functions discovered and calls resolved."""
    def __init__(self, ne, seg_index, ordinals, name_override=None):
        self.ne = ne
        self.si = seg_index
        self.seg = ne.segments()[seg_index-1]
        self.ordinals = ordinals
        # {offset:int -> real internal name}. Recovered exactly-matching names for
        # functions the export table leaves anonymous (see names.json produced by
        # the debug-symbol signature matcher). A pure label rename: it changes the
        # cProc name and every symbolic call/branch that resolves to it alike, so
        # the emitted bytes are unchanged. Export names still win at their offset.
        self.name_override = dict(name_override or {})
        self.mods = ne.module_refs()
        self.fixups = build_fixup_map(ne, self.seg, self.mods)   # site_off -> "MODULE.ord"/"segN:0xNNNN"
        self.code = ne.data[self.seg["file_off"]:self.seg["file_off"]+self.seg["length"]]
        self.rows = disasm_whole(self.code)
        self.off2idx = {o: i for i, (o, _) in enumerate(self.rows)}
        self.valid = set(self.off2idx)
        self.export_names = self._export_names()
        self.starts = self._find_starts()
        self.funcs = self._carve()          # start_off -> dict(start,end,name)

    # -- names -------------------------------------------------------------
    def _ord2name(self):
        res = self.ne.resident_names()
        non = self.ne.nonresident_names()
        m = {}
        for n, o in res[1:] + non[1:]:
            m[o] = n
        return m

    def _export_names(self):
        o2n = self._ord2name()
        out = {}
        for e in self.ne.entries():
            if e.get("kind") in ("UNUSED",):
                continue
            if e.get("seg") == self.si and "offset" in e:
                nm = o2n.get(e["ordinal"])
                if nm:
                    out[e["offset"]] = nm
        return out

    # -- function discovery ------------------------------------------------
    def _find_starts(self):
        starts = set()
        # (1) exported entries landing in this segment.
        #     export_names only covers entries that HAVE a name, which for the
        #     stripped system modules is none of them — so seed from the entry
        #     table directly as well. An exported entry point is a function
        #     start by definition, and without this the carve misses the first
        #     functions of every driver module.
        starts |= set(self.export_names)
        for e in self.ne.entries():
            if e.get("kind") != "UNUSED" and e.get("seg") == self.si and "offset" in e:
                starts.add(e["offset"])
        # (2) internal relocation targets pointing into this segment
        for lbl in self.fixups.values():
            m = re.match(r"seg(\d+):0x([0-9a-fA-F]+)$", lbl)
            if m and int(m.group(1)) == self.si:
                starts.add(int(m.group(2), 16))
        # (3) intra-segment near-call targets
        for off, asm in self.rows:
            m = re.match(r"call\s+0x([0-9a-f]+)$", asm)
            if m:
                starts.add(int(m.group(1), 16))
        # (4) prologue scan: push bp ; mov bp,sp
        for i in range(len(self.rows)-1):
            if self.rows[i][1] == "push bp" and self.rows[i+1][1].startswith("mov    bp,sp"):
                # back up over a MakeProcInstance-style export thunk if present
                s = self.rows[i][0]
                starts.add(s)
        # (5) the `enter N,0` prologue — the other standard 16-bit far-proc
        #     frame setup (opcode 0xC8). The GDI/USER modules use it alongside the
        #     push-bp form; without it a function that no intra-segment call
        #     reaches (called only via far pointer / from another segment) is
        #     invisible as a start, and the PRECEDING function's carve swallows
        #     it — e.g. GDI MulDiv (0x6942) merged 145 bytes of its `enter`-
        #     prologued neighbour, inflating the credited span.
        for off, asm in self.rows:
            if asm.startswith("enter "):
                starts.add(off)
        # (6) THUNK tail-JMP targets: a routine reached by a tail-`jmp` from
        #     >= 2 short thunks is a function start too — e.g. GDI's ~17 `Get*`
        #     accessor stubs (`mov cl,IMM; jmp 0x45fe`) all tail-jump to one
        #     shared getter at 0x45fe. Without this the getter is not a start,
        #     the preceding function's carve swallows it, and verify.py reports
        #     it "unresolved" when a reconstruction cites it.
        #
        #     STRICTLY a thunk pattern: the source `jmp` must be within 8 bytes
        #     of an already-known start (i.e. the jmp is the tail of a tiny
        #     stub), and >= 2 such thunks must target it. This deliberately does
        #     NOT count a `jmp` that sits deep inside a function — that is a
        #     shared epilogue / local control flow, not a boundary — so the carve
        #     is not over-split into thousands of fake functions.
        import bisect
        known = sorted(starts)
        jt = {}
        for off, asm in self.rows:
            m = re.match(r"jmp\s+0x([0-9a-f]+)$", asm)
            if not m:
                continue
            i = bisect.bisect_right(known, off) - 1
            if i >= 0 and 0 <= off - known[i] <= 8:   # tail of a short stub
                t = int(m.group(1), 16)
                jt[t] = jt.get(t, 0) + 1
        for tgt, n in jt.items():
            if n >= 2:
                starts.add(tgt)
        return {s for s in starts if s in self.valid}

    def _carve(self):
        order = sorted(self.starts)
        funcs = {}
        for i, s in enumerate(order):
            end = order[i+1] if i+1 < len(order) else self.seg["length"]
            funcs[s] = dict(start=s, end=end, name=self.name_at(s))
        return funcs

    def name_at(self, off):
        if off in self.export_names:
            return self.export_names[off]
        if off in self.name_override:
            return self.name_override[off]
        return f"sub_%04x" % off

    # -- call resolution ---------------------------------------------------
    def resolve_call(self, off, asm):
        """Return (kind, detail) for a call/jmp at `off`.
        kind: 'sys' -> (label, name|None); 'int' -> target_off; 'far' -> label; None."""
        # a fixup anywhere in this instruction's bytes names the target
        for k in range(off, off+7):
            lbl = self.fixups.get(k)
            if lbl:
                mm = re.match(r"seg(\d+):0x([0-9a-fA-F]+)$", lbl)
                if mm:
                    if int(mm.group(1)) == self.si:
                        return ("int", int(mm.group(2), 16))
                    return ("far", lbl)
                return ("sys", (lbl, self.ordinals.get(lbl)))
        m = re.match(r"call\s+0x([0-9a-f]+)$", asm)   # intra-segment near call
        if m:
            return ("int", int(m.group(1), 16))
        return (None, None)

    def func_rows(self, start):
        f = self.funcs.get(start)
        if not f:
            # not a known start: synthesize a span to the next known start
            nxt = next((s for s in sorted(self.starts) if s > start), self.seg["length"])
            f = dict(start=start, end=nxt, name=self.name_at(start))
        i0 = self.off2idx.get(start)
        if i0 is None:
            return f, []
        rows = []
        for off, asm in self.rows[i0:]:
            if off >= f["end"]:
                break
            rows.append((off, asm))
        # trim trailing padding (nop / int3 / lea esp fillers)
        while rows and rows[-1][1] in ("nop", "int3", "(bad)"):
            rows.pop()
        return f, rows

    def calls_in(self, start):
        """Distinct resolved calls made by a function, in order."""
        _, rows = self.func_rows(start)
        seen, out = set(), []
        for off, asm in rows:
            if not (asm.startswith("call") or asm.startswith("jmp")):
                continue
            kind, detail = self.resolve_call(off, asm)
            if kind == "sys":
                lbl, nm = detail
                key = ("sys", lbl)
                disp = f"{lbl}" + (f" = {nm}" if nm else "  [unidentified]")
            elif kind == "int":
                key = ("int", detail)
                disp = f"-> {self.name_at(detail)} (seg{self.si}:0x{detail:04x})"
            elif kind == "far":
                key = ("far", detail)
                disp = f"-> {detail} (other segment)"
            else:
                continue
            # only record jmp as a (tail)call when it leaves via a fixup/other target
            if asm.startswith("jmp") and kind == "int":
                # intra-function jump, not a call — skip local control flow
                if self.funcs.get(detail) is None:
                    continue
            if key not in seen:
                seen.add(key); out.append((kind, disp))
        return out

    def args_used(self, start):
        """Positive [bp+N] frame slots a function reads = its incoming PASCAL args."""
        _, rows = self.func_rows(start)
        slots = set()
        for _, asm in rows:
            for m in re.finditer(r"\[bp\+0x([0-9a-f]+)\]", asm):
                slots.add(int(m.group(1), 16))
        return sorted(s for s in slots if s >= 4)

    def word_at(self, off):
        """Read a little-endian WORD from the segment image, or None if OOB."""
        if 0 <= off <= len(self.code) - 2:
            return self.code[off] | (self.code[off+1] << 8)
        return None

    # -- WndProc dispatch decode ------------------------------------------
    def dispatch(self, start):
        """If `start` is a WndProc, return [(msg, handler_off), ...]; else [].
        Handles both the compare-chain form and the jump-table form
        (`sub ax,BASE; cmp ax,N; jbe; add ax,ax; jmp WORD cs:[bx+TAB]`)."""
        _, rows = self.func_rows(start)
        disp, seen, ax_is_msg = [], set(), False

        def handler_after(i):
            """Branch target for a message compare at row i. Handles three forms:
            `je H`; `jne SKIP; jmp H`; and `jne ELSE; <inline handler falls
            through>` (handler = the instruction right after the jne)."""
            for j, (o2, a2) in enumerate(rows[i+1:i+4]):
                jm = re.match(r"je     0x([0-9a-f]+)", a2)
                if jm: return int(jm.group(1), 16)
                jn = re.match(r"jne    0x([0-9a-f]+)", a2)
                if jn:
                    k = i+1+j+1
                    if k < len(rows):
                        jm2 = re.match(r"jmp    0x([0-9a-f]+)", rows[k][1])
                        return int(jm2.group(1), 16) if jm2 else rows[k][0]
                    return None
            return None

        for i, (off, asm) in enumerate(rows):
            # ---- dialog form: direct memory compare of the message slot -------
            md = re.match(r"cmp    WORD PTR \[bp\+0xc\],0x([0-9a-f]+)$", asm)
            if md:
                val = int(md.group(1), 16)
                h = handler_after(i)
                if h is not None and val not in seen:
                    disp.append((val, h)); seen.add(val)
                continue
            # ---- jump-table dispatch: jmp WORD cs:[bx+TAB] --------------------
            jt = re.match(r"jmp    WORD PTR cs:\[bx\+0x([0-9a-f]+)\]", asm)
            if jt:
                tab = int(jt.group(1), 16)
                count, base, saw_msg = None, 0, False
                for o2, a2 in reversed(rows[max(0, i-20):i]):  # look back for the range check
                    mm = re.match(r"cmp    ax,0x([0-9a-f]+)$", a2)
                    if mm and count is None:
                        count = int(mm.group(1), 16)
                    sm = re.match(r"sub    ax,0x([0-9a-f]+)$", a2)
                    if sm:
                        base += int(sm.group(1), 16)
                    if re.match(r"mov    ax,WORD PTR \[bp\+0xc\]", a2):
                        saw_msg = True
                if count is not None and saw_msg:
                    for idx in range(count + 1):
                        h = self.word_at(tab + 2*idx)
                        if h is not None and (idx + base) not in seen:
                            disp.append((idx + base, h)); seen.add(idx + base)
                continue
            if re.match(r"mov    ax,WORD PTR \[bp\+0xc\]", asm):
                ax_is_msg = True; continue
            if re.match(r"(mov    ax,|pop    ax|lea    ax|call|xor    ax,ax|sub    ax,ax|xchg .*ax)", asm):
                if not asm.startswith("cmp"): ax_is_msg = False
            m = re.match(r"cmp    ax,0x([0-9a-f]+)$", asm)
            if not m or not ax_is_msg:
                continue
            val = int(m.group(1), 16)
            handler = handler_after(i)
            if handler is not None and val not in seen:
                disp.append((val, handler)); seen.add(val)
        return disp

    def handler_calls(self, start, handler_off, handler_starts):
        """Resolved calls a single dispatch handler makes: walk from handler_off
        until the next handler / a ret / a jump out to the shared epilogue."""
        _, rows = self.func_rows(start)
        idx = {o: k for k, (o, _) in enumerate(rows)}
        k = idx.get(handler_off)
        if k is None:
            return []
        nxt = min((s for s in handler_starts if s > handler_off), default=None)
        out, seen = [], set()
        while k < len(rows):
            off, asm = rows[k]
            if off != handler_off and off in handler_starts:
                break
            if asm.startswith("ret"):
                break
            jm = re.match(r"jmp    0x([0-9a-f]+)$", asm)
            if jm and not asm.startswith("call"):
                t = int(jm.group(1), 16)
                if t < handler_off or (nxt and t >= nxt):
                    break                       # tail-jump to shared epilogue
            if asm.startswith("call") or asm.startswith("jmp"):
                kind, detail = self.resolve_call(off, asm)
                if kind == "sys":
                    lbl, nm = detail
                    key, disp = ("s", lbl), lbl + (f"={nm}" if nm else " [unident]")
                elif kind == "int":
                    key, disp = ("i", detail), f"->{self.name_at(detail)}"
                elif kind == "far":
                    key, disp = ("f", detail), f"->{detail}"
                else:
                    k += 1; continue
                if key not in seen:
                    seen.add(key); out.append(disp)
            k += 1
        return out

    def is_thunk(self, start):
        """Detect the `mov bx,IMM; push bx; jmp TARGET` MOVEABLE-entry thunk;
        return TARGET (the shared dispatcher) or None."""
        _, rows = self.func_rows(start)
        if len(rows) >= 3 and rows[0][1].startswith("mov    bx,0x") \
           and rows[1][1] == "push   bx":
            jm = re.match(r"jmp    0x([0-9a-f]+)", rows[2][1])
            if jm:
                return int(jm.group(1), 16)
        return None


def find_seg_of_entry(ne, ordinal):
    for e in ne.entries():
        if e.get("ordinal") == ordinal and "offset" in e:
            return e["seg"], e["offset"]
    return None, None


# ---- annotated printing ----------------------------------------------------
def annotate_row(sg, off, asm):
    line = f"    {off:04x}:  {asm}"
    if asm.startswith("call") or asm.startswith("jmp"):
        kind, detail = sg.resolve_call(off, asm)
        if kind == "sys":
            lbl, nm = detail
            line += f"      ; {lbl}" + (f" = {nm}" if nm else "  [UNIDENTIFIED — leave raw]")
        elif kind == "int" and (sg.funcs.get(detail) or asm.startswith("call")):
            line += f"      ; -> {sg.name_at(detail)}"
        elif kind == "far":
            line += f"      ; -> {detail}"
    m = re.match(r"mov    ah,0x([0-9a-f]+)", asm)   # flag DOS syscalls
    if m and int(m.group(1), 16) in DOS21:
        line += f"      ; AH={m.group(1)} DOS {DOS21[int(m.group(1),16)]}"
    if asm.startswith("int    0x21"):
        line += "      ; DOS call"
    return line


def print_function(sg, start, body=True):
    f, rows = sg.func_rows(start)
    args = sg.args_used(start)
    disp = sg.dispatch(start)
    hdr = f"==== {f['name']}  seg{sg.si}:0x{start:04x}"
    hdr += f"  ({len(rows)} instrs, 0x{start:04x}..0x{f['end']:04x})"
    print(hdr)
    if args:
        argstr = ", ".join(f"[bp+0x{a:02x}]" for a in args)
        print(f"   args (PASCAL frame slots read): {argstr}")
    if disp:
        print(f"   WndProc dispatch ({len(disp)} messages):")
        for val, h in sorted(disp):
            print(f"      {WM.get(val, 'msg_0x%04x'%val):20} 0x{val:04x} -> handler @0x{h:04x}")
    calls = sg.calls_in(start)
    if calls:
        sysn = [d for k, d in calls if k == "sys"]
        intn = [d for k, d in calls if k == "int"]
        farn = [d for k, d in calls if k == "far"]
        if sysn: print("   syscalls: " + "; ".join(sysn))
        if intn: print("   internal: " + "; ".join(intn))
        if farn: print("   far:      " + "; ".join(farn))
    if body:
        print()
        for off, asm in rows:
            print(annotate_row(sg, off, asm))
    print()


def trace_callgraph(sg, start):
    """BFS over internal calls from `start`; return reachable func starts in discovery order."""
    order, seen, queue = [], set(), [start]
    while queue:
        s = queue.pop(0)
        if s in seen:
            continue
        seen.add(s); order.append(s)
        for kind, disp in sg.calls_in(s):
            if kind == "int":
                m = re.search(r"seg\d+:0x([0-9a-f]+)", disp)
                if m:
                    t = int(m.group(1), 16)
                    if t not in seen:
                        queue.append(t)
    return order


# ---- commands --------------------------------------------------------------
def cmd_map(ne, ordinals):
    res = ne.resident_names(); non = ne.nonresident_names()
    modname = res[0][0] if res else "?"
    desc = non[0][0] if non else ""
    segs = ne.segments()
    print(f"# {modname}   {desc}")
    print(f"# imports: {', '.join(ne.module_refs())}")
    print(f"# {len(segs)} segments\n")
    o2n = {}
    for n, o in res[1:] + non[1:]:
        o2n[o] = n
    print("Exports:")
    for e in sorted(ne.entries(), key=lambda x: x.get("ordinal", 0)):
        if e.get("kind") in ("UNUSED", None) or "offset" not in e:
            continue
        nm = o2n.get(e["ordinal"], "")
        print(f"   @{e['ordinal']:<4} {nm:24} seg{e['seg']}:0x{e['offset']:04x}")
    print()
    for s in segs:
        if s["kind"] != "CODE":
            print(f"seg{s['idx']}: {s['kind']} 0x{s['length']:x} bytes")
            continue
        sg = Seg(ne, s["idx"], ordinals)
        nsys = sum(1 for l in sg.fixups.values() if not l.startswith("seg"))
        print(f"seg{s['idx']}: CODE 0x{s['length']:x} bytes  "
              f"{len(sg.funcs)} functions discovered  {len(sg.fixups)} fixups ({nsys} syscalls)")


def cmd_list(ne, ordinals, seg_index):
    sg = Seg(ne, seg_index, ordinals)
    print(f"# {len(sg.funcs)} functions in seg{seg_index}\n")
    for s in sorted(sg.funcs):
        print_function(sg, s, body=False)


def cmd_fn(ne, ordinals, seg_index, addr):
    sg = Seg(ne, seg_index, ordinals)
    print_function(sg, addr, body=True)


def cmd_cskel(ne, ordinals, seg_index, start):
    """Emit a ready-to-refine C skeleton for a window/dialog procedure: the
    dispatch switch (each case annotated with its handler offset and resolved
    calls) plus documented-partial templates for every reachable internal fn.
    Automates the mechanical part of Level-2 reconstruction; the writer fills
    logic and verifies. Never fabricates names (unresolved ordinals stay raw)."""
    sg = Seg(ne, seg_index, ordinals)
    disp = sg.dispatch(start)
    name = sg.name_at(start)
    hs = sorted(h for _, h in disp)
    is_dlg = any(v == 0x110 for v, _ in disp)       # WM_INITDIALOG => dialog proc
    ret_t = "BOOL" if is_dlg else "long"
    hval = "hDlg" if is_dlg else "hWnd"
    print(f"/* AUTO-SKELETON from trace.py --cskel  seg{seg_index}:0x{start:04x}"
          f"  ({'dialog proc' if is_dlg else 'window proc'}) */")
    print(f"{ret_t} FAR PASCAL {name}(HWND {hval}, unsigned message, WORD wParam, LONG lParam)")
    print("{")
    print("    switch (message) {")
    for val, h in sorted(disp):
        nm = WM.get(val, f"0x{val:04x}")
        calls = sg.handler_calls(start, h, hs)
        cs = ("calls: " + "; ".join(calls)) if calls else "(no resolved calls)"
        print(f"    case {nm}:  /* @0x{h:04x}  {cs} */")
        print(f"        break;")
    if is_dlg:
        print("    }\n    return FALSE;\n}")
    else:
        print("    default:")
        print(f"        return DefWindowProc({hval}, message, wParam, lParam); /* USER.107 */")
        print("    }\n    return 0L;\n}")
    order = [s for s in trace_callgraph(sg, start) if s != start]
    if order:
        print(f"\n/* --- {len(order)} reachable internal functions (documented-partial templates) --- */")
        for s in order:
            f, rows = sg.func_rows(s)
            args = sg.args_used(s)
            calls = sg.calls_in(s)
            sysn = [d for k, d in calls if k == "sys"]
            intn = [d for k, d in calls if k == "int"]
            astr = ", ".join(f"[bp+0x{a:02x}]" for a in args) or "none"
            print(f"/* {f['name']}  seg{sg.si}:0x{s:04x}  ({len(rows)} instrs; args {astr})")
            if sysn: print(f" *   syscalls: {'; '.join(sysn)}")
            if intn: print(f" *   internal: {'; '.join(intn)}")
            print(" *   [documented partial] */")


def cmd_wndprocs(ne, ordinals, only_seg=None):
    """Scan code segments for real window procedures (functions that load the
    `message` arg and dispatch >=2 messages), decoding each dispatch table.
    Finds bodies even when the export entry is a MOVEABLE thunk."""
    o2n = {}
    for n, o in ne.resident_names()[1:] + ne.nonresident_names()[1:]:
        o2n[o] = n
    # map seg:offset -> export name via the entry table
    ent = {}
    for e in ne.entries():
        if "offset" in e and o2n.get(e.get("ordinal")):
            ent[(e["seg"], e["offset"])] = o2n[e["ordinal"]]
    for s in ne.segments():
        if s["kind"] != "CODE" or (only_seg and s["idx"] != only_seg):
            continue
        sg = Seg(ne, s["idx"], ordinals)
        for start in sorted(sg.funcs):
            disp = sg.dispatch(start)
            if len(disp) < 2:
                continue
            nm = ent.get((s["idx"], start), "")
            tag = f"  [export {nm}]" if nm else ""
            print(f"seg{s['idx']}:0x{start:04x}{tag}  - {len(disp)} messages")
            for val, h in sorted(disp):
                print(f"    {WM.get(val, 'msg_0x%04x'%val):20} 0x{val:04x} -> 0x{h:04x}")
            print()


def cmd_trace(ne, ordinals, seg_index, start):
    sg = Seg(ne, seg_index, ordinals)
    order = trace_callgraph(sg, start)
    print(f"# call graph from seg{seg_index}:0x{start:04x} - "
          f"{len(order)} reachable functions in this segment\n")
    # roll up the full external API surface
    allsys = {}
    for s in order:
        for kind, disp in sg.calls_in(s):
            if kind == "sys":
                lbl = disp.split()[0]
                allsys[lbl] = disp
    print("External API surface (all reachable functions):")
    for lbl in sorted(allsys, key=lambda x: (x.split(".")[0], int(x.split(".")[1]))):
        print(f"   {allsys[lbl]}")
    print("\n" + "="*70 + "\n")
    for s in order:
        print_function(sg, s, body=True)


def main():
    try:                                    # box/arrow chars over a cp1252 console
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    except Exception:
        pass
    a = sys.argv[1:]
    if not a:
        print(__doc__); sys.exit(1)
    path = a[0]
    ords_path = a[a.index("--ords")+1] if "--ords" in a else "ORDINALS.md"
    ordinals = load_ordinals(ords_path)
    ne = open_ne(path)

    def int0(x): return int(x, 0)

    if "--map" in a:
        cmd_map(ne, ordinals)
    elif "--wndprocs" in a:
        rest = [x for x in a[a.index("--wndprocs")+1:] if not x.startswith("--")]
        cmd_wndprocs(ne, ordinals, int0(rest[0]) if rest else None)
    elif "--cskel" in a:
        addr = int0(a[a.index("--cskel")+1])
        rest = [x for x in a[a.index("--cskel")+2:] if not x.startswith("--")]
        cmd_cskel(ne, ordinals, int0(rest[0]) if rest else 1, addr)
    elif "--list" in a:
        rest = [x for x in a[a.index("--list")+1:] if not x.startswith("--")]
        seg = int0(rest[0]) if rest else 1
        cmd_list(ne, ordinals, seg)
    elif "--fn" in a:
        addr = int0(a[a.index("--fn")+1])
        rest = [x for x in a[a.index("--fn")+2:] if not x.startswith("--")]
        seg = int0(rest[0]) if rest else 1
        cmd_fn(ne, ordinals, seg, addr)
    elif "--trace" in a:
        tgt = a[a.index("--trace")+1]
        rest = [x for x in a[a.index("--trace")+2:] if not x.startswith("--")]
        if tgt.startswith("0x"):
            seg = int0(rest[0]) if rest else 1
            start = int0(tgt)
        else:                       # an ordinal: look up its seg:offset
            seg, start = find_seg_of_entry(ne, int0(tgt))
            if seg is None:
                print(f"ordinal {tgt} not found in entry table"); sys.exit(1)
        cmd_trace(ne, ordinals, seg, start)
    else:
        print(__doc__)


if __name__ == "__main__":
    main()
