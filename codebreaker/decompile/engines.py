"""engines.py -- every decompiler, behind one interface.

A decompiler is anything that can turn the functions of a module into C. However it works
inside, the rest of CodeBreaker asks it two things:

    prepared = engine.prepare(module_path, addresses, workdir, log)
        prepared.source     {address: (name, C text)} for every function it produced
        prepared.sig        {address: digest of everything that function's build depends on}
        prepared.problem    {address: why there is no usable source}

    built = prepared.build(addresses)
        built.entry            {address: (dll, where that function is in it)}
        built.problem          {address: why it did not compile}
        built.weak             {address: limits on what a match can show, e.g. "stub-callee"}

Preparing is cheap enough to do on every run; building is done only for functions whose
digest has no verdict yet. Judging and recording are not an engine's business: one path does
them for all engines (codebreaker.verify.reconstruct).

    engines.get("core")       the Ghidra decompiler core embedded in CodeBreaker, fed our facts
    engines.get("native")     CodeBreaker's own decompiler
    engines.get("export")     Ghidra run headless (needs a Ghidra install; slow; a second opinion)
    engines.ORDER             the order they are tried in, and who wins a tie

Adding a decompiler is one class here.
"""
import hashlib
import os
import re

ORDER = ("core", "native", "export")        # preference between equally good results
DEFAULT = ("core", "native")                # tried when none is named; "export" on request


def _sha(*parts):
    h = hashlib.sha1()
    for p in parts:
        h.update(str(p).encode("utf-8", "replace"))
        h.update(b"\0")
    return h.hexdigest()


class Built(object):
    def __init__(self):
        self.entry, self.problem, self.weak = {}, {}, {}


class Prepared(object):
    def __init__(self):
        self.source, self.sig, self.problem = {}, {}, {}

    def build(self, addrs):
        raise NotImplementedError


