r"""mcp_server.py -- CodeBreaker as an MCP server: what the terminal interface shows, for an assistant to use.

    python -m codebreaker mcp                     serve over stdin/stdout (what an MCP client starts)
    python -m codebreaker mcp --http PORT         streamable HTTP on 127.0.0.1, behind a bearer token
    claude mcp add codebreaker -- python -m codebreaker mcp      (or the project's .mcp.json)

The same reading and the same jobs as the terminal interface (both through ui/service.py), as tools an assistant such
as Claude Code calls. This process stays the MCP server (stdio or HTTP) but each tool is answered by
the CodeBreaker service (ui/serve.py; started on demand), so the terminal interface, every MCP
server and the command line share one process, its open databases and its slow answers; with no
service (CB_SERVICE=0, or it cannot start) the tools are answered here, as before. Three kinds:

    reading     nothing changes: triage, describe, inspect, strings, hex_dump, resource, modules,
                coverage, progress, functions, function, names, identify, demangle, commands,
                command_help; the code itself (analyze/image.py, call_graph.py): disasm, xrefs;
                what was made: reach_results, projects, project, source_tree, read_file (only
                inside Converted/, state/, this checkout and the source tree); worked out on
                asking, kept an hour: decompile_function, compare_versions, diff_function
    jobs        any command of `python -m codebreaker` run as a process of its own, below normal
                priority, its output kept in state/ui_jobs/ (as the terminal interface's jobs): run_command,
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
import json
import os
import re
import sys
import threading
import time
from contextlib import closing
from inspect import signature

from codebreaker.base import corpus, settings
from codebreaker.formats import inspect
from codebreaker.store import functions as F
from codebreaker.ui import service
from codebreaker.verify import verdicts as V

MOST = 60000                     # characters in one answer, at most
INTERACTIVE = {"ui", "progress", "mcp"}                 # interfaces and servers: not jobs

INSTRUCTIONS = """CodeBreaker reconstructs readable source from binaries (Windows 16/32-bit, DOS, VxD
drivers, Xbox, Xbox 360, 8/16-bit consoles, XNA, Java) and checks that the source behaves like the
original by running both in an emulator. Start with `triage` on a module (everything below at a
glance), `describe` on a file or `modules` for the corpus; `inspect` for headers/imports/exports;
`functions` and `function` for what the store holds (each function's reconstructed C and its verdict:
verified, weak, unverified, not-judged, no-compile, no-source); `decompile_function` for one function's
C now; `disasm` for its instructions, `xrefs` for who calls it and what it calls, `hex_dump` by address.
What was made lives in `projects`/`project` (Converted/), `source_tree` (the product's source tree)
and `reach_results`; `read_file` reads any of it. `compare_versions` matches two builds function by
function, `diff_function` shows one pair side by side. Anything long (decompile, measure, reach
--judge, pe-source, recompile-project) goes through `run_command`, then `job` to follow it.
A module is a corpus name (lz32.dll) or a path; an address is hex, or a function's name."""


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
    """`work(...)` with anything it prints kept out of the answer (and off the protocol); in the
    service, out of the other calls' too (service.captured is per thread there)."""
    with service.captured():
        return work(*args, **kw)


def _path(module):
    """The file (or Java folder) a module name or path stands for; raises ValueError."""
    module = (module or "").strip().strip('"')
    if not module:
        raise ValueError("name a module (a corpus name such as lz32.dll, or a path)")
    path = module if os.path.isdir(module) else corpus.retail(module) or service.stored_path(module)
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
    """service.functions for a module, remembered a few seconds (it reads the ledger)."""
    value, _ = SLOW.get(("functions", module), lambda: _quiet(service.functions, module), age=15.0, wait=120.0)
    return value


def _image(p):
    """The module as the analyses see it (analyze/image.py), read once and kept an hour."""
    from codebreaker.analyze import image
    value, waited = SLOW.get(("image", p), lambda: {"img": _quiet(image.load, p)}, age=3600.0, wait=120.0)
    if value is None:
        raise ValueError("still reading %s (%d s so far); call again" % (os.path.basename(p), waited))
    if "error" in value:
        raise ValueError(value["error"])
    return value["img"]


def _import_name(text):
    """"KERNEL32!#17" as "KERNEL32!CommonUnimpStub (#17)" when the number has a known name."""
    from codebreaker.symbols import ordinals
    mod, _, nm = (text or "").partition("!")
    if nm.startswith("#") and nm[1:].isdigit():
        o = int(nm[1:])
        real = ordinals.name(mod, o) or (ordinals.documented(mod, o) or (None,))[0]
        if real:
            return "%s!%s (#%d)" % (mod, real, o)
    return text


def _place(img, text):
    """The address `text` stands for in an image: hex (0x optional; an RVA below a 32-bit
    module's base counts from the base), segment:offset in a 16-bit module, or a name."""
    from codebreaker.decompile import describe as D
    a = _addr(text)
    if re.fullmatch(r"[0-9a-f]+:[0-9a-f]+", a) and img.bits == 16:
        seg, off = a.split(":")
        return D.address(int(seg, 16), int(off, 16))
    if re.fullmatch(r"[0-9a-f]+", a):
        va = int(a, 16)
        return va + img.base if img.flat and va < img.base else va
    low = (text or "").strip().lower()
    for table in (img.names, img.exports):
        hit = next((va for va, nm in sorted(table.items()) if nm and nm.lower() == low), None)
        if hit is not None:
            return hit
    from codebreaker.symbols import names as N              # names given elsewhere: the store, other modules
    hit = next((va for va, _k, nm, _s in _quiet(N.of_module, img.path)["functions"]
                if isinstance(va, int) and nm and nm.lower() == low), None)
    if hit is not None:
        return hit
    raise ValueError("no address or function named %r in %s (an address in hex, or a name `names` lists)"
                     % (text, img.name))


def _file_offset(p, va):
    """Where a 32-bit module's address is in its file, or None (not stored there, or not a PE)."""
    from codebreaker.formats.pe import open_pe
    if corpus.lane(p) != "pe":
        return None
    pe = open_pe(p)
    return pe._rva_to_off(va - pe.image_base) if va >= pe.image_base else None


def _hex_addresses(info):
    """The inspector's description with addresses as hex text and by-number imports named. File
    offsets and sizes stay numbers (hex_dump and resource take them as they are)."""
    from codebreaker.symbols import ordinals
    h = lambda v: "0x%x" % v if isinstance(v, int) else v
    for part in (info.get("summary") or {}, (info.get("headers") or {}).get("optional") or {}):
        for k in ("image_base", "entry", "entry_point", "code_base", "data_base"):
            if k in part:
                part[k] = h(part[k])
    for s in info.get("sections") or ():
        s["address"] = h(s.get("address"))
    for s in (info.get("exports") or {}).get("symbols") or ():
        s["address"] = h(s.get("address"))
    for imp in info.get("imports") or ():
        stem = os.path.splitext(imp.get("module", ""))[0]
        for s in imp.get("symbols") or ():
            s["slot"] = h(s.get("slot"))
            if not s.get("name") and isinstance(s.get("ordinal"), int) and s["ordinal"] >= 0:
                real = ordinals.name(stem, s["ordinal"])
                doc = None if real else ordinals.documented(stem, s["ordinal"])
                if real or doc:
                    s["name"] = real or doc[0]
                    s["named_from"] = ordinals.source(stem, s["ordinal"]) if real else doc[1]
    return info


def _identified(p):
    """{addr hex: library} of a module's functions a runtime library or tool made (identify)."""
    from codebreaker.symbols import runtime_library as R
    value, _ = SLOW.get(("identify", p), lambda: _quiet(R.identify, p), age=3600.0, wait=120.0)
    if not isinstance(value, dict) or "error" in value:
        return {}
    return {("%x" % a if isinstance(a, int) else str(a)): lib for a, (_n, lib) in value.items()}


# ---- reading ---------------------------------------------------------------------------------
def describe(path: str) -> dict:
    """What a file is, the platform it belongs to, and the lane CodeBreaker reconstructs it with
    (None when there is none yet). A corpus name is accepted too."""
    from codebreaker.coverage import reach
    p = _path(path)
    if os.path.isdir(p):
        return {"path": p, "found": "a folder", "say": "a folder: use reach through run_command to go through it"}
    out = dict(_quiet(reach.describe, p), path=p, size=os.path.getsize(p))
    out["summary"] = _hex_addresses({"summary": _quiet(inspect.describe, p).get("summary", {})})["summary"]
    return out


def inspect_module(module: str, section: str = "") -> dict:
    """The inspector: headers, sections, imports, exports, resources, version. Without `section`
    the whole description (long parts are listed instead when it is too big); with it, one part:
    summary, dos, headers, sections, exports, imports, resources, directories, relocations, ...
    Addresses are hex text; file offsets and sizes are numbers (as hex_dump and resource take
    them). Imports by number carry the name when one is known, and where it came from."""
    p = _path(module)
    if os.path.isdir(p):
        from codebreaker.java import transpile
        return {"path": p, "kind": "java", "classes": len(transpile.discover(p))}
    info = _hex_addresses(_quiet(inspect.describe, p))
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


def hex_dump(module: str, offset: int = 0, length: int = 512, addr: str = "") -> dict:
    """A hex dump of a file's bytes from file `offset`, or from the place a 32-bit module's
    address (or function name) `addr` is stored (at most 16 KB)."""
    p = _path(module)
    n = max(1, min(int(length), 16384))
    out = {}
    if addr:
        img = _image(p)
        va = _place(img, addr)
        offset = _file_offset(p, va)
        if offset is None:
            raise ValueError("%x is not stored in the file (or %s is not a 32-bit module: give `offset`)"
                             % (va, os.path.basename(p)))
        out["addr"] = "%x" % va
    out.update(offset=int(offset), length=n, size=os.path.getsize(p),
               text=inspect.hexdump(p, max(0, int(offset)), n))
    return out


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
    not-judged, no-compile, no-source), name and why; plus counts and the commonest reasons. A
    function a runtime library or a tool (the thunk compiler) made carries `library`, and is
    counted under `identified`. `cls` keeps one class (or "identified"); `find` matches a name or
    an address."""
    p = _path(module)
    found = _store_view(os.path.basename(p))
    if found is None:
        raise ValueError("%s is not a module of the corpus or the store" % module)
    if "error" in found and "functions" not in found:
        return found
    made = _identified(p)
    rows = []
    for r in found["functions"]:
        if made.get(r["addr"]):                    # made by a tool or a runtime library: not authored code
            r = dict(r, library=made[r["addr"]])
            if r["class"] == V.NO_COMPILE:
                r["why"] = "identified, not reconstructed by design (%s)" % made[r["addr"]]
        if (not cls or r["class"] == cls or (cls == "identified" and r.get("library"))) and \
                (not find or find.lower() in r["name"].lower() or _addr(find) in r["addr"]):
            rows.append(r)
    out = {k: found[k] for k in ("module", "kind_title", "sha", "in_store", "measured", "counts", "reasons")}
    out["counts"] = dict(out["counts"], identified=sum(1 for r in found["functions"] if made.get(r["addr"])))
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
    one = _quiet(service.functions, name, hit["addr"])["function"]
    if not one.get("source"):                     # not judged under these settings: what was made of it
        with closing(F.open()) as fs:
            made = [{"engine": c["engine"], "rev": c["rev"], "name": c["name"], "source": c["source"],
                     "problem": c["problem"], "older_verdicts": [{k: v[k] for k in ("profile", "verdict", "reason", "limits")}
                                                                 for v in c["verdicts"][:5]]}
                    for c in fs.stored(found["sha"], hit["addr"])]
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
    a job through run_command, except the interfaces and servers)."""
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


