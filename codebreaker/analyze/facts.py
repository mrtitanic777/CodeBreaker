"""facts.py -- the fact table: what the analyses found, kept beside the function store.

    from codebreaker.analyze import facts
    fs = facts.open()                                   # the function store's own sqlite file
    fs.put(sha, "conventions", rows)                    # replaces that module's facts of that analysis
    fs.get(sha, analysis="conventions", addr="bfee83ad")
    fs.get(sha, kind="structure")
    fs.tally(sha)                                       # {analysis: {kind: count}}

A fact is one thing an analysis established: about a function (`addr` is that function's key
in the function store -- the address in hex for a 32-bit module or a driver, "segment:offset"
for a 16-bit one) or about the module as a whole (`addr` empty). `kind` says what it is
("convention", "structure", "global", "class", "switch", ...) and `value` holds the rest as JSON.

The table lives in the same file as the function store (store/functions.py), so a function's
reconstruction, its verdicts and what is known about it are one query apart; it is created
here, on first use, and nothing in the store depends on it.
"""
import json
import sqlite3
import time

from codebreaker.decompile import describe
from codebreaker.store import functions

_SCHEMA = """
CREATE TABLE IF NOT EXISTS fact(
    module TEXT NOT NULL, analysis TEXT NOT NULL, addr TEXT NOT NULL DEFAULT '', kind TEXT NOT NULL,
    value TEXT, ts REAL);
CREATE INDEX IF NOT EXISTS fact_module ON fact(module, analysis);
CREATE INDEX IF NOT EXISTS fact_addr ON fact(module, addr);
CREATE INDEX IF NOT EXISTS fact_kind ON fact(module, kind);
"""


def key(img, va):
    """The function store's key for an address of an image ('' for None)."""
    if va is None:
        return ""
    if img.kind == "ne":
        return "%d:%04x" % describe.where(va)
    return "%x" % va


class FactStore(object):
    def __init__(self, path=None):
        self.path = path or functions.default_path()
        self.db = sqlite3.connect(self.path, timeout=60.0)
        self.db.execute("PRAGMA journal_mode=WAL")
        self.db.execute("PRAGMA synchronous=NORMAL")
        self.db.executescript(_SCHEMA)
        self.db.commit()

    def close(self):
        self.db.close()

    def put(self, module, analysis, rows):
        """rows: [(addr key, kind, value)] -- everything `analysis` has to say about `module`."""
        now = time.time()
        with self.db:
            self.db.execute("DELETE FROM fact WHERE module=? AND analysis=?", (module, analysis))
            self.db.executemany("INSERT INTO fact(module, analysis, addr, kind, value, ts) VALUES(?,?,?,?,?,?)",
                                [(module, analysis, addr, kind, json.dumps(value, sort_keys=True, default=_plain), now)
                                 for addr, kind, value in rows])

    def get(self, module, analysis=None, addr=None, kind=None):
        """[{analysis, addr, kind, value}] for a module, narrowed by whatever is given."""
        q, args = "SELECT analysis, addr, kind, value FROM fact WHERE module=?", [module]
        for col, v in (("analysis", analysis), ("addr", addr), ("kind", kind)):
            if v is not None:
                q += " AND %s=?" % col
                args.append(v)
        return [{"analysis": a, "addr": ad, "kind": k, "value": json.loads(v)} for a, ad, k, v in self.db.execute(q, args)]

    def tally(self, module):
        out = {}
        for a, k, n in self.db.execute("SELECT analysis, kind, COUNT(*) FROM fact WHERE module=? GROUP BY analysis, kind",
                                       (module,)):
            out.setdefault(a, {})[k] = n
        return out


def _plain(x):
    """JSON for what the analyses hold in sets and tuples."""
    if isinstance(x, (set, frozenset)):
        return sorted(x, key=repr)
    return str(x)


def open(path=None):                                    # noqa: A001 (as store.functions.open)
    return FactStore(path)
