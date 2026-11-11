"""expected.py -- the ORIGINAL codebreaker.formats.inspect on real corpus files and damaged copies.

Writes cases.txt (one call per line, tab separated: the same calls test.cpp makes) and expected.txt.
Run from this folder:  python expected.py
"""
import contextlib
import glob
import hashlib
import io
import json
import os
import random
import shutil
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, "C:/Users/mrtit/Documents/GitHub/CodeBreaker")
from codebreaker.base import corpus                      # noqa: E402
from codebreaker.formats import detect, dos, inspect     # noqa: E402

import time                                              # noqa: E402

TIMES = {}


import functools  # noqa: E402


def _timed(name, fn):
    @functools.wraps(fn)
    def run(*a, **k):
        t = time.perf_counter()
        try:
            return fn(*a, **k)
        finally:
            TIMES[name] = TIMES.get(name, 0.0) + time.perf_counter() - t
    return run


for _n in ("describe", "strings", "hexdump", "resource_body", "folder_map", "markdown", "render", "main"):
    setattr(inspect, _n, _timed(_n, getattr(inspect, _n)))

random.seed(20261111)
INPUTS = os.path.join(HERE, "inputs")
shutil.rmtree(INPUTS, ignore_errors=True)
os.makedirs(INPUTS)
ROOT = corpus.root()
EXTS = (".dll", ".exe", ".drv", ".ocx", ".cpl", ".vxd", ".386", ".sys", ".com")