_BUILT = {".obj", ".o", ".lib", ".exp", ".res", ".map", ".dll", ".exe", ".drv", ".vxd", ".386", ".sym", ".log",
          ".pdb", ".ilk", ".err", ".lst", ".com", ".sys", ".bin"}
_NOTES = {".md", ".txt"}


def _file_kind(rel):
    """"built" (made by a build, or in a folder a build writes), "notes", or "source"."""
    parts = rel.lower().split("/")
    ext = os.path.splitext(parts[-1])[1]
    if ext in _BUILT or any(p.startswith("_build") or p in ("build", "out", "obj") for p in parts[:-1]):
        return "built"
    return "notes" if ext in _NOTES else "source"


def source_tree(module: str, find: str = "") -> dict:
    """Where a module's source lives in the product's source tree (and why there), and its files,
    each marked source, notes (.md, .txt) or built (a build's output or working folder); counts
    of each, source first."""
    from codebreaker.source import placement
    p = _path(module)
    rel, how = placement.resolve(os.path.basename(p), p)
    folder = os.path.join(placement.tree_root(), *rel.split("/"))
    files, more = _listing(folder, find) if os.path.isdir(folder) else ([], False)
    order = {"source": 0, "notes": 1, "built": 2}
    files = sorted((dict(f, kind=_file_kind(f["file"])) for f in files), key=lambda f: order[f["kind"]])
    counts = {k: sum(1 for f in files if f["kind"] == k) for k in order}
    return {"tree": placement.tree_root(), "folder": folder, "relative": rel, "decided": how,
            "exists": os.path.isdir(folder), "counts": counts, "files": files, "more_files": more}


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
    out of it. `addr` is an address (hex) or a name. Not built or judged, and not necessarily the
    text the store holds (that was made by the measure that judged it: `function`). Says so when it
    takes longer than `wait` seconds: call again."""
    from codebreaker.decompile import decompiler as D
    p = _path(module)
    a = _addr(addr)
    wait = max(1, min(int(wait), 240))
    x86 = corpus.lane(p) == "pe" and D._is_x86(p)
    if x86:
        va = _place(_image(p), addr)
        a = "%x" % va

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


_PAIR_FIELDS = ("level: what agrees -- bytes (the same machine code but for what the loader fills in), text "
                "(the same instructions, addresses aside), shape (the same kinds of instruction and operand: "
                "registers or constants differ); how: which matching step paired them (bytes, name, text, "
                "shape, calls, likeness, position); score: likeness of the shapes, so a changed pair at 1.0 "
                "differs only in registers or constants. diff_function shows one pair side by side.")


def _versions(pa, pb, wait):
    """compare/versions.py's Comparison of two builds, worked out once and kept an hour."""
    from codebreaker.compare import versions

    def work():
        c = _quiet(versions.compare, pa, pb)
        return {"c": c, "report": c.report(), "counts": c.counts(), "code": c.code(), "pairs": c.pairs,
                "added": c.added, "removed": c.removed, "names": c.names()}
    return SLOW.get(("versions", pa, pb), work, age=3600.0, wait=max(1, min(int(wait), 240)))


