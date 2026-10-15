"""run.py -- exact: a compiled program rebuilt byte for byte from its source (docs/EXACT.md).

    python -m codebreaker exact PROJECT STAGE [options]
    python -m codebreaker exact --toolchains             the toolchain profiles there are
    python -m codebreaker exact --compile PROFILE[:VARIANT] FILE.c [switches]
                                                          one file compiled, its functions listed

PROJECT is a folder with a project.json (codebreaker/exact/project.py). Stages:

    ledger      every one-function source compiled and put against the program (exact/ledger.py)
"""
import sys

STAGES = {"ledger": "codebreaker.exact.ledger"}


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
        obj, con = tc.compile(open(argv[2], "rb").read(), switches=" ".join(argv[3:]) or None)
        if obj is None:
            print(con)
            return 1
        o = omf.read(obj)
        for fn, off, ln in omf.functions(o):
            print("%-24s %5x %5d  %s" % (fn, off, ln, o.seg["_TEXT"][off:off + ln].hex()))
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
