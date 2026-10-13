r"""ledger.py -- the coverage ledger: how much of every module of the corpus is accounted for.

    python -m codebreaker coverage                  by kind of module, then every module the store holds
    python -m codebreaker coverage --all            list the modules nothing has been done to as well
    python -m codebreaker coverage --module NAME    one module: its counts, its bytes, why functions fail
    python -m codebreaker coverage --csv FILE       one row per corpus module, for a spreadsheet
    python -m codebreaker coverage --json           the whole ledger as JSON

    from codebreaker.coverage import ledger
    led = ledger.measure()            # {'profiles', 'kinds': [row], 'total': row, 'modules': [row]}
    fs = ledger.open_store()          # the function store, opened read-only

Two sources and nothing else: the corpus (base/corpus.py) says which modules exist and how much
code each holds; the function store (store/functions.py) says what has been made of each
function. A row -- for a module, a kind of module, or everything -- counts:

    functions     functions the store knows the module has
    decompiled    of those, the ones some reconstruction has source for
    compiled      of those, the ones whose source was built and run beside the original
    verified, weak, unverified, not-judged, no-compile, no-source
                  the store's own classes (functions.classify / ORDER / TITLE) under the verifier
                  settings in force now; the six add up to `functions`
    code_bytes    bytes of code in the file (the inspector's figure)
    bytes_known   bytes that lie in a known function: from its start to the next known start, or
                  to the end of its section or segment
    bytes_verified, bytes_weak     the same, for functions of that class

Kinds are 32-bit (PE), 16-bit (NE), driver (LE and the W3 bundle) and DOS. Only the first two
can be reconstructed and judged today, so the other two show what is waiting. The store is
opened read-only: measuring coverage never changes a result and can run beside a sweep.
"""
import collections
import csv
import json
import os
import sqlite3
import time

from codebreaker.base import corpus, settings
from codebreaker.formats import inspect, ne, pe
from codebreaker.store import functions as F
from codebreaker.verify import reconstruct

KINDS = (("pe", "32-bit"), ("ne", "16-bit"), ("le", "driver"), ("dos", "DOS"))
TITLE = dict(KINDS)
_SWEPT = (".dll", ".exe", ".drv", ".ocx", ".cpl")                # what `sweep` goes through
_EXTS = {"pe": _SWEPT, "ne": _SWEPT, "le": (".vxd", ".386", ".pdr", ".sys", ".drv", ".exe"),
         "dos": (".exe", ".com", ".sys", ".vxd", ".386")}
_BUILT = ("verified", "weak", "unverified", "not-judged")        # classes that mean "compiled and run"
COUNTS = ("functions", "decompiled", "compiled") + F.ORDER + (
    "code_bytes", "bytes_known", "bytes_verified", "bytes_weak")
_rows = {}                                                       # (sha, digest) -> (stamp, counts)


def open_store(path=None):
    """The function store, read-only: the store's own reading methods (best, tally, candidates,
    addresses, modules, get_text) on a connection that cannot write. None when no store exists."""
    path = path or F.default_path()
    if not os.path.isfile(path):
        return None
    fs = F.FunctionStore.__new__(F.FunctionStore)                # its __init__ creates tables: not here
    fs.path = path
    fs.db = sqlite3.connect("file:%s?mode=ro" % path.replace("\\", "/").replace("?", "%3f").replace("#", "%23"),
                            uri=True, timeout=60.0)
    return fs


def profiles(fs=None):
    """{kind: digest} -- the verifier settings in force now, per kind of module. When they cannot
    be worked out on this machine, the settings most recently used in the store stand in."""
    out = {}
    for kind, digest in reconstruct.digests().items():
        out[kind] = digest
        if digest is None:
            row = fs and fs.db.execute(
                "SELECT v.profile FROM verdict v JOIN candidate c ON c.id=v.candidate JOIN function f "
                "ON f.id=c.function JOIN module m ON m.sha=f.module WHERE m.kind=? ORDER BY v.ts DESC LIMIT 1",
                (kind,)).fetchone()
            out[kind] = row[0] if row else None
    return out


