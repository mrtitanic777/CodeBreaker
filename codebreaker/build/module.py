"""module.py -- build a whole module from its reconstruction, and check what came out.

    python -m codebreaker build <module> [--out DIR] [--no-run]

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
import subprocess

from codebreaker.base import corpus, settings
from codebreaker.build import data, definitions, functions, resources, watcom
from codebreaker.decompile import c_text
from codebreaker.formats import detect
from codebreaker.formats import resources as resource_reader
from codebreaker.formats.ne import NEFile
from codebreaker.formats.pe import open_pe

_UNDEFINED = re.compile(r"(\S+) is an undefined reference")
_BY_ADDRESS = re.compile(r"\bfunc_0x([0-9a-f]{8})\b")
_ORDINAL_IMPORT = re.compile(r"_?([A-Za-z0-9]+)_ord_(\d+)(?:@\d+)?")
_SEGMENT_OPTIONS = ((0x40, "preload", "loadoncall"), (0x10, "moveable", "fixed"), (0x1000, "discardable", ""))
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


def _assembled16(b, nef, at, segs, stems, public, folder, env, carried=(), forced=(), elsewhere=None):
    """Assembly of the module's own instructions where C does not do (build/asm16.py): each code
    segment's lead-in -- what is before its first function, often a variable the code reads
    through CS -- linked first into the segment so that it stays where it was; and each rejected
    function, instead of a stand-in, when it can be written so and assembles back to the
    original's bytes. -> {import symbol without its underscore: (module, what)} they need."""
    from codebreaker.build import asm16
    wasm = watcom.ow_tool("wasm")
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
            return public[name][0] if name in public else entries.get(what)
        if kind == "segment":
            return in_segment.get(what[0])
        if kind == "data":                                   # a data segment: the auto one, or one carried
            if what[0] == nef.h.get("autodata_seg"):
                return "_codebreaker_data"
            return "_codebreaker_segment_%d" % what[0] if what[0] in carried else None
        mod, which = what
        return "_%s_%s" % (re.sub(r"\W", "_", mod), which) if isinstance(which, int) else "_" + re.sub(r"\W", "_", which)
    out = {}
    os.makedirs(os.path.join(folder, "asm"), exist_ok=True)

    def assemble_piece(stem, f, lo, hi):
        with open(os.path.join(folder, stem + ".asm"), "w", newline="\n") as fh:
            fh.write(f.write(f.good, lo, hi))
        r = subprocess.run([wasm, "-q", "-zcm", "-fo=" + stem + ".obj", stem + ".asm"], cwd=folder, env=env,
                           capture_output=True, text=True)
        obj = os.path.join(folder, stem + ".obj")
        if r.returncode or not os.path.isfile(obj):
            return "the assembler: " + ((r.stdout or "") + (r.stderr or "")).strip()[-160:]
        with open(obj, "rb") as fh:
            return asm16.check_piece(fh.read(), f, lo, hi)

    def assemble(stem, f):
        """Written as readable as the assembler allows: each instruction as its mnemonic where the
        probe says the assembler gives back the original's bytes, and round after round any
        that did not come back as they were go back to bytes. -> "" or why it could not be."""
        from codebreaker.build import msc6
        from codebreaker.source import assembly
        cand = f.candidates()
        ok = assembly.probe(list(cand.values()), set(), folder, mode="real16") if cand else set()
        good = {at for at, c in cand.items() if c in ok} | {at for at, t in f.ins.items() if t[3] is not None}
        why = "no rounds"
        f.good = good
        for _round in range(40):
            with open(os.path.join(folder, stem + ".asm"), "w", newline="\n") as fh:
                fh.write(f.write(good))
            r = subprocess.run([wasm, "-q", "-zcm", "-fo=" + stem + ".obj", stem + ".asm"], cwd=folder, env=env,
                               capture_output=True, text=True)
            obj = os.path.join(folder, stem + ".obj")
            if r.returncode or not os.path.isfile(obj):
                msg = (r.stdout or "") + (r.stderr or "")
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
    for _pass in range(8):
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
        for run in runs:
            if run in done:
                continue
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
            if f.text is None and f.missing:
                # it calls into the middle of a run of assembly: that place is an entry of its own
                for t in f.missing:
                    if (seg, t) not in entries:
                        wanted.add((seg, t))
                continue
            if f.text is None and f.reaches is not None:
                # it jumps into the middle of a function beside it: that one was written in
                # assembly too (nothing in its own code said so); the run takes it in
                inside = max([o for o in starts[seg] if o <= f.reaches] or [None])
                if inside is not None and at[(seg, inside)] not in forced and at[(seg, inside)] in public:
                    joined.add(at[(seg, inside)])
                    continue
            why = f.why if f.text is None else assemble("asm/" + stems[run[0]].replace("/", "_"), f)
            done[run] = ("why", why) if why else ("stem", "asm/" + stems[run[0]].replace("/", "_"), f)
        for seg, t in wanted:
            inside = max([o for o in starts[seg] if o <= t] or [None])
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
                b.origins[name].evidence.append("entered in its middle from assembly beside it")
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
    # each hole: [start, end, [(pad, stem)], used] -- its own function first, when it fits
    bins = []
    rest = []
    for lo, hi, sym in f.holes:
        name = name_of.get(sym)
        bins.append([lo, hi, [], lo])
        if name in size:
            pad = bins[-1][3] & 1                          # (OpenWatcom aligns its code to a word)
            if pad + size[name] <= hi - lo:
                bins[-1][2].append((pad, cstem[name]))
                bins[-1][3] += pad + size[name]
            else:
                rest.append(name)
        b.frame_fit = getattr(b, "frame_fit", []) + [(name, hi - lo, size.get(name, 0))]
    after = []
    for name in sorted(rest, key=lambda n: -size[n]):
        for bn in bins:
            pad = bn[3] & 1
            if bn[3] + pad + size[name] <= bn[1]:
                bn[2].append((pad, cstem[name]))
                bn[3] += pad + size[name]
                break
        else:
            moved = elsewhere(name) if elsewhere else None
            if moved:                                      # far: into the segment beside this one
                stems[name] = moved
                b.moved_c.add(name)
            else:
                after.append(cstem[name])                  # near: after the frame, in this segment
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


