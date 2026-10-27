r"""mcp_server.py -- CodeBreaker as an MCP server: what the console shows, for an assistant to use.

    python -m codebreaker mcp                     serve over stdin/stdout (what an MCP client starts)
    claude mcp add codebreaker -- python -m codebreaker mcp      (or the project's .mcp.json)

The same reading and the same jobs as the web console (server.py), as tools an assistant such
as Claude Code calls. Three kinds:

    reading     instant, nothing changes: describe, inspect, strings, hex_dump, resource,
                modules, coverage, progress, functions, function, names, identify, demangle,
                commands, command_help
    jobs        any command of `python -m codebreaker` run as a process of its own, below normal
                priority, its output kept in state/ui_jobs/ (as the console's jobs): run_command,
                jobs, job, job_file, stop_job. At most two at a time, at most one `measure`.
    writing     name_function: give a function's body a name in the function store; every module
                holding the same body then has it (symbols/names.py set_name)

Every answer is capped (MOST characters) and pages with offset/limit where it can be long. The
ledger behind coverage, modules and progress reads the whole corpus (minutes when cold), so it is
worked out in the background and remembered; a call that finds it not ready yet says so instead
of holding the client past its timeout. Stdout is the protocol: the MCP library points file 1 at
stderr while serving, so anything the commands print cannot corrupt it.
"""
import functools
import io
import json
import os
import threading
import time
from contextlib import closing, redirect_stdout

from codebreaker.base import corpus
from codebreaker.formats import inspect
from codebreaker.store import functions as F
from codebreaker.ui import server

MOST = 60000                     # characters in one answer, at most
INTERACTIVE = {"serve", "app", "workbench", "live", "progress", "mcp"}     # windows and servers: not jobs

INSTRUCTIONS = """CodeBreaker reconstructs readable source from binaries (Windows 16/32-bit, DOS, VxD
drivers, Xbox, Xbox 360, 8/16-bit consoles, XNA, Java) and checks that the source behaves like the
original by running both in an emulator. Start with `describe` on a file or `modules` for the corpus;
`inspect` for headers/imports/exports; `functions` and `function` for what the store holds (each
function's reconstructed C and its verdict: verified, weak, unverified, not-judged, no-compile,
no-source). Anything long (decompile, measure, reach --judge, pe-source, recompile-project) goes
through `run_command`, then `job` to follow it. A module is a corpus name (lz32.dll) or a path."""


# ---- plumbing --------------------------------------------------------------------------------
class Slow(object):
    """Values that take long to work out, worked out in a thread and remembered for a while."""

    def __init__(self):
        self.lock = threading.Lock()
        self.got = {}                                            # key -> (when, value)
        self.working = {}                                        # key -> (thread, started)

    def get(self, key, work, age=600.0, wait=20.0, again=False):
        """`work()`'s value if known and younger than `age` seconds, else start it and wait up to
        `wait` seconds. -> (value or None, seconds it has been working when not ready)"""
        with self.lock:
            got = self.got.get(key)
            if got and not again and time.time() - got[0] < age:
                return got[1], 0
            if key not in self.working:
                def run():
                    try:
                        value = work()
                    except Exception as e:                       # remembered, said once
                        value = {"error": "%s: %s" % (type(e).__name__, str(e)[:300])}
                    with self.lock:
                        self.got[key] = (time.time(), value)
                        self.working.pop(key, None)
                t = threading.Thread(target=run, daemon=True)
                self.working[key] = (t, time.time())
                t.start()
            t, started = self.working[key]
        t.join(wait)
        with self.lock:
            got = self.got.get(key)
            if got and not t.is_alive():
                return got[1], 0
            return None, int(time.time() - started)


SLOW = Slow()


def _not_ready(what, seconds):
    return {"status": "computing", "what": what, "seconds_so_far": seconds,
            "say": "still being worked out in the background; call again in a little while"}


def _quiet(work, *args, **kw):
    """`work(...)` with anything it prints kept out of the answer (and off the protocol)."""
    with redirect_stdout(io.StringIO()):
        return work(*args, **kw)


def _path(module):
    """The file (or Java folder) a module name or path stands for; raises ValueError."""
    module = (module or "").strip().strip('"')
    if not module:
        raise ValueError("name a module (a corpus name such as lz32.dll, or a path)")
    path = module if os.path.isdir(module) else corpus.retail(module) or server._stored_path(module)
    if not path:
        raise ValueError("no module %r: not a file, not in the corpus (%s), not in the store"
                         % (module, corpus.root()))
    return path


