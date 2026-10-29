"""win16.py -- the 16-bit module build as steps of the engine: describe, decompile, compile, gate, link.

    python -m codebreaker build <module> --engine      (or CB_ENGINE=1: module.build goes here)

    from codebreaker.engine import engine, win16
    b = win16.build("USER.EXE")                        # -> module.Built, b.image the linked file
    db = engine.open(path); engine.get(db, "ne.image")

The facts, and what each is made from (a key is a function's address in the original, "1:7cd4"):

    binary                the file (given)
    ne.segments           its segment table                                   <- binary
    ne.module             the module decompiled and made compilable (describe, the decompiler,
                          c_text: verify/lanes/win16.prepare) and its description <- binary
    ne.description        the description's lines: what was said of the module before it was
                          decompiled (the file the native decompiler reads is written from them)
                                                                              <- ne.module
    ne.functions          every function: address, name, segment, near, its object's stem
                                                                              <- ne.module
    ne.header             the header every function compiles with           <- ne.module
    ne.lines              the description's line for each function           <- ne.module
    function.c   <key>    the function's C                                   <- ne.module, ne.functions
    function.notes <key>  what the decompiler said of it besides its C (decompiler.split_notes:
                          its pointers' sizes, the globals it touches, the structures it
                          recovered)                                          <- ne.module, ne.functions
    function.l2  <key>    its L2 document (CB_C_FROM_L2), or None              <- function.notes
    function.source <key> the C as the build compiles it: calls left as addresses named, its
                          segment's code segment, the compiler's options     <- function.c, ne.functions
    function.object <key> the object the compiler makes of it (or why it would not, and the
                          stand-in's object)                                  <- function.source, ne.header
    function.never_set <key>  what it reads and never sets (the compiler's flow check)
                                                                              <- function.source, ne.header
    function.contract <key>   the description's lines for it and for each function it calls
                                                                              <- function.c, ne.lines
    function.gate <key>   run beside the original's (verify/gate16/isolated16): same, or how it
                          differs                                             <- binary, ne.header,
                                                                                 function.c, function.contract
    ne.image              the linked module and its report (build/module._build16, as
                          module.build gives it)                              <- all of the above

The stages are module.build's own, called as they are; the steps only say what each is made
from. What is particular to 16-bit Windows they ask the platform for (codebreaker/platforms/win16.py):
the segments of the file (its loader), the compiler, its options and environment (its compiler),
and the link (its packager) -- the steps of another platform would be the same steps asking its
slots. So a function's C changed (by hand: engine.put(db, "function.c", key, text)) makes again
that function's source, object, flow check and gate verdict, and the link -- nothing else.

The link is module._build16, with the compiler's work handed to it: while it runs, a function it
compiles with the options and text a function.object fact was made from gets that fact's object
(and its flow check the function.never_set fact's), and the compiler is not started. Anything
else it compiles itself (the data, the stand-ins it decides on, a function moved to a segment
of its own) -- so if the options here ever drift from _build16's, the link is slower, never
wrong. The gate is run as module.build runs it: the verdicts already known first, the
functions rebuilt from C then run beside the original's in the build just made, and the module
linked once more when some differ.
"""
import contextlib
import copy
import glob
import hashlib
import os
import shutil
import threading

from codebreaker import platforms
from codebreaker.base import cache
from codebreaker.build import functions, module
from codebreaker.engine import engine

step = engine.step
PLATFORM = "win16"
P = platforms.named(PLATFORM)                           # its six slots: loader, cpu, ... packager
_HERE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))       # codebreaker/
_real_compile, _real_never_set = P.compiler.compile.target(), functions.never_set
# (the options module._build16 compiles a function with, before its code segment's -nt=: the
# medium model, the 8087, for size, the code class it lays out first)
CFLAGS = [f for f in P.compiler.options() if f != "-ms"] + ["-mm", "-fpi87", "-os", "-nc=MCODE"]
SWITCHES = ("CB_ASM_ONLY", "CB_ASM_SEGMENTS", "CB_FORCE_C", "CB_ASM_KEEP_C", "CB_NO_GATE", "CB_NO_GUIDED")


