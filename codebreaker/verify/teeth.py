"""teeth.py -- does the judge notice when a reconstruction is wrong?

    python -m codebreaker teeth <module> [--rounds N] [--json FILE]

A verdict of "behaves as the original" is worth what the judge would have said of a wrong
reconstruction. This asks it: functions the judge has passed are each broken in one small way
that changes what they do -- a constant one higher, a comparison turned round, a plus made a
minus, a store left out, two arguments swapped -- the module is built with them, and they are
judged again. A broken function the judge still passes is a hole in the judge (or the break was
in code no input reaches, which is the same thing said of the inputs).

Works for every kind of module whose functions are rebuilt from C and run beside the original
(32-bit and 16-bit Windows, PowerPC). The broken functions of one round never reach one another,
so each is caught, or not, on its own account. Nothing is recorded in the function store.

    caught      the judge now says the function differs from the original
    survived    the judge still passes it

The share caught is given for the functions it had called VERIFIED and for those it had called
VERIFIED (weak) apart: the first is what the strong verdict is worth.
"""
import json
import os
import re

_NAME = r"[A-Za-z_]\w*"
_STRING = re.compile(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|/\*.*?\*/', re.S)


def _body(text):
    """(head with the opening brace, declarations, statements) -- breaks are made in statements."""
    head, brace, rest = text.partition("{")
    lines = rest.split("\n")
    i = 0
    while i < len(lines) and (not lines[i].strip() or re.match(r"^\s+[\w ]+[ *]\w+(\s*\[[^\]]*\])?;\s*$", lines[i])
                              or re.match(r"^\s+struct \{.*\} \w+;", lines[i])):
        i += 1
    return head + brace + "\n".join(lines[:i]) + ("\n" if i else ""), "\n".join(lines[i:])


def _constant(body, shadow):
    m = re.search(r"(?<![\w.\[])(\d+|0x[0-9a-fA-F]+)(?![\w.\]xX])", shadow)
    if not m:
        return None
    value = int(m.group(1), 0)
    return body[:m.start()] + ("0x%x" % (value + 1) if m.group(1).startswith("0x") else str(value + 1)) + body[m.end():]


def _comparison(body, shadow):
    m = re.search(r"(?<![=!<>])(==|!=)(?!=)", shadow)
    if m:
        return body[:m.start()] + ("!=" if m.group(1) == "==" else "==") + body[m.end():]
    m = re.search(r"(?<![<>=\-])\s(<|>)\s(?![<>=])", shadow)
    if m:
        return body[:m.start(1)] + (">=" if m.group(1) == "<" else "<=") + body[m.end(1):]
    return None


def _operator(body, shadow):
    m = re.search(r"(?<=[\w)\]])\s\+\s(?=[\w(*])", shadow)
    return body[:m.start()] + " - " + body[m.end():] if m else None


def _store(body, shadow):
    m = re.search(r"(?m)^[ \t]*\*[^;=\n]*=[^;=\n]*;[ \t]*\n", shadow)
    return body[:m.start()] + body[m.end():] if m else None


def _arguments(body, shadow):
    m = re.search(r"\b(%s)\((%s|\d+|0x[0-9a-fA-F]+),(%s|\d+|0x[0-9a-fA-F]+)([,)])" % (_NAME, _NAME, _NAME), shadow)
    if not m or m.group(2) == m.group(3) or m.group(1) in ("if", "while", "for", "switch", "return", "sizeof"):
        return None
    return body[:m.start(2)] + m.group(3) + "," + m.group(2) + body[m.end(3):]


BREAKS = (("constant", _constant), ("comparison", _comparison), ("operator", _operator), ("store", _store),
          ("arguments", _arguments))


def breaks(text):
    """[(kind of break, the function's text with it made)] -- every kind that applies."""
    head, body = _body(text)
    shadow = _STRING.sub(lambda m: " " * len(m.group(0)), body)
    out = []
    for kind, make in BREAKS:
        new = make(body, shadow)
        if new is not None and new != body:
            out.append((kind, head + new))
    return out


def _reach(texts):
    """{name: every function it can reach, itself included}"""
    names = set(texts)
    word = re.compile(_NAME)
    calls = {n: (set(word.findall(t)) & names) - {n} for n, t in texts.items()}
    out = {}
    for n in texts:
        seen, todo = {n}, [n]
        while todo:
            for c in calls[todo.pop()]:
                if c not in seen:
                    seen.add(c)
                    todo.append(c)
        out[n] = seen
    return out


def run(module_path, rounds=3, log=print):
    """-> {"module", "rounds", "results": [{name, break, was, now, reason}], "caught": {...}}"""
    from codebreaker.build import watcom
    from codebreaker.jobs import pool
    from codebreaker.store import functions as F
    from codebreaker.verify import reconstruct
    kind = reconstruct.kind_of(module_path)
    if not getattr(kind, "PER_FUNCTION", False):
        raise SystemExit("%s: this kind of module is rebuilt whole and compared byte for byte; there is "
                         "no judge of behaviour to test" % os.path.basename(module_path))
    fs = F.open()
    sha, digest = reconstruct.run(module_path, fs=fs, log=log)       # make sure it has been judged
    name = os.path.basename(module_path).lower()
    work = watcom._scratch("teeth", kind.KIND, name)
    m = kind.prepare(module_path, work)
    key_of = dict(m.key)
    passed = {}
    for addr, row in fs.best(sha, digest).items():
        if row["class"] in ("verified", "weak"):
            passed[addr] = row["class"]
    candidates = [n for n in m.order if key_of.get(n) in passed and breaks(m.texts[n])]
    reach = _reach(m.texts)
    original_texts = dict(m.texts)
    results, used = [], set()
    for r in range(rounds):
        chosen, taken = [], set()
        for n in candidates:
            if n in used or reach[n] & taken or any(n in reach[c] for c in chosen):
                continue
            chosen.append(n)
            taken |= reach[n]
        if not chosen:
            break
        used.update(chosen)
        made = {}
        m.texts = dict(original_texts)
        for i, n in enumerate(chosen):
            options = breaks(original_texts[n])
            made[n] = options[(i + r) % len(options)]            # the kinds of break in turn
            m.texts[n] = made[n][1]
        context, items, dropped = kind.build(m, module_path, os.path.join(work, "round%d" % r))
        judged = pool.run(kind.judge, [items[n] for n in chosen if n in items], context=context,
                          crashed=lambda item: reconstruct.NOT_RUN)
        for n in chosen:
            if n not in items:
                results.append(dict(name=n, was=passed[key_of[n]], now="does not compile", reason=dropped.get(n, ""),
                                    made=made[n][0]))
                continue
            verdict, reason, limits, _detail = judged[items[n][0]]
            results.append(dict(name=n, was=passed[key_of[n]], made=made[n][0], reason=reason,
                                now="survived" if verdict in reconstruct.GOOD else
                                "caught" if verdict == "divergent" else "not judged",
                                limits=",".join(limits)))
        if log:
            done = [x for x in results if x["now"] in ("caught", "survived")]
            log("  round %d: %d functions broken; so far %d of %d caught" % (
                r + 1, len(chosen), sum(1 for x in done if x["now"] == "caught"), len(done)))
    m.texts = original_texts
    tally = {}
    for x in results:
        if x["now"] in ("caught", "survived"):
            for group in (x["was"], "by break: " + x["made"]):
                c = tally.setdefault(group, [0, 0])
                c[0] += x["now"] == "caught"
                c[1] += 1
    return {"module": name, "kind": kind.KIND, "rounds": rounds, "results": results,
            "caught": {k: {"caught": v[0], "of": v[1]} for k, v in sorted(tally.items())}}


def main(argv):
    """python -m codebreaker teeth <module> [--rounds N] [--json FILE]

    Break functions the judge has passed, one small way each, and see whether it notices."""
    from codebreaker.base import corpus, settings
    args = [a for a in argv if not a.startswith("--")]
    rounds = int(argv[argv.index("--rounds") + 1]) if "--rounds" in argv else 3
    out = argv[argv.index("--json") + 1] if "--json" in argv else None
    args = [a for a in args if a not in (str(rounds), out)]
    if not args:
        print(main.__doc__)
        return 2
    path = args[0] if os.path.isfile(args[0]) else corpus.retail(args[0])
    if not path:
        raise SystemExit("no module %r" % args[0])
    from codebreaker import platforms
    p = platforms.identify(path)
    path = p.readable(path) if p else path               # (a .xex: its basefile, as the lanes read it)
    report = run(path, rounds)
    out = out or settings.state("teeth", report["module"] + ".json")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    with open(out, "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1)
    titles = {"verified": "it had called VERIFIED", "weak": "it had called VERIFIED (weak)"}
    for group, c in report["caught"].items():
        print("  %-34s caught %d of %d  (%d%%)" % (titles.get(group, group), c["caught"], c["of"],
                                                  100 * c["caught"] // max(c["of"], 1)))
    other = [x for x in report["results"] if x["now"] not in ("caught", "survived")]
    if other:
        print("  not counted: %d (the break did not compile, or could not be judged)" % len(other))
    for x in [x for x in report["results"] if x["now"] == "survived" and x["was"] == "verified"][:12]:
        print("    survived: %-24s %-10s %s" % (x["name"], x["made"], x.get("limits", "")))
    print("written to %s" % out)
    return 0
