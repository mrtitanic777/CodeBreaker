"""module.py -- build a whole module from its reconstruction, and check what came out.

    python -m codebreaker build <module> [--out DIR] [--no-run] [--engine]

    from codebreaker.build import module
    b = module.build("lzexpand.dll")          # -> Built; b.image is the linked file
    print(b.report())

The per-function builds (build/functions.py) make a throw-away DLL to judge functions in: it
has no data, no resources, no entry table of its own. This makes the module: the same
decompiled functions, compiled the same way (functions.compile_all -- there is one compile
path), plus the module's data as C (build/data.py), its resources compiled from `.rc`
(build/resources.py) and its exports and imports (build/definitions.py), linked by OpenWatcom
into a 16-bit NE or 32-bit PE image. Then the image is checked against the original:

    format      the same kind of file, library or program as the original
    exports     every original export present, under the same ordinal and name
    imports     nothing imported that the original does not import
    resources   the same resources (build/resources.compare)
    runs        loaded in the emulator, each exported function is called with zero arguments
                and what it did is set beside what the original does

What "built" means here, plainly: a function the compiler rejected is in the image as a
stand-in that returns 0, and so is anything called that has no reconstruction; a module can
link, export everything and still do little. The report counts both. The check is of the
container, not of behaviour -- behaviour is what verify/ judges, one function at a time.

Toolchain facts this relies on (each cost a failed build once):

  * OpenWatcom's tools are started by full path: Windows searches the caller's PATH, not the
    environment handed to the tool.
  * 16-bit: the C library to link is lib286/win/clib?.lib (the same file as the source tree's
    bld/clib/library/windows.086/m?/clib?.lib). lib286/clibl.lib one level up is an empty
    stub, and it is found first if lib286 is on the library path before lib286/win.
  * 16-bit: floating point needs -fpi87 (the emulator library lacks the fix-up symbols -fpi
    asks for, and -fpc does not work in this build).
  * 16-bit: a module is a library because the linker is told `format windows dll` -- that sets
    the header's library flag without the stock start-up object, which wants a LIBMAIN. The
    header flag, not the file name, says what the original was.
  * 16-bit: one segment holds 64 KB, and a near call cannot leave its segment, so each function
    is compiled into a code segment named for the original segment it came from (-nt=). The
    compiler's own helpers (32-bit multiply and divide, ...) then cannot be near either: the
    functions are compiled for the medium model (-mm: far code, near data -- the per-function
    builds use -ms, which is the same for data) and linked with the medium-model library.
  * 16-bit: the data object is linked first, so it starts at offset 0 of the automatic data
    segment and the code's offsets into it stay right.
  * Linker symbols holding `@`, `?` or `$` are quoted (unquoted, `@` means "read a file").
  * Exports are tied to the function at the export's address, and the entry point to the
    function at the original's entry address; a library with no entry point is not loadable
    as one, so one whose entry was not reconstructed gets a stand-in that reports success.
"""
import os
import re
import shutil
import struct
import subprocess

from codebreaker.base import cache, corpus, parallel, settings
from codebreaker.build import data, definitions, functions, resources, watcom
from codebreaker.decompile import c_text
from codebreaker.formats import detect, omf
from codebreaker.formats import resources as resource_reader
from codebreaker.formats.ne import NEFile
from codebreaker.formats.pe import open_pe
from codebreaker.jobs import pool

_UNDEFINED = re.compile(r"(\S+) is an undefined reference")
_BY_ADDRESS = re.compile(r"\bfunc_0x([0-9a-f]{8})\b")
_ORDINAL_IMPORT = re.compile(r"_?([A-Za-z0-9]+)_ord_(\d+)(?:@\d+)?")
_SEGMENT_OPTIONS = ((0x40, "preload", "loadoncall"), (0x10, "moveable", "fixed"), (0x1000, "discardable", ""),
                    (0x20, "shared", "nonshared"))
RUN_LIMIT = 40                          # exported functions called when a built module is run


class Built(object):
    """A build and what was found out about it."""

    def __init__(self, original):
        self.original, self.name = original, os.path.basename(original).lower()
        self.kind = corpus.lane(original)
        self.folder = self.image = None
        self.error = ""
        self.functions = self.with_text = 0     # in the module / given C by the decompiler
        self.rejected = {}                      # {function: why the compiler rejected it} -- stand-ins
        self.absent = []                        # called, but nothing reconstructed: stand-ins too
        self.assembled = {}                     # 16-bit: rejected C, written as its own instructions instead
        self.assembly_refused = {}              # 16-bit: why a rejected function could not be
        self.undefined = []                     # left unresolved in the image
        self.data = None                        # build.data.Data
        self.data_error = ""
        self.lost_addresses = 0                 # address constants in functions that could not be placed
        self.resources = None                   # build.resources.Result of writing and compiling them
        self.entry = ""
        self.format_same = False
        self.exports_total = self.exports_same = self.exports_on_stand_ins = 0
        self.exports_missing = []               # (Export, why)
        self.imports_extra = []
        self.imports_original = self.imports_built = 0
        self.resources_same = None              # None: the original has none
        self.resources_line = ""
        self.ran = []                           # (export, rebuilt exit, original exit, same result)
        self.stand_in_addresses = set()         # where, in the original, the rejected functions are

    @property
    def linked(self):
        return bool(self.image)

    def report(self):
        if not self.linked:
            return "%s: NOT BUILT: %s" % (self.name, self.error)
        size, was = os.path.getsize(self.image), os.path.getsize(self.original)
        out = ["%s: linked, %d bytes (the original is %d: %.0f%%)" % (self.name, size, was, 100.0 * size / max(1, was)),
               "  functions   %d in the module, %d with reconstructed C, %d compiled, %d rejected by the "
               "compiler (stand-ins)" % (self.functions, self.with_text, self.with_text - len(self.rejected),
                                         len(self.rejected)),
               "              %d called but not reconstructed (stand-ins), %d left unresolved" % (
                   len(self.absent), len(self.undefined))]
        d = self.data
        if self.data_error:
            out.append("  data        NOT CARRIED: %s" % self.data_error)
        elif d:
            out.append("  data        %d bytes as C, %d pointers written as pointers, %d left as numbers, "
                       "%d addresses used by the code not placed" % (d.size, d.pointers, len(d.unresolved),
                                                                         self.lost_addresses))
        if getattr(self, "code_data", None):
            out.append("  code data   the original's bytes the C reads through CS, in its segment: %s" % ", ".join(
                "%d (for %s)" % (sg, ", ".join(ns[:4]) + (" ..." if len(ns) > 4 else "")) for sg, ns in self.code_data))
        disputes = getattr(self, "disputes", None)
        if disputes:
            out.append("  conventions %d functions whose callers contradict how they are described (who removes "
                       "the arguments): %s" % (len(disputes), ", ".join("%s (callers: %s)" % (n, w)
                                                                         for n, w in disputes[:12])))
        if getattr(self, "contract_error", None):
            out.append("  contracts   NOT CHECKED: %s" % self.contract_error)
        if getattr(self, "gate_error", None):
            out.append("  gate        NOT CHECKED for %s" % self.gate_error)
        gated = getattr(self, "gated", None)
        if gated:
            out.append("  gated       %d functions' C differs from the original (rebuilt from their instructions)%s" % (
                len(gated), "".join("\n              %s: %s" % (n, why[:140]) for n, why in sorted(gated.items())[:12])))
        hidden = getattr(self, "hidden", None)
        if hidden:
            out.append("  HIDDEN      %d functions begin inside another (an entry sequence after its end) and "
                       "are not described: %s" % (len(hidden), " ".join("%d:%04X" % h for h in hidden[:16])))
        out.append("  entry       %s" % self.entry)
        out.append("  format      %s" % ("same as the original" if self.format_same else "DIFFERENT from the original"))
        out.append("  exports     %d of %d with the original's ordinal and name (%d of them point at stand-ins)%s" % (
            self.exports_same, self.exports_total, self.exports_on_stand_ins,
            "".join("\n              missing: %s @%d -- %s" % (e.name or "(no name)", e.ordinal, why)
                    for e, why in self.exports_missing[:12])))
        out.append("  imports     %d (the original has %d); %s" % (
            self.imports_built, self.imports_original,
            "all are the original's" if not self.imports_extra else
            "NOT in the original: " + ", ".join("%s.%s" % i for i in self.imports_extra[:8])))
        out.append("  resources   %s" % (self.resources_line or "the original has none"))
        if self.ran:
            ok = sum(1 for _e, mine, _o, _s in self.ran if mine == "ret")
            same = sum(1 for _e, _m, _o, s in self.ran if s)
            out.append("  runs        %d exported functions called with zero arguments: %d returned, %d did what "
                       "the original does (same end, same result)" % (len(self.ran), ok, same))
        return "\n".join(out)


def _quote(symbol):
    return "'%s'" % symbol


def _link(tool, env, folder, lines):
    """Run the linker on `lines`. -> (path of m.dll or None, everything it said)"""
    with open(os.path.join(folder, "m.lnk"), "w", newline="\n") as f:
        f.write("\n".join(lines) + "\n")
    for stale in ("m.dll", "m.map"):
        try:
            os.remove(os.path.join(folder, stale))
        except OSError:
            pass
    r = subprocess.run([tool, "@m.lnk"], cwd=folder, env=env, capture_output=True, text=True, errors="replace",
                       timeout=1800)
    out = os.path.join(folder, "m.dll")
    return (out if os.path.isfile(out) else None), (r.stdout or "") + (r.stderr or "")