def _sources(*patterns):
    """A digest of the source files under codebreaker/ that match the patterns."""
    h = hashlib.sha1()
    for pat in patterns:
        for p in sorted(glob.glob(os.path.join(_HERE, pat))):
            with open(p, "rb") as f:
                h.update(f.read())
    return h.hexdigest()


def _wcc():
    return P.compiler.tool() or ""


# --- the module -------------------------------------------------------------------------------
@step("ne.segments", reads=("binary",), platform=PLATFORM)
def ne_segments(ctx, key):
    """The segment table: number, kind, where in the file, length, flags (the loader's parts)."""
    return [{k: s[k] for k in ("idx", "kind", "file_off", "length", "flags", "minalloc")}
            for s in P.loader.parts(ctx.need("binary")["path"])]


def _module_stamp():
    from codebreaker.base import settings
    from codebreaker.decompile import decompiler
    switches = [os.environ.get(n, "") for n in ("CB_C_FROM_L2", "CB_SELECTOR_STRUCTS", "CB_CODE_DATA",
                                                 "CB_CALLER_ARGS", "CB_FASTCALL", "CB_INLINE_HELPERS",
                                                 "CB_PROTOTYPES", "CB_CALLER_RESULTS")]
    return (cache.tool(decompiler._exe()), decompiler._language_stamp(),
            (settings.get("struct_recovery") or "off").strip().lower(), switches,
            _sources("translate/backends/c.py") if os.environ.get("CB_C_FROM_L2") else "")


def _uses_module():
    from codebreaker.decompile import c_text, decompiler, describe
    from codebreaker.symbols import api_arguments
    from codebreaker.verify.lanes import win16 as lane
    return c_text, decompiler, describe, api_arguments, lane


@step("ne.module", reads=("binary",), platform=PLATFORM, uses=_uses_module(), stamp=_module_stamp)
def ne_module(ctx, key):
    """The module decompiled and made compilable, as module.build prepares it (the lane's
    prepare: describe16, the decompiler, c_text). -> (Module, the description's lines)"""
    from codebreaker.verify.lanes import win16 as lane
    m = lane.prepare(ctx.need("binary")["path"], ctx.work("decompiled"))
    return m, list(m.description.lines)


@step("ne.description", reads=("ne.module",), platform=PLATFORM)
def ne_description(ctx, key):
    """The description's lines (F a function, X an import, O a call's convention, bytes ...)."""
    return ctx.need("ne.module")[1]


@step("ne.functions", reads=("ne.module",), platform=PLATFORM)
def ne_functions(ctx, key):
    """Every function: {key: {va, name, seg, off, near, stem}} -- stem the name of its object in
    its segment's folder, as module._build16 names it (f0000 ... in the order of the segment)."""
    m = ctx.need("ne.module")[0]
    out, by_segment = {}, {}
    for va, name in sorted(m.by_va.items()):
        where = m.where.get(va)
        out[m.key[name]] = {"va": va, "name": name, "seg": where[0] if where else None,
                            "off": where[1] if where else None, "near": name in m.near}
        if where:
            by_segment.setdefault(where[0], []).append((where[1], m.key[name]))
    for seg, offs in by_segment.items():
        for i, (_off, k) in enumerate(sorted(offs)):
            out[k]["stem"] = "f%04d" % i
    return out


@step("ne.header", reads=("ne.module",), platform=PLATFORM)
def ne_header(ctx, key):
    """The header of types and prototypes every function is compiled with."""
    return ctx.need("ne.module")[0].header


@step("ne.lines", reads=("ne.description",), platform=PLATFORM)
def ne_lines(ctx, key):
    """{name: its line in the description} (F and X lines): how the gate answers a call to it."""
    out = {}
    for line in ctx.need("ne.description"):
        p = line.split("\t")
        if p[0] in ("F", "X") and len(p) > 2:
            out[p[2]] = line
    return out


# --- each function ----------------------------------------------------------------------------
@step("function.c", reads=("ne.module", "ne.functions"), platform=PLATFORM)
def function_c(ctx, key):
    """The function's C, as the decompiler and c_text made it (or as given by hand)."""
    name = ctx.need("ne.functions")[key]["name"]
    return ctx.need("ne.module")[0].texts.get(name)


