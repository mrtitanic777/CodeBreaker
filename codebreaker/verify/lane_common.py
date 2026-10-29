"""lane_common.py -- what every kind's judge does the same way.

A kind of module (verify/lanes/win32.py, win16.py, powerpc.py, driver.py) knows how to run its
code -- its emulator, its calling convention, which addresses mean what -- and turns a run into a
fingerprint for verify/compare.py. What is left over is the same for all of them, and lives here
so that it stays the same:

    the switches     CB_NO_FP_CACHE, CB_NO_GUIDED and CB_PLAIN, read in one place
    remembered()     a fingerprint kept across runs, under a key naming everything the run depends on
    call(), calls()  a recorded call as the text two builds of the same code agree on
    argument()       one argument of such a call
    writes()         the memory a run left changed, as the digest a fingerprint carries
    the inputs       the argument patterns the kinds' tables of inputs are made of
    guided()         branch-guided inputs (verify/guided.py), with the limits every kind uses
    explored()       the same found path by path (verify/concolic.py), where the kind can follow
                     a run's values symbolically; and the limits a verdict then carries
    judged()         one function's verdict, whatever goes wrong while reaching it
"""
import hashlib
import os

from codebreaker.verify import compare


# --- the switches -----------------------------------------------------------------------------
def fp_cache_on():
    """False when CB_NO_FP_CACHE is set: every run is made afresh, none is kept (the test suites
    run so, since a verdict must then reflect the code as it is)."""
    return not os.environ.get("CB_NO_FP_CACHE")


def guided_on():
    """False when CB_NO_GUIDED is set: no branch-guided inputs (to measure what they add)."""
    return not os.environ.get("CB_NO_GUIDED")


def concolic_on():
    """False when CB_NO_CONCOLIC is set: the branch-guided inputs are guessed (verify/guided.py)
    rather than solved for along each path (verify/concolic.py) -- to compare the two."""
    return not os.environ.get("CB_NO_CONCOLIC")


# CB_PLAIN=1: one instruction at a time on a fresh machine each run -- the slow way the fast one
# must agree with (the 32-bit emulator; read once, when the judge is loaded)
PLAIN = bool(os.environ.get("CB_PLAIN"))


# --- fingerprints kept across runs --------------------------------------------------------------
def remembered(key, run, keep=None):
    """run()'s fingerprint, or the one it gave before under `key`.

    A run is a pure function of what its key names -- the module's content, the function, the
    input, whatever else the kind varies -- so a fingerprint made once can be used again: by the
    next judgement of the same original, by the next sweep. A key of None (the module has no
    content hash) is never kept. A run that was not an observation ("err") is never kept either,
    nor one `keep(fp)` turns down."""
    if key is None or not fp_cache_on():
        return run()
    fp = compare.cache_get(key)
    if fp is None:
        fp = run()
        if "err" not in fp and (keep is None or keep(fp)):
            compare.cache_put(key, fp)
    return fp


# --- calls, as two builds agree on them ---------------------------------------------------------
def call(name, args=None):
    """One call as a fingerprint records it: `name(a,b,...)`, or just `name` when how many
    arguments it takes is not known -- compare.py counts those (argc-unknown), since two calls
    alike by name alone may still differ in what they were given."""
    return name if args is None else "%s(%s)" % (name, ",".join(args))


def calls(seq, argument, name=str, skip=None, cap=None):
    """The calls a run made, in order, as a fingerprint records them. `seq` is (name, args) for
    each, args None when their number is not known; each name goes through `name` and each
    argument through `argument(value)`. Calls `skip(name)` says are not the function's own are
    left out. Past `cap` calls the rest are one "..." (compare.py: calls-capped)."""
    out = []
    for api, args in seq:
        if skip and skip(api):
            continue
        if cap is not None and len(out) >= cap:
            out.append("...")
            break
        out.append(call(name(api), None if args is None else [argument(a) for a in args]))
    return tuple(out)


