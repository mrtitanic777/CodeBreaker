"""concolic.py -- inputs for every path of the original that its conditions let an input reach.

    got = concolic.explore(run, seeds, derive, own=..., budget=24)
    got.kept       inputs, each taking a path of the original no input before it took
    got.paths      how many distinct paths (of the function's own jumps) the runs took
    got.complete   True when those are all the paths there are (see below)

verify/guided.py guesses: it watches each comparison before a jump and tries the value compared
with in the places the input could hold it. This one knows: each run of the original is followed
by its symbolic shadow (emulate/shadow.py: the decompiler's own p-code of each instruction,
evaluated over the inputs), so the run hands back the conditions its path went by, as
expressions over the inputs (arguments, bytes of memory, what an unmodelled Windows call
returns). One condition turned the other way, with all those before it kept, is a question for
an SMT solver (z3): an input that answers it takes the same way to that jump and then the other
way -- a new path, and often a jump direction no input took yet. Asked again of each new run,
this walks the tree of the function's paths (generational search):

  * the jumps whose other side no run has reached are asked first, those of the function's own
    code before those of what it calls, then the rest (another path through the same code), and
    last a string instruction's count (`rep movsb` as many times again);
  * a condition the solver proves cannot be turned is a path that does not exist;
  * of the inputs that turn a comparison, one at its bound is taken where there is one (what
    was compared equal to what it was compared with, or one off it), so that a bound written one
    off in a rebuild shows;
  * one input is kept for each new path, to be run on both sides by the judge.

`complete` is the strong claim: every condition of every path found was turned or proved
impossible, the solver never gave up, and no run left anything unfollowed (shadow `lost`: a
value of the inputs into something the shadow does not follow, a run cut off at the step limit,
more conditions than are kept). Then the paths found are every path of the original for the
inputs the explorer may set -- addresses and selectors held where the inputs put them (pinned):
a pointer argument points where it was given. A function whose loop runs as many times as an
input says has more paths than any budget; there `complete` is False and the paths found are a
sample, as before.

`run(v)` runs the original on input v with its shadow and returns {"trail": [(site, taken)],
"path": shadow.path, "vars": shadow.vars, "lost": set, "cut": whether the run was cut off at the
step limit} or None (not an observation). An input whose run was cut off is not kept: its path is
counted (its conditions are asked about like any other), but it compares nothing to its end, and
a verdict with it in would be weak for it.
`derive(v, values, vars)` makes the input that is v with the named inputs (shadow.vars names) set
to `values`. `own(site)`: whether a jump is the function's own (its paths are counted by those).
`known`: {i: trail} of seeds already run without a shadow (by the judge, for their verdicts): of
seeds that took the same path only the first is run again with one."""
import heapq

import z3

from codebreaker.emulate import symbolic

# The solver's limits are counted in its own steps (z3's rlimit), not in time: the same function
# is then explored the same way on any machine, however busy -- a verdict must not depend on the
# load (with a time limit, one run of KEYBOARD's judge found an input another did not).
SOLVER_STEPS = 2000000          # one question's
QUESTIONS = 256                 # questions asked of one function at most


class Exploration(object):
    def __init__(self):
        self.kept, self.runs, self.queries, self.unsat, self.unknown = [], 0, 0, 0, 0
        self.paths, self.edges, self.lost, self.missed = 0, set(), set(), 0
        self.complete, self.pins, self.budget_spent = False, 0, False
        self.guessed = 0                # inputs a lane's guessing added after (lanes/win16.py)

    def summary(self):
        """One line for a verdict's notes."""
        what = "all %d paths" % self.paths if self.complete else "%d paths (sampled)" % self.paths
        return "%s, %d runs, %d questions (%d impossible)" % (what, self.runs, self.queries, self.unsat)


TURNED = ("jump", "inner")      # the conditions a question may turn: a jump's, a string instruction's


def _as_went(entry, memo):
    _site, taken, cond, _kind = entry
    t = symbolic.z3_of(cond, memo)
    return t if taken else z3.Not(t)


def _assert(s, term):
    """term asserted in solver s (the binding's own call: its checks cost more than the solving)."""
    z3.Z3_solver_assert(s.ctx.ref(), s.solver, term.as_ast())


def _edges(cond, memo):
    """For a comparison, the inputs at its bound: what was compared equal to what it was compared
    with, one above, one below (a zero test: the value 1) -- as z3 conditions to try in turn."""
    op = cond[0]
    if op == "bnot":
        return _edges(cond[2], memo)
    if op == "nz":
        x = symbolic.z3_of(cond[2], memo)
        return [x == 1]
    if op in ("eq", "ult", "ule", "slt", "sle"):
        a, b = symbolic.z3_of(cond[2], memo), symbolic.z3_of(cond[3], memo)
        return [a == b, a == b + 1, a == b - 1]
    return []


