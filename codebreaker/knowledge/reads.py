"""reads.py -- what a step read of the knowledge base, so that a changed piece makes stale exactly the facts made from it.

    from codebreaker.knowledge import reads
    with reads.recording() as r:              # (the engine does this around every step it runs)
        n = api_arguments.argc("USER32", "FindWindowA")
    r.keys                                    # {"win32:argument_bytes.header:FindWindowA"}
    reads.digest("win32:argument_bytes.header:FindWindowA")     # the piece's hash now ("absent")

A read is named "platform:kind:key" -- the piece's place in the base (knowledge/store.py) -- or
"platform:kind:*" for a whole kind read at once (the runtime-library signatures, a table). The
accessors that answer from the base record what they read (record()): knowledge/__init__'s, and
the symbols/ modules the base is made by -- prototypes.of, api_arguments' header_bytes,
stack_bytes, argc and arg_bytes, ordinals' names, sdk_structs.layout, runtime_library.match,
xdk_libraries.signatures and the name store's names of bodies. So whatever asks them (describe,
c_text's argument sizes, verify/lanes/win32's argc, the emulators' stand-ins) is recorded with no
change of its own.

The engine (engine/engine.py) runs each step inside recording() and adds every read to the
fact's inputs as ("kb", read, digest): a fact is current only while each piece it read has the
hash it had. A piece not there is "absent" -- adding it later is a change too. The name store's
pieces are "bodies:name:<body digest>" and "bodies:names:<module sha>": what name() and
names_from_bodies() answer.

Work handed to threads (jobs/pool.threads, as_done) is recorded into the asker's recording; work
handed to worker processes (jobs/pool.run) is run inside Carry, which brings each item's reads
back with its result. What a program of its own reads (the native decompiler, a DOSBox) is not
seen here: it reads files a step wrote, and those are the step's.
"""
import contextvars
import threading

_CURRENT = contextvars.ContextVar("codebreaker_knowledge_reads", default=None)
KIND = "kb"                     # the engine's name for an input that is a piece of the base
ABSENT = "absent"


class Reads(object):
    """The reads of one recording."""

    def __init__(self):
        self.keys = set()
        self.lock = threading.Lock()

    def add(self, key):
        with self.lock:
            self.keys.add(key)

    def update(self, keys):
        with self.lock:
            self.keys.update(keys)

    def __iter__(self):
        with self.lock:
            return iter(sorted(self.keys))

    def __len__(self):
        return len(self.keys)


class recording(object):
    """with recording() as r: ... -- every read of the base in the block (and in threads it
    hands work to) is in r.keys. A recording inside another keeps its own: the outer one does
    not see the inner's reads (a fact made for a step is that fact's, not the asker's)."""

    def __enter__(self):
        self.reads = Reads()
        self.token = _CURRENT.set(self.reads)
        return self.reads

    def __exit__(self, *exc):
        _CURRENT.reset(self.token)
        return False


def active():
    return _CURRENT.get() is not None


def join(platform, kind, key):
    return "%s:%s:%s" % (platform, kind, key)


def split(read):
    platform, kind, key = read.split(":", 2)
    return platform, kind, key


def record(platform, kind, key):
    """A piece was read (nothing happens outside a recording)."""
    r = _CURRENT.get()
    if r is not None:
        r.add(join(platform, kind, key))


def record_kind(platform, kind):
    record(platform, kind, "*")


def merge(keys):
    """Reads made elsewhere (a worker process) for the current recording."""
    r = _CURRENT.get()
    if r is not None and keys:
        r.update(keys)


# ---- worker processes --------------------------------------------------------------------------
class Carried(object):
    """A worker's result with the reads it made."""

    def __init__(self, value, keys):
        self.value, self.keys = value, keys


class Carry(object):
    """work(context, item) for jobs/pool.run, its reads brought back with each result (a module-
    level function wrapped: it pickles by reference)."""

    def __init__(self, work):
        self.work = work

    def __call__(self, context, item):
        with recording() as r:
            value = self.work(context, item)
        return Carried(value, sorted(r.keys))


def unwrap(value, into=None):
    """A Carry's result: its value, its reads merged into the current recording (or `into`, a
    callable given them)."""
    if isinstance(value, Carried):
        (into or merge)(value.keys)
        return value.value
    return value


# ---- what a read is now --------------------------------------------------------------------------
def _fresh(platform, kind):
    """Bring an authored or derived kind up to date with what it is made from (its file changed,
    the headers changed) before its hashes are read: a staleness check in a new process sees the
    piece as it would now be made."""
    from codebreaker.knowledge import sources
    if (platform, kind) in sources.AUTHORED:
        sources.authored(platform, kind)
        return
    try:
        if kind == "prototype" and platform in ("win16", "win32"):
            from codebreaker.symbols import prototypes
            prototypes.table()
        elif kind in ("struct", "struct.pointer") and platform == "win32":
            from codebreaker.symbols import sdk_structs
            sdk_structs._read()
        elif kind == "library_signatures" and platform == "xbox360":
            import os
            from codebreaker.symbols import xdk_libraries
            folder = xdk_libraries.lib_folder()
            if folder and os.path.isdir(folder):
                xdk_libraries.signatures(folder)
    except Exception:
        pass


_FRESH = set()                  # (platform, kind) brought up to date by this process


def digest(read):
    """The hash of a read piece now: its value's hash, ABSENT when it is not there; for a whole
    kind ("*"), a hash of every piece's; for the name store, of what it answers."""
    from codebreaker.knowledge import store
    platform, kind, key = split(read)
    kb = store.open()
    if platform != "bodies" and (platform, kind) not in _FRESH:     # (the C++ base answers, once the kind is fresh)
        _fresh(platform, kind)
        _FRESH.add((platform, kind))
    return kb.digest(read)