def _fit(value, most=MOST):
    """`value`, or when its JSON is longer than `most` characters, a note saying it was cut and
    what it holds."""
    text = json.dumps(value, default=str)
    if len(text) <= most:
        return value
    if isinstance(value, dict):
        sizes = {k: len(json.dumps(v, default=str)) for k, v in value.items()}
        keep = {k: v for k, v in value.items() if sizes[k] < 2000}
        keep["_cut"] = {"say": "too long for one answer; the long parts are named with their sizes. "
                               "Ask for one part (a section, or offset/limit).",
                        "parts": {k: n for k, n in sizes.items() if k not in keep}}
        return keep
    return {"_cut": "too long for one answer (%d characters)" % len(text), "start": text[:most]}


def _page(rows, offset, limit, most=5000):
    offset, limit = max(0, int(offset)), max(1, min(int(limit), most))
    return {"total": len(rows), "offset": offset, "count": len(rows[offset:offset + limit]),
            "rows": rows[offset:offset + limit],
            "next": offset + limit if offset + limit < len(rows) else None}


def _addr(text):
    a = (text or "").strip().lower()
    return a[2:] if a.startswith("0x") else a.rstrip("h") if a.endswith("h") and len(a) > 1 else a


def _store_view(module):
    """server.functions for a module, remembered a few seconds (it reads the ledger)."""
    value, _ = SLOW.get(("functions", module), lambda: _quiet(server.functions, module), age=15.0, wait=120.0)
    return value


# ---- reading ---------------------------------------------------------------------------------
def describe(path: str) -> dict:
    """What a file is, the platform it belongs to, and the lane CodeBreaker reconstructs it with
    (None when there is none yet). A corpus name is accepted too."""
    from codebreaker.coverage import reach
    p = _path(path)
    if os.path.isdir(p):
        return {"path": p, "found": "a folder", "say": "a folder: use reach through run_command to go through it"}
    out = dict(_quiet(reach.describe, p), path=p, size=os.path.getsize(p))
    out["summary"] = _quiet(inspect.describe, p).get("summary", {})
    return out


def inspect_module(module: str, section: str = "") -> dict:
    """The inspector: headers, sections, imports, exports, resources, version. Without `section`
    the whole description (long parts are listed instead when it is too big); with it, one part."""
    p = _path(module)
    if os.path.isdir(p):
        from codebreaker.java import transpile
        return {"path": p, "kind": "java", "classes": len(transpile.discover(p))}
    info = _quiet(inspect.describe, p)
    if section:
        if section not in info:
            raise ValueError("no part %r; there are: %s" % (section, ", ".join(info)))
        return _fit({section: info[section]})
    return _fit(info)


def strings(module: str, find: str = "", min_len: int = 4, offset: int = 0, limit: int = 300) -> dict:
    """Text found in a file (ASCII and UTF-16), with file offsets; `find` keeps those containing it."""
    rows = _quiet(inspect.strings, _path(module), max(2, min(int(min_len), 64)))
    if find:
        rows = [r for r in rows if find.lower() in r["text"].lower()]
    return _page(rows, offset, limit)


def hex_dump(module: str, offset: int = 0, length: int = 512) -> dict:
    """A hex dump of a file's bytes from `offset` (at most 16 KB)."""
    p = _path(module)
    n = max(1, min(int(length), 16384))
    return {"offset": int(offset), "length": n, "size": os.path.getsize(p),
            "text": inspect.hexdump(p, max(0, int(offset)), n)}


def resource(module: str, offset: int, size: int, type_id: int = -1, wide: bool = True):
    """One resource, by the file offset and size `inspect` gives for it: a picture (icon,
    bitmap, cursor) as an image, anything else as text."""
    body = _quiet(inspect.resource_body, _path(module), int(offset), int(size), int(type_id), bool(wide))
    if body.get("kind") == "image":
        from mcp.server.mcpserver import Image
        return Image(data=body["bytes"], format=body["content_type"].split("/")[-1])
    return _fit(body)


def _ledger():
    from codebreaker.coverage import ledger
    return _quiet(ledger.measure)