@step("function.notes", reads=("ne.module", "ne.functions"), platform=PLATFORM)
def function_notes(ctx, key):
    """What the decompiler said of the function besides its C (decompiler.split_notes), or None."""
    name = ctx.need("ne.functions")[key]["name"]
    return (getattr(ctx.need("ne.module")[0], "notes", None) or {}).get(name)


@step("function.l2", reads=("function.notes",), platform=PLATFORM)
def function_l2(ctx, key):
    """The function's L2 document, where the decompiler wrote one (CB_C_FROM_L2), else None."""
    return (ctx.need("function.notes", key) or {}).get("l2")


def _named(text, seg, funcs):
    """module._build16's `named`: a call the decompiler left as an address (`func_0x...`) goes to
    the function reconstructed there, when it can be reached from this segment."""
    by_va = {f["va"]: f for f in funcs.values()}

    def fix(mm):
        target = by_va.get(int(mm.group(1), 16))
        if target and target["seg"] is not None and (not target["near"] or target["seg"] == seg):
            return target["name"]
        return mm.group(0)
    return module._BY_ADDRESS.sub(fix, text)


@step("function.source", reads=("function.c", "ne.functions"), platform=PLATFORM, uses=(module, _named),
      stamp=lambda: CFLAGS)
def function_source(ctx, key):
    """What the build compiles for the function: {text, seg, stem, flags}."""
    funcs = ctx.need("ne.functions")
    f = funcs[key]
    text = ctx.need("function.c", key)
    if text is None or f["seg"] is None:
        return None
    return {"text": _named(text, f["seg"], funcs), "seg": f["seg"], "stem": f["stem"],
            "flags": CFLAGS + ["-nt=CODE%d" % f["seg"]]}


_FOLDERS = {}
_FOLDER_LOCK = threading.Lock()


def _folder(ctx, seg, header):
    """The folder a segment's functions are compiled in, its header beside them."""
    d = ctx.work("objects", "s%02d" % seg)
    want = cache.key(header)
    with _FOLDER_LOCK:
        if _FOLDERS.get(d) != want:
            with open(os.path.join(d, "m.h"), "w", newline="\n", encoding="utf-8") as fh:
                fh.write(header)
            _FOLDERS[d] = want
    return d


def _read(path):
    with open(path, "rb") as f:
        return f.read()


@step("function.object", reads=("function.source", "ne.header"), platform=PLATFORM,
      uses=(functions, _folder, _read), stamp=lambda: cache.tool(_wcc()))
def function_object(ctx, key):
    """The object file the compiler makes of the function: {flags, text, obj} -- or, when it is
    rejected, {error, stand_in: the object of a stand-in with its signature (or None)}."""
    src = ctx.need("function.source", key)
    if src is None:
        return None
    header = ctx.need("ne.header")
    d = _folder(ctx, src["seg"], header)
    env = P.compiler.env()
    out = {"flags": src["flags"], "text": src["text"]}
    why = _real_compile(_wcc(), src["flags"], env, d, src["stem"], src["text"])
    if why:
        out["error"] = why
        out["stand_in_text"] = functions.stand_in(src["text"])
        out["stand_in"] = None
        if not _real_compile(_wcc(), src["flags"], env, d, src["stem"], out["stand_in_text"]):
            out["stand_in"] = _read(os.path.join(d, src["stem"] + ".obj"))
    else:
        out["obj"] = _read(os.path.join(d, src["stem"] + ".obj"))
    return out


@step("function.never_set", reads=("function.source", "ne.header"), platform=PLATFORM,
      uses=(functions, _folder), stamp=lambda: (cache.tool(_wcc()), CFLAGS))
def function_never_set(ctx, key):
    """The variables the function reads and never sets (functions.never_set: the compiler's flow
    check), with the options and text it was asked of: {flags, text, names}."""
    src = ctx.need("function.source", key)
    if src is None:
        return None
    d = _folder(ctx, src["seg"], ctx.need("ne.header"))
    stem = "n" + key.replace(":", "_")                       # (only its scratch file's name)
    return {"flags": CFLAGS, "text": src["text"],
            "names": _real_never_set(_wcc(), CFLAGS, P.compiler.env(), d, stem, src["text"])}


