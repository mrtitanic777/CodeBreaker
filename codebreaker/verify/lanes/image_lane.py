"""image_lane.py -- the lane of a program image written back as its assembly source: a cartridge,
a tape block, a ROM. The NES, Master System, Game Boy, Genesis, ZX Spectrum and MSX lanes are each
one of these (verify/lanes/nes.py ...), giving only what is particular to their machine.

    LANE = Lane("gb", 1, STATES, effects, WHAT, start)
    LANE.export(globals())            # the module is then a kind reconstruct.py works with

The program's source is written by its platform's writer (the packager slot of
codebreaker/platforms/<machine>.py: source/<machine>_writer.py) and assembled; a
program that comes back identical IS the original, so each of its functions does what the
original's does. When it is not identical -- the source was edited -- each function is run in
both, from the same starting states, and what it did is compared: effects(subject, entry, state)
is a tuple of what the run showed (how it ended, the registers, the memory it left, the hardware
it touched), and WHAT names each place in it, so a difference says which.
"""
import hashlib
import os
from codebreaker.verify import verdicts as V

SAME = "the cartridge built from this source is the original, byte for byte"


class Prepared(object):
    pass


class Lane(object):
    """kind: the name results are filed under (its platform's: the writer is that platform's
    packager); rev: bump when what a verdict depends on changes; states: where each run starts
    (part of every verdict's profile); effects(subject, entry, state) -> a tuple; what: a name for
    each place in that tuple; start(state) -> the state, as a verdict's detail says it.

    subjects(module_path, written) -> (original, rebuilt): what effects() runs, for each side
    (default: the original's path and the built file's); key: how an address is written in the
    store's keys."""

    def __init__(self, kind, rev, states, effects, what, start, subjects=None, key="%04x", same=SAME):
        self.KIND, self.REV, self.SAME = kind, rev, same
        self._states, self._effects, self._what = states, effects, what
        self._start, self._subjects, self._key = start, subjects, key

    def platform(self):
        from codebreaker import platforms
        return platforms.of(self.KIND)

    def profile(self, module_path=None):
        settings = repr((self.KIND, self.REV, self._states))     # (the states are part of what a verdict depends on)
        return hashlib.sha1(settings.encode()).hexdigest()[:12], settings

    def prepare(self, module_path, work):
        m = Prepared()
        m.written = r = self.platform().packager.writer(module_path, os.path.join(work, "source"))
        with open(r["source"], encoding="latin1") as f:
            text = f.read()
        functions = r["functions"]
        m.header = text
        m.order = [functions[a].name for a in sorted(functions)]
        m.key = {functions[a].name: self._key % a for a in functions}
        m.all_keys = sorted(self._key % a for a in functions)
        m.texts = {n: "; %s, in %s" % (n, os.path.basename(r["source"])) for n in m.order}
        m.by_name = {functions[a].name: a for a in functions}
        m.sig = hashlib.sha1(("%d\0%s" % (self.REV, text)).encode("latin1", "replace")).hexdigest()
        return m

    def build(self, m, module_path, work):
        """-> (context for judge, {name: item}, {name: why it is not in the build})"""
        r = m.written
        if not r.get("linked"):
            why = (r.get("notes") or ["did not build"])[-1][:200]
            return None, {}, {n: why for n in m.order}
        original, rebuilt = self._subjects(module_path, r) if self._subjects else (module_path, r["built"])
        return (r["same"], original, rebuilt), {n: (m.key[n], m.by_name[n]) for n in m.order}, {}

    def judge(self, context, item):
        same, original, rebuilt = context
        if same:
            return V.EQUIVALENT, "", [], self.SAME
        _key, entry = item
        for state in self._states:
            a, b = self._effects(original, entry, state), self._effects(rebuilt, entry, state)
            if a != b:
                first = next(w for w, x, y in zip(self._what, a, b) if x != y)
                return V.DIVERGENT, first + "-differs", [], self._start(state)
        return V.EQUIVALENT, "", [], "the same effects from %d register states" % len(self._states)

    def export(self, namespace):
        """Make the module `namespace` belongs to a kind: KIND, REV, SAME and the four functions
        reconstruct.py calls. They are the module's own functions, not the lane's bound methods,
        because judge is handed to worker processes (jobs/pool.py), which find it by its name."""
        lane = self

        def profile(module_path=None):
            return lane.profile(module_path)

        def prepare(module_path, work):
            return lane.prepare(module_path, work)

        def build(m, module_path, work):
            return lane.build(m, module_path, work)

        def judge(context, item):
            return lane.judge(context, item)
        for f in (profile, prepare, build, judge):
            f.__module__, f.__qualname__ = namespace["__name__"], f.__name__
            namespace[f.__name__] = f
        namespace.update(LANE=self, KIND=self.KIND, REV=self.REV, SAME=self.SAME)
