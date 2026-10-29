"""lanes.py -- every lane's reconstruct-and-judge as steps of the engine, and the 32-bit module build.

    CB_ENGINE=1 python -m codebreaker measure <module>     (verify/reconstruct.run goes here)
    CB_ENGINE=1 python -m codebreaker build <module>       (a 32-bit module: module.build goes here)

    from codebreaker.engine import engine, lanes
    rows = lanes.judged(path)                      # -> Judged: the rows reconstruct.run records
    db = engine.open(path); engine.get(db, "lane.pe.rows")

A lane (verify/lanes/<kind>.py, named by each platform's descriptor) is four calls: profile,
prepare, build, judge (verify/reconstruct.py says what each gives). The same steps serve every
lane -- 32-bit Windows, 16-bit Windows' judge, the drivers, DOS, the Xbox 360, the consoles --
filed under the name its results are (K: pe, ne, le, dos, powerpc, nes, ...); each asks the lane
for the part that is its own:

    binary                    the file (given)
    lane.K.profile            the lane's profile: (digest, settings) -- everything but the source
                              a verdict depends on; asked every run      <- binary
    lane.K.module             the module decompiled (the lane's prepare) <- binary
    lane.K.keys               the keys of its functions with C, and of all <- lane.K.module
    lane.K.function <key>     one function: [name, C] -- or given by hand (engine.put): the
                              build and the rows then have that C        <- lane.K.module
    lane.K.texts              every function's name, signature and C as they now are, the header
                                                                         <- lane.K.module, lane.K.function
    lane.K.build              the functions built (the lane's build): the judge's context, each
                              function's item, why the others are not in it, and each function's
                              built signature (its compiled code and what it reaches)
                                                                         <- lane.K.module, lane.K.function
    lane.K.built <key>        what a function's verdict is reached for: its built signature, or
                              (a lane that builds the module as one) its signature; or why it is
                              not in the build                           <- lane.K.build
    lane.K.verdict <key>      the lane's judge: [verdict, reason, limits, detail]
                                                                         <- lane.K.built, lane.K.profile
    lane.K.rows               the rows reconstruct.run records in the function store
                                                                         <- all of the above

So a verdict is made again exactly when the store would judge the function again: its built
code (or, for a lane that does not build per function, its text) or the lane's profile changed.
A function whose C changed to no effect on its code keeps its verdict, as reconstruct.run kept it
by the store's `built` table. The build's files (the judge runs them) are in the database's work
folder; when they are gone, the build is made again there before a verdict is.

The 32-bit module build (module.build of a PE module, build/module._build32 through the packager):

    pe.image                  the module linked and checked: {image, report, built}
                                                                         <- lane.pe.module, lane.pe.function

A Java program is a folder of source, not a binary: its database is named by what its lane files
its results under (the lane's sha of the sources), and the folder is its fact `binary`.
"""
import copy
import hashlib
import os
import pickle
import time

from codebreaker import platforms
from codebreaker.base import cache
from codebreaker.engine import engine

step = engine.step
_HERE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))       # codebreaker/
# the code a module's decompiling and building may run (the judge's is the lane's profile)
_CODE = ("decompile", "translate", "symbols", "analyze", "formats", "knowledge", "source", "build", "java",
         "base", "platforms", "exact", "recompile", "xbox", "x360")
_SKIP = ("CB_ENGINE", "CB_LOW_LOAD", "CB_FUNCTIONS_DB", "CB_STATE", "CB_KNOWLEDGE_DB", "CB_NO_FP_CACHE",
         "CB_RESULTS_DB")
_SEEN = {}                      # (path, mtime, size) -> SHA-1 of the file, of this process


def _code():
    """A digest of the source the module's steps run: every .py under the folders of _CODE."""
    h = hashlib.sha1()
    for top in _CODE:
        for root, dirs, files in os.walk(os.path.join(_HERE, top)):
            dirs[:] = sorted(d for d in dirs if d != "__pycache__")
            for n in sorted(files):
                if n.endswith(".py"):
                    p = os.path.join(root, n)
                    st = os.stat(p)
                    k = (p, st.st_mtime_ns, st.st_size)
                    got = _SEEN.get(k)
                    if got is None:
                        with open(p, "rb") as f:
                            got = _SEEN[k] = hashlib.sha1(f.read()).hexdigest()
                    h.update(p[len(_HERE):].encode() + got.encode())
    return h.hexdigest()


def _file(module):
    """A digest of one module's file (the lane's own)."""
    import importlib
    path = importlib.import_module(module).__file__
    with open(path, "rb") as f:
        return hashlib.sha1(f.read()).hexdigest()