def corpus_modules(refresh=False):
    """[{name, path, kind, size, code_bytes}] for every module of the corpus, one per name. The
    code size comes from the inspector and is remembered in state/coverage_corpus.json until the
    file changes."""
    memo_path = settings.state("coverage_corpus.json")
    memo = {}
    if not refresh:
        try:
            with open(memo_path, encoding="utf-8") as f:
                memo = json.load(f)
        except (OSError, ValueError):
            pass
    out, changed = [], False
    for kind, _title in KINDS:
        for path in corpus.modules(kind, exts=_EXTS[kind]):
            st = os.stat(path)
            got = memo.get(path)
            if not got or got[:2] != [st.st_size, int(st.st_mtime)]:
                info = inspect.describe(path)
                got = [st.st_size, int(st.st_mtime), info["summary"].get("code_bytes", 0),
                       "le" if info["format"] == "W3" else kind]
                memo[path] = got
                changed = True
            out.append({"name": os.path.basename(path).lower(), "path": path, "kind": got[3],
                        "size": st.st_size, "code_bytes": got[2]})
    if changed:
        with open(memo_path + ".tmp", "w", encoding="utf-8") as f:
            json.dump(memo, f)
        os.replace(memo_path + ".tmp", memo_path)
    out.sort(key=lambda m: m["name"])
    return out


def extents(path, kind, addrs):
    """{address: bytes} -- how far each known function reaches: to the next known function of
    its section or segment, or to that section's end. Addresses are the store's (hexadecimal
    for a 32-bit module, 'segment:offset' for a 16-bit one)."""
    spans = []                                                    # (key, start, end)
    where = {}
    try:
        if kind == "pe":
            spans = [(n, va, va + len(raw)) for n, (va, raw, _name) in enumerate(pe.open_pe(path).code_sections())]
            for a in addrs:
                va = int(a, 16)
                where[a] = next(((k, va) for k, lo, hi in spans if lo <= va < hi), None)
        elif kind == "ne":
            spans = [(s["idx"], 0, s["length"]) for s in ne.NEFile(path).segments() if s["kind"] == "CODE"]
            for a in addrs:
                seg, off = a.split(":")
                where[a] = (int(seg), int(off, 16))
    except (OSError, ValueError):
        return {}
    ends = {k: hi for k, _lo, hi in spans}
    starts = collections.defaultdict(list)
    for a, at in where.items():
        if at and at[0] in ends and at[1] < ends[at[0]]:
            starts[at[0]].append((at[1], a))
    out = {}
    for k, found in starts.items():
        found.sort()
        for (start, a), (nxt, _b) in zip(found, found[1:] + [(ends[k], None)]):
            out[a] = nxt - start
    return out


def _stamps(fs):
    """{sha: what changes when a module's results change} -- lets an unchanged module be served
    from memory."""
    return {sha: (n, judged, last) for sha, n, judged, last in fs.db.execute(
        "SELECT f.module, COUNT(DISTINCT f.id), COUNT(v.ts), MAX(v.ts) FROM function f "
        "LEFT JOIN candidate c ON c.function=f.id LEFT JOIN verdict v ON v.candidate=c.id GROUP BY f.module")}


def _classes(fs, sha, digest):
    """({address: class}, how many have a result under these settings) for every known function
    of a module, by the store's rule: the best reconstruction's class, and no-source for a
    function nothing has been judged for."""
    best = fs.best(sha, digest) if digest else {}
    addrs = fs.addresses(sha)
    return {a: best[a]["class"] if a in best else "no-source" for a in addrs}, sum(1 for a in addrs if a in best)


def _counts(fs, sha, digest, path, kind, stamp=None):
    key = (sha, digest)
    if stamp is not None and key in _rows and _rows[key][0] == stamp:
        return dict(_rows[key][1])
    cls, judged = _classes(fs, sha, digest)
    c = dict.fromkeys(COUNTS, 0)
    c["functions"] = len(cls)
    for k in cls.values():
        c[k] += 1
    c["compiled"] = sum(c[k] for k in _BUILT)
    c["decompiled"] = len({r["addr"] for r in fs.candidates(sha, digest or "") if r["source"]})
    size = extents(path, kind, list(cls)) if path and os.path.isfile(path) else {}
    c["bytes_known"] = sum(size.values())
    c["bytes_verified"] = sum(n for a, n in size.items() if cls[a] == "verified")
    c["bytes_weak"] = sum(n for a, n in size.items() if cls[a] == "weak")
    c["judged"] = judged
    _rows[key] = (stamp, dict(c))
    return c