def compare_versions(module_a: str, module_b: str, status: str = "", offset: int = 0, limit: int = 200,
                     wait: int = 60) -> dict:
    """Two builds of a module function by function (compare/versions.py): identical, changed (with
    a likeness score), added, removed, and the names the old build gives functions the new one
    leaves unnamed. Changed pairs come first. `status` keeps identical or changed pairs only."""
    value, waited = _versions(_path(module_a), _path(module_b), wait)
    if value is None:
        return _not_ready("comparing the two builds", waited)
    if "error" in value:
        return value
    pairs = sorted((x for x in value["pairs"] if not status or x["status"] == status),
                   key=lambda x: x["status"] != "changed")
    out = {k: value[k] for k in ("report", "counts", "code")}
    out["fields"] = _PAIR_FIELDS
    out.update(added=value["added"][:500], removed=value["removed"][:500], names=value["names"][:500])
    out.update(_page(pairs, offset, limit))
    return _fit(out)


def diff_function(module_a: str, module_b: str, addr: str, wait: int = 60) -> dict:
    """One function in two builds, instruction by instruction: `addr` is its address (or name) in
    the first build, or in the second. The listings are lined up by the kinds of instruction and
    operand, each line marked ' ' the same (addresses aside), '~' the same instruction with other
    registers or constants, '-' only in the first, '+' only in the second."""
    import difflib
    pa, pb = _path(module_a), _path(module_b)
    value, waited = _versions(pa, pb, wait)
    if value is None:
        return _not_ready("comparing the two builds", waited)
    if "error" in value:
        return value
    c = value["c"]
    want, low = _addr(addr), (addr or "").strip().lower()
    pair = next((x for x in c.pairs if x["from"] == want or x["to"] == want), None) or \
        next((x for x in c.pairs if low in (x["from_name"].lower(), x["to_name"].lower())), None)

    def listing(f):
        return "\n".join("%x  %s" % row for row in f.rows)
    if not pair:
        for side, keys, what in ((c.a, c.removed, "removed: only in the first build"),
                                 (c.b, c.added, "added: only in the second build")):
            k = next((k for k in keys if k == want or side.functions[k].name.lower() == low), None)
            if k:
                return {"status": what, "addr": k, "name": side.functions[k].name, "listing": listing(side.functions[k])}
        raise ValueError("no function %r in either build (compare_versions lists the pairs)" % addr)
    fa, fb = c.a.functions[pair["from"]], c.b.functions[pair["to"]]
    out = {k: pair[k] for k in ("from", "to", "from_name", "to_name", "status", "level", "score", "sizes")}
    lines = []                     # lined up by shape, so other registers or constants do not break the line-up
    for tag, a1, a2, b1, b2 in difflib.SequenceMatcher(None, fa.shape, fb.shape, autojunk=False).get_opcodes():
        if tag == "equal":
            for i, j in zip(range(a1, a2), range(b1, b2)):
                lines.append("%s %x %-36s %x %s" % (" " if fa.text[i] == fb.text[j] else "~", fa.rows[i][0],
                                                    fa.rows[i][1], fb.rows[j][0], fb.rows[j][1]))
            continue
        lines += ["- %x %s" % fa.rows[i] for i in range(a1, a2)]
        lines += ["+ %47s %x %s" % ("", fb.rows[j][0], fb.rows[j][1]) for j in range(b1, b2)]
    out["diff"] = "\n".join(lines)
    out["differ"] = sum(1 for ln in lines if ln[0] != " ")
    return _fit(out)


