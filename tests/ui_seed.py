"""ui_seed.py -- a function store of its own for the terminal interface's tests (test_ui.py, test_ui_thin.py).

The tests run under real python with nothing of the package but codebreaker/ui, so the store is written the way the native
program's function store keeps it (native/cb/engine/function_store.cpp: an index file with the modules, profiles and
summaries, and the module's program database <sha>.sqlite in the `db` folder beside it), with sqlite3 and the schema
spelled out below. The service the tests start (CB_STATE, CB_FUNCTIONS_DB) reads it as it reads any store.

The modules are two small binaries the repository holds: lz32.dll (a PE: its first six exports are the functions) and
lzexpand.dll (a 16-bit NE, the module the interface is pointed at), both in native/cb/formats/testdata. No retail corpus is needed."""
import hashlib
import json
import os
import sqlite3
import struct
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA = os.path.join(ROOT, "native", "cb", "formats", "testdata")
DIGEST = "test-verifier-settings"     # what the rows are filed under: the interface shows the store's rows whatever their settings

INDEX = (
    "CREATE TABLE IF NOT EXISTS module(sha TEXT PRIMARY KEY, name TEXT, path TEXT, kind TEXT, ts REAL)",
    "CREATE TABLE IF NOT EXISTS profile(digest TEXT PRIMARY KEY, settings TEXT, ts REAL)",
    "CREATE TABLE IF NOT EXISTS summary(module TEXT PRIMARY KEY, functions INTEGER, identified INTEGER, judged INTEGER, last REAL,"
    " last_profile TEXT, profiles TEXT, latest TEXT)",
    "CREATE TABLE IF NOT EXISTS store_about(name TEXT PRIMARY KEY, value TEXT)")
MODULE = (
    "CREATE TABLE IF NOT EXISTS function(id INTEGER PRIMARY KEY, module TEXT NOT NULL, addr TEXT NOT NULL, name TEXT, size INTEGER,"
    " UNIQUE(module, addr))",
    "CREATE TABLE IF NOT EXISTS candidate(id INTEGER PRIMARY KEY, function INTEGER NOT NULL, engine TEXT NOT NULL, rev TEXT, sig TEXT,"
    " name TEXT, source TEXT, problem TEXT, ts REAL, UNIQUE(function, engine))",
    "CREATE TABLE IF NOT EXISTS verdict(candidate INTEGER NOT NULL, profile TEXT NOT NULL, sig TEXT, verdict TEXT, reason TEXT,"
    " limits TEXT, detail TEXT, ts REAL, carried INTEGER NOT NULL DEFAULT 0, PRIMARY KEY (candidate, profile))",
    "CREATE TABLE IF NOT EXISTS text(module TEXT NOT NULL, engine TEXT NOT NULL, kind TEXT NOT NULL, body TEXT, ts REAL,"
    " PRIMARY KEY (module, engine, kind))",
    "CREATE INDEX IF NOT EXISTS candidate_function ON candidate(function)",
    "CREATE TABLE IF NOT EXISTS identified(module TEXT NOT NULL, addr TEXT NOT NULL, name TEXT, library TEXT, ts REAL,"
    " PRIMARY KEY (module, addr))",
    "CREATE TABLE IF NOT EXISTS built(module TEXT NOT NULL, addr TEXT NOT NULL, profile TEXT NOT NULL, sig TEXT,"
    " PRIMARY KEY (module, addr, profile))",
    "CREATE TABLE IF NOT EXISTS variant(id INTEGER PRIMARY KEY, function INTEGER NOT NULL, compiler TEXT NOT NULL, source TEXT,"
    " parent INTEGER, knobs TEXT, distance INTEGER, total INTEGER, verdict TEXT, behaves TEXT, ts REAL,"
    " UNIQUE(function, compiler, knobs))",
    "CREATE INDEX IF NOT EXISTS variant_function ON variant(function)")

# What the functions were judged to be: (verdict, limits) as the judge records them, and the class the interface shows.
OUTCOME = (("equivalent", "coverage=90", "verified"), ("equivalent", "coverage=20,low-coverage", "weak"),
           ("divergent", "", "unverified"), ("inconclusive", "", "not-judged"), ("no-compile", "", "no-compile"))


def pe_module():
    return os.path.join(DATA, "lz32.dll")


def ne_module():
    return os.path.join(DATA, "lzexpand.dll")


def sha(path):
    with open(path, "rb") as f:
        return hashlib.sha256(f.read()).hexdigest()


