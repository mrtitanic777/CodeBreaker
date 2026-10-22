"""nes.py -- what is particular to reconstructing a NES program.

One of the kinds reconstruct.py works with (see there for what a kind provides). The program's
source is assembly written from its cartridge (source/nes_writer.py); it is assembled and linked
(build/m6502_asm.py, or the cc65 suite), and the cartridge built is compared with the original.
One that comes back identical IS the original, so each of its functions does what the
original's does. When it is not identical -- the source was edited -- each function is run in
CodeBreaker's 6502 (emulate/m6502.py) in both cartridges, from the same registers, and what it
did is compared: how the run ended, A X Y and the flags, the RAM it left, and every access to
the picture and sound hardware, in order.
"""
import hashlib
import os

KIND = "nes"
REV = 2
SAME = "the cartridge built from this source is the original, byte for byte"
_HARDWARE = (0x2000, 0x401F)
# the registers a function starts from, and how its 2 KB of RAM is filled (seed 0: all zero)
_STATES = [dict(a=0, x=0, y=0, fill=0), dict(a=1, x=2, y=3, fill=1), dict(a=0x80, x=0xFF, y=0x7F, fill=2),
           dict(a=0x55, x=0x10, y=0xAA, fill=3), dict(a=0, x=0, y=0, fill=4), dict(a=0xFF, x=1, y=0, fill=5)]


def profile(module_path=None):
    settings = repr(("nes", REV, _STATES))      # (the states are part of what a verdict depends on)
    return hashlib.sha1(settings.encode()).hexdigest()[:12], settings


class Prepared(object):
    pass


def prepare(module_path, work):
    from codebreaker.source import nes_writer
    m = Prepared()
    m.written = r = nes_writer.reconstruct(module_path, os.path.join(work, "source"))
    with open(r["source"], encoding="latin1") as f:
        text = f.read()
    functions = r["functions"]
    m.header = text
    m.order = [functions[a].name for a in sorted(functions)]
    m.key = {functions[a].name: "%04x" % a for a in functions}
    m.all_keys = sorted("%04x" % a for a in functions)
    m.texts = {n: "; %s, in %s" % (n, os.path.basename(r["source"])) for n in m.order}
    m.by_name = {functions[a].name: a for a in functions}
    m.sig = hashlib.sha1(("%d\0%s" % (REV, text)).encode("latin1", "replace")).hexdigest()
    return m


def build(m, module_path, work):
    """-> (context for judge, {name: item}, {name: why it is not in the build})"""
    r = m.written
    if not r.get("linked"):
        why = (r.get("notes") or ["did not build"])[-1][:200]
        return None, {}, {n: why for n in m.order}
    return (r["same"], module_path, r["built"]), {n: (m.key[n], m.by_name[n]) for n in m.order}, {}


def _effects(path, entry, state):
    from codebreaker.emulate import m6502
    from codebreaker.formats import nes
    mem = nes.read(path).memory()
    seed = state["fill"]
    for i in range(0x800 if seed else 0):              # small numbers, as counters and indexes hold
        mem[i] = ((i * 7 + seed * 13) ^ (i >> 3)) & (0x0F if seed % 2 else 0xFF)
    bus = m6502.Bus(mem)
    bus.map(*_HARDWARE, read=lambda a: 0x80 if a == 0x2002 else 0)    # (the picture unit always in vblank)
    run = m6502.run(bus, entry, steps=20000, a=state["a"], x=state["x"], y=state["y"])
    ram = bytes(bus.mem[:0x800])
    return (run["end"], run["a"], run["x"], run["y"], run["p"], ram, tuple(bus.log[:4000]))


def judge(context, item):
    same, original, built = context
    if same:
        return "equivalent", "", [], SAME
    _key, entry = item
    for state in _STATES:
        a, b = _effects(original, entry, state), _effects(built, entry, state)
        if a != b:
            what = ("end", "A", "X", "Y", "flags", "RAM", "hardware accesses")
            first = next(w for w, x, y in zip(what, a, b) if x != y)
            return "divergent", first + "-differs", [], "from A=%(a)02X X=%(x)02X Y=%(y)02X" % state
    return "equivalent", "", [], "the same effects from %d register states" % len(_STATES)