def pick(kind, n, small=False):
    mods = sorted(corpus.modules(kind, exts=EXTS), key=lambda p: (os.path.getsize(p), p))
    if small:
        mods = [m for m in mods if os.path.getsize(m) < 120000]
    return [mods[i * (len(mods) - 1) // (n - 1)] for i in range(n)]


files = []
files += pick("pe", 9) + pick("pe", 5, small=True)
files += pick("ne", 8) + pick("ne", 4, small=True)
files += pick("le", 6)
files += pick("dos", 6)
vmm = os.path.join(ROOT, "WINDOWS", "SYSTEM", "vmm32.vxd")
files.append(vmm)
for name in ("kernel32.dll", "user32.dll", "krnl386.exe", "user.exe", "commdlg.dll", "comctl32.dll"):
    p = corpus.retail(name)
    if p:
        files.append(p)
seen = set()
files = [f for f in files if not (f in seen or seen.add(f))]
others = []
for pat in ("**/*.bmp", "**/*.ico", "**/*.cur", "**/*.hlp", "**/*.inf", "**/*.fon", "**/*.txt", "**/*.ttf"):
    g = sorted(glob.glob(os.path.join(ROOT, pat), recursive=True), key=lambda p: (os.path.getsize(p), p))
    others += g[:2]


def damaged():
    out = []
    pe_small = pick("pe", 3, small=True)[1]
    ne_small = pick("ne", 3, small=True)[1]
    le_small = pick("le", 3)[1]
    dos_small = pick("dos", 3)[1]
    for tag, src, cuts in (("pe", pe_small, (0x40, 0x100, 0x200, 0x400, 0x1000, None)),
                           ("ne", ne_small, (0x40, 0x100, 0x200, 0x400, None)),
                           ("le", le_small, (0x40, 0x100, 0x400, None)),
                           ("dos", dos_small, (0x20, 0x100, None))):
        data = open(src, "rb").read()
        for cut in cuts:
            n = len(data) // 2 if cut is None else cut
            p = os.path.join(INPUTS, "%s_cut_%d.bin" % (tag, n))
            open(p, "wb").write(data[:n])
            out.append(p)
    data = bytearray(open(pe_small, "rb").read())
    for tag, lo, hi in (("dirs", 0x80, 0x100), ("sections", 0x100, 0x180), ("tables", 0x200, 0x400)):
        d = bytearray(data)
        for i in range(lo, min(hi, len(d))):
            d[i] = random.randrange(256)
        p = os.path.join(INPUTS, "pe_noise_%s.bin" % tag)
        open(p, "wb").write(bytes(d))
        out.append(p)
    data = bytearray(open(ne_small, "rb").read())
    for tag, lo, hi in (("hdr", 0x80, 0xC0), ("tables", 0x100, 0x200)):
        d = bytearray(data)
        for i in range(lo, min(hi, len(d))):
            d[i] = random.randrange(256)
        p = os.path.join(INPUTS, "ne_noise_%s.bin" % tag)
        open(p, "wb").write(bytes(d))
        out.append(p)
    for name, blob in (("mz_only.exe", b"MZ" + bytes(62)), ("mz_random.exe", b"MZ" + bytes(random.randrange(256) for _ in range(4096))),
                       ("empty.bin", b""), ("text.txt", b"hello world\r\nthis is text %d \\ path/to C:\\x.dll ERROR_CODE_X\r\n" * 3),
                       ("zeros.dat", bytes(2048)), ("random.dat", bytes(random.randrange(256) for _ in range(3000)))):
        p = os.path.join(INPUTS, name)
        open(p, "wb").write(blob)
        out.append(p)
    return out


files += others + damaged() + [os.path.join(INPUTS, "no_such_file.exe")]

# a folder to map: small files of several kinds, and the directories a map skips
FOLDER = os.path.join(INPUTS, "folder")
os.makedirs(os.path.join(FOLDER, "sub", "deeper"))
os.makedirs(os.path.join(FOLDER, ".git"))
os.makedirs(os.path.join(FOLDER, "node_modules"))
small = [f for f in files if os.path.isfile(f) and 0 < os.path.getsize(f) < 60000][:40]
for i, f in enumerate(small):
    dest = [FOLDER, os.path.join(FOLDER, "sub"), os.path.join(FOLDER, "sub", "deeper")][i % 3]
    shutil.copy(f, os.path.join(dest, os.path.basename(f)))
shutil.copy(files[0], os.path.join(FOLDER, ".git", "x.exe"))
shutil.copy(files[0], os.path.join(FOLDER, "node_modules", "y.dll"))
for name, text in (("a.c", "int main(){}\n"), ("b.py", "print(1)\n"), ("Makefile", "all:\n"), ("README", "hi\n"), ("c.json", "{}\n"),
                   ("d.unknownext", "zzz"), ("noext", "plain")):
    open(os.path.join(FOLDER, name), "w").write(text)
open(os.path.join(FOLDER, "bin.c"), "wb").write(b"\0\0\0binary")
open(os.path.join(FOLDER, "empty.c"), "w").close()

cases = []
out = open(os.path.join(HERE, "expected.txt"), "w", encoding="utf-8", newline="\n")


def case(*args):
    cases.append("\t".join(str(a) for a in args))


def emit(label, text):
    out.write("== %s\n%s\n" % (label, text))


def jd(v):
    return json.dumps(v, sort_keys=True)


def capture(argv):
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        code = inspect.main(list(argv))
    return "exit %d\n%s" % (code, buf.getvalue())


def body(f, off, size, tid, wide):
    r = dict(inspect.resource_body(f, off, size, tid, bool(wide)))
    if "bytes" in r:
        r["bytes"] = r["bytes"].decode("latin1")
    return jd(r)


for f in files:
    info = inspect.describe(f)
    case("describe", f)
    emit("describe %s" % f, jd(info))
    for section in ("summary", "dos", "headers", "directories", "sections", "exports", "imports", "resources", "relocations",
                    "tls", "debug", "certificates", "clr", "rich", "driver", "members"):
        if section in info:
            lines = inspect.render(info[section], section)
            if len(lines) < 4000:
                case("render", f, section)
                emit("render %s %s" % (f, section), "\n".join(lines))
    case("markdown", f)
    emit("markdown %s" % f, inspect.markdown(info))
    size = os.path.getsize(f) if os.path.isfile(f) else 0
    if 0 < size < 400000:
        for min_len, wide, limit in ((4, True, None), (8, False, 50), (3, True, -5), (6, True, 0)):
            case("strings", f, min_len, int(wide), "" if limit is None else limit)
            rows = inspect.strings(f, min_len, wide, limit)
            emit("strings %s %s %s %s" % (f, min_len, wide, limit), jd(rows[:400]) + "\ncount %d" % len(rows))
    elif size:
        case("strings", f, 12, 1, 100)
        emit("strings %s 12 True 100" % f, jd(inspect.strings(f, 12, True, 100)))
    if size:
        for off, ln in ((0, 256), (0x40, 100), (size - 20, 64), (-5, 32), (3, 0), (0, 4100)):
            case("hexdump", f, off, ln)
            emit("hexdump %s %s %s" % (f, off, ln), inspect.hexdump(f, off, ln))
    res = info.get("resources") or {}
    done = 0
    for t in res.get("types", []):
        for it in t["items"][:(3 if t["id"] in (1, 2, 3, 6, 16) else 1)]:
            wide = bool(it.get("wide"))
            off = it["offset"] if it["offset"] is not None else -1
            case("resource_body", f, off, it["size"], it["type_id"], int(wide))
            emit("resource_body %s %s %s %s %s" % (f, off, it["size"], it["type_id"], wide), body(f, off, it["size"], it["type_id"], wide))
            done += 1
        if done > 40:
            break
for f in files[:3]:
    for tid, off, size, wide in ((2, 0, 5, 1), (2, 0, 100, 1), (1, 0, 3, 1), (3, 0, 200, 1), (16, 0, 300, 1), (6, 0, 300, 0),
                                 (10, 0, 300, 1), (23, 0, 100, 1), (99, 0, 50, 1), (-1, 10, 0, 1), (-1, -4, 20, 1), (5, 0, 1 << 22, 1)):
        case("resource_body", f, off, size, tid, int(wide))
        emit("resource_body %s %s %s %s %s" % (f, off, size, tid, bool(wide)), body(f, off, size, tid, wide))

case("folder", FOLDER)
fm = inspect.folder_map(FOLDER)
emit("folder %s" % FOLDER, jd(fm))
case("folder_markdown", FOLDER)
emit("folder_markdown %s" % FOLDER, inspect.markdown(fm))
for name in ("a.c", "b.py", "Makefile", "README", "c.json", "d.unknownext", "bin.c", "empty.c", "missing.c", "CMakeLists.txt"):
    p = os.path.join(FOLDER, name)
    case("text_kind", p, name)
    emit("text_kind %s" % name, jd(inspect._text_kind(p, name)))
for n in (0, 1, 1023, 1024, 1536, 5 * 1024 * 1024, 3 * 1024 ** 3, 7 * 1024 ** 4):
    case("size", n)
    emit("size %d" % n, inspect._size(n))
case("xex_facts", files[0])
emit("xex_facts", jd(inspect.xex_facts(files[0])))

mains = []
pe_f, ne_f, le_f, dos_f = pick("pe", 3, small=True)[1], pick("ne", 3, small=True)[1], pick("le", 3)[1], pick("dos", 3)[1]
for target in (pe_f, ne_f, le_f, dos_f, vmm):
    mains += [[target], [target, "--section", "summary"], [target, "--section", "all"], [target, "--json"],
              [target, "--section", "headers", "--json"], [target, "--section", "sections"], [target, "--markdown"],
              [target, "--section", "bogus"], [target, "--section", "exports"], [target, "--section", "imports"],
              [target, "--section", "resources"], [target, "--section", "strings", "--min", "10"],
              [target, "--section", "strings", "--json", "--min", "14"],
              [target, "--section", "hex", "--offset", "64", "--length", "80"],
              [target, "--section", "hex", "--json"], [target, "--section", "relocations"], [target, "--section", "members"]]
mains += [[], ["--json"], [os.path.join(INPUTS, "no_such_thing.exe")], [os.path.join(INPUTS, "text.txt")],
          [os.path.join(INPUTS, "text.txt"), "--json"], [os.path.join(INPUTS, "empty.bin")],
          [pe_f, "--section", "hex", "--offset", "0x20"], [FOLDER], [FOLDER, "--json"], [FOLDER, "--markdown"],
          ["kernel32.dll", "--section", "summary"], ["user.exe", "--section", "summary"],
          [dos_f, "--unpack", os.path.join(INPUTS, "unpacked.exe")]]
md_out = os.path.join(INPUTS, "report.md")
mains += [[pe_f, "--markdown", "--out", md_out], [FOLDER, "--markdown", "--out", md_out]]
for p in corpus.modules("dos", exts=EXTS):
    try:
        if dos.packer(p):
            mains.append([p, "--unpack", os.path.join(INPUTS, "unpacked2.exe")])
            break
    except Exception:
        pass
for argv in mains:
    case("main", *argv)
    emit("main %s" % " ".join(argv), capture(argv))
for extra in ("report.md", "unpacked.exe", "unpacked2.exe"):
    p = os.path.join(INPUTS, extra)
    if os.path.isfile(p):
        emit("file %s" % extra, hashlib.sha256(open(p, "rb").read()).hexdigest())
        case("file", p)
out.close()
open(os.path.join(HERE, "cases.txt"), "w", encoding="utf-8", newline="\n").write("\n".join(cases) + "\n")
print("%d cases" % len(cases))
sys.stderr.write("python seconds by call: " + ", ".join("%s %.2f" % kv for kv in sorted(TIMES.items())) + " (total %.2f)\n" % sum(TIMES.values()))
