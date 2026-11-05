"""store.py -- the knowledge base's database: one SQLite file, every platform's knowledge keyed by platform.

    from codebreaker.knowledge import store
    kb = store.open()                                 # state/db/knowledge.sqlite (CB_KNOWLEDGE_DB)
    kb.get("win32", "prototype", "GetDlgItem")        # a value, or None
    kb.put("win32", "argument_bytes.code", "USER32.FindWindowA", 8, source="user32.dll's code")
    kb.table("win16", "prototype")                    # {key: value} of a kind, read once per process
    kb.ensure("win32", "prototype", stamp, make, source)   # a whole kind made again when its stamp changes

A piece of knowledge is (platform, kind, key, value), kept the way the program database keeps a
fact (engine/facts.py: a value is bytes, text, JSON or a pickle, with the SHA-1 of it), and with
where it came from:

    source     what it was read or worked out from: a file of the repository, the headers, a
               module's code, a person
    origin     authored   loaded from a file people write (ordinal_names.json, driver_services.json,
                          ...): the file is the knowledge, the rows its copy, made again when the
                          file changes
               derived    worked out from inputs outside the repository (the compiler's headers):
                          made again when their stamp changes
               learned    found once by a step and kept (the bytes of arguments an export removes,
                          read from its code): written once, seen by every later ask
               given      given by hand (a body named); never dropped

A whole kind that is authored or derived is made in one go by ensure(): the stamp it was made
under (the file's hash; the header folders' sizes and times) is kept in `loaded`, and when it
differs the kind's authored/derived rows are made again -- learned and given rows stay.

Bodies (the name store) are kept here too: `body` -- a function's body digest in a module, with
the name the module itself gives it -- and `body_name` -- a name given to a body, which holds in
every module with that body (symbols/names.py says how a body is digested).

The file lives beside the program databases (state/db/) but is not one of them: the size rule
that deletes those (facts.hold) leaves it alone -- what was learned and given is not made again.

The knowledge base is C++ (native/engine: engine/native_engine.py's KnowledgeBase), values decoded
on this side. (The Python one it replaced is gone: the milestone of 2026-11-04.)
"""
import os
import threading

AUTHORED, DERIVED, LEARNED, GIVEN = "authored", "derived", "learned", "given"

_OPEN = {}                      # path -> KnowledgeBase, of this process
_LOCK = threading.RLock()


def default_path():
    from codebreaker.base import settings
    return os.environ.get("CB_KNOWLEDGE_DB") or settings.state("db", "knowledge.sqlite")


def beside(store_path):
    """The knowledge base of the state folder a function store is in (a test's own store in a
    folder of its own has a knowledge base of its own)."""
    return os.path.join(os.path.dirname(os.path.abspath(store_path)), "db", "knowledge.sqlite")


def KnowledgeBase(path):
    """The knowledge base in the file `path`: the C++ one's."""
    from codebreaker.engine import native_engine
    return native_engine.KnowledgeBase(path)


def open(path=None):                                    # noqa: A001 (as sqlite3.connect / shelve.open)
    """The knowledge base in `path` (default: default_path()), one per process."""
    path = os.path.abspath(path or default_path())
    with _LOCK:
        kb = _OPEN.get(path)
        if kb is None:
            kb = _OPEN[path] = KnowledgeBase(path)
        return kb
