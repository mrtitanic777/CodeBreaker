"""commands.py -- run a module's own command line from inside a command.

Some packages are a set of tools, each with its own command line (the compiler models, the
driver-kit source chain). A command names them in a table and hands the rest of its arguments
to the one asked for:

    from codebreaker.base import commands
    commands.dispatch(TOOLS, argv, __doc__)      argv = [tool name, its arguments...]
"""
import runpy
import sys


def run_module(module, arguments):
    """Run `module` as if started with `python -m module arguments...`. -> its exit code."""
    saved = sys.argv
    sys.argv = [module] + list(arguments)
    try:
        runpy.run_module(module, run_name="__main__")
    except SystemExit as e:
        return e.code if isinstance(e.code, int) else (0 if e.code is None else 1)
    finally:
        sys.argv = saved
    return 0


def dispatch(tools, argv, doc=""):
    """argv[0] names one of `tools` {name: module}; the rest are its arguments."""
    if not argv or argv[0] not in tools:
        print(doc)
        print("tools: " + ", ".join(sorted(tools)))
        return 2
    return run_module(tools[argv[0]], argv[1:])