def _switches():
    return sorted((k, v) for k, v in os.environ.items() if k.startswith("CB_") and k not in _SKIP)


def _module_stamp(p):
    def stamp():
        from codebreaker.base import settings
        from codebreaker.decompile import decompiler
        return (_code(), _file(p.lane), _file("codebreaker.verify.reconstruct"), cache.tool(decompiler._exe()),
                decompiler._language_stamp(), decompiler.REV, (settings.get("struct_recovery") or "").lower(),
                _switches())
    return stamp


def _tools(p):
    try:
        tools = p.packager.tools() if p.packager.tools else {}
    except Exception:
        tools = {}
    if not isinstance(tools, dict):                          # (a list of programs, or one)
        tools = dict(enumerate(tools if isinstance(tools, (list, tuple)) else [tools]))
    return sorted((str(k), cache.tool(v)) for k, v in tools.items() if isinstance(v, str) and v)


def _build_stamp(p):
    def stamp():
        return (_code(), _file(p.lane), _tools(p), _switches())
    return stamp


def _kind(p):
    return p.lane_module()


def _work(ctx):
    return ctx.work("lane")


# --- the steps, one set per lane --------------------------------------------------------------
def _declare(p):
    K = p.name
    L = "lane.%s." % K

    @step(L + "profile", reads=("binary",), platform=p.id, always=True)
    def profile(ctx, key):
        """The lane's profile of the binary: [digest, settings] -- what its verdicts are filed under."""
        digest, settings = _kind(p).profile(ctx.need("binary")["path"])
        return [digest, settings]

    @step(L + "module", reads=("binary",), platform=p.id, stamp=_module_stamp(p))
    def module(ctx, key):
        """The module decompiled, as the lane prepares it (its prepare)."""
        return _kind(p).prepare(ctx.need("binary")["path"], _work(ctx))

    @step(L + "function", reads=(L + "module",), platform=p.id)
    def function(ctx, key):
        """One function: [name, C] (or given by hand: engine.put)."""
        m = ctx.need(L + "module")
        name = next((n for n in m.order if m.key.get(n) == key), None)
        return [name, m.texts.get(name)] if name is not None else None

    @step(L + "keys", reads=(L + "module",), platform=p.id)
    def keys(ctx, key):
        """The keys of the functions that have C, and of every function of the module."""
        m = ctx.need(L + "module")
        return {"keys": sorted({m.key[n] for n in m.order if n in m.key}), "all_keys": list(m.all_keys)}

    @step(L + "texts", reads=(L + "module", L + "keys", L + "function"), platform=p.id)
    def texts(ctx, key):
        """{all_keys, header, order, functions: {key: [name, signature, C]}}: the module's C as it is now
        (a function given by hand in it), and each function's signature -- what reconstruct.run
        files its verdict under (verify/reconstruct.signatures)."""
        m = _patched(ctx, L)
        kind = _kind(p)
        sig = reconstruct_signatures(m, kind)
        return {"all_keys": list(m.all_keys), "header": getattr(m, "header", "") or "",
                "order": [m.key[n] for n in m.order if n in m.key],
                "functions": {m.key[n]: [n, sig[n], m.texts.get(n)] for n in m.order if n in m.key}}

    @step(L + "build", reads=("binary", L + "module", L + "keys", L + "function"), platform=p.id,
          stamp=_build_stamp(p))
    def build(ctx, key):
        """The functions built for judging (the lane's build): {context, items, dropped, built}."""
        return _built(p, ctx.need("binary")["path"], _patched(ctx, L), _work(ctx))

    @step(L + "built", reads=(L + "build", L + "texts"), platform=p.id)
    def built(ctx, key):
        """What the function's verdict is reached for: ["built", signature] or ["dropped", why]."""
        b = ctx.need(L + "build")
        name = b["names"].get(key)
        if name is None or name not in b["items"]:
            why = b["dropped"].get(name, "") if name else ""
            return ["dropped", why or "not in the build"]
        if b["built"]:
            return ["built", b["built"].get(name)]
        return ["built", ctx.need(L + "texts")["functions"][key][1]]

    @step(L + "verdict", reads=(L + "built", L + "profile"), platform=p.id, batch=True)
    def verdict(ctxs, hint):
        """The function judged beside the original (the lane's judge, in worker processes):
        [verdict, reason, limits, detail]. Asked for by lane.K.rows, which hands it the build."""
        from codebreaker.jobs import pool
        from codebreaker.verify import reconstruct
        if hint is None:
            raise LookupError("a verdict is judged in a build of the module: ask for %srows" % L)
        out, todo = {}, {}
        for k, ctx in ctxs.items():
            b = ctx.need(L + "built", k)
            ctx.need(L + "profile")
            if b[0] == "dropped":
                out[k] = ["no-compile", b[1], "", ""]
            else:
                todo[k] = True
        if todo:
            b = hint["ready"]()
            judged = pool.run(_kind(p).judge, [b["items"][hint["names"][k]] for k in todo], context=b["context"],
                              crashed=lambda item: reconstruct.NOT_RUN)
            for k in todo:
                v, reason, limits, detail = judged.get(k, reconstruct.NOT_RUN)
                if hint["names"][k] in hint["tainted"] and v in reconstruct.GOOD:
                    limits = sorted(set(limits) | {"stub-callee"})   # a function it calls was a stand-in
                out[k] = [v, reason, ",".join(limits), detail]
        return out

    @step(L + "rows", platform=p.id,
          reads=("binary", L + "module", L + "keys", L + "texts", L + "function", L + "build", L + "built",
                 L + "verdict", L + "profile"))
    def rows(ctx, key):
        """The rows reconstruct.run records: {profile, settings, header, all_keys, rows, built}."""
        path = ctx.need("binary")["path"]
        t = ctx.need(L + "texts")
        fns = {k: f for k, f in t["functions"].items()}
        digest, settings = ctx.need(L + "profile")
        out = {"profile": digest, "settings": settings, "header": t["header"], "all_keys": t["all_keys"],
               "rows": [], "built": {}}
        have = set(fns)
        out["rows"] = [dict(addr=k, sig="-", name="", source=None, verdict="no-synth",
                            reason="the decompiler produced nothing for this function", problem="none")
                       for k in t["all_keys"] if k not in have]
        if not have:
            return out
        b = ctx.need(L + "build")
        mine = list(dict.fromkeys(t["order"]))     # (judged in the module's order, as reconstruct.run judges)
        ctx.need_many(L + "built", mine)
        names = {k: fns[k][0] for k in mine}

        def ready():
            """The build the judge runs: the kept one, or -- its files gone -- made again there."""
            if os.path.isfile(_mark(_work(ctx))):
                return b
            return _built(p, path, _patched(ctx, L), _work(ctx))
        hint = {"names": names, "tainted": set(b["tainted"]), "ready": ready}
        verdicts = ctx.need_many(L + "verdict", mine, hint=hint)
        for k in mine:
            n, sig, text = fns[k]
            v, reason, limits, detail = verdicts[k]
            row = dict(addr=k, sig=sig, name=n, source=text, verdict=v, reason=reason, limits=limits, detail=detail)
            if v == "no-compile":
                row["problem"] = "compile"
            out["rows"].append(row)
        out["built"] = {k: b["built"][n] for k, n in names.items() if b["built"] and n in b["built"]}
        return out

    return K


