"""run.py -- read a module's functions back into the source its compiler was given.

    python -m codebreaker invert <16-bit module> [--first N] [--flags "/AM /G2 /Gsw /Ox"] [--show NAME]
    python -m codebreaker invert --early FILE OFFSET LENGTH     one function of a program built with the
                                                                C compiler of 1983-84 (offset in hex)
    python -m codebreaker invert --check [MODEL]                each model against its own compiler
    python -m codebreaker invert --tool NAME [arguments]        a model's own command line: %s

The decompiler gives source that BEHAVES like a function. A compiler model gives the source
that was WRITTEN, for the functions it covers: it knows which instructions the compiler emits
for which statement, so it reads the statements off the instructions instead of searching for
them (verify/authored/repair.py searches). What it says is then put to the compiler itself: the text is
compiled by the real Microsoft C 6 and its bytes compared with the original's, outside the
places a linker fills in.

    recognised    the model read the whole function
    identical     ... and the real compiler turns that text back into the function's bytes
    declined      the function has something the model does not cover; it says nothing

    assembly      a segment-switch thunk: generated code, written as the assembly that makes it
                  (invert/segment_thunk.py rebuilds its bytes exactly)

A model never guesses: a function it does not cover is declined, with the reason. Each function
is compiled by the compiler whose frame it has (compiler_of): Microsoft C 6, or Microsoft C/C++ 8
(settings: msvc152) for the modules Visual C++ 1.52c built.

    from codebreaker.invert import run
    run.module(path)      {name: (verdict, text or reason)}
"""
import os

FLAGS = ("/AM /G2 /Gsw /Ox", "/AM /G2 /Gsw /Os", "/AM /G2 /Gsw", "/AM /G2 /Gsw /Ot")
# Microsoft C/C++ 8 (Visual C++ 1.52c) built some of the 16-bit modules; it ships much unoptimised code
FLAGS_MSVC152 = ("/AM /G2 /Gsw", "/AM /G2 /Gsw /Od", "/AM /G2 /Gsw /Os", "/AM /G2 /Gsw /Ox")


def compiler_of(code):
    """Which compiler made a function, by its frame: Microsoft C/C++ 8 enters with mov ax,ds
    (8c d8) or the near-loadds frame and leaves through lea sp,[bp-2] (8d 66 fe 1f 5d); Microsoft
    C 6 does neither."""
    code = bytes(code)
    if code[:2] == b"\x8c\xd8" or code.startswith(bytes.fromhex("558bec1e8ed8")) or bytes.fromhex("8d66fe1f5d") in code:
        return "msvc152"
    return "msc6"

