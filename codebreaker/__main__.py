"""python -m codebreaker <command> [arguments]

    module <name> [--no-verify]     reconstruct one module: decompile, build, judge, write its source
    scoreboard                      rewrite the scoreboard from stored results
    sweep <pe|ne> [options]         judge every function of every module (resumable)
    measure <name> [core|export]    judge one decompiler on one module; writes nothing to the tree

Every command is listed here, by name. Nothing is discovered by scanning folders.
"""
import importlib
import sys

COMMANDS = {
    "module": ("codebreaker.source.module_writer", []),
    "scoreboard": ("codebreaker.source.module_writer", ["--scoreboard"]),
    "sweep": ("codebreaker.verify.bank", []),
    "measure": ("codebreaker.verify.measure", []),
}


def main(argv):
    if not argv or argv[0] in ("-h", "--help", "help") or argv[0] not in COMMANDS:
        print(__doc__)
        return 0 if argv and argv[0] in ("-h", "--help", "help") else 2
    module, fixed = COMMANDS[argv[0]]
    return importlib.import_module(module).main(fixed + argv[1:])


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