def exports(path, count=6):
    """The flat addresses (hex, 8 digits) of a PE's first `count` exported functions that are not forwarders, sorted."""
    with open(path, "rb") as f:
        d = f.read()
    pe = struct.unpack_from("<I", d, 0x3C)[0]
    sections = struct.unpack_from("<H", d, pe + 6)[0]
    opt = pe + 24
    plus = struct.unpack_from("<H", d, opt)[0] == 0x20B
    base = struct.unpack_from("<Q" if plus else "<I", d, opt + (24 if plus else 28))[0]
    dir_rva, dir_size = struct.unpack_from("<II", d, opt + (112 if plus else 96))
    first = opt + struct.unpack_from("<H", d, pe + 20)[0]
    table = [struct.unpack_from("<IIII", d, first + 40 * i + 8) for i in range(sections)]   # (vsize, rva, rawsize, rawptr)

    def at(rva):
        for vsize, va, raw, ptr in table:
            if va <= rva < va + max(vsize, raw):
                return rva - va + ptr
        raise ValueError("rva %x is in no section" % rva)
    directory = at(dir_rva)
    n_functions = struct.unpack_from("<I", d, directory + 20)[0]
    address_table = struct.unpack_from("<I", d, directory + 28)[0]
    out = []
    for i in range(n_functions):
        rva = struct.unpack_from("<I", d, at(address_table) + 4 * i)[0]
        if rva and not dir_rva <= rva < dir_rva + dir_size:        # (a forwarder points back into the export directory)
            out.append("%08x" % (base + rva))
    return sorted(out[:count])


def _databases(store):
    """Where the module databases of a store with a path of its own are: `db` beside functions.sqlite, else <name>.db/."""
    if os.path.basename(store).lower() == "functions.sqlite":
        return os.path.join(os.path.dirname(os.path.abspath(store)), "db")
    return os.path.splitext(os.path.abspath(store))[0] + ".db"


def fill_store(store):
    """The store at `store` (CB_FUNCTIONS_DB) holding lz32.dll with six functions, five of them judged (one of each outcome).
    Returns (module path, sha, the first function's address)."""
    path = pe_module()
    digest = sha(path)
    addresses = exports(path)
    now = time.time()
    folder = _databases(store)
    os.makedirs(folder, exist_ok=True)
    module_db = sqlite3.connect(os.path.join(folder, digest + ".sqlite"))
    for stmt in MODULE:
        module_db.execute(stmt)
    for i, addr in enumerate(addresses):
        module_db.execute("INSERT OR IGNORE INTO function(module, addr, name, size) VALUES(?,?,NULL,NULL)", (digest, addr))
        if i >= len(OUTCOME):
            continue                                              # (a function with no candidate: not judged)
        verdict, limits, _cls = OUTCOME[i]
        fid = module_db.execute("SELECT id FROM function WHERE module=? AND addr=?", (digest, addr)).fetchone()[0]
        module_db.execute("INSERT OR REPLACE INTO candidate(function, engine, rev, sig, name, source, problem, ts) VALUES(?,?,?,?,?,?,NULL,?)",
                          (fid, "decompiler", "1", "s", "f%d" % i, "int f%d(void) { return %d; }" % (i, i), now))
        cid = module_db.execute("SELECT id FROM candidate WHERE function=? AND engine=?", (fid, "decompiler")).fetchone()[0]
        module_db.execute("INSERT OR REPLACE INTO verdict(candidate, profile, sig, verdict, reason, limits, detail, ts) VALUES(?,?,?,?,?,?,?,?)",
                          (cid, DIGEST, "s", verdict, "ret-differs" if verdict == "divergent" else "", limits, "", now))
    module_db.commit()
    module_db.close()
    index = sqlite3.connect(store)
    for stmt in INDEX:
        index.execute(stmt)
    index.execute("INSERT OR REPLACE INTO module(sha, name, path, kind, ts) VALUES(?,?,?,?,?)", (digest, "lz32.dll", path, "pe", now))
    index.execute("INSERT OR REPLACE INTO profile(digest, settings, ts) VALUES(?,?,?)", (DIGEST, "{}", now))
    latest = {}
    for _v, _l, cls in OUTCOME:
        latest[cls] = latest.get(cls, 0) + 1
    index.execute("INSERT OR REPLACE INTO summary(module, functions, identified, judged, last, last_profile, profiles, latest)"
                  " VALUES(?,?,?,?,?,?,?,?)", (digest, len(addresses), 0, len(OUTCOME), now, DIGEST, json.dumps([DIGEST]), json.dumps(latest)))
    index.commit()
    index.close()
    return path, digest, addresses[0]
