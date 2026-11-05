#!/usr/bin/env python3
"""The C++ engine and store (native/engine) held to what the Python engine and store answered: the
same small cases, and the whole record compared -- every fact row (hash, tag, size, step, version,
inputs), what each run kept and made, every store reader's answer, the knowledge base's digests,
every word of the verdict vocabulary.
Run:  python tests/test_native_engine.py     (exit code 0 = all passed)

The Python engine and store are gone (the milestone of 2026-11-04). Before they went, these four
cases were run through them and their answers recorded in tests/data/native_engine_golden.txt (as
Python literals); the C++ must give the same answers still."""
import ast
import os
import shutil
import sys
import tempfile
import threading

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.engine import engine, facts, native_engine   # noqa: E402

RESULTS = []
MADE = []
LOCK = threading.Lock()


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "PASS", ""))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


# a pipeline of its own: n.words -> n.word <i> -> n.upper <i> -> n.joined; n.len <i> (early cutoff);
# n.batch (one call, an unkept value); n.side (a file); n.peek
def made(kind, key):
    with LOCK:
        MADE.append((kind, key))


@engine.step("n.words", reads=("binary",), version="1")
def _words(ctx, key):
    made("n.words", key)
    with open(ctx.need("binary")["path"]) as f:
        return f.read().split()


@engine.step("n.word", reads=("n.words",), version="1")
def _word(ctx, key):
    made("n.word", key)
    w = ctx.need("n.words")
    return w[int(key)] if int(key) < len(w) else None


@engine.step("n.upper", reads=("n.word",), version="1")
def _upper(ctx, key):
    made("n.upper", key)
    return (ctx.need("n.word", key) or "").upper()


@engine.step("n.len", reads=("n.word",), version="1")
def _len(ctx, key):
    made("n.len", key)
    return {"n": len(ctx.need("n.word", key) or "")}


@engine.step("n.joined", reads=("n.upper", "n.words", "n.len"), version="1")
def _joined(ctx, key):
    made("n.joined", key)
    n = len(ctx.need("n.words"))
    up = ctx.need_many("n.upper", [str(i) for i in range(n)])
    lens = ctx.need_many("n.len", [str(i) for i in range(n)])
    return [" ".join(up.values()), sum(v["n"] for v in lens.values()), ctx.peek("n.upper", "0")]


@engine.step("n.batch", reads=("n.word",), batch=True, version="1")
def _batch(ctxs, hint):
    made("n.batch", tuple(sorted(ctxs)))
    return {k: engine.Unkept("?") if c.need("n.word", k) == "skip" else b"%s" % c.need("n.word", k).encode()
            for k, c in ctxs.items()}


@engine.step("n.side", reads=("n.words",), version="1")
def _side(ctx, key):
    made("n.side", key)
    with open(os.path.join(os.path.dirname(ctx.need("binary")["path"]), "side.txt")) as f:
        return f.read() + str(len(ctx.need("n.words"))) + ctx.file(os.path.join(os.path.dirname(
            ctx.need("binary")["path"]), "side.txt"))


@engine.step("n.bad", reads=("n.word",), version="1")
def _bad(ctx, key):
    return ctx.need("n.words")                                   # (not declared)


def record(db, d):
    """every fact row as kept (the place's own folder written <d>: two places, one record)"""
    def p(x):
        return x.replace(d, "<d>") if isinstance(x, str) else x
    return [(r.kind, p(r.key), r.hash, r.tag, r.size, r.step, r.version, [tuple(p(x) for x in i) for i in r.inputs],
             r.given) for r in db.rows()]


GOLDEN = os.path.join(ROOT, "tests", "data", "native_engine_golden.txt")
_GOLDEN = []


def golden(case):
    """What the Python engine and store answered for `case` (facts, store, kb, verdicts)."""
    if not _GOLDEN:
        with open(GOLDEN, encoding="utf-8") as f:
            _GOLDEN.append(ast.literal_eval(f.read()))
    return _GOLDEN[0][case]