def modules(kind: str = "", find: str = "", only_in_store: bool = False, offset: int = 0, limit: int = 200,
            refresh: bool = False) -> dict:
    """The corpus modules with their ledger numbers (functions, verified, weak, unverified, ...).
    `kind`: pe, ne, le, dos, powerpc, ...; `find`: part of a name."""
    led, waited = SLOW.get("ledger", _ledger, again=refresh)
    if led is None:
        return _not_ready("the coverage ledger", waited)
    from codebreaker.coverage import ledger
    keep = ("name", "kind", "size", "in_store", "measured", "judged") + ledger.COUNTS
    rows = [{k: r.get(k) for k in keep} for r in led.get("modules", [])
            if (not kind or r["kind"] == kind) and (not find or find.lower() in r["name"])
            and (r["in_store"] or not only_in_store)]
    return _page(rows, offset, limit)


def coverage(refresh: bool = False) -> dict:
    """How the corpus stands: totals and one row per kind of module."""
    led, waited = SLOW.get("ledger", _ledger, again=refresh)
    if led is None:
        return _not_ready("the coverage ledger", waited)
    return {k: led.get(k) for k in ("total", "kinds", "profiles", "error") if k in led}


def progress() -> dict:
    """What a running sweep is doing: results per minute, recent modules, busy workers, a rough ETA."""
    from codebreaker.ui import progress as P
    value, waited = SLOW.get("progress", lambda: _quiet(P.snapshot), age=30.0)
    return value if value is not None else _not_ready("the progress reading", waited)


def functions(module: str, cls: str = "", find: str = "", offset: int = 0, limit: int = 300) -> dict:
    """Every function the store holds for a module, with its class (verified, weak, unverified,
    not-judged, no-compile, no-source), name and why; plus counts and the commonest reasons.
    `cls` keeps one class; `find` matches a name or an address."""
    p = _path(module)
    found = _store_view(os.path.basename(p))
    if found is None:
        raise ValueError("%s is not a module of the corpus or the store" % module)
    if "error" in found and "functions" not in found:
        return found
    rows = [r for r in found["functions"] if (not cls or r["class"] == cls)
            and (not find or find.lower() in r["name"].lower() or _addr(find) in r["addr"])]
    out = {k: found[k] for k in ("module", "kind_title", "sha", "in_store", "measured", "counts", "reasons")}
    if not found["in_store"]:
        out["say"] = "not measured yet: run_command('measure', ['%s'])" % module
    out.update(_page(rows, offset, limit))
    return out


def function(module: str, addr: str) -> dict:
    """One function: its reconstructed source, the verdict and the verdict's detail. `addr` is
    its address (hex, with or without 0x) or its name."""
    p = _path(module)
    name = os.path.basename(p)
    found = _store_view(name)
    if not found or not found.get("in_store"):
        raise ValueError("%s has nothing in the store yet; measure it first" % module)
    a = _addr(addr)
    rows = found["functions"]
    hit = next((r for r in rows if r["addr"] == a), None) or \
        next((r for r in rows if r["addr"].endswith(":" + a) or r["addr"].lstrip("0") == a.lstrip("0")), None) or \
        next((r for r in rows if r["name"] and r["name"].lower() == addr.strip().lower()), None)
    if not hit:
        raise ValueError("no function %r in %s (%d functions; `functions` lists them)" % (addr, name, len(rows)))
    one = _quiet(server.functions, name, hit["addr"])["function"]
    if not one.get("source"):                     # not judged under these settings: what was made of it
        with closing(F.open()) as fs:
            made = [{"engine": e, "rev": r, "name": n, "source": s or "", "problem": pr or "",
                     "older_verdicts": [{"profile": p, "verdict": v, "reason": why or "", "limits": lim or ""}
                                        for p, v, why, lim in fs.db.execute(
                                            "SELECT profile, verdict, reason, limits FROM verdict WHERE candidate=? "
                                            "ORDER BY ts DESC LIMIT 5", (cid,))]}
                    for cid, e, r, n, s, pr in fs.db.execute(
                        "SELECT c.id, c.engine, c.rev, c.name, c.source, c.problem FROM candidate c JOIN function f "
                        "ON f.id=c.function WHERE f.module=? AND f.addr=?", (found["sha"], hit["addr"]))]
        if made:
            one["candidates"] = made
            one["say"] = ("no verdict under the verifier settings in force (profile %s); these are the stored "
                          "reconstructions, with any verdicts from earlier settings" % found.get("profile"))
    return _fit(one)


