"""run.py -- read a module's functions back into the source its compiler was given.

    python -m codebreaker invert <16-bit module> [--first N] [--flags "/AM /G2 /Gsw /Ox"] [--show NAME]
    python -m codebreaker invert --early FILE OFFSET LENGTH     one function of a program built with the
                                                                C compiler of 1983-84 (offset in hex)
    python -m codebreaker invert --check [MODEL]                each model against its own compiler
    python -m codebreaker invert --tool NAME [arguments]        a model's own command line: %s

The decompiler gives source that BEHAVES like a function. A compiler model gives the source
that was WRITTEN, for the functions it covers: it knows which instructions the compiler emits
for which statement, so it reads the statements off the instructions instead of searching for
them (verify/repair.py searches). What it says is then put to the compiler itself: the text is
compiled by the real Microsoft C 6 and its bytes compared with the original's, outside the
places a linker fills in.

    recognised    the model read the whole function
    identical     ... and the real compiler turns that text back into the function's bytes
    declined      the function has something the model does not cover; it says nothing

A model never guesses: a function it does not cover is declined, with the reason.

    from codebreaker.invert import run
    run.module(path)      {name: (verdict, text or reason)}
"""
import os

FLAGS = ("/AM /G2 /Gsw /Ox", "/AM /G2 /Gsw /Os", "/AM /G2 /Gsw", "/AM /G2 /Gsw /Ot")

# the models, and what each one's own command line does
TOOLS = {
    "early-c": "codebreaker.invert.early_c",                  # FILE OFFSET LENGTH -> C
    "early-c-frame": "codebreaker.invert.early_c_frame",      # the stack-frame model against its table
    "early-c-forward": "codebreaker.invert.early_c_forward",  # the forward model against a quad file and its object
    "quads": "codebreaker.invert.quad_dump",                  # list a quad file
    "quad-align": "codebreaker.invert.quad_align",            # a quad file beside its object, per function
    "omf": "codebreaker.invert.omf",                          # list an object file or library
    "library-match": "codebreaker.invert.library_match",      # which functions of a program are a library's
    "msc6": "codebreaker.invert.msc6",
    "msc6-templates": "codebreaker.invert.msc6_templates",
    "msc6-function": "codebreaker.invert.msc6_function",
    "msc6-registers": "codebreaker.invert.msc6_registers",
    "msc6-walk": "codebreaker.invert.msc6_walk",
    "msc6-idioms": "codebreaker.invert.msc6_idioms",          # what each compiler switch changes
    "known-differences": "codebreaker.invert.known_differences",
    "msvc2": "codebreaker.invert.msvc2",
    "msvc2-forward": "codebreaker.invert.msvc2_forward",
}
CHECKS = ("early-c-frame", "msc6-function", "msc6-registers", "msc6-walk", "msc6", "msvc2-forward", "msvc2")


def tool(name, arguments):
    """Run one model's own command line."""
    import runpy
    import sys
    if name not in TOOLS:
        raise SystemExit("no model %r (have: %s)" % (name, ", ".join(sorted(TOOLS))))
    saved = sys.argv
    sys.argv = [TOOLS[name]] + list(arguments)
    try:
        runpy.run_module(TOOLS[name], run_name="__main__")
    except SystemExit as e:
        return int(e.code or 0) if not isinstance(e.code, str) else 1
    finally:
        sys.argv = saved
    return 0


def _globals(code):
    """{offset: (name, "long" | "word")} for the variables a function names by address: a long
    shows as two words two apart used as one, anything else it stores to or pushes is a word.
    The names say what they are -- an address -- and nothing more."""
    def u16(i):
        return code[i] | (code[i + 1] << 8)
    out, n = {}, len(code)
    for i in range(max(0, n - 6)):
        if code[i] == 0xA1 and code[i + 3] == 0x0B and code[i + 4] == 0x06 and u16(i + 1) == u16(i + 5) + 2:
            out[u16(i + 5)] = ("gL_%x" % u16(i + 5), "long")
        if code[i:i + 2] == b"\x2b\xc0" and code[i + 2] == 0xA3 and code[i + 5] == 0xA3 and i + 8 <= n:
            out.setdefault(u16(i + 6), ("gL_%x" % u16(i + 6), "long"))
    for i in range(max(0, n - 3)):
        if code[i:i + 2] in (b"\xc7\x06", b"\xff\x36"):
            off = u16(i + 2)
            if off not in out and (off - 2) not in out:
                out[off] = ("gW_%x" % off, "word")
    return out


