r"""service.py -- what the interfaces read and start: one query layer under the terminal interface
(ui/terminal.py) and the MCP server (ui/mcp_server.py), so the two never disagree.

    stored_path(name)          where a module not in the corpus (a Java program's folder) came from
    functions(module[, addr])  what the store holds for a module, or for one function of it
    JOBS                       commands started from an interface, each its own process

A job is the same command a person would type (`python -m codebreaker decompile M`, `... measure M`),
run below normal priority with its output kept in state/ui_jobs/.
"""
import os
import subprocess
import sys
import threading
import time

from codebreaker.base import corpus, settings
from codebreaker.coverage import ledger
from codebreaker.source import module_writer
from codebreaker.store import functions as F

ACTIONS = {"decompile": "decompile it to C (nothing is built or judged)",
           "measure": "decompile, build and judge every function; results go to the store",
           "module": "a folder of Java source: transpile it to C++, compile every class, write the folder"}


def stored_path(name):
    """The path a module not in the corpus (a Java program's folder) was reconstructed from."""
    fs = ledger.open_store()
    row = fs and fs.db.execute("SELECT path FROM module WHERE name=? ORDER BY ts DESC LIMIT 1", (name,)).fetchone()
    return row[0] if row and row[0] and os.path.exists(row[0]) else None


class Jobs(object):
    """Commands started from the terminal interface or the MCP server, each its own process with its output in a file."""

    def __init__(self):
        self.lock = threading.Lock()
        self.jobs = {}                                           # id -> dict
        self.count = 0

    def start(self, action, module):
        """Start `action` on a module. -> the job, or raises ValueError saying why not."""
        path = (module if os.path.isdir(module) else corpus.retail(module) or stored_path(module)) if module else None
        if action not in ACTIONS:
            raise ValueError("no such action %r" % action)
        if not path:
            raise ValueError("no module %r in the corpus" % module)
        name = os.path.basename(os.path.normpath(path)).lower()
        output = os.path.join(settings.state("ui_jobs"), os.path.splitext(name)[0] + ".c") if action == "decompile" else ""
        return self.launch(action, [path] + (["--out", output] if output else []), name, output)

    def launch(self, action, args, name, output=""):
        """Run `python -m codebreaker <action> <args>` as a job (any command of __main__.COMMANDS).
        -> the job, or raises ValueError("busy: ...") while two jobs run, or a measure would be
        the second. `output` is a file the command is told to write, shown once it is done."""
        with self.lock:
            running = [j for j in self.jobs.values() if j["proc"].poll() is None]
            if len(running) >= 2 or any(j["action"] == "measure" for j in running if action == "measure"):
                raise ValueError("busy: %s is still running" % ", ".join(
                    "%s %s" % (j["action"], j["module"]) for j in running))
            self.count += 1
            jid = "%s-%d" % (time.strftime("%H%M%S"), self.count)
            folder = settings.state("ui_jobs")
            os.makedirs(folder, exist_ok=True)
            log = os.path.join(folder, jid + ".log")
            argv = [sys.executable, "-u", "-m", "codebreaker", action] + list(args)
            with open(log, "wb") as f:
                proc = subprocess.Popen(argv, cwd=settings.ROOT, stdout=f, stderr=subprocess.STDOUT,
                                        stdin=subprocess.DEVNULL,
                                        creationflags=0x00004000 if os.name == "nt" else 0)   # below normal
            self.jobs[jid] = {"id": jid, "action": action, "module": name, "proc": proc, "log": log,
                              "output": output, "started": time.time(), "ended": None}
            return self.view(self.jobs[jid])

    def view(self, job, offset=None):
        """A job as plain data; with `offset`, also its output from that byte on."""
        code = job["proc"].poll()
        if code is not None and job["ended"] is None:
            job["ended"] = time.time()
        out = {"id": job["id"], "action": job["action"], "module": job["module"], "pid": job["proc"].pid,
               "status": "running" if code is None else "done" if code == 0 else "failed", "exit": code,
               "seconds": int((job["ended"] or time.time()) - job["started"]),
               "started": time.strftime("%H:%M:%S", time.localtime(job["started"])),
               "has_output": bool(job["output"]) and code == 0 and os.path.isfile(job["output"])}
        if offset is not None:
            try:
                with open(job["log"], "rb") as f:
                    f.seek(max(0, offset))
                    chunk = f.read(1 << 18)
            except OSError:
                chunk = b""
            out["log"], out["next"] = chunk.decode("utf-8", "replace"), max(0, offset) + len(chunk)
        return out

    def all(self):
        with self.lock:
            return [self.view(j) for j in sorted(self.jobs.values(), key=lambda j: -j["started"])]

    def get(self, jid):
        return self.jobs.get(jid)

    def stop(self, jid):
        job = self.jobs.get(jid)
        if job and job["proc"].poll() is None:
            job["proc"].terminate()                              # only a process started here
        return bool(job)

    def stop_all(self):
        for jid in list(self.jobs):
            self.stop(jid)


JOBS = Jobs()


def _row(row, addr):
    """One function for the page: the store's best row for it, in words."""
    if not row:
        return {"addr": addr, "name": "", "class": "no-source", "title": F.TITLE["no-source"],
                "why": "no result under the verifier settings in force", "engine": "", "has_source": False}
    return {"addr": addr, "name": row["name"] or "", "class": row["class"], "title": F.TITLE[row["class"]],
            "why": module_writer.describe(row), "engine": row["engine"], "has_source": bool(row["source"])}


def functions(module, addr=None):
    """What the store holds for a module: {module, kind, sha, profile, in_store, counts, reasons,
    functions: [row]}; with `addr`, {..., function: that row with source, verdict, reason, limits,
    detail and its weak reasons spelled out}."""
    fs = ledger.open_store()
    try:
        led = ledger.measure(fs, module=module)
        if not led["modules"]:
            return None
        m = led["modules"][0]
        digest = led["profiles"].get(m["kind"])
        out = {"module": m["name"], "kind": m["kind"], "kind_title": ledger.TITLE[m["kind"]], "sha": m.get("sha", ""),
               "profile": digest, "in_store": m["in_store"], "measured": m["measured"],
               "counts": {k: m[k] for k in ledger.COUNTS}, "functions": [], "reasons": {}}
        if not m["in_store"]:
            return out
        best = fs.best(m["sha"], digest) if digest else {}
        if addr is not None:
            row = best.get(addr)
            one = _row(row, addr)
            if row:
                one.update(source=row["source"] or "", verdict=row["verdict"], reason=row["reason"] or "",
                           limits=row["limits"] or "", detail=row["detail"] or "", rev=row["rev"] or "",
                           problem=row["problem"] or "",
                           weak=[{"code": c, "text": F.WEAK[c]} for c in row["weak"]])
            out["function"] = one
            return out
        out["functions"] = [_row(best.get(a), a) for a in fs.addresses(m["sha"])]
        if digest:
            out["reasons"] = {k: [[why, n] for why, n in v] for k, v in ledger.reasons(fs, m["sha"], digest).items()}
        return out
    finally:
        if fs:
            fs.close()
