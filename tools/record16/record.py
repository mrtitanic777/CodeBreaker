"""Record calls into a 16-bit module while Setup runs: Setup walked to the start of copying (copy_walk,
the original modules, QEMU without acceleration) with QEMU's gdb stub on; this process attaches,
finds the module's code segments in memory every few seconds (the LDT, and each segment's bytes no
fix-up touches) and puts a breakpoint on each function's first instruction. At a hit: the
registers, the stack, DS's and ES's segments, the module's data segment, and every segment a far
pointer among the first arguments names -- what isolated16 needs to run the call again.

    python record.py <tag> [module=setupx.dll] [hits per function=3]
    -> state/record16/rec/<tag>/<function>_<n>.pkl (and base_<scan>.bin.z, the memory a call
    is recorded against). Then replay.py <tag> <build folder> runs each again, the original beside
    the build."""
import os
import pickle
import re
import struct
import subprocess
import sys
import time
import zlib

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
CB = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, HERE)
sys.path.insert(0, CB)
from codebreaker.base import corpus, runs, settings
from codebreaker.formats.ne import NEFile
from codebreaker.verify.gate16 import isolated16
from codebreaker.verify.gate16 import setup16 as s
import gdbrsp
import memmap16

tag = sys.argv[1]
module = sys.argv[2] if len(sys.argv) > 2 else "setupx.dll"
cap = int(sys.argv[3]) if len(sys.argv) > 3 else 3
OUT = settings.state("record16")              # (recordings, walks, dumps: state, not the tree)
out_dir = os.path.join(OUT, "rec", tag)
os.makedirs(out_dir, exist_ok=True)
original = corpus.retail(module)
nef = NEFile(original)
tables = isolated16._tables(original)
place = {n: tuple(v) for n, v in tables["place"].items()}
funcs = {n: tuple(v) for n, v in tables["funcs"].items()}
sigs = memmap16.signatures(original)
auto = nef.h.get("autodata_seg")
seg_data = {sg["idx"]: nef.data[sg["file_off"]:sg["file_off"] + sg["length"]] for sg in nef.segments() if sg["file_off"]}
masks = {sg["idx"]: memmap16._fixup_mask(nef, sg) for sg in nef.segments() if sg["file_off"]}
data_sites = {}                  # code segment -> an offset the loader writes the data selector at
for _sg in nef.segments():
    if _sg["file_off"] and _sg["kind"] == "CODE":
        for _r in nef.relocations(_sg):
            if _r.get("kind") == "INTERNAL" and _r.get("seg") == auto and _r["src"] == 2 and not _r["additive"]:
                data_sites[_sg["idx"]] = _r["offset"]
                break
log_f = open(os.path.join(out_dir, "record.log"), "a")


def log(*a):
    line = time.strftime("%H:%M:%S") + " " + " ".join(str(x) for x in a)
    print(line, flush=True)
    log_f.write(line + "\n")
    log_f.flush()


if "W95KEY" not in os.environ:                       # (the key Setup asks for, from settings.json)
    from codebreaker.base import settings as _s
    os.environ["W95KEY"] = _s.get("w95_key") or ""   # (settings.json, local)
port = s._free_port()
walk = subprocess.Popen([sys.executable, os.path.join(HERE, "copy_walk.py"), "rec_" + tag, "-", "12"],
                        env=dict(os.environ, COPY_WALK_GDB=str(port), QEMU_FAST="0"),
                        stdout=open(os.path.join(out_dir, "walk.log"), "w"), stderr=subprocess.STDOUT)
g = gdbrsp.GDB(port, connect_timeout=120)
g.halt()


def info():
    """(protected mode, LDT base, LDT limit) from the monitor's register dump."""
    t = g.monitor("info registers")
    cr0 = re.search(r"CR0=([0-9a-f]{8})", t)
    ldt = re.search(r"LDT=([0-9a-f]{4}) ([0-9a-f]{8}) ([0-9a-f]{8})", t)
    if not cr0 or not ldt:
        return False, 0, 0
    return bool(int(cr0.group(1), 16) & 1), int(ldt.group(2), 16), int(ldt.group(3), 16)


def descriptor(ldt_base, sel):
    d = g.read(ldt_base + (sel & ~7), 8)
    base = d[2] | d[3] << 8 | d[4] << 16 | d[7] << 24
    lim = d[0] | d[1] << 8 | (d[6] & 0x0F) << 16
    return base, lim, d[5]


work = runs.start("record16", tag)            # (the memory dumps, 64 MB each: the run's, deleted at its end)
dump_path = os.path.join(work, "rec_mem.bin").replace("\\", "/")
where = {}                       # segment -> {"selector", "base", "limit"}