def measure(fs=None, module=None, refresh=False):
    """The ledger: {'profiles': {kind: digest}, 'kinds': [row per kind], 'total': row, 'modules':
    [row per module]}. A module row has name, kind, path, size, in_store, measured (every known
    function has a result under the current settings) and the counts described at the top; a
    kind row adds modules, in_store and measured as counts. `module` limits it to one name."""
    own = fs is None
    fs = fs or open_store()
    try:
        digests = profiles(fs)
        stored = {}                                              # name -> [(sha, path, kind)]
        stamps = _stamps(fs) if fs else {}
        for sha, name, path, kind in (fs.db.execute("SELECT sha, name, path, kind FROM module") if fs else ()):
            stored.setdefault(name, []).append((sha, path or "", kind or ""))
        mods = corpus_modules(refresh)
        have = {m["name"] for m in mods}
        for name, versions in stored.items():                   # in the store but not a corpus module
            if name not in have:
                sha, path, kind = versions[0]
                size = os.path.getsize(path) if os.path.isfile(path) else 0
                mods.append({"name": name, "path": path, "kind": kind if kind in TITLE else "pe", "size": size,
                             "code_bytes": inspect.describe(path)["summary"].get("code_bytes", 0) if size else 0})
        if module:
            want = os.path.basename(module).lower()
            mods = [m for m in mods if m["name"] == want or os.path.splitext(m["name"])[0] == want]
        rows = []
        for m in sorted(mods, key=lambda m: m["name"]):
            row = dict(m, in_store=False, measured=False, judged=0, **dict.fromkeys(COUNTS[:-4] + COUNTS[-3:], 0))
            versions = stored.get(m["name"], [])
            if len(versions) > 1:                               # several files have had this name
                mine = corpus.sha(m["path"])
                versions = [v for v in versions if v[0] == mine] or versions[:1]
            if versions:
                sha = versions[0][0]
                row.update(_counts(fs, sha, digests.get(m["kind"]), m["path"], m["kind"], stamps.get(sha)))
                row.update(code_bytes=m["code_bytes"], in_store=True, sha=sha,
                           measured=bool(row["functions"]) and row["judged"] == row["functions"])
            rows.append(row)
    finally:
        if own and fs:
            fs.close()
    kinds = []
    for kind, title in KINDS:
        of = [r for r in rows if r["kind"] == kind]
        kinds.append(dict(_total(of), kind=kind, name=title))
    return {"profiles": digests, "kinds": kinds, "total": dict(_total(rows), kind="", name="all"),
            "modules": rows, "generated": time.strftime("%Y-%m-%d %H:%M:%S"), "corpus": corpus.root()}


def _total(rows):
    out = {k: sum(r[k] for r in rows) for k in COUNTS + ("judged",)}
    out.update(modules=len(rows), in_store=sum(1 for r in rows if r["in_store"]),
               measured=sum(1 for r in rows if r["measured"]))
    return out


def reasons(fs, sha, digest, most=8):
    """{class: [(reason, functions)]} -- why the functions of a module that are not verified are
    not, commonest first."""
    why = collections.defaultdict(collections.Counter)
    for r in fs.best(sha, digest).values():
        if r["class"] == "weak":
            for code in r["weak"]:
                why["weak"][F.WEAK[code]] += 1
        elif r["class"] != "verified":
            why[r["class"]][(r["reason"] or "(no reason recorded)")[:70]] += 1
    return {k: c.most_common(most) for k, c in why.items()}


# ---- as text ---------------------------------------------------------------------------------
def _pct(a, b):
    return "%5.1f%%" % (100.0 * a / b) if b else "     -"


def _table(head, rows):
    widths = [max(len(str(r[i])) for r in [head] + rows) for i in range(len(head))]
    return ["  ".join(("%-*s" if i == 0 else "%*s") % (w, c) for i, (w, c) in enumerate(zip(widths, r))).rstrip()
            for r in [head] + rows]


def _count_cells(r):
    return [r["functions"], r["decompiled"], r["compiled"]] + [r[k] for k in F.ORDER] + [_pct(r["verified"], r["functions"])]


