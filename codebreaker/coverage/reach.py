r"""reach.py -- throw anything at CodeBreaker and see how far each program gets.

    python -m codebreaker reach <file or folder> ... [--per-platform N] [--judge] [--timeout S]
                                                     [--containers N] [--name RUN] [--again]
    python -m codebreaker reach --report [RUN]       the last results again, without running anything

Every file given -- and every file in every folder given, and the programs inside every disc
image, Xbox 360 package or Games-on-Demand folder found -- is taken through these steps, as far
as it goes:

    found        what the file is (formats/detect.py), or "unknown"
    platform     the platform it belongs to (codebreaker/platforms), or the platform CodeBreaker
                 has no lane for yet, named ("Game Boy Advance (ARM7)": a gap, counted)
    judged       with --judge: the platform's lane reconstructs it and runs its functions beside
                 the original's (`measure`, in a process of its own, stopped after --timeout
                 seconds); the function store's tally of what came of it

Load stays low on purpose: one program at a time, at most --per-platform programs of each
platform (default 5: spread evenly across their sizes, from the smallest to the largest under
--max-size KB, default 4096), at most --containers programs taken out of each container
(default 20). Results are kept in state/reach/<run>.json by each file's content hash and the
tools' revision, so a program already measured with the same tools is not measured again
(--again measures it anyway). The report is written beside them, <run>.md.
"""
import collections
import hashlib
import json
import os
import subprocess
import sys
import time

# what is worth looking inside a container for: executables and cartridge images
_PROGRAM_EXTS = (".xex", ".exe", ".dll", ".xbe", ".com", ".sys", ".drv", ".vxd", ".386", ".nes", ".gb",
                 ".gbc", ".sms", ".gg", ".sg", ".md", ".gen", ".smd", ".bin", ".rom", ".sna", ".z80",
                 ".tap", ".jar", ".gba", ".nds", ".z64", ".n64", ".v64", ".sfc", ".smc", ".elf", ".self",
                 ".apk", ".ipa", ".xap")
# not programs: never measured, only counted
_SKIP_FAMILIES = ("media", "xna")
_CONTAINER_FORMATS = ("STFS package", "CD image", "Xbox disc image", "disk image", "compressed disc image")


# ---- what a file is, where detect.py has nothing to say ----------------------------------------
def _gap(path, head):
    """(title, cpu) for a program of a platform CodeBreaker has no lane for, else None."""
    ext = os.path.splitext(path)[1].lower()
    if head[:8] == b"PS-X EXE":
        return "PlayStation (PS1)", "MIPS R3000"
    if head[:4] in (b"\x80\x37\x12\x40", b"\x37\x80\x40\x12", b"\x40\x12\x37\x80"):
        return "Nintendo 64", "MIPS R4300"
    if len(head) > 0xB3 and head[0xB2] == 0x96 and head[3] == 0xEA:
        return "Game Boy Advance", "ARM7TDMI"
    if ext == ".nds":
        return "Nintendo DS", "ARM9 / ARM7"
    if ext in (".sfc", ".smc"):
        return "Super Nintendo", "65816"
    if head[:4] == b"\x7fELF" and len(head) >= 20:
        machine = int.from_bytes(head[18:20], "little" if head[5] == 1 else "big")
        cpu = {0x08: "MIPS", 0x14: "PowerPC", 0x15: "PowerPC 64", 0x28: "ARM", 0x3E: "x86-64", 0x03: "x86",
               0xB7: "AArch64", 0x2A: "SuperH", 0x17: "Cell SPU"}.get(machine, "machine 0x%X" % machine)
        return "ELF program", cpu
    if head[:2] == b"PK" and ext == ".apk":
        return "Android app", "Dalvik"
    return None


def _dotnet(path):
    """True for a .NET assembly (a PE with a CLI header, data directory 14): the `xna` lane's,
    not the x86 one's."""
    import struct
    try:
        with open(path, "rb") as f:
            head = f.read(0x1000)
        e = struct.unpack_from("<I", head, 0x3C)[0]
        opt = e + 24
        magic = struct.unpack_from("<H", head, opt)[0]
        dirs = opt + (112 if magic == 0x20B else 96)
        count = struct.unpack_from("<I", head, dirs - 4)[0]
        return count > 14 and struct.unpack_from("<II", head, dirs + 14 * 8)[1] > 0
    except (OSError, struct.error):
        return False


