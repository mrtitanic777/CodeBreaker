r"""mcp_server.py -- CodeBreaker as an MCP server: what the console shows, for an assistant to use.

    python -m codebreaker mcp                     serve over stdin/stdout (what an MCP client starts)
    python -m codebreaker mcp --http PORT         streamable HTTP on 127.0.0.1, behind a bearer token
    claude mcp add codebreaker -- python -m codebreaker mcp      (or the project's .mcp.json)

The same reading and the same jobs as the web console (server.py), as tools an assistant such
as Claude Code calls. Three kinds:

    reading     nothing changes: describe, inspect, strings, hex_dump, resource, modules,
                coverage, progress, functions, function, names, identify, demangle, commands,
                command_help; what was made: reach_results, projects, project, source_tree,
                read_file (only inside Converted/, state/, this checkout and the source tree);
                worked out on asking, kept an hour: decompile_function, compare_versions
    jobs        any command of `python -m codebreaker` run as a process of its own, below normal
                priority, its output kept in state/ui_jobs/ (as the console's jobs): run_command,
                jobs, job, job_file, stop_job. At most two at a time, at most one `measure`.
    writing     name_function: give a function's body a name in the function store; every module
                holding the same body then has it (symbols/names.py set_name)

Every answer is capped (MOST characters) and pages with offset/limit where it can be long. The
ledger behind coverage, modules and progress reads the whole corpus (minutes when cold), so it is
worked out in the background and remembered, and kept in state/mcp/ledger.json so a server
started later answers at once from the last one (saying how old it is) while a fresh one is made;
a call that finds neither says so instead of holding the client past its timeout. Stdout is the protocol: the MCP library points file 1 at
stderr while serving, so anything the commands print cannot corrupt it.
"""
import functools
import io
import json
import os
import re
import sys
import threading
import time
from contextlib import closing, redirect_stdout