def _filler(folder, wasm, env, seg, stem, n):
    """An object of `n` bytes of INT 3 in code segment `seg`: a place kept where it was."""
    stem = "asm/" + stem
    with open(os.path.join(folder, stem + ".asm"), "w", newline="\n") as fh:
        fh.write("CODE%d segment byte public 'CODE'\n        db   %d dup (0CCh)\nCODE%d ends\nend\n" % (seg, n, seg))
    subprocess.run([wasm, "-q", "-fo=" + stem + ".obj", stem + ".asm"], cwd=folder, env=env, capture_output=True)
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
                symbols = watcom._omf_publics(f.read())
        except OSError:
            continue
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
        for sym in watcom._omf_externs(obj):
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
    # each function into a code segment named for the segment it came from
    stems, by_segment = {}, {}
    for (seg, _off), name in sorted(at.items()):
        by_segment.setdefault(seg, []).append(name)
    cflags = [f for f in functions.CFLAGS16 if f != "-ms"] + ["-mm", "-fpi87", "-os"]   # (size: a function rebuilt from C goes back into its place when it fits)
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
    for seg, names in sorted(by_segment.items()):
        sub = "s%02d" % seg
        got, rejected = functions.compile_all(wcc, cflags + ["-nt=CODE%d" % seg], env, m.header,
                                              texts, names, os.path.join(folder, sub))
        stems.update({n: "%s/%s" % (sub, s) for n, s in got.items()})
        b.rejected.update(rejected)

    with open(os.path.join(folder, "m.h"), "w", newline="\n", encoding="utf-8") as f:
        f.write(m.header)                                     # for what is compiled beside the functions

    def extra(stem, text):
        return functions._compile(wcc, cflags, env, folder, stem, text)

    # every data segment besides the automatic one is carried, whether or not the C reads it:
    # it is part of the module (and code written as its own instructions refers to it)
    auto = nef.h.get("autodata_seg")
    for n, sg in segs.items():
        if sg["kind"] == "DATA" and n != auto and n not in m.segments:
            m.segments[n] = bytes(nef.data[sg["file_off"]:sg["file_off"] + sg["length"]]) if sg["file_off"] else \
                bytes(min(sg["minalloc"] or 0x10000, 0x10000))
    d = b.data = data.data16(path, at.get, m.segments)
    why = extra("data", d.source + data.far_segments(m.segments))
    if why:                                                   # the code still needs a data segment to exist
        b.data_error = "the data does not compile: %s" % why
        extra("data", "char codebreaker_data = 1;\n" + data.far_segments(m.segments))
    public = _publics(folder, stems)
    # A function written in assembly is rebuilt as assembly (analyze/origin.py says which, from
    # what its code does), not as a C translation of it -- where its instructions can be written
    # so that they assemble back to the original's; the C stays only where that cannot be done.
    from codebreaker.analyze import origin
    b.origins = {o.name: o for o in origin.module16(path).functions.values()}
    forced = {n for n in m.order if n in stems and n in b.origins and b.origins[n].language == "assembly"}
    def elsewhere(name):
        """A far function rebuilt from C, compiled into a code segment of its own beside its
        segment (CODE<n>X) -- for one that does not fit back into its place. -> stem or None."""
        seg = segment_of.get(name)
        if seg is None or name in m.near:
            return None
        os.makedirs(os.path.join(folder, "s%02dx" % seg), exist_ok=True)
        stem = "s%02dx/%s" % (seg, os.path.basename(stems[name]))
        why = functions._compile(wcc, cflags + ["-nt=CODE%dX" % seg], env, folder, stem, texts[name])
        return None if why else stem

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
    if definitions.description(path):
        lines.append("option description '%s'" % definitions.description(path).replace("'", " "))
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
    if m.interrupts:                                           # the interrupts the module takes
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
    if cs and entry not in public:
        lines.append("file entry.obj")
    for seg in sorted(by_segment):                             # each code segment loaded as the original's was
        flags = segs[seg]["flags"] if seg in segs else 0x1D10
        lines.append("segment 'CODE%d' %s" % (seg, " ".join(on if flags & bit else off for bit, on, off in
                                                              _SEGMENT_OPTIONS if (on if flags & bit else off))))
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
        r = resources.compare(original, resource_reader.read(image), b.kind == "pe")
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
    """Load a module in the emulator and call one exported function with zero arguments.
    -> (how it ended, its result)"""
    from codebreaker.emulate import emulator, windows16
    from codebreaker.verify import compare
    try:
        if kind == "ne":
            r = windows16.probe(path, ordinal=export.ordinal, args=[0] * 8)
        else:
            from codebreaker.verify import win32
            r = emulator.trace_function(path, func=export.address, args=[0] * 4, mem_init=_sections(path),
                                        stub_externals=True, sig_resolver=win32._argc)
    except ImportError:
        raise
    except Exception as e:                                    # the emulator giving up is a result too
        return "error: %s" % type(e).__name__, None
    return compare._exit_norm(r.get("exit")), r.get("ret")


