"""compare.py -- judge a reconstruction: do two builds of one function behave alike?

    verdict, reason, limits = compare.compare(fp_original, fp_rebuilt, inputs, ...)

`fp_original(v)` and `fp_rebuilt(v)` run their side on input `v` and return a FINGERPRINT of the
run -- a dict saying how it ended, what it returned, which calls it made with which arguments,
what it left in memory, how far the stack moved, whether saved registers were preserved, and
which of the original's instructions ran. This module knows nothing about processors or file
formats: a kind of module (verify/win32.py, verify/win16.py) knows how to run its code and
produce the fingerprint; the comparison, the verdicts and the list of reasons a match is weaker
than it looks are the same for all of them.

    equivalent     every input compared equal, and at least one run returned normally
    consistent     compared equal, but no run returned (both sides only ever fail alike)
    divergent      some input showed a difference; `reason` says which kind
    inconclusive   the original could not be observed on any input
"""

def _exit_norm(e):
    if e is None:
        return "none"
    e = str(e)
    return "ret" if e == "ret" else ("fault" if e.startswith("exception") else e.split(":")[0])


# Bump when the oracle changes what a trace of the SAME code observes, so fingerprints cached under
# the old behaviour are not reused. 2: every PE code section is mapped (calls into another code
# section now run instead of being stub-returned). 3: the import stub removes the arguments a
# stdcall import takes, and the fingerprint is the full call sequence WITH arguments, not the set of
# import names reached. 4: the fingerprint also carries the memory the function wrote and which of
# its instructions ran; inputs can set what imported calls return.
REV = 16


def first_difference(a, b, ret_mask=0):
    """The first observable difference between two fingerprints of the same input, or ''.
    `ret_mask` marks bits of the return value that are leftovers, not results."""
    if a["exit"] != b["exit"]:
        return "exit-differs"
    if a["deps"] != b["deps"]:
        return "deps-differ"
    # EAX is a return value only when the function returned: at a fault it holds whatever the
    # compiler happened to keep there, which says nothing about the function.
    if a["exit"] == "ret" and ((a["ret"] or 0) ^ (b["ret"] or 0)) & ~ret_mask & 0xFFFFFFFF:
        return "ret-differs"
    # A run cut off at the step limit stopped in the middle of something: the two builds spend
    # different numbers of instructions per loop pass, so memory at that instant is not comparable.
    if a.get("truncated") or b.get("truncated"):
        return ""
    wa, wb = a.get("writes"), b.get("writes")
    if wa is not None and wb is not None and wa != wb:
        return "writes-differ"
    if a.get("pop") is not None and b.get("pop") is not None and a["pop"] != b["pop"]:
        return "stack-differs"                      # a different calling convention or argument count
    if a.get("saved") and b.get("saved") is False:
        return "registers-differ"                   # the original preserves them; this does not
    return ""


def difference_detail(a, b, v=None, ret_mask=0):
    """One line saying exactly what differed between two fingerprints, for triage by cause:
    which call first differs and how, or the two return values / exits."""
    d = first_difference(a, b, ret_mask)
    if d == "exit-differs":
        out = "exit %s vs %s" % (a["exit"], b["exit"])
    elif d == "ret-differs":
        out = "ret %x vs %x" % (a["ret"] or 0, b["ret"] or 0)
    elif d == "writes-differ":
        out = "writes %s vs %s" % ((a.get("writes") or "none").split(":")[0],
                                   (b.get("writes") or "none").split(":")[0])
    elif d == "stack-differs":
        out = "removes %s bytes on return vs %s" % (a.get("pop"), b.get("pop"))
    elif d == "registers-differ":
        out = "saved registers preserved vs changed"
    elif d == "deps-differ":
        da, db = list(a["deps"]), list(b["deps"])
        i = 0
        while i < len(da) and i < len(db) and da[i] == db[i]:
            i += 1
        ra = da[i] if i < len(da) else "(nothing)"
        rb = db[i] if i < len(db) else "(nothing)"
        na, nb = ra.split("(")[0], rb.split("(")[0]
        if rb == "(nothing)":
            kind = "rebuilt stops calling"
        elif ra == "(nothing)":
            kind = "rebuilt calls extra"
        elif na != nb:
            kind = "different call"
        else:
            aa, ab = ra[len(na):].strip("()").split(","), rb[len(nb):].strip("()").split(",")
            bad = [str(k) for k in range(min(len(aa), len(ab))) if aa[k] != ab[k]]
            kind = "same call, argument %s differs" % "+".join(bad)
        out = "call #%d %s: %s vs %s" % (i, kind, ra, rb)
    else:
        return ""
    if v is not None:
        out += "  [input %s%s]" % (",".join("%x" % x for x in v["args"]),
                                    " ret=%x" % v["stub_ret"] if v.get("stub_ret") else "")
    return out[:300]


