#!/usr/bin/env python3
"""
masmlift.py — assemble reconstructed 16-bit ASM with the period MASM (stage F).

Much of KERNEL/USER/GDI was hand-written assembly, and assembly can only be
proven byte-exact by RE-ASSEMBLING it and diffing the bytes — translating it to
C never reproduces the original encoding. MASM is a 16-bit DOS program (no NTVDM
on x64), so like `cc16` drives CL, this drives MASM.EXE under the vendored
DOSBox-X.

Crucially, MASM emits an OMF `.OBJ` exactly like the C compiler, so the whole
downstream is REUSED: `objextract` pulls the code bytes + relocation mask and
`segdiff` renders the byte-exact verdict. This module only has to run the
assembler — the acceptance test for hand-asm is then identical to the C path.

    python masmlift.py --selftest            # assemble a tiny proc, extract it
    python masmlift.py foo.asm               # assemble foo.asm, dump OBJ segments
"""
import argparse
import atexit
import hashlib
import os
import subprocess
import sys
import tempfile
import threading
import time
import zlib

ROOT = os.path.dirname(os.path.abspath(__file__))
_ROOT = os.path.dirname(os.path.dirname(ROOT))                    # CodeBreaker/engine -> repo root
# Defaults are DATA_ROOT-RELATIVE so a checkout works anywhere; config.json (config.apply_config)
# overrides them for a machine whose tools live elsewhere.
DBX = os.path.join(_ROOT, "win1.0", "refs", "tools", "dosbox-x", "bin", "x64", "Release", "dosbox-x.exe")
MASMROOT = os.path.join(_ROOT, "win1.0", "build16")               # has BIN/INC/LIB

# DOSBox CPU core. `normal` is the safe interpreter; `dynamic` JITs the emulated x86
# (much faster on cache MISSES). MASM output is deterministic, so a correct core yields
# identical OBJ either way -- but dynamic cores can have edge bugs, so it's opt-in and
# validated before use (A/B vs normal must be byte-identical). WT_DOSBOX_CORE overrides.
DOSBOX_CORE = os.environ.get("WT_DOSBOX_CORE", "dynamic")   # validated byte-identical to `normal`

CONF = r"""[sdl]
autolock=false
[cpu]
cputype=386
core=%s
cycles=max""" % DOSBOX_CORE + r"""
[dos]
ver=6.22
[autoexec]
mount c "{masm}"
mount d "{work}"
set PATH=C:\BIN
set INCLUDE=C:\INC
set LIB=C:\LIB
set TMP=D:\
d:
{cmd}
echo done > D:\DONE.FLG
exit
"""


def _wait_dosbox(proc, work, timeout, aborted):
    """Poll a cold-boot DOSBox assemble run. Returns 'aborted' | 'done' | 'timeout'.

    The autoexec writes a DONE.FLG sentinel AFTER the MASM commands (all .OBJs are closed
    by then), so we can kill the box the instant assembly finishes instead of waiting out
    its shutdown. DOSBox-X under `SDL_VIDEODRIVER=dummy` frequently does NOT exit on the
    autoexec `exit` -- the process lingers until the timeout, which is what made cold-path
    modules cost a fixed ~185s (120 + 3*chunk) of pure wait despite the OBJ being ready in
    seconds. Sentinel-detection is exactly how the warm-pool worker keys off its ACK file."""
    flag = os.path.join(work, "DONE.FLG")
    waited = 0.0
    while proc.poll() is None:
        if aborted():
            proc.kill(); proc.wait(timeout=5)
            return "aborted"
        if os.path.exists(flag):
            time.sleep(0.15)                 # small settle so the last OBJ is flushed to the host FS
            proc.kill()
            return "done"
        time.sleep(0.05)
        waited += 0.05
        if waited > timeout:
            proc.kill()
            return "timeout"
    return "done"                            # exited on its own (older DOSBox, or MASM error)