# ---- code: disassembly, cross-references, a module at a glance --------------------------------
def _ascii_at(img, va, most=40):
    """The text stored at an address, when there is some (4 printable characters or more; code
    sections hold strings too, such as a flat thunk's connection-data name)."""
    s = img.span(va)
    if not s or not img.initialised(va):
        return None
    raw = bytes(s[1][va - s[0]:va - s[0] + most])
    text = raw.split(b"\0")[0]
    if len(text) >= 4 and all(32 <= b < 127 for b in text):
        return text.decode("ascii")
    return None


def _thunk(img, va):
    """The import a `jmp [slot]` at `va` goes to, or None."""
    from codebreaker.analyze import image
    s = img.span(va)
    ins = next(iter(img.linear(va, 1)), None) if s and s[2] else None
    slot = image._slot(ins) if ins is not None and ins.flow == "jmp" and ins.target is None else None
    return (slot, img.imports[slot]) if slot in img.imports else None


def _label(img, va):
    """A function's name, with the import it only jumps to; else name+offset, else the address."""
    t = img.memo(("mcp_thunk", va), lambda im: _thunk(im, va))
    if va in img.names:
        return "%s (thunk: %s)" % (img.names[va], _import_name(t[1])) if t else img.names[va]
    if t:                                          # a thunk no function starts at
        return "thunk at %x: %s" % (va, _import_name(t[1]))
    owner = img.owner(va)
    return "%s+%x" % (img.names[owner], va - owner) if owner is not None else "%x" % va


