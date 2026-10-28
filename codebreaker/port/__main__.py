"""python -m codebreaker.port SOURCE_DIR OUT_DIR [--compiler gcc|clang|msvc] [--runtime NAME] [--cc PATH]

SOURCE_DIR is read, never written: its C (*.c, *.h, in it or its src/) and its assembler modules (*.asm, in it or
its asm/). OUT_DIR gets the port: the modern sources, the platform layer, a build script, the program built, and
PORT.md - every change made and why, what the compiler still says, what could not be ported.

  --cases CASES.json   the behaviour cases (port/verify): the sanitizer pass runs them
  --asm16 ASSEMBLER    the period assembler (run under the emulator) whose listing places the assembly's tables
  --types TYPES.json   {"members": {"struct.member": "type"}}: members the source declared otherwise than the
                       program uses them (what the assembly shows) - the port's corrections, said in PORT.md
"""
import collections
import glob
import json
import os
import re
import shutil
import subprocess
import sys
import time

from codebreaker.base import cache
from codebreaker.port import pre_ansi

HERE = os.path.dirname(os.path.abspath(__file__))


def load_compilers():
    c = json.load(open(os.path.join(HERE, "compilers.json")))
    try:                                                    # (the user's own, from settings.json)
        s = json.load(open(os.path.join(HERE, "..", "..", "settings.json")))
        for k, v in s.get("port", {}).get("compilers", {}).items():
            c.setdefault(k, {}).update(v)
    except (OSError, ValueError):
        pass
    return c


def find(names, given=None):
    for c in ([given] if given else []) + names:
        p = shutil.which(c) or next(iter(sorted(glob.glob(c), reverse=True)), None)
        if p:
            return p
    return None


def env_for(cc):
    e = dict(os.environ)                                    # (a compiler's own DLLs beside it, on the path)
    e["PATH"] = os.path.dirname(cc) + os.pathsep + e.get("PATH", "")
    return e


def run(spec, argv, cwd, cc):
    """a compiler's command, after its setup script (MSVC's vcvars) in the same shell"""
    setup = find(spec.get("setup", [])) if spec.get("setup") else None
    if setup:
        argv = ["cmd", "/d", "/c", "call", setup, ">nul", "&&"] + argv
    return subprocess.run(argv, capture_output=True, text=True, env=env_for(cc), cwd=cwd, errors="replace")


def args(tpl, **kw):
    out = []
    for a in tpl:
        if a == "{srcs}":
            out += kw["srcs"]
        else:
            out.append(a.format(**{k: v for k, v in kw.items() if k != "srcs"}))
    return out


def diagnostics(spec, text, folder):
    out = []
    for l in text.splitlines():
        n = re.match(r"^(?P<file>[^:\n]+?(?::[\\/][^:\n]+)?):(?P<line>\d+):(?P<col>\d+): note: (?P<text>.*)$", l.strip())
        if n and out:                                       # (a note: what the complaint before it expected)
            out[-1].setdefault("notes", []).append(n.group("text"))
            if "in expansion of macro" in n.group("text"):   # (the outermost use: the last such note)
                out[-1].setdefault("macro_at", (out[-1]["file"], out[-1]["line"], out[-1]["col"]))   # (where it is in the
                out[-1]["file"] = os.path.basename(n.group("file"))                       #  source: the macro's
                out[-1]["line"], out[-1]["col"] = int(n.group("line")), int(n.group("col"))   # use)
            continue
        m = re.match(spec["diag"], l.strip())
        if m:
            f = os.path.basename(m.group("file"))
            out.append({"file": f, "line": int(m.group("line")), "col": int(m.group("col") or 0),
                        "kind": m.group("kind"), "text": m.group("text")})
    return out


def read_tree(src):
    files, asm, paths = {}, {}, {}
    for d in (src, os.path.join(src, "src")):
        for p in sorted(glob.glob(os.path.join(d, "*.[ch]"))):
            files[os.path.basename(p)] = open(p, encoding="latin1").read()
    for d in (src, os.path.join(src, "asm"), os.path.join(src, "src")):
        for p in sorted(glob.glob(os.path.join(d, "*.asm"))):
            asm[os.path.basename(p)] = open(p, encoding="latin1").read()
            paths[os.path.basename(p)] = p
    return files, asm, paths


def memory_used():
    """the bytes this process has committed (Windows: its private bytes; elsewhere: its resident set) - or None"""
    try:
        if os.name == "nt":
            import ctypes
            import ctypes.wintypes as w

            class PMC(ctypes.Structure):
                _fields_ = [("cb", w.DWORD), ("PageFaultCount", w.DWORD)] + \
                    [(n, ctypes.c_size_t) for n in ("PeakWorkingSetSize", "WorkingSetSize", "QuotaPeakPagedPoolUsage",
                                                    "QuotaPagedPoolUsage", "QuotaPeakNonPagedPoolUsage",
                                                    "QuotaNonPagedPoolUsage", "PagefileUsage", "PeakPagefileUsage")]
            c = PMC()
            c.cb = ctypes.sizeof(c)
            k32, psapi = ctypes.WinDLL("kernel32"), ctypes.WinDLL("psapi")
            k32.GetCurrentProcess.restype = w.HANDLE        # (a handle is 64 bits: so declared)
            psapi.GetProcessMemoryInfo.argtypes = [w.HANDLE, ctypes.c_void_p, w.DWORD]
            if psapi.GetProcessMemoryInfo(k32.GetCurrentProcess(), ctypes.byref(c), c.cb):
                return c.PagefileUsage
            return None
        with open("/proc/self/statm") as f:
            return int(f.read().split()[1]) * os.sysconf("SC_PAGE_SIZE")
    except Exception:
        return None


def memory_guard(out):
    """a watch on the port's own memory: above the ceiling (settings.json port.max_memory_gb, 4 by default) it
    stops - where it was written to OUT/port-memory.txt - before it can starve the machine (a rule that runs away)"""
    import threading
    import time
    from codebreaker.base import settings
    conf = (settings.get("port") or {}) if hasattr(settings, "get") else {}
    limit = float(conf.get("max_memory_gb", 4)) * 1024 ** 3
    if memory_used() is None:
        return

    def watch():
        while True:
            used = memory_used() or 0
            if used > limit:
                import faulthandler
                try:
                    os.makedirs(out, exist_ok=True)
                    with open(os.path.join(out, "port-memory.txt"), "w") as f:
                        f.write("the port stopped itself: %.1f GB used, the ceiling %.1f GB (settings.json "
                                "port.max_memory_gb) - where it was:\n\n" % (used / 1024 ** 3, limit / 1024 ** 3))
                        f.flush()
                        faulthandler.dump_traceback(file=f, all_threads=True)
                except OSError:
                    pass
                sys.stderr.write("port: stopped - %.1f GB of memory, above the ceiling of %.1f GB (where it was: "
                                 "%s)\n" % (used / 1024 ** 3, limit / 1024 ** 3, os.path.join(out, "port-memory.txt")))
                sys.stderr.flush()
                os._exit(3)
            time.sleep(1)
    threading.Thread(target=watch, daemon=True, name="memory-guard").start()