# the models, and what each one's own command line does
TOOLS = {
    "early-c": "codebreaker.invert.early_c",                  # FILE OFFSET LENGTH -> C
    "early-c-check": "codebreaker.invert.early_c_check",      # ... and put the text to Microsoft C 2.03
    "early-c-frame": "codebreaker.invert.early_c_frame",
    "early-c-round-trip": "codebreaker.invert.early_c_round_trip",   # compile -> read -> compile, for locals
    "early-c-frame-probe": "codebreaker.invert.early_c_frame_probe", # how the compiler places locals
    "early-c-order-probe": "codebreaker.invert.early_c_order_probe", # how it orders loads and stores      # the stack-frame model against its table
    "early-c-forward": "codebreaker.invert.early_c_forward",  # the forward model against a quad file and its object
    "quads": "codebreaker.invert.quad_dump",                  # list a quad file
    "quad-align": "codebreaker.invert.quad_align",            # a quad file beside its object, per function
    "omf": "codebreaker.formats.omf",                         # list an object file or library
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
CHECKS = ("early-c-frame", "early-c-round-trip", "msc6-function", "msc6-registers", "msc6-walk", "msc6", "msvc2-forward", "msvc2")


def tool(name, arguments):
    """Run one model's own command line."""
    from codebreaker.base import commands
    if name not in TOOLS:
        raise SystemExit("no model %r (have: %s)" % (name, ", ".join(sorted(TOOLS))))
    return commands.run_module(TOOLS[name], arguments)


_LONG = {"DWORD", "LONG", "ULONG", "long", "unsigned long", "LPARAM", "LRESULT", "COLORREF", "FARPROC"}


def _kind(c_type):
    """The model's word for an SDK type: a far pointer, a long, or an int."""
    t = c_type.replace("const ", "").strip()
    if t in _LONG:
        return "long"
    if t.startswith("LP") or "FAR" in t or "far" in t or t.endswith("*") or t in ("HPSTR", "PSTR"):
        return "far"
    return "int"


def signature(name):
    """(returns, [(name, kind)]) from the SDK headers for an exported function, or None."""
    from codebreaker.symbols import prototypes
    p = prototypes.of(name, bits=16)
    if not p:
        return None
    returns = p["returns"].strip()
    returns = "void" if returns == "void" else "long" if _kind(returns) in ("long", "far") else "int"
    return returns, [(n or "p%d" % i, _kind(t)) for i, (t, n) in enumerate(p["parameters"])
                     if t.strip() not in ("void", "")]


def _segment_signatures(module_path):
    """{(segment, offset): [(name, kind)]} for the framed functions of each code segment, from
    how they are called (msc6_calls.py)."""
    from codebreaker.formats.ne import NEFile
    from codebreaker.invert import msc6_calls
    nef = NEFile(module_path)
    out = {}
    for s in nef.segments():
        if not s.get("length"):
            continue
        try:
            sigs = msc6_calls.infer_segment_sigs(nef, s, nef.data[s["file_off"]:s["file_off"] + s["length"]])
        except Exception:
            continue
        for off, params in sigs.items():
            out[(s["idx"], off)] = params
    return out


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
    from codebreaker.invert import grammars, msc6 as model, segment_thunk, whole_function
    from codebreaker.verify.authored import match
    from codebreaker.verify.lanes import win16
    name = os.path.basename(module_path).lower()
    m = win16.prepare(module_path, settings.state("invert", name, "source"))
    where = {va: m.where[va] for va in m.by_va}
    originals = match.ARCH["x86-16"].originals(module_path, where, win16._spans(NEFile(module_path), where))
    out, texts = {}, {}
    called = _segment_signatures(module_path)
    for va, fn in sorted(m.by_va.items()):
        if va not in originals or (limit and len(out) >= limit):
            continue
        code = originals[va][0]
        thunk = segment_thunk.verify(code)
        if thunk is not None:
            out[fn] = ("assembly", segment_thunk.emit_asm(thunk, fn), None)
            continue
        tries = []
        try:
            tries += [c for _name, c in whole_function.candidates(code)]   # a function that is one known shape
        except Exception:
            pass
        sdk = signature(fn)
        returns, params = sdk if sdk else ("int", called.get(m.where[va]))
        for p in ([params, None] if params else [None]):     # the parameters known, then the model's own reading
            try:
                text, why = model.invert(code, syms=_globals(code), rtype=returns, params=p, func_base=m.where[va][1])
            except Exception as e:                           # a model that trips is a model that declines
                text, why = None, "the model failed: %s" % type(e).__name__
            if text is not None and text not in tries:
                tries.append(text)
        if not tries:
            tries = [t for g in grammars.GRAMMARS for t in g({"nargs": max(0, len(params or []))})] if \
                b"\x9a" in code else []                           # shapes the model does not read, by search
            if tries:
                texts[fn] = (va, tries)
                out[fn] = ("declined", str(why), None)
                continue
        if not tries:
            out[fn] = ("declined", str(why), None)
        else:
            out[fn] = ("recognised", tries[-1], None)
            texts[fn] = (va, tries)
    by_compiler = {}
    for fn, (va, tries) in texts.items():
        by_compiler.setdefault(compiler_of(originals[va][0]), {})[fn] = (va, tries)
    for which, group in sorted(by_compiler.items()):
        if not compiler.available(which):
            if log:
                log("  %d functions are %s's; it is not set up (settings.json: %s)" % (len(group), which, which))
            continue
        work = settings.state("invert", name, "work")
        left = dict(group)
        for fl in ([flags] if flags else FLAGS_MSVC152 if which == "msvc152" else FLAGS):
            built = compiler.compile_many({(fn, k): t for fn, (_va, tries) in left.items() for k, t in enumerate(tries)},
                                          "", fl, work, compiler=which)
            for fn, (va, tries) in list(left.items()):
                for k, text in enumerate(tries):
                    if built.get((fn, k)) and match.distance(built[(fn, k)], originals[va])[0] == "identical":
                        out[fn] = ("identical", text, fl)
                        del left[fn]
                        break
            if log:
                log("  %-8s %-20s %d of %d now identical" % (which, fl, len(group) - len(left), len(group)))
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