def _note(img, ins, fn_start=None):
    """What an instruction's destination or address operand stands for."""
    from codebreaker.analyze import image
    if ins.flow in ("call", "jmp", "jcc"):
        dest = image.destination(img, ins)
        if dest is not None and dest in img.imports:
            return _import_name(img.imports[dest])
        if dest is not None and ins.flow != "jcc":
            owner = img.owner(dest)
            if dest in img.names or owner is not None and owner != fn_start:
                return _label(img, dest)
            if owner is None:
                return "%x: no known function starts there" % dest
    for op in ins.ops:
        va = op[1] if op[0] == "i" and op[2] == 4 else op[4] if op[0] == "m" and op[1] is None else None
        if va is None:
            continue
        if va in img.imports:
            return _import_name(img.imports[va])
        text = _ascii_at(img, va)
        if text:
            return '"%s"' % text
        if va in img.names:
            return img.names[va]
    return ""


def disasm(module: str, addr: str, count: int = 0) -> dict:
    """The instructions at an address or function name. At a function's start (count 0): the whole
    function as the analyses decode it, following its branches. Anywhere else, or with `count`: that
    many instructions read straight on from there (64 when count is 0). Each line: address, file
    offset (32-bit modules), bytes, instruction, and what it refers to (a function, an import by its
    name, a string)."""
    p = _path(module)
    img = _image(p)
    va = _place(img, addr)
    n = max(1, min(int(count) or 64, 4000))
    if va in img.names and not int(count):
        fn = img.function(va)
        insns, how = fn.insns, "the function, as decoded from its start"
        head = {"function": img.names[va], "start": "%x" % va, "end": "%x" % fn.end, "limit": "%x" % fn.limit}
    else:
        s = img.span(va)
        if not s:
            raise ValueError("%x is not in %s" % (va, img.name))
        insns = img.linear(va, n)
        how = "%d instructions read straight on" % len(insns)
        owner = img.owner(va)
        head = {"inside": "%s+%x" % (img.names[owner], va - owner) if owner is not None else None}
    pe = corpus.lane(p) == "pe"
    rows = []
    for ins in insns:
        s = img.span(ins.addr)
        code = bytes(s[1][ins.addr - s[0]:ins.addr - s[0] + ins.size]) if s else b""
        off = _file_offset(p, ins.addr) if pe else None
        note = _note(img, ins, va)
        rows.append(("%x%s  %-20s %-34s%s" % (ins.addr, "  %05x" % off if off is not None else "",
                                               code.hex(" ").upper(), ins.text, ("; " + note) if note else "")).rstrip())
    return _fit(dict(head, module=img.name, addr="%x" % va, how=how, count=len(rows), listing="\n".join(rows)))


def _xref_index(img):
    """{address: [(function, site, how)]}: every direct call, jump and import-slot use in the code."""
    from codebreaker.analyze import image
    out = {}
    for fn in img.each():
        for ins in fn.insns:
            if ins.target is not None and ins.flow in ("call", "jmp") or ins.flow == "jcc" and ins.target is not None \
                    and not fn.start <= ins.target < fn.limit:
                out.setdefault(ins.target, []).append((fn.start, ins.addr, ins.flow))
            slot = image._slot(ins)
            if slot is not None and slot in img.imports:
                out.setdefault(slot, []).append((fn.start, ins.addr, (ins.flow or "reads") + " [slot]"))
    return out