def _result_in_ax(code, start, end):
    """Where a function's instructions put a value in AX that it returns with: walking back
    from each return through its straight run of instructions, the first one writing AX (or a
    call: what the callee left there goes back to the caller as it came) --
    one past the prologue's (`mov ax, ds` / `push ds; pop ax` / `mov ax, DGROUP` ahead of
    `mov ds, ax`), and not a `pop ax` (its own AX given back). -> offset, or None."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    md.detail = True
    ins = list(md.disasm(code[start:end], start))
    prologue = next((i.address + i.size for i in ins[:6] if i.mnemonic == "mov" and i.op_str == "ds, ax"), start)
    targets = set()
    for i in ins:
        if i.group(capstone.CS_GRP_JUMP) and i.operands and i.operands[0].type == capstone.CS_OP_IMM:
            targets.add(i.operands[0].imm)
    ax = {capstone.x86.X86_REG_AX, capstone.x86.X86_REG_AL, capstone.x86.X86_REG_AH, capstone.x86.X86_REG_EAX}
    for k, i in enumerate(ins):
        if i.mnemonic not in ("ret", "retf"):
            continue
        for j in range(k - 1, -1, -1):
            p = ins[j]
            if p.group(capstone.CS_GRP_CALL) and p.address >= prologue:
                return p.address                            # the callee's result, passed on as it came
            if p.address < prologue or p.mnemonic in ("jmp", "ret", "retf"):
                break
            _r, written = p.regs_access()
            if ax & set(written):
                if p.mnemonic != "pop":
                    return p.address
                break
            if p.address in targets:
                break
    return None


def _segment_number_fixups(nef):
    """Whether every fix-up the original makes to a place of its own is by the segment's
    number -- none through the entry table (a movable segment's), none a far pointer."""
    blob = nef.data
    seen = False
    for s in nef.segments():
        if not (s["flags"] & 0x100) or not s["file_off"]:
            continue
        at = s["file_off"] + (s["length"] or 0x10000)
        for k in range(struct.unpack_from("<H", blob, at)[0]):
            src, flags, _off, target = struct.unpack_from("<BBHH", blob, at + 2 + k * 8)
            if flags & 3 == 0:                                  # a place of the module's own
                seen = True
                if target & 0xFF == 0xFF or src != 2:
                    return False
    return seen


def _dos_stub(path, folder):
    """The original's DOS part (the bytes before its NE header), as a stub for the linker: its
    MZ header made to say just that much. -> the file's name, or None."""
    with open(path, "rb") as f:
        h = detect.dos_header(f.read(0x40))
        if h is None:
            return None
        f.seek(0)
        stub = bytearray(f.read(h["new_header"]))
    if len(stub) < 0x40:
        return None
    struct.pack_into("<HH", stub, 2, len(stub) % 512, (len(stub) + 511) // 512)
    with open(os.path.join(folder, "stub.exe"), "wb") as f:
        f.write(stub)
    return "stub.exe"


def _whole_file_image(original, image):
    """An original whose MZ header has DOS load the whole file (KRNL386: its real-mode start
    reads the NE header and the segments out of memory) -- ours says the same of ours."""
    size = os.path.getsize(original)
    with open(original, "rb") as f:
        last, pages = struct.unpack_from("<HH", f.read(6), 2)
    if pages * 512 < size - 511:                               # (a stub alone: nothing to do)
        return
    with open(image, "r+b") as f:
        n = os.fstat(f.fileno()).st_size
        f.seek(2)
        f.write(struct.pack("<HH", n % 512, (n + 511) // 512))


def _link_resolving(b, tool, env, folder, lines, stand_in, compile_extra):
    """Link; give whatever is undefined a stand-in and link again; whatever is still undefined
    after that is left unresolved (and reported). `stand_in(symbol)` -> C text or None."""
    image, log = _link(tool, env, folder, lines)
    names = sorted(set(_UNDEFINED.findall(log)))
    if names:
        # `__imp__Name@N` is asked for by the compiler's own runtime library, pulled in by a
        # library routine the functions use: it needs those system functions for itself. They
        # come from the import libraries, and show up in the check as imports the original
        # does not have.
        if any(s.startswith("__imp_") for s in names):
            lines = lines + ["library kernel32.lib, user32.lib, gdi32.lib, advapi32.lib"]
            names = [s for s in names if not s.startswith("__imp_")]
        texts = [(s, t) for s, t in ((s, stand_in(s)) for s in names) if t]
        objects = []
        if texts and not compile_extra("absent", "\n".join(t for _s, t in texts)):
            objects, b.absent = ["absent.obj"], [s for s, _t in texts]
        else:                                                 # one of them does not compile: each on its own
            for i, (s, t) in enumerate(texts):
                if not compile_extra("absent%03d" % i, t):
                    objects.append("absent%03d.obj" % i)
                    b.absent.append(s)
        lines = [ln for ln in lines if not ln.startswith("library ")] + ["file %s" % o for o in objects] + \
            [ln for ln in lines if ln.startswith("library ")]
        image, log = _link(tool, env, folder, lines)
        b.undefined = sorted(set(_UNDEFINED.findall(log)))
        if b.undefined:
            image, log = _link(tool, env, folder, lines + ["option undefsok"])
    if not image:
        b.error = "link: " + (log.strip()[-400:] or "the linker produced nothing")
    return image


def _source_of(fn):
    import inspect
    return inspect.getsource(fn)


def _assembled16(b, nef, at, segs, stems, public, folder, env, carried=(), forced=(), elsewhere=None):
    """Assembly of the module's own instructions where C does not do (build/asm16.py): each code
    segment's lead-in -- what is before its first function, often a variable the code reads
    through CS -- linked first into the segment so that it stays where it was; and each rejected
    function, instead of a stand-in, when it can be written so and assembles back to the
    original's bytes. -> {import symbol without its underscore: (module, what)} they need."""
    import pickle
    from codebreaker.build import asm16
    from codebreaker.source import assembly as _assembly
    from codebreaker.build import msc6 as _msc6
    wasm = watcom.ow_tool("wasm")
    asm_stamp = cache.key(cache.tool(wasm) if wasm else "", open(asm16.__file__, "rb").read(),
                          open(_assembly.__file__, "rb").read(), open(_msc6.__file__, "rb").read(),
                          _source_of(_assembled16))         # (this function's own code: the rounds are in it)
    b.lead_ins, b.placed_c, b.moved_c, b._publics = [], set(), set(), dict(public)
    if not wasm:
        return {}
    where = {name: place for place, name in at.items()}
    starts = {}
    for (seg, off) in at:
        starts.setdefault(seg, []).append(off)
    for seg in starts:
        starts[seg].sort()
    in_segment = {}
    for (seg, _off), name in sorted(at.items()):
        if name in public and name not in b.rejected:
            in_segment.setdefault(seg, public[name][0])

    entries = {}                                           # (segment, offset) -> symbol: a place called into mid-run

    def symbol_for(kind, *what):
        if kind == "function":
            name = at.get(what)
            if name in public:
                return public[name][0]
            if what not in entries and name in stems:
                # (a function built on its own -- from C, or assembled by itself -- by the name it
                # links under: NETDI's 9:0000 and 48:0000, far pointers to them in assembly
                # beside -- with no symbol, 76 functions of four segments were refused as assembly)
                return "_" + name
            return entries.get(what)
        if kind == "segment":
            # (any symbol in the segment gives its selector -- a function of it built on its own
            # too: NETDI's segments 9 and 48 hold one each, written in assembly, and the selectors
            # of them in the code beside had no symbol)
            return in_segment.get(what[0]) or next(
                ("_" + n for (sg, _o), n in sorted(at.items()) if sg == what[0] and n in stems), None)
        if kind == "data":                                   # a data segment: the auto one, or one carried
            if what[0] == nef.h.get("autodata_seg"):
                return "_codebreaker_data"
            return "_codebreaker_segment_%d" % what[0] if what[0] in carried else None
        mod, which = what
        return "_%s_%s" % (re.sub(r"\W", "_", mod), which) if isinstance(which, int) else "_" + re.sub(r"\W", "_", which)
    out = {}
    # (from nothing: what an earlier build left there is read below -- which far function may
    # move to the segment beside depends on what the pieces refer to -- and would decide it)
    shutil.rmtree(os.path.join(folder, "asm"), ignore_errors=True)
    os.makedirs(os.path.join(folder, "asm"), exist_ok=True)

    def assemble_piece(stem, f, lo, hi):
        code, msg = _wasm(wasm, env, folder, stem, f.write(f.good, lo, hi))
        obj = os.path.join(folder, stem + ".obj")
        if code or not os.path.isfile(obj):
            return "the assembler: " + msg.strip()[-160:]
        with open(obj, "rb") as fh:
            return asm16.check_piece(fh.read(), f, lo, hi)

    def assemble(stem, f):
        """Written as readable as the assembler allows: each instruction as its mnemonic where the
        probe says the assembler gives back the original's bytes, and round after round any
        that did not come back as they were go back to bytes. -> "" or why it could not be."""
        from codebreaker.build import msc6
        from codebreaker.source import assembly
        # (what the rounds came to is kept by what decides it -- the module, where the piece is,
        # its names and holes, this code and the assembler: the same piece in a later build is
        # written once, as it ended, and not probed and assembled round after round again)
        k = cache.key(f.text, f.start, f.end, asm_stamp)
        had = cache.get("assembled16", k)
        if had is not None:
            why, good = pickle.loads(had)
            f.good = set(good)
            if why:
                return why
            code, _msg = _wasm(wasm, env, folder, stem, f.write(f.good))
            obj = os.path.join(folder, stem + ".obj")
            if not code and os.path.isfile(obj):                # (checked again: the original's bytes)
                with open(obj, "rb") as fh:
                    got, mask = msc6.object_code(fh.read())
                if len(got) == f.end - f.start and not asm16.differences(got, mask, f):
                    return ""
        result = _assemble_rounds(stem, f, msc6, assembly)
        cache.put("assembled16", k, pickle.dumps((result, sorted(f.good))))
        return result

    def _assemble_rounds(stem, f, msc6, assembly):
        cand = f.candidates()
        ok = assembly.probe(list(cand.values()), set(), folder, mode="real16") if cand else set()
        good = {at for at, c in cand.items() if c in ok} | {at for at, t in f.ins.items() if t[3] is not None}
        why = "no rounds"
        f.good = good
        for _round in range(40):
            code, msg = _wasm(wasm, env, folder, stem, f.write(good))
            obj = os.path.join(folder, stem + ".obj")
            if code or not os.path.isfile(obj):
                lines = f.write(good).split("\n")
                bad = {ln.strip() for m in re.finditer(r"\((\d+)\): Error", msg) for ln in lines[int(m.group(1)) - 1:int(m.group(1))]}
                drop = {at for at in good if at in f.ins and f.mnemonic(at) in bad}
                if not drop:
                    return "the assembler: " + msg.strip()[-160:]
                good -= drop
                continue
            with open(obj, "rb") as fh:
                code, mask = msc6.object_code(fh.read())
            if len(code) == f.end - f.start:
                diff = asm16.differences(code, mask, f)
                if not diff:
                    return ""
                at = diff[0]
                hit = [a for a in good if a <= at < a + len(f.ins[a][0])] or \
                      [max([a for a in good if a <= at] or [None])]
                why = "differs at %04x" % at
            else:
                # longer or shorter: the first instruction whose bytes do not line up is blamed
                hit, why = [], "%d bytes, the original %d" % (len(code), f.end - f.start)
                for a in sorted(good):
                    n = len(f.ins[a][0])
                    if code[a - f.start:a - f.start + n] != f.blob[a:a + n]:
                        hit = [a]
                        break
            hit = [a for a in hit if a is not None]
            if not hit:
                return why
            good -= set(hit)
        return why
    lead = {}
    # A code segment with assembly in it is rebuilt as one frame from its first byte: the
    # assembly, and the lead-in, stay where they were -- hand-written code reaches into its own
    # segment by number (tables, variables, LODS through CS) -- and where each function rebuilt
    # from C was is filler: those are placed after the frame and called by their names.
    whole = {seg for seg in starts if seg in segs and segs[seg]["kind"] == "CODE" and
             any(at[(seg, o)] in forced for o in starts[seg])}
    for seg in sorted(starts):
        first = starts[seg][0]
        if first and seg in segs and segs[seg]["kind"] == "CODE" and seg not in whole:
            f = asm16.Function(nef, seg, 0, first, "codebreaker_lead_%d" % seg, symbol_for)
            if f.text is not None and not assemble("asm/lead%02d" % seg, f):
                lead[seg] = first
                b.lead_ins.append("asm/lead%02d" % seg)
                out.update({sym[1:]: w for sym, w in f.imports.items()})
    # the functions written in assembly, each run of them in a segment together -- as the file
    # they were written in had them: a jump from one into the next is a jump to a label -- and
    # each rejected C function on its own
    forced = set(forced)
    done = {}                                              # run (tuple of names) -> stem, or why it was refused
    raw_reached = {}                                       # name -> where the frame's bytes call it
    for _pass in range(40):                                # (each pass may find a place entered mid-run: its run is written again)
        runs = []
        for seg in sorted(whole):                          # one frame per such segment
            mine = tuple(at[(seg, o)] for o in starts[seg]
                         if at[(seg, o)] in public and (at[(seg, o)] in forced or at[(seg, o)] in b.rejected))
            if mine:
                runs.append(mine)
        for seg in sorted(starts):
            if seg in whole:
                continue
            run = []
            for off in starts[seg]:
                name = at[(seg, off)]
                if name in forced and name in public:
                    run.append(name)
                    continue
                if run:
                    runs.append(tuple(run))
                run = []
                if name in b.rejected and name in public:
                    runs.append((name,))
            if run:
                runs.append(tuple(run))
        joined, wanted = set(), set()

        def one(run):
            """A run written and assembled. -> (run, Function) -- many at once: each its own files"""
            seg, off = where[run[0]]
            last = where[run[-1]][1]
            later = [o for o in starts[seg] if o > last]
            end = later[0] if later else segs[seg]["length"] if seg in segs else last
            holes = []
            if seg in whole:
                off, end = 0, segs[seg]["length"]
                names = set(run)
                for k, o in enumerate(starts[seg]):
                    n = at[(seg, o)]
                    if n not in names:
                        hi = starts[seg][k + 1] if k + 1 < len(starts[seg]) else end
                        holes.append((o, hi, public[n][0] if n in public else "_" + n))
            publics = [(where[n][1], public[n][0]) for n in run]
            publics += [(o, sym) for (sg, o), sym in entries.items() if sg == seg and off <= o < end]
            f = asm16.Function(nef, seg, off, end, publics, symbol_for, cs_ok=lead.get(seg, 0),
                               extra_entries=[o for (sg, o) in entries if sg == seg and off <= o < end],
                               whole=seg in whole, holes=holes)
            f.assembled = None
            if f.text is not None:
                f.assembled = assemble("asm/" + stems[run[0]].replace("/", "_"), f)
            return run, f

        made = pool.threads(one, [r for r in runs if r not in done], jobs=parallel.workers())
        for run, f in made:
            seg = where[run[0]][0]
            for place in f.raw_far_entries:                # (from its bytes: a far call into a middle)
                if place not in entries:
                    wanted.add(place)
            if f.text is None and (f.missing or f.far_missing):
                # it calls into the middle of a run of assembly (in its segment, or far in
                # another): that place is an entry of its own
                for place in {(seg, t) for t in f.missing} | f.far_missing:
                    if place not in entries:
                        wanted.add(place)
                continue
            if f.text is None and f.reaches is not None:
                # it jumps into the middle of a function beside it: that one was written in
                # assembly too (nothing in its own code said so); the run takes it in
                inside = max([o for o in starts.get(seg, ()) if o <= f.reaches] or [None])
                if inside is not None and at[(seg, inside)] not in forced and at[(seg, inside)] in public:
                    joined.add(at[(seg, inside)])
                    continue
            why = f.why if f.text is None else f.assembled
            done[run] = ("why", why) if why else ("stem", "asm/" + stems[run[0]].replace("/", "_"), f)
        for seg, t in wanted:
            # (a segment no function is known to start in has nothing to give an entry: KRNL386's
            # segment 4 and GDI's 45, reached far from a run's bytes -- the build stopped there)
            inside = max([o for o in starts.get(seg, ()) if o <= t] or [None])
            name = at.get((seg, inside)) if inside is not None else None
            if name is None:
                continue
            if name in forced:
                entries[(seg, t)] = "_sub_%d_%04X" % (seg, t)
                for run in [r for r in done if name in r]:
                    del done[run]                          # its run is written again, with the entry
            elif name in public:
                joined.add(name)                           # called into mid-body: written in assembly
        if not joined and not wanted:
            break
        for name in joined:
            if name in b.origins:
                b.origins[name].language = "assembly"
                b.origins[name].evidence.append(
                    "called where it was from bytes of the assembly beside it (%04x)" % raw_reached[name]
                    if name in raw_reached else "entered in its middle from assembly beside it")
        forced |= joined
    claimed = set()
    for run, result in done.items():
        if result[0] != "stem" or claimed & set(run):
            continue
        if any(n not in forced and n not in b.rejected for n in run):
            continue
        claimed |= set(run)
        seg = where[run[0]][0]
        if seg in whole:
            placed = _frame_pieces(b, result[2], seg, stems, folder, wasm, env, assemble_piece, elsewhere)
            if placed is None:
                for name in run:
                    b.assembly_refused[name] = "its frame's pieces do not assemble back: " + getattr(b, "frame_why", "")
                b.placed_c = set()
                continue
            b.lead_ins += placed                           # first into its segment, in order
        for name in run:
            _assembled_one(b, name, stems, placed[0] if seg in whole else result[1])
        out.update({sym[1:]: w for sym, w in result[2].imports.items()})
    for run, result in done.items():
        if result[0] == "why":
            for name in run:
                if name not in claimed:
                    b.assembly_refused[name] = result[1]
    return out


_REGISTER_IN = re.compile(r"\b(?:in|unaff)_(AL|AH|AX|BL|BH|BX|CL|CH|CX|DL|DH|DX|SI|DI)\b")
_WHOLE = {"al": "a", "ah": "a", "ax": "a", "eax": "a", "bl": "b", "bh": "b", "bx": "b", "ebx": "b",
          "cl": "c", "ch": "c", "cx": "c", "ecx": "c", "dl": "d", "dh": "d", "dx": "d", "edx": "d",
          "si": "si", "esi": "si", "di": "di", "edi": "di"}      # (a register by its whole: AL, AH and AX are A)


def _registers_c_cannot_carry(nef, segs, at, texts):
    """Functions whose C cannot say what the original passes or returns in a register.
    -> {name: why}
    - It sets a register just before calling one that reads that register as it comes in
      (USER's CallMsgFilter, sub_1_80CD: `mov bx, 6` / `mov bx, -1`, then the hook caller
      sub_1_800A, which takes the hook type in BX; the C passed no BX, the caller looked up a
      hook record at 1 and jumped to a selector D400 -- a GP on Setup's License page).
    - It returns a word, but the original sets DX just before it returns: a DX:AX result
      (sub_1_81C8 returns es:[38h]:1Eh, the C 1Eh alone)."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    starts, takes, by_offset = {}, {}, {}
    for (sg, o), n in at.items():
        starts.setdefault(sg, []).append(o)
        by_offset.setdefault(o, []).append((sg, n))
        regs = {_WHOLE[r.lower()] for r in _REGISTER_IN.findall(texts.get(n, "").split("{", 1)[-1])}
        if regs:
            takes.setdefault(o, []).append((sg, n, regs))
    for sg in starts:
        starts[sg].sort()
    images = {}
    out = {}
    leaves_ds, calls_of = set(), {}                      # (and who calls each: a caller of one that
    for (sg, o), n in at.items():                       # leaves DS changed counts on that too)
        text = texts.get(n)
        if text is None or sg not in segs:
            continue
        if sg not in images:
            s = segs[sg]
            images[sg] = bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]])
        code = images[sg]
        later = [x for x in starts[sg] if x > o]
        ins = list(md.disasm(code[o:later[0] if later else len(code)], o))
        wide = re.search(r"\b(?:int4|uint4|xunknown4|undefined4)\b|__far\s*\*", text.split("(", 1)[0])
        # (it loads DS and never puts it back: it leaves the caller another DS -- sub_1_222B's
        # `mov ds, ax; ret`, KERNEL's sub_2_0170 `mov ds, cs:[16Eh]; ret`. Not USER's copy
        # helper sub_1_39E2: `mov dx, ds` ... `mov ds, dx` puts it back)
        if _leaves_ds(ins):
            leaves_ds.add(n)
            out.setdefault(n, "it leaves DS changed for its caller")
        for k, i in enumerate(ins):
            prev = ins[max(0, k - 4):k]
            if i.mnemonic in ("call", "lcall"):
                try:
                    target = int(i.op_str.split(",")[-1].strip(), 0)
                except ValueError:
                    continue
                for csg, callee in by_offset.get(target, []):
                    if not (i.mnemonic == "call" and csg != sg) and callee != n:
                        calls_of.setdefault(callee, set()).add(n)
                for csg, callee, regs in takes.get(target, []):
                    if (i.mnemonic == "call" and csg != sg) or callee == n:
                        continue
                    # (by the whole register: SUWIN's 7:0056 `mov ax, es:[si]` / `call 0`, and
                    # 7:0000 compares AL with the character looked for -- the C passed no AL, found
                    # no dot, and Setup set MSDOS.SYS aside as WINBOOT.---: Windows never started)
                    set_ = [p for p in prev[-3:] if _WHOLE.get(p.op_str.split(",")[0].strip()) in regs
                            and p.mnemonic in ("mov", "xor", "sub", "lea", "les", "lds", "add", "or")]
                    if set_:
                        out.setdefault(n, "it passes %s to %s in a register (%04x)" % (
                            set_[-1].op_str.split(",")[0].strip().upper(), callee, i.address))
                # ... and the caller reads ES the callee left: an ES result no C returns
                # (USER's sub_1_281C leaves ES = the task's queue, and its caller clears a
                # flag at es:[30h]; the C left ES at another segment, the flag stayed set and
                # Setup sat idle on its Options page)
                nxt = ins[k + 1] if k + 1 < len(ins) else None
                # ... and the caller branches on the flags the callee leaves: a result no C
                # returns, and a test no C can write (USER's sub_1_222B: `or ax, ax; ret`, its
                # callers `call 222B; je` -- the C returned nothing, its callers tested AX left
                # from elsewhere)
                if nxt is not None and nxt.mnemonic.startswith("j") and nxt.mnemonic not in ("jmp", "jcxz", "jecxz"):
                    for csg, callee in by_offset.get(target, []):
                        if not (i.mnemonic == "call" and csg != sg) and callee != n:
                            out.setdefault(callee, "it returns in the flags (%s branches on them, %04x)" % (n, nxt.address))
                            out.setdefault(n, "it branches on the flags %s leaves (%04x)" % (callee, nxt.address))
                if nxt is not None and re.search(r"\bes\b", nxt.op_str) \
                        and not re.match(r"\s*es\s*,", nxt.op_str) and nxt.mnemonic not in ("les", "pop"):
                    for csg, callee in by_offset.get(target, []):
                        if not (i.mnemonic == "call" and csg != sg) and callee != n:
                            out.setdefault(callee, "its callers read the ES it returns (%s, %04x)" % (n, nxt.address))
            elif i.mnemonic in ("ret", "retf") and not wide:
                if any((p.mnemonic in ("mov", "xor", "sub", "les", "lds") and p.op_str.startswith("dx,"))
                       or p.mnemonic == "cwd" for p in prev):
                    out.setdefault(n, "it returns DX:AX where its C returns a word (%04x)" % i.address)
    # (a caller of one that leaves DS changed: the original puts its own DS back after it --
    # KERNEL's sub_2_2437 `push ds; call 0170 ... pop ds`; its C kept the kernel's DS and the
    # caller after it faulted, "Invalid DPMI return from 0367:2BC3")
    for callee in leaves_ds:
        for caller in calls_of.get(callee, ()):
            out.setdefault(caller, "it calls %s, which leaves DS changed" % callee)
    return out


def _leaves_ds(ins):
    """Whether a function's instructions load DS and do not put it back: a `mov ds, x` from
    anything but a copy taken of DS (`mov dx, ds`), with no `pop ds` or restoring `mov ds`
    after it."""
    saved, changed = set(), False
    for x in ins:
        if x.mnemonic == "mov" and re.fullmatch(r"(\w+), ds", x.op_str):
            saved.add(x.op_str.split(",")[0])
        elif x.mnemonic == "mov" and x.op_str.startswith("ds,"):
            src = x.op_str.split(",", 1)[1].strip()
            changed = src not in saved
        elif x.mnemonic == "pop" and x.op_str == "ds":
            changed = False
        elif x.mnemonic == "lds":
            changed = True
    return changed


def _pack_holes(holes, size, near, aside=(), limit=None):
    """The functions rebuilt from C into the frame's holes: each in its own hole first, when it
    fits; the rest packed into what room is left (near ones first, then the biggest). `holes`:
    [(start, end, name)]; `size`: {name: bytes of code}; `aside`: far ones sent elsewhere first;
    `limit`: where the segment's resident part ends -- a function from below it goes only into
    a hole that ends below it too (KERNEL keeps only the first A940h bytes of its segment 1).
    -> ([[start, end, [(pad, name)], used]], far names with no room, near names with no room)"""
    bins, rest = [], []
    for lo, hi, name in holes:
        bins.append([lo, hi, [], lo])
        if name in size and name not in aside:
            pad = bins[-1][3] & 1                          # (OpenWatcom aligns its code to a word)
            if pad + size[name] <= hi - lo:
                bins[-1][2].append((pad, name))
                bins[-1][3] += pad + size[name]
            else:
                rest.append(name)
    own = {name: lo for lo, _hi, name in holes}
    no_room_far, no_room_near = [], []
    for name in sorted(rest, key=lambda n: (n not in near, -size[n])):         # (near ones first)
        for bn in bins:
            if limit is not None and own.get(name, 0) < limit and bn[1] > limit:
                continue
            pad = bn[3] & 1
            if bn[3] + pad + size[name] <= bn[1]:
                bn[2].append((pad, name))
                bn[3] += pad + size[name]
                break
        else:
            (no_room_near if name in near else no_room_far).append(name)
    return bins, no_room_far, no_room_near


def _resident_limit(code):
    """Where a self-loading KERNEL cuts its own code segment after start-up: GlobalReAlloc of
    CS to `mov cx, size` -- `mov cx, N / push cs / push dx / push cx / push dx`. -> N or None"""
    for at in range(len(code) - 8):
        if code[at] == 0xB9 and code[at + 3:at + 8] == b"\x0e\x52\x51\x52\x0e":
            return code[at + 1] | code[at + 2] << 8
    return None


def _frame_pieces(b, f, seg, stems, folder, wasm, env, assemble_piece, elsewhere=None):
    """The frame of a segment with assembly in it, in the order the linker is to lay it out:
    its pieces at the original's places, and in the places of the functions rebuilt from C
    (the frame's holes) those functions -- each in its own place when it fits there, and the
    ones that do not, packed into what room the holes have left (biggest first; a near call
    reaches anywhere in the segment), filler for the rest. A far one with no room goes to the
    segment beside (CODE<n>X), a near one after the frame. -> [object stem] or None."""
    from codebreaker.build import msc6
    name_of = {sym: n for n, (sym, _o) in getattr(b, "_publics", {}).items()}
    size, cstem = {}, {}
    for lo, hi, sym in f.holes:
        name = name_of.get(sym)
        st = stems.get(name) if name else None
        if st and not st.startswith("asm/"):
            try:
                with open(os.path.join(folder, st + ".obj"), "rb") as fh:
                    size[name] = len(msc6.object_code(fh.read())[0])
                cstem[name] = st
            except OSError:
                pass
    near = getattr(b, "near_names", set())

    limit = getattr(b, "resident_limit", {}).get(seg)

    def place(aside):
        bins, no_room_far, no_room_near = _pack_holes(
            [(lo, hi, name_of.get(sym)) for lo, hi, sym in f.holes], size, near, aside, limit)
        return [[lo, hi, [(pad, cstem[n]) for pad, n in objs], used] for lo, hi, objs, used in bins], \
            no_room_far, no_room_near

    # A near function with no room goes after the frame and makes the segment bigger than the
    # original's -- past 64K, in the end. A far one can go to the segment beside instead: the
    # far ones give up their places, largest first, while near ones are still left over.
    aside = set()
    bins, no_room_far, no_room_near = place(aside)
    # (no longer done: every function stays in its own segment -- a segment the original does not
    # have renumbers the selectors of all that loads after it. The link moves far ones out of a
    # segment only past 64K.)
    while no_room_near and elsewhere and os.environ.get("CB_ASIDE"):
        placed_far = sorted((n for n in size if n not in near and n not in aside and n not in no_room_far),
                            key=lambda n: -size[n])
        if not placed_far:
            break
        aside.add(placed_far[0])
        bins, no_room_far, no_room_near = place(aside)
    for lo, hi, sym in f.holes:
        name = name_of.get(sym)
        b.frame_fit = getattr(b, "frame_fit", []) + [(name, hi - lo, size.get(name, 0))]
    after = []
    for name in sorted(aside) + no_room_far + no_room_near:
        # (only those that gave up their places for near ones go to the segment beside; a far one
        # that just did not fit its own place goes after the frame, as a near one does: a segment
        # the original does not have renumbers the selectors of all that loads after it -- USER's
        # sub_39_05AC, sent to a new segment 46, broke Setup with sub_7_03F8 rebuilt from C as
        # well, though it never ran. The link moves far ones out only past 64K.)
        moved = elsewhere(name) if elsewhere and name in aside else None
        if moved:                                          # far: into the segment beside this one
            stems[name] = moved
            b.moved_c.add(name)
        else:
            after.append(cstem[name])                      # near: after the frame, in this segment
            own = next((lo for lo, _hi, sym in f.holes if name_of.get(sym) == name), None)
            if limit is not None and own is not None and own < limit:
                # (past where the segment is cut after start-up: the build is done again with it
                # as its instructions -- see _resident_limit)
                b.resident_overflow = getattr(b, "resident_overflow", {})
                b.resident_overflow[name] = ("its C finds no room below where segment %d is cut (%04x)"
                                             % (seg, limit))
    for name in size:
        if name not in b.moved_c:
            b.placed_c.add(name)
    out, at = [], f.start
    for k, (lo, hi, objs, used) in enumerate(bins + [[f.end, f.end, [], f.end]]):
        if at < lo:
            stem = "asm/frame%02d_%02d" % (seg, k)
            why = assemble_piece(stem, f, at, lo)
            if why:
                b.frame_why = "%s (%04x-%04x): %s" % (stem, at, lo, why)
                return None
            out.append(stem)
        if lo == hi:
            break
        pos = lo
        for pad, st in objs:
            if pad:
                out.append(_filler(folder, wasm, env, seg, "pad%02d_%04x" % (seg, pos), pad))
            out.append(st)
            pos = pos + pad + size[name_of_stem(st, cstem)]
        if hi > used:
            out.append(_filler(folder, wasm, env, seg, "fill%02d_%04x" % (seg, used), hi - used))
        at = hi
    return out + after


def name_of_stem(stem, cstem):
    return next(n for n, st in cstem.items() if st == stem)


def _wasm(wasm, env, folder, stem, text, flags=("-q", "-zcm")):
    """Assemble `text` as `stem`.asm into `stem`.obj -- what the assembler made of this text with
    these options is kept (base/cache.py): a piece the same as in an earlier build, or earlier in
    this one, is not assembled again. -> (0 or the assembler's exit code, its messages)"""
    from codebreaker.base import cache
    with open(os.path.join(folder, stem + ".asm"), "w", newline="\n") as fh:
        fh.write(text)
    obj = os.path.join(folder, stem + ".obj")
    k = cache.key(cache.tool(wasm), list(flags), text)
    had = cache.get("wasm16", k)
    if had is not None:
        if had[:2] == b"OK":
            with open(obj, "wb") as fh:
                fh.write(had[2:])
            return 0, ""
        return 1, had[2:].decode("utf-8", "replace")
    try:
        os.remove(obj)
    except OSError:
        pass
    r = subprocess.run([wasm] + list(flags) + ["-fo=" + stem + ".obj", stem + ".asm"], cwd=folder, env=env,
                       capture_output=True, text=True)
    msg = (r.stdout or "") + (r.stderr or "")
    if r.returncode == 0 and os.path.isfile(obj):
        with open(obj, "rb") as fh:
            cache.put("wasm16", k, b"OK" + fh.read())
        return 0, msg
    cache.put("wasm16", k, b"ER" + msg.encode("utf-8", "replace"))
    return r.returncode or 1, msg


def _near_pointer_tables(nef, at):
    """The functions whose addresses sit in the automatic data segment as near pointers -- a table
    of handlers the code calls through (`mov di, 776h` / `call word ptr [di]`): SETUPX's CONFIG.SYS
    and AUTOEXEC.BAT line handlers, DS:0776, ten offsets in segment 3. A near pointer has no
    relocation, so nothing tells the build it is code: a handler rebuilt from C that moves leaves the
    table pointing at whatever now sits there -- sub_3_36DE moved to 2A3C, TPWRITEPROFILESTRING went
    into its place, and the table's second entry ran it (a far return from a near call: a fault in
    SETUPX at 0003:3700 as Setup began to copy). A run of three or more words in a row that are all
    function starts of one code segment, or two that the segment's code loads as the table's address,
    and that segment calls or jumps through a word in memory."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    segs = {s["idx"]: s for s in nef.segments()}
    auto = nef.h.get("autodata_seg")
    if not auto or auto not in segs or not segs[auto]["file_off"]:
        return set()
    ds = segs[auto]
    data = bytes(nef.data[ds["file_off"]:ds["file_off"] + ds["length"]])
    starts = {}
    for (sg, off), name in at.items():
        starts.setdefault(sg, {})[off] = name
    out = set()
    for sg, names in starts.items():
        s = segs.get(sg)
        if not s or not s["file_off"] or len(names) < 2:
            continue
        code = bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]])
        indirect, immediates = False, set()
        for i in md.disasm(code, 0):
            if i.mnemonic in ("call", "jmp") and i.op_str.startswith("word ptr ["):
                indirect = True
            if i.mnemonic == "mov" and i.op_str.count(", 0x") == 1 and i.op_str[:2] in ("bx", "si", "di", "ax", "cx", "dx"):
                immediates.add(int(i.op_str.split(", ")[1], 16))
        if not indirect:
            continue
        for align in (0, 1):
            run = []
            for p in range(align, len(data) - 1, 2):
                w = struct.unpack_from("<H", data, p)[0]
                if w in names and w:
                    run.append((p, names[w]))
                    continue
                if len(run) >= 3 or (len(run) >= 2 and run[0][0] in immediates):
                    out.update(n for _p, n in run)
                run = []
            if len(run) >= 3 or (len(run) >= 2 and run and run[0][0] in immediates):
                out.update(n for _p, n in run)
    return out


def _register_argument_calls(nef, at):
    """The functions whose original loads AX from its frame just before a far call through a
    pointer in its frame -- `mov ax, [bp-8]` / `call dword ptr [bp-6]`: a VxD's protected-mode API
    entry (INT 2Fh AX=1684h), the service number in AX and the arguments read from the caller's
    frame as it is (SETUPX's CONFIGMG wrappers 4:13E6, 4:1470, 4:149E ...). The decompiler drops
    the register and C has no way to set it, nor to lay the frame out as the entry reads it."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    segs = {s["idx"]: s for s in nef.segments()}
    by_seg = {}
    for (sg, off), name in at.items():
        by_seg.setdefault(sg, []).append((off, name))
    out = set()
    for sg, lst in by_seg.items():
        s = segs.get(sg)
        if not s or not s["file_off"]:
            continue
        code = bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]])
        lst.sort()
        for k, (off, name) in enumerate(lst):
            end = lst[k + 1][0] if k + 1 < len(lst) else len(code)
            prev = None
            for i in md.disasm(code[off:end], off):
                if i.mnemonic == "lcall" and i.op_str.startswith("[bp - ") and prev is not None \
                        and prev.mnemonic == "mov" and prev.op_str.startswith("ax, word ptr [bp - "):
                    out.add(name)
                    break
                prev = i
    return out


def _filler(folder, wasm, env, seg, stem, n):
    """An object of `n` bytes of INT 3 in code segment `seg`: a place kept where it was."""
    stem = "asm/" + stem
    _wasm(wasm, env, folder, stem, "CODE%d segment byte public 'MCODE'\n        db   %d dup (0CCh)\nCODE%d ends\nend\n"
          % (seg, n, seg), flags=("-q",))
    return stem


def _assembled_one(b, name, stems, stem):
    """Record a function as built from assembly (`stem`)."""
    stems[name] = stem
    o = getattr(b, "origins", {}).get(name)
    b.assembled[name] = ("written in assembly: %s" % "; ".join(o.evidence[:2])) if o is not None and \
        o.language == "assembly" else "its C does not compile: %s" % b.rejected.get(name, "")
    b.rejected.pop(name, None)


def _publics(folder, stems):
    """{function name: (object-file symbol, object path relative to folder)}"""
    out = {}
    for name, stem in stems.items():
        try:
            with open(os.path.join(folder, stem + ".obj"), "rb") as f:
                body = f.read()
        except OSError:
            continue
        k = cache.key(body)                                  # (an object's publics: read once, kept by its bytes)
        had = cache.get("omf_publics", k)
        if had is None:
            symbols = omf.publics(body)
            cache.put("omf_publics", k, "\n".join(sorted(symbols)).encode("utf-8"))
        else:
            symbols = set(had.decode("utf-8").split("\n")) - {""}
        # as the compiler decorates a C name: _name (__cdecl, or __stdcall with an open argument
        # list), _name@N (__stdcall), name_ (register arguments)
        symbol = next((s for s in sorted(symbols) if s in ("_" + name, name, name + "_") or
                       re.fullmatch(r"[_@]%s@\d+" % re.escape(name), s)), None)
        if symbol:
            out[name] = (symbol, stem + ".obj")
    return out


def _from_header(header, symbol):
    """A stand-in for something the link could not find: the header's declaration of it with
    an empty body, or -- when it was never declared -- the function a call to an undeclared
    name assumes."""
    name = functions._plain(symbol)
    if not re.fullmatch(r"[A-Za-z_]\w*", name):
        return None
    m = re.search(r"(?m)^(?:extern\s+)?([^;{}#\n]*\b%s\s*\([^;{}\n]*\))\s*;" % re.escape(name), header)
    if m:
        return "%s\n{\n  return 0;\n}\n" % m.group(1)
    sized = re.fullmatch(r"_\w+@(\d+)", symbol)               # _name@N: __stdcall with N bytes of arguments
    if sized:
        return "int __stdcall %s(%s)\n{\n  return 0;\n}\n" % (
            name, ", ".join("int a%d" % i for i in range(int(sized.group(1)) // 4)) or "void")
    return "int %s()\n{\n  return 0;\n}\n" % name


# ---------------------------------------------------------------------------------------------
# 32-bit
# ---------------------------------------------------------------------------------------------
def _build32(b, path, m, folder):
    ow = watcom._ow()
    if not os.path.exists(ow["wcc"]):
        raise RuntimeError("OpenWatcom's 32-bit compiler is not set up (settings.json: openwatcom)")
    env, cflags = watcom._ow_env(ow), list(c_text.CFLAGS) + ["-e=20"]
    pe = open_pe(path)
    placed = [n for n in m.order if n in m.va]
    b.with_text = len(placed)
    at = {m.va[n]: n for n in placed}
    def extra(stem, text):
        return functions._compile(ow["wcc"], cflags, env, folder, stem, text)

    # Exported data is cut out of its section as an object under its own name. That is tried
    # first; if the compiler does not keep the pieces of a section together, the sections are
    # written whole and the data exports are reported missing.
    exported = {e.address: e.name for e in definitions.exports(path)
                if e.kind == "data" and e.name and re.fullmatch(r"[A-Za-z_]\w*", e.name) and e.name not in m.texts}
    header, texts, data_object = m.header, m.texts, None
    for named in ([exported, {}] if exported else [{}]):
        d = b.data = data.data32(path, at.get, named)
        if not d.movable:
            b.data_error = "the module has no relocation table, so nothing says which numbers are addresses"
            exported = {}
            break
        header, lost = data.relocate_header(m.header, d)
        b.lost_addresses = len(lost)                          # globals that are in no carried section
        with open(os.path.join(folder, "m.h"), "w", newline="\n", encoding="utf-8") as f:
            f.write(header)
        why = extra("data", d.source)
        if why:
            b.data_error = "the data does not compile: %s" % why
        elif data.layout_holds(open(os.path.join(folder, "data.obj"), "rb").read(), d):
            data_object, exported = "data.obj", named
            break
        else:
            b.data_error = "the compiler did not keep the pieces of a section together"
    if data_object:
        b.data_error = ""
        constants = data.address_constants(path, [int(k, 16) for k in m.all_keys])
        texts = {}
        for n in placed:
            texts[n], lost = data.relocate_text(m.texts[n], constants.get(m.va[n], ()), d, at.get)
            b.lost_addresses += len(lost)
    else:
        header, exported = m.header, {}
    stems, b.rejected = functions.compile_all(ow["wcc"], cflags, env, header, texts, placed, folder)
    objects = ["%s.obj" % stems[n] for n in placed if n in stems] + ([data_object] if data_object else [])
    public = _publics(folder, stems)
    entry = at.get(pe.image_base + pe.entry) if pe.entry else None
    if entry in public:
        start, b.entry = public[entry][0], "%s, the function at the original's entry point" % entry
    elif pe.is_dll():
        why = extra("entry", "int __stdcall codebreaker_entry(int4 module, int4 reason, int4 reserved)\n{\n  return 1;\n}\n")
        if why:                                               # the header itself does not compile
            b.error = "the module's header does not compile: %s" % why
            return None
        objects.append("entry.obj")
        start = "_codebreaker_entry@12"
        b.entry = ("a stand-in that reports success (the original's entry point has no reconstruction)"
                   if pe.entry else "a stand-in that reports success (the original has no entry point)")
    else:
        start, b.entry = None, "NONE: the program's entry point has no reconstruction"
    subsystem = pe.build_meta()["subsystem"]
    lines = ["format windows nt%s" % (" dll" if pe.is_dll() else ""),
             "runtime %s=4.0" % ("console" if subsystem == "console" else "windows"),
             "name m.dll", "option quiet, map, caseexact", "option modname='%s'" % definitions.module_name(path),
             "option offset=0x%X" % pe.image_base, "libpath %s" % watcom.ow_lib("pe")]
    if start:
        lines.append("option start=%s" % _quote(start))
    lines += ["file %s" % o for o in objects]
    res = _resources(b, path, folder)
    if res:
        lines.append("option resource=%s" % os.path.basename(res))
    # the functions the original imports by name, bound to the same DLL; then those by ordinal
    # (an ordinal is written as a bare number: `'MOD'.'@17'` would be read as "take directives
    # from the file 17"). An import whose own name is not a C name -- `_Name@12`, a C++ name --
    # is in the C text with every other character turned into `_`; it is bound back here.
    named, seen = watcom.import_map(path), set()
    # imports the original takes by number that the decompiler knows by name (COMCTL32's
    # DPA_CreateEx is its number 4): bound to the number
    from codebreaker.decompile import describe
    numbered = describe.BY_NUMBER.get(path) or describe.BY_NUMBER.get(os.path.abspath(path)) or {}
    originals = definitions.imports(path)
    odd = {re.sub(r"\W", "_", what): (mod, what) for mod, what in originals
           if isinstance(what, str) and not re.fullmatch(r"[A-Za-z_]\w*", what)}
    for o in objects:
        obj = open(os.path.join(folder, o), "rb").read()
        binds = watcom._import_lines(obj, set(m.imports), named, numbered).strip().split("\n")
        for sym in omf.externs(obj):
            mm = _ORDINAL_IMPORT.fullmatch(sym)
            if mm and (mm.group(1).upper(), int(mm.group(2))) in originals:
                binds.append("import '%s' '%s'.%s" % (sym, mm.group(1).upper(), mm.group(2)))
            plain = re.sub(r"@\d+$", "", sym[1:] if sym.startswith("_") else sym)
            if plain in odd:
                binds.append("import '%s' '%s'.'%s'" % (sym, odd[plain][0], odd[plain][1]))
        for line in binds:
            if line and line not in seen:
                seen.add(line)
                lines.append(line)

    def symbol_at(e):
        if e.kind == "data":
            return "_" + exported[e.address] if e.address in exported else None
        fn = at.get(e.address)
        return public[fn][0] if fn in public else None
    exports, b.exports_missing = definitions.export_directives(path, symbol_at)
    lines += exports + [x for x in watcom._RTL.strip().split("\n") if x]
    image = _link_resolving(b, ow["wlink"], env, folder, lines, lambda s: _from_header(header, s),
                            lambda stem, text: extra(stem, text))
    b.stand_in_addresses = {m.va[n] for n in b.rejected}
    _write_definitions(path, folder, lambda e: functions.prototype(m.texts[at[e.address]]) if e.address in at else None)
    return image


# ---------------------------------------------------------------------------------------------
# 16-bit
# ---------------------------------------------------------------------------------------------
def _build16(b, path, m, folder):
    wcc, wlink = watcom.ow_tool("wcc"), watcom.ow_tool("wlink")
    if not (wcc and wlink):
        raise RuntimeError("OpenWatcom's 16-bit compiler is not set up (settings.json: openwatcom)")
    env, nef = watcom.ow_env("ne"), NEFile(path)
    segs = {s["idx"]: s for s in nef.segments()}
    at = {m.where[va]: name for va, name in m.by_va.items() if m.where.get(va)}
    b.with_text = len(m.order)
    # the guard: a function the description does not have would be taken for the tail of the
    # one before it, and erased with it if that one is rebuilt from C -- never silently
    from codebreaker.decompile import describe as _describe
    b.hidden = _describe.hidden_functions(nef, set(at))
    # each function into a code segment named for the segment it came from
    stems, by_segment = {}, {}
    for (seg, _off), name in sorted(at.items()):
        by_segment.setdefault(seg, []).append(name)
    cflags = [f for f in functions.CFLAGS16 if f != "-ms"] + ["-mm", "-fpi87", "-os", "-nc=MCODE"]   # (MCODE: see the link order; size: a function rebuilt from C goes back into its place when it fits)
    segment_of = {name: seg for (seg, _off), name in at.items()}

    def named(text, seg):
        """A call the decompiler left as an address (`func_0x...`) goes to the function
        reconstructed at that address -- when it can be reached from this segment."""
        def fix(mm):
            target = m.by_va.get(int(mm.group(1), 16))
            if target and (target not in m.near or segment_of.get(target) == seg):
                return target
            return mm.group(0)
        return _BY_ADDRESS.sub(fix, text)

    texts = {name: named(m.texts[name], segment_of[name]) for name in segment_of}
    segments = sorted(by_segment.items())                    # (every segment's functions in the one pool)
    for (seg, _names), (got, rejected) in zip(segments, functions.compile_groups(
            wcc, env, m.header, texts,
            [(cflags + ["-nt=CODE%d" % seg], names, os.path.join(folder, "s%02d" % seg)) for seg, names in segments])):
        stems.update({n: "s%02d/%s" % (seg, s) for n, s in got.items()})
        b.rejected.update(rejected)

    with open(os.path.join(folder, "m.h"), "w", newline="\n", encoding="utf-8") as f:
        f.write(m.header)                                     # for what is compiled beside the functions

    def extra(stem, text):
        # (code the build adds -- an entry stand-in, the interrupt routines' registers, stand-ins
        # -- in a class of its own, XCODE (code to the linker: the name ends in CODE), laid out after the original's segments)
        return functions._compile(wcc, cflags + ["-nc=XCODE"] + (["-dCODEBREAKER_DATA"] if stem == "data" else []),
                                  env, folder, stem, text)        # (data.c defines the segments it declares)

    # every data segment besides the automatic one is carried, whether or not the C reads it:
    # it is part of the module (and code written as its own instructions refers to it)
    auto = nef.h.get("autodata_seg")
    for n, sg in segs.items():
        if sg["kind"] == "DATA" and n != auto and n not in m.segments:
            m.segments[n] = bytes(nef.data[sg["file_off"]:sg["file_off"] + sg["length"]]) if sg["file_off"] else \
                bytes(min(sg["minalloc"] or 0x10000, 0x10000))
    in_segment = {}                                          # a code segment -> a function in it (its selector)
    for (sg, _o), n in sorted(at.items()):
        in_segment.setdefault(sg, n)
    d = b.data = data.data16(path, at.get, m.segments, in_segment.get)
    far = data.far_segments(m.segments, path, at.get, in_segment.get)
    why = extra("data", d.source + far)
    if why:                                                   # the code still needs a data segment to exist
        b.data_error = "the data does not compile: %s" % why
        extra("data", "char codebreaker_data = 1;\n" + far)
    public = _publics(folder, stems)
    # A function written in assembly is rebuilt as assembly (analyze/origin.py says which, from
    # what its code does), not as a C translation of it -- where its instructions can be written
    # so that they assemble back to the original's; the C stays only where that cannot be done.
    from codebreaker.analyze import origin
    b.origins = {o.name: o for o in origin.module16(path, more=m.by_va).functions.values()}
    forced = {n for n in m.order if n in stems and n in b.origins and b.origins[n].language == "assembly"}
    # C the decompiler got wrong in a way that can be seen: a call it gives the caller's CS as
    # an argument (a far call whose arguments it read a word off -- see the decompiler's
    # far-call argument recovery). Until that is put right, such a function is rebuilt from
    # its own instructions rather than from C that would pass the wrong arguments.
    # ... and C with no result for a function whose instructions leave one in AX: the
    # decompiler saw nothing in the module read it (an export: Windows reads it -- KEYBOARD's
    # Inquire returns the size it copied), and compiled C leaves its own AX, not the original's
    starts = {}
    for (sg, off), n in at.items():
        starts.setdefault(sg, []).append(off)
    for n in [n for n in m.novalue if n in stems and n not in forced]:
        va = next((v for v, x in m.by_va.items() if x == n), None)
        sg, off = m.where.get(va) or (None, None)
        if sg not in segs or not segs[sg]["file_off"]:
            continue
        code = bytes(nef.data[segs[sg]["file_off"]:segs[sg]["file_off"] + segs[sg]["length"]])
        end = min([o for o in starts.get(sg, []) if o > off] + [len(code)])
        where = _result_in_ax(code, off, end)
        if where is not None:
            if n in b.origins:
                b.origins[n].evidence.append("leaves a result in AX (%04x) its C does not return (not trusted as C)"
                                             % where)
            forced.add(n)
    untrusted = {n for n in m.order if n in stems and n in m.texts
                 and re.search(r"\w+\([^;{}]*\bunaff_CS\b[^;{}]*\)", m.texts[n])}
    for n in untrusted - forced:
        if n in b.origins:
            b.origins[n].evidence.append("its decompiled calls pass the caller's CS (not trusted as C)")
    forced |= untrusted
    # ... and C one of whose calls passes clearly fewer (or more) words than the callee takes
    # off (its `ret N`): the decompiler kept to the pushes it could see, and the stack would
    # come back wrong
    # ... and C that reads ES as the function was entered: no compiled C takes ES in. The
    # original read ES as a callee left it -- USER's sub_1_7CD4 returns `mov ax, es` after
    # KERNEL's GetCodeInfo (its unwritten second result, the module); the C returned the ES
    # it came in with, SetWindowsHook stored that as the hook's module, and calling the hook
    # faulted (1:8093, selector D400) on Setup's License page.
    for n in m.order:
        if n in forced or n not in stems or n not in m.texts:
            continue
        body = m.texts[n].split("{", 1)[-1]
        if re.search(r"(?<![\w.])unaff_ES\b(?!\s*=[^=])", body):
            forced.add(n)
            if n in b.origins:
                b.origins[n].evidence.append("its C reads ES as the function was entered (not trusted as C)")
    for n, why in _registers_c_cannot_carry(nef, segs, at, m.texts).items():
        if n not in forced and n in stems:
            forced.add(n)
            if n in b.origins:
                b.origins[n].evidence.append(why + " (not trusted as C)")
    arity = {}
    with open(m.description.path, encoding="utf-8") as fh:
        for line in fh:
            parts = line.rstrip("\n").split("\t")
            if parts[0] == "F" and len(parts) > 4 and parts[4].startswith("__stdcall16") and int(parts[3]) > 0:
                arity[parts[2]] = int(parts[3]) // 2
    for n in m.order:
        if n in forced or n not in stems or n not in m.texts:
            continue
        body = m.texts[n].split("{", 1)[-1]
        for mm in re.finditer(r"\b(%s)\s*\(" % "|".join(re.escape(x) for x in arity) if arity else r"(?!x)x", body):
            args, _after = c_text._arguments_at(body, mm.end() - 1)
            words = arity[mm.group(1)]
            if len(args) * 2 < words or len(args) > words:
                if n in b.origins:
                    b.origins[n].evidence.append("its call of %s passes %d arguments for %d words (not trusted as C)"
                                                 % (mm.group(1), len(args), words))
                forced.add(n)
                break
    # ... and C that reads a variable nothing in it sets (the compiler's flow check): a value
    # the original took from a register on entry or from a buffer it had filled through a
    # pointer, which the C reads as whatever happens to be there -- right or wrong by where the
    # build happens to put things
    left = [n for n in m.order if n not in forced and n in stems and n in texts]
    unset = dict(zip(left, pool.threads(lambda n: functions.never_set(
        wcc, cflags, env, os.path.join(folder, "s%02d" % segment_of[n]), n, texts[n]), left, jobs=parallel.workers())))
    for n, names in unset.items():
        if names:
            if n in b.origins:
                b.origins[n].evidence.append("its C reads %s, which it never sets (not trusted as C)"
                                             % ", ".join(names))
            forced.add(n)
    # (for finding which rebuilt functions make a module misbehave: CB_ASM_ONLY=all rebuilds
    # every function from its own instructions; a comma list, those named; CB_ASM_SEGMENTS=1,3
    # those in the segments named)
    only = os.environ.get("CB_ASM_ONLY", "")
    segs_only = {int(x) for x in os.environ.get("CB_ASM_SEGMENTS", "").split(",") if x.strip()}
    if only or segs_only:
        named = set(m.order) if only == "all" else set(only.split(","))
        forced |= {n for n in m.order if n in stems and (n in named or segment_of.get(n) in segs_only)}
    for n in _near_pointer_tables(nef, at):
        # (its address is a number in the data segment: it must stay where it is)
        if n in stems and n not in forced:
            forced.add(n)
            if n in b.origins:
                b.origins[n].evidence.append("its address is in a table of near code pointers in the data segment "
                                             "(called through it) -- kept as its instructions, at its place")
    for n in _register_argument_calls(nef, at):
        # (a call through a pointer that takes an argument in a register: C cannot say it)
        if n in stems and n not in forced:
            forced.add(n)
            if n in b.origins:
                b.origins[n].evidence.append("it loads AX for a call through a pointer (a VxD's API entry: "
                                             "the service in AX, the arguments read from its frame) -- "
                                             "kept as its instructions")
    for n, why in kept_as_asm(b.name).items():               # (C a whole run of the module showed wrong)
        if n in stems:
            forced.add(n)
            if n in b.origins:
                b.origins[n].evidence.append("kept as its instructions (build/kept_as_asm.txt): " + why)
    for n, why in getattr(b, "gated", {}).items():           # (C that, run, did not do what the original does)
        if n in stems:
            forced.add(n)
            if n in b.origins:
                b.origins[n].evidence.append("its C, run beside the original, differs (not trusted as C): " + why[:160])
    forced -= set(os.environ.get("CB_FORCE_C", "").split(","))   # (and these as C whatever origin says)
    keep_c = os.environ.get("CB_ASM_KEEP_C")                 # (and the other way: C for these alone)
    if keep_c is not None:
        kept = set(keep_c.split(","))
        forced |= {n for n in m.order if n in stems and n not in kept}
    fresh = set()                                            # (the folders beside whose header is written)

    with open(path, "rb") as fh:
        _last, _pages = struct.unpack_from("<HH", fh.read(6), 2)
    loads_itself = _pages * 512 >= os.path.getsize(path) - 511
    b.resident_limit = {}
    if loads_itself:
        # A module that loads itself keeps only part of a segment after start-up: KERNEL cuts its
        # segment 1 to A940h (GlobalReAlloc of CS at 1:B567), the init code past it thrown away.
        # A function from below that point that finds no room below it would go after the frame,
        # past the end -- KRNL386's C fault decoder (1:A296) at 1:C602 faulted on every fault it
        # was to decode ("Invalid DPMI return from 0367:C608"). The packing the frame will do is
        # done here first, and only such a one stays as its instructions; one from past the
        # point (init code) may go after the frame.
        from codebreaker.build import msc6
        for sg, s in segs.items():
            if s["kind"] == "CODE":
                lim = _resident_limit(bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]]))
                if lim and lim < s["length"]:
                    b.resident_limit[sg] = lim
        # (the data such a segment's C reads through CS is linked after its frame: past the cut,
        # thrown away with the init code -- a function that reads it stays as its instructions,
        # which read the original's bytes where they are)
        for n in m.order:
            sg = segment_of.get(n)
            if sg in b.resident_limit and n in stems and n not in forced and \
                    re.search(r"\bcodebreaker_code_%d\b" % sg, texts.get(n, "")):
                forced.add(n)
                if n in b.origins:
                    b.origins[n].evidence.append("it reads data beside its code through CS, in a segment cut "
                                                 "after start-up (%04x)" % b.resident_limit[sg])
        starts_of = {}
        for (sg, o), nm in at.items():
            starts_of.setdefault(sg, []).append((o, nm))
        for sg, lim in b.resident_limit.items():
            order = sorted(starts_of.get(sg, []))
            size = {}
            for o, nm in order:
                if nm in forced or nm not in stems or stems[nm].startswith("asm/"):
                    continue
                try:
                    with open(os.path.join(folder, stems[nm] + ".obj"), "rb") as fh:
                        size[nm] = len(msc6.object_code(fh.read())[0])
                except OSError:
                    pass
            while True:
                holes = [(o, order[k + 1][0] if k + 1 < len(order) else segs[sg]["length"], nm)
                         for k, (o, nm) in enumerate(order) if nm in size]
                _bins, far_left, near_left = _pack_holes(holes, size, set(m.near), (), lim)
                own = {nm: o for o, _hi, nm in holes}
                stuck = [nm for nm in far_left + near_left if own[nm] < lim]
                if not stuck:
                    break
                for nm in stuck:
                    forced.add(nm)
                    del size[nm]
                    if nm in b.origins:
                        b.origins[nm].evidence.append(
                            "its C finds no room below where segment %d is cut (%04x) in a module that "
                            "loads itself" % (sg, lim))

    asm_read = {}                                            # (file -> (size, mtime, text): each read once)

    def _asm_texts():
        asm_dir = os.path.join(folder, "asm")
        texts = []
        for fn in (os.listdir(asm_dir) if os.path.isdir(asm_dir) else []):
            if fn.endswith(".asm"):
                p = os.path.join(asm_dir, fn)
                st = os.stat(p)
                had = asm_read.get(p)
                if had is None or had[:2] != (st.st_size, st.st_mtime_ns):
                    with open(p, encoding="utf-8", errors="replace") as fh:
                        had = asm_read[p] = (st.st_size, st.st_mtime_ns, fh.read())
                texts.append(had[2])
        return "\n".join(texts)

    near_seen = {}

    def _near_reached():
        """The symbols the hand-written code beside reaches near (`near ptr NAME`) -- found again
        only when the assembly has changed since."""
        text = _asm_texts()
        k = hash(text)
        if k not in near_seen:
            near_seen.clear()
            near_seen[k] = set(re.findall(r"near ptr ([\w@$?]+)", text))
        return near_seen[k]

    def elsewhere(name):
        """A far function rebuilt from C, compiled into a code segment of its own beside its
        segment (CODE<n>X) -- for one that does not fit back into its place. -> stem or None.
        Not in a module that loads itself (KRNL386: DOS loads the whole file and its own start
        sets up its segments and entries -- only the ones the original has; an export moved to
        a segment of ours is a "Call to Undefined Dynalink")."""
        seg = segment_of.get(name)
        if seg is None or name in m.near or loads_itself:
            return None
        if any(c in m.near for c in re.findall(r"\b(\w+)\s*\(", texts[name].split("{", 1)[-1])) \
                or "code __near *" in texts[name] or "codebreaker_cs()" in texts[name] \
                or re.search(r"\bcodebreaker_code_\d+\b", texts[name]):
            return None                                     # it calls near ones, which only its own segment reaches
                                                            # (or reads its segment's own bytes through CS)
        sym = public[name][0] if name in public else "_" + name
        if sym in _near_reached():
            return None                                     # hand-written code beside it reaches it near
        sub = os.path.join(folder, "s%02dx" % seg)
        os.makedirs(sub, exist_ok=True)
        if sub not in fresh:                                    # (the header beside the function, as in its own
            fresh.add(sub)                                      # folder -- this build's, not one left from before)
            with open(os.path.join(sub, "m.h"), "w", newline="\n", encoding="utf-8") as fh:
                fh.write(m.header)
            functions._HEADER[sub] = cache.key(m.header)
        stem = os.path.basename(stems[name])
        why = functions._compile(wcc, cflags + ["-nt=CODE%dX" % seg, "-nc=XCODE"], env, sub, stem, texts[name])
        b.elsewhere_why = getattr(b, "elsewhere_why", {})
        if why:
            b.elsewhere_why[name] = why
        return None if why else "s%02dx/%s" % (seg, stem)

    b.near_names = set(m.near)                                 # (for placing them: a near one stays in its segment)
    asm_imports = _assembled16(b, nef, at, segs, stems, public, folder, env, m.segments, forced, elsewhere)
    for n, why in getattr(m, "failed", {}).items():           # no C, and not rebuilt from its instructions
        if n not in b.assembled:
            b.rejected.setdefault(n, why)
    library = definitions.is_library(path)
    lines = ["format windows%s" % (" dll" if library else ""), "name m.dll",
             "option quiet, map, nodefaultlibs, caseexact, packcode=1, packdata=0",
             "option modname='%s'" % definitions.module_name(path),
             ] + (["option heapsize=%d" % nef.h["heap"]] if nef.h["heap"] else []) \
        + (["option stack=%d" % nef.h["stack"]] if nef.h["stack"] else [])     # (the linker refuses a size of 0)
    if not nef.h.get("autodata_seg"):
        # (no automatic data segment in the original -- WIN87EM, SYSTEM.DRV: their functions run on
        # the caller's DS. The build's own little DGROUP made one, and the loader set it up)
        lines.append("option noautodata")
    if definitions.description(path):
        lines.append("option description '%s'" % definitions.description(path).replace("'", " "))
    # what the original's header says of it, as its linker was told: the Windows version it
    # expects (3.10: Windows treats a 3.0 module the old way), protected mode only, and no
    # application type (KERNEL's)
    with open(path, "rb") as fh:
        raw = fh.read(0x10000)
    ne_at = struct.unpack_from("<I", raw, 0x3C)[0]
    expver, info = struct.unpack_from("<H", raw, ne_at + 0x3E)[0], struct.unpack_from("<H", raw, ne_at + 0x0C)[0]
    if expver:
        lines.append("option version=%d.%d" % (expver >> 8, expver & 0xFF))
    if info & 0x0008:
        lines.append("option protmode")
    if not info & 0x0300:
        lines.append("option noapptype")
    lines.append("libpath %s" % watcom.ow_lib("ne"))          # lib286/win: the real library, not the stub above it
    cs, ip = nef.h["csip"] >> 16, nef.h["csip"] & 0xFFFF
    entry = at.get((cs, ip)) if cs else None
    if entry in public:
        lines.append("option start=%s" % _quote(public[entry][0]))
        b.entry = "%s, the function at the original's entry point" % entry
    elif cs:
        extra("entry", "int2 __far __stdcall codebreaker_entry()\n{\n  return 1;\n}\n")
        lines.append("option start='_codebreaker_entry'")
        b.entry = "a stand-in that reports success (the original's entry point %d:%04x has no reconstruction)" % (cs, ip)
    else:
        b.entry = "none, as in the original"
    lines.append("file data.obj")                              # first: the data segment starts with it
    used_c = b.placed_c | getattr(b, "moved_c", set())
    if m.interrupts and any("codebreaker_int" in m.texts.get(n, "") for n in used_c):
        # the interrupts the module's C takes (none, with every function rebuilt from its
        # instructions: then nothing is added to the original's segments)
        asm, c = c_text.interrupt_routines({v for v, _regs in m.interrupts})
        with open(os.path.join(folder, "interrupts.asm"), "w", newline="\n") as f:
            f.write(asm)
        r = subprocess.run([watcom.ow_tool("wasm"), "-q", "interrupts.asm"], cwd=folder, env=env,
                           capture_output=True, text=True)
        why = extra("interrupt_registers", c.replace('#include "m.h"\n', ""))
        if r.returncode == 0 and not why:
            lines += ["file interrupts.obj", "file interrupt_registers.obj"]
        else:
            b.error = "the interrupt routines: %s" % (why or (r.stdout or r.stderr or "").strip()[-200:])
    lines += ["file %s.obj" % stem for stem in b.lead_ins]     # each code segment starts with its lead-in
    lines += ["file %s.obj" % s for s in dict.fromkeys(stems[n] for n in m.order if n in stems)
              if s not in b.lead_ins]                                   # (a run of
    # functions written in assembly is one object)
    # the original's data that the C of a segment reads through CS (c_text: codebreaker_code_<n>),
    # after the segment's code -- for a segment with any such function rebuilt from C
    b.code_data = []
    for seg, text in sorted(getattr(m, "code_data", {}).items()):
        readers = [n for n in by_segment.get(seg, ()) if n in stems and not stems[n].startswith("asm/")
                   and re.search(r"\bcodebreaker_code_%d\b" % seg, texts.get(n, ""))]
        if not readers:
            continue
        why = functions._compile(wcc, cflags + ["-nt=CODE%d" % seg], env, folder, "code%02d" % seg, text)
        if why:
            b.error = b.error or "the data code segment %d's C reads through CS: %s" % (seg, why)
        else:
            lines.append("file code%02d.obj" % seg)
            b.code_data.append((seg, readers))
    if cs and entry not in public:
        lines.append("file entry.obj")
    for seg in sorted(by_segment):                             # each code segment loaded as the original's was
        flags = segs[seg]["flags"] if seg in segs else 0x1D10
        lines.append("segment 'CODE%d' %s" % (seg, " ".join(on if flags & bit else off for bit, on, off in
                                                              _SEGMENT_OPTIONS if (on if flags & bit else off))))
    if auto in segs:                                           # the data segment, loaded as the original's
        flags = segs[auto]["flags"]
        how = " ".join(on if flags & bit else off for bit, on, off in _SEGMENT_OPTIONS[:2])
        lines += ["segment class 'DATA' %s" % how, "segment class 'BSS' %s" % how]
    # the original's DOS part (its stub -- KRNL386's is its real-mode start, which finds the
    # NE header by its own length and runs from the whole file DOS loads)
    stub = _dos_stub(path, folder)
    if stub:
        lines += ["option stub=%s" % stub, "option stubverbatim"]
    # its file alignment, and -- where its loader fixes are all by segment number, chained, as
    # the original linker wrote KRNL386's (which relocates itself, before there is an entry
    # table to go through) -- ours the same
    lines.append("option alignment=%d" % (1 << (nef.h.get("align_shift") or 9)))
    if _segment_number_fixups(nef):
        lines.append("option selectorfixups")
    # ... and where the original's was: its code segments first, by their numbers (segment n is
    # the original's segment n -- what reaches one by its number finds it), what the build adds
    # (CODE<n>X, the runtime's _TEXT) after them
    code_numbers = [s["idx"] for s in nef.segments() if s["kind"] == "CODE"]
    if code_numbers and all(n in by_segment for n in code_numbers):
        order = "order clname MCODE " + " ".join("segment CODE%d" % n for n in code_numbers)
        auto = nef.h.get("autodata_seg")
        # the data segments after them, each at its number as there: the automatic one (its
        # classes) and those carried as they were (CODEBREAKER_SEGMENT_<n>, class FAR_DATA).
        # Anything the build adds goes after all of the original's -- a runtime helper's code
        # put before a data segment renumbers it and every selector after it (USER's
        # segment 44, its second data segment, became 46 behind the long-division routine)
        data_numbers = [s["idx"] for s in nef.segments() if s["kind"] != "CODE"]
        if data_numbers and min(data_numbers) == max(code_numbers) + 1:
            far = [n for n in data_numbers if n != auto]
            if not far or (far == sorted(far) and (auto not in data_numbers or
                                                   all(n < auto for n in far) or all(n > auto for n in far))):
                pieces = []
                for n in data_numbers:
                    if n == auto:
                        pieces.append("clname BEGDATA clname DATA clname BSS clname STACK")
                    elif n == far[0]:
                        pieces.append("clname FAR_DATA " + " ".join("segment CODEBREAKER_SEGMENT_%d" % k for k in far))
                order += " " + " ".join(pieces)
        lines.append(order + " clname XCODE clname CODE")   # (the runtime's CODE last)
    else:
        b.layout = "segments renumbered: the original's code segments %s are not all rebuilt" % (
            [n for n in code_numbers if n not in by_segment],)
    res = _resources(b, path, folder)
    if res:
        lines.append("option resource=%s" % os.path.basename(res))
    imports = dict(m.imports)
    imports.update({k: v for k, v in d.imports.items() if k not in imports})
    imports.update({k: v for k, v in asm_imports.items() if k not in imports})
    # a C name compiled as __stdcall with an open argument list is the symbol `_name`
    for name, (mod, what) in sorted(imports.items()):
        lines.append("import _%s %s.%s" % (name, mod, what if isinstance(what, int) else _quote(what)))

    # The module's constants -- entries of segment 0xFE, KERNEL's __AHINCR, __0040H and the
    # like: values, not places. Each is a public EQU in constants.asm, which the linker exports
    # as a constant entry.
    constants = {e.ordinal: e for e in definitions.exports(path)
                 if isinstance(e.address, tuple) and e.address[0] == 0xFE}
    if constants:
        def symbol(e):
            return e.name if e.name and re.fullmatch(r"[A-Za-z_]\w*", e.name) else "constant_%d" % e.ordinal
        src = ["; %s -- the values it exports as constants (entry segment 0FEh)" % definitions.module_name(path), ""]
        for o, e in sorted(constants.items()):
            src += ["public %s" % symbol(e), "%s equ 0%Xh" % (symbol(e), e.address[1])]
        with open(os.path.join(folder, "constants.asm"), "w", newline="\n") as f:
            f.write("\n".join(src + ["end", ""]))
        r = subprocess.run([watcom.ow_tool("wasm"), "-q", "constants.asm"], cwd=folder, env=env,
                           capture_output=True, text=True)
        if r.returncode == 0:
            lines.append("file constants.obj")
        else:
            constants = {}

    def symbol_at(e):
        if e.ordinal in constants:
            return symbol(e)
        if e.address in d.exported:                         # exported data: its part of the segment
            return "_" + d.exported[e.address]
        fn = at.get(e.address)
        return public[fn][0] if fn in public else None
    exports, b.exports_missing = definitions.export_directives(path, symbol_at)
    lines += exports + ["library clibm.lib"]
    image = _link_resolving(b, wlink, env, folder, lines, lambda s: _from_header(m.header, s),
                            lambda stem, text: extra(stem, text))
    # As the function-by-function build does (build/functions.py): a function that reaches near
    # for something in another segment (the original called it far; the text has lost that)
    # cannot be linked, and a segment whose rebuilt functions come to more than 64K cannot be
    # either. Those functions become stand-ins -- the largest first, for a segment too big --
    # and the module is linked again.
    by_stem = {s: n for n, s in stems.items()}
    for _attempt in range(120):
        if image:
            break
        apart = re.search(r"file (s\d+/f\d+)\.obj.*?not in the same segment", b.error)
        over = re.search(r"size of segment CODE(\d+) exceeds 64k by (\d+) bytes", b.error)
        swap = []
        if apart and by_stem.get(apart.group(1)) and by_stem[apart.group(1)] not in b.rejected:
            line = next((ln for ln in b.error.splitlines() if apart.group(1) in ln), "")
            swap = [(by_stem[apart.group(1)], "reaches near for something in another segment: " + line.strip()[-160:])]
        elif over:
            seg, need, freed = int(over.group(1)), int(over.group(2)) + 512, 0
            # First the far ones, largest first, go to the segment beside (CODE<n>X): they are
            # called far, from anywhere. Only what that cannot free becomes stand-ins.
            moved_now = False
            for size, n in sorted(((os.path.getsize(os.path.join(folder, stems[n] + ".obj")), n)
                                   for n in by_segment.get(seg, []) if n in stems and n not in b.rejected
                                   and n not in m.near and n not in b.moved_c and n not in b.assembled
                                   and not stems[n].startswith("asm/")), reverse=True):
                if freed >= need:
                    break
                old = "file %s.obj" % stems[n]
                if old not in lines:
                    continue
                moved = elsewhere(n)
                if moved:
                    lines[lines.index(old)] = "file %s.obj" % moved
                    stems[n] = moved
                    b.moved_c.add(n)
                    freed += size
                    moved_now = True
            if moved_now:
                b.error, b.absent = "", []
                image = _link_resolving(b, wlink, env, folder, lines, lambda s: _from_header(m.header, s),
                                        lambda stem, text: extra(stem, text))
                continue
            for size, n in sorted(((os.path.getsize(os.path.join(folder, stems[n] + ".obj")), n)
                                   for n in by_segment.get(seg, []) if n in stems and n not in b.rejected), reverse=True):
                if freed >= need:
                    break
                swap.append((n, "too large: the functions of its segment do not fit in 64K when rebuilt"))
                freed += size
        if not swap:
            break
        for n, why in swap:
            sub, stem = stems[n].split("/")
            if functions._compile(wcc, cflags + ["-nt=CODE%d" % segment_of[n]], env, os.path.join(folder, sub), stem,
                                  functions.stand_in(texts[n])):
                return image                                 # not even a stand-in compiles: give up
            b.rejected[n] = why
        b.error, b.absent = "", []
        image = _link_resolving(b, wlink, env, folder, lines, lambda s: _from_header(m.header, s),
                                lambda stem, text: extra(stem, text))
    b.stand_in_addresses = {m.where[va] for va, n in m.by_va.items() if n in b.rejected}
    if image and not b.data_error:                            # is the data where the code expects it?
        try:
            text = open(os.path.join(folder, "m.map"), errors="replace").read()
        except OSError:
            text = ""
        group = re.search(r"(?m)^DGROUP\s+([0-9a-fA-F]{4}):([0-9a-fA-F]{4})", text)
        mine = re.search(r"(?m)^([0-9a-fA-F]{4}):([0-9a-fA-F]{4})[+*]?\s+_codebreaker_data\s*$", text)
        if not (group and mine and group.groups() == mine.groups()):
            b.data_error = "the data did not land at the start of the automatic data segment"
    _write_definitions(path, folder, lambda e: c_text.declaration(m.texts[at[e.address]]) if e.address in at else None)
    return image


# ---------------------------------------------------------------------------------------------
# what both kinds share
# ---------------------------------------------------------------------------------------------
def _resources(b, path, folder):
    """Write and compile the module's resources. -> the .res to link, or None."""
    if not resource_reader.read(path):
        return None
    b.resources = resources.rebuild(path, folder)
    return b.resources.link_res


def _write_definitions(path, folder, declaration_at):
    stem = os.path.splitext(os.path.basename(path))[0].lower()
    with open(os.path.join(folder, stem + ".def"), "w", newline="\n") as f:
        f.write(definitions.definition_file(path))
    with open(os.path.join(folder, stem + "_public.h"), "w", newline="\n") as f:
        f.write(definitions.public_header(path, declaration_at))


def check(b):
    """Compare the linked image with the original: format, exports, imports, resources."""
    orig, image = b.original, b.image
    b.format_same = detect.format_of(image) == detect.format_of(orig) and \
        definitions.is_library(image) == definitions.is_library(orig)
    mine = {e.ordinal: e for e in definitions.exports(image)}
    theirs = definitions.exports(orig)
    b.exports_total = len(theirs)
    for e in theirs:
        got = mine.get(e.ordinal)
        # an export that has no name in the original cannot be made without one by this linker
        if got and (got.name == e.name or (e.name is None and got.name == "ordinal_%d" % e.ordinal)):
            b.exports_same += 1
            if e.address in b.stand_in_addresses:
                b.exports_on_stand_ins += 1
        elif not any(x.ordinal == e.ordinal for x, _why in b.exports_missing):
            b.exports_missing.append((e, "in the build as %r" % (got.name if got else None)))
    want, have = definitions.imports(orig), definitions.imports(image)
    b.imports_original, b.imports_built = len(want), len(have)
    b.imports_extra = sorted(have - want, key=str)
    original = resource_reader.read(orig)
    if original:
        r = resources.compare(original, resource_reader.read(image), detect.wide_resources(b.kind))
        b.resources_same = r.same
        w = b.resources
        b.resources_line = "%d of %d the same in the image (%d identical, %d equivalent)" % (
            r.identical + r.equivalent, r.total, r.identical, r.equivalent)
        if w and not w.error:
            b.resources_line += "; written as text %d, image files %d, opaque payload %d, fallback payload %d" % (
                w.as_text, w.as_files, w.opaque, w.fallback)
        elif w:
            b.resources_line += "; NOT COMPILED: %s" % w.error
    return b


def _sections(path):
    """[(address, bytes)] for everything of a 32-bit image that is not code: the emulator maps
    a module's code itself, and reads anything else as zeros unless it is given."""
    pe = open_pe(path)
    return [(pe.image_base + s["vaddr"], bytes(pe.data[s["rawptr"]:s["rawptr"] + s["rawsize"]]))
            for s in pe.sections if s["rawsize"] and not s["chars"] & 0x20000020]


def _call(path, kind, export):
    """Load a module in the emulator and call one exported function with zero arguments: the
    platform's emulator does it (codebreaker/platforms: emulator.call). -> (how it ended, its result)"""
    from codebreaker import platforms
    return platforms.of(kind).emulator.call(path, export)


def run(b, limit=RUN_LIMIT):
    """Call the built module's exported functions in the emulator, and the original's beside."""
    mine = {e.ordinal: e for e in definitions.exports(b.image) if e.kind == "code"}
    for e in [x for x in definitions.exports(b.original) if x.kind == "code" and x.ordinal in mine][:limit]:
        got, want = _call(b.image, b.kind, mine[e.ordinal]), _call(b.original, b.kind, e)
        b.ran.append((e.name or "@%d" % e.ordinal, got[0], want[0], got == want))
    return b


def _linker(b):
    """What links a module of this kind: its platform's packager (_build16, _build32)."""
    from codebreaker import platforms
    link = platforms.of(b.kind).packager.link
    if link is None:
        raise SystemExit("%s: a %s module is not linked whole here" % (b.name, platforms.of(b.kind).title))
    return link


def build(module, outdir=None, call=True, log=print, gated=None):
    """Build a module whole. -> Built

    A 16-bit module's functions rebuilt from C are then each run beside the original's
    (verify/gate16/isolated16.py: on their own, every call out answered alike on both sides); one
    that does not do what the original does is rebuilt from its own instructions instead, and
    the module built again (b.gated: name -> how it differed). CB_NO_GATE=1 leaves them."""
    from codebreaker.verify import reconstruct
    path = corpus.retail(module)
    if not path:
        raise SystemExit("no module %r in the corpus" % module)
    if os.environ.get("CB_ENGINE") == "1" and gated is None:
        # (on the engine: the same build made of facts, only what changed made again -- engine/win16.py)
        from codebreaker import platforms
        p = platforms.identify(path)
        if p is not None and p.steps:
            return p.steps_module().build(path, outdir=outdir, call=call, log=log)
    b = Built(path)
    b.gated = dict(gated or {})
    kind = reconstruct.kind_of(path)
    b.folder = folder = outdir or settings.state("build", b.name)
    os.makedirs(folder, exist_ok=True)
    log("%s: decompiling" % b.name)
    m = kind.prepare(path, os.path.join(folder, "decompiled"))
    b.functions = len(m.all_keys)
    from_cache = False
    if b.kind == "ne" and gated is None and not os.environ.get("CB_NO_GATE"):
        # every C function's verdict already known (kept by _gate16): the ones that differ go to
        # assembly now, and the module is built once instead of twice
        known = _gate16_known(path, m)
        if known is not None:
            b.gated, gated, from_cache = dict(known), known, True
    if b.kind == "ne" and getattr(m, "description", None) is not None:
        b.disputes = _disputes(m.description)
    log("%s: compiling %d functions, data and resources" % (b.name, len(m.order)))
    image = _linker(b)(b, path, m, folder)
    if getattr(b, "resident_overflow", None):
        log("%s: %d placed past where a segment is cut: rebuilt from their instructions"
            % (b.name, len(b.resident_overflow)))
        return build(module, outdir=outdir, call=call, log=log, gated=dict(b.gated, **b.resident_overflow))
    if not image:
        return b
    b.image = os.path.join(folder, os.path.basename(path).lower())
    shutil.copyfile(image, b.image)
    if b.kind == "ne":
        _whole_file_image(path, b.image)
        if (gated is None or from_cache) and not os.environ.get("CB_NO_GATE"):
            # (built with the kept verdicts: only a C function not among them is run now)
            c = sorted(b.placed_c | getattr(b, "moved_c", set()))
            log("%s: running %d functions rebuilt from C beside the original's" % (b.name, len(c)))
            differ = _gate16(path, folder, c, m)
            failed = [(n, w) for n, w in getattr(_gate16, "not_run", []) if w.startswith("the checker failed")]
            if failed:
                b.gate_error = "%d functions (%s)" % (len(failed), failed[0][1][:120])
                log("%s: the gate could not run %d functions: %s" % (b.name, len(failed), failed[0][1][:120]))
            # (and its calls beside the original's: what each call is followed by -- the check
            # above answers a call as the description believes; this reads both as they are)
            from codebreaker.verify.gate16 import contract16
            try:
                for n, why in contract16.mismatches(path, folder, b.image, c).items():
                    differ.setdefault(n, "its calls are not the original's: " + why)
            except Exception as e:                           # (a check that fails says so: never "all agree")
                b.contract_error = "%s: %s" % (type(e).__name__, e)
                log("%s: the call-contract check FAILED: %s" % (b.name, b.contract_error))
            cache.put("gate16_set", cache.key(open(path, "rb").read(), getattr(m, "header", "")),
                      "\n".join(sorted(set(c) | set(gated or {}))).encode("utf-8"))
            if differ:
                log("%s: %d do not do what the original does: rebuilt from their instructions" % (b.name, len(differ)))
                return build(module, outdir=outdir, call=call, log=log, gated=dict(gated or {}, **differ))
    check(b)
    if call:
        run(b)
    return b


def _disputes(d):
    """[(name, who the callers say removes the arguments)] for each function the description
    says otherwise of -- the original's own calls are the evidence (describe.callers_say)."""
    from codebreaker.decompile import describe
    try:
        say = describe.callers_say(d)
        out = []
        with open(d.path, encoding="utf-8") as f:
            for line in f:
                p = line.rstrip("\n").split("\t")
                if p[0] != "F" or len(p) < 4:
                    continue
                who = say.get(describe.where(int(p[1], 16)))
                if who and who != ("callee" if int(p[3]) > 0 else "caller"):
                    out.append((p[2], who))
        return out
    except Exception as e:                                   # (a report, not the build: never stops it)
        return [("(not checked)", str(e)[:80])]


def _gate16_keys(path, names, m):
    import sys
    from codebreaker.emulate import windows16
    from codebreaker.verify.gate16 import isolated16
    from codebreaker.verify.lanes import win16
    with open(path, "rb") as f:
        module_key = cache.key(f.read())
    from codebreaker.symbols import api_arguments
    try:                                                     # (what each Windows call removes: the
        sizes = open(api_arguments._cache16_path(), "rb").read()     # checker answers calls by it)
    except OSError:
        sizes = b""
    from codebreaker.emulate import pcode, shadow, symbolic
    from codebreaker.verify import concolic, guided, lane_common
    # (and the branch-guided inputs the checker runs: the explorers, their limits, whether they are
    # on -- the path explorer's lifter too)
    ready = win16.concolic_ready()
    tools = cache.key(cache.tool(watcom.ow_tool("wcc") or ""), functions.CFLAGS16, sizes,
                      lane_common.guided_on(), lane_common.GUIDED_BUDGET, lane_common.MAX_GUIDED,
                      ready, lane_common.PATH_BUDGET, lane_common.MAX_PATHS, pcode.stamp() if ready else "",
                      *[open(sys.modules[x.__name__].__file__, "rb").read()
                        for x in (isolated16, windows16, win16, api_arguments, guided, lane_common, concolic,
                                  shadow, symbolic, pcode)])
    header = cache.key(getattr(m, "header", ""))
    # (and the description's lines for the function and for each it calls: the checker answers
    # a call as the description says the callee does -- what it removes, what it returns -- so
    # a callee described anew is a verdict to make again)
    name_line = {}
    try:
        with open(m.description.path, encoding="utf-8") as f:
            for line in f:
                p = line.split("\t")
                if p[0] in ("F", "X") and len(p) > 2:
                    name_line[p[2]] = line.rstrip("\n")
    except (AttributeError, OSError):
        pass

    def deps(n):
        called = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", m.texts.get(n, ""))) | {n}
        return sorted(name_line[c] for c in called if c in name_line)
    return {n: cache.key(module_key, tools, header, n, m.texts.get(n, ""), deps(n)) for n in names}


KEPT_AS_ASM = os.path.join(os.path.dirname(os.path.abspath(__file__)), "kept_as_asm.txt")


def kept_as_asm(module):
    """{name: why} for the functions of `module` kept as their instructions by kept_as_asm.txt:
    C the gate passes that a run of the whole module showed wrong. The list is for those the
    decompiler cannot yet write right; each line goes when it can."""
    kept = {}
    try:
        with open(KEPT_AS_ASM, encoding="utf-8") as f:
            for line in f:
                parts = line.split("#", 1)[0].split(None, 2)
                if len(parts) >= 2 and parts[0].lower() == module.lower():
                    kept[parts[1]] = parts[2].strip() if len(parts) > 2 else ""
    except OSError:
        pass
    return kept


def _gate16_known(path, m):
    """{name: how it differs} when the gate's verdict is kept for every function the module has
    as C text (the last build's set, recorded with it) -- else None."""
    names = cache.get("gate16_set", cache.key(open(path, "rb").read(), getattr(m, "header", "")))
    if names is None:
        return None
    names = [n for n in names.decode("utf-8").split("\n") if n]
    differ = {}
    for n, k in _gate16_keys(path, names, m).items():
        had = cache.get("gate16", k)
        if had is None:
            return None
        if had[:1] == b"D":
            differ[n] = had[1:].decode("utf-8", "replace")
    return differ


def _gate16(path, folder, names, m):
    """{name: how it differs} for the named functions rebuilt from C, each run beside the
    original's (verify/gate16/isolated16.py). A function's verdict is kept (base/cache.py) by what
    makes it: the original module, the function's C and the header it compiles with, the
    compiler and its options, and the source of the checker and the emulator -- a function
    the same as in an earlier build is not run again."""
    from codebreaker.verify.gate16 import isolated16
    keys = _gate16_keys(path, names, m)
    differ, run = {}, []
    for n in names:
        had = cache.get("gate16", keys[n])
        if had is None:
            run.append(n)
        elif had[:1] == b"D":
            differ[n] = had[1:].decode("utf-8", "replace")
    _gate16.not_run = []
    for n, verdict, why in isolated16.check(path, folder, run) if run else ():
        if verdict == "not run":                             # (no verdict: not kept, and said)
            _gate16.not_run.append((n, why))
            continue
        cache.put("gate16", keys[n], (b"D" + (why or "").encode("utf-8", "replace")) if verdict == "differs" else b"S")
        if verdict == "differs":
            differ[n] = why
    return differ


def main(argv):
    """python -m codebreaker build <module> [--out DIR] [--no-run] [--engine]

    Without --out the build goes into the module's project folder: Converted/<name>/build/."""
    out = argv[argv.index("--out") + 1] if "--out" in argv else None
    names = [a for a in argv if not a.startswith("--") and a != out]
    if "--engine" in argv:                                   # (a 16-bit module made by the engine)
        os.environ["CB_ENGINE"] = "1"
    if not names:
        print(__doc__)
        return 2
    bad = 0
    for name in names:
        proj = None
        where = out
        if not where:
            from codebreaker.base import converted
            path = corpus.retail(name)
            if path:
                proj = converted.project(path)
                where = proj.build
        b = build(name, outdir=where, call="--no-run" not in argv)
        print(b.report())
        if proj:
            proj.record("build", [proj.build], notes="the whole module built from its recovered source: %s"
                        % ("linked: %s" % b.image if b.linked else "did not link"))
        if b.linked:
            print("  image       %s" % b.image)
        bad += 0 if b.linked else 1
    return 1 if bad else 0