def check_key(spec, cc, n, res):
    """what one file's check depends on: the compiler and its command, the file, every header it reaches"""
    import hashlib
    h = hashlib.sha256(repr((os.path.abspath(cc), spec.get("check"), n)).encode())
    todo, seen = [n], set()
    while todo:
        x = todo.pop()
        if x in seen:
            continue
        seen.add(x)
        h.update(x.encode() + b"\0" + (res.get(x) or "").encode("latin1", "replace") + b"\0")
        todo += re.findall(r'#\s*include\s*"([^"]+)"', res.get(x) or "")
    return h.hexdigest()


def build(spec, cc, tpl, out, srcs, exe, objdir):
    """the port built a C file at a time, side by side (the workbench's workers, at most four), each file's object
    kept by all it depends on (the compiler and its command, the file, every header it reaches): a file as it was
    the last time not compiled again; then the objects linked. A compiler of another form (MSVC's): all at once"""
    at = tpl.index("{out}") if "{out}" in tpl else -1
    if spec.get("setup") or "{srcs}" not in tpl or at < 1 or tpl[at - 1] != "-o":
        return run(spec, args(tpl, cc=cc, inc=".", out=exe, srcs=srcs), out, cc)
    import hashlib
    import threading
    from concurrent.futures import ThreadPoolExecutor
    from codebreaker.base import parallel
    cargv = args([a for i, a in enumerate(tpl) if i not in (at - 1, at) and a != "{srcs}"], cc=cc, inc=".", out="")
    objdir = os.path.abspath(objdir) + ".%d" % os.getpid()   # (its own: two ports of one folder at once apart)
    os.makedirs(objdir, exist_ok=True)
    texts = {}

    def text(n):
        if n not in texts:
            try:
                texts[n] = open(os.path.join(out, n), "rb").read()
            except OSError:
                texts[n] = None
        return texts[n]

    def key(n):
        h = hashlib.sha256(repr((os.path.abspath(cc), cargv[1:], n)).encode())
        todo, seen = [n], set()
        while todo:
            x = todo.pop()
            if x in seen:
                continue
            seen.add(x)
            t = text(x)
            if t is None:
                continue
            h.update(x.encode() + b"\0" + t + b"\0")
            todo += [m.decode("latin1") for m in re.findall(rb'#\s*include\s*[<"]([^>"]+)[>"]', t)]
        return h.hexdigest()

    cs = [n for n in srcs if n.endswith(".c")]
    keys = {n: key(n) for n in cs}

    def one(n):
        k = keys[n]
        o = cache.path("port-objects", k) + ".o"
        mine = os.path.join(objdir, n[:-2] + ".o")
        if not cache.off() and sound(o):                                        # (as the last time: its object, and what it said then)
            try:
                shutil.copyfile(o, mine)
                os.utime(o)
                with open(o[:-2] + ".txt", encoding="utf-8", errors="replace") as f:
                    return 0, f.read()
            except OSError:
                pass
        p = run(spec, cargv + ["-c", n, "-o", mine], out, cc)
        if p.returncode == 0 and not cache.off() and sound(mine):
            try:
                os.makedirs(os.path.dirname(o), exist_ok=True)
                tmp = o + ".%d.%d.tmp" % (os.getpid(), threading.get_ident())
                shutil.copyfile(mine, tmp)
                with open(o[:-2] + ".txt", "w", encoding="utf-8") as f:
                    f.write(p.stdout + p.stderr)
                os.replace(tmp, o)
            except OSError:
                pass
        return p.returncode, p.stdout + p.stderr

    with ThreadPoolExecutor(max_workers=max(1, min(4, parallel.workers(), len(cs) or 1))) as ex:
        done = list(ex.map(one, cs))
    logs = "".join(t for r, t in done)
    if any(r for r, t in done):
        shutil.rmtree(objdir, ignore_errors=True)
        return subprocess.CompletedProcess([], 1, logs, "")
    here = os.path.abspath(out)
    objs = [os.path.relpath(os.path.join(objdir, n[:-2] + ".o"), here).replace("\\", "/") if n.endswith(".c") else n
            for n in srcs]
    p = run(spec, args(tpl, cc=cc, inc=".", out=exe, srcs=objs), out, cc)
    shutil.rmtree(objdir, ignore_errors=True)
    cache.prune("port-objects", 3 << 30)                     # (the kept objects held under 3 GB)
    return subprocess.CompletedProcess(p.args, p.returncode, logs + p.stdout, p.stderr)


def sound(path):
    """an object file whole: not empty, its first bytes not nothing (a build cut short leaves either)"""
    try:
        with open(path, "rb") as f:
            head = f.read(16)
        return os.path.getsize(path) > 64 and head.strip(b"\0") != b""
    except OSError:
        return False


def changed_files(res, before, cfiles):
    """the C files to check again: those whose text is not what it was, and those that include (themselves or
    through another header) a header whose text is not"""
    if not before:
        return list(cfiles)
    inc = {n: set(re.findall(r'#\s*include\s*"([^"]+)"', t)) for n, t in res.items()}
    changed = {n for n, t in res.items() if before.get(n) != t}
    hdrs = {n for n in changed if not n.endswith(".c")}

    def reaches(n, seen):
        for h in inc.get(n, ()):
            if h in hdrs:
                return True
            if h not in seen:
                seen.add(h)
                if reaches(h, seen):
                    return True
        return False
    return [n for n in cfiles if n in changed or reaches(n, set())]


def read_symbols(src):
    """{C name: its place in the data segment} of a SYMBOLS list (the recon's: name, C or D, address)"""
    for d in (src, os.path.join(src, "src")):
        p = os.path.join(d, "SYMBOLS")
        if os.path.isfile(p):
            out = {}
            for l in open(p, encoding="latin1"):
                w = l.split("#", 1)[0].split()
                if len(w) == 3 and w[1] == "D":
                    try:
                        out[w[0][1:] if w[0].startswith("_") else w[0]] = int(w[2], 16)
                    except ValueError:
                        pass
            return out
    return {}