def xrefs(module: str, addr: str) -> dict:
    """What refers to an address or function, and what a function calls: the call and jump sites
    that go there, the code that reads, writes or takes the address, the data that holds it, and,
    for a function, each call it makes. `addr` may also be an import, as MODULE!name, its name
    alone, or MODULE!#ordinal: then the code that calls it, directly or through a thunk."""
    from codebreaker.analyze import call_graph
    p = _path(module)
    img = _image(p)
    g = call_graph.build(img)
    index = img.memo("mcp_xrefs", _xref_index)
    lab = lambda v: _label(img, v)
    try:
        targets = [_place(img, addr)]
    except ValueError:
        low = addr.strip().lower()
        targets = [s for s, nm in sorted(img.imports.items()) if low in (nm.lower(), nm.split("!")[-1].lower(),
                   _import_name(nm).lower(), _import_name(nm).split("!")[-1].split(" (")[0].lower())]
        if not targets:
            raise
    out = {"module": img.name, "addr": " ".join("%x" % t for t in targets)}
    va = targets[0]
    if va in img.imports:
        out["import"] = _import_name(img.imports[va])
        if img.bits == 32:                     # the thunks that jump to it (jmp [slot]), function or not
            pattern = b"\xff\x25" + va.to_bytes(4, "little")
            for base, data, executable, _w, _n, _held in img.spans:
                at = data.find(pattern) if executable else -1
                while at >= 0:
                    targets.append(base + at)
                    at = data.find(pattern, at + 1)
            if len(targets) > 1:
                out["thunks"] = [lab(t) for t in targets[1:]]
    elif va in img.names:
        out["function"] = img.names[va]
    elif img.owner(va) is not None:
        out["inside"] = lab(va)
    if va in img.exports:
        out["exported_as"] = img.exports[va]
    t = _thunk(img, va) if va not in img.imports else None
    if t:
        out["is"] = "a thunk: jmp [%x] = %s" % (t[0], _import_name(t[1]))
    sites = sorted({(f, s, how) for t in targets for f, s, how in index.get(t, ())}, key=lambda x: x[1])
    out["from_code"] = [{"site": "%x" % s, "in": lab(f), "how": how} for f, s, how in sites][:500]
    refs = [(f, ins, how) for f, ins, a, how in g.refs if a in targets]
    out["referred"] = [{"site": "%x" % ins.addr, "in": lab(f), "how": {"r": "reads", "w": "writes", "rw": "reads and writes",
                                                                         "a": "takes the address"}[how], "ins": ins.text}
                       for f, ins, how in refs][:500]
    out["in_data"] = ["%x" % site for kind, _f, site in g.taken.get(va, []) if kind == "data"][:200]
    if va in img.names:                        # every call, and every jump that leaves the function
        from codebreaker.analyze import image
        fn = img.function(va)
        out["calls"] = []
        for ins in fn.insns:
            if ins.flow not in ("call", "jmp"):
                continue
            d = image.destination(img, ins)
            if ins.flow == "jmp" and (d is None and any(t["site"] == ins.addr for t in fn.tables)
                                      or d is not None and d not in img.imports and fn.start <= d < fn.limit):
                continue
            out["calls"].append({"site": "%x" % ins.addr, "kind": "call" if ins.flow == "call" else "tail",
                                 "to": "(decided at run time)" if d is None else _import_name(img.imports[d])
                                 if d in img.imports else lab(d)})
    if not (sites or refs or out["in_data"]):
        out["say"] = ("nothing in the module refers to it: it is entered from outside (an export)"
                      if va in img.exports or va in img.roots else "no code or data here refers to it")
    return _fit(out)


