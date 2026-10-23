"""sms.py -- what is particular to reconstructing a Master System / Game Gear program.

One of the kinds reconstruct.py works with (see there for what a kind provides). The program's
source is assembly written from its cartridge (source/sms_writer.py); it is assembled
(build/z80_asm.py, or WLA-DX), and the cartridge built is compared with the original. One that
comes back identical IS the original, so each of its functions does what the original's does.
When it is not identical -- the source was edited -- each function is run in CodeBreaker's Z80
(emulate/z80.py) in both cartridges, from the same registers, and what it did is compared: how
the run ended, the registers and flags, the 8 KB of RAM it left, and every port access and
mapper write, in order.
"""
import hashlib
import os

KIND = "sms"
REV = 1
SAME = "the cartridge built from this source is the original, byte for byte"
_MAPPER = (0xFFFC, 0xFFFF)
# the registers a function starts from, and how its RAM is filled (seed 0: all zero)
_STATES = [dict(a=0, b=0, c=0, d=0, e=0, h=0, l=0, fill=0), dict(a=1, b=2, c=3, d=0xC1, e=0x00, h=0xC2, l=0x10, fill=1),
           dict(a=0x80, b=0xFF, c=0x7F, d=0xC0, e=0x80, h=0xC1, l=0x00, fill=2),
           dict(a=0x55, b=0x10, c=0xAA, d=0xD0, e=0x00, h=0xC8, l=0x40, fill=3)]


def profile(module_path=None):
    settings = repr(("sms", REV, _STATES))
    return hashlib.sha1(settings.encode()).hexdigest()[:12], settings


class Prepared(object):
    pass


def prepare(module_path, work):
    from codebreaker.source import sms_writer
    m = Prepared()
    m.written = r = sms_writer.reconstruct(module_path, os.path.join(work, "source"))
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
    from codebreaker.emulate import z80
    from codebreaker.formats import sms
    mem = sms.read(path).memory()
    seed = state["fill"]
    for i in range(0x2000 if seed else 0):            # small numbers, as counters and indexes hold
        mem[0xC000 + i] = ((i * 7 + seed * 13) ^ (i >> 3)) & (0x0F if seed % 2 else 0xFF)
    bus = z80.Bus(mem, inp=lambda port: 0x80 if port & 0xFF == 0xBF else 0xFF)   # (the VDP always in vblank)
    bus.map(*_MAPPER)
    regs = {k: v for k, v in state.items() if k != "fill"}
    run = z80.run(bus, entry, steps=20000, **regs)
    ram = bytes(bus.mem[0xC000:0xE000])
    return (run["end"], run["a"], run["f"], run["bc"], run["de"], run["hl"], run["ix"], run["iy"], ram,
            tuple(bus.log[:4000]))


def judge(context, item):
    same, original, built = context
    if same:
        return "equivalent", "", [], SAME
    _key, entry = item
    for state in _STATES:
        a, b = _effects(original, entry, state), _effects(built, entry, state)
        if a != b:
            what = ("end", "A", "flags", "BC", "DE", "HL", "IX", "IY", "RAM", "port and mapper accesses")
            first = next(w for w, x, y in zip(what, a, b) if x != y)
            return "divergent", first + "-differs", [], "from A=%(a)02X BC=%(b)02X%(c)02X HL=%(h)02X%(l)02X" % state
    return "equivalent", "", [], "the same effects from %d register states" % len(_STATES)