from codebreaker.base import corpus, settings
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
no-source); `decompile_function` for one function's C now. What was made lives in `projects`/`project`
(Converted/), `source_tree` (the product's source tree) and `reach_results`; `read_file` reads any of
it. `compare_versions` matches two builds function by function. Anything long (decompile, measure,
reach --judge, pe-source, recompile-project) goes through `run_command`, then `job` to follow it.
A module is a corpus name (lz32.dll) or a path."""


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
    """The ledger, worked out, and kept on disk so a server started later has it at once."""
    from codebreaker.coverage import ledger
    led = _quiet(ledger.measure)
    if "error" not in led:
        path = settings.state("mcp", "ledger.json")
        with open(path + ".tmp", "w", encoding="utf-8") as f:
            json.dump({"time": time.time(), "ledger": led}, f, default=str)
        os.replace(path + ".tmp", path)
    return led


def _ledger_now(refresh=False):
    """(ledger, None) when it is known; (the one kept on disk, a note how old) while it is
    being worked out again; (None, not-ready answer) when there is neither."""
    led, waited = SLOW.get("ledger", _ledger, again=refresh)
    if led is not None:
        return led, None
    try:
        with open(settings.state("mcp", "ledger.json"), encoding="utf-8") as f:
            kept = json.load(f)
        return kept["ledger"], {"as_of": time.strftime("%Y-%m-%d %I:%M %p", time.localtime(kept["time"])),
                                "say": "the ledger kept from then; a fresh one is being worked out"}
    except (OSError, ValueError, KeyError):
        return None, _not_ready("the coverage ledger", waited)


def modules(kind: str = "", find: str = "", only_in_store: bool = False, offset: int = 0, limit: int = 200,
            refresh: bool = False) -> dict:
    """The corpus modules with their ledger numbers (functions, verified, weak, unverified, ...).
    `kind`: pe, ne, le, dos, powerpc, ...; `find`: part of a name."""
    led, note = _ledger_now(refresh)
    if led is None:
        return note
    from codebreaker.coverage import ledger
    keep = ("name", "kind", "size", "in_store", "measured", "judged") + ledger.COUNTS
    rows = [{k: r.get(k) for k in keep} for r in led.get("modules", [])
            if (not kind or r["kind"] == kind) and (not find or find.lower() in r["name"])
            and (r["in_store"] or not only_in_store)]
    return dict(_page(rows, offset, limit), **({"kept": note} if note else {}))


def coverage(refresh: bool = False) -> dict:
    """How the corpus stands: totals and one row per kind of module."""
    led, note = _ledger_now(refresh)
    if led is None:
        return note
    return dict({k: led.get(k) for k in ("total", "kinds", "profiles", "error") if k in led},
                **({"kept": note} if note else {}))


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


# ---- what was made: reach runs, project folders, the source tree --------------------------------
def reach_results(run: str = "", platform: str = "", judged_only: bool = False, find: str = "",
                  offset: int = 0, limit: int = 100) -> dict:
    """The results `reach` kept (state/reach/<run>.json). Without `run`, the runs there are. With
    it, the programs found: what each is, its platform and lane, and for those judged the tally;
    `platform` and `find` (part of a path) narrow it, plus a count per platform."""
    folder = os.path.dirname(settings.state("reach", "x"))
    if not run:
        runs = []
        for name in sorted(os.listdir(folder)):
            if name.endswith(".json"):
                try:
                    with open(os.path.join(folder, name), encoding="utf-8") as f:
                        d = json.load(f)
                except (OSError, ValueError):
                    continue
                progs = d.get("programs", [])
                runs.append({"run": name[:-5], "when": d.get("when"), "inputs": d.get("inputs"),
                             "programs": len(progs), "judged": sum(1 for r in progs if r.get("judged"))})
        return {"runs": runs}
    path = os.path.join(folder, os.path.basename(run) + ".json")
    if not os.path.isfile(path):
        raise ValueError("no reach run %r (reach_results without a run lists them)" % run)
    with open(path, encoding="utf-8") as f:
        d = json.load(f)
    per = {}
    rows = []
    for r in d.get("programs", []):
        p = per.setdefault(r.get("platform") or "(unknown)", {"programs": 0, "judged": 0, "lane": r.get("lane")})
        p["programs"] += 1
        p["judged"] += bool(r.get("judged"))
        if platform and platform.lower() not in (r.get("platform") or "").lower():
            continue
        if find and find.lower() not in (r.get("path") or "").lower():
            continue
        if judged_only and not r.get("judged"):
            continue
        rows.append({k: r.get(k) for k in ("path", "origin", "found", "platform", "lane", "family", "size", "picked",
                                           "judged") if r.get(k) not in (None, "")})
    return dict(_page(rows, offset, limit), when=d.get("when"), inputs=d.get("inputs"), platforms=per)


def _roots():
    """The folders read_file may read: project folders, the source tree, state, this checkout."""
    from codebreaker.source import placement
    out = [settings.get("converted"), settings.get("state"), settings.ROOT]
    try:
        out.append(placement.tree_root())
    except Exception:
        pass
    return [os.path.realpath(r) for r in out if r and os.path.isdir(r)]


def _inside(path, root):
    try:
        return os.path.commonpath([path, root]) == root
    except ValueError:                                           # another drive
        return False


def _listing(folder, find="", most=2000):
    rows = []
    for top, dirs, files in os.walk(folder):
        dirs[:] = sorted(d for d in dirs if not d.startswith("."))
        for name in sorted(files):
            rel = os.path.relpath(os.path.join(top, name), folder).replace("\\", "/")
            if not find or find.lower() in rel.lower():
                rows.append({"file": rel, "size": os.path.getsize(os.path.join(top, name))})
                if len(rows) >= most:
                    return rows, True
    return rows, False


def projects(find: str = "") -> dict:
    """The project folders under Converted/ (one per binary CodeBreaker has taken apart): name,
    the original's file name, its kind and the commands run on it."""
    root = settings.get("converted")
    out = []
    for name in sorted(os.listdir(root)) if os.path.isdir(root) else ():
        pj = os.path.join(root, name, "project.json")
        if not os.path.isfile(pj):
            continue
        try:
            with open(pj, encoding="utf-8") as f:
                d = json.load(f)
        except (OSError, ValueError):
            d = {}
        row = {"name": name, "file": d.get("file", ""), "kind": d.get("kind", ""), "size": d.get("size"),
               "commands": [s.get("command") for s in d.get("steps", [])]}
        if not find or find.lower() in (name + " " + row["file"]).lower():
            out.append(row)
    return {"root": root, "projects": out}


def project(name: str, find: str = "") -> dict:
    """One project folder: its project.json (what was done, each output) and its files."""
    folder = os.path.join(settings.get("converted"), os.path.basename(name))
    if not os.path.isdir(folder):
        raise ValueError("no project %r under %s (`projects` lists them)" % (name, settings.get("converted")))
    d = {}
    if os.path.isfile(os.path.join(folder, "project.json")):
        with open(os.path.join(folder, "project.json"), encoding="utf-8") as f:
            d = json.load(f)
    for step in d.get("steps", []):
        outs = step.get("outputs") or []
        if len(outs) > 20:
            step["outputs"] = outs[:20] + ["... %d more" % (len(outs) - 20)]
    files, more = _listing(folder, find)
    return _fit({"folder": folder, "project": d, "files": files, "more_files": more})


def source_tree(module: str, find: str = "") -> dict:
    """Where a module's source lives in the product's source tree (and why there), and its files."""
    from codebreaker.source import placement
    p = _path(module)
    rel, how = placement.resolve(os.path.basename(p), p)
    folder = os.path.join(placement.tree_root(), *rel.split("/"))
    files, more = _listing(folder, find) if os.path.isdir(folder) else ([], False)
    return {"tree": placement.tree_root(), "folder": folder, "relative": rel, "decided": how,
            "exists": os.path.isdir(folder), "files": files, "more_files": more}


def read_file(path: str, offset: int = 0, length: int = 40000) -> dict:
    """A text file from a project folder, the source tree, state/ or this checkout, from character
    `offset` (a binary file: use hex_dump)."""
    real = os.path.realpath(path)
    if not any(_inside(real, r) for r in _roots()):
        raise ValueError("%s is outside the folders this reads: %s" % (path, ", ".join(_roots())))
    if not os.path.isfile(real):
        raise ValueError("no file %s" % path)
    with open(real, "rb") as f:
        head = f.read(4096)
    if b"\0" in head:
        raise ValueError("%s is binary; hex_dump reads it" % path)
    with open(real, "rb") as f:
        data = f.read()
    try:
        text = data.decode("utf-8")
    except UnicodeDecodeError:                                   # the period's files: Windows-1252
        text = data.decode("cp1252", "replace")
    at, n = max(0, int(offset)), max(1, min(int(length), MOST))
    return {"path": real, "characters": len(text), "offset": at, "text": text[at:at + n],
            "next": at + n if at + n < len(text) else None}


# ---- one function now; two builds side by side ------------------------------------------------
_HEADER_ADDR = re.compile(r"\b(?:sub|func|FUN)_(?:0x)?([0-9a-fA-F]+)(?:_([0-9a-fA-F]+))?\s*\(")


def _same_place(addr, header):
    """Does a function header line name the place `addr` is (hex, or segment:offset)? Compared
    as numbers: sub_00132 is 132, sub_1_0116 is 1:0116, func_0x7f021036 is 7f021036."""
    m = _HEADER_ADDR.search(header)
    if not m:
        return False
    try:
        if ":" in addr:
            seg, off = addr.split(":", 1)
            return m.group(2) is not None and (int(m.group(1), 16), int(m.group(2), 16)) == (int(seg, 16), int(off, 16))
        return m.group(2) is None and int(m.group(1), 16) == int(addr, 16)
    except ValueError:
        return False


def _cut_function(text, addr):
    """The function in a whole module's C at `addr` (or named `addr`), with the comment lines
    just above its header."""
    named = re.compile(r"\b%s\s*\(" % re.escape(addr), re.I) if not re.fullmatch(r"[0-9a-f:]+", addr) else None
    lines = text.splitlines()
    for i, line in enumerate(lines):
        if not line or line[:1].isspace() or "(" not in line:
            continue
        if not (line.rstrip().endswith("{") or (i + 1 < len(lines) and lines[i + 1].strip() in ("{", ""))):
            continue
        if not (named.search(line) if named else _same_place(addr, line)):
            continue
        start = i
        while start > 0 and lines[start - 1].startswith(("/*", " *", "//")):
            start -= 1
        end = i + 1
        while end < len(lines) and lines[end] != "}":
            end += 1
        return "\n".join(lines[start:end + 1])
    return None


def decompile_function(module: str, addr: str, wait: int = 60) -> dict:
    """One function decompiled now, without starting a job: a 32-bit x86 function by itself (cached
    by its inputs); any other kind decompiles its whole module once (kept) and the function is cut
    out of it. Not built or judged. Says so when it takes longer than `wait` seconds: call again."""
    from codebreaker.decompile import decompiler as D
    p = _path(module)
    a = _addr(addr)
    wait = max(1, min(int(wait), 240))
    if corpus.lane(p) == "pe" and D._is_x86(p):
        try:
            va = int(a, 16)
        except ValueError:
            raise ValueError("a 32-bit function is asked for by its address (hex)")

        def work():
            got = _quiet(D.decompile, p, [va]).functions.get(va)
            return {"name": got[0], "text": got[1]} if got else {"error": "no function at %x" % va}
        value, waited = SLOW.get(("decompile", p, va), work, age=3600.0, wait=wait)
    else:
        value, waited = SLOW.get(("decompile", p), lambda: {"whole": _quiet(D.as_c, p)[0]}, age=3600.0, wait=wait)
        if value is not None and "whole" in value:
            cut = _cut_function(value["whole"], a)
            value = {"text": cut} if cut else {"error": "no function header naming %s in the module's C; "
                                                        "run_command decompile writes all of it" % addr}
    if value is None:
        return _not_ready("decompiling %s at %s" % (os.path.basename(p), addr), waited)
    if "error" in value:
        raise ValueError(value["error"])
    return dict(value, module=os.path.basename(p), addr=a, note="decompiled now; not built or judged")


def compare_versions(module_a: str, module_b: str, status: str = "", offset: int = 0, limit: int = 200,
                     wait: int = 60) -> dict:
    """Two builds of a module function by function (compare/versions.py): identical, changed (with
    a likeness score), added, removed, and the names the old build gives functions the new one
    leaves unnamed. `status` keeps identical or changed pairs only."""
    from codebreaker.compare import versions
    pa, pb = _path(module_a), _path(module_b)

    def work():
        c = _quiet(versions.compare, pa, pb)
        return {"report": c.report(), "counts": c.counts(), "code": c.code(), "pairs": c.pairs,
                "added": c.added, "removed": c.removed, "names": c.names()}
    value, waited = SLOW.get(("versions", pa, pb), work, age=3600.0, wait=max(1, min(int(wait), 240)))
    if value is None:
        return _not_ready("comparing the two builds", waited)
    if "error" in value:
        return value
    pairs = [x for x in value["pairs"] if not status or x["status"] == status]
    out = {k: value[k] for k in ("report", "counts", "code")}
    out.update(added=value["added"][:500], removed=value["removed"][:500], names=value["names"][:500])
    out.update(_page(pairs, offset, limit))
    return _fit(out)


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
           functions, function, names, identify, demangle, commands, command_help, jobs, job, job_file,
           reach_results, projects, project, source_tree, read_file, decompile_function, compare_versions)
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