def same(value):
    """`value` as the recorded literals read back (tuples and lists, sets, dicts: as written)."""
    return ast.literal_eval(repr(value))


def unplaced(out):
    """The scenario's record with the one thing that depends on where it ran taken out: the size of
    the `binary` fact, whose value holds the binary's path (each row of it, and its kind's bytes)."""
    def row(r):
        return r[:4] + (None,) + r[5:] if isinstance(r, tuple) and r and r[0] == "binary" else r
    got = []
    for x in out:
        if x[0] == "kinds":
            x = ("kinds", [(k, n, None if k == "binary" else size) for k, n, size in x[1]])
        elif len(x) == 5 and isinstance(x[4], list):
            x = x[:4] + ([row(r) for r in x[4]],)
        got.append(x)
    return got


def place(text):
    d = tempfile.mkdtemp(prefix="cbnative")
    b = os.path.join(d, "thing.bin")
    with open(b, "w") as f:
        f.write(text)
    with open(os.path.join(d, "side.txt"), "w") as f:
        f.write("side")
    db = engine.open(b, folder=os.path.join(d, "db"))
    return d, b, db


def scenario():
    """The same asks, the same changes; -> [(what, value, kept/made, record)] after each."""
    d, b, db = place("alpha beta skip gamma")
    out = []
    try:
        assert getattr(db, "native", False), type(db)

        def step(what, fn):
            MADE.clear()
            v = fn()
            out.append((what, v, engine.last_run(db), sorted(MADE, key=repr), record(db, d)))

        step("cold", lambda: engine.get(db, "n.joined"))
        step("warm", lambda: engine.get(db, "n.joined"))
        step("many", lambda: engine.get_many(db, "n.upper", ["0", "1", "3", "1"]))
        step("batch", lambda: engine.Run(db).many("n.batch", ["0", "1", "2"]))
        step("batch again", lambda: engine.Run(db).many("n.batch", ["0", "1", "2"]))
        step("side", lambda: engine.get(db, "n.side"))
        with open(os.path.join(d, "side.txt"), "w") as f:
            f.write("SIDE")
        step("side changed", lambda: engine.get(db, "n.side"))
        step("given", lambda: engine.put(db, "n.word", "1", "BETA"))           # same upper case: cutoff
        step("given used", lambda: engine.get(db, "n.joined"))
        step("given other", lambda: engine.put(db, "n.word", "1", "delta"))
        step("given other used", lambda: engine.get(db, "n.joined"))
        step("forgot", lambda: engine.forget(db, "n.word", "1"))
        step("forgot used", lambda: engine.get(db, "n.joined"))
        try:
            engine.get(db, "n.bad", "0")
            raise AssertionError("an undeclared read was let through")
        except LookupError as e:
            out.append(("refused", str(e)))
        try:
            engine.get(db, "n.nothing")
            raise AssertionError("a kind no step makes was made")
        except LookupError as e:
            out.append(("no step", str(e)))
        out.append(("why", engine.why(db, "n.joined")["inputs"]))
        out.append(("kinds", [(k, n, size) for k, n, size, _s in db.kinds()]))
        with open(b, "w") as f:
            f.write("alpha beta")
        db.cleaned.clear()
        out.append(("about", db.about("name"), db.about("nothing")))
    finally:
        db.close()
        shutil.rmtree(d, ignore_errors=True)
    return out


def test_the_same_facts_and_runs():
    a, b = unplaced(golden("facts")), unplaced(same(scenario()))
    assert len(a) == len(b)
    for x, y in zip(a, b):
        assert x == y, "\n Python: %r\n C++:    %r" % (x[:4], y[:4])