@step("function.contract", reads=("function.c", "ne.functions", "ne.lines"), platform=PLATFORM)
def function_contract(ctx, key):
    """The description's lines for the function and each it calls -- the gate answers a call as
    the description says the callee does, so a callee described anew is a verdict to make again
    (as module._gate16_keys keys a verdict)."""
    import re
    name = ctx.need("ne.functions")[key]["name"]
    lines = ctx.need("ne.lines")
    called = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", ctx.need("function.c", key) or "")) | {name}
    return sorted(lines[c] for c in called if c in lines)


def _gate_stamp():
    from codebreaker.symbols import api_arguments
    from codebreaker.verify import lane_common
    sizes = hashlib.sha1(api_arguments.sizes16()).hexdigest()
    from codebreaker.emulate import pcode
    from codebreaker.verify.lanes import win16 as lane
    ready = lane.concolic_ready()                            # (the gate's inputs found path by path)
    return (cache.tool(_wcc()), P.compiler.options(), sizes, lane_common.guided_on(), lane_common.GUIDED_BUDGET,
            lane_common.MAX_GUIDED, os.environ.get("CB_NO_GUIDED", ""), ready, lane_common.PATH_BUDGET,
            lane_common.MAX_PATHS, pcode.stamp() if ready else "")


def _uses_gate():
    from codebreaker.emulate import pcode, shadow, symbolic, windows16
    from codebreaker.symbols import api_arguments
    from codebreaker.verify import concolic, guided, lane_common
    from codebreaker.verify.gate16 import isolated16
    from codebreaker.verify.lanes import win16 as lane
    return isolated16, windows16, lane, api_arguments, guided, lane_common, concolic, shadow, symbolic, pcode


@step("function.gate", reads=("binary", "ne.header", "function.c", "function.contract"), platform=PLATFORM,
      uses=_uses_gate(), stamp=_gate_stamp, batch=True)
def function_gate(ctxs, hint):
    """The function's C run beside the original's (verify/gate16/isolated16.py): ["same", ""] or
    ["differs", how]. Made in a build (the hint: its folder and the gate's tables), so it is
    asked for by ne.image; one that could not be run is not kept."""
    from codebreaker.verify.gate16 import isolated16
    for k, ctx in ctxs.items():
        ctx.need("binary"), ctx.need("ne.header"), ctx.need("function.c", k), ctx.need("function.contract", k)
    if hint is None:
        raise LookupError("a gate verdict is made in a build of the module: ask for ne.image")
    path = next(iter(ctxs.values())).need("binary")["path"]
    by_name = {hint["names"][k]: k for k in ctxs}
    out = {}
    for n, verdict, why in isolated16.check(path, hint["folder"], sorted(by_name), tables=hint["tables"]):
        if n not in by_name:
            continue
        out[by_name[n]] = ["same", ""] if verdict == "same" else ["differs", why or ""] if verdict == "differs" \
            else engine.Unkept(["not run", why or ""])
    for k in ctxs:
        out.setdefault(k, engine.Unkept(["not run", "the checker gave no answer"]))
    return out


# --- the linked module ------------------------------------------------------------------------
_SERVED = {}                    # (header digest, options, text) -> ("obj", bytes) | ("error", why)
_NEVER = {}                     # (header digest, options, text) -> [name, ...]
_PATCHED = [0, {"served": 0, "compiled": 0}]
_PATCH_LOCK = threading.Lock()


def _served_compile(cc, cflags, env, outdir, stem, text):
    got = _SERVED.get((functions._HEADER.get(outdir), tuple(cflags), text))
    if got is None:
        _PATCHED[1]["compiled"] += 1
        return _real_compile(cc, cflags, env, outdir, stem, text)
    _PATCHED[1]["served"] += 1
    if got[0] == "error":
        return got[1]
    with open(os.path.join(outdir, stem + ".c"), "w", newline="\n", encoding="utf-8") as f:
        f.write('#include "m.h"\n\n' + text + "\n")
    with open(os.path.join(outdir, stem + ".obj"), "wb") as f:
        f.write(got[1])
    return ""