def read_bss(src):
    """{C name: its bytes} of the variables a BSS list names (the recon's: what the old build defined beginning
    without a value, by DS offset - 'range from to', 'hole at length', 'name offset')"""
    for d in (src, os.path.join(src, "src")):
        p = os.path.join(d, "BSS")
        if not os.path.isfile(p):
            continue
        ents, rng, holes = [], None, []
        for l in open(p, encoding="latin1"):
            w = l.split("#", 1)[0].split()
            if len(w) == 3 and w[0] == "range":
                rng = (int(w[1], 16), int(w[2], 16))
            elif len(w) == 3 and w[0] == "hole":
                holes.append((int(w[1], 16), int(w[2], 16)))
            elif len(w) == 2:
                ents.append((int(w[1], 16), w[0]))
        ents.sort()
        out = {}
        for i, (off, name) in enumerate(ents):
            end = ents[i + 1][0] if i + 1 < len(ents) else (rng[1] if rng else off + 2)
            end = min([h for h, n in holes if off < h < end] + [end])
            out[name[1:] if name.startswith("_") else name] = end - off
        return out
    return {}


def asm_role(text, profile):
    """what an assembler module of the program is: the old runtime's (it makes public what the runtime defines,
    or every name it makes public is the C library's or the platform layer's), data alone (no instruction in a
    code segment), or code of the program - to be ported as assembly language"""
    from codebreaker.port import asm16
    pubs = set()
    for l in text.splitlines():
        m = re.match(r"\s*PUBLIC\s+(.+)", strip_asm_comment(l), re.I)
        if m:
            pubs |= {x.strip() for x in m.group(1).split(",") if x.strip()}
    known = set(profile["libc"]) | set(profile["platform"]["calls"]) | set(profile["runtime_defines"])
    names = {p[1:] if p.startswith("_") else p for p in pubs}
    if names & set(profile["runtime_defines"]) - {"fopen", "fprintf", "malloc"} or "_main" in pubs:
        return "runtime", sorted(pubs)
    if any(re.match(r"\s*END\s+\w+", strip_asm_comment(l), re.I) for l in text.splitlines()) and \
            not any(re.match(r"\s*_main\b", strip_asm_comment(l)) for l in text.splitlines()):
        return "runtime", sorted(pubs)                      # (END start: the start-up code - the C library's now)
    lines, seg, code = asm16.parse(text), None, False
    code_segs = asm16.code_segments(lines)
    for l in lines:
        d = l.directive
        if d and d[0] == "SEGMENT":
            seg = d[1] if d[1] in code_segs else None
        elif d and d[0] == "ENDS":
            seg = None
        elif seg and l.op:
            code = True
    if not code:
        return "data", sorted(pubs)
    own = [p for p in pubs if re.match(r"_[A-Za-z]", p) and p[1:] not in known or
           re.match(r"[A-Z][A-Z0-9_]*$", p) and p.lower() not in known]   # (C names: _x is C's x, CAPS a pascal one;
    return ("code" if own else "runtime"), sorted(pubs)                  #  __x and cret the runtime's)


def strip_asm_comment(l):
    return l.split(";", 1)[0]


NEW_TYPES = {"": "int16_t", "int": "int16_t", "short": "int16_t", "short int": "int16_t", "unsigned": "uint16_t",
             "unsigned int": "uint16_t", "unsigned short": "uint16_t", "long": "int32_t", "long int": "int32_t",
             "unsigned long": "uint32_t", "char": "char", "unsigned char": "unsigned char", "void": "void"}


def c_declared(texts):
    """{name: the type it returns, today's spelling} from the C program's declarations of functions it does not
    define (extern T NEAR f();) - what it expects of the assembly's"""
    from codebreaker.port import asm16
    macros = set()
    for t in texts:
        for m in re.finditer(r"(?m)^\s*#\s*define\s+(\w+)[ \t]+([\w \t]+?)\s*$", t):
            if set(m.group(2).split()) <= asm16.CALL_WORDS:
                macros.add(m.group(1))
    out = {}
    for t in texts:
        for m in re.finditer(r"\bextern\s+([\w\s*]+?)\b([A-Za-z_]\w*)\s*\(\s*\)\s*;", pre_ansi.blank(t)):
            words = [w for w in re.findall(r"\w+|\*", m.group(1)) if w not in asm16.CALL_WORDS and w not in macros]
            stars = words.count("*")
            base = " ".join(w for w in words if w != "*")
            if stars:
                out.setdefault(m.group(2), base + " " + "*" * stars)
            elif base in NEW_TYPES:
                out.setdefault(m.group(2), NEW_TYPES[base])
    return out


NEW_SIZES = {"char": 1, "signed char": 1, "unsigned char": 1, "int8_t": 1, "uint8_t": 1, "int16_t": 2,
             "uint16_t": 2, "short": 2, "int32_t": 4, "uint32_t": 4, "long": 4, "int": 4, "unsigned": 4}


def data_places(asm_path):
    """[(old address in the data segment, name)] of the program's variables, where the recovered source has them
    (a SYMBOLS file beside it: name, D, address - as the original image placed them)"""
    p = os.path.join(os.path.dirname(asm_path), "SYMBOLS")
    out = []
    if os.path.isfile(p):
        for l in open(p, encoding="latin1"):
            w = l.split()
            if len(w) == 3 and w[1] == "D" and re.fullmatch(r"[0-9a-fA-F]+", w[2]):
                out.append((int(w[2], 16), w[0]))
    return sorted(out)


def alias_places(aliases, ct):
    """{alias: (owner, byte offset now)} of the names the old linker gave places inside other variables"""
    out = {}
    for al, (where, od) in aliases.items():
        m = re.fullmatch(r"\(?&?\s*(\w+)\s*(?:\[\s*(\d+)\s*\])?\)?", where.strip())
        if not m:
            continue
        owner, k = m.group(1), int(m.group(2) or 0)
        base = re.sub(r"\b(extern|static)\b", "", (od or "").split(owner)[0]).strip()
        if "*" in (od or "").split(owner)[0] or re.search(r"\(\s*\*", od or ""):
            size = 8
        elif base.startswith(("struct", "union")):
            size = (ct.new.get(re.sub(r"\s+", " ", base)) or {}).get("@size", 1)
        else:
            size = NEW_SIZES.get(re.sub(r"\s+", " ", base), 1)
        out[al] = (owner, k * size)
    return out


