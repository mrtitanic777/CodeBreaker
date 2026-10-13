"""
pe_trace.py — the 32-bit (PE) function carve: `trace.Seg` for the PE world.

`trace.Seg` carves 16-bit NE segments via objdump; this does the same job for
the target's PE modules (e.g. KERNEL32/USER32/GDI32) using
**capstone** (32-bit x86). It gives the PE side what the NE side already has:
function starts, per-function extents, and *resolved* calls — so a reconstruction
can cite `0xADDR` and be checked, and the ledger can one day credit PE bytes.

Function starts come from three sources, most-authoritative first:
  1. the export table (RVAs the binary itself declares) — ground truth;
  2. direct `call <imm>` targets seen in a linear sweep — real entry points;
  3. standard prologues: `push ebp; mov ebp,esp` (55 8B EC) and the MS hot-patch
     `mov edi,edi; push ebp; mov ebp,esp` (8B FF 55 8B EC).
Extents run start-to-next-start (as in trace.Seg).

Calls resolve via `pe.imports()` (a `call [IAT_slot]` -> `DLL!func`) and the
export/known-start set (a `call <imm>` -> a named export or a local `sub_VA`).
That is what makes the carve checkable rather than a raw listing.

Doctrine: this is *evidence extraction*, not understanding. It says where the
functions are and what they call; a human still reads them. Capstone is a
modern decoder — cross-check anything surprising against objdump/Ghidra.

    python pe_trace.py USER32.DLL --list            # functions + call counts
    python pe_trace.py USER32.DLL --func 0xbff9xxxx # one function, resolved
    python pe_trace.py KERNEL32.DLL --stats
"""
import bisect
from codebreaker.base import registry as cores
import capstone
from codebreaker.formats.pe import open_pe
import os
from codebreaker.base import settings as paths



target = cores.get('target')