def _store_answers():
    from codebreaker.store import functions as F
    d = tempfile.mkdtemp(prefix="cbnative")
    sha, other = "a" * 64, "b" * 64
    try:
        fs = F.open(os.path.join(d, "functions.sqlite"))
        assert type(fs).__module__.endswith("native_engine")
        fs.add_module(sha, "A.DLL", "C:/a.dll", "pe")
        fs.add_module(other, "b.exe", "C:/b.exe", "pe")
        fs.add_profile("p1", "{}")
        fs.add_functions(sha, ["1000", "1010", "1020", "1030"])
        rows = [dict(addr="1000", sig="s1", name="f", source="int f;", verdict="same", limits="truncated"),
                dict(addr="1010", sig="s2", name="g", source="int g;", verdict="divergent", reason="ret-differs"),
                dict(addr="1020", sig=None, name="h", source=None, verdict="no-synth", problem="no C"),
                dict(addr="1030", sig="s4", name="i", source="int i;", verdict="consistent", detail="d")]
        fs.record_many(sha, "decompiler", "1", "p1", rows)
        fs.record_many(sha, "other", "1", "p1", [dict(addr="1010", sig="t", name="g2", source="x", verdict="equivalent")])
        fs.record_many(sha, "decompiler", "2", "p2", [dict(addr="1000", sig="s1b", name="f", source="int f2;",
                                                           verdict="equivalent")])
        fs.set_identified(sha, {"1020": ("_memset", "libc"), "1030": ("_x", "libc")})
        fs.set_built(sha, "p1", {"1000": "o1", "1010": "o2"})
        fs.put_text(sha, "decompiler", "header", "typedef int int4;")
        v1 = fs.record_variant(sha, "1000", "msc6", "x", 4, 10, "different", behaves="same")
        fs.record_variant(sha, "1000", "msc6", "y", 0, 10, "identical", knobs="loop", parent=v1, behaves="equivalent")
        out = [fs.candidates(sha, "p1"), fs.candidates(sha, "p1", "other"), fs.judged(sha, "p1", "decompiler"),
               fs.best(sha, "p1"), fs.best(sha, "p1", prefer=("other", "decompiler")), fs.tally("p1", sha),
               fs.scores("p1", sha), fs.latest(sha), fs.addresses(sha), fs.identified(sha), fs.built(sha, "p1"),
               fs.get_text(sha, "decompiler", "header"), fs.get_text(sha, "x", "y"), fs.stored(sha, "1000"),
               fs.variants(sha), fs.best_variant(sha, "msc6"), fs.match_tally(sha, "msc6"), fs.modules(),
               fs.modules("p1"), fs.modules("p9"), fs.module_rows(), fs.module_rows("pe"), fs.module_path("a.dll"),
               fs.latest_profile("pe"), [r[:3] for r in fs.recent()], fs.verdicts_since(0),
               {k: (v["functions"], v["judged"], v["profiles"], v["latest"]) for k, v in fs.summaries().items()},
               fs._about("nothing")]
        fs.close()
        out.append([dict(m, path=m["path"]) for m in F.overview(os.path.join(d, "functions.sqlite"))])
        out.append(F.latest(sha, os.path.join(d, "functions.sqlite")))
        ro = F.open(os.path.join(d, "functions.sqlite"), readonly=True)
        out.append((ro.best(sha, "p1"), ro.addresses(other), ro.candidates(other, "p1")))
        try:
            ro.add_functions(sha, ["2000"])
            out.append("wrote")
        except Exception as e:
            out.append((type(e).__name__, str(e)))
        ro.close()
        return out
    finally:
        shutil.rmtree(d, ignore_errors=True)


def test_the_same_store_answers():
    a, b = golden("store"), same(_store_answers())
    for i, (x, y) in enumerate(zip(a, b)):
        assert x == y, "answer %d:\n Python: %r\n C++:    %r" % (i, x, y)
    assert len(a) == len(b)