def token():
    """The secret an HTTP client must send (Authorization: Bearer <it>): CB_MCP_TOKEN, else one
    made once and kept in state/mcp/http_token.txt."""
    if os.environ.get("CB_MCP_TOKEN"):
        return os.environ["CB_MCP_TOKEN"]
    path = settings.state("mcp", "http_token.txt")
    if not os.path.isfile(path):
        import secrets
        with open(path, "w", encoding="ascii") as f:
            f.write(secrets.token_urlsafe(32))
    with open(path, encoding="ascii") as f:
        return f.read().strip()


class Bearer(object):
    """ASGI: every HTTP request must carry the token; anything else gets 401 and goes no further."""

    def __init__(self, app, secret):
        import hmac
        self.app, self.secret, self.same = app, ("Bearer " + secret).encode(), hmac.compare_digest

    async def __call__(self, scope, receive, send):
        if scope["type"] == "http":
            got = dict(scope.get("headers") or []).get(b"authorization", b"")
            if not self.same(got, self.secret):
                await send({"type": "http.response.start", "status": 401,
                            "headers": [(b"content-type", b"text/plain"), (b"www-authenticate", b"Bearer")]})
                await send({"type": "http.response.body", "body": b"a bearer token is needed"})
                return
        await self.app(scope, receive, send)


