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

    ORDINALS_SDK.md — the map recovered from the SDK's own import library — is
    read first when present, then ORDINALS.md overrides it. That order is
    deliberate: the library is authoritative for the ordinal->name binding, but
    ORDINALS.md is where this project records corrections and the names it had
    to coin for internal procedures the library does not export."""
    names = {}
    sdk = os.path.join(os.path.dirname(path) or ".", "ORDINALS_SDK.md")
    if os.path.exists(sdk) and path.endswith("ORDINALS.md"):
        names.update(load_ordinals(sdk))
    if not os.path.exists(path):
        return names
    rx = re.compile(
        r'\|\s*(KERNEL|GDI|USER|KEYBOARD|MOUSE|DISPLAY|SOUND|COMM|SYSTEM)\.(\d+)\s*'
        r'\|\s*`?([^|`]+?)`?\s*\|')
    for line in open(path, encoding="utf-8", errors="replace"):
        m = rx.search(line)
        if not m:
            continue
        nm = m.group(3).strip().strip("*").strip()
        # A NAME MUST LOOK LIKE ONE. ORDINALS.md also contains audit tables
        # whose second column is prose — "| KERNEL.58 | a 9-word file
        # operation, identity unknown since the earliest sessions |
        # **`GetProfileString`** |" is a was/is table, and this regex was
        # reading the WAS column. Two ordinals were being carried by every
        # tool in the project with an English sentence as their name, which is
        # exactly how a sweep writes nonsense into the reconstruction. Rows
        # whose name is not an identifier are skipped, so a descriptive cell
        # can never override a real name (and a deliberately descriptive entry
        # like "*(the owner-teardown broadcaster)*" stays unnamed, as intended).
        if not re.fullmatch(r"[A-Za-z_]\w*", nm):
            continue
        names[f"{m.group(1)}.{int(m.group(2))}"] = nm
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
    def __init__(self, ne, seg_index, ordinals):
        self.ne = ne
        self.si = seg_index
        self.seg = ne.segments()[seg_index-1]
        self.ordinals = ordinals
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
        forced = set()
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
        # (5) DESCRIPTOR THUNKS — the shared-prologue convention.
        #
        # A function that gets its frame from a shared prologue has NO prologue
        # of its own, is never near-called (it is reached through the entry
        # table) and is not a relocation target, so rules (1)-(4) all miss it
        # and it gets swallowed by whatever function precedes it. That is the
        # carve bug that made CALENDAR's window procedure disappear into
        # RecalcLayout at seg3:0x0000, and with it sixteen calls that were then
        # reported missing from a function that never made them.
        #
        # The thunk is recognisable and exact:
        #
        #       mov bx,0x0142      ; the address of a 2-byte DESCRIPTOR
        #       push bx
        #       jmp 0x5799         ; the shared prologue
        #
        # The prologue pops that address, reads the descriptor (frame size and
        # argument bytes) and resumes at descriptor+2 — so descriptor+2 is a
        # function start, and the two bytes before it are DATA. Both facts are
        # recorded: the start, and `self.descriptors`, so a disassembler can
        # skip the bytes instead of decoding them as an instruction.
        self.descriptors = set()
        for i in range(len(self.rows) - 2):
            m = re.match(r"mov\s+bx,0x([0-9a-f]+)$", self.rows[i][1])
            if not m or self.rows[i+1][1] != "push   bx":
                continue
            if not re.match(r"jmp\s+0x[0-9a-f]+$", self.rows[i+2][1]):
                continue
            desc = int(m.group(1), 16)
            if desc + 2 < self.seg["length"]:
                self.descriptors.add(desc)
                forced.add(desc + 2)
        # `valid` is "landed on by the linear disassembly", which is the right
        # sanity check for a target scraped out of an instruction — but a
        # descriptor start is DERIVED, not scraped, and it is precisely the
        # case the linear pass cannot land on: the two data bytes in front of
        # it absorb the decoder (0x142 reads as a 3-byte `adc ax,0xbe05` and
        # swallows 0x144). Requiring `valid` here would throw away the only
        # rule that finds these functions, so forced starts bypass it.
        return {s for s in starts if s in self.valid} | forced

    def _carve(self):
        # Include offset 0: the region before the first detected function is
        # almost always non-code — a segment header string ("SOUND\\0"), a
        # color/gradient table (DISPLAY), or zero fill (KERNEL) — and was never
        # examined, leaving ~2000 bytes segment-wide accounted as neither code
        # nor data. _segment_code splits it into data (the table) and code (a
        # real function with no detected start, if any) like any other span.
        order = sorted(set(self.starts) | {0})
        funcs = {}
        self.data_runs = []            # [(start, end)] of DATA carved out of code
        for i, s in enumerate(order):
            end = order[i+1] if i+1 < len(order) else self.seg["length"]
            # Split this [start,end) span around any embedded DATA. The carve is
            # otherwise "start-to-next-start", which silently swallows the
            # padding, jump tables and standalone data tables that sit between
            # functions in a code segment (SYSTEM seg1:0x150 is a
            # `00000000 FFFF 0000` record table + an 0xFF fill run, followed by a
            # real interrupt handler at 0x231 — all carved as one bogus
            # "function"). Data is DETECTED from the bytes, not guessed.
            for cstart, cend, kind in self._segment_code(s, end):
                if kind == "data":
                    self.data_runs.append((cstart, cend))
                else:
                    funcs[cstart] = dict(start=cstart, end=cend,
                                         name=self.name_at(cstart))
        return funcs

    # -- code / data separation -------------------------------------------
    def _segment_code(self, start, end):
        """Split [start,end) into ('code'|'data', ...) spans by DETECTING data.
        Data is a fill run (>=8 equal bytes), a short-period repeating table, OR
        a span that decodes with IMPOSSIBLE instructions — the same 8086-legality
        test the differential decoder uses: a Windows 1.0 code segment has no
        186/286/386 opcodes, so a run that objdump only decodes as those (or as
        `(bad)`) is data a linear sweep walked through, not code. Conservative:
        a clean 8086 span stays code."""
        runs = self._data_runs(start, end)
        p, spans = start, []
        for ds, de in runs:
            if ds > p:
                spans.append((p, ds, "code"))
            spans.append((ds, de, "data"))
            p = de
        if p < end:
            spans.append((p, end, "code"))
        # second pass: reclassify a 'code' span as data if it is not clean 8086
        for a, z, kind in spans:
            if kind == "code" and not self._looks_like_8086(a, z):
                kind = "data"
            yield a, z, kind

    def _looks_like_8086(self, a, z):
        """Does [a,z) decode as plausible 8086 code? False if it contains
        anachronistic (186+) instructions or a cluster of undecodable bytes —
        the signature of data. Short spans (<6 B) are given the benefit."""
        import opc8086
        rows = [(o, s) for o, s in self.rows if a <= o < z]
        if len(rows) < 3:
            return True
        bad = anach = 0
        for o, asm in rows:
            # objdump renders a byte it cannot decode as an instruction either as
            # `(bad)` or as a raw `.byte 0xNN` — both are the signature of data a
            # linear sweep walked into, so count both.
            if "(bad)" in asm or asm.lstrip().startswith(".byte"):
                bad += 1
            else:
                op = self.code[o] if o < len(self.code) else 0
                # skip a legal 8086 prefix to reach the real opcode
                k = o
                while k < z and self.code[k] in (0x26, 0x2E, 0x36, 0x3E,
                                                 0xF0, 0xF2, 0xF3):
                    k += 1
                if opc8086.post8086_reason(self.code[k] if k < z else 0, asm):
                    anach += 1
        # Real Windows 1.0 code is 99.98% 8086-clean (difdec), so these signals
        # essentially never occur in genuine code: >=2 anachronisms, >=3 bad
        # bytes, or a third of the span undecodable. The last clause adds the
        # SHORT-SPAN case: a data table wedged between functions is a short span
        # (<=16 rows) with >=2 combined data signals — that catches PAINT's
        # pattern/coordinate tables at seg1:0x0a94 etc. (bogus "functions" a
        # single threshold left for the lift to choke on) WITHOUT touching a long
        # real function that merely has a small trailing jump table (low density,
        # not short).
        short = len(rows) <= 16
        return not (anach >= 2 or bad >= 3 or (anach + bad) > len(rows) // 3
                    or (short and anach + bad >= 2))

    def _data_runs(self, start, end):
        """Byte-level DATA regions in [start,end): fill runs and repeating
        tables. Returned merged and only when >= MINRUN bytes, so ordinary
        code (which is high-entropy and non-repeating) is never flagged."""
        MINFILL, MINTAB = 8, 16
        b = self.code
        runs = []
        i = start
        while i < end:
            c = b[i]
            # NUL-terminated printable string (USER's exported SZDISPLAY etc.:
            # data that decodes as VALID 8086 because ASCII bytes are legal
            # opcodes, so the anachronism test can't see it — but a run of
            # printable chars ending in NUL is unmistakably a string).
            j = i
            while j < end and (0x20 <= b[j] < 0x7f):
                j += 1
            if j - i >= 4 and j < end and b[j] == 0x00:
                runs.append([i, j + 1]); i = j + 1; continue
            # fill run: a stretch of one byte value
            j = i
            while j < end and b[j] == c:
                j += 1
            if j - i >= MINFILL and c in (0x00, 0xFF, 0x90, 0xCC):
                runs.append([i, j]); i = j; continue
            # repeating short-period table (e.g. 00 00 00 00 FF FF 00 00 ...)
            found = False
            for per in (2, 4, 8):
                j = i
                while (j + per <= end
                       and b[j:j + per] == b[i:i + per]):
                    j += per
                if j - i >= MINTAB and len(set(b[i:i + per])) <= 3:
                    runs.append([i, j]); i = j; found = True; break
            if not found:
                i += 1
        # merge adjacent/overlapping runs
        merged = []
        for r in runs:
            if merged and r[0] <= merged[-1][1] + 2:
                merged[-1][1] = max(merged[-1][1], r[1])
            else:
                merged.append(r)
        return [(a, z) for a, z in merged]

    def name_at(self, off):
        if off in self.export_names:
            return self.export_names[off]
        return f"sub_%04x" % off

    # -- call resolution ---------------------------------------------------
    def resolve_call(self, off, asm):
        """Return (kind, detail) for a call/jmp at `off`.
        kind: 'sys' -> (label, name|None); 'int' -> target_off; 'far' -> label; None."""
        # A fixup anywhere in THIS INSTRUCTION'S OWN BYTES names the target.
        #
        # The bound must be the instruction's real length, not a fixed window.
        # With a flat +7 a 3-byte `jmp 0x100c` at 0x0cc4 reached into 0x0cc8 —
        # the relocation belonging to the `call` that follows it — and reported
        # the jump as a call to WRITE.294. Every such misread is a call the
        # reconstruction would record that the binary does not make.
        i = self.off2idx.get(off)
        if i is not None and i + 1 < len(self.rows):
            end = self.rows[i + 1][0]
        else:
            end = off + 7
        for k in range(off, end):
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