def names(module: str, find: str = "", offset: int = 0, limit: int = 300) -> dict:
    """Every function and import of a module with its best name and where the name came from
    (exports, debug symbols, runtime libraries, bodies named in other modules)."""
    from codebreaker.symbols import names as N
    got = _quiet(N.of_module, _path(module))
    fns = [{"addr": "%x" % a if isinstance(a, int) else str(a), "key": k, "name": n, "source": s}
           for a, k, n, s in got["functions"] if not find or find.lower() in n.lower()]
    out = _page(fns, offset, limit)
    out["imports"] = len(got["imports"])
    return out


def identify(module: str) -> dict:
    """Functions of a module that belong to a runtime library (by signature), and what built it."""
    from codebreaker.symbols import runtime_library as R
    p = _path(module)
    found = _quiet(R.identify, p)
    rows = [{"addr": "%x" % a if isinstance(a, int) else str(a), "name": n, "library": lib}
            for a, (n, lib) in sorted(found.items())]
    out = {"identified": len(rows), "rows": rows[:2000]}
    try:
        out["compiler"] = _quiet(R.compiler, p, found)
    except Exception as e:
        out["compiler"] = "unknown (%s)" % type(e).__name__
    return _fit(out)


def demangle(decorated: list[str]) -> dict:
    """Decorated linker names (Microsoft C++ ?Foo@@..., _Bar@8) as readable declarations."""
    from codebreaker.symbols import demangle as D
    return {n: {"readable": D.readable(n), "declaration": D.declaration(n), "stack_bytes": D.stack_bytes(n)}
            for n in decorated[:200]}


def _commands():
    from codebreaker import __main__ as M
    return M


def commands() -> dict:
    """Every command of `python -m codebreaker` with its one-line usage (any of them can be run as
    a job through run_command, except the windows and servers)."""
    M = _commands()
    return {"usage": M.__doc__, "runnable": sorted(set(M.COMMANDS) - INTERACTIVE)}


def command_help(command: str) -> dict:
    """The full description of one command: its module's text (arguments, options, what it writes)."""
    M = _commands()
    if command not in M.COMMANDS:
        raise ValueError("no command %r; `commands` lists them" % command)
    import importlib
    mod = importlib.import_module(M.COMMANDS[command][0])
    return {"command": command, "module": M.COMMANDS[command][0], "fixed_args": M.COMMANDS[command][1],
            "help": (mod.__doc__ or "(no description)")[:MOST]}


# ---- jobs ------------------------------------------------------------------------------------
def run_command(command: str, args: list[str] | None = None) -> dict:
    """Start `python -m codebreaker <command> <args...>` as a background job; returns at once with
    a job id (follow it with `job`). For anything that takes more than a few seconds: decompile,
    measure, reach --judge, pe-source, recompile-project, xna, java, ... At most two jobs run at a
    time and only one measure."""
    M = _commands()
    args = [str(a) for a in (args or [])]
    if command not in M.COMMANDS:
        raise ValueError("no command %r; `commands` lists them" % command)
    if command in INTERACTIVE:
        raise ValueError("%r opens a window or a server; it is not run as a job" % command)
    label = next((os.path.basename(os.path.normpath(a)) for a in args if not a.startswith("-")), "")
    return server.JOBS.launch(command, args, label.lower())


def jobs() -> dict:
    """The jobs this server started, newest first."""
    return {"jobs": server.JOBS.all()}


def job(job_id: str, offset: int = 0, tail: int = 0) -> dict:
    """A job's state and its output from byte `offset` (up to 256 KB; `next` is where to go on).
    `tail`: just the last that many characters of the output instead."""
    j = server.JOBS.get(job_id)
    if not j:
        raise ValueError("no job %r (this server's jobs: `jobs`)" % job_id)
    out = server.JOBS.view(j, 0 if tail else int(offset))
    if tail:
        out["log"] = out["log"][-max(1, min(int(tail), MOST)):]
    elif len(out["log"]) > MOST:
        out["next"] = int(offset) + len(out["log"][:MOST].encode("utf-8"))
        out["log"] = out["log"][:MOST]
    return out


def job_file(job_id: str, offset: int = 0, length: int = MOST) -> dict:
    """The file a job wrote (a decompile's C), from character `offset`."""
    j = server.JOBS.get(job_id)
    if not j or not (j["output"] and os.path.isfile(j["output"])):
        raise ValueError("job %r wrote no file" % job_id)
    with open(j["output"], encoding="utf-8", errors="replace") as f:
        text = f.read()
    n = max(1, min(int(length), MOST))
    return {"path": j["output"], "characters": len(text), "offset": int(offset),
            "text": text[int(offset):int(offset) + n],
            "next": int(offset) + n if int(offset) + n < len(text) else None}