def explore(run, seeds, derive, own=lambda site: True, budget=24, keep=24, known=None,
            steps=SOLVER_STEPS, questions=QUESTIONS):
    got = Exploration()
    covered = set()                     # (site, taken) of the function's own jumps, any run
    other = set()                       # ... of the code it calls
    trails = set()                      # the paths: own jumps in order
    explored = set()                    # every prefix of decisions some run took
    pool = []                           # (input, observation)
    queue, order = [], [0]
    memo = {}                           # the solver's terms of the conditions, made once
    solvers = {}                        # run -> (its solver, how many of its conditions it holds)

    def took(v, o):
        """A run's path recorded; -> whether it is a new one."""
        trail = tuple(t for t in o["trail"] if own(t[0]))
        new = trail not in trails
        trails.add(trail)
        for t in o["trail"]:
            (covered if own(t[0]) else other).add(t)
        dec = [(e[0], e[1]) for e in o["path"] if e[3] in TURNED]
        for k in range(len(dec)):
            explored.add(tuple(dec[:k + 1]))
        got.lost |= set(o.get("lost") or ())
        got.pins += sum(1 for e in o["path"] if e[3] == "pin")
        pool.append((v, o))
        idx = len(pool) - 1
        k = 0
        for j, e in enumerate(o["path"]):
            if e[3] not in TURNED:
                continue
            flip = (e[0], not e[1])
            node = tuple(dec[:k]) + (flip,)
            k += 1
            if node in explored:
                continue
            # (a string instruction's count last: another number of passes of the same code)
            pri = 3 if e[3] == "inner" else 0 if own(e[0]) and flip not in covered else                 1 if flip not in covered | other else 2
            order[0] += 1
            heapq.heappush(queue, (pri, order[0], idx, j, node))
        return new

    def attempt(v):
        if got.runs >= budget:
            got.budget_spent = True
            return None
        got.runs += 1
        return run(v)

    # the seeds: of those already run, one per path they took; the others each run
    by_trail = {}
    for i, v in enumerate(seeds):
        t = (known or {}).get(i)
        if t is not None:
            t = tuple(x for x in t if own(x[0]))
            if t in by_trail:
                continue
            by_trail[t] = i
        o = attempt(v)
        if o is not None:
            took(v, o)
    while queue and len(got.kept) < keep:
        pri, _n, idx, j, node = heapq.heappop(queue)
        if node in explored:
            continue
        v, o = pool[idx]
        e = o["path"][j]
        if pri == 0 and (e[0], not e[1]) in covered:         # (reached meanwhile: now only another path)
            order[0] += 1
            heapq.heappush(queue, (2, order[0], idx, j, node))
            continue
        if got.runs >= budget or got.queries >= questions:
            got.budget_spent = True
            break
        # (one solver per run, its path's conditions asserted as far as asked: the questions of
        # one run come mostly in the order of its path, each one condition further on)
        s, k = solvers.get(idx) or (None, 0)
        if s is None or k > j:
            s, k = z3.Solver(), 0
            s.set("rlimit", steps)
        for p in o["path"][k:j]:
            _assert(s, _as_went(p, memo))
        solvers[idx] = (s, j)
        s.push()
        _assert(s, z3.Not(_as_went(e, memo)))
        got.queries += 1
        r = s.check()
        m = s.model() if r == z3.sat else None
        if m is not None:
            # (and on the edge: of the inputs that turn it, one where what was compared is what
            # it was compared with, or one past -- a bound written one off is seen there)
            for edge in _edges(e[2], memo):
                s.push()
                _assert(s, edge)
                if s.check() == z3.sat:
                    m = s.model()
                    s.pop()
                    break
                s.pop()
        s.pop()
        explored.add(node)                                   # (asked: whatever the answer)
        if r == z3.unsat:
            got.unsat += 1
            continue
        if r != z3.sat:
            got.unknown += 1
            continue
        values = {d.name(): m[d].as_long() for d in m.decls()}
        w = derive(v, values, o["vars"])
        if w is None:
            got.missed += 1
            continue
        o2 = attempt(w)
        if o2 is None:
            got.missed += 1
            continue
        dec2 = tuple((p[0], p[1]) for p in o2["path"] if p[3] in TURNED)
        if dec2[:len(node)] != node:                         # (the run did not go where asked)
            got.missed += 1
        if took(w, o2) and not o2.get("cut"):
            got.kept.append(w)
    got.paths = len(trails)
    got.edges = covered
    got.complete = (not queue or all(q[4] in explored for q in queue)) and not got.budget_spent \
        and not got.unknown and not got.missed and not got.lost and len(got.kept) < keep and bool(pool)
    return got