def _served_never_set(cc, cflags, env, outdir, stem, text):
    got = _NEVER.get((functions._HEADER.get(outdir), tuple(cflags), text))
    return list(got) if got is not None else _real_never_set(cc, cflags, env, outdir, stem, text)


@contextlib.contextmanager
def _serving(header, objects, never_sets):
    """module._build16 handed the engine's objects and flow checks while it runs."""
    hk = cache.key(header)
    mine = {}
    for o in objects:
        if o is None:
            continue
        if "error" in o:
            mine[(hk, tuple(o["flags"]), o["text"])] = ("error", o["error"])
            if o.get("stand_in") is not None:
                mine[(hk, tuple(o["flags"]), o["stand_in_text"])] = ("obj", o["stand_in"])
        else:
            mine[(hk, tuple(o["flags"]), o["text"])] = ("obj", o["obj"])
    nev = {(hk, tuple(n["flags"]), n["text"]): n["names"] for n in never_sets if n is not None}
    with _PATCH_LOCK:
        _SERVED.update(mine)
        _NEVER.update(nev)
        if not _PATCHED[0]:
            functions._compile, functions.never_set = _served_compile, _served_never_set
        _PATCHED[0] += 1
    try:
        yield _PATCHED[1]
    finally:
        with _PATCH_LOCK:
            _PATCHED[0] -= 1
            if not _PATCHED[0]:
                functions._compile, functions.never_set = _real_compile, _real_never_set
                _SERVED.clear()
                _NEVER.clear()


def _image_stamp():
    return (_sources("build/*.py", "analyze/origin.py", "verify/gate16/contract16.py", "decompile/c_text.py",
                     "decompile/describe.py", "formats/ne.py", "formats/omf.py", "formats/resources.py",
                     "formats/detect.py"),
            [cache.tool(t) for t in P.packager.tools().values()],
            cache.tool(P.compiler.library()),
            [os.environ.get(n, "") for n in SWITCHES])


def _keepable(b):
    """The Built as it can be kept: what does not pickle left out."""
    import pickle
    try:
        pickle.dumps(b, protocol=4)
        return b
    except Exception:
        pass
    k = copy.copy(b)
    for name in list(vars(k)):
        try:
            pickle.dumps(getattr(k, name), protocol=4)
        except Exception:
            setattr(k, name, None)
    return k


@step("ne.image", platform=PLATFORM, stamp=_image_stamp,
      uses=(module, _serving, _served_compile, _served_never_set, _keepable),
      reads=("binary", "ne.module", "ne.description", "ne.functions", "function.c", "function.object", "function.never_set",
             "function.gate"))
