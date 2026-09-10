# _template.py — how to add a tool to WinTranspiler.
#
# Copy this to plugins/<yourtool>.py (no leading underscore, or the loader
# skips it). Two requirements:
#   1. a module-level DESCRIPTION string (shown in `WinTranspiler list`)
#   2. runnable as __main__ reading its own sys.argv (like every other tool)
# Then it appears as `WinTranspiler <yourtool> [args]` immediately, and the next
# `build_exe.py` bakes it into WinTranspiler.exe. Nothing else to wire.
DESCRIPTION = "one-line help shown in `WinTranspiler list`"

import sys


def main():
    args = sys.argv[1:]
    print("your tool ran with:", args)


if __name__ == "__main__":
    main()