def describe(path):
    """{"found", "platform", "lane"} for one file: what it is, whose it is, and the lane's name
    (None when CodeBreaker has none for it yet)."""
    from codebreaker import platforms
    from codebreaker.formats import detect
    found = (detect.identify(path) or [{}])[0]
    fmt = found.get("format", "")
    try:
        with open(path, "rb") as f:
            head = f.read(0x200)
    except OSError:
        head = b""
    if fmt.startswith("PE") and found.get("family") == "windows" and _dotnet(path):
        return {"found": ".NET assembly", "platform": ".NET (CIL)", "lane": "xna", "family": "xna"}
    big = os.path.getsize(path) > 256 << 20 and not fmt               # (data: not read further)
    p = None if big else platforms.identify(path)
    if p is not None:
        return {"found": fmt or p.container, "platform": p.title, "lane": p.name if p.lane else None,
                "family": p.family}
    gap = _gap(path, head)
    if gap:
        return {"found": fmt or gap[0], "platform": "%s (%s)" % gap, "lane": None, "family": "gap"}
    return {"found": fmt or "unknown", "platform": "", "lane": None, "family": found.get("family", "")}


# ---- finding the programs ------------------------------------------------------------------------
def _sha(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for block in iter(lambda: f.read(1 << 20), b""):
            h.update(block)
    return h.hexdigest()


def _unpack(path, limit, log):
    """Programs inside a container (disc, package, GoD folder), written under state/reach/unpacked:
    -> [paths]. Smallest first, at most `limit`."""
    from codebreaker.base import settings
    from codebreaker.formats import media
    try:
        vols = media.volumes(path)
    except Exception as e:                                   # noqa: BLE001 (any reader's error)
        log("  %s: not opened (%s)" % (os.path.basename(path), str(e)[:80]))
        return []
    entries = [(v, e) for v in vols for e in v.files()
               if not e.is_dir and e.path.lower().endswith(_PROGRAM_EXTS) and 0 < e.size < 512 << 20]
    entries.sort(key=lambda ve: ve[1].size)
    key = hashlib.sha1(os.path.abspath(path).encode("utf-8", "replace")).hexdigest()[:12]
    out = []
    for v, e in entries[:limit]:
        dest = settings.state("reach", "unpacked", key, *[p for p in e.path.replace("\\", "/").split("/")
                                                          if p not in ("", ".", "..")])
        if not os.path.isfile(dest):
            try:
                data = v.read(e)
            except Exception:                                # noqa: BLE001
                continue
            with open(dest, "wb") as f:
                f.write(data)
        out.append(dest)
    return out


def programs(inputs, containers=20, log=print):
    """[(path, where it came from)] for every file under `inputs`, containers opened."""
    from codebreaker.formats import detect
    out = []

    def one(path, origin):
        found = (detect.identify(path) or [{}])[0].get("format", "")
        inner = _unpack(path, containers, log) if found.startswith(_CONTAINER_FORMATS) else None
        if inner:
            out.extend((p, path) for p in inner)
        else:                                                # (a container with no program: counted)
            out.append((path, origin))

    def god(folder):                                         # a Games-on-Demand folder is a container
        try:
            from codebreaker.formats.xbox import volumes as xbox
            return bool(xbox.volumes(folder))
        except Exception:                                    # noqa: BLE001
            return False

    for root in inputs:
        if os.path.isfile(root):
            one(root, "")
            continue
        for d, dirs, files in os.walk(root):
            if god(d):
                out.extend((inner, d) for inner in _unpack(d, containers, log))
                dirs[:] = []
                continue
            dirs[:] = [x for x in dirs if not x.startswith(".") and x not in ("__pycache__", "node_modules")]
            for name in files:
                one(os.path.join(d, name), root)
    return out


# ---- measuring -----------------------------------------------------------------------------------
def _tools():
    """The tools' revision: results measured by other tools are measured again."""
    try:
        here = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
        return subprocess.run(["git", "rev-parse", "--short", "HEAD"], cwd=here, capture_output=True,
                              text=True, timeout=20).stdout.strip() or "?"
    except (OSError, subprocess.SubprocessError):
        return "?"


def _job(proc):
    """A Windows job that holds `proc` and everything it starts (None elsewhere)."""
    if os.name != "nt":
        return None
    import ctypes
    from ctypes import wintypes
    k = ctypes.WinDLL("kernel32", use_last_error=True)
    k.CreateJobObjectW.restype = wintypes.HANDLE
    job = k.CreateJobObjectW(None, None)

    class Limits(ctypes.Structure):                          # JOBOBJECT_EXTENDED_LIMIT_INFORMATION
        _fields_ = [("a", ctypes.c_int64 * 2), ("flags", wintypes.DWORD), ("b", ctypes.c_size_t * 2),
                    ("c", wintypes.DWORD), ("d", ctypes.c_size_t), ("e", wintypes.DWORD),
                    ("f", wintypes.DWORD), ("io", ctypes.c_uint64 * 6), ("g", ctypes.c_size_t * 4)]
    lim = Limits()
    lim.flags = 0x2000                                       # JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE
    k.SetInformationJobObject(wintypes.HANDLE(job), 9, ctypes.byref(lim), ctypes.sizeof(lim))
    k.AssignProcessToJobObject(wintypes.HANDLE(job), wintypes.HANDLE(int(proc._handle)))
    return (k, job)


def _end(job, proc):
    """Stop `proc` and everything it started."""
    if job:
        from ctypes import wintypes
        k, h = job
        k.TerminateJobObject(wintypes.HANDLE(h), 1)
        k.CloseHandle(wintypes.HANDLE(h))
    elif proc.poll() is None:
        proc.kill()
    proc.wait()


def judge(path, timeout):
    """Run the platform's lane on one program in its own process. -> {"tally", "seconds", "stopped"}"""
    import tempfile
    t0 = time.time()
    stopped = ""
    # Output to a file, not a pipe, and the whole process tree in one job: a stopped `measure`
    # leaves its worker processes behind otherwise, and they hold a pipe (and the run) open.
    with tempfile.TemporaryFile() as out:
        proc = subprocess.Popen([sys.executable, "-m", "codebreaker", "measure", path], stdout=out,
                                stderr=subprocess.STDOUT)
        job = _job(proc)
        try:
            code = proc.wait(timeout=timeout)
        except subprocess.TimeoutExpired:
            code, stopped = None, "timed out after %d s" % timeout
        _end(job, proc)
        if code:
            out.seek(0)
            text = out.read().decode("utf-8", "replace").strip().splitlines()
            stopped = (text or ["exit %d" % code])[-1][:160]
    tally, why = tally_of(path)
    return {"tally": tally, "seconds": round(time.time() - t0, 1), "stopped": stopped or why}


def tally_of(path):
    """(the function store's {class: count} for a program, "" or why there is none)."""
    tally, stopped = {}, ""
    try:
        from codebreaker.base import corpus
        from codebreaker.store import functions as F
        from codebreaker.verify import reconstruct
        sha, digest = reconstruct.identity(corpus.retail(path))   # (filed as `measure` files it: a .xex
                                                                    # under the basefile inside it)
        fs = F.open()
        try:
            tally = fs.tally(digest, sha)
        finally:
            fs.close()
    except Exception as e:                                   # noqa: BLE001
        stopped = "no tally: %s" % str(e)[:120]
    return tally, stopped


def spread(rows, n, max_size):
    """`n` of `rows` evenly across their sizes (the smallest, the largest under `max_size`, and
    between): a sample of a platform, not its easiest programs."""
    rs = sorted((r for r in rows if r["size"] <= max_size), key=lambda r: r["size"])
    if len(rs) <= n:
        return rs
    return [rs[round(i * (len(rs) - 1) / (n - 1))] for i in range(n)] if n > 1 else rs[:1]


def run(inputs, per_platform=5, do_judge=False, timeout=300, containers=20, name="reach", again=False,
        max_size=4 << 20, log=None):
    log = log or (lambda text: print(text, flush=True))
    from codebreaker.base import settings
    store = settings.state("reach", name + ".json")
    old = {}
    if os.path.isfile(store):
        with open(store, encoding="utf-8") as f:
            old = {r["sha"]: r for r in json.load(f).get("programs", [])}
    tools = _tools()
    found = programs(inputs, containers, log)
    log("%d files found" % len(found))
    rows, seen = [], set()
    for path, origin in found:
        try:
            row = dict(describe(path), path=path, origin=origin, size=os.path.getsize(path))
            # only programs are hashed (a game library is mostly data: never read whole)
            row["sha"] = _sha(path) if (row["platform"] or row["family"] == "gap") else "path:" + path
        except OSError:
            continue
        if row["sha"] in seen:
            continue
        seen.add(row["sha"])
        rows.append(row)
    def save():                                              # after every program: a stopped run keeps its results
        kept = rows + [r for r in old.values() if r["sha"] not in seen]   # (and what earlier runs measured)
        with open(store, "w", encoding="utf-8") as f:
            json.dump({"when": time.strftime("%Y-%m-%d %I:%M %p"), "inputs": [os.path.abspath(i) for i in inputs],
                       "tools": tools, "programs": kept}, f, indent=1)

    # which get judged: programs of a platform with a lane, smallest first, per_platform of each
    by_platform = collections.defaultdict(list)
    for r in rows:
        if r["lane"] and r["family"] not in _SKIP_FAMILIES:
            by_platform[r["platform"]].append(r)
    for plat, rs in sorted(by_platform.items()):
        for r in spread(rs, per_platform, max_size):
            r["picked"] = True
            prev = old.get(r["sha"])
            if prev and prev.get("tools") == tools and prev.get("judged") and not again:
                r.update(judged=prev["judged"], tools=tools)
                continue
            if do_judge:
                log("judging %s (%s, %d KB)" % (os.path.basename(r["path"]), plat, r["size"] // 1024))
                r["judged"] = judge(r["path"], timeout)
                r["tools"] = tools
                j = r["judged"]
                log("  %s%s" % (", ".join("%s %d" % kv for kv in j["tally"].items() if kv[1]) or "nothing recorded",
                               "; " + j["stopped"] if j["stopped"] else ""))
                save()
    save()
    text = report(rows + [r for r in old.values() if r["sha"] not in seen])
    with open(settings.state("reach", name + ".md"), "w", encoding="utf-8") as f:
        f.write(text)
    return rows, text


# ---- the report ----------------------------------------------------------------------------------
def report(rows):
    """Markdown: by platform, how many programs, how far they got."""
    progs = [r for r in rows if r["platform"] or r["family"] == "gap"]
    plats = collections.OrderedDict()
    for r in sorted(progs, key=lambda r: (r["lane"] is None, r["platform"])):
        plats.setdefault(r["platform"], []).append(r)
    lines = ["# Reach", "", "What CodeBreaker made of every program it was given (`python -m codebreaker reach`).",
             "Functions and percentages are those of the judged programs that finished; a program stopped at",
             "its time limit is counted under Stopped only.", "",
             "| Platform | Programs | Lane | Judged | Functions | Verified | Weak | Unverified | Stopped |",
             "|---|---|---|---|---|---|---|---|---|"]
    for plat, rs in plats.items():
        judged = [r for r in rs if r.get("judged")]
        t = collections.Counter()
        for r in judged:                                     # (a program stopped at its time limit:
            if not r["judged"]["stopped"].startswith("timed out"):   # its functions never reached
                t.update(r["judged"]["tally"])               # are not failures; counted as stopped)
        total = sum(t.values())
        pct = (lambda n: "%d (%d%%)" % (n, 100 * n // total) if total else "-")
        stopped = sum(1 for r in judged if r["judged"]["stopped"])
        lines.append("| %s | %d | %s | %d | %s | %s | %s | %s | %s |" % (
            plat, len(rs), rs[0]["lane"] or "**none yet**", len(judged), total or "-", pct(t["verified"]),
            pct(t["weak"]), pct(t["unverified"] + t["no-compile"]), stopped or ""))
    other = collections.Counter(r["found"] for r in rows if r not in progs)
    lines += ["", "Not programs, or not recognised: %d files (%s)." % (
        sum(other.values()), ", ".join("%s %d" % kv for kv in other.most_common(8)))]
    stops = [r for r in rows if r.get("judged") and r["judged"]["stopped"]]
    if stops:
        lines += ["", "## Where judging stopped", ""]
        lines += ["- %s (%s): %s" % (os.path.basename(r["path"]), r["platform"], r["judged"]["stopped"]) for r in stops]
    return "\n".join(lines) + "\n"


def main(argv):
    import argparse
    ap = argparse.ArgumentParser(prog="codebreaker reach", description=__doc__.split("\n\n")[0])
    ap.add_argument("inputs", nargs="*")
    ap.add_argument("--per-platform", type=int, default=5)
    ap.add_argument("--judge", action="store_true")
    ap.add_argument("--timeout", type=int, default=300)
    ap.add_argument("--containers", type=int, default=20)
    ap.add_argument("--name", default="reach")
    ap.add_argument("--again", action="store_true")
    ap.add_argument("--max-size", type=int, default=4096, help="KB: larger programs are not judged")
    ap.add_argument("--report", nargs="?", const="reach")
    a = ap.parse_args(argv)
    if a.report:
        from codebreaker.base import settings
        with open(settings.state("reach", a.report + ".md"), encoding="utf-8") as f:
            print(f.read())
        return 0
    if not a.inputs:
        ap.print_help()
        return 2
    _, text = run(a.inputs, a.per_platform, a.judge, a.timeout, a.containers, a.name, a.again,
                  a.max_size * 1024)
    print(text)
    return 0