def http_app(hosts=()):
    """The server as an ASGI app (streamable HTTP at /mcp) behind the token. `hosts` are the
    extra Host names it answers to (a tunnel's), beside 127.0.0.1 and localhost."""
    from mcp.server.transport_security import TransportSecuritySettings
    allowed = ["127.0.0.1:*", "localhost:*"] + list(hosts)
    security = TransportSecuritySettings(enable_dns_rebinding_protection=True, allowed_hosts=allowed,
                                         allowed_origins=["http://127.0.0.1:*", "http://localhost:*"] +
                                         ["https://" + h for h in hosts])
    return Bearer(build().streamable_http_app(transport_security=security), token())


def main(argv):
    """python -m codebreaker mcp                                    stdio (what Claude Code starts)
    python -m codebreaker mcp --http PORT [--host H] [--allow-host NAME ...]
        streamable HTTP at http://H:PORT/mcp (H: 127.0.0.1), every request needing the bearer
        token (CB_MCP_TOKEN, else state/mcp/http_token.txt); --allow-host: a Host name besides
        127.0.0.1 and localhost to answer to, such as a tunnel's"""
    if "--http" in argv:
        import uvicorn
        port = int(argv[argv.index("--http") + 1])
        host = argv[argv.index("--host") + 1] if "--host" in argv else "127.0.0.1"
        hosts = [argv[i + 1] for i, a in enumerate(argv) if a == "--allow-host"]
        token()
        print("CodeBreaker MCP on http://%s:%d/mcp -- token in %s" % (
            host, port, "CB_MCP_TOKEN" if os.environ.get("CB_MCP_TOKEN") else settings.state("mcp", "http_token.txt")),
            file=sys.stderr, flush=True)
        uvicorn.run(http_app(hosts), host=host, port=port, log_level="warning")
    else:
        build().run("stdio")
    server.JOBS.stop_all()
    return 0