def call_arg_types(fn, texts, origs, ct):
    """[(old size, what it points to or None)] of the arguments the C program calls an assembly routine with: a
    pointer to a variable it declares (&g), a pointer variable (p) - far ones 4 bytes, as they were pushed"""
    far = getattr(call_arg_types, "far", None)
    if far is None or call_arg_types.key is not origs:
        far = set()
        for t in origs:
            for d in re.finditer(r"(?m)^[^;(){}]*\bfar\b[^;(){}]*?\b(\w+)\s*(?:\[[^\]]*\])?\s*;", pre_ansi.blank(t)):
                far.add(d.group(1))
        call_arg_types.far, call_arg_types.key = far, origs
    best = []
    for t in texts:
        for line in pre_ansi.blank(t).split("\n"):
            if not re.search(r"\b%s\s*\(" % re.escape(fn), line) or re.match(r"\s*(?:extern\b|\w[\w\s*]*\b%s\s*\([^)]*\)\s*$)"
                                                                           % re.escape(fn), line):
                continue
            spans = pre_ansi.call_args(line, fn)
            if not spans:
                continue
            out = []
            for a, b in spans:
                arg = line[a:b].strip()
                m = re.fullmatch(r"&\s*(\w+)", arg)
                v = ct.var(m.group(1)) if m else None
                if v:
                    stars = re.sub(r"\[[^\]]*\]", "", v[1]).replace(m.group(1), "").count("*")
                    out.append((4 if m.group(1) in far else 2, v[0].strip() + (" " + "*" * stars if stars else "")))
                    continue
                loc = re.search(r"\b((?:struct|union)\s+\w+|char|unsigned\s+char|int16_t|uint16_t)\s*(\*+)\s*%s\s*[;,)=]"
                                % re.escape(arg), pre_ansi.blank(t)) if re.fullmatch(r"\w+", arg) else None
                if loc:                                     # (a variable of the calling file: a parameter, a local)
                    stars = len(loc.group(2))
                    out.append((2, re.sub(r"\s+", " ", loc.group(1)) + (" " + "*" * (stars - 1) if stars > 1 else "")))
                    continue
                if re.fullmatch(r"\w+", arg) and ct.is_pointer(arg):
                    out.append((4 if arg in far else 2, ct.pointee(arg)))
                    continue
                v = ct.var(arg) if re.fullmatch(r"\w+", arg) else None
                lng = re.fullmatch(r"\w+", arg) and re.search(r"\b(?:long|int32_t|uint32_t)\s+(?:[^;(){}]*,\s*)?%s\s*[;,=]"
                                                           % re.escape(arg), pre_ansi.blank(t))
                out.append((4 if (v and re.search(r"\b(long|int32_t|uint32_t)\b", v[0])) or lng or
                            re.fullmatch(r"(?:0[xX][0-9a-fA-F]+|\d+)[lL]", arg) else 2, None))
            if sum(1 for s, to in out if to) > sum(1 for s, to in best if to):
                best = out
    return best


def runtime_tails(mods):
    """{routine: its lines} - the old runtime's routines that are straight lines to a RET (cret: the shared end of
    the routines of Lattice-era C - LEA SP,[BP-4], the POPs, RET): what a jump into one does, there"""
    out = {}
    for n, t in mods.items():
        lines = t.splitlines()
        pubs = set()
        for l in lines:
            m = re.match(r"\s*PUBLIC\s+(.+)", strip_asm_comment(l), re.I)
            if m:
                pubs |= {x.strip() for x in m.group(1).split(",") if x.strip()}
        for i, l in enumerate(lines):
            m = re.match(r"\s*(\w+)(?:\s+PROC\b.*|\s*:\s*)$", strip_asm_comment(l), re.I)
            if not m or m.group(1) not in pubs:
                continue
            body = []
            for l2 in lines[i + 1:]:
                c = strip_asm_comment(l2).strip()
                if not c:
                    continue
                if re.match(r"(?i)(?:\w+\s*:|j\w+\b|call\b|loop\w*\b|\w+\s+(?:PROC|ENDP|ENDS)\b)", c):
                    body = None                             # (not straight: left to the C library's)
                    break
                body.append("\t" + c)
                if re.match(r"(?i)ret\b", c):
                    break
            if body and re.match(r"(?i)\s*ret\b", body[-1]):
                out[m.group(1)] = body
    return out


def inline_tails(text, tails, notes, name):
    """JMP to one of the old runtime's straight routines: its instructions in place of the jump"""
    if not tails:
        return text
    out, n = [], 0
    for l in text.splitlines():
        m = re.match(r"(?i)\s*jmp\s+(?:near\s+ptr\s+|short\s+)?(\w+)\s*$", strip_asm_comment(l))
        if m and m.group(1) in tails:
            out.append("\t; (JMP %s - the old runtime's: its instructions here)" % m.group(1))
            out += tails[m.group(1)]
            n += 1
            continue
        out.append(l)
    if n:
        notes.append("%s: %d jump(s) into the old runtime's own ends (%s): their instructions in place" % (
            name, n, ", ".join(sorted({x for x in tails if re.search(r"(?i)\bjmp\s+%s\b" % x, text)}))))
    return "\n".join(out) + ("\n" if text.endswith("\n") else "")