def compare(fp_retail, fp_rebuilt, vectors, confirm=(), insns=0, directed=None, info=None, leftover=None):
    """Judge one body. `fp_retail(v)` / `fp_rebuilt(v)` fingerprint each side on input `v`.
    Returns (verdict, reason, limits). With `insns` (the original function's instruction count),
    the share of them that ran on some input is appended to limits as `coverage=NN`.

    Stops at the first input on which the two sides differ -- one difference settles it, and most
    bodies that fail do so on the first input. A body that matches every basic input with at least
    one clean return is then run on the `confirm` inputs too.

    `limits` are the reasons a match is weaker than it looks, as codes that can be counted:
      fault-only      no input returned cleanly; the two sides only fault/stop alike
      truncated       a trace hit the step or time cap
      argc-unknown    an imported call was compared by name only (its argument count is unknown)
      calls-capped    the call sequence was longer than is compared
      unobserved      some inputs could not be traced on one side
      writes-capped   a trace wrote more memory than is recorded, so its writes were not compared
      low-coverage    under half of the original's instructions ran on any input
    """
    limits = set()
    covered = set()
    reached = set()
    seen = clean = 0
    errs = set()

    def run(vs):
        nonlocal seen, clean
        for v in vs:
            a = fp_retail(v)
            b = fp_rebuilt(v)
            if "err" in a or "err" in b:
                limits.add("unobserved")
                if "err" in a:
                    errs.add("retail: " + a["err"])
                if "err" in b:
                    errs.add("rebuilt: " + b["err"])
                continue
            seen += 1
            d, mask = first_difference(a, b), 0
            if d == "ret-differs":
                mask = leftover(fp_retail, v, a) if leftover else 0
                if mask:
                    d = first_difference(a, b, mask)
                    if info is not None:
                        info["ret_bits"] = {0xFFFF0000: 16, 0xFFFFFF00: 8}.get(mask, 0)
            if d:
                if info is not None:
                    info["detail"] = difference_detail(a, b, v, mask)
                return d
            covered.update(a.get("cov") or ())
            reached.update(a.get("reach") or ())
            if a.get("writes") is None or b.get("writes") is None:
                limits.add("writes-capped")
            if a.get("truncated") or b.get("truncated"):
                limits.add("truncated")
            elif a.get("exit") == "ret":
                clean += 1
            for dep in a.get("deps", ()):
                if dep == "...":
                    limits.add("calls-capped")
                elif "(" not in dep:
                    limits.add("argc-unknown")
        return ""

    d = run(vectors)
    if d:
        return "divergent", d, []
    if not seen:
        return "inconclusive", "; ".join(sorted(errs)), sorted(limits)
    if clean and confirm:
        d = run(confirm)
        if d:
            return "divergent", d, []
    # `directed(covered, reached)` supplies inputs aimed at code no input has reached yet.
    if directed and clean:
        d = run(directed(covered, reached))
        if d:
            return "divergent", d, []
    if insns:
        pct = min(100, 100 * len(covered) // insns)
        if pct < 50:
            limits.add("low-coverage")
        limits.add("coverage=%d" % pct)
    if not clean:
        limits.add("fault-only")
        return "consistent", "", sorted(limits)
    return "equivalent", "", sorted(limits)
