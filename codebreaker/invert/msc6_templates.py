r"""msc6_templates.py -- the leaf templates of Microsoft C 6.00a (medium model, optimized): T0 the
loadds far frame, T1 return-constant, T2 return-param, T3 return-global, T4 store-constant, T5 one far
call, T6 an if-branch of leaves; anything else declined.  C++ (native/invert/msc6_small.cc;
docs/NATIVE.md); this keeps the API and the command line.

Usage:  python -m codebreaker invert --tool msc6-templates <hexbytes> [--verify]
"""
import sys
from codebreaker.invert import native_invert


def invert(code):
    """(C text, meta) for a modeled template, or (None, reason)."""
    return native_invert.small(2, code)


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    if not args:
        print(__doc__); sys.exit(2)
    code = bytes.fromhex(args[0])
    src, meta = invert(code)
    if src is None:
        print("DECLINED: %s" % meta); sys.exit(1)
    print(src)
    if "--verify" in sys.argv:
        from codebreaker.invert import compilers as ora
        for flags in ("/AM /G2 /Gsw /Ox", "/AM /G2 /Gsw /Os"):
            rc = ora.mc6_compile(src, flags)               # compile the emitted source as-is
            if rc == code:
                print("// VERIFIED byte-exact (%s): recompiles to the identical %d bytes" % (flags, len(code)))
                return
        print("// NOT byte-exact under the tried flags -- template incomplete for this function")


if __name__ == "__main__":
    main()
