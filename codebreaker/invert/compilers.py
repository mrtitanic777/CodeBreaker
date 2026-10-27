"""compilers.py -- the compilers a model is checked against, as the models call them.

A model of a code generator is only as good as its agreement with the compiler itself. Each
model has a battery of small functions; this compiles them with the real compiler and hands
back its bytes. The compilers are build/period.py's -- this module only gives them the shapes
the models were written against.

    compilers.available()                 Microsoft C 6 is set up (settings: dosbox_x, msc6)
    compilers.mc6_compile(text, flags)    the code bytes Microsoft C 6 gives for one source text
    compilers.mc6_compile_batch(jobs)     the same for [(text, flags)], one emulator session per flag set
    compilers.msvc2()                     the 32-bit compiler of 1994: .compile_c(text, flags) and .Obj
    compilers.mc_compile(text)            Microsoft C 2.03, MC1 then MC2 (settings: msc203 -- the folder
                                          holding them; dosbox_x): the object file's bytes, or None
"""
import os
import subprocess


def available():
    from codebreaker.build import msc6
    return msc6.available()


def mc6_compile_batch(jobs, timeout=900):
    """[(text, flags)] -> [code bytes or None], in order."""
    from codebreaker.build import msc6
    if not msc6.available():
        raise RuntimeError("Microsoft C 6 is not set up (settings.json: dosbox_x, msc6)")
    out = [None] * len(jobs)
    by_flags = {}
    for i, (text, flags) in enumerate(jobs):
        by_flags.setdefault(flags, {})[i] = text
    for flags, sources in by_flags.items():
        for i, built in msc6.compile_many(sources, "", flags, timeout=timeout).items():
            out[i] = bytes(built[0]) if built else None
    return out


def mc6_compile(csrc, flags="/AM /G2 /Gsw /Ox", timeout=300):
    code = mc6_compile_batch([(csrc, flags)], timeout)[0]
    if code is None:
        raise RuntimeError("Microsoft C 6 gave no object (flags=%r)" % flags)
    return code


class _Obj(object):
    def __init__(self, data):
        self.data = data

    def functions(self):
        """{symbol: (bytes, [positions the linker fills in])}"""
        from codebreaker.verify import match
        return {name: (code, [i for i, m in enumerate(mask) if m])
                for name, (code, mask) in match.object_functions(self.data).items()}


class _Msvc2(object):
    Obj = _Obj

    def __init__(self, cl):
        from codebreaker.base import settings
        from codebreaker.verify import match
        self.cl, self.env, self.work = cl, match.compiler_env(cl), settings.state("invert", "msvc2")

    def compile_c(self, text, flags="/c /O1 /Oy-"):
        """{"obj": the object file's bytes} or {"err": what the compiler said}"""
        os.makedirs(self.work, exist_ok=True)
        obj = os.path.join(self.work, "one.obj")
        if os.path.isfile(obj):
            os.remove(obj)
        with open(os.path.join(self.work, "one.c"), "w", newline="\n", encoding="latin1", errors="replace") as f:
            f.write(text + "\n")
        r = subprocess.run([self.cl, "/nologo", "/W0"] + flags.split() + ["one.c"], cwd=self.work, env=self.env,
                           capture_output=True, text=True, errors="replace", timeout=300)
        if r.returncode or not os.path.isfile(obj):
            return {"err": (r.stdout + r.stderr).strip()}
        with open(obj, "rb") as f:
            return {"obj": f.read()}


def msvc2():
    from codebreaker.verify import match
    cl = match.compiler()
    if not cl:
        raise RuntimeError("the 1994 32-bit compiler is not set up (settings.json: period_compiler)")
    return _Msvc2(cl)


def mc_compile(csrc, stem="cand", timeout=120):
    """Compile one source text through MC1 and MC2 (build/period.py). -> the .OBJ's bytes, or None"""
    from codebreaker.build import period
    try:
        compiler = period.get("msc203")
    except ValueError:
        raise RuntimeError("Microsoft C 2.03 is not set up (settings.json: msc203, dosbox_x)")
    return compiler.object(csrc, stem, timeout)


def mc_compile_multi(csrc):
    """Compile a battery of probe functions in one run. -> the .OBJ's bytes, or None"""
    return mc_compile(csrc, "probe", 180)
