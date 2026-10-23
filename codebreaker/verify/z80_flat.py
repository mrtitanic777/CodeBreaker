"""z80_flat.py -- the lane of a flat Z80 program (one block at one address): the ZX Spectrum's and
the MSX's (verify/zx.py, verify/msx.py), each giving its reader, writer and machine.

The program's source is written (source/z80_writer.py through the machine's writer) and built
(build/z80_asm.py build_flat); a block that comes back identical IS the original. When it is not
-- the source was edited -- each function is run in CodeBreaker's Z80 (emulate/z80.py) in the
original's memory and in the rebuilt one's, from the same registers, and what it did is compared:
how the run ended, the registers and flags, the memory it left, and every port access, in order.
The machine's ROM is not run: each of its bytes reads as RET, so a call into the ROM returns at
once, the same way in both runs.
"""
import hashlib
import os

STATES = [dict(a=0, b=0, c=0, d=0, e=0, h=0, l=0, fill=0), dict(a=1, b=2, c=3, d=0xC1, e=0x00, h=0xC2, l=0x10, fill=1),
          dict(a=0x80, b=0xFF, c=0x7F, d=0xD0, e=0x80, h=0xE1, l=0x00, fill=2)]


class Prepared(object):
    pass


class Lane(object):
    """kind: the name results are filed under; read(path) -> a Program (formats/zx.py, msx.py);
    writer: the machine's writer module; rom: the addresses the machine's ROM occupies."""

    def __init__(self, kind, rev, read, writer, rom):
        self.KIND, self.REV, self._read, self._writer, self._rom = kind, rev, read, writer, rom
        self.SAME = "the program built from this source is the original, byte for byte"

    def profile(self, module_path=None):
        settings = repr((self.KIND, self.REV, STATES))
        return hashlib.sha1(settings.encode()).hexdigest()[:12], settings

    def prepare(self, module_path, work):
        m = Prepared()
        m.written = r = self._writer.reconstruct(module_path, os.path.join(work, "source"))
        with open(r["source"], encoding="latin1") as f:
            text = f.read()
        functions = r["functions"]
        m.header = text
        m.order = [functions[a].name for a in sorted(functions)]
        m.key = {functions[a].name: "%04x" % a for a in functions}
        m.all_keys = sorted("%04x" % a for a in functions)
        m.texts = {n: "; %s, in %s" % (n, os.path.basename(r["source"])) for n in m.order}
        m.by_name = {functions[a].name: a for a in functions}
        m.sig = hashlib.sha1(("%d\0%s" % (self.REV, text)).encode("latin1", "replace")).hexdigest()
        return m

    def build(self, m, module_path, work):
        r = m.written
        if not r.get("linked"):
            why = (r.get("notes") or ["did not build"])[-1][:200]
            return None, {}, {n: why for n in m.order}
        p = self._read(module_path)
        return ((r["same"], p, r["built image"]), {n: (m.key[n], m.by_name[n]) for n in m.order}, {})

    def _effects(self, program, image, entry, state):
        from codebreaker.emulate import z80
        mem = bytearray(program.memory) if hasattr(program, "memory") else bytearray(0x10000)
        lo, hi = self._rom
        mem[lo:hi] = b"\xC9" * (hi - lo)                       # the ROM: every byte RET
        mem[program.origin:program.origin + len(image)] = image
        seed = state["fill"]
        end = program.origin + len(image)
        for i in range(end, 0x10000 if seed else end):         # memory past the program
            mem[i] = ((i * 7 + seed * 13) ^ (i >> 3)) & (0x0F if seed % 2 else 0xFF)
        bus = z80.Bus(mem, inp=lambda port: 0xFF)
        regs = {k: v for k, v in state.items() if k != "fill"}
        before = bytes(mem)
        run = z80.run(bus, entry, steps=20000, sp=0xFFF0, **regs)
        # what the run changed (the program lives in this memory: an edited byte the function does not
        # write is the source's difference, not the function's)
        changed = tuple((i, v) for i, (u, v) in enumerate(zip(before, bus.mem)) if u != v)
        return (run["end"], run["a"], run["f"], run["bc"], run["de"], run["hl"], run["ix"], run["iy"], changed,
                tuple(bus.log[:4000]))

    def judge(self, context, item):
        same, program, built = context
        if same:
            return "equivalent", "", [], self.SAME
        _key, entry = item
        for state in STATES:
            a = self._effects(program, program.image, entry, state)
            b = self._effects(program, built, entry, state)
            if a != b:
                what = ("end", "A", "flags", "BC", "DE", "HL", "IX", "IY", "memory", "port accesses")
                first = next(w for w, x, y in zip(what, a, b) if x != y)
                return "divergent", first + "-differs", [], "from A=%(a)02X BC=%(b)02X%(c)02X HL=%(h)02X%(l)02X" % state
        return "equivalent", "", [], "the same effects from %d register states" % len(STATES)
