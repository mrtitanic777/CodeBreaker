"""run.py -- exact: a compiled program rebuilt byte for byte from its source (docs/EXACT.md).

    python -m codebreaker exact PROJECT STAGE [options]
    python -m codebreaker exact --toolchains             the toolchain profiles there are
    python -m codebreaker exact --compile PROFILE[:VARIANT] FILE.c [switches]
                                                          one file compiled, its functions listed

PROJECT is a folder with a project.json (codebreaker/exact/project.py). Stages:

    ledger      every one-function source compiled and put against the program (exact/ledger.py)
    carve       where the program's own functions begin and end (exact/carve.py)
    modules     each source file put together, compiled whole, its functions and data checked (exact/modules.py)
    draft       functions written by the compiler's inverse model, kept when the compiler proves them (exact/draft.py)
    data        a module's initialized data: where it stands, definitions drafted (exact/data.py)
    bss         the variables without a value at their places: src/BSS (exact/bss.py)
    link        the program linked from its sources by the period linker, compared, diagnosed (exact/link.py)
    library     the library modules found in the program, in the linker's order; stand-ins (exact/library.py)
"""
import sys

STAGES = {"ledger": "codebreaker.exact.ledger", "library": "codebreaker.exact.library", "carve": "codebreaker.exact.carve",
          "modules": "codebreaker.exact.modules", "link": "codebreaker.exact.link",
          "bss": "codebreaker.exact.bss",
          "data": "codebreaker.exact.data",
          "draft": "codebreaker.exact.draft",
          "repair": "codebreaker.exact.repair",
          "names": "codebreaker.exact.names",
          "readable": "codebreaker.exact.readable",
          "seed": "codebreaker.exact.seed",
          "identify": "codebreaker.exact.identify"}


def main(argv):
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__)
        return 0 if argv else 2
    if argv[0] == "--toolchains":
        from codebreaker.exact import toolchain
        for n in toolchain.profiles():
            p = toolchain.load(n).p
            print("%-12s %s  variants: %s" % (n, p.get("title", ""), ", ".join(sorted(p.get("variants", {}))) or "-"))
        return 0
    if argv[0] == "--compile":
        from codebreaker.exact import toolchain, omf
        name, _, variant = argv[1].partition(":")
        tc = toolchain.load(name, variant or None)
        import os
        d = os.path.dirname(os.path.abspath(argv[2]))         # (the headers beside the file)
        hs = {f: open(os.path.join(d, f), "rb").read() for f in os.listdir(d) if f.lower().endswith(".h")}
        obj, con = tc.compile(open(argv[2], "rb").read(), headers=hs, switches=" ".join(argv[3:]) or None)
        if obj is None:
            print(con)
            return 1
        o = omf.read(obj)
        for text in omf.code_segments(o):
            for fn, off, ln in omf.functions(o, text):
                print("%-10s %-24s %5x %5d  %s" % (text, fn, off, ln, o.seg[text][off:off + ln].hex()))
        return 0
    from codebreaker.exact import project
    import importlib
    prj = project.load(argv[0])
    stage = argv[1] if len(argv) > 1 else "ledger"
    if stage not in STAGES:
        print("no stage %r (stages: %s)" % (stage, ", ".join(STAGES)))
        return 2
    return importlib.import_module(STAGES[stage]).main(prj, argv[2:])


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
