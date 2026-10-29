"""run.py -- one record of a run, whichever emulator made it.

Every platform's emulator slot (codebreaker/platforms: p.emulator.run) returns a Run: the same
fields under the same names, whether the run was the 32-bit tracer's, the 16-bit one's, a
driver's, the PowerPC interpreter's or one of the console machines'. A judge (verify/lanes/*.py)
turns it into what verify/compare.py compares -- through lane_common.fingerprint, lane_common.left
and lane_common.calls -- and gives only what is its own: how it names a call, which places it
leaves out, what of the result is a result.

    run.exit        how it ended, in the emulator's own words ("ret", "exception: ...", "return")
    run.ended       the same as the comparer says it: "ret", "fault", "truncated", or the word
                    before a colon (ended())
    run.results     {register: value} the result registers as the run left them ("EAX", "AX" and
                    "DX:AX", "r3" and "f1", a machine's A X Y ...)
    run.writes      [Write] the memory it wrote, in order (address, size, value, what the bytes
                    held just before -- None when not known --, harness: written by the emulator
                    for the run, not by the code: a stand-in's return through zeros)
    run.capped      more writes were made than were recorded: the writes are not all there
    run.calls       [Call] the calls out of the code, in order: name, args (None when their
                    number is not known), detail (the emulator's own note of it)
    run.io          [IO] accesses to devices, in order: step, kind ("in", "out", ...), port,
                    value, size (a machine's bus log: its own tuples)
    run.stack       how far the stack pointer moved from entry to the end (None: not known)
    run.preserved   {register: (on entry, at the end)} the registers a function must hand back,
                    None when they were not read
    run.path        the addresses of the instructions that ran (in order where the emulator keeps
                    the order)
    run.steps       instructions it took
    run.truncated   cut off at the step limit (or another of the emulator's caps)
    run.endian      how a written value's bytes lie in memory ("little", "big")
    run.raw         the emulator's own record, for what only one lane asks of it

A Run is built by its emulator's `record(...)` from the record the emulator always made (those
stay what they were: other tools read them)."""
from collections import namedtuple

Write = namedtuple("Write", "address size value old harness")
Call = namedtuple("Call", "name args detail")
IO = namedtuple("IO", "step kind port value size")


def ended(e):
    """How a run ended, as the comparer says it: "none" (no word), "ret", "fault" (an exception),
    else the word before any colon ("truncated", "wild", "int:21" -> "int")."""
    if e is None:
        return "none"
    e = str(e)
    return "ret" if e == "ret" else ("fault" if e.startswith("exception") else e.split(":")[0])


class Run(object):
    __slots__ = ("exit", "results", "writes", "capped", "calls", "io", "stack", "preserved", "path", "steps",
                 "truncated", "endian", "raw")

    def __init__(self, exit, results=None, writes=(), capped=False, calls=(), io=(), stack=None, preserved=None,
                 path=(), steps=None, truncated=False, endian="little", raw=None):
        self.exit, self.results, self.writes, self.capped = exit, dict(results or {}), writes, bool(capped)
        self.calls, self.io, self.stack, self.preserved = calls, io, stack, preserved
        self.path, self.steps, self.truncated, self.endian = path, steps, bool(truncated), endian
        self.raw = raw if raw is not None else {}

    @property
    def ended(self):
        return ended(self.exit)

    def kept(self, names=None):
        """Whether the registers a function must hand back came back as they went in (those of
        `names`, else every one recorded); False when they were not read."""
        if self.preserved is None:
            return False
        regs = self.preserved if names is None else {n: self.preserved.get(n, (None, None)) for n in names}
        return all(a == b for a, b in regs.values())

    def __repr__(self):
        return "<Run %s steps=%s calls=%d writes=%d%s>" % (self.exit, self.steps, len(self.calls), len(self.writes),
                                                           " truncated" if self.truncated else "")