def render(led, every=False):
    """The ledger as the lines `coverage` prints."""
    p = led["profiles"]
    out = ["Coverage of %s" % led["corpus"],
           "verifier settings in force: " + ", ".join("%s %s" % (TITLE[k], p.get(k) or "(none)") for k in ("pe", "ne")),
           "", "Functions, by kind of module"]
    names = ["functions", "decompiled", "compiled"] + [F.TITLE[k] for k in F.ORDER] + ["verified %"]
    out += _table(["kind", "modules", "in store", "measured"] + names,
                  [[r["name"], r["modules"], r["in_store"], r["measured"]] + _count_cells(r)
                   for r in led["kinds"] + [led["total"]]])
    out += ["", "Bytes of code, by kind of module"]
    out += _table(["kind", "code bytes", "in known functions", "", "in VERIFIED", "", "in VERIFIED (weak)", ""],
                  [[r["name"], r["code_bytes"], r["bytes_known"], _pct(r["bytes_known"], r["code_bytes"]),
                    r["bytes_verified"], _pct(r["bytes_verified"], r["code_bytes"]),
                    r["bytes_weak"], _pct(r["bytes_weak"], r["code_bytes"])] for r in led["kinds"] + [led["total"]]])
    shown = [r for r in led["modules"] if every or r["in_store"]]
    out += ["", "Modules%s" % ("" if every else " the store holds (--all lists the rest)")]
    if shown:
        out += _table(["module", "kind", "state"] + names + ["code bytes", "known", "verified"],
                      [[r["name"], TITLE[r["kind"]], _state(r)] + _count_cells(r) +
                       [r["code_bytes"], _pct(r["bytes_known"], r["code_bytes"]),
                        _pct(r["bytes_verified"], r["code_bytes"])] for r in shown])
    else:
        out.append("(none)")
    return out


def _state(r):
    return "measured" if r["measured"] else "part measured" if r["judged"] else \
        "not measured now" if r["in_store"] else "untouched"


def write_csv(led, path):
    cols = ["name", "kind", "state", "path", "size"] + list(COUNTS) + ["judged"]
    with open(path, "w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(cols)
        for r in led["modules"]:
            w.writerow([TITLE[r["kind"]] if c == "kind" else _state(r) if c == "state" else r[c] for c in cols])
    return len(led["modules"])


def main(argv):
    """python -m codebreaker coverage [--module NAME] [--all] [--csv FILE] [--json] [--refresh]

    How much of the corpus is accounted for, from the function store and the corpus. --refresh
    reads every corpus file's code size again instead of trusting the remembered ones."""
    def option(name):
        return argv[argv.index(name) + 1] if name in argv and argv.index(name) + 1 < len(argv) else None

    if "-h" in argv or "--help" in argv:
        print(__doc__)
        return 0
    fs = open_store()
    if fs is None:
        print("there is no function store yet (%s): nothing has been reconstructed; the corpus is listed alone"
              % F.default_path())
    led = measure(fs, module=option("--module"), refresh="--refresh" in argv)
    if option("--module") and not led["modules"]:
        print("no module %r in the corpus or the store" % option("--module"))
        return 2
    if option("--csv"):
        print("wrote %d modules to %s" % (write_csv(led, option("--csv")), option("--csv")))
    if "--json" in argv:
        print(json.dumps(led, indent=1))
    elif option("--module"):
        for r in led["modules"]:
            print("%s  (%s, %s)  %s" % (r["name"], TITLE[r["kind"]], _state(r), r["path"]))
            print("\n".join(_table(["", "functions", "share"], [[k if k in COUNTS[:3] else F.TITLE[k], r[k], _pct(
                r[k], r["functions"])] for k in COUNTS[:3] + F.ORDER])))
            print("code bytes %d; in known functions %d (%s); in VERIFIED %d (%s); in VERIFIED (weak) %d (%s)" % (
                r["code_bytes"], r["bytes_known"], _pct(r["bytes_known"], r["code_bytes"]).strip(),
                r["bytes_verified"], _pct(r["bytes_verified"], r["code_bytes"]).strip(),
                r["bytes_weak"], _pct(r["bytes_weak"], r["code_bytes"]).strip()))
            if r["in_store"] and led["profiles"].get(r["kind"]):
                for cls, found in sorted(reasons(fs, r["sha"], led["profiles"][r["kind"]]).items(),
                                         key=lambda kv: F.ORDER.index(kv[0])):
                    print("%s, by cause:" % F.TITLE[cls])
                    for why, n in found:
                        print("  %5d  %s" % (n, why))
    elif not option("--csv"):
        print("\n".join(render(led, every="--all" in argv)))
    if fs:
        fs.close()
    return 0