def asm_modules(port, out, res, asm, paths, roles, profile, opt, compilers):
    """the program's own assembly language, ported as assembly language (port/asm16) and assembled; its data
    alone as C definitions. -> ([objects to link], {name: its C declaration}, [notes])"""
    from codebreaker.port import asm16
    from codebreaker.base import settings
    texts = [t for n, t in res.items() if n.endswith((".c", ".h"))]
    origs = list(port.originals.values())
    ct = asm16.CTypes(texts, origs)
    calls = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", "\n".join(pre_ansi.blank(t) for t in texts)))
    far = set(re.findall(r"\bfar\b[^;(){}]*?\b(\w+)\s*\(", "\n".join(pre_ansi.blank(t) for t in origs)))
    old_asm = opt("--asm16") or (settings.get("port") or {}).get("asm16") if hasattr(settings, "get") else None
    ml = find(compilers.get("ml64", {}).get("find", []))
    objs, protos, notes = [], {}, []
    cfuncs = asm16.c_functions(origs)
    declared = c_declared(origs)
    tails = runtime_tails({n: t for n, t in asm.items() if roles[n][0] == "runtime"})
    asm = {n: (inline_tails(t, tails, notes, n) if roles[n][0] == "code" else t) for n, t in asm.items()}
    routines, externs = {}, {}                              # (each module's public routines: the others call them)
    for n, t in asm.items():
        if roles[n][0] == "code":
            m0 = asm16.Module(t, ct, cnames=calls)
            routines[n] = {p for p in m0.public if p in m0.labels}
            externs[n] = set(m0.extrn)
    for n, t in sorted(asm.items(), key=lambda x: (roles[x[0]][0] != "data", x[0])):   # (the data first: the
        role = roles[n][0]                                                          #  names it places)
        if role == "data":
            from codebreaker.port import asmdata
            msg = asmdata.messages(t)
            if msg:                                         # (the C library's numbered messages: a table of C)
                port.asm_data.append((n, msg))
                notes.append("%s: the numbered messages (__NMSG_TEXT, __FMSG_TEXT): a table in data.c" % n)
                continue
            typed = asmdata.typed(t, ct)
            if typed:                                       # (tables with addresses in them: C, initialized)
                port.asm_data.append((n, typed))
                notes.append("%s: tables with addresses in them - C definitions with initializers, in data.c" % n)
                continue
            for d in asm_data_defs(t, ct):
                port.asm_data.append((n, d))
            for al, canon in asm_data_defs.one_place.items():  # (one variable: its name and type everywhere)
                v = ct.var(canon)
                typ = (v[0] + " " + v[1].replace(canon, "", 1)).strip()
                port.renames[al] = canon
                rule = (r"(?m)^(\s*extern\s+)[^;(=]*?\b%s\s*;" % re.escape(canon), r"\g<1>%s%s%s;" % (
                    typ, "" if typ.endswith("*") else " ", canon))
                if rule not in port.decl_rules:
                    port.decl_rules.append(rule)
                notes.append("%s: %s and %s are one place - one variable, %s" % (n, al, canon, typ))
            port.asm_sizes = dict(getattr(port, "asm_sizes", {}), **asm_data_defs.sizes)
            for al, where in asm_data_defs.aliases.items():   # (names of one place: one variable)
                owner = re.search(r"[A-Za-z_]\w*", where).group(0)
                od = next((d for m_, d in port.asm_data if re.search(r"\b%s\s*\[" % owner, d)), None)
                port.aliases[al] = (where, od)
            notes.append("%s: data alone - its variables defined in data.c with the C program's types" % n)
            continue
        if role != "code":
            continue
        m = asm16.Module(t, ct, cnames=calls, cfuncs=cfuncs,
                         asm_routines=set().union(*[r for k, r in routines.items() if k != n]),
                         underscore=profile.get("c_underscore", True))
        m.used_outside = set().union(*[e for k, e in externs.items() if k != n])
        m.param_types = {p: call_arg_types(c, texts, origs, ct) for p, c in m.cname.items()}
        m.aliases = alias_places(port.aliases, ct)
        m.data_map = data_places(paths[n])
        m.far = {p for p, c in m.cname.items() if c in far}
        lib = set(profile["libc"]) | {"strcmp", "strlen", "strcpy", "strcat", "memcpy", "memset", "memcmp"}
        for p, c in list(m.cname.items()):
            if c in lib:                                    # (the program's own routine of a C library name: its
                m.cname[p] = c + "_"                        #  own kept, under a name of its own, in the C too)
                port.renames[c] = c + "_"
                notes.append("%s: its own %s kept (not the C library's), as %s_" % (n, c, c))
        if old_asm and os.path.isfile(old_asm):
            m.offsets = asm16.listing_offsets(paths[n], old_asm)
        try:
            m.run()
            text = m.emit()
        except asm16.Asm16Error as e:
            notes.append("**%s: not ported - %s**" % (n, e))
            continue
        base = os.path.splitext(n)[0] + "64"
        open(os.path.join(out, base + ".asm"), "w", newline="\n").write(text)
        for p, c in m.cname.items():
            ex = m.exit_of(p)
            k = ex.r.get("ax") if ex else None
            ret = "char *" if k is not None and k.wide() and k.kind != "frame" else "int16_t"
            ret = declared.get(c, ret)                      # (the C program's own declaration says)
            protos[c] = "%s%s%s();" % (ret, "" if ret.endswith("*") else " ", c)
        if not ml:
            notes.append("**%s: ported to %s.asm, but no ml64 to assemble it**" % (n, base))
            continue
        p = subprocess.run([ml, "/nologo", "/c", "/Cp", "/Fo" + base + ".obj", base + ".asm"], capture_output=True,
                           text=True, cwd=out)
        if p.returncode:
            notes.append("**%s: %s.asm does not assemble: %s**" % (n, base, (p.stdout + p.stderr)[-400:]))
            continue
        objs.append(base + ".obj")
        notes.append("%s: ported as assembly language to %s.asm (%d routines the C program calls)" % (
            n, base, len(m.cname)))
        if m.unreached:                                     # (code no path was found to: kept as comments - said)
            notes.append("**%s: %d lines no path reaches were left as comments (lines %s) - look at them**" % (
                n, len(m.unreached), ", ".join(map(str, m.unreached[:12]))))
    return objs, protos, notes