def module(module_path, limit=0, flags=None, log=None):
    """-> {function name: (verdict, C text or the reason it was declined, flags that gave the bytes)}"""
    from codebreaker.base import settings
    from codebreaker.build import msc6 as compiler
    from codebreaker.formats.ne import NEFile
    from codebreaker.invert import msc6 as model, whole_function
    from codebreaker.verify import match, match16, win16
    name = os.path.basename(module_path).lower()
    m = win16.prepare(module_path, settings.state("invert", name, "source"))
    where = {va: m.where[va] for va in m.by_va}
    originals = match16.original_functions(module_path, where, win16._spans(NEFile(module_path), where))
    out, texts = {}, {}
    for va, fn in sorted(m.by_va.items()):
        if va not in originals or (limit and len(out) >= limit):
            continue
        code = originals[va][0]
        tries = []
        try:
            tries += [c for _name, c in whole_function.candidates(code)]   # a function that is one known shape
        except Exception:
            pass
        try:
            text, why = model.invert(code, syms=_globals(code), func_base=m.where[va][1])
        except Exception as e:                               # a model that trips is a model that declines
            text, why = None, "the model failed: %s" % type(e).__name__
        if text is not None:
            tries.append(text)
        if not tries:
            out[fn] = ("declined", str(why), None)
        else:
            out[fn] = ("recognised", tries[-1], None)
            texts[fn] = (va, tries)
    if texts and compiler.available():
        work = settings.state("invert", name, "work")
        left = dict(texts)
        for fl in ([flags] if flags else FLAGS):
            built = compiler.compile_many({(fn, k): t for fn, (_va, tries) in left.items() for k, t in enumerate(tries)},
                                          "", fl, work)
            for fn, (va, tries) in list(left.items()):
                for k, text in enumerate(tries):
                    if built.get((fn, k)) and match.distance(built[(fn, k)], originals[va])[0] == "identical":
                        out[fn] = ("identical", text, fl)
                        del left[fn]
                        break
            if log:
                log("  %-20s %d of %d recognised functions are now identical" % (fl, len(texts) - len(left), len(texts)))
            if not left:
                break
    return out


def main(argv):
    from codebreaker.base import corpus
    flags = argv[argv.index("--flags") + 1] if "--flags" in argv else None
    limit = int(argv[argv.index("--first") + 1]) if "--first" in argv else 0
    show = argv[argv.index("--show") + 1] if "--show" in argv else None
    if "--tool" in argv:
        at = argv.index("--tool")
        return tool(argv[at + 1] if at + 1 < len(argv) else "", argv[at + 2:])
    if "--early" in argv:
        return tool("early-c", argv[argv.index("--early") + 1:])
    if "--check" in argv:
        named = [a for a in argv if not a.startswith("--")]
        worst = 0
        for name in (named or CHECKS):
            print("== %s" % name)
            worst = max(worst, tool(name, []))
        return worst
    args = [a for a in argv if not a.startswith("--") and a not in (flags, str(limit), show)]
    if not args:
        print(__doc__ % ", ".join(sorted(TOOLS)))
        return 2
    path = args[0] if os.path.isfile(args[0]) else corpus.retail(args[0])
    results = module(path, limit, flags, log=print)
    if show:
        v = results.get(show)
        print(v[1] if v else "no function %r" % show)
        return 0
    counts, reasons = {}, {}
    for verdict, text, _fl in results.values():
        counts[verdict] = counts.get(verdict, 0) + 1
        if verdict == "declined":
            why = text.split(" @")[0][:60]
            reasons[why] = reasons.get(why, 0) + 1
    print("%s: %d functions: %s" % (os.path.basename(path), len(results), ", ".join(
        "%s %d" % (k, counts[k]) for k in ("identical", "recognised", "declined") if counts.get(k))))
    for why, n in sorted(reasons.items(), key=lambda kv: -kv[1])[:8]:
        print("  declined %4d  %s" % (n, why))
    return 0