def _patched(ctx, L):
    """The module with each function's C as its fact now is (one given by hand in its place)."""
    m = ctx.need(L + "module")
    keys = ctx.need(L + "keys")["keys"]
    given = {}
    for k, f in ctx.need_many(L + "function", keys).items():
        if f is not None and m.texts.get(f[0]) != f[1]:
            given[f[0]] = f[1]
    if not given:
        return m
    m = copy.copy(m)
    m.texts = dict(m.texts, **given)
    m.sig = hashlib.sha1(("%s\0%s" % (m.sig, "".join("%s\0%s\0" % kv for kv in sorted(given.items()))))
                         .encode("utf-8", "replace")).hexdigest()
    return m


def reconstruct_signatures(m, kind):
    """{name: signature} as reconstruct.run gives them: per function for a lane that builds per
    function, else the module's own for each."""
    from codebreaker.verify import reconstruct
    if getattr(kind, "PER_FUNCTION", False):
        return reconstruct.signatures(m, m.sig)
    return {n: m.sig for n in m.order}


def _mark(work):
    return os.path.join(work, "built.mark")


def _built(p, path, m, work):
    """The lane's build, and what reconstruct.run reads of it: {context, items, dropped, names,
    built: {name: built signature} (lanes that build per function), tainted: [names]}."""
    from codebreaker.build import functions as builder
    from codebreaker.verify import reconstruct
    kind = _kind(p)
    try:
        os.remove(_mark(work))
    except OSError:
        pass
    context, items, dropped = kind.build(m, path, work)
    objects = builder.OBJECTS.get(work) if getattr(kind, "PER_FUNCTION", False) else None
    built = reconstruct.signatures(m, None, own=objects) if objects else {}
    tainted = sorted(reconstruct._reaches(m.texts, list(items), dropped))
    with open(_mark(work), "w") as f:
        f.write("%.0f\n" % time.time())
    return {"context": context, "items": items, "dropped": dict(dropped), "names": {v: k for k, v in m.key.items()},
            "built": built, "tainted": tainted}


