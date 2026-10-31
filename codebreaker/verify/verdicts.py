"""verdicts.py -- one verdict vocabulary: what a judge says of a function or a source pair, binary or source alike.

    from codebreaker.verify import verdicts as V
    V.EQUIVALENT, V.DIVERGENT, ...          the words a judge gives (stored as they are)
    V.verdict("same")        -> "equivalent"     any older spelling, as the word now (map on read)
    V.classify(verdict, limits)              -> (class, weak reasons): what it amounts to
    V.TITLE[cls], V.LABEL[cls], V.SAID[v]    how each is written for people
    V.match("does not compile") -> "no-compile"   a byte comparison's word
    V.Index(module, rows).at(origin)         a function's row by its L2 provenance {module, va}

Three things are said, each in one set of words.

1. What a JUDGE says of a pair -- a reconstructed function beside the original in an emulator
   (verify/lanes/*), a module's image beside the original's (image_lane, dos), a port's program
   beside the original (port/verify), the 16-bit gate's C beside the module's own asm
   (verify/gate16), a Java class beside its original (verify/lanes/java):

       equivalent     behaves as the original: every compared result alike
       consistent     alike only in failing -- neither side returned normally (counts as weak)
       divergent      behaves otherwise (the reason says how: ret-differs, writes-differ, ...)
       inconclusive   not judged: it could not be run, or a verdict was carried, not reached here
       no-compile     it did not build
       no-source      nothing to judge: no reconstruction of it

2. What a verdict AMOUNTS TO, best first (the function store's classes, what the module writer,
   the terminal, the MCP tools, the ledger and the reports count): verified, weak, identified
   (a runtime library's function, counted on its own), unverified, not-judged, no-compile,
   no-source. TITLE is how a source file and the reports write a class, LABEL how the terminal
   does in a narrow column; the two say the same thing.

3. What a BYTE comparison says (a source compiled and held to the original's bytes:
   verify/authored, the store's variant table): identical, same size, different, no original
   extent (nothing to compare it with), no-compile; identified (a runtime library's: linked,
   not written).

Older spellings are read as the words now (verdict(), klass(), match()): a store or a cache
written before keeps meaning what it meant. "equivalent" is the judge's word for behaving alike
and nothing else: exact/'s byte ledger (identical, differs, "equivalent" up to another compiler
state's habits) is a ledger of bytes, not a verdict, and keeps its own words.

The vocabulary is also C++ (native/engine/verdicts.cc: the function store's classes are worked out
there); with CB_NATIVE_ENGINE on, verdict(), klass(), match() and classify() answer from it. The
tables below are the words themselves, which the C++ is held to (tests/test_verdicts.py).
"""
import collections


def _native():
    from codebreaker.engine import native_engine
    return native_engine.enabled()

# ---- 1. what a judge says ------------------------------------------------------------------------
EQUIVALENT = "equivalent"
CONSISTENT = "consistent"
DIVERGENT = "divergent"
INCONCLUSIVE = "inconclusive"
NO_COMPILE = "no-compile"
NO_SOURCE = "no-source"
VERDICTS = (EQUIVALENT, CONSISTENT, DIVERGENT, INCONCLUSIVE, NO_COMPILE, NO_SOURCE)
GOOD = (EQUIVALENT, CONSISTENT)             # a verdict that is a match (weak or not)
NOT_RUN = (INCONCLUSIVE, "judge: the emulator crashed on this function", [], "")

# every older spelling of a judge's word -> the word now
OLD = {
    "no-synth": NO_SOURCE,                  # (reconstruct.run's row for a function the decompiler gave nothing)
    "same": EQUIVALENT,                     # (the 16-bit gate, the port's cases)
    "alike": EQUIVALENT,
    "differs": DIVERGENT,
    "different": DIVERGENT,
    "not run": INCONCLUSIVE,
    "not-run": INCONCLUSIVE,
    "not judged": INCONCLUSIVE,
    "untested": INCONCLUSIVE,
    "does not compile": NO_COMPILE,
    "not compiled": NO_COMPILE,
}

# how a judge's word is said in a report of pairs (the port's cases, the gate's log)
SAID = {EQUIVALENT: "same", CONSISTENT: "same (both fail)", DIVERGENT: "differs", INCONCLUSIVE: "not run",
        NO_COMPILE: "does not compile", NO_SOURCE: "no source"}


def verdict(word):
    """A judge's word as it is now (an older spelling mapped); None stays None, an unknown word
    stays itself (classify() counts it as no-source)."""
    if word is None:
        return None
    if isinstance(word, str) and _native():
        from codebreaker.engine import native_engine
        return native_engine.verdicts_call("verdict", word)[0]
    return OLD.get(word, word)


def pair(same, ran=True, built=True):
    """The verdict of a yes/no comparison of a pair."""
    if not built:
        return NO_COMPILE
    if not ran:
        return INCONCLUSIVE
    return EQUIVALENT if same else DIVERGENT


def good(word):
    return verdict(word) in GOOD