# ---------------------------------------------------------------------------------------------
# Whole-module engines: the decompiler emits the module as one translation unit
# ---------------------------------------------------------------------------------------------
class _WholeModule(Prepared):
    def __init__(self, module_path, workdir, preamble, texts, order, by_va, used, cflags, rev, name):
        Prepared.__init__(self)
        self.module_path, self.workdir, self.preamble = module_path, workdir, preamble
        self.texts, self.order, self.by_va, self.used, self.cflags = texts, order, by_va, used, cflags
        whole = _sha(name, rev, preamble, "".join(texts[n] for n in order))
        for va, n in by_va.items():
            self.source[va] = (n, texts[n])
            self.sig[va] = whole                 # one unit: any change may change any function

    def build(self, addrs):
        from codebreaker.formats.pe import open_pe
        from codebreaker.build import whole_module
        out = Built()
        # Only functions with a known address are built. The rest are library routines the
        # decompiler recognised by signature (__allshl, _memcpy): they belong to the C runtime,
        # which is linked, and defining them again collides with it.
        placed = set(self.by_va.values())
        dll, built, dropped = whole_module.build(self.module_path, self.preamble, self.texts,
                                                 [n for n in self.order if n in placed], self.used,
                                                 self.workdir, cflags=self.cflags)
        ex, base = {}, 0
        if dll:
            pb = open_pe(dll)
            ex, base = pb.export_addrs(), pb.image_base
        # functions that reach a dropped one (now a stand-in), directly or through others
        tainted = set()
        if dropped:
            pat = re.compile(r"\b(%s)\s*\(" % "|".join(re.escape(d) for d in dropped))
            calls = {n: set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", self.texts[n])) & set(built) for n in built}
            tainted = {n for n in built if pat.search(self.texts[n].split("{", 1)[-1])}
            grew = True
            while grew:
                grew = False
                for n in built:
                    if n not in tainted and calls[n] & tainted:
                        tainted.add(n)
                        grew = True
        for va in addrs:
            n = self.by_va.get(va)
            if not n:
                continue
            if n in dropped or n not in ex:
                out.problem[va] = dropped.get(n, "") or "not in the build"
            else:
                out.entry[va] = (dll, base + ex[n])
                if n in tainted:
                    out.weak[va] = {"stub-callee"}
        return out


class Core(object):
    name = "core"

    def available(self):
        from codebreaker.decompile import ghidra_core
        return ghidra_core.available()

    def rev(self):
        from codebreaker.decompile import ghidra_core
        return str(ghidra_core.REV)

    def prepare(self, module_path, addrs, workdir, log=None):
        from codebreaker.decompile import ghidra_core
        from codebreaker.verify import pipeline
        every = pipeline.LANES["pe"].functions(module_path)   # its facts cover the whole module
        preamble, texts, order, by_va, used = ghidra_core.adapt(module_path, every, workdir)
        return _WholeModule(module_path, workdir, preamble, texts, order, by_va, used,
                            ghidra_core.CFLAGS, self.rev(), self.name)


class Export(object):
    name = "export"

    def available(self):
        try:
            from codebreaker.base import config
            return bool(os.environ.get("GHIDRA_HOME") or config.load_config().get("ghidra_home"))
        except Exception:
            return False

    def rev(self):
        from codebreaker.decompile import ghidra_export
        return str(ghidra_export.REV)

    def prepare(self, module_path, addrs, workdir, log=None):
        from codebreaker.decompile import ghidra_export as whole_module
        c_path = whole_module.export(module_path, log)
        if not c_path:
            return Prepared()
        preamble, texts, order, by_va, used = whole_module.adapt(module_path, c_path)
        return _WholeModule(module_path, workdir, preamble, texts, order, by_va, used, None,
                            self.rev(), self.name)


# ---------------------------------------------------------------------------------------------
# CodeBreaker's own decompiler: one function at a time, built in batches with their callees
# ---------------------------------------------------------------------------------------------
class _PerFunction(Prepared):
    def __init__(self, module_path, addrs, workdir, rev):
        from codebreaker.verify import body32 as B
        from codebreaker.verify import pipeline
        Prepared.__init__(self)
        self.module_path, self.workdir = module_path, workdir
        self.src, self.va_of, self.order = {}, {}, []
        for va in addrs:
            try:
                fn, c = B.synth(module_path, va, 4, fast=True)
            except Exception as e:
                fn, c = None, type(e).__name__
            if not fn:
                self.problem[va] = str(c or "")[:120] or "no function recovered"
            elif fn not in self.src:             # one body reached at two addresses: keep the first
                self.src[fn] = c
                self.va_of[fn] = va
                self.order.append(fn)
        # A callee that is not among these would link to a `return 0` stand-in, and the caller
        # would then be judged on behaviour that is not the reconstruction's. Pull in the real
        # bodies of such callees, transitively, as SUPPORT: compiled and linked, not judged here.
        self.imports = B.import_map(module_path)
        self.support = pipeline.support_bodies(module_path, self.src, self.imports, self.order)
        # A function's verdict depends on its own source AND on every body it (transitively)
        # calls, because those link to their real bodies. Sign all of it.
        calls = {fn: (B._stub_refs(self.src[fn], fn) & set(self.src)) for fn in self.order + self.support}

        def closure(fn):
            seen, work = set(), [fn]
            while work:
                f = work.pop()
                if f not in seen:
                    seen.add(f)
                    work += list(calls[f])
            return seen
        self.clo = {fn: closure(fn) for fn in self.order}
        linked = set(self.src) | set(self.imports)
        self.stubbed = {fn: any(B._stub_refs(self.src[g], g) - linked for g in self.clo[fn]) for fn in self.order}
        for fn in self.order:
            va = self.va_of[fn]
            self.source[va] = (fn, self.src[fn])
            self.sig[va] = _sha("native", rev, fn, self.src[fn],
                                *[self.src[g] for g in sorted(self.clo[fn] - {fn})])

    def build(self, addrs):
        from codebreaker.formats.pe import open_pe
        from codebreaker.verify import body32 as B
        out = Built()
        todo = [fn for fn in self.order if self.va_of[fn] in set(addrs)]
        if not todo:
            return out
        need = set()
        for fn in todo:
            need |= self.clo[fn]                 # the mates a body links against are compiled too
        src, imports, outdir = self.src, self.imports, self.workdir
        wanted = [(fn, src[fn]) for fn in self.order + self.support if fn in need]
        dll, exports, rejected = B.batch_build(wanted, outdir, imports=imports)
        subs = [(dll, exports)] if dll else B._batch_or_bisect(wanted, outdir, imports=imports)
        # Bisecting around a body that does not compile leaves the good ones split across several
        # DLLs, where a callee in another half is a stand-in again. Now that the bad bodies are
        # known, build every good one together.
        good = {fn for d, ex in subs if d for fn in ex}
        if sum(1 for d, _e in subs if d) > 1:
            dll, exports = B.batch_compile([(fn, src[fn]) for fn in self.order + self.support if fn in good],
                                           os.path.join(outdir, "all"), imports)
            if dll:
                subs = [(dll, exports)]
        for dll, exports in subs:
            if not dll:
                continue
            try:
                pb = open_pe(dll)
                base, at = pb.image_base, pb.export_addrs()
            except (OSError, ValueError):
                continue                         # built, then gone or unreadable: not a result
            members = set(exports)
            for fn in todo:
                if fn in members and at.get(fn) and self.va_of[fn] not in out.entry:
                    va = self.va_of[fn]
                    out.entry[va] = (dll, base + at[fn])
                    # a callee missing from this DLL is a stand-in: the match can show less
                    if self.stubbed[fn] or any(g not in members for g in self.clo[fn]):
                        out.weak[va] = {"stub-callee"}
        for fn in todo:
            if self.va_of[fn] not in out.entry:
                out.problem[self.va_of[fn]] = rejected.get(fn, "") or "did not compile"
        return out


class Native(object):
    name = "native"

    def available(self):
        from codebreaker.base import period_tools
        return bool(period_tools.decompiler("pe"))

    def rev(self):
        from codebreaker.base import period_tools
        from codebreaker.verify import pipeline
        return "%s.%s" % (pipeline.PIPE_REV, period_tools.decompiler_build_id())

    def prepare(self, module_path, addrs, workdir, log=None):
        return _PerFunction(module_path, addrs, workdir, self.rev())


_ENGINES = {e.name: e for e in (Core(), Native(), Export())}


def get(name):
    return _ENGINES[name]


def names():
    return list(ORDER)