def asm_data_defs(text, ct):
    """the variables of an assembler module of data alone (placed by the old linker): C definitions, with the
    C program's types where it declares them"""
    from codebreaker.port import asm16
    lines = asm16.parse(text)
    pubs = set()
    for l in lines:
        d = l.directive
        if d and d[0] == "PUBLIC":
            pubs |= {x.strip() for x in d[2].split(",") if x.strip()}
    places, off, seg = [], 0, None                          # ([C name, old offset, bytes of its own])
    for l in lines:
        d = l.directive
        if d and d[0] == "SEGMENT":
            seg, off = d[1], 0
            continue
        if d and d[0] == "LABEL" and d[1] in pubs:
            places.append([d[1][1:] if d[1].startswith("_") else d[1], (seg, off), 0])
        elif d and d[0] in ("DB", "DW", "DD"):
            m = re.match(r"\s*(\d+)\s+DUP", d[2], re.I)
            n = (int(m.group(1)) if m else len(asm16.split_ops(d[2]))) * {"DB": 1, "DW": 2, "DD": 4}[d[0]]
            if places and places[-1][1][0] == seg:
                places[-1][2] += n
            off += n
    old_size = lambda base, dcl, n: pre_ansi.size_of(base, dcl, n, 2, {}) or 2
    out, aliases, done = [], {}, set()
    one_place = {}                                          # (scalars of one place under two names: one variable,
    for i, (c, at, own) in enumerate(places):               #  of the type that is a pointer if one is)
        same = [p for p in places if p[1] == at]
        v = ct.var(c)
        if len(same) < 2 or c in done or not v or "[" in v[1]:
            continue
        typed = [(p[0], ct.var(p[0])) for p in same if ct.var(p[0]) and "[" not in ct.var(p[0])[1]]
        if len(typed) < 2:
            continue
        canon = next((n for n, w in typed if "*" in w[1]), typed[-1][0])
        for n, w in typed:
            if n != canon:
                one_place[n] = canon
                done.add(n)
    asm_data_defs.one_place = one_place
    for i, (c, at, own) in enumerate(places):
        if c in done:
            continue
        v = ct.var(c)
        nv = ct.var(places[i + 1][0]) if i + 1 < len(places) else None
        same_next = bool(v and nv and re.sub(r"\s", "", nv[0]) == re.sub(r"\s", "", v[0]) and
                         ("*" in nv[1]) == ("*" in re.sub(r"\[\s*\]", "", v[1])))
        if v and re.search(r"\[\s*\]", v[1]) and (own == 0 or same_next):   # (an array that is the names after
            # it: the old linker gave them one place)
            base, dcl = v                                   #  linker gave them one place)
            el = re.sub(r"\[\s*\]", "", dcl).strip()
            es = old_size(base, el, c)
            elt = re.sub(r"\s", "", base + el.replace(c, ""))
            nxt, end = [], None
            for c2, at2, own2 in places[i + 1:]:
                v2 = ct.var(c2)
                if at2[0] != at[0]:
                    break
                d2 = re.sub(r"\s", "", v2[0] + re.sub(r"\[\s*\]", "", v2[1]).replace(c2, "")) if v2 else None
                rel = at2[1] - at[1]
                if d2 == elt and v2 and re.search(r"\[\s*\]", v2[1]) and rel == 0:
                    nxt.append((c2, None))                  # (the same array, by another name)
                elif d2 == elt and v2 and rel % es == 0 and re.search(r"\[\s*\]", v2[1]):
                    nxt.append((c2, ("&", rel // es)))      # (an array that is the rest of it, from there)
                elif d2 == elt and v2 and rel % es == 0:
                    nxt.append((c2, rel // es))             # (one of its elements)
                else:
                    end = at2[1]
                    break
            extent = (end - at[1]) if end is not None else max(es, sum(p[2] for p in places[i:i + len(nxt) + 1]))
            ks = [k if isinstance(k, int) else k[1] for c2, k in nxt if k is not None]
            count = max(1, extent // es, max(ks + [0]) + 1)
            out.append("%s %s;" % (base, dcl.replace("[]", "[%d]" % count).strip()))
            for c2, k2 in nxt:
                aliases[c2] = c if k2 is None else ("(&%s[%d])" % (c, k2[1]) if isinstance(k2, tuple) else
                                                    "%s[%d]" % (c, k2))
                done.add(c2)
            continue
        if v:
            base, dcl = v
            unit = old_size(base, re.sub(r"\[\s*\]", "", dcl), c)
            dcl = re.sub(r"\[\s*\]", "[%d]" % max(1, own // max(1, unit)), dcl)
            out.append("%s %s;" % (base, dcl.strip()))
        else:
            out.append("char %s[%d];\t\t/* (no C declaration says its type) */" % (c, max(own, 1)))
    asm_data_defs.aliases = aliases
    asm_data_defs.sizes = {c: own for c, at, own in places if own}
    return out


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 2
    src, out = argv[0], os.path.abspath(argv[1])            # (the compilers run in OUT_DIR: its own path whole)
    from codebreaker.base import parallel
    parallel.throttle(below_normal=True)                    # (the machine first; the compilers inherit it)
    memory_guard(out)                                       # (and never more of its memory than it may have)
    opt = lambda k, d=None: argv[argv.index(k) + 1] if k in argv else d
    comp = opt("--compiler", "gcc")
    profile = json.load(open(os.path.join(HERE, "runtimes", opt("--runtime", "dos_c_early") + ".json")))
    for pj in (os.path.join(src, "project.json"), os.path.join(src, "src", "project.json")):
        if os.path.isfile(pj):                              # (the recon's build: the library's objects linked first -
            firsts = [os.path.basename(x).upper() for x in json.load(open(pj)).get("first_objects", [])]
            if "BINMODE.OBJ" in firsts:                     #  BINMODE.OBJ: every file binary by default)
                profile["console_text"] = profile.get("text_mode", False)
                profile["text_mode"] = False
                profile.setdefault("notes", {})["binmode"] = "linked with BINMODE.OBJ: files open binary by " \
                    "default (no \\r\\n for \\n), so here"
    compilers = load_compilers()
    spec = compilers[comp]
    cc = find(spec["find"], opt("--cc"))
    if os.path.abspath(out).startswith(os.path.abspath(src) + os.sep) or os.path.abspath(out) == os.path.abspath(src):
        print("the port is written beside the source, never into it: give another OUT_DIR")
        return 2
    files, asm, paths = read_tree(src)
    types = json.load(open(opt("--types"))) if opt("--types") else {}
    port = pre_ansi.Port(files, profile, members=types.get("members", {}))
    port.bss = read_bss(src)
    port.ds = read_symbols(src)
    for t in files.values():                                # (and CodeBreaker's own names: g_304a is DS 304Ah)
        for m in re.finditer(r"\bg_([0-9a-f]{4})\b", t):
            port.ds.setdefault(m.group(0), int(m.group(1), 16))
    roles = {n: asm_role(t, profile) for n, t in asm.items()}
    os.makedirs(out, exist_ok=True)
    for n in ("dosrt.h", "dosrt.c"):
        shutil.copy(os.path.join(HERE, "platform", n), os.path.join(out, n))
    with open(os.path.join(out, "dosrt_config.h"), "w", newline="\n") as f:   # (the old runtime's ways)
        f.write("/* dosrt_config.h -- the old runtime's ways (made by CodeBreaker's port from the profile %s) */\n"
                "#define DOSRT_TEXT_MODE %d\n#define DOSRT_CONSOLE_TEXT %d\n#define DOSRT_EXIT_STATUS %d\n" % (
                    profile["name"], 1 if profile.get("text_mode") else 0,
                    1 if profile.get("console_text", profile.get("text_mode")) else 0,
                    0 if profile.get("exit_status") is False else 1))

    def write():
        res = port.modernize()
        for n, t in port.headers().items():
            if t is not None:
                res[n] = t
        for n, t in res.items():
            open(os.path.join(out, n), "w", encoding="latin1", newline="\n").write(t)
        return res

    objs, asm_notes = [], []
    if any(r[0] in ("code", "data") for r in roles.values()):
        res = write()                                       # (the C as far as it goes: the types the assembly needs)
        objs, protos, asm_notes = asm_modules(port, out, res, asm, paths, roles, profile, opt, compilers)
        port.asm_protos = protos
    rounds, diags = 0, []
    checkers = [(spec, cc)]                                 # (then clang's eyes too: gcc does not check a call of an
    if comp != "clang" and "clang" in compilers:            #  old-style definition, clang does)
        cl = find(compilers["clang"]["find"])
        if cl:
            checkers.append((compilers["clang"], cl))
    from concurrent.futures import ThreadPoolExecutor
    from codebreaker.base import parallel
    for k_, (spec_, cc_) in enumerate(checkers):
        start = rounds
        seen_text, seen_diags, t_round, same, last_said = {}, {}, time.time(), 0, None                  # (what each file was, and what the compiler said of it)
        while True:
            res = write()
            diags = []
            if cc_ is None:
                break
            cfiles = [n for n in sorted(res) if n.endswith(".c")]
            todo = changed_files(res, seen_text, cfiles)   # (the files whose text, or a header's they include,
            keys = {n: check_key(spec_, cc_, n, res) for n in todo}   # changed: the others said it already - and
            for n in list(todo):                            #  what an earlier run's compiler said of the same
                got = cache.value("port-checks", keys[n])  #  text, kept: not said again)
                if got is not None:
                    seen_diags[n] = got
                    todo.remove(n)
            with ThreadPoolExecutor(max(1, parallel.workers())) as ex:
                done = dict(zip(todo, ex.map(lambda n: run(spec_, args(spec_["check"], cc=cc_, inc=".", src=n), out,
                                                           cc_), todo)))
            for n in cfiles:                                # (in the files' order, as one compiler would say them)
                if n in done:
                    seen_diags[n] = diagnostics(spec_, done[n].stdout + done[n].stderr, out)
                    cache.keep("port-checks", keys[n], seen_diags[n])
                diags += seen_diags.get(n, [])
            seen_text = dict(res)
            sys.stderr.write("port: round %d (%s): %d of %d files checked, %d errors, %d warnings, %.0fs\n" % (
                rounds + 1, os.path.basename(cc_), len(todo), len(cfiles),
                sum(1 for d in diags if d["kind"] == "error"), sum(1 for d in diags if d["kind"] != "error"),
                time.time() - t_round))
            sys.stderr.flush()
            t_round = time.time()
            if k_ and not any(d["kind"] == "error" for d in diags):
                break                                       # (the second compiler: its errors only)
            if k_:
                diags = [d for d in diags if d["kind"] == "error"]
            rounds += 1
            said_ = sorted((d["file"], d["line"], d["text"]) for d in diags)
            same = same + 1 if said_ == last_said else 0    # (a round that left the compiler saying all it said
            last_said = said_                               #  before - twice: the rules have nothing more)
            if same >= 2:
                break
            if rounds - start > 8 or not pre_ansi.mend(port, res, diags):
                break
    if port.int16:                                          # (the 8086's arithmetic, where the C of today's differs)
        arith = arith16_pass(port, out, res, compilers)
        if arith:
            res = write()
    if oldlayout_pass(port, out, res, compilers):           # (records the program keeps as bytes: as they were)
        want = dict(getattr(port, "_ol_wanted", {}))
        res = write()
        lost = [(f, b) for (f, b) in want if b not in res.get(f, "").split("\n")]
        if lost:                                            # (a line the pass mended that the next text has not)
            port.mended["**lines the old-layout pass mended that did not reach the port - look at them**"] += [
                "%s: %s" % (f, b.strip()[:120]) for f, b in lost[:40]]
            sys.stderr.write("port: %d old-layout lines did not reach the port (PORT.md)\n" % len(lost))
    srcs = sorted(n for n in res if n.endswith(".c")) + ["dosrt.c"] + objs
    exe = os.path.join(out, "program.exe" if os.name == "nt" else "program")
    cases = opt("--cases")
    sanitized = []
    if cases:                                               # (what only a run shows: the sanitizer, the cases)
        sanitized = sanitize_loop(port, out, write, srcs, compilers, comp, cases)
        res = write()
    built, blog = False, ""
    if os.path.exists(exe):                                 # (an old build is not this port's: a verify would run it)
        os.remove(exe)
    if cc:
        p = build(spec, cc, spec["build"], out, srcs, exe, os.path.join(out, ".obj"))
        built, blog = p.returncode == 0, p.stdout + p.stderr
    with open(os.path.join(out, "build.sh"), "w", newline="\n") as f:
        f.write("#!/bin/sh\n# the port built by %s (made by CodeBreaker's port)\ncd \"$(dirname \"$0\")\"\n%s\n" %
                (comp, " ".join(args(spec["build"], cc=os.path.basename(cc or comp), inc=".", out=os.path.basename(exe),
                                     srcs=srcs))))
    report(out, src, comp, cc, port, roles, diags, rounds, built, blog, profile, sanitized, asm_notes)
    errs = sum(1 for d in diags if d["kind"] == "error")
    print("%s: %d files ported, %d left out (the old runtime), %d rounds; %d errors, %d warnings; %s" % (
        out, len([n for n in res if n.endswith(".c")]), len(port.left_out), rounds, errs,
        sum(1 for d in diags if d["kind"] == "warning"), "BUILT " + exe if built else "not built"))
    return 0 if built else 1


def oldlayout_pass(port, out, res, compilers):
    """the records the program takes as bytes kept in their old layout, their pointers two-byte handles
    (port/oldlayout.py); -> how many lines it changed"""
    from codebreaker.port import oldlayout
    spec = compilers.get("clang")
    cc = find(spec["find"]) if spec else None
    if not cc:
        return 0
    files = sorted(n for n in res if n.endswith(".c"))
    oldlayout.PTR_PARAMS = {f.name: [bool(re.search(r"\*|\[", pt)) for pn, pt in f.params]   # (the old-style
                            for f in port.fns.values() if f.loose}   #  functions kept: their pointer parameters)
    new, olds, found, refused = oldlayout.run(cc, out, files)
    if not olds:
        return 0
    n = 0
    texts = dict(new)
    for h in sorted(x for x in res if x.endswith((".h", ".c"))):   # (the records' definitions, wherever they are)
        t = oldlayout.declare(texts.get(h, res[h]), olds, found)
        if h.endswith(".c"):
            t = oldlayout.null_reads(t)                     # (a NULL read through: the null segment, as on DOS)
        if t != res[h]:
            texts[h] = t
    for f, t in texts.items():
        a_, b_ = res[f].split("\n"), t.split("\n")
        if len(a_) != len(b_):
            continue
        for i, (a, b) in enumerate(zip(a_, b_)):
            if a != b:                                      # (its rule: the line as it was before the rules)
                o = port.line_origin.get(f, {}).get(i + 1)
                port.line_fixes[(f, o[0], o[1]) if o else (f, a)] = b
                port._ol_wanted = getattr(port, "_ol_wanted", {})
                port._ol_wanted[(f, b)] = b
                n += 1
    port.mended["a record the program keeps as bytes (writes, reads, takes as char *): its old layout, its "
                "pointers two-byte handles (dos_ph, dos_hp)"].append(", ".join(sorted(olds)))
    for f, at, op in refused:
        port.mended["**a pointer member of a record kept old, taken by %s: not converted - look at it**" % op].append(
            "%s@%d" % (f, at))
    return n


def arith16_pass(port, out, res, compilers):
    """arith16 over the port's C (clang reads it): each line it changed kept as a rule; -> how many casts"""
    from codebreaker.port import arith16
    cc = find(compilers["clang"]["find"])
    if not cc:
        port.mended["the 16-bit arithmetic NOT checked (no clang to read the port): comparisons of unsigned and "
                    "signed 16-bit values may differ"].append("-")
        return 0
    files = sorted(n for n in res if n.endswith(".c"))
    new = arith16.run(cc, out, files)
    n = 0
    for f, t in new.items():
        for i, (a, b) in enumerate(zip(res[f].split("\n"), t.split("\n"))):
            if a != b:                                      # (its rule: the line as it was before the rules)
                o = port.line_origin.get(f, {}).get(i + 1)
                port.line_fixes[(f, o[0], o[1]) if o else (f, a)] = b
                n += b.count("_t)") - a.count("_t)")
                port.mended["the 8086's 16-bit arithmetic kept (an unsigned met by an int was unsigned; a sum was "
                            "cut to 16 bits before it was compared or made a long): a cast"].append(f)
    return n


def sanitize_loop(port, out, write, srcs, compilers, comp, cases, rounds=6):
    """the port built with AddressSanitizer and run on the cases until it reports nothing; -> the rounds' notes"""
    from codebreaker.port import sanitize, verify
    from codebreaker.base import parallel
    for name in ([comp] if comp != "msvc" else []) + ["clang", "gcc"]:
        spec = compilers.get(name)
        cc = find(spec["find"]) if spec else None
        if cc and (name != "gcc" or os.name != "nt"):       # (MinGW's gcc has no sanitizer runtime)
            break
    else:
        return ["no compiler with AddressSanitizer found: the cases were not run sanitized"]
    rt = sanitize.runtime_dir(cc)
    d = os.path.join(out, ".sanitize")
    os.makedirs(d, exist_ok=True)
    exe = os.path.join(d, "program.exe" if os.name == "nt" else "program")
    notes = []
    write()
    key = sanitize_key(out, srcs, cases, cc)               # (all it would build and run: as the last time -
    mark = os.path.join(out, ".sanitized.json")             #  what it found then, not run again)
    try:
        last = json.load(open(mark))
        if last.get("key") == key:
            return list(last.get("notes", [])) + ["(the port and its cases as the last run: not run again)"]
    except (OSError, ValueError):
        pass
    clean = False
    for r in range(rounds):
        parallel.wait_for_memory("sanitized build")         # (the sanitizer's builds and runs want memory)
        write()
        tpl = [a for a in spec["build"] if a not in ("-O1", "/O1")]
        tpl = tpl[:1] + ["-g", "-fsanitize=address"] + (["-fsanitize=array-bounds"] if name == "clang" else []) + tpl[1:]
        p = build(spec, cc, tpl, out, srcs, exe, os.path.join(d, "obj"))
        if p.returncode:
            notes.append("the sanitized build failed: %s" % (p.stdout + p.stderr)[-300:])
            break
        env = {"PATH": (rt + os.pathsep if rt else "") + os.environ.get("PATH", ""), "ASAN_OPTIONS": "detect_leaks=0"}
        reports = verify.run_cases(cases, exe, env)
        done = sanitize.mend(port, port.out, reports)
        bad = [c for c, t in reports if "AddressSanitizer" in t or "runtime error: index" in t]
        if not done:
            notes.append("round %d: %d cases run sanitized, %s" % (
                r + 1, len(reports), "no fault" if not bad else "faults not mended: " + ", ".join(bad)))
            clean = True
            break
        notes += ["round %d: %s" % (r + 1, why) for v, why in done]
    import shutil as _sh
    _sh.rmtree(d, ignore_errors=True)
    if clean:                                               # (settled: kept, with the key of what it ran)
        try:
            json.dump({"key": sanitize_key(out, srcs, cases, cc), "notes": notes}, open(mark, "w"))
        except OSError:
            pass
    return notes


def sanitize_key(out, srcs, cases, cc):
    """everything a sanitized build and its runs depend on: the port's files, the cases and their inputs, the
    other passes' ports the cases run, the compiler"""
    import hashlib
    h = hashlib.sha256(os.path.abspath(cc).encode())
    for n in sorted(os.listdir(out)):
        f = os.path.join(out, n)
        if os.path.isfile(f) and n.endswith((".c", ".h", ".asm")):
            h.update(n.encode() + b"\0" + open(f, "rb").read())
    try:
        cfg = json.load(open(cases))
        h.update(json.dumps(cfg, sort_keys=True).encode())
        base = os.path.dirname(os.path.abspath(cases))
        at = lambda p_: p_ if os.path.isabs(p_) else os.path.join(base, p_)
        for k, v in sorted(cfg.get("inputs", {}).items()):
            if os.path.isfile(at(v)):
                h.update(open(at(v), "rb").read())
        for k, v in sorted(cfg.get("programs", {}).items()):
            if os.path.isfile(at(v.get("port", ""))):
                st = os.stat(at(v["port"]))
                h.update(repr((k, st.st_size, st.st_mtime)).encode())
    except (OSError, ValueError):
        h.update(b"?")
    return h.hexdigest()


def report(out, src, comp, cc, port, roles, diags, rounds, built, blog, profile, sanitized=(), asm_notes=()):
    L = ["# Port of `%s`" % os.path.basename(os.path.normpath(src)), "",
         "Made by CodeBreaker's port from `%s` (read only), for **%s** (`%s`)." % (src, comp, cc or "not found"), "",
         "**%s.** %d rounds of the compiler's complaints mended." % ("Built" if built else "Not built", rounds), ""]
    L += ["## Left out: the old runtime", "", "| file | why |", "|---|---|"]
    L += ["| %s | %s |" % (n, w) for n, w in sorted(port.left_out.items())]
    for n, (role, pubs) in sorted(roles.items()):
        why = {"runtime": "assembler routines of the old runtime (%s): the C library and the platform layer stand in",
               "data": "data alone (%s)",
               "code": "the program's own assembly language (%s): ported as assembly language (below)"}[role]
        L.append("| %s | %s |" % (n, why % (", ".join(pubs) or "nothing public")))
    L += ["", "## Changes", "", "| change | how many | where (first) |", "|---|---|---|"]
    for k, v in sorted(port.notes.items(), key=lambda kv: -len(kv[1])):
        L.append("| %s | %d | %s |" % (k, len(v), ", ".join(v[:4]) + (" ..." if len(v) > 4 else "")))
    if asm_notes:
        L += ["", "## The program's assembly language", ""] + ["- %s" % x for x in asm_notes]
    if sanitized:
        L += ["", "## Run under AddressSanitizer (the verify cases)", ""] + ["- %s" % s for s in sanitized]
    L += ["", "## Behaviour that may differ", ""]
    L += ["- **%s**: %s" % (k, v) for k, v in profile["notes"].items()]
    L += ["- **int** is %d bits in the original and 32 here: arithmetic that relied on 16-bit wrap-around differs "
          "(structure members keep 16 bits); the verifier (verify.py) is what shows it." % profile["widths"]["int"]]
    if profile.get("char_signed"):
        L.append("- **char** was signed: built with -fsigned-char where the compiler has it.")
    left = [d for d in diags if d["kind"] in ("error", "warning")]
    L += ["", "## What the compiler still says (%d)" % len(left), ""]
    cnt = collections.Counter(re.sub(r"'[^']*'", "'..'", d["text"]) for d in left)
    L += ["- %d x %s" % (n, t) for t, n in cnt.most_common(30)]
    if not built and blog:
        L += ["", "## Build", "", "```", blog[-3000:], "```"]
    open(os.path.join(out, "PORT.md"), "w", encoding="utf-8").write("\n".join(L) + "\n")


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
