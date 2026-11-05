"""functions.py -- the function store: one place that knows every function and what was made of it.

Everything CodeBreaker does to a function ends up here, and everything that reports reads from
here: the module writer, the scoreboard, the tools that explain a failure.

    module       one original binary (by content hash)
    function     one function of a module (by address)
    candidate    one decompiler's reconstruction of a function: its source, or why it has none
    verdict      what the verifier found for a candidate, under one set of verifier settings
    text         something a decompiler's functions share, such as its header of types
    identified   a function recognised as a runtime library's (its name there, and the library)

Where it is kept. A module's functions, candidates, verdicts, texts, identified functions, built
signatures and byte-match variants are tables of that binary's program database
(state/db/<sha>.sqlite, engine/facts.py: the same file the engine keeps the binary's facts in) --
the verdict table IS the database's. The store's own file (state/functions.sqlite, CB_FUNCTIONS_DB)
is the index of them: which modules there are (sha, name, path, kind), the verifier profiles, and
a summary per module (functions, verdicts, the latest class of each function) kept as each module
is written, so the questions asked across the corpus (overview, the ledger, progress) open one file.
A store given a path of its own keeps its databases in the folder `db` beside it (a store not named
functions.sqlite: in <its name>.db/), as the knowledge base beside it is found.

Carried over: a store from before the databases held every module's rows in its own file. The
first time such a store is opened they are copied into the modules' databases, each verdict marked
`carried` (1) -- and again for rows an older checkout wrote there since (by their time). The old
tables are then left as they were and not read.

Bodies and the names given to them -- the name store -- are the knowledge base's
(knowledge/store.py: body, body_name); the store's set_bodies, bodies, name_body,
names_from_bodies and shared_bodies are kept for their callers and go there. A store made
before carries its bodies over the first time it is opened.

A verdict is kept per verifier profile (a digest of every setting a verdict depends on), so
changing the verifier adds new verdicts beside the old ones instead of erasing them: nothing
has to be re-judged to see what a change did, and old results stay comparable.

    from codebreaker.store import functions
    fs = functions.open()                          # state/functions.sqlite and state/db/
    fs.add_module(sha, "comctl32.dll", path, "pe")
    fs.record(sha, "bfee83ad", "core", rev, sig, "sub_0183AD", source, profile,
              "equivalent", limits="coverage=100")
    fs.best(sha, profile)                          # {address: the best candidate's row}
    fs.tally(profile, sha)                         # {class: count}
    fs.module_rows()                               # [(sha, name, path, kind)] (the index)

The store is C++ (native/engine: engine/native_engine.py's FunctionStore), with the module tables
on the engine's own connection to each database. (The Python store it replaced is gone: the
milestone of 2026-11-04.)
"""
import os
import sqlite3

from codebreaker.verify import verdicts as V

# How good a result is, best first. A candidate's class is what the module writer prints. The
# words -- the judge's, the classes', their titles, the limits that make a match weak -- are the
# one vocabulary's (verify/verdicts.py); they are named here too, as the store's readers know them.
ORDER, TITLE, WEAK = V.ORDER, V.TITLE, V.WEAK_LIMITS
# A function recognised as a runtime library's is counted on its own, not as a failure of
# reconstruction -- unless it was also verified, which is the stronger statement and stays.
IDENTIFIED, SCORED = V.IDENTIFIED, V.SCORED
classify = V.classify


def overview(path=None):
    """The store at a glance, quickly (one file: the index's summaries, where the coverage ledger
    takes many minutes): per module {sha, name, kind, path, functions, identified, latest: {class:
    count}}, by each function's LATEST verdict under any judge -- what it last came to, not what the
    judge in force says (a change to the judge leaves every module unjudged until it is measured
    again)."""
    fs = open(path, readonly=True)
    if fs is None:
        return []
    try:
        return fs.overview()                              # (the C++ store works it out itself)
    finally:
        fs.close()


def latest(module_sha, path=None):
    """{addr: class} by each function's latest verdict under any judge (see overview)."""
    fs = open(path, readonly=True)
    if fs is None:
        return {}
    try:
        return fs.latest(module_sha)
    finally:
        fs.close()


def default_path():
    from codebreaker.base import settings
    return os.environ.get("CB_FUNCTIONS_DB") or settings.state("functions.sqlite")


def databases_folder(path=None, own=None):
    """Where a store keeps its modules' databases: the engine's (state/db) for the store of the
    state folder; for a store given a path of its own, the folder `db` beside it (or <name>.db/
    for a store not called functions.sqlite)."""
    from codebreaker.engine import facts
    if own is None:
        own = bool(path or os.environ.get("CB_FUNCTIONS_DB"))
    path = path or default_path()
    if not own:
        return facts.root()
    if os.path.basename(path).lower() == "functions.sqlite":
        return os.path.join(os.path.dirname(os.path.abspath(path)), "db")
    return os.path.splitext(os.path.abspath(path))[0] + ".db"


def _ro(path):
    return sqlite3.connect("file:%s?mode=ro" % path.replace("\\", "/").replace("?", "%3f").replace("#", "%23"),
                           uri=True, timeout=60.0, check_same_thread=False)


def FunctionStore(path=None, readonly=False):
    """The function store's tables (see the module's text): the C++ store's."""
    from codebreaker.engine import native_engine
    return native_engine.FunctionStore(path, readonly)


def open(path=None, readonly=False):                    # noqa: A001 (as sqlite3.connect / shelve.open)
    """The function store (see the module's text). Read-only: None when there is no store yet."""
    if readonly and not os.path.isfile(path or default_path()):
        return None
    return FunctionStore(path, readonly=readonly)