# ---- 2. what it amounts to -----------------------------------------------------------------------
VERIFIED = "verified"
WEAK = "weak"
UNVERIFIED = "unverified"
NOT_JUDGED = "not-judged"
IDENTIFIED = "identified"
ORDER = (VERIFIED, WEAK, UNVERIFIED, NOT_JUDGED, NO_COMPILE, NO_SOURCE)        # best first
SCORED = ORDER[:2] + (IDENTIFIED,) + ORDER[2:]
BUILT = (VERIFIED, WEAK, UNVERIFIED, NOT_JUDGED)                               # compiled and run
TITLE = {VERIFIED: "VERIFIED", WEAK: "VERIFIED (weak)", UNVERIFIED: "UNVERIFIED", NOT_JUDGED: "NOT JUDGED",
         NO_COMPILE: "DOES NOT COMPILE", NO_SOURCE: "NO SOURCE", IDENTIFIED: "IDENTIFIED (runtime library)"}
LABEL = {VERIFIED: "verified", WEAK: "weak", UNVERIFIED: "unverified", NOT_JUDGED: "not judged",
         NO_COMPILE: "does not compile", NO_SOURCE: "no source", IDENTIFIED: "runtime library"}
CLASS = {EQUIVALENT: VERIFIED, CONSISTENT: WEAK, DIVERGENT: UNVERIFIED, INCONCLUSIVE: NOT_JUDGED,
         NO_COMPILE: NO_COMPILE, NO_SOURCE: NO_SOURCE}

# limits that make a match weaker than it looks
WEAK_LIMITS = {"fault-only": "the two sides only ever fail alike; neither returned normally",
               "low-coverage": "under half of the original's instructions ran",
               "truncated": "a run was cut off at the step limit",
               "unobserved": "some inputs could not be run on one side",
               "writes-capped": "too many memory writes to compare them all",
               "stub-callee": "a function it calls was a stand-in, not that function's reconstruction",
               "argc-unknown": "an API call was compared by name only",
               "calls-capped": "it makes more calls than are compared",
               "methods-partial": "only some of the class's methods could be run beside the original (java)"}

# every older spelling of a class -> the class now
CLASS_OLD = {"not judged": NOT_JUDGED, "does not compile": NO_COMPILE, "no source": NO_SOURCE,
             "runtime library": IDENTIFIED, "run-time library": IDENTIFIED,
             "verified (weak)": WEAK, "identified (runtime library)": IDENTIFIED}


def klass(word):
    """A class as it is now: its TITLE or LABEL, or an older spelling, read back."""
    if word is None:
        return None
    if isinstance(word, str) and _native():
        from codebreaker.engine import native_engine
        return native_engine.verdicts_call("klass", word)[0]
    w = word.strip()
    if w in TITLE:
        return w
    low = w.lower()
    if low in TITLE:
        return low
    return CLASS_OLD.get(low, w)


def classify(word, limits=""):
    """(class, weak reasons) for one verdict and its limits."""
    if (word is None or isinstance(word, str)) and isinstance(limits or "", str) and _native():
        from codebreaker.engine import native_engine
        got = native_engine.verdicts_call("classify", word, limits or "")
        return got[0], got[1:]
    v = verdict(word)
    codes = [c for c in (limits or "").split(",") if c]
    weak = [c for c in codes if c in WEAK_LIMITS]
    if v == EQUIVALENT:
        return (WEAK if weak else VERIFIED), weak
    if v == CONSISTENT:
        return WEAK, weak or ["fault-only"]
    return CLASS.get(v, NO_SOURCE), []


# ---- 3. a byte comparison ------------------------------------------------------------------------
IDENTICAL = "identical"
SAME_SIZE = "same size"
DIFFERENT = "different"
NO_EXTENT = "no original extent"
MATCHES = (IDENTICAL, SAME_SIZE, DIFFERENT, NO_EXTENT, NO_COMPILE)            # best first
MATCH_OLD = {"does not compile": NO_COMPILE, "same length, different": SAME_SIZE, "different length": DIFFERENT,
             "run-time library": IDENTIFIED, "runtime library": IDENTIFIED}
MATCH_LABEL = {IDENTICAL: "identical", SAME_SIZE: "same size", DIFFERENT: "different",
               NO_EXTENT: "no original extent", NO_COMPILE: "does not compile", IDENTIFIED: "runtime library"}


def match(word):
    """A byte comparison's word as it is now (an older spelling mapped)."""
    if word is None:
        return None
    if isinstance(word, str) and _native():
        from codebreaker.engine import native_engine
        return native_engine.verdicts_call("match", word)[0]
    return MATCH_OLD.get(word, word)


# ---- by provenance -------------------------------------------------------------------------------
Verdict = collections.namedtuple("Verdict", "module addr verdict reason limits detail")


def of_row(module, row):
    """A store row ({addr, verdict, reason, limits, detail}) as a Verdict, its word as it is now."""
    return Verdict(module, row.get("addr"), verdict(row.get("verdict")), row.get("reason") or "",
                   row.get("limits") or "", row.get("detail") or "")


class Index(object):
    """A module's rows by L2 provenance: at({module, va}) -> the row of the function the node
    stands for, or None. The module writer reads a function's verdict so, from the tree's node,
    not by its name (a function renamed keeps its verdict)."""

    def __init__(self, module, rows):
        self.module = module
        self.rows = {(module, addr): r for addr, r in rows.items()}

    def at(self, origin):
        if not origin:
            return None
        return self.rows.get((origin.get("module"), origin.get("va")))

    def origin(self, addr):
        return {"module": self.module, "va": addr}