def ne_image(ctx, key):
    """The module linked: {image: its bytes, report, gated, gate_set, rejected, built: the Built}
    -- module.build's build of a 16-bit module, made of the facts above."""
    from codebreaker.verify.gate16 import contract16, isolated16
    path = ctx.need("binary")["path"]
    m0, _lines = ctx.need("ne.module")
    funcs = ctx.need("ne.functions")
    keys = sorted(k for k, f in funcs.items() if f["seg"] is not None)
    texts = ctx.need_many("function.c", keys)
    objects = ctx.need_many("function.object", keys)
    never = ctx.need_many("function.never_set", keys)
    ctx.file(module.KEPT_AS_ASM)
    folder = ctx.work("build")
    m = copy.deepcopy(m0)
    os.makedirs(os.path.join(folder, "decompiled"), exist_ok=True)
    m.description.lines = ctx.need("ne.description")
    m.description.write(os.path.join(folder, "decompiled", "description.txt"))     # (for the decompiler)
    for k in keys:
        if texts[k] is not None:
            m.texts[funcs[k]["name"]] = texts[k]
    key_of = {f["name"]: k for k, f in funcs.items()}
    # the verdicts already known, of the functions the gate ran last time (module._gate16_known)
    gated = {}
    for n in (ctx.previous() or {}).get("gate_set", []):
        v = ctx.peek("function.gate", key_of[n]) if n in key_of else None
        if v and v[0] == "differs":
            gated[n] = v[1]
    gate_now, gate_set, served = not os.environ.get("CB_NO_GATE"), [], {}
    tables = {"funcs": isolated16._functions(m), "wide": sorted(m.wide),
              "place": {n: list(m.where[va]) for va, n in m.by_va.items() if m.where.get(va)}}
    while True:
        b = module.Built(path)
        b.gated, b.folder, b.functions = dict(gated), folder, len(m.all_keys)
        mm = copy.deepcopy(m)                                # (_build16 adds to its segments)
        with _serving(m.header, objects.values(), never.values()) as count:
            before = dict(count)
            image = P.packager.link(b, path, mm, folder)
            served = {k: count[k] - before[k] for k in count}
        if getattr(b, "resident_overflow", None):            # (as module.build: again, and not gated)
            gated, gate_now = dict(b.gated, **b.resident_overflow), False
            continue
        if not image:
            break
        b.image = os.path.join(folder, os.path.basename(path).lower())
        shutil.copyfile(image, b.image)
        module._whole_file_image(path, b.image)
        if not gate_now:
            break
        gate_now = False
        c = sorted(b.placed_c | getattr(b, "moved_c", set()))
        verdicts = ctx.need_many("function.gate", [key_of[n] for n in c if n in key_of],
                                 hint={"folder": folder, "tables": tables, "names": {key_of[n]: n for n in c}})
        differ = {n: verdicts[key_of[n]][1] for n in c if n in key_of and verdicts[key_of[n]][0] == "differs"}
        failed = [(n, verdicts[key_of[n]][1]) for n in c if n in key_of and verdicts[key_of[n]][0] == "not run"
                  and verdicts[key_of[n]][1].startswith("the checker failed")]
        if failed:
            b.gate_error = "%d functions (%s)" % (len(failed), failed[0][1][:120])
        try:
            for n, why in contract16.mismatches(path, folder, b.image, c).items():
                differ.setdefault(n, "its calls are not the original's: " + why)
        except Exception as e:
            b.contract_error = "%s: %s" % (type(e).__name__, e)
        gate_set = sorted(set(c) | set(gated))
        if differ:
            gated = dict(gated, **differ)
            continue
        break
    out = {"name": b.name, "gated": dict(getattr(b, "gated", {})), "gate_set": gate_set,
           "rejected": dict(b.rejected), "served": served, "image": None}
    if b.image:
        module.check(b)
        with open(b.image, "rb") as f:
            out["image"] = f.read()
    out["report"] = b.report()
    out["built"] = _keepable(b)
    return out


# --- module.build on the engine ---------------------------------------------------------------
def handles(path):
    return platforms.identify(path) is P


def build(module_name, outdir=None, call=True, log=print):
    """module.build of a 16-bit module, made by the engine. -> Built (b.image in `outdir`, else
    the database's build folder)."""
    import time
    from codebreaker.base import corpus
    path = corpus.retail(module_name)
    if not path:
        raise SystemExit("no module %r in the corpus" % module_name)
    t = time.time()
    db = engine.open(path)
    log("%s: on the engine (%s)" % (os.path.basename(path).lower(), db.path))
    got = engine.get(db, "ne.image", log=log)
    made = engine.last_run(db)
    log("%s: %.1fs; facts kept/made: %s" % (os.path.basename(path).lower(), time.time() - t,
                                             ", ".join("%s %d/%d" % (k, v[0], v[1]) for k, v in sorted(made.items()))))
    b = got.get("built") or module.Built(path)
    if got.get("image"):
        where = outdir or facts_folder(db)
        os.makedirs(where, exist_ok=True)
        b.image = os.path.join(where, os.path.basename(path).lower())
        with open(b.image, "wb") as f:
            f.write(got["image"])
        b.folder = os.path.join(facts_folder(db))
    else:
        b.image = None
    if call and b.image:
        module.run(b)
    from codebreaker.engine import facts
    facts.hold()
    return b


def facts_folder(db):
    from codebreaker.engine import facts
    return facts.work(db, "build")
