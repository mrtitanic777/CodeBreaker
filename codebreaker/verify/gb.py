"""gb.py -- what is particular to reconstructing a Game Boy program.

One of the kinds reconstruct.py works with (see there for what a kind provides). The program's
source is assembly written from its cartridge (source/gb_writer.py); it is assembled
(build/sm83_asm.py, or RGBDS), and the cartridge built is compared with the original. One that
comes back identical IS the original, so each of its functions does what the original's does.
When it is not identical -- the source was edited -- each function is run in CodeBreaker's
SM83 (emulate/sm83.py) in both cartridges, from the same registers, and what it did is compared:
how the run ended, the registers and flags, work RAM and high RAM as it left them, and every
access to the I/O registers and the bank controller, in order.
"""
import hashlib
import os

KIND = "gb"
REV = 1
SAME = "the cartridge built from this source is the original, byte for byte"
_IO = (0xFF00, 0xFF7F)
_MBC = (0x0000, 0x7FFF)       # writes there go to the bank controller
_STATES = [dict(a=0, b=0, c=0, d=0, e=0, h=0, l=0, fill=0), dict(a=1, b=2, c=3, d=0xC1, e=0x00, h=0xC2, l=0x10, fill=1),
           dict(a=0x80, b=0xFF, c=0x7F, d=0xC0, e=0x80, h=0xC1, l=0x00, fill=2),
           dict(a=0x55, b=0x10, c=0xAA, d=0xD0, e=0x00, h=0xC8, l=0x40, fill=3)]


def profile(module_path=None):
    settings = repr(("gb", REV, _STATES))
    return hashlib.sha1(settings.encode()).hexdigest()[:12], settings


class Prepared(object):
    pass


def prepare(module_path, work):
    from codebreaker.source import gb_writer
    m = Prepared()
    m.written = r = gb_writer.reconstruct(module_path, os.path.join(work, "source"))
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
    r = m.written
    if not r.get("linked"):
        why = (r.get("notes") or ["did not build"])[-1][:200]
        return None, {}, {n: why for n in m.order}
    return (r["same"], module_path, r["built"]), {n: (m.key[n], m.by_name[n]) for n in m.order}, {}


def _effects(path, entry, state):
    from codebreaker.emulate import sm83
    from codebreaker.formats import gb
    mem = gb.read(path).memory()
    seed = state["fill"]
    for i in range(0x2000 if seed else 0):
        mem[0xC000 + i] = ((i * 7 + seed * 13) ^ (i >> 3)) & (0x0F if seed % 2 else 0xFF)
    bus = sm83.Bus(mem)
    bus.map(*_IO, read=lambda a: 0x90 if a == 0xFF44 else 0)      # (LY in vblank)
    bus.map(0xFFFF, 0xFFFF)
    bus.map(0x2000, 0x3FFF, read=lambda a: mem[a])                 # bank select writes, logged
    regs = {k: v for k, v in state.items() if k != "fill"}
    run = sm83.run(bus, entry, steps=20000, **regs)
    ram = bytes(bus.mem[0xC000:0xE000]) + bytes(bus.mem[0xFF80:0xFFFF])
    io = tuple(x for x in bus.log if not (x[0] == "r" and 0x2000 <= x[1] <= 0x3FFF))[:4000]
    return (run["end"], run["a"], run["f"], run["bc"], run["de"], run["hl"], ram, io)


def judge(context, item):
    same, original, built = context
    if same:
        return "equivalent", "", [], SAME
    _key, entry = item
    for state in _STATES:
        a, b = _effects(original, entry, state), _effects(built, entry, state)
        if a != b:
            what = ("end", "A", "flags", "BC", "DE", "HL", "RAM", "I/O and bank accesses")
            first = next(w for w, x, y in zip(what, a, b) if x != y)
            return "divergent", first + "-differs", [], "from A=%(a)02X BC=%(b)02X%(c)02X HL=%(h)02X%(l)02X" % state
    return "equivalent", "", [], "the same effects from %d register states" % len(_STATES)