def triage(module: str) -> dict:
    """A module at a glance: what it is, its exports and imports, what made it, where the function
    store stands (counts, and each unverified function with its verdict's detail), the functions
    a tool or runtime library made, and where its authored source lives. What to look at next."""
    p = _path(module)
    name = os.path.basename(p)
    d = describe(p)
    out = {"module": name, "path": p, "is": {k: d.get(k) for k in ("found", "platform", "lane", "family", "size")},
           "summary": d.get("summary")}
    try:
        info = _hex_addresses(_quiet(inspect.describe, p))
        ex = (info.get("exports") or {}).get("symbols") or []
        out["exports"] = [s.get("name") or "#%s" % s.get("ordinal") for s in ex][:200]
        out["imports"] = {i["module"]: [s.get("name") or "#%s" % s.get("ordinal") for s in i.get("symbols", [])][:100]
                          for i in info.get("imports") or []}
    except Exception as e:
        out["inspect"] = "%s: %s" % (type(e).__name__, e)
    try:
        ident = identify(p)
        libs = {}
        for r in ident["rows"]:
            libs[r["library"]] = libs.get(r["library"], 0) + 1
        out["made_by"] = {"compiler": ident.get("compiler"), "identified": libs}
    except Exception as e:
        out["made_by"] = "%s: %s" % (type(e).__name__, e)
    try:
        fns = functions(p, limit=5000)
        out["store"] = {k: fns[k] for k in ("counts", "reasons")}
        bad = [r for r in fns["rows"] if r["class"] in (V.UNVERIFIED, V.WEAK, V.NO_COMPILE, V.NO_SOURCE)
               and not r.get("library")]
        out["to_look_at"] = []
        for r in bad[:12]:
            one = function(p, r["addr"])
            out["to_look_at"].append({"addr": r["addr"], "name": r["name"], "class": r["class"], "why": r["why"],
                                      "detail": one.get("detail", ""), "source": (one.get("source") or "")[:1500]})
        if len(bad) > 12:
            out["more_to_look_at"] = len(bad) - 12
        if not fns.get("in_store"):
            out["store"]["say"] = fns.get("say")
    except ValueError as e:
        out["store"] = str(e)
    try:
        st = source_tree(p)
        out["source"] = {"folder": st["folder"], "exists": st["exists"], "counts": st["counts"],
                         "notes": [f["file"] for f in st["files"] if f["kind"] == "notes"][:20],
                         "source": [f["file"] for f in st["files"] if f["kind"] == "source"][:40]}
    except Exception as e:
        out["source"] = "%s: %s" % (type(e).__name__, e)
    out["next"] = ("disasm and xrefs at each function to look at; read_file the notes; compare_versions "
                   "against another build of it")
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
        raise ValueError("%r is an interface or a server; it is not run as a job" % command)
    label = next((os.path.basename(os.path.normpath(a)) for a in args if not a.startswith("-")), "")
    return service.JOBS.launch(command, args, label.lower())


def jobs() -> dict:
    """The jobs started, newest first: with the service running, those of every interface (the
    terminal's, each MCP server's), each with its owner; else this server's."""
    return {"jobs": service.JOBS.all()}


def job(job_id: str, offset: int = 0, tail: int = 0) -> dict:
    """A job's state and its output from byte `offset` (up to 256 KB; `next` is where to go on).
    `tail`: just the last that many characters of the output instead."""
    j = service.JOBS.get(job_id)
    if not j:
        raise ValueError("no job %r (this server's jobs: `jobs`)" % job_id)
    out = service.JOBS.view(j, 0 if tail else int(offset))
    if tail:
        out["log"] = out["log"][-max(1, min(int(tail), MOST)):]
    elif len(out["log"]) > MOST:
        out["next"] = int(offset) + len(out["log"][:MOST].encode("utf-8"))
        out["log"] = out["log"][:MOST]
    return out


def job_file(job_id: str, offset: int = 0, length: int = MOST) -> dict:
    """The file a job wrote (a decompile's C), from character `offset`."""
    j = service.JOBS.get(job_id)
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
    if not service.JOBS.stop(job_id):
        raise ValueError("no job %r" % job_id)
    return service.JOBS.view(service.JOBS.get(job_id))


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
        shared = fs.modules_with_body(digest)
    return {"named": name, "body": digest, "modules_with_this_body": shared}


# ---- the server ------------------------------------------------------------------------------
READING = (describe, inspect_module, strings, hex_dump, resource, modules, coverage, progress,
           functions, function, names, identify, demangle, commands, command_help, jobs, job, job_file,
           reach_results, projects, project, source_tree, read_file, decompile_function, compare_versions,
           diff_function, disasm, xrefs, triage)
TOOL_NAMES = {inspect_module: "inspect", demangle: "demangle"}
TOOLS = READING + (run_command, stop_job, name_function)    # what the service answers as mcp.<name>


def _forward(fn):
    """`fn` answered by the service when one runs (ui/service.call: one process and one answer
    for every client), else here. A job this process started itself (no service then) stays here."""
    sig = signature(fn)

    @functools.wraps(fn)
    def tool(*args, **kw):
        given = sig.bind(*args, **kw).arguments
        if "job_id" in given and service.JOBS.get(given["job_id"]):
            return fn(**given)
        return service.call("mcp." + fn.__name__, **given)
    return tool


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