def _kb_answers():
    from codebreaker.knowledge import store
    d = tempfile.mkdtemp(prefix="cbnative")
    try:
        kb = store.KnowledgeBase(os.path.join(d, "knowledge.sqlite"))
        kb.put_many("win32", "prototype", {"GetDlgItem": "HWND GetDlgItem(HWND,int)", "Ünïcode": [1, "two"],
                                           "it's": {"a": 1}})
        kb.put("win32", "argument_bytes.code", "KERNEL32.#41", 8, source="code")
        kb.ensure("win16", "ordinal", "stamp1", lambda: {"K.1": "One", "K.2": "Two"}, "file")
        kb.add_module("m1", "one.dll", "")
        kb.add_module("m2", "two.dll", "")
        kb.set_bodies("m1", {"10": ("h1", "Named", "export"), "20": ("h2", None, None)})
        kb.set_bodies("m2", {"30": ("h1", None, None), "40": ("h2", "Other", "export")})
        kb.name_body("h9", "given", "named by hand")
        out = [kb.get("win32", "prototype", "Ünïcode"), kb.get("win32", "prototype", "nope", 5),
               kb.has("win32", "prototype", "it's"), kb.table("win16", "ordinal"), dict(kb.hashes("win32", "prototype")),
               kb.hash_of("win32", "prototype", "GetDlgItem"), kb.kind_digest("win32", "prototype"),
               kb.kind_digest("win32", "nothing"), kb.stamp("win16", "ordinal"), kb.about("win32", "prototype", "it's")[:2],
               [k[:5] for k in kb.kinds()], kb.bodies("m1"), kb.name("h1"), kb.name("h2"), kb.name("h9"),
               kb.names_from_bodies("m2"), kb.shared_bodies()]
        out.append([kb.digest(r) for r in ("win32:prototype:GetDlgItem", "win32:prototype:*", "win32:nope:x",
                                            "bodies:name:h1", "bodies:names:m2", "bodies:name:none")])
        kb.forget("win32", "prototype", "it's")
        out.append(kb.hashes("win32", "prototype"))
        kb.close()
        return out
    finally:
        shutil.rmtree(d, ignore_errors=True)


def test_the_same_knowledge_answers():
    a, b = golden("kb"), same(_kb_answers())
    for i, (x, y) in enumerate(zip(a, b)):
        assert x == y, "answer %d:\n Python: %r\n C++:    %r" % (i, x, y)
    assert len(a) == len(b)


def test_the_same_verdict_words():
    from codebreaker.verify import verdicts as V
    words = list(V.VERDICTS) + list(V.OLD) + list(V.MATCHES) + list(V.MATCH_OLD) + ["", "other", None]
    limits = ["", "truncated", "low-coverage,stub-callee", "x,,fault-only", "nonsense", None]
    classes = list(V.TITLE) + list(V.TITLE.values()) + list(V.LABEL.values()) + list(V.CLASS_OLD) + [" weak ", "?"]
    def answers():
        return ([V.classify(w, lim) for w in words for lim in limits], [V.verdict(w) for w in words],
                [V.match(w) for w in words], [V.klass(c) for c in classes])
    assert same(answers()) == golden("verdicts")
    for name in ("VERDICTS", "GOOD", "ORDER", "SCORED", "BUILT", "MATCHES"):        # the C++ tables are verdicts.py's
        assert tuple(native_engine.verdicts_call("table", name)) == tuple(getattr(V, name)), name
    for name in ("OLD", "CLASS_OLD", "MATCH_OLD", "CLASS", "TITLE", "LABEL", "SAID", "MATCH_LABEL", "WEAK_LIMITS"):
        f = native_engine.verdicts_call("table", name)
        assert dict(zip(f[::2], f[1::2])) == getattr(V, name), name


def main():
    for fn in (test_the_same_facts_and_runs, test_the_same_store_answers, test_the_same_knowledge_answers,
               test_the_same_verdict_words):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail[:3000]) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d failed" % (sum(1 for r in RESULTS if r[1] == "PASS"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