KINDS = [_declare(p) for p in platforms.all() if p.lane]


# --- the 32-bit module build ------------------------------------------------------------------
_PE = platforms.named("win32")


def _image_stamp():
    return (_code(), _tools(_PE), [os.environ.get(n, "") for n in ("CB_ASM_ONLY", "CB_FORCE_C")])


def _keepable(b):
    """The Built as it can be kept: what does not pickle left out."""
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


@step("pe.image", platform="win32", stamp=_image_stamp,
      reads=("binary", "lane.pe.module", "lane.pe.keys", "lane.pe.function"))
def pe_image(ctx, key):
    """The module linked (module.build's build of a 32-bit module: the packager's link, then the
    check of the image against the original): {image: its bytes, report, built: the Built}."""
    import shutil
    from codebreaker.build import module
    path = ctx.need("binary")["path"]
    m = copy.deepcopy(_patched(ctx, "lane.pe."))           # (a function given by hand in its place)
    folder = ctx.work("build")
    b = module.Built(path)
    b.folder, b.functions = folder, len(m.all_keys)
    image = _PE.packager.link(b, path, m, folder)
    out = {"name": b.name, "image": None, "rejected": dict(b.rejected)}
    if image:
        b.image = os.path.join(folder, os.path.basename(path).lower())
        shutil.copyfile(image, b.image)
        module.check(b)
        with open(b.image, "rb") as f:
            out["image"] = f.read()
    out["report"] = b.report()
    out["built"] = _keepable(b)
    return out


# --- asking ---------------------------------------------------------------------------------------
class Judged(object):
    """What lane.K.rows gives, and how the run went: rows, profile, settings, header, all_keys,
    built, made {kind: [kept, made]}."""

    def __init__(self, got, made):
        self.__dict__.update(got)
        self.made = made


def on(path):
    """Is this module's lane on these steps?"""
    p = platforms.identify(path)
    return p is not None and bool(p.lane) and p.name in KINDS


def open_module(path, p=None):
    """The module's database: a binary's by its bytes (engine.open); a module that is a folder (a
    Java program's source) by what its lane files its results under (the lane's sha)."""
    from codebreaker.engine import facts
    if os.path.isfile(path):
        return engine.open(path)
    p = p or platforms.identify(path)
    sha = p.lane_module().sha(path)
    db = facts.open_at(os.path.join(facts.root(), sha + ".sqlite"))
    db.sha = sha
    if db.about("name") is None:
        db.about("name", os.path.basename(os.path.normpath(path)))
    db.about("path", os.path.abspath(path))
    value = {"path": os.path.abspath(path), "size": 0}
    row = db.row("binary", "")
    if row is None or row.hash != engine._binary_hash(db) or row.value() != value:
        engine._give_binary(db, value)
    return db


def judged(path, log=None):
    """reconstruct.run's decompile, build and judge of a module, made by the engine. -> Judged"""
    p = platforms.identify(path)
    db = open_module(path, p)
    got = engine.get(db, "lane.%s.rows" % p.name, log=log)
    return Judged(got, engine.last_run(db))


def build(path, outdir=None, call=True, log=print):
    """module.build of a module whose platform has these steps: a 32-bit module made by the engine.
    -> Built (b.image in `outdir`, else the database's build folder)."""
    from codebreaker.base import corpus
    from codebreaker.build import module
    from codebreaker.engine import facts
    path = corpus.retail(path) or path
    p = platforms.identify(path)
    if p is not _PE:
        raise SystemExit("%s: a %s module is not linked whole here" % (os.path.basename(path).lower(), p.title))
    t = time.time()
    db = engine.open(path)
    log("%s: on the engine (%s)" % (os.path.basename(path).lower(), db.path))
    got = engine.get(db, "pe.image", log=log)
    made = engine.last_run(db)
    log("%s: %.1fs; facts kept/made: %s" % (os.path.basename(path).lower(), time.time() - t,
                                             ", ".join("%s %d/%d" % (k, v[0], v[1]) for k, v in sorted(made.items()))))
    b = got.get("built") or module.Built(path)
    if got.get("image"):
        where = outdir or facts.work(db, "build")
        os.makedirs(where, exist_ok=True)
        b.image = os.path.join(where, os.path.basename(path).lower())
        with open(b.image, "wb") as f:
            f.write(got["image"])
    else:
        b.image = None
    if call and b.image:
        module.run(b)
    facts.hold()
    return b