def rescan():
    pm, ldt_base, ldt_lim = info()
    if not pm or not ldt_lim:
        return None
    g.monitor("pmemsave 0 %d %s" % (64 << 20, dump_path))
    dump = open(dump_path, "rb").read()
    base_mem["np"] = np.frombuffer(dump, dtype=np.uint8).reshape(-1, 4096)
    base_mem["id"] += 1
    base_mem["saved"] = False
    base_mem["ldt"] = (ldt_base, ldt_lim)
    descs = memmap16.descriptors(dump, ldt_base, ldt_lim)
    found = {}
    for sg, (length, kind, picks) in sigs.items():
        best = None
        for sel, base, lim, acc in descs:
            if lim + 1 < length or base + length > len(dump):
                continue
            if kind == "CODE" and any(dump[base + o] != b for o, b in picks[:6]):
                continue
            hit = sum(1 for o, b in picks if dump[base + o] == b)
            score = hit / max(1, len(picks))
            # (a code segment by its code descriptor, not a data alias of it at the same base)
            key = (score, bool(acc & 8) == (kind == "CODE"))
            if best is None or key > best[0]:
                best = (key, sel, base, lim)
        if best and best[0][0] >= (0.98 if kind == "CODE" else 0.6):
            found[sg] = {"selector": best[1], "base": best[2], "limit": best[3]}
    # (the data segment, written to as Setup runs, is not known by its bytes: by its selector,
    # which the loader wrote into the module's own code wherever the code names it)
    for sg, info_ in list(found.items()):
        if sg == auto:
            continue
        site = data_sites.get(sg)
        if site is None:
            continue
        sel = dump[info_["base"] + site] | dump[info_["base"] + site + 1] << 8
        if sel & 4:
            d = dump[ldt_base + (sel & ~7):ldt_base + (sel & ~7) + 8]
            if d[5] & 0x80:
                found[auto] = {"selector": sel, "base": d[2] | d[3] << 8 | d[4] << 16 | d[7] << 24,
                               "limit": d[0] | d[1] << 8}
                break
    return ldt_base, found


hits = {}
bps = {}                          # linear -> name


# (QEMU looks through every breakpoint at each block it runs: a thousand made Setup crawl, 5% of
# a progress bar in four minutes. So a window of them at a time, moved on at each scan -- the
# functions named in RECORD_FIRST (a file of names) first, then those recorded least)
ACTIVE = int(os.environ.get("RECORD_ACTIVE", "60"))
first = set()
if os.environ.get("RECORD_FIRST"):
    first = set(open(os.environ["RECORD_FIRST"]).read().replace("\n", ",").split(",")) - {""}
turn = [0]