class PeCarve:
    def __init__(self, path, progress_cb=None):
        # progress_cb(bytes_swept) is called periodically during the (dominant)
        # linear capstone sweep, so a caller can drive a disassembly progress bar.
        self._progress_cb = progress_cb
        self.pe = open_pe(target.resolve(path))
        self.md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        self.md.detail = True
        self.sections = self.pe.code_sections()          # [(va, bytes, name)]
        self.imports = self.pe.imports()                 # {iat_va: (dll, name)}
        self.exports = {}                                 # export_va -> name
        for o, n, rva in self.pe.exports():
            self.exports[self.pe.image_base + rva] = n or ("Ord%d" % o)
        self._sweep()
        self.starts = self._find_starts()
        self.funcs = self._carve()

    # -- section helpers --------------------------------------------------
    def _sec_of(self, va):
        for base, data, _name in self.sections:
            if base <= va < base + len(data):
                return base, data
        return None, None

    def _sweep(self):
        """One linear capstone pass per code section. Collects every decoded
        instruction (va -> (mnem, op_str, size, call_target)) and the set of
        direct-call targets. Linear sweep drifts on embedded data — same caveat
        as trace.Seg — so starts are corroborated by exports/prologues too."""
        self.insn = {}
        self.call_targets = set()
        cb = self._progress_cb
        swept = 0                                 # bytes swept across prior sections
        cnt = 0
        for base, data, _name in self.sections:
            n = len(data)
            off = 0
            while off < n:
                made = 0
                for ins in self.md.disasm(data[off:], base + off):
                    tgt = None
                    if ins.mnemonic == "call" and ins.operands:
                        op = ins.operands[0]
                        if op.type == capstone.x86.X86_OP_IMM:
                            tgt = op.imm
                            self.call_targets.add(tgt)
                    self.insn[ins.address] = (ins.mnemonic, ins.op_str, ins.size, ins, tgt)
                    off = ins.address - base + ins.size
                    made += 1
                    cnt += 1
                    if cb and cnt % 2000 == 0:     # report every ~2000 instructions
                        cb(swept + off)
                if made == 0:
                    # capstone hit a byte it can't decode (data-in-code) and
                    # stopped; skip it and resync, or the rest of the section —
                    # and every function in it — is lost (oleaut32 carved 0).
                    off += 1
            swept += n
        if cb:
            cb(swept)

    def _find_starts(self):
        starts = set(self.exports)
        # entry point
        if self.pe.entry:
            starts.add(self.pe.image_base + self.pe.entry)
        # direct-call targets that land in a code section
        for t in self.call_targets:
            if self._sec_of(t)[0] is not None:
                starts.add(t)
        # prologue scan
        for base, data, _name in self.sections:
            n = len(data)
            for i in range(n - 4):
                b = data[i:i + 5]
                if b[:3] == b"\x55\x8b\xec":                     # push ebp; mov ebp,esp
                    starts.add(base + i)
                elif b[:5] == b"\x8b\xff\x55\x8b\xec":           # mov edi,edi; push ebp; mov ebp,esp
                    starts.add(base + i)
        # keep only addresses we actually decoded an instruction at
        return {s for s in starts if s in self.insn}

    def _carve(self):
        order = sorted(self.starts)
        out = {}
        for i, s in enumerate(order):
            # end = next start in the SAME section, else section end
            base, data = self._sec_of(s)
            secend = base + len(data)
            end = secend
            if i + 1 < len(order) and order[i + 1] < secend:
                end = order[i + 1]
            out[s] = {"start": s, "end": end, "name": self.exports.get(s)}
        return out

    # -- queries ----------------------------------------------------------
    def resolve_call(self, ins):
        """A capstone call/jmp insn -> a name, or None if not a call/branch."""
        if ins.mnemonic not in ("call", "jmp") or not ins.operands:
            return None
        op = ins.operands[0]
        if op.type == capstone.x86.X86_OP_IMM:
            t = op.imm
            if t in self.exports:
                return self.exports[t]
            if t in self.funcs or self._sec_of(t)[0] is not None:
                return "sub_%x" % t
            return None
        if op.type == capstone.x86.X86_OP_MEM:
            m = op.mem
            # absolute [disp] with no base/index = an IAT slot (or a global /
            # loader-connected thunk pointer). capstone gives disp SIGNED, so
            # mask to 32-bit before matching the IAT map. A hit is a named
            # import; a miss is an internal indirect (e.g. the per-module
            # QT_Thunk pointer) and honestly stays unresolved.
            if m.base == 0 and m.index == 0:
                imp = self.imports.get(m.disp & 0xFFFFFFFF)
                if imp:
                    return "%s!%s" % imp
                return None
        return None

    def calls_in(self, start):
        """Resolved call/jmp targets inside the function carved at `start`."""
        f = self.funcs.get(start)
        if not f:
            return []
        out = []
        va = start
        while va < f["end"]:
            rec = self.insn.get(va)
            if rec is None:
                va += 1
                continue
            _mn, _op, size, ins, _tgt = rec
            nm = self.resolve_call(ins)
            if nm:
                out.append(nm)
            va += size
        return out

    def disasm_func(self, start):
        f = self.funcs.get(start)
        if not f:
            return []
        rows, va = [], start
        while va < f["end"]:
            rec = self.insn.get(va)
            if rec is None:
                va += 1
                continue
            mn, op, size, _ins, _t = rec
            rows.append((va, mn, op))
            va += size
        return rows

    # -- data cross-references -------------------------------------------
    # `resolve_call` gives the control-flow edges; these give the *data* edges:
    # which function reads/pushes the address of a string or global. A `push
    # imm32`, `mov reg,imm32`, or absolute `[disp]` whose value lands inside the
    # image is a data reference — the join that answers "who references this
    # string?" without which a literal cannot be traced back to its caller.
    def _image_ranges(self):
        r = getattr(self, "_imgr", None)
        if r is None:
            r = [(self.pe.image_base + s["vaddr"],
                  self.pe.image_base + s["vaddr"] + max(s["vsize"], s["rawsize"]))
                 for s in self.pe.sections]
            self._imgr = r
        return r

    def _in_image(self, va):
        for lo, hi in self._image_ranges():
            if lo <= va < hi:
                return True
        return False

    def data_refs(self):
        """{func_start: sorted[referenced data VA]}.

        Records IMM and absolute-`[disp]` operands that land in the image, except
        a branch's direct target (a code edge, owned by resolve_call/calls_in) and
        recognized IAT slots (import edges, owned by resolve_call)."""
        cached = getattr(self, "_drefs", None)
        if cached is not None:
            return cached
        starts = sorted(self.funcs)
        fend = {s: self.funcs[s]["end"] for s in starts}
        out = {}
        for addr, (mn, _op, _sz, ins, _tgt) in self.insn.items():
            i = bisect.bisect_right(starts, addr) - 1
            if i < 0:
                continue
            fs = starts[i]
            if addr >= fend[fs]:
                continue
            branch = mn in ("call", "jmp") or mn.startswith("j")
            for op in ins.operands:
                va = None
                if op.type == capstone.x86.X86_OP_IMM:
                    if branch:
                        continue
                    va = op.imm & 0xFFFFFFFF
                elif op.type == capstone.x86.X86_OP_MEM:
                    m = op.mem
                    if m.base == 0 and m.index == 0:
                        cand = m.disp & 0xFFFFFFFF
                        if cand in self.imports:
                            continue
                        va = cand
                if va is not None and self._in_image(va):
                    out.setdefault(fs, set()).add(va)
        self._drefs = {s: sorted(v) for s, v in out.items()}
        return self._drefs

    def xrefs_to(self, va):
        """Sorted function-starts that reference data address `va`."""
        rev = getattr(self, "_drev", None)
        if rev is None:
            rev = {}
            for fs, vas in self.data_refs().items():
                for v in vas:
                    rev.setdefault(v, []).append(fs)
            self._drev = rev
        return sorted(rev.get(va, []))

    def string_refs(self, va2text):
        """{func_start: [(va, text), ...]} — the strings each function references.
        `va2text` is a {VA: string} map (see format_loaders.pe_strings.by_va)."""
        out = {}
        for fs, vas in self.data_refs().items():
            hits = [(v, va2text[v]) for v in vas if v in va2text]
            if hits:
                out[fs] = hits
        return out


_MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
_CARVE = {}

# Bootstrap: put the target dir on sys.path so bare sibling imports (cc32,
# segdiff, target, classify32, ...) resolve, and activate the win95 target so
# cc32's cores-based `target` works -- for standalone `python -m` use. Idempotent
# and harmless when `wt` already did this. Importing pe32util FIRST in each
# pipeline module is enough to arm the whole set.


def ensure_target():
    """Point cores at win95 + cd into a data root, idempotently (mirrors
    msvc2cov._setup so a tool works with no `wt` wrapper)."""
    roots = ([os.path.join(os.environ["WT_DATA_ROOT"], "win95")]
             if os.environ.get("WT_DATA_ROOT") else [])
    roots += [paths.data_root(), os.getcwd()]
    for r in roots:
        if os.path.isdir(os.path.join(r, "files")) or os.path.isdir(os.path.join(r, "src")):
            os.chdir(r)
            return


def carve(module):
    ensure_target()
    if module not in _CARVE:
        from codebreaker.base import registry as _registry
        target = _registry.get("target")
        from codebreaker.formats import pe_functions as pe_trace
        _CARVE[module] = pe_trace.PeCarve(target.resolve(module))
    return _CARVE[module]


_MD32 = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)


def disasm_whole32(code, base=0, progress_cb=None):
    """Linearly decode the blob as 32-bit code with capstone; return
    [(offset, asm_text), ...] with CONTIGUOUS coverage (undecodable bytes become
    a 1-byte '(bad)' row -> db). capstone is used instead of objdump because
    objdump's linear sweep MIS-SPLITS prefixed instructions (a segment/opsize
    prefix like `64`/`66` gets peeled onto its own row), which the byte-exact
    reconcile then cannot realign. capstone decodes each prefixed instruction as
    one unit with the correct length, so the reconcile converges cleanly.

    progress_cb(bytes_done) is called periodically so a caller can drive a
    per-section disassembly progress bar."""
    rows = []
    n = len(code)
    pos = 0
    cnt = 0
    while pos < n:
        ins = next(iter(_MD32.disasm(bytes(code[pos:pos + 15]), base + pos, count=1)),
                   None)
        if ins is None:
            rows.append((base + pos, "(bad)"))
            pos += 1
        else:
            text = ins.mnemonic if not ins.op_str else "%s %s" % (ins.mnemonic, ins.op_str)
            rows.append((base + pos, text))
            pos += ins.size
        cnt += 1
        if progress_cb and cnt % 2000 == 0:
            progress_cb(pos)
    if progress_cb:
        progress_cb(n)
    return rows


class PeSeg:
    """One PE executable section, shaped like trace.Seg for asmrecon32."""

    def __init__(self, module, section=None, carve=None, progress_cb=None):
        # carve: a pre-built PeCarve to REUSE (a multi-section PE otherwise re-runs
        # the whole capstone sweep once per section). progress_cb drives the
        # per-section disassembly bar.
        self.module = module
        self.path = target.resolve(module)
        self.pe = open_pe(self.path)
        self.base = self.pe.image_base
        secs = self.pe.code_sections()            # [(va, bytes, name)]
        if not secs:
            raise ValueError("%s: no executable section" % module)
        if section is None:                        # default: the largest .text-like
            pick = next((s for s in secs if s[2] == ".text"), None) or \
                max(secs, key=lambda s: len(s[1]))
        else:
            pick = next((s for s in secs if s[2] == section), None)
            if pick is None:
                raise ValueError("%s: no section %r" % (module, section))
        self.va, self.code, self.name = pick
        self.secva = self.va                       # image VA of section start
        self.rows = disasm_whole32(self.code, progress_cb=progress_cb)
        self.off2idx = {o: i for i, (o, _) in enumerate(self.rows)}

        # carve functions with PeCarve (capstone, export-table driven), then keep
        # those that fall inside THIS section and rebase VA -> section offset.
        # Reuse a caller-supplied carve when given (avoids re-sweeping every
        # section of a multi-section module).
        if carve is None:
            carve = PeCarve(self.path)
        lo, hi = self.va, self.va + len(self.code)
        self.funcs = {}
        self.export_names = {}
        for va, f in carve.funcs.items():
            if lo <= va < hi:
                s = va - self.va
                e = min(f["end"], hi) - self.va
                self.funcs[s] = {"start": s, "end": e, "name": f["name"]}
                if f["name"]:
                    self.export_names[s] = f["name"]
        self._carve = carve                        # keep for call resolution/annotate

        # base-relocation mask, section-relative (each HIGHLOW covers 4 bytes)
        self.mask = set()
        rvbase = self.va - self.base
        for rva in self.pe.base_relocs():
            o = rva - rvbase
            if 0 <= o < len(self.code):
                for k in range(4):
                    if o + k < len(self.code):
                        self.mask.add(o + k)

        # fixups: resolved call sites for annotate (site_off -> "DLL!name").
        # objdump/asmrecon don't need it for byte-exactness; annotate uses it.
        self.fixups = {}
