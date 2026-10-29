"""dos_build.py -- `dos-build`: a real-mode program made from the folder dos-source wrote (dos_writer.rebuild)."""
from codebreaker.source.dos_writer import rebuild


def main(argv):
    """python -m codebreaker dos-build <source folder> [--against ORIGINAL]

    Make a real-mode program from the folder dos-source wrote, without the original."""
    args = [a for a in argv if not a.startswith("--")]
    against = argv[argv.index("--against") + 1] if "--against" in argv else None
    args = [a for a in args if a != against]
    if not args:
        print(main.__doc__)
        return 2
    made = rebuild(args[0])
    if against:
        same = open(made, "rb").read() == open(against, "rb").read()
        print("%s: %s" % (made, "THE SAME as %s" % against if same else "differs from %s" % against))
        return 0 if same else 1
    print(made)
    return 0