def run(b, limit=RUN_LIMIT):
    """Call the built module's exported functions in the emulator, and the original's beside."""
    mine = {e.ordinal: e for e in definitions.exports(b.image) if e.kind == "code"}
    for e in [x for x in definitions.exports(b.original) if x.kind == "code" and x.ordinal in mine][:limit]:
        got, want = _call(b.image, b.kind, mine[e.ordinal]), _call(b.original, b.kind, e)
        b.ran.append((e.name or "@%d" % e.ordinal, got[0], want[0], got == want))
    return b


def build(module, outdir=None, call=True, log=print):
    """Build a module whole. -> Built"""
    from codebreaker.verify import reconstruct
    path = corpus.retail(module)
    if not path:
        raise SystemExit("no module %r in the corpus" % module)
    b = Built(path)
    kind = reconstruct.kind_of(path)
    b.folder = folder = outdir or settings.state("build", b.name)
    os.makedirs(folder, exist_ok=True)
    log("%s: decompiling" % b.name)
    m = kind.prepare(path, os.path.join(folder, "decompiled"))
    b.functions = len(m.all_keys)
    log("%s: compiling %d functions, data and resources" % (b.name, len(m.order)))
    image = (_build16 if b.kind == "ne" else _build32)(b, path, m, folder)
    if not image:
        return b
    b.image = os.path.join(folder, os.path.basename(path).lower())
    shutil.copyfile(image, b.image)
    check(b)
    if call:
        run(b)
    return b


def main(argv):
    """python -m codebreaker build <module> [--out DIR] [--no-run]"""
    out = argv[argv.index("--out") + 1] if "--out" in argv else None
    names = [a for a in argv if not a.startswith("--") and a != out]
    if not names:
        print(__doc__)
        return 2
    bad = 0
    for name in names:
        b = build(name, outdir=out, call="--no-run" not in argv)
        print(b.report())
        if b.linked:
            print("  image       %s" % b.image)
        bad += 0 if b.linked else 1
    return 1 if bad else 0