def assemble(asm_src, stem="t", timeout=180, masmroot=None):
    """Assemble asm_src with MASM under DOSBox-X. Returns {obj, lst, log, work}.

    masmroot selects the assembler: the default (MASM 4.00, 16-bit, proven
    byte-exact for the NE core) or an override such as the vendored MASM 5.10A
    (32-bit `.386`/USE32) that the PE path in asmrecon32 needs."""
    masm = masmroot or MASMROOT
    if not os.path.exists(DBX):
        raise RuntimeError("dosbox-x not found: %s (set it: CodeBreaker config dosbox_x <path>)" % DBX)
    if not os.path.exists(os.path.join(masm, "BIN", "MASM.EXE")):
        raise RuntimeError("MASM.EXE not found under %s\\BIN (set it: CodeBreaker config masm <path>)" % masm)
    work = tempfile.mkdtemp(prefix="masm_")
    with open(os.path.join(work, stem + ".ASM"), "w", newline="\r\n") as f:
        f.write(asm_src)
    cmd = "MASM %s.ASM, %s.OBJ, %s.LST, NUL; > %s.LOG 2>&1" % (stem, stem, stem, stem)
    conf = os.path.join(work, "masm.conf")
    with open(conf, "w", newline="\r\n") as f:
        f.write(CONF.format(masm=masm, work=work, cmd=cmd))
    env = dict(os.environ, SDL_VIDEODRIVER="dummy")
    # Run DOSBox as a Popen and poll a cancel check so a Cancel press kills the
    # assembler immediately instead of waiting out the run (fast power-off).
    try:
        from codebreaker.foundation import progress as _pr
        aborted = _pr.cancelled
    except Exception:
        aborted = lambda: False
    proc = subprocess.Popen([DBX, "-conf", conf, "-nolog"], env=env,
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    if _wait_dosbox(proc, work, timeout, aborted) == "aborted":
        return {"work": work, "aborted": True}
    out = {"work": work}
    for ext, key in ((".OBJ", "obj"), (".LST", "lst"), (".LOG", "log")):
        p = os.path.join(work, stem + ext)
        if os.path.exists(p):
            out[key] = open(p, "rb").read() if ext == ".OBJ" else \
                open(p, encoding="latin1").read()
    return out


# ---- warm worker pool ------------------------------------------------------
# The per-batch DOSBox BOOT is the dominant cost. A pool of PERSISTENT DOSBox
# sessions (dosworker.py) removes it: each session boots once and assembles batch
# after batch. Keyed by masmroot (16-bit MASM 4.00 vs 32-bit MASM 5.10A). On by
# default; set WT_NO_WARM_POOL=1 to force the cold-boot path.
# Workers PER pool (there are two pools: 16-bit MASM 4.00 and 32-bit MASM 5.10A),
# so total live DOSBoxes <= 2*POOL_MAX. Kept small so the pool can't peg the host:
# even throttled, ~a dozen persistent DOSBox sessions saturate all cores and
# STARVE the very MASM they exist to run. 3/pool -> <=6 DOSBoxes, well under the
# core count. Tune with WT_POOL_MAX.
POOL_MAX = max(1, int(os.environ.get("WT_POOL_MAX", "3")))
WARM_POOL = os.environ.get("WT_NO_WARM_POOL") != "1"
# Retire a warm worker after this many assemble batches (0 = never, the DEFAULT).
# Recycling is OFF by default because it does more harm than good here: a freshly-booted
# DOSBox-X worker's FIRST dispatch tends to FLAKE (its RESCAN isn't stable yet, so it misses
# the GO trigger) -> that batch eats the full ACK timeout before falling to cold. Measured:
# with recycling on, ~half of large (multi-chunk) modules cost ~50s; with it off, the same
# modules are 5-8s. Any genuine long-run worker slowdown is already capped cheaply by the
# cold-path sentinel (~9s) + the tight ACK timeout (~45s bail), so proactive recycling isn't
# needed. Set WT_WORKER_MAX_USES>0 to opt back in.
WORKER_MAX_USES = int(os.environ.get("WT_WORKER_MAX_USES", "0"))
# Max functions assembled per DOSBox dispatch. The COLD-boot path puts one MASM line
# per function into the DOSBox [autoexec] config section, which has a small fixed buffer
# -- a large module (100s of funcs) overflows it: `E_Exit: Autoexec.bat file overflow`.
# Chunking assemble_many below this bound makes ANY module size safe regardless of path
# (warm WORK.BAT has no such limit, but chunking it too is cheap: same warm session).
ASM_BATCH = max(1, int(os.environ.get("WT_ASM_BATCH", "40")))
_POOLS = {}
_POOLS_LOCK = threading.Lock()


def _get_pool(masmroot):
    from codebreaker.recon_engine import dosworker
    with _POOLS_LOCK:
        p = _POOLS.get(masmroot)
        if p is None:
            p = dosworker.WorkerPool(masmroot, DBX, POOL_MAX)
            _POOLS[masmroot] = p
        return p


def close_pools():
    with _POOLS_LOCK:
        for p in _POOLS.values():
            try:
                p.close()
            except Exception:
                pass
        _POOLS.clear()


atexit.register(close_pools)


def _cancelled():
    try:
        from codebreaker.foundation import progress
        return progress.cancelled()
    except Exception:
        return False


# --- assembled-bytes cache -----------------------------------------------------------
# Assembling MASM is the whole cost: each function boots/leases a full-system DOSBox and
# runs a 16-bit DOS assembler. But MASM is DETERMINISTIC -- identical .asm source + same
# assembler + same INCLUDE/LIB yields byte-identical .OBJ/.LST every time. So key the
# result on a hash of the source (+ the assembler dir) and reuse it: a re-verify or a
# whole-module relink whose .asm hasn't changed then splices cached bytes and boots ZERO
# DOSBoxes. One gzip'd file per function under the target's runtime dir (gitignored).
# Bump CACHE_VER if the CONF / assembler invocation changes in a way that alters output.
CACHE_VER = 1
_ASM_CACHE_DIR = None
_ASM_CACHE_RESOLVED = False


def _asm_cache_dir():
    """The persistent OBJ/LST cache dir (WT_ASM_CACHE, else <runtime_dir>/asm_obj_cache).
    Resolved once per process; None disables caching (behaves exactly as before)."""
    global _ASM_CACHE_DIR, _ASM_CACHE_RESOLVED
    if _ASM_CACHE_RESOLVED:
        return _ASM_CACHE_DIR
    _ASM_CACHE_RESOLVED = True
    if os.environ.get("WT_NO_ASM_CACHE") == "1":
        return None
    d = os.environ.get("WT_ASM_CACHE")
    if not d:
        try:
            from codebreaker.foundation import layout
            d = os.path.join(layout.runtime_dir(), "asm_obj_cache")
        except Exception:
            d = os.path.join(tempfile.gettempdir(), "wt_asm_obj_cache")
    try:
        os.makedirs(d, exist_ok=True)
    except OSError:
        d = None
    _ASM_CACHE_DIR = d
    return d


def _cache_key(src, masmroot):
    tag = "%d\0%s" % (CACHE_VER, os.path.basename(os.path.normpath(masmroot or "")))
    return hashlib.sha1((tag + "\0" + src).encode("latin1", "replace")).hexdigest()


def _cache_get(cdir, key):
    if not cdir:
        return None
    try:
        blob = zlib.decompress(open(os.path.join(cdir, key + ".z"), "rb").read())
    except (OSError, zlib.error):
        return None
    n = int.from_bytes(blob[:4], "little")
    return {"obj": bytes(blob[4:4 + n]), "lst": blob[4 + n:].decode("latin1"), "log": ""}


def _cache_put(cdir, key, res):
    if not cdir or not res.get("obj"):        # never cache an empty/failed assembly
        return
    obj = res["obj"]
    blob = len(obj).to_bytes(4, "little") + obj + (res.get("lst") or "").encode("latin1", "replace")
    p = os.path.join(cdir, key + ".z")
    tmp = "%s.%d.tmp" % (p, os.getpid())
    try:
        with open(tmp, "wb") as f:
            f.write(zlib.compress(blob, 6))
        os.replace(tmp, p)                    # atomic publish (safe under the worker pool)
    except OSError:
        try:
            os.remove(tmp)
        except OSError:
            pass


def assemble_many(items, timeout=None, masmroot=None):
    """Assemble MANY (stem, asm_src) pairs, using the persistent assembled-bytes cache.
    Each item whose .asm source is already cached is served instantly (no DOSBox); only
    the misses are actually assembled, then cached. A fully-cached call boots ZERO
    DOSBoxes -- this is what makes re-verifies and whole-module relinks near-instant.
    Returns {"work": dir, "results": {stem: {obj,lst,log}}} (or {"aborted": True})."""
    masm = masmroot or MASMROOT
    cdir = _asm_cache_dir()
    hits, misses, mkeys = {}, [], {}
    for stem, src in items:
        key = _cache_key(src, masm)
        c = _cache_get(cdir, key)
        if c is not None:
            hits[stem] = c
        else:
            misses.append((stem, src))
            mkeys[stem] = key
    if not misses:                             # everything cached: no assembler run at all
        return {"work": "", "results": hits}
    out = _assemble_uncached(misses, timeout=timeout, masmroot=masm)
    if out.get("aborted"):
        return out
    fresh = out.get("results", {})
    for stem, _src in misses:                  # persist newly-assembled results
        r = fresh.get(stem)
        if r:
            _cache_put(cdir, mkeys[stem], r)
    fresh.update(hits)                          # merge cache hits back in
    return {"work": out.get("work", ""), "results": fresh}


def _assemble_uncached(items, timeout=None, masmroot=None):
    """Assemble (stem, asm_src) pairs WITHOUT the cache -- the real MASM run. By default
    leases a WARM DOSBox worker (no re-boot); falls back to a fresh boot if the pool is
    off/unavailable. Stems must be unique and DOS 8.3-safe (callers use F0..FN)."""
    masm = masmroot or MASMROOT
    if not os.path.exists(DBX):
        raise RuntimeError("dosbox-x not found: %s (set it: CodeBreaker config dosbox_x <path>)" % DBX)
    if not os.path.exists(os.path.join(masm, "BIN", "MASM.EXE")):
        raise RuntimeError("MASM.EXE not found under %s\\BIN (set it: CodeBreaker config masm <path>)" % masm)

    # Chunk large modules so no single DOSBox dispatch can overflow the [autoexec]
    # buffer (E_Exit) on the cold-boot path. Safe for the warm path too (same session
    # handles each chunk). Results merge; an abort in any chunk aborts the whole call.
    if len(items) > ASM_BATCH:
        chunks = [items[b:b + ASM_BATCH] for b in range(0, len(items), ASM_BATCH)]
        merged = {}
        # Fan chunks across the warm pool: each sub-call leases its OWN worker, so a single
        # large call (e.g. a whole-module relink) uses all POOL_MAX DOSBoxes instead of one
        # serially. The pool semaphore still caps total live workers GLOBALLY, so concurrent
        # module-level callers (conduct) can't oversubscribe -- they just share the same cap.
        # Serial when the warm pool is off (cold-boot fallback keeps one boot at a time).
        nthreads = min(POOL_MAX, len(chunks)) if WARM_POOL else 1
        if nthreads <= 1:
            for ch in chunks:
                r = _assemble_uncached(ch, timeout=timeout, masmroot=masm)
                if r.get("aborted"):
                    return {"work": "", "aborted": True}
                merged.update(r.get("results", {}))
            return {"work": "", "results": merged}
        import concurrent.futures as _cf
        aborted = False
        with _cf.ThreadPoolExecutor(max_workers=nthreads) as ex:
            futs = [ex.submit(_assemble_uncached, ch, timeout, masm) for ch in chunks]
            for fu in _cf.as_completed(futs):
                r = fu.result()
                if r.get("aborted"):
                    aborted = True
                else:
                    merged.update(r.get("results", {}))
        return {"work": "", "aborted": True} if aborted else {"work": "", "results": merged}

    if WARM_POOL:
        try:
            pool = _get_pool(masm)
            worker = pool.lease()
        except Exception:
            worker = pool = None
        if worker is not None:
            res = worker.assemble_many(items, timeout=timeout, cancelled=_cancelled)
            if res.get("aborted"):
                pool.release(worker, keep=False)
                return {"work": "", "aborted": True}
            if not res.get("dead"):
                worker._uses = getattr(worker, "_uses", 0) + 1
                # Retire the worker before it degrades (see WORKER_MAX_USES); keep=False drops
                # it so the pool boots a fresh one next lease. work="" so the caller's rmtree is
                # a harmless no-op — the worker owns and reuses its own dir across batches.
                keep = WORKER_MAX_USES <= 0 or worker._uses < WORKER_MAX_USES
                pool.release(worker, keep=keep)
                return {"work": "", "results": res["results"]}
            pool.release(worker, keep=False)     # dead: drop it, cold-boot below
    if timeout is None:
        timeout = 120 + 3 * len(items)
    work = tempfile.mkdtemp(prefix="masm_")
    for stem, src in items:
        with open(os.path.join(work, stem + ".ASM"), "w", newline="\r\n") as f:
            f.write(src)
    cmd = "\n".join("MASM %s.ASM, %s.OBJ, %s.LST, NUL; > %s.LOG 2>&1"
                    % (s, s, s, s) for s, _ in items)
    conf = os.path.join(work, "masm.conf")
    with open(conf, "w", newline="\r\n") as f:
        f.write(CONF.format(masm=masm, work=work, cmd=cmd))
    env = dict(os.environ, SDL_VIDEODRIVER="dummy")
    try:
        from codebreaker.foundation import progress as _pr
        aborted = _pr.cancelled
    except Exception:
        aborted = lambda: False
    proc = subprocess.Popen([DBX, "-conf", conf, "-nolog"], env=env,
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    if _wait_dosbox(proc, work, timeout, aborted) == "aborted":
        return {"work": work, "aborted": True}
    results = {}
    for stem, _src in items:
        d = {}
        for ext, key in ((".OBJ", "obj"), (".LST", "lst"), (".LOG", "log")):
            p = os.path.join(work, stem + ext)
            if os.path.exists(p):
                d[key] = open(p, "rb").read() if ext == ".OBJ" else \
                    open(p, encoding="latin1").read()
        results[stem] = d
    return {"work": work, "results": results}


def functions(asm_src, flags=None):
    """Assemble and return {name: (bytes, mask)} via objextract (same as cc16)."""
    from codebreaker.format_loaders import objextract
    r = assemble(asm_src)
    if "obj" not in r:
        raise RuntimeError("MASM produced no OBJ:\n" + r.get("log", "")[:600])
    return objextract.Obj(r["obj"]).functions()


SELF_ASM = """\
_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT
        public  _addasm
_addasm proc near
        push    bp
        mov     bp, sp
        mov     ax, [bp+4]
        add     ax, [bp+6]
        pop     bp
        ret
_addasm endp
_TEXT   ends
        end
"""


def _selftest():
    from codebreaker.format_loaders import objextract
    r = assemble(SELF_ASM)
    if "obj" not in r:
        print("no OBJ; MASM log:\n" + r.get("log", "")[:800])
        return False
    fns = objextract.Obj(r["obj"]).functions()
    print("functions:", {k: len(v[0]) for k, v in fns.items()})
    # MASM upper-cases public symbols by default (_ADDASM); match case-insensitively
    key = next((k for k in fns if k.lower() == "_addasm"), None)
    b = fns.get(key, (b"", set()))[0] if key else b""
    print("_addasm bytes:", b.hex())
    # 55 8b ec  8b 46 04  03 46 06  5d c3  = push bp;mov bp,sp;mov ax,[bp+4];add;pop;ret
    ok = b.startswith(bytes.fromhex("558bec")) and bytes.fromhex("034606") in b \
        and b.endswith(b"\xc3")
    print("\nSELFTEST:", "PASS - MASM assembled byte-exact proc" if ok else "FAIL")
    return ok


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("asm", nargs="?", help="path to a .ASM file")
    ap.add_argument("--selftest", action="store_true")
    args = ap.parse_args()
    if args.selftest:
        sys.exit(0 if _selftest() else 1)
    if not args.asm:
        ap.error("give a .ASM file or --selftest")
    from codebreaker.format_loaders import objextract
    r = assemble(open(args.asm).read())
    if "obj" not in r:
        sys.exit("no OBJ produced; MASM log:\n" + r.get("log", "")[:1000])
    obj = objextract.Obj(r["obj"])
    for name, cls in obj.segs:
        b = obj.seg_bytes.get(name, b"")
        print("  %-12s class=%-8s %6d bytes" % (name, cls, len(b)))
    print("# work dir: %s" % r["work"])


if __name__ == "__main__":
    main()