def argument(v, inputs=(), above=None, small=False, frame=(), image=None):
    """One argument value in a form two builds of the same code agree on. In this order:

        "a2"     it is one of the function's own inputs (the third), when it is above `above`
                 (a small input is as likely to be a constant that happens to equal it)
        "-1"     with `small`, a small constant, as a signed number
        "S"      an address in one of the `frame` ranges: the stack, or anything else that is
                 "somewhere in this call's frame" and lies elsewhere in each build
        "P"      an address in this build's own image (`image`: a (low, high) range or a test),
                 where the two builds do not have the same layout
        "1f3a"   otherwise the value itself

    A kind with forms of its own (a selector, the contents of what a pointer points at) tests
    for those first and hands the rest to this."""
    if v in inputs and (above is None or v > above):
        return "a%d" % inputs.index(v)
    if small:
        sv = v - (1 << 32) if v & 0x80000000 else v
        if -0x10000 < sv < 0x10000:
            return "%d" % sv
    if any(span and span[0] <= v < span[1] for span in frame):
        return "S"
    if image is not None and (image(v) if callable(image) else image[0] <= v < image[1]):
        return "P"
    return "%x" % v


def writes(state):
    """The memory a run left changed -- {place: final value}, holding only places that ended up
    with a value other than the one they had -- as a fingerprint carries it: how many, and a
    digest of which and what. "" when nothing changed."""
    if not state:
        return ""
    return "%d:%s" % (len(state), hashlib.sha1(repr(sorted(state.items())).encode()).hexdigest()[:16])


# --- inputs -------------------------------------------------------------------------------------
# The argument patterns the kinds' inputs are made of. Every argument gets a DIFFERENT value
# wherever the pattern allows: with one value in every slot, a body that uses its parameters in
# the wrong order behaves the same as the right one.
def counting(n, first=1, step=1):
    """first, first + step, ...: distinct, small, increasing."""
    return [first + step * i for i in range(n)]


def descending(n, top):
    """top, top - 1, ...: large, and negative when read as signed."""
    return [top - i for i in range(n)]


def alternating(n, even, odd):
    """Arguments 0, 2, 4 ... from `even` and 1, 3, 5 ... from `odd`, each a value or a function
    of the argument's position (a pointer in one place, an integer in the next)."""
    def at(f, i):
        return f(i) if callable(f) else f
    return [at(even, i) if i % 2 == 0 else at(odd, i) for i in range(n)]


MAX_GUIDED = 16                 # branch-guided inputs kept per function (verify/guided.py)
GUIDED_BUDGET = 96              # runs of the original it may spend finding them


def guided(run, seeds, seed, **options):
    """Inputs that take branches of the original no seed took (verify/guided.py guided_inputs,
    with these limits; `options` say how wide a word is and which way round its bytes go), or
    none when CB_NO_GUIDED is set. `run(input)` traces the original with its branches logged."""
    if not guided_on():
        return []
    from codebreaker.verify import guided as explorer
    return explorer.guided_inputs(run, seeds, budget=GUIDED_BUDGET, keep=MAX_GUIDED, seed=seed, **options)


PATH_BUDGET = 32                # runs of the original the path explorer may spend (verify/concolic.py)
MAX_PATHS = 24                  # inputs it keeps, one per new path
GUIDED_REST = 32                # runs then left to guessing, where the paths found are not all


def explored(run, seeds, derive, own, known=None):
    """Inputs for the paths of the original no seed took (verify/concolic.py explore, with these
    limits): -> its Exploration, whose .kept are the inputs."""
    from codebreaker.verify import concolic
    return concolic.explore(run, seeds, derive, own=own, budget=PATH_BUDGET, keep=MAX_PATHS, known=known)


def path_limits(got):
    """What an exploration adds to a verdict's limits: `paths=N`, the distinct paths the original
    took on the inputs, and `paths-all` when those are all it has (concolic.Exploration.complete)."""
    if got is None:
        return []
    return ["paths=%d" % got.paths] + (["paths-all"] if got.complete else [])


# --- the verdict --------------------------------------------------------------------------------
def judged(compare_one):
    """(verdict, reason, limits, detail) for one function. compare_one(info) makes the comparison
    (compare.compare, handed `info`) and returns its (verdict, reason, limits). Anything that goes
    wrong on the way makes this function inconclusive -- one function must not sink the rest --
    except a missing module: a broken setup is not an observation."""
    info = {}
    try:
        verdict, reason, limits = compare_one(info)
    except ImportError:
        raise
    except Exception as e:
        verdict, reason, limits = "inconclusive", "judge: %s" % type(e).__name__, []
    return verdict, reason, list(limits), info.get("detail", "")