def stop_job(job_id: str) -> dict:
    """Stop a job this server started (only that process is stopped)."""
    if not server.JOBS.stop(job_id):
        raise ValueError("no job %r" % job_id)
    return server.JOBS.view(server.JOBS.get(job_id))


# ---- writing ---------------------------------------------------------------------------------
def name_function(module: str, addr: str, name: str) -> dict:
    """Give the function at `addr` a name in the function store. The name goes on its body, so
    every module holding the same code gets it too (the next module write uses it)."""
    from codebreaker.symbols import names as N
    if not name or not name.replace("_", "a").replace("::", "a").isalnum():
        raise ValueError("a name is letters, digits, _ and ::")
    p = _path(module)
    a = _addr(addr)
    address = int(a.split(":")[-1], 16)
    digest = _quiet(N.set_name, p, address, name)
    if not digest:
        raise ValueError("no body recorded at %s in %s (too small to tell apart, or not a function start)"
                         % (addr, os.path.basename(p)))
    with closing(F.open()) as fs:
        shared = fs.db.execute("SELECT COUNT(DISTINCT module) FROM body WHERE hash=?", (digest,)).fetchone()[0]
    return {"named": name, "body": digest, "modules_with_this_body": shared}


# ---- the server ------------------------------------------------------------------------------
READING = (describe, inspect_module, strings, hex_dump, resource, modules, coverage, progress,
           functions, function, names, identify, demangle, commands, command_help, jobs, job, job_file)
TOOL_NAMES = {inspect_module: "inspect", demangle: "demangle"}


def _said(fn):
    """`fn` with the failures it means to report (ValueError: a wrong name, nothing stored yet)
    given to the assistant as its message, not as a crash."""
    from mcp.server.mcpserver.exceptions import ToolError

    @functools.wraps(fn)
    def tool(*args, **kw):
        try:
            return fn(*args, **kw)
        except (ValueError, OSError) as e:
            raise ToolError(str(e))
    return tool


def build():
    """The MCP server with every tool, the function resource and the triage prompt."""
    from mcp.server.mcpserver import MCPServer
    from mcp_types import ToolAnnotations
    app = MCPServer("codebreaker", title="CodeBreaker", instructions=INSTRUCTIONS,
                    version=_version(), log_level="WARNING")
    for fn in READING:
        app.tool(name=TOOL_NAMES.get(fn), annotations=ToolAnnotations(read_only_hint=True))(_said(fn))
    app.tool(annotations=ToolAnnotations(read_only_hint=False, destructive_hint=True, open_world_hint=False))(_said(run_command))
    app.tool(annotations=ToolAnnotations(read_only_hint=False, destructive_hint=True))(_said(stop_job))
    app.tool(annotations=ToolAnnotations(read_only_hint=False, destructive_hint=False, idempotent_hint=True))(_said(name_function))

    @app.resource("codebreaker://function/{module}/{addr}", mime_type="text/x-c",
                  description="One function's reconstructed source, as the store holds it")
    def function_source(module: str, addr: str) -> str:
        return function(module, addr).get("source") or "/* no source: see the verdict (the `function` tool) */"

    @app.prompt(description="Take a binary apart: what it is, what is in it, where its reconstruction stands")
    def triage(path: str) -> str:
        return ("Triage %s with the CodeBreaker tools: `describe` it; `inspect` its summary, imports and "
                "exports; `functions` for what the store holds (if nothing, offer to `run_command` measure "
                "on it and follow the job); then read the worst unverified functions with `function` and "
                "say, for each, what most likely went wrong and what the original code was for." % path)
    return app


def _version():
    try:
        from importlib.metadata import version
        return version("codebreaker")
    except Exception:
        return ""


def main(argv):
    """python -m codebreaker mcp [--http PORT]    (stdio unless --http: streamable HTTP on 127.0.0.1)"""
    app = build()
    if "--http" in argv:
        port = int(argv[argv.index("--http") + 1])
        app.run("streamable-http", host="127.0.0.1", port=port)
    else:
        app.run("stdio")
    server.JOBS.stop_all()
    return 0