def want():
    cand = sorted((name for name, (sg, off) in place.items()
                   if hits.get(name, 0) < cap and sg in where and name in funcs),
                  key=lambda n: (n not in first, hits.get(n, 0), n))
    if os.environ.get("RECORD_ONLY"):                    # (those named, and no others)
        cand = [n for n in cand if n in first]
    if len(cand) > ACTIVE:
        k = turn[0] % len(cand)
        turn[0] += ACTIVE // 2
        # (half the window the most wanted, half going round all the rest)
        head = cand[:ACTIVE // 2]
        rest = cand[ACTIVE // 2:]
        k = k % max(1, len(rest))
        cand = head + (rest[k:] + rest[:k])[:ACTIVE - len(head)]
    return {where[place[n][0]]["base"] + place[n][1]: n for n in cand}


base_mem = {"np": None, "id": 0, "saved": False, "ldt": None}
hit_path = os.path.join(work, "rec_hit.bin").replace("\\", "/")


def memory():
    """The whole of memory at this stop, as the pages that differ from the last scan's dump
    (that dump kept once, compressed): every segment the call may reach, not those its
    arguments name alone -- a selector a callee returned named nothing recorded, and sub_22_33B8's
    fault went unseen."""
    if base_mem["np"] is None:
        return None
    path = os.path.join(out_dir, "base_%d.bin.z" % base_mem["id"])
    if not base_mem["saved"]:
        with open(path, "wb") as f:
            f.write(zlib.compress(base_mem["np"].tobytes(), 1))
        base_mem["saved"] = True
    g.monitor("pmemsave 0 %d %s" % (64 << 20, hit_path))
    now = np.fromfile(hit_path, dtype=np.uint8).reshape(-1, 4096)
    pages = np.nonzero((now != base_mem["np"]).any(axis=1))[0]
    return {"base": os.path.basename(path), "pages": {int(i): now[i].tobytes() for i in pages},
            "ldt": base_mem["ldt"]}


def capture(name, regs, ldt_base):
    sg, off = place[name]
    pop, far = funcs[name]
    ss, ds, es = regs["ss"], regs["ds"], regs["es"]
    sp = regs["esp"] & 0xFFFF

    def segment(sel):
        base, lim, acc = descriptor(ldt_base, sel)
        if not acc & 0x80:
            return None
        return g.read(base, min(lim, 0xFFFF) + 1), bool(acc & 8)
    stack = segment(ss)[0]
    ldt = {}
    for sel in (ds, es):
        if sel & 4 and sel != ss and sel not in ldt:
            got = segment(sel)
            if got:
                ldt[sel] = got
    data_sel = where.get(auto, {}).get("selector")
    data = segment(data_sel)[0] if data_sel else None
    first = sp + (4 if far else 2)
    words = [struct.unpack_from("<H", stack, (first + 2 * k) & 0xFFFE)[0] for k in range(16)]
    for k in range(len(words) - 1):
        sel = words[k + 1]
        if sel & 4 and (sel & 3) == 3 and sel not in ldt and sel not in (ss, data_sel) and len(ldt) < 8:
            try:
                got = segment(sel)
            except IOError:
                got = None
            if got:
                ldt[sel] = got
    rec = {"name": name, "seg": sg, "offset": off, "pop": pop, "far": far,
           "regs": {k.upper()[1:]: regs[k] & 0xFFFF for k in ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp")},
           "ss": ss, "ds": ds, "es": es, "sp": sp, "stack": stack, "data": data, "data_sel": data_sel,
           "ldt": ldt, "words": words, "when": time.time(), "memory": memory()}
    n = hits.get(name, 0)
    with open(os.path.join(out_dir, "%s_%d.pkl" % (name, n)), "wb") as f:
        pickle.dump(rec, f)
    hits[name] = n + 1


t0 = time.time()
last_scan = 0
n_hits = 0
strays = 0
skipped = 0
ldt_base = None
try:
    busy = os.path.join(OUT, "walk", "rec_" + tag, "busy")
    while walk.poll() is None:
        if os.path.exists(busy):
            # (the walk sends keys: the machine let run, no breakpoint in its way, till it is done)
            for a in list(bps):
                g.bp_del(a)
                del bps[a]
            g.cont()
            open(busy + ".ack", "w").close()
            while os.path.exists(busy) and walk.poll() is None:
                time.sleep(0.2)
            g.halt()
            os.remove(busy + ".ack")
            last_scan = 0
            continue
        if time.time() - last_scan > 8:
            t_scan = time.time()
            got = rescan()
            last_scan = time.time()
            if not got:
                skipped += 1
            if got:
                ldt_base, found = got
                moved = [sg for sg in found if sg in where and where[sg]["base"] != found[sg]["base"]]
                where = found
                new = want()
                for a in [a for a in bps if a not in new]:
                    g.bp_del(a)
                    del bps[a]
                for a, name in new.items():
                    if a not in bps and g.bp_add(a):
                        bps[a] = name
                log("scan (%.1f s, %d skipped before): %d segments found%s, %d breakpoints, %d calls recorded" % (
                    last_scan - t_scan, skipped, len(found), " (moved: %s)" % moved if moved else "", len(bps), n_hits))
                skipped = 0
        g.cont()
        stop = g.wait(8)
        if stop is None:
            g.halt()
            continue
        if not stop.startswith("T05") and not stop.startswith("S05"):
            log("stop:", stop[:40])
            if stop.startswith("W") or stop.startswith("X"):
                break
            continue
        regs = g.regs()
        # (where it stopped, from the processor's own CS base -- a GDT selector, real mode, an LDT
        # one alike: computed from the LDT alone, a stop under a GDT selector was not known, was
        # continued without stepping off, and the machine sat on that instruction for good)
        m = re.search(r"CS =[0-9a-f]{4} ([0-9a-f]{8})", g.monitor("info registers"))
        lin = (int(m.group(1), 16) if m else 0) + (regs["eip"] & 0xFFFF)
        name = bps.get(lin)
        if name is None or ldt_base is None:
            strays += 1
            if strays % 50 == 1:
                log("a stop not at a breakpoint of ours: %x (%d so far)" % (lin, strays))
            g._send("s")
            g.wait(5)
            continue
        g.bp_del(lin)
        del bps[lin]
        sg, off = place[name]
        body = g.read(lin, 8)
        ok = all(body[k] == seg_data[sg][off + k] for k in range(8) if off + k not in masks[sg] and off + k < len(seg_data[sg]))
        if ok:
            capture(name, regs, ldt_base)
            n_hits += 1
        if ok and hits.get(name, 0) < cap:
            g._send("s")                                 # (off the breakpoint, then it back)
            g.wait(5)
            if g.bp_add(lin):
                bps[lin] = name
finally:
    log("done after %.0f s: %d calls of %d functions recorded" % (time.time() - t0, n_hits, len(hits)))
    try:
        g.s.close()
    except OSError:
        pass
    try:
        walk.wait(timeout=60)
    except subprocess.TimeoutExpired:
        walk.kill()
    runs.remove(work)