def build(writes=True):
    """The MCP server with every tool, the function resource and the triage prompt; without
    `writes`, the reading tools only (nothing that starts a process or changes the store)."""
    from mcp.server.mcpserver import MCPServer
    from mcp_types import ToolAnnotations
    app = MCPServer("codebreaker", title="CodeBreaker", instructions=INSTRUCTIONS,
                    version=_version(), log_level="WARNING")
    for fn in READING:
        app.tool(name=TOOL_NAMES.get(fn), annotations=ToolAnnotations(read_only_hint=True))(_said(_forward(fn)))
    if writes:
        app.tool(annotations=ToolAnnotations(read_only_hint=False, destructive_hint=True, open_world_hint=False))(_said(_forward(run_command)))
        app.tool(annotations=ToolAnnotations(read_only_hint=False, destructive_hint=True))(_said(_forward(stop_job)))
        app.tool(annotations=ToolAnnotations(read_only_hint=False, destructive_hint=False, idempotent_hint=True))(_said(_forward(name_function)))

    @app.resource("codebreaker://function/{module}/{addr}", mime_type="text/x-c",
                  description="One function's reconstructed source, as the store holds it")
    def function_source(module: str, addr: str) -> str:
        return function(module, addr).get("source") or "/* no source: see the verdict (the `function` tool) */"

    @app.prompt(name="triage", description="Take a binary apart: what it is, what is in it, where its reconstruction stands")
    def triage_prompt(path: str) -> str:
        return ("Triage %s with the CodeBreaker tools: start with the `triage` tool (what it is, its imports "
                "and exports, the store's counts and each unverified function with its verdict); if nothing "
                "is stored, offer to `run_command` measure on it and follow the job; then for each function to "
                "look at, `disasm` and `xrefs` it and say what most likely went wrong and what the original "
                "code was for." % path)
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
    """ASGI: every HTTP request must carry the token; anything else gets 401 and goes no further.
    With `in_path` the token may instead be the URL's first segment (/<token>/mcp), for clients
    that cannot send a header."""

    def __init__(self, app, secret, in_path=False):
        import hmac
        self.app, self.secret, self.same = app, ("Bearer " + secret).encode(), hmac.compare_digest
        self.path = "/" + secret if in_path else ""

    async def __call__(self, scope, receive, send):
        if scope["type"] == "http":
            got = dict(scope.get("headers") or []).get(b"authorization", b"")
            if self.path and scope.get("path", "").startswith(self.path + "/"):
                rest = scope["path"][len(self.path):]
                scope = dict(scope, path=rest, raw_path=rest.encode())
                got = self.secret
            if not self.same(got, self.secret):
                await send({"type": "http.response.start", "status": 401,
                            "headers": [(b"content-type", b"text/plain"), (b"www-authenticate", b"Bearer")]})
                await send({"type": "http.response.body", "body": b"a bearer token is needed"})
                return
        await self.app(scope, receive, send)


def http_app(hosts=(), public=False):
    """The server as an ASGI app (streamable HTTP at /mcp) behind the token. `hosts` are the
    extra Host names it answers to (a tunnel's), beside 127.0.0.1 and localhost. `public`: for
    the internet, the reading tools only, and the token also accepted in the path."""
    from mcp.server.transport_security import TransportSecuritySettings
    allowed = ["127.0.0.1:*", "localhost:*"] + list(hosts)
    security = TransportSecuritySettings(enable_dns_rebinding_protection=True, allowed_hosts=allowed,
                                         allowed_origins=["http://127.0.0.1:*", "http://localhost:*"] +
                                         ["https://" + h for h in hosts])
    return Bearer(build(writes=not public).streamable_http_app(transport_security=security), token(), in_path=public)


def main(argv):
    """python -m codebreaker mcp                                    stdio (what Claude Code starts)
    python -m codebreaker mcp --http PORT [--host H] [--allow-host NAME ...] [--public]
        streamable HTTP at http://H:PORT/mcp (H: 127.0.0.1), every request needing the bearer
        token (CB_MCP_TOKEN, else state/mcp/http_token.txt); --allow-host: a Host name besides
        127.0.0.1 and localhost to answer to, such as a tunnel's; --public: the reading tools only,
        and the token also accepted as the URL's first segment (/<token>/mcp), for clients that
        cannot send a header"""
    service.START = True                                         # the service, started on demand
    threading.Thread(target=service.client, daemon=True).start()   # (found or started while the client connects)
    if "--http" in argv:
        import uvicorn
        port = int(argv[argv.index("--http") + 1])
        host = argv[argv.index("--host") + 1] if "--host" in argv else "127.0.0.1"
        hosts = [argv[i + 1] for i, a in enumerate(argv) if a == "--allow-host"]
        public = "--public" in argv
        token()
        print("CodeBreaker MCP on http://%s:%d/mcp%s -- token in %s" % (
            host, port, " (public: reading tools only, also at /<token>/mcp)" if public else "",
            "CB_MCP_TOKEN" if os.environ.get("CB_MCP_TOKEN") else settings.state("mcp", "http_token.txt")),
            file=sys.stderr, flush=True)
        uvicorn.run(http_app(hosts, public), host=host, port=port, log_level="warning")
    else:
        build().run("stdio")
    service.stop_mine()                                          # (only the jobs this server started)
    return 0
